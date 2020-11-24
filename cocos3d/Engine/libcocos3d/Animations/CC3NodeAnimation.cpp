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

CC3NodeAnimation::CC3NodeAnimation()
{

}

CC3NodeAnimation::~CC3NodeAnimation()
{
	
}

GLuint CC3NodeAnimation::getFrameCount()
{
	return m_frameCount;
}

bool CC3NodeAnimation::shouldInterpolate()
{
	return m_shouldInterpolate;
}

void CC3NodeAnimation::setShouldInterpolate( bool inter )
{
	m_shouldInterpolate = inter;
}

bool CC3NodeAnimation::isAnimatingLocation()
{
	return false; 
}

bool CC3NodeAnimation::isAnimatingQuaternion()
{
	return false; 
}

bool CC3NodeAnimation::isAnimatingScale()
{
	return false; 
}

bool CC3NodeAnimation::isAnimating()
{
	return (isAnimatingLocation() ||
							  isAnimatingQuaternion() ||
							  isAnimatingScale());
}

bool CC3NodeAnimation::hasVariableFrameTiming()
{
	return false; 
}

static float _interpolationEpsilon = 0.1f;

float CC3NodeAnimation::getInterpolationEpsilon()
{
	return _interpolationEpsilon; 
}

void CC3NodeAnimation::setInterpolationEpsilon( float epsilon )
{
	_interpolationEpsilon = epsilon; 
}

void CC3NodeAnimation::initWithFrameCount( GLuint numFrames )
{
	m_frameCount = numFrames;
	m_shouldInterpolate = true;
}

CC3NodeAnimation* CC3NodeAnimation::animationWithFrameCount( GLuint numFrames )
{
	CC3NodeAnimation* animation = new CC3NodeAnimation;
	animation->initWithFrameCount( numFrames );
	animation->autorelease();

	return animation;
}

void CC3NodeAnimation::establishFrameAt( float t, CC3NodeAnimationState* animState )
{
	//CCLOG_TRACE("CC3NodeAnimation animating frame at %.4f", t);
	CCAssert(t >= 0.0 && t <= 1.0, "CC3NodeAnimation animation frame time %f must be between 0.0 and 1.0"/*, t*/);
	
	// Get the index of the frame within which the given time appears,
	// and declare a possible fractional interpolation within that frame.
	GLuint frameIndex = getFrameIndexAt( t );
	GLfloat frameInterpolation = 0.0;
	
	// If we should interpolate, and we're not at the last frame, calc the interpolation amount.
	// We only bother interpolating if difference is large enough. If close enough to this frame
	// or the next frame, just use the appropriate frame outright.
	if ( m_shouldInterpolate && (frameIndex < m_frameCount - 1) ) 
	{
		float frameTime = timeAtFrame( frameIndex );
		float nextFrameTime = timeAtFrame( frameIndex + 1 );
		float frameDur = nextFrameTime - frameTime;
		if ( frameDur != 0.0f ) 
			frameInterpolation = (t - frameTime) / frameDur;

		if ( frameInterpolation < _interpolationEpsilon ) 
		{
			frameInterpolation = 0.0f;		// use this frame
		}
		else if ((1.0f - frameInterpolation) < _interpolationEpsilon) 
		{
			frameInterpolation = 0.0f;
			frameIndex++;					// use next frame
		}
		//CCLOG_TRACE("CC3NodeAnimation animating at time %.3f between frame %u at %.3f and next frame at %.3f with interpolation fraction %.3f",
		//		 t, frameIndex, frameTime, nextFrameTime, frameInterpolation);
	}

	establishFrame( frameIndex, frameInterpolation, animState );
}

/**
 * Updates the location, quaternion, and scale of the specified node animation state based on the
 * animation frame located at the specified frame, plus an interpolation amount towards the next frame.
 */
void CC3NodeAnimation::establishFrame( GLuint frameIndex, GLfloat frameInterpolation, CC3NodeAnimationState* animState )
{
	establishLocationAtFrame( frameIndex, frameInterpolation, animState );
	establishQuaternionAtFrame( frameIndex, frameInterpolation, animState );
	establishScaleAtFrame( frameIndex, frameInterpolation, animState );
}

/**
 * Updates the location of the node animation state by interpolating between the animation
 * content at the specified frame index and that at the next frame index, using the specified
 * interpolation fraction value, which will be between zero and one.
 * If frameInterpolation is zero, the lerp function will immediately return first frame.
 */
void CC3NodeAnimation::establishLocationAtFrame( GLuint frameIndex, GLfloat frameInterpolation, CC3NodeAnimationState* animState )
{
	if( !animState->isAnimatingLocation() ) 
		return;
	
	animState->setLocation( getLocationAtFrame(frameIndex).lerp( getLocationAtFrame(frameIndex + 1), frameInterpolation) );
}

/**
 * Updates the rotation quaternion of the node animation state by interpolating between the
 * animation content at the specified frame index and that at the next frame index, using
 * the specified interpolation fraction value, which will be between zero and one.
 * If frameInterpolation is zero, the slerp function will immediately return first frame.
 */
void CC3NodeAnimation::establishQuaternionAtFrame( GLuint frameIndex, GLfloat frameInterpolation, CC3NodeAnimationState* animState )
{
	if( !animState->isAnimatingQuaternion() ) 
		return;

	animState->setQuaternion( getQuaternionAtFrame(frameIndex).slerp(getQuaternionAtFrame(frameIndex + 1), frameInterpolation) );
}

/**
 * Updates the scale of the node animation state by interpolating between the the animation
 * content at the specified frame index and that at the next frame index, using the specified
 * interpolation fraction value, which will be between zero and one.
 * If frameInterpolation is zero, the lerp function will immediately return first frame.
 */
void CC3NodeAnimation::establishScaleAtFrame( GLuint frameIndex, GLfloat frameInterpolation, CC3NodeAnimationState* animState )
{
	if( !animState->isAnimatingScale() ) 
		return;

	animState->setScale( getScaleAtFrame(frameIndex).lerp( getScaleAtFrame(frameIndex + 1), frameInterpolation ) );
}

float CC3NodeAnimation::timeAtFrame( GLuint frameIndex )
{
	GLfloat thisIdx = (GLfloat)frameIndex;					// floatify
	GLfloat lastIdx = (GLfloat)(MAX(m_frameCount - 1, 1));		// floatify & ensure not zero
	return CLAMP(thisIdx / lastIdx, 0.0f, 1.0f);
}

/**
 * Template method that returns the index of the frame within which the specified time occurs.
 * The specified time will lie between the time of the animation frame at the returned index
 * and the time of the animation frame following that frame.
 */
GLuint CC3NodeAnimation::getFrameIndexAt( float t )
{
	return (GLuint)((m_frameCount - 1) * t); 
}

/**
 * Template method that returns the location at the specified animation frame.
 * Frame index numbering starts at zero.
 *
 * Default returns zero location. Subclasses with animation data should override.
 * Subclasses should ensure that if frameIndex is larger than (frameCount - 1),
 * the last frame will be returned.
 */
CC3Vector CC3NodeAnimation::getLocationAtFrame( GLuint frameIndex )
{
	return CC3Vector::kCC3VectorZero; 
}

/**
 * Template method that returns the location at the specified animation frame.
 * Frame index numbering starts at zero.
 *
 * Default returns the identity quaternion. Subclasses with animation data should override.
 * Subclasses should ensure that if frameIndex is larger than (frameCount - 1), the last
 * frame will be returned.
 */
CC3Quaternion CC3NodeAnimation::getQuaternionAtFrame( GLuint frameIndex )
{
	return CC3Quaternion::kCC3QuaternionIdentity; 
}

/**
 * Template method that returns the scale at the specified animation frame.
 * Frame index numbering starts at zero.
 *
 * Default returns unit scale. Subclasses with animation data should override.
 * Subclasses should ensure that if frameIndex is larger than (frameCount - 1),
 * the last frame will be returned.
 */
CC3Vector CC3NodeAnimation::getScaleAtFrame( GLuint frameIndex )
{
	return CC3Vector::kCC3VectorUnitCube; 
}

NS_COCOS3D_END
