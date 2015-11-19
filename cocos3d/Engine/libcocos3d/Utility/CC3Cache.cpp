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

CC3Cache::CC3Cache()
{
	_objectsByName = NULL;
	_typeName = "";
//	_mutex = 0;
}

CC3Cache::~CC3Cache()
{
	CC_SAFE_RELEASE( _objectsByName );
}

CC3Cacheable* CC3Cache::getObjectNamed( const std::string& name )
{
	CC3Cacheable* obj = (CC3Cacheable*)_objectsByName->objectForKey( name );
	return obj;
}
//
void CC3Cache::removeObject( CC3Cacheable* obj )
{
	removeObjectNamed( obj->getName() ); 
}

void CC3Cache::removeObjectNamed( const std::string& name )
{
	if ( name.empty() ) 
		return;

	// If this cache is the only thing referencing the object, it will be deallocated immediately,
	// which may interfere with with further processing of the removed object on this loop,
	// including the auto-removal of weakly-cached objects from within the dealloc method of
	// the removed object itself, resulting in a deadlock. To avoid this deadlock, the object
	// is retained before removing it from the cache, and then autoreleased. In the case of a
	// weakly-held value, we are careful to retain the NSValue wrapper, not the object itself,
	// in case this removal is occurring from within the dealloc method of the object itself.
	CC3Cacheable* obj = (CC3Cacheable*)_objectsByName->objectForKey( name );
	CC_SAFE_RETAIN( obj );
	
	_objectsByName->removeObjectForKey( name );

	if ( obj )
		obj->autorelease();		// Let the object go once the loop is done
}

void CC3Cache::setIsWeak( bool weak )
{
	_isWeak = weak;
}

bool CC3Cache::isWeak()
{
	return _isWeak;
}

void CC3Cache::removeAllObjectsOfType( int type )
{
	CCArray* names = _objectsByName->allKeys();
	if ( names )
	{
		for (unsigned int i = 0; i < names->count(); i++)
		{
			const char* key = ((CCString*)names->objectAtIndex(i))->getCString();
			_objectsByName->removeObjectForKey( key );
		}
	}
}

void CC3Cache::removeAllObjects()
{
	if ( _objectsByName )
		_objectsByName->removeAllObjects();
}

void CC3Cache::addObject( CC3Cacheable* obj )
{
	if ( !obj ) 
		return;
		
	std::string objName = obj->getName();
	CCAssert(!objName.empty(), "obj cannot be added to the cache because its name property is nil.");

	CC3Cacheable* cached = getObjectNamed( objName );
	if ( cached == obj )
		return;
	
	if ( cached != NULL )
	{
		CC3_WARNING( "Duplicated objects 0x%04x and 0x%04x with the same name %s found", (unsigned long long)obj, 
			(unsigned long long)cached, objName.c_str() );
	}

	_objectsByName->setObject( obj, objName );
		
	CC3_TRACE("[rez]Added obj[%s] to the %s cache.", objName.c_str(), _typeName.c_str());
}

CC3Cache* CC3Cache::weakCacheForType( const std::string& typeName )
{
	CC3Cache* cache = new CC3Cache;
	cache->initAsWeakCache( true, typeName );
	cache->autorelease();

	return cache;
}

CC3Cache* CC3Cache::strongCacheForType( const std::string& typeName )
{
	CC3Cache* cache = new CC3Cache;
	cache->initAsWeakCache( false, typeName );
	cache->autorelease();

	return cache;
}

void CC3Cache::initAsWeakCache( bool _weak, const std::string& typeName )
{
	setIsWeak( _weak );
	_typeName = typeName;
	_objectsByName = CCDictionary::create();
    
    // Initialize dict type as string
    CCObject* pObject = new CCObject;
    _objectsByName->setObject( pObject,  "dummy_cc3_object" );
    _objectsByName->removeAllObjects();
    CC_SAFE_DELETE( pObject );
    
	_objectsByName->retain();
}

CCDictionary* CC3Cache::getAllObjects()
{
	return _objectsByName;
}

NS_COCOS3D_END
