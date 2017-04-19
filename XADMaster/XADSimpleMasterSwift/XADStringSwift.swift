//
//  XADStringSwift.swift
//  XADMaster
//
//  Created by C.W. Betts on 4/19/17.
//
//

import Foundation
import XADMaster.XADString

extension XADStringProtocol {
	public func canDecode(withEncoding encoding: String.Encoding) -> Bool {
		return __canDecode(withEncoding: encoding.rawValue)
	}
	
	public func string(withEncoding encoding: String.Encoding) -> String? {
		return __string(withEncoding: encoding.rawValue)
	}
	
	@nonobjc public var encoding: String.Encoding {
		return String.Encoding(rawValue: __encoding)
	}
}

extension XADString {
	@nonobjc open func canDecode(withEncoding encoding: String.Encoding) -> Bool {
		return __canDecode(withEncoding: encoding.rawValue)
	}
	
	@nonobjc open func string(withEncoding encoding: String.Encoding) -> String? {
		return __string(withEncoding: encoding.rawValue)
	}
	
	@nonobjc open var encoding: String.Encoding {
		return String.Encoding(rawValue: __encoding)
	}

	@nonobjc open class func encodingName(forEncoding encoding: String.Encoding) -> XADStringEncodingName {
		return __encodingName(forEncoding: encoding.rawValue)
	}
	
	@nonobjc open class func encoding(forEncodingName encoding: XADStringEncodingName) -> String.Encoding {
		return String.Encoding(rawValue: __encoding(forEncodingName: encoding))
	}
}

extension XADString /*: ExpressibleByStringLiteral*/ {
	@nonobjc public convenience init(stringLiteral value: String) {
		self.init(string: value)
	}
	
	@nonobjc public convenience init(extendedGraphemeClusterLiteral value: String) {
		self.init(string: String(extendedGraphemeClusterLiteral: value))
	}
	
	@nonobjc public convenience init(unicodeScalarLiteral value: String) {
		self.init(string: String(unicodeScalarLiteral: value))
	}

}

extension XADStringSource {
	@nonobjc open var encoding: String.Encoding {
		return String.Encoding(rawValue: __encoding)
	}
	
	@nonobjc open var fixedEncoding: String.Encoding {
		get {
			return String.Encoding(rawValue: __fixedEncoding)
		}
		set {
			__fixedEncoding = newValue.rawValue
		}
	}

}
