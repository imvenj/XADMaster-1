#import "XADArchiveParser.h"

@interface XADNDSParser:XADArchiveParser

+(int)requiredHeaderSize;
+(BOOL)recognizeFileWithHandle:(CSHandle *)handle firstBytes:(NSData *)data name:(NSString *)name;

-(void)parse;
-(CSHandle *)handleForEntryWithDictionary:(NSDictionary<XADArchiveKeys,id> *)dict wantChecksum:(BOOL)checksum;
-(NSString *)formatName;

@end
