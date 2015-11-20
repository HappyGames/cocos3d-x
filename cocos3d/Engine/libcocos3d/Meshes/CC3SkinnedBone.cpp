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

CC3SkinnedBone::CC3SkinnedBone()
{
	m_pSkinNode = NULL;
	m_pBone = NULL;
	m_transformMatrix = NULL;
}

CC3SkinnedBone::~CC3SkinnedBone()
{
	if ( m_pSkinNode )
	{
		m_pSkinNode->removeTransformListener( this );
		m_pSkinNode = NULL;								// weak reference
	}

	if ( m_pBone )
	{
		m_pBone->removeTransformListener( this );
		m_pBone = NULL;									// weak reference
	}

	CC_SAFE_RELEASE( m_transformMatrix );
}

CC3Bone* CC3SkinnedBone::getBone()
{
	return m_pBone;
}


void CC3SkinnedBone::markTransformDirty()
{
	if ( m_transformMatrix )
		m_transformMatrix->setIsDirty( true );
}

CC3Matrix* CC3SkinnedBone::getTransformMatrix()
{
	if ( m_transformMatrix->isDirty() ) 
	{
		m_transformMatrix->populateFrom( m_pSkinNode->getSkeletalTransformMatrixInverted() );
		m_transformMatrix->multiplyBy( m_pBone->getSkeletalTransformMatrix() );
		m_transformMatrix->multiplyBy( m_pBone->getRestPoseSkeletalTransformMatrixInverted() );
		m_transformMatrix->multiplyBy( m_pSkinNode->getSkeletalTransformMatrix() );
		m_transformMatrix->setIsDirty( false );
	}
	return m_transformMatrix;
}

// This will raise an assertion without a skin node or bone.
void CC3SkinnedBone::init()
{
	initWithSkin( NULL, NULL ); 
}

void CC3SkinnedBone::initWithSkin( CC3SkinMeshNode* aNode, CC3Bone* aBone )
{
	CCAssert(aNode, "CC3SkinnedBone must be initialized with a skin node.");
	CCAssert(aBone, "CC3SkinnedBone must be initialized with a bone.");

	m_pSkinNode = aNode;							// weak reference
	m_pSkinNode->addTransformListener( this );

	m_pBone = aBone;								// weak reference
	m_pBone->addTransformListener( this );

	m_transformMatrix = CC3AffineMatrix::matrix();	// retained
	m_transformMatrix->retain();
	markTransformDirty();
}

/**
 * Allocates and initializes an autoreleased instance that
 * applies the specified bone to the specified mesh node.
 */
CC3SkinnedBone* CC3SkinnedBone::skinnedBoneWithSkin( CC3SkinMeshNode* aNode, CC3Bone* aBone )
{
	CC3SkinnedBone* sb = new CC3SkinnedBone;
	sb->initWithSkin( aNode, aBone );
	sb->autorelease();

	return sb;
}

/** Either the bone or skin node were transformed. Mark the transforms of this skinned bone dirty. */
void CC3SkinnedBone::nodeWasTransformed( CC3Node* aNode )
{
	markTransformDirty();
	if (aNode == m_pBone)
		m_pSkinNode->boneWasTransformed( m_pBone );
}

/**
 * If either of the nodes to whom I have registered as a
 * listener disappears before I do, clear the reference to it.
 */
void CC3SkinnedBone::nodeWasDestroyed( CC3Node* aNode )
{
	if (aNode == m_pSkinNode) 
		m_pSkinNode = NULL;	// weak reference
	
	if (aNode == m_pBone) 
		m_pBone = NULL;		// weak reference
}

NS_COCOS3D_END
