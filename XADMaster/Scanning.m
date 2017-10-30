#import "Scanning.h"

@implementation CSHandle (Scanning)

-(BOOL)scanForByteString:(const void *)bytes2 length:(NSInteger)length
{
    return [self scanUsingMatchingBlock:^int(const uint8_t *bytes, size_t available, off_t offset) {
        if(available<length) return NO;
        return memcmp(bytes,bytes2,length)==0;
    } maximumLength:length];
}

-(int)scanUsingMatchingFunction:(CSByteMatchingFunctionPointer)function
maximumLength:(int)maximumlength
{
	return [self scanUsingMatchingFunction:function maximumLength:maximumlength context:NULL];
}

-(int)scanUsingMatchingFunction:(CSByteMatchingFunctionPointer)function
maximumLength:(int)maximumlength context:(void *)contextptr
{
	uint8_t buffer[65536];

	off_t pos=0;
	int actual=[self readAtMost:sizeof(buffer) toBuffer:buffer];

	while(actual>=maximumlength)
	{
		for(int i=0;i<=actual-maximumlength;i++)
		{
			int res=function(&buffer[i],actual-i,pos++,contextptr);
			if(res)
			{
				[self skipBytes:i-actual];
				return res;
			}
		}

		memcpy(buffer,&buffer[actual-maximumlength+1],maximumlength-1);
		actual=[self readAtMost:sizeof(buffer)-maximumlength+1 toBuffer:&buffer[maximumlength-1]]+maximumlength-1;
	}

	for(int i=0;i<actual;i++)
	{
		int res=function(&buffer[i],actual-i,pos++,contextptr);
		if(res)
		{
			[self skipBytes:i-actual];
			return res;
		}
	}

	return 0;
}

-(int)scanUsingMatchingBlock:(CSByteMatchingFunctionBlock)function
               maximumLength:(NSInteger)maximumlength
{
    uint8_t buffer[65536];
    
    off_t pos=0;
    int actual=[self readAtMost:sizeof(buffer) toBuffer:buffer];
    
    while(actual>=maximumlength)
    {
        for(int i=0;i<=actual-maximumlength;i++)
        {
            int res=function(&buffer[i],actual-i,pos++);
            if(res)
            {
                [self skipBytes:i-actual];
                return res;
            }
        }
        
        memcpy(buffer,&buffer[actual-maximumlength+1],maximumlength-1);
        actual=[self readAtMost:sizeof(buffer)-maximumlength+1 toBuffer:&buffer[maximumlength-1]]+maximumlength-1;
    }
    
    for(int i=0;i<actual;i++)
    {
        int res=function(&buffer[i],actual-i,pos++);
        if(res)
        {
            [self skipBytes:i-actual];
            return res;
        }
    }
    
    return 0;
}
@end

