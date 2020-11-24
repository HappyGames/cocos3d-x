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

CC3NodesResource::CC3NodesResource()
{
	m_nodes = NULL;
}

CC3NodesResource::~CC3NodesResource()
{
	CC_SAFE_RELEASE( m_nodes );
}

CCArray* CC3NodesResource::getNodes()
{
	return m_nodes;
}

CC3Node* CC3NodesResource::getNodeMatching( CC3Node* node )
{
	std::string nodeName = node->getName(); 

	CCObject* pObj = NULL;
	CCArray* nodes = getNodes();
	CCARRAY_FOREACH( nodes, pObj )
	{
		CC3Node* rezNode = (CC3Node*)pObj;
		CC3Node* matchedNode = rezNode->getNodeNamed( nodeName.c_str() );
		if ( matchedNode )
			return matchedNode;
	}
	
	return NULL;
}

void CC3NodesResource::addNode( CC3Node* node )
{
	m_nodes->addObject( node ); 
}

void CC3NodesResource::removeNode( CC3Node* node )
{
	m_nodes->removeObject( node ); 
}

bool CC3NodesResource::init()
{
	if ( super::init() )
	{
		m_nodes = CCArray::create();		// retained
		m_nodes->retain();
		m_expectsVerticallyFlippedTextures = defaultExpectsVerticallyFlippedTextures();
		m_shouldFreezeInanimateNodes = defaultShouldFreezeInanimateNodes();

		return true;
	}
	
	return false;
}

CC3NodesResource* CC3NodesResource::resourceFromFile( const std::string& aFilePath, bool flipped )
{
	CC3NodesResource* rez = (CC3NodesResource*)(getResourceNamed( resourceNameFromFilePath(aFilePath) ));
	if (rez) 
		return rez;
	
	rez = resource();								// autoreleased
	rez->setExpectsVerticallyFlippedTextures( flipped );
	if ( !rez->loadFromFile( aFilePath ) ) 
		rez = NULL;	// autoreleased
	addResource( rez );
	return rez;
}

bool CC3NodesResource::expectsVerticallyFlippedTextures()
{
	return m_expectsVerticallyFlippedTextures;
}

void CC3NodesResource::setExpectsVerticallyFlippedTextures( bool expects )
{
	m_expectsVerticallyFlippedTextures = expects;
}

static bool _defaultExpectsVerticallyFlippedTextures = false;

bool CC3NodesResource::defaultExpectsVerticallyFlippedTextures()
{
	return _defaultExpectsVerticallyFlippedTextures; 
}

void CC3NodesResource::setDefaultExpectsVerticallyFlippedTextures( bool expectsFlipped )
{
	_defaultExpectsVerticallyFlippedTextures = expectsFlipped;
}

static bool _defaultShouldFreezeInanimateNodes = false;

bool CC3NodesResource::defaultShouldFreezeInanimateNodes()
{
	return _defaultShouldFreezeInanimateNodes; 
}

void CC3NodesResource::setDefaultShouldFreezeInanimateNodes( bool shouldFreeze )
{
	_defaultShouldFreezeInanimateNodes = shouldFreeze;
}

bool CC3NodesResource::shouldFreezeInanimateNodes()
{
	return m_shouldFreezeInanimateNodes;
}

void CC3NodesResource::setShouldFreezeInanimateNodes( bool shouldFreeze )
{
	m_shouldFreezeInanimateNodes = shouldFreeze;
}

CC3NodesResource* CC3NodesResource::resource()
{
	CC3NodesResource* pRes = new CC3NodesResource;
	pRes->init();
	pRes->autorelease();
	return pRes;
}

NS_COCOS3D_END
