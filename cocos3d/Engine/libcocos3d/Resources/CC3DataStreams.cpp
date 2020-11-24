/*
 * Cocos3D-X 1.0.0
 * Author: Bill Hollings
 * Copyright (c) 2010-2014 The Brenwill Workshop Ltd. All rights reserved.
 * http://www.brenwill.com
 *
 * Copyright (c) 2014-2015 Jason Wang
 * http://www.cocos3dx.org/
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * http://en.wikipedia.org/wiki/MIT_License
 */
#include "cocos3d.h"

NS_COCOS3D_BEGIN

CC3DataReader::CC3DataReader()
{
	m_pData = NULL;
}

CC3DataReader::~CC3DataReader()
{
	CC_SAFE_RELEASE( m_pData );
}

void CC3DataReader::initOnData( CCData* data )
{
	m_pData = data;
	m_pData->retain();
	m_readRange = CCRangeMake(0, 0);
	m_isBigEndian = false;
	m_wasReadBeyondEOF = false;
}

CC3DataReader* CC3DataReader::readerOnData( CCData* data )
{
	CC3DataReader* pReader = new CC3DataReader;
	pReader->initOnData( data );

	return pReader;
}

unsigned long CC3DataReader::getPosition()
{
	return m_readRange.location; 
}

unsigned long CC3DataReader::getBytesRemaining()
{
	return m_pData->getSize() - m_readRange.location; 
}

bool CC3DataReader::readAll( unsigned int count, char* bytes )
{
	m_readRange.length = count;
	unsigned long endRange = m_readRange.maxRange();
	m_wasReadBeyondEOF |= (endRange > m_pData->getSize());
	if( !m_wasReadBeyondEOF )
	{
		bytes = (char*)m_pData->getBytes() + m_readRange.location;
		m_readRange.location = endRange;
		return true;
	} else {
		memset(bytes, 0, count);	// Couldn't read, so zero the result
		return false;
	}
}

char CC3DataReader::readByte()
{
	char value;
	readAll( sizeof(value), (char*)&value );
	return value;
}

unsigned char CC3DataReader::readUnsignedByte()
{
	unsigned char value;
	readAll( sizeof(value), (char*)&value );
	return value;
}

float CC3DataReader::readFloat()
{
	float value;
	readAll( sizeof(value), (char*)&value );
	//return _isBigEndian ? NSSwapBigFloatToHost(value) : NSSwapLittleFloatToHost(value);
	return value;
}

double CC3DataReader::readDouble()
{
	double value;
	readAll( sizeof(value), (char*)&value );
	//return _isBigEndian ? NSSwapBigDoubleToHost(value) : NSSwapLittleDoubleToHost(value);
	return value;
}

int CC3DataReader::readInteger()
{
	int value;
	readAll( sizeof(value), (char*)&value );
	//return _isBigEndian ? NSSwapBigIntToHost(value) : NSSwapLittleIntToHost(value);
	return value;
}

unsigned int CC3DataReader::readUnsignedInteger()
{
	unsigned int value;
	readAll( sizeof(value), (char*)&value );
	// return _isBigEndian ? NSSwapBigIntToHost(value) : NSSwapLittleIntToHost(value);
	return value;
}

short CC3DataReader::readShort()
{
	short value;
	readAll( sizeof(value), (char*)&value );
	//return _isBigEndian ? NSSwapBigShortToHost(value) : NSSwapLittleShortToHost(value);
	return value;
}

unsigned short CC3DataReader::readUnsignedShort()
{
	unsigned  short value;
	readAll( sizeof(value), (char*)&value );
	// return _isBigEndian ? NSSwapBigShortToHost(value) : NSSwapLittleShortToHost(value);
	return value;
}

NS_COCOS3D_END
