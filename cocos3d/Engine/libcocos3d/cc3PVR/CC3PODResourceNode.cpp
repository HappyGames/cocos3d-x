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

//-(Class) resourceClass { return [CC3PODResource class]; }

CC3PODResourceNode::CC3PODResourceNode()
{
	_animationFrameCount = 0;
	_animationFrameRate = 0;
}

CC3PODResourceNode::~CC3PODResourceNode()
{

}

void CC3PODResourceNode::initWithName( const std::string& aName )
{
	CC3Node::initWithName( aName );
}

bool CC3PODResourceNode::initWithName( const std::string& aName, const std::string& aFilePath )
{
	return super::initWithName(aName, aFilePath);
}


/** Overridden to extract the animation frame count and rate. */
void CC3PODResourceNode::populateFromResource( CC3NodesResource* resource )
{
	super::populateFromResource(resource);
	
	_animationFrameCount = ((CC3PODResource*)resource)->getAnimationFrameCount();
	_animationFrameRate = ((CC3PODResource*)resource)->getAnimationFrameRate();
}

void CC3PODResourceNode::populateFrom( CC3PODResourceNode* another )
{
	super::populateFrom(another);
	
	_animationFrameCount = another->getAnimationFrameCount();
	_animationFrameRate = another->getAnimationFrameRate();
}

CCObject* CC3PODResourceNode::copyWithZone( CCZone* zone )
{
	CC3PODResourceNode* pVal = new CC3PODResourceNode;
	pVal->init();
	pVal->populateFrom( this );
	pVal->addCopiesOfChildrenFrom( this );

	return pVal;
}

GLfloat CC3PODResourceNode::getAnimationFrameRate()
{
	return _animationFrameRate;
}

GLuint CC3PODResourceNode::getAnimationFrameCount()
{
	return _animationFrameCount;
}

CC3PODResourceNode* CC3PODResourceNode::nodeFromFile( const std::string& aFilepath )
{
	CC3PODResourceNode* pNode = new CC3PODResourceNode;
	if ( pNode->initFromFile( aFilepath ) )
	{
		pNode->autorelease();
		return pNode;
	}

	CC_SAFE_DELETE( pNode );
	return pNode;
}

CC3PODResourceNode* CC3PODResourceNode::nodeFromFile( const std::string& aFilepath, bool flipped )
{
	CC3PODResourceNode* pNode = new CC3PODResourceNode;
	if ( pNode->initFromFile( aFilepath, flipped ) )
	{
		pNode->autorelease();
		return pNode;
	}

	CC_SAFE_DELETE( pNode );
	return pNode;
}

CC3PODResourceNode* CC3PODResourceNode::nodeWithName( const std::string& aName )
{
	CC3PODResourceNode* pNode = new CC3PODResourceNode;
	pNode->initWithName( aName );
	pNode->autorelease();

	return pNode;
}

CC3PODResourceNode* CC3PODResourceNode::nodeWithName( const std::string& aName, const std::string& aFilePath )
{
	CC3PODResourceNode* pNode = new CC3PODResourceNode;
	if ( pNode->initWithName( aName, aFilePath ) )
	{
		pNode->autorelease();
		return pNode;
	}

	CC_SAFE_DELETE( pNode );
	return pNode;
}

CC3NodesResource* CC3PODResourceNode::createResourceFromFile( const std::string& aFilePath )
{
    CC3PODResource* pResource = CC3PODResource::resourceFromFile( aFilePath );
	return pResource;
}

CC3NodesResource* CC3PODResourceNode::createResourceFromFile( const std::string& aFilePath, bool flipped )
{
	CC3PODResource* pResource = CC3PODResource::resourceFromFile( aFilePath );
	if ( pResource )
		pResource->setExpectsVerticallyFlippedTextures( flipped );
	
	return pResource;
}

NS_COCOS3D_END
