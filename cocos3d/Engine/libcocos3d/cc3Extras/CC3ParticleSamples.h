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
/** @file */	// Doxygen marker
#ifndef _CC3_PARTICLE_SAMPLES_H_
#define _CC3_PARTICLE_SAMPLES_H_

NS_COCOS3D_BEGIN

/**
 * CC3RandomMortalParticleNavigator confgures the lifeSpan property of particles that
 * conform to the CC3MortalParticleProtocol.
 *
 * A particle that supports CC3MortalParticleProtocol has a finite life. and when that lifetime is
 * finished, the particle will automatically expire itself by setting the isAlive property to NO.
 *
 * During initialization of each particle in the initializeParticle: method, this navigator sets
 * the lifeSpan property of the particle is set to a random value between the values of the
 * minParticleLifeSpan and maxParticleLifeSpan properties of this navigator.
 */
class CC3RandomMortalParticleNavigator : public CC3ParticleNavigator 
{
	DECLARE_SUPER( CC3ParticleNavigator );
public:
	/**
	 * Indicates the lower limit of the range of possible particle life spans.
	 *
	 * When a particle is emitted, the lifeSpan property will be set to a random value
	 * between the value of this property and the value of the maxParticleLifeSpan property.
	 *
	 * The initial value of this property is zero.
	 */
	GLfloat						getMinParticleLifeSpan();
	void						setMinParticleLifeSpan( GLfloat lifeSpan );

	/**
	 * Indicates the upper limit of the range of possible particle life spans.
	 *
	 * When a particle is emitted, the lifeSpan property will be set to a random value
	 * between the value of the minParticleLifeSpan property and the value of this property.
	 *
	 * The initial value of this property is zero.
	 */
	GLfloat						getMaxParticleLifeSpan();
	void						setMaxParticleLifeSpan( GLfloat lifeSpan );

	void						populateFrom( CC3RandomMortalParticleNavigator* another );
	virtual CCObject*			copyWithZone( CCZone* pZone );
	
	virtual bool				init();
	virtual void				initializeParticle( CC3Particle* aParticle );

protected:
	GLfloat						m_minParticleLifeSpan;
	GLfloat						m_maxParticleLifeSpan;
};

/**
 * CC3HoseParticleNavigator confgures particles that support the CC3SprayParticleProtocol
 * to be emitted in a stream, as if from the nozzle of a hose.
 *
 * A CC3HoseParticleNavigator instance is made up of two parts: the navigator and the nozzle.
 * 
 * Particles live within the context of the emitter node, and movement of the emitter node affects
 * all of the particles already emitted by that emitter. For example, if the emitter is rotating,
 * the particles will rotate along with it as they live out their lives. The navigator is attached
 * to the emitter, and configures the life-span and trajectory of the particles.
 *
 * The location and rotation of the nozzle node determine where the particles will be emitted, and
 * in what direction, respectively. Moving the nozzle does not effect the movement of the particles
 * that have already been emitted.
 * 
 * By default, the nozzle node is a child node of the emitter. However, you can change the parent
 * node of the nozzle to some other object by invoking the addChild: method on the other node,
 * with the nozzle node as the argument.
 *
 * By assigning the nozzle to a different parent node, you can have the nozzle track another node,
 * and emit particles as that node travels. For example you might attach the nozzle to the tail of
 * a rocket node, to emit a trail of particles behind the rocket as the rocket moves.
 *
 * The parent of the nozzle (the rocket, for example) does not need to be a child or descendant of
 * this navigator's emitter. Like any node, the location and rotation properties of the nozzle are
 * specified relative to its parent (eg. the rocket).
 *
 * Note the difference in behaviour of the particles by having the nozzle move instead of the emitter.
 * In the rocket example, if the emitter was attached to the tail of the rocket, the emitted particles
 * would move along with the emitter, making it very difficult to calculate realistic paths for the
 * particles. By making the emitter stationary, and attaching only the nozzle to the rocket, the point
 * of emission moves with the rocket, but the particles move and live out their lives in fixed space,
 * and it becomes much simpler to calculate their movement.
 *
 * You can even combine the two frames of reference for interesting effects. You can put both the
 * emitter and the nozzle in separate motion. For example, to create clouds moving on a rotating globe,
 * you could place the emitter at the center of the globe, so that it and the cloud particles rotate
 * around with the globe, and have the nozzle also moving across the surface of the globe to simulate
 * the clouds travelling across the surface of the globe.
 * 
 * For such a complicated scenario to work, keep in mind that the emitter and the parent of the nozzle
 * should share a common ancestor node (in this example, the globe), to make it easy for the particles
 * to transition from the nozzle frame of reference to that of the emitter.
 *
 * You can set the shape of the nozzle using the dispersionAngle property, which specifies how tight or
 * wide the spray will be, and you can set a range of speeds for the particles as they leave the emitter.
 */
class CC3HoseParticleNavigator : public CC3RandomMortalParticleNavigator, public CC3NodeTransformListenerProtocol
{
	DECLARE_SUPER( CC3RandomMortalParticleNavigator );
public:
	CC3HoseParticleNavigator();
	~CC3HoseParticleNavigator();

	static CC3HoseParticleNavigator*	navigator();
	/**
	 * The nozzle of the emitter.
	 *
	 * Particles are emitted at the origin of the nozzle and in the direction that the nozzle's
	 * forwardDirection property points (both in the local coordinate system of the nozzle node).
	 *
	 * The location and rotation of the nozzle node determine where the particles will be emitted, and
	 * in what direction, respectively. Moving the nozzle does not effect the movement of the particles
	 * that have already been emitted.
	 *
	 * By assigning the nozzle to a parent node, you can have the nozzle move with that node, and emit
	 * particles as that node travels. For example, you might attach the nozzle to the tail of a rocket
	 * node, to emit a trail of particles behind the rocket as the rocket moves.
	 *
	 * The initial value of this property is a newly-created CC3Node instance that is a child node of
	 * the emitter. However, you can change the parent node of the nozzle to some other object, by
	 * invoking the addChild: method on the other object, with the nozzle node as the argument.
	 *
	 * The parent of the nozzle does not need to be a child or descendant of the emitter. Like any node,
	 * the location and rotation properties of the nozzle are specified relative to its parent (the rocket).
	 *
	 * You can also set the nozzle to any other CC3Node instance in the scene by setting this property. 
	 */
	CC3Node*					getNozzle();
	void						setNozzle( CC3Node* nozzle );

	/**
	 * Indicates the angle of dispersion of the spray from the nozzle. This is specified as both a
	 * width and height, permitting the nozzle to have flexible shapes.
	 *
	 * During the emission of each particle, a random emission direction is chosen, within the angles
	 * specified by this property, relative to the forwardDirection of the nozzle node.
	 *
	 * The values are specified in degrees between zero and 180. The lower the angle, the tighter the stream.
	 *
	 * A different value can be specified for each of the width and height of the nozzle opening. Setting
	 * both width and height to small angles will create a tightly focused beam of particles. Setting both
	 * width and height to larger angles will result in particles emitted in a wide spray. Setting one or
	 * other of the width or height to a small angle and the other to a large angle will create a fan effect,
	 * where the particles are tightly constrained in one dimension, but spray widely in the other.
	 *
	 * The value of the shouldPrecalculateNozzleTangents property is affected by the setting of this
	 * property. When setting this property, if both components of the new value are less than 90 degrees,
	 * the value of the shouldPrecalculateNozzleTangents will automatically be set to YES, otherwise it
	 * will automatically be set to NO.
	 * 
	 * For small dispersion angles (< 90 degrees), it is possible to avoid two expensive tangent
	 * calculations every time a particle is emitted by precalculating the tangents of the dispersionAngle,
	 * and then randomizing on the value of the tangents instead of randomizing the value of the angle.
	 * For small angles, the effect is effectively the same. But for larger angles (approaching 180),
	 * randomizing the tangents has a very different effect than randomizing the emission angle. For this
	 * reason, tangent precalculation is automatically turned on for small angles (< 90 degrees) and off
	 * for larger angles. You can override this by setting the shouldPrecalculateNozzleTangents property
	 * directly, after you have set this dispersionAngle property.
	 */
	CCSize						getDispersionAngle();
	void						setDispersionAngle( const CCSize& angle );

	/**
	 * Indicates whether the emitter should precalculate tangent values for the dispersion angles, and
	 * then select a value from that range of tangents in order to determine a random direction for a particle.
	 *
	 * During the emission of each particle, a random emission direction is chosen within the angles
	 * specified by the dispersionAngle property. In order to convert the angles to direction, a tangent
	 * calculation must be made for each of the two random angles.
	 *
	 * For small dispersion angles (typically < 90 degrees), calculating the tangent once for each of the
	 * width and height of the dispersionAngle property, and then selecting a random value from the range
	 * of tangents provides equivalent randomization to selecting a random angle and then calculating its
	 * tangent. But in the first case, a tangent is only calculated once, for the dispersion angle itself,
	 * instead of each time a random angle is chosen.
	 *
	 * However, as the dispersion angle increases (approaching 180 degrees), the equivalent tangent
	 * grows exponentially, and the tangent range become ever larger, ending at infinity at 180 degrees.
	 * Therefore, as the dispersion angle increases, selecting a random value from the tangent range
	 * results in most angles clustering around the limits, resulting in very poor randomization.
	 *
	 * If this property is set to NO, whenever a particle is emitted, a random angle will be chosen within
	 * the range defined by the dispersionAngle property, for each of the width and height. Tangents are
	 * then calculated, and the particle direction set.
	 *
	 * If this property is set to YES, the dispersionAngle property will be converted into tangents, and
	 * whenever a particle is emitted, a random tangent value will be chosen within the range of tangents,
	 * and the particle direction will be set from that, without having to calculate a tangent from an
	 * angle for each particle.
	 *
	 * Typically, you will not need to set this property directly. The value of this property is set
	 * during the setting of dispersionAngle property. When setting the dispersionAngle property, if
	 * both components of the new value are less than 90 degrees, the value of this property will
	 * automatically be set to YES, otherwise it will automatically be set to NO.
	 *
	 * You can override this by setting this shouldPrecalculateNozzleTangents property directly,
	 * after you have set the dispersionAngle property.
	 *
	 * One interesting use-case for setting this property directly is to create a planar radial spray.
	 * You can accomplish this by setting the dispersionAngle property to {180, 180} and THEN setting
	 * this property to YES. Doing so will result in a hose that sprays particles in all directions in
	 * the X-Y plane, in a 2D radial particle effect. This happens because the tangents are effectively
	 * infinite for 180 degree sprays and so all of the particle are locked to the X-Y plane. You can then
	 * target this emitter at the camera, and the 2D effect will always remain perpendicular to the camera.
	 * This is a cheap way of creating a visual explosion effect without having to involve all three dimensions.
	 */
	bool						shouldPrecalculateNozzleTangents();
	void						setShouldPrecalculateNozzleTangents( bool precalculate );

	/**
	 * Indicates the lower bound of the range from which the speed of the particle will be chosen.
	 *
	 * Whenever a particle is emitted, its speed is determined by choosing a random value between the
	 * values specified by the minParticleSpeed and maxParticleSpeed properties. This speed value is
	 * then combined with the randomized initial direction to form the initial velocity of the particle.
	 */
	GLfloat						getMinParticleSpeed();
	void						setMinParticleSpeed( GLfloat speed );

	/**
	 * Indicates the upper bound of the range from which the speed of the particle will be chosen.
	 *
	 * Whenever a particle is emitted, its speed is determined by choosing a random value between the
	 * values specified by the minParticleSpeed and maxParticleSpeed properties. This speed value is
	 * then combined with the randomized initial direction to form the initial velocity of the particle.
	 */
	GLfloat						getMaxParticleSpeed();
	void						setMaxParticleSpeed( GLfloat speed );

	/**
	 * The matrix used to transform the initial location and velocity (combining direction and speed) of
	 * each particle from the local coordinates of the nozzle to the local coordinates of the emitter.
	 *
	 * If the nozzle has been assigned a different parent than the emitter, this matrix is recalculated
	 * during each update by combining the globalTransformMatrix of the nozzle and the globalTransformMatrixInverted
	 * of the emitter.
	 */
	CC3Matrix*					getNozzleMatrix();

	void						setEmitter( CC3ParticleEmitter* anEmitter );

	/** If the nozzle does not have a parent, add it to the emitter. */
	void						checkNozzleParent();

	// Protected property for copying
	CCSize						getNozzleShape();

	virtual bool				init();

	void						populateFrom( CC3HoseParticleNavigator* another );
	virtual CCObject*			copyWithZone( CCZone* pZone );

	virtual void				nodeWasTransformed( CC3Node* aNode );
	virtual void				nodeWasDestroyed( CC3Node* aNode );

	/**
	 * Determines the particle's emission location and direction in terms of the local coordinates
	 * of the nozzle, and then transforms each of these to the local coordinate system of the emitter.
	 */
	virtual void				initializeParticle( CC3Particle* aParticle );

protected:
	CC3Node*					m_pNozzle;
	CC3Matrix*					m_nozzleMatrix;
	CCSize						m_nozzleShape;
	GLfloat						m_minParticleSpeed;
	GLfloat						m_maxParticleSpeed;
	bool						m_shouldPrecalculateNozzleTangents : 1;
};

NS_COCOS3D_END

#endif
