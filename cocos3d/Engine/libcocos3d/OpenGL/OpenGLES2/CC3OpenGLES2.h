/*
 * Cocos3D-X 1.0.0
 * Author: Bill Hollings
 * Copyright (c) 2010-2014 The Brenwill Workshop Ltd. All rights reserved.
 * http://www.brenwill.com
 *
 * Copyright (c) 2014-2015 Jason Wang
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
#ifndef _CC3OPENGLES2_H_
#define _CC3OPENGLES2_H_
#include <string>

NS_COCOS3D_BEGIN

#if CC3_OGLES_2

#if APPORTABLE
#	define CC3OpenGLClass		CC3OpenGLES2Android
#else
#	define CC3OpenGLClass		CC3OpenGLES2IOS
#endif	// APPORTABLE


/** Manages the OpenGLES 2.0 state for a single GL context. */
class CC3OpenGLES2 : public CC3OpenGLProgPipeline 
{
	DECLARE_SUPER( CC3OpenGLProgPipeline );
public:
	void						unbindTexturesExceptTarget( GLenum target, GLuint tuIdx );
	void						disableTexturingAt( GLuint tuIdx );
	std::string					dumpTextureBindingsAt( GLuint tuIdx );
	void						resolveMultisampleFramebuffer( GLuint fbSrcID, GLuint fbDstID );
	void						discard( GLsizei count, const GLenum* attachments, GLuint fbID );
	void						allocateStorageForRenderbuffer( GLuint rbID, const CC3IntSize& size, GLenum format, GLuint pixelSamples );
	std::string					defaultShaderPreamble();
	
	void						releaseShaderCompiler();
	GLfloat						getVertexShaderVarRangeMin( GLenum precisionType );
	GLfloat						getVertexShaderVarRangeMax( GLenum precisionType );
	GLfloat						getVertexShaderVarPrecision( GLenum precisionType );

	GLfloat						getFragmentShaderVarRangeMin( GLenum precisionType );
	GLfloat						getFragmentShaderVarRangeMax( GLenum precisionType );

	GLfloat						getFragmentShaderVarPrecision( GLenum precisionType );
	void						initPlatformLimits();

	void						initShaderPrecisions();
	CC3Vector					getShaderPrecision( GLenum precisionType, GLenum shaderType );
	void						logShader( GLenum shaderType, const char* qualifier, const CC3Vector& precision, const CC3IntVector& logPrecision );

public:
	CC3Vector					value_Vertex_Shader_Precision[6];
	CC3Vector					value_Fragment_Shader_Precision[6];
};

/** Manages the OpenGLES 2.0 state for a single GL context under iOS. */
class CC3OpenGLES2IOS : public CC3OpenGLES2
{
	DECLARE_SUPER( CC3OpenGLES2 );
public:
	void						initPlatformLimits();
};

/** Manages the OpenGLES 2.0 state for a single GL context under Android. */
class CC3OpenGLES2Android : public CC3OpenGLES2
{
	DECLARE_SUPER( CC3OpenGLES2 );
public:
	void						initPlatformLimits();
	void						initSurfaces();
};

#endif	// CC3_OGLES_2

NS_COCOS3D_END

#endif
