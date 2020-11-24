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
#include "cocos3d.h"

NS_COCOS3D_BEGIN

void CC3MortalMeshParticle::setLifeSpan( GLfloat anInterval )
{
	m_lifeSpan = anInterval;
	m_timeToLive = m_lifeSpan;
}

void CC3MortalMeshParticle::updateBeforeTransform( CC3NodeUpdatingVisitor* visitor )
{
	m_timeToLive -= visitor->getDeltaTime();
	if (m_timeToLive <= 0.0f) 
		setIsAlive( false );
}

std::string CC3MortalMeshParticle::fullDescription()
{
	return CC3String::stringWithFormat( (char*)"%s\n\tlifeSpan: %.3f, timeToLive: %.3f",
			super::fullDescription().c_str(), m_lifeSpan, m_timeToLive );
}

bool CC3MortalMeshParticle::init()
{
	if ( super::init() )
	{
		m_lifeSpan = 0.0f;
		m_timeToLive = 0.0f;

		return true;
	}

	return false;
}

void CC3MortalMeshParticle::populateFrom( CC3MortalMeshParticle* another )
{
	super::populateFrom( another );
	
	m_lifeSpan = another->getLifeSpan();
	m_timeToLive = another->getTimeToLive();
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

	setLocation( getLocation() + m_velocity * visitor->getDeltaTime() );
}

bool CC3SprayMeshParticle::init()
{
	if ( super::init() ) 
	{
		m_velocity = CC3Vector::kCC3VectorZero;
		return true;
	}

	return false;
}

void CC3SprayMeshParticle::populateFrom( CC3SprayMeshParticle* another )
{
	super::populateFrom( another );
 	m_velocity = another->getVelocity();
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
	return CC3String::stringWithFormat( (char*)"%s\n\tvelocity: %s",
			super::fullDescription().c_str(), m_velocity.stringfy().c_str() );
}

CC3Vector CC3UniformlyEvolvingMeshParticle::getRotationVelocity()
{
	switch (m_rotationVelocityType) 
	{
		case kCC3RotationTypeAxisAngle: 
		{
			CC3Vector4 axisAngle = CC3Vector4().fromCC3Vector(getRotationAxis(), getRotationAngleVelocity());
			return CC3Quaternion().fromAxisAngle(axisAngle).toRotation();
		}
		default:
			return m_rotationVelocity;
	}
}

void CC3UniformlyEvolvingMeshParticle::setRotationVelocity( const CC3Vector& aVector )
{
	m_rotationVelocity = aVector;
	m_rotationVelocityType = kCC3RotationTypeEuler;
}

GLfloat CC3UniformlyEvolvingMeshParticle::getRotationAngleVelocity()
{
	switch (m_rotationVelocityType) 
	{
		case kCC3RotationTypeEuler: 
		{
			CC3Quaternion quat = CC3Quaternion().fromRotation(getRotationVelocity());
			CC3Vector4 axisAngle = quat.toAxisAngle();
			return axisAngle.w;
		}
		default:
			return m_rotationVelocity.x;
	}
}

void CC3UniformlyEvolvingMeshParticle::setRotationAngleVelocity( GLfloat anAngle )
{
	m_rotationVelocity = cc3v(anAngle, anAngle, anAngle);
	m_rotationVelocityType = kCC3RotationTypeAxisAngle;
}

void CC3UniformlyEvolvingMeshParticle::updateBeforeTransform( CC3NodeUpdatingVisitor* visitor )
{
	super::updateBeforeTransform( visitor );
	if( !isAlive() ) 
		return;
	
	GLfloat dt = visitor->getDeltaTime();
	
	switch (m_rotationVelocityType) 
	{
		case kCC3RotationTypeEuler: 
		{
			CC3Vector rotVel = getRotationVelocity();
			if ( !rotVel.isZero() ) 
				rotateBy( rotVel * dt );
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
	
	if ( hasColor() && !CCC4FAreEqual(m_colorVelocity, kCCC4FBlackTransparent) ) 
	{
		// We have to do the math on each component instead of using the color math functions
		// because the functions clamp prematurely, and we need negative values for the velocity.
		ccColor4F currColor = getColor4F();
		ccColor4F newColor = ccc4f(CLAMP(currColor.r + (m_colorVelocity.r * dt), 0.0f, 1.0f),
								   CLAMP(currColor.g + (m_colorVelocity.g * dt), 0.0f, 1.0f),
								   CLAMP(currColor.b + (m_colorVelocity.b * dt), 0.0f, 1.0f),
								   CLAMP(currColor.a + (m_colorVelocity.a * dt), 0.0f, 1.0f));
		setColor4F( newColor );
	}
	
}


bool CC3UniformlyEvolvingMeshParticle::init()
{
	if ( super::init() ) 
	{
		m_rotationVelocity = CC3Vector::kCC3VectorZero;
		m_rotationVelocityType = kCC3RotationTypeUnknown;
		m_colorVelocity = kCCC4FBlackTransparent;

		return true;
	}

	return false;
}

void CC3UniformlyEvolvingMeshParticle::populateFrom( CC3UniformlyEvolvingMeshParticle* another )
{
	super::populateFrom( another );
	
	m_rotationVelocity = another->getRotationVelocity();
	m_rotationVelocityType = another->m_rotationVelocityType;
	m_colorVelocity = another->getColorVelocity();
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
	return CC3String::stringWithFormat( (char*)"%s\n\tvelocity: %s",
			super::fullDescription().c_str(), m_velocity.stringfy().c_str() );
}

CC3MultiTemplateMeshParticleEmitter::CC3MultiTemplateMeshParticleEmitter()
{
	m_particleTemplateMeshes = NULL;
}

CC3MultiTemplateMeshParticleEmitter::~CC3MultiTemplateMeshParticleEmitter()
{
	CC_SAFE_RELEASE( m_particleTemplateMeshes );
}

void CC3MultiTemplateMeshParticleEmitter::addParticleTemplateMesh( CC3Mesh* aVtxArrayMesh )
{
	m_particleTemplateMeshes->addObject( aVtxArrayMesh );
}

/** Removes the specified mesh from the collection of meshes in the particleTemplateMeshes property. */
void CC3MultiTemplateMeshParticleEmitter::removeParticleTemplateMesh( CC3Mesh* aVtxArrayMesh )
{
	m_particleTemplateMeshes->removeObject( aVtxArrayMesh );
}

void CC3MultiTemplateMeshParticleEmitter::assignTemplateMeshToParticle( CC3MeshParticle* aParticle )
{
	unsigned int tmCount = m_particleTemplateMeshes->count() + (m_pParticleTemplateMesh ? 1 : 0);
	CCAssert(tmCount > 0, "No particle template meshes available in CC3MultiTemplateMeshParticleEmitter. Use the addParticleTemplateMesh: method to add template meshes for the particles.");

	unsigned int tmIdx = CC3RandomUIntBelow(tmCount);
	aParticle->setTemplateMesh( (tmIdx < m_particleTemplateMeshes->count())
									? (CC3Mesh*)m_particleTemplateMeshes->objectAtIndex( tmIdx )
									: m_pParticleTemplateMesh );
}

void CC3MultiTemplateMeshParticleEmitter::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );

	m_particleTemplateMeshes = CCArray::create();			// retained
	m_particleTemplateMeshes->retain();
}

CC3MultiTemplateMeshParticleEmitter* CC3MultiTemplateMeshParticleEmitter::nodeWithName( const std::string& aName )
{
	CC3MultiTemplateMeshParticleEmitter* pVal = new CC3MultiTemplateMeshParticleEmitter;
	pVal->initWithName( aName );
	pVal->autorelease();

	return pVal;
}

NS_COCOS3D_END
