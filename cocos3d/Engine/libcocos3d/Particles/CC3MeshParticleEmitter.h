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
#ifndef _CC3_MESH_PARTICLES_H_
#define _CC3_MESH_PARTICLES_H_

NS_COCOS3D_BEGIN

class CC3MeshParticle;
/**
 * CC3MeshParticleEmitter emits particles that conform to the CC3MeshParticleProtocol protocol.
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
 * Mesh particles are transformed by this emitter through the transformVertices method on the particle.
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
 * Each mesh particle added to or emitted by this mesh emitter uses a CC3Mesh as a
 * template. For particles created by the application outside the emitter, and added to the emitter
 * with the emitParticle: method, the application can directly set the templateMesh property of the
 * mesh particle prior to invoking the emitParticle: method of this emitter. With this technique,
 * each particle can use a different mesh template, and so each paticle can be a different shape.
 * If the templateMesh property of a particle submitted to the emitParticle: method is nil, this
 * emitter will assign the template mesh in the particleTemplateMesh property to the particle.
 *
 * For particles created within the emitter, and emitted automatically, or via the emitParticle
 * method, each particle will be assigned the template mesh specified in the particleTemplateMesh
 * property of this emitter. In this scenario, each particle will be the same shape. Subclasses,
 * such as CC3MultiTemplateMeshParticleEmitter, can extend this functionality to allow particles
 * that are automatically emitted to be assigned a template mesh that is randomly selected from
 * a collection of template meshes.
 *
 * Because all particles managed by this emitter are drawn with a single GL draw call, all
 * particles added to or emitted by this emitter are covered by the same material and texture.
 *
 * However, you can assign a different color to each particle by configuring this emitter to
 * use vertex color content by including the kCC3VertexContentColor component when setting the
 * vertexContentTypes property of this emitter.
 *
 * Similarly, if the vertexContentTypes property of this emitter includes the
 * kCC3VertexContentTextureCoordinates component, then the particles will be covered by a
 * texture. By assigning the texture coordinates of each particle to different sections of
 * the texture assigned to this emitter, each particle can effectively be textured separately.
 *
 * All memory used by the particles and the underlying vertex mesh is managed by the
 * emitter node, and is deallocated automatically when the emitter is released.
 */
class CC3MeshParticleEmitter : public CC3CommonVertexArrayParticleEmitter 
{
	DECLARE_SUPER( CC3CommonVertexArrayParticleEmitter );
public:
	CC3MeshParticleEmitter();
	~CC3MeshParticleEmitter();
	/**
	 * The mesh used as a template for the mesh of each particle emitted automatically by this emitter.
	 * Each particle created within the emitter, and emitted automatically, or via the emitParticle
	 * method, will be assigned the template mesh specified in this property.
	 *
	 * When a particle is created by the application outside the emitter, and submitted to the emitter
	 * via the emitParticle: method, the application can assign a different template mesh to it via
	 * the templateMesh property of the particle, before invoking the emitParticle: method.
	 *
	 * However, when using the emitParticle: method, the application does not have to assign a template mesh
	 * directly. If the templateMesh property of a particle submitted to the emitParticle: method is nil,
	 * this emitter will assign the template mesh in this particleTemplateMesh property to the particle.
	 *
	 * Each particle emitted by this emitter uses the vertices of this mesh as a starting point,
	 * however, each particle has access to its own copy of its mesh vertices. In this way, different
	 * particles can have different locations, rotations, vertex colors and texture coordinates.
	 *
	 * If the value of the vertexContentTypes property of this emitter have not yet been set, that
	 * property is set to the value of the vertexContentTypes property of the specified particle
	 * template mesh. Therefore, by default, the particles of this emitter will contain the same
	 * vertex content types as this template mesh.
	 *
	 * This emitter can be configured with vertex content types that are different than the template
	 * mesh, by setting the value of the vertexContentTypes property of this emitter explicitly.
	 * When vertex content is copied from the template mesh to a particle, vertex content types
	 * that do not appear in this mesh will be ignored, and particle content for content types not
	 * available in the template mesh will be given default values, and can be set during initializaton
	 * of each particle. For example, if the particle template mesh does not contain individual vertex
	 * color information, you can still define color as vertex content type for this emitter,
	 * and set the color of each particle when it is initialized.
	 *
	 * This property must be set prior to this emitter emitting any particles. It is possible to
	 * change the value of this property during emission.
	 */
	CC3Mesh*					getParticleTemplateMesh();
	void						setParticleTemplateMesh( CC3Mesh* mesh );

	/**
	 * Configures this emitter to emit particles as defined by the specified template mesh node.
	 *
	 * This is a convenience method that simply sets the particleTemplateMesh and material 
	 * properties of this emitter (including the texture) from the corresponding properties
	 * of the specified mesh node.
	 *
	 * When these properties are set, the template mesh is simply retained, but the template 
	 * material is copied, so that the material of the emitter can be configured independently
	 * from that of the template mesh node.
	 */
	CC3MeshNode*				getParticleTemplate();
	void						setParticleTemplate( CC3MeshNode* aParticleTemplate );

	/**
	 * Template method that sets the templateMesh property of the specified particle.
	 *
	 * This implementation sets the particle's templateMesh property to the mesh in the
	 * particleTemplateMesh property of this emitter.
	 *
	 * Subclasses may override this implementation to create some other selection and assignment methodology.
	 *
	 * This method is invoked automatically when a particle is emitted, or the emitParticle: method is
	 * invoked with a particle that does not already have a templateMesh. The application should never
	 * need to invoke this method directly.
	 */
	virtual void				assignTemplateMeshToParticle( CC3MeshParticle* aParticle );
	 
	/**
	 * Emits a single particle of the type specified in the particleClass property.
	 *
	 * Refer the the documentation of this method in the parent CC3ParticleEmitter class for a complete
	 * description of the emission process.
	 *
	 * The emitted particle will be assigned the template mesh defined in the particleTemplateMesh property.
	 */
	CC3Particle*				emitParticle();

	/**
	 * Adds the specified particle to the emitter and emits it.
	 *
	 * Refer the the documentation of this method in the parent CC3ParticleEmitter class for a complete
	 * description of the emission process.
	 *
	 * If the specified particle does not have a templateMesh, it will be assigned the template mesh
	 * defined in the particleTemplateMesh property of this emitter.
	 */
	bool						emitParticle( CC3Particle* aParticle );

	/**
	 * Returns a particle suitable for emission by this emitter. The returned particle can subsequently
	 * be emitted from this emitter using the emitParticle: method.
	 *
	 * The particle emitted may be an existing expired particle that is being reused, or it may be a
	 * newly instantiated particle. If an expired particle is available within this emitter, it will
	 * be reused. If not, this method invokes the makeParticle method to create a new particle.
	 *
	 * The returned particle will be assigned the template mesh defined in the particleTemplateMesh property.
	 * If the particle is being reused and originally had a different particle mesh, the template mesh of
	 * the reused particle is replaced.
	 * 
	 * You can also use the makeParticle method directly to ensure that a new particle has been created.
	 */
	CC3Particle*				acquireParticle();

	/**
	 * Creates a new autoreleased instance of a particle of the type specified by the particleClass property.
	 * The returned particle can subsequently be emitted from this emitter using the emitParticle: method.
	 *
	 * The returned particle will be assigned the template mesh defined in the particleTemplateMesh property.
	 *
	 * Distinct from the acquireParticle method, this method bypasses the reuse of expired particles
	 * and always creates a new autoreleased particle instance.
	 */
	CC3Particle*				makeParticle();

	/** Returns the particle at the specified index within the particles array, cast as a mesh particle. */
	CC3MeshParticle*			getMeshParticleAt( GLuint aParticleIndex );

	/**
	 * Indicates whether particles should be transformed when the emitter is not within view of the camera.
	 *
	 * As particles move and rotate, their vertices are transformed. This can consume significant
	 * processing when the number of particles and the complexity of the particle meshes is large.
	 *
	 * Setting this property to NO will stop the particles from being transformed when the bounding
	 * volume of the emitter does not intersect the frustum of the camera, improving application
	 * performance when the particles are offscreen. Particles will still be updated, but their
	 * vertices will not be transformed until the emitter comes within the view of the camera.
	 *
	 * Care should be taken when setting this property to NO, because the bounding volume of an emitter
	 * is calculated from the current vertices of the particles. When the particles stop being transformed,
	 * under the action of this property, the bounding volume will stop being updated. Since the particles
	 * are still updated even when not visible, this can cause a jarring visual effect when the emitter
	 * comes back into view of the camera and the particles may seem to jump unexpectedly into space that
	 * was not part of the emitter bounding volume when it previously stopped being updated. If such an
	 * effect occurs and is undesirable, this property can be left set to YES so that particles will
	 * continue to be transformed even when outside the view of the camera.
	 *
	 * When the bounding volume of the emitter is fixed, as indicated by the shouldUseFixedBoundingVolume
	 * property, it is assumed that the bounding volume has already been sized to encompass all possible
	 * paths of the particles. As a result, when this emitter uses a fixed bounding volume, the particles
	 * are never transformed when that bounding volume is outside the view of the camera. Therefore, when
	 * a fixed bounding volume is used (the shouldUseFixedBoundingVolume is set to YES), the value of
	 * this property has no effect.
	 *
	 * The initial value of this property is YES, indicating that particles will be transformed even
	 * when not visible to the camera, unless the shouldUseFixedBoundingVolume property is also set
	 * to YES, indicating that emitter has a fixed bounding volume that encompasses all particles.
	 */
	bool						shouldTransformUnseenParticles();
	void						setShouldTransformUnseenParticles( bool transformUnseenParticles );

	/**
	 * Indicates whether any of the transform properties on any of the particles have been changed,
	 * and so the vertices of the particle need to be transformed.
	 *
	 * This property is automatically set to YES when one of those properties have been changed on
	 * any of the particles and is reset to NO once the particles have been transformed.
	 *
	 * Transformation of the particles occurs automatically when the emitter is transformed.
	 */
	bool						isParticleTransformDirty();

	/**
	 * Indicates that a particle has been transformed in some way, and that the vertices need to be
	 * transformed accordingly prior to the next drawing frame.
	 *
	 * This method is invoked automatically whenever any of the transform properties of any particle
	 * (location, rotation (including any type of rotation), or scale) are changed. Usually, the
	 * application never needs to invoke this method directly.
	 */
	void						markParticleTransformDirty();

	void						initWithTag( GLuint aTag, const std::string& aName );

	void						populateFrom( CC3MeshParticleEmitter* another );
	virtual CCObject*			copyWithZone( CCZone* zone );

	void						copyTemplateContentToParticle( CC3MeshParticle* aParticle );
	void						initializeParticle( CC3Particle* aParticle );
	/** If the particles need to be transformed, do so before updating the particle mesh. */
	void						updateParticleMeshWithVisitor( CC3NodeUpdatingVisitor* visitor );
	/**
	 * Template method that returns whether the particles should be tranformed.
	 *
	 * Particles are only transformed if they are dirty and either visible or touchable.
	 * If the bounding volume is fixed, and the emitter is not in the camera's field of view,
	 * then the particles are not transformed.
	 * 
	 * Subclasses may override this methods to change how this decision is made.
	 */
	virtual bool				shouldTransformParticles( CC3NodeUpdatingVisitor* visitor );
	virtual void				transformParticles();
	/**
	 * Removes the current particle from the active particles, but possibly keep it cached for future use.
	 *
	 * If the particle being removed has the same number of vertices and vertex indices as the last living
	 * particle, swap the particle being removed with that last living particle. To do this, swap the
	 * particles in the particles collection, and copy the vertex content and indices from the last living
	 * particle into the slot of the particle being removed.
	 *
	 * If the particle being removed does not have the same number of vertices and vertex indices as the
	 * last living particle, we can't swap them. The particle must be removed, and the all of the vertex
	 * content for all following particles must be copied down to fill in the gap left by the removed
	 * particle. The vertex indices must also be copied down to fill in the gap and, in addition, must
	 * be adjusted to point to the newly moved vertex content.
	 */
	virtual void				removeParticle( CC3Particle* aParticle, GLuint anIndex );

	/** Overridden so that the transform is considered dirty if any of the particles need to be transformed. */
	virtual bool				isTransformDirty();

	static CC3MeshParticleEmitter*	nodeWithName( const std::string& aName );

protected:
	CC3Mesh*					_particleTemplateMesh;
	bool						_isParticleTransformDirty : 1;
	bool						_shouldTransformUnseenParticles : 1;
};

NS_COCOS3D_END

#endif
