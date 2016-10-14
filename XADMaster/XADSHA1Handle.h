#import "CSStreamHandle.h"
#import "Checksums.h"
#import "Progress.h"

#if defined(USE_COMMON_CRYPTO) && USE_COMMON_CRYPTO
#include <CommonCrypto/CommonDigest.h>
typedef CC_SHA1_CTX XADSHA1;
#define XADSHA1_Init CC_SHA1_Init
#define XADSHA1_Update CC_SHA1_Update
#define XADSHA1_Final CC_SHA1_Final
#else
#include "Crypto/sha.h"
typedef SHA_CTX XADSHA1;
#define XADSHA1_Init SHA1_Init
#define XADSHA1_Update SHA1_Update
#define XADSHA1_Final SHA1_Final
#endif

@interface XADSHA1Handle:CSStreamHandle
{
	CSHandle *parent;
	NSData *digest;

	XADSHA1 context;
}

-(id)initWithHandle:(CSHandle *)handle length:(off_t)length correctDigest:(NSData *)correctdigest;

-(void)resetStream;
-(int)streamAtMost:(int)num toBuffer:(void *)buffer;

@property (NS_NONATOMIC_IOSONLY, readonly) BOOL hasChecksum;
@property (NS_NONATOMIC_IOSONLY, readonly, getter=isChecksumCorrect) BOOL checksumCorrect;

@property (readonly) double estimatedProgress;

@end

