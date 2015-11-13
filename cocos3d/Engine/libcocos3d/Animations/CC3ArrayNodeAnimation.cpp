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
			_animatedQuaternions[i] = CC3Quaternion::kCC3QuaternionIdentity;

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
			_animatedScales[i] = CC3Vector::kCC3VectorUnitCube;
		
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

NS_COCOS3D_END
