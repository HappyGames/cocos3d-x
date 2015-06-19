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

CC3Layer::CC3Layer()
{
	_surfaceManager = NULL;
	_cc3Scene = NULL;
	_cc3GestureRecognizers = NULL;
}

CC3Layer::~CC3Layer()
{
	setCC3Scene( NULL );
	CC_SAFE_RELEASE( _surfaceManager );
	cc3RemoveAllGestureRecognizers();
	CC_SAFE_RELEASE( _cc3GestureRecognizers );
}

CC3Scene* CC3Layer::getCC3Scene()
{
	return _cc3Scene;
}

void CC3Layer::setCC3Scene( CC3Scene* aScene )
{
	 if ( aScene == _cc3Scene ) 
		 return;

	 if ( _cc3Scene )
	 {
		 closeCC3Scene();						// Close the old scene.
		 _cc3Scene->wasRemoved();					// Stop actions in old scene (if shouldStopActionsWhenRemoved set).
		 // _cc3Scene.deprecatedCC3Layer = nil;		// Detach this layer from old scene.
		 _cc3Scene->release();
	 }

	 _cc3Scene = aScene;
	 CC_SAFE_RETAIN( aScene );

	// _cc3Scene.deprecatedCC3Layer = self;					// Point the scene back here
	 if ( isRunningInActiveScene() ) 
		 openCC3Scene();	// If already running, open the new scene right away
}

/** 
 * Override to set the shouldAlwaysUpdateViewport to YES if the parent is not the root CCScene,
 * so that the viewport will be updated as ancestor nodes are moved around.
 */
void CC3Layer::setParent( CCNode* parent )
{
	super::setParent( parent );
#pragma _NOTE_TODO( "setShouldAlwaysUpdateViewport here" )
	//setShouldAlwaysUpdateViewport( (parent && ![parent isKindOfClass: CCScene.class] );
}

bool CC3Layer::init()
{
	_shouldTrackViewSize = true;		// Could be overridden during init if contentSize set to something other than view size
	super::init();
	_shouldAlwaysUpdateViewport = false;
	initializeControls();

	return true;
}

void CC3Layer::initializeControls()
{

}

std::string CC3Layer::description()
{
	// return [NSString stringWithFormat: @"%@ on %@", self.class, _cc3Scene]; 
	return "CC3Layer";
}

void CC3Layer::setPosition( const CCPoint& newPosition )
{
	super::setPosition( newPosition );
	updateViewport();
}

void CC3Layer::setScale( float s )
{
	super::setScale( s );
	updateViewport();
}

void CC3Layer::setScaleX( float newScaleX )
{
	super::setScaleX( newScaleX );
	updateViewport();
}

void CC3Layer::setScaleY( float newScaleY )
{
	super::setScaleY( newScaleY );
	updateViewport();
}

CCColorRef CC3Layer::getColor()
{
	return getCC3Scene()->getColor(); 
}

void CC3Layer::setColor( const CCColorRef& color )
{
	getCC3Scene()->setColor( color );  
}

CCOpacity CC3Layer::getOpacity()
{
	return getCC3Scene()->getOpacity(); 
}

void CC3Layer::setOpacity( CCOpacity opacity )
{
	getCC3Scene()->setOpacity( opacity );  
}

CC3SceneDrawingSurfaceManager* CC3Layer::getSurfaceManager()
{
	if (!_surfaceManager) 
	{
		_surfaceManager = CC3SceneDrawingSurfaceManager::surfaceManager();
		_surfaceManager->retain();

	}
	return _surfaceManager;
}

void CC3Layer::setSurfaceManager( CC3SceneDrawingSurfaceManager* surfaceManager )
{
	/*CC3Assert([surfaceManager isKindOfClass: self.surfaceManagerClass],
			  @"The surface manager must be a type of %@", self.surfaceManagerClass);*/

	if (surfaceManager == _surfaceManager) 
		return;

	CC_SAFE_RELEASE( _surfaceManager );
	CC_SAFE_RETAIN( surfaceManager );
	_surfaceManager = surfaceManager;

	updateViewport();
}

/** Invoked from Cocos2D when this layer is first displayed. Opens the 3D scene. */
void CC3Layer::onEnter()
{
	super::onEnter();
	onOpenCC3Layer();
	openCC3Scene();
}

void CC3Layer::onOpenCC3Layer()
{

}

/** Invoked automatically either from onEnter, or if new scene attached and layer is running. */
void CC3Layer::openCC3Scene()
{
	updateViewport();			// Set the camera viewport
	getCC3Scene()->open();			// Open the scene
}

/** Invoked from Cocos2D when this layer is removed. Closes the 3D scene.  */
void CC3Layer::onExit()
{
	closeCC3Scene();
	onCloseCC3Layer();
	cc3RemoveAllGestureRecognizers();
	super::onExit();
}

void CC3Layer::onCloseCC3Layer()
{

}

/** Invoked automatically either from onExit, or if old scene removed and layer is running. */
void CC3Layer::closeCC3Scene()
{
	// Must not use property accessor!
	_cc3Scene->close();
}	

bool CC3Layer::shouldTrackViewSize()
{
	return _shouldTrackViewSize;
}

void CC3Layer::setShouldTrackViewSize( bool shouldTrack )
{
	_shouldTrackViewSize = shouldTrack;
}

void CC3Layer::update( float dt )
{
	getCC3Scene()->updateScene( dt ); 
}
 
// Lazily initialized
CCArray* CC3Layer::getCC3GestureRecognizers()
{
	if ( !_cc3GestureRecognizers ) 
	{
		_cc3GestureRecognizers = CCArray::create();	// retained
		_cc3GestureRecognizers->retain();
	}

	return _cc3GestureRecognizers;
}

//void CC3Layer::addGestureRecognizer( CCGestureRecognizer* gesture )
//{
//	getCC3GestureRecognizers()->addObject( gesture );
//	// CCDirector.sharedDirector.view addGestureRecognizer: gesture];
//}
//
//void cc3RemoveGestureRecognizer: (UIGestureRecognizer*) gesture {
//	[CCDirector.sharedDirector.view removeGestureRecognizer: gesture];
//	[_cc3GestureRecognizers removeObjectIdenticalTo: gesture];
//}
//
void CC3Layer::cc3RemoveAllGestureRecognizers()
{
	//NSArray* myGRs = [_cc3GestureRecognizers copy];
	//for (UIGestureRecognizer* gr in myGRs) [self cc3RemoveGestureRecognizer: gr];
	//[myGRs release];
}

/** Draw the 3D scene with the specified drawing visitor. */
void CC3Layer::drawSceneWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	// Ensure the visitor uses the surface manager of this layer
	visitor->setSurfaceManager( getSurfaceManager() );
	
	if (_shouldAlwaysUpdateViewport) 
		updateViewport();
	
	getCC3Scene()->drawSceneWithVisitor( visitor );
}

/** Drawing under Cocos2D 3.0 and before. */
void CC3Layer::draw()
{
	drawSceneWithVisitor( getCC3Scene()->getViewDrawingVisitor() ); 
}

void CC3Layer::onContentSizeChanged()
{
	updateViewport();

	if ( !getContentSize().equals(  CCDirector::sharedDirector()->getWinSize() ) )
		setShouldTrackViewSize( false );
}

void CC3Layer::updateViewport()
{
	CCSize viewSize = CCDirector::sharedDirector()->getWinSizeInPixels();
	CCRect gbb = CCRect(0, 0, viewSize.width, viewSize.height);
	
	// Check whether the viewport covers the full UIView.
	bool isFullView = gbb.origin.equals(CCPointZero) &&
					   gbb.size.equals(viewSize);

	// Convert the bounds of this layer to a viewport
	CC3Viewport vp = CC3ViewportFromCGRect(gbb);
	
	// Set the viewport into the view surface and the camera
	if ( _surfaceManager )
	{
		_surfaceManager->setViewSurfaceOrigin( vp.origin );
		_surfaceManager->setSize( vp.size );
	}

	CC3Scene* scene = getCC3Scene();
	if ( scene )
	{
		CC3Camera* cam = scene->getActiveCamera();
		if ( cam )
		{
			cam->setViewport( vp );
			cam->setShouldClipToViewport( !isFullView );
		}
	}
	
#pragma _NOTE_TODO( "update cocos2d viewport" )
	// super::updateViewport();
}

/**
 * Invoked automatically when the OS view has been resized.
 * Ensure view surfaces are resized, and if appropriate, resize this layer.
 */
void CC3Layer::viewDidResizeTo( const CCSize& newViewSize )
{
	// Ensure the size of all view surfaces is updated to match new view size.
	CC3ViewSurfaceManager::sharedViewSurfaceManager()->setSize( CC3IntSizeFromCGSize(newViewSize) );
	
	// If this layer should track the size of the view, update the size of this layer.
	if ( shouldTrackViewSize() ) 
		setContentSize( CCNodeSizeFromViewSize(newViewSize) );

	// super::viewDidResizeTo( newViewSize );	// Propagate to descendants
#pragma _NOTE_TODO( "viewDidResizeTo" )
}

/**
 * The priority at which touch events are delegated to this layer.
 * Default is zero. Subclasses may override.
 */
int CC3Layer::getTouchPriority()
{
	return 0; 
}

/**
 * The priority at which mouse events are delegated to this layer.
 * Default is zero. Subclasses may override.
 */
int CC3Layer::getMouseDelegatePriority()
{
	return 0; 
}

bool CC3Layer::isRunningInActiveScene()
{
	return isRunning();
}

/** 
 * Processes an iOS touch or OSX mouse event at the specified point and returns whether
 * the touch event was handled or not.
 *
 * Verifies that the specified touch point is within the bounds of this layer,
 * then invokes the handleTouchType:at: method.
 */
bool CC3Layer::validateAndProcessTouchAt( const CCPoint& touchPoint, GLuint touchType )
{
	CCSize cs = getContentSize();
	CCRect nodeBounds = CCRectMake(0, 0, cs.width, cs.height);
	if ( nodeBounds.containsPoint( touchPoint ) ) 
	{
		// LogTrace(@"%@ touched at: %@", self, NSStringFromCGPoint(touchPoint));
		return handleTouchType( touchType, touchPoint );
	}
	return false;
}

bool CC3Layer::handleTouchType( GLuint touchType, const CCPoint& touchPoint )
{
	getCC3Scene()->touchEvent( touchType, touchPoint );
	return true;
}


/** Lazily allocate and populate a char string built from the description property. */
std::string CC3Layer::getRenderStreamGroupMarker()
{
	return _renderStreamGroupMarker;
}

bool CC3Layer::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	return validateAndProcessTouchAt( pTouch->getLocation(), kCCTouchBegan );
}

void CC3Layer::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	validateAndProcessTouchAt( pTouch->getLocation(), kCCTouchMoved );
}

void CC3Layer::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	validateAndProcessTouchAt( pTouch->getLocation(), kCCTouchEnded );
}

void CC3Layer::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{
	validateAndProcessTouchAt( pTouch->getLocation(), kCCTouchCancelled );
}

void CC3Layer::setContentSize( const CCSize& contentSize )
{
	CCSize oldSize = getContentSize();
	super::setContentSize( contentSize );
	if ( !contentSize.equals( oldSize ) ) 
	{
		/// invoke callback
		onContentSizeChanged();
	}
}

NS_COCOS3D_END
