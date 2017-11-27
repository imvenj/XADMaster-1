#import "XADArchiveParser.h"
#import "CSMemoryHandle.h"
#import "libxad/include/functions.h"

@interface XADLibXADParser:XADArchiveParser
{
//	XADArchivePipe *pipe;
//	XADError lasterror;

	struct xadArchiveInfoP *archive;
	struct Hook inhook,progresshook;

	struct XADInHookData
	{
		CSHandle *fh;
		const char *name;
	} indata;

	BOOL addonbuild;
	int numfilesadded,numdisksadded;

	NSMutableData *namedata;
}

+(int)requiredHeaderSize;
+(BOOL)recognizeFileWithHandle:(CSHandle *)handle firstBytes:(NSData *)data
name:(NSString *)name propertiesToAdd:(NSMutableDictionary *)props;

-(instancetype)init;

-(void)parse;
-(BOOL)newEntryCallback:(struct xadProgressInfo *)proginfo;
-(NSMutableDictionary *)dictionaryForFileInfo:(struct xadFileInfo *)info;
-(NSMutableDictionary *)dictionaryForDiskInfo:(struct xadDiskInfo *)info;

-(CSHandle *)handleForEntryWithDictionary:(NSDictionary<XADArchiveKeys,id> *)dict wantChecksum:(BOOL)checksum;

-(NSString *)formatName;

@end



@interface XADLibXADMemoryHandle:CSMemoryHandle
{
	BOOL success;
}

-(instancetype)initWithData:(NSData *)data successfullyExtracted:(BOOL)wassuccess;
@property (NS_NONATOMIC_IOSONLY, readonly) BOOL hasChecksum;
@property (NS_NONATOMIC_IOSONLY, readonly, getter=isChecksumCorrect) BOOL checksumCorrect;

@end
