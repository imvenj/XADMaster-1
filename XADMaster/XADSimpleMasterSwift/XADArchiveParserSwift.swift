//
//  XADArchiveSwift.swift
//  XADMaster
//
//  Created by C.W. Betts on 4/19/17.
//
//

import Foundation
import XADMaster.ArchiveParser

extension XADError: Error {
	public var _domain: String {
		return XADErrorDomain
	}
	
	public var _code: Int {
		return Int(rawValue)
	}
}

extension XADArchiveParser {
	@nonobjc public class func archiveParser(for handle: XADHandle, name: String) throws -> XADArchiveParser {
		var error = XADError.none
		if let archiveParse = XADArchiveParser(__for: handle, name: name, error: &error) {
			return archiveParse
		}
		throw error
	}
	
	@nonobjc public class func archiveParser(for handle: XADHandle, resourceFork fork: XADResourceFork?, name: String) throws -> XADArchiveParser {
		var error = XADError.none
		if let archiveParse = XADArchiveParser(__for: handle, resourceFork: fork, name: name, error: &error) {
			return archiveParse
		}
		throw error
	}

	@nonobjc public class func archiveParser(for handle: XADHandle, firstBytes header: Data, name: String) throws -> XADArchiveParser {
		var error = XADError.none
		if let archiveParse = XADArchiveParser(__for: handle, firstBytes: header, name: name, error: &error) {
			return archiveParse
		}
		throw error
	}
	
	@nonobjc public class func archiveParser(for handle: XADHandle, firstBytes header: Data, resourceFork fork: XADResourceFork?, name: String) throws -> XADArchiveParser {
		var error = XADError.none
		if let archiveParse = XADArchiveParser(__for: handle, firstBytes: header, resourceFork: fork, name: name, error: &error) {
			return archiveParse
		}
		throw error
	}

	@nonobjc public class func archiveParser(forPath filename: String) throws -> XADArchiveParser {
		var error = XADError.none
		if let archiveParse = XADArchiveParser(__forPath: filename, error: &error) {
			return archiveParse
		}
		throw error
	}
	
	@nonobjc public class func archiveParser(forEntryWith entry: [XADArchiveKeys : Any], archiveParser parser: XADArchiveParser, wantChecksum checksum: Bool) throws -> XADArchiveParser {
		var error = XADError.none
		if let archiveParse = XADArchiveParser(__forEntryWith: entry, archiveParser: parser, wantChecksum: checksum, error: &error) {
			return archiveParse
		}
		throw error
	}
	
	@nonobjc public class func archiveParser(forEntryWith entry: [XADArchiveKeys : Any], resourceForkDictionary forkentry: [XADArchiveKeys : Any]?, archiveParser parser: XADArchiveParser, wantChecksum checksum: Bool) throws -> XADArchiveParser {
		var error = XADError.none
		if let archiveParse = XADArchiveParser(__forEntryWith: entry, resourceForkDictionary: forkentry, archiveParser: parser, wantChecksum: checksum, error: &error) {
			return archiveParse
		}
		throw error
	}
}

extension XADArchiveParser {
	@nonobjc open func linkDestination(for dict: [XADArchiveKeys : Any]) throws -> XADString {
		var err = XADError.none
		guard let linkDest = __linkDestination(for: dict, error: &err) else {
			throw err
		}
		return linkDest
	}
	
	
	@nonobjc open func testChecksum() throws {
		let err = __testChecksumWithoutExceptions()
		if err != .none {
			throw err
		}
	}
	
	/// Will throw `XADBreakError` if the delegate
	/// requested parsing to stop.
	@nonobjc open func parse() throws {
		let err = __parseWithoutExceptions()
		if err != .none {
			throw err
		}
	}
	
	@nonobjc open func handleForEntry(with dict: [XADArchiveKeys : Any], wantChecksum checksum: Bool) throws -> XADHandle {
		var err = XADError.none
		guard let newHandle = __handleForEntry(with: dict, wantChecksum: checksum, error: &err) else {
			throw err
		}
		return newHandle
	}
	
	@available(*, deprecated, renamed: "testChecksum()")
	@nonobjc open func testChecksumWithoutExceptions() throws {
		try testChecksum()
	}
	
	@available(*, deprecated, renamed: "parse()")
	@nonobjc open func parseWithoutExceptions() throws {
		try parse()
	}
}
