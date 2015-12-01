#import "CSStreamHandle.h"
#import "XADRARParser.h"

@interface XADRARInputHandle:CSStreamHandle
{
	XADRARParser *parser;
	NSArray *parts;

	int part;
	off_t partend;

	uint32_t crc,correctcrc;
}

-(instancetype)initWithRARParser:(XADRARParser *)parent parts:(NSArray *)partarray;

-(void)resetStream;
-(int)streamAtMost:(int)num toBuffer:(void *)buffer;

-(void)startNextPart;

@end
