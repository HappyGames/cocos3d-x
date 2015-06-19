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
	_deltaTime = dt;
}

float CC3NodeUpdatingVisitor::getDeltaTime()
{
	return _deltaTime;
}


CC3NodeDrawingVisitor::CC3NodeDrawingVisitor()
{
	_drawingSequencer = NULL;				// weak reference
	_currentSkinSection = NULL;				// weak reference
	_gl = NULL;								// weak reference
	_surfaceManager = NULL;
	_renderSurface = NULL;
	_boneMatricesGlobal = NULL;
	_boneMatricesEyeSpace = NULL;
	_boneMatricesModelSpace = NULL;
}

CC3NodeDrawingVisitor::~CC3NodeDrawingVisitor()
{
	_drawingSequencer = NULL;				// weak reference
	_currentSkinSection = NULL;				// weak reference
	_gl = NULL;								// weak reference
	CC_SAFE_RELEASE(_surfaceManager);
	CC_SAFE_RELEASE(_renderSurface);
	CC_SAFE_RELEASE(_boneMatricesGlobal);
	CC_SAFE_RELEASE(_boneMatricesEyeSpace);
	CC_SAFE_RELEASE(_boneMatricesModelSpace);
}

CC3NodeDrawingVisitor* CC3NodeDrawingVisitor::visitor()
{
	CC3NodeDrawingVisitor* pV = new CC3NodeDrawingVisitor;
	pV->init();
	pV->autorelease();

	return pV;
}

CC3OpenGL* CC3NodeDrawingVisitor::getGL()
{
	if (!_gl) 
		_gl = CC3OpenGL::sharedGL();

	return _gl;
}

void CC3NodeDrawingVisitor::clearGL()
{ 
	_gl = NULL; 
}		// weak reference

GLuint CC3NodeDrawingVisitor::getCurrent2DTextureUnit()
{
	switch (_textureBindingMode) {
		case kCC3TextureBindingModeLightProbe:
			return _currentLightProbeTextureUnit;
		case kCC3TextureBindingModeModel:
			return _current2DTextureUnit;
	}

	return 0;
}

void CC3NodeDrawingVisitor::increment2DTextureUnit()
{
	switch (_textureBindingMode) {
		case kCC3TextureBindingModeLightProbe:
			_currentLightProbeTextureUnit++;
			break;
		case kCC3TextureBindingModeModel:
			_current2DTextureUnit++;
			break;
	}
}

GLuint CC3NodeDrawingVisitor::getCurrentCubeTextureUnit()
{
	switch (_textureBindingMode) {
		case kCC3TextureBindingModeLightProbe:
			return _currentLightProbeTextureUnit;
		case kCC3TextureBindingModeModel:
			return _currentCubeTextureUnit;
	}

	return 0;
}

void CC3NodeDrawingVisitor::incrementCubeTextureUnit()
{
	switch (_textureBindingMode) {
		case kCC3TextureBindingModeLightProbe:
			_currentLightProbeTextureUnit++;
			break;
		case kCC3TextureBindingModeModel:
			_currentCubeTextureUnit++;
			break;
	}
}

void CC3NodeDrawingVisitor::alignCameraViewport()
{
	// If the viewport of the camera has not been set directly, align it to the surface shape.
	// Invoked during setter. Don't use getter, to avoid infinite recursion when camera is NULL.

	if ( _camera && _renderSurface )
	{
		if ( CC3ViewportIsZero(_camera->getViewport()) ) 
			_camera->setViewport( _renderSurface->getViewport() );
	}
}

void CC3NodeDrawingVisitor::alignShotWith( CC3NodeDrawingVisitor* otherVisitor )
{
	setSurfaceManager ( otherVisitor->getSurfaceManager() );
	setCamera( otherVisitor->getCamera() );
	setRenderSurface( otherVisitor->getRenderSurface() );
}

void CC3NodeDrawingVisitor::processBeforeChildren( CC3Node* aNode )
{
	CC3PerformanceStatistics* pStatistics = getPerformanceStatistics();
	if ( pStatistics )
		pStatistics->incrementNodesVisitedForDrawing();

	if ( shouldDrawNode( aNode ) )
		aNode->transformAndDrawWithVisitor( this );

	_currentSkinSection = NULL;
}

bool CC3NodeDrawingVisitor::shouldDrawNode( CC3Node* aNode )
{
	return aNode->hasLocalContent()
			&& isNodeVisibleForDrawing( aNode )
			&& doesNodeIntersectFrustum( aNode );
}

bool CC3NodeDrawingVisitor::doesNodeIntersectFrustum( CC3Node* aNode )
{
	CC3Camera* pCam = getCamera();
	CC3Frustum* pFrustum = pCam ? pCam->getFrustum() : NULL;
	return aNode->doesIntersectFrustum( pFrustum );
}

bool CC3NodeDrawingVisitor::isNodeVisibleForDrawing( CC3Node* aNode )
{ 
	return aNode->isVisible();
}

bool CC3NodeDrawingVisitor::processChildrenOf( CC3Node* aNode )
{
	if ( !_drawingSequencer ) 
		return super::processChildrenOf( aNode );

	// Remember current node and whether children should be visited
	CC3Node* currNode = _currentNode;
	bool currSVC = _shouldVisitChildren;
	
	_shouldVisitChildren = false;	// Don't delve into node hierarchy if using sequencer
	_drawingSequencer->visitNodesWithNodeVisitor( this );
	
	// Restore current node and whether children should be visited
	_shouldVisitChildren = currSVC;
	_currentNode = currNode;
	
	return false;
}

/** Prepares GL programs, activates the rendering surface, and opens the scene and the camera. */
void CC3NodeDrawingVisitor::open()
{
	super::open();

	CC3ShaderProgram::setWillBeginDrawingScene();

	activateRenderSurface();
	openScene();
	openCamera();
}

/** 
 * Activates the render surface and applies its viewport. 
 * Subsequent GL drawing will be directed to this surface. 
 */
void CC3NodeDrawingVisitor::activateRenderSurface()
{
	CC3RenderSurface* surface = getRenderSurface();
	surface->activate();
	getGL()->setViewport( surface->getViewport() );
}

/** If this visitor was started on a CC3Scene node, set up for drawing an entire scene. */
void CC3NodeDrawingVisitor::openScene()
{
	if ( !_startingNode->isScene() )
		return;

	CC3Scene* scene = getScene();
	if ( scene )
	{
		_deltaTime = scene->getDeltaFrameTime();
		_drawingSequencer = scene->getDrawingSequencer();
	}
}

/** Template method that opens the 3D camera. */
void CC3NodeDrawingVisitor::openCamera()
{
	CC3Camera* pCam = getCamera();
	if ( pCam )
		pCam->openWithVisitor( this );

#if !CC3_GLSL
	// Hack for OpenGL & OpenGL ES fixed pipeline to force update of light position/direction
	// AFTER modelview matrix has been updated, as required by OpenGL fixed pipeline.
	// See http://www.opengl.org/archives/resources/faq/technical/lights.htm#ligh0050
	getScene()->illuminateWithVisitor( this );
#endif	// !CC3_GLSL

}

/** Close the camera. */
void CC3NodeDrawingVisitor::close()
{
	closeCamera();
	_drawingSequencer = NULL;
	super::close();
}

/** Close the camera. This is the compliment of the openCamera method. */
void CC3NodeDrawingVisitor::closeCamera()
{
	CC3Camera* pCam = getCamera();
	if ( pCam )
		pCam->closeWithVisitor( this );
}

void CC3NodeDrawingVisitor::draw( CC3Node* aNode )
{
	//LogTrace(@"Drawing %@", aNode);
	CC3OpenGL* gl = getGL();
	gl->pushGroupMarkerC( aNode->getRenderStreamGroupMarker().c_str() );

	aNode->drawWithVisitor( this );
	
	gl->popGroupMarker();
	CC3PerformanceStatistics* pStatistics = getPerformanceStatistics();
	if ( pStatistics )
		pStatistics->incrementNodesDrawn();
}

void CC3NodeDrawingVisitor::resetTextureUnits()
{
	// Under OGLES 2.0 & OGL, the required texture units are determined by the shader uniforms.
	// Under OGLES 1.1, they are determined by the number of textures attached to the mesh node.
	CC3ShaderProgram* sp = getCurrentShaderProgram();
	_current2DTextureUnit = 0;
	_currentCubeTextureUnit = sp ? sp->getTextureCubeStart() : getTextureCount();
	_currentLightProbeTextureUnit = sp ? sp->getTextureLightProbeStart() : getTextureCount();
	_textureBindingMode = kCC3TextureBindingModeModel;
}

void CC3NodeDrawingVisitor::bindEnvironmentalTextures()
{
	bindLightProbeTextures();
}

/** Retrieve any light probe textures and bind them to the GL engine. */
void CC3NodeDrawingVisitor::bindLightProbeTextures()
{
	if ( !getCurrentNode()->shouldUseLightProbes() ) 
		return;

	_textureBindingMode = kCC3TextureBindingModeLightProbe;
	
	GLuint tuCnt = getCurrentShaderProgram()->getTextureLightProbeCount();
	for (GLuint tuIdx = 0; tuIdx < tuCnt; tuIdx++)
		getLightProbeAt( tuIdx )->getTexture()->drawWithVisitor( this );
	
	_textureBindingMode = kCC3TextureBindingModeModel;
}

void CC3NodeDrawingVisitor::disableUnusedTextureUnits()
{
	// Determine the maximum number of textures of each type that could be used
	// Under OGLES 2.0 & OGL, this is determined by the shader uniforms.
	// Under OGLES 1.1, this is determined by the number of textures attached to the mesh node.
	CC3ShaderProgram* sp = getCurrentShaderProgram();
	CC3OpenGL* gl = getGL();
	GLuint tuMax = 0;
	
	// Disable remaining unused 2D textures
	tuMax = sp ? (sp->getTexture2DStart() + sp->getTexture2DCount()) : getTextureCount();
	for (GLuint tuIdx = _current2DTextureUnit; tuIdx < tuMax; tuIdx++)
		gl->disableTexturingAt( tuIdx );
	
	// Disable remaining unused cube textures
	tuMax = (sp ? (sp->getTextureCubeStart() + sp->getTextureCubeCount()) : tuMax);
	for (GLuint tuIdx = _currentCubeTextureUnit; tuIdx < tuMax; tuIdx++)
		gl->disableTexturingAt( tuIdx );
	
	// Disable remaining light probe textures
	tuMax = (sp ? (sp->getTextureLightProbeStart() + sp->getTextureLightProbeCount()) : tuMax);
	for (GLuint tuIdx = _currentLightProbeTextureUnit; tuIdx < tuMax; tuIdx++)
		gl->disableTexturingAt( tuIdx );
	
	// Ensure remaining system texture units are disabled
	gl->disableTexturingFrom( tuMax );
}

CC3SceneDrawingSurfaceManager* CC3NodeDrawingVisitor::getSurfaceManager()
{
	return _surfaceManager; 
}

void CC3NodeDrawingVisitor::setSurfaceManager( CC3SceneDrawingSurfaceManager* surfaceManager )
{
	if (surfaceManager == _surfaceManager) 
		return;

	CC_SAFE_RELEASE(_surfaceManager);
	_surfaceManager = surfaceManager;
	CC_SAFE_RETAIN(surfaceManager);
	setRenderSurface( NULL );
}

CC3RenderSurface* CC3NodeDrawingVisitor::getRenderSurface()
{
	if ( !_renderSurface ) 
		setRenderSurface( getDefaultRenderSurface() );
	
	CCAssert(_renderSurface, "%CC3NodeDrawingVisitor could not determine a default rendering surface."
			  " Prior to rendering, set the renderSurface property or surfaceManager property.");
	
	return _renderSurface;
}

CC3RenderSurface* CC3NodeDrawingVisitor::getDefaultRenderSurface()
{ 
	return getSurfaceManager()->getViewSurface();
}

void CC3NodeDrawingVisitor::setRenderSurface( CC3RenderSurface* renderSurface )
{
	if ( renderSurface == _renderSurface ) 
		return;

	CC_SAFE_RELEASE( _renderSurface );
	_renderSurface = renderSurface;
	CC_SAFE_RETAIN( renderSurface );
	alignCameraViewport();
}

void CC3NodeDrawingVisitor::setCamera( CC3Camera* camera )
{
	super::setCamera( camera );
	alignCameraViewport();
}

/**
 * Overridden to return either the real shader program, or the pure-color shader program,
 * depending on whether the node should be decorated. For example, during node picking,
 * the node is not decorated, and the pure-color program will be used.
 * The shaderProgram will be selected if it has not been assigned already.
 */
CC3ShaderProgram* CC3NodeDrawingVisitor::getCurrentShaderProgram()
{
	if ( shouldDecorateNode() )
	{
		CC3MeshNode* pMeshNode = getCurrentMeshNode();
		if ( pMeshNode )
			return pMeshNode->getShaderProgram();
	}
	else
	{
		CC3MeshNode* pMeshNode = getCurrentMeshNode();
		if ( pMeshNode )
		{
			CC3ShaderContext* pContext = pMeshNode->getShaderContext();
			if ( pContext )
				return pContext->getPureColorProgram();
		}
	}
	
	return NULL;
}

ccColor4F CC3NodeDrawingVisitor::getCurrentColor()
{
	return _currentColor;
}

void CC3NodeDrawingVisitor::setCurrentColor( const ccColor4F& color )
{
	_currentColor = color;
}

ccColor4B CC3NodeDrawingVisitor::getCurrentColor4B()
{ 
	return CCC4BFromCCC4F(getCurrentColor()); 
}

void CC3NodeDrawingVisitor::setCurrentColor4B( const ccColor4B& color4B )
{
	setCurrentColor( CCC4FFromCCC4B(color4B) );
}

CC3SkinSection* CC3NodeDrawingVisitor::getCurrentSkinSection()
{ 
	return _currentSkinSection; 
}

void CC3NodeDrawingVisitor::setCurrentSkinSection( CC3SkinSection* currentSkinSection )
{
	_currentSkinSection = currentSkinSection;
	resetBoneMatrices();
}

CC3Vector CC3NodeDrawingVisitor::transformGlobalLocationToEyeSpace( const CC3Vector& globalLocation )
{
	return CC3Matrix4x3TransformLocation(getViewMatrix(), globalLocation);
}

CC3Vector CC3NodeDrawingVisitor::transformGlobalLocationToModelSpace( const CC3Vector& globalLocation )
{
	return getCurrentNode()->getGlobalTransformMatrixInverted()->transformLocation( globalLocation );
}

const CC3Matrix4x4* CC3NodeDrawingVisitor::getProjMatrix()
{ 
	return &_projMatrix;
}

const CC3Matrix4x3* CC3NodeDrawingVisitor::getViewMatrix()
{
	return &_viewMatrix; 
}

const CC3Matrix4x3* CC3NodeDrawingVisitor::getModelMatrix()
{
	return &_modelMatrix; 
}

const CC3Matrix4x3* CC3NodeDrawingVisitor::getModelViewMatrix() 
{
	if (_isMVMtxDirty) 
	{
		CC3Matrix4x3Multiply(&_modelViewMatrix, &_viewMatrix, &_modelMatrix);
		_isMVMtxDirty = false;
	}
	return &_modelViewMatrix;
}

const CC3Matrix4x4* CC3NodeDrawingVisitor::getViewProjMatrix()
{
	if (_isVPMtxDirty) 
	{
		CC3Matrix4x4 v4x4;
		CC3Matrix4x4PopulateFrom4x3(&v4x4, &_viewMatrix);
		CC3Matrix4x4Multiply(&_viewProjMatrix, &_projMatrix, &v4x4);
		_isVPMtxDirty = false;
	}
	return &_viewProjMatrix;
}

const CC3Matrix4x4* CC3NodeDrawingVisitor::getModelViewProjMatrix()
{
	if (_isMVPMtxDirty) 
	{
		CC3Matrix4x4 m4x4;
		CC3Matrix4x4PopulateFrom4x3(&m4x4, getModelViewMatrix());
		CC3Matrix4x4Multiply(&_modelViewProjMatrix, &_projMatrix, &m4x4);
		_isMVPMtxDirty = false;
	}
	return &_modelViewProjMatrix;
}
//
//const GLKMatrix4* CC3NodeDrawingVisitor::getLayerTransformMatrix()
//{
//	return &_layerTransformMatrix; 
//}

void CC3NodeDrawingVisitor::populateProjMatrixFrom( CC3Matrix* projMtx )
{
	if ( !projMtx || _currentNode->shouldDrawInClipSpace())
		CC3Matrix4x4PopulateIdentity( &_projMatrix );
	else
		projMtx->populateCC3Matrix4x4( &_projMatrix );

	_isVPMtxDirty = true;
	_isMVPMtxDirty = true;

	// For fixed rendering pipeline, also load onto the matrix stack
	getGL()->loadProjectionMatrix( &_projMatrix );
}

void CC3NodeDrawingVisitor::populateViewMatrixFrom( CC3Matrix* viewMtx )
{
	if ( !viewMtx || _currentNode->shouldDrawInClipSpace())
		CC3Matrix4x3PopulateIdentity(&_viewMatrix);
	else
		viewMtx->populateCC3Matrix4x3( &_viewMatrix );
	
	_isVPMtxDirty = true;
	_isMVMtxDirty = true;
	_isMVPMtxDirty = true;
	
	// For fixed rendering pipeline, also load onto the matrix stack
	getGL()->loadModelviewMatrix( &_viewMatrix );
}

void CC3NodeDrawingVisitor::populateModelMatrixFrom( CC3Matrix* modelMtx )
{
	if (modelMtx)
		modelMtx->populateCC3Matrix4x3( &_modelMatrix );
	else
		CC3Matrix4x3PopulateIdentity(&_modelMatrix);
	
	_isMVMtxDirty = true;
	_isMVPMtxDirty = true;
	
	// For fixed rendering pipeline, also load onto the matrix stack
	getGL()->loadModelviewMatrix( getModelViewMatrix() );
}
//
//void CC3NodeDrawingVisitor::populateLayerTransformMatrixFrom( const GLKMatrix4* layerMtx )
//{
//	_layerTransformMatrix = (layerMtx) ? *layerMtx : GLKMatrix4Identity;
//}

const CC3Matrix4x3* CC3NodeDrawingVisitor::getGlobalBoneMatrixAt( GLuint index )
{
	ensureBoneMatrices( _boneMatricesGlobal, getModelMatrix() );
	return (const CC3Matrix4x3*)_boneMatricesGlobal->getElementAt( index );
}

const CC3Matrix4x3* CC3NodeDrawingVisitor::getEyeSpaceBoneMatrixAt( GLuint index )
{
	ensureBoneMatrices( _boneMatricesEyeSpace, getModelViewMatrix() );
	return (const CC3Matrix4x3*)_boneMatricesEyeSpace->getElementAt( index );
}

const CC3Matrix4x3* CC3NodeDrawingVisitor::getModelSpaceBoneMatrixAt( GLuint index )
{
	ensureBoneMatrices( _boneMatricesModelSpace, NULL );
	return (const CC3Matrix4x3*)_boneMatricesModelSpace->getElementAt( index );
}

/**
 * Ensures that the specified bone matrix array has been populated from the bones of the
 * current skin section. If the specified space matrix is not NULL, it is used to transform
 * the bone matrices into some other space (eg- global space, eye space), otherwise, the
 * bone matrices are left in their local space coordinates (model space).
 * Once populated, the bone matrix array is marked as being ready, so it won't be populated
 * again until being marked as not ready.
 */
void CC3NodeDrawingVisitor::ensureBoneMatrices( CC3DataArray* boneMatrices, const CC3Matrix4x3* spaceMatrix )
{
	if ( boneMatrices->isReady() )
		return;
	
	CC3Matrix4x3 sbMtx;
	CC3SkinSection* skin = getCurrentSkinSection();
	GLuint boneCnt = skin->getBoneCount();
	boneMatrices->ensureElementCapacity( boneCnt );
	for ( GLuint boneIdx = 0; boneIdx < boneCnt; boneIdx++ ) 
	{
		CC3Matrix* boneMtx = skin->getTransformMatrixForBoneAt( boneIdx );
		CC3Matrix4x3* pBoneSpaceMtx = (CC3Matrix4x3*)boneMatrices->getElementAt( boneIdx );
		if ( spaceMatrix ) 
		{
			// Use the space matrix to transform the bone matrix.
			boneMtx->populateCC3Matrix4x3( &sbMtx );
			CC3Matrix4x3Multiply( pBoneSpaceMtx, spaceMatrix, &sbMtx );
		} 
		else
		{
			boneMtx->populateCC3Matrix4x3( pBoneSpaceMtx );	// Use the untransformed bone matrix.
		}
	}

	boneMatrices->setIsReady( true );
}

/** Resets the bone matrices so they will be populated when requested for the current skin section. */
void CC3NodeDrawingVisitor::resetBoneMatrices()
{
	_boneMatricesGlobal->setIsReady( false );
	_boneMatricesEyeSpace->setIsReady( false );
	_boneMatricesModelSpace->setIsReady( false );
}

void CC3NodeDrawingVisitor::init()
{
	super::init();
	_gl = NULL;
	_surfaceManager = NULL;
	_renderSurface = NULL;
	_drawingSequencer = NULL;
	_currentSkinSection = NULL;
	_boneMatricesGlobal = CC3DataArray::dataArrayWithElementSize( sizeof(CC3Matrix4x3) );	// retained
	_boneMatricesGlobal->retain();
	_boneMatricesEyeSpace = CC3DataArray::dataArrayWithElementSize( sizeof(CC3Matrix4x3) );;	// retained
	_boneMatricesEyeSpace->retain();
	_boneMatricesModelSpace = CC3DataArray::dataArrayWithElementSize( sizeof(CC3Matrix4x3) );;	// retained
	_boneMatricesModelSpace->retain();
	
	CC3Matrix4x3PopulateIdentity( &_modelMatrix );
	CC3Matrix4x3PopulateIdentity( &_viewMatrix );
	CC3Matrix4x4PopulateIdentity( &_projMatrix );

	_isVPMtxDirty = true;
	_isMVMtxDirty = true;
	_isMVPMtxDirty = true;
	_shouldDecorateNode = true;
	_isDrawingEnvironmentMap = false;
	_currentCubeTextureUnit = 0;
	_current2DTextureUnit = 0;
}

std::string CC3NodeDrawingVisitor::fullDescription()
{
	/*return [NSString stringWithFormat: @"%@, drawing %@nodes in seq %@", [super fullDescription],
	(_shouldDecorateNode ? @"and decorating " : @""), _drawingSequencer];*/
	return "CC3NodeDrawingVisitor";
}

float CC3NodeDrawingVisitor::getDeltaTime()
{
	return _deltaTime;
}

void CC3NodeDrawingVisitor::setDeltaTime( float dt )
{
	_deltaTime = dt;
}

bool CC3NodeDrawingVisitor::shouldDecorateNode()
{
	return _shouldDecorateNode;
}

void CC3NodeDrawingVisitor::setShouldDecorateNode( bool shouldDecorate )
{
	_shouldDecorateNode = shouldDecorate;
}

bool CC3NodeDrawingVisitor::isDrawingEnvironmentMap()
{
	return _isDrawingEnvironmentMap;
}

void CC3NodeDrawingVisitor::setIsDrawingEnvironmentMap( bool bDraw )
{
	_isDrawingEnvironmentMap = bDraw;
}

CC3NodePickingVisitor::CC3NodePickingVisitor()
{
	_pickedNode = NULL;
}

CC3NodePickingVisitor::~CC3NodePickingVisitor()
{
	CC_SAFE_RELEASE( _pickedNode );
}

/** 
 * Since node picking is asynchronous, the picked node is retained when stored here.
 * When accessing, retain and autorelease it, then release and clear the iVar.
 * In the case where this is the last reference, the retain/autorelease will ensure
 * it continues to live for processing by the caller.
 */
CC3Node* CC3NodePickingVisitor::getPickedNode()
{
	if ( _pickedNode )
	{
		_pickedNode->retain();
		_pickedNode->autorelease();
	}

	CC3Node* pn =  _pickedNode;
	CC_SAFE_RELEASE_NULL( _pickedNode );

	return pn;
}

/** Overridden to initially set the shouldDecorateNode to NO. */
void CC3NodePickingVisitor::init()
{
	super::init();

	_pickedNode = NULL;
	_shouldDecorateNode = false;
	_tagColorShift = 0;
}

/** Clears the render surface and the pickedNode property. */
void CC3NodePickingVisitor::open()
{
	super::open();
	getRenderSurface()->clearColorAndDepthContent();
}

/**
 * Reads the color of the pixel at the touch point, maps that to the tag of the CC3Node
 * that was touched, and sets the picked node in the pickedNode property.
 *
 * Clears the depth buffer in case the primary scene rendering is using the same surface.
 */
void CC3NodePickingVisitor::close()
{
	CC3RenderSurface* surface = getRenderSurface();

	// Get layer touch point and convert to pixels
	CCPoint touchPoint = ccpMult(getScene()->getTouchedNodePicker()->getTouchPoint(),
								 CCDirector::sharedDirector()->getContentScaleFactor());

	// Convert the touch point to the coordinates of the surface viewport by offsetting
	// the touch point by the surface viewport origin
	CC3IntPoint vpTouchPoint = CC3IntPointAdd(CC3IntPointFromCGPoint(touchPoint), surface->getViewport().origin);

	// Read the pixel from the surface
	ccColor4B pixColor;
	surface->readColorContentFrom( CC3ViewportMake(vpTouchPoint.x, vpTouchPoint.y, 1, 1), &pixColor );
	
	// Fetch the node whose tags is mapped from the pixel color
	CC_SAFE_RELEASE( _pickedNode );
	_pickedNode = getScene()->getNodeTagged( tagFromColor( pixColor ) );
	CC_SAFE_RETAIN( _pickedNode );

	//LogTrace(@"%@ picked %@ from color %@ at position %@", self, _pickedNode,
	//		 NSStringFromCCC4B(pixColor), NSStringFromCC3IntPoint(vpTouchPoint));
	
	super::close();
}

CC3RenderSurface* CC3NodePickingVisitor::getDefaultRenderSurface()
{
	return (getScene()->shouldDisplayPickingRender()
			? getSurfaceManager()->getViewSurface()
			: getSurfaceManager()->getPickingSurface());
}

void CC3NodePickingVisitor::alignShotWith( CC3NodeDrawingVisitor* otherVisitor )
{
	setSurfaceManager( otherVisitor->getSurfaceManager() );		// Also clears renderSurface property
	setCamera( otherVisitor->getCamera() );
}

/**
 * Overridden because what matters here is not visibility, but touchability.
 * Invisible nodes will be drawn if touchable.
 */
bool CC3NodePickingVisitor::isNodeVisibleForDrawing( CC3Node* aNode )
{
	return aNode->isTouchable();
}

/** Overridden to draw the node in a uniquely identifiable color. */
void CC3NodePickingVisitor::draw( CC3Node* aNode )
{
	paintNode( aNode );
	super::draw( aNode );
}

/** Maps the specified node to a unique color, and paints the node with that color. */
void CC3NodePickingVisitor::paintNode( CC3Node* aNode )
{
	setCurrentColor4B( colorFromNodeTag( aNode->getTag() ) );
	//LogTrace(@"%@ painting %@ with color %@", self, aNode, NSStringFromCCC4B(self.currentColor4B));
}

/**
 * Maps the specified integer tag to a color, by spreading the bits of the integer across
 * the red, green and blue unsigned bytes of the color. This permits 2^24 objects to be
 * encoded by colors. This is the compliment of the tagFromColor: method.
 */
ccColor4B CC3NodePickingVisitor::colorFromNodeTag( GLuint tag )
{
	tag <<= _tagColorShift;
	GLuint mask = 255;
	GLubyte r = (tag >> 16) & mask;
	GLubyte g = (tag >> 8) & mask;
	GLubyte b = tag & mask;
	return ccc4(r, g, b, 0);	// Alpha ignored during pure-color painting
}

/**
 * Maps the specified color to a tag, by combining the bits of the red, green, and blue
 * colors into a single integer value. This is the compliment of the colorFromNodeTag: method.
 */
GLuint CC3NodePickingVisitor::tagFromColor( const ccColor4B& color )
{
	return (((GLuint)color.r << 16) | ((GLuint)color.g << 8) | (GLuint)color.b) >> _tagColorShift;
}

std::string CC3NodePickingVisitor::fullDescription()
{
	//return [NSString stringWithFormat: @"%@, picked: %@", super.fullDescription, _pickedNode];
	return "CC3NodePickingVisitor";
}

CC3NodePickingVisitor* CC3NodePickingVisitor::visitor()
{
	CC3NodePickingVisitor* pVal = new CC3NodePickingVisitor;
	pVal->init();
	pVal->autorelease();

	return pVal;
}

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
	return initWithRay( CC3RayFromLocDir(CC3Vector::kCC3VectorNull, CC3Vector::kCC3VectorNull) );
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
