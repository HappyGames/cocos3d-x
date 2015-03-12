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
#include "CC3ParticleSamples.h"
#include "../Matrices/CC3AffineMatrix.h"

NS_COCOS3D_BEGIN

void CC3RandomMortalParticleNavigator::setMaxParticleLifeSpan( GLfloat lifeSpan )
{
	_maxParticleLifeSpan = lifeSpan;
}

GLfloat CC3RandomMortalParticleNavigator::getMaxParticleLifeSpan()
{
	return _maxParticleLifeSpan;
}

void CC3RandomMortalParticleNavigator::setMinParticleLifeSpan( GLfloat lifeSpan )
{
	_minParticleLifeSpan = lifeSpan;
}

GLfloat CC3RandomMortalParticleNavigator::getMinParticleLifeSpan()
{
	return _minParticleLifeSpan;
}

bool CC3RandomMortalParticleNavigator::init()
{
	if ( super::init() )
	{
		_minParticleLifeSpan = 0.0f;
		_maxParticleLifeSpan = 0.0f;

		return true;
	}
	
	return false;
}

void CC3RandomMortalParticleNavigator::populateFrom( CC3RandomMortalParticleNavigator* another )
{
	super::populateFrom( another );
	
	_minParticleLifeSpan = another->getMinParticleLifeSpan();
	_maxParticleLifeSpan = another->getMaxParticleLifeSpan();
}

CCObject* CC3RandomMortalParticleNavigator::copyWithZone( CCZone* pZone )
{
	CC3RandomMortalParticleNavigator* pVal = new CC3RandomMortalParticleNavigator;
	pVal->init();
	pVal->populateFrom( this );

	return pVal;
}

void CC3RandomMortalParticleNavigator::initializeParticle( CC3Particle* aParticle )
{
	aParticle->setLifeSpan( CC3RandomFloatBetween(_minParticleLifeSpan, _maxParticleLifeSpan) );
}

/** Converts the angular components of the specified dispersion into tangents. */
static inline CCSize CC3ShapeFromDispersionAngle( const CCSize& anAngle ) 
{
	return CCSizeMake(tanf(CC3DegToRad(anAngle.width / 2.0f)),
					  tanf(CC3DegToRad(anAngle.height / 2.0f)));
}

/** Converts the tangential components of the specified aspect into dispersion angles. */
static inline CCSize CC3DispersionAngleFromShape( const CCSize& anAspect )
{
	return CCSizeMake(CC3RadToDeg(2.0 * atanf(anAspect.width)),
					  CC3RadToDeg(2.0 * atanf(anAspect.height)));
}

CC3HoseParticleNavigator::CC3HoseParticleNavigator()
{
	_nozzle = NULL;
	_nozzleMatrix = NULL;
}

CC3HoseParticleNavigator::~CC3HoseParticleNavigator()
{
	setNozzle( NULL );			// Setter clears listener and releases nozzle
	CC_SAFE_RELEASE( _nozzleMatrix );
}

CC3HoseParticleNavigator* CC3HoseParticleNavigator::navigator()
{
	CC3HoseParticleNavigator* pVal = new CC3HoseParticleNavigator;
	pVal->init();
	pVal->autorelease();

	return pVal;
}

void CC3HoseParticleNavigator::setEmitter( CC3ParticleEmitter* anEmitter )
{
	super::setEmitter( anEmitter );
	checkNozzleParent();
}

CC3Node* CC3HoseParticleNavigator::getNozzle()
{
	return _nozzle; 
}

void CC3HoseParticleNavigator::setNozzle( CC3Node* aNode )
{
	if (aNode == _nozzle) 
		return;
	
	if ( _nozzle )
	{
		_nozzle->removeTransformListener( this );
		if (_nozzle->getParent() == _emitter) 
			_nozzle->remove();
	}
	
	CC_SAFE_RELEASE( _nozzle );
	_nozzle = aNode;
	CC_SAFE_RETAIN( aNode );
	
	if ( _nozzle )
		_nozzle->addTransformListener( this );

	checkNozzleParent();
}

/** If the nozzle does not have a parent, add it to the emitter. */
void CC3HoseParticleNavigator::checkNozzleParent()
{
	if ( _nozzle && !_nozzle->getParent() ) 
	{
		if ( _emitter )
		{
			if (!_emitter->getName().empty()) 
				_nozzle->setName( stringWithFormat( (char*)"%s-Nozzle", _emitter->getName().c_str() ) );
			_emitter->addChild( _nozzle );
		}
	}
}

// Protected property for copying
CCSize CC3HoseParticleNavigator::getNozzleShape()
{
	return _nozzleShape; 
}

CCSize CC3HoseParticleNavigator::getDispersionAngle()
{
	return _shouldPrecalculateNozzleTangents
				? CC3DispersionAngleFromShape(_nozzleShape)
				: _nozzleShape;
}

#define kCC3TangentPrecalcThreshold 90.0f

void CC3HoseParticleNavigator::setDispersionAngle( const CCSize& dispAngle )
{
	_shouldPrecalculateNozzleTangents = (dispAngle.width < kCC3TangentPrecalcThreshold &&
										dispAngle.height < kCC3TangentPrecalcThreshold);
	_nozzleShape = _shouldPrecalculateNozzleTangents ? CC3ShapeFromDispersionAngle(dispAngle) : dispAngle;
}

bool CC3HoseParticleNavigator::shouldPrecalculateNozzleTangents()
{
	return _shouldPrecalculateNozzleTangents;
}

/** If we're flipping from one to the other, convert the nozzleShape. */
void CC3HoseParticleNavigator::setShouldPrecalculateNozzleTangents( bool shouldPrecalc )
{
	if ( _shouldPrecalculateNozzleTangents && !shouldPrecalc ) 
		_nozzleShape = CC3DispersionAngleFromShape(_nozzleShape);
	else if ( !_shouldPrecalculateNozzleTangents && shouldPrecalc ) 
		_nozzleShape = CC3ShapeFromDispersionAngle(_nozzleShape);

	_shouldPrecalculateNozzleTangents = shouldPrecalc;
}

void CC3HoseParticleNavigator::setMaxParticleSpeed( GLfloat speed )
{
	_maxParticleSpeed = speed;
}

GLfloat CC3HoseParticleNavigator::getMaxParticleSpeed()
{
	return _maxParticleSpeed;
}

void CC3HoseParticleNavigator::setMinParticleSpeed( GLfloat speed )
{
	_minParticleSpeed = speed;
}

GLfloat CC3HoseParticleNavigator::getMinParticleSpeed()
{
	return _minParticleSpeed;
}

bool CC3HoseParticleNavigator::init()
{
	if ( super::init() ) 
	{
		setNozzle( CC3Node::node() );
		_nozzleMatrix = CC3AffineMatrix::matrix();				// retained
		_nozzleMatrix->retain();
		_shouldPrecalculateNozzleTangents = true;
		setDispersionAngle( CCSizeMake(15.0, 15.0) );		// Set after so it will precalc
		_minParticleSpeed = 0.0f;
		_maxParticleSpeed = 0.0f;

		return true;
	}

	return false;
}

void CC3HoseParticleNavigator::populateFrom( CC3HoseParticleNavigator* another )
{
	super::populateFrom( another );
	
	setNozzle( another->getNozzle() );
	_nozzleMatrix->populateFrom( another->getNozzleMatrix() );
	_nozzleShape = another->getNozzleShape();
	_minParticleSpeed = another->getMinParticleSpeed();
	_maxParticleSpeed = another->getMaxParticleSpeed();
	_shouldPrecalculateNozzleTangents = another->shouldPrecalculateNozzleTangents();
}

CCObject* CC3HoseParticleNavigator::copyWithZone( CCZone* pZone )
{
	CC3HoseParticleNavigator* pVal = new CC3HoseParticleNavigator;
	pVal->init();
	pVal->populateFrom( this );

	return pVal;
}

void CC3HoseParticleNavigator::nodeWasTransformed( CC3Node* aNode )
{
	if (aNode == _nozzle) 
	{
		if ( _nozzleMatrix )
			_nozzleMatrix->setIsDirty( true );
	}
}

void CC3HoseParticleNavigator::nodeWasDestroyed( CC3Node* aNode )
{

}

CC3Matrix* CC3HoseParticleNavigator::getNozzleMatrix()
{
	if (_nozzleMatrix->isDirty()) 
	{
		if ( _nozzle && _nozzle->getParent() != _emitter )
		{
			_nozzleMatrix->populateFrom( _nozzle->getGlobalTransformMatrix() );
			_nozzleMatrix->leftMultiplyBy( _emitter->getGlobalTransformMatrixInverted() );
		}
		else
		{
			_nozzleMatrix->populateIdentity();
		}

		_nozzleMatrix->setIsDirty( false );
	}

	return _nozzleMatrix;
}

/**
 * Determines the particle's emission location and direction in terms of the local coordinates
 * of the nozzle, and then transforms each of these to the local coordinate system of the emitter.
 */
void CC3HoseParticleNavigator::initializeParticle( CC3Particle* aParticle )
{
	super::initializeParticle( aParticle );
	
	// The particle starts at the location of the nozzle, converted from the
	// nozzle's local coordinate system to the emitter's local coordinate system.
	aParticle->setLocation( getNozzleMatrix()->transformLocation( kCC3VectorZero ) );
	
	// Speed of particle is randomized.
	GLfloat emissionSpeed = CC3RandomFloatBetween(_minParticleSpeed, _maxParticleSpeed);

	// Emission direction in the nozzle's local coordinate system is towards the negative
	// Z-axis, with randomization in the X & Y directions based on the shape of the nozzle.
	// Randomization is performed either on the dispersion angle, or on the tangents of the
	// dispersion angle, depending on the value of the shouldPrecalculateNozzleTangents.
	CCSize nozzleAspect = CCSizeMake(CC3RandomFloatBetween(-_nozzleShape.width, _nozzleShape.width),
									 CC3RandomFloatBetween(-_nozzleShape.height, _nozzleShape.height));
	if ( !_shouldPrecalculateNozzleTangents ) 
		nozzleAspect = CC3ShapeFromDispersionAngle(nozzleAspect);

	CC3Vector emissionDir = CC3VectorNormalize(cc3v(nozzleAspect.width, nozzleAspect.height, 1.0f));

	// The combination of emission speed and emission direction is the emission velocity in the
	// nozzle's local coordinates. The particle velocity is then the nozzle emission velocity
	// transformed by the nozzleMatrix to convert it to the emitter's local coordinates.
	CC3Vector emissionVelocity = CC3VectorScaleUniform(emissionDir, emissionSpeed);
	aParticle->setVelocity( getNozzleMatrix()->transformDirection( emissionVelocity ) );
}

NS_COCOS3D_END
