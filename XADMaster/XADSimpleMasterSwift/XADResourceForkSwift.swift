//
//  XADResourceForkSwift.swift
//  XADMaster
//
//  Created by C.W. Betts on 4/19/17.
//
//

import Foundation
import XADMaster.ResourceFork

extension XADResourceFork {
	@nonobjc public class func resourceFork(with handle: XADHandle) throws -> XADResourceFork {
		var error = XADError.none
		guard let resFork = XADResourceFork(handle: handle, error: &error) else {
			throw error
		}
		return resFork
	}
}
