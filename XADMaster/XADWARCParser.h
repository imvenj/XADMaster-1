#import "XADArchiveParser.h"

@interface XADWARCParser:XADArchiveParser

+(int)requiredHeaderSize;
+(BOOL)recognizeFileWithHandle:(CSHandle *)handle firstBytes:(NSData *)data name:(NSString *)name;

-(void)parse;
-(NSMutableDictionary<NSString*,NSString*> *)parseHTTPHeadersWithHandle:(CSHandle *)handle;
-(NSArray *)readHTTPHeadersWithHandle:(CSHandle *)handle;

-(NSArray<NSString*> *)pathComponentsForURLString:(NSString *)urlstring;
-(NSMutableDictionary *)insertDirectory:(NSString *)name inDirectory:(NSMutableDictionary *)dir;
-(void)insertFile:(NSString *)name record:(NSMutableDictionary *)record inDirectory:(NSMutableDictionary *)dir;
-(void)buildXADPathsForFilesInDirectory:(NSMutableDictionary *)dir parentPath:(XADPath *)parent;

-(CSHandle *)handleForEntryWithDictionary:(NSDictionary<XADArchiveKeys,id> *)dict wantChecksum:(BOOL)checksum;
-(NSString *)formatName;

@end
