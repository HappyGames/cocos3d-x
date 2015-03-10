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
#ifndef _CC3_OPENGL_UTILITY_H_
#define _CC3_OPENGL_UTILITY_H_
#include "../Platforms/CC3Environment.h"
#include "../Utility/CC3Foundation.h"

#if CC3_OGLES_2
//#include <OpenGLES/ES2/gl.h>
//#include <OpenGLES/ES2/glext.h>
#include "OpenGLES2/CC3OpenGLES2Compatibility.h"
#endif	// CC3_OGLES_2

#if CC3_OGL
//#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
#include "OpenGL2/CC3OpenGLCompatibility.h"
#endif	// CC3_OGL

NS_COCOS3D_BEGIN

/** Returns a string containing the name of the specified GL enumeration code. */
std::string CC3GLEnumName(GLenum gle);

/** 
 * Returns the size of the specified GL dataType, which must be one of:
 * GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_FLOAT, GL_FIXED.
 */
size_t CC3GLElementTypeSize(GLenum dataType);

/** Returns the GL color format enum corresponding to the specified number of color and alpha bit planes. */
GLenum CC3GLColorFormatFromBitPlanes(GLint colorCount, GLint alphaCount);

/** Returns the GL depth format enum corresponding to the specified number of depth and stencil bit planes. */
GLenum CC3GLDepthFormatFromBitPlanes(GLint depthCount, GLint stencilCount);

NS_COCOS3D_END

#endif
