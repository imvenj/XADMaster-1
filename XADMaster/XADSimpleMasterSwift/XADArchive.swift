//
//  XADArchive.swift
//  XADMaster
//
//  Created by C.W. Betts on 4/19/17.
//
//

import Foundation
import XADMaster.XADArchive

extension XADArchive {
	
	@nonobjc open func handle(forEntry n: Int) throws -> XADHandle {
		var err = XADError.none
		guard let hand = __handle(forEntry: n, error: &err) else {
			throw err
		}
		return hand
	}
	
	@nonobjc open func resourceHandle(forEntry n: Int) throws -> XADHandle {
		var err = XADError.none
		guard let hand = __resourceHandle(forEntry: n, error: &err) else {
			throw err
		}
		return hand
	}
	
	@nonobjc open var nameEncoding: String.Encoding {
		get {
			return String.Encoding(rawValue: __nameEncoding)
		}
		set {
			__nameEncoding = newValue.rawValue
		}
	}
	
	@nonobjc open func contents(ofEntry n: Int) throws -> Data {
		guard let dat = __contents(ofEntry: n) else {
			throw lastError
		}
		return dat
	}
}

extension XADArchive {
	@nonobjc public static func archive(with: Data, delegate: XADArchiveDelegate? = nil) throws -> XADArchive {
		var err = XADError.none
		guard let ar = XADArchive(data: with, delegate: delegate, error: &err) else {
			throw err
		}
		return ar
	}
	
	@nonobjc public static func archive(withPath path: String, delegate: XADArchiveDelegate? = nil) throws -> XADArchive {
		var err = XADError.none
		guard let ar = XADArchive(file: path, delegate: delegate, error: &err) else {
			throw err
		}
		return ar
	}
	
	@nonobjc public static func archive(with: XADArchive, entry: Int, delegate: XADArchiveDelegate? = nil) throws -> XADArchive {
		var err = XADError.none
		guard let ar = XADArchive(archive: with, entry: entry, delegate: delegate, error: &err) else {
			throw err
		}
		return ar
	}
	
	@nonobjc public static func archive(with: XADArchive, entry: Int, immediateExtractionTo dest: String, subArchives: Bool = false) throws -> XADArchive {
		var err = XADError.none
		guard let ar = XADArchive(archive: with, entry: entry, immediateExtractionTo: dest, subArchives: subArchives, error: &err) else {
			throw err
		}
		return ar
	}
}
