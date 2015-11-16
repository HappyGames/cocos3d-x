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

GLint CC3PODNode::getPodIndex()
{ 
	return _podIndex; 
}

void CC3PODNode::setPodIndex( GLint aPODIndex )
{
	_podIndex = aPODIndex; 
}

// Subclasses must override to use instance variable.
GLint CC3PODNode::getPodContentIndex()
{
	return _podContentIndex; 
}

// Subclasses must override to use instance variable.
void CC3PODNode::setPodContentIndex( GLint aPODIndex )
{
	_podContentIndex = aPODIndex;
}
 
// Subclasses must override to use instance variable.
GLint CC3PODNode::getPodParentIndex()
{
	return _podParentIndex; 
}

// Subclasses must override to use instance variable.
void CC3PODNode::setPodParentIndex( GLint aPODIndex )
{
	_podParentIndex = aPODIndex;
}

void CC3PODNode::initAtIndex( GLint aPODIndex, CC3PODResource* aPODRez )
{
    init();
    setPodIndex( aPODIndex );
    
    SPODNode* psn = (SPODNode*)aPODRez->getNodePODStructAtIndex( aPODIndex );
    //LogRez(@"Creating %@ at index %i from: %@", [self class], aPODIndex, NSStringFromSPODNode(psn));
    setName( psn->pszName );
    setPodContentIndex( psn->nIdx );
    setPodParentIndex( psn->nIdxParent );
    
    if ( psn->pfAnimPosition )
        setLocation( *(CC3Vector*)psn->pfAnimPosition );
    if ( psn->pfAnimRotation )
        setQuaternion( *(CC3Quaternion*)psn->pfAnimRotation );
    if ( psn->pfAnimScale )
        setScale( *(CC3Vector*)psn->pfAnimScale );
    
    if ( CC3PODNodeAnimation::sPODNodeDoesContainAnimation((PODStructPtr)psn) )
        setAnimation( CC3PODNodeAnimation::animationFromSPODNode( (PODStructPtr)psn, aPODRez->getAnimationFrameCount() ) );
    else if (aPODRez->shouldFreezeInanimateNodes())
        setAnimation( CC3FrozenNodeAnimation::animationFromNodeState( this ) );
}

CC3PODNode* CC3PODNode::nodeAtIndex( GLint aPODIndex, CC3PODResource* aPODRez )
{
	CC3PODNode* pNode = new CC3PODNode;
	pNode->initAtIndex( aPODIndex, aPODRez );
	pNode->autorelease();

	return pNode;
}

void CC3PODNode::populateFrom( CC3PODNode* another )
{
	super::populateFrom( another );

	_podIndex = another->getPodIndex();
	_podContentIndex = another->getPodContentIndex();
	_podParentIndex = another->getPodParentIndex();
}

CCObject* CC3PODNode::copyWithZone( CCZone* zone )
{
	CC3PODNode* pVal = new CC3PODNode;
	pVal->init();
	pVal->populateFrom( this );
	pVal->addCopiesOfChildrenFrom( this );

	return pVal;
}

std::string CC3PODNode::description()
{
	return CC3String::stringWithFormat( (char*)"%s (POD index: %d)", super::description().c_str(), _podIndex );
}

void CC3PODNode::addAnimationFromPODFile( const std::string& podFilePath, GLuint trackID )
{
	addAnimationInResource( CC3PODResource::resourceFromFile(podFilePath), trackID );
}

GLuint CC3PODNode::addAnimationFromPODFile( const std::string& podFilePath )
{
	return addAnimationInResource( CC3PODResource::resourceFromFile(podFilePath) );
}

void CC3PODNode::addContentFromPODFile( const std::string& aFilepath )
{
	addChild( CC3PODResourceNode::nodeFromFile(aFilepath) );
}

void CC3PODNode::addContentFromPODFile( const std::string& aFilepath, const std::string& aName )
{
	addChild( CC3PODResourceNode::nodeWithName( aName, aFilepath ) );
}

CC3PODNodeAnimation::CC3PODNodeAnimation()
{
	_animatedLocations = NULL;
	_animatedLocationIndices = NULL;
	_animatedQuaternions = NULL;
	_animatedQuaternionsIndices = NULL;
	_animatedScales = NULL;
	_animatedScaleIndices = NULL;
}

CC3PODNodeAnimation::~CC3PODNodeAnimation()
{
	CC_SAFE_FREE(_animatedLocations);
	CC_SAFE_FREE(_animatedLocationIndices);
	CC_SAFE_FREE(_animatedQuaternions);
	CC_SAFE_FREE(_animatedQuaternionsIndices);
	CC_SAFE_FREE(_animatedScales);
	CC_SAFE_FREE(_animatedScaleIndices);
}

// For each type of animation content, this instance assumes responsiblity for managing
// the memory of 
void CC3PODNodeAnimation::initFromSPODNode( PODStructPtr pSPODNode, GLuint numFrames )
{
	super::initWithFrameCount( numFrames );
		
	// Start with no animation
	_animatedLocations = _animatedQuaternions = _animatedScales = NULL;
	_animatedLocationIndices = _animatedQuaternionsIndices = _animatedScaleIndices = NULL;
		
	SPODNode* psn = (SPODNode*)pSPODNode;
		
	if (psn->pfAnimPosition && (psn->nAnimFlags & ePODHasPositionAni)) {
		_animatedLocations = psn->pfAnimPosition;
		_animatedLocationIndices = psn->pnAnimPositionIdx;
		psn->pfAnimPosition = NULL;		// Clear reference so SPODNode won't try to free it.
		psn->pnAnimPositionIdx = NULL;	// Clear reference so SPODNode won't try to free it.
	}
		
	if (psn->pfAnimRotation && (psn->nAnimFlags & ePODHasRotationAni)) {
		_animatedQuaternions = psn->pfAnimRotation;
		_animatedQuaternionsIndices = psn->pnAnimRotationIdx;
		psn->pfAnimRotation = NULL;		// Clear reference so SPODNode won't try to free it.
		psn->pnAnimRotationIdx = NULL;	// Clear reference so SPODNode won't try to free it.
	}
		
	if (psn->pfAnimScale && (psn->nAnimFlags & ePODHasScaleAni)) {
		_animatedScales = psn->pfAnimScale;
		_animatedScaleIndices = psn->pnAnimScaleIdx;
		psn->pfAnimScale = NULL;		// Clear reference so SPODNode won't try to free it.
		psn->pnAnimScaleIdx = NULL;		// Clear reference so SPODNode won't try to free it.
	}
}

CC3PODNodeAnimation* CC3PODNodeAnimation::animationFromSPODNode( PODStructPtr pSPODNode, GLuint numFrames )
{
	CC3PODNodeAnimation* pNodeAnim = new CC3PODNodeAnimation;
	pNodeAnim->initFromSPODNode( pSPODNode, numFrames );
	pNodeAnim->autorelease();

	return pNodeAnim;
}

bool CC3PODNodeAnimation::sPODNodeDoesContainAnimation( PODStructPtr pSPODNode )
{
	SPODNode* psn = (SPODNode*)pSPODNode;
	return (bool)(psn->nAnimFlags & (ePODHasPositionAni | ePODHasRotationAni | ePODHasScaleAni));
}

bool CC3PODNodeAnimation::isAnimatingLocation()
{
	return _animatedLocations != NULL; 
}

bool CC3PODNodeAnimation::isAnimatingQuaternion()
{
	return _animatedQuaternions != NULL; 
}

bool CC3PODNodeAnimation::isAnimatingScale()
{
	return _animatedScales != NULL; 
}

CC3Vector CC3PODNodeAnimation::getLocationAtFrame( GLuint frameIndex )
{
	frameIndex = MIN(frameIndex, _frameCount - 1);
	GLint currFrameOffset = _animatedLocationIndices
								? _animatedLocationIndices[frameIndex]
								: (frameIndex * kPODAnimationLocationStride);
	return *(CC3Vector*)&_animatedLocations[currFrameOffset];
}

CC3Quaternion CC3PODNodeAnimation::getQuaternionAtFrame( GLuint frameIndex )
{
	frameIndex = MIN(frameIndex, _frameCount - 1);
	GLint currFrameOffset = _animatedQuaternionsIndices
								? _animatedQuaternionsIndices[frameIndex]
								: (frameIndex * kPODAnimationQuaternionStride);
	return *(CC3Quaternion*)&_animatedQuaternions[currFrameOffset];
}

CC3Vector CC3PODNodeAnimation::getScaleAtFrame( GLuint frameIndex )
{
	frameIndex = MIN(frameIndex, _frameCount - 1);
	GLint currFrameOffset = _animatedScaleIndices
								? _animatedScaleIndices[frameIndex]
								: (frameIndex * kPODAnimationScaleStride);
	return *(CC3Vector*)&_animatedScales[currFrameOffset];
}

NS_COCOS3D_END
