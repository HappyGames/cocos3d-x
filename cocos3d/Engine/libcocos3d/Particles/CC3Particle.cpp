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

CC3Particle::CC3Particle()
{
	m_pEmitter = NULL;
}

CC3Particle::~CC3Particle()
{
	m_pEmitter = NULL;			// weak reference
}

// Alloc iVar in subclases to consolidate storage
bool CC3Particle::isAlive()
{
	CCAssert(false, "CC3ParticleBase does not implement the isAlive property");
	return false;
}

// Alloc iVar in subclases to consolidate storage
void CC3Particle::setIsAlive( bool alive )
{
	CCAssert(false, "CC3ParticleBase does not implement the isAlive property");
}

CC3Vector CC3Particle::getLocation() 
{
	return CC3Vector::kCC3VectorNull; 
}

void CC3Particle::setLocation( const CC3Vector& aLocation )
{

}

CC3Vector CC3Particle::getGlobalLocation()
{
	return getEmitter()->getGlobalTransformMatrix()->transformLocation( getLocation() ); 
}

ccColor4F CC3Particle::getColor4F() 
{
	return m_pEmitter->getDiffuseColor(); 
}

void CC3Particle::setColor4F( const ccColor4F& aColor )
{
}

ccColor4B CC3Particle::getColor4B()
{
	return CCC4BFromCCC4F(getColor4F()); 
}

void CC3Particle::setColor4B( const ccColor4B& aColor )
{
}

bool CC3Particle::hasColor()
{
	return m_pEmitter->getMesh()->hasVertexColors(); 
}

void CC3Particle::remove()
{ 
	getEmitter()->removeParticle( this );
}

CCColorRef CC3Particle::getColor()
{
	return CCColorRefFromCCC4F(getColor4F()); 
}

void CC3Particle::setColor( const CCColorRef& color )
{
	ccColor4F c4f = CCC4FFromCCColorRef(color);
	c4f.a = GLfloatFromCCOpacity(getOpacity());
	setColor4F( c4f );
}

CCOpacity CC3Particle::getOpacity()
{
	return CCOpacityFromGLfloat(getColor4F().a); 
}

void CC3Particle::setOpacity( CCOpacity opacity )
{
	ccColor4F c4f = getColor4F();
	c4f.a = GLfloatFromCCOpacity(opacity);
	setColor4F( c4f );
}

CCColorRef CC3Particle::getDisplayedColor()
{
	return getColor(); 
}

bool CC3Particle::isCascadeColorEnabled()
{
	return false; 
}

void CC3Particle::setCascadeColorEnabled( bool cascadeColorEnabled )
{

}

void CC3Particle::updateDisplayedColor( const CCColorRef& color )
{

}

CCOpacity CC3Particle::getDisplayedOpacity()
{
	return getOpacity(); 
}

bool CC3Particle::isCascadeOpacityEnabled()
{
	return false; 
}

void CC3Particle::setCascadeOpacityEnabled( bool cascadeOpacityEnabled )
{

}

void CC3Particle::updateDisplayedOpacity( CCOpacity opacity )
{

}

std::string CC3Particle::fullDescription()
{
	std::string desc = "";
	desc += CC3String::stringWithFormat( (char*)"\n\tlocation: %s", getLocation().stringfy().c_str() );
	if (hasColor()) 
	{
		desc += CC3String::stringWithFormat( (char*)", colored: %s", stringFromCCC4F(getColor4F()).c_str() );
	}
	return desc;
}

void CC3Particle::initializeParticle()
{

}

void CC3Particle::finalizeParticle()
{

}

bool CC3Particle::init()
{
	m_pEmitter = NULL;
	setIsAlive( false );

	return true;
}

CC3Particle* CC3Particle::particle()
{
	CC3Particle* pVal = new CC3Particle;
	pVal->init();
	pVal->autorelease();

	return pVal;
}

CCObject* CC3Particle::copyWithZone( CCZone* zone )
{
	CC3Particle* aCopy = new CC3Particle;
	aCopy->init();
	aCopy->populateFrom( this );
	return aCopy;
}

void CC3Particle::populateFrom( CC3Particle* another )
{
	setEmitter( another->getEmitter() );
	setIsAlive( another->isAlive() );
}

void CC3Particle::updateBeforeTransform( CC3NodeUpdatingVisitor* visitor )
{

}

void CC3Particle::updateAfterTransform( CC3NodeUpdatingVisitor* visitor )
{

}

void CC3Particle::setEmitter( CC3ParticleEmitter* emitter )
{
	m_pEmitter = emitter;
}

CC3ParticleEmitter* CC3Particle::getEmitter()
{
	return m_pEmitter;
}

NS_COCOS3D_END
