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
#include "CC3PODMeshNode.h"
#include "CC3PVRTModelPOD.h"
#include "CC3PODMaterial.h"
#include "CC3PFXResource.h"
#include "CC3PODResource.h"

NS_COCOS3D_BEGIN

// Subclasses must override to use instance variable.
GLint CC3PODMeshNode::getPodMaterialIndex()
{
	return _podMaterialIndex; 
}

// Subclasses must override to use instance variable.
void CC3PODMeshNode::setPodMaterialIndex( GLint aPODIndex )
{
	_podMaterialIndex = aPODIndex;
}
 
void CC3PODMeshNode::initAtIndex( GLint aPODIndex, CC3PODResource* aPODRez )
{
	super::initAtIndex( aPODIndex, aPODRez );

	SPODNode* pmn = (SPODNode*)getNodePODStructAtIndex( aPODIndex, aPODRez );
	// If this node has a mesh, build it
	if (getPodContentIndex() >= 0) {
		setMesh( aPODRez->getMeshAtIndex( getPodContentIndex() ) );
	}
	// If this node has a material, build it
	setPodMaterialIndex( pmn->nIdxMaterial );
	if (getPodMaterialIndex() >= 0)
		setMaterial( (CC3PODMaterial*)aPODRez->getMaterialAtIndex( getPodMaterialIndex() ) );
}

PODStructPtr CC3PODMeshNode::getNodePODStructAtIndex( GLuint aPODIndex, CC3PODResource* aPODRez )
{
	return aPODRez->getMeshNodePODStructAtIndex(aPODIndex);
}

GLint CC3PODMeshNode::getPodIndex()
{
	return _podIndex; 
}

void CC3PODMeshNode::setPodIndex( GLint aPODIndex )
{
	_podIndex = aPODIndex; 
}

GLint CC3PODMeshNode::getPodContentIndex()
{
	return _podContentIndex; 
}

void CC3PODMeshNode::setPodContentIndex( GLint aPODIndex )
{
	_podContentIndex = aPODIndex; 
}

GLint CC3PODMeshNode::getPodParentIndex()
{
	return _podParentIndex; 
}

void CC3PODMeshNode::setPodParentIndex( GLint aPODIndex )
{
	_podParentIndex = aPODIndex; 
}

void CC3PODMeshNode::populateFrom( CC3PODMeshNode* another )
{
	super::populateFrom( another );

	_podIndex = another->getPodIndex();
	_podContentIndex = another->getPodContentIndex();
	_podParentIndex = another->getPodParentIndex();
	_podMaterialIndex = another->getPodMaterialIndex();
}

CCObject* CC3PODMeshNode::copyWithZone( CCZone* zone )
{
	CC3PODMeshNode* pVal = new CC3PODMeshNode;
	pVal->init();
	pVal->populateFrom( this );
	pVal->addCopiesOfChildrenFrom( this );

	return pVal;
}

void CC3PODMeshNode::setMaterial( CC3PODMaterial* aMaterial )
{
	super::setMaterial( aMaterial );
	if ( aMaterial )
	{
		CC3PFXEffect* pEffect = aMaterial->getPfxEffect();
		if ( pEffect )
			pEffect->populateMeshNode( this );
	}
}

std::string CC3PODMeshNode::description()
{
	return stringWithFormat( (char*)"%s (POD index: %d)", super::description().c_str(), _podIndex );
}

CC3PODMeshNode* CC3PODMeshNode::nodeAtIndex( GLint aPODIndex, CC3PODResource* aPODRez )
{
	CC3PODMeshNode* pNode = new CC3PODMeshNode;
	pNode->initAtIndex( aPODIndex, aPODRez );
	pNode->autorelease();

	return pNode;
}

NS_COCOS3D_END
