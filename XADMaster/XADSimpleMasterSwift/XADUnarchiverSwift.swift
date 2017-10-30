//
//  XADUnarchiverSwift.swift
//  XADMaster
//
//  Created by C.W. Betts on 4/19/17.
//
//

import Foundation
import XADMaster.Unarchiver

extension XADUnarchiver {
	@nonobjc public func parseAndUnarchive() throws {
		let error = __parseAndUnarchive()
		if error != .none {
			throw error
		}
	}
	
	@nonobjc public func forEntryWith(_ dict: [XADArchiveKeys : Any], resourceForkDictionary forkdict: [XADArchiveKeys : Any]? = nil, wantChecksum checksum: Bool) throws -> XADUnarchiver {
		return try __forEntryWith(dict, resourceForkDictionary: forkdict, wantChecksum: checksum)
	}
}
