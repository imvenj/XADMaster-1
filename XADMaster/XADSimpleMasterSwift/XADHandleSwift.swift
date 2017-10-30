//
//  XADHandleSwift.swift
//  XADMaster
//
//  Created by C.W. Betts on 10/30/17.
//

import Foundation
import XADMaster.Handle

extension XADHandle {
	@nonobjc public func readInt8() throws -> Int8 {
		var err: NSError? = nil
		let hi = __readInt8WithError(&err)
		if let err = err {
			throw err
		}
		return hi
	}
	
	@nonobjc public func readUInt8() throws -> UInt8 {
		var err: NSError? = nil
		let hi = __readUInt8WithError(&err)
		if let err = err {
			throw err
		}
		return hi
	}

	@nonobjc public func readInt16(bigEndian: Bool) throws -> Int16 {
		var err: NSError? = nil
		let hi = __readInt16(inBigEndianOrder: bigEndian, error: &err)
		if let err = err {
			throw err
		}
		return hi
	}
	
	@nonobjc public func readUInt16(bigEndian: Bool) throws -> UInt16 {
		var err: NSError? = nil
		let hi = __readUInt16(inBigEndianOrder: bigEndian, error: &err)
		if let err = err {
			throw err
		}
		return hi
	}

	@nonobjc public func readInt32(bigEndian: Bool) throws -> Int32 {
		var err: NSError? = nil
		let hi = __readInt32(inBigEndianOrder: bigEndian, error: &err)
		if let err = err {
			throw err
		}
		return hi
	}
	
	@nonobjc public func readUInt32(bigEndian: Bool) throws -> UInt32 {
		var err: NSError? = nil
		let hi = __readUInt32(inBigEndianOrder: bigEndian, error: &err)
		if let err = err {
			throw err
		}
		return hi
	}
	
	@nonobjc public func readInt64(bigEndian: Bool) throws -> Int64 {
		var err: NSError? = nil
		let hi = __readInt64(inBigEndianOrder: bigEndian, error: &err)
		if let err = err {
			throw err
		}
		return hi
	}
	
	@nonobjc public func readUInt64(bigEndian: Bool) throws -> UInt64 {
		var err: NSError? = nil
		let hi = __readUInt64(inBigEndianOrder: bigEndian, error: &err)
		if let err = err {
			throw err
		}
		return hi
	}

	@nonobjc public func readInt16BE() throws -> Int16 {
		return try readInt16(bigEndian: true)
	}
	
	@nonobjc public func readInt32BE() throws -> Int32 {
		return try readInt32(bigEndian: true)
	}

	@nonobjc public func readInt64BE() throws -> Int64 {
		return try readInt64(bigEndian: true)
	}
	
	@nonobjc public func readUInt16BE() throws -> UInt16 {
		return try readUInt16(bigEndian: true)
	}
	
	@nonobjc public func readUInt32BE() throws -> UInt32 {
		return try readUInt32(bigEndian: true)
	}
	
	@nonobjc public func readUInt64BE() throws -> UInt64 {
		return try readUInt64(bigEndian: true)
	}
	
	@nonobjc public func readInt16LE() throws -> Int16 {
		return try readInt16(bigEndian: false)
	}
	
	@nonobjc public func readInt32LE() throws -> Int32 {
		return try readInt32(bigEndian: false)
	}
	
	@nonobjc public func readInt64LE() throws -> Int64 {
		return try readInt64(bigEndian: false)
	}
	
	@nonobjc public func readUInt16LE() throws -> UInt16 {
		return try readUInt16(bigEndian: false)
	}
	
	@nonobjc public func readUInt32LE() throws -> UInt32 {
		return try readUInt32(bigEndian: false)
	}
	
	@nonobjc public func readUInt64LE() throws -> UInt64 {
		return try readUInt64(bigEndian: false)
	}

	@nonobjc public func readID() throws -> UInt32 {
		var err: NSError? = nil
		let hi = __readIDWithError(&err)
		if let err = err {
			throw err
		}
		return hi
	}
	
	@nonobjc public func readBits(_ bits: Int32) throws -> UInt32 {
		var err: NSError? = nil
		let hi = __readBits(bits, error:&err)
		if let err = err {
			throw err
		}
		return hi
	}

	@nonobjc public func readBitsLE(_ bits: Int32) throws -> UInt32 {
		var err: NSError? = nil
		let hi = __readBitsLE(bits, error:&err)
		if let err = err {
			throw err
		}
		return hi
	}
	
	@nonobjc public func readSignedBits(_ bits: Int32) throws -> Int32 {
		var err: NSError? = nil
		let hi = __readSignedBits(bits, error:&err)
		if let err = err {
			throw err
		}
		return hi
	}
	
	@nonobjc public func readSignedBitsLE(_ bits: Int32) throws -> Int32 {
		var err: NSError? = nil
		let hi = __readSignedBitsLE(bits, error:&err)
		if let err = err {
			throw err
		}
		return hi
	}
}
