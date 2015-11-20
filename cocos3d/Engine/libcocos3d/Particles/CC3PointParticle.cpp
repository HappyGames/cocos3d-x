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

void CC3PointParticle::setEmitter( CC3ParticleEmitter* emitter )
{
	super::setEmitter( emitter );
}

bool CC3PointParticle::isAlive()
{
	return m_isAlive; 
}

void CC3PointParticle::setIsAlive( bool alive )
{
	m_isAlive = alive; 
}

GLuint CC3PointParticle::getParticleIndex()
{
	return m_particleIndex; 
}

/** Overridden to update the underlying vertex indices array, if it exists. */
void CC3PointParticle::setParticleIndex( GLuint anIndex )
{
	m_particleIndex = anIndex;
	if ( m_pEmitter ) 
		((CC3PointParticleEmitter*)m_pEmitter)->setVertexIndex( anIndex, anIndex );	// Ignored if not using indexed drawing
}

GLuint CC3PointParticle::getVertexCount()
{
	return 1; 
}

CCRange CC3PointParticle::getVertexRange()
{
	return CCRangeMake(m_particleIndex, getVertexCount()); 
}

GLuint CC3PointParticle::getVertexIndexCount()
{
	return 1; 
}

CCRange CC3PointParticle::getVertexIndexRange()
{
	return CCRangeMake(m_particleIndex, getVertexIndexCount()); 
}

CC3Vector CC3PointParticle::getLocation()
{
	if ( m_pEmitter )
		return m_pEmitter->getVertexLocationAt( m_particleIndex ); 

	return CC3Vector::kCC3VectorZero;
}

void CC3PointParticle::setLocation( const CC3Vector& aLocation )
{
	if ( m_pEmitter )
		m_pEmitter->setVertexLocation( aLocation, m_particleIndex ); 
}

CC3Vector CC3PointParticle::getNormal() 
{
	if ( m_pEmitter )
		return m_pEmitter->getVertexNormalAt( m_particleIndex ); 

	return CC3Vector::kCC3VectorZero;
}

void CC3PointParticle::setNormal( const CC3Vector& aNormal )
{
	if ( m_pEmitter )
		m_pEmitter->setVertexNormal( aNormal, m_particleIndex ); 
}

bool CC3PointParticle::hasNormal()
{
	if ( m_pEmitter ) 
		return m_pEmitter->getMesh()->hasVertexNormals();

	return false;
}

ccColor4F CC3PointParticle::getColor4F()
{
	return hasColor() ? m_pEmitter->getVertexColor4FAt(m_particleIndex) : super::getColor4F(); 
}

void CC3PointParticle::setColor4F( const ccColor4F& aColor )
{
	if ( m_pEmitter )
		m_pEmitter->setVertexColor4F( aColor, m_particleIndex ); 
}

ccColor4B CC3PointParticle::getColor4B()
{
	return hasColor() ? m_pEmitter->getVertexColor4BAt(m_particleIndex) : super::getColor4B(); 
}

void CC3PointParticle::setColor4B( const ccColor4B& aColor )
{
	if ( m_pEmitter )
		m_pEmitter->setVertexColor4B( aColor, m_particleIndex ); 
}

GLfloat CC3PointParticle::getSize()
{
	CC3PointParticleEmitter* pe = (CC3PointParticleEmitter*)m_pEmitter;
	return hasSize() ? pe->getParticleSizeAt(m_particleIndex) : pe->getParticleSize();
}

void CC3PointParticle::setSize( GLfloat aSize )
{
	CC3PointParticleEmitter* pe = (CC3PointParticleEmitter*)m_pEmitter;
	pe->setParticleSize( aSize, m_particleIndex ); 
}

bool CC3PointParticle::hasSize()
{
	return getEmitter()->getMesh()->hasVertexPointSizes(); 
}

bool CC3PointParticle::hasVertexIndices()
{
	return false; 
}

std::string CC3PointParticle::fullDescription()
{
	return "CC3PointParticle";
}

void CC3PointParticle::pointNormalAt( const CC3Vector& camLoc )
{
	setNormal( camLoc.difference( getLocation() ).normalize() );
}

bool CC3PointParticle::init()
{
	if ( super::init() ) 
	{
		m_particleIndex = 0;
		return true;
	}

	return false;
}

void CC3PointParticle::populateFrom( CC3PointParticle* another )
{
	super::populateFrom( another );
	m_particleIndex = another->getParticleIndex();
}

CCObject* CC3PointParticle::copyWithZone( CCZone* zone )
{
	CC3PointParticle* pVal = new CC3PointParticle;
	pVal->init();
	pVal->populateFrom( this );

	return pVal;
}

CC3PointParticle* CC3PointParticle::particle()
{
	CC3PointParticle* pParticle = new CC3PointParticle;
	pParticle->init();
	pParticle->autorelease();

	return pParticle;
}

NS_COCOS3D_END
