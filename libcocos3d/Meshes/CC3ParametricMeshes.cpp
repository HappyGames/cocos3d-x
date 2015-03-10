/*
 * CC3ParametricMeshes.m
 *
 * Cocos3D 2.0.1
 * Author: Bill Hollings
 * Copyright (c) 2010-2014 The Brenwill Workshop Ltd. All rights reserved.
 * http://www.brenwill.com
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
 * 
 * See header file CC3ParametricMeshes.h for full API documentation.
 */

#include "CC3ParametricMeshes.h"
#include "../Utility/CC3CC2Extensions.h"

NS_COCOSLIB_BEGIN

void CC3ParametricMesh::ensureVertexContent()
{
	if (getVertexContentTypes() == kCC3VertexContentNone) 
	{
		setVertexContentTypes( (CC3VertexContent)(kCC3VertexContentLocation |
								   kCC3VertexContentNormal |
								   kCC3VertexContentTextureCoordinates) );
	}
}


void CC3ParametricMesh::populateAsTriangle( const CC3Face& face, ccTex2F* tc, GLuint divsPerSide )
{
	// Must have at least one division per side
	divsPerSide = MAX(divsPerSide, 1);

	// The fraction of each side that each division represents.
	// This is the barycentric coordinate division increment.
	GLfloat divFrac = 1.0f / divsPerSide;
	
	// Derive the normal. All vertices on the triangle will have the same normal.
	CC3Vector vtxNml = CC3FaceNormal(face);

	GLuint vertexCount = (GLuint)((divsPerSide + 2) * (divsPerSide + 1) / 2.0f);
	GLuint triangleCount = divsPerSide * divsPerSide;
	
	// Prepare the vertex content and allocate space for vertices and indices.
	ensureVertexContent();
	setAllocatedVertexCapacity( vertexCount );
	setAllocatedVertexIndexCapacity( triangleCount * 3 );

	GLuint vIdx = 0;
	GLuint iIdx = 0;
	
	// Denoting the three corners of the main triangle as c0, c1 & c2, and denoting the side
	// extending from c0 to c1 as s1, and the side extending from c0 to c2 as s2, we can work
	// in barycentric coordinates by starting at c0, iterating the divisions on the s2, and for
	// each divison on that side, iterating  the divisions on the side of the internal similar
	// triangle that is parallel to s1.
	for (GLuint i2 = 0; i2 <= divsPerSide; i2++) 
	{
		// Calculate the barycentric weight for the current division along s2 and hold it constant
		// as we iterate through divisions along s1 of the resulting internal similar triangle.
		// The number of divisions on the side of the internal similar triangle is found by subtracting
		// the current division index of s2 from the total divisions per side.
		GLfloat bw2 = divFrac * i2;
		GLuint divsSimSide1 = divsPerSide - i2;
		for (GLuint i1 = 0; i1 <= divsSimSide1; i1++) 
		{
			// Calculate the barycentric weight for the current division along s1 of the internal
			// similar triangle. The third barycentric weight falls out automatically.
			GLfloat bw1 = divFrac * i1;
			GLfloat bw0 = 1.0f - bw1 - bw2;
			CC3BarycentricWeights bcw = CC3BarycentricWeightsMake(bw0, bw1, bw2);

			// Vertex location from barycentric coordinates on the main face
			CC3Vector vtxLoc = CC3FaceLocationFromBarycentricWeights(face, bcw);
			setVertexLocation( vtxLoc, vIdx );
			
			// Vertex normal is constant. Will do nothing if this mesh does not include normals.
			setVertexNormal( vtxNml, vIdx );
			
			// Vertex texture coordinates derived from the barycentric coordinates.
			// Will do nothing if this mesh does not include texture coordinates.
			GLfloat u = bw0 * tc[0].u + bw1 * tc[1].u + bw2 * tc[2].u;
			GLfloat v = bw0 * tc[0].v + bw1 * tc[1].v + bw2 * tc[2].v;
			setVertexTexCoord2F( cc3tc(u, v), vIdx );

			// First tessellated triangle starting at the vertex and opening away from corner 0.
			if (i1 < divsSimSide1)
			{
				setVertexIndex( vIdx, iIdx++ );
				setVertexIndex( (vIdx + 1), iIdx++ );
				setVertexIndex( (vIdx + divsSimSide1 + 1), iIdx++ );
			}

			// Second tessellated triangle starting at the vertex and opening towards corner 0.
			if (i1 > 0 && i2 > 0) 
			{
				setVertexIndex( vIdx, iIdx++ );
				setVertexIndex( (vIdx - 1), iIdx++ );
				setVertexIndex( (vIdx - divsSimSide1 - 2), iIdx++ );
			}
			
			vIdx++;		// Move on to the next vertex
		}
	}
}

void CC3ParametricMesh::populateAsCenteredRectangleWithSize( const CCSize& rectSize )
{
	populateAsRectangleWithSize( rectSize, ccp(0.5f, 0.5f) );
}

void CC3ParametricMesh::populateAsCenteredRectangleWithSize( const CCSize& rectSize, const CC3Tessellation& divsPerAxis )
{
	populateAsRectangleWithSize( rectSize, ccp(0.5f, 0.5f), divsPerAxis );
}

void CC3ParametricMesh::populateAsRectangleWithSize( const CCSize& rectSize, const CCPoint& origin )
{
	populateAsRectangleWithSize( rectSize, origin, CC3TessellationMake(1, 1) );
}

void CC3ParametricMesh::populateAsRectangleWithSize( const CCSize& rectSize, const CCPoint& _origin, const CC3Tessellation& _divsPerAxis )
{
	CC3Tessellation divsPerAxis = _divsPerAxis;
	// Must be at least one tessellation face per side of the rectangle.
	divsPerAxis.x = MAX(divsPerAxis.x, 1);
	divsPerAxis.y = MAX(divsPerAxis.y, 1);

	// Move the origin of the rectangle to the specified origin
	CCPoint rectExtent = ccpFromSize(rectSize);
	CCPoint origin = ccpCompMult(rectExtent, _origin);
	CCPoint botLeft = ccpSub(CCPointZero, origin);
	CCPoint topRight = ccpSub(rectExtent, origin);

	// The size and texture span of each face in the tessellated grid
	CCSize divSize = CCSizeMake((topRight.x - botLeft.x) / divsPerAxis.x,
								(topRight.y - botLeft.y) / divsPerAxis.y);
	CCSize divTexSpan = CCSizeMake((1.0 / divsPerAxis.x), (1.0 / divsPerAxis.y));

	// Get vertices per side.
	CC3Tessellation verticesPerAxis;
	verticesPerAxis.x = divsPerAxis.x + 1;
	verticesPerAxis.y = divsPerAxis.y + 1;
	GLuint vertexCount = verticesPerAxis.x * verticesPerAxis.y;
	GLuint triangleCount = divsPerAxis.x * divsPerAxis.y * 2;
	
	// Prepare the vertex content and allocate space for vertices and indices.
	ensureVertexContent();
	setAllocatedVertexCapacity( vertexCount );
	setAllocatedVertexIndexCapacity( triangleCount * 3 );

	// Populate vertex locations, normals & texture coordinates in the X-Y plane
	// Iterate through the rows and columns of the vertex grid, from the bottom left corner,
	// and set the location of each vertex to be proportional to its position in the grid,
	// and set the normal of each vertex to point up the Z-axis.
	for (int iy = 0; iy < verticesPerAxis.y; iy++) 
	{
		for (int ix = 0; ix < verticesPerAxis.x; ix++) 
		{
			int vIndx = iy * verticesPerAxis.x + ix;

			// Vertex location
			GLfloat vx = botLeft.x + (divSize.width * ix);
			GLfloat vy = botLeft.y + (divSize.height * iy);
			setVertexLocation( cc3v(vx, vy, 0.0), vIndx );

			// Vertex normal. Will do nothing if this mesh does not include normals.
			setVertexNormal( kCC3VectorUnitZPositive, vIndx );

			// Vertex texture coordinates.
			// Will do nothing if this mesh does not include texture coordinates.
			GLfloat u = divTexSpan.width * ix;
			GLfloat v = divTexSpan.height * iy;
			setVertexTexCoord2F( cc3tc(u, v), vIndx );
		}
	}
	
	// Since the index array is a simple array, just access the array directly.
	// Iterate through the rows and columns of the faces in the grid, from the bottom left corner,
	// and specify the indexes of the three vertices in each of the two triangles of each face.
	GLushort* indices = (GLushort*)getVertexIndices()->getVertices();
	int iIndx = 0;
	for (int iy = 0; iy < divsPerAxis.y; iy++) 
	{
		for (int ix = 0; ix < divsPerAxis.x; ix++) 
		{
			GLushort botLeftOfFace;
			
			// First triangle of face wound counter-clockwise
			botLeftOfFace = iy * verticesPerAxis.x + ix;
			indices[iIndx++] = botLeftOfFace;							// Bot left
			indices[iIndx++] = botLeftOfFace + 1;						// Bot right
			indices[iIndx++] = botLeftOfFace + verticesPerAxis.x + 1;	// Top right

			// Second triangle of face wound counter-clockwise
			indices[iIndx++] = botLeftOfFace + verticesPerAxis.x + 1;	// Top right
			indices[iIndx++] = botLeftOfFace + verticesPerAxis.x;		// Top left
			indices[iIndx++] = botLeftOfFace;							// Bot left
		}
	}
}

void CC3ParametricMesh::populateAsDiskWithRadius( GLfloat radius, const CC3Tessellation& radialAndAngleDivs )
{	
	// Must be at least one radial tessellation, and three angular tessellation.
	GLuint numRadialDivs = MAX(radialAndAngleDivs.x, 1);
	GLuint numAngularDivs = MAX(radialAndAngleDivs.y, 3);

	// Calculate the spans of each radial and angular division.
	GLfloat angularDivSpan = (GLfloat)(kCC3TwoPi / numAngularDivs);		// Zero to 2Pi
	GLfloat radialDivSpan = radius / numRadialDivs;				// Zero to radius
	GLfloat radialTexDivSpan = 0.5f / numRadialDivs;				// Zero to 0.5

	// Calculate number of vertices, triangles and indices.
	GLuint vertexCount = (numRadialDivs * (numAngularDivs + 1)) + 1;
	GLuint triangleCount = ((2 * numRadialDivs) - 1) * numAngularDivs;
	
	// Prepare the vertex content and allocate space for vertices and indices.
	ensureVertexContent();
	setAllocatedVertexCapacity( vertexCount );
	setAllocatedVertexIndexCapacity( triangleCount * 3 );
	GLushort* indices = (GLushort*)getVertexIndices()->getVertices();		// Pointer to the indices
	
	//LogTrace(@"%@ populating as disk with radius: %.3f, %i radial divs, %i angular divs, %i vertices, and %i triangles",
	//			  self, radius, numRadialDivs, numAngularDivs, vertexCount, triangleCount);
	
	// Populate vertex locations, normals & texture coordinates.
	GLuint vIndx = 0;			// Vertex index
	GLuint iIndx = 0;			// Index index

	// Add the center vertex Vertex location from unit radial scaled by the radial span and ring number
	// Setters for any content that is not defined by the vertexContentTypes property will do nothing.
	setVertexLocation( kCC3VectorZero, vIndx );
	setVertexNormal( kCC3VectorUnitZPositive, vIndx );
	setVertexTexCoord2F( cc3tc(0.5f, 0.5f), vIndx );
	
	for (GLuint ia = 0; ia <= numAngularDivs; ia++) 
	{	
		GLfloat angle = angularDivSpan * ia;
		CCPoint unitRadial = ccp(cosf(angle), sinf(angle));
		
		for (GLuint ir = 1; ir <= numRadialDivs; ir++) 
		{
			vIndx++;	// Move on to the next vertex
			
			// Vertex location from unit radial scaled by the radial span and ring number
			CCPoint locPt = ccpMult(unitRadial, (radialDivSpan * ir));
			setVertexLocation( cc3v(locPt.x, locPt.y, 0.0f), vIndx );
			
			// Vertex normal always points along positive Z-axis
			// Will do nothing if this mesh does not include normals.
			setVertexNormal( kCC3VectorUnitZPositive, vIndx );

			// Vertex tex coords from unit radial scaled by the radial texture span and ring
			// number, then shifted to move range from (-0.5 <-> +0.5) to (0.0 <-> +1.0).
			// Will do nothing if this mesh does not include texture coordinates.
			CCPoint texPt = ccpAdd(ccpMult(unitRadial, (radialTexDivSpan * ir)), ccp(0.5f, 0.5f));
			setVertexTexCoord2F( cc3tc(texPt.x, texPt.y), vIndx );
			
			// Since the index array is a simple array, just access the array directly.
			// For the first ring, add one triangle rooted at the origin.
			// For all but the first ring, add two triangles to cover division trapezoid.
			// We don't create triangles for the last set of radial vertices, since they
			// overlap the first.
			if (ia < numAngularDivs) {
				if (ir == 1) {
					indices[iIndx++] = 0;							// Center vertex
					indices[iIndx++] = vIndx;						// Current vertex
					indices[iIndx++] = vIndx + numRadialDivs;		// Next angular div, same ring
				} else {
					indices[iIndx++] = vIndx;						// Current vertex
					indices[iIndx++] = vIndx + numRadialDivs;		// Next angular div, same ring
					indices[iIndx++] = vIndx + numRadialDivs - 1;	// Next angular div, prev ring
					
					indices[iIndx++] = vIndx;						// Current vertex
					indices[iIndx++] = vIndx + numRadialDivs - 1;	// Next angular div, prev ring
					indices[iIndx++] = vIndx - 1;					// Same angular div, prev ring
				}				
			}
		}
	}
}

void CC3ParametricMesh::populateAsSolidBox( const CC3Box& box )
{
	GLfloat w = box.maximum.x - box.minimum.x;		// Width of the box
	GLfloat h = box.maximum.y - box.minimum.y;		// Height of the box
	GLfloat d = box.maximum.z - box.minimum.z;		// Depth of the box
	GLfloat ufw = d + w + d + w;					// Total width of unfolded flattened box
	GLfloat ufh = d + h + d;						// Total height of unfolded flattened box
	populateAsSolidBox( box, ccp((d / ufw), (d / ufh)) );
}

void CC3ParametricMesh::populateAsCubeMappedSolidBox( const CC3Box& box )
{
	populateAsSolidBox( box, ccp((1.0 / 4.0), (1.0 / 3.0)) );
}

// Thanks to Cocos3D user andyman for contributing the prototype code and texture
// template file for this method.
void CC3ParametricMesh::populateAsSolidBox( const CC3Box& box, const CCPoint& corner )
{
	CC3Vector boxMin = box.minimum;
	CC3Vector boxMax = box.maximum;
	GLuint vertexCount = 24;
	GLuint triangleCount = 12;
	
	// Prepare the vertex content and allocate space for vertices and indices.
	ensureVertexContent();
	setAllocatedVertexCapacity( vertexCount );
	setAllocatedVertexIndexCapacity( triangleCount * 3 );
	GLushort* indices = (GLushort*)getVertexIndices()->getVertices();
	
	// Populate all six sides.
	// Setters for any content that is not defined by the vertexContentTypes property will do nothing.
	
	// Front face, CCW winding:
	setVertexLocation( cc3v(boxMin.x, boxMin.y, boxMax.z), 0 );
	setVertexNormal( kCC3VectorUnitZPositive, 0 );
	setVertexTexCoord2F( cc3tc(corner.x, corner.y), 0 );

	setVertexLocation( cc3v(boxMax.x, boxMin.y, boxMax.z), 1 );
	setVertexNormal( kCC3VectorUnitZPositive, 1 );
	setVertexTexCoord2F( cc3tc(0.5f, corner.y), 1 );
	
	setVertexLocation( cc3v(boxMax.x, boxMax.y, boxMax.z), 2 );
	setVertexNormal( kCC3VectorUnitZPositive, 2 );
	setVertexTexCoord2F( cc3tc(0.5f, (1.0f - corner.y)), 2 );
	
	setVertexLocation( cc3v(boxMin.x, boxMax.y, boxMax.z), 3 );
	setVertexNormal( kCC3VectorUnitZPositive, 3 );
	setVertexTexCoord2F( cc3tc(corner.x, (1.0f - corner.y)), 3 );
	
	// Right face, CCW winding:
	setVertexLocation( cc3v(boxMax.x, boxMin.y, boxMax.z), 4 );
	setVertexNormal( kCC3VectorUnitXPositive, 4 );
	setVertexTexCoord2F( cc3tc(0.5f, corner.y), 4 );
	
	setVertexLocation( cc3v(boxMax.x, boxMin.y, boxMin.z), 5 );
	setVertexNormal( kCC3VectorUnitXPositive, 5 );
	setVertexTexCoord2F( cc3tc((0.5f + corner.x), corner.y), 5 );
	
	setVertexLocation( cc3v(boxMax.x, boxMax.y, boxMin.z), 6 );
	setVertexNormal( kCC3VectorUnitXPositive, 6 );
	setVertexTexCoord2F( cc3tc((0.5f + corner.x), (1.0f - corner.y)), 6 );
	
	setVertexLocation( cc3v(boxMax.x, boxMax.y, boxMax.z), 7 );
	setVertexNormal( kCC3VectorUnitXPositive, 7 );
	setVertexTexCoord2F( cc3tc(0.5f, (1.0f - corner.y)), 7 );
	
	// Back face, CCW winding:
	setVertexLocation( cc3v(boxMax.x, boxMin.y, boxMin.z), 8 );
	setVertexNormal( kCC3VectorUnitZNegative, 8 );
	setVertexTexCoord2F( cc3tc((0.5f + corner.x), corner.y), 8 );
	
	setVertexLocation( cc3v(boxMin.x, boxMin.y, boxMin.z), 9 );
	setVertexNormal( kCC3VectorUnitZNegative, 9 );
	setVertexTexCoord2F( cc3tc(1.0f, corner.y), 9 );
	
	setVertexLocation( cc3v(boxMin.x, boxMax.y, boxMin.z), 10 );
	setVertexNormal( kCC3VectorUnitZNegative, 10 );
	setVertexTexCoord2F( cc3tc(1.0f, (1.0f - corner.y)), 10 );
	
	setVertexLocation( cc3v(boxMax.x, boxMax.y, boxMin.z), 11 );
	setVertexNormal( kCC3VectorUnitZNegative, 11 );
	setVertexTexCoord2F( cc3tc((0.5f + corner.x), (1.0f - corner.y)), 11 );
	
	// Left face, CCW winding:
	setVertexLocation( cc3v(boxMin.x, boxMin.y, boxMin.z), 12 );
	setVertexNormal( kCC3VectorUnitXNegative, 12 );
	setVertexTexCoord2F( cc3tc(0.0f, corner.y), 12 );
	
	setVertexLocation( cc3v(boxMin.x, boxMin.y, boxMax.z), 13 );
	setVertexNormal( kCC3VectorUnitXNegative, 13 );
	setVertexTexCoord2F( cc3tc(corner.x, corner.y), 13 );
	
	setVertexLocation( cc3v(boxMin.x, boxMax.y, boxMax.z), 14 );
	setVertexNormal( kCC3VectorUnitXNegative, 14 );
	setVertexTexCoord2F( cc3tc(corner.x, (1.0f - corner.y)), 14 );
	
	setVertexLocation( cc3v(boxMin.x, boxMax.y, boxMin.z), 15 );
	setVertexNormal( kCC3VectorUnitXNegative, 15 );
	setVertexTexCoord2F( cc3tc(0.0f, (1.0f - corner.y)), 15 );
	
	// Top face, CCW winding:
	setVertexLocation( cc3v(boxMin.x, boxMax.y, boxMin.z), 16 );
	setVertexNormal( kCC3VectorUnitYPositive, 16 );
	setVertexTexCoord2F( cc3tc(corner.x, 1.0f), 16 );
	
	setVertexLocation( cc3v(boxMin.x, boxMax.y, boxMax.z), 17 );
	setVertexNormal( kCC3VectorUnitYPositive, 17 );
	setVertexTexCoord2F( cc3tc(corner.x, (1.0f - corner.y)), 17 );
	
	setVertexLocation( cc3v(boxMax.x, boxMax.y, boxMax.z), 18 );
	setVertexNormal( kCC3VectorUnitYPositive, 18 );
	setVertexTexCoord2F( cc3tc(0.5f, (1.0f - corner.y)), 18 );
	
	setVertexLocation( cc3v(boxMax.x, boxMax.y, boxMin.z), 19 );
	setVertexNormal( kCC3VectorUnitYPositive, 19 );
	setVertexTexCoord2F( cc3tc(0.5f, 1.0f), 19 );
	
	// Bottom face, CCW winding:
	setVertexLocation( cc3v(boxMin.x, boxMin.y, boxMax.z), 20 );
	setVertexNormal( kCC3VectorUnitYNegative, 20 );
	setVertexTexCoord2F( cc3tc(corner.x, corner.y), 20 );
	
	setVertexLocation( cc3v(boxMin.x, boxMin.y, boxMin.z), 21 );
	setVertexNormal( kCC3VectorUnitYNegative, 21 );
	setVertexTexCoord2F( cc3tc(corner.x, 0.0f), 21 );
	
	setVertexLocation( cc3v(boxMax.x, boxMin.y, boxMin.z), 22 );
	setVertexNormal( kCC3VectorUnitYNegative, 22 );
	setVertexTexCoord2F( cc3tc(0.5f, 0.0f), 22 );
	
	setVertexLocation( cc3v(boxMax.x, boxMin.y, boxMax.z), 23 );
	setVertexNormal( kCC3VectorUnitYNegative, 23 );
	setVertexTexCoord2F( cc3tc(0.5f, corner.y), 23 );

	// Populate the vertex indices
	// Since the index array is a simple array, just access the array directly.
	GLubyte indxIndx = 0;
	GLubyte vtxIndx = 0;
	for (int side = 0; side < 6; side++) {
		// First trangle of side - CCW from bottom left
		indices[indxIndx++] = vtxIndx++;		// vertex 0
		indices[indxIndx++] = vtxIndx++;		// vertex 1
		indices[indxIndx++] = vtxIndx;			// vertex 2
		
		// Second triangle of side - CCW from bottom left
		indices[indxIndx++] = vtxIndx++;		// vertex 2
		indices[indxIndx++] = vtxIndx++;		// vertex 3
		indices[indxIndx++] = (vtxIndx - 4);	// vertex 0
	}
}

// Vertex index data for the 12 lines of a wire box.
static const GLubyte wireBoxIndexData[] = {
	0, 1, 1, 3, 3, 2, 2, 0,
	4, 5, 5, 7, 7, 6, 6, 4,
	0, 4, 1, 5, 2, 6, 3, 7,
};

void CC3ParametricMesh::populateAsWireBox( const CC3Box& box )
{
	CC3Vector boxMin = box.minimum;
	CC3Vector boxMax = box.maximum;
	GLuint vertexCount = 8;
	
	// Create vertexLocation array.
	CC3VertexLocations* locArray = CC3VertexLocations::vertexArray();
	locArray->setAllocatedVertexCapacity( vertexCount );
	CC3Vector* vertices = (CC3Vector*)locArray->getVertices();
	
	// Extract all 8 corner vertices from the box.
	vertices[0] = cc3v(boxMin.x, boxMin.y, boxMin.z);
	vertices[1] = cc3v(boxMin.x, boxMin.y, boxMax.z);
	vertices[2] = cc3v(boxMin.x, boxMax.y, boxMin.z);
	vertices[3] = cc3v(boxMin.x, boxMax.y, boxMax.z);
	vertices[4] = cc3v(boxMax.x, boxMin.y, boxMin.z);
	vertices[5] = cc3v(boxMax.x, boxMin.y, boxMax.z);
	vertices[6] = cc3v(boxMax.x, boxMax.y, boxMin.z);
	vertices[7] = cc3v(boxMax.x, boxMax.y, boxMax.z);
	
	GLuint lineCount = 12;
	GLuint indexCount = lineCount * 2;
	CC3VertexIndices* indexArray = CC3VertexIndices::vertexArray();
	indexArray->setDrawingMode( GL_LINES );
	indexArray->setElementType( GL_UNSIGNED_BYTE );
	indexArray->setVertexCount( indexCount );
	indexArray->setVertices( (GLvoid*)wireBoxIndexData );
	
	setVertexLocations( locArray );
	setVertexIndices( indexArray );
}

void CC3ParametricMesh::populateAsSphereWithRadius( GLfloat radius, const CC3Tessellation& _divsPerAxis )
{ 
	CC3Tessellation divsPerAxis = _divsPerAxis;
	// Must be at least one tessellation face per side of the rectangle.
	divsPerAxis.x = MAX(divsPerAxis.x, 3);
	divsPerAxis.y = MAX(divsPerAxis.y, 2);
	
	// The division span and texture span of each face in the tessellated grid.
	CCSize divSpan = CCSizeMake( (kCC3TwoPi / divsPerAxis.x), (kCC3Pi / divsPerAxis.y) );
	CCSize divTexSpan = CCSizeMake((1.0 / divsPerAxis.x), (1.0 / divsPerAxis.y));
	GLfloat halfDivTexSpanWidth = divTexSpan.width * 0.5f;
	
	// Calculate number of vertices, triangles and indices.
	CC3Tessellation verticesPerAxis;
	verticesPerAxis.x = divsPerAxis.x + 1;
	verticesPerAxis.y = divsPerAxis.y + 1;
	GLuint vertexCount = verticesPerAxis.x * verticesPerAxis.y;
	GLuint triangleCount = divsPerAxis.x * (divsPerAxis.y - 1) * 2;
	
	// Prepare the vertex content and allocate space for vertices and indices.
	ensureVertexContent();
	setAllocatedVertexCapacity( vertexCount );
	setAllocatedVertexIndexCapacity( triangleCount * 3 );
	GLushort* indices = (GLushort*)getVertexIndices()->getVertices();
	
	//LogTrace(@"%@ populating as sphere with radius %.3f, (%i, %i) divisions, %i vertices, and %i triangles",
	//			  self, radius, divsPerAxis.x, divsPerAxis.y, vertexCount, triangleCount);
	
	// Populate vertex locations, normals & texture coordinates.
	// The parametric X-axis represents the longtitude (0 to 2PI).
	// The parametric Y-axis represents the latitude (0 to PI), starting at the north pole.
	GLuint vIndx = 0;			// Vertex index
	GLuint iIndx = 0;			// Index index
	for (GLint iy = 0; iy < verticesPerAxis.y; iy++) 
	{
		// Latitude (Y): 0 to PI
		GLfloat y = divSpan.height * iy;
		GLfloat sy = sinf(y);
		GLfloat cy = cosf(y);
		
		for (GLint ix = 0; ix < verticesPerAxis.x; ix++) 
		{
			// Longtitude (X): 0 to 2PI
			GLfloat x = divSpan.width * ix;
			GLfloat sx = sinf(x);
			GLfloat cx = cosf(x);
			
			// Vertex location, starting at negative-Z axis,
			// and right-hand rotating towards negative-X axis.
			CC3Vector unitRadial = cc3v( -(sy * sx), cy, -(sy * cx) );
			setVertexLocation( CC3VectorScaleUniform(unitRadial, radius), vIndx );
			
			// Vertex normal - same as location on unit sphere
			// Will do nothing if this mesh does not include normals.
			setVertexNormal( unitRadial, vIndx );
			
			// Calculate vertex texture coordinate. Offset the texture coordinates at
			// each vertex at the poles by half of the division span (so triangle is
			// symetrical. The tex coord at the north pole is moved right and that at
			// the south pole is moved to the left.
			// Will do nothing if this mesh does not include texture coordinates.
			GLfloat uOffset = 0.0f;
			if (iy == 0) uOffset = halfDivTexSpanWidth;							// North pole
			if (iy == (verticesPerAxis.y - 1)) uOffset = -halfDivTexSpanWidth;	// South pole
			GLfloat u = divTexSpan.width * ix + uOffset;
			GLfloat v = divTexSpan.height * iy;
			setVertexTexCoord2F( cc3tc(u, (1.0f - v)), vIndx );
			
			// Since the index array is a simple array, just access the array directly.
			// For each vertex that is at the bottom-right corner of a division, add triangles.
			if (iy > 0 && ix > 0)
			{
				// For all but the first division row, add the triangle that has apex pointing south.
				if (iy > 1) {
					indices[iIndx++] = vIndx;							// Bottom right
					indices[iIndx++] = vIndx - verticesPerAxis.x;		// Top right
					indices[iIndx++] = vIndx - verticesPerAxis.x - 1;	// Top left
				}				
				
				// For all but the last division row, add the triangle that has apex pointing north.
				if (iy < (verticesPerAxis.y - 1)) {
					indices[iIndx++] = vIndx - verticesPerAxis.x - 1;	// Top left
					indices[iIndx++] = vIndx - 1;						// Bottom left
					indices[iIndx++] = vIndx;							// Bottom right
				}
			}
			vIndx++;	// Move on to the next vertex
		}
	}
}

void CC3ParametricMesh::populateAsHollowConeWithRadius( GLfloat radius, GLfloat height, const CC3Tessellation& angleAndHeightDivs )
{	
	// Must be at least one height tessellation, and three angular tessellation.
	GLuint numAngularDivs = MAX(angleAndHeightDivs.x, 3);
	GLuint numHeightDivs = MAX(angleAndHeightDivs.y, 1);
	
	// Calculate the spans of each angular and height division.
	GLfloat radiusHeightRatio = radius / height;
	GLfloat angularDivSpan = (GLfloat)kCC3TwoPi / numAngularDivs;		// Zero to 2Pi
	GLfloat heightDivSpan = height / numHeightDivs;				// Zero to height
	GLfloat radialDivSpan = radius / numHeightDivs;				// Zero to radius
	GLfloat texAngularDivSpan = 1.0f / numAngularDivs;			// Zero to one
	GLfloat texHeightDivSpan = 1.0f / numHeightDivs;			// Zero to one
	
	// Calculate number of vertices, triangles and indices.
	GLuint vertexCount = (numAngularDivs + 1) * (numHeightDivs + 1);
	GLuint triangleCount = 2 * numAngularDivs * numHeightDivs - numAngularDivs;
	
	// Prepare the vertex content and allocate space for vertices and indices.
	ensureVertexContent();
	setAllocatedVertexCapacity( vertexCount );
	setAllocatedVertexIndexCapacity( triangleCount * 3 );
	
	// Populate vertex locations, normals & texture coordinates.
	GLuint vIdx = 0;			// Vertex index
	GLuint iIdx = 0;			// Index index
	for (GLuint ia = 0; ia <= numAngularDivs; ia++) 
	{	
		GLfloat angle = angularDivSpan * ia;
		GLfloat ca = -cosf(angle);		// Put seam on Z-minus axis and proceed CCW
		GLfloat sa = -sinf(angle);		// Put seam on Z-minus axis and proceed CCW
		CC3Vector vtxNormal = CC3VectorNormalize(cc3v(sa, radiusHeightRatio, ca));
		
		for (GLuint ih = 0; ih <= numHeightDivs; ih++, vIdx++)
		{
			GLfloat vtxRadius = radius - (radialDivSpan * ih);
			GLfloat vtxHt = heightDivSpan * ih;
			CC3Vector vtxLoc = cc3v(vtxRadius * sa, vtxHt, vtxRadius * ca);
			setVertexLocation( vtxLoc, vIdx );
			
			// All vertex normals for one angular division point in the same direction
			// Will do nothing if this mesh does not include normals.
			setVertexNormal( vtxNormal, vIdx );
			
			// Vertex tex coords wrapped around and projected horizontally to the cone surface.
			// Will do nothing if this mesh does not include texture coordinates.
			ccTex2F texCoord = cc3tc(texAngularDivSpan * ia, texHeightDivSpan * ih);
			setVertexTexCoord2F( texCoord, vIdx );
			
			// First triangular face
			if (ia < numAngularDivs && ih < numHeightDivs) 
			{
				setVertexIndex( vIdx, iIdx++ );							// Current vertex
				setVertexIndex( (vIdx + numHeightDivs + 1), iIdx++ );	// Next angular div, same height
				setVertexIndex( (vIdx + numHeightDivs + 2), iIdx++ );	// Next angular div, next height

				// Only one triangular face at ring below apex
				if (ih < numHeightDivs - 1) 
				{
					setVertexIndex( (vIdx + numHeightDivs + 2), iIdx++ );	// Next angular div, next height
					setVertexIndex( (vIdx + 1), iIdx++ );					// Same angular div, next height
					setVertexIndex( vIdx, iIdx++ );							// Current vertex
				}
			}
		}
	}
}

void CC3ParametricMesh::populateAsLineStripWith( GLuint vertexCount, CC3Vector* vertices, bool shouldRetainVertices )
{
	// Create vertexLocation array.
	CC3VertexLocations* locArray = CC3VertexLocations::vertexArray();
	locArray->setDrawingMode( GL_LINE_STRIP );
	if (shouldRetainVertices)
	{
		locArray->setAllocatedVertexCapacity( vertexCount );
		memcpy(locArray->getVertices(), vertices, vertexCount * sizeof(CC3Vector));
	} else {
		locArray->setVertexCount( vertexCount );
		locArray->setVertices( vertices );
	}
	setVertexLocations( locArray );
}

NS_COCOSLIB_END
