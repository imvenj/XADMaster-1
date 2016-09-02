#import "XADStuffItParser.h"

@interface XADStuffIt5Parser:XADStuffItParser
{
}

+(int)requiredHeaderSize;
+(BOOL)recognizeFileWithHandle:(CSHandle *)handle firstBytes:(NSData *)data name:(NSString *)name;
-(void)parse DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("This function throws exceptions as part of its control flow");
-(void)parseWithNumberOfTopLevelEntries:(int)numentries DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("This function throws exceptions as part of its control flow");
-(BOOL)parseWithError:(NSError **)error;
-(BOOL)parseWithNumberOfTopLevelEntries:(int)numentries error:(NSError**)error;
-(NSString *)formatName;

@end

@interface XADStuffIt5ExeParser:XADStuffIt5Parser
{
}

+(int)requiredHeaderSize;
+(BOOL)recognizeFileWithHandle:(CSHandle *)handle firstBytes:(NSData *)data name:(NSString *)name;
-(void)parse DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("This function throws exceptions as part of its control flow");
-(BOOL)parseWithError:(NSError **)error;

@end

