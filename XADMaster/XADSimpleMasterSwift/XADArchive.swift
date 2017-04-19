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
		var err = XADError.noError
		guard let hand = __handle(forEntry: n, error: &err) else {
			throw err
		}
		return hand
	}
	
	@nonobjc open func resourceHandle(forEntry n: Int) throws -> XADHandle! {
		var err = XADError.noError
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
}
