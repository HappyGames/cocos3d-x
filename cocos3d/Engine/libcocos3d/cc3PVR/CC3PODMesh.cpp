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

void CC3PODMesh::initAtIndex( GLint aPODIndex, CC3PODResource* aPODRez )
{
    init();
    setPodIndex( aPODIndex );
    
	SPODMesh* psm = (SPODMesh*)aPODRez->getMeshPODStructAtIndex( aPODIndex );
	//LogRez(@"Creating %@ at index %i from: %@", [self class], aPODIndex, NSStringFromSPODMesh(psm));

	setVertexLocations( CC3PODVertexFactory::createVertexLocations( aPODRez, aPODIndex ) );
	setVertexNormals( CC3PODVertexFactory::createVertexNormals( aPODRez, aPODIndex ) );
	setVertexTangents( CC3PODVertexFactory::createVertexTangents( aPODRez, aPODIndex ) );
	setVertexBitangents( CC3PODVertexFactory::createVertexTangents( aPODRez, aPODIndex, true ) );
	setVertexColors( CC3PODVertexFactory::createVertexColors( aPODRez, aPODIndex ) );
	setVertexBoneWeights( CC3PODVertexFactory::createVertexBoneWeights( aPODRez, aPODIndex ) );
	setVertexBoneIndices( CC3PODVertexFactory::createVertexBoneIndices( aPODRez, aPODIndex ) );

	for (GLuint i = 0; i < psm->nNumUVW; i++) 
	{
		CC3VertexTextureCoordinates* texCoords = CC3PODVertexFactory::createVertexTextureCoordinates( aPODRez, aPODIndex, i );
		texCoords->setExpectsVerticallyFlippedTextures( aPODRez->expectsVerticallyFlippedTextures() );
		addTextureCoordinates( texCoords );
	}

	setVertexIndices( CC3PODVertexFactory::createVertexIndices( aPODRez, aPODIndex ) );

	// Once all vertex arrays are populated, if the data is interleaved, mark it as such and
	// swap the reference to the original data within the SPODMesh, so that CC3VertexArray
	// can take over responsibility for managing the data memory allocated by CPVRTModelPOD.
	// This allows CC3VertexArray to release the vertex data from memory once it has been bound
	// to a GL buffer in the graphics hardware, and allows the CPVRTModelPOD to be released
	// from memory without it removing the vertex data in the process. We can't just NULL the
	// interleaved pointer reference, because a NULL indicates to CPVRTModelPOD that the data
	// is contained within the individual vertex arrays, and it will try to free those instead.
	// So, we create a "dummy" memory allocation for CPVRTModelPOD to free when it needs to.
	// The original pointer is now being managed by the CC3VertexLocations instance.
	if ( psm->pInterleaved != NULL ) 
	{
		m_shouldInterleaveVertices = true;
		psm->pInterleaved = (PVRTuint8*)calloc(1, sizeof(PVRTuint8));
	}
	else 
	{
		m_shouldInterleaveVertices = false;
	}
}

CC3PODMesh* CC3PODMesh::meshAtIndex( GLint aPODIndex, CC3PODResource* aPODRez )
{
	CC3PODMesh* pMesh = new CC3PODMesh;
	pMesh->initAtIndex( aPODIndex, aPODRez );
	pMesh->autorelease();

	return pMesh;
}

GLint CC3PODMesh::getPodIndex()
{
	return _podIndex; 
}

void CC3PODMesh::setPodIndex( GLint aPODIndex )
{
	_podIndex = aPODIndex; 
}

void CC3PODMesh::populateFrom( CC3PODMesh* another )
{
	super::populateFrom(another);
	
	_podIndex = another->getPodIndex();
}

CCObject* CC3PODMesh::copyWithZone( CCZone* zone )
{
	CC3PODMesh* pVal = new CC3PODMesh;
	pVal->init();
	pVal->populateFrom( this );
	
	return pVal;
}

// Deprecated texture inversion. When this is invoked on a POD mesh, it does need inversion.
void CC3PODMesh::deprecatedAlign( CC3VertexTextureCoordinates* texCoords, CC3Texture* aTexture )
{
	texCoords->flipVertically();		// Avoid switching expectsVerticallyFlippedTextures
}

NS_COCOS3D_END
