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
	
	if ( sCount ) 
	{
		_stripCount = sCount;
		_stripLengths = (GLuint*)calloc(_stripCount, sizeof(GLuint));
		_stripLengthsAreRetained = true;
	}
}

void CC3DrawableVertexArray::deallocateStripLengths()
{
	if ( _stripLengthsAreRetained ) 
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
	switch ( _drawingMode ) 
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

NS_COCOS3D_END
