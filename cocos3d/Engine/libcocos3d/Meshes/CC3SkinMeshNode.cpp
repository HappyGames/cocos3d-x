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

CC3SkinMeshNode::CC3SkinMeshNode()
{
	_skinSections = NULL;
	_skeletalTransformMatrix = NULL;
	_skeletalTransformMatrixInverted = NULL;
	_deformedFaces = NULL;
}

CC3SkinMeshNode::~CC3SkinMeshNode()
{
	CC_SAFE_RELEASE( _skinSections );
	CC_SAFE_RELEASE( _skeletalTransformMatrix );
	CC_SAFE_RELEASE( _skeletalTransformMatrixInverted );
	CC_SAFE_RELEASE( _deformedFaces );
}

CCArray* CC3SkinMeshNode::getSkinSections()
{
	return _skinSections;
}

CC3SkinSection* CC3SkinMeshNode::getSkinSectionForVertexIndexAt( GLint index )
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( _skinSections, pObj )
	{
		CC3SkinSection* ss = (CC3SkinSection*)pObj;
		if ( ss->containsVertexIndex( index ) )
			return ss;
	}
	
	return NULL;
}

CC3SkinSection* CC3SkinMeshNode::getSkinSectionForFaceIndex( GLint faceIndex )
{
	return	getSkinSectionForVertexIndexAt( getVertexIndexCountFromFaceCount( faceIndex ) );
}

bool CC3SkinMeshNode::hasSkeleton()
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( _skinSections, pObj )
	{
		CC3SkinSection* ss = (CC3SkinSection*)pObj;
		if ( ss->hasSkeleton() )
			return true;
	}

	return false;
}

bool CC3SkinMeshNode::hasRigidSkeleton()
{
	return _hasRigidSkeleton; 
}

void CC3SkinMeshNode::ensureRigidSkeleton()
{
	_hasRigidSkeleton = hasSkeleton();
	super::ensureRigidSkeleton();
}

void CC3SkinMeshNode::setShouldCacheFaces( bool shouldCache )
{
	getDeformedFaces()->setShouldCacheFaces( shouldCache );
	super::setShouldCacheFaces( shouldCache );
}

CC3DeformedFaceArray* CC3SkinMeshNode::getDeformedFaces()
{
	if ( !_deformedFaces ) 
	{
		std::string facesName = CC3String::stringWithFormat( (char*)"%s-DeformedFaces", getName().c_str() );
		setDeformedFaces( CC3DeformedFaceArray::faceArrayWithName( facesName ) );
	}
	return _deformedFaces;
}

void CC3SkinMeshNode::setDeformedFaces( CC3DeformedFaceArray* aFaceArray )
{
	if (aFaceArray == _deformedFaces) 
		return;
	
	_deformedFaces->release();
	_deformedFaces = aFaceArray;
	_deformedFaces->retain();

	_deformedFaces->setNode( this );
}

CC3Face CC3SkinMeshNode::getDeformedFaceAt( GLuint faceIndex )
{
	return getDeformedFaces()->getFaceAt( faceIndex ); 
}

CC3Vector CC3SkinMeshNode::getDeformedFaceCenterAt( GLuint faceIndex )
{
	return getDeformedFaces()->getCenterAt( faceIndex ); 
}

CC3Vector CC3SkinMeshNode::getDeformedFaceNormalAt( GLuint faceIndex )
{
	return getDeformedFaces()->getNormalAt( faceIndex ); 
}

CC3Plane CC3SkinMeshNode::getDeformedFacePlaneAt( GLuint faceIndex )
{
	return getDeformedFaces()->getPlaneAt( faceIndex ); 
}

CC3Vector CC3SkinMeshNode::getDeformedVertexLocationAt( GLuint vertexIndex, GLuint faceIndex )
{
	return getDeformedFaces()->getDeformedVertexLocationAt( vertexIndex, faceIndex );
}

void CC3SkinMeshNode::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	{
		_skinSections = CCArray::create();							// retained
		_skinSections->retain();
		_skeletalTransformMatrix = CC3AffineMatrix::matrix();		// retained
		_skeletalTransformMatrix->retain();
		_skeletalTransformMatrixInverted = CC3AffineMatrix::matrix();// retained
		_skeletalTransformMatrixInverted->retain();
		_deformedFaces = NULL;
	}
}

void CC3SkinMeshNode::populateFrom( CC3SkinMeshNode* another )
{
	super::populateFrom( another );

	// The deformedFaces instance is not copied, since the deformed faces
	// are different for each mesh node and is created lazily if needed.
	// The skeletal transform matrices are not copied

	_skinSections->removeAllObjects();
	CCArray* otherSkinSections = another->getSkinSections();
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( otherSkinSections, pObj )
	{
		CC3SkinSection* ss = (CC3SkinSection*)pObj;
		_skinSections->addObject( ss->copyForNode(this)->autorelease() );
	}
}

CCObject* CC3SkinMeshNode::copyWithZone( CCZone* zone )
{
	CC3SkinMeshNode* pVal = new CC3SkinMeshNode;
	pVal->init();
	pVal->populateFrom( this );
	pVal->addCopiesOfChildrenFrom( this );

	return pVal;
}

void CC3SkinMeshNode::reattachBonesFrom( CC3Node* aNode )
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( _skinSections, pObj )
	{
		CC3SkinSection* ss = (CC3SkinSection*)pObj;
		ss->reattachBonesFrom( aNode );
	}
	
	super::reattachBonesFrom( aNode );
}

bool CC3SkinMeshNode::hasSoftBodyContent()
{
	return true; 
}

void CC3SkinMeshNode::markTransformDirty()
{
	super::markTransformDirty();
	_skeletalTransformMatrix->setIsDirty( true );
	_skeletalTransformMatrixInverted->setIsDirty( true );

	if ( _deformedFaces )
		_deformedFaces->clearDeformableCaches();
}

CC3Matrix* CC3SkinMeshNode::getSkeletalTransformMatrix()
{
	if (_skeletalTransformMatrix->isDirty()) 
	{
		_skeletalTransformMatrix->populateFrom( getGlobalTransformMatrix() );
		_skeletalTransformMatrix->leftMultiplyBy( getSoftBodyNode()->getGlobalTransformMatrixInverted() );
		_skeletalTransformMatrix->setIsDirty( false );
	}
	return _skeletalTransformMatrix;
}

CC3Matrix* CC3SkinMeshNode::getSkeletalTransformMatrixInverted()
{
	if (_skeletalTransformMatrixInverted->isDirty()) 
	{
		_skeletalTransformMatrixInverted->populateFrom( getSkeletalTransformMatrix() );
		_skeletalTransformMatrixInverted->invert();
		_skeletalTransformMatrixInverted->setIsDirty( false );
	}
	return _skeletalTransformMatrixInverted;
}

void CC3SkinMeshNode::boneWasTransformed( CC3Bone* aBone )
{
	if ( _deformedFaces )
		_deformedFaces->clearDeformableCaches(); 
}

/**
 * Returns a spherical bounding volume that will be sized to encompass the vertices of the
 * skin mesh in its bind pose. A sphere is used because for many bone-rigged characters, 
 * the bones remain within the sphere determmined by the rest pose.
 */
CC3NodeBoundingVolume* CC3SkinMeshNode::defaultBoundingVolume()
{
	return CC3NodeSphericalBoundingVolume::boundingVolume();
}

/** Overridden to skip auto-creating a bounding volume. */
void CC3SkinMeshNode::createBoundingVolumes()
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		child->createBoundingVolumes();
	}
}

/** Overridden to auto-create a bounding volume. */
void CC3SkinMeshNode::createSkinnedBoundingVolumes()
{
	createBoundingVolume();
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		child->createSkinnedBoundingVolumes();
	}
}

/** Use this bounding volume, then pass along to my descendants. */
void CC3SkinMeshNode::setSkeletalBoundingVolume( CC3NodeBoundingVolume* boundingVolume )
{
	setBoundingVolume( boundingVolume );
	super::setSkeletalBoundingVolume( boundingVolume );
}

/** 
 * Draws the mesh vertices to the GL engine.
 *
 * Enables palette matrices, binds the mesh to the GL engine, delegates to the contained
 * collection of CC3SkinSections to draw the mesh in batches, then disables palette matrices again.
 */
void CC3SkinMeshNode::drawMeshWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	CC3OpenGL* gl = visitor->getGL();
	
	gl->enableMatrixPalette( true );		// Enable the matrix palette
	
	_mesh->bindWithVisitor( visitor );	// Bind the arrays
	
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( _skinSections, pObj )
	{
		CC3SkinSection* ss = (CC3SkinSection*)pObj;
		ss->drawVerticesOfMesh( _mesh, visitor );
	}

	gl->enableMatrixPalette( false );		// We are finished with the matrix pallete so disable it.
}

void CC3SkinMeshNode::addShadowVolumesForLight( CC3Light* aLight )
{
	super::addShadowVolumesForLight( aLight );

	// Retain data required to build shadow volume mesh
	retainVertexBoneIndices();
	retainVertexBoneWeights();
}

NS_COCOS3D_END
