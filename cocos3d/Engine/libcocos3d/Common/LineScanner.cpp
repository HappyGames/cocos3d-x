/*
 * Cocos3D-X 1.0.0
 * Copyright (c) 2014-2015 Jason Wong
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

CLineScanner::CLineScanner()
{
	m_pBuffer = NULL;
	m_pDelegate = NULL;
	m_nBufferSize = 0;
	m_nCursorPos = 0;
}

CLineScanner::CLineScanner( char* buffer, unsigned long bufferSize, CLineScanDelegate* pDelegate )
{
	m_pBuffer = buffer;
	m_pDelegate = pDelegate;
	m_nBufferSize = bufferSize;
	m_nCursorPos = 0;
}

LineArray CLineScanner::scan( char* buffer, unsigned long bufferSize )
{
	m_pBuffer = buffer;
	m_nBufferSize = bufferSize;
	reset();

	LineArray lines;
	char strLine[512] = {0};
	while ( readLine( strLine ) != -1 )
	{
		std::string singleLine = strLine;
		CC3String::trim( singleLine );
		if ( !singleLine.empty() )
		{
			lines.push_back( strLine );

			if ( m_pDelegate )
			{
				m_pDelegate->onScanLine( strLine );
			}
		}
	}

	return lines;
}

void CLineScanner::reset()
{
	m_nCursorPos = 0;
}

void CLineScanner::setDelegate( CLineScanDelegate* pDelegate )
{
	m_pDelegate = pDelegate;
}

short CLineScanner::readChar()
{
	short s;
	if ( m_pBuffer == NULL )
		return -1;

	if ( m_nCursorPos+1 > m_nBufferSize )
		return -1;

	s = m_pBuffer[m_nCursorPos];
	m_nCursorPos++;

	return s;
}

long CLineScanner::readLine( char* bufOut, long max /* = MAX_LINE */ )
{
	if ( m_pBuffer == 0 )
		return 0;

	long s, n;
	char* bf;

loop:
	bf = bufOut;

	for ( n = 0; n < max; n++ )
	{
		s = readChar();
		if ( s == -1 )
			return -1;

		bf[n] = (char)s;
		if ( bf[n] == 0x0d || bf[n] == 0x0a )
		{
			bf[n] = 0;
			break;
		}
	}
	if ( n==0 )
		goto loop;

	return n;
}


NS_COCOS3D_END
