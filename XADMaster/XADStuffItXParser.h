#import "XADArchiveParser.h"
#import "CSMemoryHandle.h"

@interface XADStuffItXParser:XADArchiveParser
{
	NSData *repeatedentrydata;
	NSArray *repeatedentries;
	BOOL repeatedentryhaschecksum,repeatedentryiscorrect;
}

+(int)requiredHeaderSize;
+(BOOL)recognizeFileWithHandle:(CSHandle *)handle firstBytes:(NSData *)data name:(NSString *)name;

-(void)parse DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("This function throws exceptions as part of its control flow");
-(void)parseCatalogWithHandle:(CSHandle *)fh entryArray:(NSArray *)entries entryDictionary:(NSDictionary *)dict DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("This function throws exceptions as part of its control flow");

-(BOOL)parseWithError:(NSError **)error;
-(BOOL)parseCatalogWithHandle:(CSHandle *)fh entryArray:(NSArray *)entries entryDictionary:(NSDictionary *)dict error:(NSError**)error;

-(CSHandle *)handleForEntryWithDictionary:(NSDictionary *)dict wantChecksum:(BOOL)checksum;
-(CSHandle *)handleForSolidStreamWithObject:(id)obj wantChecksum:(BOOL)checksum;
-(NSString *)formatName;

@end

@interface XADStuffItXRepeatedEntryHandle:CSMemoryHandle
{
	BOOL haschecksum,ischecksumcorrect;
}

-(instancetype)initWithData:(NSData *)data hasChecksum:(BOOL)hascheck isChecksumCorrect:(BOOL)iscorrect;

@property (NS_NONATOMIC_IOSONLY, readonly) BOOL hasChecksum;
@property (NS_NONATOMIC_IOSONLY, readonly, getter=isChecksumCorrect) BOOL checksumCorrect;

@end
