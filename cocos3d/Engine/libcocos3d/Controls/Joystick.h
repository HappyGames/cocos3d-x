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
#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

NS_COCOS3D_BEGIN

/** Specifies a 2D position using an angular coordinate axes. */
typedef struct 
{
	float heading;		/**< The angular heading in degrees. */
	float radius;			/**< The radial distance. */
} AngularPoint;

/** A constant representing a zero angular point, with both angle and radius set to zero. */
static const AngularPoint AngularPointZero = {0.0, 0.0};

/**
 * A Joystick control constructed from two child CCNodes, one as a movable thumb CCNode that
 * visually simulates the "stick", and another as an optional background CCNode that provides
 * a backdrop over which the thumb moves.
 *
 * The user can control the Joystick by touching within the bounds of the Joystick and then
 * dragging the finger around. They thumb image will track the finger within the bounds of 
 * the Joystick. When the user raises the finger, the thumb will move in an animated fashion
 * back to its resting position, which is the anchorPoint of the Joystick.
 *
 * The current position of the thumb can be read using either X-Y coordinates via the velocity
 * property, or angular coordinates via the angularVelocity property.
 */
class Joystick : public CCLayer 
{
	DECLARE_SUPER( CCLayer );
public:
	Joystick();

	/**
	 * The current velocity of the Joystick in terms of cartesian X-Y coordinates, as measured
	 * by the current position of the thumb proportionally relative to its resting position
	 * (the anchorPoint of the Joystick) and the contentSize of the Joystick. The individual
	 * X and Y values may be either positive (up an right) or negative (down and left).
	 * The magnitude of the X-Y vector is clamped to unit length, even if the user drags the finger
	 * outside the bounds of the Joystick. The set of possible points returned by this property
	 * therefore covers the area of a circle of unit radius, centered on the origin of an X-Y plane.
	 */
	virtual CCPoint				getVelocity();

	/**
	 * The velocity of the joystick in terms of angular coordinates. The returned heading
	 * is measured in degrees from the vertical axis, with positive angles growing clockwise.
	 * The radius is clamped to a maximum of unit length. The set of possible points
	 * returned by this method therefore covers the area of a circle of unit radius.
	 */
	virtual AngularPoint		getAngularVelocity();

	/**
	 * Initializes this Joystick to have the specified contentSize
	 * and to use the specified CCNode as the thumb "stick".
	 */
	void						initWithThumb( CCNode* aNode, const CCSize& size );

	/**
	 * Allocates and initializes an autoreleased Joystick to have the specified
	 * contentSize and to use the specified CCNode as the thumb "stick".
	 */
	static Joystick*			joystickWithThumb( CCNode* aNode, const CCSize& size );

	/**
	 * Initialize this Joystick to use the specified CCNodes to draw the thumb (the "stick")
	 * and background behind the "stick". The contentSize of the Joystick will be
	 * set to that of the backgroundNode.
	 */
	virtual void				initWithThumb( CCNode* thumbNode, CCNode* backgroundNode );

	/**
	 * Allocates and initializes an autoreleased Joystick to use the specified nodes
	 * to draw the thumb (the "stick") and background behind the "stick". The contentSize
	 * of the Joystick will be set to that of the backgroundNode.
	 */
	static Joystick*			joystickWithThumb( CCNode* thumbNode, CCNode* backgroundNode );

	void						initializeEvents();

	/**
	 * Overridden to also set the limit of travel for the thumb node to
	 * keep it at all times within the bound of the Joystick contentSize.
	 */
	virtual void				setContentSize( const CCSize& newSize );

	/**
	 * Start with fresh state each time we register. Certain transitions, such as dynamically
	 * overlaying the device camera can cause abrupt breaks in targeted event state.
	 */
	virtual void				onEnter();

	/** Handle touch events one at a time. */
	virtual void				registerWithTouchDispatcher();

	virtual bool				ccTouchBegan( CCTouch* touch, CCEvent* event );
	virtual void				ccTouchEnded( CCTouch* touch, CCEvent* event );
	virtual void				ccTouchCancelled( CCTouch* touch, CCEvent* event );
	virtual void				ccTouchMoved( CCTouch* touch, CCEvent* event );

	virtual int					getTouchPriority();

protected:
	/** Process an iOS touch down or OSX mouse down event at the specified point. */
	bool						processTouchDownAt( const CCPoint& localPoint );

	/**
	 * Calculates and sets the velocity based on the specified touch point
	 * which is relative to the Joystick coordinate space. Updates the
	 * position of the thumb node to track the users movements, but constrained
	 * to the bounds of a circle inscribed within the Joystick contentSize.
	 */
	void						trackVelocity( const CCPoint& nodeTouchPoint );

	/**
	 * Immediately zeros the velocity vectors and then animates moving the thumb back
	 * to the center, using ElasticOut to give it a bounce as it centers.
	 */
	void						resetVelocity();
	
protected:
	CCNode*						_thumbNode;
	bool						_isTracking;
	CCPoint						_velocity;
	AngularPoint				_angularVelocity;
	CCPoint						_travelLimit;
};

NS_COCOS3D_END

#endif
