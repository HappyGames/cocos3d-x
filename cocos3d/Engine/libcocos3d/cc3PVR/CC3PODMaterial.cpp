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

CC3PODMaterial::CC3PODMaterial()
{
	_pfxEffect = NULL;
	_podIndex = 0;
}

CC3PODMaterial::~CC3PODMaterial()
{
	_pfxEffect = NULL;
}

GLint CC3PODMaterial::getPodIndex()
{
	return _podIndex; 
}

void CC3PODMaterial::setPodIndex( GLint aPODIndex )
{
	_podIndex = aPODIndex; 
}

CC3PFXEffect* CC3PODMaterial::getPfxEffect()
{
	return _pfxEffect;
}

static GLfloat shininessExpansionFactor = 128.0f;

GLfloat CC3PODMaterial::getShininessExpansionFactor()
{
	return shininessExpansionFactor; 
}

void CC3PODMaterial::setShininessExpansionFactor( GLfloat aFloat )
{
	shininessExpansionFactor = aFloat; 
}

void CC3PODMaterial::initAtIndex( GLint aPODIndex, CC3PODResource* aPODRez )
{
	SPODMaterial* psm = (SPODMaterial*)aPODRez->getMaterialPODStructAtIndex(aPODIndex);
	//LogRez(@"Creating %@ at index %i from: %@", [self class], aPODIndex, NSStringFromSPODMaterial(psm));
	super::initWithName( psm->pszName );
	setPodIndex( aPODIndex );
	setAmbientColor( ccc4f(psm->pfMatAmbient[0], psm->pfMatAmbient[1], psm->pfMatAmbient[2], psm->fMatOpacity) );
	setDiffuseColor( ccc4f(psm->pfMatDiffuse[0], psm->pfMatDiffuse[1], psm->pfMatDiffuse[2], psm->fMatOpacity) );
	setSpecularColor( ccc4f(psm->pfMatSpecular[0], psm->pfMatSpecular[1], psm->pfMatSpecular[2], psm->fMatOpacity) );
	setShininess( psm->fMatShininess * shininessExpansionFactor );
		
	setSourceBlendRGB( GLBlendFuncFromEPODBlendFunc(psm->eBlendSrcRGB) );
	setDestinationBlendRGB( GLBlendFuncFromEPODBlendFunc(psm->eBlendDstRGB) );
	setSourceBlendAlpha( GLBlendFuncFromEPODBlendFunc(psm->eBlendSrcA) );
	setDestinationBlendAlpha( GLBlendFuncFromEPODBlendFunc(psm->eBlendDstA) );

	// Add the bump-map texture first, then add the remaining in order.
	// Textures are only added if they are in the POD file.
	addBumpMapTexture( psm->nIdxTexBump, aPODRez );
	addTexture( psm->nIdxTexDiffuse, aPODRez );
	addTexture( psm->nIdxTexAmbient, aPODRez );
	addTexture( psm->nIdxTexSpecularColour, aPODRez );
	addTexture( psm->nIdxTexSpecularLevel, aPODRez );
	addTexture( psm->nIdxTexEmissive, aPODRez );
	addTexture( psm->nIdxTexGlossiness, aPODRez );
	addTexture( psm->nIdxTexOpacity, aPODRez );
	addTexture( psm->nIdxTexReflection, aPODRez );
	addTexture( psm->nIdxTexRefraction, aPODRez );
		
	if ( psm->pszEffectName && psm->pszEffectFile ) 
	{
		std::string pfxFile = psm->pszEffectFile;
		std::string pfxPath = aPODRez->getDirectory() +  pfxFile;
		std::string pfxName = psm->pszEffectName;
		_pfxEffect = CC3PFXResource::getEffectNamedFromFile( pfxName, pfxPath );
		if ( _pfxEffect )
			_pfxEffect->populateMaterial( this );
	}
	//	
	//// Assign any user data and take ownership of managing its memory
	//if (psm->pUserData && psm->nUserDataSize > 0) {
	//	self.userData = [NSData dataWithBytesNoCopy: psm->pUserData length: psm->nUserDataSize];
	//	psm->pUserData = NULL;		// Clear reference so SPODNode won't try to free it.
	//}
}

CC3PODMaterial* CC3PODMaterial::materialAtIndex( GLint aPODIndex, CC3PODResource* aPODRez )
{
	CC3PODMaterial* pMaterial = new CC3PODMaterial;
	pMaterial->initAtIndex( aPODIndex, aPODRez );
	pMaterial->autorelease();

	return pMaterial;
}

void CC3PODMaterial::populateFrom( CC3PODMaterial* another )
{
	super::populateFrom( another );

	_podIndex = another->getPodIndex();
}

CCObject* CC3PODMaterial::copyWithZone( CCZone* zone )
{
	CC3PODMaterial* pVal = new CC3PODMaterial;
	pVal->init();
	pVal->populateFrom( this );

	return pVal;
}

/**
 * If the specified texture index is valid, extracts the texture from the POD resource
 * and adds it to this material.
 */
void CC3PODMaterial::addTexture( GLint aPODTexIndex, CC3PODResource* aPODRez )
{
	if (aPODTexIndex >= 0 && aPODTexIndex < (GLint)aPODRez->getTextureCount())
		super::addTexture( aPODRez->getTextureAtIndex( aPODTexIndex ) );
}

/**
 * If the specified texture index is valid, extracts the texture from the POD resource,
 * configures it as a bump-map texture, and adds it to this material.
 */
void CC3PODMaterial::addBumpMapTexture( GLint aPODTexIndex, CC3PODResource* aPODRez )
{
	if (aPODTexIndex >= 0 && aPODTexIndex < (GLint)aPODRez->getTextureCount())
	{
		CC3Texture* tex = aPODRez->getTextureAtIndex(aPODTexIndex);
		CC3TextureUnitTexture* bmTex = CC3TextureUnitTexture::textureWithTexture(tex);
		bmTex->setTextureUnit( CC3BumpMapTextureUnit::textureUnit() );
		super::addTexture( bmTex );
	}
}

NS_COCOS3D_END
