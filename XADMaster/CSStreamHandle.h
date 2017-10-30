#import <Foundation/Foundation.h>
#import "CSHandle.h"
#import "CSInputBuffer.h"

#define CSStreamHandle XADStreamHandle

@interface CSStreamHandle:CSHandle
{
	off_t streampos,streamlength;
	BOOL needsreset,endofstream;
	int nextstreambyte;

	@public
	CSInputBuffer *input;
}

// Initializers
-(instancetype)initWithName:(NSString *)descname;
-(instancetype)initWithName:(NSString *)descname length:(off_t)length;
-(instancetype)initWithHandle:(CSHandle *)handle;
-(instancetype)initWithHandle:(CSHandle *)handle length:(off_t)length;
-(instancetype)initWithHandle:(CSHandle *)handle bufferSize:(int)buffersize;
-(instancetype)initWithHandle:(CSHandle *)handle length:(off_t)length bufferSize:(int)buffersize;
-(instancetype)initAsCopyOf:(CSStreamHandle *)other;

// Implemented by this class
@property (NS_NONATOMIC_IOSONLY, readonly) off_t fileSize;
@property (NS_NONATOMIC_IOSONLY, readonly) off_t offsetInFile;
@property (NS_NONATOMIC_IOSONLY, readonly) BOOL atEndOfFile;
-(BOOL)seekToFileOffset:(off_t)offs error:(NSError **)error;
-(BOOL)seekToEndOfFileWithError:(NSError **)error;
-(BOOL)readAtMost:(size_t)num toBuffer:(void *)buffer totalWritten:(ssize_t *)tw error:(NSError **)error;

// Implemented by subclasses
-(BOOL)resetStreamWithError:(NSError**)error;
-(BOOL)streamAtMost:(size_t)num toBuffer:(void *)buffer totalRead:(ssize_t *)tw error:(NSError **)error;

// Called by subclasses
-(void)endStream;
-(BOOL)_prepareStreamSeekTo:(off_t)offs error:(NSError**)error;
-(void)setStreamLength:(off_t)length;
-(void)setInputBuffer:(CSInputBuffer *)inputbuffer;

@end
