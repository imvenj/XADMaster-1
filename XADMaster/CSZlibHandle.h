#import <Foundation/Foundation.h>
#import "CSStreamHandle.h"

#define CSZlibHandle XADZlibHandle

extern NSString *const CSZlibException;

@interface CSZlibHandle:CSStreamHandle
{
	CSHandle *parent;
}

+(CSZlibHandle *)zlibHandleWithHandle:(CSHandle *)handle;
+(CSZlibHandle *)zlibHandleWithHandle:(CSHandle *)handle length:(off_t)length;
+(CSZlibHandle *)deflateHandleWithHandle:(CSHandle *)handle;
+(CSZlibHandle *)deflateHandleWithHandle:(CSHandle *)handle length:(off_t)length;

// Intializers
-(instancetype)initWithHandle:(CSHandle *)handle length:(off_t)length header:(BOOL)header name:(NSString *)descname;
-(instancetype)initAsCopyOf:(CSZlibHandle *)other;

// Public methods
-(void)setSeekBackAtEOF:(BOOL)seekateof;
-(void)setEndStreamAtInputEOF:(BOOL)endateof;

// Implemented by this class
-(void)resetStream;
-(int)streamAtMost:(int)num toBuffer:(void *)buffer;

// Internal methods
-(void)_raiseZlib NS_SWIFT_UNAVAILABLE("Call throws");

@end
