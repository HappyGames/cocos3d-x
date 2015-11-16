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
#ifndef _CC3_VERTEX_INDICES_H_
#define _CC3_VERTEX_INDICES_H_

NS_COCOS3D_BEGIN

/**
 * A CC3VertexArray that manages the drawing indices of an array of vertices.
 *
 * This class is also a type of CC3DrawableVertexArray, and as such,
 * is capable of drawing the vertices to the GL engine.
 *
 * A vertex index array is different than other vertex arrays in that instead of managing
 * actual vertex content, it manages indexes that reference the vertices of the other vertex
 * arrays. The bufferTarget property is GL_ELEMENT_ARRAY_BUFFER, the elementSize
 * property is 1, and the elementType is either GL_UNSIGNED_SHORT or GL_UNSIGNED_BYTE
 *
 * Because an index datum does not describe an aspect of a particular vertex, but rather
 * points to a vertex, index content cannot be interleaved with the vertex content. As such,
 * the content underlying a CC3VertexIndices is never interleaved and shared with the content
 * underlying the other vertex arrays in a mesh.
 */
class CC3VertexIndices : public CC3DrawableVertexArray
{
	DECLARE_SUPER( CC3DrawableVertexArray );
public:
	static CC3VertexIndices*	vertexArray();
	static CC3VertexIndices*	vertexArrayWithName( const std::string& aName );
	/**
	 * Returns the index element at the specified index in the underlying vertex content.
	 *
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * the vertexStride and elementOffset properties to access the correct element.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	GLuint						getIndexAt( GLuint index );

	/**
	 * Sets the index element at the specified index in the underlying vertex content, to
	 * the specified value.
	 * 
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * the vertexStride and elementOffset properties to access the correct element.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	void						setIndex( GLuint vertexIndex, GLuint index );

	/**
	 * Returns the vertex indices of the face from the mesh at the specified index.
	 *
	 * The specified faceIndex value refers to the index of the face, not the vertices
	 * themselves. So, a value of 5 will retrieve the three vertices that make up the
	 * fifth triangular face in this mesh. The specified index must be between zero,
	 * inclusive, and the value of the faceCount property, exclusive.
	 *
	 * The returned structure reference contains the indices of the three vertices that
	 * make up the triangular face. These indices index into the actual vertex locations
	 * in the CC3VertexLocations array.
	 *
	 * This method takes into consideration the drawingMode of this vertex array,
	 * and any padding (stride) between the vertex indices.
	 *
	 * The indices in the returned face are of type GLuint, regardless of whether the
	 * elementType property is GL_UNSIGNED_SHORT or GL_UNSIGNED_BYTE.
	 */
	CC3FaceIndices				getFaceIndicesAt( GLuint faceIndex );

	/**
	 * Convenience method to populate this index array from the specified run-length
	 * encoded array.
	 *
	 * Run-length encoded arrays are used to compactly store a set of variable-length
	 * sub-arrays of indexes, where the first element of each sub-array indicates the
	 * number of content elements contained in that sub-array.
	 *
	 * For example, if the first element of the array (element zero) contains the value 5,
	 * then the next 5 elements of the array contain the first 5 content elements of the first
	 * sub-array. Then the next element of the array (element 6) contains the length of the
	 * second sub-array, and so on.
	 *
	 * The total number of elements in the run-length array, including the run-length entries
	 * is specified by the rlaLen parameter.
	 *
	 * Run-length encoded arrays are of limited use as GL index arrays, because they cannot
	 * easily be copied into, and managed as a VBO in the GL engine, which is a performance
	 * hinderance. And becuase run-length encoded arrays intermix vertex indices and run
	 * lengths, it makes accessing individual vertex indices and faces unwieldy.
	 */
	void						populateFromRunLengthArray( GLushort* runLenArray, GLuint rlaLen );

	/**
	 * Copies vertex indices for the specified number of vertices from memory starting at the specified
	 * source vertex index to memory starting at the specified destination vertex index, and offsets
	 * each value by the specified offset amount. The value at the destination vertex will be that of
	 * the source vertex, plus the specified offset.
	 *
	 * You can use this method to copy content from one area in the vertex array to another area, while
	 * adjusting for movement of the underlying vertex content pointed to by these vertex indices.
	 */
	void						copyVertices( GLuint vtxCount, GLuint srcIdx, GLuint dstIdx, GLint offset );

	/**
	 * Copies vertex indices for the specified number of vertices from memory starting at the specified
	 * source vertex index to memory starting at the specified destination address pointer, and offsets
	 * each value by the specified offset amount. The value at the destination vertex will be that of
	 * the source vertex, plus the specified offset.
	 *
	 * You can use this method to copy content out of this vertex array to a memory location outside
	 * this vertex array, while adjusting for movement of the underlying vertex content pointed to by
	 * these vertex indices.
	 *
	 * This is a fast copy that assumes that the vertex content at the destination is of the same type
	 * (GL_UNSIGNED_BYTE or GL_UNSIGNED_SHORT) as the vertex content in this vertex array.
	 */
	void						copyVertices( GLuint vtxCount, GLuint srcIdx, GLvoid* dstPtr, GLint offset );

	/**
	 * Copies vertex indices for the specified number of vertices from memory starting at the specified
	 * source address pointer to memory starting at the specified destination vertex index, and offsets
	 * each value by the specified offset amount. The value at the destination vertex will be that of
	 * the source vertex, plus the specified offset.
	 *
	 * You can use this method to copy content into this vertex array from a memory location outside
	 * this vertex array, while adjusting for movement of the underlying vertex content pointed to by
	 * these vertex indices.
	 *
	 * This is a fast copy that assumes that the vertex content at the source is of the same type
	 * (GL_UNSIGNED_BYTE or GL_UNSIGNED_SHORT) as the vertex content in this vertex array.
	 */
	void						copyVertices( GLuint vtxCount, GLvoid* srcPtr, GLuint dstIdx, GLint offset );

	/**
	 * Copies vertex content for the specified number of vertices from memory starting at the
	 * specified source address to memory starting at the specified destination address.
	 * 
	 * You can use this method to copy content between two memory location outside this vertex array.
	 * 
	 * This is a fast straight memory copy, assumes that vertex content is consecutive and is spaced as
	 * defined by the vertexStride property, and that the vertex content at both the source and destination
	 * addresses are structured identically to the content of this vertex array. If vertex content is
	 * interleaved, the content in between consecutive elements of this vertex array will also be copied.
	 */
	/**
	 * Copies vertex indices for the specified number of vertices from memory starting at the specified
	 * source address pointer to memory starting at the specified destination address pointer, and offsets
	 * each value by the specified offset amount. The value at the destination vertex will be that of the
	 * source vertex, plus the specified offset.
	 *
	 * You can use this method to copy content between two memory location outside this vertex array,
	 * while adjusting for movement of the underlying vertex content pointed to by these vertex indices.
	 *
	 * This is a fast copy that assumes that the vertex content at the source and destination is of the
	 * same type (GL_UNSIGNED_BYTE or GL_UNSIGNED_SHORT) as the vertex content in this vertex array.
	 */
	void						copyVertices( GLuint vtxCount, GLvoid* srcPtr, GLvoid* dstPtr, GLint offset );

	GLenum						getBufferTarget();
	/** Vertex indices are not part of vertex content. */
	void						bindContent( GLvoid* pointer, GLint vaIdx, CC3NodeDrawingVisitor* visitor );
	void						drawFrom( GLuint vtxIdx, GLuint vtxCount, CC3NodeDrawingVisitor* visitor );

	std::string					getNameSuffix();
	void						initWithTag( GLuint aTag, const std::string& aName );
	GLenum						defaultSemantic();
};

NS_COCOS3D_END

#endif
