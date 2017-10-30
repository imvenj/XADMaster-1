#import <Foundation/Foundation.h>
#import "XADUnarchiver.h"
#import "CSHandle.h"

NS_ASSUME_NONNULL_BEGIN

@interface XADPlatform:NSObject

// Archive entry extraction.
+(XADError)extractResourceForkEntryWithDictionary:(NSDictionary<XADArchiveKeys,id> *)dict
unarchiver:(XADUnarchiver *)unarchiver toPath:(NSString *)destpath NS_REFINED_FOR_SWIFT;
+(XADError)updateFileAttributesAtPath:(NSString *)path
forEntryWithDictionary:(NSDictionary<XADArchiveKeys,id> *)dict parser:(XADArchiveParser *)parser
preservePermissions:(BOOL)preservepermissions NS_REFINED_FOR_SWIFT;
+(XADError)createLinkAtPath:(NSString *)path withDestinationPath:(NSString *)link NS_REFINED_FOR_SWIFT;

// Archive post-processing.
+(nullable id)readCloneableMetadataFromPath:(NSString *)path;
+(void)writeCloneableMetadata:(id)metadata toPath:(NSString *)path;
+(BOOL)copyDateFromPath:(NSString *)src toPath:(NSString *)dest;
+(BOOL)resetDateAtPath:(NSString *)path;

// Path functions.
+(BOOL)fileExistsAtPath:(NSString *)path;
+(BOOL)fileExistsAtPath:(NSString *)path isDirectory:(nullable BOOL *)isdirptr;
+(NSString *)uniqueDirectoryPathWithParentDirectory:(NSString *)parent;
+(NSString *)sanitizedPathComponent:(NSString *)component;
+(NSArray<NSString*> *)contentsOfDirectoryAtPath:(NSString *)path;
+(BOOL)moveItemAtPath:(NSString *)src toPath:(NSString *)dest;
+(BOOL)removeItemAtPath:(NSString *)path;

// Resource forks
+(nullable CSHandle *)handleForReadingResourceForkAtPath:(NSString *)path;

// Time functions.
+(double)currentTimeInSeconds;
#if __has_feature(objc_class_property)
@property (class, readonly) double currentTimeInSeconds;
#endif

@end

NS_ASSUME_NONNULL_END
