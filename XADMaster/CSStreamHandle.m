#import "CSStreamHandle.h"

#if !__has_feature(objc_arc)
#error this file needs to be compiled with Automatic Reference Counting (ARC)
#endif

@implementation CSStreamHandle
@synthesize fileSize = streamlength;
@synthesize offsetInFile = streampos;

-(id)initWithName:(NSString *)descname
{
	return [self initWithName:descname length:CSHandleMaxLength];
}

-(id)initWithName:(NSString *)descname length:(off_t)length
{
	if((self=[super initWithName:descname]))
	{
		streampos=0;
		streamlength=length;
		endofstream=NO;
		needsreset=YES;
		nextstreambyte=-1;

		input=NULL;
	}
	return self;
}

-(id)initWithHandle:(CSHandle *)handle
{
	return [self initWithHandle:handle length:CSHandleMaxLength bufferSize:4096];
}

-(id)initWithHandle:(CSHandle *)handle length:(off_t)length
{
	return [self initWithHandle:handle length:length bufferSize:4096];
}

-(id)initWithHandle:(CSHandle *)handle bufferSize:(int)buffersize
{
	return [self initWithHandle:handle length:CSHandleMaxLength bufferSize:buffersize];
}

-(id)initWithHandle:(CSHandle *)handle length:(off_t)length bufferSize:(int)buffersize;
{
	if((self=[super initWithName:[handle name]]))
	{
		streampos=0;
		streamlength=length;
		endofstream=NO;
		needsreset=YES;
		nextstreambyte=-1;

		input=CSInputBufferAlloc(handle,buffersize);
	}
	return self;
}

-(id)initAsCopyOf:(CSStreamHandle *)other
{
	[self _raiseNotSupported:_cmd];
	return nil;
}

-(void)dealloc
{
	CSInputBufferFree(input);
}


-(BOOL)atEndOfFile
{
	if(needsreset) { [self resetStreamWithError:NULL]; needsreset=NO; }

	if(endofstream) return YES;
	if(streampos==streamlength) return YES;
	if(nextstreambyte>=0) return NO;

	uint8_t b[1];
	@try
	{
		if([self streamAtMost:1 toBuffer:b totalRead:<#(ssize_t *)#> error:NULL]==1)
		{
			nextstreambyte=b[0];
			return NO;
		}
	}
	@catch(id e) {}

	endofstream=YES;
	return YES;
}

-(BOOL)seekToFileOffset:(off_t)offs error:(NSError *__autoreleasing *)error
{
	if(![self _prepareStreamSeekTo:offs error:error]) {
		return NO;
	}

	if(offs<streampos)
	{
		streampos=0;
		endofstream=NO;
		//nextstreambyte=-1;
		if(input) CSInputRestart(input);
		if (![self resetStreamWithError:error]) {
			return NO;
		}
	}

	if(offs==0) return YES;

	return [self readAndDiscardBytes:offs-streampos error:error];
}

-(BOOL)seekToEndOfFileWithError:(NSError *__autoreleasing *)error
{
	return [self readAndDiscardAtMost:CSHandleMaxLength error:error];
}

-(BOOL)readAtMost:(size_t)num toBuffer:(void *)buffer totalWritten:(ssize_t *)tw error:(NSError *__autoreleasing *)error
{
	if(needsreset) { [self resetStreamWithError:error]; needsreset=NO; }

	if(endofstream) return 0;
	if(streampos+num>streamlength) num=(int)(streamlength-streampos);
	if(!num) return 0;

	int offs=0;
	if(nextstreambyte>=0)
	{
		((uint8_t *)buffer)[0]=nextstreambyte;
		streampos++;
		nextstreambyte=-1;
		offs=1;
		if(num==1) {
			if (tw) {
				*tw = 1;
			}
			return YES;
		}
	}

	ssize_t actual = 0;
	if ([self streamAtMost:num-offs toBuffer:((uint8_t *)buffer)+offs totalRead:&actual error:error]) {
		
	}

	if(actual==0) endofstream=YES;

	streampos+=actual;

	if (tw) {
		*tw = actual + offs;
	}
	return YES;
}



-(BOOL)resetStreamWithError:(NSError *__autoreleasing *)error {
	return YES;
}

-(BOOL)streamAtMost:(size_t)num toBuffer:(void *)buffer totalRead:(ssize_t *)tw error:(NSError *__autoreleasing *)error
{
	if (tw) {
		*tw = 0;
	}
	return YES;
}




-(void)endStream
{
	endofstream=YES;
}

-(BOOL)_prepareStreamSeekTo:(off_t)offs error:(NSError *__autoreleasing *)error
{
	if(needsreset) {
		[self resetStreamWithError:error];
		needsreset=NO;
	}

	if (offs == streampos) {
		return NO;
	}
	if (endofstream && offs > streampos) {
		[self _raiseEOF];
	}
	if (offs > streamlength) {
		[self _raiseEOF];
	}
	if (nextstreambyte >= 0) {
		nextstreambyte=-1;
		streampos+=1;
		if(offs==streampos) return NO;
	}

	return YES;
}

-(void)setStreamLength:(off_t)length { streamlength=length; }

-(void)setInputBuffer:(CSInputBuffer *)inputbuffer
{
	CSInputBufferFree(input);
	input=inputbuffer;
}

@end
