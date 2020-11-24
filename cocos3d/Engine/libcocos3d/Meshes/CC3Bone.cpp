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

CC3Bone::CC3Bone()
{
	m_skeletalTransformMatrix = NULL;
	m_restPoseSkeletalTransformMatrixInverted = NULL;
}

CC3Bone::~CC3Bone()
{
	CC_SAFE_RELEASE( m_skeletalTransformMatrix );
	CC_SAFE_RELEASE( m_restPoseSkeletalTransformMatrixInverted );
}

bool CC3Bone::hasSoftBodyContent()
{
	return true; 
}

void CC3Bone::ensureRigidSkeleton()
{
	setUniformScale( 1.0f );
	disableScaleAnimation();
	super::ensureRigidSkeleton();
}

void CC3Bone::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	{
		m_skeletalTransformMatrix = CC3AffineMatrix::matrix();					// retained
		m_skeletalTransformMatrix->retain();
		m_restPoseSkeletalTransformMatrixInverted = CC3AffineMatrix::matrix();	// retained
		m_restPoseSkeletalTransformMatrixInverted->retain();
	}
}

void CC3Bone::populateFrom( CC3Bone* another )
{
	super::populateFrom( another );
	
	// The skeletal transform matrix is not copied
	m_restPoseSkeletalTransformMatrixInverted->populateFrom( another->getRestPoseSkeletalTransformMatrixInverted() );
}

CCObject* CC3Bone::copyWithZone( CCZone* zone )
{
	CC3Bone* pBone = new CC3Bone;
	pBone->init();
	pBone->populateFrom( this );

	pBone->addCopiesOfChildrenFrom( this );

	return pBone;
}

void CC3Bone::markTransformDirty()
{
	super::markTransformDirty();
	m_skeletalTransformMatrix->setIsDirty( true );
}

CC3Matrix* CC3Bone::getSkeletalTransformMatrix()
{
	if (m_skeletalTransformMatrix->isDirty()) 
	{
		m_skeletalTransformMatrix->populateFrom( getGlobalTransformMatrix() );
		m_skeletalTransformMatrix->leftMultiplyBy( getSoftBodyNode()->getGlobalTransformMatrixInverted() );
		m_skeletalTransformMatrix->setIsDirty( false );
	}

	return m_skeletalTransformMatrix;
}

CC3Matrix* CC3Bone::getRestPoseSkeletalTransformMatrixInverted()
{
	return m_restPoseSkeletalTransformMatrixInverted;
}

void CC3Bone::bindRestPose()
{
	cacheRestPoseMatrix();
	super::bindRestPose();
}

/** Inverts the transform matrix and caches it as the inverted rest pose matrix. */
void CC3Bone::cacheRestPoseMatrix()
{
	m_restPoseSkeletalTransformMatrixInverted->populateFrom( getSkeletalTransformMatrix() );
	m_restPoseSkeletalTransformMatrixInverted->invert();
}

CC3Bone* CC3Bone::create()
{
    CC3Bone* pBone = new CC3Bone;
    pBone->init();
    pBone->autorelease();
    
    return pBone;
}

NS_COCOS3D_END
