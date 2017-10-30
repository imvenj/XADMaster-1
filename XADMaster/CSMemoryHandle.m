#import "CSMemoryHandle.h"
#import "XADException.h"

#if !__has_feature(objc_arc)
#error this file needs to be compiled with Automatic Reference Counting (ARC)
#endif

@implementation CSMemoryHandle
@synthesize data = backingdata;


+(CSMemoryHandle *)memoryHandleForReadingData:(NSData *)data
{
	return [[CSMemoryHandle alloc] initWithData:data];
}

+(CSMemoryHandle *)memoryHandleForReadingBuffer:(const void *)buf length:(size_t)len
{
	return [[CSMemoryHandle alloc] initWithData:[NSData dataWithBytesNoCopy:(void *)buf length:len freeWhenDone:NO]];
}

+(CSMemoryHandle *)memoryHandleForReadingMappedFile:(NSString *)filename error:(NSError *__autoreleasing *)error
{
	NSData *mappedData = [NSData dataWithContentsOfFile:filename options:NSDataReadingMappedAlways error:error];
	if (!mappedData) {
		return nil;
	}
	return [[CSMemoryHandle alloc] initWithData:mappedData];
}

+(CSMemoryHandle *)memoryHandleForWriting
{
	return [[CSMemoryHandle alloc] initWithData:[NSMutableData data]];
}


-(id)initWithData:(NSData *)data
{
	if((self=[super initWithName:[NSString stringWithFormat:@"%@ at %p",[data class],data]]))
	{
		memorypos=0;
		backingdata=data;
	}
	return self;
}

-(id)initAsCopyOf:(CSMemoryHandle *)other
{
	if((self=[super initAsCopyOf:other]))
	{
		memorypos=other->memorypos;
		backingdata=other->backingdata;
	}
	return self;
}

-(NSMutableData *)mutableDataWithError:(NSError *__autoreleasing *)error
{
	if(![backingdata isKindOfClass:[NSMutableData class]]) {
		if (error) {
			*error = nil;
		}
		[self _raiseNotSupported:_cmd];
		return nil;
	}
	return (NSMutableData *)backingdata;
}



-(off_t)fileSize { return [backingdata length]; }

-(off_t)offsetInFile { return memorypos; }

-(BOOL)atEndOfFile { return memorypos==[backingdata length]; }



-(BOOL)seekToFileOffset:(off_t)offs error:(NSError *__autoreleasing *)error
{
	if(offs<0) {
		[self _raiseNotSupported:_cmd];
		return NO;
	}
	if(offs>[backingdata length]) {
		if (error) {
			*error = [NSError errorWithDomain:XADErrorDomain code:XADErrorEndOfFile userInfo:nil];
		}
		return NO;
	}
	memorypos=offs;
	return YES;
}

-(BOOL)seekToEndOfFileWithError:(NSError *__autoreleasing *)error
{
	memorypos=[backingdata length];
	return YES;
}

//-(void)pushBackByte:(int)byte {}

-(int)readAtMost:(int)num toBuffer:(void *)buffer
{
	if(!num) return 0;

	unsigned long len=[backingdata length];
	if(memorypos==len) return 0;
	if(memorypos+num>len) num=(int)(len-memorypos);
	memcpy(buffer,(uint8_t *)[backingdata bytes]+memorypos,num);
	memorypos+=num;
	return num;
}

-(void)writeBytes:(int)num fromBuffer:(const void *)buffer
{
	if(![backingdata isKindOfClass:[NSMutableData class]]) [self _raiseNotSupported:_cmd];
	NSMutableData *mbackingdata=(NSMutableData *)backingdata;

	if(memorypos+num>[mbackingdata length]) [mbackingdata setLength:(long)memorypos+num];
	memcpy((uint8_t *)[mbackingdata mutableBytes]+memorypos,buffer,num);
	memorypos+=num;
}


-(NSData *)fileContentsWithError:(NSError *__autoreleasing *)error { return backingdata; }

-(NSData *)remainingFileContentsWithError:(NSError *__autoreleasing *)error
{
	if(memorypos==0) return backingdata;
	else return [super remainingFileContentsWithError:error];
}

-(NSData *)readDataOfLength:(NSInteger)length error:(NSError *__autoreleasing *)error
{
	unsigned long totallen=[backingdata length];
	if(memorypos+length>totallen) {
		if (error) {
			*error = [NSError errorWithDomain:XADErrorDomain code:XADErrorEndOfFile userInfo:nil];
		}
		return nil;
	};
	NSData *subbackingdata=[backingdata subdataWithRange:NSMakeRange((long)memorypos,length)];
	memorypos+=length;
	return subbackingdata;
}

-(NSData *)readDataOfLengthAtMost:(NSInteger)length error:(NSError *__autoreleasing *)error
{
	unsigned long totallen=[backingdata length];
	if(memorypos+length>totallen) length=(int)(totallen-memorypos);
	NSData *subbackingdata=[backingdata subdataWithRange:NSMakeRange((long)memorypos,length)];
	memorypos+=length;
	return subbackingdata;
}

-(NSData *)copyDataOfLength:(NSInteger)length error:(NSError *__autoreleasing *)error
{
	return [self readDataOfLength:length error:error];
	
}

-(NSData *)copyDataOfLengthAtMost:(NSInteger)length error:(NSError *__autoreleasing *)error
{
	return [self readDataOfLengthAtMost:length error:error];
}

@end
