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
	@nonobjc open func linkDestination(for dict: [XADArchiveKeys : Any]) throws -> XADString {
		var err = XADError.none
		guard let linkDest = __linkDestination(for: dict, error: &err) else {
			throw err
		}
		return linkDest
	}
	
	/// - returns: `true` if the checksum is valid,
	/// `false` otherwise.
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
	
	/// Exception-free wrapper for subclass method
	/// Will throw `XADErrorBreak` if the delegate
	/// requested parsing to stop.
	@nonobjc open func parse() throws {
		let err = __parseWithoutExceptions()
		if err != .none {
			throw err
		}
	}
	
	/// Exception-free wrapper for subclass method
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
