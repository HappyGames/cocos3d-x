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
#ifndef _CC3_COMMON_VERTEX_ARRAY_PARTICLE_H_
#define _CC3_COMMON_VERTEX_ARRAY_PARTICLE_H_

NS_COCOS3D_BEGIN

/**
 * CC3CommonVertexArrayParticleProtocol defines the requirements for particles that are emitted
 * and managed by the CC3CommonVertexArrayParticleEmitter class.
 *
 * A CC3CommonVertexArrayParticleEmitter maintains the vertices of all particles in common
 * vertex arrays.
 */
class CC3CommonVertexArrayParticleProtocol/* : public CC3ParticleProtocol*/
{
public:
	/** Returns the number of vertices in this particle. */
	virtual GLuint				getVertexCount() = 0;

	/**
	 * Returns the range of vertices in the underlying mesh that are managed by this particle.
	 *
	 * The location element of the returned range structure contains the index to the first vertex
	 * of this particle, and the length element contains the same value as the vertexCount property.
	 */
	virtual CCRange				getVertexRange() = 0;

	/**
	 * Returns the number of vertex indices required for the mesh of this particle.
	 *
	 * Not all meshes use vertex indices. If indexed drawing is used by this particle, this method returns
	 * the number of vertex indices in the particle. If indexed drawing is not used by this particle, this
	 * property returns the same value as the vertexCount property, indicating, in effect, the number of
	 * indices that would be required if this particle was converted to using indexed drawing.
	 *
	 * This behaviour allows a particle that does not use indexed drawing to be added to an emitter that
	 * does use indexed drawing. When this happens, the missing vertex indices are automatically synthesized.
	 */
	virtual GLuint				getVertexIndexCount() = 0;

	/**
	 * Returns the range of vertex indices in the underlying mesh that are managed by this particle.
	 *
	 * The location element of the returned range structure contains the index to the first vertex index
	 * of this particle, and the length element contains the same value as the vertexIndexCount property.
	 *
	 * Not all meshes use vertex indices. If indexed drawing is used by this particle, this method returns
	 * the range of vertex indices in the particle. If indexed drawing is not used by this particle, this
	 * property returns the same value as the vertexRange property, indicating, in effect, the range of
	 * indices that would be required if this particle was converted to using indexed drawing.
	 *
	 * This behaviour allows a particle that does not use indexed drawing to be added to an emitter that
	 * does use indexed drawing. When this happens, the missing vertex indices are automatically synthesized.
	 */
	virtual CCRange				getVertexIndexRange() = 0;

	/** Returns whether this particle uses indexed vertices. */
	virtual bool				hasVertexIndices() = 0;
};

class CC3CommonVertexArrayParticle : public CC3Particle, public CC3CommonVertexArrayParticleProtocol
{
	DECLARE_SUPER( CC3Particle );
public:
	virtual GLuint				getVertexCount();
	virtual CCRange				getVertexRange();
	virtual GLuint				getVertexIndexCount();
	virtual CCRange				getVertexIndexRange();
	virtual bool				hasVertexIndices();
};

NS_COCOS3D_END

#endif
