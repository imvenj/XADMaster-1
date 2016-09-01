#import <Foundation/Foundation.h>

#import "XADArchiveParser.h"

typedef NS_ENUM(int, XADForkStyle) {
	XADIgnoredForkStyle = 0,
	XADMacOSXForkStyle = 1,
	XADHiddenAppleDoubleForkStyle = 2,
	XADVisibleAppleDoubleForkStyle = 3,
	XADHFVExplorerAppleDoubleForkStyle = 4,
	
#ifdef __APPLE__
	XADDefaultForkStyle = XADMacOSXForkStyle,
#else
	XADDefaultForkStyle = XADVisibleAppleDoubleForkStyle,
#endif
};

NS_ASSUME_NONNULL_BEGIN

@protocol XADUnarchiverDelegate;

@interface XADUnarchiver:NSObject <XADArchiveParserDelegate>
{
	XADArchiveParser *parser;
	BOOL preservepermissions;

	BOOL shouldstop;

	NSMutableArray *deferreddirectories,*deferredlinks;
}

+(nullable instancetype)unarchiverForArchiveParser:(XADArchiveParser *)archiveparser;
+(nullable instancetype)unarchiverForPath:(NSString *)path NS_SWIFT_UNAVAILABLE("Call may throw exceptions, use init(for:error:) instead");
+(nullable instancetype)unarchiverForPath:(NSString *)path error:(nullable XADError *)errorptr;

-(instancetype)init UNAVAILABLE_ATTRIBUTE;
-(instancetype)initWithArchiveParser:(XADArchiveParser *)archiveparser NS_DESIGNATED_INITIALIZER;

@property (NS_NONATOMIC_IOSONLY, readonly, strong) XADArchiveParser *archiveParser;

@property (NS_NONATOMIC_IOSONLY, assign, nullable) id<XADUnarchiverDelegate> delegate;

@property (NS_NONATOMIC_IOSONLY, copy) NSString *destination;

@property (NS_NONATOMIC_IOSONLY) XADForkStyle macResourceForkStyle;

@property (NS_NONATOMIC_IOSONLY, setter=setPreserevesPermissions:) BOOL preservesPermissions;

@property (NS_NONATOMIC_IOSONLY) double updateInterval;

-(XADError)parseAndUnarchive;

-(XADError)extractEntryWithDictionary:(NSDictionary *)dict;
-(XADError)extractEntryWithDictionary:(NSDictionary *)dict forceDirectories:(BOOL)force;
-(XADError)extractEntryWithDictionary:(NSDictionary *)dict as:(nullable NSString *)path;
-(XADError)extractEntryWithDictionary:(NSDictionary *)dict as:(nullable NSString *)path forceDirectories:(BOOL)force;

-(XADError)finishExtractions;
-(XADError)_fixDeferredLinks;
-(XADError)_fixDeferredDirectories;

-(XADUnarchiver *)unarchiverForEntryWithDictionary:(NSDictionary *)dict
wantChecksum:(BOOL)checksum error:(nullable XADError *)errorptr;
-(XADUnarchiver *)unarchiverForEntryWithDictionary:(NSDictionary *)dict
resourceForkDictionary:(nullable NSDictionary *)forkdict wantChecksum:(BOOL)checksum error:(nullable XADError *)errorptr;

-(XADError)_extractFileEntryWithDictionary:(NSDictionary *)dict as:(NSString *)destpath;
-(XADError)_extractDirectoryEntryWithDictionary:(NSDictionary *)dict as:(NSString *)destpath;
-(XADError)_extractLinkEntryWithDictionary:(NSDictionary *)dict as:(NSString *)destpath;
-(XADError)_extractArchiveEntryWithDictionary:(NSDictionary *)dict to:(NSString *)destpath name:(NSString *)filename;
-(XADError)_extractResourceForkEntryWithDictionary:(NSDictionary *)dict asAppleDoubleFile:(NSString *)destpath;

-(XADError)_updateFileAttributesAtPath:(NSString *)path forEntryWithDictionary:(NSDictionary *)dict
deferDirectories:(BOOL)defer;
-(XADError)_ensureDirectoryExists:(NSString *)path;

-(XADError)runExtractorWithDictionary:(NSDictionary *)dict outputHandle:(CSHandle *)handle;
-(XADError)runExtractorWithDictionary:(NSDictionary *)dict
outputTarget:(id)target selector:(SEL)sel argument:(id)arg;

-(NSString *)adjustPathString:(NSString *)path forEntryWithDictionary:(NSDictionary *)dict;

@property (NS_NONATOMIC_IOSONLY, readonly) BOOL _shouldStop;

@end


@protocol XADUnarchiverDelegate <NSObject>

@optional
-(void)unarchiverNeedsPassword:(XADUnarchiver *)unarchiver;

-(BOOL)unarchiver:(XADUnarchiver *)unarchiver shouldExtractEntryWithDictionary:(NSDictionary *)dict suggestedPath:(NSString *__nullable*__nullable)pathptr;
-(void)unarchiver:(XADUnarchiver *)unarchiver willExtractEntryWithDictionary:(NSDictionary *)dict to:(NSString *)path;
-(void)unarchiver:(XADUnarchiver *)unarchiver didExtractEntryWithDictionary:(NSDictionary *)dict to:(NSString *)path error:(XADError)error;

@required
-(BOOL)unarchiver:(XADUnarchiver *)unarchiver shouldCreateDirectory:(NSString *)directory;
@optional
-(BOOL)unarchiver:(XADUnarchiver *)unarchiver shouldDeleteFileAndCreateDirectory:(NSString *)directory;

@optional
-(BOOL)unarchiver:(XADUnarchiver *)unarchiver shouldExtractArchiveEntryWithDictionary:(NSDictionary *)dict to:(NSString *)path;
-(void)unarchiver:(XADUnarchiver *)unarchiver willExtractArchiveEntryWithDictionary:(NSDictionary *)dict withUnarchiver:(XADUnarchiver *)subunarchiver to:(NSString *)path;
-(void)unarchiver:(XADUnarchiver *)unarchiver didExtractArchiveEntryWithDictionary:(NSDictionary *)dict withUnarchiver:(XADUnarchiver *)subunarchiver to:(NSString *)path error:(XADError)error;

@required
-(NSString *)unarchiver:(XADUnarchiver *)unarchiver destinationForLink:(XADString *)link from:(NSString *)path;

-(BOOL)extractionShouldStopForUnarchiver:(XADUnarchiver *)unarchiver;
-(void)unarchiver:(XADUnarchiver *)unarchiver extractionProgressForEntryWithDictionary:(NSDictionary *)dict
fileFraction:(double)fileprogress estimatedTotalFraction:(double)totalprogress;

@optional
-(void)unarchiver:(XADUnarchiver *)unarchiver findsFileInterestingForReason:(NSString *)reason;

@optional
// Deprecated.
-(null_unspecified NSString *)unarchiver:(XADUnarchiver *)unarchiver pathForExtractingEntryWithDictionary:(null_unspecified NSDictionary *)dict DEPRECATED_ATTRIBUTE;
-(BOOL)unarchiver:(XADUnarchiver *)unarchiver shouldExtractEntryWithDictionary:(null_unspecified NSDictionary *)dict to:(null_unspecified NSString *)path DEPRECATED_ATTRIBUTE;
-(null_unspecified NSString *)unarchiver:(XADUnarchiver *)unarchiver linkDestinationForEntryWithDictionary:(null_unspecified NSDictionary *)dict from:(null_unspecified NSString *)path DEPRECATED_ATTRIBUTE;
@end

NS_ASSUME_NONNULL_END

