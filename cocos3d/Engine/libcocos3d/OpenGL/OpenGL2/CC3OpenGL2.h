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
#ifndef _CC3_OPENGL2_H_
#define _CC3_OPENGL2_H_
#include "../CC3OpenGLProgPipeline.h"

NS_COCOS3D_BEGIN

#if CC3_OGL

#define CC3OpenGLClass		CC3OpenGL2

/** Manages the OpenGL state for a single GL context. */
class CC3OpenGL2 : public CC3OpenGLProgPipeline 
{
	DECLARE_SUPER( CC3OpenGLProgPipeline );
public:
	void					enableShaderPointSize( bool onOff );
	void					enablePointSprites( bool onOff );
	/**
	 * Returns the vertex array that should be bound to the specified attribute.
	 *
	 * Overridden to return the vertex locations array as a default if the real vertex array is
	 * not available. This is to bypass failures in GLSL under OSX when attribute content is not
	 * bound, even if the attribute is not accessed during the execution of the shader.
	 */
	CC3VertexArray*			getVertexArrayForAttribute( CC3GLSLAttribute* attribute, CC3NodeDrawingVisitor* visitor );
	void					enablePointSpriteCoordReplace( bool onOff, GLuint tuIdx );
	void					enableTexturing( bool onOff, GLenum target, GLuint tuIdx );
	void					unbindTexturesExceptTarget( GLenum target, GLuint tuIdx );
	void					disableTexturingAt( GLuint tuIdx );
	std::string				dumpTextureBindingsAt( GLuint tuIdx );
	std::string				defaultShaderPreamble();
	void					initPlatformLimits();

protected:
	GLbitfield				value_GL_TEXTURE_CUBE_MAP;				// Track up to 32 texture units
	GLbitfield				isKnownCap_GL_TEXTURE_CUBE_MAP;			// Track up to 32 texture units

	bool					valueCap_GL_VERTEX_PROGRAM_POINT_SIZE : 1;
	bool					isKnownCap_GL_VERTEX_PROGRAM_POINT_SIZE : 1;
};

#endif	// CC3_OGL

NS_COCOS3D_END

#endif
