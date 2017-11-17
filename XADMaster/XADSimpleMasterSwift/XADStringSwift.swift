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
	
	public var encoding: String.Encoding {
		return String.Encoding(rawValue: __encoding)
	}
}

extension XADString {
	@nonobjc open class func encodingName(forEncoding encoding: String.Encoding) -> XADStringEncodingName {
		return __encodingName(forEncoding: encoding.rawValue)
	}
	
	@nonobjc open class func encoding(forEncodingName encoding: XADStringEncodingName) -> String.Encoding {
		return String.Encoding(rawValue: __encoding(forEncodingName: encoding))
	}
}

extension XADStringEncodingName {
	public init(forEncoding encoding: String.Encoding) {
		self = XADString.encodingName(forEncoding: encoding)
	}
	
	public var encoding: String.Encoding {
		return XADString.encoding(forEncodingName: self)
	}
}

// We can't have this conform to ExpressibleByStringLiteral because 
// 1. It can't be placed in the defining block because the defining block is Objective-C
// 2. The class can't be marked as final because it is an Objective-C class.
extension XADString /*: ExpressibleByStringLiteral*/ {
	@nonobjc public convenience init(stringLiteral value: String) {
		self.init(string: value)
	}
	
	@nonobjc public convenience init(extendedGraphemeClusterLiteral value: String) {
		self.init(stringLiteral: String(extendedGraphemeClusterLiteral: value))
	}
	
	@nonobjc public convenience init(unicodeScalarLiteral value: String) {
		self.init(stringLiteral: String(unicodeScalarLiteral: value))
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
