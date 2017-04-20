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
	
	@nonobjc public func forEntryWith(_ dict: [XADArchiveKeys : Any], wantChecksum checksum: Bool) throws -> XADUnarchiver {
		var err = XADError.none
		guard let unarch = __forEntryWith(dict, wantChecksum: checksum, error: &err) else {
			throw err
		}
		return unarch
	}
	
	@nonobjc public func forEntryWith(_ dict: [XADArchiveKeys : Any], resourceForkDictionary forkdict: [XADArchiveKeys : Any]?, wantChecksum checksum: Bool) throws -> XADUnarchiver {
		var err = XADError.none
		guard let unarch = __forEntryWith(dict, resourceForkDictionary: forkdict, wantChecksum: checksum, error: &err) else {
			throw err
		}
		return unarch
	}
}
