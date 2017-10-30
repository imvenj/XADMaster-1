#import "CSHandle.h"

@interface XADAppleDouble:NSObject

+(BOOL)parseAppleDoubleWithHandle:(CSHandle *)fh resourceForkOffset:(off_t *)resourceoffsetptr
               resourceForkLength:(off_t *)resourcelengthptr extendedAttributes:(NSDictionary<NSString*,NSData*> **)extattrsptr
                            error:(NSError**)error;
+(BOOL)parseAppleDoubleExtendedAttributesWithHandle:(CSHandle *)fh intoDictionary:(NSMutableDictionary<NSString*,NSData*> *)extattrs error:(NSError**)error;

+(BOOL)writeAppleDoubleHeaderToHandle:(CSHandle *)fh resourceForkSize:(int)ressize
                   extendedAttributes:(NSDictionary<NSString*,NSData*> *)extattrs error:(NSError**)error;

@end

