#include "CC3MashUpLayer.h"
#include "CC3MashUpScene.h"
#include "HUDLayer.h"
#include "HUDScene.h"
#include "ccMacros.h"


/** Cocos2D v3 auto-scales images for Retina. Cocos2D v2 & v1 do not. This affects the button sizes. */
#define kSpriteScale					(CCDirector::sharedDirector()->getContentScaleFactor())

/** Scale and position the buttons so they are usable at various screen resolutions. */

#define kControlSizeScale				(CCDirector::sharedDirector()->getContentScaleFactor() / kSpriteScale)
#define kControlPositionScale			1.0f

/** Parameters for setting up the joystick and button controls */
#define kJoystickSideLength				(80.0f * kControlPositionScale)
#define kButtonGrid						(40.0f * kControlPositionScale)
#define kJoystickPadding				(8.0f * kControlPositionScale)
#define kHUDPadding						(8.0f * kControlPositionScale)
#define kJoystickThumbFileName			"JoystickThumb.png"
#define kSwitchViewButtonFileName		"ArrowLeftButton48x48.png"
#define kInvasionButtonFileName			"GridButton48x48.png"
#define kSunlightButtonFileName			"SunlightButton48x48.png"
#define kZoomButtonFileName				"ZoomButton48x48.png"
#define kShadowButtonFileName			"ShadowButton48x48.png"
#define kShadowButtonLatchedFileName	"ShadowButtonLatched48x48.png"
#define kButtonRingFileName				"ButtonRing48x48.png"
#define kButtonShineFileName			"Shine48x48.png"
#define kGlobeName						"Globe"
#define kPeakShineOpacity				180
#define kButtonAdornmentScale			1.5.f


// Gesture support under Android is less sophisticated and more challenging than under iOS.
// When running on Android, avoid using gestures, and use underlying touch events instead.
// You can change this definition if you don't want to use gestures at all.
#define kShouldUseGestures				(CC3_IOS && !APPORTABLE)

/**
 * Returns the contained CC3Scene, cast into the appropriate type.
 * This is a convenience method to perform automatic casting.
 */
CC3MashUpScene* CC3MashUpLayer::getMashUpScene()
{
	return (CC3MashUpScene*) getCC3Scene(); 
}

void CC3MashUpLayer::setCC3Scene( CC3MashUpScene* aScene )
{
	super::setCC3Scene( aScene );
	aScene->setPrimaryCC3MashUpLayer( this );	// Point the scene back here
}

void CC3MashUpLayer::initializeControls()
{	
	setTouchEnabled( true );
	// Set the userInteractionEnabled property to NO to control the scene using gestures,
	// and to YES to control the scene using lower-level touch and mouse events.
	//setUserInteractionEnabled( !kShouldUseGestures );
	
	addJoysticks();
	addSwitchViewButton();
	addInvasionButton();
	addSunlightButton();
	addZoomButton();
	addShadowButton();
	positionButtons();
	scheduleUpdate();		// Schedule updates on each frame
}

void CC3MashUpLayer::registerWithTouchDispatcher()
{	
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);  
}

/** Creates the two joysticks that control the 3D camera direction and location. */
void CC3MashUpLayer::addJoysticks()
{
	CCSprite* jsThumb;

	// The joystick that controls the player's (camera's) direction
	jsThumb = CCSprite::create( kJoystickThumbFileName );
	jsThumb->setScale( kControlSizeScale );
	
	_directionJoystick = Joystick::joystickWithThumb( jsThumb, CCSizeMake(kJoystickSideLength, kJoystickSideLength) );

	// If you want to see the size of the Joystick backdrop, comment out the line above and uncomment the
	// following line, which include a simple backdrop image. The thumb and backdrop can be any CCNode.
	//_directionJoystick = Joystick::joystickWithThumb( jsThumb, CCSprite::create( "BrushedSteel.png" ) );
	
	CCPoint jsMiddle = _directionJoystick->getAnchorPointInPoints();
	_directionJoystick->setPosition( ccp(jsMiddle.x + kJoystickPadding, jsMiddle.y + kJoystickPadding) );
	addChild( _directionJoystick );
	
	// The joystick that controls the player's (camera's) location
	jsThumb = CCSprite::create( kJoystickThumbFileName );
	jsThumb->setScale( kControlSizeScale );
	
	_locationJoystick = Joystick::joystickWithThumb( jsThumb, CCSizeMake(kJoystickSideLength, kJoystickSideLength) );
	addChild( _locationJoystick );
	positionLocationJoystick();
}

/**
 * Creates a button that will allow the user to switch between different views of the 3D scene.
 * The type of button depends on whether we are using Cocos2D v3, or the legacy Cocos2D v2/v1.
 */
void CC3MashUpLayer::addSwitchViewButton()
{
	// The button uses an adornment, which is displayed whenever the button is selected.
	CCNodeAdornmentBase* adornment;
	
	// The adornment is a ring that fades in around the button and fades out when the button is deselected.
	CCSprite* ringSprite = CCSprite::create( kButtonRingFileName );
	adornment = CCNodeAdornmentOverlayFader::adornmentWithSprite( ringSprite );
	adornment->setZOrder( kAdornmentUnderZOrder );
	
	// Or the adornment could scale the button it is when selected.
	// To try a scaler adornment, uncomment the following line.
//	adornment = [CCNodeAdornmentScaler adornmentToScaleUniformlyBy: kButtonAdornmentScale];

	_switchViewButton = addButtonWithCallbackSelector( SEL_MenuHandler(&CC3MashUpLayer::switchViewSelected), kSwitchViewButtonFileName, adornment );
}

/**
 * Creates a button that will allow the user to create a robot invasion.
 * The type of button depends on whether we are using Cocos2D v3, or the legacy Cocos2D v2/v1.
 */
void CC3MashUpLayer::addInvasionButton()
{
	// The button uses an adornment, which is displayed whenever the button is selected.
	// The adornment is a ring that fades in around the button and fades out when the button is deselected.
	CCSprite* ringSprite = CCSprite::create( kButtonRingFileName );
	CCNodeAdornmentBase* adornment = CCNodeAdornmentOverlayFader::adornmentWithSprite( ringSprite );
	adornment->setZOrder( kAdornmentUnderZOrder );

	_invasionButton = addButtonWithCallbackSelector( SEL_MenuHandler(&CC3MashUpLayer::invade), kInvasionButtonFileName, adornment );
}

/**
 * Creates a button that will allow the user to turn the sun on or off.
 * The type of button depends on whether we are using Cocos2D v3, or the legacy Cocos2D v2/v1.
 */
void CC3MashUpLayer::addSunlightButton()
{	
	// The button uses an adornment, which is displayed whenever the button is selected.
	// The adornment is a ring that fades in around the button and fades out when the button is deselected.
	CCSprite* ringSprite = CCSprite::create( kButtonRingFileName );
	CCNodeAdornmentBase* adornment = CCNodeAdornmentOverlayFader::adornmentWithSprite( ringSprite );
	adornment->setZOrder( kAdornmentUnderZOrder );

	_sunlightButton = addButtonWithCallbackSelector( SEL_MenuHandler(&CC3MashUpLayer::cycleLights), kSunlightButtonFileName, adornment );
}

/**
 * Creates a button that will allow the user to move between viewing the whole scene and
 * viewing from the previous position.
 * The type of button depends on whether we are using Cocos2D v3, or the legacy Cocos2D v2/v1.
 */
void CC3MashUpLayer::addZoomButton()
{
	// The button uses an adornment, which is displayed whenever the button is selected.
	// The adornment is a shine that fades in on the button and fades out when the button is deselected.
	CCSprite* shineSprite = CCSprite::create( kButtonShineFileName );
	shineSprite->setColor( CCColorRefFromCCC4F(kCCC4FWhite) );
	CCNodeAdornmentBase* adornment = CCNodeAdornmentOverlayFader::adornmentWithSprite( shineSprite, kPeakShineOpacity );
	_zoomButton = addButtonWithCallbackSelector( SEL_MenuHandler(&CC3MashUpLayer::cycleZoom), kZoomButtonFileName, adornment );
}

/**
 * Creates a toggle button that will allow the user to toggle shadows on and off for a selected node.
 * The type of button depends on whether we are using Cocos2D v3, or the legacy Cocos2D v2/v1.
 */
void CC3MashUpLayer::addShadowButton()
{
	// The button uses an adornment, which is displayed whenever the button is selected. The adornment
	// is a ring that fades in around the button and fades out when the button is no longer selected.
	CCSprite* ringSprite = CCSprite::create( kButtonRingFileName );
	CCNodeAdornmentBase* adornment = CCNodeAdornmentOverlayFader::adornmentWithSprite( ringSprite );
	adornment->setZOrder( kAdornmentUnderZOrder );

	_shadowButton = addToggleButtonWithCallbackSelector( SEL_MenuHandler(&CC3MashUpLayer::toggleShadows),
												kShadowButtonFileName,
												kShadowButtonLatchedFileName,
												adornment );
}

 AdornableMenuItemImage* CC3MashUpLayer::addButtonWithCallbackSelector( SEL_MenuHandler callBackSelector, const char* imgFileName, CCNodeAdornment* adornment )
 {
	AdornableMenuItemImage* button = NULL;
	
	button = AdornableMenuItemImage::create( imgFileName, imgFileName, this, callBackSelector );
	CCMenu* viewMenu = CCMenu::create( button, NULL );
	viewMenu->setPosition( CCPointZero );
	addChild( viewMenu );
	
	adornment->setPosition( ccpCompMult(ccpFromSize(button->getContentSize()), button->getAnchorPoint()) );
	button->setAdornment( adornment );
	
	button->setScale( kControlSizeScale );
	return button;
}

AdornableMenuItemToggle* CC3MashUpLayer::addToggleButtonWithCallbackSelector( SEL_MenuHandler callBackSelector, const char* imgFileName,
								   const char* imgLatchFileName, CCNodeAdornment* adornment )
{
	AdornableMenuItemToggle* button;
	
	CCMenuItem* b = CCMenuItemImage::create( imgFileName, imgFileName );
	CCMenuItem* bl = CCMenuItemImage::create( imgLatchFileName, imgLatchFileName );
	button = AdornableMenuItemToggle::createWithTarget( this, callBackSelector, b, bl, NULL );
	CCMenu* viewMenu = CCMenu::create( button, NULL );
	viewMenu->setPosition( CCPointZero );
	addChild( viewMenu );
	
	adornment->setPosition( ccpCompMult(ccpFromSize(button->getContentSize()), button->getAnchorPoint()) );
	button->setAdornment( adornment );
	
	button->setScale( kControlSizeScale );
	return button;
}


/**
 * Positions the right-side location joystick at the right of the layer.
 * This is called at initialization, and anytime the content size of the layer changes
 * to keep the joystick in the correct location within the new layer dimensions.
 */
void CC3MashUpLayer::positionLocationJoystick()
{
	CCPoint jsMiddle = _locationJoystick->getAnchorPointInPoints();
	_locationJoystick->setPosition( ccp(getContentSize().width - (jsMiddle.x + kJoystickPadding),
									 (jsMiddle.y + kJoystickPadding)) );
}

/**
 * Positions the buttons between the two joysticks.
 * This is called at initialization, and anytime the content size of the layer changes
 * to keep the button in the correct location within the new layer dimensions.
 */
void CC3MashUpLayer::positionButtons()
{
	GLfloat middle = getContentSize().width / 2.0;
	GLfloat btnY = (kJoystickPadding * 0.5) + (kButtonGrid * 0.5);

	_shadowButton->setPosition( ccp(middle - (kButtonGrid * 0.5), btnY) );
	_zoomButton->setPosition( ccp(middle + (kButtonGrid * 0.5), btnY) );

	btnY += kButtonGrid;
	_switchViewButton->setPosition( ccp(middle - (kButtonGrid * 1.0), btnY) );
	_invasionButton->setPosition( ccp(middle, btnY) );
	_sunlightButton->setPosition( ccp(middle + (kButtonGrid * 1.0), btnY) );
}

/**
 * Updates the player (camera) direction and location from the joystick controls
 * and then updates the 3D scene.
 */
void CC3MashUpLayer::update( GLfloat dt )
{
	// Update the player direction and position in the scene from the joystick velocities
	getMashUpScene()->setPlayerDirectionControl( _directionJoystick->getVelocity() );
	getMashUpScene()->setPlayerLocationControl( _locationJoystick->getVelocity() );

	super::update( dt );
}

/** The user has pressed the switch camera view button. Tell the 3D scene so it can move the camera. */
void CC3MashUpLayer::switchViewSelected( CCObject* pSender )
{
	getMashUpScene()->switchCameraTarget(); 
}

/** The user has pressed the invade button. Tell the 3D scene. */
void CC3MashUpLayer::invade( CCObject* pSender )
{
	getMashUpScene()->invade(); 
}

/** The user has pressed the cycle lights button. Tell the 3D scene. */
void CC3MashUpLayer::cycleLights( CCObject* pSender )
{
	getMashUpScene()->cycleLights(); 
}

/** The user has pressed the zoom button. Tell the 3D scene. */
void CC3MashUpLayer::cycleZoom( CCObject* pSender )
{
	getMashUpScene()->cycleZoom(); 
}

/** The user has pressed the shadow button. Tell the 3D scene. */
void CC3MashUpLayer::toggleShadows( CCObject* pSender )
{
	bool isManagingShadows = getMashUpScene()->isManagingShadows();
	getMashUpScene()->setIsManagingShadows( !isManagingShadows );
}

/**
 * Called automatically when the contentSize has changed.
 * Move the location joystick to keep it in the bottom right corner of this layer
 * and the switch view button to keep it centered between the two joysticks.
 */
void CC3MashUpLayer::onContentSizeChanged()
{
	super::onContentSizeChanged();
//	positionLocationJoystick();
//	positionButtons();
}

/**
 * Opens a small, semi-transparent child HUD (Heads-Up-Display) window on top of the
 * main scene. This HUD window contains a close-up of the rotating globe. This window
 * is a separate CC3Layer containing a separate CC3Scene that contains a copy of the
 * globe node.
 *
 * The HUD window starts minimized at the point on the globe that was touched, and
 * smoothly expands to the top-right corner of the main scene.
 */
void CC3MashUpLayer::openGlobeHUDFromTouchAt( const CCPoint& touchPoint )
{
	if (_hudLayer) 
		return;
	
	// Determine an appropriate size for the HUD child window.
	CCSize mySize = getContentSize();
	GLfloat hudSide = MIN(mySize.width, mySize.height) * 0.5 - (kHUDPadding * 2);
	
	// Create the HUD CC3Layer, with a semi-transparent background, set its position
	// to the touch-point (offset by the size of the layer), and set its final size.
	// Start it with a small scale.
	_hudLayer = CHUDLayer::layer();
	_hudLayer->setPosition( touchPoint );
	_hudLayer->setContentSize( CCSizeMake(hudSide, hudSide) );
	_hudLayer->setScale( 0.1f );

	// Create and add a new CC3Scene, containing just a copy of the rotating globe,
	// for the HUD layer, and ensure its camera frames the globe.
	_hudLayer->setCC3Scene( makeHUDScene() );

	// Run actions to move and scale the HUD layer from its starting position
	// and size to its final expanded position and size.
	CCPoint hudMiddle = _hudLayer->getAnchorPointInPoints();
	CCPoint hudPos = ccp(mySize.width - (hudMiddle.x + kHUDPadding),
						 mySize.height - (hudMiddle.y + kHUDPadding));
	_hudLayer->runAction( CCActionMoveTo::create( 1.0f, hudPos ) );
	_hudLayer->runAction( CCActionScaleTo::create( 1.0f, 1.0f ) );
	addChild( _hudLayer );
}

/**
 * Returns a new CC3Scene containing a copy of the globe from the main scene.
 * Sets the globe rotating and makes it semi-transparent.
 */
CC3Scene* CC3MashUpLayer::makeHUDScene()
{
	CC3Scene* hudScene = CHUDScene::scene();
	
	CC3Node* globe = (CC3Node*)getCC3Scene()->getNodeNamed( kGlobeName )->copy();
	globe->setLocation( CC3Vector::kCC3VectorZero );
	globe->setRotation( CC3Vector::kCC3VectorZero );
	globe->runAction( CC3ActionRotateForever::actionWithRotationRate( cc3v(0, 30, 0) ) );
	hudScene->addChild( globe );
	hudScene->createGLBuffers();		// Won't really do anything because the Globe mesh...
									// ...has already been buffered in main scene
	hudScene->setOpacity( kCCOpacityFull * 0.75f );		// Makes everything in the scene somewhat translucent
	
	return hudScene;
}

/** Closes the HUD window by using CCActions to fade it out and then remove it. */
void CC3MashUpLayer::closeGlobeHUDFromTouchAt( const CCPoint& touchPoint )
{
	_hudLayer->stopAllActions();
	CCActionInterval* fadeHUD = CCActionFadeTo::create( 1.0f, 0 );
	CCActionInstant* removeHUD = CCActionCallFunc::create( this, SEL_CallFunc(&CC3MashUpLayer::removeGlobeHUD) );
														   
	_hudLayer->runAction( CCActionSequence::createWithTwoActions( fadeHUD, removeHUD ) );
}

/** Removes the HUD window if it exists. */
void CC3MashUpLayer::removeGlobeHUD()
{
	if (_hudLayer) 
		removeChild( _hudLayer, true );
	_hudLayer = NULL;
}

/** Toggles between opening and closing the HUD window. */
void CC3MashUpLayer::toggleGlobeHUDFromTouchAt( const CCPoint& touchPoint )
{
	if (_hudLayer)
		closeGlobeHUDFromTouchAt( touchPoint );
	else
		openGlobeHUDFromTouchAt( touchPoint );
}


#if 0

/**
 * Handle touch move event under Cocos2D v2 and below.
 *
 * The ccTouchMoved:withEvent: method is optional for the <CCTouchDelegateProtocol>.
 * The event dispatcher will not dispatch events for which there is no method
 * implementation. Since the touch-move events are both voluminous and seldom used,
 * the implementation of ccTouchMoved:withEvent: has been left out of the default
 * CC3Layer implementation. To receive and handle touch-move events for object
 * picking, it must be implemented here.
 *
 * This method will not be invoked if gestures have been enabled.
 */
void ccTouchMoved: (UITouch *)touch withEvent: (UIEvent *)event {
	[self handleTouch: touch ofType: kCCTouchMoved];
}

/**
 * Handle touch move event under Cocos2D v3 and above.
 *
 * The touchMoved:withEvent: method is optional. Since the touch-move events are both
 * voluminous and seldom used, the implementation of this method has been left out of
 * the default CC3Layer implementation. To receive and handle touch-move events for
 * object picking, it must be implemented here.
 *
 * This method will not be invoked if gestures have been enabled.
 */
void touchMoved: (UITouch*) touch withEvent: (UIEvent*) event {
	[self handleTouch: touch ofType: kCCTouchMoved];
}

#endif

#if CC3_IOS
#pragma mark Gesture support

/**
 * Invoked when this layer is being opened on the view.
 *
 * If we want to use gestures, we add the gesture recognizers here.
 *
 * By using the cc3AddGestureRecognizer: method to add the gesture recognizers,
 * we ensure that they will be torn down when this layer is removed from the view.
 *
 * This layer has child buttons on it. To ensure that those buttons receive their
 * touch events, we set cancelsTouchesInView to NO on the tap gestures recognizer
 * so that that gesture recognizer allows the touch events to propagate to the buttons.
 * We do not need to do that for the other recognizers because we don't want buttons
 * to receive touch events in the middle of a pan or pinch.
 */
void onOpenCC3Layer {
	if (self.isUserInteractionEnabled) return;
	
	// Register for tap gestures to select 3D nodes.
	// This layer has child buttons on it. To ensure that those buttons receive their
	// touch events, we set cancelsTouchesInView to NO so that the gesture recognizer
	// allows the touch events to propagate to the buttons.
	UITapGestureRecognizer* tapSelector = [[UITapGestureRecognizer alloc]
										   initWithTarget: self action: @selector(handleTapSelection:)];
	tapSelector.numberOfTapsRequired = 1;
	tapSelector.cancelsTouchesInView = NO;		// Ensures touches are passed to buttons
	[self cc3AddGestureRecognizer: tapSelector];
	
	// Register for single-finger dragging gestures used to spin the two cubes.
	UIPanGestureRecognizer* dragPanner = [[UIPanGestureRecognizer alloc]
										  initWithTarget: self action: @selector(handleDrag:)];
	dragPanner.minimumNumberOfTouches = 1;
	dragPanner.maximumNumberOfTouches = 1;
	[self cc3AddGestureRecognizer: dragPanner];

	// Register for double-finger dragging to pan the camera.
	UIPanGestureRecognizer* cameraPanner = [[UIPanGestureRecognizer alloc]
											initWithTarget: self action: @selector(handleCameraPan:)];
	cameraPanner.minimumNumberOfTouches = 2;
	cameraPanner.maximumNumberOfTouches = 2;
	[self cc3AddGestureRecognizer: cameraPanner];
	
	// Register for double-finger dragging to pan the camera.
	UIPinchGestureRecognizer* cameraMover = [[UIPinchGestureRecognizer alloc]
											 initWithTarget: self action: @selector(handleCameraMove:)];
	[self cc3AddGestureRecognizer: cameraMover];
}

/**
 * This handler is invoked when a single-tap gesture is recognized.
 *
 * If the tap occurs within a descendant CCNode that wants to capture the touch,
 * such as a menu or button, the gesture is cancelled. Otherwise, the tap is 
 * forwarded to the CC3Scene to pick the 3D node under the tap.
 */
void handleTapSelection: (UITapGestureRecognizer*) gesture {

	// Once the gesture has ended, convert the UI location to a 2D node location and
	// pick the 3D node under that location. Don't forget to test that the gesture is
	// valid and does not conflict with touches handled by this layer or its descendants.
	if ( [self cc3ValidateGesture: gesture] && (gesture.state == UIGestureRecognizerStateEnded) ) {
		CGPoint touchPoint = [self cc3ConvertUIPointToNodeSpace: gesture.location];
		[self.mashUpScene pickNodeFromTapAt: touchPoint];
	}
}

/**
 * This handler is invoked when a single-finger drag gesture is recognized.
 *
 * If the drag starts within a descendant CCNode that wants to capture the touch,
 * such as a menu or button, the gesture is cancelled.
 *
 * The CC3Scene marks where dragging begins to determine the node that is underneath
 * the touch point at that time, and is further notified as dragging proceeds.
 * It uses the velocity of the drag to spin the cube nodes. Finally, the scene is
 * notified when the dragging gesture finishes.
 *
 * The dragging movement is normalized to be specified relative to the size of the
 * layer, making it independant of the size of the layer.
 */
void handleDrag: (UIPanGestureRecognizer*) gesture {
	switch (gesture.state) {
		case UIGestureRecognizerStateBegan:
			if ( [self cc3ValidateGesture: gesture] ) {
				[self.mashUpScene startDraggingAt: [self cc3ConvertUIPointToNodeSpace: gesture.location]];
			}
			break;
		case UIGestureRecognizerStateChanged:
			[self.mashUpScene dragBy: [self cc3NormalizeUIMovement: gesture.translation]
						  atVelocity:[self cc3NormalizeUIMovement: gesture.velocity]];
			break;
		case UIGestureRecognizerStateEnded:
			[self.mashUpScene stopDragging];
			break;
		default:
			break;
	}
}

/**
 * This handler is invoked when a double-finger pan gesture is recognized.
 *
 * If the panning starts within a descendant CCNode that wants to capture the touch,
 * such as a menu or button, the gesture is cancelled.
 *
 * The CC3Scene marks the camera orientation when dragging begins, and is notified
 * as dragging proceeds. It uses the relative translation of the panning movement
 * to determine the new orientation of the camera. Finally, the scene is notified
 * when the dragging gesture finishes.
 *
 * The dragging movement is normalized to be specified relative to the size of the
 * layer, making it independant of the size of the layer.
 */
void handleCameraPan: (UIPanGestureRecognizer*) gesture {
	switch (gesture.state) {
		case UIGestureRecognizerStateBegan:
			if ( [self cc3ValidateGesture: gesture] ) [self.mashUpScene startPanningCamera];
			break;
		case UIGestureRecognizerStateChanged:
			[self.mashUpScene panCameraBy: [self cc3NormalizeUIMovement: gesture.translation]];
			break;
		case UIGestureRecognizerStateEnded:
			[self.mashUpScene stopPanningCamera];
			break;
		default:
			break;
	}
}


/**
 * This handler is invoked when a pinch gesture is recognized.
 *
 * If the pinch starts within a descendant CCNode that wants to capture the touch,
 * such as a menu or button, the gesture is cancelled.
 *
 * The CC3Scene marks the camera location when pinching begins, and is notified
 * as pinching proceeds. It uses the relative scale of the pinch gesture to determine
 * a new location for the camera. Finally, the scene is notified when the pinching
 * gesture finishes.
 *
 * Note that the pinching does not zoom the camera, although the visual effect is
 * very similar. For this application, moving the camera is more flexible and useful
 * than zooming. But other application might prefer to use the pinch gesture scale
 * to modify the uniformScale or fieldOfView properties of the camera, to perform
 * a true zooming effect.
 */
void handleCameraMove: (UIPinchGestureRecognizer*) gesture {
	switch (gesture.state) {
		case UIGestureRecognizerStateBegan:
			if ( [self cc3ValidateGesture: gesture] ) [self.mashUpScene startMovingCamera];
			break;
		case UIGestureRecognizerStateChanged:
			[self.mashUpScene moveCameraBy: gesture.scale];
			break;
		case UIGestureRecognizerStateEnded:
			[self.mashUpScene stopMovingCamera];
			break;
		default:
			break;
	}
}
#endif	// CC3_IOS

