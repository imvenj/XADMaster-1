#import <Foundation/Foundation.h>

typedef NS_ENUM(int, XADError) {
	XADErrorNone =			0x0000, /*!< no error */
	XADErrorUnknown =		0x0001, /*!< unknown error */
	XADErrorInput =			0x0002, /*!< input data buffers border exceeded */
	XADErrorOutput =		0x0003, /*!< failed to write to file */
	XADErrorBadParameters =	0x0004, /*!< function called with illegal parameters */
	XADErrorOutOfMemory =	0x0005, /*!< not enough memory available */
	XADErrorIllegalData =	0x0006, /*!< data is corrupted */
	XADErrorNotSupported =	0x0007, /*!< file not fully supported */
	XADErrorResource =		0x0008, /*!< required resource missing */
	XADErrorDecrunch =		0x0009, /*!< error on decrunching */
	XADErrorFiletype =		0x000A, /*!< unknown file type */
	XADErrorOpenFile =		0x000B, /*!< opening file failed */
	XADErrorSkip =			0x000C, /*!< file, disk has been skipped */
	XADErrorBreak =			0x000D, /*!< user break in progress hook */
	XADErrorFileExists =	0x000E, /*!< file already exists */
	XADErrorPassword =		0x000F, /*!< missing or wrong password */
	XADErrorMakeDirectory =	0x0010, /*!< could not create directory */
	XADErrorChecksum =		0x0011, /*!< wrong checksum */
	XADErrorVerify =		0x0012, /*!< verify failed (disk hook) */
	XADErrorGeometry =		0x0013, /*!< wrong drive geometry */
	XADErrorDataFormat =	0x0014, /*!< unknown data format */
	XADErrorEmpty =			0x0015, /*!< source contains no files */
	XADErrorFileSystem =	0x0016, /*!< unknown filesystem */
	XADErrorFileDirectory =	0x0017, /*!< name of file exists as directory */
	XADErrorShortBuffer =	0x0018, /*!< buffer was too short */
	XADErrorEncoding =		0x0019, /*!< text encoding was defective */
	XADErrorLink =			0x001a, /*!< could not create link */

	XADErrorSubArchive = 0x10000
};

#ifndef CLANG_ANALYZER_NORETURN
	#ifdef __clang__
		#if __has_feature(attribute_analyzer_noreturn)
			#define CLANG_ANALYZER_NORETURN __attribute__((analyzer_noreturn))
		#else
			#define CLANG_ANALYZER_NORETURN
		#endif
	#else
		#define CLANG_ANALYZER_NORETURN
	#endif
#endif

extern NSString * __nonnull const XADExceptionName;
extern NSString * __nonnull const XADErrorDomain;

NS_SWIFT_UNAVAILABLE("Exceptions aren't supported by Swift")
@interface XADException:NSObject
{
}

+(void)raiseUnknownException CLANG_ANALYZER_NORETURN;
+(void)raiseInputException CLANG_ANALYZER_NORETURN;
+(void)raiseOutputException CLANG_ANALYZER_NORETURN;
+(void)raiseIllegalDataException CLANG_ANALYZER_NORETURN;
+(void)raiseNotSupportedException CLANG_ANALYZER_NORETURN;
+(void)raiseDecrunchException CLANG_ANALYZER_NORETURN;
+(void)raisePasswordException CLANG_ANALYZER_NORETURN;
+(void)raiseChecksumException CLANG_ANALYZER_NORETURN;
+(void)raiseDataFormatException CLANG_ANALYZER_NORETURN;
+(void)raiseOutOfMemoryException CLANG_ANALYZER_NORETURN;
+(void)raiseExceptionWithXADError:(XADError)errnum CLANG_ANALYZER_NORETURN;

+(XADError)parseException:(nonnull id)exception;
+(nullable NSString *)describeXADError:(XADError)errnum;

@end

extern NSString *__nullable XADDescribeError(XADError errnum);

static const XADError XADNoError API_DEPRECATED_WITH_REPLACEMENT("XADErrorNone", macosx(10.0, 10.8)) = XADErrorNone;
static const XADError XADUnknownError API_DEPRECATED_WITH_REPLACEMENT("XADErrorUnknown", macosx(10.0, 10.8)) = XADErrorUnknown;
static const XADError XADInputError API_DEPRECATED_WITH_REPLACEMENT("XADErrorInput", macosx(10.0, 10.8)) = XADErrorInput;
static const XADError XADOutputError API_DEPRECATED_WITH_REPLACEMENT("XADErrorOutput", macosx(10.0, 10.8)) = XADErrorOutput;
static const XADError XADBadParametersError API_DEPRECATED_WITH_REPLACEMENT("XADErrorBadParameters", macosx(10.0, 10.8)) = XADErrorBadParameters;
static const XADError XADOutOfMemoryError API_DEPRECATED_WITH_REPLACEMENT("XADErrorOutOfMemory", macosx(10.0, 10.8)) = XADErrorOutOfMemory;
static const XADError XADIllegalDataError API_DEPRECATED_WITH_REPLACEMENT("XADErrorIllegalData", macosx(10.0, 10.8)) = XADErrorIllegalData;
static const XADError XADNotSupportedError API_DEPRECATED_WITH_REPLACEMENT("XADErrorNotSupported", macosx(10.0, 10.8)) = XADErrorNotSupported;
static const XADError XADResourceError API_DEPRECATED_WITH_REPLACEMENT("XADErrorResource", macosx(10.0, 10.8)) = XADErrorResource;
static const XADError XADDecrunchError API_DEPRECATED_WITH_REPLACEMENT("XADErrorDecrunch", macosx(10.0, 10.8)) = XADErrorDecrunch;
static const XADError XADFiletypeError API_DEPRECATED_WITH_REPLACEMENT("XADErrorFiletype", macosx(10.0, 10.8)) = XADErrorFiletype;
static const XADError XADOpenFileError API_DEPRECATED_WITH_REPLACEMENT("XADErrorOpenFile", macosx(10.0, 10.8)) = XADErrorOpenFile;
static const XADError XADSkipError API_DEPRECATED_WITH_REPLACEMENT("XADErrorSkip", macosx(10.0, 10.8)) = XADErrorSkip;
static const XADError XADBreakError API_DEPRECATED_WITH_REPLACEMENT("XADErrorBreak", macosx(10.0, 10.8)) = XADErrorBreak;
static const XADError XADFileExistsError API_DEPRECATED_WITH_REPLACEMENT("XADErrorFileExists", macosx(10.0, 10.8)) = XADErrorFileExists;
static const XADError XADPasswordError API_DEPRECATED_WITH_REPLACEMENT("XADErrorPassword", macosx(10.0, 10.8)) = XADErrorPassword;
static const XADError XADMakeDirectoryError API_DEPRECATED_WITH_REPLACEMENT("XADErrorMakeDirectory", macosx(10.0, 10.8)) = XADErrorMakeDirectory;
static const XADError XADChecksumError API_DEPRECATED_WITH_REPLACEMENT("XADErrorChecksum", macosx(10.0, 10.8)) = XADErrorChecksum;
static const XADError XADVerifyError API_DEPRECATED_WITH_REPLACEMENT("XADErrorVerify", macosx(10.0, 10.8)) = XADErrorVerify;
static const XADError XADGeometryError API_DEPRECATED_WITH_REPLACEMENT("XADErrorGeometry", macosx(10.0, 10.8)) = XADErrorGeometry;
static const XADError XADDataFormatError API_DEPRECATED_WITH_REPLACEMENT("XADErrorDataFormat", macosx(10.0, 10.8)) = XADErrorDataFormat;
static const XADError XADEmptyError API_DEPRECATED_WITH_REPLACEMENT("XADErrorEmpty", macosx(10.0, 10.8)) = XADErrorEmpty;
static const XADError XADFileSystemError API_DEPRECATED_WITH_REPLACEMENT("XADErrorFileSystem", macosx(10.0, 10.8)) = XADErrorFileSystem;
static const XADError XADFileDirectoryError API_DEPRECATED_WITH_REPLACEMENT("XADErrorFileDirectory", macosx(10.0, 10.8)) = XADErrorFileDirectory;
static const XADError XADShortBufferError API_DEPRECATED_WITH_REPLACEMENT("XADErrorShortBuffer", macosx(10.0, 10.8)) = XADErrorShortBuffer;
static const XADError XADEncodingError API_DEPRECATED_WITH_REPLACEMENT("XADErrorEncoding", macosx(10.0, 10.8)) = XADErrorEncoding;
static const XADError XADLinkError API_DEPRECATED_WITH_REPLACEMENT("XADErrorLink", macosx(10.0, 10.8)) = XADErrorLink;

static const XADError XADSubArchiveError API_DEPRECATED_WITH_REPLACEMENT("XADErrorSubArchive", macosx(10.0, 10.8)) = XADErrorSubArchive;
