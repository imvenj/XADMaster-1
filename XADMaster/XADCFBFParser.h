#import "XADArchiveParser.h"

@interface XADCFBFParser:XADArchiveParser
{
	int minsize,secsize,minisecsize;

	uint32_t rootdirectorynode,firstminisector;

	int numsectors,numminisectors;
	uint32_t *sectable,*minisectable;
}

+(int)requiredHeaderSize;
+(BOOL)recognizeFileWithHandle:(CSHandle *)handle firstBytes:(NSData *)data name:(NSString *)name;

-(void)parse DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("This function throws exceptions as part of its control flow");
-(XADString *)decodeFileNameWithBytes:(uint8_t *)bytes length:(int)length;
-(void)processEntry:(uint32_t)n atPath:(XADPath *)path entries:(NSArray *)entries;
-(void)seekToSector:(uint32_t)sector;
-(uint32_t)nextSectorAfter:(uint32_t)sector;

-(CSHandle *)handleForEntryWithDictionary:(NSDictionary *)dict wantChecksum:(BOOL)checksum;
-(NSString *)formatName;

@end
