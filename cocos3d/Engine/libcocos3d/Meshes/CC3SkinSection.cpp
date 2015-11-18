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

NS_COCOS3D_END
