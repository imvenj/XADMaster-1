#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class XADStringSource, UniversalDetector;

//! The supported encodings used by \c XADString
typedef NSString *XADStringEncodingName NS_STRING_ENUM;

extern XADStringEncodingName const XADUTF8StringEncodingName;
extern XADStringEncodingName const XADASCIIStringEncodingName;

extern XADStringEncodingName const XADISOLatin1StringEncodingName;
extern XADStringEncodingName const XADISOLatin2StringEncodingName;
extern XADStringEncodingName const XADISOLatin3StringEncodingName;
extern XADStringEncodingName const XADISOLatin4StringEncodingName;
extern XADStringEncodingName const XADISOLatin5StringEncodingName;
extern XADStringEncodingName const XADISOLatin6StringEncodingName;
extern XADStringEncodingName const XADISOLatin7StringEncodingName;
extern XADStringEncodingName const XADISOLatin8StringEncodingName;
extern XADStringEncodingName const XADISOLatin9StringEncodingName;
extern XADStringEncodingName const XADISOLatin10StringEncodingName;
extern XADStringEncodingName const XADISOLatin11StringEncodingName;
extern XADStringEncodingName const XADISOLatin12StringEncodingName;
extern XADStringEncodingName const XADISOLatin13StringEncodingName;
extern XADStringEncodingName const XADISOLatin14StringEncodingName;
extern XADStringEncodingName const XADISOLatin15StringEncodingName;
extern XADStringEncodingName const XADISOLatin16StringEncodingName;

extern XADStringEncodingName const XADShiftJISStringEncodingName;

extern XADStringEncodingName const XADWindowsCP1250StringEncodingName;
extern XADStringEncodingName const XADWindowsCP1251StringEncodingName;
extern XADStringEncodingName const XADWindowsCP1252StringEncodingName;
extern XADStringEncodingName const XADWindowsCP1253StringEncodingName;
extern XADStringEncodingName const XADWindowsCP1254StringEncodingName;

extern XADStringEncodingName const XADMacOSRomanStringEncodingName;
extern XADStringEncodingName const XADMacOSJapaneseStringEncodingName;
extern XADStringEncodingName const XADMacOSTraditionalChineseStringEncodingName;
extern XADStringEncodingName const XADMacOSKoreanStringEncodingName;
extern XADStringEncodingName const XADMacOSArabicStringEncodingName;
extern XADStringEncodingName const XADMacOSHebrewStringEncodingName;
extern XADStringEncodingName const XADMacOSGreekStringEncodingName;
extern XADStringEncodingName const XADMacOSCyrillicStringEncodingName;
extern XADStringEncodingName const XADMacOSSimplifiedChineseStringEncodingName;
extern XADStringEncodingName const XADMacOSRomanianStringEncodingName;
extern XADStringEncodingName const XADMacOSUkranianStringEncodingName;
extern XADStringEncodingName const XADMacOSThaiStringEncodingName;
extern XADStringEncodingName const XADMacOSCentralEuropeanRomanStringEncodingName;
extern XADStringEncodingName const XADMacOSIcelandicStringEncodingName;
extern XADStringEncodingName const XADMacOSTurkishStringEncodingName;
extern XADStringEncodingName const XADMacOSCroatianStringEncodingName;


@protocol XADString <NSObject>

-(BOOL)canDecodeWithEncodingName:(XADStringEncodingName)encoding;
@property (NS_NONATOMIC_IOSONLY, readonly, copy, nullable) NSString *string;
-(nullable NSString *)stringWithEncodingName:(XADStringEncodingName)encoding;
@property (NS_NONATOMIC_IOSONLY, readonly, copy, nullable) NSData *data;

@property (NS_NONATOMIC_IOSONLY, readonly) BOOL encodingIsKnown;
@property (NS_NONATOMIC_IOSONLY, readonly, copy) XADStringEncodingName encodingName;
@property (NS_NONATOMIC_IOSONLY, readonly) float confidence;

@property (NS_NONATOMIC_IOSONLY, readonly, retain, nullable) XADStringSource *source;

#ifdef __APPLE__
-(BOOL)canDecodeWithEncoding:(NSStringEncoding)encoding;
-(NSString *)stringWithEncoding:(NSStringEncoding)encoding;
@property (NS_NONATOMIC_IOSONLY, readonly) NSStringEncoding encoding;
#endif

@end



@interface XADString:NSObject <XADString,NSCopying>
{
	NSData *data;
	NSString *string;
	XADStringSource *source;
}

+(instancetype)XADStringWithString:(NSString *)string;
+(instancetype)analyzedXADStringWithData:(NSData *)bytedata source:(XADStringSource *)stringsource;
+(nullable instancetype)decodedXADStringWithData:(NSData *)bytedata encodingName:(XADStringEncodingName)encoding;

+(NSString *)escapedStringForData:(NSData *)data encodingName:(XADStringEncodingName)encoding;
+(NSString *)escapedStringForBytes:(const void *)bytes length:(size_t)length encodingName:(XADStringEncodingName)encoding;
+(NSString *)escapedASCIIStringForBytes:(const void *)bytes length:(size_t)length;
+(NSData *)escapedASCIIDataForString:(NSString *)string;

-(instancetype)init UNAVAILABLE_ATTRIBUTE;
-(instancetype)initWithData:(NSData *)bytedata source:(XADStringSource *)stringsource NS_DESIGNATED_INITIALIZER;
-(instancetype)initWithString:(NSString *)knownstring NS_DESIGNATED_INITIALIZER;

-(BOOL)canDecodeWithEncodingName:(XADStringEncodingName)encoding;
@property (NS_NONATOMIC_IOSONLY, readonly, copy, nullable) NSString *string;
-(nullable NSString *)stringWithEncodingName:(XADStringEncodingName)encoding;
@property (NS_NONATOMIC_IOSONLY, readonly, copy, nullable) NSData *data;

@property (NS_NONATOMIC_IOSONLY, readonly) BOOL encodingIsKnown;
@property (NS_NONATOMIC_IOSONLY, readonly, copy) XADStringEncodingName encodingName;
@property (NS_NONATOMIC_IOSONLY, readonly) float confidence;

@property (NS_NONATOMIC_IOSONLY, readonly, retain, nullable) XADStringSource *source;

-(BOOL)hasASCIIPrefix:(NSString *)asciiprefix;
-(XADString *)XADStringByStrippingASCIIPrefixOfLength:(NSInteger)length;

#ifdef __APPLE__
-(BOOL)canDecodeWithEncoding:(NSStringEncoding)encoding;
-(NSString *)stringWithEncoding:(NSStringEncoding)encoding;
@property (NS_NONATOMIC_IOSONLY, readonly) NSStringEncoding encoding;
#endif

@end

@interface XADString (PlatformSpecific)

+(BOOL)canDecodeData:(NSData *)data encodingName:(XADStringEncodingName)encoding;
+(BOOL)canDecodeBytes:(const void *)bytes length:(size_t)length encodingName:(XADStringEncodingName)encoding;
+(nullable NSString *)stringForData:(NSData *)data encodingName:(XADStringEncodingName)encoding;
+(nullable NSString *)stringForBytes:(const void *)bytes length:(size_t)length encodingName:(XADStringEncodingName)encoding;
+(nullable NSData *)dataForString:(NSString *)string encodingName:(XADStringEncodingName)encoding;
+(NSArray<NSString*> *)availableEncodingNames;

#ifdef __APPLE__
+(XADStringEncodingName)encodingNameForEncoding:(NSStringEncoding)encoding;
+(NSStringEncoding)encodingForEncodingName:(XADStringEncodingName)encoding;
#endif

@end




@interface XADStringSource:NSObject
{
	UniversalDetector *detector;
	XADStringEncodingName fixedencodingname;
	BOOL mac,hasanalyzeddata;

	#ifdef __APPLE__
	NSStringEncoding fixedencoding;
	#endif
}

-(instancetype)init NS_DESIGNATED_INITIALIZER;

-(void)analyzeData:(NSData *)data;

@property (NS_NONATOMIC_IOSONLY, readonly) BOOL hasAnalyzedData;
@property (NS_NONATOMIC_IOSONLY, readonly, copy) XADStringEncodingName encodingName;
@property (NS_NONATOMIC_IOSONLY, readonly) float confidence;
@property (NS_NONATOMIC_IOSONLY, readonly, retain, nullable) UniversalDetector *detector;

@property (NS_NONATOMIC_IOSONLY, readwrite, copy, nullable) XADStringEncodingName fixedEncodingName;
@property (NS_NONATOMIC_IOSONLY, readonly) BOOL hasFixedEncoding;
@property (NS_NONATOMIC_IOSONLY, readwrite) BOOL prefersMacEncodings;

#ifdef __APPLE__
@property (NS_NONATOMIC_IOSONLY, readonly) NSStringEncoding encoding;
@property (NS_NONATOMIC_IOSONLY) NSStringEncoding fixedEncoding;
#endif

@end

NS_ASSUME_NONNULL_END
