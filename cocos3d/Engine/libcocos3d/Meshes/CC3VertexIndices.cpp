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
	if ( _elementType == GL_UNSIGNED_BYTE )
		return *(GLubyte*)ptr;

	return *(GLushort*)ptr;
}

void CC3VertexIndices::setIndex( GLuint vtxIdx, GLuint index )
{
	GLvoid* ptr = getAddressOfElement(index);
	if (_elementType == GL_UNSIGNED_BYTE) 
		*(GLubyte*)ptr = vtxIdx;
	else
		*(GLushort*)ptr = vtxIdx;
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
	
	while( rlaIdx < rlaLen ) 
	{
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
	while( rlaIdx < rlaLen ) 
	{
		GLushort runLength = runLenArray[rlaIdx++];
		_stripLengths[runNum++] = runLength;
		for (int i = 0; i < runLength; i++) 
		{
			setIndex( runLenArray[rlaIdx++], elemNum++ );
		}
	}
}

NS_COCOS3D_END
