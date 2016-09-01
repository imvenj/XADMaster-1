#import "CSHandle.h"
#import "XADException.h"

NS_ASSUME_NONNULL_BEGIN

@interface XADResourceFork:NSObject
{
	NSDictionary *resources;
}

+(nullable instancetype)resourceForkWithHandle:(CSHandle *)handle NS_SWIFT_UNAVAILABLE("Call may throw, use init(handle:error:) instead");
+(nullable instancetype)resourceForkWithHandle:(CSHandle *)handle error:(nullable XADError *)errorptr;

-(instancetype)init NS_DESIGNATED_INITIALIZER;

-(void)parseFromHandle:(CSHandle *)handle;
-(nullable NSData *)resourceDataForType:(uint32_t)type identifier:(int)identifier;

-(nullable NSMutableDictionary *)_parseResourceDataFromHandle:(CSHandle *)handle;
-(nullable NSDictionary *)_parseMapFromHandle:(CSHandle *)handle withDataObjects:(NSMutableDictionary *)dataobjects;
-(nullable NSDictionary *)_parseReferencesFromHandle:(CSHandle *)handle count:(int)count;

@end

NS_ASSUME_NONNULL_END
