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

CC3VertexLocations* CC3VertexLocations::vertexArray()
{
	CC3VertexLocations* pArray = new CC3VertexLocations;
	pArray->init();
	pArray->autorelease();
	return pArray;
}

CC3VertexLocations* CC3VertexLocations::vertexArrayWithName( const std::string& aName )
{
	CC3VertexLocations* pArray = new CC3VertexLocations;
	pArray->initWithName( aName );
	pArray->autorelease();

	return pArray;
}


void CC3VertexLocations::markBoundaryDirty()
{
	m_boundaryIsDirty = true;
	m_radiusIsDirty = true;
}

// Mark boundary dirty, but only if vertices are valid (to avoid marking dirty on dealloc)
void CC3VertexLocations::verticesWereChanged()
{
	if (m_vertices && m_vertexCount) 
		markBoundaryDirty(); 
}

void CC3VertexLocations::setVertexCount( GLuint count )
{
	super::setVertexCount( count );
	markBoundaryDirty();
}

// Template method that populates this instance from the specified other instance.
// This method is invoked automatically during object copying via the copyWithZone: method.
void CC3VertexLocations::populateFrom( CC3VertexLocations* another )
{
	super::populateFrom( another );

	m_firstVertex = another->m_firstVertex;
	m_boundingBox = another->getBoundingBox();
	m_centerOfGeometry = another->getCenterOfGeometry();
	m_radius = another->getRadius();
	m_boundaryIsDirty = another->m_boundaryIsDirty;
	m_radiusIsDirty = another->m_radiusIsDirty;
}

CCObject* CC3VertexLocations::copyWithZone( CCZone* zone )
{
	CC3VertexLocations* pVal = new CC3VertexLocations;
	pVal->init();
	pVal->populateFrom( this );

	return pVal;
}

CC3Vector CC3VertexLocations::getLocationAt( GLuint index )
{
	CC3Vector loc = *(CC3Vector*)getAddressOfElement(index);
	switch (m_elementSize) 
	{
		case 2:
			loc.z = 0.0f;
		case 3:
		case 4:			// Will just read the first three components
		default:
			break;
	}
	return loc;
}

void CC3VertexLocations::setLocation( const CC3Vector& aLocation, GLuint index )
{
	if ( index >= m_allocatedVertexCapacity )
		return;

	GLvoid* elemAddr = getAddressOfElement(index);
	switch (m_elementSize) {
		case 2:		// Just store X & Y
			*(CCPoint*)elemAddr = *(CCPoint*)&aLocation;
			break;
		case 4:		// Convert to 4D with w = 1
			*(CC3Vector4*)elemAddr = CC3Vector4().fromLocation(aLocation);
			break;
		case 3:
		default:
			*(CC3Vector*)elemAddr = aLocation;
			break;
	}
	markBoundaryDirty();
}

CC3Vector4 CC3VertexLocations::getHomogeneousLocationAt( GLuint index )
{
	CC3Vector4 hLoc = *(CC3Vector4*)getAddressOfElement(index);
	switch (m_elementSize) {
		case 2:
			hLoc.z = 0.0f;
		case 3:
			hLoc.w = 1.0f;
		case 4:
		default:
			break;
	}
	return hLoc;
}

void CC3VertexLocations::setHomogeneousLocation( const CC3Vector4& aLocation, GLuint index )
{
	GLvoid* elemAddr = getAddressOfElement(index);
	switch (m_elementSize) {
		case 2:		// Just store X & Y
			*(CCPoint*)elemAddr = *(CCPoint*)&aLocation;
			break;
		case 3:		// Truncate to 3D
			*(CC3Vector*)elemAddr = *(CC3Vector*)&aLocation;
			break;
		case 4:		
		default:
			*(CC3Vector4*)elemAddr = aLocation;
			break;
	}
	markBoundaryDirty();
}

CC3Face CC3VertexLocations::getFaceAt( GLuint faceIndex )
{
	return getFaceFromIndices( getFaceIndicesAt(faceIndex) ); 
}

CC3Face CC3VertexLocations::getFaceFromIndices( const CC3FaceIndices& faceIndices )
{
	return CC3Face(getLocationAt( faceIndices.vertices[0] ),
					   getLocationAt( faceIndices.vertices[1] ),
					   getLocationAt( faceIndices.vertices[2] ));
}

/** Returns the boundingBox, building it if necessary. */
CC3Box CC3VertexLocations::getBoundingBox()
{
	buildBoundingBoxIfNecessary();
	return m_boundingBox;
}

/** Returns the centerOfGeometry, calculating it via the bounding box if necessary. */
CC3Vector CC3VertexLocations::getCenterOfGeometry()
{
	buildBoundingBoxIfNecessary();
	return m_centerOfGeometry;
}

/** Builds the bounding box if it needs to be built. */
void CC3VertexLocations::buildBoundingBoxIfNecessary()
{
	if (m_boundaryIsDirty) 
		buildBoundingBox(); 
}

/** Returns the radius, calculating it if necessary. */
GLfloat CC3VertexLocations::getRadius()
{
	calcRadiusIfNecessary();
	return m_radius;
}

/** Calculates the radius if it necessary. */
void CC3VertexLocations::calcRadiusIfNecessary()
{
	if (m_radiusIsDirty) 
		calcRadius(); 
}

/**
 * Calculates and populates the boundingBox and centerOfGeometry properties
 * from the vertex locations.
 *
 * This method is invoked automatically when the bounding box or centerOfGeometry property
 * is accessed for the first time after the vertices property has been set.
 */
void CC3VertexLocations::buildBoundingBox()
{
	// If we don't have vertices, but do have a non-zero vertexCount, raise an assertion
	CCAssert( !( !m_vertices && m_vertexCount ), "CC3VertexLocations bounding box requested after vertex data have been released");
	CCAssert(m_elementType == GL_FLOAT, "CC3VertexLocations must have elementType GLFLOAT to build the bounding box");

	CC3Vector vl, vlMin, vlMax;
	vl = (m_vertexCount > 0) ?  getLocationAt(0) : CC3Vector::kCC3VectorZero;
	vlMin = vl;
	vlMax = vl;
	for (GLuint i = 1; i < m_vertexCount; i++) 
	{
		vl = getLocationAt(i);
		vlMin = vlMin.minimize( vl );
		vlMax = vlMax.maxmize( vl );
	}
	m_boundingBox.minimum = vlMin;
	m_boundingBox.maximum = vlMax;
	m_centerOfGeometry = m_boundingBox.getCenter();
	m_boundaryIsDirty = false;
}

/**
 * Calculates and populates the radius property from the vertex locations.
 *
 * This method is invoked automatically when the radius property is accessed
 * for the first time after the boundary has been marked dirty.
 */
void CC3VertexLocations::calcRadius()
{
	CCAssert(m_elementType == GL_FLOAT, "CC3VertexLocations must have elementType GLFLOAT to calculate mesh radius");

	CC3Vector cog = getCenterOfGeometry();		// Will measure it if necessary
	if (m_vertices && m_vertexCount) 
	{
		// Work with the square of the radius so that all distances can be compared
		// without having to run expensive square-root calculations.
		GLfloat radiusSq = 0.0;
		for (GLuint i=0; i < m_vertexCount; i++) 
		{
			CC3Vector vl = getLocationAt( i );
			GLfloat distSq = vl.distanceSquared( cog );
			radiusSq = MAX(radiusSq, distSq);
		}

		m_radius = sqrtf(radiusSq);		// Now finally take the square-root
		m_radiusIsDirty = false;
	}
}

void CC3VertexLocations::moveMeshOriginTo( const CC3Vector& aLocation )
{
	for (GLuint i = 0; i < m_vertexCount; i++) 
	{
		CC3Vector locOld = getLocationAt(i);
		CC3Vector locNew = locOld.difference( aLocation );
		setLocation(locNew, i);
	}
	markBoundaryDirty();
	updateGLBuffer();
}

void CC3VertexLocations::moveMeshOriginToCenterOfGeometry()
{
	moveMeshOriginTo( getCenterOfGeometry() ); 
}

/** Overridden to ensure the bounding box and radius are built before releasing the vertices. */
void CC3VertexLocations::releaseRedundantContent()
{
	buildBoundingBoxIfNecessary();
	calcRadiusIfNecessary();
	super::releaseRedundantContent();
}

void CC3VertexLocations::drawFrom( GLuint vtxIdx, GLuint vtxCount, CC3NodeDrawingVisitor* visitor )
{
	super::drawFrom( vtxIdx, vtxCount, visitor );

	visitor->getGL()->drawVerticiesAs( m_drawingMode, m_firstVertex + vtxIdx, vtxCount );
}

std::string CC3VertexLocations::getNameSuffix()
{
	return "Locations"; 
}

void CC3VertexLocations::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	{
		m_firstVertex = 0;
		m_centerOfGeometry = CC3Vector::kCC3VectorZero;
		m_boundingBox = CC3Box::kCC3BoxZero;
		m_radius = 0.0;
		markBoundaryDirty();
	}
}

GLenum CC3VertexLocations::defaultSemantic()
{
	return kCC3SemanticVertexLocation; 
}

NS_COCOS3D_END
