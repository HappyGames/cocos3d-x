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
#ifndef _CC3_PARTICLE_H_
#define _CC3_PARTICLE_H_

NS_COCOS3D_BEGIN

/**
 * Constant representing an infinite interval of time.
 *
 * This can be used with the emissionDuration and emissionInterval properties.
 */
static const GLfloat kCC3ParticleInfiniteInterval = kCC3MaxGLfloat;

/**
 * Constant representing an infinite rate of emission.
 *
 * This can be used with the emissionRate property, and indicates
 * that all particles should be emitted at once.
 */
static const GLfloat kCC3ParticleInfiniteEmissionRate = kCC3MaxGLfloat;

/**
 * Constant representing an unlimited number of particles.
 *
 * This can be used with the maximumParticleCapacity property, and indicates that there
 * is no pre-defined maximum limit to the number of particles that will be emitted.
 */
static const GLuint kCC3ParticlesNoMax = kCC3MaxGLuint;


class CC3ParticleEmitter;
class CC3NodeUpdatingVisitor;

/**
 * CC3Particle represents a single particle emitted by a CC3ParticleEmitter particle emitter.
 *
 * When creating a particle system, you write application-specific implementation of CC3Particle
 * to embody the state and life-cycle behaviour of each particle. You do not always need to create a
 * customized subclass of CC3ParticleEmitter.
 * 
 * To implement a specific particle system, create an implementation of CC3Particle, and override
 * the initializeParticle and updateBeforeTransform: methods (and possibly the updateAfterTransform:
 * method) to define the initial state, and life-cycle behaviour of the particle.
 *
 * Particles can be added to an emitter by the application directly, or can be created and emitted
 * from the emitter automatically, based on configuration within the emitter. In both cases, the
 * interaction on the particle is the same.
 *
 * When a particle starts its life, the emitter will automatically invoke the initializeParticle
 * method on the particle.
 *
 * Then, during the life-cycle of a particle, the emitter will periodically update the particle by
 * invoking the updateBeforeTransform: and updateAfterTransform: callback methods. These method
 * invocations include the time interval since the last update, so that the particle can emulate
 * realistic real-time behaviour.
 *
 * Be aware that, in the interests of performance and memory conservation, expired particles may be
 * cached and reused, and particle emission may not always involve instantiating a new instance of
 * your CC3Particle implementation class.
 *
 * With this in mind, you should not depend on init method being invoked during particle emission.
 * All code that establishes the initial emitted state of a particle should be included in the
 * initializeParticle method.
 *
 * From within the initializeParticle, updateBeforeTransform: and updateAfterTransform methods,
 * the particle has access to the emitter (and the node hierarchy and scene it sits in) through
 * the emitter property. In addition, the particle can read and manipulate its own drawable content. 
 *
 * Beyond these basic drawable content properties, when you create in implementation of
 * CC3Particle, you should add any other content that is needed to determine the behaviour
 * of your particle. For example, you might include a velocity property for particles that are
 * following a path (or even a path object to define that path more explicitly), and a timeToLive
 * property, for particles that have a finite lifespan. There are several protocol extensions,
 * such as CC3MortalParticleProtocol and CC3UniformlyMovingParticleProtocol that provide standard
 * definitions of basic additional functionality in this respect.
 *
 * It is up to the particle to determine when it expires. Some particles may never expire.
 * Others may keep track of their life or path and expire at a certain time or place.
 *
 * Once your custom particle has detemined that it has expired, in the updateBeforeTransform: or
 * updateAfterTransform method, you can set the isAlive property of the particle to NO. When either
 * of those methods returns, the emitter will then automatically remove the particle (and set it
 * aside for possible reuse). Expired particles are not drawn and do not receive further
 * updateBeforeTransform: or updateAfterTransform: callback method invocations.
 *
 * You can also set the isAlive property to NO in the initializeParticle method
 * to cause the emission of the particle to be aborted.
 */
class CC3Particle : public CCObject
{
public:
	CC3Particle();
	virtual ~CC3Particle();
	/**
	 * The location of this particle in the local coordinate system of the emitter.
	 *
	 * You can set this property in the initializeParticle and updateBeforeTransform: methods
	 * to move the particle around.
	 *
	 * The initial value of this property, set prior to the invocation of the 
	 * initializeParticle method, is kCC3VectorZero.
	 */
	virtual CC3Vector			getLocation();
	virtual void				setLocation( const CC3Vector& location );

	/**
	 * The location of the particle in 3D space, relative to the global origin.
	 *
	 * This is calculated by using the globalTransformMatrix of the emitter to transform
	 * the location of this particle.
	 */
	virtual CC3Vector			getGlobalLocation();

	/**
	 * If this particle has individual color content, (which can be checked with the hasColor
	 * property), this property indicates the color in which this particle will appear.
	 *
	 * If this particle has individual color content, you can set this property at any
	 * time to define the color of the entire particle.
	 *
	 * Particles are configured for individual color content by including the kCC3VertexContentColor
	 * component flag when setting the vertexContentTypes property of the emitter.
	 *
	 * Setting this property will set the color of all vertices in the particle to the assigned color.
	 *
	 * Reading this property returns the color value of the first vertex in the particle. If this
	 * particle does not support individual color content, this property will always return the value
	 * of the diffuseColor property of the emitter. In this condition, it is safe to set this property,
	 * but changes will have no effect.
	 */
	virtual ccColor4F			getColor4F();
	virtual void				setColor4F( const ccColor4F& color );

	/**
	 * If this particle has individual color content, (which can be checked with the hasColor
	 * property), this property indicates the color in which this particle will appear.
	 *
	 * If this particle has individual color content, you can set this property at any
	 * time to define the color of the entire particle.
	 *
	 * Particles are configured for individual color content by including the kCC3VertexContentColor
	 * component flag when setting the vertexContentTypes property of the emitter.
	 *
	 * Setting this property will set the color of all vertices in the particle to the assigned color.
	 * 
	 * Reading this property returns the color value of the first vertex in the particle. If this
	 * particle does not support individual color content, this property will always return the value
	 * of the diffuseColor property of the emitter. In this condition, it is safe to set this property,
	 * but changes will have no effect.
	 */
	virtual ccColor4B			getColor4B();
	virtual void				setColor4B( const ccColor4B& color );

	/**
	 * Indicates whether this particle has individual color content. This is determine by the
	 * configuration of the emitter. Within an emitter, either all particles have this content, or none do.
	 *
	 * When this property returns YES, each particle can be set to a different color. When this property
	 * returns NO, all particles will have the color specified by the diffuseColor property of the emitter.
	 *
	 * Particles are configured for individual color content by including the kCC3VertexContentColor
	 * component flag when setting the vertexContentTypes property of the emitter.
	 */
	virtual bool				hasColor();

	/**
	 * Removes this particle from the emitter. The differs from setting the isAlive property to NO,
	 * in that the removal is processed immediately, using the removeParticle: method of the emitter.
	 */
	virtual void				remove();

	/**
	 * The color of this particle.
	 *
	 * If this particle has individual color content, (which can be checked with the hasColor
	 * property), this property indicates the color in which this particle will appear.
	 *
	 * If this particle has individual color content, you can set this property at any
	 * time to define the color of the entire particle.
	 *
	 * Particles are configured for individual color content by including the kCC3VertexContentColor
	 * component flag when setting the vertexContentTypes property of the emitter.
	 *
	 * Setting this property will set the color of all vertices in the particle to the assigned color.
	 * 
	 * Reading this property returns the color value of the first vertex in the particle. If this
	 * particle does not support individual color content, this property will always return the value
	 * of the diffuseColor property of the emitter. In this condition, it is safe to set this property,
	 * but changes will have no effect.
	 */
	virtual CCColorRef			getColor();
	virtual void				setColor( const CCColorRef& color );

	/**
	 * The opacity of this particle.
	 *
	 * If this particle has individual color content, (which can be checked with the hasColor
	 * property), this property indicates the opacity in which this particle will appear.
	 *
	 * If this particle has individual color content, you can set this property at any
	 * time to define the opacity of the entire particle.
	 *
	 * Particles are configured for individual color content by including the kCC3VertexContentColor
	 * component flag when setting the vertexContentTypes property of the emitter.
	 *
	 * Setting this property will set the opacity of all vertices in the particle to the assigned value.
	 *
	 * Reading this property returns the color value of the first vertex in the particle. If this
	 * particle does not support individual color content, this property will always return the value
	 * of the opacity of the diffuseColor property of the emitter. In this condition, it is safe to
	 * set this property, but changes will have no effect.
	 */
	virtual CCOpacity			getOpacity();
	virtual void				setOpacity( CCOpacity opacity );

	/**
	 * Initializes this instance.
	 *
	 * When initializing a particle, be aware that, in the interests of performance and memory
	 * conservation, expired particles can and will be cached and reused, and particle emission
	 * may not always involve instantiating a new instance of your particle class.
	 *
	 * With this in mind, you should not depend on the init method being invoked during particle
	 * emission. All code that establishes the initial emitted state of a  particle should be
	 * included in the initializeParticle method, or should be set in the initializeParticle:
	 * method of the particle emitter or particle navigator.
	 */
	virtual bool				init();

	/**
	 * Allocates and initializes an autoreleased instance.
	 *
	 * When initializing a particle, be aware that, in the interests of performance and memory
	 * conservation, expired particles can and will be cached and reused, and particle emission
	 * may not always involve instantiating a new instance of your particle class.
	 *
	 * With this in mind, you should not depend on the init method being invoked during particle
	 * emission. All code that establishes the initial emitted state of a  particle should be
	 * included in the initializeParticle method, or should be set in the initializeParticle:
	 * method of the particle emitter or particle navigator.
	 */
	static CC3Particle*			particle();

	/**
	 * Template method that populates this instance from the specified other instance.
	 *
	 * This method is invoked automatically during object copying via the copy or copyWithZone:
	 * method. In most situations, the application should use the copy method, and should never
	 * need to invoke this method directly.
	 * 
	 * Subclasses that add additional instance state (instance variables) should extend copying by
	 * overriding this method to copy that additional state. Superclass that override this method should
	 * be sure to invoke the superclass implementation to ensure that superclass state is copied as well.
	 */
	virtual void				populateFrom( CC3Particle* another );

	// Alloc iVar in subclases to consolidate storage
	virtual bool				isAlive();

	// Alloc iVar in subclases to consolidate storage
	virtual void				setIsAlive( bool alive );

	virtual CCColorRef			getDisplayedColor();

	virtual bool				isCascadeColorEnabled();
	virtual void				setCascadeColorEnabled( bool cascadeColorEnabled );

	virtual void				updateDisplayedColor( const CCColorRef& color );
	virtual CCOpacity			getDisplayedOpacity();

	virtual bool				isCascadeOpacityEnabled();
	virtual void				setCascadeOpacityEnabled( bool cascadeOpacityEnabled );

	virtual void				updateDisplayedOpacity( CCOpacity opacity );
	virtual std::string			fullDescription();

	virtual void				initializeParticle();
	virtual void				finalizeParticle();

	virtual CCObject*			copyWithZone( CCZone* zone );

	virtual void				updateBeforeTransform( CC3NodeUpdatingVisitor* visitor );
	virtual void				updateAfterTransform( CC3NodeUpdatingVisitor* visitor );

	virtual CC3ParticleEmitter*	getEmitter();
	virtual void				setEmitter( CC3ParticleEmitter* emitter );

	virtual GLfloat				getLifeSpan() { return 0.f; }
	virtual void				setLifeSpan( GLfloat lifeSpan ) {  }
	virtual GLfloat				getTimeToLive() { return 0.f; }

	virtual CC3Vector			getVelocity() { return CC3Vector::kCC3VectorZero; }
	virtual void				setVelocity( const CC3Vector& velocity ) {  }

	virtual CC3Vector			getRotation() { return CC3Vector::kCC3VectorZero; }
	virtual void				setRotation( const CC3Vector& rotation ) {  }
	virtual CC3Vector			getRotationVelocity() { return CC3Vector::kCC3VectorZero; }
	virtual void				setRotationVelocity( const CC3Vector& rotation ) {  }
	virtual CC3Vector			getRotationAxis() { return CC3Vector::kCC3VectorZero; }
	virtual void				setRotationAxis( const CC3Vector& rotation ) {  }
	virtual GLfloat				getRotationAngle() { return 0.f; }
	virtual void				setRotationAngle( GLfloat angle ) {  }
	virtual GLfloat				getRotationAngleVelocity() { return 0.f; }
	virtual void				setRotationAngleVelocity( GLfloat angle ) {  }

	virtual GLfloat				getSizeVelocity() { return 0.f; }
	virtual void				setSizeVelocity( GLfloat velocity ) {  }
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
	virtual GLfloat				getSize() { return 0.f; }
	virtual void				setSize( GLfloat size ) {  }

	virtual ccColor4F			getColorVelocity() { return ccc4f(0, 0, 0, 1); }
	virtual void				setColorVelocity( const ccColor4F& colorVel ) { };

	/**
	 * The index of this particle within the collection of particles managed by the emitter.
	 *
	 * You should not assume that this property will be consistent during the lifetime of
	 * the particle. It can and will change spontaneously as other particles expire and
	 * the emitter manages its collection of particles.
	 *
	 * This property is set by the particle emitter as it manages its collection of particles.
	 * The application must treat this property as read-only, and must never set this property directly.
	 *
	 * At any time, this value is unique across all current living particles managed by the emitter.
	 */
	virtual	GLuint				getParticleIndex() { return -1; }
	virtual void				setParticleIndex( GLuint index ) { }

	/**
	 * Invoked automatically, if the particle has vertex normal content, to point the normal vector
	 * of the particle at the specified location, which is expressed in terms of the local coordinate
	 * system of the emitter.
	 *
	 * To point the particle itself at the location, we use vector math. The vector from the emitter to
	 * the particle is subtracted from the vector from the emitter to the specified location. The result
	 * is a vector that points from the particle to the given location. This vector is normalized and
	 * set in the normal property.
	 *
	 * This method is invoked automatically by the emitter if the particle has a normal, and the
	 * shouldUseLighting property of the emitter is set to YES, to keep the normal of the particle
	 * pointed towards the camera, so that the particle will appear to interact with the scene lighting.
	 */
	virtual void				pointNormalAt( const CC3Vector& camLoc ) { }

protected:
	CC3ParticleEmitter*			m_pEmitter;
};

NS_COCOS3D_END

#endif
