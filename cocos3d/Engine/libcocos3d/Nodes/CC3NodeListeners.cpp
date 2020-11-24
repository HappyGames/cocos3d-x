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

CC3NodeTransformListeners::CC3NodeTransformListeners()
{
	m_pNode = NULL;
}

CC3NodeTransformListeners::~CC3NodeTransformListeners()
{
	/// TODO::Free all listeners
}

unsigned int CC3NodeTransformListeners::getListenerCount()
{
	return m_transformListenerWrappers.size();
}

bool CC3NodeTransformListeners::isEmpty()
{
	return getListenerCount() == 0;
}

TransformListenerList& CC3NodeTransformListeners::transformListeners()
{
	/// TODO: Create and return a array and add all listeners to this array
	return m_transformListenerWrappers;
}

void CC3NodeTransformListeners::addTransformListener( CC3NodeTransformListenerProtocol* listener )
{
	if ( !listener )
		return;

	m_transformListenerWrappers.push_back( listener );
}

void CC3NodeTransformListeners::removeTransformListener( CC3NodeTransformListenerProtocol* listener )
{
	for (unsigned int i = 0; i < m_transformListenerWrappers.size(); i++)
	{
		if ( m_transformListenerWrappers[i] == listener )
		{
			m_transformListenerWrappers.erase( m_transformListenerWrappers.begin() + i );
			break;
		}
	}
}

void CC3NodeTransformListeners::removeAllTransformListeners()
{
	m_transformListenerWrappers.clear();
}

void CC3NodeTransformListeners::notifyTransformListeners()
{
	for (unsigned int i = 0; i < m_transformListenerWrappers.size(); i++)
	{
		CC3NodeTransformListenerProtocol* listener = m_transformListenerWrappers[i];
		if ( listener )
			listener->nodeWasTransformed( m_pNode );
	}
}

void CC3NodeTransformListeners::notifyDestructionListeners()
{
	for (unsigned int i = 0; i < m_transformListenerWrappers.size(); i++)
	{
		CC3NodeTransformListenerProtocol* listener = m_transformListenerWrappers[i];
		if ( listener )
			listener->nodeWasDestroyed( m_pNode );
	}
}

void CC3NodeTransformListeners::initForNode( CC3Node* node )
{
	m_pNode = node;
}

CC3NodeTransformListeners* CC3NodeTransformListeners::listenersForNode( CC3Node* node )
{
	CC3NodeTransformListeners* pListeners = new CC3NodeTransformListeners;
	pListeners->initForNode( node );
	pListeners->autorelease();

	return pListeners;
}

NS_COCOS3D_END
