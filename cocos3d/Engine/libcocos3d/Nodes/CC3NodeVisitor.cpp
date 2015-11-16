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

CC3NodeVisitor::CC3NodeVisitor()
{
	_currentNode = NULL;
	_pendingRemovals = NULL;
	_camera = NULL;
	_startingNode = NULL;
}

CC3NodeVisitor::~CC3NodeVisitor()
{
	_startingNode = NULL;			// weak reference
	_currentNode = NULL;			// weak reference
	CC_SAFE_RELEASE( _camera );
	CC_SAFE_RELEASE( _pendingRemovals );
}

void CC3NodeVisitor::init()
{
    _currentNode = NULL;
    _startingNode = NULL;
    _camera = NULL;
    _pendingRemovals = NULL;
    _shouldVisitChildren = true;
}

CC3NodeVisitor* CC3NodeVisitor::visitor()
{
    CC3NodeVisitor* pV = new CC3NodeVisitor;
    pV->init();
    pV->autorelease();
    
    return pV;
}

std::string CC3NodeVisitor::fullDescription()
{
    /*return [NSString stringWithFormat: @"%@ visiting %@ %@ children, %lu removals",
     [self description], _startingNode, (_shouldVisitChildren ? @"and" : @"but not"),
     (unsigned long)_pendingRemovals.count];*/
    return "CC3NodeVisitor";
}

CC3PerformanceStatistics* CC3NodeVisitor::getPerformanceStatistics()
{ 
	return _startingNode->getPerformanceStatistics(); 
}

CC3Camera* CC3NodeVisitor::getCamera() 
{
	if ( !_camera ) 
		setCamera( getDefaultCamera() );

	return _camera;
}

void CC3NodeVisitor::setCamera( CC3Camera* camera )
{
	if ( camera == _camera )
		return;

	CC_SAFE_RELEASE(_camera);
	_camera = camera;
	CC_SAFE_RETAIN(camera);
}

CC3Camera* CC3NodeVisitor::getDefaultCamera()
{ 
	return _startingNode->getActiveCamera();
}

CC3Node* CC3NodeVisitor::getCurrentNode()
{
	return _currentNode;
}

bool CC3NodeVisitor::visit( CC3Node* aNode )
{
	bool rslt = false;
	
	if ( !aNode ) 
		return rslt;					// Must have a node to work on
	
	_currentNode = aNode;				// Make the node being processed available. Not retained.

	if ( !_startingNode ) 
	{									// If this is the first node, start up
		_startingNode = aNode;			// Not retained
		open();							// Open the visitor
	}

	rslt = process( aNode );			// Process the node and its children recursively

	if ( aNode == _startingNode )       // If we're back to the first node, finish up
	{		
		close();						// Close the visitor
		_startingNode = NULL;			// Not retained
	}
	
	_currentNode = NULL;				// Done with this node now.

	return rslt;
}

/** Template method that is invoked automatically during visitation to process the specified node. */
bool CC3NodeVisitor::process( CC3Node* aNode )
{
	bool rslt = false;
	//LogTrace(@"%@ visiting %@ %@ children", self, aNode, (_shouldVisitChildren ? @"and" : @"but not"));
	
	processBeforeChildren( aNode );	// Heavy lifting before visiting children

	// Recurse through the child nodes if required
	if (_shouldVisitChildren) 
		rslt = processChildrenOf( aNode );

	processAfterChildren( aNode );		// Heavy lifting after visiting children

	return rslt;
}

/**
 * Template method that is invoked automatically to process the specified node when
 * that node is visited, before the visit: method is invoked on the child nodes of
 * the specified node.
 * 
 * This abstract implementation does nothing. Subclasses will override to process
 * each node as it is visited.
 */
void CC3NodeVisitor::processBeforeChildren( CC3Node* aNode )
{

}

/**
 * If the shouldVisitChildren property is set to YES, this template method is invoked
 * automatically to cause the visitor to visit the child nodes of the specified node .
 *
 * This implementation invokes the visit: method on this visitor for each of the
 * children of the specified node. This establishes a depth-first traveral of the
 * node hierarchy.
 *
 * Subclasses may override this method to establish a different traversal.
 */
bool CC3NodeVisitor::processChildrenOf( CC3Node* aNode )
{
	CC3Node* currNode = _currentNode;		// Remember current node
	
	CCArray* children = aNode->getChildren();

	CCObject* pObject;
	CCARRAY_FOREACH( children, pObject )
	{
		CC3Node* child = (CC3Node*)pObject;
		if ( child ) 
		{
			if ( visit( child ) )
			{
				_currentNode = currNode;
				return true;
			}
		}
	}
	
	_currentNode = currNode;				// Restore current node
	return false;
}

/**
 * Invoked automatically to process the specified node when that node is visited,
 * after the visit: method is invoked on the child nodes of the specified node.
 * 
 * This abstract implementation does nothing. Subclasses will override to process
 * each node as it is visited.
 */
void CC3NodeVisitor::processAfterChildren( CC3Node* aNode )
{
}

/**
 * Template method that prepares the visitor to perform a visitation run. This method
 * is invoked automatically prior to the first node being visited. It is not invoked
 * for each node visited.
 *
 * This implementation does nothing. Subclasses can override to initialize their state,
 * or to set any external state needed, such as GL state, prior to starting a visitation
 * run, and should invoke this superclass implementation.
 */
void CC3NodeVisitor::open()
{
}

/**
 * Invoked automatically after the last node has been visited during a visitation run.
 * This method is invoked automatically after all nodes have been visited.
 * It is not invoked for each node visited.
 *
 * This implementation processes the removals of any nodes that were requested to
 * be removed via the requestRemovalOf: method during the visitation run. Subclasses
 * can override to clean up their state, or to reset any external state, such as GL
 * state, upon completion of a visitation run, and should invoke this superclass
 * implementation to process any removal requests.
 */
void CC3NodeVisitor::close()
{ 
	processRemovals();
}

void CC3NodeVisitor::requestRemovalOf( CC3Node* aNode )
{
	if (!_pendingRemovals) 
	{
		_pendingRemovals = CCArray::create();
		_pendingRemovals->retain();
	}

	_pendingRemovals->addObject( aNode );
}

CC3Node* CC3NodeVisitor::getStartingNode()
{
	return NULL;
}

void CC3NodeVisitor::processRemovals()
{
	CCObject* pObject;
	CCARRAY_FOREACH( _pendingRemovals, pObject )
	{
		CC3Node* child = (CC3Node*)pObject;
		if ( child ) 
		{
			child->remove();
		}
	}

	if ( _pendingRemovals )
		_pendingRemovals->removeAllObjects();
}

CC3Scene* CC3NodeVisitor::getScene()
{ 
	return _startingNode->getScene();
}

CC3MeshNode* CC3NodeVisitor::getCurrentMeshNode()
{
	return (CC3MeshNode*)getCurrentNode(); 
}

CC3Material* CC3NodeVisitor::getCurrentMaterial()
{ 
	return getCurrentMeshNode()->getMaterial(); 
}

GLuint CC3NodeVisitor::getTextureCount()
{ 
	return getCurrentMeshNode()->getTextureCount();
}

CC3TextureUnit* CC3NodeVisitor::getCurrentTextureUnitAt( GLuint texUnit )
{
	CC3Texture* pTex = getCurrentMaterial()->getTextureForTextureUnit( texUnit );
	if ( pTex )
		return pTex->getTextureUnit();

	return NULL;
}

CC3ShaderProgram* CC3NodeVisitor::getCurrentShaderProgram()
{
	return getCurrentMeshNode()->getShaderProgram();
}

CC3Mesh* CC3NodeVisitor::getCurrentMesh()
{
	return getCurrentMeshNode()->getMesh();
}

GLuint CC3NodeVisitor::getLightCount()
{
	return (GLuint)getScene()->getLights()->count(); 
}

CC3Light* CC3NodeVisitor::getLightAt( GLuint index )
{
	CC3Scene* pScene = getScene();
	if ( pScene == NULL )
		return NULL;

	CCArray* lights = pScene->getLights();
	if (index < lights->count()) 
		return (CC3Light*)lights->objectAtIndex( index );

	return NULL;
}

GLuint CC3NodeVisitor::getLightProbeCount()
{
	GLuint lpCnt = (GLuint)getScene()->getLightProbes()->count();
	CC3ShaderProgram* sp = getCurrentShaderProgram();
	if (sp) 
		lpCnt = MIN(lpCnt, sp->getTextureLightProbeCount());
	return lpCnt;
}

CC3LightProbe* CC3NodeVisitor::getLightProbeAt( GLuint index )
{
	CCArray* lps = getScene()->getLightProbes();
	if (index < lps->count())
		return (CC3LightProbe*)lps->objectAtIndex( index );
	
	return NULL;
}


NS_COCOS3D_END
