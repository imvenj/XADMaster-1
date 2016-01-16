#import <Foundation/Foundation.h>

#ifdef _WIN32
#import "regex.h"
#else
#import <regex.h>
#endif

NS_ASSUME_NONNULL_BEGIN

@interface XADRegex:NSObject
{
	NSString *patternstring;
	regex_t preg;
	regmatch_t *matches;
	NSRange matchrange;
	NSData *currdata;
}

+(nullable instancetype)regexWithPattern:(NSString *)pattern options:(int)options;
+(nullable instancetype)regexWithPattern:(NSString *)pattern;

+(NSString *)patternForLiteralString:(NSString *)string;
+(NSString *)patternForGlob:(NSString *)glob;

+(NSString *)null;

-(nullable instancetype)initWithPattern:(NSString *)pattern options:(int)options NS_DESIGNATED_INITIALIZER;

-(void)beginMatchingString:(NSString *)string;
//-(void)beginMatchingString:(NSString *)string range:(NSRange)range;
-(void)beginMatchingData:(NSData *)data;
-(void)beginMatchingData:(NSData *)data range:(NSRange)range;
-(void)finishMatching;
-(BOOL)matchNext;
-(nullable NSString *)stringForMatch:(NSInteger)n;
@property (NS_NONATOMIC_IOSONLY, readonly, copy, nullable) NSArray<NSString*> *allMatches;

-(BOOL)matchesString:(NSString *)string;
-(nullable NSString *)matchedSubstringOfString:(NSString *)string;
-(nullable NSArray<NSString*> *)capturedSubstringsOfString:(NSString *)string;
-(nullable NSArray<NSString*> *)allMatchedSubstringsOfString:(NSString *)string;
-(nullable NSArray<NSString*> *)allCapturedSubstringsOfString:(NSString *)string;
-(nullable NSArray<NSString*> *)componentsOfSeparatedString:(NSString *)string;

/*
-(NSString *)expandReplacementString:(NSString *)replacement;
*/

@property (NS_NONATOMIC_IOSONLY, readonly, copy) NSString *pattern;
@property (NS_NONATOMIC_IOSONLY, readonly, copy) NSString *description;

@end

@interface NSString (XADRegex)

-(BOOL)matchedByPattern:(NSString *)pattern;
-(BOOL)matchedByPattern:(NSString *)pattern options:(int)options;

-(nullable NSString *)substringMatchedByPattern:(NSString *)pattern;
-(nullable NSString *)substringMatchedByPattern:(NSString *)pattern options:(int)options;

-(nullable NSArray<NSString*> *)substringsCapturedByPattern:(NSString *)pattern;
-(nullable NSArray<NSString*> *)substringsCapturedByPattern:(NSString *)pattern options:(int)options;

-(nullable NSArray<NSString*> *)allSubstringsMatchedByPattern:(NSString *)pattern;
-(nullable NSArray<NSString*> *)allSubstringsMatchedByPattern:(NSString *)pattern options:(int)options;

-(nullable NSArray<NSString*> *)allSubstringsCapturedByPattern:(NSString *)pattern;
-(nullable NSArray<NSString*> *)allSubstringsCapturedByPattern:(NSString *)pattern options:(int)options;

-(nullable NSArray<NSString*> *)componentsSeparatedByPattern:(NSString *)pattern;
-(nullable NSArray<NSString*> *)componentsSeparatedByPattern:(NSString *)pattern options:(int)options;

@property (NS_NONATOMIC_IOSONLY, readonly, copy) NSString *escapedPattern;

@end

NS_ASSUME_NONNULL_END

/*@interface NSMutableString (XADRegex)

-(void)replacePattern:(NSString *)pattern with:(NSString *)replacement;
-(void)replacePattern:(NSString *)pattern with:(NSString *)replacement options:(int)options;
-(void)replacePattern:(NSString *)pattern usingSelector:(SEL)selector onObject:(id)object;
-(void)replacePattern:(NSString *)pattern usingSelector:(SEL)selector onObject:(id)object options:(int)options;
-(void)replaceEveryPattern:(NSString *)pattern with:(NSString *)replacement;
-(void)replaceEveryPattern:(NSString *)pattern with:(NSString *)replacement options:(int)options;
-(void)replaceEveryPattern:(NSString *)pattern usingSelector:(SEL)selector onObject:(id)object;
-(void)replaceEveryPattern:(NSString *)pattern usingSelector:(SEL)selector onObject:(id)object options:(int)options;

@end*/