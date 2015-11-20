/*
 * Cocos3D-X 1.0.0
 * Author: Bill Hollings
 * Copyright (c) 2010-2014 The Brenwill Workshop Ltd. All rights reserved.
 * http://www.brenwill.com
 *
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

void CC3NodeUpdatingVisitor::processBeforeChildren( CC3Node* aNode )
{
	//LogTrace(@"Updating %@ after %.3f ms", aNode, _deltaTime * 1000.0f);
	CC3PerformanceStatistics* pStatistics = getPerformanceStatistics();
	if ( pStatistics )
		pStatistics->incrementNodesUpdated();

	aNode->processUpdateBeforeTransform( this );

	// Process the transform AFTER updateBeforeTransform: invoked
	super::processBeforeChildren( aNode );
}

void CC3NodeUpdatingVisitor::processAfterChildren( CC3Node* aNode )
{
	aNode->processUpdateAfterTransform( this );
	super::processAfterChildren( aNode );
}

std::string CC3NodeUpdatingVisitor::fullDescription()
{
	/*return [NSString stringWithFormat: @"%@, dt: %.3f ms",
	[super fullDescription], _deltaTime * 1000.0f];*/
	return "CC3NodeUpdatingVisitor";
}

CC3NodeUpdatingVisitor* CC3NodeUpdatingVisitor::visitor()
{
	CC3NodeUpdatingVisitor* pV = new CC3NodeUpdatingVisitor;
	pV->init();
	pV->autorelease();

	return pV;
}

void CC3NodeUpdatingVisitor::setDeltaTime( float dt )
{
	m_fDeltaTime = dt;
}

float CC3NodeUpdatingVisitor::getDeltaTime()
{
	return m_fDeltaTime;
}

NS_COCOS3D_END
