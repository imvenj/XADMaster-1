#import "CSHandle.h"
#import "CSSubHandle.h"
#import "XADException.h"

#include <sys/stat.h>

#if !__has_feature(objc_arc)
#error this file needs to be compiled with Automatic Reference Counting (ARC)
#endif

NSString *const CSOutOfMemoryException=@"CSOutOfMemoryException";
NSString *const CSEndOfFileException=@"CSEndOfFileException";
NSString *const CSNotImplementedException=@"CSNotImplementedException";
NSString *const CSNotSupportedException=@"CSNotSupportedException";



@implementation CSHandle
@synthesize name;

-(id)initWithName:(NSString *)descname
{
	if((self=[super init]))
	{
		name=[descname copy];

		bitoffs=-1;

		writebyte=0;
		writebitsleft=8;
	}
	return self;
}

-(id)initAsCopyOf:(CSHandle *)other
{
	if((self=[super init]))
	{
		name=[[other name] stringByAppendingString:@" (copy)"];

		bitoffs=other->bitoffs;
		readbyte=other->readbyte;
		readbitsleft=other->readbitsleft;
		writebyte=other->writebyte;
		writebitsleft=other->writebitsleft;
	}
	return self;
}

-(void)close {}



//-(off_t)fileSize { [self _raiseNotImplemented:_cmd]; return 0; }
-(off_t)fileSize { return CSHandleMaxLength; }

-(off_t)offsetInFile { [self _raiseNotImplemented:_cmd]; return 0; }

-(BOOL)atEndOfFile { [self _raiseNotImplemented:_cmd]; return NO; }

-(BOOL)seekToFileOffset:(off_t)offs error:(NSError**)error
{
	[self _raiseNotImplemented:_cmd];
	return NO;
}
-(BOOL)seekToEndOfFileWithError:(NSError**)error
{
	[self _raiseNotImplemented:_cmd];
	return NO;
}
-(BOOL)pushBackByte:(uint8_t)byte error:(NSError**)error
{
	[self _raiseNotImplemented:_cmd];
	return NO;
}
-(BOOL)readAtMost:(size_t)num toBuffer:(void *)buffer totalWritten:(ssize_t*)tw error:(NSError**)error;
{
	[self _raiseNotImplemented:_cmd];
	return NO;
}
-(BOOL)writeBytes:(size_t)num fromBuffer:(const void *)buffer error:(NSError**)error
{
	[self _raiseNotImplemented:_cmd];
	return NO;
}


-(void)flushReadBits { readbitsleft=0; }


-(NSData *)readLine
{
	int (*readatmost_ptr)(id,SEL,int,void *)=(void *)[self methodForSelector:@selector(readAtMost:toBuffer:)];

	NSMutableData *data=[NSMutableData data];
	for(;;)
	{
		uint8_t b[1];
		int actual=readatmost_ptr(self,@selector(readAtMost:toBuffer:),1,b);

		if(actual==0)
		{
			if([data length]==0) [self _raiseEOF];
			else break;
		}

		if(b[0]=='\n') break;

		[data appendBytes:b length:1];
	}

	const char *bytes=[data bytes];
	long length=[data length];
	if(length&&bytes[length-1]=='\r') [data setLength:length-1];

	return [NSData dataWithData:data];
}

-(NSString *)readLineWithEncoding:(NSStringEncoding)encoding
{
	return [[NSString alloc] initWithData:[self readLine] encoding:encoding];
}

-(NSString *)readUTF8Line
{
	return [[NSString alloc] initWithData:[self readLine] encoding:NSUTF8StringEncoding];
}


-(off_t)readAndDiscardAtMost:(off_t)num
{
	off_t skipped=0;
	uint8_t buf[16384];
	while(skipped<num)
	{
		off_t numbytes=num-skipped>sizeof(buf)?sizeof(buf):num-skipped;
		ssize_t actual = 0;
		BOOL success = [self readAtMost:numbytes toBuffer:buf totalWritten:&actual error:NULL];
		skipped+=actual;
		if(actual==0 || !success) break;
	}
	return skipped;
}

/*-(void)_raiseClosed
{
	[NSException raise:@"CSFileNotOpenException"
	format:@"Attempted to read from file \"%@\", which was not open.",name];
}*/

-(void)_raiseMemory
{
	[NSException raise:CSOutOfMemoryException
	format:@"Out of memory while attempting to read from file \"%@\" (%@).",name,[self class]];
}

-(void)_raiseEOF
{
	[NSException raise:CSEndOfFileException
	format:@"Attempted to read past the end of file \"%@\" (%@).",name,[self class]];
}

-(void)_raiseNotImplemented:(SEL)selector
{
	[NSException raise:CSNotImplementedException
	format:@"Attempted to use unimplemented method +[%@ %@] when reading from file \"%@\".",[self class],NSStringFromSelector(selector),name];
}

-(void)_raiseNotSupported:(SEL)selector
{
	[NSException raise:CSNotSupportedException
	format:@"Attempted to use unsupported method +[%@ %@] when reading from file \"%@\".",[self class],NSStringFromSelector(selector),name];
}

-(NSString *)description
{
	return [NSString stringWithFormat:@"%@ for \"%@\", position %qu",
	[self class],name,[self offsetInFile]];
}



-(id)copyWithZone:(NSZone *)zone
{
	return [[[self class] allocWithZone:zone] initAsCopyOf:self];
}

#pragma mark Swift-friendly read/write functions

-(BOOL)skipBytes:(off_t)bytes error:(NSError**)error
{
	return [self seekToFileOffset:[self offsetInFile]+bytes error:error];
}

-(int8_t)readInt8WithError:(NSError**)error
{
	int8_t c;
	NSError *intErr;
	BOOL valid = [self readBytes:1 toBuffer:&c error:&intErr];
	if (!valid) {
		if (error) {
			*error = intErr;
		}
		return 0;
	}
	return c;
}
-(uint8_t)readUInt8WithError:(NSError**)error
{
	uint8_t c;
	NSError *intErr;
	BOOL valid = [self readBytes:1 toBuffer:&c error:&intErr];
	if (!valid) {
		if (error) {
			*error = intErr;
		}
		return 0;
	}
	return c;
}


#define CSWriteValueImpl(type,name,conv) \
-(BOOL)name:(type)val error:(NSError**)error \
{ \
uint8_t bytes[sizeof(type)]; \
conv(bytes,val); \
return [self writeBytes:sizeof(type) fromBuffer:bytes error:error]; \
}

CSWriteValueImpl(int16_t,writeInt16BE,CSSetInt16BE)
CSWriteValueImpl(int32_t,writeInt32BE,CSSetInt32BE)
//CSWriteValueImpl(int64_t,writeInt64BE,CSSetInt64BE)
CSWriteValueImpl(uint16_t,writeUInt16BE,CSSetUInt16BE)
CSWriteValueImpl(uint32_t,writeUInt32BE,CSSetUInt32BE)
//CSWriteValueImpl(uint64_t,writeUInt64BE,CSSetUInt64BE)

CSWriteValueImpl(int16_t,writeInt16LE,CSSetInt16LE)
CSWriteValueImpl(int32_t,writeInt32LE,CSSetInt32LE)
//CSWriteValueImpl(int64_t,writeInt64LE,CSSetInt64LE)
CSWriteValueImpl(uint16_t,writeUInt16LE,CSSetUInt16LE)
CSWriteValueImpl(uint32_t,writeUInt32LE,CSSetUInt32LE)
//CSWriteValueImpl(uint64_t,writeUInt64LE,CSSetUInt64LE)

CSWriteValueImpl(uint32_t,writeID,CSSetUInt32BE)

#undef CSWriteValueImpl


#define CSReadValueImpl(type,name,conv) \
-(type) name ##WithError: (NSError**)error \
{ \
uint8_t bytes[sizeof(type)]; \
ssize_t totalWritten = 0; \
BOOL success = [self readAtMost:sizeof(type) toBuffer:bytes totalWritten:&totalWritten error:error]; \
if (totalWritten != sizeof(type)) { \
success = NO;\
if (error) { \
 *error = [NSError errorWithDomain:XADErrorDomain code:XADErrorEndOfFile userInfo:nil];\
} \
} \
if(!success) return 0; \
return conv(bytes); \
}

//CSReadValueImpl(int8_t,readInt8,(int8_t)*)
//CSReadValueImpl(uint8_t,readUInt8,(uint8_t)*)

CSReadValueImpl(int16_t,readInt16BE,CSInt16BE)
CSReadValueImpl(int32_t,readInt32BE,CSInt32BE)
CSReadValueImpl(int64_t,readInt64BE,CSInt64BE)
CSReadValueImpl(uint16_t,readUInt16BE,CSUInt16BE)
CSReadValueImpl(uint32_t,readUInt32BE,CSUInt32BE)
CSReadValueImpl(uint64_t,readUInt64BE,CSUInt64BE)

CSReadValueImpl(int16_t,readInt16LE,CSInt16LE)
CSReadValueImpl(int32_t,readInt32LE,CSInt32LE)
CSReadValueImpl(int64_t,readInt64LE,CSInt64LE)
CSReadValueImpl(uint16_t,readUInt16LE,CSUInt16LE)
CSReadValueImpl(uint32_t,readUInt32LE,CSUInt32LE)
CSReadValueImpl(uint64_t,readUInt64LE,CSUInt64LE)

-(int16_t)readInt16InBigEndianOrder:(BOOL)isbigendian error:(NSError **)error
{
	if(isbigendian)
		return [self readInt16BEWithError:error];
	else
		return [self readInt16LEWithError:error];
}
-(int32_t)readInt32InBigEndianOrder:(BOOL)isbigendian error:(NSError **)error {
	if(isbigendian)
		return [self readInt32BEWithError:error];
	else
		return [self readInt32LEWithError:error];
}
-(int64_t)readInt64InBigEndianOrder:(BOOL)isbigendian error:(NSError **)error {
	if(isbigendian)
		return [self readInt64BEWithError:error];
	else
		return [self readInt64LEWithError:error];
}
-(uint16_t)readUInt16InBigEndianOrder:(BOOL)isbigendian error:(NSError **)error {
	if(isbigendian)
		return [self readUInt16BEWithError:error];
	else
		return [self readUInt16LEWithError:error];
}
-(uint32_t)readUInt32InBigEndianOrder:(BOOL)isbigendian error:(NSError **)error {
	if(isbigendian)
		return [self readUInt32BEWithError:error];
	else
		return [self readUInt32LEWithError:error];
}
-(uint64_t)readUInt64InBigEndianOrder:(BOOL)isbigendian error:(NSError **)error {
	if(isbigendian)
		return [self readUInt64BEWithError:error];
	else
		return [self readUInt64LEWithError:error];
}

CSReadValueImpl(uint32_t,readID,CSUInt32BE)
#undef CSReadValueImpl


-(uint32_t)readBits:(int)bits error:(NSError**)error;
{
	int res=0,done=0;
	
	if ([self offsetInFile] != bitoffs)
		readbitsleft = 0;
	while (done < bits) {
		NSError *intErr = nil;
		if(!readbitsleft) {
			uint8_t tempByte = [self readUInt8WithError:&intErr];
			if (tempByte == 0 && intErr) {
				if (error) {
					*error = intErr;
				}
				return 0;
			}
			readbyte = tempByte;
			bitoffs=[self offsetInFile];
			readbitsleft=8;
		}
		
		int num=bits-done;
		if(num>readbitsleft) num=readbitsleft;
		res=(res<<num)|((readbyte>>(readbitsleft-num))&((1<<num)-1));
		
		done+=num;
		readbitsleft-=num;
	}
	return res;
}
-(uint32_t)readBitsLE:(int)bits error:(NSError**)error;
{
	int res=0,done=0;
	
	if ([self offsetInFile] != bitoffs) {
		readbitsleft = 0;
	}
	while (done<bits) {
		NSError *intErr = nil;
		if (!readbitsleft) {
			uint8_t tempByte = [self readUInt8WithError:&intErr];
			if (tempByte == 0 && intErr) {
				if (error) {
					*error = intErr;
				}
				return 0;
			}
			readbyte = tempByte;
			bitoffs=[self offsetInFile];
			readbitsleft=8;
		}
		
		int num=bits-done;
		if(num>readbitsleft) num=readbitsleft;
		res=res|(((readbyte>>(8-readbitsleft))&((1<<num)-1))<<done);
		
		done+=num;
		readbitsleft-=num;
	}
	return res;
}
-(int32_t)readSignedBits:(int)bits error:(NSError**)error;
{
	NSError *tmpErr = nil;
	uint32_t res=[self readBits:bits error:&tmpErr];
	if (res == 0 && tmpErr) {
		if (error) {
			*error = tmpErr;
		}
		return 0;
	}
	//	return res|((res&(1<<(bits-1)))*0xffffffff);
	return -(res&(1<<(bits-1)))|res;
}
-(int32_t)readSignedBitsLE:(int)bits error:(NSError**)error;
{
	NSError *tmpErr = nil;
	uint32_t res=[self readBitsLE:bits error:&tmpErr];
	if (res == 0 && tmpErr) {
		if (error) {
			*error = tmpErr;
		}
		return 0;
	}
	//	return res|((res&(1<<(bits-1)))*0xffffffff);
	return -(res&(1<<(bits-1)))|res;
}

-(NSData *)readLineWithError:(NSError**)error;
{
	BOOL (*readatmost_ptr)(id,SEL,size_t,void *,ssize_t*,NSError**)=(void *)[self methodForSelector:@selector(readAtMost:toBuffer:totalWritten:error:)];
	
	NSMutableData *data=[NSMutableData data];
	for(;;) {
		NSError *intErr = nil;
		uint8_t b[1];
		ssize_t actual = 0;
		BOOL success = readatmost_ptr(self,@selector(readAtMost:toBuffer:totalWritten:error:),1,b, &actual, &intErr);
		
		if (actual==0) {
			if (!success && intErr.code != eofErr && ![intErr.domain isEqualToString:NSOSStatusErrorDomain]) {
				if (error) {
					*error = intErr;
				}
				return nil;
			}
			if ([data length]==0) {
				if (error) {
					*error = [NSError errorWithDomain:NSOSStatusErrorDomain code:eofErr userInfo:nil];
				}
				return nil;
			} else
				break;
		}
		
		if(b[0]=='\n') break;
		
		[data appendBytes:b length:1];
	}
	
	const char *bytes=[data bytes];
	long length=[data length];
	if(length&&bytes[length-1]=='\r') [data setLength:length-1];
	
	return [NSData dataWithData:data];
}

-(NSString *)readLineWithEncoding:(NSStringEncoding)encoding error:(NSError**)error;
{
	NSData *ourData = [self readLineWithError:error];
	if (ourData) {
		NSString *toRet = [[NSString alloc] initWithData:ourData encoding:encoding];
		if (!toRet) {
			if (error) {
				*error = [NSError errorWithDomain:NSCocoaErrorDomain code:NSFileReadInapplicableStringEncodingError userInfo:@{NSStringEncodingErrorKey: @(encoding)}];
			}
			return nil;
		}
		return toRet;
	}
	return nil;
}

-(NSString *)readUTF8LineWithError:(NSError**)error;
{
	return [self readLineWithEncoding:NSUTF8StringEncoding error:error];
}

-(NSData *)fileContentsWithError:(NSError**)error;
{
	if (![self seekToFileOffset:0 error:error]) {
		return nil;
	}
	return [self remainingFileContentsWithError:error];
}
-(NSData *)remainingFileContentsWithError:(NSError**)error;
{
	uint8_t buffer[16384];
	NSMutableData *data = [NSMutableData data];
	ssize_t actual = 0;
	
	do {
		BOOL success = [self readAtMost:sizeof(buffer) toBuffer:buffer totalWritten:&actual error:error];
		if (!success) {
			return nil;
		}
		[data appendBytes:buffer length:actual];
	} while (actual != 0);
	
	return [NSData dataWithData:data];
}

-(NSData *)readDataOfLength:(NSInteger)length error:(NSError**)error;
{
	return [self copyDataOfLength:length error:error];
}
-(NSData *)readDataOfLengthAtMost:(NSInteger)length error:(NSError**)error;
{
	return [self copyDataOfLengthAtMost:length error:error];
}

-(NSData *)copyDataOfLength:(NSInteger)length error:(NSError**)error;
{
	NSMutableData *data=[NSMutableData dataWithLength:length];
	if (!data) {
		if (error) {
			*error = [NSError errorWithDomain:NSPOSIXErrorDomain code:ENOMEM userInfo:nil];
		}
		return nil;
	}
	if (![self readBytes:length toBuffer:[data mutableBytes] error:error]) {
		return nil;
	}
	return [data copy];
}

-(NSData *)copyDataOfLengthAtMost:(NSInteger)length error:(NSError**)error;
{
	NSMutableData *data=[NSMutableData dataWithLength:length];
	if(!data) {
		if (error) {
			*error = [NSError errorWithDomain:NSPOSIXErrorDomain code:ENOMEM userInfo:nil];
		}
		return nil;
	}
	ssize_t actual = 0;
	BOOL success = [self readAtMost:length toBuffer:[data mutableBytes] totalWritten:&actual error:error];
	if (!success) {
		return nil;
	}
	[data setLength:actual];
	return [data copy];

}

-(BOOL)readBytes:(size_t)num toBuffer:(void *)buffer error:(NSError**)error;
{
	ssize_t actual = 0;
	BOOL success = [self readAtMost:num toBuffer:buffer totalWritten:&actual error:error];
	if (actual != num) {
		if (success) {
			if (error) {
				*error = [NSError errorWithDomain:NSOSStatusErrorDomain code:eofErr userInfo:nil];
			}
		}
		return NO;
	}
	return YES;
}

-(off_t)readAndDiscardAtMost:(off_t)num error:(NSError**)error;
{
	off_t skipped=0;
	uint8_t buf[16384];
	while (skipped < num) {
		off_t numbytes = MIN(num - skipped, sizeof(buf));
		ssize_t actual;
		BOOL success = [self readAtMost:numbytes toBuffer:buf totalWritten:&actual error:error];
		if (!success) {
			return 0;
		}
		skipped+=actual;
		if(actual==0) break;
	}
	return skipped;
}

-(BOOL)readAndDiscardBytes:(off_t)num error:(NSError**)error;
{
	NSError *intErr = nil;
	off_t size = [self readAndDiscardAtMost:num error:&intErr];
	if (size == 0 && intErr) {
		if (error) {
			*error = intErr;
		}
		return NO;
	}
	
	if (size != num) {
		if (error) {
			*error = [NSError errorWithDomain:NSOSStatusErrorDomain code:eofErr userInfo:nil];
		}
		return NO;
	}
	
	return YES;
}

-(CSHandle *)subHandleOfLength:(off_t)length error:(NSError**)error;
{
	return [[CSSubHandle alloc] initWithHandle:[self copy] from:[self offsetInFile] length:length error:error];
}

-(CSHandle *)subHandleFrom:(off_t)start length:(off_t)length error:(NSError**)error;
{
	return [[CSSubHandle alloc] initWithHandle:[self copy] from:start length:length error:error];
}

-(CSHandle *)subHandleToEndOfFileFrom:(off_t)start error:(NSError**)error;
{
	off_t size=[self fileSize];
	if (size == CSHandleMaxLength) {
        return [[CSSubHandle alloc] initWithHandle:[self copy]
                                              from:start length:CSHandleMaxLength
                                             error:error];
	} else {
        return [[CSSubHandle alloc] initWithHandle:[self copy]
                                              from:start length:size-start error:error];
	}
}

-(CSHandle *)nonCopiedSubHandleOfLength:(off_t)length error:(NSError**)error;
{
	return [[CSSubHandle alloc] initWithHandle:self from:[self offsetInFile] length:length error:error];
}
-(CSHandle *)nonCopiedSubHandleFrom:(off_t)start length:(off_t)length error:(NSError**)error;
{
	return [[CSSubHandle alloc] initWithHandle:self from:start length:length error:error];
}
-(CSHandle *)nonCopiedSubHandleToEndOfFileFrom:(off_t)start error:(NSError**)error;
{
	off_t size=[self fileSize];
	if(size==CSHandleMaxLength)
	{
        return [[CSSubHandle alloc] initWithHandle:self
                                              from:start length:CSHandleMaxLength error:error];
	}
	else
	{
        return [[CSSubHandle alloc] initWithHandle:self
                                              from:start length:size-start error:error];
	}
}

-(BOOL)writeInt8:(int8_t)val error:(NSError**)error;
{
	return [self writeBytes:1 fromBuffer:&val error:error];
}

-(BOOL)writeUInt8:(uint8_t)val error:(NSError**)error;
{
	return [self writeBytes:1 fromBuffer:&val error:error];
}

-(BOOL)writeBits:(int)bits value:(uint32_t)val error:(NSError**)error;
{
	int bitsleft=bits;
	while (bitsleft) {
		if (!writebitsleft) {
			if (![self writeUInt8:writebyte error:error])
				return NO;
			writebyte=0;
			writebitsleft=8;
		}
		
		int num=bitsleft;
		if(num>writebitsleft) num=writebitsleft;
		writebyte|=((val>>(bitsleft-num))&((1<<num)-1))<<(writebitsleft-num);
		
		bitsleft-=num;
		writebitsleft-=num;
	}
	return YES;
}

-(BOOL)writeSignedBits:(int)bits value:(int32_t)val error:(NSError**)error;
{
	return [self writeBits:bits value:val error:error];

}
-(BOOL)flushWriteBitsWithError:(NSError**)error;
{
	BOOL goodData = YES;
	if (writebitsleft!=8) {
		goodData = [self writeUInt8:writebyte error:error];
	}
	if (!goodData) {
		return NO;
	}
	writebyte=0;
	writebitsleft=8;

	return goodData;
}

-(BOOL)writeData:(NSData *)data error:(NSError**)error;
{
	return [self writeBytes:(int)[data length] fromBuffer:[data bytes] error:error];
}


@end
