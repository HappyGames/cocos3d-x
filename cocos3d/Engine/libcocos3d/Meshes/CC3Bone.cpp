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

CC3Bone::CC3Bone()
{
	_skeletalTransformMatrix = NULL;
	_restPoseSkeletalTransformMatrixInverted = NULL;
}

CC3Bone::~CC3Bone()
{
	CC_SAFE_RELEASE( _skeletalTransformMatrix );
	CC_SAFE_RELEASE( _restPoseSkeletalTransformMatrixInverted );
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
		_skeletalTransformMatrix = CC3AffineMatrix::matrix();					// retained
		_skeletalTransformMatrix->retain();
		_restPoseSkeletalTransformMatrixInverted = CC3AffineMatrix::matrix();	// retained
		_restPoseSkeletalTransformMatrixInverted->retain();
	}
}

void CC3Bone::populateFrom( CC3Bone* another )
{
	super::populateFrom( another );
	
	// The skeletal transform matrix is not copied
	_restPoseSkeletalTransformMatrixInverted->populateFrom( another->getRestPoseSkeletalTransformMatrixInverted() );
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
	_skeletalTransformMatrix->setIsDirty( true );
}

CC3Matrix* CC3Bone::getSkeletalTransformMatrix()
{
	if (_skeletalTransformMatrix->isDirty()) 
	{
		_skeletalTransformMatrix->populateFrom( getGlobalTransformMatrix() );
		_skeletalTransformMatrix->leftMultiplyBy( getSoftBodyNode()->getGlobalTransformMatrixInverted() );
		_skeletalTransformMatrix->setIsDirty( false );
	}
	return _skeletalTransformMatrix;
}

CC3Matrix* CC3Bone::getRestPoseSkeletalTransformMatrixInverted()
{
	return _restPoseSkeletalTransformMatrixInverted;
}

void CC3Bone::bindRestPose()
{
	cacheRestPoseMatrix();
	super::bindRestPose();
}

/** Inverts the transform matrix and caches it as the inverted rest pose matrix. */
void CC3Bone::cacheRestPoseMatrix()
{
	_restPoseSkeletalTransformMatrixInverted->populateFrom( getSkeletalTransformMatrix() );
	_restPoseSkeletalTransformMatrixInverted->invert();
}

CC3Bone* CC3Bone::create()
{
    CC3Bone* pBone = new CC3Bone;
    pBone->init();
    pBone->autorelease();
    
    return pBone;
}

NS_COCOS3D_END
