#import "CSHandle.h"

#define CSMemoryHandle XADMemoryHandle

@interface CSMemoryHandle:CSHandle
{
	NSData *backingdata;
	off_t memorypos;
}

+(CSMemoryHandle *)memoryHandleForReadingData:(NSData *)data;
+(CSMemoryHandle *)memoryHandleForReadingBuffer:(const void *)buf length:(unsigned)len;
+(CSMemoryHandle *)memoryHandleForReadingMappedFile:(NSString *)filename;
+(CSMemoryHandle *)memoryHandleForWriting;

// Initializers
-(instancetype)initWithData:(NSData *)dataobj;
-(instancetype)initAsCopyOf:(CSMemoryHandle *)other;

// Public methods
-(NSData *)data;
-(NSMutableData *)mutableData;

// Implemented by this class
@property (NS_NONATOMIC_IOSONLY, readonly) off_t fileSize;
@property (NS_NONATOMIC_IOSONLY, readonly) off_t offsetInFile;
@property (NS_NONATOMIC_IOSONLY, readonly) BOOL atEndOfFile;

-(void)seekToFileOffset:(off_t)offs DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("Call can throw exception");
-(void)seekToEndOfFile DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("Call can throw exception");
//-(void)pushBackByte:(int)byte DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("Call can throw exception");
-(int)readAtMost:(int)num toBuffer:(void *)buffer DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("Call can throw exception");
-(void)writeBytes:(int)num fromBuffer:(const void *)buffer DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("Call can throw exception");


-(BOOL)seekToFileOffset:(off_t)offs error:(NSError**)error;
-(BOOL)seekToEndOfFileWithError:(NSError**)error;
//-(BOOL)pushBackByte:(uint8_t)byte error:(NSError**)error;
-(BOOL)readAtMost:(size_t)num toBuffer:(void *)buffer totalWritten:(ssize_t*)tw error:(NSError**)error;
-(BOOL)writeBytes:(size_t)num fromBuffer:(const void *)buffer error:(NSError**)error;


-(NSData *)fileContents;
-(NSData *)remainingFileContents;
-(NSData *)readDataOfLength:(int)length;
-(NSData *)readDataOfLengthAtMost:(int)length;
-(NSData *)copyDataOfLength:(int)length;
-(NSData *)copyDataOfLengthAtMost:(int)length;

@end
