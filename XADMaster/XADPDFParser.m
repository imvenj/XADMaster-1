#import "XADPDFParser.h"
#import "CSMemoryHandle.h"
#import "CSMultiHandle.h"
#import "XADLZMAHandle.h"

static int SortPages(id first,id second,void *context);

static NSDictionary *TIFFShortEntry(int tag,int value);
static NSDictionary *TIFFLongEntry(int tag,int value);
static NSDictionary *TIFFLongEntryForImageStart(int tag);
static NSDictionary *TIFFShortArrayEntry(int tag,NSData *data);
static NSDictionary *TIFFUndefinedArrayEntry(int tag,NSData *data);
static NSData *CreateTIFFHeaderWithEntries(NSArray *entries);

static NSData *CreateNewJPEGHeaderWithColourProfile(NSData *fileheader,NSData *profile,int *skiplength);

@implementation XADPDFParser

+(int)requiredHeaderSize { return 5+48; }

+(BOOL)recognizeFileWithHandle:(CSHandle *)handle firstBytes:(NSData *)data name:(NSString *)name
{
	const uint8_t *bytes=data.bytes;
	NSInteger length=data.length;

	if(length<5+48) return NO;

	if(bytes[0]!='%') return NO;
	if(bytes[1]!='P') return NO;
	if(bytes[2]!='D') return NO;
	if(bytes[3]!='F') return NO;
	if(bytes[4]!='-') return NO;

	return YES;
}

-(id)init
{
	if(self=[super init])
	{
		parser=nil;
	}
	return self;
}

-(void)dealloc
{
	[parser release];
	[super dealloc];
}

-(void)parse
{
	parser=[[PDFParser parserWithHandle:self.handle] retain];
	[parser setPasswordRequestAction:@selector(needsPassword:) target:self];

	[parser parse];

	BOOL isencrypted=[parser needsPassword];

	// Find image objects in object list
	NSMutableArray *images=[NSMutableArray array];
	NSEnumerator *enumerator=[[parser objectDictionary] objectEnumerator];
	id object;
	while(object=[enumerator nextObject])
	{
		if([object isKindOfClass:[PDFStream class]]&&[object isImage])
		[images addObject:object];
	}

	// Traverse page tree to find which images are referenced from which pages
	NSMutableDictionary *order=[NSMutableDictionary dictionary];
	NSDictionary *root=[parser pagesRoot];
	NSMutableArray *stack=[NSMutableArray arrayWithObject:[[root arrayForKey:@"Kids"] objectEnumerator]];
	int page=0;
	while(stack.count)
	{
		id curr=[stack.lastObject nextObject];
		if(!curr) [stack removeLastObject];
		else
		{
			NSString *type=curr[@"Type"];
			if([type isEqual:@"Pages"])
			{
				[stack addObject:[[curr arrayForKey:@"Kids"] objectEnumerator]];
			}
			else if([type isEqual:@"Page"])
			{
				page++;
				NSDictionary *xobjects=curr[@"Resources"][@"XObject"];
				NSEnumerator *enumerator=[xobjects objectEnumerator];
				id object;
				while(object=[enumerator nextObject])
				{
					if([object isKindOfClass:[PDFStream class]]&&[object isImage])
					order[[object reference]] = @(page);
				}
			}
			else @throw @"Invalid PDF structure";
		}
	}

	// Sort images in page order.
	[images sortUsingFunction:(void *)SortPages context:order];

	// Output images.
	enumerator=[images objectEnumerator];
	PDFStream *image;
	while(image=[enumerator nextObject])
	{
		PDFObjectReference *ref=image.reference;
		NSNumber *page=order[ref];

		NSString *name;
		if(page) name=[NSString stringWithFormat:@"Page %@, object %d",page,ref.number];
		else name=[NSString stringWithFormat:@"Object %d",ref.number];

		NSString *imgname=image.dictionary[@"Name"];
		if(imgname) name=[NSString stringWithFormat:@"%@ (%@)",name,imgname];

		NSNumber *length=image.dictionary[@"Length"];
		NSArray *decode=[image imageDecodeArray];

		int width=image.imageWidth;
		int height=image.imageHeight;
		int bpc=image.imageBitsPerComponent;
		NSInteger components=image.numberOfImageComponents;

		NSData *colourprofile=[image imageICCColourProfile];
		NSInteger profilesize=0;
		if(colourprofile) profilesize=(colourprofile.length+1)&~1;

		NSMutableDictionary *dict=[NSMutableDictionary dictionaryWithObjectsAndKeys:
			//[self XADStringWithString:[parser isCompressed]?@"Zlib":@"None"],XADCompressionNameKey,
			length,XADCompressedSizeKey,
			image,@"PDFStream",
		nil];

		if(image.JPEGImage)
		{
//if([image imageType]==PDFCMYKImageType) [self reportInterestingFileWithReason:@"CMYK JPEG"];
//if([[image imageColourSpaceName] isEqual:@"ICCBased"]) [self reportInterestingFileWithReason:@"JPEG with ICC profile"];
			NSString *compname=[self compressionNameForStream:image excludingLast:YES];
			dict[XADCompressionNameKey] = [self XADStringWithString:compname];

			if(!image.hasMultipleFilters && !isencrypted && !colourprofile)
			dict[XADFileSizeKey] = length;

			dict[@"PDFStreamType"] = @"JPEG";

			if(colourprofile) dict[@"PDFJPEGColourProfile"] = colourprofile;

			name=[name stringByAppendingPathExtension:@"jpg"];
		}
		else if(image.JPEG2000Image)
		{
			NSString *compname=[self compressionNameForStream:image excludingLast:YES];
			dict[XADCompressionNameKey] = [self XADStringWithString:compname];

			if(!image.hasMultipleFilters && !isencrypted)
			dict[XADFileSizeKey] = length;

			dict[@"PDFStreamType"] = @"JPEG";

			name=[name stringByAppendingPathExtension:@"jp2"];
		}
		else
		{
			NSInteger bytesperrow=(width*bpc*components+7)/8;
			NSData *palettedata=nil;

			int type=image.imageType;
			switch(type)
			{
				case PDFIndexedImageType:
					if(image.imagePaletteType==PDFRGBImageType)
					{
						// Build TIFF palette data.

						NSInteger numpalettecolours=image.numberOfImagePaletteColours;
						NSData *pdfpalette=[image imagePaletteData];

						if(pdfpalette)
						{
							int numtiffcolours=1<<bpc;
							uint8_t bytes[3*2*numtiffcolours];
							uint8_t *ptr=bytes;

							const uint8_t *palettebytes=pdfpalette.bytes;

							for(int col=0;col<3;col++)
							for(int i=0;i<numtiffcolours;i++)
							{
								if(i<numpalettecolours)
								{
									CSSetUInt16LE(ptr,palettebytes[3*i+col]*0x101);
								}
								else
								{
									CSSetUInt16LE(ptr,0);
								}
								ptr+=2;
							}

							palettedata=[NSData dataWithBytes:bytes length:sizeof(bytes)];
						}
					}
					else
					{
						// Unpack other palette images if possible.
						if(bpc==8)
						{
							NSData *palettedata=[image imagePaletteData];

							if(palettedata)
							{
								NSInteger palettecomponents=image.numberOfImagePaletteComponents;

								dict[@"PDFTIFFExpandedPaletteData"] = palettedata;
								dict[@"PDFTIFFExpandedComponents"] = @(palettecomponents);

								// Override image parameters.
								type=image.imagePaletteType;
								components=palettecomponents;
								bytesperrow=palettecomponents*width;
							}
						}
						else goto giveup;
					}
				break;

				case PDFSeparationImageType:
				case PDFGrayImageType:
				case PDFRGBImageType:
				case PDFCMYKImageType:
				case PDFMaskImageType:
				case PDFLabImageType:
				break;

				default:
					goto giveup;
			}

			if([image.imageColourSpaceName isEqual:@"CalRGB"]) [self reportInterestingFileWithReason:@"CalRGB image"];
			if([image.imageColourSpaceName isEqual:@"CalGray"]) [self reportInterestingFileWithReason:@"CalGray image"];
			if([image.imageColourSpaceName isEqual:@"Lab"]) [self reportInterestingFileWithReason:@"Lab image"];

			NSMutableArray *entries=[NSMutableArray array];

			[entries addObject:TIFFShortEntry(256,width)];
			[entries addObject:TIFFShortEntry(257,height)];

			if(components==1)
			{
				[entries addObject:TIFFShortEntry(258,bpc)];
			}
			else
			{
				uint8_t bytes[components*2];
				for(int i=0;i<components;i++) CSSetUInt16LE(&bytes[i*2],bpc);
				NSData *data=[NSData dataWithBytes:bytes length:components*2];
				[entries addObject:TIFFShortArrayEntry(258,data)]; // BitsPerSample
			}

			[entries addObject:TIFFShortEntry(259,1)]; // Compression

			switch(type)
			{
				case PDFSeparationImageType:
					[entries addObject:TIFFShortEntry(262,0)]; // PhotoMetricInterpretation = WhiteIsZero
				break;

				case PDFGrayImageType:
					if(decode)
					{
						float zeropoint=[decode[0] floatValue];
						float onepoint=[decode[1] floatValue];
						if(zeropoint>onepoint) [entries addObject:TIFFShortEntry(262,0)]; // PhotoMetricInterpretation = WhiteIsZero
						else [entries addObject:TIFFShortEntry(262,1)]; // PhotoMetricInterpretation = BlackIsZero
					}
					else
					{
						[entries addObject:TIFFShortEntry(262,1)]; // PhotoMetricInterpretation = BlackIsZero
					}
				break;

				case PDFRGBImageType:
					[entries addObject:TIFFShortEntry(262,2)]; // PhotoMetricInterpretation = RGB
				break;

				case PDFIndexedImageType:
					[entries addObject:TIFFShortEntry(262,3)]; // PhotoMetricInterpretation = Palette
				break;

				case PDFMaskImageType:
					//[entries addObject:TIFFShortEntry(262,4)]; // PhotoMetricInterpretation = Mask
					// Apparently no program knows what to do with TIFF masks, so use BlackIsZero instead.
					[entries addObject:TIFFShortEntry(262,1)]; // PhotoMetricInterpretation = BlackIsZero
				break;

				case PDFCMYKImageType:
					[entries addObject:TIFFShortEntry(262,5)]; // PhotoMetricInterpretation = Separated
				break;

				case PDFLabImageType:
					[entries addObject:TIFFShortEntry(262,8)]; // PhotoMetricInterpretation = CIELAB
				break;
			}

			[entries addObject:TIFFLongEntryForImageStart(273)]; // StripOffsets
			if(components>1) [entries addObject:TIFFShortEntry(277,(int)components)]; // SamplesPerPixel
			[entries addObject:TIFFLongEntry(278,height)]; // RowsPerStrip
			[entries addObject:TIFFLongEntry(279,(int)(bytesperrow*height))]; // StripByteCounts

			if(palettedata) [entries addObject:TIFFShortArrayEntry(320,palettedata)]; // Palette
			if(type==PDFCMYKImageType) [entries addObject:TIFFShortEntry(332,1)]; // InkSet = CMYK
			if(colourprofile) [entries addObject:TIFFUndefinedArrayEntry(0x8773,colourprofile)];

			NSData *headerdata=CreateTIFFHeaderWithEntries(entries);
			off_t headersize=headerdata.length;

			NSString *compname=[self compressionNameForStream:image excludingLast:NO];
			dict[XADCompressionNameKey] = [self XADStringWithString:compname];

			dict[XADFileSizeKey] = @(headersize+bytesperrow*height);
			dict[@"PDFTIFFDataLength"] = @(bytesperrow*height);
			dict[@"PDFTIFFHeader"] = headerdata;
			dict[@"PDFStreamType"] = @"TIFF";

			name=[name stringByAppendingPathExtension:@"tiff"];
		}

		giveup:
		dict[XADFileNameKey] = [self XADPathWithString:name];
		if(isencrypted) dict[XADIsEncryptedKey] = @YES;

		[self addEntryWithDictionary:dict];
	}
}

-(void)needsPassword:(PDFParser *)parserarg
{
	if(![parserarg setPassword:self.password]) [XADException raisePasswordException];
}

-(NSString *)compressionNameForStream:(PDFStream *)stream excludingLast:(BOOL)excludelast
{
	NSMutableString *string=[NSMutableString string];

	NSDictionary *dict=stream.dictionary;
	NSArray *filter=[dict arrayForKey:@"Filter"];

	if(filter)
	{
		NSInteger count=filter.count;
		if(excludelast) count--;

		for(NSInteger i=count-1;i>=0;i--)
		{
			NSString *name=filter[i];
			if([name hasSuffix:@"Decode"]) name=[name substringToIndex:name.length-6];
			if(i!=count-1) [string appendString:@"+"];
			[string appendString:name];
		}
	}

	if(!string.length) return @"None";
	return string;
}

-(CSHandle *)handleForEntryWithDictionary:(NSDictionary *)dict wantChecksum:(BOOL)checksum
{
	NSString *streamtype=dict[@"PDFStreamType"];
	PDFStream *stream=dict[@"PDFStream"];

	if([streamtype isEqual:@"JPEG"])
	{
		CSHandle *handle=[stream JPEGHandle];

		NSData *profile=dict[@"PDFJPEGColourProfile"];
		if(profile)
		{
			NSData *fileheader=[handle readDataOfLengthAtMost:256];
			[handle seekToFileOffset:0];

			int skiplength;
			NSData *newheader=CreateNewJPEGHeaderWithColourProfile(fileheader,profile,&skiplength);
			if(newheader)
			{
				return [CSMultiHandle multiHandleWithHandles:
				[CSMemoryHandle memoryHandleForReadingData:newheader],
				[handle nonCopiedSubHandleToEndOfFileFrom:skiplength],
				nil];
			}
			else
			{
				return handle;
			}
		}
		else
		{
			return handle;
		}
	}
	else if([streamtype isEqual:@"TIFF"])
	{
		CSHandle *handle=[stream handle];
		if(!handle) return nil;

		NSNumber *length=dict[@"PDFTIFFDataLength"];
		if(length) handle=[handle nonCopiedSubHandleOfLength:length.longLongValue];

		NSData *header=dict[@"PDFTIFFHeader"];
		if(!header) return nil;

		NSData *palette=dict[@"PDFTIFFExpandedPaletteData"];
		if(palette)
		{
			int components=[dict[@"PDFTIFFExpandedComponents"] intValue];

			handle=[[[XAD8BitPaletteExpansionHandle alloc] initWithHandle:handle
			length:stream.imageWidth*stream.imageHeight*components
			numberOfChannels:components palette:palette] autorelease];
		}

		return [CSMultiHandle multiHandleWithHandles:
		[CSMemoryHandle memoryHandleForReadingData:header],handle,nil];
	}
	else
	{
		return nil;
	}
}

-(NSString *)formatName { return @"PDF"; }

@end




static int SortPages(id first,id second,void *context)
{
	NSDictionary *order=(NSDictionary *)context;
	NSNumber *firstpage=order[[first reference]];
	NSNumber *secondpage=order[[second reference]];
	if(!firstpage&&!secondpage) return 0;
	else if(!firstpage) return 1;
	else if(!secondpage) return -1;
	else return [firstpage compare:secondpage];
}

static NSDictionary *TIFFShortEntry(int tag,int value)
{
	return @{@"Tag": @(tag),
		@"Type": @3,
		@"Count": @1,
		@"Value": @(value)};
}


static NSDictionary *TIFFLongEntry(int tag,int value)
{
	return @{@"Tag": @(tag),
		@"Type": @4,
		@"Count": @1,
		@"Value": @(value)};
}

static NSDictionary *TIFFLongEntryForImageStart(int tag)
{
	return @{@"Tag": @(tag),
		@"Type": @4,
		@"Count": @1};
}

static NSDictionary *TIFFShortArrayEntry(int tag,NSData *data)
{
	return @{@"Tag": @(tag),
		@"Type": @3,
		@"Count": @(data.length/2),
		@"Data": data};
}

static NSDictionary *TIFFUndefinedArrayEntry(int tag,NSData *data)
{
	return @{@"Tag": @(tag),
		@"Type": @7,
		@"Count": @(data.length),
		@"Data": data};
}

static NSData *CreateTIFFHeaderWithEntries(NSArray *entries)
{
	CSMemoryHandle *header=[CSMemoryHandle memoryHandleForWriting];

	// Write TIFF header.
	[header writeUInt8:'I']; // Magic number for little-endian TIFF.
	[header writeUInt8:'I'];
	[header writeUInt16LE:42];
	[header writeUInt32LE:8]; // Offset of IFD.

	// Write IFD header.
	[header writeUInt16LE:entries.count]; // Number of IFD entries.

	uint32_t dataoffset=(int)(8+2+entries.count*12+4);
	uint32_t datasize=0;

	NSEnumerator *enumerator;
	NSDictionary *entry;

	// Calculate total data size.
	enumerator=[entries objectEnumerator];
	while((entry=[enumerator nextObject]))
	{
		NSData *data=entry[@"Data"];
		NSInteger length=data.length;
		datasize+=(length+1)&~1;
	}

	uint32_t imagestart=dataoffset+datasize;

	// Write IFD entries.
	enumerator=[entries objectEnumerator];
	while((entry=[enumerator nextObject]))
	{
		NSNumber *tag=entry[@"Tag"];
		NSNumber *type=entry[@"Type"];
		NSNumber *count=entry[@"Count"];

		[header writeUInt16LE:tag.intValue];
		[header writeUInt16LE:type.intValue];
		[header writeUInt32LE:count.unsignedIntValue];

		if(count.intValue==1)
		{
			NSNumber *value=entry[@"Value"];
			if(value) [header writeUInt32LE:value.unsignedIntValue];
			else [header writeUInt32LE:imagestart];
		}
		else
		{
			NSData *data=entry[@"Data"];
			[header writeUInt32LE:dataoffset];

			NSInteger length=data.length;
			dataoffset+=(length+1)&~1;
		}
	}

	// Write IFD footer.
	[header writeUInt32LE:0]; // Next IFD offset.

	// Write data segments.
	enumerator=[entries objectEnumerator];
	while((entry=[enumerator nextObject]))
	{
		NSData *data=entry[@"Data"];
		[header writeData:data];
		if(data.length&1) [header writeUInt8:0];
	}

	return header.data;
}

static NSData *CreateNewJPEGHeaderWithColourProfile(NSData *fileheader,NSData *profile,int *skiplength)
{
	NSInteger length=fileheader.length;
	const uint8_t *bytes=fileheader.bytes;

	if(length<4) return nil;
	if(bytes[0]!=0xff && bytes[1]!=0xd8) return nil;

	NSMutableData *newheader=[NSMutableData data];

	if(bytes[2]==0xff && bytes[3]==0xe0) // JFIF.
	{
		if(length<6) return nil;
		int jfiflength=CSUInt16BE(&bytes[4]);
		if(length<4+jfiflength) return nil;

		[newheader appendBytes:bytes length:4+jfiflength];
		*skiplength=4+jfiflength;
	}
	else if(bytes[2]==0xff && (bytes[3]>=0xda && bytes[3]<=0xfe)) // Some other JPEG chunk.
	{
		[newheader appendBytes:bytes length:2];
		*skiplength=2;
	}
	else
	{
		return nil;
	}

	NSInteger profilelength=profile.length;
	const uint8_t *profilebytes=profile.bytes;

	NSInteger numchunks=(profilelength+65518)/65519;

	for(NSInteger i=0;i<numchunks;i++)
	{
		NSInteger chunkbytes;
		if(i==numchunks-1) chunkbytes=profilelength-i*65519;
		else chunkbytes=65519;

		NSInteger chunksize=chunkbytes+16;

		[newheader appendBytes:(uint8_t [18]){
			0xff,0xe2,chunksize>>8,chunksize&0xff,
			'I','C','C','_','P','R','O','F','I','L','E',0,
			i+1,numchunks} length:18];

		[newheader appendBytes:&profilebytes[i*65519] length:chunkbytes];
	}

	return newheader;
}



@implementation XAD8BitPaletteExpansionHandle

-(id)initWithHandle:(CSHandle *)parent length:(off_t)length
numberOfChannels:(int)numberofchannels palette:(NSData *)palettedata
{
	if((self=[super initWithHandle:parent length:length]))
	{
		palette=[palettedata retain];
		numchannels=numberofchannels;
	}
	return self;
}

-(void)dealloc
{
	[palette release];
	[super dealloc];
}

-(void)resetByteStream
{
	currentchannel=numchannels;
}

-(uint8_t)produceByteAtOffset:(off_t)pos
{
	if(currentchannel>=numchannels)
	{
		const uint8_t *palettebytes=palette.bytes;
		NSInteger palettelength=palette.length;

		int pixel=CSInputNextByte(input);

		if(pixel<palettelength/numchannels) memcpy(bytebuffer,&palettebytes[pixel*numchannels],numchannels);
		else memset(bytebuffer,0,numchannels);

		currentchannel=0;
	}

	return bytebuffer[currentchannel++];
}

@end
