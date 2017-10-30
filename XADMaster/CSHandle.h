#import <Foundation/Foundation.h>
#include <stdint.h>

//DEPRECATED_ATTRIBUTE

#define XAD_DEPRECATED_THROW DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("Call can throw exception") UNAVAILABLE_ATTRIBUTE

#define CSHandleMaxLength 0x7fffffffffffffffll
#define CSHandle XADHandle


// Kludge 64-bit support for Mingw. TODO: Should this be used on Linux too?
#if defined(__MINGW32__) && !defined(__CYGWIN__)
#include <unistd.h>
#include <fcntl.h>
#define off_t off64_t
#define fseeko fseeko64
#define lseek lseek64
#define ftello ftello64
#endif


extern NSString *const CSOutOfMemoryException;
extern NSString *const CSEndOfFileException;
extern NSString *const CSNotImplementedException;
extern NSString *const CSNotSupportedException;



@interface CSHandle:NSObject <NSCopying>
{
	NSString *name;
	off_t bitoffs;
	uint8_t readbyte,readbitsleft;
	uint8_t writebyte,writebitsleft;
}

-(instancetype)initWithName:(NSString *)descname;
-(instancetype)initAsCopyOf:(CSHandle *)other;
-(void)close;


// Methods implemented by subclasses

@property (NS_NONATOMIC_IOSONLY, readonly) off_t fileSize;
@property (NS_NONATOMIC_IOSONLY, readonly) off_t offsetInFile;
@property (NS_NONATOMIC_IOSONLY, readonly) BOOL atEndOfFile;

-(void)seekToFileOffset:(off_t)offs XAD_DEPRECATED_THROW;
-(void)seekToEndOfFile XAD_DEPRECATED_THROW;
-(void)pushBackByte:(int)byte XAD_DEPRECATED_THROW;
-(int)readAtMost:(int)num toBuffer:(void *)buffer XAD_DEPRECATED_THROW;
-(void)writeBytes:(int)num fromBuffer:(const void *)buffer XAD_DEPRECATED_THROW;

-(BOOL)seekToFileOffset:(off_t)offs error:(NSError**)error;
-(BOOL)seekToEndOfFileWithError:(NSError**)error;
-(BOOL)pushBackByte:(uint8_t)byte error:(NSError**)error;
-(BOOL)readAtMost:(size_t)num toBuffer:(void *)buffer totalWritten:(ssize_t*)tw error:(NSError**)error;
-(BOOL)writeBytes:(size_t)num fromBuffer:(const void *)buffer error:(NSError**)error;


// Utility methods
-(void)_raiseMemory NS_SWIFT_UNAVAILABLE("Call throws exception");
-(void)_raiseEOF NS_SWIFT_UNAVAILABLE("Call throws exception");
-(void)_raiseNotImplemented:(SEL)selector NS_SWIFT_UNAVAILABLE("Call throws exception");
-(void)_raiseNotSupported:(SEL)selector NS_SWIFT_UNAVAILABLE("Call throws exception");

@property (readonly, copy) NSString *name;
@property (readonly, copy) NSString *description;

-(BOOL)skipBytes:(off_t)bytes error:(NSError**)error;

-(int8_t)readInt8WithError:(NSError**)error NS_REFINED_FOR_SWIFT;
-(uint8_t)readUInt8WithError:(NSError**)error NS_REFINED_FOR_SWIFT;

-(int16_t)readInt16BEWithError:(NSError**)error NS_REFINED_FOR_SWIFT;
-(int32_t)readInt32BEWithError:(NSError**)error NS_REFINED_FOR_SWIFT;
-(int64_t)readInt64BEWithError:(NSError**)error NS_REFINED_FOR_SWIFT;
-(uint16_t)readUInt16BEWithError:(NSError**)error NS_REFINED_FOR_SWIFT;
-(uint32_t)readUInt32BEWithError:(NSError**)error NS_REFINED_FOR_SWIFT;
-(uint64_t)readUInt64BEWithError:(NSError**)error NS_REFINED_FOR_SWIFT;

-(int16_t)readInt16LEWithError:(NSError**)error NS_REFINED_FOR_SWIFT;
-(int32_t)readInt32LEWithError:(NSError**)error NS_REFINED_FOR_SWIFT;
-(int64_t)readInt64LEWithError:(NSError**)error NS_REFINED_FOR_SWIFT;
-(uint16_t)readUInt16LEWithError:(NSError**)error NS_REFINED_FOR_SWIFT;
-(uint32_t)readUInt32LEWithError:(NSError**)error NS_REFINED_FOR_SWIFT;
-(uint64_t)readUInt64LEWithError:(NSError**)error NS_REFINED_FOR_SWIFT;

-(int16_t)readInt16InBigEndianOrder:(BOOL)isbigendian error:(NSError**)error;
-(int32_t)readInt32InBigEndianOrder:(BOOL)isbigendian error:(NSError**)error;
-(int64_t)readInt64InBigEndianOrder:(BOOL)isbigendian error:(NSError**)error;
-(uint16_t)readUInt16InBigEndianOrder:(BOOL)isbigendian error:(NSError**)error;
-(uint32_t)readUInt32InBigEndianOrder:(BOOL)isbigendian error:(NSError**)error;
-(uint64_t)readUInt64InBigEndianOrder:(BOOL)isbigendian error:(NSError**)error;

-(uint32_t)readIDWithError:(NSError**)error NS_REFINED_FOR_SWIFT;

-(uint32_t)readBits:(int)bits error:(NSError**)error NS_REFINED_FOR_SWIFT;
-(uint32_t)readBitsLE:(int)bits error:(NSError**)error NS_REFINED_FOR_SWIFT;
-(int32_t)readSignedBits:(int)bits error:(NSError**)error NS_REFINED_FOR_SWIFT;
-(int32_t)readSignedBitsLE:(int)bits error:(NSError**)error NS_REFINED_FOR_SWIFT;
-(void)flushReadBits;

-(NSData *)readLineWithError:(NSError**)error;
-(NSString *)readLineWithEncoding:(NSStringEncoding)encoding error:(NSError**)error;
-(NSString *)readUTF8LineWithError:(NSError**)error;

-(NSData *)fileContentsWithError:(NSError**)error;
-(NSData *)remainingFileContentsWithError:(NSError**)error;
-(NSData *)readDataOfLength:(NSInteger)length error:(NSError**)error;
-(NSData *)readDataOfLengthAtMost:(NSInteger)length error:(NSError**)error;
-(NSData *)copyDataOfLength:(NSInteger)length error:(NSError**)error NS_RETURNS_RETAINED;
-(NSData *)copyDataOfLengthAtMost:(NSInteger)length error:(NSError**)error NS_RETURNS_RETAINED;
-(BOOL)readBytes:(size_t)num toBuffer:(void *)buffer error:(NSError**)error;

-(off_t)readAndDiscardAtMost:(off_t)num error:(NSError**)error;
-(BOOL)readAndDiscardBytes:(off_t)num error:(NSError**)error;

-(CSHandle *)subHandleOfLength:(off_t)length error:(NSError**)error;
-(CSHandle *)subHandleFrom:(off_t)start length:(off_t)length error:(NSError**)error;
-(CSHandle *)subHandleToEndOfFileFrom:(off_t)start error:(NSError**)error;
-(CSHandle *)nonCopiedSubHandleOfLength:(off_t)length error:(NSError**)error;
-(CSHandle *)nonCopiedSubHandleFrom:(off_t)start length:(off_t)length error:(NSError**)error;
-(CSHandle *)nonCopiedSubHandleToEndOfFileFrom:(off_t)start error:(NSError**)error;

-(BOOL)writeInt8:(int8_t)val error:(NSError**)error;
-(BOOL)writeUInt8:(uint8_t)val error:(NSError**)error;

-(BOOL)writeInt16BE:(int16_t)val error:(NSError**)error;
-(BOOL)writeInt32BE:(int32_t)val error:(NSError**)error;
//-(void)writeInt64BE:(int64_t)val;
-(BOOL)writeUInt16BE:(uint16_t)val error:(NSError**)error;
-(BOOL)writeUInt32BE:(uint32_t)val error:(NSError**)error;
//-(void)writeUInt64BE:(uint64_t)val;

-(BOOL)writeInt16LE:(int16_t)val error:(NSError**)error;
-(BOOL)writeInt32LE:(int32_t)val error:(NSError**)error;
//-(void)writeInt64LE:(int64_t)val error:(NSError**)error;
-(BOOL)writeUInt16LE:(uint16_t)val error:(NSError**)error;
-(BOOL)writeUInt32LE:(uint32_t)val error:(NSError**)error;
//-(void)writeUInt64LE:(uint64_t)val error:(NSError**)error;

-(BOOL)writeID:(uint32_t)val error:(NSError**)error;

-(BOOL)writeBits:(int)bits value:(uint32_t)val error:(NSError**)error;
-(BOOL)writeSignedBits:(int)bits value:(int32_t)val error:(NSError**)error;
-(BOOL)flushWriteBitsWithError:(NSError**)error;

-(BOOL)writeData:(NSData *)data error:(NSError**)error;

@end

static inline int16_t CSInt16BE(const uint8_t *b) { return ((int16_t)b[0]<<8)|(int16_t)b[1]; }
static inline int32_t CSInt32BE(const uint8_t *b) { return ((int32_t)b[0]<<24)|((int32_t)b[1]<<16)|((int32_t)b[2]<<8)|(int32_t)b[3]; }
static inline int64_t CSInt64BE(const uint8_t *b) { return ((int64_t)b[0]<<56)|((int64_t)b[1]<<48)|((int64_t)b[2]<<40)|((int64_t)b[3]<<32)|((int64_t)b[4]<<24)|((int64_t)b[5]<<16)|((int64_t)b[6]<<8)|(int64_t)b[7]; }
static inline uint16_t CSUInt16BE(const uint8_t *b) { return ((uint16_t)b[0]<<8)|(uint16_t)b[1]; }
static inline uint32_t CSUInt32BE(const uint8_t *b) { return ((uint32_t)b[0]<<24)|((uint32_t)b[1]<<16)|((uint32_t)b[2]<<8)|(uint32_t)b[3]; }
static inline uint64_t CSUInt64BE(const uint8_t *b) { return ((uint64_t)b[0]<<56)|((uint64_t)b[1]<<48)|((uint64_t)b[2]<<40)|((uint64_t)b[3]<<32)|((uint64_t)b[4]<<24)|((uint64_t)b[5]<<16)|((uint64_t)b[6]<<8)|(uint64_t)b[7]; }
static inline int16_t CSInt16LE(const uint8_t *b) { return ((int16_t)b[1]<<8)|(int16_t)b[0]; }
static inline int32_t CSInt32LE(const uint8_t *b) { return ((int32_t)b[3]<<24)|((int32_t)b[2]<<16)|((int32_t)b[1]<<8)|(int32_t)b[0]; }
static inline int64_t CSInt64LE(const uint8_t *b) { return ((int64_t)b[7]<<56)|((int64_t)b[6]<<48)|((int64_t)b[5]<<40)|((int64_t)b[4]<<32)|((int64_t)b[3]<<24)|((int64_t)b[2]<<16)|((int64_t)b[1]<<8)|(int64_t)b[0]; }
static inline uint16_t CSUInt16LE(const uint8_t *b) { return ((uint16_t)b[1]<<8)|(uint16_t)b[0]; }
static inline uint32_t CSUInt32LE(const uint8_t *b) { return ((uint32_t)b[3]<<24)|((uint32_t)b[2]<<16)|((uint32_t)b[1]<<8)|(uint32_t)b[0]; }
static inline uint64_t CSUInt64LE(const uint8_t *b) { return ((uint64_t)b[7]<<56)|((uint64_t)b[6]<<48)|((uint64_t)b[5]<<40)|((uint64_t)b[4]<<32)|((uint64_t)b[3]<<24)|((uint64_t)b[2]<<16)|((uint64_t)b[1]<<8)|(uint64_t)b[0]; }

static inline void CSSetInt16BE(uint8_t *b,int16_t n) { b[0]=(n>>8)&0xff; b[1]=n&0xff; }
static inline void CSSetInt32BE(uint8_t *b,int32_t n) { b[0]=(n>>24)&0xff; b[1]=(n>>16)&0xff; b[2]=(n>>8)&0xff; b[3]=n&0xff; }
static inline void CSSetUInt16BE(uint8_t *b,uint16_t n) { b[0]=(n>>8)&0xff; b[1]=n&0xff; }
static inline void CSSetUInt32BE(uint8_t *b,uint32_t n) { b[0]=(n>>24)&0xff; b[1]=(n>>16)&0xff; b[2]=(n>>8)&0xff; b[3]=n&0xff; }
static inline void CSSetInt16LE(uint8_t *b,int16_t n) { b[1]=(n>>8)&0xff; b[0]=n&0xff; }
static inline void CSSetInt32LE(uint8_t *b,int32_t n) { b[3]=(n>>24)&0xff; b[2]=(n>>16)&0xff; b[1]=(n>>8)&0xff; b[0]=n&0xff; }
static inline void CSSetUInt16LE(uint8_t *b,uint16_t n) { b[1]=(n>>8)&0xff; b[0]=n&0xff; }
static inline void CSSetUInt32LE(uint8_t *b,uint32_t n) { b[3]=(n>>24)&0xff; b[2]=(n>>16)&0xff; b[1]=(n>>8)&0xff; b[0]=n&0xff; }

