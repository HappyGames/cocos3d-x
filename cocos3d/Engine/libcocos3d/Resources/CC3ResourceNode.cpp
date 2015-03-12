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
#include "CC3ResourceNode.h"

NS_COCOS3D_BEGIN

CC3ResourceNode::CC3ResourceNode()
{

}

CC3ResourceNode::~CC3ResourceNode()
{

}

void CC3ResourceNode::populateFromResource( CC3NodesResource* resource )
{
	if ( !resource ) 
		return;
	
	/*CC3Assert( [resource isKindOfClass: self.resourceClass],
			  @"%@ must be populated from a resource of type %@. It cannot be populated from %@",
			  self, self.resourceClass, resource);*/
	
	if ( m_sName.empty() ) 
		setName( resource->getName() );  
	
	setUserData( resource->getUserData() );

	removeAllChildren();

	CCObject* pObj = NULL;
	CCARRAY_FOREACH( resource->getNodes(), pObj )
	{
		CC3Node* aNode = (CC3Node*)pObj;
		addChild( aNode );
	}
}

bool CC3ResourceNode::loadFromFile( const std::string& aFilepath )
{
	std::string aAbosultePath = CCFileUtils::sharedFileUtils()->fullPathForFilename( aFilepath.c_str() );
	CC3NodesResource* pRez = createResourceFromFile( aAbosultePath );
	if ( pRez )
	{
		populateFromResource( pRez );
		return true;
	}

	return false;
}

bool CC3ResourceNode::loadFromFile( const std::string& aFilepath, bool flipped )
{
	std::string aAbosultePath = CCFileUtils::sharedFileUtils()->fullPathForFilename( aFilepath.c_str() );
	CC3NodesResource* pRez = createResourceFromFile( aAbosultePath, flipped );
	if ( pRez )
	 {
		 populateFromResource( pRez );
		 return true;
	}

	return false;
}

bool CC3ResourceNode::initFromFile( const std::string& aFilepath )
{
	if ( super::init() )
		return loadFromFile( aFilepath );

	return false;
}

CC3ResourceNode* CC3ResourceNode::nodeFromFile( const std::string& aFilepath )
{
	CC3ResourceNode* pNode = new CC3ResourceNode;
	if ( pNode->initFromFile( aFilepath ) )
	{
		pNode->autorelease();
		return pNode;
	}

	CC_SAFE_DELETE( pNode );
	return pNode;
}

bool CC3ResourceNode::initFromFile( const std::string& aFilepath, bool flipped )
{
	return loadFromFile( aFilepath, flipped );
}

CC3ResourceNode* CC3ResourceNode::nodeFromFile( const std::string& aFilepath, bool flipped )
{
	CC3ResourceNode* pNode = new CC3ResourceNode;
	pNode->initFromFile( aFilepath, flipped );
	pNode->autorelease();

	return pNode;
}

bool CC3ResourceNode::initWithName( const std::string& aName, const std::string& aFilepath )
{
	super::initWithName( aName );
	return loadFromFile( aFilepath );
}

CC3ResourceNode* CC3ResourceNode::nodeWithName( const std::string& aName, const std::string& aFilepath )
{
	CC3ResourceNode* pNode = new CC3ResourceNode;
	pNode->initWithName( aName, aFilepath );
	pNode->autorelease();

	return pNode;
}

CC3NodesResource* CC3ResourceNode::createResourceFromFile( const std::string& aFilePath )
{
	CCAssert( false, "Subclasses must override this method" ); 
	return NULL;
}

CC3NodesResource* CC3ResourceNode::createResourceFromFile( const std::string& aFilePath, bool flipped )
{
	CCAssert( false, "Subclasses must override this method" ); 
	return NULL;
}

NS_COCOS3D_END
