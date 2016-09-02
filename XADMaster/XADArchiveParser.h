#import <Foundation/Foundation.h>
#import "XADException.h"
#import "XADString.h"
#import "XADPath.h"
#import "XADRegex.h"
#import "CSHandle.h"
#import "XADSkipHandle.h"
#import "XADResourceFork.h"
#import "Checksums.h"

NS_ASSUME_NONNULL_BEGIN

extern NSString *XADFileNameKey;
extern NSString *XADCommentKey;
extern NSString *XADFileSizeKey;
extern NSString *XADCompressedSizeKey;
extern NSString *XADCompressionNameKey;

extern NSString *XADLastModificationDateKey;
extern NSString *XADLastAccessDateKey;
extern NSString *XADLastAttributeChangeDateKey;
extern NSString *XADLastBackupDateKey;
extern NSString *XADCreationDateKey;

extern NSString *XADIsDirectoryKey;
extern NSString *XADIsResourceForkKey;
extern NSString *XADIsArchiveKey;
extern NSString *XADIsHiddenKey;
extern NSString *XADIsLinkKey;
extern NSString *XADIsHardLinkKey;
extern NSString *XADLinkDestinationKey;
extern NSString *XADIsCharacterDeviceKey;
extern NSString *XADIsBlockDeviceKey;
extern NSString *XADDeviceMajorKey;
extern NSString *XADDeviceMinorKey;
extern NSString *XADIsFIFOKey;
extern NSString *XADIsEncryptedKey;
extern NSString *XADIsCorruptedKey;

extern NSString *XADExtendedAttributesKey;
extern NSString *XADFileTypeKey;
extern NSString *XADFileCreatorKey;
extern NSString *XADFinderFlagsKey;
extern NSString *XADFinderInfoKey;
extern NSString *XADPosixPermissionsKey;
extern NSString *XADPosixUserKey;
extern NSString *XADPosixGroupKey;
extern NSString *XADPosixUserNameKey;
extern NSString *XADPosixGroupNameKey;
extern NSString *XADDOSFileAttributesKey;
extern NSString *XADWindowsFileAttributesKey;
extern NSString *XADAmigaProtectionBitsKey;

extern NSString *XADIndexKey;
extern NSString *XADDataOffsetKey;
extern NSString *XADDataLengthKey;
extern NSString *XADSkipOffsetKey;
extern NSString *XADSkipLengthKey;

extern NSString *XADIsSolidKey;
extern NSString *XADFirstSolidIndexKey;
extern NSString *XADFirstSolidEntryKey;
extern NSString *XADNextSolidIndexKey;
extern NSString *XADNextSolidEntryKey;
extern NSString *XADSolidObjectKey;
extern NSString *XADSolidOffsetKey;
extern NSString *XADSolidLengthKey;

// Archive properties only
extern NSString *XADArchiveNameKey;
extern NSString *XADVolumesKey;
extern NSString *XADVolumeScanningFailedKey;
extern NSString *XADDiskLabelKey;

@protocol XADArchiveParserDelegate;

@interface XADArchiveParser:NSObject
{
	CSHandle *sourcehandle;
	XADSkipHandle *skiphandle;
	XADResourceFork *resourcefork;

	NSString *password;
	NSString *passwordencodingname;
	BOOL caresaboutpasswordencoding;

	NSMutableDictionary *properties;
	XADStringSource *stringsource;

	int currindex;

	id parsersolidobj;
	NSMutableDictionary *firstsoliddict,*prevsoliddict;
	id currsolidobj;
	CSHandle *currsolidhandle;
	BOOL forcesolid;

	BOOL shouldstop;
}

+(nullable Class)archiveParserClassForHandle:(CSHandle *)handle firstBytes:(NSData *)header
resourceFork:(XADResourceFork *)fork name:(NSString *)name propertiesToAdd:(NSMutableDictionary *)props;
+(nullable XADArchiveParser *)archiveParserForHandle:(CSHandle *)handle name:(NSString *)name NS_SWIFT_UNAVAILABLE("Throws uncaught exception!");
+(nullable XADArchiveParser *)archiveParserForHandle:(CSHandle *)handle name:(NSString *)name error:(NSError *__nullable*__nullable)errorptr;
+(nullable XADArchiveParser *)archiveParserForHandle:(CSHandle *)handle resourceFork:(nullable XADResourceFork *)fork name:(NSString *)name NS_SWIFT_UNAVAILABLE("Throws uncaught exception!");
+(nullable XADArchiveParser *)archiveParserForHandle:(CSHandle *)handle resourceFork:(nullable XADResourceFork *)fork name:(NSString *)name error:(NSError *__nullable*__nullable)errorptr;
+(nullable XADArchiveParser *)archiveParserForHandle:(CSHandle *)handle firstBytes:(NSData *)header name:(NSString *)name NS_SWIFT_UNAVAILABLE("Uncaught exception!");
+(nullable XADArchiveParser *)archiveParserForHandle:(CSHandle *)handle firstBytes:(NSData *)header name:(NSString *)name error:(NSError *__nullable*__nullable)errorptr;
+(nullable XADArchiveParser *)archiveParserForHandle:(CSHandle *)handle firstBytes:(NSData *)header resourceFork:(nullable XADResourceFork *)fork name:(NSString *)name NS_SWIFT_UNAVAILABLE("Throws uncaught exception!");
+(nullable XADArchiveParser *)archiveParserForHandle:(CSHandle *)handle firstBytes:(NSData *)header resourceFork:(nullable XADResourceFork *)fork name:(NSString *)name error:(NSError *__nullable*__nullable)errorptr;
+(nullable XADArchiveParser *)archiveParserForPath:(NSString *)filename NS_SWIFT_UNAVAILABLE("Throws uncaught exception!");
+(nullable XADArchiveParser *)archiveParserForPath:(NSString *)filename error:(NSError *__nullable*__nullable)errorptr;
+(nullable XADArchiveParser *)archiveParserForEntryWithDictionary:(NSDictionary *)entry archiveParser:(XADArchiveParser *)parser wantChecksum:(BOOL)checksum NS_SWIFT_UNAVAILABLE("Throws uncaught exception!");
+(nullable XADArchiveParser *)archiveParserForEntryWithDictionary:(NSDictionary *)entry archiveParser:(XADArchiveParser *)parser wantChecksum:(BOOL)checksum error:(NSError *__nullable*__nullable)errorptr;
+(nullable XADArchiveParser *)archiveParserForEntryWithDictionary:(NSDictionary *)entry resourceForkDictionary:(nullable NSDictionary *)forkentry archiveParser:(XADArchiveParser *)parser wantChecksum:(BOOL)checksum NS_SWIFT_UNAVAILABLE("Throws uncaught exception!");
+(nullable XADArchiveParser *)archiveParserForEntryWithDictionary:(NSDictionary *)entry resourceForkDictionary:(nullable NSDictionary *)forkentry archiveParser:(XADArchiveParser *)parser wantChecksum:(BOOL)checksum error:(NSError *__nullable*__nullable)errorptr;
 
-(instancetype)init NS_DESIGNATED_INITIALIZER;

@property (nonatomic, retain) XADHandle *handle;
@property (NS_NONATOMIC_IOSONLY, retain) XADResourceFork *resourceFork;
@property (NS_NONATOMIC_IOSONLY, copy) NSString *name;
@property (NS_NONATOMIC_IOSONLY, copy) NSString *filename;
@property (NS_NONATOMIC_IOSONLY, copy) NSArray<NSString*> *allFilenames;

@property (NS_NONATOMIC_IOSONLY, assign) id<XADArchiveParserDelegate> delegate;

@property (NS_NONATOMIC_IOSONLY, readonly, copy) NSDictionary *properties;
@property (NS_NONATOMIC_IOSONLY, readonly, copy) NSString *currentFilename;

@property (NS_NONATOMIC_IOSONLY, getter=isEncrypted, readonly) BOOL encrypted;
@property (NS_NONATOMIC_IOSONLY, copy, nullable) NSString *password;
@property (NS_NONATOMIC_IOSONLY, readonly) BOOL hasPassword;

@property (NS_NONATOMIC_IOSONLY, copy) NSString *encodingName;
@property (NS_NONATOMIC_IOSONLY, readonly) float encodingConfidence;
@property (NS_NONATOMIC_IOSONLY, readonly) BOOL caresAboutPasswordEncoding;
@property (NS_NONATOMIC_IOSONLY, copy, nullable) NSString *passwordEncodingName;
@property (NS_NONATOMIC_IOSONLY, readonly, strong) XADStringSource *stringSource;

-(XADString *)linkDestinationForDictionary:(NSDictionary *)dict NS_SWIFT_UNAVAILABLE("This function throws exceptions as part of its control flow; use linkDestinationForDictionary:error: instead");
-(XADString *)linkDestinationForDictionary:(NSDictionary *)dict error:(NSError **)errorptr;
-(NSDictionary *)extendedAttributesForDictionary:(NSDictionary *)dict;
-(NSData *)finderInfoForDictionary:(NSDictionary *)dict;

@property (NS_NONATOMIC_IOSONLY, readonly) BOOL wasStopped;

@property (NS_NONATOMIC_IOSONLY, readonly) BOOL hasChecksum;
-(BOOL)testChecksum;
-(XADError)testChecksumWithoutExceptions;



// Internal functions

+(NSArray *)scanForVolumesWithFilename:(NSString *)filename regex:(XADRegex *)regex;
+(NSArray *)scanForVolumesWithFilename:(NSString *)filename
regex:(XADRegex *)regex firstFileExtension:(nullable NSString *)firstext;

@property (NS_NONATOMIC_IOSONLY, readonly) BOOL shouldKeepParsing;

-(CSHandle *)handleAtDataOffsetForDictionary:(NSDictionary *)dict;
@property (NS_NONATOMIC_IOSONLY, readonly, copy) XADSkipHandle *skipHandle;
-(CSHandle *)zeroLengthHandleWithChecksum:(BOOL)checksum;
-(CSHandle *)subHandleFromSolidStreamForEntryWithDictionary:(NSDictionary *)dict;

@property (NS_NONATOMIC_IOSONLY, readonly, copy) NSArray *volumes;
@property (NS_NONATOMIC_IOSONLY, readonly, retain) CSHandle *currentHandle;
-(off_t)offsetForVolume:(int)disk offset:(off_t)offset;

-(void)setObject:(id)object forPropertyKey:(NSString *)key;
-(void)addPropertiesFromDictionary:(NSDictionary *)dict;
-(void)setIsMacArchive:(BOOL)ismac;

-(void)addEntryWithDictionary:(NSMutableDictionary *)dict;
-(void)addEntryWithDictionary:(NSMutableDictionary *)dict retainPosition:(BOOL)retainpos;

-(XADString *)XADStringWithString:(NSString *)string;
-(XADString *)XADStringWithData:(NSData *)data;
-(XADString *)XADStringWithData:(NSData *)data encodingName:(NSString *)encoding;
-(XADString *)XADStringWithBytes:(const void *)bytes length:(NSInteger)length;
-(XADString *)XADStringWithBytes:(const void *)bytes length:(NSInteger)length encodingName:(NSString *)encoding;
-(XADString *)XADStringWithCString:(const char *)cstring;
-(XADString *)XADStringWithCString:(const char *)cstring encodingName:(NSString *)encoding;

@property (NS_NONATOMIC_IOSONLY, readonly, copy) XADPath *XADPath;
-(XADPath *)XADPathWithString:(NSString *)string;
-(XADPath *)XADPathWithUnseparatedString:(NSString *)string;
-(XADPath *)XADPathWithData:(NSData *)data separators:(const char *)separators;
-(XADPath *)XADPathWithData:(NSData *)data encodingName:(NSString *)encoding separators:(const char *)separators;
-(XADPath *)XADPathWithBytes:(const void *)bytes length:(NSInteger)length separators:(const char *)separators;
-(XADPath *)XADPathWithBytes:(const void *)bytes length:(NSInteger)length encodingName:(NSString *)encoding separators:(const char *)separators;
-(XADPath *)XADPathWithCString:(const char *)cstring separators:(const char *)separators;
-(XADPath *)XADPathWithCString:(const char *)cstring encodingName:(NSString *)encoding separators:(const char *)separators;

@property (NS_NONATOMIC_IOSONLY, readonly, copy, nullable) NSData *encodedPassword;
@property (NS_NONATOMIC_IOSONLY, readonly, nullable) const char *encodedCStringPassword;

-(void)reportInterestingFileWithReason:(NSString *)reason,... NS_FORMAT_FUNCTION(1,0);



// Subclasses implement these:

+(int)requiredHeaderSize;
+(BOOL)recognizeFileWithHandle:(CSHandle *)handle firstBytes:(NSData *)data
name:(NSString *)name;
+(BOOL)recognizeFileWithHandle:(CSHandle *)handle firstBytes:(NSData *)data
name:(NSString *)name propertiesToAdd:(NSMutableDictionary *)props;
+(BOOL)recognizeFileWithHandle:(CSHandle *)handle firstBytes:(NSData *)data
resourceFork:(XADResourceFork *)fork name:(NSString *)name propertiesToAdd:(NSMutableDictionary *)props;
+(nullable NSArray *)volumesForHandle:(CSHandle *)handle firstBytes:(NSData *)data
name:(NSString *)name;

-(void)parse;
-(nullable CSHandle *)handleForEntryWithDictionary:(NSDictionary *)dict wantChecksum:(BOOL)checksum NS_SWIFT_UNAVAILABLE("This function throws exceptions as part of its control flow; use handleForEntryWithDictionary:wantChecksum:error: instead");
@property (NS_NONATOMIC_IOSONLY, readonly, copy) NSString *formatName;

-(nullable CSHandle *)handleForSolidStreamWithObject:(id)obj wantChecksum:(BOOL)checksum;

// Exception-free wrappers for subclass methods:
// parseWithoutExceptions will in addition return XADBreakError if the delegate
// requested parsing to stop.

-(XADError)parseWithoutExceptions;
-(BOOL)parseWithError:(NSError**)error;
-(nullable CSHandle *)handleForEntryWithDictionary:(NSDictionary *)dict wantChecksum:(BOOL)checksum error:(NSError**)errorptr;

@end

@protocol XADArchiveParserDelegate <NSObject>
@optional

-(void)archiveParser:(XADArchiveParser *)parser foundEntryWithDictionary:(NSDictionary *)dict;
-(BOOL)archiveParsingShouldStop:(XADArchiveParser *)parser;
-(void)archiveParserNeedsPassword:(XADArchiveParser *)parser;
-(void)archiveParser:(XADArchiveParser *)parser findsFileInterestingForReason:(NSString *)reason;

@end

NSMutableArray *XADSortVolumes(NSMutableArray *volumes,NSString *firstfileextension);

NS_ASSUME_NONNULL_END
