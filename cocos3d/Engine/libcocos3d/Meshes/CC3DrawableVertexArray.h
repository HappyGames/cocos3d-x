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
#ifndef _CC3_DRAWABLE_VERTEX_ARRAY_H_
#define _CC3_DRAWABLE_VERTEX_ARRAY_H_

NS_COCOS3D_BEGIN

/**
 * This abstract subclass of  CC3VertexArray adds the functionality to draw the vertex
 * content to the display through the GL engine.
 *
 * The underlying content is drawn by invoking the drawWithVisitor: method, and can be drawn
 * in a single GL call for all vertices, or the vertices can be arranged in strips, and
 * the strips drawn serially.
 *
 * You define vertex strips using the stripCount and stripLengths properties, or using
 * the allocateStripLengths: method to set both properties at once.
 * 
 * Using vertex strips performs more GL calls, and will be less efficient, but in some
 * applications, might assist in the organization of mesh vertex content.
 *
 * Alternately, a subset of the vertices may be drawn by invoking the
 * drawFrom:forCount:withVisitor: method instead of the drawWithVisitor: method.
 */
class CC3DrawableVertexArray : public CC3VertexArray 
{
	DECLARE_SUPER( CC3VertexArray );
public:
	CC3DrawableVertexArray();
	~CC3DrawableVertexArray();
	/**
	 * The drawing mode indicating how the vertices are connected (points, lines, triangles...).
	 * This must be set with a valid GL drawing mode enumeration.
	 *
	 * The default value is GL_TRIANGLES.
	 */
	GLenum						getDrawingMode();
	void						setDrawingMode( GLenum drawingMode );

	/**
	 * The underlying content can be drawn in strips, using multiple GL calls, rather than
	 * a single call. This property indicates the number of strips to draw. A value of
	 * zero indicates that vertex drawing should be done in a single GL call.
	 */
	GLuint						getStripCount();
	void						setStripCount( GLuint count );

	/**
	 * An array of values, each indicating the number of vertices to draw in the corresponding
	 * strip. The stripCount property indicates the number of items in this array. 
	 * If drawing is not performed in strips (stripCount is zero), this property will be NULL.
	 *
	 * An easy way to create a suitable array for this property, and set the associated
	 * stripCount property at the same time, is to invoke the allocateStripLengths: method.
	 */
	GLuint*						getStripLengths();
	void						setStripLengths( GLuint* lengths );


	/**
	 * Draws the vertices, either in strips, or in a single call, depending on the value
	 * of the stripCount property.
	 *
	 * This method is invoked automatically from the draw method of CC3Mesh.
	 */
	virtual void				drawWithVisitor( CC3NodeDrawingVisitor* visitor );

	/**
	 * Draws the specified number of vertices, starting at the specified vertex index,
	 * in a single GL draw call.
	 *
	 * This method can be used to draw a subset of thevertices. This can be used when this array
	 * holds content for a number of meshes, or when content is being sectioned for palette matrices.
	 *
	 * This abstract implementation collects drawing performance statistics if the visitor
	 * is configured to do so. Subclasses will override to perform appropriate drawing
	 * activity, but should also invoke this superclass implementation to perform the
	 * collection of performance content.
	 */
	virtual void				drawFrom( GLuint vertexIndex, GLuint vertexCount, CC3NodeDrawingVisitor* visitor );

	/**
	 * Sets the specified number of strips into the stripCount property, then allocates an
	 * array of Gluints of that length, and sets that array in the stripLengths property.
	 *
	 * It is safe to invoke this method more than once. The previously allocated
	 * array of strip-lengths will be deallocated before the new array is created.
	 *
	 * The array can be deallocated by invoking the deallocateStripLengths method.
	 */
	virtual void				allocateStripLengths( GLuint sCount );

	/**
	 * Deallocates the array of strip-lengths that was created by a previous invocation
	 * of the allocateStripLengths: method.
	 *
	 * It is safe to invoke this method more than once, or even if allocateStripLengths:
	 * was not previously invoked.
	 *
	 * This method is invoked automatically when this instance is deallocated.
	 */
	virtual void				deallocateStripLengths();

	/**
	 * Returns the number of faces in this array.
	 *
	 * This is calculated from the number of vertices, taking into
	 * consideration the drawing mode of this array.
	 */
	virtual GLuint				getFaceCount();

	/**
	 * Returns the number of faces to be drawn from the specified number
	 * of vertex indices, based on the drawingMode of this array.
	 */ 
	virtual GLuint				getFaceCountFromVertexIndexCount( GLuint vc );

	/**
	 * Returns the number of vertex indices required to draw the specified
	 * number of faces, based on the drawingMode of this array.
	 */ 
	virtual GLuint				getVertexIndexCountFromFaceCount( GLuint fc );

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
	 */
	virtual CC3FaceIndices		getFaceIndicesAt( GLuint faceIndex );

	/**
	 * Returns the indicies for the face at the specified face index,
	 * within an array of vertices of the specified length.
	 */
	CC3FaceIndices				getFaceIndicesAt( GLuint faceIndex, GLuint stripLen );

	// Template method that populates this instance from the specified other instance.
	// This method is invoked automatically during object copying via the copyWithZone: method.
	void						populateFrom( CC3DrawableVertexArray* another );
	virtual CCObject*			copyWithZone( CCZone* zone );

	void						initWithTag( GLuint aTag, const std::string& aName );


protected:
	GLenum						m_drawingMode;
	GLuint						m_stripCount;
	GLuint*						m_stripLengths;
	bool						m_stripLengthsAreRetained : 1;
};

NS_COCOS3D_END

#endif
