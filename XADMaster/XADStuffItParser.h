#import "XADArchiveParser.h"

@interface XADStuffItParser:XADArchiveParser
{
}

-(void)parse DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("This function throws exceptions as part of its control flow");
-(XADString *)nameOfCompressionMethod:(int)method;

-(CSHandle *)handleForEntryWithDictionary:(NSDictionary *)dict wantChecksum:(BOOL)checksum;
-(CSHandle *)decryptHandleForEntryWithDictionary:(NSDictionary *)dict handle:(CSHandle *)fh;
-(NSString *)formatName;

@end
