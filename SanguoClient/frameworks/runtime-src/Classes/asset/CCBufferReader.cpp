//
//  BufferReader
//
//  Created by user on 12-5-12.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#include "CCBufferReader.h"
#include <stdio.h>
#include <stdlib.h>

NS_CC_BEGIN

BufferReader::BufferReader(unsigned char* buffer, int size, Endian endian):
    m_buffer(buffer),
    m_size(size),
    m_endian(endian),
    m_currentPosition(0)
{
}

BufferReader::~BufferReader()
{
}

int BufferReader::read()
{
    if(m_currentPosition < m_size)
    {
        unsigned char* pdata = m_buffer;
        pdata += m_currentPosition;
 
        unsigned  char data = *(pdata++);
        m_currentPosition++;
       
        return data;
    }
    return -1;    
}

unsigned char BufferReader::readByte()
{
    return read();
}

short BufferReader::readShort()
{
    int ch1 = read();
    int ch2 = read();
    
    if(m_endian == BIG_ENDIAN_MODE)
    {
        return (short)((ch1 << 8) | ch2);
    }
    else 
    {
       return (short)((ch2 << 8) | ch1);
    }    
}

int BufferReader::readInt()
{
    int ch1 = read();
    int ch2 = read();
    int ch3 = read();
    int ch4 = read();
    
    if(m_endian == BIG_ENDIAN_MODE)
    {
        return (int)((ch1 << 24) | (ch2 << 16) | (ch3 << 8) | ch4);
    }
    else 
    {
        return (int)((ch4 << 24) | (ch3 << 16) | (ch2 << 8) | ch1);
    }    
}

float BufferReader::readFloat()
{
    int value = readInt();
    float f;
    memcpy(&f, &value, sizeof(float));
    return f;
}

std::string* BufferReader::readString()
{
    unsigned int size = readShort();
    try 
    {
        char* str = new char[size + 1];
        for(int i = 0; i < size; i++)
        {
            str[i] = read();
        }
        str[size] = '\0';
        
        auto ret = new std::string(str);
        delete str;
        return ret;
    } 
    catch(const std::bad_alloc& e) 
    {
        printf("Malloc failed");
        return NULL;
    }    
}

NS_CC_END


