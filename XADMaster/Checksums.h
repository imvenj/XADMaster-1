#import "CSHandle.h"
#import "CSSubHandle.h"
#import "CSStreamHandle.h"

@interface CSHandle (Checksums)

@property (readonly) BOOL hasChecksum;
@property (readonly, getter=isChecksumCorrect) BOOL checksumCorrect;

@end

@interface CSSubHandle (Checksums)

@property (readonly) BOOL hasChecksum;
@property (readonly, getter=isChecksumCorrect) BOOL checksumCorrect;

@end

@interface CSStreamHandle (Checksums)

-(BOOL)hasChecksum;
-(BOOL)isChecksumCorrect;

@end

#define CSChecksumWrapperHandle XADChecksumWrapperHandle

@interface CSChecksumWrapperHandle:CSHandle
{
	CSHandle *parent,*checksum;
}

-(instancetype)initWithHandle:(CSHandle *)handle checksumHandle:(CSHandle *)checksumhandle;

-(off_t)fileSize;
-(off_t)offsetInFile;
-(BOOL)atEndOfFile;
-(void)seekToFileOffset:(off_t)offs;
-(void)seekToEndOfFile;
-(void)pushBackByte:(int)byte;
-(int)readAtMost:(int)num toBuffer:(void *)buffer;
-(void)writeBytes:(int)num fromBuffer:(const void *)buffer;

-(BOOL)hasChecksum;
-(BOOL)isChecksumCorrect;

@end
