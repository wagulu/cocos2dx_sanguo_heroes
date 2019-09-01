//
//  BufferReader
//
//  Created by user on 12-5-12.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef __CC_BUFFER_READER_H__
#define __CC_BUFFER_READER_H__

#include "CCBufferReader.h"
#include "base/CCValue.h"
#include <string>

NS_CC_BEGIN

enum Endian
{
    BIG_ENDIAN_MODE,
    LITTLE_ENDIAN_MODE
};

class BufferReader
{ 
public:
    
    unsigned char readByte();
	short readShort();
	int readInt();
    float readFloat();
    std::string* readString();
    
    BufferReader(unsigned char* buffer, int size, Endian endian);
    ~BufferReader();

private:
    int read();
private:
    static const int ERROR_OF_EOF = -1;
    Endian m_endian;
    unsigned char* m_buffer;
    int m_size;
    int m_currentPosition;
};


NS_CC_END

#endif
