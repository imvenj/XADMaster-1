#import <Foundation/Foundation.h>
#import "CSHandle.h"

#define CSMultiHandle XADMultiHandle

extern NSString *const CSSizeOfSegmentUnknownException;
extern NSErrorDomain const CSMultiHandleErrorDomain;

typedef NS_ENUM(NSInteger, CSMultiHandleError) {
	CSMultiHandleErrorUnknownSizeOfSegment,
};

@interface CSMultiHandle:CSHandle
{
	NSArray<CSHandle*> *handles;
	NSInteger currhandle;
}

+(instancetype)multiHandleWithHandleArray:(NSArray<CSHandle*> *)handlearray;
+(instancetype)multiHandleWithHandles:(CSHandle *)firsthandle,... NS_REQUIRES_NIL_TERMINATION;

// Initializers
-(instancetype)initWithHandles:(NSArray<CSHandle*> *)handlearray;
-(instancetype)initAsCopyOf:(CSMultiHandle *)other;

// Public methods
@property (readonly, copy) NSArray<CSHandle*> *handles;
@property (readonly) CSHandle *currentHandle;

// Implemented by this class
@property (NS_NONATOMIC_IOSONLY, readonly) off_t fileSize;
@property (NS_NONATOMIC_IOSONLY, readonly) off_t offsetInFile;
@property (NS_NONATOMIC_IOSONLY, readonly) BOOL atEndOfFile;

-(BOOL)seekToFileOffset:(off_t)offs error:(NSError**)error;
-(BOOL)seekToEndOfFileWithError:(NSError**)error;
-(BOOL)readAtMost:(size_t)num toBuffer:(void *)buffer totalWritten:(ssize_t*)tw error:(NSError**)error;


// Internal methods
-(void)_raiseSizeUnknownForSegment:(long)i NS_SWIFT_UNAVAILABLE("Call throws exception");

@end
