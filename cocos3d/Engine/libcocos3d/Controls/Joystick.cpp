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
#include "Joystick.h"
#include "..\Utility\CC3CC2Extensions.h"
#include "..\Utility\CC3Logging.h"

NS_COCOS3D_BEGIN

/** The time it takes the thumb to spring back to center once the user lets go. */
#define kThumbSpringBackDuration 1.0

#define kJoystickEventPriority	0

static inline CCSize ccNodeScaledSize( CCNode* aNode )
{
	if ( aNode )
	{
		CCSize contentSize = aNode->getContentSize();
		float scaleX = aNode->getScaleX();
		float scaleY = aNode->getScaleY();

		return CCSizeMake( contentSize.width * scaleX, contentSize.height * scaleY );
	}

	return CCSizeZero;
}

Joystick::Joystick()
{
	_thumbNode = NULL;
	_isTracking = false;
}

CCPoint Joystick::getVelocity()
{
	return _velocity;
}

AngularPoint Joystick::getAngularVelocity()
{
	return _angularVelocity;
}

void Joystick::initWithThumb( CCNode* aNode, const CCSize& size )
{
	CCAssert(aNode, "Thumb node must not be nil");
	if ( super::init() ) 
	{
		initializeEvents();
		_isTracking = false;
		_velocity = CCPointZero;
		_angularVelocity = AngularPointZero;
		ignoreAnchorPointForPosition( false );
		setAnchorPoint( ccp(0.5f, 0.5f) );

		// Add thumb node as a child and position it at the center
		// Must do following in this order: add thumb / set size / get anchor point
		_thumbNode = aNode;
		_thumbNode->setAnchorPoint( ccp(0.5f, 0.5f) );
		addChild( _thumbNode, 1 );
		setContentSize( size );	
		_thumbNode->setPosition( getAnchorPointInPoints() );
	}
}

void Joystick::initializeEvents()
{
	setTouchEnabled( true );
	setTouchPriority( kJoystickEventPriority );
}

Joystick* Joystick::joystickWithThumb( CCNode* aNode, const CCSize& size )
{
	Joystick* pVal = new Joystick;
	pVal->initWithThumb( aNode, size );
	pVal->autorelease();

	return pVal;
}

void Joystick::initWithThumb( CCNode* aNode, CCNode* bgNode )
{
	initWithThumb( aNode, ccNodeScaledSize(bgNode) );
	
	if (bgNode)
	{
		// Position the background node at the center and behind the thumb node 
		bgNode->setAnchorPoint( ccp(0.5f, 0.5f) );
		bgNode->setPosition( getAnchorPointInPoints() );
		addChild( bgNode );
	}
}

Joystick* Joystick::joystickWithThumb( CCNode* thumbNode, CCNode* backgroundNode )
{
	Joystick* pVal = new Joystick;
	pVal->initWithThumb( thumbNode, backgroundNode );
	pVal->autorelease();

	return pVal;
}

void Joystick::setContentSize( const CCSize& newSize )
{
	super::setContentSize( newSize );
	_travelLimit = ccpMult(ccpSub(ccpFromSize(getContentSize()),
								  ccpFromSize(ccNodeScaledSize(_thumbNode))), 0.5);
}

void Joystick::onEnter()
{
	super::onEnter();
	resetVelocity();
}

void Joystick::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate( this,
														  kJoystickEventPriority,
														  true );
}

int Joystick::getTouchPriority()
{
	return kJoystickEventPriority;
}

bool Joystick::ccTouchBegan( CCTouch* touch, CCEvent* event )
{
	return processTouchDownAt( convertTouchToNodeSpace( touch ) );
}

void Joystick::ccTouchEnded( CCTouch* touch, CCEvent* event )
{
	CCAssert(_isTracking, "Touch ended that was never begun");
	resetVelocity();
}

void Joystick::ccTouchCancelled( CCTouch* touch, CCEvent* event )
{
	CCAssert(_isTracking, "Touch cancelled that was never begun");
	resetVelocity();
}

void Joystick::ccTouchMoved( CCTouch* touch, CCEvent* event )
{
	CCAssert(_isTracking, "Touch moved that was never begun");
	trackVelocity( convertTouchToNodeSpace( touch ) );
}

bool Joystick::processTouchDownAt( const CCPoint& localPoint )
{
	if(_isTracking) 
		return false;
	
	CCSize cs = getContentSize();
	CCRect nodeBounds = CCRectMake(0, 0, cs.width, cs.height);
	if ( nodeBounds.containsPoint( localPoint ) )
	{
		_isTracking = true;
		_thumbNode->stopAllActions();
		trackVelocity( localPoint );
		return true;
	}
	return false;
}

void Joystick::trackVelocity( const CCPoint& nodeTouchPoint )
{
	CCPoint ankPt = getAnchorPointInPoints();

	// Get the touch point relative to the joystick home (anchor point)
	CCPoint relPoint = ccpSub(nodeTouchPoint, ankPt);
	
	// Determine the raw unconstrained velocity vector
	CCPoint rawVelocity = CCPointMake(relPoint.x / _travelLimit.x,
									  relPoint.y / _travelLimit.y);

	// If necessary, normalize the velocity vector relative to the travel limits
	float rawVelLen = ccpLength(rawVelocity);
	_velocity = (rawVelLen <= 1.0) ? rawVelocity : ccpMult(rawVelocity, 1.0f/rawVelLen);

	// Calculate the vector in angular coordinates
	// ccpToAngle returns counterclockwise positive relative to X-axis.
	// We want clockwise positive relative to the Y-axis.
	float angle = 90.0f - CC_RADIANS_TO_DEGREES(ccpToAngle(_velocity));
	if ( angle > 180.0f ) 
		angle -= 360.0f;

	_angularVelocity.radius = ccpLength(_velocity);
	_angularVelocity.heading = angle;
	
	// Update the thumb's position, clamping it within the contentSize of the Joystick
	if ( _thumbNode )
		_thumbNode->setPosition( ccpAdd(ccpCompMult(_velocity, _travelLimit), ankPt) );
}

void Joystick::resetVelocity()
{
	_isTracking = false;
	_velocity = CCPointZero;
	_angularVelocity = AngularPointZero;

	if ( _thumbNode )
		_thumbNode->runAction( CCActionEaseElasticOut::create( CCActionMoveTo::create( kThumbSpringBackDuration, getAnchorPointInPoints() ) ) );
}

NS_COCOS3D_END
