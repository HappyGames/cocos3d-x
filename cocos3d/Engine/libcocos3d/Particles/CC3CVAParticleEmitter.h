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
#ifndef _CC3_COMMON_VERTEX_ARRAY_PARTICLE_EMITTER_H_
#define _CC3_COMMON_VERTEX_ARRAY_PARTICLE_EMITTER_H_

NS_COCOS3D_BEGIN

class CC3CommonVertexArrayParticle;
/**
 * A CC3CommonVertexArrayParticleEmitter maintains the vertices of all particles in common
 * vertex arrays.
 *
 * This class forms the basis of both point particle emitters and mesh particle emitters.
 */
class CC3CommonVertexArrayParticleEmitter : public CC3ParticleEmitter 
{
	DECLARE_SUPER( CC3ParticleEmitter );
public:
	void						initWithTag( GLuint aTag, const std::string& aName );
	
	void						populateFrom( CC3CommonVertexArrayParticleEmitter* another );
	virtual CCObject*			copyWithZone( CCZone* zone );

	std::string					fullDescription();
	// Overridden to retain all vertex content in memory and dynamically write to GL buffer.
	void						setVertexContentTypes( CC3VertexContent vtxContentTypes );
	void						setVertexLocation( const CC3Vector& aLocation, GLuint vtxIndex );

	void						setVertexHomogeneousLocation( const CC3Vector4& aLocation, GLuint vtxIndex );

	void						setVertexNormal( const CC3Vector& aNormal, GLuint vtxIndex );

	void						setVertexColor4F( const ccColor4F& aColor, GLuint vtxIndex );
	void						setVertexColor4B( const ccColor4B& aColor, GLuint vtxIndex );

	void						setVertexTexCoord2F( const ccTex2F& aTex2F, GLuint texUnit, GLuint vtxIndex );

	/** Ensures space has been allocated for the specified particle. */
	bool						ensureParticleCapacityFor( CC3Particle* aParticle );

	/** Clears the range of dirty vertices and vertex indices. */
	void						clearDirtyVertexRanges();

	/**
	 * Process the transform if the vertices have been changed at all,
	 * to ensure that particle vertices are transformed.
	 */
	bool						isTransformDirty();

	/** Updates the mesh vertex counts and marks the range of vertices that are affected by this particle. */
	void						acceptParticle( CC3Particle* aParticle );

	/**
	 * Adds the specified range to the range of dirty vertices.
	 * The result is to form a union of the specified range and the current range.
	 */
	void						addDirtyVertexRange( const CCRange& aRange );

	/**
	 * Adds the specified vertex to the range of dirty vertices.
	 * The result is to form a union of the specified vertex and the current range.
	 */
	void						addDirtyVertex( GLuint vtxIdx );

	/**
	 * Adds the specified range to the range of dirty vertex indices.
	 * The result is to form a union of the specified range and the current range.
	 */
	void						addDirtyVertexIndexRange( const CCRange& aRange );

	/**
	 * Adds the specified vertex index to the range of dirty vertex indices.
	 * The result is to form a union of the specified vertex index and the current range.
	 */
	void						addDirtyVertexIndex( GLuint vtxIdx );

	/** Returns whether any vertices are dirty, by being either expanded or changed. */
	bool						verticesAreDirty();

	/** Returns whether any vertex indices are dirty, by being either expanded or changed. */
	bool						vertexIndicesAreDirty();

	/** Updates the mesh after particles have been updated.  */
	void						processUpdateBeforeTransform( CC3NodeUpdatingVisitor* visitor );

	/**
	 * Updates the particle mesh by updating the particle count, copying the particle
	 * vertex data to the GL buffer, and updating the bounding volume of this node.
	 */
	void						updateParticleMeshWithVisitor( CC3NodeUpdatingVisitor* visitor );
	/**
	 * If the mesh is using GL VBO's, update them. If the mesh was expanded,
	 * recreate the VBO's, otherwise update them.
	 */
	void						updateParticleMeshGLBuffers();

	CC3CommonVertexArrayParticle* getCommonVertexArrayParticleAt( GLuint aParticleIndex );
	CC3Particle*				getParticleWithVertexAt( GLuint vtxIndex );
	CC3Particle*				getParticleWithVertexIndexAt( GLuint index );

	/** Shrinks the mesh vertex count by the vertex count of the particle. */
	void						removeParticle( CC3Particle* aParticle, GLuint anIndex );
	void						removeAllParticles();

protected:
	CCRange						_dirtyVertexRange;
	CCRange						_dirtyVertexIndexRange;
	bool						_wasVertexCapacityChanged;
};

NS_COCOS3D_END

#endif
