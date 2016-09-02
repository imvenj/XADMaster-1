#import "CSHandle.h"

#import <stdio.h>

#define CSFileHandle XADFileHandle

extern NSString *CSCannotOpenFileException;
extern NSString *CSFileErrorException;

@interface CSFileHandle:CSHandle
{
	FILE *fh;
	BOOL close;

	NSLock *multilock;
	CSFileHandle *parent;
	off_t pos;
}

+(CSFileHandle *)fileHandleForReadingAtPath:(NSString *)path DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("Call can throw exception");
+(CSFileHandle *)fileHandleForWritingAtPath:(NSString *)path DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("Call can throw exception");
+(CSFileHandle *)fileHandleForReadingAtPath:(NSString *)path error:(NSError**)error;
+(CSFileHandle *)fileHandleForWritingAtPath:(NSString *)path error:(NSError**)error;
+(CSFileHandle *)fileHandleForPath:(NSString *)path modes:(NSString *)modes DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("Call can throw exception");
+(CSFileHandle *)fileHandleForPath:(NSString *)path modes:(NSString *)modes error:(NSError**)error;

// Initializers
-(instancetype)initWithFilePointer:(FILE *)file closeOnDealloc:(BOOL)closeondealloc name:(NSString *)descname;
-(instancetype)initAsCopyOf:(CSFileHandle *)other;
-(void)close;

// Public methods
@property (readonly, assign) FILE *filePointer;

// Implemented by this class
@property (NS_NONATOMIC_IOSONLY, readonly) off_t fileSize;
@property (NS_NONATOMIC_IOSONLY, readonly) off_t offsetInFile;
@property (NS_NONATOMIC_IOSONLY, readonly) BOOL atEndOfFile;

-(void)seekToFileOffset:(off_t)offs DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("Call can throw exception");
-(void)seekToEndOfFile DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("Call can throw exception");
-(void)pushBackByte:(int)byte DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("Call can throw exception");
-(int)readAtMost:(int)num toBuffer:(void *)buffer DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("Call can throw exception");
-(void)writeBytes:(int)num fromBuffer:(const void *)buffer DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("Call can throw exception");


-(BOOL)seekToFileOffset:(off_t)offs error:(NSError**)error;
-(BOOL)seekToEndOfFileWithError:(NSError**)error;
-(BOOL)pushBackByte:(uint8_t)byte error:(NSError**)error;
-(BOOL)readAtMost:(size_t)num toBuffer:(void *)buffer totalWritten:(ssize_t*)tw error:(NSError**)error;
-(BOOL)writeBytes:(size_t)num fromBuffer:(const void *)buffer error:(NSError**)error;

// Internal methods
-(void)_raiseError NS_SWIFT_UNAVAILABLE("Call always throws exception");
-(void)_setMultiMode NS_SWIFT_UNAVAILABLE("Call always throws exception");

@end
