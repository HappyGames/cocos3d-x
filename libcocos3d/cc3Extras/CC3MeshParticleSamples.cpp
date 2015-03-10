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
#include "CC3MeshParticleSamples.h"
#include "../Utility/CC3CC2Extensions.h"

NS_COCOS3D_BEGIN

void CC3MortalMeshParticle::setLifeSpan( GLfloat anInterval )
{
	_lifeSpan = anInterval;
	_timeToLive = _lifeSpan;
}

void CC3MortalMeshParticle::updateBeforeTransform( CC3NodeUpdatingVisitor* visitor )
{
	_timeToLive -= visitor->getDeltaTime();
	if (_timeToLive <= 0.0f) 
		setIsAlive( false );
}

std::string CC3MortalMeshParticle::fullDescription()
{
	return stringWithFormat( (char*)"%s\n\tlifeSpan: %.3f, timeToLive: %.3f",
			super::fullDescription().c_str(), _lifeSpan, _timeToLive );
}

bool CC3MortalMeshParticle::init()
{
	if ( super::init() )
	{
		_lifeSpan = 0.0f;
		_timeToLive = 0.0f;

		return true;
	}

	return false;
}

void CC3MortalMeshParticle::populateFrom( CC3MortalMeshParticle* another )
{
	super::populateFrom( another );
	
	_lifeSpan = another->getLifeSpan();
	_timeToLive = another->getTimeToLive();
}

CCObject* CC3MortalMeshParticle::copyWithZone( CCZone* zone )
{
	CC3MortalMeshParticle* pVal = new CC3MortalMeshParticle;
	pVal->init();
	pVal->populateFrom( this );

	return pVal;
}

void CC3SprayMeshParticle::updateBeforeTransform( CC3NodeUpdatingVisitor* visitor )
{
	super::updateBeforeTransform( visitor );
	if( !isAlive() ) 
		return;

	setLocation( CC3VectorAdd(getLocation(), CC3VectorScaleUniform(getVelocity(), visitor->getDeltaTime())) );
}

bool CC3SprayMeshParticle::init()
{
	if ( super::init() ) 
	{
		_velocity = kCC3VectorZero;
		return true;
	}

	return false;
}

void CC3SprayMeshParticle::populateFrom( CC3SprayMeshParticle* another )
{
	super::populateFrom( another );
 	_velocity = another->getVelocity();
}

CCObject* CC3SprayMeshParticle::copyWithZone( CCZone* zone )
{
	CC3SprayMeshParticle* pVal = new CC3SprayMeshParticle;
	pVal->init();
	pVal->populateFrom( this );

	return pVal;
}

std::string  CC3SprayMeshParticle::fullDescription()
{
	return stringWithFormat( (char*)"%s\n\tvelocity: %s",
			super::fullDescription().c_str(), stringFromCC3Vector(_velocity).c_str() );
}

CC3Vector CC3UniformlyEvolvingMeshParticle::getRotationVelocity()
{
	switch (_rotationVelocityType) 
	{
		case kCC3RotationTypeAxisAngle: 
		{
			CC3Vector4 axisAngle = CC3Vector4FromCC3Vector(getRotationAxis(), getRotationAngleVelocity());
			return CC3RotationFromQuaternion(CC3QuaternionFromAxisAngle(axisAngle));
		}
		default:
			return _rotationVelocity;
	}
}

void CC3UniformlyEvolvingMeshParticle::setRotationVelocity( const CC3Vector& aVector )
{
	_rotationVelocity = aVector;
	_rotationVelocityType = kCC3RotationTypeEuler;
}

GLfloat CC3UniformlyEvolvingMeshParticle::getRotationAngleVelocity()
{
	switch (_rotationVelocityType) 
	{
		case kCC3RotationTypeEuler: 
		{
			CC3Quaternion quat = CC3QuaternionFromRotation(getRotationVelocity());
			CC3Vector4 axisAngle = CC3AxisAngleFromQuaternion(quat);
			return axisAngle.w;
		}
		default:
			return _rotationVelocity.x;
	}
}

void CC3UniformlyEvolvingMeshParticle::setRotationAngleVelocity( GLfloat anAngle )
{
	_rotationVelocity = cc3v(anAngle, anAngle, anAngle);
	_rotationVelocityType = kCC3RotationTypeAxisAngle;
}

void CC3UniformlyEvolvingMeshParticle::updateBeforeTransform( CC3NodeUpdatingVisitor* visitor )
{
	super::updateBeforeTransform( visitor );
	if( !isAlive() ) 
		return;
	
	GLfloat dt = visitor->getDeltaTime();
	
	switch (_rotationVelocityType) 
	{
		case kCC3RotationTypeEuler: 
		{
			CC3Vector rotVel = getRotationVelocity();
			if ( !CC3VectorIsZero(rotVel) ) 
				rotateBy( CC3VectorScaleUniform(rotVel, dt) );
			break;
		}
		case kCC3RotationTypeAxisAngle: 
		{
			GLfloat rotAngVel = getRotationAngleVelocity();
			if (rotAngVel != 0.0f) 
			{
				GLfloat currAngle = getRotationAngle();
				currAngle += rotAngVel * dt;
				setRotationAngle( currAngle );
			}
			break;
		}
		default:
			break;
	}
	
	if ( hasColor() && !CCC4FAreEqual(_colorVelocity, kCCC4FBlackTransparent) ) 
	{
		// We have to do the math on each component instead of using the color math functions
		// because the functions clamp prematurely, and we need negative values for the velocity.
		ccColor4F currColor = getColor4F();
		ccColor4F newColor = ccc4f(CLAMP(currColor.r + (_colorVelocity.r * dt), 0.0f, 1.0f),
								   CLAMP(currColor.g + (_colorVelocity.g * dt), 0.0f, 1.0f),
								   CLAMP(currColor.b + (_colorVelocity.b * dt), 0.0f, 1.0f),
								   CLAMP(currColor.a + (_colorVelocity.a * dt), 0.0f, 1.0f));
		setColor4F( newColor );
		/*LogTrace(@"Updating color of %@ from %@ to %@", self,
					  NSStringFromCCC4F(currColor), NSStringFromCCC4F(newColor));*/
	}
	
}


bool CC3UniformlyEvolvingMeshParticle::init()
{
	if ( super::init() ) 
	{
		_rotationVelocity = kCC3VectorZero;
		_rotationVelocityType = kCC3RotationTypeUnknown;
		_colorVelocity = kCCC4FBlackTransparent;

		return true;
	}

	return false;
}

void CC3UniformlyEvolvingMeshParticle::populateFrom( CC3UniformlyEvolvingMeshParticle* another )
{
	super::populateFrom( another );
	
	_rotationVelocity = another->getRotationVelocity();
	_rotationVelocityType = another->_rotationVelocityType;
	_colorVelocity = another->getColorVelocity();
}

CCObject* CC3UniformlyEvolvingMeshParticle::copyWithZone( CCZone* zone )
{
	CC3UniformlyEvolvingMeshParticle* pVal = new CC3UniformlyEvolvingMeshParticle;
	pVal->init();
	pVal->populateFrom( this );

	return pVal;
}

std::string CC3UniformlyEvolvingMeshParticle::fullDescription()
{
	return stringWithFormat( (char*)"%s\n\tvelocity: %s",
			super::fullDescription().c_str(), stringFromCC3Vector(_velocity).c_str() );
}

CC3MultiTemplateMeshParticleEmitter::CC3MultiTemplateMeshParticleEmitter()
{
	_particleTemplateMeshes = NULL;
}

CC3MultiTemplateMeshParticleEmitter::~CC3MultiTemplateMeshParticleEmitter()
{
	CC_SAFE_RELEASE( _particleTemplateMeshes );
}

void CC3MultiTemplateMeshParticleEmitter::addParticleTemplateMesh( CC3Mesh* aVtxArrayMesh )
{
	_particleTemplateMeshes->addObject( aVtxArrayMesh );
	//LogTrace(@"%@ added particle template mesh %@ with %i vertices and %i vertex indices",
	//			  self, aVtxArrayMesh, aVtxArrayMesh.vertexCount, aVtxArrayMesh.vertexIndexCount);
}

/** Removes the specified mesh from the collection of meshes in the particleTemplateMeshes property. */
void CC3MultiTemplateMeshParticleEmitter::removeParticleTemplateMesh( CC3Mesh* aVtxArrayMesh )
{
	_particleTemplateMeshes->removeObject( aVtxArrayMesh );
}

void CC3MultiTemplateMeshParticleEmitter::assignTemplateMeshToParticle( CC3MeshParticle* aParticle )
{
	unsigned int tmCount = _particleTemplateMeshes->count() + (_particleTemplateMesh ? 1 : 0);
	CCAssert(tmCount > 0, "No particle template meshes available in CC3MultiTemplateMeshParticleEmitter. Use the addParticleTemplateMesh: method to add template meshes for the particles.");

	unsigned int tmIdx = CC3RandomUIntBelow(tmCount);
	aParticle->setTemplateMesh( (tmIdx < _particleTemplateMeshes->count())
									? (CC3Mesh*)_particleTemplateMeshes->objectAtIndex( tmIdx )
									: _particleTemplateMesh );
}

void CC3MultiTemplateMeshParticleEmitter::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );

	_particleTemplateMeshes = CCArray::create();			// retained
	_particleTemplateMeshes->retain();
}

CC3MultiTemplateMeshParticleEmitter* CC3MultiTemplateMeshParticleEmitter::nodeWithName( const std::string& aName )
{
	CC3MultiTemplateMeshParticleEmitter* pVal = new CC3MultiTemplateMeshParticleEmitter;
	pVal->initWithName( aName );
	pVal->autorelease();

	return pVal;
}

NS_COCOS3D_END
