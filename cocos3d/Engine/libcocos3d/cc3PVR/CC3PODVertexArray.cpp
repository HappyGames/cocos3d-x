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
#include "CC3PVRTModelPOD.h"
#include "CC3PVRFoundation.h"

NS_COCOS3D_BEGIN

CC3VertexLocations*	CC3PODVertexFactory::createVertexLocations( CC3PODResource* aPODRez, GLint aPODIndex )
{
	CC3VertexLocations* pValue = new CC3VertexLocations();
	if ( pValue->init() )
	{
		SPODMesh* psm = (SPODMesh*)aPODRez->getMeshPODStructAtIndex( aPODIndex );
		CPODData* pcd = (CPODData*)&psm->sVertex;

		GLint elemSize = pcd->n;
		if ( elemSize > 0 )
		{
			pValue->setElementType( GLElementTypeFromEPVRTDataType(pcd->eType) );
			pValue->setShouldNormalizeContent( CC3ShouldNormalizeEPVRTDataType(pcd->eType) );
			pValue->setElementSize( elemSize );
			pValue->setVertexStride( pcd->nStride );
			pValue->setVertexCount( psm->nNumVertex );
			
			if ( psm->pInterleaved ) 
			{	// vertex data is interleaved
				pValue->setVertices( psm->pInterleaved );
				pValue->setElementOffset( (GLuint)(intptr_t)pcd->pData );
			} 
			else 
			{	// not interleaved
				pValue->setVertices( pcd->pData );
				pcd->pData = NULL;					// Clear data reference from CPODData so it won't try to free it.
				pValue->setElementOffset( 0 );
			}

			pValue->setVertexCapacityWithoutAllocation( pValue->getVertexCount() );	// CC3VertexArray instance will free data when needed.

			pValue->setDrawingMode( GLDrawingModeForSPODMesh(psm) );

			pValue->allocateStripLengths( psm->nNumStrips );

			GLuint* pLens = pValue->getStripLengths();
			for (GLuint i = 0; i < psm->nNumStrips; i++)
				pLens[i] = pValue->getVertexIndexCountFromFaceCount( psm->pnStripLength[i] );

			pValue->autorelease();

			return pValue;
		}
	}

    CC_SAFE_DELETE( pValue );
    return NULL;
}

CC3VertexIndices* CC3PODVertexFactory::createVertexIndices( CC3PODResource* aPODRez, GLint aPODIndex )
{
	CC3VertexIndices* pValue = new CC3VertexIndices();
	if ( pValue->init() )
	{
		SPODMesh* psm = (SPODMesh*)aPODRez->getMeshPODStructAtIndex( aPODIndex );
		CPODData* pcd = (CPODData*)&psm->sFaces;

		GLint elemSize = pcd->n;
		if ( elemSize > 0 )
		{
			pValue->setElementType( GLElementTypeFromEPVRTDataType(pcd->eType) );
			pValue->setShouldNormalizeContent( CC3ShouldNormalizeEPVRTDataType(pcd->eType) );
			pValue->setElementSize( elemSize );
			pValue->setVertexStride( pcd->nStride );
			pValue->setVertexCount( psm->nNumVertex );
			
            pValue->setVertices( pcd->pData );
            pcd->pData = NULL;				// Clear data reference from CPODData so it won't try to free it.
            pValue->setElementOffset( 0 );  // Indices are not interleaved.

            pValue->setVertexCount( pValue->getVertexIndexCountFromFaceCount( psm->nNumFaces ) );
			pValue->setVertexCapacityWithoutAllocation( pValue->getVertexCount() );	// CC3VertexArray instance will free data when needed.

			pValue->setDrawingMode( GLDrawingModeForSPODMesh(psm) );

			pValue->allocateStripLengths( psm->nNumStrips );

			GLuint* pLens = pValue->getStripLengths();
			for (GLuint i = 0; i < psm->nNumStrips; i++)
				pLens[i] = pValue->getVertexIndexCountFromFaceCount( psm->pnStripLength[i] );

			pValue->autorelease();

			return pValue;
		}
	}

    CC_SAFE_DELETE( pValue );
    return NULL;
}

CC3VertexNormals* CC3PODVertexFactory::createVertexNormals( CC3PODResource* aPODRez, GLint aPODIndex )
{
    CC3VertexNormals* pValue = new CC3VertexNormals();
	if ( pValue->init() )
	{
		SPODMesh* psm = (SPODMesh*)aPODRez->getMeshPODStructAtIndex( aPODIndex );
		CPODData* pcd = (CPODData*)&psm->sNormals;

		GLint elemSize = pcd->n;
		if ( elemSize > 0 )
		{
			pValue->setElementType( GLElementTypeFromEPVRTDataType(pcd->eType) );
			pValue->setShouldNormalizeContent( CC3ShouldNormalizeEPVRTDataType(pcd->eType) );
			pValue->setElementSize( elemSize );
			pValue->setVertexStride( pcd->nStride );
			pValue->setVertexCount( psm->nNumVertex );
			
			if ( psm->pInterleaved ) 
			{	// vertex data is interleaved
				pValue->setVertices( psm->pInterleaved );
				pValue->setElementOffset( (GLuint)(intptr_t)pcd->pData );
			} 
			else 
			{	// not interleaved
				pValue->setVertices( pcd->pData );
				pValue->setVertexCapacityWithoutAllocation( pValue->getVertexCount() );	// CC3VertexArray instance will free data when needed.
				pcd->pData = NULL;					// Clear data reference from CPODData so it won't try to free it.
				pValue->setElementOffset( 0 );
			}

			pValue->autorelease();

			return pValue;
		}
	}

    CC_SAFE_DELETE( pValue );
    return NULL;
}

CC3VertexTangents* CC3PODVertexFactory::createVertexTangents( CC3PODResource* aPODRez, GLint aPODIndex, bool bitangents )
{
    CC3VertexTangents* pValue = new CC3VertexTangents();
	if ( pValue->init() )
	{
		SPODMesh* psm = (SPODMesh*)aPODRez->getMeshPODStructAtIndex( aPODIndex );
		CPODData* pcd = bitangents ? (CPODData*)&psm->sBinormals : (CPODData*)&psm->sTangents;

		GLint elemSize = pcd->n;
		if ( elemSize > 0 )
		{
			pValue->setElementType( GLElementTypeFromEPVRTDataType(pcd->eType) );
			pValue->setShouldNormalizeContent( CC3ShouldNormalizeEPVRTDataType(pcd->eType) );
			pValue->setElementSize( elemSize );
			pValue->setVertexStride( pcd->nStride );
			pValue->setVertexCount( psm->nNumVertex );
			
			if ( psm->pInterleaved ) 
			{	// vertex data is interleaved
				pValue->setVertices( psm->pInterleaved );
				pValue->setElementOffset( (GLuint)(intptr_t)pcd->pData );
			} 
			else 
			{	// not interleaved
				pValue->setVertices( pcd->pData );
				pValue->setVertexCapacityWithoutAllocation( pValue->getVertexCount() );	// CC3VertexArray instance will free data when needed.
				pcd->pData = NULL;					// Clear data reference from CPODData so it won't try to free it.
				pValue->setElementOffset( 0 );
			}

			pValue->autorelease();

			return pValue;
		}
	}

    CC_SAFE_DELETE( pValue );
    return NULL;
}

CC3VertexColors* CC3PODVertexFactory::createVertexColors( CC3PODResource* aPODRez, GLint aPODIndex )
{
    CC3VertexColors* pValue = new CC3VertexColors();
	if ( pValue->init() )
	{
		SPODMesh* psm = (SPODMesh*)aPODRez->getMeshPODStructAtIndex( aPODIndex );
		CPODData* pcd = (CPODData*)&psm->sVtxColours;

		GLint elemSize = pcd->n;
		if ( elemSize > 0 )
		{
			pValue->setElementType( GLElementTypeFromEPVRTDataType(pcd->eType) );
			pValue->setShouldNormalizeContent( CC3ShouldNormalizeEPVRTDataType(pcd->eType) );
			pValue->setElementSize( elemSize );
			pValue->setVertexStride( pcd->nStride );
			pValue->setVertexCount( psm->nNumVertex );
			
			if ( psm->pInterleaved ) 
			{	// vertex data is interleaved
				pValue->setVertices( psm->pInterleaved );
				pValue->setElementOffset( (GLuint)(intptr_t)pcd->pData );
			} 
			else 
			{	// not interleaved
				pValue->setVertices( pcd->pData );
				pValue->setVertexCapacityWithoutAllocation( pValue->getVertexCount() );	// CC3VertexArray instance will free data when needed.
				pcd->pData = NULL;					// Clear data reference from CPODData so it won't try to free it.
				pValue->setElementOffset( 0 );
			}

			// Element size must be 4 for colors. POD loader sometimes provides incorrect value!
			pValue->setElementSize( 4 );

			pValue->autorelease();

			return pValue;
		}
	}

    CC_SAFE_DELETE( pValue );
    return NULL;
}

CC3VertexBoneWeights* CC3PODVertexFactory::createVertexBoneWeights( CC3PODResource* aPODRez, GLint aPODIndex )
{
    CC3VertexBoneWeights* pValue = new CC3VertexBoneWeights();
	if ( pValue->init() )
	{
		SPODMesh* psm = (SPODMesh*)aPODRez->getMeshPODStructAtIndex( aPODIndex );
		CPODData* pcd = (CPODData*)&psm->sBoneWeight;

		GLint elemSize = pcd->n;
		if ( elemSize > 0 )
		{
			pValue->setElementType( GLElementTypeFromEPVRTDataType(pcd->eType) );
			pValue->setShouldNormalizeContent( CC3ShouldNormalizeEPVRTDataType(pcd->eType) );
			pValue->setElementSize( elemSize );
			pValue->setVertexStride( pcd->nStride );
			pValue->setVertexCount( psm->nNumVertex );
			
			if ( psm->pInterleaved ) 
			{	// vertex data is interleaved
				pValue->setVertices( psm->pInterleaved );
				pValue->setElementOffset( (GLuint)(intptr_t)pcd->pData );
			} 
			else 
			{	// not interleaved
				pValue->setVertices( pcd->pData );
				pValue->setVertexCapacityWithoutAllocation( pValue->getVertexCount() );	// CC3VertexArray instance will free data when needed.
				pcd->pData = NULL;					// Clear data reference from CPODData so it won't try to free it.
				pValue->setElementOffset( 0 );
			}

			pValue->autorelease();

			return pValue;
		}
	}

    CC_SAFE_DELETE( pValue );
    return NULL;
}

CC3VertexBoneIndices* CC3PODVertexFactory::createVertexBoneIndices( CC3PODResource* aPODRez, GLint aPODIndex )
{
    CC3VertexBoneIndices* pValue = new CC3VertexBoneIndices();
	if ( pValue->init() )
	{
		SPODMesh* psm = (SPODMesh*)aPODRez->getMeshPODStructAtIndex( aPODIndex );
		CPODData* pcd = (CPODData*)&psm->sBoneIdx;

		GLint elemSize = pcd->n;
		if ( elemSize > 0 )
		{
			pValue->setElementType( GLElementTypeFromEPVRTDataType(pcd->eType) );
			pValue->setShouldNormalizeContent( CC3ShouldNormalizeEPVRTDataType(pcd->eType) );
			pValue->setElementSize( elemSize );
			pValue->setVertexStride( pcd->nStride );
			pValue->setVertexCount( psm->nNumVertex );
			
			if ( psm->pInterleaved ) 
			{	// vertex data is interleaved
				pValue->setVertices( psm->pInterleaved );
				pValue->setElementOffset( (GLuint)(intptr_t)pcd->pData );
			} 
			else 
			{	// not interleaved
				pValue->setVertices( pcd->pData );
				pValue->setVertexCapacityWithoutAllocation( pValue->getVertexCount() );	// CC3VertexArray instance will free data when needed.
				pcd->pData = NULL;					// Clear data reference from CPODData so it won't try to free it.
				pValue->setElementOffset( 0 );
			}

			pValue->autorelease();

			return pValue;
		}
	}

    CC_SAFE_DELETE( pValue );
    return NULL;
}

CC3VertexTextureCoordinates* CC3PODVertexFactory::createVertexTextureCoordinates( CC3PODResource* aPODRez, GLint aPODIndex, GLint texUnit )
{
	CC3VertexTextureCoordinates* pValue = new CC3VertexTextureCoordinates();
	if ( pValue->init() )
    {
		SPODMesh* psm = (SPODMesh*)aPODRez->getMeshPODStructAtIndex( aPODIndex );
		CPODData* pcd = (CPODData*)&psm->psUVW[texUnit];

		GLint elemSize = pcd->n;
		if ( elemSize > 0 )
		{
			pValue->setElementType( GLElementTypeFromEPVRTDataType(pcd->eType) );
			pValue->setShouldNormalizeContent( CC3ShouldNormalizeEPVRTDataType(pcd->eType) );
			pValue->setElementSize( elemSize );
			pValue->setVertexStride( pcd->nStride );
			pValue->setVertexCount( psm->nNumVertex );
			
			if ( psm->pInterleaved ) 
			{	// vertex data is interleaved
				pValue->setVertices( psm->pInterleaved );
				pValue->setElementOffset( (GLuint)(intptr_t)pcd->pData );
			} 
			else 
			{	// not interleaved
				pValue->setVertices( pcd->pData );
				pValue->setVertexCapacityWithoutAllocation( pValue->getVertexCount() );	// CC3VertexArray instance will free data when needed.
				pcd->pData = NULL;					// Clear data reference from CPODData so it won't try to free it.
				pValue->setElementOffset( 0 );
			}

			pValue->autorelease();

			return pValue;
		}
    }
    
    CC_SAFE_DELETE( pValue );
    return pValue;
}

NS_COCOS3D_END
