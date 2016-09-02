#import "XADArchiveParser.h"

@interface XADDiskDoublerParser:XADArchiveParser
{
}

+(int)requiredHeaderSize;
+(BOOL)recognizeFileWithHandle:(CSHandle *)handle firstBytes:(NSData *)data name:(NSString *)name;

-(void)parse DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("This function throws exceptions as part of its control flow");
-(void)parseArchive DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("This function throws exceptions as part of its control flow");
-(void)parseArchive2 DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("This function throws exceptions as part of its control flow");

-(BOOL)parseArchive1WithError:(NSError**)error;
-(BOOL)parseArchive2WithError:(NSError**)error;
-(uint32_t)parseFileHeaderWithHandle:(CSHandle *)fh name:(XADPath *)name;

-(NSString *)nameForMethod:(int)method;

-(CSHandle *)handleForEntryWithDictionary:(NSDictionary *)dict wantChecksum:(BOOL)checksum;
-(NSString *)formatName;

@end
