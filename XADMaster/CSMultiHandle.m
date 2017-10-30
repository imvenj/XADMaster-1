#import "CSMultiHandle.h"

#if !__has_feature(objc_arc)
#error this file needs to be compiled with Automatic Reference Counting (ARC)
#endif

NSString *const CSSizeOfSegmentUnknownException=@"CSSizeOfSegmentUnknownException";
NSErrorDomain const CSMultiHandleErrorDomain = @"CSMultiHandleError";

@implementation CSMultiHandle
@synthesize handles;

+(CSMultiHandle *)multiHandleWithHandleArray:(NSArray *)handlearray
{
	if(!handlearray) return nil;
	long count=[handlearray count];
	if(count==0) return nil;
	else if(count==1) return handlearray[0];
	else return [[self alloc] initWithHandles:handlearray];
}

+(CSMultiHandle *)multiHandleWithHandles:(CSHandle *)firsthandle,...
{
	if(!firsthandle) return nil;

	NSMutableArray *array=[NSMutableArray arrayWithObject:firsthandle];
	CSHandle *handle;
	va_list va;

	va_start(va,firsthandle);
	while((handle=va_arg(va,CSHandle *))) [array addObject:handle];
	va_end(va);

	return [self multiHandleWithHandleArray:array];
}


-(id)initWithHandles:(NSArray *)handlearray
{
	if((self=[super initWithName:[NSString stringWithFormat:@"%@, and %ld more combined",[handlearray[0] name],(long)[handlearray count]-1]]))
	{
		handles=[handlearray copy];
		currhandle=0;
	}
	return self;
}

-(id)initAsCopyOf:(CSMultiHandle *)other
{
	if((self=[super initAsCopyOf:other]))
	{
		NSMutableArray *handlearray=[NSMutableArray arrayWithCapacity:[other->handles count]];
		NSEnumerator *enumerator=[other->handles objectEnumerator];
		CSHandle *handle;
		while((handle=[enumerator nextObject])) [handlearray addObject:[handle copy]];

		handles=[handlearray copy];
		currhandle=other->currhandle;
	}
	return self;
}

-(CSHandle *)currentHandle { return handles[currhandle]; }

-(off_t)fileSize
{
	off_t size=0;
	long count=[handles count];
	for(int i=0;i<count-1;i++)
	{
		off_t segsize=[(CSHandle *)handles[i] fileSize];
		if(segsize==CSHandleMaxLength) [self _raiseSizeUnknownForSegment:i];
		size+=segsize;
	}

	off_t segsize=[(CSHandle *)[handles lastObject] fileSize];
	if(segsize==CSHandleMaxLength) return CSHandleMaxLength;
	else return size+segsize;
}

-(off_t)offsetInFile
{
	off_t offs=0;
	for(int i=0;i<currhandle;i++)
	{
		off_t segsize=[(CSHandle *)handles[i] fileSize];
		if(segsize==CSHandleMaxLength) [self _raiseSizeUnknownForSegment:i];
		offs+=segsize;
	}
	return offs+[handles[currhandle] offsetInFile];
}

-(BOOL)atEndOfFile
{
	return currhandle==[handles count]-1&&[handles[currhandle] atEndOfFile];
}

-(BOOL)seekToFileOffset:(off_t)offs error:(NSError *__autoreleasing *)error
{
	long count=[handles count];

	if(offs==0)
	{
		currhandle=0;
	}
	else
	{
		for(currhandle=0;currhandle<count-1;currhandle++)
		{
			off_t segsize=[(CSHandle *)handles[currhandle] fileSize];
			if(segsize==CSHandleMaxLength) {
				if (error) {
					*error = [NSError errorWithDomain:CSMultiHandleErrorDomain code:CSMultiHandleErrorUnknownSizeOfSegment userInfo:@{NSLocalizedDescriptionKey: [NSString stringWithFormat:@"Size of CSMultiHandle segment %ld (%@) unknown.",currhandle,handles[currhandle]]}];
				}
				return NO;
			};
			if(offs<segsize) break;
			offs-=segsize;
		}
	}

	return [(CSHandle *)handles[currhandle] seekToFileOffset:offs error:error];
}

-(BOOL)seekToEndOfFileWithError:(NSError *__autoreleasing *)error
{
	currhandle=[handles count]-1;
	return [(CSHandle *)handles[currhandle] seekToEndOfFileWithError:error];
}

-(BOOL)readAtMost:(size_t)num toBuffer:(void *)buffer totalWritten:(ssize_t*)tw error:(NSError**)error;
{
	int total=0;
	for(;;)
	{
		off_t actual=[handles[currhandle] readAtMost:num-total toBuffer:((char *)buffer)+total];
		total+=actual;
		if(total==num||currhandle==[handles count]-1) return total;
		currhandle++;
		[(CSHandle *)handles[currhandle] seekToFileOffset:0];
	}
}

-(void)_raiseSizeUnknownForSegment:(long)i
{
	[NSException raise:CSSizeOfSegmentUnknownException
	format:@"Size of CSMultiHandle segment %ld (%@) unknown.",i,handles[i]];
}

@end
