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

CC3TextureUnit::CC3TextureUnit()
{
	init();
}

CC3Vector CC3TextureUnit::getLightDirection()
{
	// Extract half-scaled normal vector from constantColor, according to RGB <-> normal mapping
	CC3Vector hv;
	switch (m_rgbNormalMap) 
	{
		case kCC3DOT3RGB_XZY:
			hv = cc3v(m_constantColor.r, m_constantColor.b, m_constantColor.g);
			break;
		case kCC3DOT3RGB_YXZ:
			hv = cc3v(m_constantColor.g, m_constantColor.r, m_constantColor.b);
			break;
		case kCC3DOT3RGB_YZX:
			hv = cc3v(m_constantColor.b, m_constantColor.r, m_constantColor.g);
			break;
		case kCC3DOT3RGB_ZXY:
			hv = cc3v(m_constantColor.g, m_constantColor.b, m_constantColor.r);
			break;
		case kCC3DOT3RGB_ZYX:
			hv = cc3v(m_constantColor.b, m_constantColor.g, m_constantColor.r);
			break;
		case kCC3DOT3RGB_XYZ:
		default:
			hv = cc3v(m_constantColor.r, m_constantColor.g, m_constantColor.b);
			break;
	}
	// Convert half-scaled vector between 0.0 and 1.0 to range +/- 1.0.
	return hv.scaleUniform(2.0f).difference(CC3Vector::kCC3VectorUnitCube);
}

void CC3TextureUnit::setLightDirection( const CC3Vector& aDirection )
{
	CC3Vector direction = aDirection;
	// Normalize direction, then half-shift to move value from +/-1.0 to between 0.0 and 1.0
	direction = aDirection.normalize();
	CC3Vector hv = direction.average(CC3Vector::kCC3VectorUnitCube);
	
	// Set constantColor from normal direction, according to RGB <-> normal mapping
	switch (m_rgbNormalMap) 
	{
		case kCC3DOT3RGB_XYZ:
			m_constantColor = ccc4f(hv.x, hv.y, hv.z, 1.0f);
			break;
		case kCC3DOT3RGB_XZY:
			m_constantColor = ccc4f(hv.x, hv.z, hv.y, 1.0f);
			break;
		case kCC3DOT3RGB_YXZ:
			m_constantColor = ccc4f(hv.y, hv.x, hv.z, 1.0f);
			break;
		case kCC3DOT3RGB_YZX:
			m_constantColor = ccc4f(hv.y, hv.z, hv.x, 1.0f);
			break;
		case kCC3DOT3RGB_ZXY:
			m_constantColor = ccc4f(hv.z, hv.x, hv.y, 1.0f);
			break;
		case kCC3DOT3RGB_ZYX:
			m_constantColor = ccc4f(hv.z, hv.y, hv.x, 1.0f);
			break;
	}
}

bool CC3TextureUnit::isBumpMap()
{
	return false;
}

CCColorRef CC3TextureUnit::getColor()
{
	return CCColorRefFromCCC4F(m_constantColor);
}

void CC3TextureUnit::setColor( const CCColorRef& aColor )
{
	ccColor4F c4f = CCC4FFromCCColorRef(aColor);
	m_constantColor.r = c4f.r;
	m_constantColor.g = c4f.g;
	m_constantColor.b = c4f.b;
}

CCOpacity CC3TextureUnit::getOpacity()
{
	return CCOpacityFromGLfloat(m_constantColor.a); 
}

void CC3TextureUnit::setOpacity( CCOpacity opacity )
{
	m_constantColor.a = GLfloatFromCCOpacity(opacity);
}

CCColorRef CC3TextureUnit::getDisplayedColor()
{
	return getColor();
}

bool CC3TextureUnit::isCascadeColorEnabled()
{
	return false;
}

void CC3TextureUnit::setCascadeColorEnabled( bool cascadeColorEnabled )
{

}

void CC3TextureUnit::updateDisplayedColor( const CCColorRef& color )
{

}

CCOpacity CC3TextureUnit::getDisplayedOpacity()
{
	return getOpacity();
}

bool CC3TextureUnit::isCascadeOpacityEnabled()
{
	return false;
}

void CC3TextureUnit::setCascadeOpacityEnabled( bool cascadeOpacityEnabled )
{

}

void CC3TextureUnit::updateDisplayedOpacity( CCOpacity opacity )
{

}

bool CC3TextureUnit::init()
{
	m_textureEnvironmentMode = GL_MODULATE;
	m_constantColor = kCCC4FBlackTransparent;
	m_rgbNormalMap = kCC3DOT3RGB_XYZ;

	return true;
}

CC3TextureUnit* CC3TextureUnit::textureUnit()
{
	CC3TextureUnit* pUnit = new CC3TextureUnit;
	pUnit->init();
	pUnit->autorelease();

	return pUnit;
}

// Template method that populates this instance from the specified other instance.
// This method is invoked automatically during object copying via the copyWithZone: method.
void CC3TextureUnit::populateFrom( CC3TextureUnit* another )
{
	m_textureEnvironmentMode = another->getTextureEnvironmentMode();
	m_constantColor = another->getConstantColor();
	m_rgbNormalMap = another->getRgbNormalMap();
}

CCObject* CC3TextureUnit::copyWithZone( CCZone* zone )
{
	CC3TextureUnit* aCopy = new CC3TextureUnit;
	aCopy->populateFrom( this );

	return aCopy;
}

void CC3TextureUnit::bindWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	CC3OpenGL* gl = visitor->getGL();
	GLuint tuIdx = visitor->getCurrent2DTextureUnit();
	gl->setTextureEnvMode(m_textureEnvironmentMode, tuIdx );
	gl->setTextureEnvColor( m_constantColor, tuIdx );
}

void CC3TextureUnit::bindDefaultWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	CC3OpenGL* gl = visitor->getGL();
	GLuint tuIdx = visitor->getCurrent2DTextureUnit();
	gl->setTextureEnvMode( GL_MODULATE, tuIdx );
	gl->setTextureEnvColor( kCCC4FBlackTransparent, tuIdx );
}

GLenum CC3TextureUnit::getTextureEnvironmentMode()
{
	return m_textureEnvironmentMode;
}

void CC3TextureUnit::setTextureEnvironmentMode( GLenum mode )
{
	m_textureEnvironmentMode = mode;
}

void CC3TextureUnit::setConstantColor( const ccColor4F& color )
{
	m_constantColor = color;
}

ccColor4F CC3TextureUnit::getConstantColor()
{
	return m_constantColor;
}

CC3DOT3RGB CC3TextureUnit::getRgbNormalMap()
{
	return m_rgbNormalMap;
}

void CC3TextureUnit::setRgbNormalMap( CC3DOT3RGB normalMap )
{
	m_rgbNormalMap = normalMap;
}

CC3ConfigurableTextureUnit::CC3ConfigurableTextureUnit()
{
	init();
}

bool CC3ConfigurableTextureUnit::isBumpMap()
{
	return getTextureEnvironmentMode() == GL_COMBINE &&
			(m_combineRGBFunction == GL_DOT3_RGB || m_combineRGBFunction == GL_DOT3_RGBA);
}

bool CC3ConfigurableTextureUnit::init()
{
	super::init();
	setTextureEnvironmentMode( GL_COMBINE );
	m_combineRGBFunction = GL_MODULATE;
	m_rgbSource0 = GL_TEXTURE;
	m_rgbSource1 = GL_PREVIOUS;
	m_rgbSource2 = GL_CONSTANT;
	m_rgbOperand0 = GL_SRC_COLOR;
	m_rgbOperand1 = GL_SRC_COLOR;
	m_rgbOperand2 = GL_SRC_ALPHA;
	m_combineAlphaFunction = GL_MODULATE;
	m_alphaSource0 = GL_TEXTURE;
	m_alphaSource1 = GL_PREVIOUS;
	m_alphaSource2 = GL_CONSTANT;
	m_alphaOperand0 = GL_SRC_ALPHA;
	m_alphaOperand1 = GL_SRC_ALPHA;
	m_alphaOperand2 = GL_SRC_ALPHA;

	return true;
}

// Template method that populates this instance from the specified other instance.
// This method is invoked automatically during object copying via the copyWithZone: method.
void CC3ConfigurableTextureUnit::populateFrom( CC3ConfigurableTextureUnit* another )
{
	super::populateFrom( another );
	
	m_combineRGBFunction = another->getCombineRGBFunction();
	m_rgbSource0 = another->getRgbSource0();
	m_rgbSource1 = another->getRgbSource1();
	m_rgbSource2 = another->getRgbSource2();
	m_rgbOperand0 = another->getRgbOperand0();
	m_rgbOperand1 = another->getRgbOperand1();
	m_rgbOperand2 = another->getRgbOperand2();
	m_combineAlphaFunction = another->getCombineAlphaFunction();
	m_alphaSource0 = another->getAlphaSource0();
	m_alphaSource1 = another->getAlphaSource1();
	m_alphaSource2 = another->getAlphaSource2();
	m_alphaOperand0 = another->getAlphaOperand0();
	m_alphaOperand1 = another->getAlphaOperand1();
	m_alphaOperand2 = another->getAlphaOperand2();
}

CCObject* CC3ConfigurableTextureUnit::copyWithZone( CCZone* zone )
{
	CC3ConfigurableTextureUnit* pVal = new CC3ConfigurableTextureUnit;
	pVal->init();
	pVal->populateFrom( this );
	
	return pVal;
}

GLuint CC3ConfigurableTextureUnit::getAlphaOperand0()
{
	return m_alphaOperand0;
}

GLuint CC3ConfigurableTextureUnit::getAlphaOperand1()
{
	return m_alphaOperand1;
}

GLuint CC3ConfigurableTextureUnit::getAlphaOperand2()
{
	return m_alphaOperand2;
}

GLuint CC3ConfigurableTextureUnit::getAlphaSource0()
{
	return m_alphaSource2;
}

GLuint CC3ConfigurableTextureUnit::getAlphaSource1()
{
	return m_alphaSource1;
}

GLuint CC3ConfigurableTextureUnit::getAlphaSource2()
{
	return m_alphaSource2;
}

GLuint CC3ConfigurableTextureUnit::getCombineAlphaFunction()
{
	return m_combineAlphaFunction;
}

GLuint CC3ConfigurableTextureUnit::getRgbOperand0()
{
	return m_rgbOperand0;
}

GLuint CC3ConfigurableTextureUnit::getRgbOperand1()
{
	return m_rgbOperand1;
}

GLuint CC3ConfigurableTextureUnit::getRgbOperand2()
{
	return m_rgbOperand2;
}

GLuint CC3ConfigurableTextureUnit::getCombineRGBFunction()
{
	return m_combineRGBFunction;
}

GLuint CC3ConfigurableTextureUnit::getRgbSource0()
{
	return m_rgbSource0;
}

GLuint CC3ConfigurableTextureUnit::getRgbSource1()
{
	return m_rgbSource1;
}

GLuint CC3ConfigurableTextureUnit::getRgbSource2()
{
	return m_rgbSource2;
}

void CC3ConfigurableTextureUnit::setTextureEnvironmentMode( GLenum mode )
{
	m_textureEnvironmentMode = mode;
}

GLenum CC3ConfigurableTextureUnit::getTextureEnvironmentMode()
{
	return m_textureEnvironmentMode;
}

CC3BumpMapTextureUnit::CC3BumpMapTextureUnit()
{
	init();
}

bool CC3BumpMapTextureUnit::isBumpMap()
{ 
	return true;
}

bool CC3BumpMapTextureUnit::init()
{
	if ( super::init() )
	{
		m_textureEnvironmentMode = GL_COMBINE;
		return true;
	}

	return false;
}

CC3BumpMapTextureUnit* CC3BumpMapTextureUnit::textureUnit()
{
	CC3BumpMapTextureUnit* pUnit = new CC3BumpMapTextureUnit;
	pUnit->init();
	pUnit->autorelease();

	return pUnit;
}

NS_COCOS3D_END
