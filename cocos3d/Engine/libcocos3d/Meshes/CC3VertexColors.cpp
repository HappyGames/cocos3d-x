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

CC3VertexColors* CC3VertexColors::vertexArray()
{
	CC3VertexColors* colors = new CC3VertexColors;
	colors->init();
	colors->autorelease();

	return colors;
}

CC3VertexColors* CC3VertexColors::vertexArrayWithName( const std::string& aName )
{
	CC3VertexColors* colors = new CC3VertexColors;
	colors->initWithName( aName );
	colors->autorelease();

	return colors;
}

void CC3VertexColors::setElementType( GLenum elementType )
{
	_elementType = elementType;
	setShouldNormalizeContent( (_elementType != GL_FLOAT) );
}

ccColor4F CC3VertexColors::getColor4FAt( GLuint index )
{
	switch (_elementType) 
	{
		case GL_FIXED:
		case GL_UNSIGNED_BYTE:
			return CCC4FFromCCC4B(*(ccColor4B*)getAddressOfElement(index));
		case GL_FLOAT:
		default:
			return *(ccColor4F*)getAddressOfElement(index);
	}
}

void CC3VertexColors::setColor4F( const ccColor4F& aColor, GLuint index )
{
	switch (_elementType) {
		case GL_FIXED:
		case GL_UNSIGNED_BYTE:
			*(ccColor4B*)getAddressOfElement(index) = CCC4BFromCCC4F(aColor);
			break;
		case GL_FLOAT:
		default:
			*(ccColor4F*)getAddressOfElement(index) = aColor;
	}
}

ccColor4B CC3VertexColors::getColor4BAt( GLuint index )
{
	switch (_elementType) {
		case GL_FLOAT:
			return CCC4BFromCCC4F(*(ccColor4F*)getAddressOfElement(index));
		case GL_FIXED:
		case GL_UNSIGNED_BYTE:
		default:
			return *(ccColor4B*)getAddressOfElement(index);
	}
}

void CC3VertexColors::setColor4B( const ccColor4B& aColor, GLuint index )
{
	switch (_elementType) {
		case GL_FLOAT:
			*(ccColor4F*)getAddressOfElement(index) = CCC4FFromCCC4B(aColor);
			break;
		case GL_FIXED:
		case GL_UNSIGNED_BYTE:
		default:
			*(ccColor4B*)getAddressOfElement(index) = aColor;
	}
}

/**
 * Since material color tracking mucks with both ambient and diffuse material colors under
 * the covers, we won't really know what the ambient and diffuse material color values will
 * be when we get back to setting them...so indicate that to the corresponding trackers.
 */
void CC3VertexColors::bindContent( GLvoid* pointer, GLint vaIdx, CC3NodeDrawingVisitor* visitor )
{
	super::bindContent( pointer, vaIdx, visitor );
	
	CC3OpenGL* gl = visitor->getGL();
	gl->isKnownMat_GL_AMBIENT = false;
	gl->isKnownMat_GL_DIFFUSE = false;
}


/** Returns the color of the first vertex. */
CCColorRef CC3VertexColors::getColor()
{
	if (getVertexCount() == 0) 
		return CCColorRefFromCCC4F(kCCC4FBlackTransparent);

	return CCColorRefFromCCC4B(getColor4BAt(0));
}

/** Sets the color of each vertex without changing the individual opacity of each vertex. */
void CC3VertexColors::setColor( const CCColorRef& color )
{
	ccColor4B c4b = CCC4BFromCCColorRef(color);
	GLuint vtxCnt = getVertexCount();
	for (GLuint vtxIdx = 0; vtxIdx < vtxCnt; vtxIdx++) 
	{
		c4b.a = getColor4BAt( vtxIdx ).a;
		setColor4B( c4b, vtxIdx );
	}
	updateGLBuffer();
}

/** Returns the opacity of the first vertex. */
CCOpacity CC3VertexColors::getOpacity()
{
	return (getVertexCount() > 0) ? CCOpacityFromGLubyte(getColor4BAt(0).a) : kCCOpacityFull; 
}

/** Sets the opacity of each vertex without changing the individual color of each vertex. */
void CC3VertexColors::setOpacity( CCOpacity opacity )
{
	GLubyte alpha = GLubyteFromCCOpacity(opacity);
	GLuint vtxCnt = getVertexCount();
	for (GLuint vtxIdx = 0; vtxIdx < vtxCnt; vtxIdx++) 
	{
		ccColor4B vtxCol = getColor4BAt(vtxIdx);
		vtxCol.a = alpha;
		setColor4B( vtxCol, vtxIdx );
	}
	updateGLBuffer();
}

std::string CC3VertexColors::getNameSuffix()
{
	return "Colors"; 
}

void CC3VertexColors::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	{
		setElementType( GL_UNSIGNED_BYTE );	// Use setter, so shouldNormalizeContent also set
		_elementSize = 4;
	}
}

GLenum CC3VertexColors::defaultSemantic()
{
	return kCC3SemanticVertexColor; 
}

NS_COCOS3D_END
