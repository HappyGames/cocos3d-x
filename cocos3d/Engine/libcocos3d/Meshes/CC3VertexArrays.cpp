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

CC3VertexArrayContent::CC3VertexArrayContent()
{
	init();
}

void CC3VertexArrayContent::init()
{
	{
		_vertices = NULL;
		_vertexCount = 0;
//		_allocatedVertexCapacity = 0;
		_vertexStride = 0;
		_bufferID = 0;
		_bufferUsage = GL_STATIC_DRAW;
		_shouldAllowVertexBuffering = true;
		_shouldReleaseRedundantContent = true;
	}
}

CC3VertexArray::CC3VertexArray()
{

}

CC3VertexArray::~CC3VertexArray()
{
	deleteGLBuffer();
	setAllocatedVertexCapacity( 0 );
}

GLvoid* CC3VertexArray::getVertices()
{
	return _vertices; 
}

/**
 * Safely disposes of any existing allocated vertex memory, maintaining the existing vertex
 * count while doing so if the new vertices are not empty. This handles the case where vertex
 * count is set before vertices. Also, notify subclasses that the vertices have changed.
 */
void CC3VertexArray::setVertices( GLvoid* vtxs )
{
	if (vtxs != _vertices) 
	{
		GLuint currVtxCount = _vertexCount;
		setAllocatedVertexCapacity( 0 );		// Safely disposes existing vertices
		_vertices = vtxs;
		if (_vertices) 
			_vertexCount = currVtxCount;
		
		verticesWereChanged();
	}
}

/** The vertices array has been changed. Default is to do nothing. Some subclasses may want to react. */
void CC3VertexArray::verticesWereChanged()
{

}

GLint CC3VertexArray::getElementSize()
{
	return _elementSize; 
}

/**
 * If the element size is set after vertex memory has been allocated, reallocate it.
 * If that reallocation fails, don't change the size.
 */
void CC3VertexArray::setElementSize( GLint elemSize )
{
	GLint currSize = _elementSize;
	_elementSize = elemSize;
	if ( !allocateVertexCapacity(_allocatedVertexCapacity) ) 
		_elementSize = currSize;
}

GLenum CC3VertexArray::getElementType()
{
	return _elementType; 
}

/**
 * If the element type is set after vertex memory has been allocated, reallocate it.
 * If that reallocation fails, don't change the size.
 */
void CC3VertexArray::setElementType( GLenum elemType )
{
	GLenum currType = _elementType;
	_elementType = elemType;
	if ( !allocateVertexCapacity(_allocatedVertexCapacity) )
		_elementType = currType;
}

GLuint CC3VertexArray::getElementLength()
{
	return (GLuint)CC3GLElementTypeSize(_elementType) * _elementSize; 
}

GLuint CC3VertexArray::getVertexStride()
{
	return _vertexStride ? _vertexStride : getElementLength(); 
}

/**
 * If the stride is set after vertex memory has been allocated, reallocate it.
 * If that reallocation fails, don't change the stride.
 */
void CC3VertexArray::setVertexStride( GLuint stride )
{
	GLuint currStride = _vertexStride;
	_vertexStride = stride;
	if ( !allocateVertexCapacity(_allocatedVertexCapacity) ) 
		_vertexStride = currStride;
}

void CC3VertexArray::setShouldNormalizeContent( bool shouldNormalize )
{
	_shouldNormalizeContent = shouldNormalize;
}

bool CC3VertexArray::shouldNormalizeContent()
{
	return _shouldNormalizeContent;
}

bool CC3VertexArray::shouldReleaseRedundantContent()
{
	return _shouldReleaseRedundantContent;
}

void CC3VertexArray::setShouldReleaseRedundantContent( bool release )
{
	_shouldReleaseRedundantContent = release;
}

bool CC3VertexArray::shouldAllowVertexBuffering()
{
	return _shouldAllowVertexBuffering;
}

void CC3VertexArray::setShouldAllowVertexBuffering( bool allowVertexBuffering )
{
	_shouldAllowVertexBuffering = allowVertexBuffering;
}

GLuint CC3VertexArray::getBufferUsage()
{
	return _bufferUsage;
}

void CC3VertexArray::setBufferUsage( GLenum usage )
{
	_bufferUsage = usage;
}

GLuint CC3VertexArray::getSemantic()
{
	return _semantic;
}

GLuint CC3VertexArray::getElementOffset()
{
	return _elementOffset;
}

void CC3VertexArray::setElementOffset( GLuint elementOffset )
{
	_elementOffset = elementOffset;
}

GLuint CC3VertexArray::getAvailableVertexCount()
{
	return (_allocatedVertexCapacity > 0) ? _allocatedVertexCapacity : _vertexCount;
}

GLenum CC3VertexArray::getBufferTarget()
{
	return GL_ARRAY_BUFFER; 
}

GLenum CC3VertexArray::defaultSemantic()
{
	//CCAssert(false, "CC3VertexArray does not implement the defaultSemantic class property");
	return kCC3SemanticNone;
}

void CC3VertexArray::setBufferID( GLuint bufferID )
{
	_bufferID = bufferID;
}

GLuint CC3VertexArray::getBufferID()
{
	return _bufferID;
}

void CC3VertexArray::setVertexCount( GLuint count )
{
	_vertexCount = count;
}

GLuint CC3VertexArray::getVertexCount()
{
	return _vertexCount;
}

void CC3VertexArray::setSemantic( GLenum semantic )
{
	_semantic = semantic;
}

void CC3VertexArray::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName ); 
	{
		_vertices = NULL;
		_vertexCount = 0;
		_allocatedVertexCapacity = 0;
		_elementType = GL_FLOAT;
		_elementSize = 3;
		_vertexStride = 0;
		_bufferID = 0;
		_bufferUsage = GL_STATIC_DRAW;
		_elementOffset = 0;
		_shouldNormalizeContent = false;
		_shouldAllowVertexBuffering = true;
		_shouldReleaseRedundantContent = true;
		_semantic = defaultSemantic();
	}
}

void CC3VertexArray::initWithTag( GLuint aTag )
{
	super::initWithTag( aTag );
}

CC3VertexArray* CC3VertexArray::vertexArray()
{
	CC3VertexArray* pArray = new CC3VertexArray;
	pArray->init();
	pArray->autorelease();
	return pArray;
}

CC3VertexArray* CC3VertexArray::vertexArrayWithTag( GLuint aTag )
{
	CC3VertexArray* pArray = new CC3VertexArray;
	pArray->initWithTag( aTag );
	pArray->autorelease();

	return pArray;
}

CC3VertexArray* CC3VertexArray::vertexArrayWithName( const std::string& aName )
{
	CC3VertexArray* pArray = new CC3VertexArray;
	pArray->initWithName( aName );
	pArray->autorelease();

	return pArray;
}

CC3VertexArray* CC3VertexArray::vertexArrayWithTag( GLuint aTag, const std::string& aName )
{
	CC3VertexArray* pArray = new CC3VertexArray;
	pArray->initWithTag( aTag, aName.c_str() );
	pArray->autorelease();

	return pArray;
}

GLvoid* CC3VertexArray::interleaveWith( CC3VertexArray* otherVtxArray, GLuint elemOffset )
{
	setVertexStride( otherVtxArray->getVertexStride() );
	setVertexCount( otherVtxArray->getVertexCount() );
	setElementOffset( elemOffset );
	setVertices( otherVtxArray->getVertices() );		// Must do last, because can be cleared by other setters.
	return (GLbyte*)getVertices()  + getElementOffset();
}

GLvoid* CC3VertexArray::interleaveWith( CC3VertexArray* otherVtxArray )
{
	return interleaveWith( otherVtxArray, getElementOffset() );
}

void CC3VertexArray::populateFrom( CC3VertexArray* another )
{
	super::populateFrom( another ); 

	_semantic = another->getSemantic();
	_elementType = another->getElementType();
	_elementSize = another->getElementSize();
	_vertexStride = another->getVertexStride();
	_bufferUsage = another->getBufferUsage();
	_elementOffset = another->getElementOffset();
	_shouldNormalizeContent = another->shouldNormalizeContent();
	_shouldAllowVertexBuffering = another->shouldAllowVertexBuffering();
	_shouldReleaseRedundantContent = another->shouldReleaseRedundantContent();

	deleteGLBuffer();		// Data has yet to be buffered. Get rid of old buffer if necessary.

	// If the original has its data stored in memory that it allocated, allocate the
	// same amount in this copy, and copy the data over. Otherwise, the memory is
	// being managed externally, so simply copy the vertices reference over.
	if (another->getAllocatedVertexCapacity()) 
	{
		setAllocatedVertexCapacity( another->getAllocatedVertexCapacity() );
		memcpy(_vertices, another->getVertices(), (_allocatedVertexCapacity * getVertexStride()));
	} else {
		_vertices = another->getVertices();
	}
	_vertexCount = another->getVertexCount();
}

CCObject* CC3VertexArray::copyWithZone( CCZone* zone )
{
	CC3VertexArray* pVal = new CC3VertexArray;
	pVal->init();
	pVal->populateFrom( this );
	
	return pVal;
}

GLuint CC3VertexArray::getAllocatedVertexCapacity()
{
	return _allocatedVertexCapacity; 
}

void CC3VertexArray::setAllocatedVertexCapacity( GLuint vtxCount )
{
	CCAssert((vtxCount == 0 || getVertexStride() > 0), 
		"CC3VertexArray must have the stride defined before allocating vertices. Set the elementType and elementSize properties before setting the allocatedVertexCapacity property.");

	allocateVertexCapacity( vtxCount );
}

bool CC3VertexArray::allocateVertexCapacity( GLuint vtxCount )
{
	// If current capacity is zero, we may still have an externally set pointer. clear it now so that
	// we don't reallocate it, or in case of reverting back to zero, we don't leave the pointer hanging.
	if (_allocatedVertexCapacity == 0) 
		_vertices = NULL;

	// If nothing is changing, we don't need to do anything else.
	// Do this after testing for current zero capacity and clearing pointer.
	if (_allocatedVertexCapacity == vtxCount) 
		return true;

	GLvoid* newVertices = NULL;
	
	// Don't use realloc to free memory that was previously allocated. Behaviour of realloc is
	// undefined and implementation dependent when the requested size is zero.
	if (vtxCount > 0) 
	{
		// Returned pointer will be non-NULL on successful allocation and NULL on failed allocation.
		// If we fail, log an error and return without changing anything.
		newVertices = realloc(_vertices, (vtxCount * getVertexStride()));
		if ( !newVertices ) 
		{
			CCLOGERROR("[vtx]CC3VertexArray could not allocate space for %d vertices");
			return false;
		}
	} else {
		free(_vertices);
	}
	
	CC3_TRACE("[vtx]CC3VertexArray changed vertex allocation from %d vertices to %d vertices", _allocatedVertexCapacity, vtxCount);
	
	// Don't use vertices setter, because it will attempt to deallocate again.
	// But do notify subclasses that the vertices have changed.
	_vertices = newVertices;
	_allocatedVertexCapacity = vtxCount;
	_vertexCount = vtxCount;
	verticesWereChanged();
	
	return true;
}


static GLuint lastAssignedVertexArrayTag;

GLuint CC3VertexArray::nextTag()
{
	return ++lastAssignedVertexArrayTag; 
}

void CC3VertexArray::resetTagAllocation()
{
	lastAssignedVertexArrayTag = 0; 
}

void CC3VertexArray::createGLBuffer()
{
	if ( _shouldAllowVertexBuffering && !_bufferID ) 
	{
		CC3OpenGL* gl = CC3OpenGL::sharedGL();
		GLenum targBuf = getBufferTarget();
		GLsizeiptr buffSize = getVertexStride() * getAvailableVertexCount();
		
		_bufferID = gl->generateBuffer();
		gl->bindBuffer( _bufferID, targBuf );
		gl->loadBufferTarget( targBuf, _vertices, buffSize, _bufferUsage );
		gl->setBufferDebugLabel( getName().c_str(), _bufferID );
		gl->unbindBufferTarget( targBuf );
	} 
	else 
	{
		CC3_TRACE("[vtx]CC3VertexArray NOT creating GL server buffer because shouldAllowVertexBuffering is %s or buffer ID already set to %d",
			_shouldAllowVertexBuffering ? "true" : "false", _bufferID);
	}
}

void CC3VertexArray::updateGLBufferStartingAt( GLuint offsetIndex, GLuint vtxCount )
{
	if (_bufferID) 
	{
		CC3OpenGL* gl = CC3OpenGL::sharedGL();
		GLenum targBuf = getBufferTarget();
		GLuint vtxStride = getVertexStride();

		gl->bindBuffer( _bufferID, targBuf );
		gl->updateBufferTarget( targBuf, _vertices, (offsetIndex * vtxStride), (vtxCount * vtxStride) );
		gl->unbindBufferTarget( targBuf );

//		CCLOG_TRACE("[vtx]CC3VertexArray updated GL server buffer with %d bytes starting at %d", (vtxCount * vtxStride), (offsetIndex * vtxStride));
	}
}

void CC3VertexArray::updateGLBuffer()
{
	updateGLBufferStartingAt( 0, _vertexCount ); 
}

void CC3VertexArray::deleteGLBuffer()
{
	if (_bufferID) 
	{
		CC3_TRACE("[vtx]CC3VertexArray deleting GL server buffer ID %d", _bufferID);
		CC3OpenGL::sharedGL()->deleteBuffer(_bufferID);
		_bufferID = 0;
	}
}

bool CC3VertexArray::isUsingGLBuffer()
{
	return _bufferID != 0; 
}

void CC3VertexArray::releaseRedundantContent()
{
	if ( !(_bufferID && _shouldReleaseRedundantContent) ) 
		return;

	GLuint currVtxCount = _vertexCount;
	setAllocatedVertexCapacity( 0 );
	_vertexCount = currVtxCount;		// Maintain vertexCount for drawing
}

void CC3VertexArray::bindContentToAttributeAt( GLint vaIdx, CC3NodeDrawingVisitor* visitor )
{
	if (_bufferID) 
	{											// use GL buffer if it exists
		// MARK_TRACE_HERE
		//CCLOG("CC3VertexArray binding GL buffer containing %d vertices", _vertexCount);
		visitor->getGL()->bindBuffer( _bufferID, getBufferTarget() );
		bindContent( ((GLbyte*)0 + _elementOffset), vaIdx, visitor );	// Cast handles OSX 64-bit pointers
	} 
	else if (_vertexCount && _vertices) 
	{					// use local client array if it exists
		// MARK_TRACE_HERE
		//CCLOG("CC3VertexArray using local array containing %d vertices", _vertexCount);
		visitor->getGL()->unbindBufferTarget( getBufferTarget() );
		bindContent( ((GLbyte*)_vertices + _elementOffset), vaIdx, visitor );
	} 
	else 
	{
//		CCLOG_TRACE("[vtx]CC3VertexArray no vertices to bind");
	}
}

/**
 * Template method that binds the GL engine to the values of the elementSize, elementType
 * and vertexStride properties, along with the specified data pointer, and enables the
 * type of aspect managed by this instance (locations, normals...) in the GL engine.
 */
void CC3VertexArray::bindContent( GLvoid* pointer, GLint vaIdx, CC3NodeDrawingVisitor* visitor )
{
	visitor->getGL()->bindVertexContent( pointer, _elementSize, _elementType, _vertexStride, _shouldNormalizeContent, vaIdx );
}

GLvoid* CC3VertexArray::getAddressOfElement( GLuint index )
{
	// Check vertices still in memory, and if allocated,
	// that index is less than number of vertices allocated
	CCAssert(_vertices || !_bufferID, "Vertex content is no longer in application memory. To retain mesh data in main memory, invoke the retainVertexContent method on this mesh before invoking the releaseRedundantContent method.");
	CCAssert(_vertices, "Vertex content is missing.");
	CCAssert(_allocatedVertexCapacity == 0 || index < _allocatedVertexCapacity, "Requested index %d is greater than number of vertices allocated: %d."/*, index, _allocatedVertexCapacity*/);
	return (GLbyte*)_vertices + (getVertexStride() * index) + _elementOffset;
}

void CC3VertexArray::copyVertices( GLuint vtxCount, GLuint srcIdx, GLuint dstIdx )
{
	if (vtxCount == 0) 
		return;	// Fail safe. Vertex address may be NULL if no vertices to copy.
	GLvoid* srcPtr = getAddressOfElement( srcIdx );
	GLvoid* dstPtr = getAddressOfElement( dstIdx );
	copyVertices( vtxCount, srcPtr, dstPtr );
}

void CC3VertexArray::copyVertices( GLuint vtxCount, GLuint srcIdx, GLvoid* dstPtr )
{
	if (vtxCount == 0) 
		return;	// Fail safe. Vertex address may be NULL if no vertices to copy.
	GLvoid* srcPtr = getAddressOfElement( srcIdx );
	copyVertices( vtxCount, srcPtr, dstPtr );
}

void CC3VertexArray::copyVertices( GLuint vtxCount, GLvoid* srcPtr, GLuint dstIdx )
{
	if (vtxCount == 0) 
		return;	// Fail safe. Vertex address may be NULL if no vertices to copy.
	GLvoid* dstPtr = getAddressOfElement( dstIdx );
	copyVertices( vtxCount, srcPtr, dstPtr );
}

void CC3VertexArray::copyVertices( GLuint vtxCount, GLvoid* srcPtr, GLvoid* dstPtr )
{
	if (vtxCount == 0) 
		return;	// Fail safe. Vertex address may be NULL if no vertices to copy.
	memcpy(dstPtr, srcPtr, (vtxCount * getVertexStride()));
}

std::string CC3VertexArray::describeVertices()
{
	return describeVertices(_vertexCount); 
}

std::string CC3VertexArray::describeVertices( GLuint vtxCount )
{
	return describeVertices( vtxCount, 0 ); 
}

std::string CC3VertexArray::describeVertices( GLuint vtxCount, GLuint startElem )
{
	GLuint endElem = MIN(startElem + vtxCount, _vertexCount);
	std::string desc = "";
	desc += CC3String::stringWithFormat( (char*)"Content of CC3VertexArray" );
	if (_vertices) 
	{
		for (GLuint elemIdx = startElem; elemIdx < endElem; elemIdx++) 
		{
			desc += CC3String::stringWithFormat( (char*)"\n\t%d:", elemIdx );
			GLvoid* elemArray = getAddressOfElement(elemIdx);
			for (int eaIdx = 0; eaIdx < _elementSize; eaIdx++) 
			{
				switch (_elementType) {
					case GL_FLOAT:
						desc += CC3String::stringWithFormat( (char*)" %.3f,", ((GLfloat*)elemArray)[eaIdx] );
						break;
					case GL_BYTE:
						desc += CC3String::stringWithFormat( (char*)" %d,", ((GLbyte*)elemArray)[eaIdx] );
						break;
					case GL_UNSIGNED_BYTE:
						desc += CC3String::stringWithFormat( (char*)" %d,", ((GLubyte*)elemArray)[eaIdx] );
						break;
					case GL_SHORT:
						desc += CC3String::stringWithFormat( (char*)" %d,", ((GLshort*)elemArray)[eaIdx] );
						break;
					case GL_UNSIGNED_SHORT:
						desc += CC3String::stringWithFormat( (char*)" %d,", ((GLushort*)elemArray)[eaIdx] );
						break;
					case GL_FIXED:
						//desc += CC3String::stringWithFormat( " %d,", ((GLfixed*)elemArray)[eaIdx] );
						break;
					default:
						desc += CC3String::stringWithFormat( (char*)" unknown type (%u),", _elementType );
						break;
				}
			}
		}
	} else {
		desc += CC3String::stringWithFormat( (char*)" Elements are no longer in memory." );
	}
	return desc;
}


bool CC3VertexArray::initFromCPODData( PODClassPtr aCPODData, PODStructPtr aSPODMesh )
{
	CPODData* pcd = (CPODData*)aCPODData;
	SPODMesh* psm = (SPODMesh*)aSPODMesh;
	if ( super::init() )
	{
		GLint elemSize = pcd->n;
		//LogRez(@"\t%@ %@ from: %@", (elemSize ? @"Creating" : @"Skipping"), [self class], NSStringFromCPODData(pcd));
		if ( elemSize ) 
		{
			setElementType( GLElementTypeFromEPVRTDataType(pcd->eType) );
			setShouldNormalizeContent( CC3ShouldNormalizeEPVRTDataType(pcd->eType) );
			setElementSize( elemSize );
			setVertexStride( pcd->nStride );
			setVertexCount( psm->nNumVertex );
			setElementsFromCPODData( pcd, psm );
		} 
		else
		{
			return false;
		}
	}

	return true;
}

CC3VertexArray* CC3VertexArray::arrayFromCPODData( PODClassPtr aCPODData, PODStructPtr aSPODMesh )
{
	CC3VertexArray* vertexArray = new CC3VertexArray;
	if ( vertexArray->initFromCPODData( aCPODData, aSPODMesh ) )
	{
		vertexArray->autorelease();
		return vertexArray;
	}

	CC_SAFE_DELETE( vertexArray );

	return vertexArray;
}

/** Template method extracts the vertex data from the specified SPODMesh and CPODData structures.  */
void CC3VertexArray::setElementsFromCPODData( CPODData* aCPODData, SPODMesh* aSPODMesh )
{
	if (aSPODMesh->pInterleaved) {					// vertex data is interleaved
		setVertices( aSPODMesh->pInterleaved );
		setElementOffset( (GLuint)(intptr_t)aCPODData->pData );
	} else {										// not interleaved
		setVertices( aCPODData->pData );
		_allocatedVertexCapacity = _vertexCount;	// CC3VertexArray instance will free data when needed.
		aCPODData->pData = NULL;					// Clear data reference from CPODData so it won't try to free it.
		setElementOffset( 0 );
	}
}


CC3DrawableVertexArray::CC3DrawableVertexArray()
{

}

CC3DrawableVertexArray::~CC3DrawableVertexArray()
{
	deallocateStripLengths();
}

GLenum CC3DrawableVertexArray::getDrawingMode()
{
	return _drawingMode;
}

void CC3DrawableVertexArray::setDrawingMode( GLenum drawingMode )
{
	_drawingMode = drawingMode;
}

GLuint CC3DrawableVertexArray::getStripCount()
{
	return _stripCount;
}

void CC3DrawableVertexArray::setStripCount( GLuint count )
{
	_stripCount = count;
}

GLuint* CC3DrawableVertexArray::getStripLengths()
{
	return _stripLengths;
}

void CC3DrawableVertexArray::setStripLengths( GLuint* lengths )
{
	_stripLengths = lengths;
}

// Template method that populates this instance from the specified other instance.
// This method is invoked automatically during object copying via the copyWithZone: method.
void CC3DrawableVertexArray::populateFrom( CC3DrawableVertexArray* another )
{
	super::populateFrom( another );

	_drawingMode = another->getDrawingMode();

	// Allocate memory for strips, then copy them over
	allocateStripLengths( another->getStripCount() );
	GLuint* otherStripLengths = another->getStripLengths();
	for( GLuint i = 0; i < _stripCount; i++ ) 
	{
		_stripLengths[i] = otherStripLengths[i];
	}
}

CCObject* CC3DrawableVertexArray::copyWithZone( CCZone* zone )
{
	CC3DrawableVertexArray* pVal = new CC3DrawableVertexArray;
	pVal->init();
	pVal->populateFrom( this );
	
	return pVal;
}

void CC3DrawableVertexArray::drawWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	if (_stripCount) 
	{
		// MARK_TRACE_HERE
		//CCLOG("CC3DrawableVertexArray drawing %d strips", _stripCount);
		GLuint startOfStrip = 0;
		for (GLuint i = 0; i < _stripCount; i++) 
		{
			GLuint stripLen = _stripLengths[i];
			// MARK_TRACE_HERE
			//CCLOG("CC3DrawableVertexArray drawing strip %d of %d starting at %d for length %d", i, _stripCount, startOfStrip, stripLen);
			drawFrom( startOfStrip, stripLen, visitor );
			startOfStrip += stripLen;
		}
	} 
	else 
	{
		drawFrom( 0, _vertexCount, visitor );
	}
}

void CC3DrawableVertexArray::drawFrom( GLuint vertexIndex, GLuint vtxCount, CC3NodeDrawingVisitor* visitor )
{
	// MARK_TRACE_HERE
	//CCLOG("CC3DrawableVertexArray drawing %u vertices", vtxCount);
	CC3PerformanceStatistics* pStatistics = visitor->getPerformanceStatistics();
	if ( pStatistics )
		pStatistics->addSingleCallFacesPresented( getFaceCountFromVertexIndexCount( vtxCount ) );
}

void CC3DrawableVertexArray::allocateStripLengths( GLuint sCount )
{
	deallocateStripLengths();			// get rid of any existing array
	
	if (sCount) 
	{
		_stripCount = sCount;
		_stripLengths = (GLuint*)calloc(_stripCount, sizeof(GLuint));
		_stripLengthsAreRetained = true;
	}
}

void CC3DrawableVertexArray::deallocateStripLengths()
{
	if (_stripLengthsAreRetained) 
	{
		free(_stripLengths);
		_stripLengthsAreRetained = false;
	}
	_stripLengths = NULL;
	_stripCount = 0;
}

/** Converts the specified vertex count to a face count, based on the drawingMode property. */
GLuint CC3DrawableVertexArray::getFaceCountFromVertexIndexCount( GLuint vc )
{
	switch (getDrawingMode()) 
	{
		case GL_TRIANGLES:
			return vc / 3;
		case GL_TRIANGLE_STRIP:
		case GL_TRIANGLE_FAN:
			return vc - 2;
		case GL_LINES:
			return vc / 2;
		case GL_LINE_STRIP:
			return vc - 1;
		case GL_LINE_LOOP:
		case GL_POINTS:
			return vc;
		default:
			CCAssert(false, "CC3DrawableVertexArray encountered unknown drawing mode %d"/*, getDrawingMode()*/);
			return 0;
	}
}

/** Converts the specified face count to a vertex count, based on the drawingMode property. */
GLuint CC3DrawableVertexArray::getVertexIndexCountFromFaceCount( GLuint fc )
{
	switch (getDrawingMode()) 
	{
		case GL_TRIANGLES:
			return fc * 3;
		case GL_TRIANGLE_STRIP:
		case GL_TRIANGLE_FAN:
			return fc + 2;
		case GL_LINES:
			return fc * 2;
		case GL_LINE_STRIP:
			return fc + 1;
		case GL_LINE_LOOP:
		case GL_POINTS:
			return fc;
		default:
			CCAssert(false, "CC3DrawableVertexArray encountered unknown drawing mode %d"/*, getDrawingMode()*/);
			return 0;
	}
}

/**
 * If drawing is being done with strips, accumulate the number of faces per strip
 * by converting the number of vertices in each strip to faces. Otherwise, simply
 * convert the total number of vertices to faces.
 */
GLuint CC3DrawableVertexArray::getFaceCount()
{
	if (_stripCount) 
	{
		GLuint fCnt = 0;
		for (GLuint i = 0; i < _stripCount; i++) 
		{
			fCnt += getFaceCountFromVertexIndexCount(_stripLengths[i]);
		}
		return fCnt;
	}
	else 
	{
		return getFaceCountFromVertexIndexCount( _vertexCount );
	}
}

/**
 * If drawing is being done with strips, accumulate the number of faces per strip
 * prior to the strip that contains the specified face, then add the offset to
 * the face within that strip to retrieve the correct face from that strip.
 * If strips are not in use, simply extract the face from the full element array.
 */
CC3FaceIndices CC3DrawableVertexArray::getFaceIndicesAt( GLuint faceIndex )
{
	if (_stripCount) 
	{
		// Mesh is divided into strips. Find the strip that contains the face,
		// by accumulating faces and element counts until we reach the strip
		// that contains the face. Then extract the face from that strip and
		// offset it by the number of vertices in all the previous strips.
		GLuint currStripStartFaceCnt = 0;
		GLuint nextStripStartFaceCnt = 0;
		GLuint stripStartVtxCnt = 0;
		for (GLuint i = 0; i < _stripCount; i++) 
		{
			GLuint stripLen = _stripLengths[i];
			nextStripStartFaceCnt += getFaceCountFromVertexIndexCount( stripLen );
			if (nextStripStartFaceCnt > faceIndex) 
			{
				CC3FaceIndices faceIndices = getFaceIndicesAt( faceIndex - currStripStartFaceCnt, stripLen );
				// Offset the indices of the face by the number of vertices
				// accumulated from all the previous strips and return them.
				faceIndices.vertices[0] += stripStartVtxCnt;
				faceIndices.vertices[1] += stripStartVtxCnt;
				faceIndices.vertices[2] += stripStartVtxCnt;
				return faceIndices;
			}
			currStripStartFaceCnt = nextStripStartFaceCnt;
			stripStartVtxCnt += stripLen;
		}
		CCAssert(false, "CC3DrawableVertexArray requested face index %d is larger than face count %d"/*, faceIndex, getFaceCount()*/);
		return kCC3FaceIndicesZero;
	} else {
		// Mesh is monolithic. Simply extract the face from the vertices array.
		return getFaceIndicesAt( faceIndex, _vertexCount );
	}
}

/**
 * Returns the indicies for the face at the specified face index,
 * within an array of vertices of the specified length.
 */
CC3FaceIndices CC3DrawableVertexArray::getFaceIndicesAt( GLuint faceIndex, GLuint stripLen )
{
	GLuint firstVtxIdx;			// The first index of the face.
	switch (getDrawingMode()) 
	{
		case GL_TRIANGLES:
			firstVtxIdx = faceIndex * 3;
			return CC3FaceIndicesMake(firstVtxIdx, firstVtxIdx + 1, firstVtxIdx + 2);
		case GL_TRIANGLE_STRIP:
			firstVtxIdx = faceIndex;
			if (CC3IntIsEven(faceIndex)) {		// The winding order alternates
				return CC3FaceIndicesMake(firstVtxIdx, firstVtxIdx + 1, firstVtxIdx + 2);
			} else {
				return CC3FaceIndicesMake(firstVtxIdx, firstVtxIdx + 2, firstVtxIdx + 1);
			}
		case GL_TRIANGLE_FAN:
			firstVtxIdx = faceIndex + 1;
			return CC3FaceIndicesMake(0, firstVtxIdx, firstVtxIdx + 1);
		case GL_LINES:
			firstVtxIdx = faceIndex * 2;
			return CC3FaceIndicesMake(firstVtxIdx, firstVtxIdx + 1, 0);
		case GL_LINE_STRIP:
			firstVtxIdx = faceIndex;
			return CC3FaceIndicesMake(firstVtxIdx, firstVtxIdx + 1, 0);
		case GL_LINE_LOOP:
			{
				firstVtxIdx = faceIndex;
				GLuint nextVtxIdx = (faceIndex < stripLen - 1) ? firstVtxIdx + 1 : 0;
				return CC3FaceIndicesMake(firstVtxIdx, nextVtxIdx, 0);
			}
		case GL_POINTS:
			firstVtxIdx = faceIndex;
			return CC3FaceIndicesMake(firstVtxIdx, 0, 0);
		default:
			CCAssert(false, "CC3DrawableVertexArray encountered unknown drawing mode %d"/*, getDrawingMode()*/);
			return kCC3FaceIndicesZero;
	}
}

void CC3DrawableVertexArray::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	{
		_drawingMode = GL_TRIANGLES;
		_stripCount = 0;
		_stripLengths = NULL;
		_stripLengthsAreRetained = false;
	}
}

bool CC3DrawableVertexArray::initFromCPODData( PODClassPtr aCPODData, PODStructPtr aSPODMesh )
{
	SPODMesh* psm = (SPODMesh*)aSPODMesh;
	if ( super::initFromCPODData( aCPODData, aSPODMesh ) )
	{
		setDrawingMode( GLDrawingModeForSPODMesh(aSPODMesh) );

		allocateStripLengths( psm->nNumStrips );
		for (GLuint i = 0; i < psm->nNumStrips; i++)
			_stripLengths[i] = getVertexIndexCountFromFaceCount( psm->pnStripLength[i] );
	
		return true;
	}

	return false;
}

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
	_boundaryIsDirty = true;
	_radiusIsDirty = true;
}

// Mark boundary dirty, but only if vertices are valid (to avoid marking dirty on dealloc)
void CC3VertexLocations::verticesWereChanged()
{
	if (_vertices && _vertexCount) 
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

	_firstVertex = another->_firstVertex;
	_boundingBox = another->getBoundingBox();
	_centerOfGeometry = another->getCenterOfGeometry();
	_radius = another->getRadius();
	_boundaryIsDirty = another->_boundaryIsDirty;
	_radiusIsDirty = another->_radiusIsDirty;
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
	switch (_elementSize) 
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
	if ( index >= _allocatedVertexCapacity )
		return;

	GLvoid* elemAddr = getAddressOfElement(index);
	switch (_elementSize) {
		case 2:		// Just store X & Y
			*(CCPoint*)elemAddr = *(CCPoint*)&aLocation;
			break;
		case 4:		// Convert to 4D with w = 1
			*(CC3Vector4*)elemAddr = CC3Vector4FromLocation(aLocation);
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
	switch (_elementSize) {
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
	switch (_elementSize) {
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
	return CC3FaceMake(getLocationAt( faceIndices.vertices[0] ),
					   getLocationAt( faceIndices.vertices[1] ),
					   getLocationAt( faceIndices.vertices[2] ));
}

/** Returns the boundingBox, building it if necessary. */
CC3Box CC3VertexLocations::getBoundingBox()
{
	buildBoundingBoxIfNecessary();
	return _boundingBox;
}

/** Returns the centerOfGeometry, calculating it via the bounding box if necessary. */
CC3Vector CC3VertexLocations::getCenterOfGeometry()
{
	buildBoundingBoxIfNecessary();
	return _centerOfGeometry;
}

/** Builds the bounding box if it needs to be built. */
void CC3VertexLocations::buildBoundingBoxIfNecessary()
{
	if (_boundaryIsDirty) 
		buildBoundingBox(); 
}

/** Returns the radius, calculating it if necessary. */
GLfloat CC3VertexLocations::getRadius()
{
	calcRadiusIfNecessary();
	return _radius;
}

/** Calculates the radius if it necessary. */
void CC3VertexLocations::calcRadiusIfNecessary()
{
	if (_radiusIsDirty) 
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
	CCAssert( !( !_vertices && _vertexCount ), "CC3VertexLocations bounding box requested after vertex data have been released");
	CCAssert(_elementType == GL_FLOAT, "CC3VertexLocations must have elementType GLFLOAT to build the bounding box");

	CC3Vector vl, vlMin, vlMax;
	vl = (_vertexCount > 0) ?  getLocationAt(0) : CC3Vector::kCC3VectorZero;
	vlMin = vl;
	vlMax = vl;
	for (GLuint i = 1; i < _vertexCount; i++) 
	{
		vl = getLocationAt(i);
		vlMin = vlMin.minimize( vl );
		vlMax = vlMax.maxmize( vl );
	}
	_boundingBox.minimum = vlMin;
	_boundingBox.maximum = vlMax;
	_centerOfGeometry = CC3BoxCenter(_boundingBox);
	_boundaryIsDirty = false;
}

/**
 * Calculates and populates the radius property from the vertex locations.
 *
 * This method is invoked automatically when the radius property is accessed
 * for the first time after the boundary has been marked dirty.
 */
void CC3VertexLocations::calcRadius()
{
	CCAssert(_elementType == GL_FLOAT, "CC3VertexLocations must have elementType GLFLOAT to calculate mesh radius");

	CC3Vector cog = getCenterOfGeometry();		// Will measure it if necessary
	if (_vertices && _vertexCount) 
	{
		// Work with the square of the radius so that all distances can be compared
		// without having to run expensive square-root calculations.
		GLfloat radiusSq = 0.0;
		for (GLuint i=0; i < _vertexCount; i++) 
		{
			CC3Vector vl = getLocationAt( i );
			GLfloat distSq = vl.distanceSquared( cog );
			radiusSq = MAX(radiusSq, distSq);
		}

		_radius = sqrtf(radiusSq);		// Now finally take the square-root
		_radiusIsDirty = false;
	}
}

void CC3VertexLocations::moveMeshOriginTo( const CC3Vector& aLocation )
{
	for (GLuint i = 0; i < _vertexCount; i++) 
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

	visitor->getGL()->drawVerticiesAs( _drawingMode, _firstVertex + vtxIdx, vtxCount );
}

std::string CC3VertexLocations::getNameSuffix()
{
	return "Locations"; 
}

void CC3VertexLocations::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	{
		_firstVertex = 0;
		_centerOfGeometry = CC3Vector::kCC3VectorZero;
		_boundingBox = kCC3BoxZero;
		_radius = 0.0;
		markBoundaryDirty();
	}
}

GLenum CC3VertexLocations::defaultSemantic()
{
	return kCC3SemanticVertexLocation; 
}


/** CC3VertexLocations manages freeing either dedicated or interleaved data */
void CC3VertexLocations::setElementsFromCPODData( CPODData* aCPODData, SPODMesh* aSPODMesh )
{
	super::setElementsFromCPODData( aCPODData, aSPODMesh );
	_allocatedVertexCapacity = _vertexCount;	// CC3VertexArray instance will free data when needed.
}


CC3VertexLocations* CC3VertexLocations::arrayFromCPODData( PODClassPtr aCPODData, PODStructPtr aSPODMesh )
{
	CC3VertexLocations* pArray = new CC3VertexLocations;
	if ( pArray->initFromCPODData( aCPODData, aSPODMesh ) )
	{
		pArray->autorelease();
		return pArray;
	}

	CC_SAFE_DELETE(pArray);

	return pArray;
}


CC3VertexNormals* CC3VertexNormals::vertexArray()
{
	CC3VertexNormals* normals = new CC3VertexNormals;
	normals->init();
	normals->autorelease();

	return normals;
}

CC3VertexNormals* CC3VertexNormals::vertexArrayWithName( const std::string& aName )
{
	CC3VertexNormals* normals = new CC3VertexNormals;
	normals->initWithName( aName );
	normals->autorelease();

	return normals;
}

CC3Vector CC3VertexNormals::getNormalAt( GLuint index )
{
	return *(CC3Vector*)getAddressOfElement(index); 
}

void CC3VertexNormals::setNormal( const CC3Vector& aNormal, GLuint index )
{
	*(CC3Vector*)getAddressOfElement(index) = aNormal;
}

void CC3VertexNormals::flipNormals()
{
	GLuint vtxCnt = getVertexCount();
	for (GLuint vtxIdx = 0; vtxIdx < vtxCnt; vtxIdx++) 
	{
		CC3Vector* pn = (CC3Vector*)getAddressOfElement(vtxIdx);
		*pn = (*pn).negate();
	}
}

std::string CC3VertexNormals::getNameSuffix()
{
	return "Normals"; 
}

GLenum CC3VertexNormals::defaultSemantic()
{
	return kCC3SemanticVertexNormal; 
}


CC3VertexNormals* CC3VertexNormals::arrayFromCPODData( PODClassPtr aCPODData, PODStructPtr aSPODMesh )
{
	CC3VertexNormals* normals = new CC3VertexNormals;
	if ( normals->initFromCPODData( aCPODData, aSPODMesh ) )
	{
		normals->autorelease();
		return normals;
	}
	
	CC_SAFE_DELETE( normals );

	return normals;
}

CC3VertexTangents* CC3VertexTangents::vertexArray()
{
	CC3VertexTangents* tangents = new CC3VertexTangents;
	tangents->init();
	tangents->autorelease();

	return tangents;
}

CC3Vector CC3VertexTangents::getTangentAt( GLuint index )
{
	return *(CC3Vector*)getAddressOfElement(index); 
}

void CC3VertexTangents::setTangent( const CC3Vector& aTangent, GLuint index )
{
	*(CC3Vector*)getAddressOfElement(index) = aTangent;
}

std::string CC3VertexTangents::getNameSuffix()
{
	return "Tangents"; 
}

GLenum CC3VertexTangents::defaultSemantic()
{
	return kCC3SemanticVertexTangent; 
}

CC3VertexTangents* CC3VertexTangents::arrayFromCPODData( PODClassPtr aCPODData, PODStructPtr aSPODMesh )
{
	CC3VertexTangents* tangents = new CC3VertexTangents;
	if ( tangents->initFromCPODData( aCPODData, aSPODMesh ) )
	{
		tangents->autorelease();
		return tangents;
	}

	CC_SAFE_DELETE( tangents );
	
	return tangents;
}

CC3VertexColors* CC3VertexColors::vertexArray()
{
	CC3VertexColors* colors = new CC3VertexColors;
	colors->init();
	colors->autorelease();

	return colors;
}

CC3VertexColors* CC3VertexColors::vertexArrayWithName( const std::string& aName )
{
	CC3VertexColors* colors = new CC3VertexColors;
	colors->initWithName( aName );
	colors->autorelease();

	return colors;
}

void CC3VertexColors::setElementType( GLenum elementType )
{
	_elementType = elementType;
	setShouldNormalizeContent( (_elementType != GL_FLOAT) );
}

ccColor4F CC3VertexColors::getColor4FAt( GLuint index )
{
	switch (_elementType) 
	{
		case GL_FIXED:
		case GL_UNSIGNED_BYTE:
			return CCC4FFromCCC4B(*(ccColor4B*)getAddressOfElement(index));
		case GL_FLOAT:
		default:
			return *(ccColor4F*)getAddressOfElement(index);
	}
}

void CC3VertexColors::setColor4F( const ccColor4F& aColor, GLuint index )
{
	switch (_elementType) {
		case GL_FIXED:
		case GL_UNSIGNED_BYTE:
			*(ccColor4B*)getAddressOfElement(index) = CCC4BFromCCC4F(aColor);
			break;
		case GL_FLOAT:
		default:
			*(ccColor4F*)getAddressOfElement(index) = aColor;
	}
}

ccColor4B CC3VertexColors::getColor4BAt( GLuint index )
{
	switch (_elementType) {
		case GL_FLOAT:
			return CCC4BFromCCC4F(*(ccColor4F*)getAddressOfElement(index));
		case GL_FIXED:
		case GL_UNSIGNED_BYTE:
		default:
			return *(ccColor4B*)getAddressOfElement(index);
	}
}

void CC3VertexColors::setColor4B( const ccColor4B& aColor, GLuint index )
{
	switch (_elementType) {
		case GL_FLOAT:
			*(ccColor4F*)getAddressOfElement(index) = CCC4FFromCCC4B(aColor);
			break;
		case GL_FIXED:
		case GL_UNSIGNED_BYTE:
		default:
			*(ccColor4B*)getAddressOfElement(index) = aColor;
	}
}

/**
 * Since material color tracking mucks with both ambient and diffuse material colors under
 * the covers, we won't really know what the ambient and diffuse material color values will
 * be when we get back to setting them...so indicate that to the corresponding trackers.
 */
void CC3VertexColors::bindContent( GLvoid* pointer, GLint vaIdx, CC3NodeDrawingVisitor* visitor )
{
	super::bindContent( pointer, vaIdx, visitor );
	
	CC3OpenGL* gl = visitor->getGL();
	gl->isKnownMat_GL_AMBIENT = false;
	gl->isKnownMat_GL_DIFFUSE = false;
}


/** Returns the color of the first vertex. */
CCColorRef CC3VertexColors::getColor()
{
	if (getVertexCount() == 0) 
		return CCColorRefFromCCC4F(kCCC4FBlackTransparent);

	return CCColorRefFromCCC4B(getColor4BAt(0));
}

/** Sets the color of each vertex without changing the individual opacity of each vertex. */
void CC3VertexColors::setColor( const CCColorRef& color )
{
	ccColor4B c4b = CCC4BFromCCColorRef(color);
	GLuint vtxCnt = getVertexCount();
	for (GLuint vtxIdx = 0; vtxIdx < vtxCnt; vtxIdx++) 
	{
		c4b.a = getColor4BAt( vtxIdx ).a;
		setColor4B( c4b, vtxIdx );
	}
	updateGLBuffer();
}

/** Returns the opacity of the first vertex. */
CCOpacity CC3VertexColors::getOpacity()
{
	return (getVertexCount() > 0) ? CCOpacityFromGLubyte(getColor4BAt(0).a) : kCCOpacityFull; 
}

/** Sets the opacity of each vertex without changing the individual color of each vertex. */
void CC3VertexColors::setOpacity( CCOpacity opacity )
{
	GLubyte alpha = GLubyteFromCCOpacity(opacity);
	GLuint vtxCnt = getVertexCount();
	for (GLuint vtxIdx = 0; vtxIdx < vtxCnt; vtxIdx++) 
	{
		ccColor4B vtxCol = getColor4BAt(vtxIdx);
		vtxCol.a = alpha;
		setColor4B( vtxCol, vtxIdx );
	}
	updateGLBuffer();
}

std::string CC3VertexColors::getNameSuffix()
{
	return "Colors"; 
}

void CC3VertexColors::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	{
		setElementType( GL_UNSIGNED_BYTE );	// Use setter, so shouldNormalizeContent also set
		_elementSize = 4;
	}
}

GLenum CC3VertexColors::defaultSemantic()
{
	return kCC3SemanticVertexColor; 
}

// Element size must be 4 for colors. POD loader sometimes provides incorrect value!
bool CC3VertexColors::initFromCPODData( PODClassPtr aCPODData, PODStructPtr aSPODMesh )
{
	if ( super::initFromCPODData( aCPODData, aSPODMesh ) )
	{
		_elementSize = 4;
		return true;
	}

	return false;
}

CC3VertexColors* CC3VertexColors::arrayFromCPODData( PODClassPtr aCPODData, PODStructPtr aSPODMesh )
{
	CC3VertexColors* pColors = new CC3VertexColors;
	if ( pColors->initFromCPODData( aCPODData, aSPODMesh ) )
	{
		pColors->autorelease();
		return pColors;
	}

	CC_SAFE_DELETE( pColors );

	return pColors;
}

CC3VertexTextureCoordinates* CC3VertexTextureCoordinates::vertexArray()
{
	CC3VertexTextureCoordinates* vertices =new CC3VertexTextureCoordinates;
	vertices->init();
	vertices->autorelease();

	return vertices;
}

CC3VertexTextureCoordinates* CC3VertexTextureCoordinates::vertexArrayWithName( const std::string& aName )
{
	CC3VertexTextureCoordinates* vertices =new CC3VertexTextureCoordinates;
	vertices->initWithName( aName );
	vertices->autorelease();

	return vertices;
}

CCRect CC3VertexTextureCoordinates::getTextureRectangle()
{
	return _textureRectangle; 
}

CCRect CC3VertexTextureCoordinates::getEffectiveTextureRectangle()
{
	if (_expectsVerticallyFlippedTextures) 
	{
		return CCRectMake(_textureRectangle.origin.x * _mapSize.width,
						  (1.0f - _textureRectangle.origin.y) * _mapSize.height,
						  _textureRectangle.size.width * _mapSize.width,
						  -_textureRectangle.size.height * _mapSize.height);
	} else {
		return CCRectMake(_textureRectangle.origin.x * _mapSize.width,
						  _textureRectangle.origin.y * _mapSize.height,
						  _textureRectangle.size.width * _mapSize.width,
						  _textureRectangle.size.height * _mapSize.height);
	}
}

void CC3VertexTextureCoordinates::setTextureRectangle( const CCRect& aRect )
{
	CCRect oldRect = _textureRectangle;
	_textureRectangle = aRect;
	alignWithTextureRectangle( aRect, oldRect );
}
	
// Template method that populates this instance from the specified other instance.
// This method is invoked automatically during object copying via the copyWithZone: method.
void CC3VertexTextureCoordinates::populateFrom( CC3VertexTextureCoordinates* another )
{
	super::populateFrom(another);
	
	_mapSize = another->_mapSize;
	_textureRectangle = another->_textureRectangle;
	_expectsVerticallyFlippedTextures = another->expectsVerticallyFlippedTextures();
}

CCObject* CC3VertexTextureCoordinates::copyWithZone( CCZone* zone )
{
	CC3VertexTextureCoordinates* pVal = new CC3VertexTextureCoordinates;
	pVal->init();
	pVal->populateFrom( this );
	
	return pVal;
}

bool CC3VertexTextureCoordinates::expectsVerticallyFlippedTextures()
{
	return _expectsVerticallyFlippedTextures;
}

void CC3VertexTextureCoordinates::setExpectsVerticallyFlippedTextures( bool expects )
{
	_expectsVerticallyFlippedTextures = expects;
}

static bool _defaultExpectsVerticallyFlippedTextures = false;

bool CC3VertexTextureCoordinates::defaultExpectsVerticallyFlippedTextures()
{
	return _defaultExpectsVerticallyFlippedTextures;
}

void CC3VertexTextureCoordinates::setDefaultExpectsVerticallyFlippedTextures( bool expectsFlipped )
{
	_defaultExpectsVerticallyFlippedTextures = expectsFlipped;
}

ccTex2F CC3VertexTextureCoordinates::getTexCoord2FAt( GLuint index )
{
	return *(ccTex2F*)getAddressOfElement(index); 
}

void CC3VertexTextureCoordinates::setTexCoord2F( const ccTex2F& aTex2F, GLuint index )
{
	*(ccTex2F*)getAddressOfElement(index) = aTex2F;
}

/**
 * Aligns the vertex texture coordinates with the area of the texture defined
 * by the newRect. The oldRect describes the area of the texture that is currently
 * mapped by the texture coordinates.
 */
void CC3VertexTextureCoordinates::alignWithTextureRectangle( const CCRect& newRect, const CCRect& oldRect )
{
	// The size of the texture mapping in its natural state
	GLfloat mw = _mapSize.width;
	GLfloat mh = _mapSize.height;
	
	// Old rect components
	GLfloat ox = oldRect.origin.x;
	GLfloat oy = oldRect.origin.y;
	GLfloat ow = oldRect.size.width;
	GLfloat oh = oldRect.size.height;
	
	// New rect components
	GLfloat nx = newRect.origin.x;
	GLfloat ny = newRect.origin.y;
	GLfloat nw = newRect.size.width;
	GLfloat nh = newRect.size.height;
	
	GLfloat hx = 1.0f - _mapSize.height;	// Height translation due to texture inversion
	
	// For each texture coordinate, convert to the original coordinate, taking into consideration
	// the mapSize and the old texture rectangle. Then, convert to the new coordinate, taking into
	// consideration the mapSize and the new texture rectangle.
	for (GLuint i = 0; i < _vertexCount; i++) 
	{
		ccTex2F* ptc = (ccTex2F*)getAddressOfElement(i);
		
		GLfloat origU = ((ptc->u / mw) - ox) / ow;			// Revert to original value
		ptc->u = (nx + (origU * nw)) * mw;					// Calc new value
		
		// Take into consideration whether the texture is flipped.
		if (_expectsVerticallyFlippedTextures) {
			GLfloat origV = (1.0f - (ptc->v / mh) - oy) / oh;	// Revert to original value
			ptc->v = (1.0f - (ny + (origV * nh))) * mh;			// Calc new value
		} else {
			GLfloat origV = (((ptc->v - hx) / mh) - oy) / oh;	// Revert to original value
			ptc->v = (ny + (origV * nh)) * mh + hx;				// Calc new value
		}
	}
	updateGLBuffer();
}

void CC3VertexTextureCoordinates::alignWithTextureCoverage( const CCSize& texCoverage )
{
	CCAssert((texCoverage.width && texCoverage.height), "CC3VertexTextureCoordinates mapsize cannot have zero dimension");

	// Don't waste time adjusting if nothing is changing
	// (eg. POT textures, or new texture has same texture map as old).
	if (texCoverage.equals(_mapSize)) 
		return;
	
	CC3_TRACE( "[vtx]CC3VertexTextureCoordinates aligning and changing map size from %s to %s but not flipping vertically", 
		stringFromCCSize(_mapSize).c_str(), stringFromCCSize(texCoverage).c_str() );

	// The scale factor
	CCSize mapRatio = CCSizeMake(texCoverage.width / _mapSize.width, texCoverage.height / _mapSize.height);
	
	// The amount by which to translate the image vertically
	GLfloat currVertXln = 1.0f - _mapSize.height;
	GLfloat newVertXln = 1.0f - texCoverage.height;
	
	for (GLuint i = 0; i < _vertexCount; i++) 
	{
		ccTex2F* ptc = (ccTex2F*)getAddressOfElement(i);
		ptc->u *= mapRatio.width;
		ptc->v = (ptc->v - currVertXln) * mapRatio.height + newVertXln;
	}
	_mapSize = texCoverage;	// Remember what we've set the map size to

	updateGLBuffer();
}

void CC3VertexTextureCoordinates::alignWithInvertedTextureCoverage( const CCSize& texCoverage )
{
	CCAssert((texCoverage.width && texCoverage.height), "CC3VertexTextureCoordinates mapsize %s cannot have zero dimension"/*,
			  stringFromCCSize(texCoverage).c_str()*/);
	CC3_TRACE( "[vtx]CC3VertexTextureCoordinates aligning and changing map size from %s to %s and flipping vertically", 
		stringFromCCSize(_mapSize).c_str(), stringFromCCSize(texCoverage).c_str() );
	
	CCSize mapRatio = CCSizeMake(texCoverage.width / _mapSize.width, texCoverage.height / _mapSize.height);
	
	for (GLuint i = 0; i < _vertexCount; i++) 
	{
		ccTex2F* ptc = (ccTex2F*)getAddressOfElement( i );
		ptc->u *= mapRatio.width;
		ptc->v = texCoverage.height - (ptc->v * mapRatio.height);
	}

	// Remember that we've flipped and what we've set the map size to
	_mapSize = texCoverage;
	_expectsVerticallyFlippedTextures = !_expectsVerticallyFlippedTextures;
	
	updateGLBuffer();
	CC3_TRACE("[vtx]CC3VertexTextureCoordinates aligned and flipped vertically");
}

void CC3VertexTextureCoordinates::alignWithTexture( CC3Texture* texture )
{
	if (!texture) 
		return;
	if ( XOR(_expectsVerticallyFlippedTextures, texture->isUpsideDown()) ) 
	{
		alignWithInvertedTextureCoverage( texture->getCoverage() );
	} else {
		alignWithTextureCoverage( texture->getCoverage() );
	}
}

void CC3VertexTextureCoordinates::alignWithInvertedTexture( CC3Texture* texture )
{
	if (!texture) 
		return;

	alignWithInvertedTextureCoverage( texture->getCoverage() );
}

void CC3VertexTextureCoordinates::flipVertically()
{
	GLfloat minV = kCC3MaxGLfloat;
	GLfloat maxV = -kCC3MaxGLfloat;
	for (GLuint i = 0; i < _vertexCount; i++) 
	{
		ccTex2F* ptc = (ccTex2F*)getAddressOfElement(i);
		minV = MIN(ptc->v, minV);
		maxV = MAX(ptc->v, maxV);
	}
	for (GLuint i = 0; i < _vertexCount; i++) 
	{
		ccTex2F* ptc = (ccTex2F*)getAddressOfElement(i);
		ptc->v = minV + maxV - ptc->v;
	}
	updateGLBuffer();
}

void CC3VertexTextureCoordinates::flipHorizontally()
{
	GLfloat minU = kCC3MaxGLfloat;
	GLfloat maxU = -kCC3MaxGLfloat;
	for (GLuint i = 0; i < _vertexCount; i++) 
	{
		ccTex2F* ptc = (ccTex2F*)getAddressOfElement(i);
		minU = MIN(ptc->u, minU);
		maxU = MAX(ptc->u, maxU);
	}
	for (GLuint i = 0; i < _vertexCount; i++) 
	{
		ccTex2F* ptc = (ccTex2F*)getAddressOfElement(i);
		ptc->u = minU + maxU - ptc->u;
	}

	updateGLBuffer();
}

void CC3VertexTextureCoordinates::repeatTexture( const ccTex2F& repeatFactor )
{
	CCSize repeatSize = CCSizeMake(repeatFactor.u * _mapSize.width, repeatFactor.v * _mapSize.height);
	alignWithTextureCoverage( repeatSize );
}

std::string CC3VertexTextureCoordinates::getNameSuffix()
{
	return "TexCoords"; 
}

void CC3VertexTextureCoordinates::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	{
		_elementType = GL_FLOAT;
		_elementSize = 2;
		_mapSize = CCSizeMake(1, 1);
		_textureRectangle = kCC3UnitTextureRectangle;
		_expectsVerticallyFlippedTextures = defaultExpectsVerticallyFlippedTextures();
	}
}

GLenum CC3VertexTextureCoordinates::defaultSemantic()
{
	return kCC3SemanticVertexTexture; 
}

void CC3VertexTextureCoordinates::initFromSPODMesh( PODStructPtr aSPODMesh, GLuint texUnit )
{
	SPODMesh* psm = (SPODMesh*)aSPODMesh;
	if (texUnit < psm->nNumUVW)
	{
		initFromCPODData( &psm->psUVW[texUnit], aSPODMesh );
	}
}

CC3VertexTextureCoordinates* CC3VertexTextureCoordinates::arrayFromSPODMesh( PODStructPtr aSPODMesh, GLuint texUnit )
{
	CC3VertexTextureCoordinates* pTextureCoordinates = new CC3VertexTextureCoordinates;
	pTextureCoordinates->initFromSPODMesh( aSPODMesh, texUnit );
	pTextureCoordinates->autorelease();

	return pTextureCoordinates;
}


CC3VertexIndices* CC3VertexIndices::vertexArray()
{
	CC3VertexIndices* indices = new CC3VertexIndices;
	indices->init();
	indices->autorelease();

	return indices;
}

CC3VertexIndices* CC3VertexIndices::vertexArrayWithName( const std::string& aName )
{
	CC3VertexIndices* indices = new CC3VertexIndices;
	indices->initWithName( aName );
	indices->autorelease();

	return indices;
}

GLenum CC3VertexIndices::getBufferTarget()
{
	return GL_ELEMENT_ARRAY_BUFFER; 
}

GLuint CC3VertexIndices::getIndexAt( GLuint index )
{
	GLvoid* ptr = getAddressOfElement(index);
	return _elementType == GL_UNSIGNED_BYTE ? *(GLubyte*)ptr : *(GLushort*)ptr;
}

void CC3VertexIndices::setIndex( GLuint vtxIdx, GLuint index )
{
	GLvoid* ptr = getAddressOfElement(index);
	if (_elementType == GL_UNSIGNED_BYTE) 
	{
		*(GLubyte*)ptr = vtxIdx;
	} else {
		*(GLushort*)ptr = vtxIdx;
	}
}

CC3FaceIndices CC3VertexIndices::getFaceIndicesAt( GLuint faceIndex )
{
	CC3FaceIndices idxIndices = super::getFaceIndicesAt(faceIndex);
	return CC3FaceIndicesMake(getIndexAt(idxIndices.vertices[0]),
							  getIndexAt(idxIndices.vertices[1]),
							  getIndexAt(idxIndices.vertices[2]));
}

/** Vertex indices are not part of vertex content. */
void CC3VertexIndices::bindContent( GLvoid* pointer, GLint vaIdx, CC3NodeDrawingVisitor* visitor )
{

}


void CC3VertexIndices::drawFrom( GLuint vtxIdx, GLuint vtxCount, CC3NodeDrawingVisitor* visitor )
{
	super::drawFrom( vtxIdx, vtxCount, visitor );

	GLbyte* firstVtx = _bufferID ? 0 : (GLbyte*)_vertices;
	firstVtx += getVertexStride() * vtxIdx;
	firstVtx += _elementOffset;
	
	visitor->getGL()->drawIndicies( firstVtx, vtxCount, _elementType, _drawingMode );
}

void CC3VertexIndices::copyVertices( GLuint vtxCount, GLuint srcIdx, GLuint dstIdx, GLint offset )
{
	GLvoid* srcPtr = getAddressOfElement(srcIdx);
	GLvoid* dstPtr = getAddressOfElement(dstIdx);
	copyVertices( vtxCount, srcPtr, dstPtr, offset );
}

void CC3VertexIndices::copyVertices( GLuint vtxCount, GLuint srcIdx, GLvoid* dstPtr, GLint offset )
{
	GLvoid* srcPtr = getAddressOfElement(srcIdx);
	copyVertices( vtxCount, srcPtr, dstPtr, offset );
}

void CC3VertexIndices::copyVertices( GLuint vtxCount, GLvoid* srcPtr, GLuint dstIdx, GLint offset )
{
	GLvoid* dstPtr = getAddressOfElement(dstIdx);
	copyVertices( vtxCount, srcPtr, dstPtr, offset );
}

void CC3VertexIndices::copyVertices( GLuint vtxCount, GLvoid* srcPtr, GLvoid* dstPtr, GLint offset )
{
	if (_elementType == GL_UNSIGNED_BYTE) 
	{
		GLubyte* srcByte = (GLubyte*)srcPtr;
		GLubyte* dstByte = (GLubyte*)dstPtr;
		for (GLuint vtxIdx = 0; vtxIdx < vtxCount; vtxIdx++) 
		{
			dstByte[vtxIdx] = srcByte[vtxIdx] + offset;
		}
	} 
	else 
	{
		GLushort* srcShort = (GLushort*)srcPtr;
		GLushort* dstShort = (GLushort*)dstPtr;
		for (GLuint vtxIdx = 0; vtxIdx < vtxCount; vtxIdx++) 
		{
			dstShort[vtxIdx] = srcShort[vtxIdx] + offset;
		}
	}
}

std::string CC3VertexIndices::getNameSuffix()
{
	return "Indices"; 
}

void CC3VertexIndices::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	{
		_elementType = GL_UNSIGNED_SHORT;
		_elementSize = 1;
	}
}

GLenum CC3VertexIndices::defaultSemantic()
{
	return kCC3SemanticNone; 
}

void CC3VertexIndices::populateFromRunLengthArray( GLushort* runLenArray, GLuint rlaLen )
{
	GLuint elemNum, rlaIdx, runNum;
	
	// Iterate through the runs in the array to count
	// the number of runs and total number of vertices
	runNum = 0;
	elemNum = 0;
	rlaIdx = 0;
	while(rlaIdx < rlaLen) {
		GLushort runLength = runLenArray[rlaIdx];
		elemNum += runLength;
		rlaIdx += runLength + 1;
		runNum++;
	}
	
	// Allocate space for the vertices and the runs
	setAllocatedVertexCapacity( elemNum );
	allocateStripLengths( runNum );
	
	// Iterate through the runs in the array, copying the
	// vertices and run-lengths to this vertex index array
	runNum = 0;
	elemNum = 0;
	rlaIdx = 0;
	while(rlaIdx < rlaLen) 
	{
		GLushort runLength = runLenArray[rlaIdx++];
		_stripLengths[runNum++] = runLength;
		for (int i = 0; i < runLength; i++) 
		{
			setIndex( runLenArray[rlaIdx++], elemNum++ );
		}
	}
}

/** Calc vertexCount after drawingMode has been set. */
bool CC3VertexIndices::initFromCPODData( PODClassPtr aCPODData, PODStructPtr aSPODMesh )
{
	if ( super::initFromCPODData( aCPODData, aSPODMesh ) )
	{
		setVertexCount( getVertexIndexCountFromFaceCount( ((SPODMesh*)aSPODMesh)->nNumFaces ) );
		_allocatedVertexCapacity = _vertexCount;	// CC3VertexArray instance will free data when needed.

		return true;
	}

	return false;
}

void CC3VertexIndices::setElementsFromCPODData( CPODData* aCPODData, SPODMesh* aSPODMesh )
{
	setVertices( aCPODData->pData );
	aCPODData->pData = NULL;			// Clear data reference from CPODData so it won't try to free it.
	setElementOffset( 0 );				// Indices are not interleaved.
}

CC3VertexIndices* CC3VertexIndices::arrayFromCPODData( PODClassPtr aCPODData, PODStructPtr aSPODMesh )
{
	CC3VertexIndices* pIndices = new CC3VertexIndices;
	if ( pIndices->initFromCPODData( aCPODData, aSPODMesh ) )
	{
		pIndices->autorelease();
		return pIndices;
	}
	
	CC_SAFE_DELETE( pIndices );

	return pIndices;
}

CC3VertexPointSizes* CC3VertexPointSizes::vertexArray()
{
	CC3VertexPointSizes* sizes = new CC3VertexPointSizes;
	sizes->init();
	sizes->autorelease();

	return sizes;
}

GLfloat CC3VertexPointSizes::getPointSizeAt( GLuint index )
{
	return *(GLfloat*)getAddressOfElement(index); 
}

void CC3VertexPointSizes::setPointSize( GLfloat aSize, GLuint index )
{
	*(GLfloat*)getAddressOfElement(index) = aSize;
}

std::string CC3VertexPointSizes::getNameSuffix()
{
	return "PointSizes"; 
}

void CC3VertexPointSizes::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	{
		_elementType = GL_FLOAT;
		_elementSize = 1;
	}
}

GLenum CC3VertexPointSizes::defaultSemantic()
{
	return kCC3SemanticVertexPointSize; 
}

CC3VertexBoneWeights* CC3VertexBoneWeights::vertexArray()
{
	CC3VertexBoneWeights* weights = new CC3VertexBoneWeights;
	weights->init();
	weights->autorelease();

	return weights;
}

GLfloat CC3VertexBoneWeights::getWeightForBoneInfluence( GLuint influenceIndex, GLuint vtxIndex )
{
	return getBoneWeightsAt(vtxIndex)[influenceIndex];
}

void CC3VertexBoneWeights::setWeight( GLfloat weight, GLuint influenceIndex, GLuint vtxIndex )
{
	getBoneWeightsAt(vtxIndex)[influenceIndex] = weight;
}

GLfloat* CC3VertexBoneWeights::getBoneWeightsAt( GLuint vtxIndex )
{
	return (GLfloat*)getAddressOfElement(vtxIndex); 
}

void CC3VertexBoneWeights::setBoneWeights( GLfloat* weights, GLuint vtxIndex )
{
	GLfloat* vtxWeights = getBoneWeightsAt(vtxIndex);
	GLint numWts = getElementSize();
	for (int i = 0; i < numWts; i++) 
		vtxWeights[i] = weights[i];
}

std::string CC3VertexBoneWeights::getNameSuffix()
{
	return "Weights"; 
}

void CC3VertexBoneWeights::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	{
		_elementType = GL_FLOAT;
		_elementSize = 0;
	}
}

GLenum CC3VertexBoneWeights::defaultSemantic()
{
	return kCC3SemanticVertexBoneWeights; 
}

CC3VertexBoneIndices* CC3VertexBoneIndices::vertexArray()
{
	CC3VertexBoneIndices* indices = new CC3VertexBoneIndices;
	indices->init();
	indices->autorelease();

	return indices;
}

CC3VertexBoneWeights* CC3VertexBoneWeights::arrayFromCPODData( PODClassPtr aCPODData, PODStructPtr aSPODMesh )
{
	CC3VertexBoneWeights* pWeights = new CC3VertexBoneWeights;
	if ( pWeights->initFromCPODData( aCPODData, aSPODMesh ) )
	{
		pWeights->autorelease();
		return pWeights;
	}

	CC_SAFE_DELETE( pWeights );
	
	return pWeights;
}

GLuint CC3VertexBoneIndices::getBoneIndexForBoneInfluence( GLuint influenceIndex, GLuint vtxIndex )
{
	if (_elementType == GL_UNSIGNED_BYTE) 
	{
		GLubyte* boneIndices = (GLubyte*)getAddressOfElement(vtxIndex);
		return boneIndices[influenceIndex];
	} 
	else
	{
		GLushort* boneIndices = (GLushort*)getAddressOfElement(vtxIndex);
		return boneIndices[influenceIndex];
	}
}

void CC3VertexBoneIndices::setBoneIndex( GLuint boneIndex, GLuint influenceIndex, GLuint vtxIndex )
{
	if (_elementType == GL_UNSIGNED_BYTE) 
	{
		GLubyte* boneIndices = (GLubyte*)getAddressOfElement(vtxIndex);
		boneIndices[influenceIndex] = boneIndex;
	} else {
		GLushort* boneIndices = (GLushort*)getAddressOfElement(vtxIndex);
		boneIndices[influenceIndex] = boneIndex;
	}
}

GLvoid* CC3VertexBoneIndices::getBoneIndicesAt( GLuint vtxIndex )
{
	return getAddressOfElement(vtxIndex); 
}

void CC3VertexBoneIndices::setBoneIndices( GLvoid* boneIndices, GLuint vtxIndex )
{
	GLint numMtx = getElementSize();
	if (_elementType == GL_UNSIGNED_BYTE) 
	{
		GLubyte* vtxBoneIndices = (GLubyte*)getAddressOfElement(vtxIndex);
		for (int i = 0; i < numMtx; i++) vtxBoneIndices[i] = ((GLubyte*)boneIndices)[i];
	} else {
		GLushort* vtxBoneIndices = (GLushort*)getAddressOfElement(vtxIndex);
		for (int i = 0; i < numMtx; i++) vtxBoneIndices[i] = ((GLushort*)boneIndices)[i];
	}
}

std::string CC3VertexBoneIndices::getNameSuffix()
{
	return "BoneIndices"; 
}

void CC3VertexBoneIndices::initWithTag( GLuint aTag, const char* aName )
{
	super::initWithTag( aTag, aName );
	{
		_elementType = GL_UNSIGNED_BYTE;
		_elementSize = 0;
	}
}

GLenum CC3VertexBoneIndices::defaultSemantic()
{
	return kCC3SemanticVertexBoneIndices; 
}

CC3VertexBoneIndices* CC3VertexBoneIndices::arrayFromCPODData( PODClassPtr aCPODData, PODStructPtr aSPODMesh )
{
	CC3VertexBoneIndices* pIndices = new CC3VertexBoneIndices;
	if ( pIndices->initFromCPODData( aCPODData, aSPODMesh ) )
	{
		pIndices->autorelease();
		return pIndices;
	}
	
	CC_SAFE_DELETE( pIndices );

	return pIndices;
}

NS_COCOS3D_END
