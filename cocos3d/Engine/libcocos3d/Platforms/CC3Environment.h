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
#ifndef _CC3_ENVIRONMENT_H_
#define _CC3_ENVIRONMENT_H_
#include "cocos2d.h"

/** Running on iOS - use ifdef instead of defined() operator to allow CC3_IOS to be used in expansions */
#ifndef CC3_IOS
#	ifdef __IPHONE_OS_VERSION_MAX_ALLOWED
#		define CC3_IOS			1
#	else
#		define CC3_IOS			0
#	endif
#endif


/** Running on OSX - use ifdef instead of defined() operator to allow CC3_IOS to be used in expansions */
#ifndef CC3_OSX
#	ifdef __MAC_OS_X_VERSION_MAX_ALLOWED
#		define CC3_OSX			1
#	else
#		define CC3_OSX			0
#	endif
#endif


/** Convenience tests for whether we are linking to specific Cocos2D versions or functionality. */
#ifndef CC3_CC2_1
#	define CC3_CC2_1				(COCOS2D_VERSION < 0x020000)
#endif
#ifndef CC3_CC2_2
#	define CC3_CC2_2				(COCOS2D_VERSION >= 0x020000 && COCOS2D_VERSION < 0x030000)
#endif
#ifndef CC3_CC2_CLASSIC
#	define CC3_CC2_CLASSIC			(COCOS2D_VERSION < 0x030000)
#endif
#ifndef CC3_CC2_3
#	define CC3_CC2_3				(COCOS2D_VERSION >= 0x030000)
#endif
#ifndef CC3_CC2_RENDER_QUEUE
#	define CC3_CC2_RENDER_QUEUE		(COCOS2D_VERSION >= 0x030100)
#endif

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	/** Running OpenGL ES 2 under iOS and android. */
	#ifndef CC3_OGLES_2
	#	define CC3_OGLES_2		1
	#endif
#elif CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 
	/** Running OpenGL under OSX on the Mac and win32. */
	#ifndef CC3_OGL
	#	define CC3_OGL			1
	#endif
#endif

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID 
    /** Running on Android via Apportable. Explicitly set as a build setting. */
    #ifndef APPORTABLE
    #	define APPORTABLE		1
    #endif
#else
    #ifndef APPORTABLE
    #	define APPORTABLE		0
    #endif
#endif


/** Running an OpenGL version that supports GLSL (any but OpenGL ES 1.1). */
#ifndef CC3_GLSL
#	define CC3_GLSL			1
#endif

#endif
