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
#ifndef _CC3_VERTEX_LOCATIONS_H_
#define _CC3_VERTEX_LOCATIONS_H_

NS_COCOS3D_BEGIN

/**
 * A CC3VertexArray that manages the location aspect of an array of vertices.
 *
 * This class is also a type of CC3DrawableVertexArray, and as such, is capable of drawing
 * the vertices to the GL engine.
 *
 * Since the vertex locations determine the size and shape of the mesh, this class is
 * also responsible for determining the boundingBox of the mesh.
 */
class CC3VertexLocations : public CC3DrawableVertexArray 
{
	DECLARE_SUPER( CC3DrawableVertexArray );
public:
	static CC3VertexLocations*	vertexArray();
	static CC3VertexLocations*	vertexArrayWithName( const std::string& aName );
	/**
	 * An index reference to the first element that will be drawn.
	 *
	 * Typically, all vertices are to be drawn, and this property will be zero. In some applications,
	 * large sets of underlying content may be used for the vertex arrays of more than one mesh.
	 * In such a case, it may be desirable to start drawing from an vertex that is not the first
	 * vertex of the array. This property can be set to indicate at which element index to start drawing.
	 * If drawing is being performed in strips, this will be the index of the start of the first strip to be drawn.
	 *
	 * The initial value is zero.
	 */
	GLuint						getFirstVertex();
	void						setFirstVertex( GLuint vertex );

	/** Returns the axially-aligned bounding box of this mesh. */
	CC3Box						getBoundingBox();

	/** Returns the center of geometry of this mesh. */
	CC3Vector					getCenterOfGeometry();

	/**
	 * Returns the radius of a spherical boundary, centered on the centerOfGeometry,
	 * that encompasses all the vertices of this mesh.
	 */
	GLfloat						getRadius();

	/** Marks the boundary, including bounding box and radius, as dirty, and need of recalculation. */
	void						markBoundaryDirty();

	/**
	 * Returns the location element at the specified index in the underlying vertex content.
	 *
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * the vertexStride and elementOffset properties to access the correct element.
	 *
	 * This implementation takes into consideration the elementSize property. If the value
	 * of the elementSize property is 2, the returned vector will contain zero in the Z component.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	CC3Vector					getLocationAt( GLuint index );

	/**
	 * Sets the location element at the specified index in the underlying vertex content to
	 * the specified location value.
	 * 
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * the vertexStride and elementOffset properties to access the correct element.
	 *
	 * This implementation takes into consideration the elementSize property. If the value
	 * of the elementSize property is 2, the Z component of the specified vector will be
	 * ignored. If the value of the elementSize property is 4, the specified vector will
	 * be converted to a 4D vector, with the W component set to one, before storing.
	 * 
	 * If the new vertex location changes the bounding box of this instance, and this
	 * instance is being used by any mesh nodes, be sure to invoke the markBoundingVolumeDirty
	 * method on all mesh nodes that use this vertex array, to ensure that the boundingVolume
	 * encompasses the new vertex location.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	void						setLocation( const CC3Vector& aLocation, GLuint index );

	/**
	 * Returns the location element at the specified index in the underlying vertex content,
	 * as a four-dimensional location in the 4D homogeneous coordinate space.
	 *
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * the vertexStride and elementOffset properties to access the correct element.
	 *
	 * This implementation takes into consideration the elementSize property. If the
	 * value of the elementSize property is 3, the returned vector will contain one
	 * in the W component. If the value of the elementSize property is 2, the returned
	 * vector will contain zero in the Z component and one in the W component.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	CC3Vector4					getHomogeneousLocationAt( GLuint index );

	/**
	 * Sets the location element at the specified index in the underlying vertex content to
	 * the specified four-dimensional location in the 4D homogeneous coordinate space.
	 * 
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * the vertexStride and elementOffset properties to access the correct element.
	 *
	 * This implementation takes into consideration the elementSize property. If the value
	 * of the elementSize property is 3, the W component of the specified vector will be
	 * ignored. If the value of the elementSize property is 2, both the W and Z components
	 * of the specified vector will be ignored.
	 * 
	 * If the new vertex location changes the bounding box of this instance, and this
	 * instance is being used by any mesh nodes, be sure to invoke the markBoundingVolumeDirty
	 * method on all mesh nodes that use this vertex array, to ensure that the boundingVolume
	 * encompasses the new vertex location.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	void						setHomogeneousLocation( const CC3Vector4& aLocation, GLuint index );

	/**
	 * Changes the mesh vertices so that the origin of the mesh is at the specified location.
	 *
	 * The origin of the mesh is the location (0,0,0) in the local coordinate system, and is the
	 * location around which all transforms are performed.
	 *
	 * This method can be used to adjust the mesh structure to make it easier to apply transformations,
	 * by moving the origin of the transformations to a more convenient location in the mesh.
	 *
	 * This method changes the location component of every vertex in the mesh. This can be quite costly,
	 * and should only be performed once, to adjust a mesh so that it is easier to manipulate. As an
	 * alternate, you should consider changing the origin of the mesh at development time using a 3D editor.
	 * 
	 * Do not use this method to move your model around. Instead, use the transform properties (location,
	 * rotation and scale) of the CC3MeshNode that contains this mesh, and let the GL engine do the heavy
	 * lifting of transforming the mesh vertices.
	 * 
	 * If this mesh is being used by any mesh nodes, be sure to invoke the markBoundingVolumeDirty method
	 * on all nodes that use this mesh, to ensure that the boundingVolume is recalculated using the new
	 * location values. Invoking this method on the CC3MeshNode instead will automatically invoke the
	 * markBoundingVolumeDirty method.
	 *
	 * This method ensures that the GL VBO that holds the vertex data is updated.
	 */
	void						moveMeshOriginTo( const CC3Vector& aLocation );

	/**
	 * Changes the mesh vertices so that the origin of the mesh is at the center of geometry of the mesh.
	 *
	 * The origin of the mesh is the location (0,0,0) in the local coordinate system, and is the
	 * location around which all transforms are performed.
	 *
	 * This method can be used to adjust the mesh structure to make it easier to apply transformations,
	 * by moving the origin of the transformations to the center of the mesh.
	 *
	 * This method changes the location component of every vertex in the mesh. This can be quite costly,
	 * and should only be performed once, to adjust a mesh so that it is easier to manipulate. As an
	 * alternate, you should consider changing the origin of the mesh at development time using a 3D editor.
	 * 
	 * Do not use this method to move your model around. Instead, use the transform properties (location,
	 * rotation and scale) of the CC3MeshNode that contains this mesh, and let the GL engine do the heavy
	 * lifting of transforming the mesh vertices.
	 * 
	 * If this mesh is being used by any mesh nodes, be sure to invoke the markBoundingVolumeDirty method
	 * on all nodes that use this mesh, to ensure that the boundingVolume is recalculated using the new
	 * location values. Invoking this method on the CC3MeshNode instead will automatically invoke the
	 * markBoundingVolumeDirty method.
	 *
	 * This method ensures that the GL VBO that holds the vertex data is updated.
	 */
	void						moveMeshOriginToCenterOfGeometry();

	/**
	 * Returns the face from the mesh at the specified index.
	 *
	 * The specified faceIndex value refers to the index of the face, not the vertices
	 * themselves. So, a value of 5 will retrieve the three vertices that make up the
	 * fifth triangular face in this mesh. The specified index must be between zero,
	 * inclusive, and the value of the faceCount property, exclusive.
	 *
	 * The returned face structure contains only the locations of the vertices. If the vertex
	 * locations are interleaved with other vertex content, such as color or texture coordinates,
	 * or other padding, that content will not appear in the returned face structure.
	 *
	 * This method takes into consideration the drawingMode of this vertex array,
	 * and any padding (stride) between the vertex indices.
	 *
	 * This method is only meaningful if the vertices are drawn directly from this vertex
	 * array, without using vertex indexing. If vertex indexing is in use (the mesh contains
	 * an instance of CC3VertexIndices) the order of the vertices in this array will likely
	 * not be accurate.
	 */
	CC3Face						getFaceAt( GLuint faceIndex );

	/**
	 * Returns the mesh face that is made up of the three vertices at the three indices
	 * within the specified face indices structure. Because indexing is used, the three
	 * vertices that make up the face may not be contiguous within this array.
	 *
	 * The returned face structure contains only the locations of the vertices. If the vertex
	 * locations are interleaved with other vertex content, such as color or texture coordinates,
	 * or other padding, that content will not appear in the returned face structure.
	 */
	CC3Face						getFaceFromIndices( const CC3FaceIndices& faceIndices );

	void						setVertexCount( GLuint count );

	// Template method that populates this instance from the specified other instance.
	// This method is invoked automatically during object copying via the copyWithZone: method.
	void						populateFrom( CC3VertexLocations* another );
	virtual CCObject*			copyWithZone( CCZone* zone );

	/** Builds the bounding box if it needs to be built. */
	void						buildBoundingBoxIfNecessary();

	/**
	 * Calculates and populates the boundingBox and centerOfGeometry properties
	 * from the vertex locations.
	 *
	 * This method is invoked automatically when the bounding box or centerOfGeometry property
	 * is accessed for the first time after the vertices property has been set.
	 */
	void						buildBoundingBox();

	/** Calculates the radius if it necessary. */
	void						calcRadiusIfNecessary();

	/**
	 * Calculates and populates the radius property from the vertex locations.
	 *
	 * This method is invoked automatically when the radius property is accessed
	 * for the first time after the boundary has been marked dirty.
	 */
	void						calcRadius();

	/** Overridden to ensure the bounding box and radius are built before releasing the vertices. */
	void						releaseRedundantContent();
	void						drawFrom( GLuint vtxIdx, GLuint vtxCount, CC3NodeDrawingVisitor* visitor );
	std::string					getNameSuffix();
	void						initWithTag( GLuint aTag, const std::string& aName );
	GLenum						defaultSemantic();

protected:
	// Mark boundary dirty, but only if vertices are valid (to avoid marking dirty on dealloc)
	void						verticesWereChanged();

protected:
	GLuint						m_firstVertex;
	CC3Box						m_boundingBox;
	CC3Vector					m_centerOfGeometry;
	GLfloat						m_radius;
	bool						m_boundaryIsDirty : 1;
	bool						m_radiusIsDirty : 1;
};

NS_COCOS3D_END

#endif
