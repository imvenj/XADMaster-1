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
	@nonobjc public class func archiveParser(for handle: XADHandle, resourceFork fork: XADResourceFork? = nil, name: String) throws -> XADArchiveParser {
		var error = XADError.none
		if let archiveParse = XADArchiveParser(__for: handle, resourceFork: fork, name: name, error: &error) {
			return archiveParse
		}
		throw error
	}

	@nonobjc public class func archiveParser(for handle: XADHandle, firstBytes header: Data, resourceFork fork: XADResourceFork? = nil, name: String) throws -> XADArchiveParser {
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
	
	@nonobjc public class func archiveParser(forEntryWith entry: [XADArchiveKeys : Any], resourceForkDictionary forkentry: [XADArchiveKeys : Any]? = nil, archiveParser parser: XADArchiveParser, wantChecksum checksum: Bool) throws -> XADArchiveParser {
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
	
	
	@nonobjc open func testChecksum() throws -> Bool {
		let err = __testChecksumWithoutExceptions()
		switch err {
		case .checksum:
			return false
			
		case .none:
			return true
			
		default:
			throw err
		}
	}
	
	/// Will throw `XADErrorBreak` if the delegate
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
		if try testChecksum() == false {
			// match the Objective-C method's behavior
			throw XADError.checksum
		}
	}
	
	@available(*, deprecated, renamed: "parse()")
	@nonobjc open func parseWithoutExceptions() throws {
		try parse()
	}
	
}
