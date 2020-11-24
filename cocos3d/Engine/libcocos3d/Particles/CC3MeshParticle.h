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
#ifndef _CC3_MESH_PARTICLE_H_
#define _CC3_MESH_PARTICLE_H_

NS_COCOS3D_BEGIN

class CC3Mesh;
class CC3MeshParticleEmitter;
class CC3Rotator;
class CC3MutableRotator;
class CC3DirectionalRotator;
class CC3TargettingRotator;

/**
 * CC3MeshParticleProtocol defines the requirements for mesh particles that are emitted
 * and managed by the CC3MeshParticleEmitter class.
 *
 * Each mesh particle is comprised of an individual mesh. Like any mesh, a mesh particle
 * represents a true 3D object that can have length, width and depth, can be moved, rotated
 * and scaled, and can be colored and covered with a texture.
 *
 * Each mesh particle uses a CC3Mesh as a template. But, because it is a particle,
 * this basic mesh template is copied into the mesh arrays of the CC3MeshParticleEmitter, where
 * it is merged with the meshes of the other particles managed by the emitter, and is submitted
 * to the GL engine in a single draw call.
 *
 * Like mesh nodes, mesh particles may be transformed (moved, rotated & scaled). However, unlike
 * mesh nodes, the vertices of a mesh particle are transformed by the CPU in application memory,
 * and the transformed vertices are drawn without further individual transformation by the GPU.
 *
 * Mesh particles are transformed by the emitter through the transformVertices method on the particle.
 * This method is invoked automatically by the emitter when a particle has been changed, and the mesh
 * particle implementation defines what type of transform occurs when this method is invoked.
 *
 * This creates a trade-off, where, relative to mesh nodes, the GPU rendering performance is
 * dramatically improved for large numbers of mesh particles, but the CPU load is increased
 * when mesh particles are constantly being transformed, particularly for larger meshes.
 *
 * Relative to mesh nodes, mesh particles work best when there are many small meshes that are
 * transfomed slowly, where the benefit of drawing in a single call outweighs the cost of 
 * processing the vertices in the CPU. For larger meshes, it is more effective to use mesh nodes,
 * where the transformations can be carried out by the GPU.
 *
 * See the notes of the CC3MeshParticleEmitter class for more info.
 */
class CC3MeshParticleProtocol
{
public:
	/**
	 * The CC3Mesh used as a template for the mesh of this particle.
	 *
	 * This particle uses the vertices of this mesh as a starting point. The vertices for the particle
	 * are copied into the underlying common mesh that supports all particles emitted by a single emitter.
	 * The particle can then manipulate its own copy of the vertices, and can have its own locations,
	 * rotations, vertex colors and texture coordinates.
	 *
	 * For particles created outside the emitter, and added to the emitter with the emitParticle:
	 * method, this property can be set directly by the application to define the mesh of this particle.
	 * For particles created within the emitter, and emitted automatically, or via the emitParticle
	 * method, this property will be assigned by the emitter, usually from a pre-defined template mesh.
	 */
	virtual CC3Mesh*				getTemplateMesh() = 0;
	virtual void					setTemplateMesh( CC3Mesh* templateMesh ) = 0;

	/**
	 * Returns the index offset, in the underlying mesh vertex arrays, of the first vertex of this particle.
	 *
	 * This offset can be used to access content directly within the underlying mesh vertex arrays.
	 */
	virtual GLuint					getFirstVertexOffset() = 0;
	virtual void					setFirstVertexOffset( GLuint firstVertexOffset ) = 0;

	/**
	 * Returns the index offset, in the underlying mesh vertex index array, of the first vertex index
	 * of this particle.
	 *
	 * This offset can be used to access the vertex indices directly within the underlying mesh vertex
	 * index array.
	 *
	 * If the underlying mesh is not using indexed vertices, this property will be set to the same value
	 * as the firstVertexOffset property.
	 */
	virtual GLuint					getFirstVertexIndexOffset() = 0;
	virtual void					setFirstVertexIndexOffset( GLuint indexOffset ) = 0;

	/**
	 * Transforms the vertices of this particle.
	 *
	 * For each emitter, all particles are submitted to the GL engine in a single draw call.
	 * This means that all vertices for all particles from that emitter will use the same GL
	 * transform matrix, which is defined by the transformation properties of the emitter.
	 *
	 * To allow each mesh particle to be transformed independently, the vertices for each particle
	 * must be transformed in memory by the CPU.
	 *
	 * This method is invoked automatically on any particle that has been updated, when the emitter
	 * is transformed. Usually the application never needs to invoke this method directly.
	 */
	virtual void					transformVertices() = 0;
};


/**
 * CC3MeshParticle is a standard base implementation of the CC3MeshParticleProtocol.
 *
 * CC3MeshParticle brings many of the capabilities of a CC3MeshNode to particles.
 *
 * Like mesh nodes, particles of this type can be flexibly moved, rotated and scaled, and the
 * vertices will be automatically transformed into the vertex arrays of the emitter.
 *
 * Although all particles in a single emitter must be covered by the same material and texture,
 * particles of this type may be assigned a textureRectangle, allowing each particle to use a
 * separate section of the emitter's texture, effectively texturing each particle separately.
 *
 * The individual vertices of each particle can be manipulated using the same family of vertex
 * access methods available to mesh nodes.
 */
class CC3MeshParticle : public CC3CommonVertexArrayParticle, public CC3MeshParticleProtocol
{
	DECLARE_SUPER( CC3CommonVertexArrayParticle );
public:
	CC3MeshParticle();
	~CC3MeshParticle();
	/**
	 * The emitter that emitted this particle.
	 *
	 * For CC3MeshParticle, the emitter must be of type CC3MeshParticleEmitter.
	 */
	void						setEmitter( CC3ParticleEmitter* anEmitter );

	/**
	 * Returns the rotator that manages the local rotation of this particle.
	 *
	 * CC3Rotator is the base class of a class cluster, of which different subclasses perform
	 * different types of rotation. The type of object returned by this property may change,
	 * depending on what rotational changes have been made to this particle.
	 *
	 * For example, if no rotation is applied to this particle, this property will return a base
	 * CC3Rotator. After the rotation of this node has been changed, this property will return
	 * a CC3MutableRotator, and if directional properties, such as forwardDirection have been
	 * accessed or changed, this property will return a CC3DirectionalRotator. The creation
	 * of the type of rotator required to support the various rotations is automatic.
	 */
	CC3Rotator*					getRotator();
	void						setRotator( CC3Rotator* rotator );

	/**
	 * The location of this particle in the local coordinate system of the emitter.
	 *
	 * You can set this property in the initializeParticle and updateBeforeTransform: methods
	 * to move the particle around.
	 *
	 * The initial value of this property, set prior to the invocation of the 
	 * initializeParticle method, is kCC3VectorZero.
	 */
	CC3Vector					getLocation();
	void						setLocation( const CC3Vector& location );

	/**
	 * Translates the location of this property by the specified vector.
	 *
	 * The incoming vector specify the amount of change in location, not the final location.
	 */
	void						translateBy( const CC3Vector& aVector );

	/**
	 * The rotational orientation of the particle in 3D space, relative to the emitter. The global
	 * rotation of the particle is therefore a combination of the global rotation of the emitter
	 * and the value of this rotation property. This value contains three Euler angles, defining
	 * a rotation of this nodearound the X, Y and Z axes. Each angle is specified in degrees.
	 *
	 * Rotation is performed in Y-X-Z order, which is the OpenGL default. Depending on the nature
	 * of the object you are trying to control, you can think of this order as yaw, then pitch,
	 * then roll, or heading, then inclination, then tilt,
	 *
	 * When setting this value, each component is converted to modulo +/-360 degrees.
	 *
	 * Rotational transformation can also be specified using the rotationAxis and rotationAngle
	 * properties, or the quaternion property. Subsequently, this property can be read to return
	 * the corresponding Euler angles.
	 */
	CC3Vector					getRotation();
	void						setRotation( const CC3Vector& rotation );

	/**
	 * Rotates this particle from its current rotational state by the specified Euler angles in degrees.
	 *
	 * The incoming Euler angles specify the amount of change in rotation, not the final rotational state.
	 */
	void						rotateBy( const CC3Vector& aRotation );

	/**
	 * The rotation of the particle in 3D space, relative to the parent of this node, expressed
	 * as a quaternion.
	 *
	 * Rotational transformation can also be specified using the rotation property (Euler angles),
	 * or the rotationAxis and rotationAngle properties. Subsequently, this property can be read
	 * to return the corresponding quaternion.
	 */
	CC3Quaternion				getQuaternion();
	void						setQuaternion( const CC3Quaternion& quaternion );

	/**
	 * Rotates this particle from its current rotational state by the specified quaternion.
	 *
	 * The incoming quaternion specifies the amount of change in rotation, not the final rotational state.
	 */
	void						rotateByQuaternion( const CC3Quaternion& aQuaternion );

	/**
	 * The axis of rotation of the particle in 3D space, relative to the emitter, expressed as
	 * a directional vector. This axis can be used in conjunction with the rotationAngle property
	 * to describe the rotation as a single angular rotation around an arbitrary axis.
	 *
	 * Under the identity rotation (no rotation), the rotationAngle is zero and the rotationAxis
	 * is undefined. Under that condition, this property will return the zero vector kCC3VectorZero.
	 *
	 * Rotational transformation can also be specified using the rotation property (Euler
	 * angles), or the quaternion property. Subsequently, this property can be read to return
	 * the corresponding axis of rotation.
	 */
	CC3Vector					getRotationAxis();
	void						setRotationAxis( const CC3Vector& rotationAixs );

	/**
	 * The angular rotation around the axis specified in the rotationAxis property.
	 *
	 * When setting this value, it is converted to modulo +/-360 degrees. When reading this
	 * value after making changes using rotateByAngle:aroundAxis:, or using another rotation
	 * property, the value of this property will be clamped to +/-180 degrees.
	 *
	 * For example, if current rotation is 170 degrees around the rotationAxis, invoking
	 * the rotateByAngle:aroundAxis: method using the same rotation axis and 20 degrees,
	 * reading this property will return -170 degrees, not 190 degrees.
	 *
	 * Rotational transformation can also be specified using the rotation property (Euler
	 * angles), or the quaternion property. Subsequently, this property can be read to
	 * return the corresponding angle of rotation.
	 */
	GLfloat						getRotationAngle();

	/**
	 * Rotates this particle from its current rotational state by rotating around
	 * the specified axis by the specified angle in degrees.
	 *
	 * The incoming axis and angle specify the amount of change in rotation, not the final rotational state.
	 *
	 * Thanks to Cocos3D user nt901 for contributing to the development of this feature
	 */
	void						rotateByAngle( GLfloat anAngle, const CC3Vector& anAxis );

	/**
	 * The direction in which this particle is pointing.
	 *
	 * The value of this property is specified in the local coordinate system of this particle.
	 *
	 * The initial value of this property is kCC3VectorUnitZPositive, pointing down the positive
	 * Z-axis in the local coordinate system of this particle. When this particle is rotated, the
	 * original positive-Z axis of the node's local coordinate system will point in this direction.
	 *
	 * Pointing the particle in a particular direction does not fully define its rotation in 3D space,
	 * because the particle can be oriented in any rotation around the axis along the forwardDirection
	 * vector (think of pointing a camera at a scene, and then rotating the camera along the axis
	 * of its lens, landscape towards portrait).
	 *
	 * The orientation around this axis is defined by specifying an additional 'up' direction, which
	 * fixes the rotation around the forwardDirection by specifying which direction is considered to
	 * be 'up'. The 'up' direction is specified by setting the referenceUpDirection property, which
	 * is independent of the tilt of the local axes, and does not need to be perpendicular to the
	 * forwardDirection.
	 *
	 * The value returned for this property is of unit length. When setting this
	 * property, the value will be normalized to be a unit vector.
	 *
	 * A valid direction vector is required. Attempting to set this property
	 * to the zero vector (kCC3VectorZero) will raise an assertion error.
	 */
	CC3Vector					getForwardDirection();
	void						setForwardDirection( const CC3Vector& forwardDirection );

	/**
	 * The direction that is considered to be 'up' when rotating to face in a particular direction,
	 * by using one of the directional properties forwardDirection, target, or targetLocation.
	 *
	 * As explained in the note for the forwardDirection, specifying a forwardDirection alone is not
	 * sufficient to determine the rotation of a particle in 3D space. This property indicates which
	 * direction should be considered 'up' when orienting the rotation of the particle to face a
	 * direction, target, or target location.
	 * 
	 * The interpretation of whether the value of this property is specified in local or global
	 * coordinates depends on how the direction of pointing is being specified. 
	 *
	 * When using the forwardDirection property, the value of this property is taken to be specified
	 * in the local coordinate system. When using either the target or targetLocation properties,
	 * the interpretation of whether the value of this property is specified in the local or global
	 * coordinate system is determined by the value of the targettingConstraint property.
	 *
	 * The initial value of this property is kCC3VectorUnitYPositive, pointing parallel
	 * to the positive Y-axis, and in most cases, this property can be left with that value.
	 *
	 * The value returned is of unit length. When setting this property, the value will be normalized
	 * to be a unit vector.
	 *
	 * When setting this property, a valid direction vector is required. Attempting to set this
	 * property to the zero vector (kCC3VectorZero) will raise an assertion error.
	 */
	CC3Vector					getReferenceUpDirection();
	void						setReferenceUpDirection( const CC3Vector& direction );

	/**
	 * The direction, in the particle's coordinate system, that is considered to be 'up'.
	 * This corresponds to the referenceUpDirection, after it has been transformed by the rotations
	 * of this particle. For example, rotating the particle upwards to point towards an elevated
	 * target will move the upDirection of this particle away from the referenceUpDirection.
	 *
	 * The value returned by this property is in the local coordinate system of this particle.
	 *
	 * The value returned is of unit length. 
	 */
	CC3Vector					getUpDirection();

	/**
	 * The direction in the particle's coordinate system that would be considered to be "off to the right"
	 * when looking out from the particle, along the forwardDirection and with the upDirection defined.
	 *
	 * The value returned by this property is in the local coordinate system of this particle.
	 *
	 * The value returned is of unit length. 
	 */
	CC3Vector					getRightDirection();

	/**
	 * Sets the texture rectangle of this particle, for all texture units.
	 *
	 * This property facilitates the use of sprite-sheets, where the mesh is covered by a small
	 * fraction of a larger texture.
	 *
	 * Setting this property adjusts the texture coordinates of this particle so that they
	 * map to the specified texture rectangle within the bounds of the texture.
	 *
	 * The texture rectangle applied here takes into consideration the textureRectangle
	 * property of the particleTemplateMesh, and the mapSize of the texture itself.
	 *
	 * See the notes for this same property on CC3MeshNode and CC3Mesh for more information
	 * about applying texture rectangles to meshes.
	 *
	 * Once applied, the value of this property is not retained, and reading this property
	 * returns a null rectangle. Subclasses may override to cache the value of this property.
	 */
	CCRect						getTextureRectangle();
	void						setTextureRectangle( const CCRect& texRect );

	/**
	 * Sets the texture rectangle of this particle, for the specified texture unit.
	 *
	 * See the notes for the textureRectangle property of this class, for an explanation
	 * of the use of this property.
	 */
	void						setTextureRectangle( const CCRect& aRect, GLuint texUnit );

	/**
	 * Returns the location element at the specified index from the vertex content.
	 *
	 * The index refers to vertices, not bytes, and is relative to the content of this particle,
	 * not the entire underlying mesh. The implementation takes into consideration whether the
	 * vertex content is interleaved to access the correct vertex data component.
	 *
	 * This implementation takes into consideration the dimensionality of the underlying vertex content.
	 * If the dimensionality is 2, the returned vector will contain zero in the Z component.
	 */
	CC3Vector					getVertexLocationAt( GLuint index );

	/**
	 * Sets the location element at the specified index in the vertex content to the specified value.
	 * 
	 * The index refers to vertices, not bytes, and is relative to the content of this particle,
	 * not the entire underlying mesh. The implementation takes into consideration whether the
	 * vertex content is interleaved to access the correct vertex data component.
	 *
	 * This implementation takes into consideration the dimensionality of the underlying vertex content.
	 * If the dimensionality is 2, the Z component of the specified vector will be ignored. If the
	 * dimensionality is 4, the specified vector will be converted to a 4D vector, with the W component
	 * set to one, before storing.
	 */
	void						setVertexLocation( const CC3Vector& aLocation, GLuint index );

	/**
	 * Returns the location element at the specified index in the underlying vertex content,
	 * as a four-dimensional location in the 4D homogeneous coordinate space.
	 *
	 * The index refers to vertices, not bytes, and is relative to the content of this particle,
	 * not the entire underlying mesh. The implementation takes into consideration whether the
	 * vertex content is interleaved to access the correct vertex data component.
	 *
	 * This implementation takes into consideration the dimensionality of the underlying vertex
	 * content. If the dimensionality is 3, the returned vector will contain one in the W component.
	 * If the dimensionality is 2, the returned vector will contain zero in the Z component and one
	 * in the W component.
	 */
	CC3Vector4					getVertexHomogeneousLocationAt( GLuint index );

	/**
	 * Sets the location element at the specified index in the underlying vertex content
	 * to the specified four-dimensional location in the 4D homogeneous coordinate space.
	 * 
	 * The index refers to vertices, not bytes, and is relative to the content of this particle,
	 * not the entire underlying mesh. The implementation takes into consideration whether the
	 * vertex content is interleaved to access the correct vertex data component.
	 *
	 * This implementation takes into consideration the dimensionality of the underlying vertex content.
	 * If the dimensionality is 3, the W component of the specified vector will be ignored. If the
	 * dimensionality is 2, both the W and Z components of the specified vector will be ignored.
	 */
	void						setVertexHomogeneousLocation( const CC3Vector4& aLocation, GLuint index );

	/**
	 * Returns the normal element at the specified index from the vertex content.
	 *
	 * The index refers to vertices, not bytes, and is relative to the content of this particle,
	 * not the entire underlying mesh. The implementation takes into consideration whether the
	 * vertex content is interleaved to access the correct vertex data component.
	 */
	CC3Vector					getVertexNormalAt( GLuint index );

	/**
	 * Sets the normal element at the specified index in the vertex content to the specified value.
	 * 
	 * The index refers to vertices, not bytes, and is relative to the content of this particle,
	 * not the entire underlying mesh. The implementation takes into consideration whether the
	 * vertex content is interleaved to access the correct vertex data component.
	 */
	void						setVertexNormal( const CC3Vector& aNormal, GLuint index );

	/**
	 * Returns the color element at the specified index from the vertex content.
	 *
	 * The index refers to vertices, not bytes, and is relative to the content of this particle,
	 * not the entire underlying mesh. The implementation takes into consideration whether the
	 * vertex content is interleaved to access the correct vertex data component.
	 */
	ccColor4F					getVertexColor4FAt( GLuint index );

	/**
	 * Sets the color element at the specified index in the vertex content to the specified value.
	 * 
	 * The index refers to vertices, not bytes, and is relative to the content of this particle,
	 * not the entire underlying mesh. The implementation takes into consideration whether the
	 * vertex content is interleaved to access the correct vertex data component.
	 */
	void						setVertexColor4F( const ccColor4F& aColor, GLuint index );

	/**
	 * Returns the color element at the specified index from the vertex content.
	 *
	 * The index refers to vertices, not bytes, and is relative to the content of this particle,
	 * not the entire underlying mesh. The implementation takes into consideration whether the
	 * vertex content is interleaved to access the correct vertex data component.
	 */
	ccColor4B					getVertexColor4BAt( GLuint index );

	/**
	 * Sets the color element at the specified index in the vertex content to the specified value.
	 * 
	 * The index refers to vertices, not bytes, and is relative to the content of this particle,
	 * not the entire underlying mesh. The implementation takes into consideration whether the
	 * vertex content is interleaved to access the correct vertex data component.
	 */
	void						setVertexColor4B( const ccColor4B& aColor, GLuint index );

	/**
	 * Returns the texture coordinate element at the specified index from the vertex content
	 * at the specified texture unit index.
	 *
	 * The index refers to vertices, not bytes, and is relative to the content of this particle,
	 * not the entire underlying mesh. The implementation takes into consideration whether the
	 * vertex content is interleaved to access the correct vertex data component.
	 */
	ccTex2F						getVertexTexCoord2FForTextureUnit( GLuint texUnit, GLuint index );

	/**
	 * Sets the texture coordinate element at the specified index in the vertex content,
	 * at the specified texture unit index, to the specified texture coordinate value.
	 * 
	 * The index refers to vertices, not bytes, and is relative to the content of this particle,
	 * not the entire underlying mesh. The implementation takes into consideration whether the
	 * vertex content is interleaved to access the correct vertex data component.
	 */
	void						setVertexTexCoord2F( const ccTex2F& aTex2F, GLuint texUnit, GLuint index );

	/**
	 * Returns the texture coordinate element at the specified index from the vertex content
	 * at the commonly used texture unit zero.
	 *
	 * This is a convenience method that is equivalent to invoking the
	 * vertexTexCoord2FForTextureUnit:at: method, with zero as the texture unit index.
	 *
	 * The index refers to vertices, not bytes, and is relative to the content of this particle,
	 * not the entire underlying mesh. The implementation takes into consideration whether the
	 * vertex content is interleaved to access the correct vertex data component.
	 */
	ccTex2F						getVertexTexCoord2FAt( GLuint index );

	/**
	 * Sets the texture coordinate element at the specified index in the vertex content,
	 * at the commonly used texture unit zero, to the specified texture coordinate value.
	 *
	 * This is a convenience method that delegates to the setVertexTexCoord2F:forTextureUnit:at:
	 * method, passing in zero for the texture unit index.
	 *
	 * The index refers to vertices, not bytes, and is relative to the content of this particle,
	 * not the entire underlying mesh. The implementation takes into consideration whether the
	 * vertex content is interleaved to access the correct vertex data component.
	 */
	void						setVertexTexCoord2F( const ccTex2F& aTex2F, GLuint index );

	/**
	 * Returns the index element at the specified index from the vertex content.
	 *
	 * The index refers to vertices, not bytes, and is relative to the content of this particle,
	 * not the entire underlying mesh.
	 *
	 * Similarly, the returned vertex index is relative to the content of this particle, not the
	 * entire underlying mesh.
	 */
	GLuint						getVertexIndexAt( GLuint index );

	/**
	 * Sets the index element at the specified index in the vertex content to the specified value.
	 * 
	 * The index refers to vertices, not bytes, and is relative to the content of this particle,
	 * not the entire underlying mesh. Similarly, the vertexIndex is relative to the content of
	 * this particle, not the entire underlying mesh.
	 */
	void						setVertexIndex( GLuint vertexIndex, GLuint index );

	/** Indicates whether this particle contains vertex location content. */
	bool						hasVertexLocations();

	/** Indicates whether this particle contains vertex normal content. */
	bool						hasVertexNormals();

	/** Indicates whether this particle contains vertex color content. */
	bool						hasVertexColors();

	/** Indicates whether this particle contains vertex texture coordinate content. */
	bool						hasVertexTextureCoordinates();

	/**
	 * Indicates whether any of the transform properties, location, rotation, or scale
	 * have been changed, and so the vertices of this particle need to be transformed.
	 *
	 * This property is automatically set to YES when one of those properties have been
	 * changed, and is reset to NO once the particle vertices have been transformed.
	 *
	 * Transformation of the vertices occurs automatically when the emitter is transformed.
	 */
	bool						isTransformDirty();

	/**
	 * Indicates that the particle has been transformed in some way, and that the vertices need to be
	 * transformed accordingly prior to the next drawing frame.
	 *
	 * This method is invoked automatically whenever any of the transform properties of this particle
	 * (location, rotation (including any type of rotation), or scale) has been changed. Usually, the
	 * application never needs to invoke this method directly.
	 */
	void						markTransformDirty();

	/**
	 * Transforms the vertices of this particle.
	 *
	 * For an  emitter, all particles are submitted to the GL engine in a single draw call.
	 * This means that all vertices for all particles from that emitter will use the same GL
	 * transform matrix, which is defined by the transformation properties of the emitter.
	 *
	 * To allow each mesh particle to be transformed independently, the vertices for each particle
	 * must be transformed in memory by the CPU.
	 *
	 * This method is invoked automatically on each particle when the emitter is transformed.
	 *
	 * This implementation checks the isTransformDirty property to see if this particle has been moved,
	 * rotated, or scaled since the previous transform. If so, this method traverses the vertices in
	 * this particle, transforming each vertex according to the transformation properties (location,
	 * rotation and scale) of this particle.
	 *
	 * If neither rotation or scaling has been applied to this particle, this implementation performs
	 * an optimized translation of the vertex locations only. If rotation has been applied to this
	 * particle as well, then the locations of the vertices are generally transformed using the location,
	 * rotation and scaling transformations that have been applied to this particle, and the normals of
	 * the vertices are rotated using the rotation applied to this particle.
	 *
	 * After the vertex locations have been transformed, this method also invokes the transformVertexColors
	 * method to update the colors of the individual vertices of this particle if individual particle and
	 * vertex colors are supported by the emitter.
	 *
	 * If your particles have specialized transformation requirements, or can be optimized in some 
	 * other way, you can override this method to transform the vertices of this particle differently.
	 * You should check the value of the isTransformDirty property before doing any work, and you should
	 * set the value of that property to NO after the vertices have been transformed.
	 *
	 * This method is invoked automatically on any particle that has been updated, when the emitter
	 * is transformed. Usually the application never needs to invoke this method directly.
	 */
	void						transformVertices();

	/**
	 * Indicates whether the color of this particle has been changed, and so the vertices of this
	 * particle need to be updated with the new color.
	 *
	 * This property is automatically set to YES when any of the color properties (color, opacity, color4f,
	 * color4B) has been changed, and is reset to NO once the particle vertices have been transformed.
	 *
	 * Transformation of the vertices occurs automatically when the emitter is transformed.
	 */
	bool						isColorDirty();

	/**
	 * Indicates that the color of the particle has been changed, and that the vertices need to be
	 * transformed accordingly prior to the next drawing frame.
	 *
	 * This method is invoked automatically whenever any of the color properties (color, opacity, color4f,
	 * color4B) has been changed. Usually, the application never needs to invoke this method directly.
	 */
	void						markColorDirty();

	/**
	 * Template method that transforms the color of each of the vertices of this particle.
	 *
	 * This implementation checks the isColorDirty property to determine if this particle has been colored
	 * since the previous transform. If so, this method traverses the vertices in this particle, copying
	 * the color into each vertex.
	 *
	 * This method is invoked automatically from the transformVertices method of any particle whose color
	 * has been updated by setting one of the color properties (color, opacity, color4f, color4B).
	 */
	void						transformVertexColors();

	CC3Mesh*					getMesh();

	bool						isAlive();
	void						setIsAlive( bool alive );

	GLuint						getFirstVertexOffset();
	void						setFirstVertexOffset( GLuint vtxOffset );

	GLuint						getVertexCount();
	CCRange						getVertexRange();

	GLuint						getFirstVertexIndexOffset();	
	void						setFirstVertexIndexOffset( GLuint vtxIdxOffset );

	GLuint						getVertexIndexCount();
	CCRange						getVertexIndexRange();

	bool						hasVertexIndices();
	bool						shouldTrackTarget();

	/**
	 * Returns the rotator property, cast as a CC3MutableRotator.
	 *
	 * If the rotator is not already a CC3MutableRotator, a new CC3MutableRotator
	 * is created and its state is copied from the current rotator.
	 *
	 * This design allows particles that do not require rotation to use the empty and smaller
	 * CC3Rotator instance, but allows an automatic upgrade to a mutable rotator
	 * when the node needs to make changes to the rotational properties.
	 *
	 * This property should only be accessed if the intention is to swap the existing
	 * rotator with a directional rotator.
	 */
	CC3MutableRotator*			getMutableRotator();

	/**
	 * Returns the rotator property, cast as a CC3DirectionalRotator.
	 *
	 * If the rotator is not already a CC3DirectionalRotator, a new CC3DirectionalRotator
	 * is created and its state is copied from the current rotator.
	 *
	 * This design allows most particles to use a simpler and smaller CC3Rotator instance,
	 * but allow an automatic upgrade to a larger and more complex directional rotator
	 * when the node needs to make use of pointing or tracking functionality.
	 *
	 * This implementation returns a reversing directional rotator class that orients
	 * the positive-Z axis of the node along the forwardDirection.
	 *
	 * This property should only be accessed if the intention is to swap the existing
	 * rotator with a directional rotator.
	 */
	CC3DirectionalRotator*		getDirectionalRotator();

	/**
	 * Returns the rotator property, cast as a CC3TargettingRotator.
	 *
	 * If the rotator is not already a CC3TargettingRotator, a new CC3TargettingRotator
	 * is created and its state is copied from the current rotator.
	 *
	 * This design allows most particles to use a simpler and smaller CC3Rotator instance,
	 * but allow an automatic upgrade to a larger and more complex directional rotator
	 * when the node needs to make use of pointing or tracking functionality.
	 *
	 * This implementation returns a reversing directional rotator class that orients
	 * the positive-Z axis of the node along the forwardDirection.
	 *
	 * This property should only be accessed if the intention is to swap the existing
	 * rotator with a directional rotator.
	 */
	CC3TargettingRotator*		getTargettingRotator();

	void						setRotationAngle( GLfloat anAngle );
	ccColor4F					getColor4F();
	void						setColor4F( const ccColor4F& aColor );
	ccColor4B					getColor4B();
	void						setColor4B( const ccColor4B& aColor );

	/** Returns whether the mesh vertices can be transformed using only translation. */
	bool						doesUseTranslationOnly();

	void						translateVertices();
	/**
	 * Transform the vertices using translation, rotation and scaling, by allocating a transform matrix
	 * and transforming it in place using the location, rotator, and scale properties of this particle.
	 */
	void						fullyTransformVertices();

	/** Apply the location, rotation and scaling transforms to the specified matrix data. */
	virtual void				applyLocalTransformsTo( CC3Matrix4x3* mtx );

	/**
	 * Template method that prepares the specified matrix to be transformed by the transform
	 * properties of this particle. This implementation starts the matrix as an identity matrix.
	 */
	virtual void				prepareForTransform( CC3Matrix4x3* mtx );

	/** Template method that applies the local location property to the specified matrix. */
	virtual void				applyTranslationTo( CC3Matrix4x3* mtx );

	/** Template method that applies the rotation in the rotator to the specified transform matrix. */
	virtual void				applyRotationTo( CC3Matrix4x3* mtx );

	virtual void				populateFrom( CC3MeshParticle* another );
	virtual CCObject*			copyWithZone( CCZone* zone );

	virtual bool				init();

	static CC3MeshParticle*		particle();

	virtual void				setTemplateMesh( CC3Mesh* templateMesh );
	virtual CC3Mesh*			getTemplateMesh();

protected:
	CC3Rotator*					m_pRotator;
	CC3Mesh*					m_pTemplateMesh;
	CC3Vector					m_location;
	GLuint						m_firstVertexOffset;
	GLuint						m_firstVertexIndexOffset;
	bool						m_isAlive : 1;
	bool						m_isTransformDirty : 1;
	bool						m_isColorDirty : 1;
};

/** 
 * CC3ScalableMeshParticle is a type of CC3MeshParticle that can be scaled.
 *
 * This class is distinct from CC3MeshParticle so that mesh particle that do not require
 * scaling do not have to carry storage for scaling information.
 */
class CC3ScalableMeshParticle : public CC3MeshParticle
{
	DECLARE_SUPER( CC3MeshParticle );
public:
	/**
	 * The scale of the particle in each dimension, relative to the emitter.
	 *
	 * Unless non-uniform scaling is needed, it is recommended that you use the uniformScale property instead.
	 */
	CC3Vector					getScale();
	void						setScale( const CC3Vector& scale );

	/**
	 * The scale of the particle, uniform in each dimension, relative to the emitter.
	 *
	 * Unless non-uniform scaling is needed, it is recommended that you use this property instead
	 * of the scale property.
	 *
	 * If non-uniform scaling is applied via the scale property, this uniformScale property will
	 * return the length of the scale property vector divided by the length of a unit cube (sqrt(3.0)),
	 * as an approximation of the overall scaling condensed to a single scalar value.
	 */
	GLfloat						getUniformScale();
	void						setUniformScale( GLfloat uniformScale );

	/** Indicates whether current local scaling (via the scale property) is uniform along all axes. */
	bool						isUniformlyScaledLocally();

	/**
	 * Returns whether the current transform applied to this particle is rigid.
	 *
	 * A rigid transform contains only rotation and translation transformations and does not include
	 * any scaling transformation. For the transform to be rigid, this particle must have unity scaling.
	 */
	bool						isTransformRigid();

	/** Returns whether the mesh vertices can be transformed using only translation. */
	bool						doesUseTranslationOnly();

	/** Invoke super, then apply the scaling transforms to the specified matrix data. */
	void						applyLocalTransformsTo( CC3Matrix4x3* mtx );

	/** Template method that applies the local scale property to the specified transform matrix. */
	void						applyScalingTo( CC3Matrix4x3* mtx );

	bool						init();

	void						populateFrom( CC3ScalableMeshParticle* another );
	virtual CCObject*			copyWithZone( CCZone* zone );

protected:
	CC3Vector					m_scale;
};

NS_COCOS3D_END

#endif
