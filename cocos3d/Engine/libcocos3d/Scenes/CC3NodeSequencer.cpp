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

void CC3NodeEvaluator::init()
{

}

bool CC3NodeEvaluator::evaluate( CC3Node* aNode )
{
	return false; 
}

CC3NodeEvaluator* CC3NodeEvaluator::evaluator()
{
	CC3NodeEvaluator* pVal = new CC3NodeEvaluator;
	pVal->init();
	pVal->autorelease();

	CC3_WARNING( "creating default empty node evaluator, check this" );

	return pVal;
}

void CC3NodeEvaluator::populateFrom( CC3NodeEvaluator* another )
{

}

CCObject* CC3NodeEvaluator::copyWithZone( CCZone* zone )
{
	CC3NodeEvaluator* aCopy = new CC3NodeEvaluator;
	aCopy->populateFrom( this );
	return aCopy;
}

CC3NodeAcceptor* CC3NodeAcceptor::evaluator()
{
	CC3NodeAcceptor* pVal = new CC3NodeAcceptor;
	pVal->init();
	pVal->autorelease();

	return pVal;
}

bool CC3NodeAcceptor::evaluate( CC3Node* aNode )
{
	return true; 
}

CC3NodeRejector* CC3NodeRejector::evaluator()
{
	CC3NodeRejector* pVal = new CC3NodeRejector;
	pVal->init();
	pVal->autorelease();

	return pVal;
}

CC3LocalContentNodeEvaluator* CC3LocalContentNodeEvaluator::evaluator()
{
	CC3LocalContentNodeEvaluator* pVal = new CC3LocalContentNodeEvaluator;
	pVal->init();
	pVal->autorelease();

	return pVal;
}

bool CC3LocalContentNodeEvaluator::evaluate( CC3Node* aNode )
{
	if ( !aNode->hasLocalContent() ) 
		return false;

	return evaluateLocalContentNode( (CC3LocalContentNode*)aNode );
}

bool CC3LocalContentNodeEvaluator::evaluateLocalContentNode( CC3LocalContentNode* lcNode )
{
	return false; 
}

CC3LocalContentNodeAcceptor* CC3LocalContentNodeAcceptor::evaluator()
{
	CC3LocalContentNodeAcceptor* pVal = new CC3LocalContentNodeAcceptor;
	pVal->init();
	pVal->autorelease();

	return pVal;
}

bool CC3LocalContentNodeAcceptor::evaluateLocalContentNode( CC3LocalContentNode* lcNode )
{
	return true;
}

CC3OpaqueNodeAcceptor* CC3OpaqueNodeAcceptor::evaluator()
{
	CC3OpaqueNodeAcceptor* pVal = new CC3OpaqueNodeAcceptor;
	pVal->init();
	pVal->autorelease();

	return pVal;
}

bool CC3OpaqueNodeAcceptor::evaluateLocalContentNode( CC3LocalContentNode* lcNode )
{
	return lcNode->isOpaque(); 
}

CC3TranslucentNodeAcceptor* CC3TranslucentNodeAcceptor::evaluator()
{
	CC3TranslucentNodeAcceptor* pVal = new CC3TranslucentNodeAcceptor;
	pVal->init();
	pVal->autorelease();

	return pVal;
}

bool CC3TranslucentNodeAcceptor::evaluateLocalContentNode( CC3LocalContentNode* lcNode )
{
	return !lcNode->isOpaque(); 
}

CC3NodeSequencer::CC3NodeSequencer()
{
	_evaluator = NULL;
}

CC3NodeSequencer::~CC3NodeSequencer()
{
	CC_SAFE_RELEASE( _evaluator ); 
}

CCArray* CC3NodeSequencer::getNodes()
{
	return CCArray::create(); 
}

bool CC3NodeSequencer::allowSequenceUpdates()
{
	return _allowSequenceUpdates;
}

void CC3NodeSequencer::setAllowSequenceUpdates( bool allow )
{
	_allowSequenceUpdates = allow;
}

bool CC3NodeSequencer::shouldUseOnlyForwardDistance()
{
	return false; 
}

void CC3NodeSequencer::setShouldUseOnlyForwardDistance( bool onlyForward )
{

}

void CC3NodeSequencer::init()
{
	return initWithEvaluator( NULL );
}

CC3NodeSequencer* CC3NodeSequencer::sequencer()
{
	CC3NodeSequencer* pVal = new CC3NodeSequencer;
	pVal->init();
	pVal->autorelease();

	return pVal;
}

void CC3NodeSequencer::initWithEvaluator( CC3NodeEvaluator* anEvaluator )
{
	setEvaluator( anEvaluator );
	_allowSequenceUpdates = true;
}

CC3NodeSequencer* CC3NodeSequencer::sequencerWithEvaluator( CC3NodeEvaluator* anEvaluator )
{
	CC3NodeSequencer* pVal = new CC3NodeSequencer;
	pVal->initWithEvaluator( anEvaluator );
	pVal->autorelease();

	return pVal;
}

void CC3NodeSequencer::populateFrom( CC3NodeSequencer* another )
{
	_allowSequenceUpdates = another->allowSequenceUpdates();
}

CCObject* CC3NodeSequencer::copyWithZone( CCZone* zone )
{
	CC3NodeSequencer* aCopy = new CC3NodeSequencer;
	aCopy->initWithEvaluator( (CC3NodeEvaluator*)(_evaluator->copy()->autorelease()) );
	aCopy->populateFrom( this );
	return aCopy;
}

bool CC3NodeSequencer::add( CC3Node* aNode, CC3NodeSequencerVisitor* visitor )
{
	return false; 
}

bool CC3NodeSequencer::remove( CC3Node* aNode, CC3NodeSequencerVisitor* visitor )
{
	return false; 
}

bool CC3NodeSequencer::updateSequenceWithVisitor( CC3NodeSequencerVisitor* visitor )
{
	identifyMisplacedNodesWithVisitor( visitor );
	if (visitor->hasMisplacedNodes()) 
	{
		//LogTrace(@"%@ detected %lu misplaced nodes: %@",
		//		 self, (unsigned long)visitor.misplacedNodes.count, visitor.misplacedNodes);

		CCObject* pObj = NULL;
		CCARRAY_FOREACH( visitor->getMisplacedNodes(), pObj )
		{
			CC3Node* aNode = (CC3Node*)pObj;
			if ( remove( aNode, visitor ) )
				add( aNode, visitor );
		}
		
		visitor->clearMisplacedNodes();
		return true;
	}

	return false;
}

void CC3NodeSequencer::identifyMisplacedNodesWithVisitor( CC3NodeSequencerVisitor* visitor )
{

}

void CC3NodeSequencer::visitNodesWithNodeVisitor( CC3NodeVisitor* nodeVisitor )
{

}

void CC3NodeSequencer::setEvaluator( CC3NodeEvaluator* evaluator )
{
	CC_SAFE_RELEASE( _evaluator );
	CC_SAFE_RETAIN( evaluator );
	_evaluator = evaluator;
}

CC3BTreeNodeSequencer::CC3BTreeNodeSequencer()
{

}

CC3BTreeNodeSequencer::~CC3BTreeNodeSequencer()
{
	CC_SAFE_RELEASE( _sequencers );
}

void CC3BTreeNodeSequencer::initWithEvaluator( CC3NodeEvaluator* anEvaluator )
{
	super::initWithEvaluator( anEvaluator );
	_sequencers = CCArray::create();		// retained
	_sequencers->retain();
}

void CC3BTreeNodeSequencer::populateFrom( CC3BTreeNodeSequencer* another )
{
	super::populateFrom( another );

	CCArray* otherChildren = another->getSequencers();

	CCObject* pObj;
	CCARRAY_FOREACH( otherChildren, pObj )
	{
		CC3NodeSequencer* otherChild = (CC3NodeSequencer*)pObj;
		addSequencer( (CC3NodeSequencer*)(otherChild->copy()->autorelease()) ); 
	}
}

CCObject* CC3BTreeNodeSequencer::copyWithZone( CCZone* zone )
{
	CC3BTreeNodeSequencer* pVal = new CC3BTreeNodeSequencer;
	pVal->init();
	pVal->populateFrom( this );

	return pVal;
}

void CC3BTreeNodeSequencer::addSequencer( CC3NodeSequencer* aNodeSequencer )
{
	_sequencers->addObject( aNodeSequencer );
}

/** Iterates through the sequencers, adding it to the first one that accepts the node. */
bool CC3BTreeNodeSequencer::add( CC3Node* aNode, CC3NodeSequencerVisitor* visitor )
{
	if ( _evaluator && _evaluator->evaluate( aNode ) )
	{
		CCObject* pObj;
		CCARRAY_FOREACH( _sequencers, pObj )
		{
			CC3NodeSequencer* s = (CC3NodeSequencer*)pObj;
			if ( s->add( aNode, visitor ) )
			{
				return true;
			}
		}
	}

	return false;
}

/** Iterates through the sequencers, asking each to remove the node. */
bool CC3BTreeNodeSequencer::remove( CC3Node* aNode, CC3NodeSequencerVisitor* visitor )
{
	CCObject* pObj;
	CCARRAY_FOREACH( _sequencers, pObj )
	{
		CC3NodeSequencer* s = (CC3NodeSequencer*)pObj;
		if ( s->remove( aNode, visitor ) )
		{
			return true;
		}
	}

	return false;
}

/** Iterates through the sequencers, collecting misplaced nodes in the visitor. */
void CC3BTreeNodeSequencer::identifyMisplacedNodesWithVisitor( CC3NodeSequencerVisitor* visitor )
{
	if (_allowSequenceUpdates)
	{
		CCObject* pObj;
		CCARRAY_FOREACH( _sequencers, pObj )
		{
			CC3NodeSequencer* s = (CC3NodeSequencer*)pObj;
			s->identifyMisplacedNodesWithVisitor( visitor );
		}
	}
}

void CC3BTreeNodeSequencer::visitNodesWithNodeVisitor( CC3NodeVisitor* aNodeVisitor )
{
	CCObject* pObj;
	CCARRAY_FOREACH( _sequencers, pObj )
	{
		CC3NodeSequencer* s = (CC3NodeSequencer*)pObj;
		s->visitNodesWithNodeVisitor( aNodeVisitor );
	}
}

/** Concatenates the nodes from the contained sequencers into one array. */
CCArray* CC3BTreeNodeSequencer::getNodes()
{
	CCArray* nodes = CCArray::create();
	
	CCObject* pObj;
	CCARRAY_FOREACH( _sequencers, pObj )
	{
		CC3NodeSequencer* s = (CC3NodeSequencer*)pObj;
		nodes->addObjectsFromArray( s->getNodes() );
	}

	return nodes;
}

bool CC3BTreeNodeSequencer::shouldUseOnlyForwardDistance()
{
	CCObject* pObj;
	CCARRAY_FOREACH( _sequencers, pObj )
	{
		CC3NodeSequencer* s = (CC3NodeSequencer*)pObj;
		if ( s->shouldUseOnlyForwardDistance() )
			return true;
	}

	return false;
}

void CC3BTreeNodeSequencer::setShouldUseOnlyForwardDistance( bool onlyForward )
{
	CCObject* pObj;
	CCARRAY_FOREACH( _sequencers, pObj )
	{
		CC3NodeSequencer* s = (CC3NodeSequencer*)pObj;
		s->setShouldUseOnlyForwardDistance( onlyForward );
	}
}

CC3BTreeNodeSequencer* CC3BTreeNodeSequencer::sequencerLocalContentOpaqueFirst()
{
	CC3BTreeNodeSequencer* bTree = sequencerWithEvaluator( CC3LocalContentNodeAcceptor::evaluator() );
	bTree->addSequencer( CC3NodeArraySequencer::sequencerWithEvaluator( CC3OpaqueNodeAcceptor::evaluator() ) );
	bTree->addSequencer( CC3NodeArrayZOrderSequencer::sequencerWithEvaluator( CC3TranslucentNodeAcceptor::evaluator() ) );
	return bTree;
}

CC3BTreeNodeSequencer* CC3BTreeNodeSequencer::sequencerLocalContentOpaqueFirstGroupTextures()
{
	CC3BTreeNodeSequencer* bTree = sequencerWithEvaluator( CC3LocalContentNodeAcceptor::evaluator() );
	bTree->addSequencer( CC3MeshNodeArraySequencerGroupTextures::sequencerWithEvaluator( CC3OpaqueNodeAcceptor::evaluator() ) );
	bTree->addSequencer( CC3NodeArrayZOrderSequencer::sequencerWithEvaluator( CC3TranslucentNodeAcceptor::evaluator() ) );
	return bTree;
}

CC3BTreeNodeSequencer* CC3BTreeNodeSequencer::sequencerLocalContentOpaqueFirstGroupMeshes()
{
	CC3BTreeNodeSequencer* bTree = sequencerWithEvaluator( CC3LocalContentNodeAcceptor::evaluator() );
	bTree->addSequencer( CC3MeshNodeArraySequencerGroupMeshes::sequencerWithEvaluator( CC3OpaqueNodeAcceptor::evaluator() ) );
	bTree->addSequencer( CC3NodeArrayZOrderSequencer::sequencerWithEvaluator( CC3TranslucentNodeAcceptor::evaluator() ) );
	return bTree;
}

CC3BTreeNodeSequencer* CC3BTreeNodeSequencer::sequencerWithEvaluator( CC3NodeEvaluator* anEvaluator )
{
	CC3BTreeNodeSequencer* pVal = new CC3BTreeNodeSequencer;
	pVal->initWithEvaluator( anEvaluator );
	pVal->autorelease();

	return pVal;
}

CCArray* CC3BTreeNodeSequencer::getSequencers()
{
	return _sequencers;
}

CC3NodeArraySequencer::CC3NodeArraySequencer()
{
	_nodes = NULL;
}

CC3NodeArraySequencer::~CC3NodeArraySequencer()
{
	CC_SAFE_RELEASE( _nodes );
}

CCArray* CC3NodeArraySequencer::getNodes()
{
	return CCArray::createWithArray( _nodes ); 
}

void CC3NodeArraySequencer::initWithEvaluator( CC3NodeEvaluator* anEvaluator )
{
	super::initWithEvaluator( anEvaluator );
	_nodes = CCArray::create();		// retained
	_nodes->retain();
}

/**
 * Iterates through the existing nodes, passing them as sequential pairs, along with the node
 * to be added, to the shouldInsertNode:between:and:withVisitor: template method. If that method
 * returns YES, the node is inserted into the array at that point. If that method never returns
 * YES, the node is added at the end of the array. Returns whether the node was added.
 */
bool CC3NodeArraySequencer::add( CC3Node* aNode, CC3NodeSequencerVisitor* visitor )
{
	if ( _evaluator && _evaluator->evaluate( aNode ) ) 
	{
		unsigned int nodeCount = _nodes->count();
		for (unsigned int i = 0; i < nodeCount; i++) 
		{
			CC3Node* leftNode = i > 0 ? (CC3Node*)_nodes->objectAtIndex( i - 1 ) : NULL;
			CC3Node* rightNode = (CC3Node*)_nodes->objectAtIndex( i );
			CCAssert(aNode != rightNode, "CC3NodeArraySequenceralready contains node aNode!");
			if ( shouldInsertNode( aNode, leftNode, rightNode, visitor ) ) 
			{
				_nodes->insertObject( aNode, i );
				return true;
			}
		}
		_nodes->addObject( aNode );
		return true;
	}

	return false;
}

bool CC3NodeArraySequencer::shouldInsertNode( CC3Node* aNode, CC3Node* leftNode, CC3Node* rightNode, CC3NodeSequencerVisitor* visitor )
{
	return false;
}

bool CC3NodeArraySequencer::remove( CC3Node* aNode, CC3NodeSequencerVisitor* visitor )
{
	unsigned int nodeIndex = _nodes->indexOfObject( aNode );
	if (nodeIndex != -1) 
	{
		_nodes->removeObjectAtIndex( nodeIndex );
		return true;
	}
	return false;
}

void CC3NodeArraySequencer::identifyMisplacedNodesWithVisitor( CC3NodeSequencerVisitor* visitor )
{
	// Leave if sequence updating should not happen or if there is nothing to sort.
	if (!_allowSequenceUpdates || _nodes->count() == 0) 
		return;

	CCObject* pObj;
	CCARRAY_FOREACH( _nodes, pObj )
	{
		CC3Node* aNode = (CC3Node*)pObj;
		if ( !(_evaluator && _evaluator->evaluate( aNode )) )
			visitor->addMisplacedNode( aNode );
	}
}

void CC3NodeArraySequencer::visitNodesWithNodeVisitor( CC3NodeVisitor* aNodeVisitor )
{
	CCObject* pObj;
	CCARRAY_FOREACH( _nodes, pObj )
	{
		CC3Node* aNode = (CC3Node*)pObj;
		aNodeVisitor->visit( aNode );
	}
}

CC3NodeArraySequencer* CC3NodeArraySequencer::sequencerWithEvaluator( CC3NodeEvaluator* anEvaluator )
{
	CC3NodeArraySequencer* pSequencer = new CC3NodeArraySequencer;
	pSequencer->initWithEvaluator( anEvaluator );
	pSequencer->autorelease();

	return pSequencer;
}

void CC3NodeArrayZOrderSequencer::initWithEvaluator( CC3NodeEvaluator* anEvaluator )
{
	super::initWithEvaluator( anEvaluator );
	_shouldUseOnlyForwardDistance = false;
}

void CC3NodeArrayZOrderSequencer::populateFrom( CC3NodeArrayZOrderSequencer* another )
{
	super::populateFrom( another );
	_shouldUseOnlyForwardDistance = another->shouldUseOnlyForwardDistance();
}

CCObject* CC3NodeArrayZOrderSequencer::copyWithZone( CCZone* zone )
{
	CC3NodeArrayZOrderSequencer* pVal = new CC3NodeArrayZOrderSequencer;
	pVal->init();
	pVal->populateFrom( this );

	return pVal;
}

/**
 * If either the Z-order of the node of interest is greater than the Z-order of the rightNode,
 * or the Z-order is equal, and the distance from the node of interest to the camera is greater
 * than the distance from the rightNode to the camera, return YES, otherwise return NO.
 * 
 * Since the array is traversed from front to back, the node will have already been tested
 * against the leftNode. Nodes without a boundingVolume are added to the end of the array.
 */
bool CC3NodeArrayZOrderSequencer::shouldInsertNode( CC3Node* aNode, CC3Node* leftNode, CC3Node* rightNode, CC3NodeSequencerVisitor* visitor )
{
	// Check explicit Z-Order first. It overrides camera distance.
	if (aNode->getZOrder() > rightNode->getZOrder()) 
		return true;

	if (aNode->getZOrder() < rightNode->getZOrder()) 
		return false;

	// Next check camera distance based on bounding volume centers.
	return (aNode->getCameraDistanceProduct() >= rightNode->getCameraDistanceProduct());
}

/**
 * Identify nodes that do not pass the evaluator or are not in the correct Z-order sequence.
 * Any node whose distance to the camera is greater than the nodes before it is considered misplaced.
 */
void CC3NodeArrayZOrderSequencer::identifyMisplacedNodesWithVisitor( CC3NodeSequencerVisitor* visitor )
{
	// Leave if sequence updating should not happen or if there is nothing to sort.
	if (!_allowSequenceUpdates || _nodes->count() == 0) 
		return;

	CC3Camera* cam = visitor->getScene()->getActiveCamera();
	if (!cam) 
		return;		// Can't do anything without a camera.

	CC3Vector camGlobalLoc = cam->getGlobalLocation();
	GLint prevZOrder = kCC3MaxGLint;
	GLfloat prevCamDistProduct = kCC3MaxGLfloat;

	CCObject* pObj;
	CCARRAY_FOREACH( _nodes, pObj )
	{
		CC3Node* aNode = (CC3Node*)pObj;
		if ( !(_evaluator && _evaluator->evaluate( aNode )) )
			visitor->addMisplacedNode( aNode );
		else 
		{
			// Get vector from node's center of geometry to camera.
			CC3Vector node2Cam = CC3VectorDifference(aNode->getGlobalCenterOfGeometry(), camGlobalLoc);

			// Determine the direction in which to measure from the camera. This will either be
			// in the direction of a straight line between the camera and the node, or will be
			// restricted to the direction "staight-out" from the camera.
			CC3Vector measureDir = _shouldUseOnlyForwardDistance ? cam->getForwardDirection() : node2Cam;

			// Cache the dot product of the direction vector, and the vector between the node
			// and the camera. This is a relative measure of the distance in that direction.
			// In the case of measuring along the line between the node and camera, it will be
			// the square of the distance. Comparing the squares of the distance instead of the
			// distance itself also has the benefit of avoiding expensive square-root calculations.
			GLfloat camDistProd = CC3VectorDot(node2Cam, measureDir);
			aNode->setCameraDistanceProduct( camDistProd );

			// If this node is closer than the previous node in the array, update the
			// previous Z-order and distance value. Otherwise, mark the node as misplaced.
			// Explicit Z-order overrides actual distance.
			if ( aNode->getZOrder() < prevZOrder ) 
			{
				prevZOrder = aNode->getZOrder();
				prevCamDistProduct = camDistProd;
			} else if ( aNode->getZOrder() > prevZOrder )
				visitor->addMisplacedNode( aNode );
			else if ( camDistProd <= prevCamDistProduct ) {
				prevZOrder = aNode->getZOrder();
				prevCamDistProduct = camDistProd;
			} else
				visitor->addMisplacedNode( aNode );
		}
	}
}

bool CC3MeshNodeArraySequencer::add( CC3Node* aNode, CC3NodeSequencerVisitor* visitor )
{
	return aNode->isMeshNode() && super::add( aNode, visitor );
}

/**
 * Nodes are always instances of CC3MeshNode, so cast them as such
 * and invoke the shouldInsertMeshNode:between:and:withVisitor: template method.
 */
bool CC3MeshNodeArraySequencer::shouldInsertNode( CC3Node* aNode, CC3Node* leftNode, CC3Node* rightNode, CC3NodeSequencerVisitor* visitor )
{
	return shouldInsertMeshNode( (CC3MeshNode*)aNode, (CC3MeshNode*)leftNode, (CC3MeshNode*)rightNode, visitor );
}

bool CC3MeshNodeArraySequencer::shouldInsertMeshNode( CC3MeshNode* aNode, CC3MeshNode* leftNode, CC3MeshNode* rightNode, CC3NodeSequencerVisitor* visitor )
{
	return false; 
}


bool CC3MeshNodeArraySequencerGroupTextures::shouldInsertMeshNode( CC3MeshNode* aNode, CC3MeshNode* leftNode, CC3MeshNode* rightNode, CC3NodeSequencerVisitor* visitor )
{
	//if just starting, skip because we never insert at beginning
	if (leftNode == NULL) 
		return false;
	
	// If the left texture is the same, but the right one isn't,
	// this is where we want to insert to keep like textures together.
	CC3Texture* tex = aNode->getTexture();
	CC3Texture* leftTex = leftNode->getTexture();
	CC3Texture* rightTex = rightNode->getTexture();
	return (tex == leftTex && tex != rightTex);
}

bool CC3MeshNodeArraySequencerGroupMeshes::shouldInsertMeshNode( CC3MeshNode* aNode, CC3MeshNode* leftNode, CC3MeshNode* rightNode, CC3NodeSequencerVisitor* visitor )
{	
	//if just starting, skip because we never insert at beginning
	if (leftNode == NULL) 
		return false;
	
	// If the left mesh is the same, but the right one isn't,
	// this is where we want to insert to keep like meshes together.
	CC3Mesh* mesh = aNode->getMesh();
	CC3Mesh* leftMesh = leftNode->getMesh();
	CC3Mesh* rightMesh = rightNode->getMesh();
	return (mesh == leftMesh && mesh != rightMesh);
}

CC3NodeSequencerVisitor::CC3NodeSequencerVisitor()
{
	_misplacedNodes = NULL;
	_scene = NULL;
}

CC3NodeSequencerVisitor::~CC3NodeSequencerVisitor()
{
	_scene = NULL;
	CC_SAFE_RELEASE( _misplacedNodes );
}

CC3Scene* CC3NodeSequencerVisitor::getScene()
{
	return _scene;
}

void CC3NodeSequencerVisitor::setScene( CC3Scene* scene )
{
	_scene = scene;
}

void CC3NodeSequencerVisitor::init()
{
	return initWithScene( NULL );
}

void CC3NodeSequencerVisitor::initWithScene( CC3Scene* aCC3Scene )
{
	_scene = aCC3Scene;							// weak reference
	_misplacedNodes = CCArray::create();		// retained
	_misplacedNodes->retain();
}

CC3NodeSequencerVisitor* CC3NodeSequencerVisitor::visitorWithScene( CC3Scene* aCC3Scene )
{
	CC3NodeSequencerVisitor* visitor = new CC3NodeSequencerVisitor;
	visitor->initWithScene( aCC3Scene );
	visitor->autorelease();

	return visitor;
}

bool CC3NodeSequencerVisitor::hasMisplacedNodes()
{
	return (_misplacedNodes->count() > 0); 
}

void CC3NodeSequencerVisitor::addMisplacedNode( CC3Node* aNode )
{ 
	_misplacedNodes->addObject( aNode ); 
}

void CC3NodeSequencerVisitor::clearMisplacedNodes()
{
	_misplacedNodes->removeAllObjects(); 
}

CCArray* CC3NodeSequencerVisitor::getMisplacedNodes()
{
	return _misplacedNodes;
}

NS_COCOS3D_END
