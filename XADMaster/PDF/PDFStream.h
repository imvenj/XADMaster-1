#import <Foundation/Foundation.h>

#import "NSDictionaryNumberExtension.h"

#import "../CSHandle.h"
#import "../CSByteStreamHandle.h"

typedef NS_ENUM(int, PDFImageType) {
	PDFUnsupportedImageType = 0,
	PDFIndexedImageType  = 1,
	PDFGrayImageType = 2,
	PDFRGBImageType = 3,
	PDFCMYKImageType = 4,
	PDFLabImageType = 5,
	PDFSeparationImageType = 6,
	PDFMaskImageType = 7
};

@class PDFParser,PDFObjectReference;

@interface PDFStream:NSObject
{
	NSDictionary *dict;
	CSHandle *fh;
	off_t offs;
	PDFObjectReference *ref;
	PDFParser *parser;
}

-(instancetype)initWithDictionary:(NSDictionary *)dictionary fileHandle:(CSHandle *)filehandle
offset:(off_t)offset reference:(PDFObjectReference *)reference parser:(PDFParser *)owner;

@property (NS_NONATOMIC_IOSONLY, readonly, retain) NSDictionary *dictionary;
@property (NS_NONATOMIC_IOSONLY, readonly, retain) PDFObjectReference *reference;

@property (NS_NONATOMIC_IOSONLY, readonly, getter=isImage) BOOL image;
@property (NS_NONATOMIC_IOSONLY, readonly, getter=isJPEGImage) BOOL JPEGImage;
@property (NS_NONATOMIC_IOSONLY, readonly, getter=isJPEG2000Image) BOOL JPEG2000Image;

@property (NS_NONATOMIC_IOSONLY, readonly) int imageWidth;
@property (NS_NONATOMIC_IOSONLY, readonly) int imageHeight;
@property (NS_NONATOMIC_IOSONLY, readonly) int imageBitsPerComponent;

@property (NS_NONATOMIC_IOSONLY, readonly) PDFImageType imageType;
@property (NS_NONATOMIC_IOSONLY, readonly) NSInteger numberOfImageComponents;
-(NSString *)imageColourSpaceName;

@property (NS_NONATOMIC_IOSONLY, readonly) PDFImageType imagePaletteType;
@property (NS_NONATOMIC_IOSONLY, readonly) NSInteger numberOfImagePaletteComponents;
-(NSString *)imagePaletteColourSpaceName;
@property (NS_NONATOMIC_IOSONLY, readonly) NSInteger numberOfImagePaletteColours;
-(NSData *)imagePaletteData;
-(id)_paletteColourSpaceObject;

-(PDFImageType)_typeForColourSpaceObject:(id)colourspace;
-(NSInteger)_numberOfComponentsForColourSpaceObject:(id)colourspace;
-(NSString *)_nameForColourSpaceObject:(id)colourspace;

-(NSData *)imageICCColourProfile;
-(NSData *)_ICCColourProfileForColourSpaceObject:(id)colourspace;

-(NSString *)imageSeparationName;
-(NSArray *)imageDecodeArray;

@property (NS_NONATOMIC_IOSONLY, readonly) BOOL hasMultipleFilters;
-(NSString *)finalFilter;

-(CSHandle *)rawHandle;
-(CSHandle *)handle;
-(CSHandle *)JPEGHandle;
-(CSHandle *)handleExcludingLast:(BOOL)excludelast;
-(CSHandle *)handleExcludingLast:(BOOL)excludelast decrypted:(BOOL)decrypted;
-(CSHandle *)handleForFilterName:(NSString *)filtername decodeParms:(NSDictionary *)decodeparms parentHandle:(CSHandle *)parent;
-(CSHandle *)predictorHandleForDecodeParms:(NSDictionary *)decodeparms parentHandle:(CSHandle *)parent;

-(NSString *)description;

@end

@interface PDFASCII85Handle:CSByteStreamHandle
{
	uint32_t val;
	BOOL finalbytes;
}

-(void)resetByteStream;
-(uint8_t)produceByteAtOffset:(off_t)pos;

@end

@interface PDFHexHandle:CSByteStreamHandle

-(uint8_t)produceByteAtOffset:(off_t)pos;

@end




@interface PDFTIFFPredictorHandle:CSByteStreamHandle
{
	int cols,comps,bpc;
	int prev[4];
}

-(instancetype)initWithHandle:(CSHandle *)handle columns:(int)columns
components:(int)components bitsPerComponent:(int)bitspercomp;
-(uint8_t)produceByteAtOffset:(off_t)pos;

@end

@interface PDFPNGPredictorHandle:CSByteStreamHandle
{
	int cols,comps,bpc;
	uint8_t *prevbuf;
	int type;
}

-(instancetype)initWithHandle:(CSHandle *)handle columns:(int)columns
components:(int)components bitsPerComponent:(int)bitspercomp;
-(void)resetByteStream;
-(uint8_t)produceByteAtOffset:(off_t)pos;

@end

