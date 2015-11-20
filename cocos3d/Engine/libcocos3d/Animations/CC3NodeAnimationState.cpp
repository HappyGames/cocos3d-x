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

CC3NodeAnimationState::CC3NodeAnimationState()
{
	m_pNode = NULL;
	m_pAnimation = NULL;
}

CC3NodeAnimationState::~CC3NodeAnimationState()
{
	m_pNode = NULL;
	CC_SAFE_RELEASE( m_pAnimation );
}

bool CC3NodeAnimationState::isEnabled()
{
	return m_isEnabled; 
}

void CC3NodeAnimationState::setIsEnabled( bool isEnabled )
{
	m_isEnabled = isEnabled;
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
	return m_fBlendingWeight; 
}

void CC3NodeAnimationState::setBlendingWeight( GLfloat blendingWeight )
{
	m_fBlendingWeight = CLAMP(blendingWeight, 0.0f, 1.0f);
	markDirty();
}

CC3Vector CC3NodeAnimationState::getLocation()
{
	return m_location; 
}

void CC3NodeAnimationState::setLocation( const CC3Vector& location )
{
	m_location = location;
	markDirty();
}

CC3Quaternion CC3NodeAnimationState::getQuaternion()
{
	return m_quaternion; 
}

void CC3NodeAnimationState::setQuaternion( const CC3Quaternion& quaternion )
{
	m_quaternion = quaternion;
	markDirty();
}

CC3Vector CC3NodeAnimationState::getScale()
{
	return m_scale; 
}

void CC3NodeAnimationState::setScale( const CC3Vector& scale )
{
	m_scale = scale;
	markDirty();
}

void CC3NodeAnimationState::markDirty()
{
	m_pNode->markAnimationDirty(); 
}

GLuint CC3NodeAnimationState::getFrameCount()
{
	return m_pAnimation->getFrameCount(); 
}

bool CC3NodeAnimationState::isAnimatingLocation()
{
	return m_isLocationAnimationEnabled && m_pAnimation->isAnimatingLocation(); 
}

bool CC3NodeAnimationState::isAnimatingQuaternion()
{
	return m_isQuaternionAnimationEnabled && m_pAnimation->isAnimatingQuaternion(); 
}

bool CC3NodeAnimationState::isAnimatingScale()
{
	return m_isScaleAnimationEnabled && m_pAnimation->isAnimatingScale(); 
}

bool CC3NodeAnimationState::isAnimating()
{
	return (isEnabled() && (isAnimatingLocation() ||
												 isAnimatingQuaternion() ||
												 isAnimatingScale())); }

bool CC3NodeAnimationState::hasVariableFrameTiming()
{
	return m_pAnimation->hasVariableFrameTiming(); 
}

void CC3NodeAnimationState::establishFrameAt( float t )
{
	m_fAnimationTime = t;
	if (isEnabled()) 
		m_pAnimation->establishFrameAt( t, this );
}

void CC3NodeAnimationState::initWithAnimation( CC3NodeAnimation* animation, GLuint trackID, CC3Node* node )
{
	CCAssert(animation, "CC3NodeAnimationState must be created with a valid animation.");
	CCAssert(node, "CC3NodeAnimationState must be created with a valid node.");
	m_pNode = node;						// weak reference
	m_pAnimation = animation;
	animation->retain();
	m_trackID = trackID;
	m_fBlendingWeight = 1.0f;
	m_fAnimationTime = 0.0f;
	m_location = CC3Vector::kCC3VectorZero;
	m_quaternion = CC3Quaternion::kCC3QuaternionIdentity;
	m_scale = CC3Vector::kCC3VectorUnitCube;
	m_isEnabled = true;
	m_isLocationAnimationEnabled = true;
	m_isQuaternionAnimationEnabled = true;
	m_isScaleAnimationEnabled = true;
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
	return CC3String::stringWithFormat( (char*)"CC3NodeAnimationState for node %s with animation on track %u",
			 m_pNode->getName().c_str(), m_trackID );
}

std::string CC3NodeAnimationState::describeCurrentState()
{
	std::string desc = "";
	desc += CC3String::stringWithFormat( (char*)"Time: %.4f", m_fAnimationTime );
	if (isAnimatingLocation()) 
		desc += CC3String::stringWithFormat( (char*)" Loc: %s", getLocation().stringfy().c_str() );
	if (isAnimatingQuaternion())
		desc += CC3String::stringWithFormat( (char*)" Quat: %s", getQuaternion().stringfy().c_str() );
	if (isAnimatingScale())
		desc += CC3String::stringWithFormat( (char*)" Scale: %s", getScale().stringfy().c_str() );
	if ( !isAnimating()) 
		desc += CC3String::stringWithFormat( (char*)" No animation enabled." );

	return desc;
}

std::string CC3NodeAnimationState::describeStateForFrames( GLuint frameCount, float startTime, float endTime )
{
	startTime = CLAMP(startTime, 0.0f, 1.0f);
	endTime = CLAMP(endTime, 0.0f, 1.0f);

	// Generating the description changes current state, so cache it for resortation below
	float currTime = m_fAnimationTime;
	bool wasCurrentlyEnabled = isEnabled();
	setIsEnabled( true );
	
	float frameDur = 0.0f;
	if (frameCount > 1) 
		frameDur = (endTime - startTime) / (GLfloat)(frameCount - 1);
	std::string desc = "";
	desc += CC3String::stringWithFormat( (char*)"%s animated state on track %d over %d frames from %.4f to %.4f:", m_pNode->getName().c_str(), m_trackID, frameCount, startTime, endTime );
	if (isAnimating() && frameCount > 0)
	{
		for (GLuint fIdx = 0; fIdx < frameCount; fIdx++) 
		{
			establishFrameAt( startTime + (frameDur * fIdx) );
			desc += CC3String::stringWithFormat( (char*)"\n\t%s", describeCurrentState().c_str() );
		}
	}
	else
	{
		desc += " No animation enabled.";
	}

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
	return m_isLocationAnimationEnabled;
}

void CC3NodeAnimationState::setIsLocationAnimationEnabled( bool enable )
{
	m_isLocationAnimationEnabled = enable;
}

bool CC3NodeAnimationState::isQuaternionAnimationEnabled()
{
	return m_isQuaternionAnimationEnabled;
}

void CC3NodeAnimationState::setIsQuaternionAnimationEnabled( bool enable )
{
	m_isQuaternionAnimationEnabled = enable;
}

bool CC3NodeAnimationState::isScaleAnimationEnabled()
{
	return m_isScaleAnimationEnabled;
}

void CC3NodeAnimationState::setIsScaleAnimationEnabled( bool enable )
{
	m_isScaleAnimationEnabled = enable;
}

float CC3NodeAnimationState::getAnimationTime()
{
	return m_fAnimationTime;
}

void CC3NodeAnimationState::setAnimationTime( float time )
{
	m_fAnimationTime = time;
}

GLuint CC3NodeAnimationState::getTrackID()
{
	return m_trackID;
}

void CC3NodeAnimationState::setTrackID( GLuint trackID )
{
	m_trackID = trackID;
}

CC3NodeAnimation* CC3NodeAnimationState::getAnimation()
{
	return m_pAnimation;
}


NS_COCOS3D_END
