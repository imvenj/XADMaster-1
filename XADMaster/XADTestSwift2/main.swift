//
//  main.swift
//  XADTestSwift2
//
//  Created by C.W. Betts on 1/16/16.
//
//

import Foundation
import XADMaster.XADArchiveParser
import XADMaster.XADUtilities.CRC


var XADCRCTable_edb88320_Swift: [UInt32] = {
	var swiftHatesCArrays = XADCRCTable_edb88320
	var whatTuple = withUnsafePointer(&swiftHatesCArrays, { (largeTuple) -> UnsafePointer<UInt32> in
		return unsafeBitCast(largeTuple, UnsafePointer<UInt32>.self)
	})
	let buffered = UnsafeBufferPointer(start: whatTuple, count: 256)
	return Array(buffered)
}()

class TestDelegate: NSObject, XADArchiveParserDelegate {
	var count: Int32 = 0
	
	func archiveParser(parser: XADArchiveParser!, foundEntryWithDictionary dict: [NSObject : AnyObject]!) {
		print("Entry \(count++): \(dict)")
		
		do {
			let fh = try parser.handleForEntryWithDictionary(dict, wantChecksum: true)
		
			let data = fh.remainingFileContents()
		/*
		//	if(![dict objectForKey:XADIsResourceForkKey])
		if([[[dict objectForKey:XADCompressionNameKey] string] isEqual:@"LZMA+BCJ"])
		{
		NSMutableString *name=[NSMutableString stringWithString:[[dict objectForKey:XADFileNameKey] string]];
		[name replaceOccurrencesOfString:@"/" withString:@"_" options:0 range:NSMakeRange(0,[name length])];
		[data writeToFile:name atomically:YES];
		}
		*/
		
			var crc: UInt32 = 0
			var xor: UInt8 = 0;
		
			let bytes = UnsafePointer<UInt8>(data?.bytes ?? nil)
			let length = data?.length ?? 0
		if(bytes != nil)
		{
			crc=XADCalculateCRC(0xffffffff,bytes,length,XADCRCTable_edb88320_Swift)^0xffffffff;
			for i in 0..<length {
				xor^=bytes[i];
			}
		}
		
			print(String(format: "Checksum: %@, Length: %qd, CRC32: %08x, XOR: 0x%02x (%d)",
		fh.hasChecksum ? fh.checksumCorrect ? "Correct" : "Incorrect" : "Unknown",
		UInt64(data?.length ?? 0) ,crc, xor, xor))
		
				print(String(format:"\n%@", data.subdataWithRange(NSMakeRange(0, min(data.length, 256) ))))
		} catch _ {
			print("oops...")
		}
	}
	
	func archiveParsingShouldStop(parser: XADArchiveParser!) -> Bool {
		return false
	}
}

func figureOutPassword(fileName: String) -> String? {
	let envPass = getenv("XADTestPassword")
	if envPass != nil {
		return String.fromCString(envPass)
	}
	
	var matches = (fileName as NSString).substringsCapturedByPattern("_pass_(.+)\\.[pP][aA][rR][tT][0-9]+\\.[rR][aA][rR]$")
	if let matches = matches {
		return matches[1]
	}
	
	matches = (fileName as NSString).substringsCapturedByPattern("_pass_(.+)\\.[^.]+$")
	if let matches = matches {
		return matches[1]
	}

	return nil
}

/*
int main(int argc,char **argv)
{
for(int i=1;i<argc;i++)
{
NSAutoreleasePool *pool=[[NSAutoreleasePool alloc] init];

NSString *filename=[NSString stringWithUTF8String:argv[i]];
XADArchiveParser *parser=[XADArchiveParser archiveParserForPath:filename];

NSLog(@"Parsing file \"%@\" with parser \"%@\".",filename,[parser formatName]);

[parser setDelegate:[[TestDelegate new] autorelease]];

NSString *pass=FigureOutPassword(filename);
if(pass) [parser setPassword:pass];

NSLog(@"Archive format: \"%@\", properties: %@",[parser formatName],[parser properties]);
[parser parse];
NSLog(@"Archive format: \"%@\", properties: %@",[parser formatName],[parser properties]);

[pool release];
}
return 0;
}
*/


print("Hello, World!")

