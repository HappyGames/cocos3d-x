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

#if CC3_OGL

void CC3OpenGL2::enableShaderPointSize( bool onOff )
{ 
	cc3_SetGLCap(GL_VERTEX_PROGRAM_POINT_SIZE, onOff, valueCap_GL_VERTEX_PROGRAM_POINT_SIZE, 
		isKnownCap_GL_VERTEX_PROGRAM_POINT_SIZE); 
}

void CC3OpenGL2::enablePointSprites( bool onOff )
{
	cc3_SetGLCap(GL_POINT_SPRITE, onOff, valueCap_GL_POINT_SPRITE, isKnownCap_GL_POINT_SPRITE); 
}

/**
 * Returns the vertex array that should be bound to the specified attribute.
 *
 * Overridden to return the vertex locations array as a default if the real vertex array is
 * not available. This is to bypass failures in GLSL under OSX when attribute content is not
 * bound, even if the attribute is not accessed during the execution of the shader.
 */
CC3VertexArray* CC3OpenGL2::getVertexArrayForAttribute( CC3GLSLAttribute* attribute, CC3NodeDrawingVisitor* visitor )
{
	CC3VertexArray* va = super::getVertexArrayForAttribute( attribute, visitor );
	if ( va )
		return va;
	CC3Mesh* pMesh = visitor->getCurrentMesh();
	if ( pMesh )
		return pMesh->getVertexLocations();
	
	return NULL;
}

void CC3OpenGL2::enablePointSpriteCoordReplace( bool onOff, GLuint tuIdx )
{
	if ( CC3CheckGLBooleanAt(tuIdx, onOff, &value_GL_COORD_REPLACE, &isKnownCap_GL_COORD_REPLACE) ) 
	{
		activateTextureUnit( tuIdx );
		glTexEnvi( GL_POINT_SPRITE, GL_COORD_REPLACE, (onOff ? GL_TRUE : GL_FALSE) );
		//LogGLErrorTrace(@"glTexEnvi(%@, %@, %@)", NSStringFromGLEnum(GL_POINT_SPRITE), NSStringFromGLEnum(GL_COORD_REPLACE), (onOff ? @"GL_TRUE" : @"GL_FALSE"));
	}
}

void CC3OpenGL2::enableTexturing( bool onOff, GLenum target, GLuint tuIdx )
{
	if ( target == GL_TEXTURE_CUBE_MAP )
	{
		if ( CC3CheckGLBooleanAt(tuIdx, onOff, &value_GL_TEXTURE_CUBE_MAP, &isKnownCap_GL_TEXTURE_CUBE_MAP) ) 
		{
			activateTextureUnit( tuIdx );
			if ( onOff )
				glEnable(target);
			else
				glDisable(target);
			//LogGLErrorTrace(@"gl%@able(%@)", (onOff ? @"En" : @"Dis"), NSStringFromGLEnum(target));
		}
		return;
	}

	// If one of the other targets is being enabled, cube-mapping must be disabled, because it has higher priority
	if ( onOff ) 
		enableTexturing( false, GL_TEXTURE_CUBE_MAP, tuIdx );

	// If not cube-map, allow superclass to handle other targets
	super::enableTexturing( onOff, target, tuIdx );
}

void CC3OpenGL2::unbindTexturesExceptTarget( GLenum target, GLuint tuIdx )
{
	GLenum otherTarget = (target == GL_TEXTURE_2D) ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;
	bindTexture( 0, otherTarget, tuIdx );
}

void CC3OpenGL2::disableTexturingAt( GLuint tuIdx )
{
	/// disable texture 2d
	enableTexturing( false, GL_TEXTURE_2D, tuIdx );
	bindTexture( 0, GL_TEXTURE_2D, tuIdx );

	/// disable texture cube
	enableTexturing( false, GL_TEXTURE_CUBE_MAP, tuIdx );
	bindTexture( 0, GL_TEXTURE_CUBE_MAP, tuIdx );
}

std::string CC3OpenGL2::dumpTextureBindingsAt( GLuint tuIdx )
{
	return  stringWithFormat( (char*)"%s: %d, %s: %d",
			stringFromGLEnum(GL_TEXTURE_2D).c_str(), getInteger( GL_TEXTURE_BINDING_2D ),
			stringFromGLEnum(GL_TEXTURE_CUBE_MAP).c_str(), getInteger( GL_TEXTURE_BINDING_CUBE_MAP ) );
}


std::string CC3OpenGL2::defaultShaderPreamble()
{
	return
		"#version 120\n"
		"#define precision //precision\n"
		"#define highp\n"
		"#define mediump\n"
		"#define lowp\n"
		"#define CC3_PLATFORM_IOS 0\n"
		"#define CC3_PLATFORM_OSX 1\n"
		"#define CC3_PLATFORM_ANDROID 0\n";
}

void CC3OpenGL2::initPlatformLimits()
{
	super::initPlatformLimits();
	
	value_GL_MAX_VERTEX_UNIFORM_VECTORS = getInteger( GL_MAX_VERTEX_UNIFORM_COMPONENTS ) / 4;
	//LogInfoIfPrimary(@"Maximum GLSL uniform vectors per vertex shader: %u", value_GL_MAX_VERTEX_UNIFORM_VECTORS);
	
	value_GL_MAX_FRAGMENT_UNIFORM_VECTORS = getInteger( GL_MAX_FRAGMENT_UNIFORM_COMPONENTS ) / 4;
	//LogInfoIfPrimary(@"Maximum GLSL uniform vectors per fragment shader: %u", value_GL_MAX_FRAGMENT_UNIFORM_VECTORS);
	
	value_GL_MAX_VARYING_VECTORS = getInteger( GL_MAX_VARYING_FLOATS ) / 4;
	//LogInfoIfPrimary(@"Maximum GLSL varying vectors per shader program: %u", value_GL_MAX_VARYING_VECTORS);

	// Ensure texture units not larger than the fixed pipeline texture units,
	// regardless of whether fixed or programmable pipeline is in effect.
	value_GL_MAX_TEXTURE_UNITS = getInteger( GL_MAX_TEXTURE_UNITS );
	//LogInfoIfPrimary(@"Maximum texture units: %u", value_GL_MAX_TEXTURE_UNITS);
	
	value_GL_MAX_SAMPLES = getInteger( GL_MAX_SAMPLES );
	//LogInfoIfPrimary(@"Maximum anti-aliasing samples: %u", value_GL_MAX_SAMPLES);
	
	value_GL_MAX_CUBE_MAP_TEXTURE_SIZE = getInteger( GL_MAX_CUBE_MAP_TEXTURE_SIZE );
	//LogInfoIfPrimary(@"Maximum cube map texture size: %u", value_GL_MAX_CUBE_MAP_TEXTURE_SIZE);
}

#endif	// CC3_OGL && CC3_GLSL

NS_COCOS3D_END
