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

CC3VertexTextureCoordinates* CC3VertexTextureCoordinates::vertexArray()
{
	CC3VertexTextureCoordinates* vertices =new CC3VertexTextureCoordinates;
	vertices->init();
	vertices->autorelease();

	return vertices;
}

CC3VertexTextureCoordinates* CC3VertexTextureCoordinates::vertexArrayWithName( const std::string& aName )
{
	CC3VertexTextureCoordinates* vertices =new CC3VertexTextureCoordinates;
	vertices->initWithName( aName );
	vertices->autorelease();

	return vertices;
}

CCRect CC3VertexTextureCoordinates::getTextureRectangle()
{
	return m_textureRectangle; 
}

CCRect CC3VertexTextureCoordinates::getEffectiveTextureRectangle()
{
	if (m_expectsVerticallyFlippedTextures) 
	{
		return CCRectMake(m_textureRectangle.origin.x * m_mapSize.width,
						  (1.0f - m_textureRectangle.origin.y) * m_mapSize.height,
						  m_textureRectangle.size.width * m_mapSize.width,
						  -m_textureRectangle.size.height * m_mapSize.height);
	} else {
		return CCRectMake(m_textureRectangle.origin.x * m_mapSize.width,
						  m_textureRectangle.origin.y * m_mapSize.height,
						  m_textureRectangle.size.width * m_mapSize.width,
						  m_textureRectangle.size.height * m_mapSize.height);
	}
}

void CC3VertexTextureCoordinates::setTextureRectangle( const CCRect& aRect )
{
	CCRect oldRect = m_textureRectangle;
	m_textureRectangle = aRect;
	alignWithTextureRectangle( aRect, oldRect );
}
	
// Template method that populates this instance from the specified other instance.
// This method is invoked automatically during object copying via the copyWithZone: method.
void CC3VertexTextureCoordinates::populateFrom( CC3VertexTextureCoordinates* another )
{
	super::populateFrom(another);
	
	m_mapSize = another->m_mapSize;
	m_textureRectangle = another->m_textureRectangle;
	m_expectsVerticallyFlippedTextures = another->expectsVerticallyFlippedTextures();
}

CCObject* CC3VertexTextureCoordinates::copyWithZone( CCZone* zone )
{
	CC3VertexTextureCoordinates* pVal = new CC3VertexTextureCoordinates;
	pVal->init();
	pVal->populateFrom( this );
	
	return pVal;
}

bool CC3VertexTextureCoordinates::expectsVerticallyFlippedTextures()
{
	return m_expectsVerticallyFlippedTextures;
}

void CC3VertexTextureCoordinates::setExpectsVerticallyFlippedTextures( bool expects )
{
	m_expectsVerticallyFlippedTextures = expects;
}

static bool _defaultExpectsVerticallyFlippedTextures = false;

bool CC3VertexTextureCoordinates::defaultExpectsVerticallyFlippedTextures()
{
	return _defaultExpectsVerticallyFlippedTextures;
}

void CC3VertexTextureCoordinates::setDefaultExpectsVerticallyFlippedTextures( bool expectsFlipped )
{
	_defaultExpectsVerticallyFlippedTextures = expectsFlipped;
}

ccTex2F CC3VertexTextureCoordinates::getTexCoord2FAt( GLuint index )
{
	return *(ccTex2F*)getAddressOfElement(index); 
}

void CC3VertexTextureCoordinates::setTexCoord2F( const ccTex2F& aTex2F, GLuint index )
{
	*(ccTex2F*)getAddressOfElement(index) = aTex2F;
}

/**
 * Aligns the vertex texture coordinates with the area of the texture defined
 * by the newRect. The oldRect describes the area of the texture that is currently
 * mapped by the texture coordinates.
 */
void CC3VertexTextureCoordinates::alignWithTextureRectangle( const CCRect& newRect, const CCRect& oldRect )
{
	// The size of the texture mapping in its natural state
	GLfloat mw = m_mapSize.width;
	GLfloat mh = m_mapSize.height;
	
	// Old rect components
	GLfloat ox = oldRect.origin.x;
	GLfloat oy = oldRect.origin.y;
	GLfloat ow = oldRect.size.width;
	GLfloat oh = oldRect.size.height;
	
	// New rect components
	GLfloat nx = newRect.origin.x;
	GLfloat ny = newRect.origin.y;
	GLfloat nw = newRect.size.width;
	GLfloat nh = newRect.size.height;
	
	GLfloat hx = 1.0f - m_mapSize.height;	// Height translation due to texture inversion
	
	// For each texture coordinate, convert to the original coordinate, taking into consideration
	// the mapSize and the old texture rectangle. Then, convert to the new coordinate, taking into
	// consideration the mapSize and the new texture rectangle.
	for (GLuint i = 0; i < m_vertexCount; i++) 
	{
		ccTex2F* ptc = (ccTex2F*)getAddressOfElement(i);
		
		GLfloat origU = ((ptc->u / mw) - ox) / ow;			// Revert to original value
		ptc->u = (nx + (origU * nw)) * mw;					// Calc new value
		
		// Take into consideration whether the texture is flipped.
		if (m_expectsVerticallyFlippedTextures) {
			GLfloat origV = (1.0f - (ptc->v / mh) - oy) / oh;	// Revert to original value
			ptc->v = (1.0f - (ny + (origV * nh))) * mh;			// Calc new value
		} else {
			GLfloat origV = (((ptc->v - hx) / mh) - oy) / oh;	// Revert to original value
			ptc->v = (ny + (origV * nh)) * mh + hx;				// Calc new value
		}
	}
	updateGLBuffer();
}

void CC3VertexTextureCoordinates::alignWithTextureCoverage( const CCSize& texCoverage )
{
	CCAssert((texCoverage.width && texCoverage.height), "CC3VertexTextureCoordinates mapsize cannot have zero dimension");

	// Don't waste time adjusting if nothing is changing
	// (eg. POT textures, or new texture has same texture map as old).
	if (texCoverage.equals(m_mapSize)) 
		return;
	
	CC3_TRACE( "[vtx]CC3VertexTextureCoordinates aligning and changing map size from %s to %s but not flipping vertically", 
		stringFromCCSize(m_mapSize).c_str(), stringFromCCSize(texCoverage).c_str() );

	// The scale factor
	CCSize mapRatio = CCSizeMake(texCoverage.width / m_mapSize.width, texCoverage.height / m_mapSize.height);
	
	// The amount by which to translate the image vertically
	GLfloat currVertXln = 1.0f - m_mapSize.height;
	GLfloat newVertXln = 1.0f - texCoverage.height;
	
	for (GLuint i = 0; i < m_vertexCount; i++) 
	{
		ccTex2F* ptc = (ccTex2F*)getAddressOfElement(i);
		ptc->u *= mapRatio.width;
		ptc->v = (ptc->v - currVertXln) * mapRatio.height + newVertXln;
	}
	m_mapSize = texCoverage;	// Remember what we've set the map size to

	updateGLBuffer();
}

void CC3VertexTextureCoordinates::alignWithInvertedTextureCoverage( const CCSize& texCoverage )
{
	CCAssert((texCoverage.width && texCoverage.height), "CC3VertexTextureCoordinates mapsize %s cannot have zero dimension"/*,
			  stringFromCCSize(texCoverage).c_str()*/);
	CC3_TRACE( "[vtx]CC3VertexTextureCoordinates aligning and changing map size from %s to %s and flipping vertically", 
		stringFromCCSize(m_mapSize).c_str(), stringFromCCSize(texCoverage).c_str() );
	
	CCSize mapRatio = CCSizeMake(texCoverage.width / m_mapSize.width, texCoverage.height / m_mapSize.height);
	
	for (GLuint i = 0; i < m_vertexCount; i++) 
	{
		ccTex2F* ptc = (ccTex2F*)getAddressOfElement( i );
		ptc->u *= mapRatio.width;
		ptc->v = texCoverage.height - (ptc->v * mapRatio.height);
	}

	// Remember that we've flipped and what we've set the map size to
	m_mapSize = texCoverage;
	m_expectsVerticallyFlippedTextures = !m_expectsVerticallyFlippedTextures;
	
	updateGLBuffer();
	CC3_TRACE("[vtx]CC3VertexTextureCoordinates aligned and flipped vertically");
}

void CC3VertexTextureCoordinates::alignWithTexture( CC3Texture* texture )
{
	if ( !texture )
		return;
    
	if ( XOR(m_expectsVerticallyFlippedTextures, texture->isUpsideDown()) )
		alignWithInvertedTextureCoverage( texture->getCoverage() );
	else
		alignWithTextureCoverage( texture->getCoverage() );
}

void CC3VertexTextureCoordinates::alignWithInvertedTexture( CC3Texture* texture )
{
	if (!texture) 
		return;

	alignWithInvertedTextureCoverage( texture->getCoverage() );
}

void CC3VertexTextureCoordinates::flipVertically()
{
	GLfloat minV = kCC3MaxGLfloat;
	GLfloat maxV = -kCC3MaxGLfloat;
	for (GLuint i = 0; i < m_vertexCount; i++) 
	{
		ccTex2F* ptc = (ccTex2F*)getAddressOfElement(i);
		minV = MIN(ptc->v, minV);
		maxV = MAX(ptc->v, maxV);
	}
	for (GLuint i = 0; i < m_vertexCount; i++) 
	{
		ccTex2F* ptc = (ccTex2F*)getAddressOfElement(i);
		ptc->v = minV + maxV - ptc->v;
	}
	updateGLBuffer();
}

void CC3VertexTextureCoordinates::flipHorizontally()
{
	GLfloat minU = kCC3MaxGLfloat;
	GLfloat maxU = -kCC3MaxGLfloat;
	for (GLuint i = 0; i < m_vertexCount; i++) 
	{
		ccTex2F* ptc = (ccTex2F*)getAddressOfElement(i);
		minU = MIN(ptc->u, minU);
		maxU = MAX(ptc->u, maxU);
	}
	for (GLuint i = 0; i < m_vertexCount; i++) 
	{
		ccTex2F* ptc = (ccTex2F*)getAddressOfElement(i);
		ptc->u = minU + maxU - ptc->u;
	}

	updateGLBuffer();
}

void CC3VertexTextureCoordinates::repeatTexture( const ccTex2F& repeatFactor )
{
	CCSize repeatSize = CCSizeMake(repeatFactor.u * m_mapSize.width, repeatFactor.v * m_mapSize.height);
	alignWithTextureCoverage( repeatSize );
}

std::string CC3VertexTextureCoordinates::getNameSuffix()
{
	return "TexCoords"; 
}

void CC3VertexTextureCoordinates::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	{
		m_elementType = GL_FLOAT;
		m_elementSize = 2;
		m_mapSize = CCSizeMake(1, 1);
		m_textureRectangle = kCC3UnitTextureRectangle;
		m_expectsVerticallyFlippedTextures = defaultExpectsVerticallyFlippedTextures();
	}
}

GLenum CC3VertexTextureCoordinates::defaultSemantic()
{
	return kCC3SemanticVertexTexture; 
}

NS_COCOS3D_END
