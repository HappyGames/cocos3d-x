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
#ifndef _CC3_PARTICLE_NAVIGATOR_H_
#define _CC3_PARTICLE_NAVIGATOR_H_

NS_COCOS3D_BEGIN

class CC3Particle;
class CC3ParticleEmitter;
/**
 * A particle navigator is assigned to a single particle emitter, and is responsible for configuring
 * the life cycle and emission path of the particle on behalf of the emitter.
 *
 * When creating your own particle system, customization is accomplished primarily by creating
 * your own implementation of the CC3ParticleProtocol, and your own CC3ParticleNavigator subclass.
 * You are encouraged to create subclasses of CC3ParticleNavigator (perhaps starting from one of
 * the existing provided subclasses).
 *
 * During particle initialization, the emitter, the navigator, and the particle itself are given a
 * chance to participate in the initialization of the particle. The navigator is distinct from the
 * emitter itself in that the navigator is primarily designed to direct the shape of the emission,
 * by setting particle properties such as the location, direction, and speed of the particle.
 * This separation of responsibilities often means that a single navigator class can be used to
 * direct any type of particle.
 *
 * For example, a particle navigator designed to emit partiles in the the shape of a fountain could
 * be used to create a fountain of point particles, a fountain of mesh particles, or a fountain of
 * some other class of particles that supported the protocol required by the navigator.
 *
 * Similarly, a navigator designed to lay particles out on a grid, or sprinkle stars across the
 * sky could do so with point particles or mesh particles.
 * 
 * The particle navigator is only involved in the initialization of the particle. It does not
 * interact with the particle once it has been emitted.
 *
 * Different particle navigators will have different requirements for configuring particles.
 * The requiredParticleProtocol property of this navigator indicates the protocol that the
 * particles must support in order to be prepared by this navigator during initialization.
 */
class CC3ParticleNavigator : public CCObject
{
public:
	CC3ParticleNavigator();
	virtual ~CC3ParticleNavigator();
	/**
	 * The emitter whose particles are prepared by this navigator.
	 *
	 * This property is set automatically when the navigator is attached to the emitter.
	 * Usually the application never needs to set this property directly.
	 */
	virtual CC3ParticleEmitter*	getEmitter();
	virtual void				setEmitter( CC3ParticleEmitter* emitter );

	/**
	 * Template method that initializes the particle. For particles that follow a planned life-cycle
	 * and trajectory, this navigator configures that life-cycle and trajectory for the particle
	 * prior to the particle being emitted.
	 *
	 * This method is invoked automatically from the emitter after the emitter has initialized
	 * the particle and before the initializeParticle method is invoked on the particle itself.
	 *
	 * Subclasses will override this method to configure the particle. Subclasses should always invoke
	 * the superclass implementation to ensure the superclass initialization behaviour is performed.
	 * 
	 * In this method, you can set the isAlive property of the particle to NO to cause the
	 * emission of the particle to be aborted.
	 * 
	 * When this method is invoked, the particle may have just been instantiated, or it may be an
	 * older expired particle that is being reused. With this in mind, this method should include all
	 * code that establishes the initial emitted state of a particle that is to be set by the navigator.
	 * You should not rely on any state set in the instance initializer of the particle class.
	 *
	 * This method is invoked automatically by the emitter when a particle is emitted.
	 * Usually the application never has need to invoke this method directly.
	 */
	virtual void				initializeParticle( CC3Particle* aParticle );

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
	void						populateFrom( CC3ParticleNavigator* another );
	virtual CCObject*			copyWithZone( CCZone* pZone );

	virtual bool				init();

protected:
	CC3ParticleEmitter*			m_pEmitter;
};

NS_COCOS3D_END

#endif
