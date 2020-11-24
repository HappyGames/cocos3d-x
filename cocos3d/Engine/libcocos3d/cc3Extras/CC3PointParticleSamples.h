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
/** @file */	// Doxygen marker
#ifndef _CC3POINT_PARTICLE_SAMPLES_H_
#define _CC3POINT_PARTICLE_SAMPLES_H_

NS_COCOS3D_BEGIN

/**
 * To make evolutionary changes to this particle, implement the updateBeforeTransform: method.
 * In doing so, be sure to invoke the superclass implementation, which checks whether this
 * particle is still alive or has expired. Once the superclass implementation returns, you can
 * check the isAlive property before spending time making any further modifications.
 */
class CC3MortalPointParticle : public CC3PointParticle
{
	DECLARE_SUPER( CC3PointParticle );
public:
	/**
	 * This template callback method is invoked automatically whenever the emitter is updated
	 * during a scheduled 3D scene update.
	 *
	 * The CC3MortalPointParticle implementation checks to see if the whether this particle is
	 * still alive or has expired, and sets the isAlive property accordingly.
	 *
	 * You can override this method to update the evolution of the particle. You should invoke this
	 * superclass implementation and test the isAlive property before making any further modifications.
	 *
	 * Subclasses that override this method should invoke this superclass implementation first,
	 * and should check the isAlive property prior to making any further modifications..
	 */
	virtual void				updateBeforeTransform( CC3NodeUpdatingVisitor* visitor );

	virtual void				setLifeSpan( GLfloat anInterval );
	virtual GLfloat				getLifeSpan();
	virtual GLfloat				getTimeToLive();

	virtual void				populateFrom( CC3MortalPointParticle* another );
	virtual CCObject*			copyWithZone( CCZone* zone );

	virtual std::string			fullDescription();

protected:
	GLfloat						m_lifeSpan;
	GLfloat						m_timeToLive;
};

/**
 * CC3SprayPointParticle is a type of CC3MortalPointParticle that implements the
 * CC3SprayParticleProtocol to configure the particle to move in a straight line at a steady speed.
 */
class CC3SprayPointParticle : public CC3MortalPointParticle
{
	DECLARE_SUPER( CC3MortalPointParticle );
public:
	/**
	 * This template callback method is invoked automatically whenever the emitter is updated
	 * during a scheduled 3D scene update.
	 *
	 * The direction and speed are specified by the velocity property. To produce uniform motion,
	 * this method multiplies this velocity by the interval since the previous update, and the
	 * resulting distance vector is added to the location of this particle
	 *
	 * Subclasses that override this method should invoke this superclass implementation first,
	 * and should check the isAlive property prior to making any further modifications..
	 */
	virtual void				updateBeforeTransform( CC3NodeUpdatingVisitor* visitor );
	virtual bool				init();
	
	virtual void				populateFrom( CC3SprayPointParticle* another );
	virtual CCObject*			copyWithZone( CCZone* zone );

	virtual std::string			fullDescription();
	virtual CC3Vector			getVelocity();
	virtual void				setVelocity( const CC3Vector& vel );

protected:
	CC3Vector					m_velocity;
};

/**
 * CC3UniformlyEvolvingPointParticle is a type of CC3SprayPointParticle that implements the
 * CC3UniformlyGrowingPointParticleProtocol and CC3UniformlyFadingParticleProtocol protocols
 * to configure steadily changing color and size that vary linearly from an intitial color
 * and size to a final color and size.
 *
 * The rate of change of the particle's color and size are specified by the colorVelocity 
 * and sizeVelocity properties respectively.
 *
 * To produce uniform evolution, the updateBeforeTransform: method multiplies each of these
 * velocities by the interval since the previous update, and adds each result, accordingly,
 * to the color and size properties of this particle. Color and size are only updated if the
 * underlying mesh supports that content.
 */
class CC3UniformlyEvolvingPointParticle : public CC3SprayPointParticle
{
	DECLARE_SUPER( CC3SprayPointParticle );
public:
	/**
	 * This template callback method is invoked automatically whenever the emitter is updated
	 * during a scheduled 3D scene update.
	 *
	 * The direction and speed are specified by the velocity property. The rate of change of the
	 * particle's color and size are specified by the colorVelocity and sizeVelocity properties respectively.
	 *
	 * To produce uniform evolution, this method multiplies each of these three velocities by the interval
	 * since the previous update, and adds each result, accordingly, to the location, color and size
	 * properties of this particle. Color and size are only updated if this particle supports that content.
	 *
	 * Subclasses that override this method should invoke this superclass implementation first,
	 * and should check the isAlive property prior to making any further modifications..
	 */
	virtual void				updateBeforeTransform( CC3NodeUpdatingVisitor* visitor );

	virtual void				populateFrom( CC3UniformlyEvolvingPointParticle* another );
	virtual CCObject*			copyWithZone( CCZone* zone );

	virtual GLfloat				getSizeVelocity();
	virtual void				setSizeVelocity( GLfloat velocity );
	virtual ccColor4F			getColorVelocity();
	virtual void				setColorVelocity( const ccColor4F& colorVel );

protected:
	ccColor4F					m_colorVelocity;
	GLfloat						m_sizeVelocity;
};

/**
 * CC3VariegatedPointParticle is the type of particle emitted by a CC3VariegatedPointParticleHoseEmitter.
 * It supports the CC3VariegatedPointParticleProtocol and allows the emitter to configure the
 * particle with a steadily changing color and size, based on the lifespan of the particle.
 */
class CC3VariegatedPointParticle : public CC3UniformlyEvolvingPointParticle
{
	DECLARE_SUPER( CC3UniformlyEvolvingPointParticle );
public:
	/**
	 * Adjusts the velocity of the color and point size by dividing by the life span.
	 *
	 * This is done here, because the color and size velocity is determined by the
	 * CC3VariegatedPointParticleHoseEmitter that created the particle, but the lifeSpan is
	 * not available until the navigator sets it. Since the emitter initializes the particle 
	 * first, then the navigator, and then the particle itself, we can get the particle to
	 * adjust the velocity now that the lifeSpan is known.
	 *
	 * An alternative to this process could be to have the navigator determine the color and
	 * size velocities. It is done this way here to highlight the interaction between the
	 * three levels of initialization. This also allows the navigator to focus on the particle's
	 * path, and the emitter to focus on the visuals, and lets the particle itself stitch it
	 * all together as needed for any particular application.
	 */
	virtual void				initializeParticle();

	static CC3VariegatedPointParticle*	particle();
};

/**
 * When used as an ending size or component of an ending color for a
 * CC3VariegatedPointParticleHoseEmitter, indicates that that component
 * should stay constant at the value at which it started.
 */
#define kCC3ParticleConstantComponent	-1.0

/**
 * When used as the ending size for a CC3VariegatedPointParticleHoseEmitter,
 * indicates that the size should stay constant at the starting size.
 */
static const GLfloat kCC3ParticleConstantSize = kCC3ParticleConstantComponent;

/**
 * When used as the ending color for a CC3VariegatedPointParticleHoseEmitter,
 * indicates that the color should stay constant at the starting color.
 */
static const ccColor4F kCC3ParticleConstantColor = { kCC3ParticleConstantComponent,
													 kCC3ParticleConstantComponent,
													 kCC3ParticleConstantComponent,
													 kCC3ParticleConstantComponent };

/**
 * When used as the ending color for a CC3VariegatedPointParticleHoseEmitter, indicates
 * that the starting color should simply fade out, rather than change to an ending color.
 */
static const ccColor4F kCC3ParticleFadeOut = { kCC3ParticleConstantComponent,
											   kCC3ParticleConstantComponent,
											   kCC3ParticleConstantComponent,
											   0.0f };

/**
 * CC3VariegatedPointParticleHoseEmitter is a type of CC3PointParticleEmitter whose particles
 * can have a color and size that evolves during the lifetime of the particle.
 *
 * CC3VariegatedPointParticleHoseEmitter configures particles of that support the
 * CC3VariegatedPointParticleProtocol, and can set an individual initial and final color
 * and size for each particle, each selected randomly from a range of values.
 */
class CC3VariegatedPointParticleHoseEmitter : public CC3PointParticleEmitter 
{
	DECLARE_SUPER( CC3PointParticleEmitter );
public:
	CC3VariegatedPointParticleHoseEmitter();
	~CC3VariegatedPointParticleHoseEmitter();

	/**
	 * Indicates the lower bound of the range from which the initial size of the particle will be chosen.
	 *
	 * Whenever a particle is emitted, its starting size is determined by choosing a random value between
	 * the values specified by the minParticleStartingSize and maxParticleStartingSize properties.
	 */
	virtual GLfloat				getMinParticleStartingSize();
	virtual void				setMinParticleStartingSize( GLfloat minSize );

	/**
	 * Indicates the upper bound of the range from which the initial size of the particle will be chosen.
	 *
	 * Whenever a particle is emitted, its starting size is determined by choosing a random value between
	 * the values specified by the minParticleStartingSize and maxParticleStartingSize properties.
	 */
	virtual GLfloat				getMaxParticleStartingSize();
	virtual void				setMaxParticleStartingSize( GLfloat maxSize );

	/**
	 * Indicates the lower bound of the range from which the final size of the particle will be chosen.
	 *
	 * Whenever a particle is emitted, its final size is determined by choosing a random value between
	 * the values specified by the minParticleEndingSize and maxParticleEndingSize properties. This is
	 * used to determine the rate at which the size will change while the particle is alive, and the
	 * result is set into the sizeVelocity property of the particle.
	 *
	 * In addition to a specific size value, you can use the special value kCC3ParticleConstantSize to
	 * indicate that the final size of the particle should be the same as the starting size. Using this
	 * value for either minParticleEndingSize or maxParticleEndingSize will allow the starting size to
	 * be set randomly and to stay constant throughout the life of the particle.
	 */
	virtual GLfloat				getMinParticleEndingSize();
	virtual void				setMinParticleEndingSize( GLfloat minSize );

	/**
	 * Indicates the upper bound of the range from which the final size of the particle will be chosen.
	 *
	 * Whenever a particle is emitted, its final size is determined by choosing a random value between
	 * the values specified by the minParticleEndingSize and maxParticleEndingSize properties. This is
	 * used to determine the rate at which the size will change while the particle is alive, and the
	 * result is set into the sizeVelocity property of the particle.
	 *
	 * In addition to a specific size value, you can use the special value kCC3ParticleConstantSize to
	 * indicate that the final size of the particle should be the same as the starting size. Using this
	 * value for either minParticleEndingSize or maxParticleEndingSize will allow the starting size to
	 * be set randomly and to stay constant throughout the life of the particle.
	 */
	virtual GLfloat				getMaxParticleEndingSize();
	virtual void				setMaxParticleEndingSize( GLfloat maxSize );

	/**
	 * Indicates the lower bound of the range from which the initial color of the particle will be chosen.
	 *
	 * Whenever a particle is emitted, its starting color is determined by choosing a random value between
	 * the values specified by the minParticleStartingColor and maxParticleStartingColor properties. The
	 * color is randomized by choosing a random value for each component from the numerical range defined
	 * by the value of that component in the minParticleStartingColor and maxParticleStartingColor properties.
	 */
	virtual ccColor4F			getMinParticleStartingColor();
	virtual void				setMinParticleStartingColor( const ccColor4F& color );

	/**
	 * Indicates the upper bound of the range from which the initial color of the particle will be chosen.
	 *
	 * Whenever a particle is emitted, its starting color is determined by choosing a random value between
	 * the values specified by the minParticleStartingColor and maxParticleStartingColor properties. The
	 * color is randomized by choosing a random value for each component from the numerical range defined
	 * by the value of that component in the minParticleStartingColor and maxParticleStartingColor properties.
	 */
	virtual ccColor4F			getMaxParticleStartingColor();
	virtual void				setMaxParticleStartingColor( const ccColor4F& color );

	/**
	 * Indicates the lower bound of the range from which the final color of the particle will be chosen.
	 *
	 * Whenever a particle is emitted, its starting color is determined by choosing a random value between
	 * the values specified by the minParticleStartingColor and maxParticleStartingColor properties. The
	 * color is randomized by choosing a random value for each component from the numerical range defined
	 * by the value of that component in the minParticleStartingColor and maxParticleStartingColor properties.
	 *
	 * This final color is used to determine the rate at which the color will change while the particle
	 * is alive, and the result is set into the colorVelocity property of the particle.
	 *
	 * In addition to a specific final color value, you can use the special values:
	 *   - kCC3ParticleConstantColor
	 *   - kCC3ParticleFadeOut
	 * to indicate, respectively, that the final color of the particle should be the same as the starting
	 * color, or that the final color should be the same as the starting color, except that it should fade
	 * away during the lifetime of the particle.
	 *
	 * In a more general sense, setting any of the component values of either the minParticleEndingColor
	 * or maxParticleEndingColor properties to kCC3ParticleConstantComponent will cause the value of that
	 * component to stay constant throughout the lifetime of the particle.
	 */
	virtual ccColor4F			getMinParticleEndingColor();
	virtual void				setMinParticleEndingColor( const ccColor4F& color );


	/**
	 * Indicates the upper bound of the range from which the final color of the particle will be chosen.
	 *
	 * Whenever a particle is emitted, its starting color is determined by choosing a random value between
	 * the values specified by the minParticleStartingColor and maxParticleStartingColor properties. The
	 * color is randomized by choosing a random value for each component from the numerical range defined
	 * by the value of that component in the minParticleStartingColor and maxParticleStartingColor properties.
	 *
	 * This final color is used to determine the rate at which the color will change while the particle
	 * is alive, and the result is set into the colorVelocity property of the particle.
	 *
	 * In addition to a specific final color value, you can use the special values:
	 *   - kCC3ParticleConstantColor
	 *   - kCC3ParticleFadeOut
	 * to indicate, respectively, that the final color of the particle should be the same as the starting
	 * color, or that the final color should be the same as the starting color, except that it should fade
	 * away during the lifetime of the particle.
	 *
	 * In a more general sense, setting any of the component values of either the minParticleEndingColor
	 * or maxParticleEndingColor properties to kCC3ParticleConstantComponent will cause the value of that
	 * component to stay constant throughout the lifetime of the particle.
	 */
	virtual ccColor4F			getMaxParticleEndingColor();
	virtual void				setMaxParticleEndingColor( const ccColor4F& color );

	virtual void				initWithTag( GLuint aTag, const std::string& aName );

	void						populateFrom( CC3VariegatedPointParticleHoseEmitter* another );
	virtual CCObject*			copyWithZone( CCZone* zone );

	void						initializeParticle( CC3Particle* aParticle );

	virtual CC3Particle*		makeParticle();

	static CC3VariegatedPointParticleHoseEmitter* nodeWithName( const std::string& aName );

protected:
	GLfloat						m_minParticleStartingSize;
	GLfloat						m_maxParticleStartingSize;
	GLfloat						m_minParticleEndingSize;
	GLfloat						m_maxParticleEndingSize;
	ccColor4F					m_minParticleStartingColor;
	ccColor4F					m_maxParticleStartingColor;
	ccColor4F					m_minParticleEndingColor;
	ccColor4F					m_maxParticleEndingColor;
};

NS_COCOS3D_END

#endif
