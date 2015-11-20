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

CC3Resource::CC3Resource()
{
	m_directory = "";
}

CC3Resource::~CC3Resource()
{	
	remove();		// remove this instance from the cache
}

std::string CC3Resource::getDirectory()
{
	return m_directory;
}

void CC3Resource::setDirectory( const std::string& directory )
{
	m_directory = directory;
}

bool CC3Resource::loadFromFile( const std::string& filePath )
{
	if (m_wasLoaded) 
	{
		CC3_TRACE("[rez]CC3Resource[%s] has already been loaded.", filePath.c_str());
		return m_wasLoaded;
	}
	
	// Resolve an absolute path in either the application bundle resource
	// directory or the Cocos3D bundle resource directory.
	std::string absFilePath = filePath;

	if ( absFilePath.empty() )
	{
		CC3_TRACE( "[rez]Could not locate resource file '%s' in either the application resources or the Cocos3D library resources", filePath.c_str() );
		return false;
	}

	CC3_TRACE("[rez]--------------------------------------------------");
	CC3_TRACE("[rez]Loading resource from file '%s'", absFilePath.c_str());
	
	if ( m_sName.c_str() ) 
		setName( resourceNameFromFilePath( absFilePath ) );

	if ( m_directory.empty() ) 
	{
		std::string sDir = CC3String::getDirectory( absFilePath );
		setDirectory( sDir );
	}

	m_wasLoaded = processFile( absFilePath );	// Main subclass loading method
	
	if (!m_wasLoaded)
	{
		CC3_TRACE("[rez]Could not load resource file '%s'", absFilePath.c_str());
	}

	CC3_TRACE("");		// Empty line to separate from next logs
	
	return m_wasLoaded;
}

bool CC3Resource::processFile( const std::string& anAbsoluteFilePath )
{
	return false; 
}

bool CC3Resource::saveToFile( const std::string& filePath )
{
	CCAssert(false, "CC3Resource does not support saving the resource content back to a file.");
	return false;
}

bool CC3Resource::init()
{
	if ( super::init() )
	{
		m_directory = "";
		m_isBigEndian = false;
		m_wasLoaded = false;

		return true;
	}

	return false;
}

CC3Resource* CC3Resource::resource()
{
	CC3Resource* pRes = new CC3Resource;
	pRes->init();
	pRes->autorelease();
	return pRes;
}

bool CC3Resource::initFromFile( const std::string& filePath )
{
	init();		// Use self so subclasses will init properly
	if ( !loadFromFile(filePath) ) 
		return false;

	return true;
}

CC3Resource* CC3Resource::resourceFromFile( const std::string& filePath )
{
	CC3Resource* rez = getResourceNamed( resourceNameFromFilePath(filePath) );
	if (rez) 
		return rez;

	rez = new CC3Resource;
	if ( rez->initFromFile( filePath ) )
	{
		addResource( rez );
		rez->autorelease();
		return rez;
	}
	
	CC_SAFE_DELETE( rez );

	return rez;
}

std::string CC3Resource::resourceNameFromFilePath( const std::string& filePath )
{
	return CC3String::getFileName( filePath );
}

void CC3Resource::remove()
{
	removeResource( this ); 
}

static CC3Cache* _resourceCache = NULL;

void CC3Resource::ensureCache()
{
	if ( !_resourceCache ) 
	{
		_resourceCache = CC3Cache::weakCacheForType( "resource" );
		_resourceCache->retain();	// retained
	}
}

void CC3Resource::addResource( CC3Resource* resource )
{
	ensureCache();
	_resourceCache->addObject( resource );
}

CC3Resource* CC3Resource::getResourceNamed( const std::string& name )
{
	ensureCache();
	return (CC3Resource*)_resourceCache->getObjectNamed(name);
}

void CC3Resource::removeResource( CC3Resource* resource )
{
	ensureCache();
	_resourceCache->removeObject( resource ); 
}

void CC3Resource::removeResourceNamed( const std::string& name )
{
	ensureCache();
	_resourceCache->removeObjectNamed( name ); 
}

void CC3Resource::removeAllResources()
{ 
	if ( _resourceCache )
		_resourceCache->removeAllObjectsOfType( 0 );

	CC_SAFE_RELEASE_NULL( _resourceCache );
}

bool CC3Resource::isPreloading()
{
	return _resourceCache ? !_resourceCache->isWeak() : false; 
}

void CC3Resource::setIsPreloading( bool _isPreloading )
{
	ensureCache();
	_resourceCache->setIsWeak( !_isPreloading );
}

std::string CC3Resource::cachedResourcesDescription()
{
	std::string desc = "";
	/*[_resourceCache enumerateObjectsUsingBlock: ^(CC3Resource* rez, BOOL* stop) {
		if ( [rez isKindOfClass: self] ) [desc appendFormat: @"\n\t%@", rez.constructorDescription];
	}];*/
	return desc;
}

// Class variable tracking the most recent tag value assigned for CC3Identifiables.
// This class variable is automatically incremented whenever the method nextTag is called.
static GLuint lastAssignedResourceTag = 0;

GLuint CC3Resource::nextTag()
{
	return ++lastAssignedResourceTag; 
}

void CC3Resource::resetTagAllocation()
{
	lastAssignedResourceTag = 0; 
}

NS_COCOS3D_END
