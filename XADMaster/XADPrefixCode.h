#import <Foundation/Foundation.h>
#import "CSInputBuffer.h"

extern NSString *const XADInvalidPrefixCodeException;

typedef struct XADCodeTreeNode XADCodeTreeNode;
typedef struct XADCodeTableEntry XADCodeTableEntry;

@interface XADPrefixCode:NSObject
{
	XADCodeTreeNode *tree;
	int numentries,minlength,maxlength;
	BOOL isstatic;

	int currnode;
	NSMutableArray *stack;

	int tablesize;
	XADCodeTableEntry *table1,*table2;
}

+(instancetype)prefixCode;
+(instancetype)prefixCodeWithLengths:(const int *)lengths numberOfSymbols:(int)numsymbols
maximumLength:(int)maxlength shortestCodeIsZeros:(BOOL)zeros;

-(instancetype)init NS_DESIGNATED_INITIALIZER;
-(instancetype)initWithStaticTable:(int (*)[2])statictable NS_DESIGNATED_INITIALIZER;
-(instancetype)initWithLengths:(const int *)lengths numberOfSymbols:(int)numsymbols
maximumLength:(int)maxlength shortestCodeIsZeros:(BOOL)zeros;

-(void)addValue:(int)value forCodeWithHighBitFirst:(uint32_t)code length:(int)length;
-(void)addValue:(int)value forCodeWithHighBitFirst:(uint32_t)code length:(int)length repeatAt:(int)repeatpos;
-(void)addValue:(int)value forCodeWithLowBitFirst:(uint32_t)code length:(int)length;
-(void)addValue:(int)value forCodeWithLowBitFirst:(uint32_t)code length:(int)length repeatAt:(int)repeatpos;

-(void)startBuildingTree;
-(void)startZeroBranch;
-(void)startOneBranch;
-(void)finishBranches;
-(void)makeLeafWithValue:(int)value;
-(void)_pushNode;
-(void)_popNode;

-(void)_makeTable;
-(void)_makeTableLE;

@end

int CSInputNextSymbolUsingCode(CSInputBuffer *buf,XADPrefixCode *code);
int CSInputNextSymbolUsingCodeLE(CSInputBuffer *buf,XADPrefixCode *code);
