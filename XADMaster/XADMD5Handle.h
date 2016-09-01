#import "CSStreamHandle.h"
#import "Checksums.h"
#import "Progress.h"

#include "Crypto/md5.h"

@interface XADMD5Handle:CSStreamHandle
{
	CSHandle *parent;
	NSData *digest;

	MD5_CTX context;
}

-(instancetype)initWithHandle:(CSHandle *)handle length:(off_t)length correctDigest:(NSData *)correctdigest;

-(void)resetStream;
-(int)streamAtMost:(int)num toBuffer:(void *)buffer;

@property (NS_NONATOMIC_IOSONLY, readonly) BOOL hasChecksum;
@property (NS_NONATOMIC_IOSONLY, readonly, getter=isChecksumCorrect) BOOL checksumCorrect;

@property (readonly) double estimatedProgress;

@end

