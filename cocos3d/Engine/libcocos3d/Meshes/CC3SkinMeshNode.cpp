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
	m_skinSections = NULL;
	m_pSkeletalTransformMatrix = NULL;
	m_pSkeletalTransformMatrixInverted = NULL;
	m_deformedFaces = NULL;
}

CC3SkinMeshNode::~CC3SkinMeshNode()
{
	CC_SAFE_RELEASE( m_skinSections );
	CC_SAFE_RELEASE( m_pSkeletalTransformMatrix );
	CC_SAFE_RELEASE( m_pSkeletalTransformMatrixInverted );
	CC_SAFE_RELEASE( m_deformedFaces );
}

CCArray* CC3SkinMeshNode::getSkinSections()
{
	return m_skinSections;
}

CC3SkinSection* CC3SkinMeshNode::getSkinSectionForVertexIndexAt( GLint index )
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_skinSections, pObj )
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
	CCARRAY_FOREACH( m_skinSections, pObj )
	{
		CC3SkinSection* ss = (CC3SkinSection*)pObj;
		if ( ss->hasSkeleton() )
			return true;
	}

	return false;
}

bool CC3SkinMeshNode::hasRigidSkeleton()
{
	return m_hasRigidSkeleton; 
}

void CC3SkinMeshNode::ensureRigidSkeleton()
{
	m_hasRigidSkeleton = hasSkeleton();
	super::ensureRigidSkeleton();
}

void CC3SkinMeshNode::setShouldCacheFaces( bool shouldCache )
{
	getDeformedFaces()->setShouldCacheFaces( shouldCache );
	super::setShouldCacheFaces( shouldCache );
}

CC3DeformedFaceArray* CC3SkinMeshNode::getDeformedFaces()
{
	if ( !m_deformedFaces ) 
	{
		std::string facesName = CC3String::stringWithFormat( (char*)"%s-DeformedFaces", getName().c_str() );
		setDeformedFaces( CC3DeformedFaceArray::faceArrayWithName( facesName ) );
	}
	return m_deformedFaces;
}

void CC3SkinMeshNode::setDeformedFaces( CC3DeformedFaceArray* aFaceArray )
{
	if (aFaceArray == m_deformedFaces) 
		return;
	
	m_deformedFaces->release();
	m_deformedFaces = aFaceArray;
	m_deformedFaces->retain();

	m_deformedFaces->setNode( this );
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
		m_skinSections = CCArray::create();							// retained
		m_skinSections->retain();
		m_pSkeletalTransformMatrix = CC3AffineMatrix::matrix();		// retained
		m_pSkeletalTransformMatrix->retain();
		m_pSkeletalTransformMatrixInverted = CC3AffineMatrix::matrix();// retained
		m_pSkeletalTransformMatrixInverted->retain();
		m_deformedFaces = NULL;
	}
}

void CC3SkinMeshNode::populateFrom( CC3SkinMeshNode* another )
{
	super::populateFrom( another );

	// The deformedFaces instance is not copied, since the deformed faces
	// are different for each mesh node and is created lazily if needed.
	// The skeletal transform matrices are not copied

	m_skinSections->removeAllObjects();
	CCArray* otherSkinSections = another->getSkinSections();
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( otherSkinSections, pObj )
	{
		CC3SkinSection* ss = (CC3SkinSection*)pObj;
		m_skinSections->addObject( ss->copyForNode(this)->autorelease() );
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
	CCARRAY_FOREACH( m_skinSections, pObj )
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
	m_pSkeletalTransformMatrix->setIsDirty( true );
	m_pSkeletalTransformMatrixInverted->setIsDirty( true );

	if ( m_deformedFaces )
		m_deformedFaces->clearDeformableCaches();
}

CC3Matrix* CC3SkinMeshNode::getSkeletalTransformMatrix()
{
	if (m_pSkeletalTransformMatrix->isDirty()) 
	{
		m_pSkeletalTransformMatrix->populateFrom( getGlobalTransformMatrix() );
		m_pSkeletalTransformMatrix->leftMultiplyBy( getSoftBodyNode()->getGlobalTransformMatrixInverted() );
		m_pSkeletalTransformMatrix->setIsDirty( false );
	}
	return m_pSkeletalTransformMatrix;
}

CC3Matrix* CC3SkinMeshNode::getSkeletalTransformMatrixInverted()
{
	if (m_pSkeletalTransformMatrixInverted->isDirty()) 
	{
		m_pSkeletalTransformMatrixInverted->populateFrom( getSkeletalTransformMatrix() );
		m_pSkeletalTransformMatrixInverted->invert();
		m_pSkeletalTransformMatrixInverted->setIsDirty( false );
	}
	return m_pSkeletalTransformMatrixInverted;
}

void CC3SkinMeshNode::boneWasTransformed( CC3Bone* aBone )
{
	if ( m_deformedFaces )
		m_deformedFaces->clearDeformableCaches(); 
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
	
	m_pMesh->bindWithVisitor( visitor );	// Bind the arrays
	
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_skinSections, pObj )
	{
		CC3SkinSection* ss = (CC3SkinSection*)pObj;
		ss->drawVerticesOfMesh( m_pMesh, visitor );
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
