#import "CSHandle.h"
#import "XADException.h"

NS_ASSUME_NONNULL_BEGIN

@interface XADResourceFork:NSObject
{
	NSDictionary *resources;
}

+(nullable instancetype)resourceForkWithHandle:(CSHandle *)handle NS_SWIFT_UNAVAILABLE("This function throws exceptions as part of its control flow; use resourceForkWithHandle:error: instead");
+(nullable instancetype)resourceForkWithHandle:(CSHandle *)handle error:(NSError *__nullable*__nullable)errorptr;

-(instancetype)init NS_DESIGNATED_INITIALIZER;

-(void)parseFromHandle:(CSHandle *)handle;
-(nullable NSData *)resourceDataForType:(uint32_t)type identifier:(int)identifier;

-(nullable NSMutableDictionary *)_parseResourceDataFromHandle:(CSHandle *)handle;
-(nullable NSDictionary *)_parseMapFromHandle:(CSHandle *)handle withDataObjects:(NSMutableDictionary *)dataobjects;
-(nullable NSDictionary *)_parseReferencesFromHandle:(CSHandle *)handle count:(int)count;

@end

NS_ASSUME_NONNULL_END
