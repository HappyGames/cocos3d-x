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
std::string NSStringFromCC3PVRShamanSemantic(CC3PVRShamanSemantic semantic) 
{
	switch (semantic) 
	{
		case kCC3PVRShamanSemanticNone: return "kCC3PVRShamanSemanticNone";
			
		case kCC3PVRShamanSemanticLightSpotFalloff: return "kCC3PVRShamanSemanticLightSpotFalloff";
		case kCC3PVRShamanSemanticViewportSize: return "kCC3PVRShamanSemanticViewportSize";
		case kCC3PVRShamanSemanticViewportClipping: return "kCC3PVRShamanSemanticViewportClipping";
		case kCC3PVRShamanSemanticElapsedTimeLastFrame: return "kCC3PVRShamanSemanticElapsedTimeLastFrame";
			
		case kCC3PVRShamanSemanticAppBase: return "kCC3PVRShamanSemanticAppBase";
		default: 
			return CC3String::stringWithFormat( (char*)"Unknown PVRShaman semantic (%d)", semantic );
	}
}

GLenum CC3PVRShamanShaderSemantics::getSemanticForPFXSemanticName( const std::string& semanticName )
{
	return CC3PVRShamanShaderSemantics::getSemanticForPVRShamanSemanticName(semanticName);
}

/** Handles populating PVRShaman-specific content and delegates remainder to the standard population mechanisms.  */
bool CC3PVRShamanShaderSemantics::populateUniform( CC3GLSLUniform* uniform, CC3NodeDrawingVisitor* visitor )
{
	//LogTrace(@"%@ retrieving semantic value for %@", self, uniform.fullDescription);
	GLenum semantic = uniform->getSemantic();
	GLuint semanticIndex = uniform->getSemanticIndex();
	GLint uniformSize = uniform->getSize();
	CC3Viewport vp;

	if ( super::populateUniform( uniform, visitor ) )
		return true;
	
	switch (semantic)
	{
		// Sets a vec2, specific to PVRShaman, that combines the falloff angle (in degrees) and exponent
		case kCC3PVRShamanSemanticLightSpotFalloff:
			for (GLint i = 0; i < uniformSize; i++) 
			{
				CC3Light* light = visitor->getLightAt( semanticIndex + i );
				uniform->setPoint( ccp(light->getSpotCutoffAngle(), light->getSpotExponent()), i );
			}
			return true;
		case kCC3PVRShamanSemanticElapsedTimeLastFrame:
			// Time of last frame. Just subtract frame time from current time
			uniform->setFloat( visitor->getScene()->getElapsedTimeSinceOpened() - visitor->getDeltaTime() );
			return true;
		case kCC3PVRShamanSemanticViewportSize:
			vp = visitor->getRenderSurface()->getViewport();
			uniform->setPoint( ccp(vp.w, vp.h) );
			return true;
		case kCC3PVRShamanSemanticViewportClipping: 
			{
				// Applies the field of view angle to the narrower aspect.
				vp = visitor->getRenderSurface()->getViewport();
				GLfloat aspect = (GLfloat) vp.w / (GLfloat) vp.h;
				CC3Camera* cam = visitor->getCamera();
				GLfloat fovWidth, fovHeight;
				if (aspect >= 1.0f) {			// Landscape
					fovHeight = CC3DegToRad(cam->getEffectiveFieldOfView());
					fovWidth = fovHeight * aspect;
				} else {						// Portrait
					fovWidth = CC3DegToRad(cam->getEffectiveFieldOfView());
					fovHeight = fovWidth / aspect;
				}
				uniform->setVector4( CC3Vector4(cam->getNearClippingDistance(), cam->getFarClippingDistance(), fovWidth, fovHeight) );
				return true;
			}
		default: 
			return false;
	}
}

static CCDictionary* _semanticsByPVRShamanSemanticName = NULL;

GLenum CC3PVRShamanShaderSemantics::getSemanticForPVRShamanSemanticName( const std::string& semanticName )
{
	ensurePVRShamanSemanticMap();
	CCInteger* semNum = (CCInteger*)_semanticsByPVRShamanSemanticName->objectForKey( semanticName );
	return semNum ? semNum->getValue() : kCC3SemanticNone;
}

void CC3PVRShamanShaderSemantics::addSemantic( GLenum semantic, const std::string& semanticName )
{
	ensurePVRShamanSemanticMap();
	_semanticsByPVRShamanSemanticName->setObject( CCInteger::create(semantic), semanticName );
}

void CC3PVRShamanShaderSemantics::ensurePVRShamanSemanticMap()
{
	if (_semanticsByPVRShamanSemanticName) 
		return;

	_semanticsByPVRShamanSemanticName = CCDictionary::create();		// retained
	_semanticsByPVRShamanSemanticName->retain();
	
	addSemantic( kCC3SemanticVertexLocation, "POSITION" );
	addSemantic( kCC3SemanticVertexNormal, "NORMAL" );
	addSemantic( kCC3SemanticVertexTangent, "TANGENT" );
	addSemantic( kCC3SemanticVertexBitangent, "BINORMAL" );
	addSemantic( kCC3SemanticVertexTexture, "UV" );
	addSemantic( kCC3SemanticVertexColor, "VERTEXCOLOR" );
	addSemantic( kCC3SemanticVertexBoneIndices, "BONEINDEX" );
	addSemantic( kCC3SemanticVertexBoneWeights, "BONEWEIGHT" );

	addSemantic( kCC3SemanticModelMatrix, "WORLD" );
	addSemantic( kCC3SemanticModelMatrixInv, "WORLDI" );
	addSemantic( kCC3SemanticModelMatrixInvTran, "WORLDIT" );
	
	addSemantic( kCC3SemanticViewMatrix, "VIEW" );
	addSemantic( kCC3SemanticViewMatrixInv, "VIEWI" );
	addSemantic( kCC3SemanticViewMatrixInvTran, "VIEWIT" );
	
	addSemantic( kCC3SemanticProjMatrix, "PROJECTION" );
	addSemantic( kCC3SemanticProjMatrixInv, "PROJECTIONI" );
	addSemantic( kCC3SemanticProjMatrixInvTran, "PROJECTIONIT" );
	
	addSemantic( kCC3SemanticModelViewMatrix, "WORLDVIEW" );
	addSemantic( kCC3SemanticModelViewMatrixInv, "WORLDVIEWI" );
	addSemantic( kCC3SemanticModelViewMatrixInvTran, "WORLDVIEWIT" );
	
	addSemantic( kCC3SemanticModelViewProjMatrix, "WORLDVIEWPROJECTION" );
	addSemantic( kCC3SemanticModelViewProjMatrixInv, "WORLDVIEWPROJECTIONI" );
	addSemantic( kCC3SemanticModelViewProjMatrixInvTran, "WORLDVIEWPROJECTIONIT" );
	
	addSemantic( kCC3SemanticViewProjMatrix, "VIEWPROJECTION" );
	addSemantic( kCC3SemanticViewProjMatrixInv, "VIEWPROJECTIONI" );
	addSemantic( kCC3SemanticViewProjMatrixInvTran, "VIEWPROJECTIONIT" );
	
	addSemantic( kCC3SemanticModelLocalMatrix, "OBJECT" );
	addSemantic( kCC3SemanticModelLocalMatrixInv, "OBJECTI" );
	addSemantic( kCC3SemanticModelLocalMatrixInvTran, "OBJECTIT" );
	
	addSemantic( kCC3SemanticNone, "UNPACKMATRIX" );
	
	addSemantic( kCC3SemanticMaterialOpacity, "MATERIALOPACITY" );
	addSemantic( kCC3SemanticMaterialShininess, "MATERIALSHININESS" );
	addSemantic( kCC3SemanticMaterialColorAmbient, "MATERIALCOLORAMBIENT" );
	addSemantic( kCC3SemanticMaterialColorDiffuse, "MATERIALCOLORDIFFUSE" );
	addSemantic( kCC3SemanticMaterialColorSpecular, "MATERIALCOLORSPECULAR" );
	
	addSemantic( kCC3SemanticVertexBoneCount, "BONECOUNT" );
	addSemantic( kCC3SemanticBoneMatricesGlobal, "BONEMATRIXARRAY" );
	addSemantic( kCC3SemanticBoneMatricesInvTranGlobal, "BONEMATRIXARRAYIT" );
	
	addSemantic( kCC3SemanticLightColorDiffuse, "LIGHTCOLOR" );
	addSemantic( kCC3SemanticLightPositionGlobal, "LIGHTPOSWORLD" );
	addSemantic( kCC3SemanticLightPositionEyeSpace, "LIGHTPOSEYE" );
	addSemantic( kCC3SemanticLightPositionModelSpace, "LIGHTPOSMODEL" );
	addSemantic( kCC3SemanticLightInvertedPositionGlobal, "LIGHTDIRWORLD" );
	addSemantic( kCC3SemanticLightInvertedPositionEyeSpace, "LIGHTDIREYE" );
	addSemantic( kCC3SemanticLightInvertedPositionModelSpace, "LIGHTDIRMODEL" );
	addSemantic( kCC3SemanticLightAttenuation, "LIGHTATTENUATION" );
	addSemantic( kCC3PVRShamanSemanticLightSpotFalloff, "LIGHTFALLOFF" );

	addSemantic( kCC3SemanticCameraLocationModelSpace, "EYEPOSMODEL" );
	addSemantic( kCC3SemanticCameraLocationGlobal, "EYEPOSWORLD" );

	addSemantic( kCC3SemanticTextureSampler, "TEXTURE" );
	addSemantic( kCC3SemanticAnimationFraction, "ANIMATION" );
	
	addSemantic( kCC3SemanticDrawCountCurrentFrame, "GEOMENTRYCOUNTER" );
	addSemantic( kCC3PVRShamanSemanticViewportSize, "VIEWPORTPIXELSIZE" );
	addSemantic( kCC3PVRShamanSemanticViewportClipping, "VIEWPORTCLIPPING" );
	
	addSemantic( kCC3SemanticSceneTime, "TIME" );
	addSemantic( kCC3SemanticSceneTimeCosine, "TIMECOS" );
	addSemantic( kCC3SemanticSceneTimeSine, "TIMESIN" );
	addSemantic( kCC3SemanticSceneTimeTangent, "TIMETAN" );

	addSemantic( kCC3SemanticSceneTime, "TIME2PI" );
	addSemantic( kCC3SemanticSceneTimeCosine, "TIME2PICOS" );
	addSemantic( kCC3SemanticSceneTimeSine, "TIME2PISIN" );
	addSemantic( kCC3SemanticSceneTimeTangent, "TIME2PITAN" );

	addSemantic( kCC3PVRShamanSemanticElapsedTimeLastFrame, "LASTTIME" );
	addSemantic( kCC3SemanticFrameTime, "ELAPSEDTIME" );

	addSemantic( kCC3SemanticCenterOfGeometry, "BOUNDINGCENTER" );
	addSemantic( kCC3SemanticBoundingRadius, "BOUNDINGSPHERERADIUS" );
	addSemantic( kCC3SemanticBoundingBoxSize, "BOUNDINGBOXSIZE" );
	addSemantic( kCC3SemanticBoundingBoxMin, "BOUNDINGBOXMIN" );
	addSemantic( kCC3SemanticBoundingBoxMax, "BOUNDINGBOXMAX" );

	addSemantic( kCC3SemanticRandomNumber, "RANDOM" );
}

void CC3PVRShamanShaderSemantics::purgePVRShamanSemanticMap()
{
	CC_SAFE_RELEASE( _semanticsByPVRShamanSemanticName );
}

NS_COCOS3D_END
