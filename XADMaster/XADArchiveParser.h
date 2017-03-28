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

typedef NSString *XADArchiveKeys NS_EXTENSIBLE_STRING_ENUM;

extern XADArchiveKeys XADFileNameKey;
extern XADArchiveKeys XADCommentKey;
extern XADArchiveKeys XADFileSizeKey;
extern XADArchiveKeys XADCompressedSizeKey;
extern XADArchiveKeys XADCompressionNameKey;

extern XADArchiveKeys XADLastModificationDateKey;
extern XADArchiveKeys XADLastAccessDateKey;
extern XADArchiveKeys XADLastAttributeChangeDateKey;
extern XADArchiveKeys XADLastBackupDateKey;
extern XADArchiveKeys XADCreationDateKey;

extern XADArchiveKeys XADIsDirectoryKey;
extern XADArchiveKeys XADIsResourceForkKey;
extern XADArchiveKeys XADIsArchiveKey;
extern XADArchiveKeys XADIsHiddenKey;
extern XADArchiveKeys XADIsLinkKey;
extern XADArchiveKeys XADIsHardLinkKey;
extern XADArchiveKeys XADLinkDestinationKey;
extern XADArchiveKeys XADIsCharacterDeviceKey;
extern XADArchiveKeys XADIsBlockDeviceKey;
extern XADArchiveKeys XADDeviceMajorKey;
extern XADArchiveKeys XADDeviceMinorKey;
extern XADArchiveKeys XADIsFIFOKey;
extern XADArchiveKeys XADIsEncryptedKey;
extern XADArchiveKeys XADIsCorruptedKey;

extern XADArchiveKeys XADExtendedAttributesKey;
extern XADArchiveKeys XADFileTypeKey;
extern XADArchiveKeys XADFileCreatorKey;
extern XADArchiveKeys XADFinderFlagsKey;
extern XADArchiveKeys XADFinderInfoKey;
extern XADArchiveKeys XADPosixPermissionsKey;
extern XADArchiveKeys XADPosixUserKey;
extern XADArchiveKeys XADPosixGroupKey;
extern XADArchiveKeys XADPosixUserNameKey;
extern XADArchiveKeys XADPosixGroupNameKey;
extern XADArchiveKeys XADDOSFileAttributesKey;
extern XADArchiveKeys XADWindowsFileAttributesKey;
extern XADArchiveKeys XADAmigaProtectionBitsKey;

extern XADArchiveKeys XADIndexKey;
extern XADArchiveKeys XADDataOffsetKey;
extern XADArchiveKeys XADDataLengthKey;
extern XADArchiveKeys XADSkipOffsetKey;
extern XADArchiveKeys XADSkipLengthKey;

extern XADArchiveKeys XADIsSolidKey;
extern XADArchiveKeys XADFirstSolidIndexKey;
extern XADArchiveKeys XADFirstSolidEntryKey;
extern XADArchiveKeys XADNextSolidIndexKey;
extern XADArchiveKeys XADNextSolidEntryKey;
extern XADArchiveKeys XADSolidObjectKey;
extern XADArchiveKeys XADSolidOffsetKey;
extern XADArchiveKeys XADSolidLengthKey;

// Archive properties only
extern XADArchiveKeys XADArchiveNameKey;
extern XADArchiveKeys XADVolumesKey;
extern XADArchiveKeys XADVolumeScanningFailedKey;
extern XADArchiveKeys XADDiskLabelKey;

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
+(nullable XADArchiveParser *)archiveParserForHandle:(CSHandle *)handle name:(NSString *)name error:(nullable XADError *)errorptr;
+(nullable XADArchiveParser *)archiveParserForHandle:(CSHandle *)handle resourceFork:(nullable XADResourceFork *)fork name:(NSString *)name NS_SWIFT_UNAVAILABLE("Throws uncaught exception!");
+(nullable XADArchiveParser *)archiveParserForHandle:(CSHandle *)handle resourceFork:(nullable XADResourceFork *)fork name:(NSString *)name error:(nullable XADError *)errorptr;
+(nullable XADArchiveParser *)archiveParserForHandle:(CSHandle *)handle firstBytes:(NSData *)header name:(NSString *)name NS_SWIFT_UNAVAILABLE("Uncaught exception!");
+(nullable XADArchiveParser *)archiveParserForHandle:(CSHandle *)handle firstBytes:(NSData *)header name:(NSString *)name error:(nullable XADError *)errorptr;
+(nullable XADArchiveParser *)archiveParserForHandle:(CSHandle *)handle firstBytes:(NSData *)header resourceFork:(nullable XADResourceFork *)fork name:(NSString *)name NS_SWIFT_UNAVAILABLE("Throws uncaught exception!");
+(nullable XADArchiveParser *)archiveParserForHandle:(CSHandle *)handle firstBytes:(NSData *)header resourceFork:(nullable XADResourceFork *)fork name:(NSString *)name error:(nullable XADError *)errorptr;
+(nullable XADArchiveParser *)archiveParserForPath:(NSString *)filename NS_SWIFT_UNAVAILABLE("Throws uncaught exception!");
+(nullable XADArchiveParser *)archiveParserForPath:(NSString *)filename error:(nullable XADError *)errorptr;
+(nullable XADArchiveParser *)archiveParserForEntryWithDictionary:(NSDictionary<XADArchiveKeys,id> *)entry archiveParser:(XADArchiveParser *)parser wantChecksum:(BOOL)checksum NS_SWIFT_UNAVAILABLE("Throws uncaught exception!");
+(nullable XADArchiveParser *)archiveParserForEntryWithDictionary:(NSDictionary<XADArchiveKeys,id> *)entry archiveParser:(XADArchiveParser *)parser wantChecksum:(BOOL)checksum error:(nullable XADError *)errorptr;
+(nullable XADArchiveParser *)archiveParserForEntryWithDictionary:(NSDictionary<XADArchiveKeys,id> *)entry resourceForkDictionary:(nullable NSDictionary *)forkentry archiveParser:(XADArchiveParser *)parser wantChecksum:(BOOL)checksum NS_SWIFT_UNAVAILABLE("Throws uncaught exception!");
+(nullable XADArchiveParser *)archiveParserForEntryWithDictionary:(NSDictionary<XADArchiveKeys,id> *)entry resourceForkDictionary:(nullable NSDictionary<XADArchiveKeys,id> *)forkentry archiveParser:(XADArchiveParser *)parser wantChecksum:(BOOL)checksum error:(nullable XADError *)errorptr;
 
-(instancetype)init NS_DESIGNATED_INITIALIZER;

@property (nonatomic, retain) XADHandle *handle;
@property (NS_NONATOMIC_IOSONLY, retain, nullable) XADResourceFork *resourceFork;
@property (NS_NONATOMIC_IOSONLY, copy) NSString *name;
@property (NS_NONATOMIC_IOSONLY, copy) NSString *filename;
@property (NS_NONATOMIC_IOSONLY, copy) NSArray<NSString*> *allFilenames;

@property (NS_NONATOMIC_IOSONLY, assign, nullable) id<XADArchiveParserDelegate> delegate;

@property (NS_NONATOMIC_IOSONLY, readonly, copy) NSDictionary<XADArchiveKeys,id> *properties;
@property (NS_NONATOMIC_IOSONLY, readonly, copy) NSString *currentFilename;

@property (NS_NONATOMIC_IOSONLY, getter=isEncrypted, readonly) BOOL encrypted;
@property (NS_NONATOMIC_IOSONLY, copy, nullable) NSString *password;
@property (NS_NONATOMIC_IOSONLY, readonly) BOOL hasPassword;

@property (NS_NONATOMIC_IOSONLY, copy) XADStringEncodingName encodingName;
@property (NS_NONATOMIC_IOSONLY, readonly) float encodingConfidence;
@property (NS_NONATOMIC_IOSONLY, readonly) BOOL caresAboutPasswordEncoding;
@property (NS_NONATOMIC_IOSONLY, copy, nullable) XADStringEncodingName passwordEncodingName;
@property (NS_NONATOMIC_IOSONLY, readonly, strong) XADStringSource *stringSource;

-(nullable XADString *)linkDestinationForDictionary:(NSDictionary<XADArchiveKeys,id> *)dict NS_SWIFT_UNAVAILABLE("Throws uncaught exception!");
-(nullable XADString *)linkDestinationForDictionary:(NSDictionary<XADArchiveKeys,id> *)dict error:(XADError *)errorptr;
-(NSDictionary *)extendedAttributesForDictionary:(NSDictionary<XADArchiveKeys,id> *)dict;
-(NSData *)finderInfoForDictionary:(NSDictionary<XADArchiveKeys,id> *)dict;

@property (NS_NONATOMIC_IOSONLY, readonly) BOOL wasStopped;

@property (NS_NONATOMIC_IOSONLY, readonly) BOOL hasChecksum;
-(BOOL)testChecksum;
-(XADError)testChecksumWithoutExceptions;



// Internal functions

+(NSArray<NSString*> *)scanForVolumesWithFilename:(NSString *)filename regex:(XADRegex *)regex;
+(NSArray<NSString*> *)scanForVolumesWithFilename:(NSString *)filename
regex:(XADRegex *)regex firstFileExtension:(nullable NSString *)firstext;

@property (NS_NONATOMIC_IOSONLY, readonly) BOOL shouldKeepParsing;

-(CSHandle *)handleAtDataOffsetForDictionary:(NSDictionary<XADArchiveKeys,id> *)dict;
@property (NS_NONATOMIC_IOSONLY, readonly, copy) XADSkipHandle *skipHandle;
-(CSHandle *)zeroLengthHandleWithChecksum:(BOOL)checksum;
-(CSHandle *)subHandleFromSolidStreamForEntryWithDictionary:(NSDictionary<XADArchiveKeys,id> *)dict;

@property (NS_NONATOMIC_IOSONLY, readonly, copy) NSArray *volumes;
@property (NS_NONATOMIC_IOSONLY, readonly, retain) CSHandle *currentHandle;
-(off_t)offsetForVolume:(int)disk offset:(off_t)offset;

-(void)setObject:(id)object forPropertyKey:(NSString *)key;
-(void)addPropertiesFromDictionary:(NSDictionary<XADArchiveKeys,id> *)dict;
-(void)setIsMacArchive:(BOOL)ismac;

-(void)addEntryWithDictionary:(NSMutableDictionary<XADArchiveKeys,id> *)dict;
-(void)addEntryWithDictionary:(NSMutableDictionary<XADArchiveKeys,id> *)dict retainPosition:(BOOL)retainpos;

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
@property (class, readonly) int requiredHeaderSize;
+(BOOL)recognizeFileWithHandle:(CSHandle *)handle firstBytes:(NSData *)data
name:(NSString *)name;
+(BOOL)recognizeFileWithHandle:(CSHandle *)handle firstBytes:(NSData *)data
name:(NSString *)name propertiesToAdd:(NSMutableDictionary<XADArchiveKeys,id> *)props;
+(BOOL)recognizeFileWithHandle:(CSHandle *)handle firstBytes:(NSData *)data
resourceFork:(XADResourceFork *)fork name:(NSString *)name propertiesToAdd:(NSMutableDictionary<XADArchiveKeys,id> *)props;
+(nullable NSArray<NSString*> *)volumesForHandle:(CSHandle *)handle firstBytes:(NSData *)data
name:(NSString *)name;

-(void)parse;
-(nullable CSHandle *)handleForEntryWithDictionary:(NSDictionary<XADArchiveKeys,id> *)dict wantChecksum:(BOOL)checksum;
@property (NS_NONATOMIC_IOSONLY, readonly, copy) NSString *formatName;

-(nullable CSHandle *)handleForSolidStreamWithObject:(id)obj wantChecksum:(BOOL)checksum;

//! Exception-free wrappers for subclass methods:
//! \c parseWithoutExceptions will in addition return \c XADBreakError if the delegate
//! requested parsing to stop.
-(XADError)parseWithoutExceptions;
-(nullable CSHandle *)handleForEntryWithDictionary:(NSDictionary<XADArchiveKeys,id> *)dict wantChecksum:(BOOL)checksum error:(nullable XADError *)errorptr;

@end

@protocol XADArchiveParserDelegate <NSObject>
@optional

-(void)archiveParser:(XADArchiveParser *)parser foundEntryWithDictionary:(NSDictionary<XADArchiveKeys,id> *)dict;
-(BOOL)archiveParsingShouldStop:(XADArchiveParser *)parser;
-(void)archiveParserNeedsPassword:(XADArchiveParser *)parser;
-(void)archiveParser:(XADArchiveParser *)parser findsFileInterestingForReason:(NSString *)reason;

@end

NSMutableArray *XADSortVolumes(NSMutableArray *volumes,NSString *firstfileextension) UNAVAILABLE_ATTRIBUTE;

NS_ASSUME_NONNULL_END
