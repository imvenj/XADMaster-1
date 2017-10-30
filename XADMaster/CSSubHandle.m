#import "CSSubHandle.h"
#import "XADException.h"

#if !__has_feature(objc_arc)
#error this file needs to be compiled with Automatic Reference Counting (ARC)
#endif

@implementation CSSubHandle
@synthesize parentHandle = parent;
@synthesize startOffsetInParent = start;

-(id)initWithHandle:(CSHandle *)handle from:(off_t)from length:(off_t)length error:(NSError**)error
{
	if((self=[super initWithName:[NSString stringWithFormat:@"%@ (Subrange from %qd, length %qd)",[handle name],from,length]]))
	{
		parent=handle;
		start=from;
		end=from+length;
		
		if (!parent) {
			if (error) {
				*error = [NSError errorWithDomain:NSOSStatusErrorDomain code:paramErr userInfo:nil];
			}
			return nil;
		}

		if (![parent seekToFileOffset:start error:error]) {
			return nil;
		}
	}
	return self;
}


-(id)initAsCopyOf:(CSSubHandle *)other
{
	if((self=[super initAsCopyOf:other]))
	{
		parent=[other->parent copy];
		start=other->start;
		end=other->end;
	}
	return self;
}

-(off_t)fileSize
{
	return end-start;
/*	off_t parentsize=[parent fileSize];
	if(parentsize>end) return end-start;
	else if(parentsize<start) return 0;
	else return parentsize-start;*/
}

-(off_t)offsetInFile
{
	return [parent offsetInFile]-start;
}

-(BOOL)atEndOfFile
{
	return [parent offsetInFile]==end||[parent atEndOfFile];
}

-(BOOL)seekToFileOffset:(off_t)offs error:(NSError**)error
{
    if(offs<0) {
        if (error) {
            *error = [NSError errorWithDomain:XADErrorDomain code:XADErrorBadParameters userInfo:nil];
        }
        return NO;
    }
    if(offs>end) {
        if (error) {
            *error = [NSError errorWithDomain:XADErrorDomain code:XADErrorEndOfFile userInfo:nil];
        }
        return NO;
    }
    return [parent seekToFileOffset:offs+start error:error];
}

-(BOOL)seekToEndOfFileWithError:(NSError *__autoreleasing *)error
{
//	@try
	{
		return [parent seekToFileOffset:end error:error];
	}
/*	@catch(NSException *e)
	{
		if([[e name] isEqual:@"CSEndOfFileException"]) [parent seekToEndOfFile];
		else @throw;
	}*/
}

-(BOOL)readAtMost:(size_t)num toBuffer:(void *)buffer totalWritten:(ssize_t*)tw error:(NSError**)error;
{
	off_t curr=[parent offsetInFile];
	if(curr+num>end) num=(ssize_t)(end-curr);
	if(num<=0) {
		if (tw) {
			*tw = 0;
		}
		return 0;
	} else {
		return [parent readAtMost:num toBuffer:buffer totalWritten:tw error:error];
	}
}

@end
