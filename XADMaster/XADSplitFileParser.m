#import "XADSplitFileParser.h"
#import "XADRegex.h"
#import "XADPlatform.h"
#import "CSFileHandle.h"

@implementation XADSplitFileParser

+(int)requiredHeaderSize { return 0; }

+(BOOL)recognizeFileWithHandle:(CSHandle *)handle firstBytes:(NSData *)data name:(NSString *)name
{
	if(!name) return NO;
	if(![handle isKindOfClass:[CSFileHandle class]]) return NO;

	// Check if filename is of the form .001
	NSArray *matches=[name substringsCapturedByPattern:@"^(.*)\\.([0-9]{3})$" options:REG_ICASE];
	if(!matches) return NO;

	// Find another filename in the series. Pick .001 if the given file is not already that,
	// and .002 otherwise.
	NSString *otherext;
	if([matches[2] isEqual:@"001"]) otherext=@"002";
	else otherext=@"001";

	// Check if this other file exists, too.
	NSString *othername=[NSString stringWithFormat:@"%@.%@",matches[1],otherext];
	return [XADPlatform fileExistsAtPath:othername];
}

+(NSArray *)volumesForHandle:(CSHandle *)handle firstBytes:(NSData *)data name:(NSString *)name
{
	NSArray *matches=[name substringsCapturedByPattern:@"^(.*)\\.[0-9]{3}$" options:REG_ICASE];
	if(matches)
	{
		return [self scanForVolumesWithFilename:name
		regex:[XADRegex regexWithPattern:[NSString stringWithFormat:@"^%@\\.[0-9]{3}$",
			[matches[1] escapedPattern]] options:REG_ICASE]
		firstFileExtension:nil];
	}

	return nil;
}

-(void)parse
{
	NSString *basename=self.name.stringByDeletingPathExtension;
	CSHandle *handle=self.handle;

	NSMutableDictionary *dict=[NSMutableDictionary dictionaryWithObjectsAndKeys:
		[self XADPathWithUnseparatedString:basename],XADFileNameKey,
		@(handle.fileSize),XADFileSizeKey,
		@(handle.fileSize),XADCompressedSizeKey,
	nil];

	NSString *ext=basename.pathExtension;
	if([ext caseInsensitiveCompare:@"zip"]==0)
	dict[XADIsArchiveKey] = @YES;

	[self addEntryWithDictionary:dict];
}

-(CSHandle *)handleForEntryWithDictionary:(NSDictionary *)dict wantChecksum:(BOOL)checksum
{
	CSHandle *handle=self.handle;
	[handle seekToFileOffset:0];
	return handle;
}

-(NSString *)formatName { return @"Split file"; }

@end
