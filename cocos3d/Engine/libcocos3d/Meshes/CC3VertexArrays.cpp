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
    _vertices = NULL;
    _vertexCount = 0;
    _allocatedVertexCapacity = 0;
    _vertexStride = 0;
    _bufferID = 0;
    _bufferUsage = GL_STATIC_DRAW;
    _shouldAllowVertexBuffering = true;
    _shouldReleaseRedundantContent = true;
    _wasVertexCapacityChanged = false;
}

void CC3VertexArrayContent::init()
{
		
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

void CC3VertexArray::setVertexCapacityWithoutAllocation( GLuint capacity )
{
    _allocatedVertexCapacity = capacity;
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

NS_COCOS3D_END
