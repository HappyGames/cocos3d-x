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

CC3Material::CC3Material()
{
	m_pTexture = NULL;
	m_textureOverlays = NULL;
}

CC3Material::~CC3Material()
{
	removeAllTextures();

	CC_SAFE_RELEASE( m_pTexture );
	CC_SAFE_RELEASE( m_textureOverlays );
}

static std::string getNameSuffix()
{ 
	return "Material"; 
}

bool CC3Material::shouldUseLighting()
{
	return m_shouldUseLighting;
}

void CC3Material::setShouldUseLighting( bool shouldUse )
{
	m_shouldUseLighting = shouldUse;
}

void CC3Material::setSpecularColor( const ccColor4F& color )
{
	m_specularColor = color;
}

ccColor4F CC3Material::getSpecularColor()
{
	return m_specularColor;
}

ccColor4F CC3Material::getEmissionColor()
{
	return m_emissionColor;
}

void CC3Material::setEmissionColor( const ccColor4F& color )
{
	m_emissionColor = color;
}

ccColor4F CC3Material::getAmbientColor()
{
	return m_ambientColor;
}

void CC3Material::setAmbientColor( const ccColor4F& color )
{
	m_ambientColor = color;
}

ccColor4F CC3Material::getDiffuseColor()
{
	return m_diffuseColor;
}

void CC3Material::setDiffuseColor( const ccColor4F& color )
{
	m_diffuseColor = color;
}

void CC3Material::setBlendFuncRGB( const ccBlendFunc& blendFunc )
{
	m_blendFuncRGB = blendFunc;
}

ccBlendFunc CC3Material::getBlendFuncRGB()
{
	return m_blendFuncRGB;
}

void CC3Material::setBlendFuncAlpha( const ccBlendFunc& blendFunc )
{
	m_blendFuncAlpha = blendFunc;
}

// Clamp to allowed range
void CC3Material::setShininess( GLfloat shininess )
{ 
	m_fShininess = CLAMP(shininess, 0.0f, kCC3MaximumMaterialShininess); 
}

GLfloat CC3Material::getShininess()
{
	return m_fShininess;
}

// Clamp to allowed range
void CC3Material::setReflectivity( GLfloat reflectivity )
{ 
	m_fReflectivity = CLAMP(reflectivity, 0.0f, 1.0f); 
}

GLfloat CC3Material::getReflectivity()
{
	return m_fReflectivity;
}

GLenum CC3Material::getSourceBlend() 
{
	return getSourceBlendRGB(); 
}

void CC3Material::setSourceBlend( GLenum aBlend )
{
	setSourceBlendRGB( aBlend );
	setSourceBlendAlpha( aBlend );
}

GLenum CC3Material::getDestinationBlend()
{ 
	return getDestinationBlendRGB(); 
}

void CC3Material::setDestinationBlend( GLenum aBlend )
{
	setDestinationBlendRGB( aBlend );
	setDestinationBlendAlpha( aBlend );
}

GLenum CC3Material::getSourceBlendRGB()
{ 
	return m_blendFuncRGB.src; 
}

void CC3Material::setSourceBlendRGB( GLenum aBlend )
{ 
	m_blendFuncRGB.src = aBlend; 
}

GLenum CC3Material::getDestinationBlendRGB()
{ 
	return m_blendFuncRGB.dst; 
}

void CC3Material::setDestinationBlendRGB( GLenum aBlend )
{ 
	m_blendFuncRGB.dst = aBlend; 
}

GLenum CC3Material::getSourceBlendAlpha()
{ 
	return m_blendFuncAlpha.src; 
}

void CC3Material::setSourceBlendAlpha( GLenum aBlend )
{
	m_blendFuncAlpha.src = aBlend; 
}

GLenum CC3Material::getDestinationBlendAlpha() 
{ 
	return m_blendFuncAlpha.dst; 
}

void CC3Material::setDestinationBlendAlpha( GLenum aBlend )
{ 
	m_blendFuncAlpha.dst = aBlend; 
}

bool CC3Material::shouldBlendAtFullOpacity()
{ 
	return m_shouldBlendAtFullOpacity; 
}

void CC3Material::setShouldBlendAtFullOpacity( bool shouldBlendAtFullOpacity )
{
	m_shouldBlendAtFullOpacity = shouldBlendAtFullOpacity;
	setIsOpaque( isOpaque() );
}

bool CC3Material::isOpaque()
{ 
	return (getSourceBlendRGB() == GL_ONE && getDestinationBlendRGB() == GL_ZERO); 
}

/**
 * If I should be opaque, turn off alpha blending. If I should not be opaque and I
 * already have a blend, leave it alone. Otherwise, set an appropriate standard blend.
 */
void CC3Material::setIsOpaque( bool shouldBeOpaque )
{
	if (shouldBeOpaque) 
	{
		// I should be opaque, so turn off alpha blending altogether.
		setSourceBlend( GL_ONE );
		setDestinationBlend( GL_ZERO );
	} 
	else 
	{
		// If a source blend has not yet been set AND the texture does NOT contain pre-multiplied
		// alpha, set a source alpha blend. If the texture contains pre-multiplied alpha, leave the
		// source blend at GL_ONE and apply the opacity to the color of the material instead.
		bool noPreMultAlpha = !hasTexturePremultipliedAlpha();
		if ( (getSourceBlendRGB() == GL_ONE) && noPreMultAlpha ) 
			setSourceBlendRGB( GL_SRC_ALPHA );

		if ( (getSourceBlendAlpha() == GL_ONE) && noPreMultAlpha )
			setSourceBlendAlpha( GL_SRC_ALPHA );
		
		// If destination blend has not yet been set, set it a destination alpha blend.
		if (getDestinationBlendRGB() == GL_ZERO) 
			setDestinationBlendRGB( GL_ONE_MINUS_SRC_ALPHA );

		if (getDestinationBlendAlpha() == GL_ZERO) 
			setDestinationBlendAlpha( GL_ONE_MINUS_SRC_ALPHA );
	}
}

bool CC3Material::shouldDrawLowAlpha()
{
	switch (m_alphaTestFunction) {
		case GL_ALWAYS:
		case GL_LESS:
		case GL_LEQUAL:
			return true;
		default:
			return false;
	}
}

void CC3Material::setShouldDrawLowAlpha( bool shouldDraw )
{
	m_alphaTestFunction = shouldDraw ? GL_ALWAYS : GL_GREATER;
}

bool CC3Material::shouldApplyOpacityToColor()
{ 
	return getSourceBlendRGB() == GL_ONE && hasTexturePremultipliedAlpha();
}

ccColor4F CC3Material::getEffectiveAmbientColor()
{
	return shouldApplyOpacityToColor() ? CCC4FBlendAlpha(getAmbientColor()) : getAmbientColor();
}

ccColor4F CC3Material::getEffectiveDiffuseColor()
{
	return shouldApplyOpacityToColor() ? CCC4FBlendAlpha(getDiffuseColor()) : getDiffuseColor();
}

ccColor4F CC3Material::getEffectiveSpecularColor()
{
	return shouldApplyOpacityToColor() ? CCC4FBlendAlpha(getSpecularColor()) : getSpecularColor();
}

ccColor4F CC3Material::getEffectiveEmissionColor()
{
	return shouldApplyOpacityToColor() ? CCC4FBlendAlpha(getEmissionColor()) : getEmissionColor();
}

CCColorRef CC3Material::getColor() { return CCColorRefFromCCC4F(m_shouldUseLighting ? m_diffuseColor : m_emissionColor); }

void CC3Material::setColor( const CCColorRef& color )
{
	ccColor4F c4f = CCC4FFromCCColorRef(color);

	if (m_shouldUseLighting) {
		m_ambientColor.r = c4f.r;
		m_ambientColor.g = c4f.g;
		m_ambientColor.b = c4f.b;
		
		m_diffuseColor.r = c4f.r;
		m_diffuseColor.g = c4f.g;
		m_diffuseColor.b = c4f.b;
	} else {
		m_emissionColor.r = c4f.r;
		m_emissionColor.g = c4f.g;
		m_emissionColor.b = c4f.b;
	}
}

CCOpacity CC3Material::getOpacity()
{
	return CCOpacityFromGLfloat(m_shouldUseLighting ? m_diffuseColor.a : m_emissionColor.a); 
}

/**
 * Set opacity of all colors, retaining the colors of each, and sets the isOpaque property
 * to the appropriate value. This is a convenience that ensures that a previously opaque node
 * can be faded without having to turn isOpaque off separately.
 */
void CC3Material::setOpacity( CCOpacity opacity )
{
	GLfloat alpha = GLfloatFromCCOpacity(opacity);
	m_ambientColor.a = alpha;
	m_diffuseColor.a = alpha;
	m_specularColor.a = alpha;
	m_emissionColor.a = alpha;

	// As a convenience, set the blending to be compatible with the opacity level.
	// If the opacity has been reduced below full, set isOpaque to NO to ensure alpha
	// blending will occur. If the opacity is full, set isOpaque to YES only if if the
	// shouldBlendAtFullOpacity flag is set to YES. This ensures that a texture
	// with transparency will still blend, even when this material is at full opacity.
	setIsOpaque( (opacity == kCCOpacityFull && !shouldBlendAtFullOpacity()) );
}

CCColorRef CC3Material::getDisplayedColor()
{ 
	return getColor(); 
}

bool CC3Material::isCascadeColorEnabled()
{
	return false; 
}

void CC3Material::setCascadeColorEnabled( bool cascadeColorEnabled )
{

}

void CC3Material::updateDisplayedColor( const CCColorRef& color )
{

}

CCOpacity CC3Material::getDisplayedOpacity()
{ 
	return getOpacity(); 
}

bool CC3Material::isCascadeOpacityEnabled()
{ 
	return false; 
}

void CC3Material::setCascadeOpacityEnabled( bool cascadeOpacityEnabled )
{

}

void CC3Material::updateDisplayedOpacity( CCOpacity opacity )
{

}

ccBlendFunc CC3Material::getBlendFunc()
{ 
	return getBlendFuncRGB(); 
}

void CC3Material::setBlendFunc( const ccBlendFunc& blendFunc )
{
	setBlendFuncRGB( blendFunc );
	setBlendFuncAlpha( blendFunc );
}

static ccBlendFunc _defaultBlendFunc = {GL_ONE, GL_ZERO};

ccBlendFunc CC3Material::getDefaultBlendFunc()
{ 
	return _defaultBlendFunc; 
}

void CC3Material::setDefaultBlendFunc( const ccBlendFunc& aBlendFunc )
{ 
	_defaultBlendFunc = aBlendFunc; 
}

ccBlendFunc CC3Material::getBlendFuncAlpha()
{
	return m_blendFuncAlpha;
}

GLuint CC3Material::getAlphaTestFunction()
{
	return m_alphaTestFunction;
}

GLfloat CC3Material::getAlphaTestReference()
{
	return m_fAlphaTestReference;
}

void CC3Material::setAlphaTestReference( float alphaTestRef )
{
	m_fAlphaTestReference = alphaTestRef; 
}

GLuint CC3Material::getTextureCount()
{ 
	return (m_textureOverlays ? (GLuint)m_textureOverlays->count() : 0) + (m_pTexture ? 1 : 0); 
}

CC3Texture* CC3Material::getTexture()
{ 
	return m_pTexture; 
}

void CC3Material::setTexture( CC3Texture* aTexture )
{
	if (aTexture == m_pTexture) 
		return;
	
	CC_SAFE_RELEASE(m_pTexture);
	m_pTexture = aTexture;
	CC_SAFE_RETAIN(aTexture);
	
	texturesHaveChanged();
}

// If the texture property has not been set yet, set it. Otherwise add as an overlay.
void CC3Material::addTexture( CC3Texture* aTexture )
{
	CC3_TRACE( "Adding texture to CC3Material" );
	if ( !m_pTexture ) 
	{
		setTexture( aTexture );
	} 
	else 
	{
		CCAssert(aTexture, "CC3Materail cannot add a nil overlay texture");
		if ( !m_textureOverlays ) 
		{
			m_textureOverlays = CCArray::createWithCapacity(4);		// retained
			m_textureOverlays->retain();
		}

		GLuint maxTexUnits = CC3OpenGL::sharedGL()->getMaxNumberOfTextureUnits();
		if ( getTextureCount() < maxTexUnits ) 
		{
			m_textureOverlays->addObject( aTexture );
		} 
		else 
		{
			CC3_TRACE("Attempt to add texture  to CC3Material ignored because platform supports only %d texture units.", maxTexUnits);
		}

		texturesHaveChanged();
	}
}

// If it's the texture property, clear it, otherwise remove the overlay.
void CC3Material::removeTexture( CC3Texture* aTexture )
{
	CC3_TRACE("Removing texture from CC3Material");
	if (aTexture == m_pTexture)
	{
		setTexture( NULL );
	} 
	else 
	{
		if (m_textureOverlays && aTexture) 
		{
			m_textureOverlays->removeObject( aTexture );
			texturesHaveChanged();
			if (m_textureOverlays->count() == 0) 
			{
				m_textureOverlays->release();
				m_textureOverlays = NULL;
			}
		}
	}
}

void CC3Material::removeAllTextures()
{
	// Remove the first texture
	removeTexture( m_pTexture );

	// Remove the overlay textures
#if 0
	if (_textureOverlays) 
	{
		CCArray* myOTs = (CCArray*)_textureOverlays->copy();

		CCObject* pObj = NULL;
		CCARRAY_FOREACH( myOTs, pObj )
		{
			CC3Texture* pTex = (CC3Texture*)pObj;
			if ( pTex )
				removeTexture( pTex );
		}
		
		myOTs->release();
	}
#else
	while( m_textureOverlays && m_textureOverlays->count() > 0 )
	{
		removeTexture( (CC3Texture*)m_textureOverlays->objectAtIndex(0) );
	}
#endif
}

CC3Texture* CC3Material::getTextureForTextureUnit( GLuint texUnit )
{
	if (texUnit == 0) 
		return m_pTexture;
	
	texUnit--;	// Remaining texture units are indexed into the overlays array

	if ( m_textureOverlays == NULL ) 
		return NULL;

	if (texUnit < m_textureOverlays->count()) 
		return (CC3Texture*)m_textureOverlays->objectAtIndex( texUnit );

	return NULL;
}

void CC3Material::setTexture( CC3Texture* aTexture, GLuint texUnit )
{
	if (texUnit == 0) {
		setTexture( aTexture );
	} else if (texUnit < getTextureCount()) {
		CCAssert(aTexture, "CC3Material cannot set an overlay texture to nil");
		GLuint overlayIdx = texUnit - 1;
		if ( aTexture != m_textureOverlays->objectAtIndex( overlayIdx )) {
			m_textureOverlays->replaceObjectAtIndex( overlayIdx, aTexture );
			texturesHaveChanged();
		}
	} else {
		addTexture( aTexture );
	}
}

// Returns a texture if name is equal or both are nil.
CC3Texture* CC3Material::getTextureNamed( const char* aName )
{
	std::string tcName;
	
	// First check if the first texture is the one
	if (m_pTexture) {
		tcName = m_pTexture->getName();
		if (tcName == aName || (tcName.empty() && !aName)) 
			return m_pTexture;
	}
	// Then look for it in the overlays array
	if (m_textureOverlays) {
		CCObject* pObj = NULL;
		CCARRAY_FOREACH( m_textureOverlays, pObj )
		{
			CC3Texture* pTex = (CC3Texture*)pObj;
			if ( pTex )
			{
				tcName = pTex->getName();
				if (tcName == aName || (tcName.empty() && !aName)) 
					return m_pTexture;
			}
		}
	}

	return NULL;
}

/**
 * The textures have changed in some way.
 *
 * Updates the blend, by setting the shouldBlendAtFullOpacity property, based on whether
 * any textures have an alpha channel, which in turn will update the isOpaque property.
 */
void CC3Material::texturesHaveChanged()
{
	setShouldBlendAtFullOpacity( hasTextureAlpha() );
}

bool CC3Material::hasTextureAlpha()
{
	// Check the first texture.
	if (m_pTexture && m_pTexture->hasAlpha()) 
		return true;
	
	// Then check in the overlays array
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_textureOverlays, pObj )
	{
		CC3Texture* pTex = (CC3Texture*)pObj;
		if ( pTex && pTex->hasAlpha() )
		{
			return true;
		}
	}
	
	return false;
}

bool CC3Material::hasTexturePremultipliedAlpha()
{
	// Check the first texture.
	if (m_pTexture && m_pTexture->hasPremultipliedAlpha())
		return true;
	
	// Then check in the overlays array
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_textureOverlays, pObj )
	{
		CC3Texture* pTex = (CC3Texture*)pObj;
		if ( pTex && pTex->hasPremultipliedAlpha() )
		{
			return true;
		}
	}
	
	return false;
}

// Check the first texture, hen check in the overlays array
CC3Texture* CC3Material::getTextureCube()
{
	if (m_pTexture && m_pTexture->isTextureCube()) 
		return m_pTexture;

	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_textureOverlays, pObj )
	{
		CC3Texture* pTex = (CC3Texture*)pObj;
		if ( pTex && pTex->isTextureCube() )
		{
			return pTex;
		}
	}

	return NULL;
}

bool CC3Material::hasTextureCube()
{ 
	return (getTextureCube() != NULL); 
}

// Check the first texture, hen check in the overlays array
bool CC3Material::hasBumpMap()
{
	if (m_pTexture && m_pTexture->isBumpMap()) 
		return true;

	// Then check in the overlays array
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_textureOverlays, pObj )
	{
		CC3Texture* pTex = (CC3Texture*)pObj;
		if ( pTex && pTex->isBumpMap() )
		{
			return true;
		}
	}

	return false;
}

CC3Vector CC3Material::getLightDirection()
{
	// Check the first texture.
	if (m_pTexture && m_pTexture->isBumpMap()) 
		return m_pTexture->getLightDirection();
	
	// Then check in the overlays array
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_textureOverlays, pObj )
	{
		CC3Texture* pTex = (CC3Texture*)pObj;
		if ( pTex && pTex->isBumpMap() )
		{
			return pTex->getLightDirection();
		}
	}
	
	return CC3Vector::kCC3VectorZero;
}

void CC3Material::setLightDirection( const CC3Vector& aDirection )
{
	// Set the first texture.
	if ( m_pTexture )
		m_pTexture->setLightDirection( aDirection );
	
	// Then set in the overlays array
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_textureOverlays, pObj )
	{
		CC3Texture* pTex = (CC3Texture*)pObj;
		if ( pTex )
		{
			return pTex->setLightDirection( aDirection );
		}
	}
}

void CC3Material::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	{
		m_pTexture = NULL;
		m_textureOverlays = NULL;
		m_ambientColor = kCC3DefaultMaterialColorAmbient;
		m_diffuseColor = kCC3DefaultMaterialColorDiffuse;
		m_specularColor = kCC3DefaultMaterialColorSpecular;
		m_emissionColor = kCC3DefaultMaterialColorEmission;
		m_fShininess = kCC3DefaultMaterialShininess;
		m_fReflectivity = kCC3DefaultMaterialReflectivity;
		setBlendFunc( getDefaultBlendFunc() );
		m_shouldBlendAtFullOpacity = false;
		m_alphaTestFunction = GL_ALWAYS;
		m_fAlphaTestReference = 0.0f;
		m_shouldUseLighting = true;
	}
}

void CC3Material::initWithTag( GLuint aTag )
{
	super::initWithTag( aTag );
}

CC3Material* CC3Material::material()
{
	CC3Material* pMat = new CC3Material;
	pMat->init();
	pMat->autorelease();
	return pMat;
}

CC3Material* CC3Material::materialWithTag( GLuint aTag )
{
	CC3Material* pMat = new CC3Material;
	pMat->initWithTag( aTag );
	pMat->autorelease();
	return pMat;
}

CC3Material* CC3Material::materialWithName( const std::string& aName )
{
	CC3Material* pMat = new CC3Material;
	pMat->initWithName( aName );
	pMat->autorelease();
	return pMat;
}

CC3Material* CC3Material::materialWithTag( GLuint aTag, const std::string& aName )
{
	CC3Material* pMat = new CC3Material;
	pMat->initWithTag( aTag, aName );
	pMat->autorelease();
	return pMat;
}

CC3Material* CC3Material::shiny() 
{
	CC3Material* mat = material();
	mat->setSpecularColor( kCCC4FWhite );
	mat->setShininess( 75.0f );
	return mat;
}

CC3Material* CC3Material::shinyWhite()
{
	CC3Material* mat = shiny();
	mat->setDiffuseColor( kCCC4FWhite );
	return mat;
}

void CC3Material::populateFrom( CC3Material* another )
{
	super::populateFrom( another );

	m_ambientColor = another->getAmbientColor();
	m_diffuseColor = another->getDiffuseColor();
	m_specularColor = another->getSpecularColor();
	m_emissionColor = another->getEmissionColor();
	m_fShininess = another->getShininess();
	m_fReflectivity = another->getReflectivity();
	m_blendFuncRGB = another->getBlendFuncRGB();
	m_blendFuncAlpha = another->getBlendFuncAlpha();
	m_alphaTestFunction = another->getAlphaTestFunction();
	m_fAlphaTestReference = another->getAlphaTestReference();
	m_shouldUseLighting = another->shouldUseLighting();
	
	CC_SAFE_RELEASE( m_pTexture );
	m_pTexture = another->m_pTexture;	
	CC_SAFE_RETAIN( m_pTexture );	// retained - don't want to trigger texturesHaveChanged
	
	// Remove any existing overlays and add the overlays from the other material.
	if ( m_textureOverlays )
		m_textureOverlays->removeAllObjects();

	CCArray* otherOTs = another->m_textureOverlays;
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( otherOTs, pObj )
	{
		CC3Texture* pTex = (CC3Texture*)pObj;
		if ( pTex )
		{
			addTexture( pTex );
		}
	}
}

CCObject* CC3Material::copyWithZone( CCZone* zone )
{
	CC3Material* pMat = new CC3Material;
	pMat->init();
	pMat->populateFrom( this );

	return pMat;
}

// Class variable tracking the most recent tag value assigned for CC3Materials.
// This class variable is automatically incremented whenever the method nextTag is called.
static GLuint lastAssignedMaterialTag;

static GLuint nextTag()
{ 
	return ++lastAssignedMaterialTag; 
}

static void resetTagAllocation()
{ 
	lastAssignedMaterialTag = 0; 
}

/**
 * Template method to apply the material and texture properties to the GL engine.
 * The visitor keeps track of which texture unit is being processed, with each texture
 * incrementing the appropriate texture unit counter as it draws. GL texture units that
 * are not used by the textures are disabled.
 */
void CC3Material::drawWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	visitor->resetTextureUnits();

	if (visitor->shouldDecorateNode())
	{
		applyAlphaTestWithVisitor( visitor );
		applyBlendWithVisitor( visitor );
		applyColorsWithVisitor( visitor );
		drawTexturesWithVisitor( visitor );
	} else {
		unbindWithVisitor( visitor );
	}

	visitor->disableUnusedTextureUnits();
}

/**
 * Enables or disables alpha testing in the GL engine, depending on the whether or not
 * the alphaTestFunction indicates that alpha testing should occur, and applies the
 * alphaTestFunction and alphaTestReference properties.
 */
void CC3Material::applyAlphaTestWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	CC3OpenGL* gl = visitor->getGL();
	bool shouldAlphaTest = (m_alphaTestFunction != GL_ALWAYS);
	gl->enableAlphaTesting( shouldAlphaTest );
	if (shouldAlphaTest) 
		gl->setAlphaFunc( m_alphaTestFunction, m_fAlphaTestReference );
}

/**
 * Enables or disables blending in the GL engine, depending on the whether or not this
 * instance is opaque or not, and applies the sourceBlend and destinationBlend properties.
 */
void CC3Material::applyBlendWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	CC3OpenGL* gl = visitor->getGL();
	bool shouldBlend = !isOpaque();
	gl->enableBlend( shouldBlend );
	if (shouldBlend) 
		gl->setBlendFunc( m_blendFuncRGB.src, m_blendFuncRGB.dst, m_blendFuncAlpha.src, m_blendFuncAlpha.dst );
}

/**
 * If the shouldUseLighting property is YES, applies the color and shininess properties to
 * the GL engine, otherwise turns lighting off and applies diffuse color as a flat color.
 */
void CC3Material::applyColorsWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	CC3OpenGL* gl = visitor->getGL();
	if (m_shouldUseLighting) 
	{
		gl->enableLighting( true );
		gl->setMaterialAmbientColor( getEffectiveAmbientColor() );
		gl->setMaterialDiffuseColor( getEffectiveDiffuseColor() );
		gl->setMaterialSpecularColor( getEffectiveSpecularColor() );
		gl->setMaterialEmissionColor( getEffectiveEmissionColor() );
		gl->setMaterialShininess( getShininess() );
	} else {
		gl->enableLighting( false );
	}
	visitor->setCurrentColor( getEffectiveEmissionColor() );
}

/**
 * Draws the texture property and the texture overlays using separate GL texture units. 
 * The visitor keeps track of which texture unit is being processed, with each texture 
 * incrementing the appropriate texture unit index as it draws.
 *
 * The 2D texture are assigned to the lower texture units, and cube-map textures are assigned
 * to texture units above all the 2D textures. This ensures that the same texture types are
 * consistently assigned to the shader samplers, to avoid the shaders recompiling on the
 * fly to adapt to changing texture types. 
 *
 * After the 2D and cube textures contained in this material are bound, additional 
 * environmental textures, such as light probes, are bound on top.
 *
 * GL texture units of each type that were not used by the textures are disabled by the
 * mesh node after this method is complete.
 */
void CC3Material::drawTexturesWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	if ( m_pTexture )
		m_pTexture->drawWithVisitor( visitor );

	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_textureOverlays, pObj )
	{
		CC3Texture* pTex = (CC3Texture*)pObj;
		if ( pTex )
		{
			pTex->drawWithVisitor( visitor );
		}
	}

	visitor->bindEnvironmentalTextures();
}

void CC3Material::unbindWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	CC3OpenGL* gl = visitor->getGL();
	gl->enableLighting( false );
	gl->enableBlend( false );
	gl->enableAlphaTesting( false );
}

std::string CC3Material::getNameSuffix()
{
	return "Material";
}

NS_COCOS3D_END
