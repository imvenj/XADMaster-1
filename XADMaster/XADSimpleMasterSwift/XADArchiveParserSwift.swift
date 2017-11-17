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

extension XADError: CustomStringConvertible {
	public var description: String {
		if let errDesc = XADDescribeError(self) {
			return errDesc
		}
		if self == .none {
			return "No Error"
		}
		return "Unknown error \(rawValue)"
	}
}

extension XADArchiveParser {
	/// - returns: `true` if the checksum is valid,
	/// `false` otherwise.
	/// Throws if there was a failure.
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
