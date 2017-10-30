#import <Foundation/Foundation.h>
#import "CSHandle.h"

#define CSMemoryHandle XADMemoryHandle

@interface CSMemoryHandle:CSHandle
{
	NSData *backingdata;
	off_t memorypos;
}

+(CSMemoryHandle *)memoryHandleForReadingData:(NSData *)data;
+(CSMemoryHandle *)memoryHandleForReadingBuffer:(const void *)buf length:(size_t)len;
+(CSMemoryHandle *)memoryHandleForReadingMappedFile:(NSString *)filename error:(NSError**)error;
+(CSMemoryHandle *)memoryHandleForWriting;

// Initializers
-(instancetype)initWithData:(NSData *)dataobj;
-(instancetype)initAsCopyOf:(CSMemoryHandle *)other;

// Public methods
@property (NS_NONATOMIC_IOSONLY, readonly, strong) NSData *data;
-(NSMutableData *)mutableDataWithError:(NSError**)error;

// Implemented by this class
@property (NS_NONATOMIC_IOSONLY, readonly) off_t fileSize;
@property (NS_NONATOMIC_IOSONLY, readonly) off_t offsetInFile;
@property (NS_NONATOMIC_IOSONLY, readonly) BOOL atEndOfFile;


-(BOOL)seekToFileOffset:(off_t)offs error:(NSError**)error;
-(BOOL)seekToEndOfFileWithError:(NSError**)error;
//-(BOOL)pushBackByte:(uint8_t)byte error:(NSError**)error;
-(BOOL)readAtMost:(size_t)num toBuffer:(void *)buffer totalWritten:(ssize_t*)tw error:(NSError**)error;
-(BOOL)writeBytes:(size_t)num fromBuffer:(const void *)buffer error:(NSError**)error;


-(NSData *)fileContentsWithError:(NSError **)error;
-(NSData *)remainingFileContentsWithError:(NSError **)error;
-(NSData *)readDataOfLength:(NSInteger)length error:(NSError **)error;
-(NSData *)readDataOfLengthAtMost:(NSInteger)length error:(NSError **)error;
-(NSData *)copyDataOfLength:(NSInteger)length error:(NSError **)error;
-(NSData *)copyDataOfLengthAtMost:(NSInteger)length error:(NSError **)error;

@end
