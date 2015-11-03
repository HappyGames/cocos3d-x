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

/** Attaches any contained skin sections to the new skeleton copy under this soft body node. */
void CC3SoftBodyNode::addCopiesOfChildrenFrom( CC3Node* another )
{
	super::addCopiesOfChildrenFrom( another );
	reattachBonesFrom( this );
}

CC3SoftBodyNode* CC3SoftBodyNode::getSoftBodyNode()
{
	return this; 
}

CC3Vector CC3SoftBodyNode::getSkeletalScale()
{
	return CC3Vector::kCC3VectorUnitCube; 
}

CC3SoftBodyNode* CC3SoftBodyNode::nodeWithName( const std::string& aName )
{
	CC3SoftBodyNode* pNode = new CC3SoftBodyNode;
	pNode->initWithName( aName );
	pNode->autorelease();

	return pNode;
}

CCObject* CC3SoftBodyNode::copyWithZone( CCZone* zone )
{
	CC3SoftBodyNode* pNode = new CC3SoftBodyNode;
	pNode->init();
	pNode->populateFrom( this );
	pNode->addCopiesOfChildrenFrom( this );

	return pNode;
}

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

CC3SkinSection::CC3SkinSection()
{
	_node = NULL;				// weak reference
	_skinnedBones = NULL;
}

CC3SkinSection::~CC3SkinSection()
{
	_node = NULL;
	CC_SAFE_RELEASE( _skinnedBones );
}

bool CC3SkinSection::hasSkeleton()
{
	return getBoneCount() > 0; 
}

GLuint CC3SkinSection::getBoneCount()
{
	return (GLuint)_skinnedBones->count(); 
}

CCArray* CC3SkinSection::getBones()
{
	CCArray* bones = CCArray::create();

	CCObject* pObj = NULL;
	CCARRAY_FOREACH( _skinnedBones, pObj )
	{
		CC3SkinnedBone* sb = (CC3SkinnedBone*)pObj;
		if ( sb )
			bones->addObject( sb->getBone() );
	}
	
	return bones;
}

CC3Bone* CC3SkinSection::getBoneAt( GLuint boneIdx )
{
	return ((CC3SkinnedBone*)(_skinnedBones->objectAtIndex(boneIdx)))->getBone();
}

int CC3SkinSection::getVertexStart()
{
	return _vertexStart;
}

int CC3SkinSection::getVertexCount()
{
	return _vertexCount;
}

void CC3SkinSection::addBone( CC3Bone* aBone )
{
	_skinnedBones->addObject( CC3SkinnedBone::skinnedBoneWithSkin( _node, aBone ) );
}

bool CC3SkinSection::hasRigidSkeleton()
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( _skinnedBones, pObj )
	{
		CC3SkinnedBone* sb = (CC3SkinnedBone*)pObj;
		if ( !sb->getBone()->getSkeletalTransformMatrix()->isRigid() )
			return false;
	}
	
	return (_skinnedBones->count() > 0);	// YES if all bones returned YES, but NO if no bones.
}

bool CC3SkinSection::containsVertexIndex( GLint aVertexIndex )
{
	return (aVertexIndex >= _vertexStart) && (aVertexIndex < _vertexStart + _vertexCount);
}

CC3Vector CC3SkinSection::getDeformedVertexLocationAt( GLuint vtxIdx )
{
	CC3Mesh* skinMesh = _node->getMesh();
	
	// The locations of this vertex before and after deformation.
	// The latter is to be calculated and returned by this method.
	CC3Vector restLoc = skinMesh->getVertexLocationAt( vtxIdx );
	CC3Vector defLoc = CC3Vector::kCC3VectorZero;
	
	// Calc the weighted sum of the deformation contributed by each bone to this vertex.
	// Iterate through the bones associated with this vertex.
	GLuint vuCnt = skinMesh->getVertexBoneCount();
	for (GLuint vuIdx = 0; vuIdx < vuCnt; vuIdx++) 
	{	
		// Get a bone and its weighting for this vertex.
		GLfloat vtxWt = skinMesh->getVertexWeightForBoneInfluence( vuIdx, vtxIdx );
		GLuint vtxBoneIdx = skinMesh->getVertexBoneIndexForBoneInfluence( vuIdx, vtxIdx );
		CC3SkinnedBone* skinnedBone = ((CC3SkinnedBone*)(_skinnedBones->objectAtIndex(vtxBoneIdx)));
		
		// Use the bone to deform the vertex, apply the weighting for this bone,
		// and add to the summed location.
		CC3Vector boneDefLoc = skinnedBone->getTransformMatrix()->transformLocation( restLoc );
		CC3Vector wtdBoneDefLoc = boneDefLoc.scaleUniform( vtxWt );
		defLoc = defLoc.add( wtdBoneDefLoc );

		/*LogTrace(@"%@ vu: %i, bone at %i, weight %.3f transforming vertex at %i: %@ to %@ to wtd: %@ to sum: %@",
				 self, vuIdx, vtxBoneIdx, vtxWt, vtxIdx,
				 NSStringFromCC3Vector(restLoc),
				 NSStringFromCC3Vector(boneDefLoc),
				 NSStringFromCC3Vector(wtdBoneDefLoc),
				 NSStringFromCC3Vector(defLoc));*/
	}
	return defLoc;
}

void CC3SkinSection::init()
{ 
	return initForNode( NULL ); 
}

void CC3SkinSection::initForNode( CC3SkinMeshNode* aNode )
{
	_node = aNode;							// weak reference
	_skinnedBones = CCArray::create();		// retained
	_skinnedBones->retain();
	_vertexStart = 0;
	_vertexCount = 0;
}

CC3SkinSection* CC3SkinSection::skinSectionForNode( CC3SkinMeshNode* aNode )
{
	CC3SkinSection* pSection = new CC3SkinSection;
	pSection->initForNode( aNode );
	pSection->autorelease();

	return pSection;
}

// Extract the old bones into an array, and for each, look for the
// bone with the same name as a descendant of the specified node.
void CC3SkinSection::reattachBonesFrom( CC3Node* aNode )
{
	CCArray* oldBones = getBones();
	_skinnedBones->removeAllObjects();

	CCObject* pObj = NULL;
	CCARRAY_FOREACH( oldBones, pObj )
	{
		CC3Bone* ob = (CC3Bone*)pObj;
		addBone( (CC3Bone*)(aNode->getNodeNamed( ob->getName().c_str() )) );
	}
}

void CC3SkinSection::populateFrom( CC3SkinSection* another )
{
	_vertexStart = another->getVertexStart();
	_vertexCount = another->getVertexCount();

	// Each bone is retained but not copied, and will be swapped for copied bones via reattachBonesFrom:
	_skinnedBones->removeAllObjects();
	CCArray* otherBones = another->getBones();

	CCObject* pObj = NULL;
	CCARRAY_FOREACH( otherBones, pObj )
	{
		CC3Bone* ob = (CC3Bone*)pObj;
		addBone( ob );
	}
}

CCObject* CC3SkinSection::copyWithZone( CCZone* zone )
{
	return copyForNode( NULL, zone ); 
}

CCObject* CC3SkinSection::copyForNode( CC3SkinMeshNode* aNode )
{
	return copyForNode( aNode, NULL ); 
}

CCObject* CC3SkinSection::copyForNode( CC3SkinMeshNode* aNode, CCZone* zone )
{
	CC3SkinSection* aCopy = new CC3SkinSection;
	aCopy->initForNode( aNode );
	aCopy->populateFrom( this );
	return aCopy;
}

void CC3SkinSection::drawVerticesOfMesh( CC3Mesh* mesh, CC3NodeDrawingVisitor* visitor )
{
	visitor->setCurrentSkinSection( this );
	if ( mesh != NULL )
		mesh->drawVerticesFrom( _vertexStart, _vertexCount, visitor );
}

CC3Matrix* CC3SkinSection::getTransformMatrixForBoneAt( GLuint boneIdx )
{
	return ((CC3SkinnedBone*)(_skinnedBones->objectAtIndex(boneIdx)))->getTransformMatrix();
}

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
	//LogTrace(@"%@ with global scale %@ and rest pose %@ %@ inverted to %@",
	//		 self, NSStringFromCC3Vector(self.globalScale), _skeletalTransformMatrix,
	//		 (_restPoseSkeletalTransformMatrixInverted.isRigid ? @"rigidly" : @"adjoint"), _restPoseSkeletalTransformMatrixInverted);
	//LogTrace(@"Validating right multiply: %@ \nvalidating left multiply: %@",
	//		 [CC3AffineMatrix matrixByMultiplying: _skeletalTransformMatrix by: _restPoseSkeletalTransformMatrixInverted],
	//		 [CC3AffineMatrix matrixByMultiplying: _restPoseSkeletalTransformMatrixInverted by: _skeletalTransformMatrix]);
}

CC3SkinnedBone::CC3SkinnedBone()
{
	_skinNode = NULL;
	_bone = NULL;
	_transformMatrix = NULL;
}

CC3SkinnedBone::~CC3SkinnedBone()
{
	if ( _skinNode )
	{
		_skinNode->removeTransformListener( this );
		_skinNode = NULL;								// weak reference
	}

	if ( _bone )
	{
		_bone->removeTransformListener( this );
		_bone = NULL;									// weak reference
	}

	CC_SAFE_RELEASE( _transformMatrix );
}

CC3Bone* CC3SkinnedBone::getBone()
{
	return _bone;
}


void CC3SkinnedBone::markTransformDirty()
{
	if ( _transformMatrix )
		_transformMatrix->setIsDirty( true );
}

CC3Matrix* CC3SkinnedBone::getTransformMatrix()
{
	if ( _transformMatrix->isDirty() ) 
	{
		_transformMatrix->populateFrom( _skinNode->getSkeletalTransformMatrixInverted() );
		_transformMatrix->multiplyBy( _bone->getSkeletalTransformMatrix() );
		_transformMatrix->multiplyBy( _bone->getRestPoseSkeletalTransformMatrixInverted() );
		_transformMatrix->multiplyBy( _skinNode->getSkeletalTransformMatrix() );
		_transformMatrix->setIsDirty( false );
	}
	return _transformMatrix;
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

	_skinNode = aNode;							// weak reference
	_skinNode->addTransformListener( this );

	_bone = aBone;								// weak reference
	_bone->addTransformListener( this );

	_transformMatrix = CC3AffineMatrix::matrix();	// retained
	_transformMatrix->retain();
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
	if (aNode == _bone)
		_skinNode->boneWasTransformed( _bone );
}

/**
 * If either of the nodes to whom I have registered as a
 * listener disappears before I do, clear the reference to it.
 */
void CC3SkinnedBone::nodeWasDestroyed( CC3Node* aNode )
{
	if (aNode == _skinNode) 
		_skinNode = NULL;	// weak reference
	
	if (aNode == _bone) 
		_bone = NULL;		// weak reference
}

CC3DeformedFaceArray::CC3DeformedFaceArray()
{
	_node = NULL;
}

CC3DeformedFaceArray::~CC3DeformedFaceArray()
{
	_node = NULL;
	deallocateDeformedVertexLocations();
}

CC3SkinMeshNode* CC3DeformedFaceArray::getNode()
{
	return _node;
}

void CC3DeformedFaceArray::setNode( CC3SkinMeshNode* aNode )
{
	_node = aNode;								// weak reference
	setMesh( aNode->getMesh() );
	deallocateDeformedVertexLocations();
}

void CC3DeformedFaceArray::setShouldCacheFaces( bool shouldCache )
{
	super::setShouldCacheFaces( shouldCache );
	if ( !_shouldCacheFaces ) 
		deallocateDeformedVertexLocations();
}

void CC3DeformedFaceArray::clearDeformableCaches()
{
	markCentersDirty();
	markNormalsDirty();
	markPlanesDirty();
	markDeformedVertexLocationsDirty();
}

GLuint CC3DeformedFaceArray::getVertexCount()
{
	return _mesh ? _mesh->getVertexCount() : 0;
}

CC3Face CC3DeformedFaceArray::getFaceAt( GLuint faceIndex )
{
	CC3FaceIndices faceIndices = _node->getFaceIndicesAt( faceIndex );
	if (_shouldCacheFaces) 
	{
		CC3Vector* vtxLocs = getDeformedVertexLocations();
		return CC3Face(vtxLocs[faceIndices.vertices[0]],
						   vtxLocs[faceIndices.vertices[1]],
						   vtxLocs[faceIndices.vertices[2]]);
	} 
	else 
	{
		CC3SkinSection* ss = _node->getSkinSectionForFaceIndex( faceIndex );
		return CC3Face(ss->getDeformedVertexLocationAt( faceIndices.vertices[0] ),
						   ss->getDeformedVertexLocationAt( faceIndices.vertices[1] ),
						   ss->getDeformedVertexLocationAt( faceIndices.vertices[2] ));
	}
}

void CC3DeformedFaceArray::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	{
		_node = NULL;
		_deformedVertexLocations = NULL;
		_deformedVertexLocationsAreRetained = false;
		_deformedVertexLocationsAreDirty = true;
	}
}

// Template method that populates this instance from the specified other instance.
// This method is invoked automatically during object copying via the copyWithZone: method.
void CC3DeformedFaceArray::populateFrom( CC3DeformedFaceArray* another )
{
	super::populateFrom( another );
	
	_node = another->getNode();		// weak reference
	
	// If deformed vertex locations should be retained, allocate memory and copy the data over.
	deallocateDeformedVertexLocations();
	if (another->_deformedVertexLocationsAreRetained) 
	{
		allocateDeformedVertexLocations();
		memcpy(_deformedVertexLocations, another->getDeformedVertexLocations(), (getVertexCount() * sizeof(CC3Vector)));
	} else {
		_deformedVertexLocations = another->getDeformedVertexLocations();
	}
	_deformedVertexLocationsAreDirty = another->_deformedVertexLocationsAreDirty;
}

CCObject* CC3DeformedFaceArray::copyWithZone( CCZone* zone )
{
	CC3DeformedFaceArray* pVal = new CC3DeformedFaceArray;
	pVal->init();
	pVal->populateFrom( this );

	return pVal;
}

CC3Vector* CC3DeformedFaceArray::getDeformedVertexLocations()
{
	if (_deformedVertexLocationsAreDirty || !_deformedVertexLocations)
		populateDeformedVertexLocations();

	return _deformedVertexLocations;
}

void CC3DeformedFaceArray::setDeformedVertexLocations( CC3Vector* vtxLocs )
{
	deallocateDeformedVertexLocations();			// Safely disposes existing vertices
	_deformedVertexLocations = vtxLocs;
}

CC3Vector CC3DeformedFaceArray::getDeformedVertexLocationAt( GLuint vertexIndex, GLuint faceIndex )
{
	if (_shouldCacheFaces) 
		return getDeformedVertexLocations()[vertexIndex];

	return _node->getSkinSectionForFaceIndex(faceIndex)->getDeformedVertexLocationAt( vertexIndex );
}

CC3Vector* CC3DeformedFaceArray::allocateDeformedVertexLocations()
{
	deallocateDeformedVertexLocations();
	GLuint vtxCount = getVertexCount();
	if (vtxCount) 
	{
		_deformedVertexLocations = (CC3Vector*)calloc(vtxCount, sizeof(CC3Vector));
		_deformedVertexLocationsAreRetained = true;
		CC3_TRACE("CC3DeformedFaceArray allocated space for %d deformed vertex locations", vtxCount);
	}
	return _deformedVertexLocations;
}

void CC3DeformedFaceArray::deallocateDeformedVertexLocations()
{
	if (_deformedVertexLocationsAreRetained && _deformedVertexLocations) 
	{
		free(_deformedVertexLocations);
		_deformedVertexLocations = NULL;
		_deformedVertexLocationsAreRetained = false;
		CC3_TRACE("CC3DeformedFaceArray deallocated %d previously allocated deformed vertex locations", getVertexCount());
	}
}

void CC3DeformedFaceArray::populateDeformedVertexLocations()
{
	CC3_TRACE("CC3DeformedFaceArray populating %d deformed vertex locations", getVertexCount());
	if ( !_deformedVertexLocations )
		allocateDeformedVertexLocations();
	
	// Mark all the location vectors in the cached array as unset, so we can keep
	// track of which vertices have been set, as we iterate through the mesh vertices.
	GLuint vtxCount = getVertexCount();
	for (GLuint vtxIdx = 0; vtxIdx < vtxCount; vtxIdx++)
		_deformedVertexLocations[vtxIdx] = CC3Vector::kCC3VectorNull;

	// Determine whether the mesh is indexed.
	// If it is, we iterate through the indexes.
	// If it isn't, we iterate through the vertices.
	GLuint vtxIdxCount = _mesh->getVertexIndexCount();
	bool meshIsIndexed = (vtxIdxCount > 0);
	if (!meshIsIndexed) 
		vtxIdxCount = vtxCount;

	// The skin sections are assigned to contiguous ranges of vertex indices.
	// We can avoid looking up the skin section for each vertex by assuming that they
	// will appear in groups, check the current skin section for each vertex index,
	// and only change when needed.
	
	// Get the skin section of the first vertex
	CC3SkinSection* ss = _node->getSkinSectionForVertexIndexAt(0);
	for (GLuint vtxIdxPos = 0; vtxIdxPos < vtxIdxCount; vtxIdxPos++) 
	{
		// Make sure the current skin section deforms this vertex, otherwise get the correct one
		if ( !ss->containsVertexIndex(vtxIdxPos) )
		{
			ss = _node->getSkinSectionForVertexIndexAt( vtxIdxPos );
			//LogTrace(@"Selecting %@ for vertex at %i", ss, vtxIdxPos);
		}
		
		// Get the actual vertex index. If the mesh is indexed, we look it up, from the vertex
		// index position. If the mesh is not indexed, then it IS the vertex index position.
		GLuint vtxIdx = meshIsIndexed ? _mesh->getVertexIndexAt(vtxIdxPos) : vtxIdxPos;
		
		// If the cached vertex location has not yet been set, use the skin section to
		// deform the vertex location at the current index, and set it into the cache array.
		if ( _deformedVertexLocations[vtxIdx].isNull() )
		{
			_deformedVertexLocations[vtxIdx] = ss->getDeformedVertexLocationAt( vtxIdx );
			
// 			LogTrace(@"Setting deformed vertex %i to %@", vtxIdx,
// 						  NSStringFromCC3Vector(_deformedVertexLocations[vtxIdx]));
		}
	}
	_deformedVertexLocationsAreDirty = false;
}

void CC3DeformedFaceArray::markDeformedVertexLocationsDirty()
{
	_deformedVertexLocationsAreDirty = true; 
}

CC3DeformedFaceArray* CC3DeformedFaceArray::faceArrayWithName( const std::string& aName )
{
	CC3DeformedFaceArray* pFaceArray = new CC3DeformedFaceArray;
	pFaceArray->initWithName( aName.c_str() );
	pFaceArray->autorelease();

	return pFaceArray;
}

NS_COCOS3D_END
