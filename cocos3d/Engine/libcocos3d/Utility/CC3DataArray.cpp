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

CC3DataArray::CC3DataArray()
{
	
}

CC3DataArray::~CC3DataArray()
{

}
	
unsigned int CC3DataArray::getElementSize()
{ 
	return MAX(m_elementSize, 1); 
}

void CC3DataArray::setElementSize( unsigned int elementSize )
{
	if (elementSize == getElementSize())
		return;
	unsigned int elemCap = getElementCapacity();		// Retrieve before changing size.
	m_elementSize = MAX(elementSize, 1);
	setElementCapacity( elemCap );
}

unsigned int CC3DataArray::getElementCapacity()
{ 
	return m_data.size() / getElementSize(); 
}

void CC3DataArray::setElementCapacity( unsigned int elementCapacity )
{
	if ( elementCapacity == getElementCapacity() ) 
		return;

	m_data.reserve( getElementSize() * elementCapacity );
}

void CC3DataArray::ensureElementCapacity( unsigned int  elementCapacity )
{
	if (elementCapacity > getElementCapacity()) 
		setElementCapacity( elementCapacity );
}

void* CC3DataArray::getElementAt( unsigned int index )
{
	return (char*)m_data.data() + getElementSize() * index;
}

bool CC3DataArray::isReady()
{
	return m_isReady;
}

void CC3DataArray::setIsReady( bool bReady )
{
	m_isReady = bReady;
}

void CC3DataArray::init()
{ 
	initWithElementSize( 1 ); 
}

void CC3DataArray::initWithElementSize( unsigned int elementSize )
{
	m_elementSize = elementSize;
	m_isReady = false;	
}

CC3DataArray* CC3DataArray::dataArrayWithElementSize( unsigned int elementSize )
{
	CC3DataArray* dataArray = new CC3DataArray;
	dataArray->initWithElementSize( elementSize );
	dataArray->autorelease();

	return dataArray;
}

NS_COCOS3D_END
