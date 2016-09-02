#import "XADMacArchiveParser.h"

@interface XAD7ZipParser:XADMacArchiveParser
{
	off_t startoffset;

	NSDictionary *mainstreams;

	NSDictionary *currfolder;
	CSHandle *currfolderhandle;
}

+(int)requiredHeaderSize;
+(BOOL)recognizeFileWithHandle:(CSHandle *)handle firstBytes:(NSData *)data name:(NSString *)name;
+(NSArray *)volumesForHandle:(CSHandle *)handle firstBytes:(NSData *)data name:(NSString *)name;

-(instancetype)init;

-(void)parseWithSeparateMacForks DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("This function throws exceptions as part of its control flow");

-(NSArray *)parseFilesForHandle:(CSHandle *)handle DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("This function throws exceptions as part of its control flow");

-(void)parseBitVectorForHandle:(CSHandle *)handle array:(NSArray *)array key:(NSString *)key DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("This function throws exceptions as part of its control flow");
-(NSIndexSet *)parseDefintionVectorForHandle:(CSHandle *)handle numberOfElements:(NSInteger)num DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("This function throws exceptions as part of its control flow");
-(void)parseDatesForHandle:(CSHandle *)handle array:(NSMutableArray *)array key:(NSString *)key DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("This function throws exceptions as part of its control flow");
-(void)parseCRCsForHandle:(CSHandle *)handle array:(NSMutableArray *)array DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("This function throws exceptions as part of its control flow");
-(void)parseNamesForHandle:(CSHandle *)handle array:(NSMutableArray *)array DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("This function throws exceptions as part of its control flow");
-(void)parseAttributesForHandle:(CSHandle *)handle array:(NSMutableArray *)array DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("This function throws exceptions as part of its control flow");

-(NSDictionary *)parseStreamsForHandle:(CSHandle *)handle DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("This function throws exceptions as part of its control flow");
-(NSArray<NSMutableDictionary<NSString*,id>*> *)parsePackedStreamsForHandle:(CSHandle *)handle DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("This function throws exceptions as part of its control flow");
-(NSArray<NSMutableDictionary<NSString*,id>*> *)parseFoldersForHandle:(CSHandle *)handle packedStreams:(NSArray *)packedstreams DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("This function throws exceptions as part of its control flow");
-(void)parseFolderForHandle:(CSHandle *)handle dictionary:(NSMutableDictionary *)dictionary
packedStreams:(NSArray *)packedstreams packedStreamIndex:(NSInteger *)packedstreamindex DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("This function throws exceptions as part of its control flow");
-(void)parseSubStreamsInfoForHandle:(CSHandle *)handle folders:(NSArray *)folders DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("This function throws exceptions as part of its control flow");

-(BOOL)parseWithSeparateMacForksWithError:(NSError **)error;

-(NSArray *)parseFilesForHandle:(CSHandle *)handle error:(NSError**)error;

-(BOOL)parseBitVectorForHandle:(CSHandle *)handle array:(NSArray *)array key:(NSString *)key error:(NSError**)error;
-(NSIndexSet *)parseDefintionVectorForHandle:(CSHandle *)handle numberOfElements:(NSInteger)num error:(NSError**)error;
-(BOOL)parseDatesForHandle:(CSHandle *)handle array:(NSMutableArray *)array key:(NSString *)key error:(NSError**)error;
-(BOOL)parseCRCsForHandle:(CSHandle *)handle array:(NSMutableArray *)array error:(NSError**)error;
-(BOOL)parseNamesForHandle:(CSHandle *)handle array:(NSMutableArray *)array error:(NSError**)error;
-(BOOL)parseAttributesForHandle:(CSHandle *)handle array:(NSMutableArray *)array error:(NSError**)error;

-(NSDictionary *)parseStreamsForHandle:(CSHandle *)handle error:(NSError**)error;
-(NSArray<NSMutableDictionary<NSString*,id>*> *)parsePackedStreamsForHandle:(CSHandle *)handle error:(NSError**)error;
-(NSArray<NSMutableDictionary<NSString*,id>*> *)parseFoldersForHandle:(CSHandle *)handle packedStreams:(NSArray *)packedstreams error:(NSError**)error;
-(BOOL)parseFolderForHandle:(CSHandle *)handle dictionary:(NSMutableDictionary *)dictionary
			  packedStreams:(NSArray *)packedstreams packedStreamIndex:(NSInteger *)packedstreamindex error:(NSError**)error;
-(BOOL)parseSubStreamsInfoForHandle:(CSHandle *)handle folders:(NSArray *)folders error:(NSError**)error;
-(void)setupDefaultSubStreamsForFolders:(NSArray *)folders;
-(NSArray *)collectAllSubStreamsFromFolders:(NSArray *)folders;

-(CSHandle *)rawHandleForEntryWithDictionary:(NSDictionary *)dict wantChecksum:(BOOL)checksum;
-(CSHandle *)handleForSolidStreamWithObject:(id)obj wantChecksum:(BOOL)checksum;
-(CSHandle *)handleForStreams:(NSDictionary *)streams folderIndex:(int)folderindex;
-(CSHandle *)outHandleForFolder:(NSDictionary *)folder index:(int)index;
-(CSHandle *)inHandleForFolder:(NSDictionary *)folder coder:(NSDictionary *)coder index:(int)index;
-(CSHandle *)inHandleForFolder:(NSDictionary *)folder index:(int)index;

-(int)IDForCoder:(NSDictionary *)coder;
-(off_t)compressedSizeForFolder:(NSDictionary *)folder;
-(off_t)uncompressedSizeForFolder:(NSDictionary *)folder;
-(NSString *)compressorNameForFolder:(NSDictionary *)folder;
-(NSString *)compressorNameForFolder:(NSDictionary *)folder index:(int)index;
-(NSString *)compressorNameForCoder:(NSDictionary *)coder;
-(BOOL)isFolderEncrypted:(NSDictionary *)folder;
-(BOOL)isFolderEncrypted:(NSDictionary *)folder index:(int)index;

-(NSString *)formatName;

@end

@interface XAD7ZipSFXParser:XAD7ZipParser
{
}

+(int)requiredHeaderSize;
+(BOOL)recognizeFileWithHandle:(CSHandle *)handle firstBytes:(NSData *)data
name:(NSString *)name propertiesToAdd:(NSMutableDictionary *)props;

-(void)parse DEPRECATED_ATTRIBUTE NS_SWIFT_UNAVAILABLE("This function throws exceptions as part of its control flow");
-(NSString *)formatName;

@end
