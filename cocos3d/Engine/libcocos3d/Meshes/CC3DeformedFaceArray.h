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
#ifndef _CC3_DEFORMED_FACE_ARRAY_H_
#define _CC3_DEFORMED_FACE_ARRAY_H_

NS_COCOS3D_BEGIN

/**
 * CC3DeformedFaceArray extends CC3FaceArray to hold the deformed positions of each vertex.
 * From this, the deformed shape and orientation of each face in the mesh can be retrieved.
 *
 * If configured to cache the face data (if the shouldCacheFaces is set to YES),
 * the instance will register as a transform listener with the skin mesh node,
 * so that the faces can be rebuilt if the skin mesh node or any of the bones move.
 */
class CC3DeformedFaceArray : public CC3FaceArray 
{
	DECLARE_SUPER( CC3FaceArray );
public:
	CC3DeformedFaceArray();
	virtual ~CC3DeformedFaceArray();
	/**
	 * The skin mesh node containing the vertices for which this face array is managing faces.
	 *
	 * Setting this property will also set the mesh property, and will cause the
	 * deformedVertexLocations, centers, normals, planes and neighbours properties
	 * to be deallocated and then re-built on the next access.
	 */
	CC3SkinMeshNode*			getNode();
	void						setNode( CC3SkinMeshNode* node );

	/**
	 * Indicates the number of vertices in the deformedVertexLocations array, as retrieved from the mesh.
	 *
	 * The value of this property will be zero until either the node or mesh properties are set.
	 */
	GLuint						getVertexCount();

	/**
	 * An array containing the vertex locations of the underlying mesh,
	 * as deformed by the current position and orientation of the bones.
	 *
	 * This property will be lazily initialized on the first access after the node property 
	 * has been set, by an automatic invocation of the populateDeformedVertexLocations method. 
	 * When created in this manner, the memory allocated to hold the data in the returned 
	 * array will be managed by this instance.
	 *
	 * Alternately, this property may be set directly to an array that was created externally. 
	 * In this case, the underlying data memory is not managed by this instance, and it is up 
	 * to the application to manage the allocation and deallocation of the underlying data memory,
	 * and to ensure that the array is large enough to contain the number of CC3Vector structures
	 * specified by the vertexCount property.
	 */
	CC3Vector*					getDeformedVertexLocations();
	void						setDeformedVertexLocations( CC3Vector* locations );

	/**
	 * Returns the deformed vertex location of the face at the specified vertex index,
	 * that is contained in the face with the specified index, lazily initializing the
	 * deformedVertexLocations property if needed.
	 */
	CC3Vector					getDeformedVertexLocationAt( GLuint vertexIndex, GLuint faceIndex );

	/**
	 * Populates the contents of the deformedVertexLocations property from the associated
	 * mesh, automatically allocating memory for the property if needed.
	 *
	 * This method is invoked automatically on the first access of the deformedVertexLocations
	 * property after the node property has been set. Usually, the application never needs to
	 * invoke this method directly.
	 *
	 * However, if the deformedVertexLocations property has been set to an array created
	 * outside this instance, this method may be invoked to populate that array from the mesh.
	 */
	void						populateDeformedVertexLocations();

	/**
	 * Allocates underlying memory for the deformedVertexLocations property, and returns
	 * a pointer to the allocated memory.
	 *
	 * This method will allocate enough memory for the deformedVertexLocations property
	 * to hold the number of CC3Vector structures specified by the vertexCount property.
	 *
	 * This method is invoked automatically by the populateDeformedVertexLocations
	 * method. Usually, the application never needs to invoke this method directly.
	 *
	 * It is safe to invoke this method more than once, but understand that any  previously 
	 * allocated memory will be safely released prior to the allocation of the new memory. 
	 * The memory allocated earlier will therefore be lost and should not be referenced.
	 * 
	 * The memory allocated will automatically be released when this instance is deallocated.
	 */
	CC3Vector*					allocateDeformedVertexLocations();

	/**
	 * Deallocates the underlying memory that was previously allocated with the 
	 * allocateDeformedVertexLocations method. It is safe to invoke this method more than 
	 * once, or even if the allocateDeformedVertexLocations method was not previously invoked.
	 *
	 * This method is invoked automatically when allocateDeformedVertexLocations is invoked, and when
	 * this instance is deallocated. Usually, the application never needs to invoke this method directly.
	 */
	void						deallocateDeformedVertexLocations();

	/** Marks the deformed vertices data as dirty. It will be automatically repopulated on the next access. */
	void						markDeformedVertexLocationsDirty();
		
	/**
	 * Clears any caches that contain deformable information, including deformed vertices, 
	 * plus face centers, normals, and planes.
	 */
	void						clearDeformableCaches();

	static CC3DeformedFaceArray* faceArrayWithName( const std::string& aName );

	CC3Face						getFaceAt( GLuint faceIndex );
	void						initWithTag( GLuint aTag, const std::string& aName );

	// Template method that populates this instance from the specified other instance.
	// This method is invoked automatically during object copying via the copyWithZone: method.
	void						populateFrom( CC3DeformedFaceArray* another );
	virtual CCObject*			copyWithZone( CCZone* zone );

	/**
	 * Adds this array as listener to bone movements.
	 * If turning off, clears all caches except neighbours.
	 */
	void						setShouldCacheFaces( bool shouldCache );
	
protected:
	CC3SkinMeshNode*			m_pNode;
	CC3Vector*					m_deformedVertexLocations;
	bool						m_deformedVertexLocationsAreRetained : 1;
	bool						m_deformedVertexLocationsAreDirty : 1;
};

NS_COCOS3D_END

#endif
