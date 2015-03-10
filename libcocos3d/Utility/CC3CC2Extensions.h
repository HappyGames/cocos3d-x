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
#ifndef _CCL_CC3CC2EXTENSIONS_H_
#define _CCL_CC3CC2EXTENSIONS_H_
#include "../cc3Helper/Common.h"

NS_COCOS3D_BEGIN

/** In Cocos2D v1 & v2, opacity is defined as an integer value between 0 and 255. */
typedef GLubyte CCOpacity;

/** Maximum opacity value (GLubyte 255) in Cocos2D v1 & v2. */
#define kCCOpacityFull					255

/** Convert GLfloat to CCOpacity (GLubyte) in Cocos2D v1 & v2. */
#define CCOpacityFromGLfloat(glf)		CCColorByteFromFloat(glf)

/** Convert CCOpacity (GLubyte) to GLfloat in Cocos2D v1 & v2. */
#define GLfloatFromCCOpacity(ccOp)		CCColorFloatFromByte(ccOp)

/** Convert GLubyte to CCOpacity (GLubyte) in Cocos2D v1 & v2. */
#define CCOpacityFromGLubyte(glub)		(glub)

/** Convert CCOpacity (GLubyte) to GLubyte in Cocos2D v1 & v2. */
#define GLubyteFromCCOpacity(ccOp)		(ccOp)

/** In Cocos2D v1 & v2, color is defined as a ccColor3B structure containing 3 GLubyte color components. */
typedef ccColor3B CCColorRef;

/** Convert ccColor4F to CCColorRef (ccColor3B) in Cocos2D v1 & v2. */
#define CCColorRefFromCCC4F(c4f)		CCC3BFromCCC4F(c4f)

/** Convert CCColorRef (ccColor3B) to ccColor4F in Cocos2D v1 & v2. */
#define CCC4FFromCCColorRef(ccRef)		CCC4FFromColorAndOpacity(ccRef, kCCOpacityFull)

/** Convert ccColor4B to CCColorRef (ccColor3B) in Cocos2D v1 & v2. */
#define CCColorRefFromCCC4B(c4b)		CCC3BFromCCC4B(c4b)

/** Convert CCColorRef (ccColor3B) to ccColor4B in Cocos2D v1 & v2. */
#define CCC4BFromCCColorRef(ccRef)		CCC4BFromColorAndOpacity(ccRef, kCCOpacityFull)

static inline unsigned long ccNextPOT(  unsigned long x  )
{
	x = x - 1;
	x = x | (x >> 1);
	x = x | (x >> 2);
	x = x | (x >> 4);
	x = x | (x >> 8);
	x = x | (x >>16);
	return x + 1;
}

#define CCNextPOT			ccNextPOT
#define CCTexture			cocos2d::CCTexture2D
#define viewSize			winSize
#define viewSizeInPixels	winSizeInPixels

#define CCTexturePixelFormat			CCTexture2DPixelFormat
#define CCTexturePixelFormat_RGBA8888	kCCTexture2DPixelFormat_RGBA8888
#define CCTexturePixelFormat_RGB888		kCCTexture2DPixelFormat_RGB888
#define CCTexturePixelFormat_RGB565		kCCTexture2DPixelFormat_RGB565
#define CCTexturePixelFormat_A8			kCCTexture2DPixelFormat_A8
#define CCTexturePixelFormat_I8			kCCTexture2DPixelFormat_I8
#define CCTexturePixelFormat_AI88		kCCTexture2DPixelFormat_AI88
#define CCTexturePixelFormat_RGBA4444	kCCTexture2DPixelFormat_RGBA4444
#define CCTexturePixelFormat_RGB5A1		kCCTexture2DPixelFormat_RGB5A1
#define CCTexturePixelFormat_PVRTC4		kCCTexture2DPixelFormat_PVRTC4
#define CCTexturePixelFormat_PVRTC2		kCCTexture2DPixelFormat_PVRTC2
#define CCTexturePixelFormat_Default	kCCTexture2DPixelFormat_Default

#define CCActionFadeTo			CCFadeTo
#define CCActionFadeIn			CCFadeIn
#define CCActionFadeOut			CCFadeOut
#define CCActionHide			CCHide
#define CCActionTintTo			CCTintTo
#define CCActionMoveTo			CCMoveTo
#define CCActionScaleTo			CCScaleTo
#define CCActionSequence		CCSequence
#define CCActionRepeat			CCRepeat
#define CCActionEaseOut			CCEaseOut
#define CCActionEaseIn			CCEaseIn
#define CCActionEaseInOut		CCEaseInOut
#define CCActionEaseBounceOut	CCEaseBounceOut
#define CCActionEaseElasticOut	CCEaseElasticOut
#define CCActionCallFunc		CCCallFunc

#define CCSystemVersion_iOS_5_0 kCCiOSVersion_5_0
#define CCSystemVersion_iOS_6_0 kCCiOSVersion_6_0_0

/** 
 * Returns the size of a CCNode that will cover the specified view size, 
 * taking into consideration whether the view is a Retina view.
 */
static inline CCSize CCNodeSizeFromViewSize(const CCSize viewSize)
{
	GLfloat viewScaleFactor = 1.0f / CCDirector::sharedDirector()->getContentScaleFactor();
	return CCSizeMake(viewSize.width * viewScaleFactor,
					  viewSize.height * viewScaleFactor);
}

enum {
	kCCTouchBegan,
	kCCTouchMoved,
	kCCTouchEnded,
	kCCTouchCancelled,

	kCCTouchMax,
};

NS_COCOS3D_END

#endif
