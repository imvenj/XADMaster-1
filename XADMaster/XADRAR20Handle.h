#import "XADFastLZSSHandle.h"
#import "XADRARParser.h"
#import "XADPrefixCode.h"
#import "RARAudioDecoder.h"

@interface XADRAR20Handle:XADFastLZSSHandle
{
	__unsafe_unretained XADRARParser *parser;

	NSArray *files;
	int file;
	off_t endpos;

	XADPrefixCode *maincode,*offsetcode,*lengthcode;
	XADPrefixCode *audiocode[4];

	int lastoffset,lastlength;
	int oldoffset[4],oldoffsetindex;

	BOOL audioblock;
	int channel,channeldelta,numchannels;
	RAR20AudioState audiostate[4];

	int lengthtable[1028];
}

-(id)initWithRARParser:(XADRARParser *)parentparser files:(NSArray *)filearray;

-(void)resetLZSSHandle;
-(void)startNextFile;
-(void)expandFromPosition:(off_t)pos;
-(void)allocAndParseCodes;

@end
