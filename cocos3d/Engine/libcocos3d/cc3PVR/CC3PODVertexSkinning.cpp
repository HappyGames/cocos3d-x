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

CC3PODSkinMeshNode* CC3PODSkinMeshNode::nodeAtIndex( GLint aPODIndex, CC3PODResource* aPODRez )
{
	CC3PODSkinMeshNode* pNode = new CC3PODSkinMeshNode;
	pNode->initAtIndex( aPODIndex, aPODRez );
	pNode->autorelease();

	return pNode;
}

GLint CC3PODSkinMeshNode::getPodIndex()
{
	return _podIndex; 
}

void CC3PODSkinMeshNode::setPodIndex( GLint aPODIndex )
{
	_podIndex = aPODIndex; 
}

GLint CC3PODSkinMeshNode::getPodContentIndex()
{
	return _podContentIndex; 
}

void CC3PODSkinMeshNode::setPodContentIndex( GLint aPODIndex )
{
	_podContentIndex = aPODIndex; 
}

GLint CC3PODSkinMeshNode::getPodParentIndex()
{
	return _podParentIndex; 
}

void CC3PODSkinMeshNode::setPodParentIndex( GLint aPODIndex )
{
	_podParentIndex = aPODIndex; 
}

GLint CC3PODSkinMeshNode::getPodMaterialIndex()
{
	return _podMaterialIndex; 
}

void CC3PODSkinMeshNode::setPodMaterialIndex( GLint aPODIndex )
{
	_podMaterialIndex = aPODIndex; 
}

PODStructPtr CC3PODSkinMeshNode::getNodePODStructAtIndex( GLuint aPODIndex, CC3PODResource* aPODRez )
{
    return aPODRez->getMeshNodePODStructAtIndex( aPODIndex );
}

/** 
 * Overridden to verify that the mesh is not constructed from triangle strips,
 * which are not compatible with the way that skin sections render mesh sections.
 */
void CC3PODSkinMeshNode::setMesh( CC3Mesh* mesh )
{
	CCAssert(mesh->getDrawingMode() != GL_TRIANGLE_STRIP,
			  "%CC3PODSkinMeshNode does not support the use of triangle strips."
			  " Vertex-skinned meshes must be constructed from triangles.");
	super::setMesh( mesh );
}

/** Overridden to extract the bone batches from the associated POD mesh structure */
void CC3PODSkinMeshNode::initAtIndex( GLint aPODIndex, CC3PODResource* aPODRez )
{
    init();
    
    setPodIndex( aPODIndex );
    
    SPODNode* psn = (SPODNode*)getNodePODStructAtIndex( aPODIndex, aPODRez );
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
    
    SPODNode* pmn = (SPODNode*)getNodePODStructAtIndex( aPODIndex, aPODRez );
    
    // If this node has a mesh, build it
    if ( getPodContentIndex() >= 0 )
        setMesh( aPODRez->getMeshAtIndex( getPodContentIndex() ) );
    
    // If this node has a material, build it
    setPodMaterialIndex( pmn->nIdxMaterial );
    if ( getPodMaterialIndex() >= 0)
        setMaterial( aPODRez->getMaterialAtIndex( getPodMaterialIndex() ) );
    
	if ( getPodContentIndex() >= 0 )
	{
		SPODMesh* psm = (SPODMesh*)aPODRez->getMeshPODStructAtIndex( getPodContentIndex() );
		GLint batchCount = psm->sBoneBatches.nBatchCnt;
		for (GLint batchIndex = 0; batchIndex < batchCount; batchIndex++) 
		{
			m_skinSections->addObject( CC3PODSkinSection::skinSectionFromBatchAtIndex( batchIndex, psm, this ) );
		}
	}
}

void CC3PODSkinMeshNode::populateFrom( CC3PODSkinMeshNode* another )
{
	super::populateFrom( another );
	
	_podIndex = another->getPodIndex();
	_podContentIndex = another->getPodContentIndex();
	_podParentIndex = another->getPodParentIndex();
	_podMaterialIndex = another->getPodMaterialIndex();
}

CCObject* CC3PODSkinMeshNode::copyWithZone( CCZone* zone )
{
	CC3PODSkinMeshNode* pVal = new CC3PODSkinMeshNode;
	pVal->init();
	pVal->populateFrom( this );
	pVal->addCopiesOfChildrenFrom( this );

	return pVal;
}

/** Link the nodes in the bone batches. */
void CC3PODSkinMeshNode::linkToPODNodes( CCArray* nodeArray )
{
    int parentIndex = getPodParentIndex();
    if ( parentIndex >= 0 )
    {
        //LogTrace(@"Linking %@ with parent index %i", self, self.podParentIndex);
        CC3Node* parentNode = (CC3Node*)nodeArray->objectAtIndex( parentIndex );
        parentNode->addChild( this );
    }
    
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_skinSections, pObj )
	{
		CC3PODSkinSection* skinSctn = (CC3PODSkinSection*)pObj;
		skinSctn->linkToPODNodes( nodeArray );
	}
}

std::string CC3PODSkinMeshNode::description()
{
	return CC3String::stringWithFormat( (char*)"%s (POD index: %d)", super::description().c_str(), _podIndex );
}

void CC3PODSkinSection::init()
{
	super::init();
	_podBoneCount = 0;
	_podBoneNodeIndices = NULL;
}

void CC3PODSkinSection::initFromBatchAtIndex( GLint aBatchIndex, PODStructPtr aSPODMesh, CC3SkinMeshNode* aNode )
{
	SPODMesh* psm = (SPODMesh*)aSPODMesh;
	initForNode( aNode );
	CPVRTBoneBatches* pBatches = &psm->sBoneBatches;
	GLint batchCount = pBatches->nBatchCnt;

	GLint currFaceOffset = pBatches->pnBatchOffset[aBatchIndex];
	GLint nextFaceOffset = (aBatchIndex < batchCount - 1)
		? pBatches->pnBatchOffset[aBatchIndex + 1]
	: psm->nNumFaces;

	if ( aNode->getMesh() )
	{
		m_vertexStart = aNode->getMesh()->getVertexIndexCountFromFaceCount( currFaceOffset );
		m_vertexCount =  aNode->getMesh()->getVertexIndexCountFromFaceCount( nextFaceOffset - currFaceOffset );
	}

	_podBoneCount = pBatches->pnBatchBoneCnt[aBatchIndex];
	_podBoneNodeIndices = &(pBatches->pnBatches[aBatchIndex * pBatches->nBatchBoneMax]);
}

CC3PODSkinSection* CC3PODSkinSection::skinSectionFromBatchAtIndex( GLint aBatchIndex, PODStructPtr aSPODMesh, CC3SkinMeshNode* aNode )
{
	CC3PODSkinSection* pStcn = new CC3PODSkinSection;
	pStcn->initFromBatchAtIndex( aBatchIndex, aSPODMesh, aNode );
	pStcn->autorelease();

	return pStcn;
}

void CC3PODSkinSection::linkToPODNodes( CCArray* nodeArray )
{
	for (GLuint boneNum = 0; boneNum < _podBoneCount; boneNum++) 
	{
		GLint boneIndex = _podBoneNodeIndices[boneNum];
		CC3Bone* boneNode = (CC3Bone*)nodeArray->objectAtIndex(boneIndex);
		CC3_TRACE( "Adding bone node %s at index %d to CC3PODSkinSection", boneNode->getName().c_str(), boneIndex );
		addBone( boneNode );
	}
	_podBoneNodeIndices = NULL;		// Remove reference since this array will be released
	_podBoneCount = 0;
}

std::string CC3PODSkinSection::fullDescription()
{
	/*return [NSString stringWithFormat: @"%@ from original bone count %i of indices at %p",
			[super fullDescription], _podBoneCount, _podBoneNodeIndices];*/
	return "CC3PODSkinSection";
}


GLint CC3PODBone::getPodIndex()
{
	return _podIndex; 
}

void CC3PODBone::setPodIndex( GLint aPODIndex )
{
	_podIndex = aPODIndex; 
}

GLint CC3PODBone::getPodContentIndex()
{
	return _podContentIndex; 
}

void CC3PODBone::setPodContentIndex( GLint aPODIndex )
{
	_podContentIndex = aPODIndex; 
}

GLint CC3PODBone::getPodParentIndex()
{
	return _podParentIndex; 
}

void CC3PODBone::setPodParentIndex( GLint aPODIndex )
{
	_podParentIndex = aPODIndex; 
}

void CC3PODBone::populateFrom( CC3PODBone* another )
{
	super::populateFrom( another );
	
	_podIndex = another->getPodIndex();
	_podContentIndex = another->getPodContentIndex();
	_podParentIndex = another->getPodParentIndex();
}

CCObject* CC3PODBone::copyWithZone( CCZone* zone )
{
	CC3PODBone* pVal = new CC3PODBone;
	pVal->init();
	pVal->populateFrom( this );
	pVal->addCopiesOfChildrenFrom( this );

	return pVal;
}

std::string CC3PODBone::description()
{
	return CC3String::stringWithFormat( (char*)"%s (POD index: %d)", super::description().c_str(), _podIndex );
}

void CC3PODBone::linkToPODNodes( CCArray *nodeArray )
{
    int parentIndex = getPodParentIndex();
    if ( parentIndex >= 0 )
    {
        //LogTrace(@"Linking %@ with parent index %i", self, self.podParentIndex);
        CC3Node* parentNode = (CC3Node*)nodeArray->objectAtIndex( parentIndex );
        parentNode->addChild( this );
    }
}

void CC3PODBone::initAtIndex(GLint aPODIndex, cocos3d::CC3PODResource *aPODRez )
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

CC3PODBone* CC3PODBone::nodeAtIndex( GLint aPODIndex, CC3PODResource* aPODRez )
{
	CC3PODBone* pBone = new CC3PODBone;
    pBone->initAtIndex( aPODIndex, aPODRez );
	pBone->autorelease();

	return pBone;
}

NS_COCOS3D_END
