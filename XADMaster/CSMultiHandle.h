#import "CSHandle.h"

#define CSMultiHandle XADMultiHandle

extern NSString *const CSSizeOfSegmentUnknownException;

@interface CSMultiHandle:CSHandle
{
	NSArray<CSHandle*> *handles;
	NSInteger currhandle;
}

+(CSMultiHandle *)multiHandleWithHandleArray:(NSArray<CSHandle*> *)handlearray;
+(CSMultiHandle *)multiHandleWithHandles:(CSHandle *)firsthandle,... NS_REQUIRES_NIL_TERMINATION;

// Initializers
-(instancetype)initWithHandles:(NSArray<CSHandle*> *)handlearray;
-(instancetype)initAsCopyOf:(CSMultiHandle *)other;

// Public methods
@property (readonly, retain) NSArray<CSHandle*> *handles;
@property (readonly) CSHandle *currentHandle;

// Implemented by this class
@property (NS_NONATOMIC_IOSONLY, readonly) off_t fileSize;
@property (NS_NONATOMIC_IOSONLY, readonly) off_t offsetInFile;
@property (NS_NONATOMIC_IOSONLY, readonly) BOOL atEndOfFile;

-(void)seekToFileOffset:(off_t)offs DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("Call can throw exception");
-(void)seekToEndOfFile DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("Call can throw exception");
-(int)readAtMost:(int)num toBuffer:(void *)buffer DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("Call can throw exception");


-(BOOL)seekToFileOffset:(off_t)offs error:(NSError**)error;
-(BOOL)seekToEndOfFileWithError:(NSError**)error;
-(BOOL)readAtMost:(size_t)num toBuffer:(void *)buffer totalWritten:(ssize_t*)tw error:(NSError**)error;


// Internal methods
-(void)_raiseSizeUnknownForSegment:(long)i;

@end
