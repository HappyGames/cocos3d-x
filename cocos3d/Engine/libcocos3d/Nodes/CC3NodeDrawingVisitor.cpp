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

CC3NodeDrawingVisitor::CC3NodeDrawingVisitor()
{
	m_drawingSequencer = NULL;				// weak reference
	m_currentSkinSection = NULL;				// weak reference
	m_pGL = NULL;								// weak reference
	m_surfaceManager = NULL;
	m_renderSurface = NULL;
	m_boneMatricesGlobal = NULL;
	m_boneMatricesEyeSpace = NULL;
	m_boneMatricesModelSpace = NULL;
}

CC3NodeDrawingVisitor::~CC3NodeDrawingVisitor()
{
	m_drawingSequencer = NULL;				// weak reference
	m_currentSkinSection = NULL;				// weak reference
	m_pGL = NULL;								// weak reference
	CC_SAFE_RELEASE(m_surfaceManager);
	CC_SAFE_RELEASE(m_renderSurface);
	CC_SAFE_RELEASE(m_boneMatricesGlobal);
	CC_SAFE_RELEASE(m_boneMatricesEyeSpace);
	CC_SAFE_RELEASE(m_boneMatricesModelSpace);
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
	if (!m_pGL) 
		m_pGL = CC3OpenGL::sharedGL();

	return m_pGL;
}

void CC3NodeDrawingVisitor::clearGL()
{ 
	m_pGL = NULL; 
}		// weak reference

GLuint CC3NodeDrawingVisitor::getCurrent2DTextureUnit()
{
	switch (m_textureBindingMode) {
		case kCC3TextureBindingModeLightProbe:
			return m_currentLightProbeTextureUnit;
		case kCC3TextureBindingModeModel:
			return m_current2DTextureUnit;
	}

	return 0;
}

void CC3NodeDrawingVisitor::increment2DTextureUnit()
{
	switch (m_textureBindingMode) {
		case kCC3TextureBindingModeLightProbe:
			m_currentLightProbeTextureUnit++;
			break;
		case kCC3TextureBindingModeModel:
			m_current2DTextureUnit++;
			break;
	}
}

GLuint CC3NodeDrawingVisitor::getCurrentCubeTextureUnit()
{
	switch (m_textureBindingMode) {
		case kCC3TextureBindingModeLightProbe:
			return m_currentLightProbeTextureUnit;
		case kCC3TextureBindingModeModel:
			return m_currentCubeTextureUnit;
	}

	return 0;
}

void CC3NodeDrawingVisitor::incrementCubeTextureUnit()
{
	switch (m_textureBindingMode) {
		case kCC3TextureBindingModeLightProbe:
			m_currentLightProbeTextureUnit++;
			break;
		case kCC3TextureBindingModeModel:
			m_currentCubeTextureUnit++;
			break;
	}
}

void CC3NodeDrawingVisitor::alignCameraViewport()
{
	// If the viewport of the camera has not been set directly, align it to the surface shape.
	// Invoked during setter. Don't use getter, to avoid infinite recursion when camera is NULL.

	if ( m_pCamera && m_renderSurface )
	{
		if ( CC3ViewportIsZero(m_pCamera->getViewport()) ) 
			m_pCamera->setViewport( m_renderSurface->getViewport() );
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

	m_currentSkinSection = NULL;
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
	if ( !m_drawingSequencer ) 
		return super::processChildrenOf( aNode );

	// Remember current node and whether children should be visited
	CC3Node* currNode = m_pCurrentNode;
	bool currSVC = m_shouldVisitChildren;
	
	m_shouldVisitChildren = false;	// Don't delve into node hierarchy if using sequencer
	m_drawingSequencer->visitNodesWithNodeVisitor( this );
	
	// Restore current node and whether children should be visited
	m_shouldVisitChildren = currSVC;
	m_pCurrentNode = currNode;
	
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
	if ( !m_pStartingNode->isScene() )
		return;

	CC3Scene* scene = getScene();
	if ( scene )
	{
		m_fDeltaTime = scene->getDeltaFrameTime();
		m_drawingSequencer = scene->getDrawingSequencer();
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
	m_drawingSequencer = NULL;
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
	m_current2DTextureUnit = 0;
	m_currentCubeTextureUnit = sp ? sp->getTextureCubeStart() : getTextureCount();
	m_currentLightProbeTextureUnit = sp ? sp->getTextureLightProbeStart() : getTextureCount();
	m_textureBindingMode = kCC3TextureBindingModeModel;
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

	m_textureBindingMode = kCC3TextureBindingModeLightProbe;
	
	GLuint tuCnt = getCurrentShaderProgram()->getTextureLightProbeCount();
	for (GLuint tuIdx = 0; tuIdx < tuCnt; tuIdx++)
		getLightProbeAt( tuIdx )->getTexture()->drawWithVisitor( this );
	
	m_textureBindingMode = kCC3TextureBindingModeModel;
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
	for (GLuint tuIdx = m_current2DTextureUnit; tuIdx < tuMax; tuIdx++)
		gl->disableTexturingAt( tuIdx );
	
	// Disable remaining unused cube textures
	tuMax = (sp ? (sp->getTextureCubeStart() + sp->getTextureCubeCount()) : tuMax);
	for (GLuint tuIdx = m_currentCubeTextureUnit; tuIdx < tuMax; tuIdx++)
		gl->disableTexturingAt( tuIdx );
	
	// Disable remaining light probe textures
	tuMax = (sp ? (sp->getTextureLightProbeStart() + sp->getTextureLightProbeCount()) : tuMax);
	for (GLuint tuIdx = m_currentLightProbeTextureUnit; tuIdx < tuMax; tuIdx++)
		gl->disableTexturingAt( tuIdx );
	
	// Ensure remaining system texture units are disabled
	gl->disableTexturingFrom( tuMax );
}

CC3SceneDrawingSurfaceManager* CC3NodeDrawingVisitor::getSurfaceManager()
{
	return m_surfaceManager; 
}

void CC3NodeDrawingVisitor::setSurfaceManager( CC3SceneDrawingSurfaceManager* surfaceManager )
{
	if (surfaceManager == m_surfaceManager) 
		return;

	CC_SAFE_RELEASE(m_surfaceManager);
	m_surfaceManager = surfaceManager;
	CC_SAFE_RETAIN(surfaceManager);
	setRenderSurface( NULL );
}

CC3RenderSurface* CC3NodeDrawingVisitor::getRenderSurface()
{
	if ( !m_renderSurface ) 
		setRenderSurface( getDefaultRenderSurface() );
	
	CCAssert(m_renderSurface, "%CC3NodeDrawingVisitor could not determine a default rendering surface."
			  " Prior to rendering, set the renderSurface property or surfaceManager property.");
	
	return m_renderSurface;
}

CC3RenderSurface* CC3NodeDrawingVisitor::getDefaultRenderSurface()
{ 
	return getSurfaceManager()->getViewSurface();
}

void CC3NodeDrawingVisitor::setRenderSurface( CC3RenderSurface* renderSurface )
{
	if ( renderSurface == m_renderSurface ) 
		return;

	CC_SAFE_RELEASE( m_renderSurface );
	m_renderSurface = renderSurface;
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
	return m_currentColor;
}

void CC3NodeDrawingVisitor::setCurrentColor( const ccColor4F& color )
{
	m_currentColor = color;
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
	return m_currentSkinSection; 
}

void CC3NodeDrawingVisitor::setCurrentSkinSection( CC3SkinSection* currentSkinSection )
{
	m_currentSkinSection = currentSkinSection;
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
	return &m_projMatrix;
}

const CC3Matrix4x3* CC3NodeDrawingVisitor::getViewMatrix()
{
	return &m_viewMatrix; 
}

const CC3Matrix4x3* CC3NodeDrawingVisitor::getModelMatrix()
{
	return &m_modelMatrix; 
}

const CC3Matrix4x3* CC3NodeDrawingVisitor::getModelViewMatrix() 
{
	if (m_isMVMtxDirty) 
	{
		CC3Matrix4x3Multiply(&m_modelViewMatrix, &m_viewMatrix, &m_modelMatrix);
		m_isMVMtxDirty = false;
	}
	return &m_modelViewMatrix;
}

const CC3Matrix4x4* CC3NodeDrawingVisitor::getViewProjMatrix()
{
	if (m_isVPMtxDirty) 
	{
		CC3Matrix4x4 v4x4;
		CC3Matrix4x4PopulateFrom4x3(&v4x4, &m_viewMatrix);
		CC3Matrix4x4Multiply(&m_viewProjMatrix, &m_projMatrix, &v4x4);
		m_isVPMtxDirty = false;
	}
	return &m_viewProjMatrix;
}

const CC3Matrix4x4* CC3NodeDrawingVisitor::getModelViewProjMatrix()
{
	if (m_isMVPMtxDirty) 
	{
		CC3Matrix4x4 m4x4;
		CC3Matrix4x4PopulateFrom4x3(&m4x4, getModelViewMatrix());
		CC3Matrix4x4Multiply(&m_modelViewProjMatrix, &m_projMatrix, &m4x4);
		m_isMVPMtxDirty = false;
	}
	return &m_modelViewProjMatrix;
}
//
//const GLKMatrix4* CC3NodeDrawingVisitor::getLayerTransformMatrix()
//{
//	return &_layerTransformMatrix; 
//}

void CC3NodeDrawingVisitor::populateProjMatrixFrom( CC3Matrix* projMtx )
{
	if ( !projMtx || m_pCurrentNode->shouldDrawInClipSpace())
		CC3Matrix4x4PopulateIdentity( &m_projMatrix );
	else
		projMtx->populateCC3Matrix4x4( &m_projMatrix );

	m_isVPMtxDirty = true;
	m_isMVPMtxDirty = true;

	// For fixed rendering pipeline, also load onto the matrix stack
	getGL()->loadProjectionMatrix( &m_projMatrix );
}

void CC3NodeDrawingVisitor::populateViewMatrixFrom( CC3Matrix* viewMtx )
{
	if ( !viewMtx || m_pCurrentNode->shouldDrawInClipSpace())
		CC3Matrix4x3PopulateIdentity(&m_viewMatrix);
	else
		viewMtx->populateCC3Matrix4x3( &m_viewMatrix );
	
	m_isVPMtxDirty = true;
	m_isMVMtxDirty = true;
	m_isMVPMtxDirty = true;
	
	// For fixed rendering pipeline, also load onto the matrix stack
	getGL()->loadModelviewMatrix( &m_viewMatrix );
}

void CC3NodeDrawingVisitor::populateModelMatrixFrom( CC3Matrix* modelMtx )
{
	if (modelMtx)
		modelMtx->populateCC3Matrix4x3( &m_modelMatrix );
	else
		CC3Matrix4x3PopulateIdentity(&m_modelMatrix);
	
	m_isMVMtxDirty = true;
	m_isMVPMtxDirty = true;
	
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
	ensureBoneMatrices( m_boneMatricesGlobal, getModelMatrix() );
	return (const CC3Matrix4x3*)m_boneMatricesGlobal->getElementAt( index );
}

const CC3Matrix4x3* CC3NodeDrawingVisitor::getEyeSpaceBoneMatrixAt( GLuint index )
{
	ensureBoneMatrices( m_boneMatricesEyeSpace, getModelViewMatrix() );
	return (const CC3Matrix4x3*)m_boneMatricesEyeSpace->getElementAt( index );
}

const CC3Matrix4x3* CC3NodeDrawingVisitor::getModelSpaceBoneMatrixAt( GLuint index )
{
	ensureBoneMatrices( m_boneMatricesModelSpace, NULL );
	return (const CC3Matrix4x3*)m_boneMatricesModelSpace->getElementAt( index );
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
	m_boneMatricesGlobal->setIsReady( false );
	m_boneMatricesEyeSpace->setIsReady( false );
	m_boneMatricesModelSpace->setIsReady( false );
}

void CC3NodeDrawingVisitor::init()
{
	super::init();
	m_pGL = NULL;
	m_surfaceManager = NULL;
	m_renderSurface = NULL;
	m_drawingSequencer = NULL;
	m_currentSkinSection = NULL;
	m_boneMatricesGlobal = CC3DataArray::dataArrayWithElementSize( sizeof(CC3Matrix4x3) );	// retained
	m_boneMatricesGlobal->retain();
	m_boneMatricesEyeSpace = CC3DataArray::dataArrayWithElementSize( sizeof(CC3Matrix4x3) );;	// retained
	m_boneMatricesEyeSpace->retain();
	m_boneMatricesModelSpace = CC3DataArray::dataArrayWithElementSize( sizeof(CC3Matrix4x3) );;	// retained
	m_boneMatricesModelSpace->retain();
	
	CC3Matrix4x3PopulateIdentity( &m_modelMatrix );
	CC3Matrix4x3PopulateIdentity( &m_viewMatrix );
	CC3Matrix4x4PopulateIdentity( &m_projMatrix );

	m_isVPMtxDirty = true;
	m_isMVMtxDirty = true;
	m_isMVPMtxDirty = true;
	m_shouldDecorateNode = true;
	m_isDrawingEnvironmentMap = false;
	m_currentCubeTextureUnit = 0;
	m_current2DTextureUnit = 0;
}

std::string CC3NodeDrawingVisitor::fullDescription()
{
	/*return [NSString stringWithFormat: @"%@, drawing %@nodes in seq %@", [super fullDescription],
	(_shouldDecorateNode ? @"and decorating " : @""), _drawingSequencer];*/
	return "CC3NodeDrawingVisitor";
}

float CC3NodeDrawingVisitor::getDeltaTime()
{
	return m_fDeltaTime;
}

void CC3NodeDrawingVisitor::setDeltaTime( float dt )
{
	m_fDeltaTime = dt;
}

bool CC3NodeDrawingVisitor::shouldDecorateNode()
{
	return m_shouldDecorateNode;
}

void CC3NodeDrawingVisitor::setShouldDecorateNode( bool shouldDecorate )
{
	m_shouldDecorateNode = shouldDecorate;
}

bool CC3NodeDrawingVisitor::isDrawingEnvironmentMap()
{
	return m_isDrawingEnvironmentMap;
}

void CC3NodeDrawingVisitor::setIsDrawingEnvironmentMap( bool bDraw )
{
	m_isDrawingEnvironmentMap = bDraw;
}

NS_COCOS3D_END
