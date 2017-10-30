#import "XADAppleDouble.h"
#import "XADException.h"

// AppleDouble format referenced from:
// http://www.opensource.apple.com/source/Libc/Libc-391.2.3/darwin/copyfile.c

@implementation XADAppleDouble

+(BOOL)parseAppleDoubleWithHandle:(CSHandle *)fh resourceForkOffset:(off_t *)resourceoffsetptr
resourceForkLength:(off_t *)resourcelengthptr extendedAttributes:(NSDictionary<NSString*,NSData*> **)extattrsptr error:(NSError **)error
{
    NSError *tmpErr = nil;
    BOOL success = YES;
    if([fh readUInt32BEWithError:&tmpErr]!=0x00051607) {
        if (tmpErr) {
            if (error) {
                *error = tmpErr;
            }
        } else if (error) {
            *error = [NSError errorWithDomain:XADErrorDomain code:XADErrorIllegalData userInfo:nil];
        }
        return NO;
    }
    if([fh readUInt32BEWithError:error]!=0x00020000) {
        if (tmpErr) {
            if (error) {
                *error = tmpErr;
            }
        } else if (error) {
            *error = [NSError errorWithDomain:XADErrorDomain code:XADErrorIllegalData userInfo:nil];
        }
        return NO;
    }

	success = [fh skipBytes:16 error:error];
    if (!success) {
        return NO;
    }

	int num=[fh readUInt16BE];

	uint32_t rsrcoffs=0,rsrclen=0;
	uint32_t finderoffs=0,finderlen=0;

	for(int i=0;i<num;i++)
	{
		uint32_t entryid=[fh readUInt32BE];
		uint32_t entryoffs=[fh readUInt32BE];
		uint32_t entrylen=[fh readUInt32BE];

		switch(entryid)
		{
			case 2: // Resource fork
				rsrcoffs=entryoffs;
				rsrclen=entrylen;
			break;
			case 9: // Finder info
				finderoffs=entryoffs;
				finderlen=entrylen;
			break;
		}
	}

	if(!rsrcoffs&&!finderoffs) return NO;

	// Load FinderInfo struct and extended attributes if available.
	NSData *finderinfo=nil;
	NSMutableDictionary *extattrs=nil;
 	if(finderoffs)
	{
		// First 32 bytes are the FinderInfo struct.
		success = [fh seekToFileOffset:finderoffs error:error];
        if (!success) {
            return NO;
        }
		if(finderlen>32) finderinfo=[fh readDataOfLength:32 error:&tmpErr];
		else finderinfo=[fh readDataOfLength:finderlen error:&tmpErr];

		// Add FinderInfo to extended attributes only if it is not empty.
		static const uint8_t zerobytes[32]={0x00};
		if(memcmp([finderinfo bytes],zerobytes,[finderinfo length])!=0)
		{
			extattrs=[NSMutableDictionary dictionaryWithObject:finderinfo
			forKey:@"com.apple.FinderInfo"];
		}

		// The FinderInfo struct is optionally followed by the extended attributes.
		if(finderlen>70)
		{
			if(!extattrs) extattrs=[NSMutableDictionary dictionary];
			[self parseAppleDoubleExtendedAttributesWithHandle:fh intoDictionary:extattrs error:&tmpErr];
		}
	}

	if(resourceoffsetptr) *resourceoffsetptr=rsrcoffs;
	if(resourcelengthptr) *resourcelengthptr=rsrclen;
	if(extattrsptr) *extattrsptr=extattrs;

	return YES;
}

+(BOOL)parseAppleDoubleExtendedAttributesWithHandle:(CSHandle *)fh intoDictionary:(NSMutableDictionary *)extattrs error:(NSError **)error
{
	[fh skipBytes:2 error:error];
	uint32_t magic=[fh readUInt32BEWithError:error];

    if(magic!=0x41545452) {
        if (error) {
            *error = [NSError errorWithDomain:XADErrorDomain code:XADErrorIllegalData userInfo:nil];
        }
        return NO;
    }

	/*uint32_t debug=*/[fh readUInt32BEWithError:error];
	/*uint32_t totalsize=*/[fh readUInt32BEWithError:error];
	/*uint32_t datastart=*/[fh readUInt32BEWithError:error];
	/*uint32_t datalength=*/[fh readUInt32BEWithError:error];
	[fh skipBytes:12 error:error];
	/*int flags=*/[fh readUInt16BEWithError:error];
	int numattrs=[fh readUInt16BEWithError:error];

	struct
	{
		int offset,length,namelen;
		uint8_t namebytes[256];
	} entries[numattrs];

	for(int i=0;i<numattrs;i++)
	{
		entries[i].offset=[fh readUInt32BEWithError:error];
		entries[i].length=[fh readUInt32BEWithError:error];
		/*int flags=*/[fh readUInt16BEWithError:error];
		entries[i].namelen=[fh readUInt8WithError:error];
		[fh readBytes:entries[i].namelen toBuffer:entries[i].namebytes error:error];

		int padbytes=(-(entries[i].namelen+11))&3;
		[fh skipBytes:padbytes error:error]; // Align to 4 bytes.
	}

	for(int i=0;i<numattrs;i++)
	{
		off_t curroffset=[fh offsetInFile];

		// Find the entry that comes next in the file to avoid seeks.
		int minoffset=INT_MAX;
		int minindex=-1;
		for(int j=0;j<numattrs;j++)
		{
			if(entries[j].offset>=curroffset && entries[j].offset<minoffset)
			{
				minoffset=entries[j].offset;
				minindex=j;
			}
		}
		if(minindex<0) break; // File structure was messed up, so give up.

		if(minoffset!=curroffset) [fh seekToFileOffset:minoffset error:error];
		NSData *data=[fh readDataOfLength:entries[minindex].length error:error];

		NSString *name=[[[NSString alloc] initWithBytes:entries[minindex].namebytes
		length:entries[minindex].namelen-1 encoding:NSUTF8StringEncoding] autorelease];

		extattrs[name] = data;
	}
    return YES;
}



+(BOOL)writeAppleDoubleHeaderToHandle:(CSHandle *)fh resourceForkSize:(int)ressize
extendedAttributes:(NSDictionary *)extattrs error:(NSError **)error
{
	// AppleDouble header template.
	uint8_t header[0x32]=
	{
		/*  0 */ 0x00,0x05,0x16,0x07, 0x00,0x02,0x00,0x00,
		/*  8 */ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		/* 24 */ 0x00,0x02,
		/* 26 */ 0x00,0x00,0x00,0x09, 0x00,0x00,0x00,0x32, 0x00,0x00,0x00,0x00,
		/* 38 */ 0x00,0x00,0x00,0x02, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
		/* 50 */
	};

	// Calculate FinderInfo and extended attributes size field.
	NSMutableDictionary *encodedkeys=[NSMutableDictionary dictionary];
	int numattributes=0,attributeentrysize=0,attributedatasize=0;

	// Sort keys and iterate over them.
	NSArray *keys=[[extattrs allKeys] sortedArrayUsingSelector:@selector(compare:)];
	NSEnumerator *enumerator=[keys objectEnumerator];
	NSString *key;
	while((key=[enumerator nextObject]))
	{
		// Ignore FinderInfo.
		if([key isEqual:@"com.apple.FinderInfo"]) continue;

 		NSData *data=extattrs[key];
		NSData *keydata=[key dataUsingEncoding:NSUTF8StringEncoding];
		int namelen=(int)[keydata length]+1;
		if(namelen>128) continue; // Skip entries with too long names.

		numattributes++;
		attributeentrysize+=(11+namelen+3)&~3; // Aligned to 4 bytes.
		attributedatasize+=[data length];

		encodedkeys[key] = keydata;
	}

	// Set FinderInfo size field and resource fork offset field.
	if(numattributes)
	{
		CSSetUInt32BE(&header[34],32+38+attributeentrysize+attributedatasize);
		CSSetUInt32BE(&header[42],50+32+38+attributeentrysize+attributedatasize);
	}
	else
	{
		CSSetUInt32BE(&header[34],32);
		CSSetUInt32BE(&header[42],50+32);
	}

	// Set resource fork size field.
	CSSetUInt32BE(&header[46],ressize);

	// Write AppleDouble header.
	[fh writeBytes:sizeof(header) fromBuffer:header];

	// Write FinderInfo structure.
	NSData *finderinfo=extattrs[@"com.apple.FinderInfo"];
	if(finderinfo)
	{
		if([finderinfo length]<32) [XADException raiseUnknownException];
		[fh writeBytes:32 fromBuffer:[finderinfo bytes]];
	}
	else
	{
		uint8_t emptyfinderinfo[32]={ 0x00 };
		[fh writeBytes:32 fromBuffer:emptyfinderinfo];
	}

	// Write extended attributes if needed.
	if(numattributes)
	{
		// Attributes section header template.
		uint8_t attributesheader[38]=
		{
			/*  0 */ 0x00,0x00,
			/*  2 */  'A', 'T', 'T', 'R', 0x00,0x00,0x00,0x00,
			/* 10 */ 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
			/* 18 */ 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
			/* 26 */ 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
			/* 34 */ 0x00,0x00, 0x00,0x00,
			/* 38 */
		};

		int datastart=50+32+38+attributeentrysize;

		// Set header fields.
		CSSetUInt32BE(&attributesheader[10],datastart+attributedatasize); // total_size
		CSSetUInt32BE(&attributesheader[14],datastart); // data_start
		CSSetUInt32BE(&attributesheader[18],attributedatasize); // data_length
		CSSetUInt16BE(&attributesheader[36],numattributes); // num_attrs

		// Write attributes section header.
		[fh writeBytes:sizeof(attributesheader) fromBuffer:attributesheader];

		// Write attribute entries.
		int currdataoffset=datastart;
		NSEnumerator *enumerator=[keys objectEnumerator];
		NSString *key;
		while((key=[enumerator nextObject]))
		{
			NSData *data=extattrs[key];
			NSData *keydata=encodedkeys[key];
			if(!keydata) continue;

			int namelen=(int)([keydata length]+1);

			// Attribute entry header template.
			uint8_t entryheader[11]=
			{
				/*  0 */ 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
				/*  8 */ 0x00,0x00, namelen,
				/* 11 */ 
			};

			// Set entry header fields.
			CSSetUInt32BE(&entryheader[0],currdataoffset); // offset
			CSSetUInt32BE(&entryheader[4],(uint32_t)[data length]); // length

			// Write entry header.
			[fh writeBytes:sizeof(entryheader) fromBuffer:entryheader];

			// Write name.
			char namebytes[namelen];
			[key getCString:namebytes maxLength:namelen encoding:NSUTF8StringEncoding];
			[fh writeBytes:namelen fromBuffer:namebytes];

			// Calculate and write padding.
			int padbytes=(-(namelen+11))&3;
			uint8_t zerobytes[4]={ 0x00 };
			[fh writeBytes:padbytes fromBuffer:zerobytes];

			// Update data pointer.
			currdataoffset+=[data length];
		}

		// Write attribute data.
		enumerator=[keys objectEnumerator];
		while((key=[enumerator nextObject]))
		{
			NSData *data=extattrs[key];
			NSData *keydata=encodedkeys[key];
			if(!keydata) continue;

			[fh writeData:data];
		}
	}
    return YES;
}

@end

