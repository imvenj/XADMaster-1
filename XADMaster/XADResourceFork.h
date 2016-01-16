#import "CSHandle.h"
#import "XADException.h"

@interface XADResourceFork:NSObject
{
	NSDictionary *resources;
}

+(instancetype)resourceForkWithHandle:(CSHandle *)handle NS_SWIFT_UNAVAILABLE("This function throws exceptions as part of its control flow; use resourceForkWithHandle:error: instead");
+(instancetype)resourceForkWithHandle:(CSHandle *)handle error:(NSError **)errorptr;

-(instancetype)init NS_DESIGNATED_INITIALIZER;

-(void)parseFromHandle:(CSHandle *)handle;
-(NSData *)resourceDataForType:(uint32_t)type identifier:(int)identifier;

-(NSMutableDictionary *)_parseResourceDataFromHandle:(CSHandle *)handle;
-(NSDictionary *)_parseMapFromHandle:(CSHandle *)handle withDataObjects:(NSMutableDictionary *)dataobjects;
-(NSDictionary *)_parseReferencesFromHandle:(CSHandle *)handle count:(int)count;

@end
