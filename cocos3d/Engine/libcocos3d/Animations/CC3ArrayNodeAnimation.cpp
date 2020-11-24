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
	return m_animatedLocations != NULL; 
}

bool CC3ArrayNodeAnimation::isAnimatingQuaternion()
{
	return m_animatedQuaternions != NULL; 
}

bool CC3ArrayNodeAnimation::isAnimatingScale() 
{
	return m_animatedScales != NULL; 
}

bool CC3ArrayNodeAnimation::hasVariableFrameTiming()
{
	return m_frameTimes != NULL; 
}

void CC3ArrayNodeAnimation::initWithFrameCount( GLuint numFrames )
{
	super::initWithFrameCount( numFrames );
	{
		m_frameTimes = NULL;
		m_animatedLocations = NULL;
		m_animatedQuaternions = NULL;
		m_animatedScales = NULL;
		m_frameTimesAreRetained = false;
		m_locationsAreRetained = false;
		m_quaternionsAreRetained = false;
		m_scalesAreRetained = false;
	}
}

// All times should be in range between zero and one
float CC3ArrayNodeAnimation::timeAtFrame( GLuint frameIndex )
{
	if (!m_frameTimes) 
		return super::timeAtFrame(frameIndex);

	return m_frameTimes[MIN(frameIndex, m_frameCount - 1)];
}

// Iterate backwards through the frames looking for the first frame whose time is at or before
// the specified frame time, and return that frame. If the specified frame is before the first
// frame, return the first frame.
GLuint CC3ArrayNodeAnimation::getFrameIndexAt( float t )
{
	if (!m_frameTimes) 
		return super::getFrameIndexAt(t);

	for (GLint fIdx = m_frameCount - 1; fIdx >= 0; fIdx--)	// start at last frame
		if (m_frameTimes[fIdx] <= t) 
			return fIdx;			// return frame

	return 0;
}

CC3Vector CC3ArrayNodeAnimation::getLocationAtFrame( GLuint frameIndex )
{
	if (!m_animatedLocations) 
		return super::getLocationAtFrame( frameIndex );

	return m_animatedLocations[MIN(frameIndex, m_frameCount - 1)];
}

CC3Quaternion CC3ArrayNodeAnimation::getQuaternionAtFrame( GLuint frameIndex )
{
	if (!m_animatedQuaternions) 
		return super::getQuaternionAtFrame( frameIndex );

	return m_animatedQuaternions[MIN(frameIndex, m_frameCount - 1)];
}

CC3Vector CC3ArrayNodeAnimation::getScaleAtFrame( GLuint frameIndex )
{
	if (!m_animatedScales) 
		return super::getScaleAtFrame( frameIndex );

	return m_animatedScales[MIN(frameIndex, m_frameCount - 1)];
}

void CC3ArrayNodeAnimation::setFrameTimes( float* frameTimes )
{
	deallocateFrameTimes();			// get rid of any existing array
	m_frameTimes = frameTimes;
}

void CC3ArrayNodeAnimation::setAnimatedLocations( CC3Vector* vectorArray )
{
	deallocateLocations();				// get rid of any existing array
	m_animatedLocations = vectorArray;
}

void CC3ArrayNodeAnimation::setAnimatedQuaternions( CC3Quaternion* vectorArray )
{
	deallocateQuaternions();			// get rid of any existing array
	m_animatedQuaternions = vectorArray;
}

void CC3ArrayNodeAnimation::setAnimatedScales( CC3Vector* vectorArray )
{
	deallocateScales();				// get rid of any existing array
	m_animatedScales= vectorArray;
}

float* CC3ArrayNodeAnimation::allocateFrameTimes()
{
	if (m_frameCount) 
	{
		setFrameTimes( (float*)calloc(m_frameCount, sizeof(float)) );
		m_frameTimesAreRetained = true;
		CC3_TRACE("CC3ArrayNodeAnimation allocated space for %d frame times", m_frameCount);
	}
	return m_frameTimes;
}

void CC3ArrayNodeAnimation::deallocateFrameTimes()
{
	if (m_frameTimesAreRetained) 
	{
		free(m_frameTimes);
		m_frameTimes = NULL;
		m_frameTimesAreRetained = false;
		CC3_TRACE("CC3ArrayNodeAnimation deallocated %d previously allocated frame times", m_frameCount);
	}
}

CC3Vector* CC3ArrayNodeAnimation::allocateLocations()
{
	if (m_frameCount) 
	{
		setAnimatedLocations( (CC3Vector*)calloc(m_frameCount, sizeof(CC3Vector)) );
		m_locationsAreRetained = true;
		CC3_TRACE("CC3ArrayNodeAnimation allocated space for %d locations", m_frameCount);
	}
	return m_animatedLocations;
}

void CC3ArrayNodeAnimation::deallocateLocations()
{
	if (m_locationsAreRetained) 
	{
		free(m_animatedLocations);
		m_animatedLocations = NULL;
		m_locationsAreRetained = false;
		CC3_TRACE("CC3ArrayNodeAnimation deallocated %d previously allocated animated locations", m_frameCount);
	}
}

CC3Quaternion* CC3ArrayNodeAnimation::allocateQuaternions()
{
	if (m_frameCount) 
	{
		setAnimatedQuaternions( (CC3Quaternion*)calloc(m_frameCount, sizeof(CC3Quaternion)) );

		for (GLuint i = 0; i < m_frameCount; i++) 
			m_animatedQuaternions[i] = CC3Quaternion::kCC3QuaternionIdentity;

		m_quaternionsAreRetained = true;
		CC3_TRACE("CC3ArrayNodeAnimation allocated space for %d quaternions", m_frameCount);
	}

	return m_animatedQuaternions;
}

void CC3ArrayNodeAnimation::deallocateQuaternions()
{
	if (m_quaternionsAreRetained) 
	{
		free(m_animatedQuaternions);
		m_animatedQuaternions = NULL;
		m_quaternionsAreRetained = false;
		CC3_TRACE("CC3ArrayNodeAnimation deallocated %d previously allocated animated quaternions", m_frameCount);
	}
}

CC3Vector* CC3ArrayNodeAnimation::allocateScales()
{
	if (m_frameCount) 
	{
		setAnimatedScales( (CC3Vector*)calloc(m_frameCount, sizeof(CC3Vector)) );
		for (GLuint i = 0; i < m_frameCount; i++) 
			m_animatedScales[i] = CC3Vector::kCC3VectorUnitCube;
		
		m_scalesAreRetained = true;
		CC3_TRACE("CC3ArrayNodeAnimation allocated space for %d scales", m_frameCount);
	}
	return m_animatedScales;
}

void CC3ArrayNodeAnimation::deallocateScales()
{
	if (m_scalesAreRetained) 
	{
		free(m_animatedScales);
		m_animatedScales = NULL;
		m_scalesAreRetained = false;
		CC3_TRACE("CC3ArrayNodeAnimation deallocated %d previously allocated animated scales", m_frameCount);
	}
}

NS_COCOS3D_END
