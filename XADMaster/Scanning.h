#import "CSHandle.h"

typedef int (*CSByteMatchingFunctionPointer)(const uint8_t *bytes,int available,off_t offset,void *state);
typedef int (^CSByteMatchingFunctionBlock)(const uint8_t *bytes,size_t available,off_t offset);

@interface CSHandle (Scanning)

-(BOOL)scanForByteString:(const void *)bytes length:(NSInteger)length;
-(int)scanUsingMatchingFunction:(CSByteMatchingFunctionPointer)function
maximumLength:(int)maximumlength;
-(int)scanUsingMatchingFunction:(CSByteMatchingFunctionPointer)function
maximumLength:(int)maximumlength context:(void *)contextptr;
-(int)scanUsingMatchingBlock:(CSByteMatchingFunctionBlock)function
               maximumLength:(NSInteger)maximumlength;

@end
