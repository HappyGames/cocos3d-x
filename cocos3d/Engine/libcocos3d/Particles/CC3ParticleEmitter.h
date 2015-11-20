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
#ifndef _CC3_PARTICLE_EMITTER_H_
#define _CC3_PARTICLE_EMITTER_H_

NS_COCOS3D_BEGIN

class CC3Particle;
class CC3ParticleNavigator;
/**
 * A CC3MeshNode that emits 3D particles.
 *
 * Particles are small, simple objects that can each have their own location, movement,
 * color, and lifetime. They are used where many simple and similar objects are needed.
 * Examples might include confetti, stars, tiles, bricks, etc.
 *
 * One key way that particles differ from mesh nodes is that all vertices for all of the particles
 * managed by an emitter are submitted to the GL engine in a single draw call. This is much more
 * efficient than each mesh making its own GL call, and for large numbers of small objects, this
 * technique dramatically improves performance.
 *
 * Particles can be added to an emitter directly, using the emitParticle: method, can be created
 * and emitted from the emitter manually, using the emitParticle and emitParticles: method, or
 * can be emitted automatically at a pre-determined rate from the emitter by setting the emission
 * properties of the emitter.
 
 * Typically, particles are automatically created and emitted by the emitter at some predetermined
 * rate and pattern, such as a fountain, fire, hose, etc. and follow a pre-determined algorithmic
 * path with a finite life span.
 *
 * Alternately, particles can be added to the emitter by the application directly, with the emitter
 * containing and managing the particles, but leaving the application in control of particle control
 * and interaction. In this use case, the emitter acts as a mesh batching system, allowing the meshes
 * from a large number of distinct objects to be submitted to the GL engine in a single draw call.
 * For example, the application may want to create a large number of bricks, tiles, plants, etc.,
 * and have them efficiently managed and rendered by an emitter.
 *
 * All particles added to or emitted by this emitter will be covered by the same material, and optional
 * texture, as determined by the material and texture properties of this emitter node. But each particle
 * may have its own location, movement, orientation, normals, and colors.
 *
 * Although all particles are covered by the same material and texture, if the vertexContentTypes
 * property of this emitter is configured with the kCC3VertexContentColor component, then each
 * particle can be assigned a different color. And for particles that support texture mapping,
 * such as mesh particles, then each particle can be covered by a different section of the
 * texture assigned to the emitter, effectivly allowing each particle to be textured differently.
 *
 * Particles managed by a CC3ParticleEmitter live in the 3D scene, as distinct from the 2D
 * particles available through the Cocos2D CCParticleSystem class.
 * 
 * For many particle effects, 2D is sufficient, and can be quite effective. You can use a Cocos2D
 * CCParticleSystem instance with a CC3Billboard, to embed 2D particle systems within a 3D Cocos3D scene.
 *
 * However, for applications that need particles to move in three dimensions, you can use this class,
 * or one of its specialized subclasses. Each particle emitted by this emitter has a 3D location,
 * and can appear in front of or behind other 3D objects, depending on relative distance from the camera.
 *
 * A particle system embodies three classes working together to emit and control particles.
 *   -# The emitter is responsible for generating and managing particles of a particular type.
 *      Particles that have expired can be reused, reinitialized and re-emitted by the emitter.
 *      Typically, you will use one of the standard emitters.
 *   -# The navigator is attached to the emitter and configures the lifetime and path of the
 *      particle. You can use one of the standard navigators, but you will often write your
 *      own navigator to provide more interesting emission characterstics and finer control
 *      of how each particle is configured.
 *   -# The particle itself is responsible for executing the behaviour and motion of the particle
 *      throughout its lifespan. You will generally always write your own particle subclass.
 *
 * When a particle is added or emitted, all three of these objects get a chance to initialize
 * and configure the particle. Typically, the emitter is responsible for instanitating a new
 * particle, or arranging to reuse an expired particle. The navigator initializes the lifetime
 * and path configuration information within the particle, or the particle itself can do so
 * during its own initialization. During this process, the emitter invokes the initializeParticle:
 * method on itself and the navigator, and then the initializeParticle method on the particle.
 *
 * The isAlive property is automatically set to YES before the initializeParticle method is
 * invoked on the particle, so you don't have to set it there. You can, however, set it to
 * NO during execution of the initializeParticle method, to abort the emission of that particle.
 *
 * Subsequently, on each update pass, the emitter will automatically invoke the updateBeforeTransform:
 * method (and optionally the updateAfterTransform: method) on the particle. You will override this
 * method to define the behaviour of your particles over time. If your particles have a finite lifespan,
 * you can indicate that a particle has expired by having the particle set its own isAlive property
 * to NO within the updateBeforeTransform: (or updateAfterTransform:) method. The emitte will then
 * arrange to remove the particle and set it aside for future reuse.
 *
 * To enhance performance and memory, particles that have expired are retained and reused as further
 * particles are emitted. This is transparent to the particles (and the developer), as the reused
 * particle follow the same initialize/update life-cycle described above. The isAlive property is
 * reset to YES, and the initializeParticle: methods of the emitter and navigator, and the
 * initializeParticle method of the particle are invoked when the particle is reused and emitted again.
 *
 * Like all mesh nodes, the emitter contains a CC3Material instance that determines how the particle
 * content will blend with content from other 3D objects that overlap this emitter, and to specify
 * the texture that covers the particles.
 *
 * You can indicate the rate at which particle are emitted by setting either of the emissionRate
 * or emissionInterval properties. You can set for how long the emitter should emit particles
 * using the emissionDuration property.
 *
 * For emitters with finite emissionDuration, you can set the shouldRemoveOnFinish to YES to
 * indicate that the emitter should remove itself automatically from the 3D scene, once all
 * particles have expired, cleaning up all memory usage by the emitter and particles along the
 * way. This features allows you to set a transient particle generator, such as an explosion,
 * going and then forget about it.
 *
 * By default, the boundingVolume of the emitter will automatically be recalculated every time
 * a particle moves. Although this is convenient and ensures accuracy, recalculating the bounding
 * volume can often be an expensive operation. To avoid this, you can manually set static boundaries
 * in the boundingVolume of this emitter node and then set the shouldUseFixedBoundingVolume property
 * of this emitter to YES to indicate that you don't want the emitter to recalculate its
 * boundingVolume on each update.
 *
 * During development, you can verify the size of this static bounding volume your settings by
 * setting the shouldDrawBoundingVolume property to YES to make the bounding volume visible to
 * ensure that the bounding volume is sized appropriately to contain all the particles, without
 * being overly expansive.
 *
 * You may be wondering how to determine the correct static bounding volume properties. You can do
 * this at development time by setting the shouldMaximize property of the boundingVolume of this
 * emitter to YES, and setting the shouldUseFixedBoundingVolume property of this emitter to NO, so
 * that the boundingVolume will be recalculated on each update. After the emitter has finished,
 * output the boundingVolume to the log using LogDebug to record the maximume size that the bounding
 * volume grew to during particle emission. This will give you an idea of how big to set the static
 * boundary properties of the boundingVolume of your emitter.
 *
 * All memory used by the particles and the underlying vertex mesh is managed by this
 * emitter node, and is deallocated automatically when the emitter is released.
 */
class CC3ParticleEmitter : public CC3MeshNode 
{
	DECLARE_SUPER( CC3MeshNode );
public:
	CC3ParticleEmitter();
	virtual ~CC3ParticleEmitter();
	/**
	 * For particles that follow a planned life-cycle and trajectory, the particle navigator configures that
	 * life-cycle and trajectory into each particle prior to the particle being emitted by this emitter.
	 *
	 * The particle navigator is strictly used during initial configuration of the particle.
	 * It does not play any active part in managing the life-cycle or trajectory of the particle
	 * once it has been emitted.
	 *
	 * A particle navigator is not required for particles that can determine their own life-cycle
	 * and trajectory dynamically, without the need for configuration.
	 *
	 * Because the particle navigator may have specialized configuration requirements, when setting
	 * this property, the class indicated by the particleClass property is evaluated to see if it
	 * supports the protocol required by this navigator, as indicated by the requiredParticleProtocol
	 * property of the navigator, and raises an assertion if the particleClass does not support the
	 * protocol required by the navigator.
	 *
	 * The initial value of this property is nil, indicating that the particles will not be
	 * configured with a life-cycle and trajectory by a navigator.
	 */
	virtual CC3ParticleNavigator* getParticleNavigator();
	virtual void				setParticleNavigator( CC3ParticleNavigator* navigator );

	/**
	 * Indicates the length of time that the emitter will emit particles.
	 *
	 * Setting this value to kCC3ParticleInfiniteDuration indicates that the emitter should continue
	 * emitting particles forever, or until the pause or stop method are invoked or the isEmitting is
	 * manually set to NO.
	 *
	 * The initial value is kCC3ParticleInfiniteDuration.
	 */
	virtual GLfloat				getEmissionDuration();
	virtual void				setEmissionDuration( GLfloat emiationDuration );

	/**
	 * For emitters with a finite emissionDuration, indicates the length of time that this
	 * emitter has been emitting particles.
	 *
	 * When the value of this property exceeds the value of the emissionDuration property,
	 * the pause method is automatically invoked to cease the emission of particles.
	 */
	virtual GLfloat				getElapsedTime();

	/**
	 * The rate that particles will be emitted, expressed in particles per second.
	 * You can use this property as an alternate to the emissionInterval property.
	 * 
	 * Emission begins when the play method is invoked.
	 * 
	 * The initial value of this property is zero, indicating that no particles
	 * will be automatically emitted.
	 *
	 * As an alternate to setting this property to engage automatic emission, you can leave this
	 * property at its initial value and manually invoke the emitParticle method whenever you
	 * determine that you want to emit a particle, or you may use the emitParticle: method to add
	 * a particle that you have created outside the emitter.
	 */
	virtual GLfloat				getEmissionRate();
	virtual void				setEmissionRate( GLfloat emissionRate );

	/**
	 * The interval between each emission of a particle, expressed in seconds.
	 * You can use this property as an alternate to the emissionRate property.
	 * 
	 * Emission begins when the play method is invoked.
	 * 
	 * The initial value of this property is kCC3ParticleInfiniteDuration,
	 * indicating that no particles will be automatically emitted.
	 *
	 * As an alternate to setting this property to engage automatic emission, you can leave this
	 * property at its initial value and manually invoke the emitParticle method whenever you
	 * determine that you want to emit a particle, or you may use the emitParticle: method to add
	 * a particle that you have created outside the emitter.
	 */
	virtual GLfloat				getEmissionInterval();
	virtual void				setEmissionInterval( GLfloat emissionInterval );

	/**
	 * Indicates that this emitter should automatically be removed from its parent, and
	 * from the 3D scene when it is finished (when the isFinished property turns to YES).
	 *
	 * This is useful for emitters that are created to generate a transient effect such as an explosion.
	 *
	 * The initial value of this property is NO.
	 */
	virtual bool				shouldRemoveOnFinish();
	virtual void				setShouldRemoveOnFinish( bool removeOnFinish );
			
	/**
	 * Indicates the maximum number of particles that can be alive at any one time in the
	 * particle system managed by this emitter.
	 *
	 * The initial number of particles is zero. As the number of particles grows, memory is
	 * allocated for them in increments defined by the particleCapacityExpansionIncrement
	 * property, until this capacity is reached. Once this value is reached, no further memory
	 * will be allocated, and new particles will only be emitted as old ones die off.
	 *
	 * You can set the value of this property to kCC3ParticlesNoMax to indicate that no pre-defined
	 * limit to the number of particles exists. However, you should be careful when designing your
	 * particle emitter so it either reaches a steady state, or has a short enough lifetime, that
	 * the memory requirements do not continue to grow without bounds.
	 * 
	 * This property does not define the maximum number of particles that can be emitted over time.
	 * As particles age, you can indicate that a particle has expired by setting the isAlive property
	 * of the CC3ParticleProtocol to NO in the updateBeforeTransform: or updateAfterTransform: methods
	 * of the particle. This frees that particle to be re-initialized and re-emitted at a later time.
	 *
	 * The value of this property defines the maximum amount of memory that will be allocated
	 * for particles, and their vertex content, used by this emitter. When this emitter is
	 * deallocated, that memory will automatically be released.
	 *
	 * The initial value of this property is kCC3ParticlesNoMax, indicating that there is no
	 * pre-defined maximum limit to the number of particles that will be emitted.
	 */
	virtual GLuint				getMaximumParticleCapacity();
	virtual void				setMaximumParticleCapacity( GLuint particleCapacity );

	/**
	 * Indicates the current maximum number of particles that can be alive at any one time in the
	 * particle system managed by this emitter, before further memory will need to be allocated.
	 * This property is a measure of the amount of memory that has currently been allocated for particles.
	 *
	 * The initial number of particles is zero, and the initial capacity is zero. As the number
	 * of particles grows, memory is allocated for them in increments defined by the
	 * particleCapacityExpansionIncrement property, until the maximum capacity defined by the
	 * maximumParticleCapacity property is reached. After each memory allocation, this
	 * currentParticleCapacity property indicates how many particles can be alive simultaneously
	 * before a further memory allocation will be required.
	 * 
	 * This property does not define the maximum number of particles that can be emitted over time
	 * without further expansion. As particles age, you can indicate that a particle has expired by
	 * setting theisAlive property of the CC3ParticleProtocol to NO in the updateBeforeTransform: or
	 * updateAfterTransform: methods of the particle. This frees that particle to be re-initialized
	 * and re-emitted at a later time.
	 */
	virtual GLuint				getCurrentParticleCapacity();

	/**
	 * The amount of additional particle capacity that will be allocated each time space for
	 * additional particle is created.
	 *
	 * The initial number of particles is zero. As the number of particles grows, memory is allocated
	 * for them in increments defined by this property until the maximum capacity defined by the
	 * maximumParticleCapacity property is reached.
	 *
	 * Setting the value of this property equal to the value of the maximumParticleCapacity property
	 * will allocate the memory for all particles in one stage when the first particle is added or
	 * emitted. If the number of particles of your emitter is fairly steady, this is the most efficient
	 * way to allocate memory. If the number of particles is not easy to quantify in advance, or can
	 * vary considerably, it may be best to set this property with value smaller than that of the
	 * maximumParticleCapacity property.
	 *
	 * The initial value of this property is 100.
	 */
	virtual GLuint				getParticleCapacityExpansionIncrement();
	virtual void				setParticleCapacityExpansionIncrement( GLuint increment );

	/**
	 * Returns whether the maximum number of particles has been reached. This occurs when the value
	 * of the particleCount property reaches the value of the maximumParticleCapacity property.
	 * When this occurs, no further particles will be emitted until some particles expire.
	 */
	virtual bool				isFull();

	/**
	 * Indicates whether the emitter should invoke the updateBeforeTransform: method on each
	 * particle before the emitter node and particles have been transformed.
	 *
	 * Since most active particles need to perform activities before being transformed, such as
	 * updating their location, the initial value of this property is YES. If your customized
	 * particles do not require such behaviour (for example, static particles such as stars or
	 * bling decoration on another node), set the value of this property to NO to avoid unnecessary
	 * iteration over a potentially large number of particles.
	 */
	virtual bool				shouldUpdateParticlesBeforeTransform();
	virtual void				setShouldUpdateParticlesBeforeTransform( bool shouldUpdate );

	/**
	 * Indicates whether the emitter should invoke the updateAfterTransform: method on each
	 * particle after the emitter node and particles have been transformed.
	 *
	 * Since it is uncommon for particles to need to perform activities after being transformed,
	 * the initial value of this property is NO, in order to avoid unnecessary iteration over a
	 * potentially large number of particles. If your customized particles have defined behaviour
	 * that is to be performed after the particles and emitter have been transformed, set the value
	 * of this property to YES.
	 */
	virtual bool				shouldUpdateParticlesAfterTransform();
	virtual void				setShouldUpdateParticlesAfterTransform( bool shouldUpdate );

	/** Begins, or resumes, the emission of particles by setting the isEmitting property to YES. */
	virtual void				play();

	/**
	 * Ceases the emission of particles by setting the isEmitting property to NO.
	 * Particles that have already been emitted will continue to be updated and displayed.
	 *
	 * Particle emission can be resumed by invoking the play method again.
	 *
	 * As an alternate to stopping emission manually, you can set the emissionDuration
	 * property to cause particles to be emitted for a finite time and then stop.
	 */
	virtual void				pause();

	/**
	 * Ceases the emission of particles by setting the isEmitting property to NO.
	 * Particles that have already been emitted will no longer be updated and displayed,
	 * effectively causing those particles to abruptly disappear from view.
	 *
	 * Particle emission can be restarted by invoking the play method again.
	 *
	 * In most cases, for best visual effect, you should use the pause method instead
	 * to stop the emission of of new particles, but allow those that have already been
	 * emitted to live out their lives.
	 */
	virtual void				stop();

	/**
	 * Indicates whether the emitter is currently emitting particles.
	 *
	 * For emitters with a finite emissionDuration, the value of this property will
	 * automatically be set to NO once that emissionDuration has passed.
	 *
	 * For emitters with infinite emissionDuration, or for emitters with a finite emissionDuration
	 * that has not yet passed, setting the value of this property to NO will stop the emitter from
	 * emitting any further particles. Particles that have already been emitted will continue to be
	 * updated and displayed.
	 *
	 * Setting this property to YES has the same effect as invoking the play method. Setting this
	 * property to NO has the same effect as invoking the pause method.
	 * 
	 * Emission can be started or restarted by setting this property to YES.
	 */
	virtual bool				isEmitting();

	/**
	 * Indicates whether this emitter is active.
	 *
	 * It is active if either particles are currently being emitted, or particles have
	 * been emitted but have not yet lived out their lives.
	 *
	 * Formally, this property returns YES if either the isEmitting property returns YES or the
	 * value of the particleCount property is greater than zero. Otherwise this property returns NO.
	 *
	 * The stop method can be used to force this emitter to be immediately inactive.
	 */
	virtual bool				isActive();

	/**
	 * Indicates whether particle emission has ceased and all particles have lived out their lives.
	 *
	 * This property will return YES if the isEmitting property was previously set to YES (or the
	 * play method was previously invoked), and the isActive property now has a value of NO.
	 *
	 * This property is distinguished from the isActive property in that the isActive property
	 * will be NO both before and after emission, whereas the isFinished property will be NO
	 * both before and during emission, and will be YES only after emission.
	 *
	 * The stop method can be used to force this condition immediately.
	 */
	virtual bool				isFinished();

	/**
	 * Emits a single particle of the type specified in the particleClass property.
	 *
	 * Each particle is initialized prior to emission. A particle can be initialized in any of the
	 * initializeParticle: method of the emitter, the initializeParticle: method of the particle
	 * navigator, or the initializeParticle method of the particle itself. Each particle system has
	 * different needs. To initialize each particle, you should override a combination of these
	 * methods, as appropriate to initialize the particles.
	 *
	 * For particles that follow a planned life-cycle and trajectory, the initializeParticle: method
	 * of the particle navigator is the appropriate place to initialize the life-cycle and trajectory
	 * of the particle. For particles that are more self-aware and self-determining, the initializeParticle
	 * method of the particle itself may be the best place to initialized the particle.
	 *
	 * If the emitter is set to emit particles automatically, by setting an emissionRate or
	 * emissionInterval, and then invoking play, you do not need to invoke this method directly.
	 * It will be invoked automatically when it is time to emit a particle. This is the most
	 * common situation, and so in most cases, you will never invoke this method directly.
	 *
	 * However, there are some situations where the application might want more control over the
	 * creation or configuration of particles. One example might be if you want to create a quantity
	 * of fixed particles, such as a chain, or lights on a tree, that are not emitted at a steady
	 * rate. Another example might be that you want to be able to configure or track each particle
	 * from the application code, once it has been created, emitted, and returned from this method.
	 *
	 * In these situations, you can avoid invoking play (and avoid setting the isEmitting flag set
	 * to YES), and then invoke the emitParticle method whenever you want to create a new particle.
	 * 
	 * This method returns the emitted particle. If the maximum capacity has been reached, as
	 * defined by the maximumParticleCapacity property, or if the particle itself aborts the
	 * emission by setting the isAlive property to NO in the initializeParticle method of the
	 * particle, this method will return nil.
	 *
	 * You may also use the emitParticle: method to create partiles outside of the emitter and add
	 * them to the emitter. You can use that emitParticle: method instead of this method when you
	 * want to select different particle classes. This emitParticle class will always emit a
	 * particle of the class defined by the particleClass property.
	 */
	virtual CC3Particle*		emitParticle();

	/**
	 * Emits the specified number of particles, by invoking the emitParticle method repeatedly.
	 *
	 * Returns the number of particles that were emitted. If a particle aborts emission, or if
	 * the maximum number of particles, as defined by the maximumParticleCapacity property is
	 * reached, the returned number may be less that the specified count.
	 */
	virtual GLuint				emitParticles( GLuint count );

	/**
	 * Adds the specified particle to the emitter and emits it.
	 *
	 * This method allows the application to create and initialize particles outside of the emitter,
	 * instead of having the emitter instantiate and intialize them.
	 *
	 * This method is particularly useful when the application wants to create and emit a particle
	 * of a class other than the class indicated by the particleClass method. In doing so, the
	 * application must adhere to the requirement that the particle must implement the protocols
	 * specified by the requiredParticleProtocol property of both this emitter and the particle
	 * navigator. Submitting a particle to this method that does not implement both of these
	 * required protocols will raise an assertion exception.
	 *
	 * This method is also useful when the application does not want particles to be automatically
	 * emitted, but wants the emitter to efficiently manage and render a large number of particles
	 * created by the application. For example, the application may want to create and manage a large
	 * number of bricks, tiles, plants, swarms, etc.
	 * 
	 * Particles added by this method follow exactly the same initialization and update process as
	 * particles that are emitted by this emitter. As with each emitted particle, for each particle
	 * added using this method the initializeParticle: method is invoked on each of the emitter and
	 * navigator in turn, and then the initializeParticle method is invoked on the particle itself.
	 * 
	 * There are only two differences between the emitParticle and emitParticle: methods:
	 *   - The emitParticle method will reuse an expired particle if one is available. The emitParticle:
	 *     method accepts a new particle instance on each invocation.
	 *   - The emitParticle method automatically instantiates particles of the class indicated
	 *     by the particleClass property. The emitParticle: method allows the application to instantiate
	 *     a particle of any class that implements the protocols defined by the requiredParticleProtocol
	 *     property of both this emitter and the particle navigator.
	 * 
	 * You may combine use of the emitParticle method and the emitParticle: method. You may also combine use
	 * of automatic emission (by configuring an emission schedule within this emitter and then invoking the
	 * play method), and manual emission using a combination of the emitParticle and emitParticle: methods.
	 *
	 * When using a combination of emission techniques, particles added by this method are eligible
	 * to be reused automatically by the emitter once they have expired.
	 *
	 * When initializing particles outside of the emitter prior to invoking this method, be sure that
	 * the emitter property of the particle is set to nil when submitting the particle to this method,
	 * as the emitter uses this property as an indication of whether the particle was created outside
	 * the emitter, or generated automatically inside the emitter.
	 */
	virtual bool				emitParticle( CC3Particle* aParticle );

	/**
	 * Returns a particle suitable for emission by this emitter. The returned particle can subsequently
	 * be emitted from this emitter using the emitParticle: method.
	 *
	 * The particle emitted may be an existing expired particle that is being reused, or it may be a
	 * newly instantiated particle. If an expired particle is available within this emitter, it will
	 * be reused. If not, this method invokes the makeParticle method to create a new particle.
	 * 
	 * You can also use the makeParticle method directly to ensure that a new particle has been created.
	 */
	virtual CC3Particle*		acquireParticle();

	/**
	 * Creates a new autoreleased instance of a particle of the type specified by the particleClass property.
	 * The returned particle can subsequently be emitted from this emitter using the emitParticle: method.
	 *
	 * Distinct from the acquireParticle method, this method bypasses the reuse of expired particles
	 * and always creates a new autoreleased particle instance.
	 */
	virtual CC3Particle*		makeParticle();

	/**
	 * Template method that initializes the particle. This method is invoked automatically
	 * from the emitParticle method just prior to the emission of the specified particle.
	 *
	 * This method is invoked after the isAlive property of the particle has been set to YES,
	 * and prior to the invocation of the initializeParticle: on the particle navigator and the
	 * initializeParticle method on the particle.
	 * 
	 * In this method, you can set the isAlive property of the particle to NO to cause the
	 * emission of the particle to be aborted.
	 * 
	 * When this method is invoked, the particle may have just been instantiated, or it may be an
	 * older expired particle that is being reused. With this in mind, this method should include all
	 * code that establishes the initial emitted state of a particle that is to be set by the emitter.
	 * You should not rely on any state set in the instance initializer of the particle class.
	 *
	 * This method is invoked automatically by the emitter when a particle is emitted.
	 * Usually the application never has need to invoke this method directly.
	 */
	virtual void				initializeParticle( CC3Particle* aParticle );

	/**
	 * Removes the specified particle from the emitter, sets the isAlive property of the particle to NO,
	 * and retains the particle for reuse.
	 *
	 * Normally, the recommended mechanism for removing a particle is to set its isAlive property to NO,
	 * which will cause the particle to automatically be removed on the next update loop, if either of the
	 * shouldUpdateParticlesBeforeTransform or shouldUpdateParticlesAfterTransform properties is set to YES.
	 *
	 * This method may be used instead, in cases where the shouldUpdateParticlesBeforeTransform and
	 * shouldUpdateParticlesAfterTransform properties are both set to NO, or where the update loop is delayed
	 * (perhaps due to less frequent updates of the particles), and the particle must be removed immediately.
	 *
	 * If the specified particle is not currently alive, or has already been removed, this method does nothing.
	 */
	virtual void				removeParticle( CC3Particle* aParticle );

	/** Removes all the particles from the emitter. They remain cached for reuse. */
	virtual void				removeAllParticles();

	/**
	 * Finalizes and removes the specified particle. Finalization must happen first, because the
	 * particle may be removed to create space, which can result in its deallocation if this emitter
	 * is all that is holding onto it.
	 */
	virtual void				finalizeAndRemoveParticle( CC3Particle* aParticle, GLuint anIndex );

	/**
	 * Removes the current particle from the active particles, but keep it cached for future use.
	 *
	 * This basic implementation simply decrements the particleCount. Subclasses will define behaviour
	 * for removing the particle from the particles collection, and for moving the underlying vertex content.
	 */
	virtual void				removeParticle( CC3Particle* aParticle, GLuint anIndex );

	/** Ensures space has been allocated for the specified particle. */
	virtual bool				ensureParticleCapacityFor( CC3Particle* aParticle );

	/**
	 * If the specified particle is not being reused, it is added to the particles collection,
	 * at the end of the living particles, and in front of any expired particles.
	 * This emitter is also attached to the particle.
	 */
	virtual void				addNewParticle( CC3Particle* aParticle );

	/**
	 * Template method that accepts the particle if initialization did not abort the particle.
	 *
	 * This implementation simply increments the particleCount property. Subclasses may override
	 * to perform additional activity to accept the particle.
	 */
	virtual void				acceptParticle( CC3Particle* aParticle );

	/**
	 * The array of particles.
	 *
	 * The application must not change the contents of this array directly.
	 */
	virtual CCArray*			getParticles();

	/**
	 * The number of particles that are currently alive and being displayed by this emitter. The value of
	 * this property will increase as particles are emitted, and will decrease as particles age and expire.
	 */
	virtual GLuint				getParticleCount();

	/** Returns the particle at the specified index within the particles array. */
	CC3Particle*				getParticleAt( GLuint aParticleIndex );

	/**
	 * Returns the particle that contains the vertex at the specified index, or nil if no particle
	 * contains the specified vertex.
	 */
	CC3Particle*				getParticleWithVertexAt( GLuint vtxIndex );

	/**
	 * Returns the particle that contains the vertex index at the specified index, or nil if no particle
	 * contains the specified vertex index.
	 *
	 * If the mesh of this emitter contains a vertex index array, the value returned by this method may
	 * be different than that returned by the particleWithVertexAt: method, which references the index of
	 * the vertex, whereas this method references the index of the vertex index that points to the vertex.
	 *
	 * If the mesh of this emitter does not contain a vertex index array, the value returned by this
	 * method will be the same as the value returned by the particleWithVertexAt: method, because in
	 * that case, there is a one-to-one relationship between a vertex and its index.
	 */
	CC3Particle*				getParticleWithVertexIndexAt( GLuint index );
		
	/**
	 * Returns the particle that contains the face at the specified index, or nil if no particle
	 * contains the specified face.
	 *
	 * This is a convenience method that determines the first vertex index associated with the
	 * specified face, taking into consideration the drawingMode of this emitter, and then invokes
	 * the particleWithVertexIndexAt: method to retrieve the particle from that vertex index.
	 */
	CC3Particle*				getParticleWithFaceAt( GLuint faceIndex );

	void						initWithTag( GLuint aTag, const std::string& aName );

	/** Particles are not copied. */
	void						populateFrom( CC3ParticleEmitter* another );
	virtual CCObject*			copyWithZone( CCZone* zone );
	
	std::string					fullDescription();
	
	/**
	 * Invoked during node updates.
	 * Emits new particles, updates existing particles, and expires aging particles.
	 */
	virtual void				processUpdateBeforeTransform( CC3NodeUpdatingVisitor* visitor );

	/**
	 * For each particle, invoke the updateBeforeTransform: method.
	 * If the particle has expired, remove it from the particles array.
	 */
	virtual void				updateParticlesBeforeTransform( CC3NodeUpdatingVisitor* visitor );

	/** Update the particles after the transform, and then update the mesh node.  */
	virtual void				processUpdateAfterTransform( CC3NodeUpdatingVisitor* visitor );

	/**
	 * For each particle, invoke the updateAfterTransform: method.
	 * If the particle has expired, remove it from the particles array.
	 */
	virtual void				updateParticlesAfterTransform( CC3NodeUpdatingVisitor* visitor );

	/** Template method that checks if its time to quit emitting. */
	virtual void				checkDuration( GLfloat dt );

	/**
	 * Template method that checks if its time to emit a particle,
	 * and if so, invokes the emitParticle method to emit the particle.
	 */
	virtual void				checkEmission( GLfloat dt );

	/** If transitioning to emitting from not, mark as such and reset timers. */
	virtual void				setIsEmitting( bool shouldEmit );

	/** Overridden to test if active as well. If not active, there is nothing to intersect. */
	virtual bool				doesIntersectBoundingVolume( CC3BoundingVolume* otherBoundingVolume );

	/** Overridden to set the wireframe to automatically update as parent changes. */
	virtual void				setShouldDrawLocalContentWireframeBox( bool shouldDraw );

protected:
	CCArray*					m_particles;
	CC3ParticleNavigator*		m_particleNavigator;
	GLuint						m_currentParticleCapacity;
	GLuint						m_maximumParticleCapacity;
	GLuint						m_particleCapacityExpansionIncrement;
	GLuint						m_particleCount;
	GLfloat						m_emissionDuration;
	GLfloat						m_elapsedTime;
	GLfloat						m_emissionInterval;
	GLfloat						m_timeSinceEmission;
	bool						m_shouldRemoveOnFinish : 1;
	bool						m_isEmitting : 1;
	bool						m_wasStarted : 1;
	bool						m_shouldUpdateParticlesBeforeTransform : 1;
	bool						m_shouldUpdateParticlesAfterTransform : 1;
};

NS_COCOS3D_END

#endif
