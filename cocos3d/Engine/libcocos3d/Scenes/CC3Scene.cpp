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

CC3Scene::CC3Scene()
{
	m_pBackdrop = NULL;
	m_pFog = NULL;
	m_pActiveCamera = NULL;
	m_pDrawingSequenceVisitor = NULL;
	m_pDrawingSequencer = NULL;
	m_pViewDrawingVisitor = NULL;
	m_pEnvMapDrawingVisitor = NULL;
	m_pUpdateVisitor = NULL;
	m_pShadowVisitor = NULL;
	m_pTouchedNodePicker = NULL;
	m_pPerformanceStatistics = NULL;
	m_lights = NULL;
	m_lightProbes = NULL;
	m_billboards = NULL;
}

/**
 * Descendant nodes will be removed by superclass. Their removal may invoke
 * didRemoveDescendant:, which references several of these instance variables.
 * Make sure they are all made nil in addition to being released here!
 */
CC3Scene::~CC3Scene()
{
	//LogInfo(@"Deallocating %@ on thread %@", self, NSThread.currentThread);
	setBackdrop( NULL );					// Use setter to stop any actions
	setFog( NULL );							// Use setter to stop any actions
	setActiveCamera( NULL );				// Use setter to release and make nil
	setDrawingSequencer( NULL );			// Use setter to release and make nil
	setDrawingSequenceVisitor( NULL );		// Use setter to release and make nil
	setViewDrawingVisitor( NULL );			// Use setter to release and make nil
	setEnvMapDrawingVisitor( NULL );		// Use setter to release and make nil
	setUpdateVisitor( NULL );				// Use setter to release and make nil
	setShadowVisitor( NULL );				// Use setter to release and make nil
	setTouchedNodePicker( NULL );			// Use setter to release and make nil
	setPerformanceStatistics( NULL );		// Use setter to release and make nil
	
	CC_SAFE_RELEASE( m_lights );
	CC_SAFE_RELEASE( m_lightProbes );
	CC_SAFE_RELEASE( m_billboards );
}

bool CC3Scene::isScene()
{
	return true; 
}

#pragma _NOTE_TODO( "CC3Scene::getController()" )
//CC3ViewController* CC3Scene::getController()
//{
//	return _cc3Layer->getController(); 
//}

bool CC3Scene::shouldDisplayPickingRender()
{
	return m_shouldDisplayPickingRender;
}

CC3Camera* CC3Scene::getActiveCamera()
{
	return m_pActiveCamera; 
}

void CC3Scene::setActiveCamera( CC3Camera* aCamera )
{
	if (aCamera == m_pActiveCamera) return;
	
	CC3Camera* oldCam = m_pActiveCamera;
	m_pActiveCamera = aCamera;
	CC_SAFE_RETAIN(aCamera);
	activeCameraChangedFrom( oldCam );
	CC_SAFE_RELEASE(oldCam);
}

/** The active camera has changed. Update whoever cares. */
void CC3Scene::activeCameraChangedFrom( CC3Camera* oldCam  )
{
	CC3Camera* newCam = getActiveCamera();
	
	// Update the visitors that make use of the active camera
	CC3NodeUpdatingVisitor* updateVisitor = getUpdateVisitor();
	if ( updateVisitor )
		updateVisitor->setCamera( newCam );

	CC3NodeDrawingVisitor* viewDrawingVisitor = getViewDrawingVisitor();
	if ( viewDrawingVisitor )
		viewDrawingVisitor->setCamera( newCam );

	CC3NodeDrawingVisitor* shadowVisitor = getShadowVisitor();
	if ( shadowVisitor )
		shadowVisitor->setCamera( newCam );

	CC3TouchedNodePicker* nodePicker = getTouchedNodePicker();
	if ( nodePicker )
	{
		CC3NodePickingVisitor* pickingVisitor = nodePicker->getPickVisitor();
		if ( pickingVisitor )
			pickingVisitor->setCamera( newCam );
	}

	// Ensure camera screen-rendering configuration is preserved
	if ( newCam && oldCam )
	{
		newCam->setViewport( oldCam->getViewport() );
		newCam->setHasInfiniteDepthOfField( oldCam->hasInfiniteDepthOfField() );

		// Move camera listeners (eg- shadow casting volumes && nodes targetting the camera) to new camera.
		// If the listener is a node that should always target the camera, or its target is already the old
		// camera, set its target to the new camera. Otherwise, just transfer the listener.
		std::vector<CC3NodeTransformListenerProtocol*> camListeners;
		oldCam->getTransformListeners( camListeners );
		for(unsigned int i = 0; i < camListeners.size(); i++)
		{
			CC3Node* tn = dynamic_cast<CC3Node*>(camListeners[i]);
			if ( tn  && ( tn->shouldAutotargetCamera() || (oldCam && (tn->getTarget() == oldCam))) ) 
			{
				tn->setTarget( newCam );
			} else 
			{
				oldCam->removeTransformListener( camListeners[i] );
				newCam->addTransformListener( camListeners[i] );
			}
		}
	}
}

CCArray* CC3Scene::getLights()
{
	return m_lights;
}

CC3MeshNode* CC3Scene::getBackdrop()
{
	return m_pBackdrop;
}

void CC3Scene::setBackdrop( CC3MeshNode* backdrop )
{
	if (backdrop == m_pBackdrop) 
		return;
	
	if ( m_pBackdrop )
		m_pBackdrop->stopAllActions();		// Ensure all actions stopped before releasing
	
	CC_SAFE_RELEASE(m_pBackdrop);
	m_pBackdrop = backdrop;
	CC_SAFE_RETAIN(backdrop);
	
	if ( m_pBackdrop )
		m_pBackdrop->setParent( this );		// Ensure shaders can access scene content
}

CC3Fog* CC3Scene::getFog()
{
	return m_pFog;
}

void CC3Scene::setFog( CC3Fog* fog )
{
	if (fog == m_pFog) 
		return;
	
	if ( m_pFog )
	{
		m_pFog->stopAllActions();			// Ensure all actions stopped before releasing
		m_pFog->release();
	}

	m_pFog = fog;
	CC_SAFE_RETAIN( fog );

	if ( m_pFog )
		m_pFog->setParent( this );				// Ensure shaders can access scene content
}

CC3TouchedNodePicker* CC3Scene::getTouchedNodePicker()
{
	return m_pTouchedNodePicker;
}

void CC3Scene::setTouchedNodePicker( CC3TouchedNodePicker* nodePicker )
{
	CC_SAFE_RELEASE(m_pTouchedNodePicker);
	CC_SAFE_RETAIN(nodePicker);
	m_pTouchedNodePicker = nodePicker;
}

void CC3Scene::setIsRunning( bool shouldRun )
{
	super::setIsRunning( shouldRun );

	if ( m_pBackdrop )
		m_pBackdrop->setIsRunning( shouldRun );

	if ( m_pFog )
		m_pFog->setIsRunning( shouldRun );
}

CCColorRef CC3Scene::getColor()
{
	return m_pBackdrop ? m_pBackdrop->getColor() : super::getColor(); 
}

void CC3Scene::setColor( const CCColorRef& color )
{ 
	m_pBackdrop->setColor( color ); 
}

CCOpacity CC3Scene::getOpacity()
{
	return m_pBackdrop ? m_pBackdrop->getOpacity() : super::getOpacity(); 
}

void CC3Scene::setOpacity( CCOpacity opacity )
{
	if ( m_pBackdrop )
		m_pBackdrop->setOpacity( opacity );
	super::setOpacity( opacity );
}

bool CC3Scene::init()
{ 
	initWithName( "" ); 
	return true;
}

void CC3Scene::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	m_lights = CCArray::create();			// retained
	m_lights->retain();
	m_lightProbes = CCArray::create();	// retained
	m_lightProbes->retain();
	m_billboards = CCArray::create();		// retained
	m_billboards->retain();
	setDrawingSequenceVisitor( CC3NodeSequencerVisitor::visitorWithScene( this ) );
	setDrawingSequencer( CC3BTreeNodeSequencer::sequencerLocalContentOpaqueFirst() );
	setViewDrawingVisitor( CC3NodeDrawingVisitor::visitor() );
	setEnvMapDrawingVisitor( NULL );
	setShadowVisitor( NULL );
	setUpdateVisitor( CC3NodeUpdatingVisitor::visitor() );
	setTouchedNodePicker( CC3TouchedNodePicker::pickerOnScene( this ) );

	CHECK_GL_ERROR_DEBUG();

	m_pLayer = NULL;
	m_pBackdrop = NULL;
	m_pFog = NULL;
	m_pActiveCamera = NULL;
	m_pPerformanceStatistics = NULL;
	m_ambientLight = kCC3DefaultLightColorAmbientScene;
	m_minUpdateInterval = kCC3DefaultMinimumUpdateInterval;
	m_maxUpdateInterval = kCC3DefaultMaximumUpdateInterval;
	m_deltaFrameTime = 0;
	m_timeAtOpen = 0;
	m_elapsedTimeSinceOpened = 0;
	m_shouldDisplayPickingRender = false;
	processInitializeScene();
	//LogGLErrorState(@"after initializing %@", self);
}

/** Establish 3D environment, initialize scene, then tear 3D environment down. */
void CC3Scene::processInitializeScene()
{
	CC3NodeDrawingVisitor* visitor = getViewDrawingVisitor();
	open3DWithVisitor( visitor );
	initializeScene();
	close3DWithVisitor( visitor );
}

// Default does nothing. Subclasses will customize.
void CC3Scene::initializeScene()
{

}

CC3Scene* CC3Scene::scene()
{
	CC3Scene* pScene = new CC3Scene;
	pScene->init();
	pScene->autorelease();
	return pScene;
}

void CC3Scene::populateFrom( CC3Scene* another )
{
	super::populateFrom( another );
	
	// Lights, light probes, billboards & drawing sequence collections,
	// plus activeCamera will be populated as children are added.
	// No need to configure node picker.
	
	setDrawingSequencer( (CC3NodeSequencer*)another->getDrawingSequencer()->copy()->autorelease() );
	setPerformanceStatistics( (CC3PerformanceStatistics*)another->getPerformanceStatistics()->copy()->autorelease() );

	CCAssert( false, "vistor population is not implemented" );
	// Env map visitor is created lazily
	//setViewDrawingVisitor = [[another->getViewDrawingVisitor class] visitor];		// retained
	//setShadowVisitor = [[another.shadowVisitor class] visitor];				// retained
	//setUpdateVisitor = [[another.updateVisitor class] visitor];				// retained
	//setDrawingSequenceVisitor = [[another.drawingSequenceVisitor class] visitorWithScene: self];	// retained
	//setTouchedNodePicker = [[another.touchedNodePicker class] pickerOnScene: self];		// retained

	setBackdrop( (CC3MeshNode*)another->getBackdrop()->copy()->autorelease() );
	setFog( (CC3Fog*)another->getFog()->copy()->autorelease() );
	
	m_ambientLight = another->getAmbientLight();
	m_minUpdateInterval = another->getMinUpdateInterval();
	m_maxUpdateInterval = another->getMaxUpdateInterval();
	m_shouldDisplayPickingRender = another->shouldDisplayPickingRender();
}

CCObject* CC3Scene::copyWithZone( CCZone* zone )
{
	CC3Scene* pVal = new CC3Scene;
	pVal->init();
	pVal->populateFrom( this );
	pVal->addCopiesOfChildrenFrom( this );

	return pVal;
}

void CC3Scene::open()
{
	m_timeAtOpen = CC3Platform::getCurrentMilliseconds();
	m_elapsedTimeSinceOpened = 0;
	
	play();
	updateScene();

	// Establish 3D environment, run scene open behaviour, then tear 3D environment down.
	CC3NodeDrawingVisitor* visitor = getViewDrawingVisitor();
	open3DWithVisitor( visitor );
	onOpen();
	close3DWithVisitor( visitor );
}

void CC3Scene::onOpen()
{
}

void CC3Scene::close()
{
	pause();
	onClose();
}

void CC3Scene::onClose()
{

}

void CC3Scene::play()
{
	setIsRunning( true );
}

void CC3Scene::pause()
{
	setIsRunning( false );
}

CC3NodeUpdatingVisitor* CC3Scene::getUpdateVisitor()
{
	return m_pUpdateVisitor;
}

void CC3Scene::setUpdateVisitor( CC3NodeUpdatingVisitor* visitor )
{
	CC_SAFE_RELEASE(m_pUpdateVisitor);
	CC_SAFE_RETAIN( visitor );
	m_pUpdateVisitor = visitor;
}

float CC3Scene::getMinUpdateInterval()
{
	return m_minUpdateInterval;
}

void CC3Scene::setMinUpdateInterval( float interval )
{
	m_minUpdateInterval = interval;
}

float CC3Scene::getMaxUpdateInterval()
{
	return m_maxUpdateInterval;
}

void CC3Scene::setMaxUpdateInterval( float interval )
{
	m_maxUpdateInterval = interval;
}

/**
 * If needed, clamps the specified interval value, then invokes a sequence of template methods.
 * Does nothing except update times if this instance is not running.
 */
void CC3Scene::updateScene( float dt )
{
	updateTimes( dt );

	if( !isRunning() )
		return;

	// Clamp the specified interval to a range defined by the minimum and maximum
	// update intervals. If the maximum update interval limit is zero or negative,
	// its value is ignored, and the dt value is not limited to a maximum value.
	m_deltaFrameTime = CLAMP(dt, m_minUpdateInterval,
							(m_maxUpdateInterval > 0.0 ? m_maxUpdateInterval : dt));
	
	//LogTrace(@"******* %@ starting update: %.2f ms (clamped from %.2f ms)",
	//		 self, _deltaFrameTime * 1000.0, dt * 1000.0);
	
	m_pTouchedNodePicker->dispatchPickedNode();
	
	m_pUpdateVisitor->setDeltaTime( m_deltaFrameTime );
	m_pUpdateVisitor->visit( this );
	
	updateCamera( m_deltaFrameTime );
	updateBillboards( m_deltaFrameTime );
	updateShadows( m_deltaFrameTime );
	updateDrawSequence();
	
	//LogTrace(@"******* %@ exiting update", self);
}

void CC3Scene::updateScene()
{
	bool wasRunning = m_isRunning;
	m_isRunning = true;
	updateScene( m_minUpdateInterval );
	m_isRunning = wasRunning;
}

GLfloat CC3Scene::getDeltaFrameTime()
{
	return m_deltaFrameTime;
}

/** Updates various scene timing values. */
void CC3Scene::updateTimes( float dt )
{
	m_elapsedTimeSinceOpened = CC3Platform::getCurrentMilliseconds() - m_timeAtOpen;
	
	if ( m_pPerformanceStatistics )
		m_pPerformanceStatistics->addUpdateTime( dt );
}

/** Template method to update the camera. */
void CC3Scene::updateCamera( float dt )
{

}

/** Template method to update shadows cast by the lights. */
void CC3Scene::updateShadows( float dt )
{
	CCObject* obj = NULL;
	CCARRAY_FOREACH( m_lights, obj )
	{
		CC3Light* lgt = (CC3Light*)obj;
		lgt->updateShadows();
	}
}

/**
 * Template method to update any billboards.
 * Iterates through all billboards, instructing them to align with the camera if needed.
 */
void CC3Scene::updateBillboards( float dt )
{
	CCObject* obj = NULL;
	CCARRAY_FOREACH( m_billboards, obj )
	{
		CC3Billboard* bb = (CC3Billboard*)obj;
		bb->alignToCamera( m_pActiveCamera );
	}

	//LogTrace(@"%@ updated %lu billboards", self, (unsigned long)_billboards.count);
}

void CC3Scene::drawScene()
{
	drawSceneWithVisitor( getViewDrawingVisitor() ); 
}
	
void CC3Scene::drawSceneWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	if ( !isVisible() ) 
		return;
	
	// Check and clear any GL error that occurred before 3D code
	// LogGLErrorState(@"before drawing %@", self);
	// LogTrace(@"******* %@ entering drawing visit", self);
	
	collectFrameInterval();	// Collect the frame interval in the performance statistics.

	open3DWithVisitor( visitor );
	
	m_pTouchedNodePicker->pickTouchedNodeWithVisitor( visitor );
	if ( !m_shouldDisplayPickingRender ) 
		drawSceneContentWithVisitor( visitor );

	close3DWithVisitor( visitor );

	draw2DBillboardsWithVisitor( visitor );	// Back to 2D now

	// Check and clear any GL error that occurred during 3D code
	//LogGLErrorState(@"after drawing %@", self);
	//LogTrace(@"******* %@ exiting drawing visit", self);
}

void CC3Scene::drawSceneContentWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	illuminateWithVisitor( visitor );		// Light up your world!
	drawBackdropWithVisitor( visitor );		// Draw the backdrop if it exists

	visitor->visit( this );					// Draw the scene components
	
	// Shadows are drawn with a specialized visitor
	if ( m_pShadowVisitor )
	{
		m_pShadowVisitor->alignShotWith( visitor );
		drawShadowsWithVisitor( m_pShadowVisitor );
	}
}

void CC3Scene::drawSceneContentForEnvironmentMapWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	visitor->getRenderSurface()->clearColorAndDepthContent();
	drawSceneContentWithVisitor( visitor );
}

void CC3Scene::drawBackdropWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	visitor->visit( getBackdrop() );
}

/**
 * Extract the interval since the previous frame from the CCDirector,
 * and add it to the performance statistics.
 */
void CC3Scene::collectFrameInterval()
{
	if (m_pPerformanceStatistics)
		m_pPerformanceStatistics->addFrameTime( (float)CCDirector::sharedDirector()->getAnimationInterval() );
}

void CC3Scene::open3DWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	//LogTrace(@"%@ opening the 3D scene", self);
	visitor->getGL()->alignFor3DDrawingWithVisitor( visitor );
}

void CC3Scene::close3DWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	//LogTrace(@"%@ closing the 3D scene", self);

	CC3OpenGL* gl = visitor->getGL();
	
	// Setup drawing configuration for Cocos2D
	gl->alignFor2DDrawingWithVisitor( visitor );

	// Re-align culling as expected by Cocos2D
	gl->enableCullFace( false );
	gl->setCullFace( GL_BACK );
	gl->setFrontFace( GL_CCW );

	// Make sure the drawing surface is set back to the view surface
	CC3RenderSurface* viewSurface = CC3ViewSurfaceManager::sharedViewSurfaceManager()->getRenderingSurface();
	if ( viewSurface )
	{
		viewSurface->activate();
		((CC3GLRenderbuffer*)viewSurface->getColorAttachment())->bind();
	}

	// Set depth testing to 2D values, and close depth testing,
	// either by turning it off, or clearing the depth buffer
	gl->setDepthFunc( GL_LEQUAL );
	gl->setDepthMask( true );
	closeDepthTestWithVisitor( visitor );
	
	// Reset the viewport to the 2D canvas and disable scissor clipping to the viewport.
	CCSize viewSize = CCDirector::sharedDirector()->getWinSizeInPixels();
	gl->setViewport( CC3ViewportMake(0, 0, (GLint)viewSize.width, (GLint)viewSize.height) );
	gl->enableScissorTest( false );

	// Disable lights and fog. Done outside alignFor2DDrawingWithVisitor because they apply to billboards
	gl->enableLighting( false );
	gl->enableTwoSidedLighting( false );
	CCObject* obj = NULL;
	CCARRAY_FOREACH( m_lights, obj )
	{
		CC3Light* lgt = (CC3Light*)obj;
		lgt->turnOffWithVisitor( visitor );
	}
	gl->enableFog( false );
}

void CC3Scene::closeDepthTestWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	visitor->getGL()->enableDepthTest( false );
}

void CC3Scene::illuminateWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	CC3Light::disableReservedLightsWithVisitor( visitor );	// disable any lights used by 2D scene

	//LogTrace(@"%@ lighting is %@", self, (self.isIlluminated ? @"on" : @"off"));

	// Set the ambient light for the whole scene
	visitor->getGL()->setSceneAmbientLightColor( m_ambientLight );

	// Turn on any individual lights
	CCObject* obj = NULL;
	CCARRAY_FOREACH( m_lights, obj )
	{
		CC3Light* lgt = (CC3Light*)obj;
		lgt->turnOnWithVisitor( visitor );
	}

	configureFogWithVisitor( visitor );
}

bool CC3Scene::isIlluminated()
{
	return (m_lights->count() > 0 ||
			!(ccc4FEqual(m_ambientLight, kCCC4FBlack) ||
			  ccc4FEqual(m_ambientLight, kCCC4FBlackTransparent)));
}

ccColor4F CC3Scene::getAmbientLight()
{
	return m_ambientLight;
}

void CC3Scene::setAmbientLight( const ccColor4F& color )
{
	m_ambientLight = color;
}

ccColor4F CC3Scene::getTotalIllumination()
{
	ccColor4F totLgt = getAmbientLight();
	//LogTrace(@"Start with scene ambient illumination %@", NSStringFromCCC4F(totLgt));
	CCObject* obj = NULL;
	CCARRAY_FOREACH( m_lights, obj )
	{
		CC3Light* lgt = (CC3Light*)obj;
		if ( lgt->isVisible() )
		{
			//LogTrace(@"Adding illumination from %@", lgt.fullDescription);
			ccColor4F la = lgt->getAmbientColor();
			ccColor4F ld = lgt->getDiffuseColor();
			totLgt.r += (la.r + ld.r);
			totLgt.g += (la.g + ld.g);
			totLgt.b += (la.b + ld.b);
			totLgt.a += (la.a + ld.a);
		}
	}

	return totLgt;
}

void CC3Scene::updateRelativeLightIntensities()
{
	ccColor4F totLgt = getTotalIllumination();
	CCObject* obj = NULL;
	CCARRAY_FOREACH( m_lights, obj )
	{
		CC3Light* lgt = (CC3Light*)obj;
		lgt->updateRelativeIntensityFrom( totLgt );
	}
}

CC3PerformanceStatistics* CC3Scene::getPerformanceStatistics()
{
	return m_pPerformanceStatistics;
}

void CC3Scene::setPerformanceStatistics( CC3PerformanceStatistics* statistics )
{
	CC_SAFE_RELEASE(m_pPerformanceStatistics);
	CC_SAFE_RETAIN(statistics);
	m_pPerformanceStatistics = statistics;
}

bool CC3Scene::doesContainShadows()
{
	return m_pShadowVisitor != NULL; 
}

/** Template method to draw shadows cast by the lights. */
void CC3Scene::drawShadowsWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	if ( !doesContainShadows() )
		return;

	visitor->getGL()->setClearStencil( 0 );
	visitor->getRenderSurface()->clearStencilContent();

	CCObject* obj = NULL;
	CCARRAY_FOREACH( m_lights, obj )
	{
		CC3Light* lgt = (CC3Light*)obj;
		lgt->drawShadowsWithVisitor( visitor );
	}
}

/** If this scene contains fog, configure it in the GL engine. */
void CC3Scene::configureFogWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	visitor->getGL()->bindFog( m_pFog, visitor );
}

/**
 * Draws any 2D overlay billboards.
 * This is invoked after close3D, so the drawing of billboards occurs in 2D.
 */
void CC3Scene::draw2DBillboardsWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	/*if (_billboards.count == 0) return;
	
	LogTrace(@"%@ drawing %lu billboards", self, (unsigned long)_billboards.count);
	CC3Viewport vp = self.activeCamera.viewport;
	CGRect localBounds = CGRectMake(0.0f, 0.0f, vp.w, vp.h);

	CCRenderer* renderer = visitor.billboardCCRenderer;
	[renderer invalidateState];
	
	for (CC3Billboard* bb in _billboards)
		[bb draw2dWithinBounds: localBounds withRenderer: renderer withVisitor: visitor];
	
	[renderer flush];*/
}

CC3NodeDrawingVisitor* CC3Scene::getViewDrawingVisitor()
{
	return m_pViewDrawingVisitor;
}

void CC3Scene::setViewDrawingVisitor( CC3NodeDrawingVisitor* visitor )
{
	CC_SAFE_RELEASE( m_pViewDrawingVisitor );
	CC_SAFE_RETAIN( visitor );

	m_pViewDrawingVisitor = visitor;
}

CC3NodeDrawingVisitor* CC3Scene::getEnvMapDrawingVisitor()
{
	if ( !m_pEnvMapDrawingVisitor ) 
	{
		setEnvMapDrawingVisitor( CC3NodeDrawingVisitor::visitor() );
		m_pEnvMapDrawingVisitor->setIsDrawingEnvironmentMap( true );
		m_pEnvMapDrawingVisitor->setCamera( CC3Camera::nodeWithName( "EnvMapCamera" ) );
		m_pEnvMapDrawingVisitor->getCamera()->setFieldOfView( 90.0f );
	}
	return m_pEnvMapDrawingVisitor;
}

void CC3Scene::setEnvMapDrawingVisitor( CC3NodeDrawingVisitor* visitor )
{
	CC_SAFE_RELEASE(m_pEnvMapDrawingVisitor);
	CC_SAFE_RETAIN( visitor );
	m_pEnvMapDrawingVisitor = visitor;
}

CC3NodeDrawingVisitor* CC3Scene::getShadowVisitor()
{
	return m_pShadowVisitor;
}

void CC3Scene::setShadowVisitor( CC3NodeDrawingVisitor* visitor )
{
	CC_SAFE_RELEASE(m_pShadowVisitor);
	CC_SAFE_RETAIN(visitor);
	m_pShadowVisitor = visitor;
}

void CC3Scene::setDrawingSequenceVisitor( CC3NodeSequencerVisitor* visitor )
{
	CC_SAFE_RELEASE(m_pDrawingSequenceVisitor);
	CC_SAFE_RETAIN(visitor);
	m_pDrawingSequenceVisitor = visitor;
}

bool CC3Scene::isUsingDrawingSequence()
{
	return (m_pDrawingSequencer != NULL); 
}

CC3NodeSequencer* CC3Scene::getDrawingSequencer()
{
	return m_pDrawingSequencer; 
}

/**
 * Property setter overridden to add all the decendent nodes of this scene
 * into the new  node sequencer.
 */
void CC3Scene::setDrawingSequencer( CC3NodeSequencer* aNodeSequencer )
{
	if (aNodeSequencer == m_pDrawingSequencer) 
		return;
	CC_SAFE_RELEASE( m_pDrawingSequencer );
	m_pDrawingSequencer = aNodeSequencer;
	CC_SAFE_RETAIN( aNodeSequencer );
	
	if (m_pDrawingSequencer) 
	{
		CCArray* allNodes = flatten();

		CCObject* obj = NULL;
		CCARRAY_FOREACH( allNodes, obj )
		{
			CC3Node* aNode = (CC3Node*)obj;
			m_pDrawingSequencer->add( aNode, m_pDrawingSequenceVisitor );
		}
	}
}

void CC3Scene::updateDrawSequence()
{
	if (m_pDrawingSequencer && m_pDrawingSequencer->allowSequenceUpdates())
	{
		m_pDrawingSequencer->updateSequenceWithVisitor( m_pDrawingSequenceVisitor );
		//LogTrace(@"%@ updated %@", self, [_drawingSequencer fullDescription]);
	}
}

/**
 * A property on a descendant node has changed that potentially affects its order in
 * the drawing sequence. To put it in the correct drawing order, remove it from the
 * drawingSequencer and then re-add it.
 */
void CC3Scene::descendantDidModifySequencingCriteria( CC3Node* aNode )
{
	if (m_pDrawingSequencer)
		if (m_pDrawingSequencer->remove( aNode, m_pDrawingSequenceVisitor))
			m_pDrawingSequencer->add( aNode, m_pDrawingSequenceVisitor );
}

CC3Scene* CC3Scene::getScene()
{
	return this;
}

/**
 * Overridden to attempt to add each node to the drawingSequencer, and to add any nodes
 * that require special handling, like cameras, lights and billboards to their respective
 * caches. The node being added is first flattened, so that this processing is performed
 * not only on that node, but all its hierarchical decendants.
 */
void CC3Scene::didAddDescendant( CC3Node* aNode )
{
	//LogTrace(@"Adding %@ as descendant to %@", aNode, self);
	
	// Collect all the nodes being added, including all descendants,
	// and see if they require special treatment
	CCArray* allAdded = aNode->flatten();
	CCObject* obj = NULL;
	CCARRAY_FOREACH( allAdded, obj )
	{
		CC3Node* addedNode = (CC3Node*)obj;
		// Attempt to add the node to the draw sequence sorter.
		m_pDrawingSequencer->add( addedNode, m_pDrawingSequenceVisitor );

		// If the node is a light, add it to the collection of lights
		if (addedNode->isLight())
			m_lights->addObject( addedNode );

		// If the node is a light probe, add it to the collection of light probes
		if (addedNode->isLightProbe())
			m_lightProbes->addObject( addedNode );

		// if the node is the first camera to be added, make it the active camera.
		if (addedNode->isCamera() && !m_pActiveCamera) 
			setActiveCamera( (CC3Camera*)addedNode );

		// If the node is a billboard, add it to the collection of billboards
		if (addedNode->isBillboard()) 
			m_billboards->addObject( addedNode );

		// If the node is a shadow, check if we need to add the shadow visitor
		if (addedNode->isShadowVolume()) 
			checkNeedShadowVisitor();
	}
}

/**
 * Overridden to attempt to remove each node from the drawingSequencer, and to remove any nodes
 * that require special handling, like lights and billboards from their respective caches.
 * The node being removed is first flattened, so that this processing is performed not only
 * on that node, but all its hierarchical decendants.
 */
void CC3Scene::didRemoveDescendant( CC3Node* aNode )
{
	//LogTrace(@"Removing %@ as descendant of %@", aNode, self);
	
	// Collect all the nodes being removed, including all descendants,
	// and see if they require special treatment
	CCArray* allRemoved = aNode->flatten();
	CCObject* obj = NULL;
	CCARRAY_FOREACH( allRemoved, obj )
	{
		CC3Node* removedNode = (CC3Node*)obj;
		// Attempt to remove the node to the draw sequence sorter.
		m_pDrawingSequencer->remove( removedNode, m_pDrawingSequenceVisitor );

		// If the node is a light, remove it from the collection of lights
		if (removedNode->isLight())
			m_lights->removeObject( removedNode );

		// If the node is a light probe, remove it from the collection of light probes
		if (removedNode->isLightProbe())
			m_lightProbes->removeObject( removedNode );

		// If the node is a billboard, remove it from the collection of billboards
		if (removedNode->isBillboard()) 
			m_billboards->removeObject( removedNode );

		// If the node is a shadow, check if we need to remove the shadow visitor
		if (removedNode->isShadowVolume()) 
			checkNeedShadowVisitor();
	}
}

/**
 * Check to see if any lights are casting shadows. If they are, ensure the shadowVisitor
 * exists. If no lights are casting a shadow, and the shadowVisitor exists, remove it.
 */
void CC3Scene::checkNeedShadowVisitor()
{
	bool needsShadowVisitor = false;

	CCObject* obj = NULL;
	CCARRAY_FOREACH( m_lights, obj )
	{
		CC3Light* lgt = (CC3Light*)obj;
		needsShadowVisitor |= lgt->hasShadows();
	}

	if (needsShadowVisitor && !m_pShadowVisitor) 
		setShadowVisitor( CC3ShadowDrawingVisitor::visitor() );

	if (!needsShadowVisitor && m_pShadowVisitor) 
		setShadowVisitor( NULL );
}

void CC3Scene::touchEvent( GLuint touchType, const CCPoint& touchPoint )
{
	switch (touchType) 
	{
		case kCCTouchBegan:
			pickNodeFromTouchEvent( touchType, touchPoint );
			break;
		case kCCTouchMoved:
			break;
		case kCCTouchEnded:
			break;
		default:
			break;
	}
}

void CC3Scene::pickNodeFromTapAt( const CCPoint& tPoint )
{
	pickNodeFromTouchEvent( kCCTouchEnded, tPoint );
}

void CC3Scene::pickNodeFromTouchEvent( GLuint tType, const CCPoint& tPoint )
{
	if ( m_pTouchedNodePicker )
		m_pTouchedNodePicker->pickNodeFromTouchEvent( tType, tPoint );
}

/** Default does nothing. Subclasses that handle touch events will override. */
void CC3Scene::nodeSelected( CC3Node* aNode, GLuint touchType, const CCPoint& touchPoint )
{

}

unsigned long CC3Scene::getElapsedTimeSinceOpened()
{
	return m_elapsedTimeSinceOpened;
}

CCArray* CC3Scene::getLightProbes()
{
	return m_lightProbes;
}

CC3TouchedNodePicker::CC3TouchedNodePicker()
{
	m_pScene = NULL;				// weak reference
	m_pPickVisitor = NULL;
	m_pPickedNode = NULL;;
}

CC3TouchedNodePicker::~CC3TouchedNodePicker()
{
	m_pScene = NULL;				// weak reference
	CC_SAFE_RELEASE( m_pPickVisitor );
	CC_SAFE_RELEASE( m_pPickedNode );
}

CC3NodePickingVisitor* CC3TouchedNodePicker::getPickVisitor()
{
	return m_pPickVisitor;
}

void CC3TouchedNodePicker::setPickVisitor( CC3NodePickingVisitor* visitor )
{
	CC_SAFE_RELEASE( m_pPickVisitor );
	CC_SAFE_RETAIN( visitor );
	m_pPickVisitor = visitor;
}

CCPoint	CC3TouchedNodePicker::getTouchPoint()
{
	return m_touchPoint;
}

CC3Node* CC3TouchedNodePicker::getPickedNode()
{
	return m_pPickedNode;
}

void CC3TouchedNodePicker::setPickedNode( CC3Node* node )
{
	CC_SAFE_RETAIN( node );
	CC_SAFE_RELEASE( m_pPickedNode );
	m_pPickedNode = node;
}

void CC3TouchedNodePicker::pickNodeFromTouchEvent( GLuint tType, const CCPoint& tPoint )
{
	// If the touch type is different than the previous touch type,
	// add the touch type to the queue. Only the types are queued...not the location.

	/*@synchronized(self)*/ 
	{
		if (m_queuedTouchCount == 0 || tType != m_touchQueue[m_queuedTouchCount - 1] ) {
			if (m_queuedTouchCount == kCC3TouchQueueLength) m_queuedTouchCount = 0;
			m_touchQueue[m_queuedTouchCount++] = tType;
			m_wasTouched = true;
		}
	}

	// Update the touch location, even if the touch type is the same as the previous touch.
	m_touchPoint = tPoint;

	//LogTrace(@"%@ touched %@ at %@. Queue length now %u.", self, NSStringFromTouchType(tType),
	//		 NSStringFromCGPoint(_touchPoint), _queuedTouchCount);
}

void CC3TouchedNodePicker::pickTouchedNodeWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	if ( !(m_wasTouched || m_pScene->shouldDisplayPickingRender()) ) 
		return;
	
	m_wasPicked = m_wasTouched;
	m_wasTouched = false;
	
	if ( m_pPickVisitor )
	{
		// Draw the scene for node picking. Don't bother drawing the backdrop.
		m_pPickVisitor->alignShotWith( visitor );
		m_pPickVisitor->visit( m_pScene );

		setPickedNode( m_pPickVisitor->getPickedNode() );
	}
}

void CC3TouchedNodePicker::dispatchPickedNode()
{
	if ( !m_wasPicked ) 
		return;

	m_wasPicked = false;
		
	GLuint touchesToDispatch[kCC3TouchQueueLength];

	GLuint touchCount;
	/*@synchronized(self) */
	{
		touchCount = m_queuedTouchCount;
		memcpy( touchesToDispatch, m_touchQueue, (touchCount * sizeof(GLuint)) );
		m_queuedTouchCount = 0;
	}

	if ( m_pPickedNode )
	{
		for (GLuint i = 0; i < touchCount; i++) 
		{
			//LogTrace(@"%@ dispatching %@ with picked node %@ at %@ touched node %@",
			//		 self, NSStringFromTouchType(_touchQueue[i]), _pickedNode.touchableNode,
			//		 NSStringFromCGPoint(_touchPoint), _pickedNode);
			m_pScene->nodeSelected( m_pPickedNode->getTouchableNode(), m_touchQueue[i], m_touchPoint );
		}
	}
	
	setPickedNode( NULL );	// Clear the node once it has been dispatched
}


void CC3TouchedNodePicker::init()
{
	initOnScene( NULL ); 
}

void CC3TouchedNodePicker::initOnScene( CC3Scene* aCC3Scene )
{
	m_pScene = aCC3Scene;					// weak reference
	setPickVisitor( CC3NodePickingVisitor::visitor() );
	m_touchPoint = CCPointZero;
	m_wasTouched = false;
	m_wasPicked = false;
	m_pPickedNode = NULL;
	m_queuedTouchCount = 0;
}

CC3TouchedNodePicker* CC3TouchedNodePicker::pickerOnScene( CC3Scene* aCC3Scene )
{
	CC3TouchedNodePicker* picker = new CC3TouchedNodePicker;
	picker->initOnScene( aCC3Scene );
	picker->autorelease();

	return picker;
}


NS_COCOS3D_END
