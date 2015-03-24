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

std::string stringFromGLEnum(GLenum gle) 
{
	return CC3GLEnumName(gle);
}

bool CC3DepthFormatIncludesStencil(GLenum depthFormat) 
{
	switch (depthFormat) {
		case GL_DEPTH_STENCIL:			// textures
		case GL_DEPTH24_STENCIL8:		// renderbuffers
			return true;
			break;
		default:
			return false;
	}
}

std::string GetGLErrorText(GLenum errCode) 
{
	switch (errCode)
	{
		case GL_NO_ERROR:
			return "GL_NO_ERROR: It's all good";
		case GL_INVALID_ENUM:
			return "GL_INVALID_ENUM: Bad enumerated argument";
		case GL_INVALID_VALUE:
			return "GL_INVALID_VALUE: Numeric argument is out of range";
		case GL_INVALID_OPERATION:
			return "GL_INVALID_OPERATION: Operation not allowed in current state";
		case GL_STACK_OVERFLOW:
			return "GL_STACK_OVERFLOW: Operation would cause stack overflow";
		case GL_STACK_UNDERFLOW:
			return "GL_STACK_UNDERFLOW: Operation would cause stack underflow";
		case GL_OUT_OF_MEMORY:
			return "GL_OUT_OF_MEMORY: Not enough memory to perform operation";
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			return "GL_INVALID_FRAMEBUFFER_OPERATION: Operation not allowed on frame buffer";
		default:
			return stringWithFormat( (char*)"Unknown GL error (0x%04X)", errCode );
	}
}

static bool _shouldLogGLCalls = false;

void CC3StartLoggingGLCalls(void) 
{
	if ( !_shouldLogGLCalls ) 
	{
		printf("Starting GL logging.\n");
		_shouldLogGLCalls = true;
	}
}

void CC3StopLoggingGLCalls(void) 
{
	if (_shouldLogGLCalls)
	{
		printf("Stopping GL logging.\n");
		_shouldLogGLCalls = true;
	}
}

void DoLogGLErrorState(const char* fmt, ...) 
{
	char buffer[1024] = {0};  // large buffers
	GetVarargs(buffer, 1024, fmt);
	std::string glTxt = buffer;

	GLenum errCode = glGetError();
	if (errCode) 
	{
		glTxt = stringWithFormat( (char*)"[***GL ERROR***] %s from %s",  GetGLErrorText(errCode).c_str(), glTxt.c_str() );
		CLoggers::sharedLoggers()->logMessageDirectly( CC3_LOG_INFO, glTxt );
	}
	else if (_shouldLogGLCalls) 
	{
		CLoggers::sharedLoggers()->logMessageDirectly( CC3_LOG_INFO, glTxt );
	}
}

NS_COCOS3D_END
