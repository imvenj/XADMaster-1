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
		return try XADArchive(data: with, delegate: delegate)
	}
	
	@nonobjc public static func archive(withPath path: String, delegate: XADArchiveDelegate? = nil) throws -> XADArchive {
		return try XADArchive(file: path, delegate: delegate)
	}
	
	@nonobjc public static func archive(with: XADArchive, entry: Int, delegate: XADArchiveDelegate? = nil) throws -> XADArchive {
		return try XADArchive(archive: with, entry: entry, delegate: delegate)
	}
	
	@nonobjc public static func archive(with: XADArchive, entry: Int, immediateExtractionTo dest: String, subArchives: Bool = false) throws -> XADArchive {
		return try XADArchive(archive: with, entry: entry, immediateExtractionTo: dest, subArchives: subArchives)
	}
}
