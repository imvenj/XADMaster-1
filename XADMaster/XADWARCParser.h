#import "XADArchiveParser.h"

@interface XADWARCParser:XADArchiveParser
{
}

+(int)requiredHeaderSize;
+(BOOL)recognizeFileWithHandle:(CSHandle *)handle firstBytes:(NSData *)data name:(NSString *)name;

-(void)parse DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("This function throws exceptions as part of its control flow");
-(NSMutableDictionary *)parseHTTPHeadersWithHandle:(CSHandle *)handle DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("This function throws exceptions as part of its control flow");
-(NSArray *)readHTTPHeadersWithHandle:(CSHandle *)handle;

-(BOOL)parseWithError:(NSError **)error;
-(NSMutableDictionary *)parseHTTPHeadersWithHandle:(CSHandle *)handle error:(NSError**)error;


-(NSArray *)pathComponentsForURLString:(NSString *)urlstring;
-(NSMutableDictionary *)insertDirectory:(NSString *)name inDirectory:(NSMutableDictionary *)dir;
-(void)insertFile:(NSString *)name record:(NSMutableDictionary *)record inDirectory:(NSMutableDictionary *)dir;
-(void)buildXADPathsForFilesInDirectory:(NSMutableDictionary *)dir parentPath:(XADPath *)parent;

-(CSHandle *)handleForEntryWithDictionary:(NSDictionary *)dict wantChecksum:(BOOL)checksum;
-(NSString *)formatName;

@end
