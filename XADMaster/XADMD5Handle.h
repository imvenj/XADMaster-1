#import "CSStreamHandle.h"
#import "Checksums.h"
#import "Progress.h"

#if defined(USE_COMMON_CRYPTO) && USE_COMMON_CRYPTO
#include <CommonCrypto/CommonDigest.h>
typedef CC_MD5_CTX XADMD5;
#define XADMD5_Init CC_MD5_Init
#define XADMD5_Update CC_MD5_Update
#define XADMD5_Final CC_MD5_Final
#else
#include "Crypto/md5.h"
typedef MD5_CTX XADMD5;
#define XADMD5_Init MD5_Init
#define XADMD5_Update MD5_Update
#define XADMD5_Final MD5_Final
#endif

@interface XADMD5Handle:CSStreamHandle
{
	CSHandle *parent;
	NSData *digest;

	XADMD5 context;
}

-(instancetype)initWithHandle:(CSHandle *)handle length:(off_t)length correctDigest:(NSData *)correctdigest;

-(void)resetStream;
-(int)streamAtMost:(int)num toBuffer:(void *)buffer;

@property (NS_NONATOMIC_IOSONLY, readonly) BOOL hasChecksum;
@property (NS_NONATOMIC_IOSONLY, readonly, getter=isChecksumCorrect) BOOL checksumCorrect;

@property (readonly) double estimatedProgress;

@end

