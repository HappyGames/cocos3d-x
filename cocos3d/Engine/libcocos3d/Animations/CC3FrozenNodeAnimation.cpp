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


void CC3FrozenNodeAnimation::init()
{
    return initWithFrameCount(1);
}

void CC3FrozenNodeAnimation::initWithFrameCount( GLuint numFrames )
{
    super::initWithFrameCount(1);
    {
        m_shouldInterpolate = false;
        m_location = CC3Vector::kCC3VectorNull;
        m_quaternion = CC3Quaternion::kCC3QuaternionNull;
        m_scale = CC3Vector::kCC3VectorNull;
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

CC3Vector CC3FrozenNodeAnimation::getScale()
{
	return m_scale;
}

CC3Quaternion CC3FrozenNodeAnimation::getQuaternion()
{
	return m_quaternion;
}

CC3Vector CC3FrozenNodeAnimation::getLocation()
{
	return m_location;
}

bool CC3FrozenNodeAnimation::isAnimatingLocation()
{
	return !m_location.isNull(); 
}

bool CC3FrozenNodeAnimation::isAnimatingQuaternion()
{
	return !m_quaternion.isNull(); 
}

bool CC3FrozenNodeAnimation::isAnimatingScale()
{
	return !m_scale.isNull(); 
}

void CC3FrozenNodeAnimation::populateFromNodeState( CC3Node* node )
{
	m_location = node->getLocation();
	m_quaternion = node->getQuaternion();
	m_scale = node->getScale();
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

NS_COCOS3D_END
