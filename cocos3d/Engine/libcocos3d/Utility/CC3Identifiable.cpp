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

static GLint instanceCount = 0;

CC3Identifiable::CC3Identifiable()
{
	m_nTag = 0;
	m_sName = "";
	_userData = NULL;
}

CC3Identifiable::~CC3Identifiable()
{
	instanceCount--;
}

bool CC3Identifiable::deriveNameFrom( CC3Identifiable* another )
{
	return deriveNameFrom( another, getNameSuffix().c_str() );
}

bool CC3Identifiable::deriveNameFrom( CC3Identifiable* another, const std::string& suffix )
{
	if ( m_sName.empty() ) 
		return false;
	std::string otherName = another->getName();
	if ( otherName.empty() ) 
		return false;
	if ( suffix.empty() ) 
		return false;

	setName( CC3String::stringWithFormat( (char*)"%s-%s", otherName.c_str(), suffix.c_str() ) );

	return true;
}

std::string CC3Identifiable::getNameSuffix()
{
	CCAssert(false, "Subclass of CC3Identifiable must override the nameSuffix property.");
	return NULL;
}

bool CC3Identifiable::init()
{
	initWithName( "" ); 
	return true;
}

void CC3Identifiable::initWithTag( GLuint aTag )
{ 
	return initWithTag( aTag, "" );
}

void CC3Identifiable::initWithName( const std::string& aName )
{
	return initWithTag( nextTag(), aName );
}

void CC3Identifiable::initWithTag( GLuint aTag, const std::string& aName )
{
	instanceCount++;
	setTag( aTag );
	setName( aName );
	initUserData();
}

void CC3Identifiable::populateFrom( CC3Identifiable* another )
{
	copyUserDataFrom( another );
	setName( another->getName() );
}

// Implementation to keep compiler happy so this method can be included in interface for documentation.
CCObject* CC3Identifiable::copy()
{
	return CCObject::copy(); 
}

CCObject* CC3Identifiable::copyWithZone( CCZone* zone )
{
	CC3Identifiable* aCopy = new CC3Identifiable;
	aCopy->populateFrom( this );
	aCopy->setName( getName() );

	CC3_WARNING( "copyWithZone with default implementation" );

	return aCopy;
}

CCObject* CC3Identifiable::copyWithName( const std::string& aName )
{ 
	return copyWithZone( NULL, aName ); 
}

CCObject* CC3Identifiable::copyWithZone( CCZone* zone, const std::string& aName )
{
	CC3Identifiable* pObj = (CC3Identifiable*)copyWithZone( zone );
	pObj->setName( aName );

	return pObj;
}

bool CC3Identifiable::shouldIncludeInDeepCopy()
{
	return true; 
}

void CC3Identifiable::initUserData()
{
	_userData = NULL; 
}

void CC3Identifiable::copyUserDataFrom( CC3Identifiable* another )
{
	setUserData( another->getUserData() ); 
}

void* CC3Identifiable::getUserData()
{
	return _userData;
}

void CC3Identifiable::setUserData( void* data )
{
	_userData = data;
}

// Class variable tracking the most recent tag value assigned. This class variable is 
// automatically incremented whenever the method nextTag is called.
static GLuint lastAssignedTag = 0;

GLuint CC3Identifiable::nextTag()
{
	return ++lastAssignedTag; 
}

void CC3Identifiable::resetTagAllocation()
{
	lastAssignedTag = 0;  
}

std::string CC3Identifiable::fullDescription()
{
	return CC3String::stringWithFormat( (char*)"%s :%d", m_sName.c_str(), m_nTag );
}

GLint CC3Identifiable::getInstanceCount()
{
	return instanceCount; 
}

void CC3Identifiable::initAtIndex( int aPODIndex, CC3PODResource* aPODRez )
{
	init();
	setPodIndex( aPODIndex );
}

std::string CC3Identifiable::getName()
{
	return m_sName;
}

void CC3Identifiable::setName( const std::string& name )
{
	m_sName = name;
}

GLuint CC3Identifiable::getTag()
{
	return m_nTag;
}

void CC3Identifiable::setTag( GLuint tag )
{
	m_nTag = tag;
}

NS_COCOS3D_END
