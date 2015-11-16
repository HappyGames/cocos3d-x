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
