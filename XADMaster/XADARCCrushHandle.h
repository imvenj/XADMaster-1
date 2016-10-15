#import "CSByteStreamHandle.h"
#import "LZW.h"

@interface XADARCCrushHandle:CSByteStreamHandle
{
	LZW *lzw;
	int symbolsize,nextsizebump;
	BOOL useliteralbit;

	int numrecentstrings,ringindex;
	BOOL stringring[500];

	int usageindex;

	int currbyte;
	uint8_t buffer[8192];
	uint8_t usage[8192];
}

-(instancetype)initWithHandle:(CSHandle *)handle;
-(instancetype)initWithHandle:(CSHandle *)handle length:(off_t)length;

-(void)resetByteStream;
-(uint8_t)produceByteAtOffset:(off_t)pos;

@end

