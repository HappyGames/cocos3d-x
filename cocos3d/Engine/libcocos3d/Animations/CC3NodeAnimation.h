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

#ifndef _CCL_CC3NODEANIMATION_H_
#define _CCL_CC3NODEANIMATION_H_

NS_COCOS3D_BEGIN

/** 
 * A CC3NodeAnimation manages the animation of a node.
 *
 * An instance is held in the animation property of the node itself, and the node delegates
 * to its CC3NodeAnimation when the establishAnimationFrameAt: method is invoked on the node.
 *
 * Animations define animated content in in a series of frames (often called key-frames),
 * and can be configured to interpolate the animated state between these frames if necessary,
 * ensuring smooth animation, regardless of how many, or how widely spaced, the frames of
 * actual animated content are.
 *
 * A single CC3NodeAnimation instance can be shared by multiple nodes. This is a typical
 * situation when creating many copies of a node that is animated.
 *
 * CC3NodeAnimation is an abstract class. Subclasses define concrete animation data storage.
 */
class CC3NodeAnimation : public CCObject 
{
public:
	CC3NodeAnimation();
	virtual ~CC3NodeAnimation();
	/**
	 * The number of frames of animated content.
	 *
	 * This property indicates the number of frames for which animated content is available (often
	 * called key-frames). Because animations can be configured to interpolate between frames, it
	 * is quite common for the effective number of animated frames to be substantially higher than
	 * the number of frames of available animated content.
	 *
	 * As an extreme example, this property might indicate only two frames of animated content
	 * (a beginning and end state). If that animation played out over 10 seconds, it would interpolate
	 * several hundred "tween-frames", creating a smooth transition from the beginning to end state.
	 * More commonly, animations will specify a number of frames of content, to ensure sophisticated
	 * and realistic animation.
	 */
	virtual GLuint				getFrameCount();
	/**
	 * Indicates whether this animation should interpolate between frames, to ensure smooth
	 * transitions between frame content.
	 *
	 * The initial value of this property is YES.
	 */
	virtual bool				shouldInterpolate();
	virtual void				setShouldInterpolate( bool inter ); 

	
	/** Indicates whether location animated content is available and is enabled.  */
	virtual bool				isAnimatingLocation();

	/** Indicates whether rotation quaternion animated content is available and is enabled. */
	virtual bool				isAnimatingQuaternion();

	/** Indicates whether scale animated content is available and is enabled. */
	virtual bool				isAnimatingScale();

	/** 
	 * Indicates whether animation is enabled and any animated content (location, quaternion,
	 * or scale) is available and enabled.
	 */
	virtual bool				isAnimating();

	/**
	 * Indicates whether the time interval between frames can vary from frame to frame, or whether
	 * the time interval between frames is constant across all frames.
	 *
	 * If this property returns NO, the frames of this animation are equally spaced in time.
	 */
	virtual bool				hasVariableFrameTiming();
	
	/**
	 * Indicates a fractional value of a frame below which interpolation will not be performed.
	 *
	 * If an animation time is within this fraction above or below an exact frame time (relative to
	 * the previous or next frame), the frame time itself is used, instead of interpolating between
	 * that frame time and the next or previous frame time. This allows the animation to avoid an
	 * interpolation calculation if the difference will be unnoticable when compared to simply using
	 * the values for that specific frame.
	 *
	 * This value is specified as a fraction of a frame (between zero and one). The initial value is
	 * set at 0.1, indicating that no interpolation will be performed if the animation time is within
	 * 10% of the duration between the frame and the frame before or after it.
	 *
	 * Setting the value of this property to zero will cause interpolation to always be performed,
	 * and setting the value to one will cause interpolation to never be performed.
	 *
	 * The value of this class-side property affects all animation.
	 */
	static float				getInterpolationEpsilon();
	static void					setInterpolationEpsilon( float epsilon );

	/** Initializes this instance to animate with the specified number of animation frames. */
	virtual void				initWithFrameCount( GLuint numFrames );

	/**
	 * Allocates and initializes an autoreleased instance to animate
	 * with the specified number of animation frames.
	 */
	static CC3NodeAnimation*	animationWithFrameCount( GLuint numFrames );

	
	/**
	 * Updates the location, quaternion, and scale of the specified animation state based on the
	 * animation frame located at the specified time, which should be a value between zero and one,
	 * with zero indicating the first animation frame, and one indicating the last animation frame.
	 *
	 * Only those properties of the animation state for which there is animation data will be changed.
	 * If the shouldInterpolate property is set to YES, linear interpolation of the frame
	 * data is performed, based on the frameCount and the specified time.
	 */
	virtual void				establishFrameAt( float t, CC3NodeAnimationState* animState );

	/**
	 * Returns the time at which the frame at the specified index occurs. The returned time
	 * value will be between zero and one, where zero represents the time of the first frame
	 * and one represents the time of the last frame.
	 *
	 * This base implementation assumes a constant time between each frame and the next, so the
	 * returned value is calculated as (frameIndex / (frameCount - 1)), which is then clamped to
	 * the range between zero and one. Subclasses that allow variable times between frames will
	 * override to return the appropriate value.
	 */
	virtual float				timeAtFrame( GLuint frameIndex );

	/**
	 * Template method that returns the index of the frame within which the specified time occurs.
	 * The specified time will lie between the time of the animation frame at the returned index
	 * and the time of the animation frame following that frame.
	 */
	virtual GLuint				getFrameIndexAt( float t );

	/**
	 * Updates the location, quaternion, and scale of the specified node animation state based on the
	 * animation frame located at the specified frame, plus an interpolation amount towards the next frame.
	 */
	virtual void				establishFrame( GLuint frameIndex, GLfloat frameInterpolation, CC3NodeAnimationState* animState );

	/**
	 * Updates the location of the node animation state by interpolating between the animation
	 * content at the specified frame index and that at the next frame index, using the specified
	 * interpolation fraction value, which will be between zero and one.
	 * If frameInterpolation is zero, the lerp function will immediately return first frame.
	 */
	virtual void				establishLocationAtFrame( GLuint frameIndex, GLfloat frameInterpolation, CC3NodeAnimationState* animState );

	/**
	 * Updates the rotation quaternion of the node animation state by interpolating between the
	 * animation content at the specified frame index and that at the next frame index, using
	 * the specified interpolation fraction value, which will be between zero and one.
	 * If frameInterpolation is zero, the slerp function will immediately return first frame.
	 */
	virtual void				establishQuaternionAtFrame( GLuint frameIndex, GLfloat frameInterpolation, CC3NodeAnimationState* animState );

	/**
	 * Updates the scale of the node animation state by interpolating between the the animation
	 * content at the specified frame index and that at the next frame index, using the specified
	 * interpolation fraction value, which will be between zero and one.
	 * If frameInterpolation is zero, the lerp function will immediately return first frame.
	 */
	virtual void				establishScaleAtFrame( GLuint frameIndex, GLfloat frameInterpolation, CC3NodeAnimationState* animState );

	/**
	 * Template method that returns the location at the specified animation frame.
	 * Frame index numbering starts at zero.
	 *
	 * Default returns zero location. Subclasses with animation data should override.
	 * Subclasses should ensure that if frameIndex is larger than (frameCount - 1),
	 * the last frame will be returned.
	 */
	virtual CC3Vector			getLocationAtFrame( GLuint frameIndex );

	/**
	 * Template method that returns the location at the specified animation frame.
	 * Frame index numbering starts at zero.
	 *
	 * Default returns the identity quaternion. Subclasses with animation data should override.
	 * Subclasses should ensure that if frameIndex is larger than (frameCount - 1), the last
	 * frame will be returned.
	 */
	virtual CC3Quaternion		getQuaternionAtFrame( GLuint frameIndex );

	/**
	 * Template method that returns the scale at the specified animation frame.
	 * Frame index numbering starts at zero.
	 *
	 * Default returns unit scale. Subclasses with animation data should override.
	 * Subclasses should ensure that if frameIndex is larger than (frameCount - 1),
	 * the last frame will be returned.
	 */
	virtual CC3Vector			getScaleAtFrame( GLuint frameIndex );

protected:
	GLuint						m_frameCount;
	bool						m_shouldInterpolate : 1;
};

NS_COCOS3D_END

#endif
