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

CC3NodeAnimationSegment::CC3NodeAnimationSegment()
{
	m_pBaseAnimation = NULL;
}

CC3NodeAnimationSegment::~CC3NodeAnimationSegment()
{
	CC_SAFE_RELEASE( m_pBaseAnimation );
}

CC3NodeAnimation* CC3NodeAnimationSegment::getBaseAnimation()
{
	return m_pBaseAnimation;
}

GLfloat CC3NodeAnimationSegment::getStartTime()
{
	return m_startTime;
}

void CC3NodeAnimationSegment::setStartTime( float time )
{
	m_startTime = time;
}

GLfloat CC3NodeAnimationSegment::getEndTime()
{
	return m_endTime;
}

void CC3NodeAnimationSegment::setEndTime( float time )
{
	m_endTime = time;
}

GLuint CC3NodeAnimationSegment::getFrameCount()
{
	return m_pBaseAnimation->getFrameCount(); 
}

bool CC3NodeAnimationSegment::shouldInterpolate()
{
	return m_pBaseAnimation->shouldInterpolate(); 
}

void CC3NodeAnimationSegment::setShouldInterpolate( bool _shouldInterpolate )
{
	m_pBaseAnimation->setShouldInterpolate( _shouldInterpolate );
}

bool CC3NodeAnimationSegment::isAnimatingLocation()
{
	return m_pBaseAnimation->isAnimatingLocation(); 
}

bool CC3NodeAnimationSegment::isAnimatingQuaternion()
{
	return m_pBaseAnimation->isAnimatingQuaternion(); 
}

bool CC3NodeAnimationSegment::isAnimatingScale()
{
	return m_pBaseAnimation->isAnimatingScale(); 
}

bool CC3NodeAnimationSegment::hasVariableFrameTiming()
{
	return m_pBaseAnimation->hasVariableFrameTiming(); 
}

GLuint CC3NodeAnimationSegment::getStartFrameIndex()
{
	return m_pBaseAnimation->getFrameIndexAt( getStartTime() ); 
}

void CC3NodeAnimationSegment::setStartFrameIndex( GLuint startFrameIndex )
{
	setStartTime( m_pBaseAnimation->timeAtFrame(startFrameIndex) );
}

GLuint CC3NodeAnimationSegment::getEndFrameIndex()
{
	return m_pBaseAnimation->getFrameIndexAt( getEndTime() ); 
}

void CC3NodeAnimationSegment::setEndFrameIndex( GLuint endFrameIndex )
{
	setEndTime( m_pBaseAnimation->timeAtFrame( endFrameIndex ) );
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
		m_pBaseAnimation = baseAnimation;
		m_pBaseAnimation->retain();
		m_startTime = startTime;
		m_endTime = endTime;
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
	float adjTime = m_startTime + ((m_endTime - m_startTime) * t);
	return m_pBaseAnimation->getFrameIndexAt(adjTime);
}

float CC3NodeAnimationSegment::timeAtFrame( GLuint frameIndex )
{
	return m_pBaseAnimation->timeAtFrame( frameIndex ); 
}

CC3Vector CC3NodeAnimationSegment::getLocationAtFrame( GLuint frameIndex )
{
	return m_pBaseAnimation->getLocationAtFrame( frameIndex );
}

CC3Quaternion CC3NodeAnimationSegment::getQuaternionAtFrame( GLuint frameIndex )
{
	return m_pBaseAnimation->getQuaternionAtFrame( frameIndex );
}

CC3Vector CC3NodeAnimationSegment::getScaleAtFrame( GLuint frameIndex  )
{
	return m_pBaseAnimation->getScaleAtFrame( frameIndex );
}

NS_COCOS3D_END
