#import "XADLibXADParser.h"
#import "CSMultiHandle.h"



static xadUINT32 ProgressFunc(struct Hook *hook,xadPTR object,struct xadProgressInfo *info);
static xadUINT32 InFunc(struct Hook *hook,xadPTR object,struct xadHookParam *param);
static xadUINT32 OutFunc(struct Hook *hook,xadPTR object,struct xadHookParam *param);

static struct xadMasterBaseP *xmb;


@implementation XADLibXADParser

struct xadMasterBaseP *xadOpenLibrary(xadINT32 version);

+(int)requiredHeaderSize
{
	if(!xmb) xmb=xadOpenLibrary(12);

	return (int)((struct xadMasterBaseP *)xmb)->xmb_RecogSize;
}

+(BOOL)recognizeFileWithHandle:(CSHandle *)handle firstBytes:(NSData *)data name:(NSString *)name
{
	if(!xmb) xmb=xadOpenLibrary(12);

	// Kludge to recognize ADF disk images, since the filesystem parsers don't provide recognition functions
	NSString *ext=name.pathExtension.lowercaseString;
	if([ext isEqual:@"adf"]) return YES;

	struct XADInHookData indata;
	indata.fh=handle;
	indata.name=name.UTF8String;

	struct Hook inhook;
	inhook.h_Entry=(xadUINT32(*)(struct Hook *,xadPTR,void*))InFunc;
	inhook.h_Data=(void *)&indata;

	if(xadRecogFile(xmb,data.length,data.bytes,
		XAD_INHOOK,&inhook,
	TAG_DONE)) return YES;

	return NO;
}

-(id)init
{
	if((self=[super init]))
	{
		archive=NULL;
		namedata=nil;
	}
	return self;
}

-(void)dealloc
{
	xadFreeInfo(xmb,archive); // check?
	xadFreeObjectA(xmb,archive,NULL);

	[namedata release];

	[super dealloc];
}

-(void)parse
{
	namedata=[[self.name dataUsingEncoding:NSUTF8StringEncoding] mutableCopy];
	[namedata increaseLengthBy:1];

	if(!(archive=xadAllocObjectA(xmb,XADOBJ_ARCHIVEINFO,NULL)))
	{
		[XADException raiseOutOfMemoryException];
	}

	indata.fh=self.handle;
	indata.name=namedata.bytes;
	inhook.h_Entry=(xadUINT32(*)(struct Hook *,xadPTR,void*))InFunc;
	inhook.h_Data=(void *)&indata;

	progresshook.h_Entry=(xadUINT32(*)(struct Hook *,xadPTR,void*))ProgressFunc;
	progresshook.h_Data=(void *)self;

	addonbuild=YES;
	numfilesadded=0;
	numdisksadded=0;

	struct TagItem tags[]={
		XAD_INHOOK,(uintptr_t)&inhook,
		XAD_PROGRESSHOOK,(uintptr_t)&progresshook,
	TAG_DONE};

	int err=xadGetInfoA(xmb,archive,tags);
/*	if(!err&&archive->xaip_ArchiveInfo.xai_DiskInfo)
	{
		xadFreeInfo(xmb,archive);
		[[self handle] seekToFileOffset:0];
		err=xadGetDiskInfo(xmb,archive,XAD_INDISKARCHIVE,tags,TAG_DONE);
	}
	else if(err==XADERR_FILETYPE)
*/
	if(err==XADERR_FILETYPE)
	{
		err=xadGetDiskInfoA(xmb,archive,tags);
	}

	if(err) [XADException raiseExceptionWithXADError:err];

/*	if(![fileinfos count])
	{
		if(error) *error=XADERR_DATAFORMAT;
		return NO;
	}*/

	if(!addonbuild) // encountered entries which could not be immediately added
	{
		struct xadFileInfo *fileinfo=archive->xaip_ArchiveInfo.xai_FileInfo;

		for(int i=0;i<numfilesadded&&fileinfo;i++) fileinfo=fileinfo->xfi_Next;

		while(fileinfo&&self.shouldKeepParsing)
		{
			[self addEntryWithDictionary:[self dictionaryForFileInfo:fileinfo]];
			fileinfo=fileinfo->xfi_Next;
		}

		struct xadDiskInfo *diskinfo=archive->xaip_ArchiveInfo.xai_DiskInfo;

		for(int i=0;i<numdisksadded&&diskinfo;i++) diskinfo=diskinfo->xdi_Next;

		while(diskinfo&&self.shouldKeepParsing)
		{
			[self addEntryWithDictionary:[self dictionaryForDiskInfo:diskinfo]];
			diskinfo=diskinfo->xdi_Next;
		}
	}
}

-(BOOL)newEntryCallback:(struct xadProgressInfo *)proginfo
{
	if(addonbuild)
	{
		struct xadFileInfo *info=proginfo->xpi_FileInfo;
		if(info)
		{
			if(!(info->xfi_Flags&XADFIF_EXTRACTONBUILD)||(info->xfi_Flags&XADFIF_ENTRYMAYCHANGE))
			{
				addonbuild=NO;
			}
			else
			{
				[self addEntryWithDictionary:[self dictionaryForFileInfo:info]];
				numfilesadded++;
			}
		}
		else
		{
			struct xadDiskInfo *info=proginfo->xpi_DiskInfo;

			if(!(info->xdi_Flags&XADDIF_EXTRACTONBUILD)||(info->xdi_Flags&XADDIF_ENTRYMAYCHANGE))
			{
				addonbuild=NO;
			}
			else
			{
				[self addEntryWithDictionary:[self dictionaryForDiskInfo:info]];
				numdisksadded++;
			}
		}
	}
	return self.shouldKeepParsing;
}

-(NSMutableDictionary *)dictionaryForFileInfo:(struct xadFileInfo *)info
{
	NSMutableDictionary *dict=[NSMutableDictionary dictionaryWithObjectsAndKeys:
		[self XADPathWithCString:(const char *)info->xfi_FileName separators:XADEitherPathSeparator],XADFileNameKey,
		@(info->xfi_CrunchSize),XADCompressedSizeKey,
		[NSValue valueWithPointer:info],@"LibXADFileInfo",
	nil];

	if(!(info->xfi_Flags&XADFIF_NOUNCRUNCHSIZE))
	dict[XADFileSizeKey] = @(info->xfi_Size);

	if(!(info->xfi_Flags&XADFIF_NODATE))
	{
		xadUINT32 timestamp;
		xadConvertDates(xmb,XAD_DATEXADDATE,&info->xfi_Date,XAD_GETDATEUNIX,&timestamp,TAG_DONE);

		dict[XADLastModificationDateKey] = [NSDate dateWithTimeIntervalSince1970:timestamp];
	}

	//if(info->xfi_Flags&XADFIF_NOFILENAME)
	// TODO: set no filename flag

	if(info->xfi_Flags&XADFIF_UNIXPROTECTION)
	dict[XADPosixPermissionsKey] = @(info->xfi_UnixProtect);

	if(info->xfi_Protection)
	dict[XADAmigaProtectionBitsKey] = @(info->xfi_Protection);

	if(info->xfi_Flags&XADFIF_DIRECTORY)
	dict[XADIsDirectoryKey] = @YES;

	if(info->xfi_Flags&XADFIF_LINK)
	dict[XADLinkDestinationKey] = [self XADStringWithCString:(const char *)info->xfi_LinkName];

	if(info->xfi_Flags&XADFIF_CRYPTED)
	dict[XADIsEncryptedKey] = @YES;

//	if(info->xfi_Flags&XADFIF_PARTIALFILE) // TODO: figure out what this is
//	[dict setObject:[NSNumber numberWithBool:YES] forKey:XADIsPartialKey];

	if(info->xfi_OwnerUID)
	dict[XADPosixUserKey] = @(info->xfi_OwnerUID);

	if(info->xfi_OwnerGID)
	dict[XADPosixGroupKey] = @(info->xfi_OwnerGID);

	if(info->xfi_UserName)
	dict[XADPosixUserNameKey] = [self XADStringWithCString:(const char *)info->xfi_UserName];

	if(info->xfi_GroupName)
	dict[XADPosixGroupNameKey] = [self XADStringWithCString:(const char *)info->xfi_GroupName];

	if(info->xfi_Comment)
	dict[XADCommentKey] = [self XADStringWithCString:(const char *)info->xfi_Comment];

	if(archive->xaip_ArchiveInfo.xai_Flags&XADAIF_FILECORRUPT) [self setObject:@YES forPropertyKey:XADIsCorruptedKey];

	return dict;
}

-(NSMutableDictionary *)dictionaryForDiskInfo:(struct xadDiskInfo *)info
{
	int sectors;
	if(!(info->xdi_Flags&(XADDIF_NOCYLINDERS|XADDIF_NOCYLSECTORS)))
	sectors=(info->xdi_HighCyl-info->xdi_LowCyl+1)*info->xdi_CylSectors;
	else sectors=info->xdi_TotalSectors;

	NSString *filename=self.name.stringByDeletingPathExtension;
	if(numdisksadded>0) filename=[NSString stringWithFormat:@"%@.%d.adf",filename,numdisksadded];
	else filename=[NSString stringWithFormat:@"%@.adf",filename];

	NSMutableDictionary *dict=[NSMutableDictionary dictionaryWithObjectsAndKeys:
		[self XADPathWithUnseparatedString:filename],XADFileNameKey,
		@(sectors*info->xdi_SectorSize),XADFileSizeKey,
		[NSValue valueWithPointer:info],@"LibXADDiskInfo",
	nil];

	return dict;
}



-(CSHandle *)handleForEntryWithDictionary:(NSDictionary *)dict wantChecksum:(BOOL)checksum
{
	NSMutableData *data;
	xadERROR err;

	struct xadFileInfo *info=[dict[@"LibXADFileInfo"] pointerValue];
	if(info)
	{
		const char *pass=NULL;
		if(info->xfi_Flags&XADFIF_CRYPTED) pass=self.encodedCStringPassword;

		if(info->xfi_Flags&XADFIF_NOUNCRUNCHSIZE) data=[NSMutableData data];
		else data=[NSMutableData dataWithCapacity:(long)info->xfi_Size];

		struct Hook outhook;
		outhook.h_Entry=(xadUINT32(*)(struct Hook *,xadPTR,void*))OutFunc;
		outhook.h_Data=(void *)data;

		err=xadFileUnArc(xmb,archive,
			XAD_ENTRYNUMBER,info->xfi_EntryNumber,
			XAD_OUTHOOK,&outhook,
			pass?XAD_PASSWORD:TAG_IGNORE,pass,
		TAG_DONE);
	}
	else
	{
		struct xadDiskInfo *info=[dict[@"LibXADDiskInfo"] pointerValue];

		data=[NSMutableData dataWithCapacity:[dict[XADFileSizeKey] unsignedIntValue]];

		struct Hook outhook;
		outhook.h_Entry=(xadUINT32(*)(struct Hook *,xadPTR,void*))OutFunc;
		outhook.h_Data=(void *)data;

		err=xadDiskUnArc(xmb,archive,
			XAD_ENTRYNUMBER,info->xdi_EntryNumber,
			XAD_OUTHOOK,&outhook,
		TAG_DONE);
	}

	return [[[XADLibXADMemoryHandle alloc] initWithData:data
	successfullyExtracted:err==XADERR_OK] autorelease];
}

-(NSString *)formatName
{
	if(!archive->xaip_ArchiveInfo.xai_Client) return @"libxad";

	NSString *format=[[[NSString alloc] initWithBytes:archive->xaip_ArchiveInfo.xai_Client->xc_ArchiverName
	length:strlen(archive->xaip_ArchiveInfo.xai_Client->xc_ArchiverName) encoding:NSISOLatin1StringEncoding] autorelease];
	return format;
}



static xadUINT32 InFunc(struct Hook *hook,xadPTR object,struct xadHookParam *param)
{
	struct xadArchiveInfo *archive=object;
	struct XADInHookData *data=(struct XADInHookData *)hook->h_Data;

	CSHandle *fh=data->fh;

	switch(param->xhp_Command)
	{
		case XADHC_INIT:
		{
			if([fh respondsToSelector:@selector(handles)])
			{
				NSArray *handles=[(id)fh handles];
				NSInteger count=handles.count;

				archive->xai_MultiVolume=calloc(sizeof(xadSize),count+1);

				off_t total=0;
				for(NSInteger i=0;i<count;i++)
				{
					archive->xai_MultiVolume[i]=total;
					total+=[handles[i] fileSize];
				}
			}

			archive->xai_InName=(xadSTRPTR)data->name;

			return XADERR_OK;
		}

		case XADHC_SEEK:
			[fh skipBytes:param->xhp_CommandData];
			param->xhp_DataPos=fh.offsetInFile;
			return XADERR_OK;

		case XADHC_READ:
			[fh readBytes:(int)param->xhp_BufferSize toBuffer:param->xhp_BufferPtr];
			param->xhp_DataPos=fh.offsetInFile;
			return XADERR_OK;

		case XADHC_FULLSIZE:
		{
			off_t filesize=fh.fileSize;
			if(filesize==CSHandleMaxLength) return XADERR_NOTSUPPORTED;
			param->xhp_CommandData=filesize;
			return XADERR_OK;
		}

		case XADHC_FREE:
			free(archive->xai_MultiVolume);
			archive->xai_MultiVolume=NULL;
			return XADERR_OK;

		 default:
			return XADERR_NOTSUPPORTED;
	}
}

static xadUINT32 ProgressFunc(struct Hook *hook,xadPTR object,struct xadProgressInfo *info)
{
	XADLibXADParser *parser=(XADLibXADParser *)hook->h_Data;

	switch(info->xpi_Mode)
	{
		case XADPMODE_PROGRESS:
			//return [archive _progressCallback:info];
			return XADPIF_OK;

		case XADPMODE_NEWENTRY:
			if(![parser newEntryCallback:info]) return 0;
			return XADPIF_OK;

		case XADPMODE_END:
		case XADPMODE_ERROR:
		case XADPMODE_GETINFOEND:
		default:
		break;
	}

	return XADPIF_OK;
}

static xadUINT32 OutFunc(struct Hook *hook,xadPTR object,struct xadHookParam *param)
{
	NSMutableData *data=(NSMutableData *)hook->h_Data;

	switch(param->xhp_Command)
	{
		case XADHC_INIT:
		case XADHC_FREE:
			return XADERR_OK;

		case XADHC_WRITE:
			[data appendBytes:param->xhp_BufferPtr length:(long)param->xhp_BufferSize];
			return XADERR_OK;

		 default:
			return XADERR_NOTSUPPORTED;
	}
}

@end




@implementation XADLibXADMemoryHandle
@synthesize checksumCorrect = success;

-(id)initWithData:(NSData *)data successfullyExtracted:(BOOL)wassuccess
{
	if((self=[super initWithData:data]))
	{
		success=wassuccess;
	}
	return self;
}

-(BOOL)hasChecksum { return YES; }

-(BOOL)isChecksumCorrect { return success; }

@end
