#import <Foundation/Foundation.h>
#import "CSStreamHandle.h"

#include <setjmp.h>

#define CSByteStreamHandle XADByteStreamHandle

@interface CSByteStreamHandle:CSStreamHandle
{
	uint8_t (*bytestreamproducebyte_ptr)(id,SEL,off_t);
	int bytesproduced;
	@public
	jmp_buf eofenv;
}

// Intializers
-(instancetype)initWithName:(NSString *)descname length:(off_t)length;
-(instancetype)initWithHandle:(CSHandle *)handle length:(off_t)length bufferSize:(int)buffersize;
-(instancetype)initAsCopyOf:(CSByteStreamHandle *)other;

// Implemented by this class
-(BOOL)streamAtMost:(size_t)num toBuffer:(void *)buffer totalRead:(ssize_t *)tw error:(NSError **)error;
-(BOOL)resetStreamWithError:(NSError **)error;

// Implemented by subclasses
-(void)resetByteStream;
-(uint8_t)produceByteAtOffset:(off_t)pos;

// Called by subclasses
-(void)endByteStream;

@end



extern NSString *const CSByteStreamEOFReachedException;

static inline void CSByteStreamEOF(CSByteStreamHandle *self) __attribute__((noreturn));
static inline void CSByteStreamEOF(CSByteStreamHandle *self) { longjmp(self->eofenv,1); }
