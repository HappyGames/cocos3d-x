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
#include "cocos3d.h"

NS_COCOS3D_BEGIN

#if CC3_OGLES_2


void CC3OpenGLES2::unbindTexturesExceptTarget( GLenum target, GLuint tuIdx )
{
	GLenum otherTarget = (target == GL_TEXTURE_2D) ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;
	bindTexture( 0, otherTarget, tuIdx );
}

void CC3OpenGLES2::disableTexturingAt( GLuint tuIdx )
{
	enableTexturing( false, GL_TEXTURE_2D, tuIdx );
	bindTexture( 0, GL_TEXTURE_2D, tuIdx );
	enableTexturing( false, GL_TEXTURE_CUBE_MAP, tuIdx );
	bindTexture( 0, GL_TEXTURE_CUBE_MAP, tuIdx );
}

std::string CC3OpenGLES2::dumpTextureBindingsAt( GLuint tuIdx )
{
	return CC3String::stringWithFormat( (char*)"%s: %d, %s: %d",
			stringFromGLEnum(GL_TEXTURE_2D).c_str(), getInteger( GL_TEXTURE_BINDING_2D ),
			stringFromGLEnum(GL_TEXTURE_CUBE_MAP).c_str(), getInteger( GL_TEXTURE_BINDING_CUBE_MAP ));
}			


void CC3OpenGLES2::resolveMultisampleFramebuffer( GLuint fbSrcID, GLuint fbDstID )
{
#if !APPORTABLE
	bindFramebuffer( fbSrcID, GL_READ_FRAMEBUFFER_APPLE );
	bindFramebuffer( fbDstID, GL_DRAW_FRAMEBUFFER_APPLE );
	glResolveMultisampleFramebufferAPPLE();
	bindFramebuffer( fbSrcID, GL_FRAMEBUFFER );
#endif
}

void CC3OpenGLES2::discard( GLsizei count, const GLenum* attachments, GLuint fbID )
{
#if !APPORTABLE
	bindFramebuffer( fbID );
	glDiscardFramebufferEXT(GL_FRAMEBUFFER, count, attachments);
#endif
}

void CC3OpenGLES2::allocateStorageForRenderbuffer( GLuint rbID, const CC3IntSize& size, GLenum format, GLuint pixelSamples )
{
	bindRenderbuffer( rbID );

#if !APPORTABLE
	if ( pixelSamples > 1 ) 
	{
		glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, pixelSamples, format, size.width, size.height);
	} 
	else
#endif
	{
		glRenderbufferStorage(GL_RENDERBUFFER, format, size.width, size.height);
	}
}

std::string CC3OpenGLES2::defaultShaderPreamble()
{
#if APPORTABLE
	return
		"#define CC3_PLATFORM_IOS 0\n"
		"#define CC3_PLATFORM_OSX 0\n"
		"#define CC3_PLATFORM_ANDROID 1\n";
#else
	return
		"#define CC3_PLATFORM_IOS 1\n"
		"#define CC3_PLATFORM_OSX 0\n"
		"#define CC3_PLATFORM_ANDROID 0\n";
#endif	// APPORTABLE
}

void CC3OpenGLES2::releaseShaderCompiler()
{
	glReleaseShaderCompiler();
	//LogGLErrorTrace(@"glReleaseShaderCompiler()");
}

GLfloat CC3OpenGLES2::getVertexShaderVarRangeMin( GLenum precisionType )
{
	return value_Vertex_Shader_Precision[precisionType - GL_LOW_FLOAT].x;
}

GLfloat CC3OpenGLES2::getVertexShaderVarRangeMax( GLenum precisionType )
{
	return value_Vertex_Shader_Precision[precisionType - GL_LOW_FLOAT].y;
}

GLfloat CC3OpenGLES2::getVertexShaderVarPrecision( GLenum precisionType )
{
	return value_Vertex_Shader_Precision[precisionType - GL_LOW_FLOAT].z;
}

GLfloat CC3OpenGLES2::getFragmentShaderVarRangeMin( GLenum precisionType )
{
	return value_Fragment_Shader_Precision[precisionType - GL_LOW_FLOAT].x;
}

GLfloat CC3OpenGLES2::getFragmentShaderVarRangeMax( GLenum precisionType )
{
	return value_Fragment_Shader_Precision[precisionType - GL_LOW_FLOAT].y;
}

GLfloat CC3OpenGLES2::getFragmentShaderVarPrecision( GLenum precisionType )
{
	return value_Fragment_Shader_Precision[precisionType - GL_LOW_FLOAT].z;
}


void CC3OpenGLES2::initPlatformLimits()
{
	super::initPlatformLimits();
	
	value_GL_MAX_VERTEX_UNIFORM_VECTORS = getInteger( GL_MAX_VERTEX_UNIFORM_VECTORS );
	//LogInfoIfPrimary(@"Maximum GLSL uniform vectors per vertex shader: %u", value_GL_MAX_VERTEX_UNIFORM_VECTORS);
	
	value_GL_MAX_FRAGMENT_UNIFORM_VECTORS = getInteger( GL_MAX_FRAGMENT_UNIFORM_VECTORS );
	//LogInfoIfPrimary(@"Maximum GLSL uniform vectors per fragment shader: %u", value_GL_MAX_FRAGMENT_UNIFORM_VECTORS);
	
	value_GL_MAX_VARYING_VECTORS = getInteger( GL_MAX_VARYING_VECTORS );
	//LogInfoIfPrimary(@"Maximum GLSL varying vectors per shader program: %u", value_GL_MAX_VARYING_VECTORS);

	initShaderPrecisions();
	
	value_GL_MAX_CUBE_MAP_TEXTURE_SIZE = getInteger( GL_MAX_CUBE_MAP_TEXTURE_SIZE );
	//LogInfoIfPrimary(@"Maximum cube map texture size: %u", value_GL_MAX_CUBE_MAP_TEXTURE_SIZE);
}

void CC3OpenGLES2::initShaderPrecisions()
{
	value_Vertex_Shader_Precision[0] = getShaderPrecision( GL_LOW_FLOAT, GL_VERTEX_SHADER );
	value_Vertex_Shader_Precision[1] = getShaderPrecision( GL_MEDIUM_FLOAT, GL_VERTEX_SHADER );
	value_Vertex_Shader_Precision[2] = getShaderPrecision( GL_HIGH_FLOAT, GL_VERTEX_SHADER );
	value_Vertex_Shader_Precision[3] = getShaderPrecision( GL_LOW_INT, GL_VERTEX_SHADER );
	value_Vertex_Shader_Precision[4] = getShaderPrecision( GL_MEDIUM_INT, GL_VERTEX_SHADER );
	value_Vertex_Shader_Precision[5] = getShaderPrecision( GL_HIGH_INT, GL_VERTEX_SHADER );
	
	value_Fragment_Shader_Precision[0] = getShaderPrecision( GL_LOW_FLOAT, GL_FRAGMENT_SHADER );
	value_Fragment_Shader_Precision[1] = getShaderPrecision( GL_MEDIUM_FLOAT, GL_FRAGMENT_SHADER );
	value_Fragment_Shader_Precision[2] = getShaderPrecision( GL_HIGH_FLOAT, GL_FRAGMENT_SHADER );
	value_Fragment_Shader_Precision[3] = getShaderPrecision( GL_LOW_INT, GL_FRAGMENT_SHADER );
	value_Fragment_Shader_Precision[4] = getShaderPrecision( GL_MEDIUM_INT, GL_FRAGMENT_SHADER );
	value_Fragment_Shader_Precision[5] = getShaderPrecision( GL_HIGH_INT, GL_FRAGMENT_SHADER );
}

CC3Vector CC3OpenGLES2::getShaderPrecision( GLenum precisionType, GLenum shaderType )
{
	CC3Vector precision;
	CC3IntVector logPrecision;
	
	glGetShaderPrecisionFormat(shaderType, precisionType, &logPrecision.x, &logPrecision.z);
	//LogGLErrorTrace(@"glGetShaderPrecisionFormat(%@, %@, %i, %i, %i)",
	//				NSStringFromGLEnum(shaderType), NSStringFromGLEnum(precisionType),
	//				logPrecision.x, logPrecision.y, logPrecision.z);
	
	switch (precisionType) {
		case GL_LOW_FLOAT:
			logPrecision.x = logPrecision.z;	// Returns 0 - but seems to be +/- 0-1 range, so use precision instead
			precision.x = powf(2.0, -logPrecision.x);
			precision.y = powf(2.0, logPrecision.y);
			precision.z = powf(2.0, -logPrecision.z);
			logShader( shaderType, "lowp", precision, logPrecision );
			break;
		case GL_MEDIUM_FLOAT:
			precision.x = powf(2.0, -logPrecision.x);
			precision.y = powf(2.0, logPrecision.y);
			precision.z = powf(2.0, -logPrecision.z);
			logShader( shaderType, "mediump", precision, logPrecision );
			break;
		case GL_HIGH_FLOAT:
			precision.x = powf(2.0, -logPrecision.x);
			precision.y = powf(2.0, logPrecision.y);
			precision.z = powf(2.0, -logPrecision.z);
			logShader( shaderType, "highp", precision, logPrecision );
			break;
		case GL_LOW_INT:
			precision.x = -1 << logPrecision.x;
			precision.y =  1 << logPrecision.y;
			precision.z = 1.0;
			logShader( shaderType, "lowp", precision, logPrecision );
			break;
		case GL_MEDIUM_INT:
			precision.x = -1 << logPrecision.x;
			precision.y =  1 << logPrecision.y;
			precision.z = 1.0;
			logShader( shaderType, "mediump", precision, logPrecision );
			break;
		case GL_HIGH_INT:
			precision.x = -1 << logPrecision.x;
			precision.y =  1 << logPrecision.y;
			precision.z = 1.0;
			logShader( shaderType, "highp", precision, logPrecision );
			break;
		default:
			//LogError(@"Unexpected shader precision: %@", NSStringFromGLEnum(precisionType));
			break;
	}
	
	return precision;
}

void CC3OpenGLES2::logShader( GLenum shaderType, const char* qualifier, const CC3Vector& precision, const CC3IntVector& logPrecision )
{
	/*LogInfoIfPrimary(@"Range of %@ shader %@ float: (min: (+/-)%g, max: (+/-)%g, precision: %g) log2: %@",
					 ((shaderType == GL_VERTEX_SHADER) ? @"vertex" : @"fragment"),
					 qualifier, precision.x, precision.y, precision.z,
					 NSStringFromCC3IntVector(logPrecision));*/
}

void CC3OpenGLES2IOS::initPlatformLimits()
{
	super::initPlatformLimits();
	
	value_GL_MAX_SAMPLES = getInteger( GL_MAX_SAMPLES_APPLE );
	//LogInfoIfPrimary(@"Maximum anti-aliasing samples: %u", value_GL_MAX_SAMPLES);
}

void CC3OpenGLES2Android::initPlatformLimits()
{
	super::initPlatformLimits();
	
	value_GL_MAX_SAMPLES = 1;
	//LogInfoIfPrimary(@"Maximum anti-aliasing samples: %u", value_GL_MAX_SAMPLES);
}

void CC3OpenGLES2Android::initSurfaces()
{
	super::initSurfaces();
	
	// Under Android, the on-screen surface is hardwired to framebuffer 0 and renderbuffer 0.
	// Apportable assumes that the first allocation of each is for the on-screen surface, and
	// therefore ignores that first allocation. We force that ignored allocation here, so that
	// off-screen surfaces can be allocated before the primary on-screen surface.
	generateRenderbuffer();
	generateFramebuffer();
}

#endif	// CC3_OGLES_2

NS_COCOS3D_END
