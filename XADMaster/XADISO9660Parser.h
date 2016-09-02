#import "XADArchiveParser.h"

@interface XADISO9660Parser:XADArchiveParser
{
	int blocksize;
	BOOL isjoliet,ishighsierra;
	CSHandle *fh;
}

+(int)requiredHeaderSize;
+(BOOL)recognizeFileWithHandle:(CSHandle *)handle firstBytes:(NSData *)data
name:(NSString *)name propertiesToAdd:(NSMutableDictionary *)props;

-(instancetype)init;

-(void)parse DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("This function throws exceptions as part of its control flow");
-(void)parseVolumeDescriptorAtBlock:(uint32_t)block DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("This function throws exceptions as part of its control flow");
-(void)parseDirectoryWithPath:(XADPath *)path atBlock:(uint32_t)block length:(uint32_t)length DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("This function throws exceptions as part of its control flow");
-(BOOL)parseVolumeDescriptorAtBlock:(uint32_t)block error:(NSError**)error;
-(BOOL)parseDirectoryWithPath:(XADPath *)path atBlock:(uint32_t)block length:(uint32_t)length error:(NSError**)error;

-(XADString *)readStringOfLength:(int)length;
-(NSDate *)readLongDateAndTime;
-(NSDate *)readShortDateAndTime;
-(NSDate *)parseDateAndTimeWithBytes:(const uint8_t *)buffer long:(BOOL)islong;
-(NSDate *)parseLongDateAndTimeWithBytes:(const uint8_t *)buffer;
-(NSDate *)parseShortDateAndTimeWithBytes:(const uint8_t *)buffer;

-(CSHandle *)handleForEntryWithDictionary:(NSDictionary *)dict wantChecksum:(BOOL)checksum;
-(NSString *)formatName;

@end
