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
#include "cocos3d.h"

NS_COCOS3D_BEGIN

void CC3MortalPointParticle::setLifeSpan( GLfloat anInterval )
{
	_lifeSpan = anInterval;
	_timeToLive = _lifeSpan;
}

GLfloat CC3MortalPointParticle::getLifeSpan()
{
	return _lifeSpan;
}

GLfloat CC3MortalPointParticle::getTimeToLive()
{
	return _timeToLive;
}

void CC3MortalPointParticle::updateBeforeTransform( CC3NodeUpdatingVisitor* visitor )
{
	GLfloat dt = visitor->getDeltaTime();
	_timeToLive -= dt;
	if (_timeToLive <= 0.0f)
		setIsAlive( false );
	//else
	//	[((id<CC3MortalPointParticleDeprecated>)self) updateLife: dt];
}

void CC3MortalPointParticle::populateFrom( CC3MortalPointParticle* another )
{
	super::populateFrom( another );
	
	_lifeSpan = another->getLifeSpan();
	_timeToLive = another->getTimeToLive();
}

CCObject* CC3MortalPointParticle::copyWithZone( CCZone* zone )
{
	CC3MortalPointParticle* pVal = new CC3MortalPointParticle;
	pVal->init();
	pVal->populateFrom( this );

	return pVal;
}

std::string CC3MortalPointParticle::fullDescription()
{
	return stringWithFormat( (char*)"%s\n\tlifeSpan: %.3f, timeToLive: %.3f",
			super::fullDescription().c_str(), _lifeSpan, _timeToLive );
}

void CC3SprayPointParticle::updateBeforeTransform( CC3NodeUpdatingVisitor* visitor )
{
	super::updateBeforeTransform( visitor );

	if( !isAlive() ) 
		return;

	setLocation( CC3VectorAdd(getLocation(), CC3VectorScaleUniform(getVelocity(), visitor->getDeltaTime())) );
}

bool CC3SprayPointParticle::init()
{
	if ( super::init() ) 
	{
		_velocity = kCC3VectorZero;
		return true;
	}

	return false;
}

void CC3SprayPointParticle::populateFrom( CC3SprayPointParticle* another )
{
	super::populateFrom( another );
	_velocity = another->getVelocity();
}

CCObject* CC3SprayPointParticle::copyWithZone( CCZone* zone )
{
	CC3SprayPointParticle* pVal = new CC3SprayPointParticle;
	pVal->init();
	pVal->populateFrom( this );

	return pVal;
}

std::string CC3SprayPointParticle::fullDescription()
{
	return stringWithFormat( (char*)"%s\n\tvelocity: %s",
			super::fullDescription().c_str(), stringFromCC3Vector(_velocity).c_str() );
}

CC3Vector CC3SprayPointParticle::getVelocity()
{
	return _velocity;
}

void CC3SprayPointParticle::setVelocity( const CC3Vector& vel )
{
	_velocity = vel;
}

void CC3UniformlyEvolvingPointParticle::updateBeforeTransform( CC3NodeUpdatingVisitor* visitor )
{
	super::updateBeforeTransform( visitor );

	if( !isAlive() ) 
		return;
	
	GLfloat dt = visitor->getDeltaTime();
	
	if ( hasSize() )
		setSize( getSize() + (_sizeVelocity * dt) );
	
	if ( hasColor() )
	{
		// We have to do the math on each component instead of using the color math functions
		// because the functions clamp prematurely, and we need negative values for the velocity.
		ccColor4F currColor = getColor4F();
		setColor4F( ccc4f(CLAMP(currColor.r + (_colorVelocity.r * dt), 0.0f, 1.0f),
					CLAMP(currColor.g + (_colorVelocity.g * dt), 0.0f, 1.0f),
					CLAMP(currColor.b + (_colorVelocity.b * dt), 0.0f, 1.0f),
					CLAMP(currColor.a + (_colorVelocity.a * dt), 0.0f, 1.0f)) 
		);
	}
}

void CC3UniformlyEvolvingPointParticle::populateFrom( CC3UniformlyEvolvingPointParticle* another )
{
	super::populateFrom( another );
	
	_sizeVelocity = another->getSizeVelocity();
	_colorVelocity = another->getColorVelocity();
}

CCObject* CC3UniformlyEvolvingPointParticle::copyWithZone( CCZone* zone )
{
	CC3UniformlyEvolvingPointParticle* pVal = new CC3UniformlyEvolvingPointParticle;
	pVal->init();
	pVal->populateFrom( this );

	return pVal;
}

void CC3UniformlyEvolvingPointParticle::setSizeVelocity( GLfloat velocity )
{
	_sizeVelocity = velocity;
}

GLfloat CC3UniformlyEvolvingPointParticle::getSizeVelocity()
{
	return _sizeVelocity;
}

void CC3UniformlyEvolvingPointParticle::setColorVelocity( const ccColor4F& colorVel )
{
	_colorVelocity = colorVel;
}

ccColor4F CC3UniformlyEvolvingPointParticle::getColorVelocity()
{
	return _colorVelocity;
}	

void CC3VariegatedPointParticle::initializeParticle()
{
	super::initializeParticle();

	ccColor4F colVel = getColorVelocity();
	setColorVelocity( ccc4f(colVel.r / _lifeSpan,
							colVel.g / _lifeSpan,
							colVel.b / _lifeSpan,
							colVel.a / _lifeSpan)
	);
	
	GLfloat sizeVel = getSizeVelocity();
	sizeVel /= _lifeSpan;
	setSizeVelocity( sizeVel );
}

CC3VariegatedPointParticle* CC3VariegatedPointParticle::particle()
{
	CC3VariegatedPointParticle* pParticle = new CC3VariegatedPointParticle;
	pParticle->init();
	pParticle->autorelease();

	return pParticle;
}

CC3VariegatedPointParticleHoseEmitter::CC3VariegatedPointParticleHoseEmitter()
{

}

CC3VariegatedPointParticleHoseEmitter::~CC3VariegatedPointParticleHoseEmitter()
{

}

GLfloat CC3VariegatedPointParticleHoseEmitter::getMinParticleStartingSize()
{
	return _minParticleStartingSize;
}

void CC3VariegatedPointParticleHoseEmitter::setMinParticleStartingSize( GLfloat minSize )
{
	_minParticleStartingSize = minSize;
}

GLfloat CC3VariegatedPointParticleHoseEmitter::getMinParticleEndingSize()
{
	return _minParticleEndingSize;
}

void CC3VariegatedPointParticleHoseEmitter::setMinParticleEndingSize( GLfloat minSize )
{
	_minParticleEndingSize = minSize;
}

GLfloat CC3VariegatedPointParticleHoseEmitter::getMaxParticleStartingSize()
{
	return _maxParticleStartingSize;
}

void CC3VariegatedPointParticleHoseEmitter::setMaxParticleStartingSize( GLfloat maxSize )
{
	_maxParticleStartingSize = maxSize;
}

GLfloat CC3VariegatedPointParticleHoseEmitter::getMaxParticleEndingSize()
{
	return _maxParticleEndingSize;
}

void CC3VariegatedPointParticleHoseEmitter::setMaxParticleEndingSize( GLfloat maxSize )
{
	_maxParticleEndingSize = maxSize;
}

ccColor4F CC3VariegatedPointParticleHoseEmitter::getMinParticleStartingColor()
{
	return _minParticleStartingColor;
}

void CC3VariegatedPointParticleHoseEmitter::setMinParticleStartingColor( const ccColor4F& color )
{
	_minParticleStartingColor = color;
}

ccColor4F CC3VariegatedPointParticleHoseEmitter::getMinParticleEndingColor()
{
	return _minParticleEndingColor;
}

void CC3VariegatedPointParticleHoseEmitter::setMinParticleEndingColor( const ccColor4F& color )
{
	_minParticleEndingColor = color;
}

ccColor4F CC3VariegatedPointParticleHoseEmitter::getMaxParticleEndingColor()
{
	return _maxParticleEndingColor;
}

void CC3VariegatedPointParticleHoseEmitter::setMaxParticleEndingColor( const ccColor4F& color )
{
	_maxParticleEndingColor = color;
}

ccColor4F CC3VariegatedPointParticleHoseEmitter::getMaxParticleStartingColor()
{
	return _maxParticleStartingColor;
}

void CC3VariegatedPointParticleHoseEmitter::setMaxParticleStartingColor( const ccColor4F& color )
{
	_maxParticleStartingColor = color;
}

void CC3VariegatedPointParticleHoseEmitter::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );

	setParticleNavigator( CC3HoseParticleNavigator::navigator() );

	_minParticleStartingSize = kCC3DefaultParticleSize;
	_maxParticleStartingSize = kCC3DefaultParticleSize;
	_minParticleEndingSize = kCC3DefaultParticleSize;
	_maxParticleEndingSize = kCC3DefaultParticleSize;
	_minParticleStartingColor = kCCC4FWhite;
	_maxParticleStartingColor = kCCC4FWhite;
	_minParticleEndingColor = kCCC4FWhite;
	_maxParticleEndingColor = kCCC4FWhite;
}

void CC3VariegatedPointParticleHoseEmitter::populateFrom( CC3VariegatedPointParticleHoseEmitter* another )
{
	super::populateFrom( another );
	
	_minParticleStartingSize = another->getMinParticleStartingSize();
	_maxParticleStartingSize = another->getMaxParticleStartingSize();
	_minParticleEndingSize = another->getMinParticleEndingSize();
	_maxParticleEndingSize = another->getMaxParticleEndingSize();
	_minParticleStartingColor = another->getMinParticleStartingColor();
	_maxParticleStartingColor = another->getMaxParticleStartingColor();
	_minParticleEndingColor = another->getMinParticleEndingColor();
	_maxParticleEndingColor = another->getMaxParticleEndingColor();
}

CCObject* CC3VariegatedPointParticleHoseEmitter::copyWithZone( CCZone* zone )
{
	CC3VariegatedPointParticleHoseEmitter* pVal = new CC3VariegatedPointParticleHoseEmitter;
	pVal->init();
	pVal->populateFrom( this );
	pVal->addCopiesOfChildrenFrom( this );
	
	return pVal;
}

/** Returns a random number between min and max, or returns alt if either min or max is negative. */
#define CC3RandomOrAlt(min, max, alt) (((min) >= 0.0f && (max) >= 0.0f) ? CC3RandomFloatBetween((min), (max)) : (alt))

void CC3VariegatedPointParticleHoseEmitter::initializeParticle( CC3Particle* aParticle )
{
	super::initializeParticle( aParticle );
	
	// Set the particle's initial color and color velocity, which is calculated by taking the
	// difference of the start and end colors. This assumes that the color changes over one second.
	// The particle itself will figure out how the overall change should be adjusted for its lifespan.
	if ( getMesh()->hasVertexColors() )
	{
		ccColor4F startColor = RandomCCC4FBetween(_minParticleStartingColor, _maxParticleStartingColor);
		aParticle->setColor4F( startColor );
		
		// End color is treated differently. If any component of either min or max is negative,
		// it indicates that the corresponding component of the start color should be used,
		// otherwise a random value between min and max is chosen.
		// This allows random colors to be chosen, but to have them stay constant.
		// For exmaple, setting all color components to -1 and alpha to zero, indicates that
		// the particle should stay the same color, but fade away.
		ccColor4F endColor;
		endColor.r = CC3RandomOrAlt(_minParticleEndingColor.r, _maxParticleEndingColor.r, startColor.r);
		endColor.g = CC3RandomOrAlt(_minParticleEndingColor.g, _maxParticleEndingColor.g, startColor.g);
		endColor.b = CC3RandomOrAlt(_minParticleEndingColor.b, _maxParticleEndingColor.b, startColor.b);
		endColor.a = CC3RandomOrAlt(_minParticleEndingColor.a, _maxParticleEndingColor.a, startColor.a);
		
		// We have to do the math on each component instead of using the color math functions
		// because the functions clamp prematurely, and we need negative values for the velocity.
		aParticle->setColorVelocity( ccc4f((endColor.r - startColor.r),
										(endColor.g - startColor.g),
										(endColor.b - startColor.b),
										(endColor.a - startColor.a)) );
	}
	
	// Set the particle's initial size and size velocity, which is calculated by taking the
	// difference of the start and end sizes. This assumes that the color changes over one second.
	// The particle itself will figure out how the overall change should be adjusted for its lifespan.
	if ( getMesh()->hasVertexPointSizes() )
	{
		GLfloat startSize = CC3RandomFloatBetween(_minParticleStartingSize, _maxParticleStartingSize);
		aParticle->setSize( startSize );
		
		// End size is treated differently. If either min or max is negative, it indicates that
		// the start size should be used, otherwise a random value between min and max is chosen.
		// This allows a random size to be chosen, but to have it stay constant.
		GLfloat endSize = CC3RandomOrAlt(_minParticleEndingSize, _maxParticleEndingSize, startSize);
		aParticle->setSizeVelocity( endSize - startSize );
	}
}

CC3Particle* CC3VariegatedPointParticleHoseEmitter::makeParticle()
{
	return CC3VariegatedPointParticle::particle();
}

CC3VariegatedPointParticleHoseEmitter* CC3VariegatedPointParticleHoseEmitter::nodeWithName( const std::string& aName )
{
	CC3VariegatedPointParticleHoseEmitter* pVal = new CC3VariegatedPointParticleHoseEmitter;
	pVal->initWithName( aName );
	pVal->autorelease();

	return pVal;
}

NS_COCOS3D_END
