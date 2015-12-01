#import "CSByteStreamHandle.h"
#import "PPMd/VariantG.h"
#import "PPMd/VariantH.h"
#import "PPMd/VariantI.h"
#import "PPMd/SubAllocatorVariantG.h"
#import "PPMd/SubAllocatorVariantH.h"
#import "PPMd/SubAllocatorVariantI.h"
#import "PPMd/SubAllocatorBrimstone.h"

@interface XADPPMdVariantGHandle:CSByteStreamHandle
{
	PPMdModelVariantG model;
	PPMdSubAllocatorVariantG *alloc;
	int max;
}

-(instancetype)initWithHandle:(CSHandle *)handle maxOrder:(int)maxorder subAllocSize:(int)suballocsize;
-(instancetype)initWithHandle:(CSHandle *)handle length:(off_t)length maxOrder:(int)maxorder subAllocSize:(int)suballocsize;

-(void)resetByteStream;
-(uint8_t)produceByteAtOffset:(off_t)pos;

@end

@interface XADPPMdVariantHHandle:CSByteStreamHandle
{
	PPMdModelVariantH model;
	PPMdSubAllocatorVariantH *alloc;
	int max;
}

-(instancetype)initWithHandle:(CSHandle *)handle maxOrder:(int)maxorder subAllocSize:(int)suballocsize;
-(instancetype)initWithHandle:(CSHandle *)handle length:(off_t)length maxOrder:(int)maxorder subAllocSize:(int)suballocsize;

-(void)resetByteStream;
-(uint8_t)produceByteAtOffset:(off_t)pos;

@end

@interface XADPPMdVariantIHandle:CSByteStreamHandle
{
	PPMdModelVariantI model;
	PPMdSubAllocatorVariantI *alloc;
	int max,method;
}

-(instancetype)initWithHandle:(CSHandle *)handle maxOrder:(int)maxorder subAllocSize:(int)suballocsize modelRestorationMethod:(int)mrmethod;
-(instancetype)initWithHandle:(CSHandle *)handle length:(off_t)length maxOrder:(int)maxorder subAllocSize:(int)suballocsize modelRestorationMethod:(int)mrmethod;

-(void)resetByteStream;
-(uint8_t)produceByteAtOffset:(off_t)pos;

@end

@interface XADStuffItXBrimstoneHandle:CSByteStreamHandle
{
	PPMdModelVariantG model;
	PPMdSubAllocatorBrimstone *alloc;
	int max;
}

-(instancetype)initWithHandle:(CSHandle *)handle maxOrder:(int)maxorder subAllocSize:(int)suballocsize;
-(instancetype)initWithHandle:(CSHandle *)handle length:(off_t)length maxOrder:(int)maxorder subAllocSize:(int)suballocsize;

-(void)resetByteStream;
-(uint8_t)produceByteAtOffset:(off_t)pos;

@end

@interface XAD7ZipPPMdHandle:XADPPMdVariantHHandle

-(void)resetByteStream;

@end
