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
#ifndef _CC3_VERTEX_ARRAYS_H_
#define _CC3_VERTEX_ARRAYS_H_

NS_COCOS3D_BEGIN

class CC3NodeDrawingVisitor;
class CC3Texture;

/**
 * CC3VertexArrayContent contains the vertex content data on behalf of a CC3VertexArray.
 *
 * This is a simple direct-access class with public instance variables, making this class little
 * more than a memory-allocated structure. This design is deliberate.
 *
 * When vertex content is interleaved, multiple vertex arrays share access to the same vertex content
 * memory and the same GL buffer resources. In this situation, a single CC3VertexArrayContent instance
 * will be shared between the vertex arrays whose data are interleaved within the mesh, giving all
 * interleaved vertex arrays access to the same vertex content memory and GL buffer resources.
 *
 * When vertex content is not interleaved, each vertex array will contain a separate instance of
 * CC3VertexArrayContent, giving each vertex array access to its own vertex content memory and GL
 * buffer resources.
 */
class CC3VertexArrayContent : public CCObject 
{
public:
	CC3VertexArrayContent();

	void						init();

public:
	GLvoid*						m_vertices;
	GLuint						m_vertexCount;
	GLuint						m_allocatedVertexCapacity;
	GLuint						m_vertexStride;
	GLuint						m_bufferID;
	GLenum						m_bufferUsage;
	CCRange						m_dirtyVertexRange;
	bool						m_shouldAllowVertexBuffering : 1;
	bool						m_shouldReleaseRedundantContent : 1;
	bool						m_wasVertexCapacityChanged : 1;		// Future use to track dirty vertex range
};

/**
 * CC3VertexArray manages the content associated with an aspect of a vertex. CC3VertexArray
 * is an abstract implementation, and there are several sublcasses, each specialized to
 * manage the vertex content for a different vertex aspect (locations, normals, colors,
 * texture mapping, indices...).
 *
 * Each instance of a subclass of CC3VertexArray maintains a reference to the underlying
 * vertex content in memory, along with various parameters describing the underlying content,
 * such as its type, element size, stride, etc.
 *
 * The underlying content can be interleaved and shared by several CC3VertexArray subclasses,
 * each looking at a different aspect of the content for each vertex. In this case, the vertices
 * property of each of those vertex array instances will reference the same underlying content
 * memory, and the elementOffset property of each CC3VertexArray instance will indicate at which
 * offset in each vertex content the datum of interest to that instance is located.
 *
 * The CC3VertexArray instance also manages buffering the content to the GL engine, including
 * loading it into a server-side GL vertex buffer object (VBO) if desired. Once loaded into
 * the GL engine buffers, the underlying content can be released from the CC3VertexArray instance,
 * thereby freeing memory, by using the releaseRedundantContent method.
 *
 * The CC3DrawableVertexArray abstract subclass adds the functionality to draw the vertex
 * content to the display through the GL engine.
 *
 * When drawing the vertices to the GL engine, each subclass remembers which vertices were
 * last drawn, and only binds the vertices to the GL engine when a different set of vertices
 * of the same type are drawn. This allows the application to organize the CC3MeshNodes
 * within the CC3Scene so that nodes using the same mesh vertices are drawn together, before
 * moving on to other meshes. This strategy can minimize the number of vertex pointer
 * switches in the GL engine, which improves performance.
 *
 * Vertex arrays support the NSCopying protocol, but in normal operation, the need to create
 * copies of vertex arrays is rare.
 *
 * By default, when a mesh node is copied, it does not make a separate copy of its model.
 * Both the original and the copy make use of the same mesh instance. Similarly, when
 * a mesh is copied, it does not make separate copies of its vertex arrays. Instead,
 * both the original and the copy make use of the same vertex array instances.
 *
 * However, in some cases, such as populating a mesh from a template and then manipulating
 * the contents of each resulting mesh individually, creating copies of vertex arrays can
 * be useful.
 *
 * If you do find need to create a copy of a vertex array, you can do so by invoking the
 * copy method. However, you should take care to understand several points about copying
 * vertex arrays:
 *   - Copying a vertex array creates a full copy of the vertex content. This may consume
 *     significant memory.
 *   - The vertex content is copied for each vertex array copy. If several vertex arrays share
 *     interleaved content, multiple copies of that content will be created. This is almost
 *     never what you intend to do, and results in significant redundant content in memory.
 *     Instead, consider creating a copy of one of the vertex arrays, and then manually
 *     populating the others so that the interleaved vertex content can be shared.
 *   - If the value of the shouldReleaseRedundantContent property of the original vertex
 *     array is YES and releaseRedundantContent has been invoked, there will be no vertex
 *     content to be copied.
 *   - The new vertex array will not have a GL vertex buffer object associated with it.
 *     To buffer the vertex content of the new vertex array, invoke the createGLBuffer method
 *     on the new vertex array.
 */
class CC3VertexArray : public CC3Identifiable 
{
	DECLARE_SUPER( CC3Identifiable );
public:
	CC3VertexArray();
	virtual ~CC3VertexArray();
	/**
	 * The CC3VertexArrayContent instance that contains the vertex content data on behalf of this vertex array.
	 *
	 * This property is set automatically when the vertex array is assigned to a mesh, or when the
	 * shouldInterleaveVertices property of the mesh is changed. Usually, the application never needs
	 * to access or set this property.
	 */
	//@property(nonatomic, retain) CC3VertexArrayContent* vertexContent;

	/**
	 * Indicates the vertex attribute semantic of this array.
	 *
	 * Under OpenGL ES 2, this values are used to match a vertex array to its semantic usage
	 * within a GLSL vertex shader.
	 *
	 * The initial value of this property is set by from the defaultSemantic class property,
	 * which subclasses override to provide an appropriate semantic value from the
	 * CC3VertexContentSemantic enumeration, based on the vertex array type.
	 *
	 * The app may change this property to a custom value if desired. The custom value should be
	 * kept within the range defined by kCC3SemanticAppBase and kCC3SemanticMax.
	 */
	GLenum						getSemantic();
	void						setSemantic( GLenum semantic );

	/**
	 * The default value for the semantic property.
	 *
	 * Each subclass will provide an appropriate value from the CC3VertexContentSemantic enumeration.
	 */
	virtual GLenum				defaultSemantic();

	/**
	 * A pointer to the underlying vertex content. If the underlying content memory is assigned
	 * to this instance using this property directly, the underlying content memory is neither
	 * retained nor deallocated by this instance. It is up to the application to manage the
	 * allocation and deallocation of the underlying content memory.
	 *
	 * Alternately, the allocatedVertexCapacity property can be used to have this instance allocate
	 * and manage the underlying vertex content. When this is done, the underlying content memory
	 * will be retained and deallocated by this instance.
	 *
	 * The underlying content can be interleaved and shared by several CC3VertexArray subclasses,
	 * each looking at a different aspect of the content for each vertex. In this case, the
	 * vertices property of each of those vertex array instances will reference the same
	 * underlying content memory, and the elementOffset property will indicate at which offset
	 * in each vertex content the datum of interest to that instance is located.
	 */
	GLvoid*						getVertices();
	virtual void				setVertices( GLvoid* vertices );

	/**
	 * The number of vertices in the underlying content referenced by the vertices property.
	 * The vertices property must point to an underlying memory space that is large enough
	 * to hold the amount of content specified by this property.
	 *
	 * The initial value is zero.
	 *
	 * Setting the value of the allocatedVertexCapacity property also sets the value of this
	 * property to the same value. After setting the allocatedVertexCapacity property, if you
	 * will not be using all of the allocated vertices immediately, you should set the value
	 * of this vertexCount property to the actual number of vertices in use.
	 */
	GLuint						getVertexCount();
	virtual void				setVertexCount( GLuint count );

	/**
	 * When using interleaved content, this property indicates the offset, within the content for a
	 * single vertex, at which the datum managed by this instance is located. When content is not
	 * interleaved, and the vertices content is dedicated to this instance, this property will be zero.
	 *
	 * The initial value is zero.
	 */
	GLuint						getElementOffset();
	void						setElementOffset( GLuint elementOffset );

	/**
	 * The number of components associated with each vertex in the underlying content.
	 *
	 * As an example, the location of each vertex in 3D space is specified by three components (X,Y & Z),
	 * so the value of this property in an instance tracking vertex locations would be three.
	 *
	 * When allocating non-interleaved vertex memory, setting this property affects the amount of
	 * memory allocated by the allocatedVertexCapacity property. If this property is set after
	 * the allocatedVertexCapacity property has been set, vertex memory will be reallocated again.
	 * To avoid allocating twice, if you are not interleaving content, and you need to set this
	 * property, do so before setting the allocatedVertexCapacity property.
	 *
	 * The initial value is three. Subclass may override this default.
	 */
	GLint						getElementSize();
	virtual void				setElementSize( GLint size );

	/**
	 * The type of content associated with each component of a vertex.
	 * This must be a valid enumerated GL content type suitable for the type of element.
	 *
	 * When allocating non-interleaved vertex memory, setting this property affects the amount of
	 * memory allocated by the allocatedVertexCapacity property. If this property is set after
	 * the allocatedVertexCapacity property has been set, vertex memory will be reallocated again.
	 * To avoid allocating twice, if you are not interleaving content, and you need to set this
	 * property, do so before setting the allocatedVertexCapacity property.
	 *
	 * The initial value is GL_FLOAT.
	 */
	GLenum						getElementType();
	void						setElementType( GLenum elementType );

	/**
	 * Returns the length, or size, of each individual element, measured in bytes.
	 *
	 * The returned value is the result of multiplying the size of the content type identified
	 * by the elementType property, with the value of the elementSize property.
	 *
	 * For example, if the elementType property is GL_FLOAT and the elementSize property
	 * is 3, this property will return (sizeof(GLfloat) * 3) = (4 * 3) = 12.
	 *
	 * For non-interleaved content, the value of this property will be the same as the
	 * value of the vertexStride property. For interleaved content, the value of this
	 * property will be smaller than the value of the vertexStride property.
	 */
	GLuint						getElementLength();

	/**
	 * The number of bytes between consecutive vertices for the vertex aspect being
	 * managed by this instance.
	 *
	 * If the underlying content is not interleaved, and contains only the content managed
	 * by this instance, the value of this property will be the same as that of the
	 * elementLength property, and this property does not need to be set explicitly.
	 * 
	 * If the underlying content is interleaved and contains content for several vertex aspects
	 * (location, normals, colors...) interleaved in one memory space, this value should
	 * be set by the application to indicate the distance, in bytes, from one element of
	 * this aspect to the next.
	 *
	 * When allocating interleaved vertex memory, setting this property affects the amount of
	 * memory allocated by the allocatedVertexCapacity property. If this property is set after
	 * the allocatedVertexCapacity property has been set, vertex memory will be reallocated again.
	 * To avoid allocating twice, if you need to set this property, do so before setting the
	 * allocatedVertexCapacity property.
	 *
	 * The initial value of this property is the same as the value of the elementLength property.
	 */
	GLuint						getVertexStride();
	void						setVertexStride( GLuint stride );

	/**
	 * Indicates whether the vertex content should be normalized during drawing.
	 *
	 * This property applies only to OpenGL ES 2. When using OpenGL ES 1, this property can be ignored.
	 *
	 * Under OpenGL ES 2, vertex content that is provided in an integer format (eg. the elementType property
	 * is set to anything other than GL_FLOAT), this property indicates whether the element content should
	 * be normalized, by being divided by their maximum range, to convert them into floating point variables
	 * between 0 & 1 (for unsigned integer types), or -1 & +1 (for signed integer types).
	 *
	 * If this property is set to YES, the element content will be normalized, otherwise it will be
	 * used as is. The normalization activity takes place in the GL engine.
	 *
	 * The default value of this property is NO, indicating that the element content will not be
	 * normalized during drawing.
	 */
	bool						shouldNormalizeContent();
	void						setShouldNormalizeContent( bool shouldNormalize );

	/**
	 * If the underlying content has been loaded into a GL engine vertex buffer object, this
	 * property holds the ID of that GL buffer as provided by the GL engine when the
	 * createGLBuffer method was invoked. If the createGLBuffer method was not invoked,
	 * and the underlying vertex was not loaded into a GL VBO, this property will be zero.
	 */
	GLuint						getBufferID();
	void						setBufferID( GLuint bufferID );

	/**
	 * The GL engine buffer target. Must be one of GL_ARRAY_BUFFER or GL_ELEMENT_ARRAY_BUFFER.
	 *
	 * The default value is GL_ARRAY_BUFFER. Subclasses that manage index content will override.
	 */
	virtual GLenum				getBufferTarget();

	/**
	 * The GL engine buffer usage hint, used by the GL engine to arrange content for access when
	 * loading content into a server-side vertex buffer object.
	 *
	 * The default value is GL_STATIC_DRAW, indicating to the GL engine that the content will
	 * generally not be re-accessed after loading. If you will be updating the content frequently,
	 * you can change this to GL_DYNAMIC_DRAW.
	 */
	GLenum						getBufferUsage();
	void						setBufferUsage( GLenum usage );

	/**
	 * Allocates and initializes an autoreleased unnamed instance with an automatically
	 * generated unique tag value. The tag value is generated using a call to nextTag.
	 */
	static CC3VertexArray*		vertexArray();

	/** Allocates and initializes an unnamed autoreleased instance with the specified tag. */
	static CC3VertexArray*		vertexArrayWithTag( GLuint aTag );

	/**
	 * Allocates and initializes an autoreleased instance with the specified name and an
	 * automatically generated unique tag value. The tag value is generated using a call to nextTag.
	 */
	static CC3VertexArray*		vertexArrayWithName( const std::string& aName );

	/** Allocates and initializes an autoreleased instance with the specified tag and name. */
	static CC3VertexArray*		vertexArrayWithTag( GLuint aTag, const std::string& aName );

	/**
	 * Configure this vertex array to use the same underlying vertex content as the specified
	 * other vertex array, with the content used by this array interleaved with the content from
	 * the other vertex array. This can be repeated with other arrays to interleave the content
	 * from several vertex arrays into one underlying memory buffer.
	 *
	 * This is a convenience method that sets the vertices, vertexStride, and vertexCount
	 * properties of this vertex array to be the same as those of the other vertex array,
	 * and then sets the elementOffset property of this vertex array to the specified
	 * elemOffset value.
	 *
	 * Returns a pointer to the vertices array, offset by the elemOffset. This is effectively
	 * a pointer to the first element in this vertex array, and can be used as a starting
	 * point to iterate the array to populate it.
	 */
	virtual GLvoid*				interleaveWith( CC3VertexArray* otherVtxArray, GLuint elemOffset );

	/**
	 * Configure, or reconfigure, this vertex array to use the same underlying vertex content as
	 * the specified other vertex array, with the content used by this array interleaved with the
	 * content from the other vertex array. This can be repeated with other arrays to interleave
	 * the content from several vertex arrays into one underlying memory buffer.
	 *
	 * This is a convenience method that invokes the interleaveWith:usingOffset: method, passing
	 * the existing value of the elementOffset property of this vertex array for the elemOffset.
	 *
	 * Returns a pointer to the vertices array, offset by the elementOffset of this vertex array.
	 * This is effectively a pointer to the first element in this vertex array, and can be used
	 * as a starting point to iterate the array to populate it.
	 *
	 * This method assumes that the elementOffset property has already been set. The returned
	 * pointer will not be accurate if the elementOffset property has not been set already.
	 *
	 * Because of this, when creating a new mesh containing several interleaved vertex arrays,
	 * it is better to use the interleaveWith:usingOffset: method. This method is useful when
	 * changing the vertex capacity of the mesh, and you want to retain the existing elementCount
	 * property of each vertex array.
	 */
	virtual GLvoid*				interleaveWith( CC3VertexArray* otherVtxArray );

	/**
	 * Allocates, reallocates, or deallocates underlying memory for the specified number of vertices,
	 * taking into consideration the amount of memory required by each vertex. Specifically, the total
	 * amount of memory allocated will be (allocatedVertexCapacity * self.vertexStride) bytes.
	 *
	 * Setting this property affects the value of the vertices and vertexCount properties. After
	 * setting this property, the vertices property will point to the allocated memory, and the
	 * vertexCount property will be set to the same value as this property. After setting this
	 * property, if you will not be using all of the allocated vertices immediately, you should
	 * set the value of the vertexCount property to the actual number of vertices in use.
	 *
	 * Use of this property is not required if the vertex content has already been loaded into memory
	 * by a file loader, or defined by a static array. In that situation, you should set the vertices
	 * and vertexCount properties directly, and avoid using this property.
	 *
	 * Since memory allocation is dependent on the vertex stride, before setting this property, ensure
	 * that the vertexStride, or elementSize and elementType properties have been set appropriately.
	 * If the underlying content is to be interleaved, set the value of the vertexStride property to the
	 * appropriate value before setting this property. If the underlying content will NOT be interleaved,
	 * the vertexStride property can be determined by the elementType and elementSize properties, and
	 * you should set the correct values of those two properties before setting the value of this property.
	 *
	 * This property may be set repeatedly to manage the underlying mesh vertex content as a
	 * dynamically-sized array, growing and shrinking the allocated memory as needed. When doing
	 * so, keep in mind the vertices property can change as a result of any reallocation of memory.
	 *
	 * In addition, you can set this property to zero to safely deallocate all memory used by the
	 * vertex content of this array. After setting this property to zero, the value of the vertices
	 * property will be a NULL pointer, and the value of the vertexCount property will be zero.
	 *
	 * When setting the value of this property to a new non-zero value, all current vertex content,
	 * up to the lesser of the new and old values of this property, will be preserved. However,
	 * keep in mind that, if the memory allocation has increased, that vertex content may have been
	 * moved to a new location, resulting in a change to the vertices property.
	 *
	 * If the value of this property is increased (including from zero on the first assignement),
	 * vertex content for those vertices beyond the old value of this property will be undefined,
	 * and must be populated by the application before attempting to draw that vertex content.
	 *
	 * If you are not ready to populate the newly allocated vertex content yet, after setting the value
	 * of this property, you can set the value of the vertexCount property to a value less than the
	 * value of this property (including to zero) to stop such undefined vertex content from being drawn.
	 *
	 * When interleaving content, this method should be invoked on only one of the CC3VertexArray
	 * instances that are sharing the underlying content (typically the CC3VertexLocations instance).
	 * After allocating on one CC3VertexArray instances, set the vertices property of the other
	 * instances to be equal to the vertices property of the CC3VertexArray instance on which this
	 * method was invoked (or just simply to the pointer returned by this method).
	 */
	GLuint						getAllocatedVertexCapacity();
	void						setAllocatedVertexCapacity( GLuint capacity );
    
    /* Just set vertex capacity, this is used for 3rd-party data import, you should call setAllocatedVertexCapacity usually*/
    void                        setVertexCapacityWithoutAllocation( GLuint capacity );

	/**
	 * Indicates whether this instance should allow the vertex content to be copied to a vertex
	 * buffer object within the GL engine when the createGLBuffer method is invoked.
	 *
	 * The initial value of this property is YES. In most cases, this is appropriate, but for
	 * specific meshes, it might make sense to retain content in main memory and submit it to the
	 * GL engine during each frame rendering.
	 *
	 * As an alternative to setting this property to NO, consider leaving it as YES, and making
	 * use of the updateGLBuffer and updateGLBufferStartingAt:forLength: to dynamically update
	 * the content in the GL engine buffer. Doing so permits the content to be copied to the GL engine
	 * only when it has changed, and permits copying only the range of content that has changed,
	 * both of which offer performance improvements over submitting all of the vertex content on
	 * each frame render.
	 */
	bool						shouldAllowVertexBuffering();
	void						setShouldAllowVertexBuffering( bool allowVertexBuffering );

	/** 
	 * If the shouldAllowVertexBuffering property is set to YES, creates a vertex buffer
	 * object (VBO) within the GL engine, copies the content referenced by the vertices into
	 * the GL engine  (which may make use of VRAM), and sets the value of the bufferID
	 * property to that of the new GL buffer.
	 *
	 * If memory for the vertices was allocated via the allocatedVertexCapacity property,
	 * the GL VBO size is set to the same as the amount allocated by this instance. If
	 * memory was allocated externally, the GL VBO size is set to the value of vertexCount.
	 * 
	 * Calling this method is optional. Using GL engine buffers is more efficient than passing
	 * arrays on each GL draw call, but is optional. If you choose not to call this method,
	 * this instance will pass the mesh content properties to the GL engine on each draw call.
	 *
	 * If the GL engine cannot allocate space for any of the buffers, this instance will
	 * revert to passing the array content for any unallocated buffer on each draw call.
	 *
	 * When using interleaved content, this method should be invoked on only one of the 
	 * CC3VertexArrays that share the content. The bufferID property of that instance should
	 * then be copied to the other vertex arrays.
	 *
	 * Consider using the createGLBuffers of the mesh class instead of this method, which
	 * automatically handles the buffering all vertex arrays used by the mesh, and correctly
	 * coordinates buffering interleaved content.
	 *
	 * It is safe to invoke this method more than once, but subsequent invocations will do nothing.
	 *
	 * This method is invoked automatically by the createGLBuffers method of the mesh class, which
	 * also coordinates the invocations across multiple CC3VertexArray instances when interleaved
	 * content is shared between them, along with the subsequent copying of the bufferID's.
	 */
	virtual void				createGLBuffer();

	/**
	 * Deletes the GL engine buffers created with createGLBuffer.
	 *
	 * After calling this method, if they have not been released by createGLBuffer,
	 * the vertex content will be passed to the GL engine on each subsequent draw operation.
	 * It is safe to call this method even if GL buffers have not been created.
	 * 
	 * This method may be invoked at any time to free up GL memory, but only if this vertex
	 * array will not be used again, or if the content was not released by releaseRedundantContent.
	 * This would be the case if the allocatedVertexCapacity property was not set.
	 *
	 * This method is invoked automatically when this instance is deallocated.
	 */
	virtual void				deleteGLBuffer();

	/**
	 * Updates the GL engine buffer with the element content contained in this array,
	 * starting at the vertex at the specified offsetIndex, and extending for
	 * the specified number of vertices.
	 */
	virtual void				updateGLBufferStartingAt( GLuint offsetIndex, GLuint vertexCount );

	/** Updates the GL engine buffer with all of the element content contained in this array. */
	virtual void				updateGLBuffer();

	/**
	 * Returns whether the underlying vertex content has been loaded into a GL engine vertex
	 * buffer object. Vertex buffer objects are engaged via the createGLBuffer method.
	 */
	virtual bool				isUsingGLBuffer();

	/**
	 * Indicates whether this instance should release the content held in the elments array
	 * when the releaseRedundantContent method is invoked.
	 *
	 * The initial value of this property is YES. In most cases, this is appropriate,
	 * but in some circumstances it might make sense to retain some content (usually the
	 * vertex locations) in main memory for potantial use in collision detection, etc.
	 */
	bool						shouldReleaseRedundantContent();
	void						setShouldReleaseRedundantContent( bool release );

	/**
	 * Once the vertices content has been buffered into a GL vertex buffer object (VBO)
	 * within the GL engine, via the createGLBuffer method, this method can be used
	 * to release the content in main memory that is now redundant.
	 *
	 * If the shouldReleaseRedundantContent property is set to NO, or if the vertices
	 * content has not been successfully buffered to a VBO in the GL engine. this method
	 * does nothing. It is safe to invokde this method even if createGLBuffer has not
	 * been invoked, and even if VBO buffering was unsuccessful.
	 *
	 * Typically, this method is not invoked directly by the application. Instead, 
	 * consider using the same method on a node assembly in order to release as much
	 * memory as possible in one simply method invocation.
	 *
	 * Subclasses may extend this behaviour to remove content loaded, for example, from files,
	 * but should ensure that content is only released if bufferId is valid (not zero),
	 * and the shouldReleaseRedundantContent property is set to YES.
	 */
	virtual void				releaseRedundantContent();

	/**
	 * Binds the vertex content to the vertex attribute at the specified index in the GL engine.
	 *
	 * This is invoked automatically from the CC3Mesh containing this instance.
	 * Usually, the application never needs to invoke this method directly.
	 */
	virtual void				bindContentToAttributeAt( GLint vaIdx, CC3NodeDrawingVisitor* visitor );

	/**
	 * Returns a pointer to the element in the underlying content at the specified index.
	 * The implementation takes into consideration the vertexStride and elementOffset
	 * properties to locate the aspect of interest in this instance.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, or the index is beyond the vertexCount,
	 * this method will raise an assertion exception.
	 */
	virtual GLvoid*				getAddressOfElement( GLuint index );

	/**
	 * Copies vertex content for the specified number of vertices from memory starting at the
	 * specified source vertex index to memory starting at the specified destination vertex index.
	 *
	 * You can use this method to copy content from one area in the vertex array to another area.
	 * 
	 * This is a fast straight memory copy, and assumes that vertex content is consecutive and is
	 * spaced as defined by the vertexStride property. If vertex content is interleaved, the content
	 * in between consecutive elements of this vertex array will also be copied.
	 */
	virtual void				copyVertices( GLuint vtxCount, GLuint srcIdx, GLuint dstIdx );

	/**
	 * Copies vertex content for the specified number of vertices from memory starting at the
	 * specified source vertex index to memory starting at the specified destination address pointer.
	 * 
	 * You can use this method to copy content out of this vertex array to a memory location outside
	 * this vertex array.
	 * 
	 * This is a fast straight memory copy, assumes that vertex content is consecutive and is spaced
	 * as defined by the vertexStride property, and deposits the vertex content at the destination
	 * address in exactly the same format as in this vertex array. If vertex content is interleaved,
	 * the content in between consecutive elements of this vertex array will also be copied.
	 */
	virtual void				copyVertices( GLuint vtxCount, GLuint srcIdx, GLvoid* dstPtr );

	/**
	 * Copies vertex content for the specified number of vertices from memory starting at the
	 * specified source address to memory starting at the specified destination vertex index.
	 * 
	 * You can use this method to copy content into this vertex array from a memory location outside
	 * this vertex array.
	 * 
	 * This is a fast straight memory copy, assumes that vertex content is consecutive is spaced
	 * as defined by the vertexStride property, and that the vertex content at the source address
	 * is structured identically to the content in this vertex array. If vertex content is interleaved,
	 * the content in between consecutive elements of this vertex array will also be copied.
	 */
	virtual void				copyVertices( GLuint vtxCount, GLvoid* srcPtr, GLuint dstIdx );

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
	virtual void				copyVertices( GLuint vtxCount, GLvoid* srcPtr, GLvoid* dstPtr );

	/**
	 * Returns a string containing a description of the elements of this vertex array, with
	 * the contents of each element output on a different line. The number of values output
	 * on each line is dictated by the elementSize property.
	 *
	 * The output contains the all of the vertices in this vertex array. The total number
	 * of values output will therefore be (elementSize * vertexCount).
	 */
	std::string					describeVertices();

	/**
	 * Returns a string containing a description of the specified elements, with the contents
	 * of each element output on a different line. The number of values output on each line
	 * is dictated by the elementSize property.
	 *
	 * The output contains the number of elements specified, starting at the first element in
	 * this vertex array, and is limited to the number of vertices in this array. The total
	 * number of values output will therefore be (elementSize * MIN(vtxCount, vertexCount)).
	 */
	std::string					describeVertices( GLuint vtxCount );

	/**
	 * Returns a string containing a description of the specified elements, with the contents
	 * of each element output on a different line. The number of values output on each line
	 * is dictated by the elementSize property. 
	 *
	 * The output contains the number of vertices specified, starting at the element at the
	 * specified index, and is limited to the number of vertices in this array. The total number
	 * of values output will therefore be (elementSize * MIN(vtxCount, vertexCount - startElem)).
	 */
	std::string					describeVertices( GLuint vtxCount, GLuint startElem );

	/**
	 * The number of available vertices. If the vertices have been allocated by this
	 * array, that is the number of available vertices. Otherwise, if the vertices were
	 * allocated elsewhere, it is the number specified by the vertexCount value.
	 */
	GLuint						getAvailableVertexCount();

	virtual void				initWithTag( GLuint aTag, const std::string& aName );
	virtual void				initWithTag( GLuint aTag );

	void						populateFrom( CC3VertexArray* another );
	virtual CCObject*			copyWithZone( CCZone* zone );

	GLuint						nextTag();
	void						resetTagAllocation();


protected:
	/** The vertices array has been changed. Default is to do nothing. Some subclasses may want to react. */
	virtual void				verticesWereChanged();

	/**
	 * Allocates new vertex memory, reallocates existing vertex memory, or deallocates existing vertex memory.
	 *
	 * If vtxCount is non-zero, and the vertices property is NULL, memory is allocated, and the vertices
	 * property is set to point to it. If vtxCount is non-zero, and the vertices property is non-NULL,
	 * the existing memory is reallocated, possibly changing the value of the vertices property in the
	 * process. If vtxCount is zero, any previously allocated vertices are safely deallocated.
	 *
	 * Returns NO if an error occurs, otherwise returns YES.
	 */
	bool						allocateVertexCapacity( GLuint vtxCount );

	/**
	* Template method that binds the GL engine to the values of the elementSize, elementType
	* and vertexStride properties, along with the specified data pointer, and enables the
	* type of aspect managed by this instance (locations, normals...) in the GL engine.
	*/
	virtual void				bindContent( GLvoid* pointer, GLint vaIdx, CC3NodeDrawingVisitor* visitor );

protected:
	GLuint						m_elementOffset;
	GLint						m_elementSize;
	GLenum						m_elementType;
	GLuint						m_allocatedVertexCapacity;

	GLvoid*						m_vertices;
	GLuint						m_vertexCount;
	GLuint						m_bufferID;
	GLenum						m_bufferUsage;
	GLenum						m_semantic;
	GLuint						m_vertexStride : 8;
	bool						m_shouldNormalizeContent : 1;
	bool						m_shouldAllowVertexBuffering : 1;
	bool						m_shouldReleaseRedundantContent : 1;
	bool						m_wasVertexCapacityChanged : 1;		// Future use to track dirty vertex range
};

NS_COCOS3D_END

#endif
