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

#include "CC3NodeAnimation.h"
#include "../cc3Helper/Logging.h"

NS_COCOS3D_BEGIN

CC3NodeAnimation::CC3NodeAnimation()
{

}

CC3NodeAnimation::~CC3NodeAnimation()
{
	
}

GLuint CC3NodeAnimation::getFrameCount()
{
	return _frameCount;
}

bool CC3NodeAnimation::shouldInterpolate()
{
	return _shouldInterpolate;
}

void CC3NodeAnimation::setShouldInterpolate( bool inter )
{
	_shouldInterpolate = inter;
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
	_frameCount = numFrames;
	_shouldInterpolate = true;
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
	if ( _shouldInterpolate && (frameIndex < _frameCount - 1) ) 
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
	
	animState->setLocation( CC3VectorLerp(getLocationAtFrame(frameIndex), getLocationAtFrame(frameIndex + 1), frameInterpolation) );
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

	animState->setQuaternion( CC3QuaternionSlerp(getQuaternionAtFrame(frameIndex), getQuaternionAtFrame(frameIndex + 1), frameInterpolation) );
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

	animState->setScale( CC3VectorLerp( getScaleAtFrame(frameIndex), getScaleAtFrame(frameIndex + 1), frameInterpolation) );
}

float CC3NodeAnimation::timeAtFrame( GLuint frameIndex )
{
	GLfloat thisIdx = (GLfloat)frameIndex;					// floatify
	GLfloat lastIdx = (GLfloat)(MAX(_frameCount - 1, 1));		// floatify & ensure not zero
	return CLAMP(thisIdx / lastIdx, 0.0f, 1.0f);
}

/**
 * Template method that returns the index of the frame within which the specified time occurs.
 * The specified time will lie between the time of the animation frame at the returned index
 * and the time of the animation frame following that frame.
 */
GLuint CC3NodeAnimation::getFrameIndexAt( float t )
{
	return (GLuint)((_frameCount - 1) * t); 
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
	return kCC3VectorZero; 
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
	return kCC3QuaternionIdentity; 
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
	return kCC3VectorUnitCube; 
}

CC3ArrayNodeAnimation::CC3ArrayNodeAnimation()
{

}

CC3ArrayNodeAnimation::~CC3ArrayNodeAnimation()
{
	deallocateFrameTimes();
	deallocateLocations();
	deallocateQuaternions();
	deallocateScales();
}

bool CC3ArrayNodeAnimation::isAnimatingLocation()
{
	return _animatedLocations != NULL; 
}

bool CC3ArrayNodeAnimation::isAnimatingQuaternion()
{
	return _animatedQuaternions != NULL; 
}

bool CC3ArrayNodeAnimation::isAnimatingScale() 
{
	return _animatedScales != NULL; 
}

bool CC3ArrayNodeAnimation::hasVariableFrameTiming()
{
	return _frameTimes != NULL; 
}

void CC3ArrayNodeAnimation::initWithFrameCount( GLuint numFrames )
{
	super::initWithFrameCount( numFrames );
	{
		_frameTimes = NULL;
		_animatedLocations = NULL;
		_animatedQuaternions = NULL;
		_animatedScales = NULL;
		_frameTimesAreRetained = false;
		_locationsAreRetained = false;
		_quaternionsAreRetained = false;
		_scalesAreRetained = false;
	}
}

// All times should be in range between zero and one
float CC3ArrayNodeAnimation::timeAtFrame( GLuint frameIndex )
{
	if (!_frameTimes) 
		return super::timeAtFrame(frameIndex);

	return _frameTimes[MIN(frameIndex, _frameCount - 1)];
}

// Iterate backwards through the frames looking for the first frame whose time is at or before
// the specified frame time, and return that frame. If the specified frame is before the first
// frame, return the first frame.
GLuint CC3ArrayNodeAnimation::getFrameIndexAt( float t )
{
	if (!_frameTimes) 
		return super::getFrameIndexAt(t);

	for (GLint fIdx = _frameCount - 1; fIdx >= 0; fIdx--)	// start at last frame
		if (_frameTimes[fIdx] <= t) 
			return fIdx;			// return frame

	return 0;
}

CC3Vector CC3ArrayNodeAnimation::getLocationAtFrame( GLuint frameIndex )
{
	if (!_animatedLocations) 
		return super::getLocationAtFrame( frameIndex );

	return _animatedLocations[MIN(frameIndex, _frameCount - 1)];
}

CC3Quaternion CC3ArrayNodeAnimation::getQuaternionAtFrame( GLuint frameIndex )
{
	if (!_animatedQuaternions) 
		return super::getQuaternionAtFrame( frameIndex );

	return _animatedQuaternions[MIN(frameIndex, _frameCount - 1)];
}

CC3Vector CC3ArrayNodeAnimation::getScaleAtFrame( GLuint frameIndex )
{
	if (!_animatedScales) 
		return super::getScaleAtFrame( frameIndex );

	return _animatedScales[MIN(frameIndex, _frameCount - 1)];
}

void CC3ArrayNodeAnimation::setFrameTimes( float* frameTimes )
{
	deallocateFrameTimes();			// get rid of any existing array
	_frameTimes = frameTimes;
}

void CC3ArrayNodeAnimation::setAnimatedLocations( CC3Vector* vectorArray )
{
	deallocateLocations();				// get rid of any existing array
	_animatedLocations = vectorArray;
}

void CC3ArrayNodeAnimation::setAnimatedQuaternions( CC3Quaternion* vectorArray )
{
	deallocateQuaternions();			// get rid of any existing array
	_animatedQuaternions = vectorArray;
}

void CC3ArrayNodeAnimation::setAnimatedScales( CC3Vector* vectorArray )
{
	deallocateScales();				// get rid of any existing array
	_animatedScales= vectorArray;
}

float* CC3ArrayNodeAnimation::allocateFrameTimes()
{
	if (_frameCount) 
	{
		setFrameTimes( (float*)calloc(_frameCount, sizeof(float)) );
		_frameTimesAreRetained = true;
		CC3_TRACE("CC3ArrayNodeAnimation allocated space for %d frame times", _frameCount);
	}
	return _frameTimes;
}

void CC3ArrayNodeAnimation::deallocateFrameTimes()
{
	if (_frameTimesAreRetained) 
	{
		free(_frameTimes);
		_frameTimes = NULL;
		_frameTimesAreRetained = false;
		CC3_TRACE("CC3ArrayNodeAnimation deallocated %d previously allocated frame times", _frameCount);
	}
}

CC3Vector* CC3ArrayNodeAnimation::allocateLocations()
{
	if (_frameCount) 
	{
		setAnimatedLocations( (CC3Vector*)calloc(_frameCount, sizeof(CC3Vector)) );
		_locationsAreRetained = true;
		CC3_TRACE("CC3ArrayNodeAnimation allocated space for %d locations", _frameCount);
	}
	return _animatedLocations;
}

void CC3ArrayNodeAnimation::deallocateLocations()
{
	if (_locationsAreRetained) 
	{
		free(_animatedLocations);
		_animatedLocations = NULL;
		_locationsAreRetained = false;
		CC3_TRACE("CC3ArrayNodeAnimation deallocated %d previously allocated animated locations", _frameCount);
	}
}

CC3Quaternion* CC3ArrayNodeAnimation::allocateQuaternions()
{
	if (_frameCount) 
	{
		setAnimatedQuaternions( (CC3Quaternion*)calloc(_frameCount, sizeof(CC3Quaternion)) );

		for (GLuint i = 0; i < _frameCount; i++) 
			_animatedQuaternions[i] = kCC3QuaternionIdentity;

		_quaternionsAreRetained = true;
		CC3_TRACE("CC3ArrayNodeAnimation allocated space for %d quaternions", _frameCount);
	}

	return _animatedQuaternions;
}

void CC3ArrayNodeAnimation::deallocateQuaternions()
{
	if (_quaternionsAreRetained) 
	{
		free(_animatedQuaternions);
		_animatedQuaternions = NULL;
		_quaternionsAreRetained = false;
		CC3_TRACE("CC3ArrayNodeAnimation deallocated %d previously allocated animated quaternions", _frameCount);
	}
}

CC3Vector* CC3ArrayNodeAnimation::allocateScales()
{
	if (_frameCount) 
	{
		setAnimatedScales( (CC3Vector*)calloc(_frameCount, sizeof(CC3Vector)) );
		for (GLuint i = 0; i < _frameCount; i++) 
			_animatedScales[i] = kCC3VectorUnitCube;
		
		_scalesAreRetained = true;
		CC3_TRACE("CC3ArrayNodeAnimation allocated space for %d scales", _frameCount);
	}
	return _animatedScales;
}

void CC3ArrayNodeAnimation::deallocateScales()
{
	if (_scalesAreRetained) 
	{
		free(_animatedScales);
		_animatedScales = NULL;
		_scalesAreRetained = false;
		CC3_TRACE("CC3ArrayNodeAnimation deallocated %d previously allocated animated scales", _frameCount);
	}
}

CC3Vector CC3FrozenNodeAnimation::getScale()
{
	return _scale;
}

CC3Vector4 CC3FrozenNodeAnimation::getQuaternion()
{
	return _quaternion;
}

CC3Vector CC3FrozenNodeAnimation::getLocation()
{
	return _location;
}

bool CC3FrozenNodeAnimation::isAnimatingLocation()
{
	return !CC3VectorIsNull(_location); 
}

bool CC3FrozenNodeAnimation::isAnimatingQuaternion()
{
	return !CC3QuaternionIsNull(_quaternion); 
}

bool CC3FrozenNodeAnimation::isAnimatingScale()
{
	return !CC3VectorIsNull(_scale); 
}

void CC3FrozenNodeAnimation::populateFromNodeState( CC3Node* node )
{
	_location = node->getLocation();
	_quaternion = node->getQuaternion();
	_scale = node->getScale();
}

void CC3FrozenNodeAnimation::establishFrameAt( float t, CC3NodeAnimationState* animState )
{
	CCAssert(t >= 0.0 && t <= 1.0, "CC3FrozenNodeAnimation animation frame time %f must be between 0.0 and 1.0"/*, t*/);

	if(animState->isAnimatingLocation())
		animState->setLocation( getLocation() );
	if(animState->isAnimatingQuaternion()) 
		animState->setQuaternion( getQuaternion() );
	if(animState->isAnimatingScale()) 
		animState->setScale( getScale() );
}

void CC3FrozenNodeAnimation::init()
{
	return initWithFrameCount(1); 
}

void CC3FrozenNodeAnimation::initWithFrameCount( GLuint numFrames )
{
	super::initWithFrameCount(1);
	{
		_shouldInterpolate = false;
		_location = kCC3VectorNull;
		_quaternion = kCC3QuaternionNull;
		_scale = kCC3VectorNull;
	}
}

CC3FrozenNodeAnimation* CC3FrozenNodeAnimation::animation()
{
	CC3FrozenNodeAnimation* pAnimation = new CC3FrozenNodeAnimation;
	pAnimation->init();
	pAnimation->autorelease();

	return pAnimation;
}

void CC3FrozenNodeAnimation::initFromNodeState( CC3Node* aNode )
{
	init();
	populateFromNodeState( aNode );
}

CC3FrozenNodeAnimation* CC3FrozenNodeAnimation::animationFromNodeState( CC3Node* aNode )
{
	CC3FrozenNodeAnimation* pAnimation = new CC3FrozenNodeAnimation;
	pAnimation->initFromNodeState( aNode );
	pAnimation->autorelease();

	return pAnimation;
}

CC3NodeAnimationSegment::CC3NodeAnimationSegment()
{
	_baseAnimation = NULL;
}

CC3NodeAnimationSegment::~CC3NodeAnimationSegment()
{
	CC_SAFE_RELEASE( _baseAnimation );
}

CC3NodeAnimation* CC3NodeAnimationSegment::getBaseAnimation()
{
	return _baseAnimation;
}

GLfloat CC3NodeAnimationSegment::getStartTime()
{
	return _startTime;
}

void CC3NodeAnimationSegment::setStartTime( float time )
{
	_startTime = time;
}

GLfloat CC3NodeAnimationSegment::getEndTime()
{
	return _endTime;
}

void CC3NodeAnimationSegment::setEndTime( float time )
{
	_endTime = time;
}

GLuint CC3NodeAnimationSegment::getFrameCount()
{
	return _baseAnimation->getFrameCount(); 
}

bool CC3NodeAnimationSegment::shouldInterpolate()
{
	return _baseAnimation->shouldInterpolate(); 
}

void CC3NodeAnimationSegment::setShouldInterpolate( bool _shouldInterpolate )
{
	_baseAnimation->setShouldInterpolate( _shouldInterpolate );
}

bool CC3NodeAnimationSegment::isAnimatingLocation()
{
	return _baseAnimation->isAnimatingLocation(); 
}

bool CC3NodeAnimationSegment::isAnimatingQuaternion()
{
	return _baseAnimation->isAnimatingQuaternion(); 
}

bool CC3NodeAnimationSegment::isAnimatingScale()
{
	return _baseAnimation->isAnimatingScale(); 
}

bool CC3NodeAnimationSegment::hasVariableFrameTiming()
{
	return _baseAnimation->hasVariableFrameTiming(); 
}

GLuint CC3NodeAnimationSegment::getStartFrameIndex()
{
	return _baseAnimation->getFrameIndexAt( getStartTime() ); 
}

void CC3NodeAnimationSegment::setStartFrameIndex( GLuint startFrameIndex )
{
	setStartTime( _baseAnimation->timeAtFrame(startFrameIndex) );
}

GLuint CC3NodeAnimationSegment::getEndFrameIndex()
{
	return _baseAnimation->getFrameIndexAt( getEndTime() ); 
}

void CC3NodeAnimationSegment::setEndFrameIndex( GLuint endFrameIndex )
{
	setEndTime( _baseAnimation->timeAtFrame( endFrameIndex ) );
}

// Will raise assertion because base animation cannot be nil.
void CC3NodeAnimationSegment::init()
{
	return initOnAnimation( NULL );
}

void CC3NodeAnimationSegment::initOnAnimation( CC3NodeAnimation* baseAnimation )
{
	return initOnAnimation( baseAnimation, 0.0f , 1.0f );
}

CC3NodeAnimationSegment* CC3NodeAnimationSegment::animationOnAnimation( CC3NodeAnimation* baseAnimation )
{
	CC3NodeAnimationSegment* pSeg = new CC3NodeAnimationSegment;
	pSeg->initOnAnimation( baseAnimation );
	pSeg->autorelease();

	return pSeg;
}

void CC3NodeAnimationSegment::initOnAnimation( CC3NodeAnimation* baseAnimation, float startTime, float endTime )
{
	CCAssert(baseAnimation, "CC3NodeAnimationSegment cannot be initialized without a base animation");
	super::initWithFrameCount(0);
	{
		_baseAnimation = baseAnimation;
		_baseAnimation->retain();
		_startTime = startTime;
		_endTime = endTime;
	}
}

CC3NodeAnimationSegment* CC3NodeAnimationSegment::animationOnAnimation( CC3NodeAnimation* baseAnimation, float startTime, float endTime )
{
	CC3NodeAnimationSegment* pSeg = new CC3NodeAnimationSegment;
	pSeg->initOnAnimation( baseAnimation, startTime, endTime );
	pSeg->autorelease();

	return pSeg;
}

void CC3NodeAnimationSegment::initOnAnimation( CC3NodeAnimation* baseAnimation, GLuint startFrameIndex, GLuint endFrameIndex )
{
	initOnAnimation(baseAnimation);
	setStartFrameIndex( startFrameIndex );
	setEndFrameIndex( endFrameIndex );
}

CC3NodeAnimationSegment* CC3NodeAnimationSegment::animationOnAnimation( CC3NodeAnimation* baseAnimation, GLuint startFrameIndex, GLuint endFrameIndex )
{
	CC3NodeAnimationSegment* pSeg = new CC3NodeAnimationSegment;
	pSeg->initOnAnimation( baseAnimation, startFrameIndex, endFrameIndex );
	pSeg->autorelease();

	return pSeg;
}

/**
 * Overridden to interpret the specified time as within the range specified by the startTime
 * and endTime properties, and then to retrieve the corresponding frame index from the base
 * animation.
 */
GLuint CC3NodeAnimationSegment::getFrameIndexAt( float t )
{
	float adjTime = _startTime + ((_endTime - _startTime) * t);
	return _baseAnimation->getFrameIndexAt(adjTime);
}

float CC3NodeAnimationSegment::timeAtFrame( GLuint frameIndex )
{
	return _baseAnimation->timeAtFrame( frameIndex ); 
}

CC3Vector CC3NodeAnimationSegment::getLocationAtFrame( GLuint frameIndex )
{
	return _baseAnimation->getLocationAtFrame( frameIndex );
}

CC3Quaternion CC3NodeAnimationSegment::getQuaternionAtFrame( GLuint frameIndex )
{
	return _baseAnimation->getQuaternionAtFrame( frameIndex );
}

CC3Vector CC3NodeAnimationSegment::getScaleAtFrame( GLuint frameIndex  )
{
	return _baseAnimation->getScaleAtFrame( frameIndex );
}

CC3NodeAnimationState::CC3NodeAnimationState()
{
	_node = NULL;
	_animation = NULL;
}

CC3NodeAnimationState::~CC3NodeAnimationState()
{
	_node = NULL;
	CC_SAFE_RELEASE( _animation );
}

bool CC3NodeAnimationState::isEnabled()
{
	return _isEnabled; 
}

void CC3NodeAnimationState::setIsEnabled( bool isEnabled )
{
	_isEnabled = isEnabled;
	markDirty();
}

void CC3NodeAnimationState::enable()
{
	setIsEnabled( true );
}

void CC3NodeAnimationState::disable()
{
	setIsEnabled( false );
}

GLfloat CC3NodeAnimationState::getBlendingWeight()
{
	return _blendingWeight; 
}

void CC3NodeAnimationState::setBlendingWeight( GLfloat blendingWeight )
{
	_blendingWeight = CLAMP(blendingWeight, 0.0f, 1.0f);
	markDirty();
}

CC3Vector CC3NodeAnimationState::getLocation()
{
	return _location; 
}

void CC3NodeAnimationState::setLocation( const CC3Vector& location )
{
	_location = location;
	markDirty();
}

CC3Quaternion CC3NodeAnimationState::getQuaternion()
{
	return _quaternion; 
}

void CC3NodeAnimationState::setQuaternion( const CC3Quaternion& quaternion )
{
	_quaternion = quaternion;
	markDirty();
}

CC3Vector CC3NodeAnimationState::getScale()
{
	return _scale; 
}

void CC3NodeAnimationState::setScale( const CC3Vector& scale )
{
	_scale = scale;
	markDirty();
}

void CC3NodeAnimationState::markDirty()
{
	_node->markAnimationDirty(); 
}

GLuint CC3NodeAnimationState::getFrameCount()
{
	return _animation->getFrameCount(); 
}

bool CC3NodeAnimationState::isAnimatingLocation()
{
	return _isLocationAnimationEnabled && _animation->isAnimatingLocation(); 
}

bool CC3NodeAnimationState::isAnimatingQuaternion()
{
	return _isQuaternionAnimationEnabled && _animation->isAnimatingQuaternion(); 
}

bool CC3NodeAnimationState::isAnimatingScale()
{
	return _isScaleAnimationEnabled && _animation->isAnimatingScale(); 
}

bool CC3NodeAnimationState::isAnimating()
{
	return (isEnabled() && (isAnimatingLocation() ||
												 isAnimatingQuaternion() ||
												 isAnimatingScale())); }

bool CC3NodeAnimationState::hasVariableFrameTiming()
{
	return _animation->hasVariableFrameTiming(); 
}

void CC3NodeAnimationState::establishFrameAt( float t )
{
	_animationTime = t;
	if (isEnabled()) 
		_animation->establishFrameAt( t, this );
}

void CC3NodeAnimationState::initWithAnimation( CC3NodeAnimation* animation, GLuint trackID, CC3Node* node )
{
	CCAssert(animation, "CC3NodeAnimationState must be created with a valid animation.");
	CCAssert(node, "CC3NodeAnimationState must be created with a valid node.");
	_node = node;						// weak reference
	_animation = animation;
	animation->retain();
	_trackID = trackID;
	_blendingWeight = 1.0f;
	_animationTime = 0.0f;
	_location = kCC3VectorZero;
	_quaternion = kCC3QuaternionIdentity;
	_scale = kCC3VectorUnitCube;
	_isEnabled = true;
	_isLocationAnimationEnabled = true;
	_isQuaternionAnimationEnabled = true;
	_isScaleAnimationEnabled = true;
	establishFrameAt( 0.0f );		// Start on the initial frame
}

CC3NodeAnimationState* CC3NodeAnimationState::animationStateWithAnimation( CC3NodeAnimation* animation, GLuint trackID, CC3Node* node )
{
	CC3NodeAnimationState* pState = new CC3NodeAnimationState;
	pState->initWithAnimation( animation, trackID, node );
	pState->autorelease();

	return pState;
}

static GLuint _lastTrackID = 0;

// Pre-increment to start with one. Zero reserved for default track.
GLuint CC3NodeAnimationState::generateTrackID()
{
	return ++_lastTrackID; 
}

std::string CC3NodeAnimationState::description()
{
	return stringWithFormat( (char*)"CC3NodeAnimationState for node %s with animation on track %u",
			 _node->getName().c_str(), _trackID );
}

std::string CC3NodeAnimationState::describeCurrentState()
{
	std::string desc = "";
	desc += stringWithFormat( (char*)"Time: %.4f", _animationTime );
	if (isAnimatingLocation()) 
		desc += stringWithFormat( (char*)" Loc: %s", stringFromCC3Vector(getLocation()).c_str() );
	if (isAnimatingQuaternion())
		desc += stringWithFormat( (char*)" Quat: %s", stringFromCC3Quaternion(getQuaternion()).c_str() );
	if (isAnimatingScale())
		desc += stringWithFormat( (char*)" Scale: %s", stringFromCC3Vector(getScale()).c_str() );
	if ( !isAnimating()) 
		desc += stringWithFormat( (char*)" No animation enabled." );

	return desc;
}

std::string CC3NodeAnimationState::describeStateForFrames( GLuint frameCount, float startTime, float endTime )
{
	startTime = CLAMP(startTime, 0.0f, 1.0f);
	endTime = CLAMP(endTime, 0.0f, 1.0f);

	// Generating the description changes current state, so cache it for resortation below
	float currTime = _animationTime;
	bool wasCurrentlyEnabled = isEnabled();
	setIsEnabled( true );
	
	float frameDur = 0.0f;
	if (frameCount > 1) 
		frameDur = (endTime - startTime) / (GLfloat)(frameCount - 1);
	std::string desc = "";
	desc += stringWithFormat( (char*)"%s animated state on track %d over %d frames from %.4f to %.4f:", _node->getName().c_str(), _trackID, frameCount, startTime, endTime );
	if (isAnimating() && frameCount > 0)
		for (GLuint fIdx = 0; fIdx < frameCount; fIdx++) 
		{
			establishFrameAt( startTime + (frameDur * fIdx) );
			desc += stringWithFormat( (char*)"\n\t%s", describeCurrentState().c_str() );
		}
	else
		desc += " No animation enabled.";
	
	// Return to where we were before the description was generated
	establishFrameAt( currTime );
	setIsEnabled( wasCurrentlyEnabled );

	return desc;
}

std::string CC3NodeAnimationState::describeStateForFrames( GLuint frameCount )
{
	return describeStateForFrames( frameCount, 0.0f, 1.0f );
}

bool CC3NodeAnimationState::isLocationAnimationEnabled()
{
	return _isLocationAnimationEnabled;
}

void CC3NodeAnimationState::setIsLocationAnimationEnabled( bool enable )
{
	_isLocationAnimationEnabled = enable;
}

bool CC3NodeAnimationState::isQuaternionAnimationEnabled()
{
	return _isQuaternionAnimationEnabled;
}

void CC3NodeAnimationState::setIsQuaternionAnimationEnabled( bool enable )
{
	_isQuaternionAnimationEnabled = enable;
}

bool CC3NodeAnimationState::isScaleAnimationEnabled()
{
	return _isScaleAnimationEnabled;
}

void CC3NodeAnimationState::setIsScaleAnimationEnabled( bool enable )
{
	_isScaleAnimationEnabled = enable;
}

float CC3NodeAnimationState::getAnimationTime()
{
	return _animationTime;
}

void CC3NodeAnimationState::setAnimationTime( float time )
{
	_animationTime = time;
}

GLuint CC3NodeAnimationState::getTrackID()
{
	return _trackID;
}

void CC3NodeAnimationState::setTrackID( GLuint trackID )
{
	_trackID = trackID;
}

CC3NodeAnimation* CC3NodeAnimationState::getAnimation()
{
	return _animation;
}


NS_COCOS3D_END
