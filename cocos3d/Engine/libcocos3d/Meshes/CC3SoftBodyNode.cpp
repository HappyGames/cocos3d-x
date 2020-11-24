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

/** Attaches any contained skin sections to the new skeleton copy under this soft body node. */
void CC3SoftBodyNode::addCopiesOfChildrenFrom( CC3Node* another )
{
	super::addCopiesOfChildrenFrom( another );
	reattachBonesFrom( this );
}

CC3SoftBodyNode* CC3SoftBodyNode::getSoftBodyNode()
{
	return this; 
}

CC3Vector CC3SoftBodyNode::getSkeletalScale()
{
	return CC3Vector::kCC3VectorUnitCube; 
}

CC3SoftBodyNode* CC3SoftBodyNode::nodeWithName( const std::string& aName )
{
	CC3SoftBodyNode* pNode = new CC3SoftBodyNode;
	pNode->initWithName( aName );
	pNode->autorelease();

	return pNode;
}

CCObject* CC3SoftBodyNode::copyWithZone( CCZone* zone )
{
	CC3SoftBodyNode* pNode = new CC3SoftBodyNode;
	pNode->init();
	pNode->populateFrom( this );
	pNode->addCopiesOfChildrenFrom( this );

	return pNode;
}

NS_COCOS3D_END
