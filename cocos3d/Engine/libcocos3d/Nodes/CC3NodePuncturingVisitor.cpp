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

CC3NodePuncture::CC3NodePuncture()
{
	_node = NULL;
}

CC3NodePuncture::~CC3NodePuncture()
{
	CC_SAFE_RELEASE( _node );
}

CC3Node* CC3NodePuncture::getNode()
{
	return _node;
}

CC3Vector CC3NodePuncture::getPunctureLocation()
{
	return _punctureLocation;
}

CC3Vector CC3NodePuncture::getGlobalPunctureLocation()
{
	return _globalPunctureLocation;
}

GLfloat CC3NodePuncture::getSQGlobalPunctureDistance()
{
	return _sqGlobalPunctureDistance;
}

void CC3NodePuncture::initOnNode( CC3Node* aNode, const CC3Ray& aRay )
{
	_node = aNode;
	_node->retain();
	_punctureLocation = aNode->getLocationOfGlobalRayIntesection( aRay );
	_globalPunctureLocation = aNode->getGlobalTransformMatrix()->transformLocation( _punctureLocation );
	_sqGlobalPunctureDistance = _globalPunctureLocation.distanceSquared( aRay.startLocation );
}

CC3NodePuncture* CC3NodePuncture::punctureOnNode( CC3Node* aNode, const CC3Ray& aRay )
{
	CC3NodePuncture* pPuncture = new CC3NodePuncture;
	pPuncture->initOnNode( aNode, aRay );
	pPuncture->autorelease();

	return pPuncture;
}

CC3NodePuncturingVisitor::CC3NodePuncturingVisitor()
{

}

CC3NodePuncturingVisitor::~CC3NodePuncturingVisitor()
{
	_nodePunctures->release();
}

CC3NodePuncture* CC3NodePuncturingVisitor::getNodePunctureAt( unsigned int index )
{
	return (CC3NodePuncture*)_nodePunctures->objectAtIndex( index );
}

unsigned int CC3NodePuncturingVisitor::getNodeCount()
{ 
	return _nodePunctures->count();
}

CC3Node* CC3NodePuncturingVisitor::getPuncturedNodeAt( unsigned int index )
{
	return getNodePunctureAt( index )->getNode(); 
}

CC3Node* CC3NodePuncturingVisitor::getClosestPuncturedNode()
{ 
	return getNodeCount() > 0 ? getPuncturedNodeAt( 0 ) : NULL; 
}

CC3Vector CC3NodePuncturingVisitor::getPunctureLocationAt( unsigned int index )
{
	return getNodePunctureAt( index )->getPunctureLocation();
}

CC3Vector CC3NodePuncturingVisitor::getClosestPunctureLocation()
{
	return getNodeCount() > 0 ? getPunctureLocationAt(0) : CC3Vector::kCC3VectorNull;
}

CC3Vector CC3NodePuncturingVisitor::getGlobalPunctureLocationAt( unsigned int index )
{
	return getNodePunctureAt( index )->getGlobalPunctureLocation();
}

CC3Vector CC3NodePuncturingVisitor::getClosestGlobalPunctureLocation()
{
	return getNodeCount() > 0 ? getGlobalPunctureLocationAt( 0 ) : CC3Vector::kCC3VectorNull;
}

void CC3NodePuncturingVisitor::open()
{
	super::open();
	_nodePunctures->removeAllObjects();
}

/**
 * Utility method that returns whether the specified node is punctured by the ray.
 *   - Returns NO if the node has no bounding volume.
 *   - Returns NO if the node is invisible, unless the shouldPunctureInvisibleNodes property
 *     has been set to YES.
 *   - Returns NO if the ray starts within the bounding volume, unless the 
 *     shouldPunctureFromInside property has been set to YES.
 */
bool CC3NodePuncturingVisitor::doesPuncture( CC3Node* aNode )
{
	CC3BoundingVolume* bv = aNode->getBoundingVolume();
	if ( !bv ) 
		return false;
	if ( !_shouldPunctureInvisibleNodes && !aNode->isVisible() ) 
		return false;
	if ( !_shouldPunctureFromInside && bv->doesIntersectLocation( _ray.startLocation ) ) 
		return false;
	return bv->doesIntersectRay( _ray );
}

void CC3NodePuncturingVisitor::processBeforeChildren( CC3Node* aNode )
{
	if ( doesPuncture( aNode ) ) 
	{
		CC3NodePuncture* np = CC3NodePuncture::punctureOnNode( aNode, _ray );
		unsigned int nodeCount = _nodePunctures->count();
		for (unsigned int i = 0; i < nodeCount; i++) 
		{
			CC3NodePuncture* existNP = (CC3NodePuncture*)_nodePunctures->objectAtIndex( i );
			if (np->getSQGlobalPunctureDistance() < existNP->getSQGlobalPunctureDistance()) {
				_nodePunctures->insertObject( np, i );
				return;
			}
		}
		_nodePunctures->addObject( np );
	}
}

void CC3NodePuncturingVisitor::init()
{ 
	return initWithRay( CC3Ray(CC3Vector::kCC3VectorNull, CC3Vector::kCC3VectorNull) );
}

void CC3NodePuncturingVisitor::initWithRay( const CC3Ray& aRay )
{
	super::init();
	_ray = aRay;
	_nodePunctures = CCArray::create();
	_nodePunctures->retain();
	_shouldPunctureFromInside = false;
	_shouldPunctureInvisibleNodes = false;
}

CC3NodePuncturingVisitor* CC3NodePuncturingVisitor::visitorWithRay( const CC3Ray& aRay )
{ 
	CC3NodePuncturingVisitor* pVisitor = new CC3NodePuncturingVisitor;
	pVisitor->initWithRay( aRay );
	pVisitor->autorelease();

	return pVisitor;
}

NS_COCOS3D_END
