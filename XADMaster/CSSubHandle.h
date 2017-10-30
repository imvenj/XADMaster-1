#import <Foundation/Foundation.h>
#import "CSHandle.h"

#define CSSubHandle XADSubHandle

@interface CSSubHandle:CSHandle
{
	CSHandle *parent;
	off_t start,end;
}

// Initializers
-(instancetype)initWithHandle:(CSHandle *)handle from:(off_t)from length:(off_t)length error:(NSError**)error;
-(instancetype)initAsCopyOf:(CSSubHandle *)other;

// Public methods
@property (readonly, strong) CSHandle *parentHandle;
@property (readonly) off_t startOffsetInParent;

// Implemented by this class
@property (NS_NONATOMIC_IOSONLY, readonly) off_t fileSize;
@property (NS_NONATOMIC_IOSONLY, readonly) off_t offsetInFile;
@property (NS_NONATOMIC_IOSONLY, readonly) BOOL atEndOfFile;

-(BOOL)seekToFileOffset:(off_t)offs error:(NSError**)error;
-(BOOL)seekToEndOfFileWithError:(NSError**)error;
-(BOOL)readAtMost:(size_t)num toBuffer:(void *)buffer totalWritten:(ssize_t*)tw error:(NSError**)error;

@end
