/*
 * Cocos3D-X 1.0.0
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
#ifndef _CCLIB_CC3_LINE_SCANNER_H_
#define _CCLIB_CC3_LINE_SCANNER_H_

NS_COCOS3D_BEGIN

class CLineScanDelegate
{
public:
	virtual void		onScanLine( const std::string& lineScanned ) = 0;
};

typedef std::vector<std::string> LineArray;
class CLineScanner
{
public:
	CLineScanner();
	CLineScanner( char* buffer, unsigned long bufferSize, CLineScanDelegate* pDelegate = NULL );

public:
	LineArray			scan( char* buffer, unsigned long bufferSize );
	void				reset();
	void				setDelegate( CLineScanDelegate* pDelegate );

protected:
	long				readLine( char* bufOut, long max = 512 );
	short				readChar();

protected:
	char*				m_pBuffer;
	unsigned long		m_nBufferSize;
	unsigned long		m_nCursorPos;

	CLineScanDelegate*	m_pDelegate;
};


NS_COCOS3D_END

#endif
