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

std::string stringFromCC3VertexContent(CC3VertexContent vtxContent) 
{
	std::string desc = "";
	bool first = true;
	if (vtxContent & kCC3VertexContentLocation) {
		desc += CC3String::stringWithFormat( (char*)"%s", first ? " (" : " + " );
		desc += CC3String::stringWithFormat( (char*)"Location" );
		first = false;
	}
	if (vtxContent & kCC3VertexContentNormal) {
		desc += CC3String::stringWithFormat( (char*)"%s", first ? " (" : " + " );
		desc += CC3String::stringWithFormat( (char*)"Normal" );
		first = false;
	}
	if (vtxContent & kCC3VertexContentTangent) {
		desc += CC3String::stringWithFormat( (char*)"%s", first ? " (" : " + " );
		desc += CC3String::stringWithFormat( (char*)"Tangent" );
		first = false;
	}
	if (vtxContent >= kCC3VertexContentBitangent) {
		desc += CC3String::stringWithFormat( (char*)"%s", first ? " (" : " + " );
		desc += CC3String::stringWithFormat( (char*)"Bitangent" );
		first = false;
	}
	if (vtxContent & kCC3VertexContentColor) {
		desc += CC3String::stringWithFormat( (char*)"%s", first ? " (" : " + " );
		desc += CC3String::stringWithFormat( (char*)"Color" );
		first = false;
	}
	if (vtxContent & kCC3VertexContentTextureCoordinates) {
		desc += CC3String::stringWithFormat( (char*)"%s", first ? " (" : " + " );
		desc += CC3String::stringWithFormat( (char*)"TexCoords" );
		first = false;
	}
	if (vtxContent & kCC3VertexContentPointSize) {
		desc += CC3String::stringWithFormat( (char*)"%s", first ? " (" : " + " );
		desc += CC3String::stringWithFormat( (char*)"PointSize" );
		first = false;
	}
	if (vtxContent & kCC3VertexContentBoneWeights) {
		desc += CC3String::stringWithFormat( (char*)"%s", first ? " (" : " + " );
		desc += CC3String::stringWithFormat( (char*)"BoneWieghts" );
		first = false;
	}
	if (vtxContent & kCC3VertexContentBoneIndices) {
		desc += CC3String::stringWithFormat( (char*)"%s", first ? " (" : " + " );
		desc += CC3String::stringWithFormat( (char*)"BoneIndices" );
		first = false;
	}
	desc += CC3String::stringWithFormat( (char*)"%s", first ? "(None)" : ")" );
	return desc;
}

CC3Mesh::CC3Mesh()
{
	m_vertexLocations = NULL;
	m_vertexNormals = NULL;
	m_vertexTangents = NULL;
	m_vertexBitangents = NULL;
	m_vertexColors = NULL;
	m_vertexTextureCoordinates = NULL;
	m_overlayTextureCoordinates = NULL;
	m_vertexBoneWeights = NULL;
	m_vertexBoneIndices = NULL;
	m_vertexPointSizes = NULL;
	m_vertexIndices = NULL;
	m_faces = NULL;
}

CC3Mesh::~CC3Mesh()
{
	CC_SAFE_RELEASE( m_vertexLocations );
	CC_SAFE_RELEASE( m_vertexNormals );
	CC_SAFE_RELEASE( m_vertexTangents );
	CC_SAFE_RELEASE( m_vertexBitangents );
	CC_SAFE_RELEASE( m_vertexColors );
	CC_SAFE_RELEASE( m_vertexTextureCoordinates );
	CC_SAFE_RELEASE( m_overlayTextureCoordinates );
	CC_SAFE_RELEASE( m_vertexBoneWeights );
	CC_SAFE_RELEASE( m_vertexBoneIndices );
	CC_SAFE_RELEASE( m_vertexPointSizes );
	CC_SAFE_RELEASE( m_vertexIndices );
	CC_SAFE_RELEASE( m_faces );
}

void CC3Mesh::setName( const std::string& name )
{
	super::setName( name );

	if ( m_vertexLocations )
		m_vertexLocations->deriveNameFrom( this );
	if ( m_vertexNormals )
		m_vertexNormals->deriveNameFrom( this );
	if ( m_vertexTangents )
		m_vertexTangents->deriveNameFrom( this );
	if ( m_vertexBitangents )
		m_vertexBitangents->deriveNameFrom( this );
	if ( m_vertexColors )
		m_vertexColors->deriveNameFrom( this );
	if ( m_vertexTextureCoordinates )
		m_vertexTextureCoordinates->deriveNameFrom( this );
	if ( m_vertexBoneIndices )
		m_vertexBoneIndices->deriveNameFrom( this );
	if ( m_vertexBoneWeights )
		m_vertexBoneWeights->deriveNameFrom( this );
	if ( m_vertexPointSizes )
		m_vertexPointSizes->deriveNameFrom( this );
	if ( m_vertexIndices )
		m_vertexIndices->deriveNameFrom( this );

	CCObject* pObj;
	CCARRAY_FOREACH( m_overlayTextureCoordinates, pObj )
	{
		CC3VertexTextureCoordinates* otc = (CC3VertexTextureCoordinates*)pObj;
		otc->deriveNameFrom( this );
	}
}

std::string CC3Mesh::getNameSuffix()
{
	return "Mesh"; 
}

CC3VertexLocations* CC3Mesh::getVertexLocations() 
{ 
	return m_vertexLocations; 
}

void CC3Mesh::setVertexLocations( CC3VertexLocations* vtxLocs )
{
	if (vtxLocs == m_vertexLocations) 
		return;
	
	CC_SAFE_RELEASE(m_vertexLocations);
	m_vertexLocations = vtxLocs;
	CC_SAFE_RETAIN( vtxLocs );

	if ( m_vertexLocations )
		m_vertexLocations->deriveNameFrom( this );
}

bool CC3Mesh::hasVertexLocations()
{
	return (m_vertexLocations != NULL); 
}

CC3VertexNormals* CC3Mesh::getVertexNormals()
{
	return m_vertexNormals; 
}

void CC3Mesh::setVertexNormals( CC3VertexNormals* vtxNorms )
{
	if (vtxNorms == m_vertexNormals)
		return;

	CC_SAFE_RELEASE(m_vertexNormals);
	m_vertexNormals= vtxNorms;
	CC_SAFE_RETAIN(vtxNorms);

	if ( m_vertexNormals )
		m_vertexNormals->deriveNameFrom( this );
}

bool CC3Mesh::hasVertexNormals()
{
	return (m_vertexNormals != NULL); 
}

void CC3Mesh::flipNormals()
{
	m_vertexNormals->flipNormals(); 
}

CC3VertexTangents* CC3Mesh::getVertexTangents()
{
	return m_vertexTangents; 
}

void CC3Mesh::setVertexTangents( CC3VertexTangents* vtxTans )
{
	if (vtxTans == m_vertexTangents) 
		return;

	CC_SAFE_RELEASE(m_vertexTangents);
	m_vertexTangents = vtxTans;
	CC_SAFE_RETAIN(vtxTans);

	if ( m_vertexTangents )
		m_vertexTangents->deriveNameFrom( this );
}

bool CC3Mesh::hasVertexTangents()
{
	return (m_vertexTangents != NULL); 
}

CC3VertexTangents* CC3Mesh::getVertexBitangents()
{
	return m_vertexBitangents; 
}

void CC3Mesh::setVertexBitangents( CC3VertexTangents* vtxBitans )
{
	if (vtxBitans == m_vertexBitangents) 
		return;

	CC_SAFE_RELEASE(m_vertexBitangents);
	m_vertexBitangents = vtxBitans;
	CC_SAFE_RETAIN(vtxBitans);

	if ( m_vertexBitangents )
	{
		m_vertexBitangents->deriveNameFrom( this, "Bitangents" );
		m_vertexBitangents->setSemantic( kCC3SemanticVertexBitangent );
	}
}

bool CC3Mesh::hasVertexBitangents()
{
	return (m_vertexBitangents != NULL); 
}

CC3VertexColors* CC3Mesh::getVertexColors()
{
	return m_vertexColors; 
}

void CC3Mesh::setVertexColors( CC3VertexColors* vtxCols )
{
	if (vtxCols == m_vertexColors) 
		return;

	CC_SAFE_RELEASE(m_vertexColors);
	m_vertexColors = vtxCols;
	CC_SAFE_RETAIN(vtxCols);

	if ( m_vertexColors )
		m_vertexColors->deriveNameFrom( this );
}

bool CC3Mesh::hasVertexColors()
{
	return (m_vertexColors != NULL); 
}

GLenum CC3Mesh::getVertexColorType()
{
	return m_vertexColors ? m_vertexColors->getElementType() : GL_FALSE; 
}

CC3VertexBoneIndices* CC3Mesh::getVertexBoneIndices()
{
	return m_vertexBoneIndices; 
}

void CC3Mesh::setVertexBoneIndices( CC3VertexBoneIndices* vertexBoneIndices )
{
	if (vertexBoneIndices == m_vertexBoneIndices) 
		return;

	CC_SAFE_RELEASE(m_vertexBoneIndices);
	m_vertexBoneIndices = vertexBoneIndices;
	CC_SAFE_RETAIN(vertexBoneIndices);

	if (m_vertexBoneIndices)
		m_vertexBoneIndices->deriveNameFrom( this );
}

bool CC3Mesh::hasVertexBoneIndices()
{
	return (m_vertexBoneIndices != NULL);
}

CC3VertexBoneWeights* CC3Mesh::getVertexBoneWeights()
{
	return m_vertexBoneWeights; 
}

void CC3Mesh::setVertexBoneWeights( CC3VertexBoneWeights* vertexBoneWeights )
{
	if (vertexBoneWeights == m_vertexBoneWeights) 
		return;

	CC_SAFE_RELEASE(m_vertexBoneWeights);
	m_vertexBoneWeights = vertexBoneWeights;
	CC_SAFE_RETAIN(vertexBoneWeights);

	if ( m_vertexBoneWeights )
		m_vertexBoneWeights->deriveNameFrom( this );
}

bool CC3Mesh::hasVertexBoneWeights()
{
	return (m_vertexBoneWeights != NULL); 
}

CC3VertexPointSizes* CC3Mesh::getVertexPointSizes()
{
	return m_vertexPointSizes; 
}

void CC3Mesh::setVertexPointSizes( CC3VertexPointSizes* vtxSizes )
{
	if (vtxSizes == m_vertexPointSizes) 
		return;

	CC_SAFE_RELEASE(m_vertexPointSizes);
	m_vertexPointSizes = vtxSizes;
	CC_SAFE_RETAIN(vtxSizes);

	if ( m_vertexPointSizes )
		m_vertexPointSizes->deriveNameFrom( this );
}

bool CC3Mesh::hasVertexPointSizes()
{
	return (m_vertexPointSizes != NULL); 
}

CC3VertexIndices* CC3Mesh::getVertexIndices()
{
	return m_vertexIndices; 
}

void CC3Mesh::setVertexIndices( CC3VertexIndices* vtxInd )
{
	if (vtxInd == m_vertexIndices) 
		return;

	CC_SAFE_RELEASE(m_vertexIndices);
	m_vertexIndices = vtxInd;
	CC_SAFE_RETAIN(vtxInd);

	if ( m_vertexIndices )
		m_vertexIndices->deriveNameFrom( this );
}

bool CC3Mesh::hasVertexIndices()
{
	return (m_vertexIndices != NULL); 
}

CC3VertexTextureCoordinates* CC3Mesh::getVertexTextureCoordinates()
{
	return m_vertexTextureCoordinates; 
}

void CC3Mesh::setVertexTextureCoordinates( CC3VertexTextureCoordinates* vtxTexCoords )
{
	if (vtxTexCoords == m_vertexTextureCoordinates) 
		return;

	CC_SAFE_RELEASE(m_vertexTextureCoordinates);
	m_vertexTextureCoordinates = vtxTexCoords;
	CC_SAFE_RETAIN(vtxTexCoords);

	if ( m_vertexTextureCoordinates )
		m_vertexTextureCoordinates->deriveNameFrom( this );
}

bool CC3Mesh::hasVertexTextureCoordinates()
{
	return (m_vertexTextureCoordinates != NULL); 
}

GLuint CC3Mesh::getTextureCoordinatesArrayCount()
{
	return (m_overlayTextureCoordinates ? (GLuint)m_overlayTextureCoordinates->count() : 0) + (m_vertexTextureCoordinates ? 1 : 0);
}

void CC3Mesh::addTextureCoordinates( CC3VertexTextureCoordinates* vtxTexCoords )
{
	CCAssert(vtxTexCoords, "Overlay texture cannot be nil");
	CCAssert(!m_overlayTextureCoordinates || ((m_overlayTextureCoordinates->count() + 1) <
											  CC3OpenGL::sharedGL()->getMaxNumberOfTextureUnits()),
			  "Too many overlaid textures. This platform only supports %i texture units."/*,
			  CC3OpenGL::sharedGL()->getMaxNumberOfTextureUnits()*/);
	
	// Set the first texture coordinates into vertexTextureCoordinates
	if (!m_vertexTextureCoordinates) 
	{
		setVertexTextureCoordinates( vtxTexCoords );
	} 
	else 
	{
		// Add subsequent texture coordinate arrays to the array of overlayTextureCoordinates,
		// creating it first if necessary
		if( !m_overlayTextureCoordinates )	
		{
			m_overlayTextureCoordinates = CCArray::create();	// retained
			m_overlayTextureCoordinates->retain();
		}
		m_overlayTextureCoordinates->addObject( vtxTexCoords );
		vtxTexCoords->deriveNameFrom( this );
	}
}

void CC3Mesh::removeTextureCoordinates( CC3VertexTextureCoordinates* aTexCoord )
{
	// LogTrace(@"Removing %@ from %@", aTexCoord, self);
	
	// If the array to be removed is actually the vertexTextureCoordinates, remove it
	if (m_vertexTextureCoordinates == aTexCoord) 
	{
		setVertexTextureCoordinates( NULL );
	} else {
		// Otherwise, find it in the array of overlays and remove it,
		// and remove the overlay array if it is now empty
		if (m_overlayTextureCoordinates && aTexCoord) 
		{
			m_overlayTextureCoordinates->removeObject( aTexCoord );
			if (m_overlayTextureCoordinates->count() == 0) 
			{
				m_overlayTextureCoordinates->release();
				m_overlayTextureCoordinates = NULL;
			}
		}
	}
}

void CC3Mesh::removeAllTextureCoordinates()
{
	// Remove the first texture coordinates
	setVertexTextureCoordinates( NULL );
	
	if ( m_overlayTextureCoordinates )
	{
		// Remove the overlay texture coordinates
		CCArray* myOTCs = (CCArray*)m_overlayTextureCoordinates->copy();
		CCObject* pObj;
		CCARRAY_FOREACH( myOTCs, pObj )
		{
			CC3VertexTextureCoordinates* otc = (CC3VertexTextureCoordinates*)pObj;
			removeTextureCoordinates( otc );
		}
		myOTCs->release();
	}
}

CC3VertexTextureCoordinates* CC3Mesh::getTextureCoordinatesNamed( const std::string& aName )
{
	std::string tcName = "";
	
	// First check if the first texture coordinates is the one
	if (m_vertexTextureCoordinates) {
		tcName = m_vertexTextureCoordinates->getName();
		if (tcName == aName || (tcName.empty() && aName.empty())) {		// Name equal or both nil.
			return m_vertexTextureCoordinates;
		}
	}
	// Then look for it in the overlays array
	CCObject* pObj;
	CCARRAY_FOREACH( m_overlayTextureCoordinates, pObj )
	{
		CC3VertexTextureCoordinates* otc = (CC3VertexTextureCoordinates*)pObj;
		tcName = otc->getName();
		if (tcName==aName || (tcName.empty() && aName.empty()))  // Name equal or both nil.
		{		
			return otc;
		}
	}
	
	return NULL;
}

// If first texture unit, return vertexTextureCoordinates property.
// Otherwise, if texUnit within bounds of overlays, get overlay.
// Otherwise, look up the texture coordinates for the previous texture unit
// recursively until one is found, or we reach first texture unit.
CC3VertexTextureCoordinates* CC3Mesh::getTextureCoordinatesForTextureUnit( GLuint texUnit )
{
	if ( texUnit == 0) 
		return m_vertexTextureCoordinates;
	else if (texUnit < getTextureCoordinatesArrayCount()) 
		return (CC3VertexTextureCoordinates*)m_overlayTextureCoordinates->objectAtIndex( texUnit - 1 );
	
	return getTextureCoordinatesForTextureUnit( texUnit - 1 );
}

void CC3Mesh::setTextureCoordinates( CC3VertexTextureCoordinates* aTexCoords, GLuint texUnit )
{
	CCAssert(aTexCoords, "Overlay texture coordinates cannot be nil");
	if (texUnit == 0) 
		setVertexTextureCoordinates( aTexCoords );
	else if (texUnit < getTextureCoordinatesArrayCount())
		m_overlayTextureCoordinates->replaceObjectAtIndex( texUnit - 1,  aTexCoords );
	else 
		addTextureCoordinates( aTexCoords );
}

CC3VertexArray* CC3Mesh::getVertexArrayForSemantic( GLenum semantic, GLuint semanticIndex )
{
	switch ( semantic ) 
	{
		case kCC3SemanticVertexLocation: 
			return getVertexLocations();
		case kCC3SemanticVertexNormal: 
			return getVertexNormals();
		case kCC3SemanticVertexTangent: 
			return getVertexTangents();
		case kCC3SemanticVertexBitangent: 
			return getVertexBitangents();
		case kCC3SemanticVertexColor: 
			return getVertexColors();
		case kCC3SemanticVertexBoneWeights: 
			return getVertexBoneWeights();
		case kCC3SemanticVertexBoneIndices: 
			return getVertexBoneIndices();
		case kCC3SemanticVertexPointSize: 
			return getVertexPointSizes();
		case kCC3SemanticVertexTexture: 
			return getTextureCoordinatesForTextureUnit( semanticIndex );
		default: 
			return NULL;
	}
}

bool CC3Mesh::shouldInterleaveVertices()
{
	return m_shouldInterleaveVertices; 
}

void CC3Mesh::setShouldInterleaveVertices( bool shouldInterleave )
{
	m_shouldInterleaveVertices = shouldInterleave;
	if ( !m_shouldInterleaveVertices )
	{
		CC3_TRACE("CC3Mesh has been configured to use non-interleaved vertex content. "
			"To improve performance, it is recommended that you interleave all vertex content, "
			"unless you need to frequently update one type of vertex content without updating the others.");
	}
}

CC3VertexContent CC3Mesh::getVertexContentTypes()
{
	unsigned int vtxContent = kCC3VertexContentNone;
	if (hasVertexLocations()) vtxContent |= kCC3VertexContentLocation;
	if (hasVertexNormals()) vtxContent |= kCC3VertexContentNormal;
	if (hasVertexTangents()) vtxContent |= kCC3VertexContentTangent;
	if (hasVertexBitangents()) vtxContent |= kCC3VertexContentBitangent;
	if (hasVertexColors()) vtxContent |= kCC3VertexContentColor;
	if (hasVertexTextureCoordinates()) vtxContent |= kCC3VertexContentTextureCoordinates;
	if (hasVertexBoneWeights()) vtxContent |= kCC3VertexContentBoneWeights;
	if (hasVertexBoneIndices()) vtxContent |= kCC3VertexContentBoneIndices;
	if (hasVertexPointSizes()) vtxContent |= kCC3VertexContentPointSize;
	return (CC3VertexContent)vtxContent;
}

void CC3Mesh::setVertexContentTypes( CC3VertexContent vtxContentTypes )
{
	createVertexContent( vtxContentTypes );
	updateVertexStride();
}

void CC3Mesh::createVertexContent( CC3VertexContent vtxContentTypes )
{
	// Always create a new vertex locations
	if (!m_vertexLocations) 
		setVertexLocations( CC3VertexLocations::vertexArray() );
	
	// Vertex normals
	if (vtxContentTypes & kCC3VertexContentNormal) {
		if (!m_vertexNormals)
			setVertexNormals( CC3VertexNormals::vertexArray() );
	} else {
		setVertexNormals( NULL );
	}
	
	// Vertex tangents
	if (vtxContentTypes & kCC3VertexContentTangent) {
		if (!m_vertexTangents) 
			setVertexTangents( CC3VertexTangents::vertexArray() );
	} else {
		setVertexTangents( NULL );
	}
	
	// Vertex bitangents
	if (vtxContentTypes & kCC3VertexContentBitangent) {
		if (!m_vertexBitangents) 
			setVertexBitangents( CC3VertexTangents::vertexArray() );
	} else {
		setVertexBitangents( NULL );
	}
	
	// Vertex colors
	if (vtxContentTypes & kCC3VertexContentColor) {
		if (!m_vertexColors) 
			setVertexColors( CC3VertexColors::vertexArray() );
	} else {
		setVertexColors( NULL );
	}
	
	// Vertex texture coordinates
	if (vtxContentTypes & kCC3VertexContentTextureCoordinates) {
		if (!m_vertexTextureCoordinates) 
			setVertexTextureCoordinates( CC3VertexTextureCoordinates::vertexArray() );
	} else {
		removeAllTextureCoordinates();
	}
	
	// Weights
	if (vtxContentTypes & kCC3VertexContentBoneWeights) {
		if (!m_vertexBoneWeights) 
			setVertexBoneWeights( CC3VertexBoneWeights::vertexArray() );
	} else {
		setVertexBoneWeights( NULL );
	}
	
	// Matrix indices
	if (vtxContentTypes & kCC3VertexContentBoneIndices) {
		if (!m_vertexBoneIndices) 
			setVertexBoneIndices( CC3VertexBoneIndices::vertexArray() );
	} else {
		setVertexBoneIndices( NULL );
	}
	
	// Point sizes
	if (vtxContentTypes & kCC3VertexContentPointSize) {
		if (!m_vertexPointSizes) 
			setVertexPointSizes( CC3VertexPointSizes::vertexArray() );
	} else {
		setVertexPointSizes( NULL );
	}
	
}

GLuint CC3Mesh::getVertexStride()
{
	GLuint stride = 0;
	if (m_vertexLocations) stride += m_vertexLocations->getElementLength();
	if (m_vertexNormals) stride += m_vertexNormals->getElementLength();
	if (m_vertexTangents) stride += m_vertexTangents->getElementLength();
	if (m_vertexBitangents) stride += m_vertexBitangents->getElementLength();
	if (m_vertexColors) stride += m_vertexColors->getElementLength();
	if (m_vertexBoneIndices) stride += m_vertexBoneIndices->getElementLength();
	if (m_vertexBoneWeights) stride += m_vertexBoneWeights->getElementLength();
	if (m_vertexPointSizes) stride += m_vertexPointSizes->getElementLength();
	if (m_vertexTextureCoordinates) stride += m_vertexTextureCoordinates->getElementLength();

	CCObject* pObj;
	CCARRAY_FOREACH( m_overlayTextureCoordinates, pObj )
	{
		CC3VertexTextureCoordinates* otc = (CC3VertexTextureCoordinates*)pObj;
		stride += otc->getElementLength();
		
	}
	return stride;
}

void CC3Mesh::setVertexStride( GLuint vtxStride )
{
	if ( !m_shouldInterleaveVertices ) 
		return;

	if ( m_vertexLocations )
		m_vertexLocations->setVertexStride( vtxStride );
	if ( m_vertexNormals )
		m_vertexNormals->setVertexStride( vtxStride );
	if ( m_vertexTangents )
		m_vertexTangents->setVertexStride( vtxStride );
	if ( m_vertexBitangents )
		m_vertexBitangents->setVertexStride( vtxStride );
	if ( m_vertexColors )
		m_vertexColors->setVertexStride( vtxStride );
	if ( m_vertexBoneIndices )
		m_vertexBoneIndices->setVertexStride( vtxStride );
	if ( m_vertexBoneWeights )
		m_vertexBoneWeights->setVertexStride( vtxStride );
	if ( m_vertexPointSizes )
		m_vertexPointSizes->setVertexStride( vtxStride );
	if ( m_vertexTextureCoordinates )
		m_vertexTextureCoordinates->setVertexStride( vtxStride );

	CCObject* pObj;
	CCARRAY_FOREACH( m_overlayTextureCoordinates, pObj )
	{
		CC3VertexTextureCoordinates* otc = (CC3VertexTextureCoordinates*)pObj;
		otc->setVertexStride( vtxStride );
	}
}

GLuint CC3Mesh::updateVertexStride()
{
	GLuint stride = 0;
	
	if (m_vertexLocations) {
		if (m_shouldInterleaveVertices) m_vertexLocations->setElementOffset( stride );
		stride += m_vertexLocations->getElementLength();
	}
	if (m_vertexNormals) {
		if (m_shouldInterleaveVertices) m_vertexNormals->setElementOffset( stride );
		stride += m_vertexNormals->getElementLength();
	}
	if (m_vertexTangents) {
		if (m_shouldInterleaveVertices) m_vertexTangents->setElementOffset( stride );
		stride += m_vertexTangents->getElementLength();
	}
	if (m_vertexBitangents) {
		if (m_shouldInterleaveVertices) m_vertexBitangents->setElementOffset( stride );
		stride += m_vertexBitangents->getElementLength();
	}
	if (m_vertexColors) {
		if (m_shouldInterleaveVertices) m_vertexColors->setElementOffset( stride );
		stride += m_vertexColors->getElementLength();
	}
	if (m_vertexTextureCoordinates) {
		if (m_shouldInterleaveVertices) m_vertexTextureCoordinates->setElementOffset( stride );
		stride += m_vertexTextureCoordinates->getElementLength();
	}

	CCObject* pObj;
	CCARRAY_FOREACH( m_overlayTextureCoordinates, pObj )
	{
		CC3VertexTextureCoordinates* otc = (CC3VertexTextureCoordinates*)pObj;
		if (m_shouldInterleaveVertices) 
			otc->setElementOffset( stride );
		stride += otc->getElementLength();
	}

	if (m_vertexBoneWeights) {
		if (m_shouldInterleaveVertices) m_vertexBoneWeights->setElementOffset( stride );
		stride += m_vertexBoneWeights->getElementLength();
	}
	if (m_vertexBoneIndices) {
		if (m_shouldInterleaveVertices) m_vertexBoneIndices->setElementOffset( stride );
		stride += m_vertexBoneIndices->getElementLength();
	}
	if (m_vertexPointSizes) {
		if (m_shouldInterleaveVertices) m_vertexPointSizes->setElementOffset( stride );
		stride += m_vertexPointSizes->getElementLength();
	}
	
	setVertexStride( stride );
	return stride;
}

GLuint CC3Mesh::getAllocatedVertexCapacity()
{ 
	return m_vertexLocations ? m_vertexLocations->getAllocatedVertexCapacity() : 0; 
}

void CC3Mesh::setAllocatedVertexCapacity( GLuint vtxCount )
{
	if (!m_vertexLocations) 
		setVertexLocations( CC3VertexLocations::vertexArray() );
    
	m_vertexLocations->setAllocatedVertexCapacity( vtxCount );
	
    if ( shouldInterleaveVertices() )
	{
		if ( m_vertexNormals )
			m_vertexNormals->interleaveWith( m_vertexLocations );
		if ( m_vertexTangents )
			m_vertexTangents->interleaveWith( m_vertexLocations );
		if ( m_vertexBitangents )
			m_vertexBitangents->interleaveWith( m_vertexLocations );
		if ( m_vertexColors )
			m_vertexColors->interleaveWith( m_vertexLocations );
		if ( m_vertexBoneIndices )
			m_vertexBoneIndices->interleaveWith( m_vertexLocations );
		if ( m_vertexBoneWeights )
			m_vertexBoneWeights->interleaveWith( m_vertexLocations );
		if ( m_vertexPointSizes )
			m_vertexPointSizes->interleaveWith( m_vertexLocations );
		if ( m_vertexTextureCoordinates )
			m_vertexTextureCoordinates->interleaveWith( m_vertexLocations );

		CCObject* pObj;
		CCARRAY_FOREACH( m_overlayTextureCoordinates, pObj )
		{
			CC3VertexTextureCoordinates* otc = (CC3VertexTextureCoordinates*)pObj;
			otc->interleaveWith( m_vertexLocations );
		}
	} 
	else 
	{
		if ( m_vertexNormals )
			m_vertexNormals->setAllocatedVertexCapacity( vtxCount );
		if ( m_vertexTangents )
			m_vertexTangents->setAllocatedVertexCapacity( vtxCount );
		if ( m_vertexBitangents )
			m_vertexBitangents->setAllocatedVertexCapacity( vtxCount );
		if ( m_vertexColors )
			m_vertexColors->setAllocatedVertexCapacity( vtxCount );
		if ( m_vertexBoneIndices )
			m_vertexBoneIndices->setAllocatedVertexCapacity( vtxCount );
		if ( m_vertexBoneWeights )
			m_vertexBoneWeights->setAllocatedVertexCapacity( vtxCount );
		if ( m_vertexPointSizes )
			m_vertexPointSizes->setAllocatedVertexCapacity( vtxCount );
		if ( m_vertexTextureCoordinates )
			m_vertexTextureCoordinates->setAllocatedVertexCapacity( vtxCount );

		CCObject* pObj;
		CCARRAY_FOREACH( m_overlayTextureCoordinates, pObj )
		{
			CC3VertexTextureCoordinates* otc = (CC3VertexTextureCoordinates*)pObj;
			otc->setAllocatedVertexCapacity( vtxCount );
		}
	}
}

bool CC3Mesh::ensureVertexCapacity( GLuint vtxCount )
{
	GLuint currVtxCap = getAllocatedVertexCapacity();
	if (currVtxCap > 0 && currVtxCap < vtxCount) 
	{
		setAllocatedVertexCapacity( (GLuint)(vtxCount * getCapacityExpansionFactor()) );
		return ( getAllocatedVertexCapacity() > currVtxCap );
	}
	return false;
}

bool CC3Mesh::ensureCapacity( GLuint vtxCount )
{
	return ensureVertexCapacity( vtxCount ); 
}

GLfloat CC3Mesh::getCapacityExpansionFactor()
{
	return m_capacityExpansionFactor;
}

GLvoid* CC3Mesh::interleavedVertices()
{
	return (m_shouldInterleaveVertices && m_vertexLocations) ? m_vertexLocations->getVertices() : NULL;
}

GLuint CC3Mesh::getAllocatedVertexIndexCapacity()
{ 
	return m_vertexIndices ? m_vertexIndices->getAllocatedVertexCapacity() : 0; 
}

void CC3Mesh::setAllocatedVertexIndexCapacity( GLuint vtxCount )
{
	if ( !m_vertexIndices && vtxCount > 0 ) 
		setVertexIndices( CC3VertexIndices::vertexArray() );
	m_vertexIndices->setAllocatedVertexCapacity( vtxCount );
}

void CC3Mesh::copyVertices( GLuint vtxCount, GLuint srcIdx, GLuint dstIdx )
{
	m_vertexLocations->copyVertices( vtxCount, srcIdx, dstIdx );
	if ( !m_shouldInterleaveVertices ) 
	{
		m_vertexNormals->copyVertices( vtxCount, srcIdx, dstIdx );
		m_vertexTangents->copyVertices( vtxCount, srcIdx, dstIdx );
		m_vertexBitangents->copyVertices( vtxCount, srcIdx, dstIdx );
		m_vertexColors->copyVertices( vtxCount, srcIdx, dstIdx );
		m_vertexBoneIndices->copyVertices( vtxCount, srcIdx, dstIdx );
		m_vertexBoneWeights->copyVertices( vtxCount, srcIdx, dstIdx );
		m_vertexPointSizes->copyVertices( vtxCount, srcIdx, dstIdx );
		m_vertexTextureCoordinates->copyVertices( vtxCount, srcIdx, dstIdx );

		CCObject* pObj;
		CCARRAY_FOREACH( m_overlayTextureCoordinates, pObj )
		{
			CC3VertexTextureCoordinates* otc = (CC3VertexTextureCoordinates*)pObj;
			otc->copyVertices( vtxCount, srcIdx, dstIdx );
		}
	}
}

void CC3Mesh::copyVertices( GLuint vtxCount, GLuint srcIdx, CC3Mesh* srcMesh, GLuint dstIdx )
{
	// If both meshes have the same interleaved content,
	// the copying can be optimized to a memory copy.
	if ((getVertexContentTypes() == srcMesh->getVertexContentTypes()) &&
		getVertexStride() == srcMesh->getVertexStride() &&
		(shouldInterleaveVertices() && srcMesh->shouldInterleaveVertices())) 
	{
		CC3_TRACE("using optimized memory copy");
		getVertexLocations()->copyVertices( vtxCount, srcMesh->interleavedVertices(), dstIdx );
	} else {
		// Can't optimize, so must default to copying vertex element by vertex element
		CC3_TRACE("using vertex-by-vertex copy");
		for (GLuint i = 0; i < vtxCount; i++) 
			copyVertexAt( srcIdx + i, srcMesh, dstIdx + i );
	}
}

void CC3Mesh::copyVertexAt( GLuint srcIdx, CC3Mesh* srcMesh, GLuint dstIdx )
{
	if (hasVertexLocations()) 
		setVertexLocation( srcMesh->getVertexLocationAt( srcIdx ), dstIdx );
	if (hasVertexNormals()) 
		setVertexNormal( srcMesh->getVertexNormalAt( srcIdx ), dstIdx );
	if (hasVertexTangents()) 
		setVertexTangent( srcMesh->getVertexTangentAt( srcIdx ), dstIdx );
	if (hasVertexBitangents()) 
		setVertexBitangent( srcMesh->getVertexBitangentAt( srcIdx ), dstIdx );
	if (hasVertexColors()) 
		setVertexColor4F( srcMesh->getVertexColor4FAt( srcIdx ), dstIdx );
	if (hasVertexBoneWeights()) 
		setVertexBoneWeights( srcMesh->getVertexBoneWeightsAt( srcIdx ), dstIdx );
	if (hasVertexBoneIndices()) 
		setVertexBoneIndices( srcMesh->getVertexBoneIndicesAt( srcIdx ), dstIdx );
	if (hasVertexPointSizes()) 
		setVertexPointSize( srcMesh->getVertexPointSizeAt( srcIdx ), dstIdx );
	GLuint tcCount = getTextureCoordinatesArrayCount();
	for (GLuint i = 0; i < tcCount; i++) 
	{
		setVertexTexCoord2F( srcMesh->getVertexTexCoord2FForTextureUnit( i, srcIdx ), i, dstIdx );
	}
}

void CC3Mesh::copyVertexIndices( GLuint vtxCount, GLuint srcIdx, GLuint dstIdx, GLint offset )
{
	m_vertexIndices->copyVertices( vtxCount, srcIdx, dstIdx, offset );
}

void CC3Mesh::copyVertexIndices( GLuint vtxCount, GLuint srcIdx, CC3Mesh* srcMesh, GLuint dstIdx, GLint offset )
{
	if ( !m_vertexIndices ) 
		return;	// If there are no vertex indices, leave
	
	CC3VertexIndices* srcVtxIdxs = srcMesh->getVertexIndices();
	if (srcVtxIdxs) 
	{
		// If the template mesh has vertex indices, copy them over and offset them.
		// If both vertex index arrays are of the same type, we can optimize to a fast copy.
		if (srcVtxIdxs->getElementType() == m_vertexIndices->getElementType())
		{
			m_vertexIndices->copyVertices( vtxCount, srcVtxIdxs->getAddressOfElement( srcIdx ), dstIdx,  offset );
		} else {
			for (GLuint vtxIdx = 0; vtxIdx < vtxCount; vtxIdx++) 
			{
				GLuint srcVtx = srcVtxIdxs->getIndexAt( srcIdx + vtxIdx );
				m_vertexIndices->setIndex( srcVtx + offset, dstIdx + vtxIdx );
			}
		}
	} 
	else 
	{
		// If the source mesh does NOT have vertex indices, manufacture one for each vertex,
		// simply pointing directly to that vertex, taking the offset into consideration.
		// There will be a 1:1 mapping of indices to vertices.
		for (GLuint vtxIdx = 0; vtxIdx < vtxCount; vtxIdx++) 
		{
			m_vertexIndices->setIndex( offset + vtxIdx, dstIdx + vtxIdx );
		}
	}
}

GLuint CC3Mesh::getVertexCount()
{
	return m_vertexLocations ? m_vertexLocations->getVertexCount() : 0; 
}

void CC3Mesh::setVertexCount( GLuint vCount )
{
	// If we're attempting to set too many vertices for indexed drawing, log an error, but don't abort.
	if(m_vertexIndices && (vCount > (kCC3MaxGLushort + 1)))
		CCLOGERROR("Setting vertexCount property of CC3Mesh to %d vertices. This mesh uses indexed drawing, which is limited by OpenGL ES to %d vertices."
				 " Vertices beyond that limit will not be drawn.", vCount, (kCC3MaxGLushort + 1));
	
	if ( m_vertexLocations )
		m_vertexLocations->setVertexCount( vCount );
	
	if ( m_vertexNormals )
		m_vertexNormals->setVertexCount( vCount );
	
	if ( m_vertexTangents )
		m_vertexTangents->setVertexCount( vCount );
	
	if ( m_vertexBitangents )
		m_vertexBitangents->setVertexCount( vCount );
	
	if ( m_vertexColors )
		m_vertexColors->setVertexCount( vCount );

	if ( m_vertexBoneIndices )
		m_vertexBoneIndices->setVertexCount( vCount );
	
	if ( m_vertexBoneWeights )
		m_vertexBoneWeights->setVertexCount( vCount );
	
	if ( m_vertexPointSizes )
		m_vertexPointSizes->setVertexCount( vCount );

	if ( m_vertexTextureCoordinates )
		m_vertexTextureCoordinates->setVertexCount( vCount );

	CCObject* pObj;
	CCARRAY_FOREACH( m_overlayTextureCoordinates, pObj )
	{
		CC3VertexTextureCoordinates* otc = (CC3VertexTextureCoordinates*)pObj;
		otc->setVertexCount( vCount );
	}
}

GLuint CC3Mesh::getVertexIndexCount()
{ 
	return m_vertexIndices ? m_vertexIndices->getVertexCount() :getVertexCount(); 
}

void CC3Mesh::setVertexIndexCount( GLuint vCount )
{
	if ( m_vertexIndices )
		m_vertexIndices->setVertexCount( vCount ); 
}

CC3Vector CC3Mesh::getVertexLocationAt( GLuint index )
{
	return m_vertexLocations ? m_vertexLocations->getLocationAt( index ) : CC3Vector::kCC3VectorZero;
}

void CC3Mesh::setVertexLocation( const CC3Vector& aLocation, GLuint index )
{
	m_vertexLocations->setLocation( aLocation, index );
}

CC3Vector4 CC3Mesh::getVertexHomogeneousLocationAt( GLuint index )
{
	return m_vertexLocations ? m_vertexLocations->getHomogeneousLocationAt( index ) : CC3Vector4::kCC3Vector4ZeroLocation;
}

void CC3Mesh::setVertexHomogeneousLocation( const CC3Vector4& aLocation, GLuint index )
{
	m_vertexLocations->setHomogeneousLocation( aLocation, index );
}

CC3Vector CC3Mesh::getVertexNormalAt( GLuint index )
{
	return m_vertexNormals ? m_vertexNormals->getNormalAt( index ) : CC3Vector::kCC3VectorUnitZPositive;
}

void CC3Mesh::setVertexNormal( const CC3Vector& aNormal, GLuint index )
{
	if ( m_vertexNormals )
		m_vertexNormals->setNormal( aNormal, index );
}

CC3Vector CC3Mesh::getVertexTangentAt( GLuint index )
{
	return m_vertexTangents ? m_vertexTangents->getTangentAt( index ) : CC3Vector::kCC3VectorUnitXPositive;
}

void CC3Mesh::setVertexTangent( const CC3Vector& aTangent, GLuint index )
{
	if ( m_vertexTangents ) 
		m_vertexTangents->setTangent( aTangent, index );
}

CC3Vector CC3Mesh::getVertexBitangentAt( GLuint index )
{
	return m_vertexBitangents ? m_vertexBitangents->getTangentAt( index ) : CC3Vector::kCC3VectorUnitYPositive;
}

void CC3Mesh::setVertexBitangent( const CC3Vector& aTangent, GLuint index )
{
	if ( m_vertexBitangents )
		m_vertexBitangents->setTangent( aTangent, index );
}

ccColor4F CC3Mesh::getVertexColor4FAt( GLuint index )
{
	return m_vertexColors ? m_vertexColors->getColor4FAt( index ) : kCCC4FBlackTransparent;
}

void CC3Mesh::setVertexColor4F( const ccColor4F& aColor, GLuint index )
{
	if ( m_vertexColors ) 
		m_vertexColors->setColor4F( aColor, index );
}

ccColor4B CC3Mesh::getVertexColor4BAt( GLuint index )
{
	return m_vertexColors ? m_vertexColors->getColor4BAt( index ) : ccc4(0, 0, 0, 0);
}

void CC3Mesh::setVertexColor4B( const ccColor4B& aColor, GLuint index )
{
	if ( m_vertexColors )
		m_vertexColors->setColor4B( aColor, index );
}

GLuint CC3Mesh::getVertexBoneCount()
{ 
	return m_vertexBoneWeights ? m_vertexBoneWeights->getElementSize() : 0; 
}

GLfloat CC3Mesh::getVertexWeightForBoneInfluence( GLuint influenceIndex, GLuint vtxIndex )
{
	return m_vertexBoneWeights ? m_vertexBoneWeights->getWeightForBoneInfluence( influenceIndex, vtxIndex ) : 0.0f;
}

void CC3Mesh::setVertexWeight( GLfloat weight, GLuint influenceIndex, GLuint vtxIndex )
{
	if ( m_vertexBoneWeights )
		m_vertexBoneWeights->setWeight( weight, influenceIndex, vtxIndex );
}

GLfloat* CC3Mesh::getVertexBoneWeightsAt( GLuint vtxIndex )
{
	return m_vertexBoneWeights ? m_vertexBoneWeights->getBoneWeightsAt( vtxIndex ) : NULL;
}

void CC3Mesh::setVertexBoneWeights( GLfloat* weights, GLuint vtxIndex )
{
	if ( m_vertexBoneWeights )
		m_vertexBoneWeights->setBoneWeights( weights, vtxIndex );
}

GLuint CC3Mesh::getVertexBoneIndexForBoneInfluence( GLuint influenceIndex, GLuint vtxIndex )
{
	return m_vertexBoneIndices ? m_vertexBoneIndices->getBoneIndexForBoneInfluence( influenceIndex, vtxIndex ) : 0;
}

void CC3Mesh::setVertexBoneIndex( GLuint boneIndex, GLuint influenceIndex, GLuint vtxIndex )
{
	if ( m_vertexBoneIndices )
		m_vertexBoneIndices->setBoneIndex( boneIndex, influenceIndex, vtxIndex );
}

GLvoid* CC3Mesh::getVertexBoneIndicesAt( GLuint vtxIndex )
{
	return m_vertexBoneIndices ? m_vertexBoneIndices->getBoneIndicesAt(vtxIndex) : NULL;
}

void CC3Mesh::setVertexBoneIndices( GLvoid* boneIndices, GLuint vtxIndex )
{
	if ( m_vertexBoneIndices )
		m_vertexBoneIndices->setBoneIndices( boneIndices, vtxIndex );
}

GLenum CC3Mesh::getVertexBoneIndexType()
{
	return m_vertexBoneIndices->getElementType(); 
}

GLfloat CC3Mesh::getVertexPointSizeAt( GLuint vtxIndex )
{
	return m_vertexPointSizes ? m_vertexPointSizes->getPointSizeAt( vtxIndex ) : 0.0f;
}

void CC3Mesh::setVertexPointSize( GLfloat aSize, GLuint vtxIndex )
{
	if ( m_vertexPointSizes )
		m_vertexPointSizes->setPointSize( aSize, vtxIndex );
}

void CC3Mesh::updatePointSizesGLBuffer()
{ 
	if ( m_vertexPointSizes )
		m_vertexPointSizes->updateGLBuffer(); 
}

ccTex2F CC3Mesh::getVertexTexCoord2FAt( GLuint index )
{
	return getVertexTexCoord2FForTextureUnit( 0 , index );
}

void CC3Mesh::setVertexTexCoord2F( const ccTex2F& aTex2F, GLuint index )
{
	setVertexTexCoord2F( aTex2F, 0, index );
}

ccTex2F CC3Mesh::getVertexTexCoord2FForTextureUnit( GLuint texUnit, GLuint index )
{
	CC3VertexTextureCoordinates* texCoords = getTextureCoordinatesForTextureUnit( texUnit );
	return texCoords ? texCoords->getTexCoord2FAt( index ) : tex2( 0.0f, 0.0f );
}

void CC3Mesh::setVertexTexCoord2F( const ccTex2F& aTex2F, GLuint texUnit, GLuint index )
{
	CC3VertexTextureCoordinates* texCoords = getTextureCoordinatesForTextureUnit( texUnit );
	if ( texCoords )
		texCoords->setTexCoord2F( aTex2F, index );
}

GLuint CC3Mesh::getVertexIndexAt( GLuint index )
{
	return m_vertexIndices ? m_vertexIndices->getIndexAt( index ) : 0;
}

void CC3Mesh::setVertexIndex( GLuint vertexIndex, GLuint index )
{
	if ( m_vertexIndices )
		m_vertexIndices->setIndex( vertexIndex, index );
}

CC3FaceArray* CC3Mesh::getFaces()
{
	if ( !m_faces ) 
	{
		std::string facesName = CC3String::stringWithFormat( (char*)"%s-Faces", getName().c_str() );
		setFaces( CC3FaceArray::faceArrayWithName( facesName ) );
	}
	return m_faces;
}

void CC3Mesh::setFaces( CC3FaceArray* aFaceArray )
{
	if (aFaceArray == m_faces) 
		return;
	
	CC_SAFE_RELEASE( m_faces );
	m_faces = aFaceArray;
	CC_SAFE_RETAIN( m_faces );

	if ( m_faces )
		m_faces->setMesh( this );
}

bool CC3Mesh::shouldCacheFaces()
{ 
	return m_faces ? m_faces->shouldCacheFaces() : false; 
}

void CC3Mesh::setShouldCacheFaces( bool shouldCache )
{
	getFaces()->setShouldCacheFaces( shouldCache ); 
}

GLuint CC3Mesh::getFaceCount()
{
	if (m_vertexIndices) 
		return m_vertexIndices->getFaceCount();

	if (m_vertexLocations) 
		return m_vertexLocations->getFaceCount();

	return 0;
}

CC3Face CC3Mesh::getFaceFromIndices( const CC3FaceIndices& faceIndices )
{
	return m_vertexLocations ? m_vertexLocations->getFaceFromIndices( faceIndices ) : CC3Face::kCC3FaceZero;
}

CC3FaceIndices CC3Mesh::getUncachedFaceIndicesAt( GLuint faceIndex )
{
	if (m_vertexIndices) 
		return m_vertexIndices->getFaceIndicesAt( faceIndex );
	if (m_vertexLocations) 
		return m_vertexLocations->getFaceIndicesAt( faceIndex );
	CCAssert(false, "CC3Mesh has no drawable vertex array and cannot retrieve indices for a face.");

	return kCC3FaceIndicesZero;
}

GLuint CC3Mesh::getFaceCountFromVertexIndexCount( GLuint vc )
{
	if (m_vertexIndices) 
		return m_vertexIndices->getFaceCountFromVertexIndexCount( vc );
	if (m_vertexLocations) 
		return m_vertexLocations->getFaceCountFromVertexIndexCount( vc );

	CCAssert(false, "CC3Mesh has no drawable vertex array and cannot convert vertex count to face count.");
	
	return 0;
}

GLuint CC3Mesh::getVertexIndexCountFromFaceCount( GLuint fc )
{
	if (m_vertexIndices) 
		return m_vertexIndices->getVertexIndexCountFromFaceCount(fc);
	if (m_vertexLocations) 
		return m_vertexLocations->getVertexIndexCountFromFaceCount(fc);
	CCAssert(false, "CC3Mesh has no drawable vertex array and cannot convert face count to vertex count.");
	return 0;
}

CC3Face CC3Mesh::getFaceAt( GLuint faceIndex )
{
	return getFaceFromIndices( getFaceIndicesAt( faceIndex ) );
}

CC3FaceIndices CC3Mesh::getFaceIndicesAt( GLuint faceIndex )
{
	return getFaces()->getIndicesAt( faceIndex ); 
}

CC3Vector CC3Mesh::getFaceCenterAt( GLuint faceIndex )
{
	return getFaces()->getCenterAt( faceIndex ); 
}

CC3Vector CC3Mesh::getFaceNormalAt( GLuint faceIndex )
{
	return getFaces()->getNormalAt( faceIndex ); 
}

CC3Plane CC3Mesh::getFacePlaneAt( GLuint faceIndex )
{
	return getFaces()->getPlaneAt( faceIndex ); 
}

CC3FaceNeighbours CC3Mesh::getFaceNeighboursAt( GLuint faceIndex )
{
	return getFaces()->getNeighboursAt( faceIndex ); 
}

GLuint CC3Mesh::findFirst( GLuint maxHitCount, CC3MeshIntersection* intersections, 
	const CC3Ray& aRay, bool acceptBackFaces, bool acceptBehind )
{	
	GLuint hitIdx = 0;
	GLuint faceCount = getFaceCount();
	for (GLuint faceIdx = 0; faceIdx < faceCount && hitIdx < maxHitCount; faceIdx++) 
	{
		CC3MeshIntersection* hit = &intersections[hitIdx];
		hit->faceIndex = faceIdx;
		hit->face = getFaceAt( faceIdx );
        hit->facePlane = CC3Plane::planeFromFace( hit->face );
		
		// Check if the ray is not parallel to the face, is approaching from the front,
		// or is approaching from the back and that is okay.
		GLfloat dirDotNorm = aRay.direction.dot( hit->facePlane.getNormal() );
		hit->wasBackFace = dirDotNorm > 0.0f;
		if (dirDotNorm < 0.0f || (hit->wasBackFace && acceptBackFaces)) {
			
			// Find the point of intersection of the ray with the plane
			// and check that it is not behind the start of the ray.
			CC3Vector4 loc4 = CC3RayIntersectionWithPlane(aRay, hit->facePlane);
			if (acceptBehind || loc4.w >= 0.0f) {
				hit->location = loc4.cc3Vector();
				hit->distance = loc4.w;
				hit->barycentricLocation = CC3FaceBarycentricWeights(hit->face, hit->location);
				if ( CC3BarycentricWeightsAreInsideTriangle(hit->barycentricLocation) ) 
					hitIdx++;
			}
		}
	}
	return hitIdx;
}

/**
 * If the interleavesVertices property is set to NO, creates GL vertex buffer objects for all
 * vertex arrays used by this mesh by invoking createGLBuffer on each contained vertex array.
 *
 * If the shouldInterleaveVertices property is set to YES, indicating that the underlying data is
 * shared across the contained vertex arrays, this method invokes createGLBuffer only on the
 * vertexLocations and vertexIndices vertex arrays, and copies the bufferID property from
 * the vertexLocations vertex array to the other vertex arrays (except vertexIndicies).
 */
void CC3Mesh::createGLBuffers()
{
	if ( m_vertexLocations )
		m_vertexLocations->createGLBuffer();
	
	if (m_shouldInterleaveVertices) 
	{
		if ( m_vertexLocations )
		{
			GLuint commonBufferId = m_vertexLocations->getBufferID();
			if ( m_vertexNormals )
				m_vertexNormals->setBufferID( commonBufferId );
			if ( m_vertexTangents )
				m_vertexTangents->setBufferID( commonBufferId );
			if ( m_vertexBitangents )
				m_vertexBitangents->setBufferID( commonBufferId );
			if ( m_vertexColors )
				m_vertexColors->setBufferID( commonBufferId );
			if ( m_vertexBoneIndices )
				m_vertexBoneIndices->setBufferID( commonBufferId );
			if ( m_vertexBoneWeights )
				m_vertexBoneWeights->setBufferID( commonBufferId );
			if ( m_vertexPointSizes )
				m_vertexPointSizes->setBufferID( commonBufferId );
			if ( m_vertexTextureCoordinates )
				m_vertexTextureCoordinates->setBufferID( commonBufferId );
			CCObject* pObj;
			CCARRAY_FOREACH( m_overlayTextureCoordinates, pObj )
			{
				CC3VertexTextureCoordinates* otc = (CC3VertexTextureCoordinates*)pObj;
				otc->setBufferID( commonBufferId );
			}
		}
	} 
	else 
	{
		if ( m_vertexNormals )
			m_vertexNormals->createGLBuffer();
		
		if ( m_vertexTangents )
			m_vertexTangents->createGLBuffer();
		
		if ( m_vertexBitangents )
			m_vertexBitangents->createGLBuffer();
		
		if ( m_vertexColors )
			m_vertexColors->createGLBuffer();
		
		if ( m_vertexBoneIndices )
			m_vertexBoneIndices->createGLBuffer();
		
		if ( m_vertexBoneWeights )
			m_vertexBoneWeights->createGLBuffer();
		
		if ( m_vertexPointSizes )
			m_vertexPointSizes->createGLBuffer();
		
		if ( m_vertexTextureCoordinates )
			m_vertexTextureCoordinates->createGLBuffer();

		CCObject* pObj;
		CCARRAY_FOREACH( m_overlayTextureCoordinates, pObj )
		{
			CC3VertexTextureCoordinates* otc = (CC3VertexTextureCoordinates*)pObj;
			otc->createGLBuffer();
		}
	}

	if ( m_vertexIndices )
		m_vertexIndices->createGLBuffer();
}

void CC3Mesh::deleteGLBuffers()
{
	if ( m_vertexLocations )
		m_vertexLocations->deleteGLBuffer();
	
	if ( m_vertexNormals )
		m_vertexNormals->deleteGLBuffer();
	
	if ( m_vertexTangents )
		m_vertexTangents->deleteGLBuffer();
	
	if ( m_vertexBitangents )
		m_vertexBitangents->deleteGLBuffer();
	
	if ( m_vertexColors )
		m_vertexColors->deleteGLBuffer();
	
	if ( m_vertexBoneIndices )
		m_vertexBoneIndices->deleteGLBuffer();

	if ( m_vertexBoneWeights )
		m_vertexBoneWeights->deleteGLBuffer();
	
	if ( m_vertexPointSizes )
		m_vertexPointSizes->deleteGLBuffer();
	
	if ( m_vertexTextureCoordinates )
		m_vertexTextureCoordinates->deleteGLBuffer();
	CCObject* pObj;
	CCARRAY_FOREACH( m_overlayTextureCoordinates, pObj )
	{
		CC3VertexTextureCoordinates* otc = (CC3VertexTextureCoordinates*)pObj;
		otc->deleteGLBuffer();
	}

	if ( m_vertexIndices )
		m_vertexIndices->deleteGLBuffer();
}

bool CC3Mesh::isUsingGLBuffers()
{
	if (m_vertexLocations && m_vertexLocations->isUsingGLBuffer()) return true;
	if (m_vertexNormals && m_vertexNormals->isUsingGLBuffer()) return true;
	if (m_vertexTangents && m_vertexTangents->isUsingGLBuffer()) return true;
	if (m_vertexBitangents && m_vertexBitangents->isUsingGLBuffer()) return true;
	if (m_vertexColors && m_vertexColors->isUsingGLBuffer()) return true;
	if (m_vertexBoneIndices && m_vertexBoneIndices->isUsingGLBuffer()) return true;
	if (m_vertexBoneWeights && m_vertexBoneWeights->isUsingGLBuffer()) return true;
	if (m_vertexPointSizes && m_vertexPointSizes->isUsingGLBuffer()) return true;
	if (m_vertexTextureCoordinates && m_vertexTextureCoordinates->isUsingGLBuffer()) return true;
	CCObject* pObj;
	CCARRAY_FOREACH( m_overlayTextureCoordinates, pObj )
	{
		CC3VertexTextureCoordinates* otc = (CC3VertexTextureCoordinates*)pObj;
		 if (otc->isUsingGLBuffer()) 
			 return true;
	}
	
	return false;
}

void CC3Mesh::releaseRedundantContent()
{
	if ( m_vertexLocations )
		m_vertexLocations->releaseRedundantContent();
	if ( m_vertexNormals )
		m_vertexNormals->releaseRedundantContent();
	if ( m_vertexTangents )
		m_vertexTangents->releaseRedundantContent();
	if ( m_vertexBitangents )
		m_vertexBitangents->releaseRedundantContent();
	if ( m_vertexColors )
		m_vertexColors->releaseRedundantContent();
	if ( m_vertexBoneIndices )
		m_vertexBoneIndices->releaseRedundantContent();
	if ( m_vertexBoneWeights )
		m_vertexBoneWeights->releaseRedundantContent();
	if ( m_vertexPointSizes )
		m_vertexPointSizes->releaseRedundantContent();
	if ( m_vertexTextureCoordinates )
		m_vertexTextureCoordinates->releaseRedundantContent();
	CCObject* pObj;
	CCARRAY_FOREACH( m_overlayTextureCoordinates, pObj )
	{
		CC3VertexTextureCoordinates* otc = (CC3VertexTextureCoordinates*)pObj;
		otc->releaseRedundantContent();
	}

	if ( m_vertexIndices )
		m_vertexIndices->releaseRedundantContent();
}

void CC3Mesh::retainVertexContent()
{
	retainVertexLocations();
	retainVertexNormals();
	retainVertexTangents();
	retainVertexBitangents();
	retainVertexColors();
	retainVertexBoneIndices();
	retainVertexBoneWeights();
	retainVertexPointSizes();
	retainVertexTextureCoordinates();
}

void CC3Mesh::retainVertexLocations()
{
	if ( m_vertexLocations )
		m_vertexLocations->setShouldReleaseRedundantContent( false ); 
}

void CC3Mesh::retainVertexNormals()
{
	if ( !hasVertexNormals() ) return;
	
	if (m_shouldInterleaveVertices) 
		retainVertexLocations();

	if ( m_vertexNormals )
		m_vertexNormals->setShouldReleaseRedundantContent( false );
}

void CC3Mesh::retainVertexTangents()
{
	if ( !hasVertexTangents() ) return;
	
	if (m_shouldInterleaveVertices) 
		retainVertexLocations();

	if ( m_vertexTangents )
		m_vertexTangents->setShouldReleaseRedundantContent( false );
}

void CC3Mesh::retainVertexBitangents()
{
	if ( !hasVertexBitangents() ) 
		return;
	
	if (m_shouldInterleaveVertices) 
		retainVertexLocations();

	if ( m_vertexBitangents )
		m_vertexBitangents->setShouldReleaseRedundantContent( false );
}

void CC3Mesh::retainVertexColors()
{
	if ( !hasVertexColors() ) 
		return;
	
	if (m_shouldInterleaveVertices) 
		retainVertexLocations();

	if ( m_vertexColors )
		m_vertexColors->setShouldReleaseRedundantContent( false );
}

void CC3Mesh::retainVertexBoneWeights()
{
	if ( !hasVertexBoneWeights() )
		return;
	
	if (m_shouldInterleaveVertices) 
		retainVertexLocations();

	if ( m_vertexBoneWeights )
		m_vertexBoneWeights->setShouldReleaseRedundantContent( false );
}

void CC3Mesh::retainVertexBoneIndices()
{
	if ( !hasVertexBoneIndices() ) 
		return;
	
	if (m_shouldInterleaveVertices) 
		retainVertexLocations();

	if ( m_vertexBoneIndices )
		m_vertexBoneIndices->setShouldReleaseRedundantContent( false );
}

void CC3Mesh::retainVertexPointSizes()
{
	if ( !hasVertexPointSizes() ) 
		return;
	
	if (m_shouldInterleaveVertices) 
		retainVertexLocations();

	if ( m_vertexPointSizes ) 
		m_vertexPointSizes->setShouldReleaseRedundantContent( false );
}

void CC3Mesh::retainVertexTextureCoordinates()
{
	if ( !hasVertexTextureCoordinates() ) 
		return;
	
	if (m_shouldInterleaveVertices) 
		retainVertexLocations();

	if ( m_vertexTextureCoordinates )
		m_vertexTextureCoordinates->setShouldReleaseRedundantContent( false );

	CCObject* pObj;
	CCARRAY_FOREACH( m_overlayTextureCoordinates, pObj )
	{
		CC3VertexTextureCoordinates* otc = (CC3VertexTextureCoordinates*)pObj;
		otc->setShouldReleaseRedundantContent( false );
	}
}

void CC3Mesh::retainVertexIndices()
{
	if ( m_vertexIndices )
		m_vertexIndices->setShouldReleaseRedundantContent( false ); 
}

void CC3Mesh::doNotBufferVertexContent()
{
	doNotBufferVertexLocations();
	doNotBufferVertexNormals();
	doNotBufferVertexTangents();
	doNotBufferVertexBitangents();
	doNotBufferVertexColors();
	doNotBufferVertexBoneIndices();
	doNotBufferVertexBoneWeights();
	doNotBufferVertexPointSizes();
	doNotBufferVertexTextureCoordinates();
}

void CC3Mesh::doNotBufferVertexLocations()
{
	m_vertexLocations->setShouldAllowVertexBuffering( false ); 
}

void CC3Mesh::doNotBufferVertexNormals()
{
	if (m_shouldInterleaveVertices) 
		doNotBufferVertexLocations();
	m_vertexNormals->setShouldAllowVertexBuffering( false );
}

void CC3Mesh::doNotBufferVertexTangents()
{
	if (m_shouldInterleaveVertices) 
		doNotBufferVertexLocations();

	if ( m_vertexTangents )
		m_vertexTangents->setShouldAllowVertexBuffering( false );
}

void CC3Mesh::doNotBufferVertexBitangents()
{
	if (m_shouldInterleaveVertices) 
		doNotBufferVertexLocations();

	if ( m_vertexBitangents )
		m_vertexBitangents->setShouldAllowVertexBuffering( false );
}

void CC3Mesh::doNotBufferVertexColors()
{
	if (m_shouldInterleaveVertices) 
		doNotBufferVertexLocations();

	if ( m_vertexColors )
		m_vertexColors->setShouldAllowVertexBuffering( false );
}

void CC3Mesh::doNotBufferVertexBoneWeights()
{
	if (m_shouldInterleaveVertices) 
		doNotBufferVertexLocations();

	if ( m_vertexBoneWeights )
		m_vertexBoneWeights->setShouldAllowVertexBuffering( false );
}

void CC3Mesh::doNotBufferVertexBoneIndices()
{
	if (m_shouldInterleaveVertices) 
		doNotBufferVertexLocations();

	if ( m_vertexBoneIndices )
		m_vertexBoneIndices->setShouldAllowVertexBuffering( false );
}

void CC3Mesh::doNotBufferVertexPointSizes()
{
	if (m_shouldInterleaveVertices) 
		doNotBufferVertexLocations();

	if ( m_vertexPointSizes )
		m_vertexPointSizes->setShouldAllowVertexBuffering( false );
}

void CC3Mesh::doNotBufferVertexTextureCoordinates()
{
	if (m_shouldInterleaveVertices) 
		doNotBufferVertexLocations();

	if ( m_vertexTextureCoordinates )
		m_vertexTextureCoordinates->setShouldAllowVertexBuffering( false );

	CCObject* pObj;
	CCARRAY_FOREACH( m_overlayTextureCoordinates, pObj )
	{
		CC3VertexTextureCoordinates* otc = (CC3VertexTextureCoordinates*)pObj;
		otc->setShouldAllowVertexBuffering( false );
	}
}

void CC3Mesh::doNotBufferVertexIndices()
{ 
	if ( m_vertexIndices )
		m_vertexIndices->setShouldAllowVertexBuffering( false ); 
}

void CC3Mesh::updateGLBuffersStartingAt( GLuint offsetIndex, GLuint vertexCount )
{
	if ( m_vertexLocations )
		m_vertexLocations->updateGLBufferStartingAt( offsetIndex, vertexCount );

	if ( !m_shouldInterleaveVertices )
	{
		if ( m_vertexNormals )
			m_vertexNormals->updateGLBufferStartingAt( offsetIndex, vertexCount );
		if ( m_vertexTangents )
			m_vertexTangents->updateGLBufferStartingAt( offsetIndex, vertexCount );
		if ( m_vertexBitangents )
			m_vertexBitangents->updateGLBufferStartingAt( offsetIndex, vertexCount );
		if ( m_vertexColors )
			m_vertexColors->updateGLBufferStartingAt( offsetIndex, vertexCount );
		if ( m_vertexBoneIndices )
			m_vertexBoneIndices->updateGLBufferStartingAt( offsetIndex, vertexCount );
		if ( m_vertexBoneWeights )
			m_vertexBoneWeights->updateGLBufferStartingAt( offsetIndex, vertexCount );
		if ( m_vertexPointSizes )
			m_vertexPointSizes->updateGLBufferStartingAt( offsetIndex, vertexCount );
		if ( m_vertexTextureCoordinates )
			m_vertexTextureCoordinates->updateGLBufferStartingAt( offsetIndex, vertexCount );

		CCObject* pObj;
		CCARRAY_FOREACH( m_overlayTextureCoordinates, pObj )
		{
			CC3VertexTextureCoordinates* otc = (CC3VertexTextureCoordinates*)pObj;
			otc->updateGLBufferStartingAt( offsetIndex, vertexCount );
		}
	}
}

void CC3Mesh::updateGLBuffers()
{
	updateGLBuffersStartingAt( 0, getVertexCount() ); 
}

void CC3Mesh::updateVertexLocationsGLBuffer()
{
	if ( m_vertexLocations )
		m_vertexLocations->updateGLBuffer(); 
}

void CC3Mesh::updateVertexNormalsGLBuffer()
{
	if ( m_vertexNormals )
		m_vertexNormals->updateGLBuffer(); 
}

void CC3Mesh::updateVertexTangentsGLBuffer()
{
	if ( m_vertexTangents )
		m_vertexTangents->updateGLBuffer(); 
}

void CC3Mesh::updateVertexBitangentsGLBuffer()
{
	if ( m_vertexBitangents )
		m_vertexBitangents->updateGLBuffer(); 
}

void CC3Mesh::updateVertexColorsGLBuffer()
{
	if ( m_vertexColors )
		m_vertexColors->updateGLBuffer(); 
}

void CC3Mesh::updateVertexBoneWeightsGLBuffer()
{
	if ( m_vertexBoneWeights )
		m_vertexBoneWeights->updateGLBuffer(); 
}

void CC3Mesh::updateVertexBoneIndicesGLBuffer()
{
	if ( m_vertexBoneIndices )
		m_vertexBoneIndices->updateGLBuffer(); 
}

void CC3Mesh::updateVertexTextureCoordinatesGLBuffer()
{
	updateVertexTextureCoordinatesGLBufferForTextureUnit( 0 );
}

void CC3Mesh::updateVertexTextureCoordinatesGLBufferForTextureUnit( GLuint texUnit )
{
	getTextureCoordinatesForTextureUnit(texUnit)->updateGLBuffer();
}

void CC3Mesh::updateVertexIndicesGLBuffer()
{
	if ( m_vertexIndices )
		m_vertexIndices->updateGLBuffer(); 
}

CC3Vector CC3Mesh::getCenterOfGeometry()
{
	return m_vertexLocations ? m_vertexLocations->getCenterOfGeometry() : CC3Vector::kCC3VectorZero; 
}

CC3Box CC3Mesh::getBoundingBox()
{
	return m_vertexLocations ? m_vertexLocations->getBoundingBox() : CC3Box::kCC3BoxNull;
}

GLfloat CC3Mesh::getRadius()
{
	return m_vertexLocations ? m_vertexLocations->getRadius() : 0.0f; 
}

void CC3Mesh::moveMeshOriginTo( const CC3Vector& aLocation )
{ 
	if ( m_vertexLocations )
		m_vertexLocations->moveMeshOriginTo( aLocation ); 
}

void CC3Mesh::moveMeshOriginToCenterOfGeometry()
{
	if ( m_vertexLocations )
		m_vertexLocations->moveMeshOriginToCenterOfGeometry(); 
}


CCColorRef CC3Mesh::getColor()
{
	return m_vertexColors ? m_vertexColors->getColor() : CCColorRefFromCCC4F(kCCC4FBlackTransparent); 
}

void CC3Mesh::setColor( const CCColorRef& aColor )
{ 
	if ( m_vertexColors )
		m_vertexColors->setColor( aColor ); 
}

CCOpacity CC3Mesh::getOpacity()
{ 
	return m_vertexColors ? m_vertexColors->getOpacity() : kCCOpacityFull; 
}

void CC3Mesh::setOpacity( CCOpacity opacity )
{
	if ( m_vertexColors )
		m_vertexColors->setOpacity( opacity ); 
}

bool CC3Mesh::expectsVerticallyFlippedTextures()
{
	GLuint tcCount = getTextureCoordinatesArrayCount();
	for (GLuint texUnit = 0; texUnit < tcCount; texUnit++)
		if ( expectsVerticallyFlippedTextureInTextureUnit( texUnit ) ) 
			return true;
	return false;
}

void CC3Mesh::setExpectsVerticallyFlippedTextures( bool expectsFlipped )
{
	GLuint tcCount = getTextureCoordinatesArrayCount();
	for (GLuint texUnit = 0; texUnit < tcCount; texUnit++)
		setExpectsVerticallyFlippedTexture( expectsFlipped, texUnit );
}

bool CC3Mesh::expectsVerticallyFlippedTextureInTextureUnit( GLuint texUnit )
{
	return getTextureCoordinatesForTextureUnit( texUnit )->expectsVerticallyFlippedTextures();
}

void CC3Mesh::setExpectsVerticallyFlippedTexture( bool expectsFlipped, GLuint texUnit )
{
	getTextureCoordinatesForTextureUnit( texUnit )->setExpectsVerticallyFlippedTextures( expectsFlipped );
}

void CC3Mesh::alignTextureUnit( GLuint texUnit, CC3Texture* aTexture )
{
	// Don't align if texture count > texture coordinates count
	if (texUnit >= getTextureCoordinatesArrayCount()) 
		return;

	getTextureCoordinatesForTextureUnit( texUnit )->alignWithTexture( aTexture );
}

void CC3Mesh::flipVerticallyTextureUnit( GLuint texUnit )
{
	getTextureCoordinatesForTextureUnit(texUnit)->flipVertically();
}

void CC3Mesh::flipTexturesVertically()
{
	GLuint tcCount = getTextureCoordinatesArrayCount();
	for (GLuint texUnit = 0; texUnit < tcCount; texUnit++)
		getTextureCoordinatesForTextureUnit(texUnit)->flipVertically();
}

void CC3Mesh::flipHorizontallyTextureUnit( GLuint texUnit )
{
	getTextureCoordinatesForTextureUnit(texUnit)->flipHorizontally();
}

void CC3Mesh::flipTexturesHorizontally()
{
	GLuint tcCount = getTextureCoordinatesArrayCount();
	for (GLuint texUnit = 0; texUnit < tcCount; texUnit++)
		getTextureCoordinatesForTextureUnit(texUnit)->flipHorizontally();
}

void CC3Mesh::repeatTexture( const ccTex2F& repeatFactor, GLuint texUnit )
{
	getTextureCoordinatesForTextureUnit(texUnit)->repeatTexture( repeatFactor );
}

void CC3Mesh::repeatTexture( const ccTex2F& repeatFactor )
{
	GLuint tcCount = getTextureCoordinatesArrayCount();
	for (GLuint texUnit = 0; texUnit < tcCount; texUnit++)
		getTextureCoordinatesForTextureUnit(texUnit)->repeatTexture( repeatFactor );
}

CCRect CC3Mesh::getTextureRectangleForTextureUnit( GLuint texUnit )
{
	CC3VertexTextureCoordinates* texCoords = getTextureCoordinatesForTextureUnit(texUnit);
	return texCoords ? texCoords->getTextureRectangle() : kCC3UnitTextureRectangle;
}

void CC3Mesh::setTextureRectangle( const CCRect& aRect, GLuint texUnit )
{
	getTextureCoordinatesForTextureUnit(texUnit)->setTextureRectangle( aRect );
}

CCRect CC3Mesh::getTextureRectangle()
{ 
	return getTextureRectangleForTextureUnit(0); 
}

void CC3Mesh::setTextureRectangle( const CCRect& aRect )
{
	GLuint tcCount = getTextureCoordinatesArrayCount();
	for (GLuint i = 0; i < tcCount; i++)
		getTextureCoordinatesForTextureUnit(i)->setTextureRectangle( aRect );
}

GLenum CC3Mesh::getDrawingMode() 
{
	if (m_vertexIndices) 
		return m_vertexIndices->getDrawingMode();
	if (m_vertexLocations) 
		return m_vertexLocations->getDrawingMode();
	return GL_TRIANGLES;
}

void CC3Mesh::setDrawingMode( GLenum aMode )
{
	if ( m_vertexIndices )
		m_vertexIndices->setDrawingMode( aMode );
	if ( m_vertexLocations )
		m_vertexLocations->setDrawingMode( aMode );
}

void CC3Mesh::drawWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	bindWithVisitor( visitor );
	drawVerticesWithVisitor( visitor );
}

void CC3Mesh::drawFrom( GLuint vertexIndex, GLuint vertexCount, CC3NodeDrawingVisitor* visitor )
{
	bindWithVisitor( visitor );
	drawVerticesFrom( vertexIndex, vertexCount, visitor );
}

void CC3Mesh::bindWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	visitor->getGL()->bindMesh( this, visitor );
}

/**
 * Populates any shader program uniform variables that have draw scope,
 * and then draws the mesh vertices to the GL engine.
 *
 * If the vertexIndices property is not nil, the draw method is invoked on that
 * CC3VertexIndices instance. Otherwise, the draw method is invoked on the
 * CC3VertexLocations instance in the vertexLocations property.
 */
void CC3Mesh::drawVerticesWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	CC3ShaderProgram* pShaderProgram = visitor->getCurrentShaderProgram();
	if ( pShaderProgram )
		pShaderProgram->populateDrawScopeUniformsWithVisitor( visitor );

	if (m_vertexIndices)
		m_vertexIndices->drawWithVisitor( visitor );
	else if ( m_vertexLocations )
		m_vertexLocations->drawWithVisitor( visitor );
}

/**
 * Populates any shader program uniform variables that have draw scope,
 * and then draws the specified range of mesh vertices to the GL engine.
 *
 * If the vertexIndices property is not nil, the draw method is invoked on that
 * CC3VertexIndices instance. Otherwise, the draw method is invoked on the
 * CC3VertexLocations instance in the vertexLocations property.
 */
void CC3Mesh::drawVerticesFrom( GLuint vertexIndex, GLuint vertexCount, CC3NodeDrawingVisitor* visitor )
{
	// LogTrace(@"Drawing %@ from %u for %u vertices", self, vertexIndex, vertexCount);

	CC3ShaderProgram* pShaderProgram = visitor->getCurrentShaderProgram();
	if ( pShaderProgram )
		pShaderProgram->populateDrawScopeUniformsWithVisitor( visitor );

	if (m_vertexIndices)
		m_vertexIndices->drawFrom( vertexIndex, vertexCount, visitor );
	else if ( m_vertexLocations )
		m_vertexLocations->drawFrom( vertexIndex, vertexCount, visitor );
}

void CC3Mesh::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	m_vertexLocations = NULL;
	m_vertexNormals = NULL;
	m_vertexTangents = NULL;
	m_vertexBitangents = NULL;
	m_vertexColors = NULL;
	m_vertexBoneIndices = NULL;
	m_vertexBoneWeights = NULL;
	m_vertexPointSizes = NULL;
	m_vertexTextureCoordinates = NULL;
	m_overlayTextureCoordinates = NULL;
	m_vertexIndices = NULL;
	m_faces = NULL;
	m_shouldInterleaveVertices = true;
	m_capacityExpansionFactor = 1.25f;
}

void CC3Mesh::initWithTag( GLuint aTag )
{
	super::initWithTag( aTag );
}

void CC3Mesh::populateFrom( CC3Mesh* another )
{
	super::populateFrom( another );
	
	m_shouldInterleaveVertices = another->shouldInterleaveVertices();
	m_capacityExpansionFactor = another->getCapacityExpansionFactor();
	
	// Share vertex arrays between copies
	setVertexLocations( (CC3VertexLocations*)another->getVertexLocations()->copy()->autorelease() );
	setVertexNormals( (CC3VertexNormals*)another->getVertexNormals()->copy()->autorelease() );
	setVertexTangents( (CC3VertexTangents*)another->getVertexTangents()->copy()->autorelease() );
	setVertexBitangents( (CC3VertexTangents*)another->getVertexBitangents()->copy()->autorelease() );
	setVertexColors( (CC3VertexColors*)another->getVertexColors()->copy()->autorelease() );
	setVertexBoneIndices( (CC3VertexBoneIndices*)another->getVertexBoneIndices()->copy()->autorelease() );
	setVertexBoneWeights( (CC3VertexBoneWeights*)another->getVertexBoneWeights()->copy()->autorelease() );
	setVertexPointSizes( (CC3VertexPointSizes*)another->getVertexPointSizes()->copy()->autorelease() );
	setVertexTextureCoordinates( (CC3VertexTextureCoordinates*)another->getVertexTextureCoordinates()->copy()->autorelease() );
	
	// Remove any existing overlay textures and add the overlay textures from the other vertex array.
	m_overlayTextureCoordinates->removeAllObjects();
	CCArray* otherOTCs = another->m_overlayTextureCoordinates;

	CCObject* pObj;
	CCARRAY_FOREACH( otherOTCs, pObj )
	{
		CC3VertexTextureCoordinates* otc = (CC3VertexTextureCoordinates*)pObj;
		addTextureCoordinates( (CC3VertexTextureCoordinates*)otc->copy()->autorelease() );
	}
	
	setVertexIndices( (CC3VertexIndices*)another->getVertexIndices()->copy()->autorelease() );
	
	setFaces( (CC3FaceArray*)another->getFaces()->copy()->autorelease() );
}

CCObject* CC3Mesh::copyWithZone( CCZone* zone )
{
	CC3Mesh* pVal = new CC3Mesh;
	pVal->init();
	pVal->populateFrom( this );

	return pVal;
}

CC3Mesh* CC3Mesh::mesh()
{
	CC3Mesh* pMesh = new CC3Mesh;
	pMesh->init();
	pMesh->autorelease();

	return pMesh;
}

CC3Mesh* CC3Mesh::meshWithTag( GLuint aTag )
{
	CC3Mesh* pMesh = new CC3Mesh;
	pMesh->initWithTag( aTag );
	pMesh->autorelease();

	return pMesh;
}

CC3Mesh* CC3Mesh::meshWithName( const std::string& aName )
{
	CC3Mesh* pMesh = new CC3Mesh;
	pMesh->initWithName( aName );
	pMesh->autorelease();

	return pMesh;
}

CC3Mesh* CC3Mesh::meshWithTag( GLuint aTag, const std::string& aName )
{
	CC3Mesh* pMesh = new CC3Mesh;
	pMesh->initWithTag( aTag, aName.c_str() );
	pMesh->autorelease();

	return pMesh;
}

// Class variable tracking the most recent tag value assigned for CC3Meshs.
// This class variable is automatically incremented whenever the method nextTag is called.
static GLuint lastAssignedMeshTag = 0;

GLuint CC3Mesh::nextTag()
{
	return ++lastAssignedMeshTag; 
}

void CC3Mesh::resetTagAllocation()
{
	lastAssignedMeshTag = 0; 
}


void CC3Mesh::ensureVertexContent()
{
	if (getVertexContentTypes() == kCC3VertexContentNone) 
	{
		setVertexContentTypes( (CC3VertexContent)(kCC3VertexContentLocation |
			kCC3VertexContentNormal |
			kCC3VertexContentTextureCoordinates) );
	}
}

void CC3Mesh::populateAsTriangle( const CC3Face& face, ccTex2F* tc, GLuint divsPerSide )
{
	// Must have at least one division per side
	divsPerSide = MAX(divsPerSide, 1);

	// The fraction of each side that each division represents.
	// This is the barycentric coordinate division increment.
	GLfloat divFrac = 1.0f / divsPerSide;

	// Derive the normal. All vertices on the triangle will have the same normal.
	CC3Vector vtxNml = face.getNormal();

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

void CC3Mesh::populateAsCenteredRectangleWithSize( const CCSize& rectSize )
{
	populateAsRectangleWithSize( rectSize, ccp(0.5f, 0.5f) );
}

void CC3Mesh::populateAsCenteredRectangleWithSize( const CCSize& rectSize, const CC3Tessellation& divsPerAxis )
{
	populateAsRectangleWithSize( rectSize, ccp(0.5f, 0.5f), divsPerAxis );
}

void CC3Mesh::populateAsRectangleWithSize( const CCSize& rectSize, const CCPoint& origin )
{
	populateAsRectangleWithSize( rectSize, origin, CC3TessellationMake(1, 1) );
}

void CC3Mesh::populateAsRectangleWithSize( const CCSize& rectSize, const CCPoint& _origin, const CC3Tessellation& _divsPerAxis )
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
	CCSize divTexSpan = CCSizeMake((1.0f / divsPerAxis.x), (1.0f / divsPerAxis.y));

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
			setVertexLocation( cc3v(vx, vy, 0.0f), vIndx );

			// Vertex normal. Will do nothing if this mesh does not include normals.
			setVertexNormal( CC3Vector::kCC3VectorUnitZPositive, vIndx );

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

void CC3Mesh::populateAsDiskWithRadius( GLfloat radius, const CC3Tessellation& radialAndAngleDivs )
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

	// Populate vertex locations, normals & texture coordinates.
	GLuint vIndx = 0;			// Vertex index
	GLuint iIndx = 0;			// Index index

	// Add the center vertex Vertex location from unit radial scaled by the radial span and ring number
	// Setters for any content that is not defined by the vertexContentTypes property will do nothing.
	setVertexLocation( CC3Vector::kCC3VectorZero, vIndx );
	setVertexNormal( CC3Vector::kCC3VectorUnitZPositive, vIndx );
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
			setVertexNormal( CC3Vector::kCC3VectorUnitZPositive, vIndx );

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
			if ( ia < numAngularDivs ) 
			{
				if ( ir == 1 ) 
				{
					indices[iIndx++] = 0;							// Center vertex
					indices[iIndx++] = vIndx;						// Current vertex
					indices[iIndx++] = vIndx + numRadialDivs;		// Next angular div, same ring
				} 
				else 
				{
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

void CC3Mesh::populateAsSolidBox( const CC3Box& box )
{
	GLfloat w = box.maximum.x - box.minimum.x;		// Width of the box
	GLfloat h = box.maximum.y - box.minimum.y;		// Height of the box
	GLfloat d = box.maximum.z - box.minimum.z;		// Depth of the box
	GLfloat ufw = d + w + d + w;					// Total width of unfolded flattened box
	GLfloat ufh = d + h + d;						// Total height of unfolded flattened box
	populateAsSolidBox( box, ccp((d / ufw), (d / ufh)) );
}

void CC3Mesh::populateAsCubeMappedSolidBox( const CC3Box& box )
{
	populateAsSolidBox( box, ccp((1.0 / 4.0), (1.0 / 3.0)) );
}

// Thanks to Cocos3D user andyman for contributing the prototype code and texture
// template file for this method.
void CC3Mesh::populateAsSolidBox( const CC3Box& box, const CCPoint& corner )
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
	setVertexNormal( CC3Vector::kCC3VectorUnitZPositive, 0 );
	setVertexTexCoord2F( cc3tc(corner.x, corner.y), 0 );

	setVertexLocation( cc3v(boxMax.x, boxMin.y, boxMax.z), 1 );
	setVertexNormal( CC3Vector::kCC3VectorUnitZPositive, 1 );
	setVertexTexCoord2F( cc3tc(0.5f, corner.y), 1 );

	setVertexLocation( cc3v(boxMax.x, boxMax.y, boxMax.z), 2 );
	setVertexNormal( CC3Vector::kCC3VectorUnitZPositive, 2 );
	setVertexTexCoord2F( cc3tc(0.5f, (1.0f - corner.y)), 2 );

	setVertexLocation( cc3v(boxMin.x, boxMax.y, boxMax.z), 3 );
	setVertexNormal( CC3Vector::kCC3VectorUnitZPositive, 3 );
	setVertexTexCoord2F( cc3tc(corner.x, (1.0f - corner.y)), 3 );

	// Right face, CCW winding:
	setVertexLocation( cc3v(boxMax.x, boxMin.y, boxMax.z), 4 );
	setVertexNormal( CC3Vector::kCC3VectorUnitXPositive, 4 );
	setVertexTexCoord2F( cc3tc(0.5f, corner.y), 4 );

	setVertexLocation( cc3v(boxMax.x, boxMin.y, boxMin.z), 5 );
	setVertexNormal( CC3Vector::kCC3VectorUnitXPositive, 5 );
	setVertexTexCoord2F( cc3tc((0.5f + corner.x), corner.y), 5 );

	setVertexLocation( cc3v(boxMax.x, boxMax.y, boxMin.z), 6 );
	setVertexNormal( CC3Vector::kCC3VectorUnitXPositive, 6 );
	setVertexTexCoord2F( cc3tc((0.5f + corner.x), (1.0f - corner.y)), 6 );

	setVertexLocation( cc3v(boxMax.x, boxMax.y, boxMax.z), 7 );
	setVertexNormal( CC3Vector::kCC3VectorUnitXPositive, 7 );
	setVertexTexCoord2F( cc3tc(0.5f, (1.0f - corner.y)), 7 );

	// Back face, CCW winding:
	setVertexLocation( cc3v(boxMax.x, boxMin.y, boxMin.z), 8 );
	setVertexNormal( CC3Vector::kCC3VectorUnitZNegative, 8 );
	setVertexTexCoord2F( cc3tc((0.5f + corner.x), corner.y), 8 );

	setVertexLocation( cc3v(boxMin.x, boxMin.y, boxMin.z), 9 );
	setVertexNormal( CC3Vector::kCC3VectorUnitZNegative, 9 );
	setVertexTexCoord2F( cc3tc(1.0f, corner.y), 9 );

	setVertexLocation( cc3v(boxMin.x, boxMax.y, boxMin.z), 10 );
	setVertexNormal( CC3Vector::kCC3VectorUnitZNegative, 10 );
	setVertexTexCoord2F( cc3tc(1.0f, (1.0f - corner.y)), 10 );

	setVertexLocation( cc3v(boxMax.x, boxMax.y, boxMin.z), 11 );
	setVertexNormal( CC3Vector::kCC3VectorUnitZNegative, 11 );
	setVertexTexCoord2F( cc3tc((0.5f + corner.x), (1.0f - corner.y)), 11 );

	// Left face, CCW winding:
	setVertexLocation( cc3v(boxMin.x, boxMin.y, boxMin.z), 12 );
	setVertexNormal( CC3Vector::kCC3VectorUnitXNegative, 12 );
	setVertexTexCoord2F( cc3tc(0.0f, corner.y), 12 );

	setVertexLocation( cc3v(boxMin.x, boxMin.y, boxMax.z), 13 );
	setVertexNormal( CC3Vector::kCC3VectorUnitXNegative, 13 );
	setVertexTexCoord2F( cc3tc(corner.x, corner.y), 13 );

	setVertexLocation( cc3v(boxMin.x, boxMax.y, boxMax.z), 14 );
	setVertexNormal( CC3Vector::kCC3VectorUnitXNegative, 14 );
	setVertexTexCoord2F( cc3tc(corner.x, (1.0f - corner.y)), 14 );

	setVertexLocation( cc3v(boxMin.x, boxMax.y, boxMin.z), 15 );
	setVertexNormal( CC3Vector::kCC3VectorUnitXNegative, 15 );
	setVertexTexCoord2F( cc3tc(0.0f, (1.0f - corner.y)), 15 );

	// Top face, CCW winding:
	setVertexLocation( cc3v(boxMin.x, boxMax.y, boxMin.z), 16 );
	setVertexNormal( CC3Vector::kCC3VectorUnitYPositive, 16 );
	setVertexTexCoord2F( cc3tc(corner.x, 1.0f), 16 );

	setVertexLocation( cc3v(boxMin.x, boxMax.y, boxMax.z), 17 );
	setVertexNormal( CC3Vector::kCC3VectorUnitYPositive, 17 );
	setVertexTexCoord2F( cc3tc(corner.x, (1.0f - corner.y)), 17 );

	setVertexLocation( cc3v(boxMax.x, boxMax.y, boxMax.z), 18 );
	setVertexNormal( CC3Vector::kCC3VectorUnitYPositive, 18 );
	setVertexTexCoord2F( cc3tc(0.5f, (1.0f - corner.y)), 18 );

	setVertexLocation( cc3v(boxMax.x, boxMax.y, boxMin.z), 19 );
	setVertexNormal( CC3Vector::kCC3VectorUnitYPositive, 19 );
	setVertexTexCoord2F( cc3tc(0.5f, 1.0f), 19 );

	// Bottom face, CCW winding:
	setVertexLocation( cc3v(boxMin.x, boxMin.y, boxMax.z), 20 );
	setVertexNormal( CC3Vector::kCC3VectorUnitYNegative, 20 );
	setVertexTexCoord2F( cc3tc(corner.x, corner.y), 20 );

	setVertexLocation( cc3v(boxMin.x, boxMin.y, boxMin.z), 21 );
	setVertexNormal( CC3Vector::kCC3VectorUnitYNegative, 21 );
	setVertexTexCoord2F( cc3tc(corner.x, 0.0f), 21 );

	setVertexLocation( cc3v(boxMax.x, boxMin.y, boxMin.z), 22 );
	setVertexNormal( CC3Vector::kCC3VectorUnitYNegative, 22 );
	setVertexTexCoord2F( cc3tc(0.5f, 0.0f), 22 );

	setVertexLocation( cc3v(boxMax.x, boxMin.y, boxMax.z), 23 );
	setVertexNormal( CC3Vector::kCC3VectorUnitYNegative, 23 );
	setVertexTexCoord2F( cc3tc(0.5f, corner.y), 23 );

	// Populate the vertex indices
	// Since the index array is a simple array, just access the array directly.
	GLubyte indxIndx = 0;
	GLubyte vtxIndx = 0;
	for (int side = 0; side < 6; side++)
    {
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

void CC3Mesh::populateAsWireBox( const CC3Box& box )
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

void CC3Mesh::populateAsSphereWithRadius( GLfloat radius, const CC3Tessellation& _divsPerAxis )
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
			setVertexLocation( unitRadial * radius, vIndx );

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

void CC3Mesh::populateAsHollowConeWithRadius( GLfloat radius, GLfloat height, const CC3Tessellation& angleAndHeightDivs )
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
		CC3Vector vtxNormal = cc3v(sa, radiusHeightRatio, ca).normalize();

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

void CC3Mesh::populateAsLineStripWith( GLuint vertexCount, CC3Vector* vertices, bool shouldRetainVertices )
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

CC3FaceArray::CC3FaceArray()
{
	m_pMesh = NULL;	
}

CC3FaceArray::~CC3FaceArray()
{
	m_pMesh = NULL;					// weak reference
	deallocateIndices();
	deallocateCenters();
	deallocateNormals();
	deallocatePlanes();
	deallocateNeighbours();
}

CC3Mesh* CC3FaceArray::getMesh()
{
	return m_pMesh;
}

void CC3FaceArray::setMesh( CC3Mesh* aMesh )
{
	m_pMesh = aMesh;		// weak reference
	deallocateIndices();
	deallocateCenters();
	deallocateNormals();
	deallocatePlanes();
	deallocateNeighbours();
}

/** If turning off, clears all caches except neighbours. */
void CC3FaceArray::setShouldCacheFaces( bool shouldCache )
{
	m_shouldCacheFaces = shouldCache;
	if (!m_shouldCacheFaces) 
	{
		deallocateIndices();
		deallocateCenters();
		deallocateNormals();
		deallocatePlanes();
	}
}

bool CC3FaceArray::shouldCacheFaces()
{
	return m_shouldCacheFaces;
}

GLuint CC3FaceArray::getFaceCount()
{ 
	return m_pMesh ? m_pMesh->getFaceCount() : 0;
}

CC3Face CC3FaceArray::getFaceAt( GLuint faceIndex )
{
	return m_pMesh ? m_pMesh->getFaceAt( faceIndex ) : CC3Face::kCC3FaceZero;
}

void CC3FaceArray::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	m_pMesh = NULL;
	m_shouldCacheFaces = false;
	m_indices = NULL;
	m_indicesAreRetained = false;
	m_indicesAreDirty = true;
	m_centers = NULL;
	m_centersAreRetained = false;
	m_centersAreDirty = true;
	m_normals = NULL;
	m_normalsAreRetained = false;
	m_normalsAreDirty = true;
	m_planes = NULL;
	m_planesAreRetained = false;
	m_planesAreDirty = true;
	m_neighbours = NULL;
	m_neighboursAreRetained = false;
	m_neighboursAreDirty = true;
}

void CC3FaceArray::initWithTag( GLuint aTag )
{
	super::initWithTag( aTag );
}

CC3FaceArray* CC3FaceArray::faceArray()
{
	CC3FaceArray* pFaceArray = new CC3FaceArray;
	pFaceArray->init();
	pFaceArray->autorelease();

	return pFaceArray;
}

CC3FaceArray* CC3FaceArray::faceArrayWithTag( GLuint aTag )
{
	CC3FaceArray* pFaceArray = new CC3FaceArray;
	pFaceArray->initWithTag( aTag );
	pFaceArray->autorelease();

	return pFaceArray;
}

CC3FaceArray* CC3FaceArray::faceArrayWithName( const std::string& aName )
{
	CC3FaceArray* pFaceArray = new CC3FaceArray;
	pFaceArray->initWithName( aName.c_str() );
	pFaceArray->autorelease();

	return pFaceArray;
}

CC3FaceArray* CC3FaceArray::faceArrayWithTag( GLuint aTag, const std::string& aName )
{
	CC3FaceArray* pFaceArray = new CC3FaceArray;
	pFaceArray->initWithTag( aTag, aName.c_str() );
	pFaceArray->autorelease();

	return pFaceArray;
}

// Template method that populates this instance from the specified other instance.
// This method is invoked automatically during object copying via the copyWithZone: method.
void CC3FaceArray::populateFrom( CC3FaceArray* another )
{
	super::populateFrom( another );
	
	m_pMesh = another->getMesh();		// weak reference
	
	m_shouldCacheFaces = another->shouldCacheFaces();
	
	// If indices should be retained, allocate memory and copy the data over.
	deallocateIndices();
	if (another->m_indicesAreRetained) 
	{
		allocateIndices();
		memcpy(m_indices, another->getIndices(), (getFaceCount() * sizeof(CC3FaceIndices)));
	} else {
		m_indices = another->getIndices();
	}
	m_indicesAreDirty = another->m_indicesAreDirty;
	
	// If centers should be retained, allocate memory and copy the data over.
	deallocateCenters();
	if (another->m_centersAreRetained) 
	{
		allocateCenters();
		memcpy(m_centers, another->getCenters(), (getFaceCount() * sizeof(CC3Vector)));
	} else {
		m_centers = another->getCenters();
	}
	m_centersAreDirty = another->m_centersAreDirty;
	
	// If normals should be retained, allocate memory and copy the data over.
	deallocateNormals();
	if (another->m_normalsAreRetained) 
	{
		allocateNormals();
		memcpy(m_normals, another->getNormals(), (getFaceCount() * sizeof(CC3Vector)));
	} else {
		m_normals = another->getNormals();
	}
	m_normalsAreDirty = another->m_normalsAreDirty;
	
	// If planes should be retained, allocate memory and copy the data over.
	deallocatePlanes();
	if (another->m_planesAreRetained) {
		allocatePlanes();
		memcpy(m_planes, another->getPlanes(), (getFaceCount() * sizeof(CC3Plane)));
	} else {
		m_planes = another->getPlanes();
	}
	m_planesAreDirty = another->m_planesAreDirty;
	
	// If neighbours should be retained, allocate memory and copy the data over.
	deallocateNeighbours();
	if (another->m_neighboursAreRetained) {
		allocateNeighbours();
		memcpy(m_neighbours, another->getNeighbours(), (getFaceCount() * sizeof(CC3FaceNeighbours)));
	} else {
		m_neighbours = another->getNeighbours();
	}
	m_neighboursAreDirty = another->m_neighboursAreDirty;
}

CCObject* CC3FaceArray::copyWithZone( CCZone* zone )
{
	CC3FaceArray* pVal = new CC3FaceArray;
	pVal->init();
	pVal->populateFrom( this );

	return pVal;
}

CC3FaceIndices* CC3FaceArray::getIndices()
{
	if (m_indicesAreDirty || !m_indices) 
		populateIndices();
	return m_indices;
}

void CC3FaceArray::setIndices( CC3FaceIndices* faceIndices )
{
	deallocateIndices();			// Safely disposes existing vertices
	m_indices = faceIndices;
}

CC3FaceIndices CC3FaceArray::getUncachedIndicesAt( GLuint faceIndex )
{
	return m_pMesh->getUncachedFaceIndicesAt( faceIndex );
}

CC3FaceIndices CC3FaceArray::getIndicesAt( GLuint faceIndex )
{
	if (m_shouldCacheFaces) 
		return getIndices()[faceIndex];

	return getUncachedIndicesAt( faceIndex );
}

CC3FaceIndices* CC3FaceArray::allocateIndices()
{
	deallocateIndices();
	GLuint faceCount = getFaceCount();
	if (faceCount) 
	{
		m_indices = (CC3FaceIndices*)calloc(faceCount, sizeof(CC3FaceIndices));
		m_indicesAreRetained = true;
		CC3_TRACE("CC3FaceArray allocated space for %d face indices", getFaceCount());
	}
	return m_indices;
}

void CC3FaceArray::deallocateIndices()
{
	if (m_indicesAreRetained && m_indices) 
	{
		free(m_indices);
		m_indices = NULL;
		m_indicesAreRetained = false;
		CC3_TRACE("CC3FaceArray deallocated %d previously allocated indices", getFaceCount());
	}
}

void CC3FaceArray::populateIndices()
{
	CC3_TRACE("%CC3FaceArray populating %d face indices", getFaceCount());
	if ( !m_indices ) 
		allocateIndices();
	
	GLuint faceCount = getFaceCount();
	for (GLuint faceIdx = 0; faceIdx < faceCount; faceIdx++) 
	{
		m_indices[faceIdx] = getUncachedIndicesAt( faceIdx );
    }
	m_indicesAreDirty = false;
}

void CC3FaceArray::markIndicesDirty()
{
	m_indicesAreDirty = true; 
}

CC3Vector* CC3FaceArray::getCenters()
{
	if (m_centersAreDirty || !m_centers) 
		populateCenters();
	return m_centers;
}

void CC3FaceArray::setCenters( CC3Vector* faceCenters )
{
	deallocateCenters();			// Safely disposes existing vertices
	m_centers = faceCenters;
}

CC3Vector CC3FaceArray::getCenterAt( GLuint faceIndex )
{
	if (m_shouldCacheFaces) 
		return getCenters()[faceIndex];
	return getFaceAt( faceIndex ).getCenter();
}

CC3Vector* CC3FaceArray::allocateCenters()
{
	deallocateCenters();
	GLuint faceCount = getFaceCount();
	if (faceCount) 
	{
		m_centers = (CC3Vector*)calloc(faceCount, sizeof(CC3Vector));
		m_centersAreRetained = true;
	}
	return m_centers;
}

void CC3FaceArray::deallocateCenters()
{
	if (m_centersAreRetained && m_centers) 
	{
		free(m_centers);
		m_centers = NULL;
		m_centersAreRetained = false;
	}
}

void CC3FaceArray::populateCenters()
{
	if ( !m_centers ) 
		allocateCenters();
	
	GLuint faceCount = getFaceCount();
	for (GLuint faceIdx = 0; faceIdx < faceCount; faceIdx++) 
	{
		m_centers[faceIdx] = getFaceAt(faceIdx).getCenter();
    }
	m_centersAreDirty = false;
}

void CC3FaceArray::markCentersDirty()
{
	m_centersAreDirty = true; 
}

CC3Vector* CC3FaceArray::getNormals()
{
	if (m_normalsAreDirty || !m_normals) 
		populateNormals();

	return m_normals;
}

void CC3FaceArray::setNormals( CC3Vector* faceNormals )
{
	deallocateNormals(); 			// Safely disposes existing vertices
	m_normals = faceNormals;
}

CC3Vector CC3FaceArray::getNormalAt( GLuint faceIndex )
{
	if (m_shouldCacheFaces) 
		return getNormals()[faceIndex];
	return getFaceAt(faceIndex).getNormal();
}

CC3Vector* CC3FaceArray::allocateNormals()
{
	deallocateNormals();
	GLuint faceCount = getFaceCount();
	if (faceCount) 
	{
		m_normals = (CC3Vector*)calloc( faceCount, sizeof(CC3Vector) );
		m_normalsAreRetained = true;
	}
	return m_normals;
}

void CC3FaceArray::deallocateNormals()
{
	if (m_normalsAreRetained && m_normals) 
	{
		free( m_normals );
		m_normals = NULL;
		m_normalsAreRetained = false;
	}
}

void CC3FaceArray::populateNormals()
{
	if ( !m_normals ) 
		allocateNormals();
	
	GLuint faceCount = getFaceCount();
	for (GLuint faceIdx = 0; faceIdx < faceCount; faceIdx++) 
	{
		m_normals[faceIdx] = getFaceAt(faceIdx).getNormal();
	}
	m_normalsAreDirty = false;
}

void CC3FaceArray::markNormalsDirty()
{
	m_normalsAreDirty = true; 
}

CC3Plane* CC3FaceArray::getPlanes()
{
	if (m_planesAreDirty || !m_planes) 
		populatePlanes();

	return m_planes;
}

void CC3FaceArray::setPlanes( CC3Plane* facePlanes )
{
	deallocatePlanes();			// Safely disposes existing vertices
	m_planes = facePlanes;
}

CC3Plane CC3FaceArray::getPlaneAt( GLuint faceIndex )
{
	if (m_shouldCacheFaces) 
		return getPlanes()[faceIndex];
    return CC3Plane::planeFromFace( getFaceAt(faceIndex) );
}

CC3Plane* CC3FaceArray::allocatePlanes()
{
	deallocatePlanes();
	GLuint faceCount = getFaceCount();
	if (faceCount) 
	{
		m_planes = (CC3Plane*)calloc(faceCount, sizeof(CC3Plane));
		m_planesAreRetained = true;
	}
	return m_planes;
}

void CC3FaceArray::deallocatePlanes()
{
	if (m_planesAreRetained && m_planes) 
	{
		free(m_planes);
		m_planes = NULL;
		m_planesAreRetained = false;
	}
}

void CC3FaceArray::populatePlanes()
{
	if ( !m_planes ) 
		allocatePlanes();
	
	GLuint faceCount = getFaceCount();
	for (GLuint faceIdx = 0; faceIdx < faceCount; faceIdx++) 
	{
        m_planes[faceIdx] = CC3Plane::planeFromFace( getFaceAt(faceIdx) );
	}
	m_planesAreDirty = false;
}

void CC3FaceArray::markPlanesDirty()
{
	m_planesAreDirty = true; 
}

CC3FaceNeighbours* CC3FaceArray::getNeighbours()
{
	if (m_neighboursAreDirty || !m_neighbours) 
		populateNeighbours();

	return m_neighbours;
}

void CC3FaceArray::setNeighbours( CC3FaceNeighbours* faceNeighbours )
{
	deallocateNeighbours();		// Safely disposes existing vertices
	m_neighbours = faceNeighbours;
}

CC3FaceNeighbours CC3FaceArray::getNeighboursAt( GLuint faceIndex )
{
	return getNeighbours()[faceIndex];
}

CC3FaceNeighbours* CC3FaceArray::allocateNeighbours()
{
	deallocateNeighbours();
	GLuint faceCount = getFaceCount();
	if (faceCount) 
	{
		m_neighbours = (CC3FaceNeighbours*)calloc(faceCount, sizeof(CC3FaceNeighbours));
		m_neighboursAreRetained = true;
	}
	return m_neighbours;
}

void CC3FaceArray::deallocateNeighbours()
{
	if (m_neighboursAreRetained && m_neighbours) 
	{
		free(m_neighbours);
		m_neighbours = NULL;
		m_neighboursAreRetained = false;
	}
}

void CC3FaceArray::populateNeighbours()
{
	if ( !m_neighbours ) 
		allocateNeighbours();
	
	GLuint faceCnt = getFaceCount();
	
	// Break all neighbour links. Done in batch so that we can skip
	// testing neighbour connections from both directions later.
	for (GLuint faceIdx = 0; faceIdx < faceCnt; faceIdx++) 
	{
		GLuint* neighbourEdge = m_neighbours[faceIdx].edges;
		neighbourEdge[0] = neighbourEdge[1] = neighbourEdge[2] = kCC3FaceNoNeighbour;
	}
	
	// Iterate through all the faces
	for (GLuint f1Idx = 0; f1Idx < faceCnt; f1Idx++)
	{
		// Get the neighbours of the current face, and if any of the edges still
		// need to have a neighbour assigned, look for them. We check this early
		// to avoid iterating through the remaining faces
		GLuint* f1Neighbours = m_neighbours[f1Idx].edges;
		if (f1Neighbours[0] == kCC3FaceNoNeighbour ||
			f1Neighbours[1] == kCC3FaceNoNeighbour ||
			f1Neighbours[2] == kCC3FaceNoNeighbour) 
		{
			// For the current face, retrieve the vertex indices
			GLuint* f1Vertices = m_pMesh->getFaceIndicesAt(f1Idx).vertices;
			
			// Iterate through all the faces beyond the current face
			for (GLuint f2Idx = f1Idx + 1; f2Idx < faceCnt; f2Idx++) 
			{
				// Get the neighbours of the other face, and if any of the edges still
				// need to have a neighbour assigned, see if any of the edges between
				// the current face and other face match. We check for neighbours early
				// to avoid iterating through all the face combinations.
				GLuint* f2Neighbours = m_neighbours[f2Idx].edges;
				if (f2Neighbours[0] == kCC3FaceNoNeighbour ||
					f2Neighbours[1] == kCC3FaceNoNeighbour ||
					f2Neighbours[2] == kCC3FaceNoNeighbour) 
				{
					// For the other face, retrieve the vertex indices
					GLuint* f2Vertices = m_pMesh->getFaceIndicesAt(f2Idx).vertices;
					
					// Compare each edge of the current face with each edge of the other face
					for (int f1EdgeIdx = 0; f1EdgeIdx < 3; f1EdgeIdx++) 
					{
						// If this edge already has a neighbour, skip it
						if (f1Neighbours[f1EdgeIdx] == (GLuint)kCC3FaceNoNeighbour) 
						{
							// Get the end points of an edge of the current face
							GLuint f1EdgeStart = f1Vertices[f1EdgeIdx];
							GLuint f1EdgeEnd = f1Vertices[(f1EdgeIdx < 2) ? (f1EdgeIdx + 1) : 0];
							
							// Iterate each edge of other face and compare against current face edge
							for (int f2EdgeIdx = 0; f2EdgeIdx < 3; f2EdgeIdx++) 
							{
								// If this edge already has a neighbour, skip it
								if (f2Neighbours[f2EdgeIdx] == (GLuint)kCC3FaceNoNeighbour) 
								{
									// Get the end points of an edge of the other face
									GLuint f2EdgeStart = f2Vertices[f2EdgeIdx];
									GLuint f2EdgeEnd = f2Vertices[(f2EdgeIdx < 2) ? (f2EdgeIdx + 1) : 0];
									
									// If the two edges have the same endpoints, mark each as a neighbour of the other
									if ((f1EdgeStart == f2EdgeStart && f1EdgeEnd == f2EdgeEnd) ||
										(f1EdgeStart == f2EdgeEnd && f1EdgeEnd == f2EdgeStart) )
									{
										f1Neighbours[f1EdgeIdx] = f2Idx;
										f2Neighbours[f2EdgeIdx] = f1Idx;
									}
								}
							}
						}
					}
				}
			}
		}
	}
    
	m_neighboursAreDirty = false;
}

void CC3FaceArray::markNeighboursDirty()
{
	m_neighboursAreDirty = true; 
}

NS_COCOS3D_END
