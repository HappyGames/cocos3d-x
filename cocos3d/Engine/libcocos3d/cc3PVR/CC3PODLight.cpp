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
#include "../Utility/CC3Foundation.h"	// extern must be first, since foundation also imported via other imports
#include "CC3PODLight.h"
#include "CC3PVRTModelPOD.h"
#include "../Utility/CC3Math.h"
#include "CC3PODResource.h"

NS_COCOS3D_BEGIN

GLint CC3PODLight::getPodIndex()
{
	return _podIndex; 
}

void CC3PODLight::setPodIndex( GLint aPODIndex )
{
	_podIndex = aPODIndex; 
}

GLint CC3PODLight::getPodContentIndex()
{
	return _podContentIndex; 
}

void CC3PODLight::setPodContentIndex( GLint aPODIndex )
{
	_podContentIndex = aPODIndex; 
}

GLint CC3PODLight::getPodParentIndex()
{
	return _podParentIndex; 
}

void CC3PODLight::setPodParentIndex( GLint aPODIndex )
{
	_podParentIndex = aPODIndex; 
}

GLint CC3PODLight::getPodTargetIndex()
{
	return _podTargetIndex; 
}

void CC3PODLight::setPodTargetIndex( GLint aPODIndex )
{
	_podTargetIndex = aPODIndex; 
}

void CC3PODLight::initAtIndex( GLint aPODIndex, CC3PODResource* aPODRez )
{
	super::initAtIndex( aPODIndex, aPODRez );
	// Get the light content
	if (getPodContentIndex() >= 0) 
	{
		SPODLight* psl = (SPODLight*)aPODRez->getLightPODStructAtIndex(getPodContentIndex());
		//LogRez(@"Setting %@ parameters from %@", [self class], NSStringFromSPODLight(psl));
		setPodTargetIndex( psl->nIdxTarget );

		setAmbientColor( kCC3DefaultLightColorAmbient );
		setDiffuseColor( ccc4f(psl->pfColour[0], psl->pfColour[1], psl->pfColour[2], 1.0) );
		setSpecularColor( kCC3DefaultLightColorSpecular );

		setAttenuation( CC3AttenuationCoefficientsMake(psl->fConstantAttenuation,
															psl->fLinearAttenuation,
															psl->fQuadraticAttenuation) );
		switch (psl->eType) 
		{
			case ePODDirectional:
				setIsDirectionalOnly( true );
				break;
			case ePODPoint:
				setIsDirectionalOnly( false );
				break;
			case ePODSpot:
				setIsDirectionalOnly( false );
				setSpotCutoffAngle( CC3RadToDeg(psl->fFalloffAngle) );
				setSpotExponent( psl->fFalloffExponent );
				break;
			default:
				break;
		}
	}
}

PODStructPtr CC3PODLight::getNodePODStructAtIndex( GLuint aPODIndex, CC3PODResource* aPODRez )
{
	return aPODRez->getLightNodePODStructAtIndex(aPODIndex);
}

void CC3PODLight::populateFrom( CC3PODLight* another )
{
	super::populateFrom(another);

	_podIndex = another->getPodIndex();
	_podContentIndex = another->getPodContentIndex();
	_podParentIndex = another->getPodParentIndex();
	_podTargetIndex = another->getPodTargetIndex();
}

CCObject* CC3PODLight::copyWithZone( CCZone* zone )
{
	CC3PODLight* pVal = new CC3PODLight;
	pVal->init();
	pVal->populateFrom( this );
	
	return pVal;
}

// The direction of a light in a POD file is taken from the transform of the up direction!
CC3Vector4 CC3PODLight::getGlobalHomogeneousPosition()
{
	if (isDirectionalOnly()) 
		return CC3Vector4FromCC3Vector(getGlobalUpDirection(), 0.0f);

	return super::getGlobalHomogeneousPosition();
}

std::string CC3PODLight::description()
{
	return stringWithFormat( (char*)"%s (POD index: %d)", super::description().c_str(), _podIndex );
}

CC3PODLight* CC3PODLight::nodeAtIndex( GLint aPODIndex, CC3PODResource* aPODRez )
{
	CC3PODLight* pLight = new CC3PODLight;
	pLight->initAtIndex( aPODIndex, aPODRez );
	pLight->autorelease();

	return pLight;
}

NS_COCOS3D_END
