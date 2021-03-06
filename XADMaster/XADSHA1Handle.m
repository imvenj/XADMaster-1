#import "XADSHA1Handle.h"
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

@implementation XADSHA1Handle
{
	NSData *digest;
	
	XADSHA1 context;
}

-(id)initWithHandle:(CSHandle *)handle length:(off_t)length correctDigest:(NSData *)correctdigest;
{
	if((self=[super initWithParentHandle:handle length:length]))
	{
		digest=[correctdigest retain];
	}
	return self;
}

-(void)dealloc
{
	[digest release];
	[super dealloc];
}

-(void)resetStream
{
	XADSHA1_Init(&context);
	[parent seekToFileOffset:0];
}

-(int)streamAtMost:(int)num toBuffer:(void *)buffer
{
	int actual=[parent readAtMost:num toBuffer:buffer];
	XADSHA1_Update(&context,buffer,actual);
	return actual;
}

-(BOOL)hasChecksum { return YES; }

-(BOOL)isChecksumCorrect
{
	if(digest.length!=20) return NO;

	XADSHA1 copy;
	copy=context;

	uint8_t buf[20];
	XADSHA1_Final(buf,&copy);

	return memcmp(digest.bytes,buf,20)==0;
}

-(double)estimatedProgress { return parent.estimatedProgress; }

@end


