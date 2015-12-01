#import "CSBlockStreamHandle.h"

@interface XADStuffItXBlockHandle:CSBlockStreamHandle
{
	CSHandle *parent;
	off_t startoffs;
	uint8_t *buffer;
	size_t currsize;
}

-(instancetype)initWithHandle:(CSHandle *)handle;

-(void)resetBlockStream;
-(int)produceBlockAtOffset:(off_t)pos;

@end
