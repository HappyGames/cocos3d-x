/*
 * Cocos3D-X 1.0.0
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
#ifndef _CCL_CC3_COMMON_H_
#define _CCL_CC3_COMMON_H_
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

#ifdef __cplusplus
#	define NS_COCOS3D_BEGIN					namespace cocos3d {
#	define NS_COCOS3D_END					}
#	define USING_NS_COCOS3D					using namespace cocos3d
#else
#	define NS_COCOS3D_BEGIN 
#	define NS_COCOS3D_END 
#	define USING_NS_COCOS3D
#endif 

#ifndef _WINDOWS_
#define vsnprintf_s vsnprintf
#endif

#ifndef GetVarargs
#define GetVarargs(msg, len, fmt)			\
{											\
	va_list  ArgPtr;						\
	va_start ( ArgPtr, fmt );				\
	vsnprintf_s( msg, len, fmt, ArgPtr );	\
	va_end( ArgPtr );						\
}
#endif

#ifndef DECLARE_SUPER
#define DECLARE_SUPER( superClass ) \
	typedef superClass super;
#endif

// #pragma - output string when compile
#define _PRAGMA_STR1(x)		#x
#define _PRAGMA_STR2(x)		_PRAGMA_STR1 (x)
#define _NOTE(x)			message (__FILE__ "(" _PRAGMA_STR2(__LINE__) ") : -NOTE- " #x)
#define _NOTE_ARG(x,y)		message (__FILE__ "(" _PRAGMA_STR2(__LINE__) ") : -NOTE- " #x _PRAGMA_STR2(y))
// use macros like this:
//	#pragma _NOTE_ARG( "Number of buffers = ", NUM_COMM_BUFFERS )
#define _NOTE_TODO(x)		_NOTE_ARG(			[TODO] , x )
// use macros like this:
//	#pragma _NOTE_TODO( "Fixed the bug" )

//#include "XmlOp.hpp"

/************************************************************************/
/* LOG SYSTEM															*/

#ifdef _DEBUG
#define CC3_TRACE( format, ... )		cocos3d::CLoggers::sharedLoggers()->logMessage( cocos3d::CC3_LOG_INFO, format, ##__VA_ARGS__ )
#define CC3_ERROR( format, ... )		cocos3d::CLoggers::sharedLoggers()->logMessage( cocos3d::CC3_LOG_ERROR, format, ##__VA_ARGS__ )
#define CC3_WARNING( format, ... )		cocos3d::CLoggers::sharedLoggers()->logMessage( cocos3d::CC3_LOG_WARNING, format, ##__VA_ARGS__ )
#define CC3_TAG( tag, format, ... )		cocos3d::CLoggers::sharedLoggers()->logMessage( cocos3d::CC3_LOG_INFO, tag format, ##__VA_ARGS__ )
#else
#define CC3_TRACE( format, ... )		
#define CC3_ERROR( format, ... )		
#define CC3_WARNING( format, ... )	
#define CC3_TAG( tag, format, ... )	
#endif
/************************************************************************/

/************************************************************************/
/* singleton class declare macro                                        */
/************************************************************************/
//singleton class declare
#ifndef COCOS_SINGLETON_DECL
#define COCOS_SINGLETON_DECL(name)					\
protected:											\
	static C##name* m_p##name;						\
public:												\
	static C##name* shared##name();					\
	static void		purge();						\
public:
#endif

//singleton class implement
#ifndef COCOS_SINGLETON_IMPL
#define COCOS_SINGLETON_IMPL(name)					\
	C##name* C##name::m_p##name = 0;				\
	C##name* C##name::shared##name()				\
	{												\
		if ( m_p##name == NULL )					\
			m_p##name = new C##name;				\
		return m_p##name;							\
	}												\
	void C##name::purge()							\
	{												\
		CC_SAFE_DELETE(m_p##name);					\
	}
#endif

//////////////////////////////////////////////////////////////////////////

#ifndef DISABLE_CODE_BLOCK_BEGIN
#define DISABLE_CODE_BLOCK_BEGIN	if ( 0 ) { 
#endif

#ifndef DISABLE_CODE_BLOCK_END
#define DISABLE_CODE_BLOCK_END		} 
#endif

#ifdef _WIN32
#ifndef INFINITY
#define INFINITY (float)(HUGE_VAL)
#endif
#endif

#endif
