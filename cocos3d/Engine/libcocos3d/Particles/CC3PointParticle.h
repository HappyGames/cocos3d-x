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
#ifndef _CC3_POINT_PARTICLE_H_
#define _CC3_POINT_PARTICLE_H_

NS_COCOS3D_BEGIN


/** Default size for particles. */
static const GLfloat kCC3DefaultParticleSize = 32.0;

/** Constant used with the particleSizeMinimum property to indicate no minimum size for particles. */
static const GLfloat kCC3ParticleSizeMinimumNone = 1.0;

/**
 * Constant used with the particleSizeMaximum property to indicate no maximum size
 * for particles, beyond any platform limit.
 */
static const GLfloat kCC3ParticleSizeMaximumNone = kCC3MaxGLfloat;


/**
 * CC3PointParticle is a standard base implementation of the CC3PointParticleProtocol.
 *
 * CC3PointParticle provides accessors for the particle normal and size.
 */
class CC3PointParticle : public CC3CommonVertexArrayParticle
{
	DECLARE_SUPER( CC3CommonVertexArrayParticle );
public:
	virtual void				setEmitter( CC3ParticleEmitter* emitter );

	/**
	 * If this particle has vertex normal content, (which can be checked with the hasNormal property),
	 * this property indicates the vertex normal that the particle uses to interact with light sources.
	 * 
	 * This property is automatically and dynamically adjusted by the emitter, based on the particle's
	 * orientation with respect to the camera. Unless you have specific reason to change this property,
	 * and know what you are doing, you should leave the value of this property alone.
	 *
	 * If this particle does not have vertex normal content, this property will always return kCC3VectorZero.
	 * In this condition, it is safe to set this property, but changes will have no effect.
	 */
	CC3Vector					getNormal();
	void						setNormal( const CC3Vector& normal );

	/**
	 * Indicates whether this particle has vertex normal content, as determined by the vertexContentTypes
	 * property of the emitter. Within an emitter, either all particles have normal content, or none do.
	 *
	 * When this property returns YES, each particle will have a normal vector and will interact with
	 * light sources. When this property returns NO, each particle will ignore lighting conditions.
	 */
	bool						hasNormal();

	/**
	 * If this particle has individual size content, (which can be checked with the hasSize property),
	 * this property indicates the size at which this particle will appear.
	 *
	 * If this particle has individual size content, you can set this property at any time to define
	 * the size of the particle.
	 * 
	 * If this particle does not have individual size content, this property will always return the
	 * value of the particleSize property of the emitter. In this condition, it is safe to set this
	 * property, but changes will have no effect.
	 */
	GLfloat						getSize();
	void						setSize( GLfloat size );

	/**
	 * Indicates whether this particle has vertex size content, as determined by the vertexContentTypes
	 * property of the emitter. Within an emitter, either all particles have size content, or none do.
	 *
	 * When this property returns YES, each particle can be set to a different size. When this property
	 * returns NO, all particles will have the size specified by the particleSize property of the emitter.
	 */
	bool						hasSize();

	bool						isAlive();
	void						setIsAlive( bool alive );

	GLuint						getParticleIndex();
	/** Overridden to update the underlying vertex indices array, if it exists. */
	void						setParticleIndex( GLuint anIndex );

	GLuint						getVertexCount();
	CCRange						getVertexRange();
	GLuint						getVertexIndexCount();
	CCRange						getVertexIndexRange();
	
	CC3Vector					getLocation();
	void						setLocation( const CC3Vector& aLocation );

	ccColor4F					getColor4F();
	void						setColor4F( const ccColor4F& aColor );
	ccColor4B					getColor4B();
	void						setColor4B( const ccColor4B& aColor );

	bool						hasVertexIndices();
	std::string					fullDescription();
	void						pointNormalAt( const CC3Vector& camLoc );

	bool						init();
	void						populateFrom( CC3PointParticle* another );
	virtual CCObject*			copyWithZone( CCZone* zone );

	static CC3PointParticle*	particle();
	
protected:
	GLuint						m_particleIndex;
	bool						m_isAlive : 1;
};

NS_COCOS3D_END

#endif
