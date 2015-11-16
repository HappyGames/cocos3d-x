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
	_vertexLocations = NULL;
	_vertexNormals = NULL;
	_vertexTangents = NULL;
	_vertexBitangents = NULL;
	_vertexColors = NULL;
	_vertexTextureCoordinates = NULL;
	_overlayTextureCoordinates = NULL;
	_vertexBoneWeights = NULL;
	_vertexBoneIndices = NULL;
	_vertexPointSizes = NULL;
	_vertexIndices = NULL;
	_faces = NULL;
}

CC3Mesh::~CC3Mesh()
{
	CC_SAFE_RELEASE( _vertexLocations );
	CC_SAFE_RELEASE( _vertexNormals );
	CC_SAFE_RELEASE( _vertexTangents );
	CC_SAFE_RELEASE( _vertexBitangents );
	CC_SAFE_RELEASE( _vertexColors );
	CC_SAFE_RELEASE( _vertexTextureCoordinates );
	CC_SAFE_RELEASE( _overlayTextureCoordinates );
	CC_SAFE_RELEASE( _vertexBoneWeights );
	CC_SAFE_RELEASE( _vertexBoneIndices );
	CC_SAFE_RELEASE( _vertexPointSizes );
	CC_SAFE_RELEASE( _vertexIndices );
	CC_SAFE_RELEASE( _faces );
}

void CC3Mesh::setName( const std::string& name )
{
	super::setName( name );

	if ( _vertexLocations )
		_vertexLocations->deriveNameFrom( this );
	if ( _vertexNormals )
		_vertexNormals->deriveNameFrom( this );
	if ( _vertexTangents )
		_vertexTangents->deriveNameFrom( this );
	if ( _vertexBitangents )
		_vertexBitangents->deriveNameFrom( this );
	if ( _vertexColors )
		_vertexColors->deriveNameFrom( this );
	if ( _vertexTextureCoordinates )
		_vertexTextureCoordinates->deriveNameFrom( this );
	if ( _vertexBoneIndices )
		_vertexBoneIndices->deriveNameFrom( this );
	if ( _vertexBoneWeights )
		_vertexBoneWeights->deriveNameFrom( this );
	if ( _vertexPointSizes )
		_vertexPointSizes->deriveNameFrom( this );
	if ( _vertexIndices )
		_vertexIndices->deriveNameFrom( this );

	CCObject* pObj;
	CCARRAY_FOREACH( _overlayTextureCoordinates, pObj )
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
	return _vertexLocations; 
}

void CC3Mesh::setVertexLocations( CC3VertexLocations* vtxLocs )
{
	if (vtxLocs == _vertexLocations) 
		return;
	
	CC_SAFE_RELEASE(_vertexLocations);
	_vertexLocations = vtxLocs;
	CC_SAFE_RETAIN( vtxLocs );

	if ( _vertexLocations )
		_vertexLocations->deriveNameFrom( this );
}

bool CC3Mesh::hasVertexLocations()
{
	return (_vertexLocations != NULL); 
}

CC3VertexNormals* CC3Mesh::getVertexNormals()
{
	return _vertexNormals; 
}

void CC3Mesh::setVertexNormals( CC3VertexNormals* vtxNorms )
{
	if (vtxNorms == _vertexNormals)
		return;

	CC_SAFE_RELEASE(_vertexNormals);
	_vertexNormals= vtxNorms;
	CC_SAFE_RETAIN(vtxNorms);

	if ( _vertexNormals )
		_vertexNormals->deriveNameFrom( this );
}

bool CC3Mesh::hasVertexNormals()
{
	return (_vertexNormals != NULL); 
}

void CC3Mesh::flipNormals()
{
	_vertexNormals->flipNormals(); 
}

CC3VertexTangents* CC3Mesh::getVertexTangents()
{
	return _vertexTangents; 
}

void CC3Mesh::setVertexTangents( CC3VertexTangents* vtxTans )
{
	if (vtxTans == _vertexTangents) 
		return;

	CC_SAFE_RELEASE(_vertexTangents);
	_vertexTangents = vtxTans;
	CC_SAFE_RETAIN(vtxTans);

	if ( _vertexTangents )
		_vertexTangents->deriveNameFrom( this );
}

bool CC3Mesh::hasVertexTangents()
{
	return (_vertexTangents != NULL); 
}

CC3VertexTangents* CC3Mesh::getVertexBitangents()
{
	return _vertexBitangents; 
}

void CC3Mesh::setVertexBitangents( CC3VertexTangents* vtxBitans )
{
	if (vtxBitans == _vertexBitangents) 
		return;

	CC_SAFE_RELEASE(_vertexBitangents);
	_vertexBitangents = vtxBitans;
	CC_SAFE_RETAIN(vtxBitans);

	if ( _vertexBitangents )
	{
		_vertexBitangents->deriveNameFrom( this, "Bitangents" );
		_vertexBitangents->setSemantic( kCC3SemanticVertexBitangent );
	}
}

bool CC3Mesh::hasVertexBitangents()
{
	return (_vertexBitangents != NULL); 
}

CC3VertexColors* CC3Mesh::getVertexColors()
{
	return _vertexColors; 
}

void CC3Mesh::setVertexColors( CC3VertexColors* vtxCols )
{
	if (vtxCols == _vertexColors) 
		return;

	CC_SAFE_RELEASE(_vertexColors);
	_vertexColors = vtxCols;
	CC_SAFE_RETAIN(vtxCols);

	if ( _vertexColors )
		_vertexColors->deriveNameFrom( this );
}

bool CC3Mesh::hasVertexColors()
{
	return (_vertexColors != NULL); 
}

GLenum CC3Mesh::getVertexColorType()
{
	return _vertexColors ? _vertexColors->getElementType() : GL_FALSE; 
}

CC3VertexBoneIndices* CC3Mesh::getVertexBoneIndices()
{
	return _vertexBoneIndices; 
}

void CC3Mesh::setVertexBoneIndices( CC3VertexBoneIndices* vertexBoneIndices )
{
	if (vertexBoneIndices == _vertexBoneIndices) 
		return;

	CC_SAFE_RELEASE(_vertexBoneIndices);
	_vertexBoneIndices = vertexBoneIndices;
	CC_SAFE_RETAIN(vertexBoneIndices);

	if (_vertexBoneIndices)
		_vertexBoneIndices->deriveNameFrom( this );
}

bool CC3Mesh::hasVertexBoneIndices()
{
	return (_vertexBoneIndices != NULL);
}

CC3VertexBoneWeights* CC3Mesh::getVertexBoneWeights()
{
	return _vertexBoneWeights; 
}

void CC3Mesh::setVertexBoneWeights( CC3VertexBoneWeights* vertexBoneWeights )
{
	if (vertexBoneWeights == _vertexBoneWeights) 
		return;

	CC_SAFE_RELEASE(_vertexBoneWeights);
	_vertexBoneWeights = vertexBoneWeights;
	CC_SAFE_RETAIN(vertexBoneWeights);

	if ( _vertexBoneWeights )
		_vertexBoneWeights->deriveNameFrom( this );
}

bool CC3Mesh::hasVertexBoneWeights()
{
	return (_vertexBoneWeights != NULL); 
}

CC3VertexPointSizes* CC3Mesh::getVertexPointSizes()
{
	return _vertexPointSizes; 
}

void CC3Mesh::setVertexPointSizes( CC3VertexPointSizes* vtxSizes )
{
	if (vtxSizes == _vertexPointSizes) 
		return;

	CC_SAFE_RELEASE(_vertexPointSizes);
	_vertexPointSizes = vtxSizes;
	CC_SAFE_RETAIN(vtxSizes);

	if ( _vertexPointSizes )
		_vertexPointSizes->deriveNameFrom( this );
}

bool CC3Mesh::hasVertexPointSizes()
{
	return (_vertexPointSizes != NULL); 
}

CC3VertexIndices* CC3Mesh::getVertexIndices()
{
	return _vertexIndices; 
}

void CC3Mesh::setVertexIndices( CC3VertexIndices* vtxInd )
{
	if (vtxInd == _vertexIndices) 
		return;

	CC_SAFE_RELEASE(_vertexIndices);
	_vertexIndices = vtxInd;
	CC_SAFE_RETAIN(vtxInd);

	if ( _vertexIndices )
		_vertexIndices->deriveNameFrom( this );
}

bool CC3Mesh::hasVertexIndices()
{
	return (_vertexIndices != NULL); 
}

CC3VertexTextureCoordinates* CC3Mesh::getVertexTextureCoordinates()
{
	return _vertexTextureCoordinates; 
}

void CC3Mesh::setVertexTextureCoordinates( CC3VertexTextureCoordinates* vtxTexCoords )
{
	if (vtxTexCoords == _vertexTextureCoordinates) 
		return;

	CC_SAFE_RELEASE(_vertexTextureCoordinates);
	_vertexTextureCoordinates = vtxTexCoords;
	CC_SAFE_RETAIN(vtxTexCoords);

	if ( _vertexTextureCoordinates )
		_vertexTextureCoordinates->deriveNameFrom( this );
}

bool CC3Mesh::hasVertexTextureCoordinates()
{
	return (_vertexTextureCoordinates != NULL); 
}

GLuint CC3Mesh::getTextureCoordinatesArrayCount()
{
	return (_overlayTextureCoordinates ? (GLuint)_overlayTextureCoordinates->count() : 0) + (_vertexTextureCoordinates ? 1 : 0);
}

void CC3Mesh::addTextureCoordinates( CC3VertexTextureCoordinates* vtxTexCoords )
{
	CCAssert(vtxTexCoords, "Overlay texture cannot be nil");
	CCAssert(!_overlayTextureCoordinates || ((_overlayTextureCoordinates->count() + 1) <
											  CC3OpenGL::sharedGL()->getMaxNumberOfTextureUnits()),
			  "Too many overlaid textures. This platform only supports %i texture units."/*,
			  CC3OpenGL::sharedGL()->getMaxNumberOfTextureUnits()*/);
	
	// Set the first texture coordinates into vertexTextureCoordinates
	if (!_vertexTextureCoordinates) 
	{
		setVertexTextureCoordinates( vtxTexCoords );
	} 
	else 
	{
		// Add subsequent texture coordinate arrays to the array of overlayTextureCoordinates,
		// creating it first if necessary
		if( !_overlayTextureCoordinates )	
		{
			_overlayTextureCoordinates = CCArray::create();	// retained
			_overlayTextureCoordinates->retain();
		}
		_overlayTextureCoordinates->addObject( vtxTexCoords );
		vtxTexCoords->deriveNameFrom( this );
	}
}

void CC3Mesh::removeTextureCoordinates( CC3VertexTextureCoordinates* aTexCoord )
{
	// LogTrace(@"Removing %@ from %@", aTexCoord, self);
	
	// If the array to be removed is actually the vertexTextureCoordinates, remove it
	if (_vertexTextureCoordinates == aTexCoord) 
	{
		setVertexTextureCoordinates( NULL );
	} else {
		// Otherwise, find it in the array of overlays and remove it,
		// and remove the overlay array if it is now empty
		if (_overlayTextureCoordinates && aTexCoord) 
		{
			_overlayTextureCoordinates->removeObject( aTexCoord );
			if (_overlayTextureCoordinates->count() == 0) 
			{
				_overlayTextureCoordinates->release();
				_overlayTextureCoordinates = NULL;
			}
		}
	}
}

void CC3Mesh::removeAllTextureCoordinates()
{
	// Remove the first texture coordinates
	setVertexTextureCoordinates( NULL );
	
	if ( _overlayTextureCoordinates )
	{
		// Remove the overlay texture coordinates
		CCArray* myOTCs = (CCArray*)_overlayTextureCoordinates->copy();
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
	if (_vertexTextureCoordinates) {
		tcName = _vertexTextureCoordinates->getName();
		if (tcName == aName || (tcName.empty() && aName.empty())) {		// Name equal or both nil.
			return _vertexTextureCoordinates;
		}
	}
	// Then look for it in the overlays array
	CCObject* pObj;
	CCARRAY_FOREACH( _overlayTextureCoordinates, pObj )
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
		return _vertexTextureCoordinates;
	else if (texUnit < getTextureCoordinatesArrayCount()) 
		return (CC3VertexTextureCoordinates*)_overlayTextureCoordinates->objectAtIndex( texUnit - 1 );
	
	return getTextureCoordinatesForTextureUnit( texUnit - 1 );
}

void CC3Mesh::setTextureCoordinates( CC3VertexTextureCoordinates* aTexCoords, GLuint texUnit )
{
	CCAssert(aTexCoords, "Overlay texture coordinates cannot be nil");
	if (texUnit == 0) 
		setVertexTextureCoordinates( aTexCoords );
	else if (texUnit < getTextureCoordinatesArrayCount())
		_overlayTextureCoordinates->replaceObjectAtIndex( texUnit - 1,  aTexCoords );
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
	return _shouldInterleaveVertices; 
}

void CC3Mesh::setShouldInterleaveVertices( bool shouldInterleave )
{
	_shouldInterleaveVertices = shouldInterleave;
	if ( !_shouldInterleaveVertices )
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
	if (!_vertexLocations) 
		setVertexLocations( CC3VertexLocations::vertexArray() );
	
	// Vertex normals
	if (vtxContentTypes & kCC3VertexContentNormal) {
		if (!_vertexNormals)
			setVertexNormals( CC3VertexNormals::vertexArray() );
	} else {
		setVertexNormals( NULL );
	}
	
	// Vertex tangents
	if (vtxContentTypes & kCC3VertexContentTangent) {
		if (!_vertexTangents) 
			setVertexTangents( CC3VertexTangents::vertexArray() );
	} else {
		setVertexTangents( NULL );
	}
	
	// Vertex bitangents
	if (vtxContentTypes & kCC3VertexContentBitangent) {
		if (!_vertexBitangents) 
			setVertexBitangents( CC3VertexTangents::vertexArray() );
	} else {
		setVertexBitangents( NULL );
	}
	
	// Vertex colors
	if (vtxContentTypes & kCC3VertexContentColor) {
		if (!_vertexColors) 
			setVertexColors( CC3VertexColors::vertexArray() );
	} else {
		setVertexColors( NULL );
	}
	
	// Vertex texture coordinates
	if (vtxContentTypes & kCC3VertexContentTextureCoordinates) {
		if (!_vertexTextureCoordinates) 
			setVertexTextureCoordinates( CC3VertexTextureCoordinates::vertexArray() );
	} else {
		removeAllTextureCoordinates();
	}
	
	// Weights
	if (vtxContentTypes & kCC3VertexContentBoneWeights) {
		if (!_vertexBoneWeights) 
			setVertexBoneWeights( CC3VertexBoneWeights::vertexArray() );
	} else {
		setVertexBoneWeights( NULL );
	}
	
	// Matrix indices
	if (vtxContentTypes & kCC3VertexContentBoneIndices) {
		if (!_vertexBoneIndices) 
			setVertexBoneIndices( CC3VertexBoneIndices::vertexArray() );
	} else {
		setVertexBoneIndices( NULL );
	}
	
	// Point sizes
	if (vtxContentTypes & kCC3VertexContentPointSize) {
		if (!_vertexPointSizes) 
			setVertexPointSizes( CC3VertexPointSizes::vertexArray() );
	} else {
		setVertexPointSizes( NULL );
	}
	
}

GLuint CC3Mesh::getVertexStride()
{
	GLuint stride = 0;
	if (_vertexLocations) stride += _vertexLocations->getElementLength();
	if (_vertexNormals) stride += _vertexNormals->getElementLength();
	if (_vertexTangents) stride += _vertexTangents->getElementLength();
	if (_vertexBitangents) stride += _vertexBitangents->getElementLength();
	if (_vertexColors) stride += _vertexColors->getElementLength();
	if (_vertexBoneIndices) stride += _vertexBoneIndices->getElementLength();
	if (_vertexBoneWeights) stride += _vertexBoneWeights->getElementLength();
	if (_vertexPointSizes) stride += _vertexPointSizes->getElementLength();
	if (_vertexTextureCoordinates) stride += _vertexTextureCoordinates->getElementLength();

	CCObject* pObj;
	CCARRAY_FOREACH( _overlayTextureCoordinates, pObj )
	{
		CC3VertexTextureCoordinates* otc = (CC3VertexTextureCoordinates*)pObj;
		stride += otc->getElementLength();
		
	}
	return stride;
}

void CC3Mesh::setVertexStride( GLuint vtxStride )
{
	if ( !_shouldInterleaveVertices ) 
		return;

	if ( _vertexLocations )
		_vertexLocations->setVertexStride( vtxStride );
	if ( _vertexNormals )
		_vertexNormals->setVertexStride( vtxStride );
	if ( _vertexTangents )
		_vertexTangents->setVertexStride( vtxStride );
	if ( _vertexBitangents )
		_vertexBitangents->setVertexStride( vtxStride );
	if ( _vertexColors )
		_vertexColors->setVertexStride( vtxStride );
	if ( _vertexBoneIndices )
		_vertexBoneIndices->setVertexStride( vtxStride );
	if ( _vertexBoneWeights )
		_vertexBoneWeights->setVertexStride( vtxStride );
	if ( _vertexPointSizes )
		_vertexPointSizes->setVertexStride( vtxStride );
	if ( _vertexTextureCoordinates )
		_vertexTextureCoordinates->setVertexStride( vtxStride );

	CCObject* pObj;
	CCARRAY_FOREACH( _overlayTextureCoordinates, pObj )
	{
		CC3VertexTextureCoordinates* otc = (CC3VertexTextureCoordinates*)pObj;
		otc->setVertexStride( vtxStride );
	}
}

GLuint CC3Mesh::updateVertexStride()
{
	GLuint stride = 0;
	
	if (_vertexLocations) {
		if (_shouldInterleaveVertices) _vertexLocations->setElementOffset( stride );
		stride += _vertexLocations->getElementLength();
	}
	if (_vertexNormals) {
		if (_shouldInterleaveVertices) _vertexNormals->setElementOffset( stride );
		stride += _vertexNormals->getElementLength();
	}
	if (_vertexTangents) {
		if (_shouldInterleaveVertices) _vertexTangents->setElementOffset( stride );
		stride += _vertexTangents->getElementLength();
	}
	if (_vertexBitangents) {
		if (_shouldInterleaveVertices) _vertexBitangents->setElementOffset( stride );
		stride += _vertexBitangents->getElementLength();
	}
	if (_vertexColors) {
		if (_shouldInterleaveVertices) _vertexColors->setElementOffset( stride );
		stride += _vertexColors->getElementLength();
	}
	if (_vertexTextureCoordinates) {
		if (_shouldInterleaveVertices) _vertexTextureCoordinates->setElementOffset( stride );
		stride += _vertexTextureCoordinates->getElementLength();
	}

	CCObject* pObj;
	CCARRAY_FOREACH( _overlayTextureCoordinates, pObj )
	{
		CC3VertexTextureCoordinates* otc = (CC3VertexTextureCoordinates*)pObj;
		if (_shouldInterleaveVertices) 
			otc->setElementOffset( stride );
		stride += otc->getElementLength();
	}

	if (_vertexBoneWeights) {
		if (_shouldInterleaveVertices) _vertexBoneWeights->setElementOffset( stride );
		stride += _vertexBoneWeights->getElementLength();
	}
	if (_vertexBoneIndices) {
		if (_shouldInterleaveVertices) _vertexBoneIndices->setElementOffset( stride );
		stride += _vertexBoneIndices->getElementLength();
	}
	if (_vertexPointSizes) {
		if (_shouldInterleaveVertices) _vertexPointSizes->setElementOffset( stride );
		stride += _vertexPointSizes->getElementLength();
	}
	
	setVertexStride( stride );
	return stride;
}

GLuint CC3Mesh::getAllocatedVertexCapacity()
{ 
	return _vertexLocations ? _vertexLocations->getAllocatedVertexCapacity() : 0; 
}

void CC3Mesh::setAllocatedVertexCapacity( GLuint vtxCount )
{
	if (!_vertexLocations) 
		setVertexLocations( CC3VertexLocations::vertexArray() );
    
	_vertexLocations->setAllocatedVertexCapacity( vtxCount );
	
    if ( shouldInterleaveVertices() )
	{
		if ( _vertexNormals )
			_vertexNormals->interleaveWith( _vertexLocations );
		if ( _vertexTangents )
			_vertexTangents->interleaveWith( _vertexLocations );
		if ( _vertexBitangents )
			_vertexBitangents->interleaveWith( _vertexLocations );
		if ( _vertexColors )
			_vertexColors->interleaveWith( _vertexLocations );
		if ( _vertexBoneIndices )
			_vertexBoneIndices->interleaveWith( _vertexLocations );
		if ( _vertexBoneWeights )
			_vertexBoneWeights->interleaveWith( _vertexLocations );
		if ( _vertexPointSizes )
			_vertexPointSizes->interleaveWith( _vertexLocations );
		if ( _vertexTextureCoordinates )
			_vertexTextureCoordinates->interleaveWith( _vertexLocations );

		CCObject* pObj;
		CCARRAY_FOREACH( _overlayTextureCoordinates, pObj )
		{
			CC3VertexTextureCoordinates* otc = (CC3VertexTextureCoordinates*)pObj;
			otc->interleaveWith( _vertexLocations );
		}
	} 
	else 
	{
		if ( _vertexNormals )
			_vertexNormals->setAllocatedVertexCapacity( vtxCount );
		if ( _vertexTangents )
			_vertexTangents->setAllocatedVertexCapacity( vtxCount );
		if ( _vertexBitangents )
			_vertexBitangents->setAllocatedVertexCapacity( vtxCount );
		if ( _vertexColors )
			_vertexColors->setAllocatedVertexCapacity( vtxCount );
		if ( _vertexBoneIndices )
			_vertexBoneIndices->setAllocatedVertexCapacity( vtxCount );
		if ( _vertexBoneWeights )
			_vertexBoneWeights->setAllocatedVertexCapacity( vtxCount );
		if ( _vertexPointSizes )
			_vertexPointSizes->setAllocatedVertexCapacity( vtxCount );
		if ( _vertexTextureCoordinates )
			_vertexTextureCoordinates->setAllocatedVertexCapacity( vtxCount );

		CCObject* pObj;
		CCARRAY_FOREACH( _overlayTextureCoordinates, pObj )
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
	return _capacityExpansionFactor;
}

GLvoid* CC3Mesh::interleavedVertices()
{
	return (_shouldInterleaveVertices && _vertexLocations) ? _vertexLocations->getVertices() : NULL;
}

GLuint CC3Mesh::getAllocatedVertexIndexCapacity()
{ 
	return _vertexIndices ? _vertexIndices->getAllocatedVertexCapacity() : 0; 
}

void CC3Mesh::setAllocatedVertexIndexCapacity( GLuint vtxCount )
{
	if ( !_vertexIndices && vtxCount > 0 ) 
		setVertexIndices( CC3VertexIndices::vertexArray() );
	_vertexIndices->setAllocatedVertexCapacity( vtxCount );
}

void CC3Mesh::copyVertices( GLuint vtxCount, GLuint srcIdx, GLuint dstIdx )
{
	_vertexLocations->copyVertices( vtxCount, srcIdx, dstIdx );
	if ( !_shouldInterleaveVertices ) 
	{
		_vertexNormals->copyVertices( vtxCount, srcIdx, dstIdx );
		_vertexTangents->copyVertices( vtxCount, srcIdx, dstIdx );
		_vertexBitangents->copyVertices( vtxCount, srcIdx, dstIdx );
		_vertexColors->copyVertices( vtxCount, srcIdx, dstIdx );
		_vertexBoneIndices->copyVertices( vtxCount, srcIdx, dstIdx );
		_vertexBoneWeights->copyVertices( vtxCount, srcIdx, dstIdx );
		_vertexPointSizes->copyVertices( vtxCount, srcIdx, dstIdx );
		_vertexTextureCoordinates->copyVertices( vtxCount, srcIdx, dstIdx );

		CCObject* pObj;
		CCARRAY_FOREACH( _overlayTextureCoordinates, pObj )
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
	_vertexIndices->copyVertices( vtxCount, srcIdx, dstIdx, offset );
}

void CC3Mesh::copyVertexIndices( GLuint vtxCount, GLuint srcIdx, CC3Mesh* srcMesh, GLuint dstIdx, GLint offset )
{
	if ( !_vertexIndices ) 
		return;	// If there are no vertex indices, leave
	
	CC3VertexIndices* srcVtxIdxs = srcMesh->getVertexIndices();
	if (srcVtxIdxs) 
	{
		// If the template mesh has vertex indices, copy them over and offset them.
		// If both vertex index arrays are of the same type, we can optimize to a fast copy.
		if (srcVtxIdxs->getElementType() == _vertexIndices->getElementType())
		{
			_vertexIndices->copyVertices( vtxCount, srcVtxIdxs->getAddressOfElement( srcIdx ), dstIdx,  offset );
		} else {
			for (GLuint vtxIdx = 0; vtxIdx < vtxCount; vtxIdx++) 
			{
				GLuint srcVtx = srcVtxIdxs->getIndexAt( srcIdx + vtxIdx );
				_vertexIndices->setIndex( srcVtx + offset, dstIdx + vtxIdx );
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
			_vertexIndices->setIndex( offset + vtxIdx, dstIdx + vtxIdx );
		}
	}
}

GLuint CC3Mesh::getVertexCount()
{
	return _vertexLocations ? _vertexLocations->getVertexCount() : 0; 
}

void CC3Mesh::setVertexCount( GLuint vCount )
{
	// If we're attempting to set too many vertices for indexed drawing, log an error, but don't abort.
	if(_vertexIndices && (vCount > (kCC3MaxGLushort + 1)))
		CCLOGERROR("Setting vertexCount property of CC3Mesh to %d vertices. This mesh uses indexed drawing, which is limited by OpenGL ES to %d vertices."
				 " Vertices beyond that limit will not be drawn.", vCount, (kCC3MaxGLushort + 1));
	
	if ( _vertexLocations )
		_vertexLocations->setVertexCount( vCount );
	
	if ( _vertexNormals )
		_vertexNormals->setVertexCount( vCount );
	
	if ( _vertexTangents )
		_vertexTangents->setVertexCount( vCount );
	
	if ( _vertexBitangents )
		_vertexBitangents->setVertexCount( vCount );
	
	if ( _vertexColors )
		_vertexColors->setVertexCount( vCount );

	if ( _vertexBoneIndices )
		_vertexBoneIndices->setVertexCount( vCount );
	
	if ( _vertexBoneWeights )
		_vertexBoneWeights->setVertexCount( vCount );
	
	if ( _vertexPointSizes )
		_vertexPointSizes->setVertexCount( vCount );

	if ( _vertexTextureCoordinates )
		_vertexTextureCoordinates->setVertexCount( vCount );

	CCObject* pObj;
	CCARRAY_FOREACH( _overlayTextureCoordinates, pObj )
	{
		CC3VertexTextureCoordinates* otc = (CC3VertexTextureCoordinates*)pObj;
		otc->setVertexCount( vCount );
	}
}

GLuint CC3Mesh::getVertexIndexCount()
{ 
	return _vertexIndices ? _vertexIndices->getVertexCount() :getVertexCount(); 
}

void CC3Mesh::setVertexIndexCount( GLuint vCount )
{
	if ( _vertexIndices )
		_vertexIndices->setVertexCount( vCount ); 
}

CC3Vector CC3Mesh::getVertexLocationAt( GLuint index )
{
	return _vertexLocations ? _vertexLocations->getLocationAt( index ) : CC3Vector::kCC3VectorZero;
}

void CC3Mesh::setVertexLocation( const CC3Vector& aLocation, GLuint index )
{
	_vertexLocations->setLocation( aLocation, index );
}

CC3Vector4 CC3Mesh::getVertexHomogeneousLocationAt( GLuint index )
{
	return _vertexLocations ? _vertexLocations->getHomogeneousLocationAt( index ) : CC3Vector4::kCC3Vector4ZeroLocation;
}

void CC3Mesh::setVertexHomogeneousLocation( const CC3Vector4& aLocation, GLuint index )
{
	_vertexLocations->setHomogeneousLocation( aLocation, index );
}

CC3Vector CC3Mesh::getVertexNormalAt( GLuint index )
{
	return _vertexNormals ? _vertexNormals->getNormalAt( index ) : CC3Vector::kCC3VectorUnitZPositive;
}

void CC3Mesh::setVertexNormal( const CC3Vector& aNormal, GLuint index )
{
	if ( _vertexNormals )
		_vertexNormals->setNormal( aNormal, index );
}

CC3Vector CC3Mesh::getVertexTangentAt( GLuint index )
{
	return _vertexTangents ? _vertexTangents->getTangentAt( index ) : CC3Vector::kCC3VectorUnitXPositive;
}

void CC3Mesh::setVertexTangent( const CC3Vector& aTangent, GLuint index )
{
	if ( _vertexTangents ) 
		_vertexTangents->setTangent( aTangent, index );
}

CC3Vector CC3Mesh::getVertexBitangentAt( GLuint index )
{
	return _vertexBitangents ? _vertexBitangents->getTangentAt( index ) : CC3Vector::kCC3VectorUnitYPositive;
}

void CC3Mesh::setVertexBitangent( const CC3Vector& aTangent, GLuint index )
{
	if ( _vertexBitangents )
		_vertexBitangents->setTangent( aTangent, index );
}

ccColor4F CC3Mesh::getVertexColor4FAt( GLuint index )
{
	return _vertexColors ? _vertexColors->getColor4FAt( index ) : kCCC4FBlackTransparent;
}

void CC3Mesh::setVertexColor4F( const ccColor4F& aColor, GLuint index )
{
	if ( _vertexColors ) 
		_vertexColors->setColor4F( aColor, index );
}

ccColor4B CC3Mesh::getVertexColor4BAt( GLuint index )
{
	return _vertexColors ? _vertexColors->getColor4BAt( index ) : ccc4(0, 0, 0, 0);
}

void CC3Mesh::setVertexColor4B( const ccColor4B& aColor, GLuint index )
{
	if ( _vertexColors )
		_vertexColors->setColor4B( aColor, index );
}

GLuint CC3Mesh::getVertexBoneCount()
{ 
	return _vertexBoneWeights ? _vertexBoneWeights->getElementSize() : 0; 
}

GLfloat CC3Mesh::getVertexWeightForBoneInfluence( GLuint influenceIndex, GLuint vtxIndex )
{
	return _vertexBoneWeights ? _vertexBoneWeights->getWeightForBoneInfluence( influenceIndex, vtxIndex ) : 0.0f;
}

void CC3Mesh::setVertexWeight( GLfloat weight, GLuint influenceIndex, GLuint vtxIndex )
{
	if ( _vertexBoneWeights )
		_vertexBoneWeights->setWeight( weight, influenceIndex, vtxIndex );
}

GLfloat* CC3Mesh::getVertexBoneWeightsAt( GLuint vtxIndex )
{
	return _vertexBoneWeights ? _vertexBoneWeights->getBoneWeightsAt( vtxIndex ) : NULL;
}

void CC3Mesh::setVertexBoneWeights( GLfloat* weights, GLuint vtxIndex )
{
	if ( _vertexBoneWeights )
		_vertexBoneWeights->setBoneWeights( weights, vtxIndex );
}

GLuint CC3Mesh::getVertexBoneIndexForBoneInfluence( GLuint influenceIndex, GLuint vtxIndex )
{
	return _vertexBoneIndices ? _vertexBoneIndices->getBoneIndexForBoneInfluence( influenceIndex, vtxIndex ) : 0;
}

void CC3Mesh::setVertexBoneIndex( GLuint boneIndex, GLuint influenceIndex, GLuint vtxIndex )
{
	if ( _vertexBoneIndices )
		_vertexBoneIndices->setBoneIndex( boneIndex, influenceIndex, vtxIndex );
}

GLvoid* CC3Mesh::getVertexBoneIndicesAt( GLuint vtxIndex )
{
	return _vertexBoneIndices ? _vertexBoneIndices->getBoneIndicesAt(vtxIndex) : NULL;
}

void CC3Mesh::setVertexBoneIndices( GLvoid* boneIndices, GLuint vtxIndex )
{
	if ( _vertexBoneIndices )
		_vertexBoneIndices->setBoneIndices( boneIndices, vtxIndex );
}

GLenum CC3Mesh::getVertexBoneIndexType()
{
	return _vertexBoneIndices->getElementType(); 
}

GLfloat CC3Mesh::getVertexPointSizeAt( GLuint vtxIndex )
{
	return _vertexPointSizes ? _vertexPointSizes->getPointSizeAt( vtxIndex ) : 0.0f;
}

void CC3Mesh::setVertexPointSize( GLfloat aSize, GLuint vtxIndex )
{
	if ( _vertexPointSizes )
		_vertexPointSizes->setPointSize( aSize, vtxIndex );
}

void CC3Mesh::updatePointSizesGLBuffer()
{ 
	if ( _vertexPointSizes )
		_vertexPointSizes->updateGLBuffer(); 
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
	return _vertexIndices ? _vertexIndices->getIndexAt( index ) : 0;
}

void CC3Mesh::setVertexIndex( GLuint vertexIndex, GLuint index )
{
	if ( _vertexIndices )
		_vertexIndices->setIndex( vertexIndex, index );
}

CC3FaceArray* CC3Mesh::getFaces()
{
	if ( !_faces ) 
	{
		std::string facesName = CC3String::stringWithFormat( (char*)"%s-Faces", getName().c_str() );
		setFaces( CC3FaceArray::faceArrayWithName( facesName ) );
	}
	return _faces;
}

void CC3Mesh::setFaces( CC3FaceArray* aFaceArray )
{
	if (aFaceArray == _faces) 
		return;
	
	CC_SAFE_RELEASE( _faces );
	_faces = aFaceArray;
	CC_SAFE_RETAIN( _faces );

	if ( _faces )
		_faces->setMesh( this );
}

bool CC3Mesh::shouldCacheFaces()
{ 
	return _faces ? _faces->shouldCacheFaces() : false; 
}

void CC3Mesh::setShouldCacheFaces( bool shouldCache )
{
	getFaces()->setShouldCacheFaces( shouldCache ); 
}

GLuint CC3Mesh::getFaceCount()
{
	if (_vertexIndices) 
		return _vertexIndices->getFaceCount();

	if (_vertexLocations) 
		return _vertexLocations->getFaceCount();

	return 0;
}

CC3Face CC3Mesh::getFaceFromIndices( const CC3FaceIndices& faceIndices )
{
	return _vertexLocations ? _vertexLocations->getFaceFromIndices( faceIndices ) : CC3Face::kCC3FaceZero;
}

CC3FaceIndices CC3Mesh::getUncachedFaceIndicesAt( GLuint faceIndex )
{
	if (_vertexIndices) 
		return _vertexIndices->getFaceIndicesAt( faceIndex );
	if (_vertexLocations) 
		return _vertexLocations->getFaceIndicesAt( faceIndex );
	CCAssert(false, "CC3Mesh has no drawable vertex array and cannot retrieve indices for a face.");

	return kCC3FaceIndicesZero;
}

GLuint CC3Mesh::getFaceCountFromVertexIndexCount( GLuint vc )
{
	if (_vertexIndices) 
		return _vertexIndices->getFaceCountFromVertexIndexCount( vc );
	if (_vertexLocations) 
		return _vertexLocations->getFaceCountFromVertexIndexCount( vc );

	CCAssert(false, "CC3Mesh has no drawable vertex array and cannot convert vertex count to face count.");
	
	return 0;
}

GLuint CC3Mesh::getVertexIndexCountFromFaceCount( GLuint fc )
{
	if (_vertexIndices) 
		return _vertexIndices->getVertexIndexCountFromFaceCount(fc);
	if (_vertexLocations) 
		return _vertexLocations->getVertexIndexCountFromFaceCount(fc);
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
	if ( _vertexLocations )
		_vertexLocations->createGLBuffer();
	
	if (_shouldInterleaveVertices) 
	{
		if ( _vertexLocations )
		{
			GLuint commonBufferId = _vertexLocations->getBufferID();
			if ( _vertexNormals )
				_vertexNormals->setBufferID( commonBufferId );
			if ( _vertexTangents )
				_vertexTangents->setBufferID( commonBufferId );
			if ( _vertexBitangents )
				_vertexBitangents->setBufferID( commonBufferId );
			if ( _vertexColors )
				_vertexColors->setBufferID( commonBufferId );
			if ( _vertexBoneIndices )
				_vertexBoneIndices->setBufferID( commonBufferId );
			if ( _vertexBoneWeights )
				_vertexBoneWeights->setBufferID( commonBufferId );
			if ( _vertexPointSizes )
				_vertexPointSizes->setBufferID( commonBufferId );
			if ( _vertexTextureCoordinates )
				_vertexTextureCoordinates->setBufferID( commonBufferId );
			CCObject* pObj;
			CCARRAY_FOREACH( _overlayTextureCoordinates, pObj )
			{
				CC3VertexTextureCoordinates* otc = (CC3VertexTextureCoordinates*)pObj;
				otc->setBufferID( commonBufferId );
			}
		}
	} 
	else 
	{
		if ( _vertexNormals )
			_vertexNormals->createGLBuffer();
		
		if ( _vertexTangents )
			_vertexTangents->createGLBuffer();
		
		if ( _vertexBitangents )
			_vertexBitangents->createGLBuffer();
		
		if ( _vertexColors )
			_vertexColors->createGLBuffer();
		
		if ( _vertexBoneIndices )
			_vertexBoneIndices->createGLBuffer();
		
		if ( _vertexBoneWeights )
			_vertexBoneWeights->createGLBuffer();
		
		if ( _vertexPointSizes )
			_vertexPointSizes->createGLBuffer();
		
		if ( _vertexTextureCoordinates )
			_vertexTextureCoordinates->createGLBuffer();

		CCObject* pObj;
		CCARRAY_FOREACH( _overlayTextureCoordinates, pObj )
		{
			CC3VertexTextureCoordinates* otc = (CC3VertexTextureCoordinates*)pObj;
			otc->createGLBuffer();
		}
	}

	if ( _vertexIndices )
		_vertexIndices->createGLBuffer();
}

void CC3Mesh::deleteGLBuffers()
{
	if ( _vertexLocations )
		_vertexLocations->deleteGLBuffer();
	
	if ( _vertexNormals )
		_vertexNormals->deleteGLBuffer();
	
	if ( _vertexTangents )
		_vertexTangents->deleteGLBuffer();
	
	if ( _vertexBitangents )
		_vertexBitangents->deleteGLBuffer();
	
	if ( _vertexColors )
		_vertexColors->deleteGLBuffer();
	
	if ( _vertexBoneIndices )
		_vertexBoneIndices->deleteGLBuffer();

	if ( _vertexBoneWeights )
		_vertexBoneWeights->deleteGLBuffer();
	
	if ( _vertexPointSizes )
		_vertexPointSizes->deleteGLBuffer();
	
	if ( _vertexTextureCoordinates )
		_vertexTextureCoordinates->deleteGLBuffer();
	CCObject* pObj;
	CCARRAY_FOREACH( _overlayTextureCoordinates, pObj )
	{
		CC3VertexTextureCoordinates* otc = (CC3VertexTextureCoordinates*)pObj;
		otc->deleteGLBuffer();
	}

	if ( _vertexIndices )
		_vertexIndices->deleteGLBuffer();
}

bool CC3Mesh::isUsingGLBuffers()
{
	if (_vertexLocations && _vertexLocations->isUsingGLBuffer()) return true;
	if (_vertexNormals && _vertexNormals->isUsingGLBuffer()) return true;
	if (_vertexTangents && _vertexTangents->isUsingGLBuffer()) return true;
	if (_vertexBitangents && _vertexBitangents->isUsingGLBuffer()) return true;
	if (_vertexColors && _vertexColors->isUsingGLBuffer()) return true;
	if (_vertexBoneIndices && _vertexBoneIndices->isUsingGLBuffer()) return true;
	if (_vertexBoneWeights && _vertexBoneWeights->isUsingGLBuffer()) return true;
	if (_vertexPointSizes && _vertexPointSizes->isUsingGLBuffer()) return true;
	if (_vertexTextureCoordinates && _vertexTextureCoordinates->isUsingGLBuffer()) return true;
	CCObject* pObj;
	CCARRAY_FOREACH( _overlayTextureCoordinates, pObj )
	{
		CC3VertexTextureCoordinates* otc = (CC3VertexTextureCoordinates*)pObj;
		 if (otc->isUsingGLBuffer()) 
			 return true;
	}
	
	return false;
}

void CC3Mesh::releaseRedundantContent()
{
	if ( _vertexLocations )
		_vertexLocations->releaseRedundantContent();
	if ( _vertexNormals )
		_vertexNormals->releaseRedundantContent();
	if ( _vertexTangents )
		_vertexTangents->releaseRedundantContent();
	if ( _vertexBitangents )
		_vertexBitangents->releaseRedundantContent();
	if ( _vertexColors )
		_vertexColors->releaseRedundantContent();
	if ( _vertexBoneIndices )
		_vertexBoneIndices->releaseRedundantContent();
	if ( _vertexBoneWeights )
		_vertexBoneWeights->releaseRedundantContent();
	if ( _vertexPointSizes )
		_vertexPointSizes->releaseRedundantContent();
	if ( _vertexTextureCoordinates )
		_vertexTextureCoordinates->releaseRedundantContent();
	CCObject* pObj;
	CCARRAY_FOREACH( _overlayTextureCoordinates, pObj )
	{
		CC3VertexTextureCoordinates* otc = (CC3VertexTextureCoordinates*)pObj;
		otc->releaseRedundantContent();
	}

	if ( _vertexIndices )
		_vertexIndices->releaseRedundantContent();
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
	if ( _vertexLocations )
		_vertexLocations->setShouldReleaseRedundantContent( false ); 
}

void CC3Mesh::retainVertexNormals()
{
	if ( !hasVertexNormals() ) return;
	
	if (_shouldInterleaveVertices) 
		retainVertexLocations();

	if ( _vertexNormals )
		_vertexNormals->setShouldReleaseRedundantContent( false );
}

void CC3Mesh::retainVertexTangents()
{
	if ( !hasVertexTangents() ) return;
	
	if (_shouldInterleaveVertices) 
		retainVertexLocations();

	if ( _vertexTangents )
		_vertexTangents->setShouldReleaseRedundantContent( false );
}

void CC3Mesh::retainVertexBitangents()
{
	if ( !hasVertexBitangents() ) 
		return;
	
	if (_shouldInterleaveVertices) 
		retainVertexLocations();

	if ( _vertexBitangents )
		_vertexBitangents->setShouldReleaseRedundantContent( false );
}

void CC3Mesh::retainVertexColors()
{
	if ( !hasVertexColors() ) 
		return;
	
	if (_shouldInterleaveVertices) 
		retainVertexLocations();

	if ( _vertexColors )
		_vertexColors->setShouldReleaseRedundantContent( false );
}

void CC3Mesh::retainVertexBoneWeights()
{
	if ( !hasVertexBoneWeights() )
		return;
	
	if (_shouldInterleaveVertices) 
		retainVertexLocations();

	if ( _vertexBoneWeights )
		_vertexBoneWeights->setShouldReleaseRedundantContent( false );
}

void CC3Mesh::retainVertexBoneIndices()
{
	if ( !hasVertexBoneIndices() ) 
		return;
	
	if (_shouldInterleaveVertices) 
		retainVertexLocations();

	if ( _vertexBoneIndices )
		_vertexBoneIndices->setShouldReleaseRedundantContent( false );
}

void CC3Mesh::retainVertexPointSizes()
{
	if ( !hasVertexPointSizes() ) 
		return;
	
	if (_shouldInterleaveVertices) 
		retainVertexLocations();

	if ( _vertexPointSizes ) 
		_vertexPointSizes->setShouldReleaseRedundantContent( false );
}

void CC3Mesh::retainVertexTextureCoordinates()
{
	if ( !hasVertexTextureCoordinates() ) 
		return;
	
	if (_shouldInterleaveVertices) 
		retainVertexLocations();

	if ( _vertexTextureCoordinates )
		_vertexTextureCoordinates->setShouldReleaseRedundantContent( false );

	CCObject* pObj;
	CCARRAY_FOREACH( _overlayTextureCoordinates, pObj )
	{
		CC3VertexTextureCoordinates* otc = (CC3VertexTextureCoordinates*)pObj;
		otc->setShouldReleaseRedundantContent( false );
	}
}

void CC3Mesh::retainVertexIndices()
{
	if ( _vertexIndices )
		_vertexIndices->setShouldReleaseRedundantContent( false ); 
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
	_vertexLocations->setShouldAllowVertexBuffering( false ); 
}

void CC3Mesh::doNotBufferVertexNormals()
{
	if (_shouldInterleaveVertices) 
		doNotBufferVertexLocations();
	_vertexNormals->setShouldAllowVertexBuffering( false );
}

void CC3Mesh::doNotBufferVertexTangents()
{
	if (_shouldInterleaveVertices) 
		doNotBufferVertexLocations();

	if ( _vertexTangents )
		_vertexTangents->setShouldAllowVertexBuffering( false );
}

void CC3Mesh::doNotBufferVertexBitangents()
{
	if (_shouldInterleaveVertices) 
		doNotBufferVertexLocations();

	if ( _vertexBitangents )
		_vertexBitangents->setShouldAllowVertexBuffering( false );
}

void CC3Mesh::doNotBufferVertexColors()
{
	if (_shouldInterleaveVertices) 
		doNotBufferVertexLocations();

	if ( _vertexColors )
		_vertexColors->setShouldAllowVertexBuffering( false );
}

void CC3Mesh::doNotBufferVertexBoneWeights()
{
	if (_shouldInterleaveVertices) 
		doNotBufferVertexLocations();

	if ( _vertexBoneWeights )
		_vertexBoneWeights->setShouldAllowVertexBuffering( false );
}

void CC3Mesh::doNotBufferVertexBoneIndices()
{
	if (_shouldInterleaveVertices) 
		doNotBufferVertexLocations();

	if ( _vertexBoneIndices )
		_vertexBoneIndices->setShouldAllowVertexBuffering( false );
}

void CC3Mesh::doNotBufferVertexPointSizes()
{
	if (_shouldInterleaveVertices) 
		doNotBufferVertexLocations();

	if ( _vertexPointSizes )
		_vertexPointSizes->setShouldAllowVertexBuffering( false );
}

void CC3Mesh::doNotBufferVertexTextureCoordinates()
{
	if (_shouldInterleaveVertices) 
		doNotBufferVertexLocations();

	if ( _vertexTextureCoordinates )
		_vertexTextureCoordinates->setShouldAllowVertexBuffering( false );

	CCObject* pObj;
	CCARRAY_FOREACH( _overlayTextureCoordinates, pObj )
	{
		CC3VertexTextureCoordinates* otc = (CC3VertexTextureCoordinates*)pObj;
		otc->setShouldAllowVertexBuffering( false );
	}
}

void CC3Mesh::doNotBufferVertexIndices()
{ 
	if ( _vertexIndices )
		_vertexIndices->setShouldAllowVertexBuffering( false ); 
}

void CC3Mesh::updateGLBuffersStartingAt( GLuint offsetIndex, GLuint vertexCount )
{
	if ( _vertexLocations )
		_vertexLocations->updateGLBufferStartingAt( offsetIndex, vertexCount );

	if ( !_shouldInterleaveVertices )
	{
		if ( _vertexNormals )
			_vertexNormals->updateGLBufferStartingAt( offsetIndex, vertexCount );
		if ( _vertexTangents )
			_vertexTangents->updateGLBufferStartingAt( offsetIndex, vertexCount );
		if ( _vertexBitangents )
			_vertexBitangents->updateGLBufferStartingAt( offsetIndex, vertexCount );
		if ( _vertexColors )
			_vertexColors->updateGLBufferStartingAt( offsetIndex, vertexCount );
		if ( _vertexBoneIndices )
			_vertexBoneIndices->updateGLBufferStartingAt( offsetIndex, vertexCount );
		if ( _vertexBoneWeights )
			_vertexBoneWeights->updateGLBufferStartingAt( offsetIndex, vertexCount );
		if ( _vertexPointSizes )
			_vertexPointSizes->updateGLBufferStartingAt( offsetIndex, vertexCount );
		if ( _vertexTextureCoordinates )
			_vertexTextureCoordinates->updateGLBufferStartingAt( offsetIndex, vertexCount );

		CCObject* pObj;
		CCARRAY_FOREACH( _overlayTextureCoordinates, pObj )
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
	if ( _vertexLocations )
		_vertexLocations->updateGLBuffer(); 
}

void CC3Mesh::updateVertexNormalsGLBuffer()
{
	if ( _vertexNormals )
		_vertexNormals->updateGLBuffer(); 
}

void CC3Mesh::updateVertexTangentsGLBuffer()
{
	if ( _vertexTangents )
		_vertexTangents->updateGLBuffer(); 
}

void CC3Mesh::updateVertexBitangentsGLBuffer()
{
	if ( _vertexBitangents )
		_vertexBitangents->updateGLBuffer(); 
}

void CC3Mesh::updateVertexColorsGLBuffer()
{
	if ( _vertexColors )
		_vertexColors->updateGLBuffer(); 
}

void CC3Mesh::updateVertexBoneWeightsGLBuffer()
{
	if ( _vertexBoneWeights )
		_vertexBoneWeights->updateGLBuffer(); 
}

void CC3Mesh::updateVertexBoneIndicesGLBuffer()
{
	if ( _vertexBoneIndices )
		_vertexBoneIndices->updateGLBuffer(); 
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
	if ( _vertexIndices )
		_vertexIndices->updateGLBuffer(); 
}

CC3Vector CC3Mesh::getCenterOfGeometry()
{
	return _vertexLocations ? _vertexLocations->getCenterOfGeometry() : CC3Vector::kCC3VectorZero; 
}

CC3Box CC3Mesh::getBoundingBox()
{
	return _vertexLocations ? _vertexLocations->getBoundingBox() : CC3Box::kCC3BoxNull;
}

GLfloat CC3Mesh::getRadius()
{
	return _vertexLocations ? _vertexLocations->getRadius() : 0.0f; 
}

void CC3Mesh::moveMeshOriginTo( const CC3Vector& aLocation )
{ 
	if ( _vertexLocations )
		_vertexLocations->moveMeshOriginTo( aLocation ); 
}

void CC3Mesh::moveMeshOriginToCenterOfGeometry()
{
	if ( _vertexLocations )
		_vertexLocations->moveMeshOriginToCenterOfGeometry(); 
}


CCColorRef CC3Mesh::getColor()
{
	return _vertexColors ? _vertexColors->getColor() : CCColorRefFromCCC4F(kCCC4FBlackTransparent); 
}

void CC3Mesh::setColor( const CCColorRef& aColor )
{ 
	if ( _vertexColors )
		_vertexColors->setColor( aColor ); 
}

CCOpacity CC3Mesh::getOpacity()
{ 
	return _vertexColors ? _vertexColors->getOpacity() : kCCOpacityFull; 
}

void CC3Mesh::setOpacity( CCOpacity opacity )
{
	if ( _vertexColors )
		_vertexColors->setOpacity( opacity ); 
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
	if (_vertexIndices) 
		return _vertexIndices->getDrawingMode();
	if (_vertexLocations) 
		return _vertexLocations->getDrawingMode();
	return GL_TRIANGLES;
}

void CC3Mesh::setDrawingMode( GLenum aMode )
{
	if ( _vertexIndices )
		_vertexIndices->setDrawingMode( aMode );
	if ( _vertexLocations )
		_vertexLocations->setDrawingMode( aMode );
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

	if (_vertexIndices)
		_vertexIndices->drawWithVisitor( visitor );
	else if ( _vertexLocations )
		_vertexLocations->drawWithVisitor( visitor );
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
	//LogTrace(@"Drawing %@ from %u for %u vertices", self, vertexIndex, vertexCount);

	CC3ShaderProgram* pShaderProgram = visitor->getCurrentShaderProgram();
	if ( pShaderProgram )
		pShaderProgram->populateDrawScopeUniformsWithVisitor( visitor );

	if (_vertexIndices)
		_vertexIndices->drawFrom( vertexIndex, vertexCount, visitor );
	else if ( _vertexLocations )
		_vertexLocations->drawFrom( vertexIndex, vertexCount, visitor );
}

void CC3Mesh::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	_vertexLocations = NULL;
	_vertexNormals = NULL;
	_vertexTangents = NULL;
	_vertexBitangents = NULL;
	_vertexColors = NULL;
	_vertexBoneIndices = NULL;
	_vertexBoneWeights = NULL;
	_vertexPointSizes = NULL;
	_vertexTextureCoordinates = NULL;
	_overlayTextureCoordinates = NULL;
	_vertexIndices = NULL;
	_faces = NULL;
	_shouldInterleaveVertices = true;
	_capacityExpansionFactor = 1.25f;
}

void CC3Mesh::initWithTag( GLuint aTag )
{
	super::initWithTag( aTag );
}

void CC3Mesh::populateFrom( CC3Mesh* another )
{
	super::populateFrom( another );
	
	_shouldInterleaveVertices = another->shouldInterleaveVertices();
	_capacityExpansionFactor = another->getCapacityExpansionFactor();
	
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
	_overlayTextureCoordinates->removeAllObjects();
	CCArray* otherOTCs = another->_overlayTextureCoordinates;

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

	//LogTrace(@"%@ populating as disk with radius: %.3f, %i radial divs, %i angular divs, %i vertices, and %i triangles",
	//			  self, radius, numRadialDivs, numAngularDivs, vertexCount, triangleCount);

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
			setVertexLocation( unitRadial.scaleUniform( radius ), vIndx );

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
	_mesh = NULL;	
}

CC3FaceArray::~CC3FaceArray()
{
	_mesh = NULL;					// weak reference
	deallocateIndices();
	deallocateCenters();
	deallocateNormals();
	deallocatePlanes();
	deallocateNeighbours();
}

CC3Mesh* CC3FaceArray::getMesh()
{
	return _mesh;
}

void CC3FaceArray::setMesh( CC3Mesh* aMesh )
{
	_mesh = aMesh;		// weak reference
	deallocateIndices();
	deallocateCenters();
	deallocateNormals();
	deallocatePlanes();
	deallocateNeighbours();
}

/** If turning off, clears all caches except neighbours. */
void CC3FaceArray::setShouldCacheFaces( bool shouldCache )
{
	_shouldCacheFaces = shouldCache;
	if (!_shouldCacheFaces) 
	{
		deallocateIndices();
		deallocateCenters();
		deallocateNormals();
		deallocatePlanes();
	}
}

bool CC3FaceArray::shouldCacheFaces()
{
	return _shouldCacheFaces;
}

GLuint CC3FaceArray::getFaceCount()
{ 
	return _mesh ? _mesh->getFaceCount() : 0;
}

CC3Face CC3FaceArray::getFaceAt( GLuint faceIndex )
{
	return _mesh ? _mesh->getFaceAt( faceIndex ) : CC3Face::kCC3FaceZero;
}

void CC3FaceArray::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	_mesh = NULL;
	_shouldCacheFaces = false;
	_indices = NULL;
	_indicesAreRetained = false;
	_indicesAreDirty = true;
	_centers = NULL;
	_centersAreRetained = false;
	_centersAreDirty = true;
	_normals = NULL;
	_normalsAreRetained = false;
	_normalsAreDirty = true;
	_planes = NULL;
	_planesAreRetained = false;
	_planesAreDirty = true;
	_neighbours = NULL;
	_neighboursAreRetained = false;
	_neighboursAreDirty = true;
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
	
	_mesh = another->getMesh();		// weak reference
	
	_shouldCacheFaces = another->shouldCacheFaces();
	
	// If indices should be retained, allocate memory and copy the data over.
	deallocateIndices();
	if (another->_indicesAreRetained) 
	{
		allocateIndices();
		memcpy(_indices, another->getIndices(), (getFaceCount() * sizeof(CC3FaceIndices)));
	} else {
		_indices = another->getIndices();
	}
	_indicesAreDirty = another->_indicesAreDirty;
	
	// If centers should be retained, allocate memory and copy the data over.
	deallocateCenters();
	if (another->_centersAreRetained) 
	{
		allocateCenters();
		memcpy(_centers, another->getCenters(), (getFaceCount() * sizeof(CC3Vector)));
	} else {
		_centers = another->getCenters();
	}
	_centersAreDirty = another->_centersAreDirty;
	
	// If normals should be retained, allocate memory and copy the data over.
	deallocateNormals();
	if (another->_normalsAreRetained) 
	{
		allocateNormals();
		memcpy(_normals, another->getNormals(), (getFaceCount() * sizeof(CC3Vector)));
	} else {
		_normals = another->getNormals();
	}
	_normalsAreDirty = another->_normalsAreDirty;
	
	// If planes should be retained, allocate memory and copy the data over.
	deallocatePlanes();
	if (another->_planesAreRetained) {
		allocatePlanes();
		memcpy(_planes, another->getPlanes(), (getFaceCount() * sizeof(CC3Plane)));
	} else {
		_planes = another->getPlanes();
	}
	_planesAreDirty = another->_planesAreDirty;
	
	// If neighbours should be retained, allocate memory and copy the data over.
	deallocateNeighbours();
	if (another->_neighboursAreRetained) {
		allocateNeighbours();
		memcpy(_neighbours, another->getNeighbours(), (getFaceCount() * sizeof(CC3FaceNeighbours)));
	} else {
		_neighbours = another->getNeighbours();
	}
	_neighboursAreDirty = another->_neighboursAreDirty;
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
	if (_indicesAreDirty || !_indices) 
		populateIndices();
	return _indices;
}

void CC3FaceArray::setIndices( CC3FaceIndices* faceIndices )
{
	deallocateIndices();			// Safely disposes existing vertices
	_indices = faceIndices;
}

CC3FaceIndices CC3FaceArray::getUncachedIndicesAt( GLuint faceIndex )
{
	return _mesh->getUncachedFaceIndicesAt( faceIndex );
}

CC3FaceIndices CC3FaceArray::getIndicesAt( GLuint faceIndex )
{
	if (_shouldCacheFaces) 
		return getIndices()[faceIndex];

	return getUncachedIndicesAt( faceIndex );
}

CC3FaceIndices* CC3FaceArray::allocateIndices()
{
	deallocateIndices();
	GLuint faceCount = getFaceCount();
	if (faceCount) 
	{
		_indices = (CC3FaceIndices*)calloc(faceCount, sizeof(CC3FaceIndices));
		_indicesAreRetained = true;
		CC3_TRACE("CC3FaceArray allocated space for %d face indices", getFaceCount());
	}
	return _indices;
}

void CC3FaceArray::deallocateIndices()
{
	if (_indicesAreRetained && _indices) 
	{
		free(_indices);
		_indices = NULL;
		_indicesAreRetained = false;
		CC3_TRACE("CC3FaceArray deallocated %d previously allocated indices", getFaceCount());
	}
}

void CC3FaceArray::populateIndices()
{
	CC3_TRACE("%CC3FaceArray populating %d face indices", getFaceCount());
	if ( !_indices ) 
		allocateIndices();
	
	GLuint faceCount = getFaceCount();
	for (GLuint faceIdx = 0; faceIdx < faceCount; faceIdx++) 
	{
		_indices[faceIdx] = getUncachedIndicesAt( faceIdx );
		
		//LogTrace(@"Face %i has indices %@", faceIdx,
		//			  NSStringFromCC3FaceIndices(_indices[faceIdx]));
	}
	_indicesAreDirty = false;
}

void CC3FaceArray::markIndicesDirty()
{
	_indicesAreDirty = true; 
}

CC3Vector* CC3FaceArray::getCenters()
{
	if (_centersAreDirty || !_centers) 
		populateCenters();
	return _centers;
}

void CC3FaceArray::setCenters( CC3Vector* faceCenters )
{
	deallocateCenters();			// Safely disposes existing vertices
	_centers = faceCenters;
}

CC3Vector CC3FaceArray::getCenterAt( GLuint faceIndex )
{
	if (_shouldCacheFaces) 
		return getCenters()[faceIndex];
	return getFaceAt( faceIndex ).getCenter();
}

CC3Vector* CC3FaceArray::allocateCenters()
{
	deallocateCenters();
	GLuint faceCount = getFaceCount();
	if (faceCount) 
	{
		_centers = (CC3Vector*)calloc(faceCount, sizeof(CC3Vector));
		_centersAreRetained = true;
		//LogTrace(@"%@ allocated space for %u face centers", self, faceCount);
	}
	return _centers;
}

void CC3FaceArray::deallocateCenters()
{
	if (_centersAreRetained && _centers) 
	{
		free(_centers);
		_centers = NULL;
		_centersAreRetained = false;
		//LogTrace(@"%@ deallocated %u previously allocated centers", self, self.faceCount);
	}
}

void CC3FaceArray::populateCenters()
{
	//LogTrace(@"%@ populating %u face centers", self, self.faceCount);
	if ( !_centers ) 
		allocateCenters();
	
	GLuint faceCount = getFaceCount();
	for (GLuint faceIdx = 0; faceIdx < faceCount; faceIdx++) 
	{
		_centers[faceIdx] = getFaceAt(faceIdx).getCenter();

		//LogTrace(@"Face %i has vertices %@ and center %@", faceIdx,
		//			  NSStringFromCC3Face([self faceAt: faceIdx]),
		//			  NSStringFromCC3Vector(_centers[faceIdx]));
	}
	_centersAreDirty = false;
}

void CC3FaceArray::markCentersDirty()
{
	_centersAreDirty = true; 
}

CC3Vector* CC3FaceArray::getNormals()
{
	if (_normalsAreDirty || !_normals) 
		populateNormals();

	return _normals;
}

void CC3FaceArray::setNormals( CC3Vector* faceNormals )
{
	deallocateNormals(); 			// Safely disposes existing vertices
	_normals = faceNormals;
}

CC3Vector CC3FaceArray::getNormalAt( GLuint faceIndex )
{
	if (_shouldCacheFaces) 
		return getNormals()[faceIndex];
	return getFaceAt(faceIndex).getNormal();
}

CC3Vector* CC3FaceArray::allocateNormals()
{
	deallocateNormals();
	GLuint faceCount = getFaceCount();
	if (faceCount) 
	{
		_normals = (CC3Vector*)calloc(faceCount, sizeof(CC3Vector));
		_normalsAreRetained = true;
		//LogTrace(@"%@ allocated space for %u face normals", self, faceCount);
	}
	return _normals;
}

void CC3FaceArray::deallocateNormals()
{
	if (_normalsAreRetained && _normals) 
	{
		free(_normals);
		_normals = NULL;
		_normalsAreRetained = false;
		//LogTrace(@"%@ deallocated %u previously allocated normals", self, self.faceCount);
	}
}

void CC3FaceArray::populateNormals()
{
	//LogTrace(@"%@ populating %u face normals", self, self.faceCount);
	if ( !_normals ) 
		allocateNormals();
	
	GLuint faceCount = getFaceCount();
	for (GLuint faceIdx = 0; faceIdx < faceCount; faceIdx++) 
	{
		_normals[faceIdx] = getFaceAt(faceIdx).getNormal();
		
		//LogTrace(@"Face %i has vertices %@ and normal %@", faceIdx,
		//			  NSStringFromCC3Face([self faceAt: faceIdx]),
		//			  NSStringFromCC3Vector(_normals[faceIdx]));
	}
	_normalsAreDirty = false;
}

void CC3FaceArray::markNormalsDirty()
{
	_normalsAreDirty = true; 
}

CC3Plane* CC3FaceArray::getPlanes()
{
	if (_planesAreDirty || !_planes) 
		populatePlanes();

	return _planes;
}

void CC3FaceArray::setPlanes( CC3Plane* facePlanes )
{
	deallocatePlanes();			// Safely disposes existing vertices
	_planes = facePlanes;
}

CC3Plane CC3FaceArray::getPlaneAt( GLuint faceIndex )
{
	if (_shouldCacheFaces) 
		return getPlanes()[faceIndex];
    return CC3Plane::planeFromFace( getFaceAt(faceIndex) );
}

CC3Plane* CC3FaceArray::allocatePlanes()
{
	deallocatePlanes();
	GLuint faceCount = getFaceCount();
	if (faceCount) 
	{
		_planes = (CC3Plane*)calloc(faceCount, sizeof(CC3Plane));
		_planesAreRetained = true;
		//LogTrace(@"%@ allocated space for %u face planes", self, faceCount);
	}
	return _planes;
}

void CC3FaceArray::deallocatePlanes()
{
	if (_planesAreRetained && _planes) 
	{
		free(_planes);
		_planes = NULL;
		_planesAreRetained = false;
		//LogTrace(@"%@ deallocated %u previously allocated planes", self, self.faceCount);
	}
}

void CC3FaceArray::populatePlanes()
{
	//LogTrace(@"%@ populating %u face planes", self, self.faceCount);
	if ( !_planes ) 
		allocatePlanes();
	
	GLuint faceCount = getFaceCount();
	for (GLuint faceIdx = 0; faceIdx < faceCount; faceIdx++) 
	{
        _planes[faceIdx] = CC3Plane::planeFromFace( getFaceAt(faceIdx) );
		
		//LogTrace(@"Face %i has vertices %@ and plane %@", faceIdx,
		//			  NSStringFromCC3Face([self faceAt: faceIdx]),
		//			  NSStringFromCC3Plane(_planes[faceIdx]));
	}
	_planesAreDirty = false;
}

void CC3FaceArray::markPlanesDirty()
{
	_planesAreDirty = true; 
}

CC3FaceNeighbours* CC3FaceArray::getNeighbours()
{
	if (_neighboursAreDirty || !_neighbours) 
		populateNeighbours();

	return _neighbours;
}

void CC3FaceArray::setNeighbours( CC3FaceNeighbours* faceNeighbours )
{
	deallocateNeighbours();		// Safely disposes existing vertices
	_neighbours = faceNeighbours;
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
		_neighbours = (CC3FaceNeighbours*)calloc(faceCount, sizeof(CC3FaceNeighbours));
		_neighboursAreRetained = true;
		//LogTrace(@"%@ allocated space for %u face neighbours", self, faceCount);
	}
	return _neighbours;
}

void CC3FaceArray::deallocateNeighbours()
{
	if (_neighboursAreRetained && _neighbours) 
	{
		free(_neighbours);
		_neighbours = NULL;
		_neighboursAreRetained = false;
		//LogTrace(@"%@ deallocated %u previously allocated neighbour structures", self, self.faceCount);
	}
}

void CC3FaceArray::populateNeighbours()
{
	//LogTrace(@"%@ populating neighbours for %u faces", self, self.faceCount);
	if ( !_neighbours ) 
		allocateNeighbours();
	
	GLuint faceCnt = getFaceCount();
	
	// Break all neighbour links. Done in batch so that we can skip
	// testing neighbour connections from both directions later.
	for (GLuint faceIdx = 0; faceIdx < faceCnt; faceIdx++) 
	{
		GLuint* neighbourEdge = _neighbours[faceIdx].edges;
		neighbourEdge[0] = neighbourEdge[1] = neighbourEdge[2] = kCC3FaceNoNeighbour;
	}
	
	// Iterate through all the faces
	for (GLuint f1Idx = 0; f1Idx < faceCnt; f1Idx++)
	{
		// Get the neighbours of the current face, and if any of the edges still
		// need to have a neighbour assigned, look for them. We check this early
		// to avoid iterating through the remaining faces
		GLuint* f1Neighbours = _neighbours[f1Idx].edges;
		if (f1Neighbours[0] == kCC3FaceNoNeighbour ||
			f1Neighbours[1] == kCC3FaceNoNeighbour ||
			f1Neighbours[2] == kCC3FaceNoNeighbour) 
		{

			// For the current face, retrieve the vertex indices
			GLuint* f1Vertices = _mesh->getFaceIndicesAt(f1Idx).vertices;
			
			// Iterate through all the faces beyond the current face
			for (GLuint f2Idx = f1Idx + 1; f2Idx < faceCnt; f2Idx++) 
			{
				// Get the neighbours of the other face, and if any of the edges still
				// need to have a neighbour assigned, see if any of the edges between
				// the current face and other face match. We check for neighbours early
				// to avoid iterating through all the face combinations.
				GLuint* f2Neighbours = _neighbours[f2Idx].edges;
				if (f2Neighbours[0] == kCC3FaceNoNeighbour ||
					f2Neighbours[1] == kCC3FaceNoNeighbour ||
					f2Neighbours[2] == kCC3FaceNoNeighbour) 
				{
					// For the other face, retrieve the vertex indices
					GLuint* f2Vertices = _mesh->getFaceIndicesAt(f2Idx).vertices;
					
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
										/*LogTrace(@"Matched face %@ with face %@",
													  NSStringFromCC3FaceIndices(_indices[f1Idx]),
													  NSStringFromCC3FaceIndices(_indices[f2Idx]));*/
									}
								}
							}
						}
					}
				}
			}
			/*LogTrace(@"Face %i has indices %@ and neighbours %@", f1Idx,
						  NSStringFromCC3FaceIndices([_mesh faceIndicesAt: f1Idx]),
						  NSStringFromCC3FaceNeighbours(_neighbours[f1Idx]));*/
		}
		
	}
	_neighboursAreDirty = false;
	//LogTrace(@"%@ finished building neighbours", self);
}

void CC3FaceArray::markNeighboursDirty()
{
	_neighboursAreDirty = true; 
}

NS_COCOS3D_END
