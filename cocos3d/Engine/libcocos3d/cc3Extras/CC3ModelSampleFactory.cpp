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
#include "teapot.h"

NS_COCOS3D_BEGIN


CC3ModelSampleFactory::CC3ModelSampleFactory()
{
	m_teapotVertexLocations = NULL;
	m_teapotVertexNormals = NULL;
	m_teapotVertexIndices = NULL;
	m_teapotVertexTextureCoordinates = NULL;
	m_teapotVertexColors = NULL;
	m_pTexturedTeapotMesh = NULL;
	m_pMulticoloredTeapotMesh  = NULL;
	m_pUnicoloredTeapotMesh = NULL;
}

CC3ModelSampleFactory::~CC3ModelSampleFactory()
{
	CC_SAFE_RELEASE( m_teapotVertexLocations );
	CC_SAFE_RELEASE( m_teapotVertexNormals );
	CC_SAFE_RELEASE( m_teapotVertexIndices );
	CC_SAFE_RELEASE( m_teapotVertexTextureCoordinates );
	CC_SAFE_RELEASE( m_teapotVertexColors );
	CC_SAFE_RELEASE( m_pTexturedTeapotMesh );
	CC_SAFE_RELEASE( m_pMulticoloredTeapotMesh );
	CC_SAFE_RELEASE( m_pUnicoloredTeapotMesh );
}

CC3Mesh* CC3ModelSampleFactory::makeTeapotMeshNamed( const std::string& aName )
{
	CC3Mesh* mesh = CC3Mesh::meshWithName( aName );
	mesh->setShouldInterleaveVertices( false );
	mesh->setVertexLocations( m_teapotVertexLocations );
	mesh->setVertexNormals( m_teapotVertexNormals );
	mesh->setVertexIndices( m_teapotVertexIndices );
	return mesh;
}

CC3MeshNode* CC3ModelSampleFactory::makeUniColoredTeapotNamed( const std::string& aName, const ccColor4F& color )
{
	CC3MeshNode* teapot = CC3MeshNode::nodeWithName( aName );
	teapot->setMesh( getUnicoloredTeapotMesh() );
	teapot->setMaterial( CC3Material::shiny() );
	teapot->setDiffuseColor( color );
	return teapot;
}

CC3MeshNode* CC3ModelSampleFactory::makeMultiColoredTeapotNamed( const std::string& aName )
{
	CC3MeshNode* teapot = CC3MeshNode::nodeWithName( aName );
	teapot->setMesh( getMulticoloredTeapotMesh() );
	teapot->setMaterial( CC3Material::shiny() );
	return teapot;
}

CC3MeshNode* CC3ModelSampleFactory::makeTexturableTeapotNamed( const std::string& aName )
{
	CC3MeshNode* teapot = CC3MeshNode::nodeWithName( aName );
	teapot->setMesh( getTexturedTeapotMesh() );
	return teapot;
}

// Initialize teapot vertex arrays that can be reused in many teapots.
void CC3ModelSampleFactory::initTeapotVertexArrays()
{	
	// Vertex locations come from the teapot.h header file
	m_teapotVertexLocations = CC3VertexLocations::vertexArrayWithName( "TeapotVertices" );
	m_teapotVertexLocations->retain();
	m_teapotVertexLocations->setVertexCount( num_teapot_vertices );
	m_teapotVertexLocations->setVertices( teapot_vertices );
	
	// Vertex normals come from the teapot.h header file
	m_teapotVertexNormals = CC3VertexNormals::vertexArrayWithName( "TeapotNormals" );
	m_teapotVertexNormals->retain();
	m_teapotVertexNormals->setVertexCount( num_teapot_normals );
	m_teapotVertexNormals->setVertices( teapot_normals );
	
	// Vertex indices populated from the run-length array in the teapot.h header file
	m_teapotVertexIndices = CC3VertexIndices::vertexArrayWithName( "TeapotIndicies" );
	m_teapotVertexIndices->retain();
	m_teapotVertexIndices->populateFromRunLengthArray( (GLushort*)new_teapot_indicies, num_teapot_indices );
	m_teapotVertexIndices->setDrawingMode( GL_TRIANGLE_STRIP );
	
	// Scan vertex location array to find the min & max of each vertex dimension.
	// This can be used below to create both simple color gradient and texture wraps for the mesh.
	CC3Vector vl, vlMin, vlMax, vlRange;
	CC3Vector* vLocs = (CC3Vector*)m_teapotVertexLocations->getVertices();
	GLuint vCount = m_teapotVertexLocations->getVertexCount();
	vl = vLocs[0];
	vlMin = vl;
	vlMax = vl;
	for (GLuint i = 1; i < vCount; i++)
	{
		vl = vLocs[i];
		vlMin = vlMin.minimize( vl );
		vlMax = vlMax.maxmize( vl );
	}
	
	vlRange.x = vlMax.x - vlMin.x;
	vlRange.y = vlMax.y - vlMin.y;
	vlRange.z = vlMax.z - vlMin.z;
	
	// Create a color array to assign colors to each vertex in a simple gradient pattern.
	// This would never happen in practice. Normally, the color array would be applied
	// and extracted as part of the creation of a mesh in a visual editor.
	m_teapotVertexColors = CC3VertexColors::vertexArrayWithName( "TeapotColors" );
	m_teapotVertexColors->retain();
	m_teapotVertexColors->setAllocatedVertexCapacity( vCount );
	ccColor4B* vCols = (ccColor4B*)m_teapotVertexColors->getVertices();
	for (GLuint i=0; i < vCount; i++) 
	{
		vCols[i].r = (GLubyte)(255 * (vLocs[i].x - vlMin.x) / vlRange.x);
		vCols[i].g = (GLubyte)(255 * (vLocs[i].y - vlMin.y) / vlRange.y);
		vCols[i].b = (GLubyte)(255 * (vLocs[i].z - vlMin.z) / vlRange.z);
		vCols[i].a = 255;
	}

	// To allow the color of the individual vertices to be changed at a later time, the vertex
	// colors are retained in memory by setting the shouldReleaseRedundantContent property to NO.
	m_teapotVertexColors->setShouldReleaseRedundantContent( false );
	
	// Progamatically create a texture array to map an arbitrary texture to the mesh vertices
	// in the X-Y plane. This would never happen in practice. Normally, the texture array would
	// be painted and extracted as part of the creation of a mesh in a 3D visual editor.
	m_teapotVertexTextureCoordinates = CC3VertexTextureCoordinates::vertexArrayWithName( "TeapotTexture" );
	m_teapotVertexTextureCoordinates->retain();
	m_teapotVertexTextureCoordinates->setAllocatedVertexCapacity( vCount );
	ccTex2F* vTexCoord = (ccTex2F*)m_teapotVertexTextureCoordinates->getVertices();
	for (GLuint i=0; i < vCount; i++) 
	{
		vTexCoord[i].u = (vLocs[i].x - vlMin.x) / vlRange.x;
		vTexCoord[i].v = (vLocs[i].y - vlMin.y) / vlRange.y;
	}

	// To allow textures of differing sizes to be assigned to this mesh, the texture coordinates
	// are retained in memory by setting the shouldReleaseRedundantContent property to NO.
	m_teapotVertexTextureCoordinates->setShouldReleaseRedundantContent( false );
}

// Initialize several teapot meshes that can be reused in many teapots.
void CC3ModelSampleFactory::initTeapotMeshes()
{	
	// Set up the vertex arrays that will be shared by all teapots
	initTeapotVertexArrays();
	
	// Mesh to support a teapot with single-colored material
	m_pUnicoloredTeapotMesh = makeTeapotMeshNamed( "UnicoloredTeapot" );
	m_pUnicoloredTeapotMesh->retain();
	
	// Mesh to support a teapot with separately colored vertices
	m_pMulticoloredTeapotMesh = makeTeapotMeshNamed( "MulticolorTeapot" );
	m_pMulticoloredTeapotMesh->retain();

	m_pMulticoloredTeapotMesh->setVertexColors( m_teapotVertexColors );
	
	// Mesh to support a teapot with a textured surface
	m_pTexturedTeapotMesh = makeTeapotMeshNamed( "TexturedTeapot" );
	m_pTexturedTeapotMesh->retain();
	m_pTexturedTeapotMesh->setVertexTextureCoordinates( m_teapotVertexTextureCoordinates );
}

void CC3ModelSampleFactory::init()
{
	initTeapotMeshes();
}

static CC3ModelSampleFactory* _factory = NULL;

CC3ModelSampleFactory* CC3ModelSampleFactory::factory()
{
	if (!_factory)
	{
		_factory = new CC3ModelSampleFactory;		// retained
		_factory->init();
		_factory->autorelease();

		_factory->retain();
	}
	return _factory;
}

void CC3ModelSampleFactory::deleteFactory()
{
	CC_SAFE_RELEASE( _factory );
}

CC3Mesh* CC3ModelSampleFactory::getMulticoloredTeapotMesh()
{
	return m_pMulticoloredTeapotMesh;
}

CC3Mesh* CC3ModelSampleFactory::getTexturedTeapotMesh()
{
	return m_pTexturedTeapotMesh;
}

CC3Mesh* CC3ModelSampleFactory::getUnicoloredTeapotMesh()
{
	return m_pUnicoloredTeapotMesh;
}

NS_COCOS3D_END
