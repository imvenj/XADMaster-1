#import "CSByteStreamHandle.h"

@interface XADRAR15CryptHandle:CSByteStreamHandle
{
	NSData *password;

	uint16_t key0,key1,key2,key3;
}

-(instancetype)initWithHandle:(CSHandle *)handle length:(off_t)length password:(NSData *)passdata;

-(void)resetByteStream;
-(uint8_t)produceByteAtOffset:(off_t)pos;

@end
