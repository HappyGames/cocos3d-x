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

std::string stringFromCC3Semantic(CC3Semantic semantic) 
{
	switch (semantic) 
	{
		case kCC3SemanticNone: return "kCC3SemanticNone";

		// VERTEX CONTENT --------------
		case kCC3SemanticVertexLocation: return "kCC3SemanticVertexLocation";
		case kCC3SemanticVertexNormal: return "kCC3SemanticVertexNormal";
		case kCC3SemanticVertexTangent: return "kCC3SemanticVertexTangent";
		case kCC3SemanticVertexBitangent: return "kCC3SemanticVertexBitangent";
		case kCC3SemanticVertexColor: return "kCC3SemanticVertexColor";
		case kCC3SemanticVertexPointSize: return "kCC3SemanticVertexPointSize";
		case kCC3SemanticVertexBoneWeights: return "kCC3SemanticVertexBoneWeights";
		case kCC3SemanticVertexBoneIndices: return "kCC3SemanticVertexBoneIndices";
		case kCC3SemanticVertexTexture: return "kCC3SemanticVertexTexture";
			
		case kCC3SemanticHasVertexNormal: return "kCC3SemanticHasVertexNormal";
		case kCC3SemanticShouldNormalizeVertexNormal: return "kCC3SemanticShouldNormalizeVertexNormal";
		case kCC3SemanticShouldRescaleVertexNormal: return "kCC3SemanticShouldRescaleVertexNormal";
		case kCC3SemanticHasVertexTangent: return "kCC3SemanticHasVertexTangent";
		case kCC3SemanticHasVertexBitangent: return "kCC3SemanticHasVertexBitangent";
		case kCC3SemanticHasVertexColor: return "kCC3SemanticHasVertexColor";
		case kCC3SemanticHasVertexWeight: return "kCC3SemanticHasVertexWeight";
		case kCC3SemanticHasVertexMatrixIndex: return "kCC3SemanticHasVertexMatrixIndex";
		case kCC3SemanticHasVertexTextureCoordinate: return "kCC3SemanticHasVertexTextureCoordinate";
		case kCC3SemanticHasVertexPointSize: return "kCC3SemanticHasVertexPointSize";
		case kCC3SemanticIsDrawingPoints: return "kCC3SemanticIsDrawingPoints";
		case kCC3SemanticShouldDrawFrontFaces: return "kCC3SemanticShouldDrawFrontFaces";
		case kCC3SemanticShouldDrawBackFaces: return "kCC3SemanticShouldDrawBackFaces";

			// ENVIRONMENT MATRICES --------------
		case kCC3SemanticModelLocalMatrix: return "kCC3SemanticModelLocalMatrix";
		case kCC3SemanticModelLocalMatrixInv: return "kCC3SemanticModelLocalMatrixInv";
		case kCC3SemanticModelLocalMatrixInvTran: return "kCC3SemanticModelLocalMatrixInvTran";
		case kCC3SemanticModelMatrix: return "kCC3SemanticModelMatrix";
		case kCC3SemanticModelMatrixInv: return "kCC3SemanticModelMatrixInv";
		case kCC3SemanticModelMatrixInvTran: return "kCC3SemanticModelMatrixInvTran";
		case kCC3SemanticViewMatrix: return "kCC3SemanticViewMatrix";
		case kCC3SemanticViewMatrixInv: return "kCC3SemanticViewMatrixInv";
		case kCC3SemanticViewMatrixInvTran: return "kCC3SemanticViewMatrixInvTran";
		case kCC3SemanticModelViewMatrix: return "kCC3SemanticModelViewMatrix";
		case kCC3SemanticModelViewMatrixInv: return "kCC3SemanticModelViewMatrixInv";
		case kCC3SemanticModelViewMatrixInvTran: return "kCC3SemanticModelViewMatrixInvTran";
		case kCC3SemanticProjMatrix: return "kCC3SemanticProjMatrix";
		case kCC3SemanticProjMatrixInv: return "kCC3SemanticProjMatrixInv";
		case kCC3SemanticProjMatrixInvTran: return "kCC3SemanticProjMatrixInvTran";
		case kCC3SemanticViewProjMatrix: return "kCC3SemanticViewProjMatrix";
		case kCC3SemanticViewProjMatrixInv: return "kCC3SemanticViewProjMatrixInv";
		case kCC3SemanticViewProjMatrixInvTran: return "kCC3SemanticViewProjMatrixInvTran";
		case kCC3SemanticModelViewProjMatrix: return "kCC3SemanticModelViewProjMatrix";
		case kCC3SemanticModelViewProjMatrixInv: return "kCC3SemanticModelViewProjMatrixInv";
		case kCC3SemanticModelViewProjMatrixInvTran: return "kCC3SemanticModelViewProjMatrixInvTran";
			
		// CAMERA -----------------
		case kCC3SemanticCameraLocationGlobal: return "kCC3SemanticCameraLocationGlobal";
		case kCC3SemanticCameraLocationModelSpace: return "kCC3SemanticCameraLocationModelSpace";
		case kCC3SemanticCameraFrustum: return "kCC3SemanticCameraFrustum";
		case kCC3SemanticCameraFrustumDepth: return "kCC3SemanticCameraFrustumDepth";
			
		// MATERIALS --------------
		case kCC3SemanticColor: return "kCC3SemanticColor";
		case kCC3SemanticMaterialColorAmbient: return "kCC3SemanticMaterialColorAmbient";
		case kCC3SemanticMaterialColorDiffuse: return "kCC3SemanticMaterialColorDiffuse";
		case kCC3SemanticMaterialColorSpecular: return "kCC3SemanticMaterialColorSpecular";
		case kCC3SemanticMaterialColorEmission: return "kCC3SemanticMaterialColorEmission";
		case kCC3SemanticMaterialOpacity: return "kCC3SemanticMaterialOpacity";
		case kCC3SemanticMaterialShininess: return "kCC3SemanticMaterialShininess";
		case kCC3SemanticMaterialReflectivity: return "kCC3SemanticMaterialReflectivity";

		// LIGHTING --------------
		case kCC3SemanticIsUsingLighting: return "kCC3SemanticIsUsingLighting";
		case kCC3SemanticSceneLightColorAmbient: return "kCC3SemanticSceneLightColorAmbient";

		case kCC3SemanticLightIsEnabled: return "kCC3SemanticLightIsEnabled";
		case kCC3SemanticLightPositionGlobal: return "kCC3SemanticLightPositionGlobal";
		case kCC3SemanticLightPositionEyeSpace: return "kCC3SemanticLightPositionEyeSpace";
		case kCC3SemanticLightPositionModelSpace: return "kCC3SemanticLightPositionModelSpace";
		case kCC3SemanticLightInvertedPositionGlobal: return "kCC3SemanticLightInvertedPositionGlobal";
		case kCC3SemanticLightInvertedPositionEyeSpace: return "kCC3SemanticLightInvertedPositionEyeSpace";
		case kCC3SemanticLightInvertedPositionModelSpace: return "kCC3SemanticLightInvertedPositionModelSpace";
		case kCC3SemanticLightColorAmbient: return "kCC3SemanticLightColorAmbient";
		case kCC3SemanticLightColorDiffuse: return "kCC3SemanticLightColorDiffuse";
		case kCC3SemanticLightColorSpecular: return "kCC3SemanticLightColorSpecular";
		case kCC3SemanticLightAttenuation: return "kCC3SemanticLightAttenuation";
		case kCC3SemanticLightSpotDirectionGlobal: return "kCC3SemanticLightSpotDirectionGlobal";
		case kCC3SemanticLightSpotDirectionEyeSpace: return "kCC3SemanticLightSpotDirectionEyeSpace";
		case kCC3SemanticLightSpotDirectionModelSpace: return "kCC3SemanticLightSpotDirectionModelSpace";
		case kCC3SemanticLightSpotExponent: return "kCC3SemanticLightSpotExponent";
		case kCC3SemanticLightSpotCutoffAngle: return "kCC3SemanticLightSpotCutoffAngle";
		case kCC3SemanticLightSpotCutoffAngleCosine: return "kCC3SemanticLightSpotCutoffAngleCosine";

		case kCC3SemanticIsUsingLightProbes: return "kCC3SemanticIsUsingLightProbes";
		case kCC3SemanticLightProbeCount: return "kCC3SemanticLightProbeCount";
		case kCC3SemanticLightProbeIsEnabled: return "kCC3SemanticLightProbeIsEnabled";
		case kCC3SemanticLightProbeLocationGlobal: return "kCC3SemanticLightProbeLocationGlobal";
		case kCC3SemanticLightProbeLocationEyeSpace: return "kCC3SemanticLightProbeLocationEyeSpace";
		case kCC3SemanticLightProbeLocationModelSpace: return "kCC3SemanticLightProbeLocationModelSpace";
		case kCC3SemanticLightProbeColorDiffuse: return "kCC3SemanticLightProbeColorDiffuse";
			
		case kCC3SemanticFogIsEnabled: return "kCC3SemanticFogIsEnabled";
		case kCC3SemanticFogColor: return "kCC3SemanticFogColor";
		case kCC3SemanticFogAttenuationMode: return "kCC3SemanticFogAttenuationMode";
		case kCC3SemanticFogDensity: return "kCC3SemanticFogDensity";
		case kCC3SemanticFogStartDistance: return "kCC3SemanticFogStartDistance";
		case kCC3SemanticFogEndDistance: return "kCC3SemanticFogEndDistance";
			
		// TEXTURES --------------
		case kCC3SemanticTextureCount: return "kCC3SemanticTextureCount";
		case kCC3SemanticTextureSampler: return "kCC3SemanticTextureSampler";
		case kCC3SemanticTexture2DCount: return "kCC3SemanticTexture2DCount";
		case kCC3SemanticTexture2DSampler: return "kCC3SemanticTexture2DSampler";
		case kCC3SemanticTextureCubeCount: return "kCC3SemanticTextureCubeCount";
		case kCC3SemanticTextureCubeSampler: return "kCC3SemanticTextureCubeSampler";
		case kCC3SemanticTextureLightProbeSampler: return "kCC3SemanticTextureLightProbeSampler";
			
		case kCC3SemanticTexUnitMode: return "kCC3SemanticTexUnitMode";
		case kCC3SemanticTexUnitConstantColor: return "kCC3SemanticTexUnitConstantColor";
		case kCC3SemanticTexUnitCombineRGBFunction: return "kCC3SemanticTexUnitCombineRGBFunction";
		case kCC3SemanticTexUnitSource0RGB: return "kCC3SemanticTexUnitSource0RGB";
		case kCC3SemanticTexUnitSource1RGB: return "kCC3SemanticTexUnitSource1RGB";
		case kCC3SemanticTexUnitSource2RGB: return "kCC3SemanticTexUnitSource2RGB";
		case kCC3SemanticTexUnitOperand0RGB: return "kCC3SemanticTexUnitOperand0RGB";
		case kCC3SemanticTexUnitOperand1RGB: return "kCC3SemanticTexUnitOperand1RGB";
		case kCC3SemanticTexUnitOperand2RGB: return "kCC3SemanticTexUnitOperand2RGB";
		case kCC3SemanticTexUnitCombineAlphaFunction: return "kCC3SemanticTexUnitCombineAlphaFunction";
		case kCC3SemanticTexUnitSource0Alpha: return "kCC3SemanticTexUnitSource0Alpha";
		case kCC3SemanticTexUnitSource1Alpha: return "kCC3SemanticTexUnitSource1Alpha";
		case kCC3SemanticTexUnitSource2Alpha: return "kCC3SemanticTexUnitSource2Alpha";
		case kCC3SemanticTexUnitOperand0Alpha: return "kCC3SemanticTexUnitOperand0Alpha";
		case kCC3SemanticTexUnitOperand1Alpha: return "kCC3SemanticTexUnitOperand1Alpha";
		case kCC3SemanticTexUnitOperand2Alpha: return "kCC3SemanticTexUnitOperand2Alpha";
			
		// MODEL ----------------
		case kCC3SemanticAnimationFraction: return "kCC3SemanticAnimationFraction";
		case kCC3SemanticCenterOfGeometry: return "kCC3SemanticCenterOfGeometry";
		case kCC3SemanticBoundingBoxMin: return "kCC3SemanticBoundingBoxMin";
		case kCC3SemanticBoundingBoxMax: return "kCC3SemanticBoundingBoxMax";
		case kCC3SemanticBoundingBoxSize: return "kCC3SemanticBoundingBoxSize";
		case kCC3SemanticBoundingRadius: return "kCC3SemanticBoundingRadius";
			
		// BONE SKINNING
		case kCC3SemanticVertexBoneCount: return "kCC3SemanticVertexBoneCount";
		case kCC3SemanticBatchBoneCount: return "kCC3SemanticBatchBoneCount";

		// BONE SKINNING MATRICES
		case kCC3SemanticBoneMatricesGlobal: return "kCC3SemanticBoneMatricesGlobal";
		case kCC3SemanticBoneMatricesInvTranGlobal: return "kCC3SemanticBoneMatricesInvTranGlobal";
		case kCC3SemanticBoneMatricesEyeSpace: return "kCC3SemanticBoneMatricesEyeSpace";
		case kCC3SemanticBoneMatricesInvTranEyeSpace: return "kCC3SemanticBoneMatricesInvTranEyeSpace";
		case kCC3SemanticBoneMatricesModelSpace: return "kCC3SemanticBoneMatricesModelSpace";
		case kCC3SemanticBoneMatricesInvTranModelSpace: return "kCC3SemanticBoneMatricesInvTranModelSpace";

		// BONE SKINNING DISCRETE TRANSFORMS
		case kCC3SemanticBoneQuaternionsGlobal: return "kCC3SemanticBoneQuaternionsGlobal";
		case kCC3SemanticBoneTranslationsGlobal: return "kCC3SemanticBoneTranslationsGlobal";
		case kCC3SemanticBoneScalesGlobal: return "kCC3SemanticBoneScalesGlobal";
		case kCC3SemanticBoneQuaternionsEyeSpace: return "kCC3SemanticBoneQuaternionsEyeSpace";
		case kCC3SemanticBoneTranslationsEyeSpace: return "kCC3SemanticBoneTranslationsEyeSpace";
		case kCC3SemanticBoneScalesEyeSpace: return "kCC3SemanticBoneScalesEyeSpace";
		case kCC3SemanticBoneQuaternionsModelSpace: return "kCC3SemanticBoneQuaternionsModelSpace";
		case kCC3SemanticBoneTranslationsModelSpace: return "kCC3SemanticBoneTranslationsModelSpace";
		case kCC3SemanticBoneScalesModelSpace: return "kCC3SemanticBoneScalesModelSpace";
			
		// PARTICLES ------------
		case kCC3SemanticPointSize: return "kCC3SemanticPointSize";
		case kCC3SemanticPointSizeAttenuation: return "kCC3SemanticPointSizeAttenuation";
		case kCC3SemanticPointSizeMinimum: return "kCC3SemanticPointSizeMinimum";
		case kCC3SemanticPointSizeMaximum: return "kCC3SemanticPointSizeMaximum";
		case kCC3SemanticPointSpritesIsEnabled: return "kCC3SemanticPointSpritesIsEnabled";
			
		// TIME ------------------
		case kCC3SemanticFrameTime: return "kCC3SemanticFrameTime";
			
		case kCC3SemanticSceneTime: return "kCC3SemanticSceneTime";
		case kCC3SemanticSceneTimeSine: return "kCC3SemanticSceneTimeSine";
		case kCC3SemanticSceneTimeCosine: return "kCC3SemanticSceneTimeCosine";
		case kCC3SemanticSceneTimeTangent: return "kCC3SemanticSceneTimeTangent";

		// MISC ENVIRONMENT ---------
		case kCC3SemanticViewport: return "kCC3SemanticViewport";
		case kCC3SemanticDrawCountCurrentFrame: return "kCC3SemanticDrawCountCurrentFrame";
		case kCC3SemanticRandomNumber: return "kCC3SemanticRandomNumber";
			
		case kCC3SemanticAppBase: return "kCC3SemanticAppBase";
		case kCC3SemanticMax: return "kCC3SemanticMax";

		default: 
			return CC3String::stringWithFormat( (char*)"Unknown state semantic (%d)", semantic );
	}
}

CC3GLSLVariableConfiguration::CC3GLSLVariableConfiguration()
{
	init();
}

CC3GLSLVariableConfiguration::~CC3GLSLVariableConfiguration()
{

}

void CC3GLSLVariableConfiguration::init()
{
	m_name = "";
	m_semantic = kCC3SemanticNone;
	m_semanticIndex = 0;
}

std::string CC3GLSLVariableConfiguration::getName()
{
	return m_name;
}

void CC3GLSLVariableConfiguration::setName( const std::string& name )
{
	m_name = name;
}

GLenum CC3GLSLVariableConfiguration::getSemantic()
{
	return m_semantic;
}
	
void CC3GLSLVariableConfiguration::setSemantic( GLenum semantic )
{
	m_semantic = semantic;
}

GLuint CC3GLSLVariableConfiguration::getSemanticIndex()
{
	return m_semanticIndex;
}

void CC3GLSLVariableConfiguration::setSemanticIndex( GLuint index )
{
	m_semanticIndex = index;
}

CC3ShaderSemanticsBase* CC3ShaderSemanticsBase::semanticsDelegate()
{
	CC3ShaderSemanticsBase* pSemanticBase = new CC3ShaderSemanticsBase;
	pSemanticBase->autorelease();
	

	return pSemanticBase;
}

std::string CC3ShaderSemanticsBase::getNameOfSemantic( GLenum semantic )
{
	return stringFromCC3Semantic( (CC3Semantic)semantic ); 
}

bool CC3ShaderSemanticsBase::configureVariable( CC3GLSLVariable* variable )
{
	return false; 
}

/**
 * Returns a variable scope derived from the specified semantic.
 *
 * Subclasses that permit application-specific semantics should override this method to
 * handle those additional semantics if they should not default to kCC3GLSLVariableScopeNode.
 */
CC3GLSLVariableScope CC3ShaderSemanticsBase::getVariableScopeForSemantic( GLenum semantic )
{
	switch (semantic) 
	{
		// Draw scope semantics
		case kCC3SemanticDrawCountCurrentFrame:
		case kCC3SemanticRandomNumber:

		case kCC3SemanticBatchBoneCount:

		case kCC3SemanticBoneMatricesGlobal:
		case kCC3SemanticBoneMatricesInvTranGlobal:
		case kCC3SemanticBoneMatricesEyeSpace:
		case kCC3SemanticBoneMatricesInvTranEyeSpace:
		case kCC3SemanticBoneMatricesModelSpace:
		case kCC3SemanticBoneMatricesInvTranModelSpace:

		case kCC3SemanticBoneQuaternionsGlobal:
		case kCC3SemanticBoneTranslationsGlobal:
		case kCC3SemanticBoneScalesGlobal:
		case kCC3SemanticBoneQuaternionsEyeSpace:
		case kCC3SemanticBoneTranslationsEyeSpace:
		case kCC3SemanticBoneScalesEyeSpace:
		case kCC3SemanticBoneQuaternionsModelSpace:
		case kCC3SemanticBoneTranslationsModelSpace:
		case kCC3SemanticBoneScalesModelSpace:
			
			return kCC3GLSLVariableScopeDraw;

			
		// Scene scope semantics
			
		case kCC3SemanticViewMatrix:
		case kCC3SemanticViewMatrixInv:
		case kCC3SemanticViewMatrixInvTran:
		case kCC3SemanticProjMatrix:
		case kCC3SemanticProjMatrixInv:
		case kCC3SemanticProjMatrixInvTran:
		case kCC3SemanticViewProjMatrix:
		case kCC3SemanticViewProjMatrixInv:
		case kCC3SemanticViewProjMatrixInvTran:
			
		case kCC3SemanticCameraLocationGlobal:
		case kCC3SemanticCameraFrustum:
		case kCC3SemanticCameraFrustumDepth:
		case kCC3SemanticViewport:

		case kCC3SemanticSceneLightColorAmbient:

		case kCC3SemanticLightIsEnabled:
		case kCC3SemanticLightPositionGlobal:
		case kCC3SemanticLightPositionEyeSpace:
		case kCC3SemanticLightInvertedPositionGlobal:
		case kCC3SemanticLightInvertedPositionEyeSpace:
		case kCC3SemanticLightColorAmbient:
		case kCC3SemanticLightColorDiffuse:
		case kCC3SemanticLightColorSpecular:
		case kCC3SemanticLightAttenuation:
		case kCC3SemanticLightSpotDirectionGlobal:
		case kCC3SemanticLightSpotDirectionEyeSpace:
		case kCC3SemanticLightSpotExponent:
		case kCC3SemanticLightSpotCutoffAngle:
		case kCC3SemanticLightSpotCutoffAngleCosine:

		case kCC3SemanticFogIsEnabled:
		case kCC3SemanticFogColor:
		case kCC3SemanticFogAttenuationMode:
		case kCC3SemanticFogDensity:
		case kCC3SemanticFogStartDistance:
		case kCC3SemanticFogEndDistance:
			
		case kCC3SemanticFrameTime:
		case kCC3SemanticSceneTime:
		case kCC3SemanticSceneTimeSine:
		case kCC3SemanticSceneTimeCosine:
		case kCC3SemanticSceneTimeTangent:
			
			return kCC3GLSLVariableScopeScene;
			
			
		// Node scope semantics
		default:
			return kCC3GLSLVariableScopeNode;
	}
}

/**
 * For semantics that may have more than one target, such as components of lights, or textures,
 * the iteration loops in this method are designed to deal with two situations:
 *   - If the uniform is declared as an array of single types (eg- an array of floats, bools, or
 *     vec3's), the uniform semantic index will be zero and the uniform size will be larger than one.
 *   - If the uniform is declared as a scalar (eg- distinct uniforms for each light, etc), the
 *     uniform size will be one, but the uniform semantic index can be larger than zero.
 */
bool CC3ShaderSemanticsBase::populateUniform( CC3GLSLUniform* uniform, CC3NodeDrawingVisitor* visitor )
{
	// LogTrace(@"%@ drawing %@ retrieving semantic value for %@", self, visitor->currentNode, uniform.fullDescription);
	GLenum semantic = uniform->getSemantic();
	GLuint semanticIndex = uniform->getSemanticIndex();
	GLint uniformSize = uniform->getSize();
	
	CC3Material* mat;
	CC3PointParticleEmitter* emitter;
	CC3Matrix4x4 m4x4;
	CC3Matrix4x3 m4x3,  mRslt4x3, tfmMtx;
	const CC3Matrix4x3 *pm4x3;
	CC3Matrix3x3 m3x3;
	CC3Viewport vp;
	float sceneTime;
	GLuint boneCnt = 0, tuCnt = 0, texCnt = 0;
	bool isInverted = false, isPtEmitter = false;
	
	CC3Mesh* currentMesh = visitor->getCurrentMesh();
	float maximumPointSize = 0.f;

	switch (semantic) 
	{	
		// ATTRIBUTE QUALIFIERS --------------
		case kCC3SemanticHasVertexNormal:
			if ( currentMesh )
				uniform->setBoolean( currentMesh->hasVertexNormals() );
			return true;
		case kCC3SemanticShouldNormalizeVertexNormal:
			uniform->setBoolean( (visitor->getCurrentMeshNode()->getEffectiveNormalScalingMethod() == kCC3NormalScalingNormalize) );
			return true;
		case kCC3SemanticShouldRescaleVertexNormal:
			uniform->setBoolean( (visitor->getCurrentMeshNode()->getEffectiveNormalScalingMethod() == kCC3NormalScalingRescale) );
			return true;
		case kCC3SemanticHasVertexTangent:
			if ( currentMesh )
				uniform->setBoolean( currentMesh->hasVertexTangents() );
			return true;
		case kCC3SemanticHasVertexBitangent:
			if ( currentMesh )
				uniform->setBoolean( currentMesh->hasVertexBitangents() );
			return true;
		case kCC3SemanticHasVertexColor:
			if ( currentMesh )
				uniform->setBoolean( currentMesh->hasVertexColors() );
			return true;
		case kCC3SemanticHasVertexWeight:
			if ( currentMesh )
				uniform->setBoolean( currentMesh->hasVertexBoneWeights() );
			return true;
		case kCC3SemanticHasVertexMatrixIndex:
			if ( currentMesh )
				uniform->setBoolean( currentMesh->hasVertexBoneIndices() );
			return true;
		case kCC3SemanticHasVertexTextureCoordinate:
			if ( currentMesh )
				uniform->setBoolean( currentMesh->hasVertexTextureCoordinates() );
			return true;
		case kCC3SemanticHasVertexPointSize:
			if ( currentMesh )
				uniform->setBoolean( currentMesh->hasVertexPointSizes() );
			return true;
		case kCC3SemanticIsDrawingPoints:
			uniform->setBoolean( visitor->getCurrentMeshNode()->getDrawingMode() == GL_POINTS );
			return true;
		case kCC3SemanticShouldDrawFrontFaces:
			uniform->setBoolean( !visitor->getCurrentMeshNode()->shouldCullFrontFaces() );
			return true;
		case kCC3SemanticShouldDrawBackFaces:
			uniform->setBoolean( !visitor->getCurrentMeshNode()->shouldCullBackFaces() );
			return true;

		// ENVIRONMENT MATRICES --------------
		case kCC3SemanticModelLocalMatrix:
			// Get local matrix as P(-1).T where T is node transform P(-1) is inv-xfm of parent
			visitor->getCurrentMeshNode()->getParent()->getGlobalTransformMatrixInverted()->populateCC3Matrix4x3( &m4x3 );
			visitor->getCurrentMeshNode()->getGlobalTransformMatrix()->populateCC3Matrix4x3( &tfmMtx );
			CC3Matrix4x3Multiply(&mRslt4x3, &m4x3, &tfmMtx);
			uniform->setMatrix4x3( &mRslt4x3 );
			return true;
		case kCC3SemanticModelLocalMatrixInv:
			// Get local matrix as P(-1).T where T is node transform P(-1) is inv-xfm of parent
			visitor->getCurrentMeshNode()->getParent()->getGlobalTransformMatrixInverted()->populateCC3Matrix4x3( &m4x3 );
			visitor->getCurrentMeshNode()->getGlobalTransformMatrix()->populateCC3Matrix4x3( &tfmMtx );
			CC3Matrix4x3Multiply(&mRslt4x3, &m4x3, &tfmMtx);
			// Now invert
			CC3Matrix4x3InvertAdjoint(&mRslt4x3);
			uniform->setMatrix4x3( &mRslt4x3 );
			return true;
		case kCC3SemanticModelLocalMatrixInvTran:
			// Get local matrix as P(-1).T where T is node transform P(-1) is inv-xfm of parent
			visitor->getCurrentMeshNode()->getParent()->getGlobalTransformMatrixInverted()->populateCC3Matrix4x3( &m4x3 );
			visitor->getCurrentMeshNode()->getGlobalTransformMatrix()->populateCC3Matrix4x3( &tfmMtx );
			CC3Matrix4x3Multiply(&mRslt4x3, &m4x3, &tfmMtx);
			CC3Matrix3x3PopulateFrom4x3(&m3x3, &mRslt4x3);
			CC3Matrix3x3InvertAdjointTranspose(&m3x3);
			uniform->setMatrix3x3( &m3x3 );
			return true;
			
		case kCC3SemanticModelMatrix:
			uniform->setMatrix4x3( visitor->getModelMatrix() );
			return true;
		case kCC3SemanticModelMatrixInv:
			CC3Matrix4x3PopulateFrom4x3(&m4x3, visitor->getModelMatrix());
			CC3Matrix4x3InvertAdjoint(&m4x3);
			uniform->setMatrix4x3( &m4x3 );
			return true;
		case kCC3SemanticModelMatrixInvTran:
			CC3Matrix3x3PopulateFrom4x3(&m3x3, visitor->getModelMatrix());
			CC3Matrix3x3InvertAdjointTranspose(&m3x3);
			uniform->setMatrix3x3( &m3x3 );
			return true;
			
		case kCC3SemanticViewMatrix:
			uniform->setMatrix4x3( visitor->getViewMatrix() );
			return true;
		case kCC3SemanticViewMatrixInv:
			CC3Matrix4x3PopulateFrom4x3(&m4x3, visitor->getViewMatrix());
			CC3Matrix4x3InvertAdjoint(&m4x3);
			uniform->setMatrix4x3( &m4x3 );
			return true;
		case kCC3SemanticViewMatrixInvTran:
			CC3Matrix3x3PopulateFrom4x3(&m3x3, visitor->getViewMatrix());
			CC3Matrix3x3InvertAdjointTranspose(&m3x3);
			uniform->setMatrix3x3( &m3x3 );
			return true;
			
		case kCC3SemanticModelViewMatrix:
			uniform->setMatrix4x3( visitor->getModelViewMatrix() );
			return true;
		case kCC3SemanticModelViewMatrixInv:
			CC3Matrix4x3PopulateFrom4x3(&m4x3, visitor->getModelViewMatrix());
			CC3Matrix4x3InvertAdjoint(&m4x3);
			uniform->setMatrix4x3( &m4x3 );
			return true;
		case kCC3SemanticModelViewMatrixInvTran:
			CC3Matrix3x3PopulateFrom4x3(&m3x3, visitor->getModelViewMatrix());
			CC3Matrix3x3InvertAdjointTranspose(&m3x3);
			uniform->setMatrix3x3( &m3x3 );
			return true;
			
		case kCC3SemanticProjMatrix:
			uniform->setMatrix4x4( visitor->getProjMatrix() );
			return true;
		case kCC3SemanticProjMatrixInv:
			CC3Matrix4x4PopulateFrom4x4(&m4x4, visitor->getProjMatrix());
			CC3Matrix4x4InvertAdjoint(&m4x4);
			uniform->setMatrix4x4( &m4x4 );
			return true;
		case kCC3SemanticProjMatrixInvTran:
			CC3Matrix3x3PopulateFrom4x4(&m3x3, visitor->getProjMatrix());
			CC3Matrix3x3InvertAdjointTranspose(&m3x3);
			uniform->setMatrix3x3( &m3x3 );
			return true;
			
		case kCC3SemanticViewProjMatrix:
			uniform->setMatrix4x4( visitor->getViewProjMatrix() );
			return true;
		case kCC3SemanticViewProjMatrixInv:
			CC3Matrix4x4PopulateFrom4x4(&m4x4, visitor->getViewProjMatrix());
			CC3Matrix4x4InvertAdjoint(&m4x4);
			uniform->setMatrix4x4( &m4x4 );
			return true;
		case kCC3SemanticViewProjMatrixInvTran:
			CC3Matrix3x3PopulateFrom4x4(&m3x3, visitor->getViewProjMatrix());
			CC3Matrix3x3InvertAdjointTranspose(&m3x3);
			uniform->setMatrix3x3( &m3x3 );
			return true;
			
		case kCC3SemanticModelViewProjMatrix:
			uniform->setMatrix4x4( visitor->getModelViewProjMatrix() );
			return true;
		case kCC3SemanticModelViewProjMatrixInv:
			CC3Matrix4x4PopulateFrom4x4(&m4x4, visitor->getModelViewProjMatrix());
			CC3Matrix4x4InvertAdjoint(&m4x4);
			uniform->setMatrix4x4( &m4x4 );
			return true;
		case kCC3SemanticModelViewProjMatrixInvTran:
			CC3Matrix3x3PopulateFrom4x4(&m3x3, visitor->getModelViewProjMatrix());
			CC3Matrix3x3InvertAdjointTranspose(&m3x3);
			uniform->setMatrix3x3( &m3x3 );
			return true;
	
		// BONE SKINNING ----------------
		case kCC3SemanticVertexBoneCount:
			uniform->setInteger( visitor->getCurrentMeshNode()->getVertexBoneCount() );
			return true;
		case kCC3SemanticBatchBoneCount:
			uniform->setInteger( visitor->getCurrentSkinSection()->getBoneCount() );
			return true;

		// BONE SKINNING MATRICES ----------------
		case kCC3SemanticBoneMatricesGlobal:
			CC3AssertBoneUniformForSkinSection(uniform, visitor->getCurrentSkinSection());
			boneCnt = visitor->getCurrentSkinSection()->getBoneCount();
			for (GLuint boneIdx = 0; boneIdx < boneCnt; boneIdx++)
				uniform->setMatrix4x3( visitor->getGlobalBoneMatrixAt(boneIdx), boneIdx );
			return true;
		case kCC3SemanticBoneMatricesInvTranGlobal:
			CC3AssertBoneUniformForSkinSection(uniform, visitor->getCurrentSkinSection());
			boneCnt = visitor->getCurrentSkinSection()->getBoneCount();
			for (GLuint boneIdx = 0; boneIdx < boneCnt; boneIdx++) 
			{
				CC3Matrix3x3PopulateFrom4x3(&m3x3, visitor->getGlobalBoneMatrixAt(boneIdx));
				CC3Matrix3x3InvertAdjointTranspose(&m3x3);
				uniform->setMatrix3x3( &m3x3, boneIdx );
			}
			return true;
		case kCC3SemanticBoneMatricesEyeSpace:
			CC3AssertBoneUniformForSkinSection(uniform, visitor->getCurrentSkinSection());
			boneCnt = visitor->getCurrentSkinSection()->getBoneCount();
			for (GLuint boneIdx = 0; boneIdx < boneCnt; boneIdx++)
				uniform->setMatrix4x3( visitor->getEyeSpaceBoneMatrixAt(boneIdx), boneIdx );
			return true;
		case kCC3SemanticBoneMatricesInvTranEyeSpace:
			CC3AssertBoneUniformForSkinSection(uniform, visitor->getCurrentSkinSection());
			boneCnt = visitor->getCurrentSkinSection()->getBoneCount();
			for (GLuint boneIdx = 0; boneIdx < boneCnt; boneIdx++) 
			{
				CC3Matrix3x3PopulateFrom4x3(&m3x3, visitor->getEyeSpaceBoneMatrixAt(boneIdx));
				CC3Matrix3x3InvertAdjointTranspose(&m3x3);
				uniform->setMatrix3x3( &m3x3, boneIdx );
			}
			return true;
		case kCC3SemanticBoneMatricesModelSpace:
			{
				CC3AssertBoneUniformForSkinSection(uniform, visitor->getCurrentSkinSection());
				CC3SkinSection* pSection = visitor->getCurrentSkinSection();
				if ( pSection )
				{
					boneCnt = pSection->getBoneCount();
					for (GLuint boneIdx = 0; boneIdx < boneCnt; boneIdx++)
						uniform->setMatrix4x3( visitor->getModelSpaceBoneMatrixAt(boneIdx), boneIdx );
				}
			}
			return true;
		case kCC3SemanticBoneMatricesInvTranModelSpace:
			{
				CC3AssertBoneUniformForSkinSection(uniform, visitor->getCurrentSkinSection());
				CC3SkinSection* pSection = visitor->getCurrentSkinSection();
				if ( pSection )
				{
					boneCnt = pSection->getBoneCount();
					for (GLuint boneIdx = 0; boneIdx < boneCnt; boneIdx++) 
					{
						CC3Matrix3x3PopulateFrom4x3(&m3x3, visitor->getModelSpaceBoneMatrixAt(boneIdx));
						CC3Matrix3x3InvertAdjointTranspose(&m3x3);
						uniform->setMatrix3x3( &m3x3, boneIdx );
					}
				}	
			}		
			return true;
			
		// BONE SKINNING DISCRETE TRANSFORMS
		case kCC3SemanticBoneQuaternionsGlobal:
			{
				CC3AssertBoneUniformForSkinSection(uniform, visitor->getCurrentSkinSection());
				CC3SkinSection* pSection = visitor->getCurrentSkinSection();
				if ( pSection )
				{
					boneCnt = pSection->getBoneCount();
					for (GLuint boneIdx = 0; boneIdx < boneCnt; boneIdx++) 
					{
						pm4x3 = visitor->getGlobalBoneMatrixAt( boneIdx );
						uniform->setVector4( CC3Matrix4x3ExtractQuaternion(pm4x3), boneIdx );
					}
				}
			}
			return true;
		case kCC3SemanticBoneTranslationsGlobal:
			{
				CC3AssertBoneUniformForSkinSection(uniform, visitor->getCurrentSkinSection());
				CC3SkinSection* pSection = visitor->getCurrentSkinSection();
				if ( pSection )
				{
					boneCnt = pSection->getBoneCount();
					for (GLuint boneIdx = 0; boneIdx < boneCnt; boneIdx++) 
					{
						pm4x3 = visitor->getGlobalBoneMatrixAt( boneIdx );
						uniform->setVector( CC3Matrix4x3ExtractTranslation(pm4x3), boneIdx );
					}
				}
			}
			return true;
		case kCC3SemanticBoneScalesGlobal:
		case kCC3SemanticBoneScalesEyeSpace:		// Same as global because view matrix is not scaled
			{
				CC3AssertBoneUniformForSkinSection(uniform, visitor->getCurrentSkinSection());
				CC3SkinSection* pSection = visitor->getCurrentSkinSection();
				if ( pSection )
				{
					boneCnt = pSection->getBoneCount();
					for (GLuint boneIdx = 0; boneIdx < boneCnt; boneIdx++)
						uniform->setVector( visitor->getCurrentMeshNode()->getGlobalScale().scale( pSection->getBoneAt(boneIdx)->getSkeletalScale() ) );
				}
			}
			return true;
		case kCC3SemanticBoneQuaternionsEyeSpace:
			{
				CC3AssertBoneUniformForSkinSection(uniform, visitor->getCurrentSkinSection());
				CC3SkinSection* pSection = visitor->getCurrentSkinSection();
				if ( pSection )
				{
					boneCnt = pSection->getBoneCount();
					for (GLuint boneIdx = 0; boneIdx < boneCnt; boneIdx++) 
					{
						pm4x3 = visitor->getEyeSpaceBoneMatrixAt( boneIdx );
						uniform->setVector4( CC3Matrix4x3ExtractQuaternion(pm4x3), boneIdx );
					}
				}
			}
			return true;
		case kCC3SemanticBoneTranslationsEyeSpace:
			{
				CC3AssertBoneUniformForSkinSection(uniform, visitor->getCurrentSkinSection());
				CC3SkinSection* pSection = visitor->getCurrentSkinSection();
				if ( pSection )
				{
					boneCnt = pSection->getBoneCount();
					for (GLuint boneIdx = 0; boneIdx < boneCnt; boneIdx++) 
					{
						pm4x3 = visitor->getEyeSpaceBoneMatrixAt( boneIdx );
						uniform->setVector( CC3Matrix4x3ExtractTranslation(pm4x3), boneIdx );
					}
				}
			}
			return true;
		case kCC3SemanticBoneQuaternionsModelSpace:
			{
				CC3AssertBoneUniformForSkinSection(uniform, visitor->getCurrentSkinSection());
				CC3SkinSection* pSection = visitor->getCurrentSkinSection();
				if ( pSection )
				{
					boneCnt = pSection->getBoneCount();
					for (GLuint boneIdx = 0; boneIdx < boneCnt; boneIdx++) 
					{
						pm4x3 = visitor->getModelSpaceBoneMatrixAt( boneIdx );
						uniform->setVector4( CC3Matrix4x3ExtractQuaternion(pm4x3), boneIdx );
					}
				}
			}
			return true;
		case kCC3SemanticBoneTranslationsModelSpace:
			{
				CC3AssertBoneUniformForSkinSection(uniform, visitor->getCurrentSkinSection());
				CC3SkinSection* pSection = visitor->getCurrentSkinSection();
				if ( pSection )
				{
					boneCnt = pSection->getBoneCount();
					for (GLuint boneIdx = 0; boneIdx < boneCnt; boneIdx++) 
					{
						pm4x3 = visitor->getModelSpaceBoneMatrixAt( boneIdx );
						uniform->setVector( CC3Matrix4x3ExtractTranslation(pm4x3), boneIdx );
					}
				}
			}
			return true;
		case kCC3SemanticBoneScalesModelSpace:
			{
				CC3AssertBoneUniformForSkinSection(uniform, visitor->getCurrentSkinSection());
				CC3SkinSection* pSection = visitor->getCurrentSkinSection();
				if ( pSection )
				{
					boneCnt = pSection->getBoneCount();
					for (GLuint boneIdx = 0; boneIdx < boneCnt; boneIdx++)
						uniform->setVector( pSection->getBoneAt(boneIdx)->getSkeletalScale() );
				}
			}
			return true;

		// CAMERA -----------------
		case kCC3SemanticCameraLocationGlobal:
			if ( visitor->getCamera() )
				uniform->setVector( visitor->getCamera()->getGlobalLocation() );
			return true;
		case kCC3SemanticCameraLocationModelSpace:
			if ( visitor->getCamera() )
				uniform->setVector( visitor->transformGlobalLocationToModelSpace( visitor->getCamera()->getGlobalLocation() ) );
			return true;
		case kCC3SemanticCameraFrustum: 
			{
				// Applies the field of view angle to the narrower aspect.
				vp = visitor->getRenderSurface()->getViewport();
				GLfloat aspect = (GLfloat) vp.w / (GLfloat) vp.h;
				CC3Camera* cam = visitor->getCamera();
				if ( cam )
				{
					GLfloat fovWidth, fovHeight;
					if (aspect >= 1.0f) 
					{			// Landscape
						fovHeight = CC3DegToRad(cam->getEffectiveFieldOfView());
						fovWidth = fovHeight * aspect;
					} else {						// Portrait
						fovWidth = CC3DegToRad(cam->getEffectiveFieldOfView());
						fovHeight = fovWidth / aspect;
					}
					uniform->setVector4( CC3Vector4(fovWidth, fovHeight,
						cam->getNearClippingDistance(),
						cam->getFarClippingDistance()) );
				}
				
			return true;
		}
		case kCC3SemanticCameraFrustumDepth: 
			{
				CC3Camera* cam = visitor->getCamera();
				if ( cam )
				{
					cam->getProjectionMatrix()->populateCC3Matrix4x4( &m4x4 );
					uniform->setVector4( CC3Vector4(cam->getFarClippingDistance(),
						cam->getNearClippingDistance(),
						m4x4.c3r3, m4x4.c4r3) );
				}
			return true;
		}
		case kCC3SemanticViewport:
			vp = visitor->getRenderSurface()->getViewport();
			uniform->setIntVector4( CC3IntVector4Make(vp.x, vp.y, vp.w, vp.h) );
			return true;
			
		// MATERIALS --------------
		case kCC3SemanticColor:
			uniform->setColor4F( visitor->getCurrentColor() );
			return true;
		case kCC3SemanticMaterialColorAmbient:
			uniform->setColor4F( visitor->getCurrentMaterial()->getEffectiveAmbientColor() );
			return true;
		case kCC3SemanticMaterialColorDiffuse:
			uniform->setColor4F( visitor->getCurrentMaterial()->getEffectiveDiffuseColor() );
			return true;
		case kCC3SemanticMaterialColorSpecular:
			uniform->setColor4F( visitor->getCurrentMaterial()->getEffectiveSpecularColor() );
			return true;
		case kCC3SemanticMaterialColorEmission:
			uniform->setColor4F( visitor->getCurrentMaterial()->getEffectiveEmissionColor() );
			return true;
		case kCC3SemanticMaterialOpacity:
			uniform->setFloat( visitor->getCurrentMaterial()->getEffectiveDiffuseColor().a );
			return true;
		case kCC3SemanticMaterialShininess:
			uniform->setFloat( visitor->getCurrentMaterial()->getShininess() );
			return true;
		case kCC3SemanticMaterialReflectivity:
			uniform->setFloat( visitor->getCurrentMaterial()->getReflectivity() );
			return true;
		case kCC3SemanticMinimumDrawnAlpha:
			mat = visitor->getCurrentMaterial();
			uniform->setFloat( (mat->shouldDrawLowAlpha() ? 0.0f : mat->getAlphaTestReference()) );
			return true;
			
		// LIGHTING --------------
		case kCC3SemanticIsUsingLighting:
			uniform->setBoolean( visitor->getCurrentNode()->shouldUseLighting() );
			return true;
		case kCC3SemanticSceneLightColorAmbient:
			{
				CC3Scene* pScene = visitor->getScene();
				if ( pScene )
					uniform->setColor4F( visitor->getScene()->getAmbientLight() );
			}
			
			return true;
		case kCC3SemanticLightIsEnabled:
			for (GLint i = 0; i < uniformSize; i++)
			{
				CC3Light* light = visitor->getLightAt( semanticIndex + i );
				if ( light )
					uniform->setBoolean( light->isVisible(), i );
			}
			return true;

		case kCC3SemanticLightInvertedPositionGlobal:
			isInverted = true;
		case kCC3SemanticLightPositionGlobal:
			for (GLint i = 0; i < uniformSize; i++) 
			{
				CC3Light* light = visitor->getLightAt( semanticIndex + i );
				if ( light )
				{
					CC3Vector4 ltPos = light->getGlobalHomogeneousPosition();
					if (isInverted)
						ltPos = ltPos.homogeneousNegate();
					uniform->setVector4( ltPos, i );
				}
			}
			return true;
			
		case kCC3SemanticLightInvertedPositionEyeSpace:
			isInverted = true;
		case kCC3SemanticLightPositionEyeSpace:
			for (GLint i = 0; i < uniformSize; i++) 
			{
				CC3Light* light = visitor->getLightAt( semanticIndex + i );
				if ( light )
				{
					CC3Vector4 ltPos = light->getGlobalHomogeneousPosition();
					if (isInverted) ltPos = ltPos.homogeneousNegate();
					// Transform global position/direction to eye space and normalize if direction
					ltPos = CC3Matrix4x3TransformCC3Vector4(visitor->getViewMatrix(), ltPos);
					if (light->isDirectionalOnly()) 
						ltPos = ltPos.normalize();
					uniform->setVector4( ltPos, i );
				}
			}
			return true;
			
		case kCC3SemanticLightInvertedPositionModelSpace:
			isInverted = true;
		case kCC3SemanticLightPositionModelSpace:
			visitor->getCurrentMeshNode()->getGlobalTransformMatrixInverted()->populateCC3Matrix4x3( &m4x3 );
			for (GLint i = 0; i < uniformSize; i++) 
			{
				CC3Light* light = visitor->getLightAt( semanticIndex + i );
				if ( light )
				{
					CC3Vector4 ltPos = light->getGlobalHomogeneousPosition();
					if (isInverted) 
						ltPos = ltPos.homogeneousNegate();
					// Transform global position/direction to model space and normalize if direction
					ltPos = CC3Matrix4x3TransformCC3Vector4(&m4x3, ltPos);
					if (light->isDirectionalOnly()) 
						ltPos = ltPos.normalize();
					uniform->setVector4( ltPos, i );
				}
			}
			return true;

		case kCC3SemanticLightColorAmbient:
			for (GLint i = 0; i < uniformSize; i++) 
			{
				CC3Light* light = visitor->getLightAt( semanticIndex + i );
				if ( light )
				{
					ccColor4F ltColor = light->isVisible() ? light->getAmbientColor() : kCCC4FBlackTransparent;
					uniform->setColor4F( ltColor, i );
				}
			}
			return true;
		case kCC3SemanticLightColorDiffuse:
			for (GLint i = 0; i < uniformSize; i++) 
			{
				CC3Light* light = visitor->getLightAt( semanticIndex + i );
				if ( light )
				{
					ccColor4F ltColor = light->isVisible() ? light->getDiffuseColor() : kCCC4FBlackTransparent;
					uniform->setColor4F( ltColor, i );
				}
			}
			return true;
		case kCC3SemanticLightColorSpecular:
			for (GLint i = 0; i < uniformSize; i++) 
			{
				CC3Light* light = visitor->getLightAt( semanticIndex + i );
				if ( light )
				{
					ccColor4F ltColor = light->isVisible() ? light->getSpecularColor() : kCCC4FBlackTransparent;
					uniform->setColor4F( ltColor, i );
				}
			}
			return true;
		case kCC3SemanticLightAttenuation:
			for (GLint i = 0; i < uniformSize; i++) 
			{
				CC3Light* light = visitor->getLightAt( semanticIndex + i );
				if ( light )
				{
					CC3AttenuationCoefficients ac = CC3AttenuationCoefficientsLegalize(light->getAttenuation());
					uniform->setVector( *(CC3Vector*)&ac, i );
				}
			}
			return true;

		case kCC3SemanticLightSpotDirectionGlobal:
			for (GLint i = 0; i < uniformSize; i++)
			{
				CC3Light* light = visitor->getLightAt( semanticIndex + i );
				if ( light )
					uniform->setVector( light->getGlobalForwardDirection(), i );
			}
			return true;
		case kCC3SemanticLightSpotDirectionEyeSpace:
			for (GLint i = 0; i < uniformSize; i++) 
			{
				CC3Light* light = visitor->getLightAt( semanticIndex + i );
				if ( light )
				{
					CC3Vector spotDir = light->getGlobalForwardDirection();
					// Transform global direction to eye space and normalize
					spotDir = CC3Matrix4x3TransformDirection(visitor->getViewMatrix(), spotDir);
					uniform->setVector( spotDir.normalize(), i );

				}
			}
			return true;
		case kCC3SemanticLightSpotDirectionModelSpace:
			visitor->getCurrentMeshNode()->getGlobalTransformMatrixInverted()->populateCC3Matrix4x3( &m4x3 );
			for (GLint i = 0; i < uniformSize; i++) 
			{
				CC3Light* light = visitor->getLightAt( semanticIndex + i );
				if ( light )
				{
					CC3Vector spotDir = light->getGlobalForwardDirection();
					// Transform global direction to model space and normalize
					spotDir = CC3Matrix4x3TransformDirection(&m4x3, spotDir);
					uniform->setVector( spotDir.normalize(), i );
				}
			}
			return true;
		case kCC3SemanticLightSpotExponent:
			for (GLint i = 0; i < uniformSize; i++) 
			{
				CC3Light* light = visitor->getLightAt( semanticIndex + i );
				if ( light )
					uniform->setFloat( light->getSpotExponent(), i );
			}
			return true;
		case kCC3SemanticLightSpotCutoffAngle:
			for (GLint i = 0; i < uniformSize; i++) 
			{
				CC3Light* light = visitor->getLightAt( semanticIndex + i );
				if ( light )
					uniform->setFloat( light->getSpotCutoffAngle(), i );
			}
			return true;
		case kCC3SemanticLightSpotCutoffAngleCosine:
			for (GLint i = 0; i < uniformSize; i++) 
			{
				CC3Light* light = visitor->getLightAt( semanticIndex + i );
				if ( light )
					uniform->setFloat( cosf(CC3DegToRad(light->getSpotCutoffAngle())), i );
			}
			return true;
			
		case kCC3SemanticIsUsingLightProbes:
			uniform->setBoolean( visitor->getCurrentNode()->shouldUseLightProbes() );
			return true;
		case kCC3SemanticLightProbeCount:
			uniform->setInteger( visitor->getLightProbeCount() );
			return true;
		case kCC3SemanticLightProbeIsEnabled:
			for (GLint i = 0; i < uniformSize; i++)
			{
				CC3Node* lp = visitor->getLightProbeAt( semanticIndex + i );
				uniform->setBoolean( lp->isVisible(), i );
			}
			return true;
		case kCC3SemanticLightProbeLocationGlobal:
			for (GLint i = 0; i < uniformSize; i++) 
			{
				CC3Node* lp = visitor->getLightProbeAt( semanticIndex + i );
				uniform->setVector( lp->getGlobalLocation() );
			}
			return true;
		case kCC3SemanticLightProbeLocationEyeSpace:
			for (GLint i = 0; i < uniformSize; i++) 
			{
				CC3Node* lp = visitor->getLightProbeAt( semanticIndex + i );
				uniform->setVector( visitor->transformGlobalLocationToEyeSpace( lp->getGlobalLocation() ) );
			}
			return true;
		case kCC3SemanticLightProbeLocationModelSpace:
			for (GLint i = 0; i < uniformSize; i++) 
			{
				CC3Node* lp = visitor->getLightProbeAt( semanticIndex + i );
				uniform->setVector( visitor->transformGlobalLocationToModelSpace( lp->getGlobalLocation() ) );
			}
			return true;
		case kCC3SemanticLightProbeColorDiffuse:
			for (GLint i = 0; i < uniformSize; i++)
			{
				CC3Node* lp = visitor->getLightProbeAt( semanticIndex + i );
				ccColor4F lpColor = lp->isVisible() ? lp->getDiffuseColor() : kCCC4FBlackTransparent;
				uniform->setColor4F( lpColor, i );
			}
			return true;
			
		case kCC3SemanticFogIsEnabled:
			{
				CC3Scene* pScene = visitor->getScene();
				if ( pScene )
					uniform->setBoolean( pScene->getFog()->isVisible() );
			}
			return true;
		case kCC3SemanticFogColor:
			{
				CC3Scene* pScene = visitor->getScene();
				if ( pScene )
					uniform->setColor4F( pScene->getFog()->getDiffuseColor() );
			}
			return true;
		case kCC3SemanticFogAttenuationMode:
			{
				CC3Scene* pScene = visitor->getScene();
				if ( pScene )
					uniform->setInteger( pScene->getFog()->getAttenuationMode() );
			}
			return true;
		case kCC3SemanticFogDensity:
			{
				CC3Scene* pScene = visitor->getScene();
				if ( pScene )
					uniform->setFloat( pScene->getFog()->getDensity() );
			}
			return true;
		case kCC3SemanticFogStartDistance:
			{
				CC3Scene* pScene = visitor->getScene();
				if ( pScene )
					uniform->setFloat( pScene->getFog()->getStartDistance() );
			}
			return true;
		case kCC3SemanticFogEndDistance:
			{
				CC3Scene* pScene = visitor->getScene();
				if ( pScene )
					uniform->setFloat( pScene->getFog()->getEndDistance() );
			}
			return true;

		// TEXTURES --------------
		case kCC3SemanticTextureCount:
			// Count all textures of any type
			uniform->setInteger( visitor->getTextureCount() );
			return true;

		case kCC3SemanticTexture2DCount:
			mat = visitor->getCurrentMaterial();
			tuCnt = visitor->getTextureCount();
			// Count just the textures whose sampler semantic is of the correct type
			for (GLuint tuIdx = 0; tuIdx < tuCnt; tuIdx++)
				if ( mat->getTextureForTextureUnit(tuIdx)->getSamplerSemantic() == kCC3SemanticTexture2DSampler ) 
					texCnt++;

			uniform->setInteger( texCnt );
			return true;
			
		case kCC3SemanticTextureCubeCount:
			mat = visitor->getCurrentMaterial();
			tuCnt = visitor->getTextureCount();
			// Count just the textures whose sampler semantic is of the correct type
			for (GLuint tuIdx = 0; tuIdx < tuCnt; tuIdx++)
				if ( mat->getTextureForTextureUnit(tuIdx)->getSamplerSemantic() == kCC3SemanticTextureCubeSampler ) 
					texCnt++;
			uniform->setInteger( texCnt );
			return true;

		case kCC3SemanticTextureSampler:
			// Samplers that can be any type are simply consecutive texture unit indices
			// starting at the semanticIndex of the uniform. Typically, semanticIndex > 0
			// and uniformSize > 1 are mutually exclusive.
			for (GLint i = 0; i < uniformSize; i++) 
				uniform->setInteger( semanticIndex + i, i );
			return true;

		case kCC3SemanticTexture2DSampler:
			// 2D samplers always come first and are consecutive, so we can simply use consecutive
			// texture unit indices starting at the semanticIndex of the uniform. Typically,
			// semanticIndex > 0 and uniformSize > 1 are mutually exclusive.
			semanticIndex += visitor->getCurrentShaderProgram()->getTexture2DStart();
			for (GLint i = 0; i < uniformSize; i++)
				uniform->setInteger( semanticIndex + i, i );
			return true;
			
		case kCC3SemanticTextureCubeSampler:
			// Cube samplers always come after 2D samplers, and are consecutive, so we can simply
			// use consecutive texture unit indices starting at the semanticIndex of the uniform,
			// plus an offset to skip any 2D textures. Typically, semanticIndex > 0 and
			// uniformSize > 1 are mutually exclusive.
			semanticIndex += visitor->getCurrentShaderProgram()->getTextureCubeStart();
			for (GLint i = 0; i < uniformSize; i++) 
				uniform->setInteger( semanticIndex + i, i );
			return true;
			
		case kCC3SemanticTextureLightProbeSampler:
			// Light probe samplers always come after the model's 2D & cube samplers, and are
			// consecutive, so we can simply use consecutive texture unit indices starting at
			// the semanticIndex of the uniform, plus an offset to skip the model textures.
			// Typically, semanticIndex > 0 and uniformSize > 1 are mutually exclusive.
			semanticIndex += visitor->getCurrentShaderProgram()->getTextureLightProbeStart();
			for (GLint i = 0; i < uniformSize; i++) 
				uniform->setInteger( semanticIndex + i, i );
			return true;
			
		// The semantics below mimic OpenGL ES 1.1 configuration functionality for combining texture units.
		// In most shaders, these will be left unused in favor of customized the texture combining in code.
		case kCC3SemanticTexUnitMode:
			for (GLint i = 0; i < uniformSize; i++) 
			{
				CC3TextureUnit* tu = visitor->getCurrentTextureUnitAt( semanticIndex + i );
				uniform->setInteger( tu ? tu->getTextureEnvironmentMode() : i );
			}
			return true;
		case kCC3SemanticTexUnitConstantColor:
			for (GLint i = 0; i < uniformSize; i++) 
			{
				CC3TextureUnit* tu = visitor->getCurrentTextureUnitAt( semanticIndex + i );
				uniform->setColor4F( tu ? tu->getConstantColor() :  kCCC4FBlackTransparent, i );
			}
			return true;
            
		// MODEL ----------------
		case kCC3SemanticCenterOfGeometry:
			uniform->setVector( visitor->getCurrentMeshNode()->getMesh()->getCenterOfGeometry() );
			return true;
		case kCC3SemanticBoundingRadius:
			uniform->setFloat( visitor->getCurrentMeshNode()->getMesh()->getRadius() );
			return true;
		case kCC3SemanticBoundingBoxMin:
			uniform->setVector( visitor->getCurrentMeshNode()->getMesh()->getBoundingBox().minimum );
			return true;
		case kCC3SemanticBoundingBoxMax:
			uniform->setVector( visitor->getCurrentMeshNode()->getMesh()->getBoundingBox().maximum );
			return true;
		case kCC3SemanticBoundingBoxSize:
			uniform->setVector( visitor->getCurrentMeshNode()->getMesh()->getBoundingBox().getSize() );
			return true;
		case kCC3SemanticAnimationFraction:
			uniform->setFloat( visitor->getCurrentMeshNode()->animationTimeOnTrack(0) );
			return true;

		// PARTICLES ------------
		case kCC3SemanticPointSize: {
			emitter = dynamic_cast<CC3PointParticleEmitter*>(visitor->getCurrentNode());
			isPtEmitter = (emitter != NULL);
			uniform->setFloat( isPtEmitter ? emitter->getNormalizedParticleSize() : 0.0f );
			return true;
		}
		case kCC3SemanticPointSizeAttenuation: {
			emitter = dynamic_cast<CC3PointParticleEmitter*>(visitor->getCurrentNode());
			isPtEmitter = (emitter != NULL);
			CC3AttenuationCoefficients ac = (isPtEmitter)
												? CC3AttenuationCoefficientsLegalize(emitter->getParticleSizeAttenuation())
												: kCC3AttenuationNone;
			uniform->setVector( *(CC3Vector*)&ac );
			return true;
		}
		case kCC3SemanticPointSizeMinimum: {
			emitter = dynamic_cast<CC3PointParticleEmitter*>(visitor->getCurrentNode());
			isPtEmitter = (emitter != NULL);
			uniform->setFloat( isPtEmitter ? emitter->getNormalizedParticleSizeMinimum() : 0.0f );
			return true;
		}
		case kCC3SemanticPointSizeMaximum: {
			emitter = dynamic_cast<CC3PointParticleEmitter*>(visitor->getCurrentNode());
			isPtEmitter = (emitter != NULL);
			maximumPointSize = visitor->getGL()->value_GL_MAX_POINT_SIZE;
			maximumPointSize = CLAMP( maximumPointSize, emitter->getNormalizedParticleSizeMaximum(), maximumPointSize );
			uniform->setFloat( isPtEmitter ? maximumPointSize : 0.0f );
			return true;
		}
		case kCC3SemanticPointSpritesIsEnabled:
			uniform->setBoolean( visitor->getCurrentMeshNode()->isDrawingPointSprites() );
			return true;
			
		// TIME ------------------
		case kCC3SemanticFrameTime:
			uniform->setFloat( visitor->getDeltaTime() );
			return true;
		case kCC3SemanticSceneTime:
			sceneTime = (float)visitor->getScene()->getElapsedTimeSinceOpened();
			uniform->setPoint( ccp(sceneTime, fmodf(sceneTime, 1.0)) );
			return true;
		case kCC3SemanticSceneTimeSine:
			sceneTime = (float)visitor->getScene()->getElapsedTimeSinceOpened();
			uniform->setVector4( CC3Vector4(sinf(sceneTime),
												sinf(sceneTime / 2.0f),
												sinf(sceneTime / 4.0f),
												sinf(sceneTime / 8.0f)) );
			return true;
		case kCC3SemanticSceneTimeCosine:
			sceneTime = (float)visitor->getScene()->getElapsedTimeSinceOpened();
			uniform->setVector4( CC3Vector4(cosf(sceneTime),
												cosf(sceneTime / 2.0f),
												cosf(sceneTime / 4.0f),
												cosf(sceneTime / 8.0f)) );
			return true;
		case kCC3SemanticSceneTimeTangent:
			sceneTime = (float)visitor->getScene()->getElapsedTimeSinceOpened();
			uniform->setVector4( CC3Vector4(tanf(sceneTime),
												tanf(sceneTime / 2.0f),
												tanf(sceneTime / 4.0f),
												tanf(sceneTime / 8.0f)) );
			return true;
			
		// MISC ENVIRONMENT ---------
		case kCC3SemanticDrawCountCurrentFrame:
			//uniform->setInteger( CC3GLDraws() );
#pragma _NOTE_TODO( "kCC3SemanticDrawCountCurrentFrame implementation here" )
			return true;
		case kCC3SemanticRandomNumber:
			uniform->setFloat( CC3RandomFloat() );
			return true;
			
		default:
            if ( populateColorUniforms(uniform, visitor) )
                return true;
            
            return false;
	}
}

bool CC3ShaderSemanticsBase::populateColorUniforms( CC3GLSLUniform *uniform, CC3NodeDrawingVisitor *visitor )
{
    GLint uniformSize = uniform->getSize();
    //GLuint semanticIndex = uniform->getSemanticIndex();
    
    switch ( uniform->getSemantic() )
    {
        case kCC3SemanticTexUnitCombineRGBFunction:
            for (GLint i = 0; i < uniformSize; i++) {
                //CC3ConfigurableTextureUnit* ctu = (CC3ConfigurableTextureUnit*)visitor->getCurrentTextureUnitAt( semanticIndex + i );
                CCAssert( false, "kCC3SemanticTexUnitCombineRGBFunction functor is not implemented" );
                /*bool isCTU = [ctu isKindOfClass: [CC3ConfigurableTextureUnit class] );
                 uniform->setInteger: (isCTU ? ctu.combineRGBFunction :  GL_MODULATE) at: i );*/
            }
            return true;
        case kCC3SemanticTexUnitSource0RGB:
            for (GLint i = 0; i < uniformSize; i++) {
                //CC3ConfigurableTextureUnit* ctu = (CC3ConfigurableTextureUnit*)visitor->getCurrentTextureUnitAt( semanticIndex + i );
                CCAssert( false, "kCC3SemanticTexUnitSource0RGB functor is not implemented" );
                /*BOOL isCTU = [ctu isKindOfClass: [CC3ConfigurableTextureUnit class] );
                 uniform->setInteger: (isCTU ? ctu.rgbSource0 :  GL_TEXTURE) at: i );*/
            }
            return true;
        case kCC3SemanticTexUnitSource1RGB:
            for (GLint i = 0; i < uniformSize; i++) {
                //CC3ConfigurableTextureUnit* ctu = (CC3ConfigurableTextureUnit*)visitor->getCurrentTextureUnitAt( semanticIndex + i );
                CCAssert( false, "kCC3SemanticTexUnitSource1RGB functor is not implemented" );
                /*BOOL isCTU = [ctu isKindOfClass: [CC3ConfigurableTextureUnit class] );
                 uniform->setInteger: (isCTU ? ctu.rgbSource1 :  GL_PREVIOUS) at: i );*/
            }
            return true;
        case kCC3SemanticTexUnitSource2RGB:
            for (GLint i = 0; i < uniformSize; i++) {
                //CC3ConfigurableTextureUnit* ctu = (CC3ConfigurableTextureUnit*)visitor->getCurrentTextureUnitAt( semanticIndex + i );
                CCAssert( false, "kCC3SemanticTexUnitSource2RGB functor is not implemented" );
                /*BOOL isCTU = [ctu isKindOfClass: [CC3ConfigurableTextureUnit class] );
                 uniform->setInteger: (isCTU ? ctu.rgbSource2 :  GL_CONSTANT) at: i );*/
            }
            return true;
        case kCC3SemanticTexUnitOperand0RGB:
            for (GLint i = 0; i < uniformSize; i++) {
                //CC3ConfigurableTextureUnit* ctu = (CC3ConfigurableTextureUnit*)visitor->getCurrentTextureUnitAt( semanticIndex + i );
                CCAssert( false, "kCC3SemanticTexUnitOperand0RGB functor is not implemented" );
                /*BOOL isCTU = [ctu isKindOfClass: [CC3ConfigurableTextureUnit class] );
                 uniform->setInteger: (isCTU ? ctu.rgbOperand0 :  GL_SRC_COLOR) at: i );*/
            }
            return true;
        case kCC3SemanticTexUnitOperand1RGB:
            for (GLint i = 0; i < uniformSize; i++) {
                //CC3ConfigurableTextureUnit* ctu = (CC3ConfigurableTextureUnit*)visitor->getCurrentTextureUnitAt( semanticIndex + i );
                CCAssert( false, "kCC3SemanticTexUnitOperand1RGB functor is not implemented" );
                /*BOOL isCTU = [ctu isKindOfClass: [CC3ConfigurableTextureUnit class] );
                 uniform->setInteger: (isCTU ? ctu.rgbOperand1 :  GL_SRC_COLOR) at: i );*/
            }
            return true;
        case kCC3SemanticTexUnitOperand2RGB:
            for (GLint i = 0; i < uniformSize; i++) {
                //CC3ConfigurableTextureUnit* ctu = (CC3ConfigurableTextureUnit*)visitor->getCurrentTextureUnitAt( semanticIndex + i );
                CCAssert( false, "kCC3SemanticTexUnitOperand2RGB functor is not implemented" );
                /*BOOL isCTU = [ctu isKindOfClass: [CC3ConfigurableTextureUnit class] );
                 uniform->setInteger: (isCTU ? ctu.rgbOperand2 :  GL_SRC_ALPHA) at: i );*/
            }
            return true;
        case kCC3SemanticTexUnitCombineAlphaFunction:
            for (GLint i = 0; i < uniformSize; i++) {
                //CC3ConfigurableTextureUnit* ctu = (CC3ConfigurableTextureUnit*)visitor->getCurrentTextureUnitAt( semanticIndex + i );
                CCAssert( false, "kCC3SemanticTexUnitCombineAlphaFunction functor is not implemented" );
                /*BOOL isCTU = [ctu isKindOfClass: [CC3ConfigurableTextureUnit class] );
                 uniform->setInteger: (isCTU ? ctu.combineAlphaFunction :  GL_MODULATE) at: i );*/
            }
            return true;
        case kCC3SemanticTexUnitSource0Alpha:
            for (GLint i = 0; i < uniformSize; i++) {
                //CC3ConfigurableTextureUnit* ctu = (CC3ConfigurableTextureUnit*)visitor->getCurrentTextureUnitAt( semanticIndex + i );
                CCAssert( false, "kCC3SemanticTexUnitSource0Alpha functor is not implemented" );
                /*BOOL isCTU = [ctu isKindOfClass: [CC3ConfigurableTextureUnit class] );
                 uniform->setInteger: (isCTU ? ctu.alphaSource0 :  GL_TEXTURE) at: i );*/
            }
            return true;
        case kCC3SemanticTexUnitSource1Alpha:
            for (GLint i = 0; i < uniformSize; i++) {
                //CC3ConfigurableTextureUnit* ctu = (CC3ConfigurableTextureUnit*)visitor->getCurrentTextureUnitAt( semanticIndex + i );
                CCAssert( false, "kCC3SemanticTexUnitSource1Alpha functor is not implemented" );
                /*BOOL isCTU = [ctu isKindOfClass: [CC3ConfigurableTextureUnit class] );
                 uniform->setInteger: (isCTU ? ctu.alphaSource1 :  GL_PREVIOUS) at: i );*/
            }
            return true;
        case kCC3SemanticTexUnitSource2Alpha:
            for (GLint i = 0; i < uniformSize; i++) {
                //CC3ConfigurableTextureUnit* ctu = (CC3ConfigurableTextureUnit*)visitor->getCurrentTextureUnitAt( semanticIndex + i );
                CCAssert( false, "kCC3SemanticTexUnitSource2Alpha functor is not implemented" );
                /*BOOL isCTU = [ctu isKindOfClass: [CC3ConfigurableTextureUnit class] );
                 uniform->setInteger: (isCTU ? ctu.alphaSource2 :  GL_CONSTANT) at: i );*/
            }
            return true;
        case kCC3SemanticTexUnitOperand0Alpha:
            for (GLint i = 0; i < uniformSize; i++) {
                //CC3ConfigurableTextureUnit* ctu = (CC3ConfigurableTextureUnit*)visitor->getCurrentTextureUnitAt( semanticIndex + i );
                CCAssert( false, "kCC3SemanticTexUnitOperand0Alpha functor is not implemented" );
                /*BOOL isCTU = [ctu isKindOfClass: [CC3ConfigurableTextureUnit class] );
                 uniform->setInteger: (isCTU ? ctu.alphaOperand0 :  GL_SRC_ALPHA) at: i );*/
            }
            return true;
        case kCC3SemanticTexUnitOperand1Alpha:
            for (GLint i = 0; i < uniformSize; i++) {
                //CC3ConfigurableTextureUnit* ctu = (CC3ConfigurableTextureUnit*)visitor->getCurrentTextureUnitAt( semanticIndex + i );
                CCAssert( false, "kCC3SemanticTexUnitOperand1Alpha functor is not implemented" );
                /*BOOL isCTU = [ctu isKindOfClass: [CC3ConfigurableTextureUnit class] );
                 uniform->setInteger: (isCTU ? ctu.alphaOperand1 :  GL_SRC_ALPHA) at: i );*/
            }
            return true;
        case kCC3SemanticTexUnitOperand2Alpha:
            for (GLint i = 0; i < uniformSize; i++) {
                //CC3ConfigurableTextureUnit* ctu = (CC3ConfigurableTextureUnit*)visitor->getCurrentTextureUnitAt( semanticIndex + i );
                CCAssert( false, "kCC3SemanticTexUnitOperand2Alpha functor is not implemented" );
                /*BOOL isCTU = [ctu isKindOfClass: [CC3ConfigurableTextureUnit class] );
                 uniform->setInteger: (isCTU ? ctu.alphaOperand2 :  GL_SRC_ALPHA) at: i );*/
            }
            return true;
        default:
            return false;
            
    }
    
    return false;
}

CC3ShaderSemanticsByVarName::CC3ShaderSemanticsByVarName()
{
	m_varConfigsByName = NULL;
}

CC3ShaderSemanticsByVarName::~CC3ShaderSemanticsByVarName()
{
	if ( m_varConfigsByName )
		m_varConfigsByName->removeAllObjects();

	CC_SAFE_RELEASE( m_varConfigsByName );
}

void CC3ShaderSemanticsByVarName::init()
{
	m_varConfigsByName = CCDictionary::create();		// retained
	m_varConfigsByName->retain();
}

/**
 * Uses the variable name property to look up a configuration and sets the semantic
 * and semanticIndex properties of the specified variable from that configuration.
 */
bool CC3ShaderSemanticsByVarName::configureVariable( CC3GLSLVariable* variable )
{
	CC3GLSLVariableConfiguration* varConfig = (CC3GLSLVariableConfiguration*)m_varConfigsByName->objectForKey( variable->getName() );
	if (varConfig) 
	{
		variable->setSemantic( varConfig->getSemantic() );
		variable->setSemanticIndex( varConfig->getSemanticIndex() );
		variable->setScope( getVariableScopeForSemantic( varConfig->getSemantic() ) );
		return true;
	}
	return false;
}

void CC3ShaderSemanticsByVarName::addVariableConfiguration( CC3GLSLVariableConfiguration* varConfig )
{
	m_varConfigsByName->setObject(varConfig, varConfig->getName() );
}

void CC3ShaderSemanticsByVarName::mapVarName( const std::string& name, GLenum semantic, GLuint semanticIndex )
{
	CC3GLSLVariableConfiguration* varConfig = new CC3GLSLVariableConfiguration();
	varConfig->setName( name );
	varConfig->setSemantic( semantic );
	varConfig->setSemanticIndex( semanticIndex );
	addVariableConfiguration( varConfig );
	varConfig->release();
}

void CC3ShaderSemanticsByVarName::mapVarName( const std::string& name, GLenum semantic )
{
	mapVarName( name, semantic, 0 );
}

void CC3ShaderSemanticsByVarName::populateWithDefaultVariableNameMappings()
{	
	// VETEX ATTRIBUTES --------------
	mapVarName( "a_cc3Position"	, kCC3SemanticVertexLocation );				/**< Vertex location. */
	mapVarName( "a_cc3Normal"	, kCC3SemanticVertexNormal );					/**< Vertex normal. */
	mapVarName( "a_cc3Tangent"	, kCC3SemanticVertexTangent );				/**< Vertex tangent. */
	mapVarName( "a_cc3Bitangent", kCC3SemanticVertexBitangent );			/**< Vertex bitangent (aka binormal). */
	mapVarName( "a_cc3Color"	, kCC3SemanticVertexColor );					/**< Vertex color. */
	mapVarName( "a_cc3BoneWeights", kCC3SemanticVertexBoneWeights );		/**< Vertex skinning bone weights (each an array of length specified by u_cc3VertexBoneCount). */
	mapVarName( "a_cc3BoneIndices", kCC3SemanticVertexBoneIndices );		/**< Vertex skinning bone indices (each an array of length specified by u_cc3VertexBoneCount). */
	mapVarName( "a_cc3PointSize", kCC3SemanticVertexPointSize );			/**< Vertex point size. */
	
	// If only one texture coordinate attribute is used, the index suffix ("a_cc3TexCoordN") is optional.
	mapVarName( "a_cc3TexCoord", kCC3SemanticVertexTexture );				/**< Vertex texture coordinate for the first texture unit. */
	GLuint maxTexUnits = CC3OpenGL::sharedGL()->getMaxNumberOfTextureUnits();
	for (GLuint tuIdx = 0; tuIdx < maxTexUnits; tuIdx++)
		mapVarName( CC3String::stringWithFormat( (char*)"a_cc3TexCoord%d", tuIdx ), kCC3SemanticVertexTexture, tuIdx );	/**< Vertex texture coordinate for a texture unit. */
	
	// VERTEX STATE --------------
	mapVarName( "u_cc3VertexHasNormal", kCC3SemanticHasVertexNormal );							/**< (bool) Whether a vertex normal is available. */
	mapVarName( "u_cc3VertexHasTangent", kCC3SemanticHasVertexTangent );						/**< (bool) Whether a vertex tangent is available. */
	mapVarName( "u_cc3VertexHasBitangent", kCC3SemanticHasVertexBitangent );					/**< (bool) Whether a vertex bitangent is available. */
	mapVarName( "u_cc3VertexHasColor", kCC3SemanticHasVertexColor );							/**< (bool) Whether a vertex color is available. */
	mapVarName( "u_cc3VertexHasWeights", kCC3SemanticHasVertexWeight );						/**< (bool) Whether a vertex weight is available. */
	mapVarName( "u_cc3VertexHasMatrixIndices", kCC3SemanticHasVertexMatrixIndex );				/**< (bool) Whether a vertex matrix index is available. */
	mapVarName( "u_cc3VertexHasTexCoord", kCC3SemanticHasVertexTextureCoordinate );			/**< (bool) Whether a vertex texture coordinate is available. */
	mapVarName( "u_cc3VertexHasPointSize", kCC3SemanticHasVertexPointSize );					/**< (bool) Whether a vertex point size is available. */
	mapVarName( "u_cc3VertexShouldNormalizeNormal", kCC3SemanticShouldNormalizeVertexNormal );	/**< (bool) Whether vertex normals should be normalized. */
	mapVarName( "u_cc3VertexShouldRescaleNormal", kCC3SemanticShouldRescaleVertexNormal );		/**< (bool) Whether vertex normals should be rescaled. */
	mapVarName( "u_cc3VertexShouldDrawFrontFaces", kCC3SemanticShouldDrawFrontFaces );			/**< (bool) Whether the front side of each face is to be drawn. */
	mapVarName( "u_cc3VertexShouldDrawBackFaces", kCC3SemanticShouldDrawBackFaces );			/**< (bool) Whether the back side of each face is to be drawn. */
	
	// ENVIRONMENT MATRICES --------------
	mapVarName( "u_cc3MatrixModelLocal", kCC3SemanticModelLocalMatrix );						/**< (mat4) Current model-to-parent matrix. */
	mapVarName( "u_cc3MatrixModelLocalInv", kCC3SemanticModelLocalMatrixInv );					/**< (mat4) Inverse of current model-to-parent matrix. */
	mapVarName( "u_cc3MatrixModelLocalInvTran", kCC3SemanticModelLocalMatrixInvTran );			/**< (mat3) Inverse-transpose of current model-to-parent matrix. */
		
	mapVarName( "u_cc3MatrixModel", kCC3SemanticModelMatrix );									/**< (mat4) Current model-to-world matrix. */
	mapVarName( "u_cc3MatrixModelInv", kCC3SemanticModelMatrixInv );							/**< (mat4) Inverse of current model-to-world matrix. */
	mapVarName( "u_cc3MatrixModelInvTran", kCC3SemanticModelMatrixInvTran );					/**< (mat3) Inverse-transpose of current model-to-world matrix. */
	
	mapVarName( "u_cc3MatrixView", kCC3SemanticViewMatrix );									/**< (mat4) Camera view matrix. */
	mapVarName( "u_cc3MatrixViewInv", kCC3SemanticViewMatrixInv );								/**< (mat4) Inverse of camera view matrix. */
	mapVarName( "u_cc3MatrixViewInvTran", kCC3SemanticViewMatrixInvTran );						/**< (mat3) Inverse-transpose of camera view matrix. */
	
	mapVarName( "u_cc3MatrixModelView", kCC3SemanticModelViewMatrix );							/**< (mat4) Current model-view matrix. */
	mapVarName( "u_cc3MatrixModelViewInv", kCC3SemanticModelViewMatrixInv );					/**< (mat4) Inverse of current model-view matrix. */
	mapVarName( "u_cc3MatrixModelViewInvTran", kCC3SemanticModelViewMatrixInvTran );			/**< (mat3) Inverse-transpose of current model-view matrix. */
	
	mapVarName( "u_cc3MatrixProj", kCC3SemanticProjMatrix );									/**< (mat4) Camera projection matrix. */
	mapVarName( "u_cc3MatrixProjInv", kCC3SemanticProjMatrixInv );								/**< (mat4) Inverse of camera projection matrix. */
	mapVarName( "u_cc3MatrixProjInvTran", kCC3SemanticProjMatrixInvTran );						/**< (mat3) Inverse-transpose of camera projection matrix. */
	
	mapVarName( "u_cc3MatrixViewProj", kCC3SemanticViewProjMatrix );							/**< (mat4) Camera view and projection matrix. */
	mapVarName( "u_cc3MatrixViewProjInv", kCC3SemanticViewProjMatrixInv );						/**< (mat4) Inverse of camera view and projection matrix. */
	mapVarName( "u_cc3MatrixViewProjInvTran", kCC3SemanticViewProjMatrixInvTran );				/**< (mat3) Inverse-transpose of camera view and projection matrix. */
	
	mapVarName( "u_cc3MatrixModelViewProj", kCC3SemanticModelViewProjMatrix );					/**< (mat4) Current model-view-projection matrix. */
	mapVarName( "u_cc3MatrixModelViewProjInv", kCC3SemanticModelViewProjMatrixInv );			/**< (mat4) Inverse of current model-view-projection matrix. */
	mapVarName( "u_cc3MatrixModelViewProjInvTran", kCC3SemanticModelViewProjMatrixInvTran );	/**< (mat3) Inverse-transpose of current model-view-projection matrix. */
	
	// BONE SKINNING ----------------
	mapVarName( "u_cc3VertexBoneCount", kCC3SemanticVertexBoneCount );							/**< (int) Number of bones influencing each vertex (ie- number of bone-weights & bone-indices specified on each vertex) */
	mapVarName( "u_cc3BatchBoneCount", kCC3SemanticBatchBoneCount );								/**< (int) Number of bones that are being used by the current skin section. */

	// BONE SKINNING MATRICES ----------------
	mapVarName( "u_cc3BoneMatricesGlobal", kCC3SemanticBoneMatricesGlobal );					/**< (mat4[]) Array of bone matrices in the current mesh skin section in global coordinates (length of array is specified by u_cc3BatchBoneCount). */
	mapVarName( "u_cc3BoneMatricesInvTranGlobal", kCC3SemanticBoneMatricesInvTranGlobal );		/**< (mat3[]) Array of inverse-transposes of the bone matrices in the current mesh skin section in global coordinates (length of array is specified by u_cc3BatchBoneCount). */
	mapVarName( "u_cc3BoneMatricesEyeSpace", kCC3SemanticBoneMatricesEyeSpace );				/**< (mat4[]) Array of bone matrices in the current mesh skin section in eye space (length of array is specified by u_cc3BatchBoneCount). */
	mapVarName( "u_cc3BoneMatricesInvTranEyeSpace", kCC3SemanticBoneMatricesInvTranEyeSpace );	/**< (mat3[]) Array of inverse-transposes of the bone matrices in the current mesh skin section in eye space (length of array is specified by u_cc3BatchBoneCount). */
	mapVarName( "u_cc3BoneMatricesModel", kCC3SemanticBoneMatricesModelSpace );				/**< (mat4[]) Array of bone matrices in the current mesh skin section in model space (length of array is specified by u_cc3BatchBoneCount). */
	mapVarName( "u_cc3BoneMatricesInvTranModel", kCC3SemanticBoneMatricesInvTranModelSpace );	/**< (mat3[]) Array of inverse-transposes of the bone matrices in the current mesh skin section in model space (length of array is specified by u_cc3BatchBoneCount). */

	// BONE SKINNING DISCRETE TRANSFORMS
	mapVarName( "u_cc3BoneQuaternionsGlobal", kCC3SemanticBoneQuaternionsGlobal );				/**< (vec4[]) Array of bone quaternions in the current mesh skin section in global coordinates (length of array is specified by u_cc3BatchBoneCount). */
	mapVarName( "u_cc3BoneTranslationsGlobal", kCC3SemanticBoneTranslationsGlobal );			/**< (vec3[]) Array of bone translations in the current mesh skin section in global coordinates (length of array is specified by u_cc3BatchBoneCount). */
	mapVarName( "u_cc3BoneScalesGlobal", kCC3SemanticBoneScalesGlobal );						/**< (vec3[]) Array of bone scales in the current mesh skin section in global coordinates (length of array is specified by u_cc3BatchBoneCount). */
	mapVarName( "u_cc3BoneQuaternionsEyeSpace", kCC3SemanticBoneQuaternionsEyeSpace );			/**< (vec4[]) Array of bone quaternions in the current mesh skin section in eye space (length of array is specified by u_cc3BatchBoneCount). */
	mapVarName( "u_cc3BoneTranslationsEyeSpace", kCC3SemanticBoneTranslationsEyeSpace );		/**< (vec3[]) Array of bone translations in the current mesh skin section in eye space (length of array is specified by u_cc3BatchBoneCount). */
	mapVarName( "u_cc3BoneScalesEyeSpace", kCC3SemanticBoneScalesEyeSpace );					/**< (vec3[]) Array of bone scales in the current mesh skin section in eye space (length of array is specified by u_cc3BatchBoneCount). */
	mapVarName( "u_cc3BoneQuaternionsModelSpace", kCC3SemanticBoneQuaternionsModelSpace );		/**< (vec4[]) Array of bone quaternions in the current mesh skin section in model space (length of array is specified by u_cc3BatchBoneCount). */
	mapVarName( "u_cc3BoneTranslationsModelSpace", kCC3SemanticBoneTranslationsModelSpace );	/**< (vec3[]) Array of bone translations in the current mesh skin section in model space (length of array is specified by u_cc3BatchBoneCount). */
	mapVarName( "u_cc3BoneScalesModelSpace", kCC3SemanticBoneScalesModelSpace );				/**< (vec3[]) Array of bone scales in the current mesh skin section in model space (length of array is specified by u_cc3BatchBoneCount). */
	
	// CAMERA -----------------
	mapVarName( "u_cc3CameraPositionGlobal", kCC3SemanticCameraLocationGlobal );		/**< (vec3) Location of the camera in global coordinates. */
	mapVarName( "u_cc3CameraPositionModel", kCC3SemanticCameraLocationModelSpace );	/**< (vec3) Location of the camera in local coordinates of model (not camera). */
	mapVarName( "u_cc3CameraFrustum", kCC3SemanticCameraFrustum );						/**< (vec4) Dimensions of the camera frustum (FOV width (radians), FOV height (radians), near clip, far clip). */
	mapVarName( "u_cc3CameraFrustumDepth", kCC3SemanticCameraFrustumDepth );			/**< (vec4) The depth of the camera frustum (far clip, near clip, -(f+n)/(f-n), -2nf/(f-n)). */
	mapVarName( "u_cc3CameraViewport", kCC3SemanticViewport );							/**< (vec4 or ivec4) The viewport rectangle in pixels (x, y, width, height). */
	
	// MATERIALS --------------
	mapVarName( "u_cc3Color", kCC3SemanticColor );									/**< (vec4) Color when lighting & materials are not in use. */
	mapVarName( "u_cc3MaterialAmbientColor", kCC3SemanticMaterialColorAmbient );	/**< (vec4) Ambient color of the material. */
	mapVarName( "u_cc3MaterialDiffuseColor", kCC3SemanticMaterialColorDiffuse );	/**< (vec4) Diffuse color of the material. */
	mapVarName( "u_cc3MaterialSpecularColor", kCC3SemanticMaterialColorSpecular );	/**< (vec4) Specular color of the material. */
	mapVarName( "u_cc3MaterialEmissionColor", kCC3SemanticMaterialColorEmission );	/**< (vec4) Emission color of the material. */
	mapVarName( "u_cc3MaterialOpacity", kCC3SemanticMaterialOpacity );				/**< (float) Opacity of the material. */
	mapVarName( "u_cc3MaterialShininess", kCC3SemanticMaterialShininess );			/**< (float) Shininess of the material (0 <> 128). */
	mapVarName( "u_cc3MaterialReflectivity", kCC3SemanticMaterialReflectivity );	/**< (float) Reflectivity of the material (0 <> 1). */
	mapVarName( "u_cc3MaterialMinimumDrawnAlpha", kCC3SemanticMinimumDrawnAlpha );	/**< (float) Minimum alpha value to be drawn, otherwise will be rendered fully tranparent. */
	
	// LIGHTING --------------
	mapVarName( "u_cc3LightIsUsingLighting", kCC3SemanticIsUsingLighting );					/**< (bool) Whether the model will interact with scene lighting (either lights or light probes). */
	mapVarName( "u_cc3LightSceneAmbientLightColor", kCC3SemanticSceneLightColorAmbient );		/**< (vec4) Ambient light color of the scene. */
	
	mapVarName( "u_cc3LightIsEnabled", kCC3SemanticLightIsEnabled );							/**< (bool[]) Whether each light is available and enabled. */
	mapVarName( "u_cc3LightIsLightEnabled", kCC3SemanticLightIsEnabled );						/**< (bool[]) @deprecated. Renamed to u_cc3LightIsEnabled. */
	mapVarName( "u_cc3LightPositionEyeSpace", kCC3SemanticLightPositionEyeSpace );				/**< (vec4[]) Location of each light in eye space. */
	mapVarName( "u_cc3LightPositionGlobal", kCC3SemanticLightPositionGlobal );					/**< (vec4[]) Location of each light in global coordinates. */
	mapVarName( "u_cc3LightPositionModel", kCC3SemanticLightPositionModelSpace );				/**< (vec4[]) Location of each light in local coordinates of model (not light). */
	mapVarName( "u_cc3LightAmbientColor", kCC3SemanticLightColorAmbient );						/**< (vec4[]) Ambient color of each light. */
	mapVarName( "u_cc3LightDiffuseColor", kCC3SemanticLightColorDiffuse );						/**< (vec4[]) Diffuse color of each light. */
	mapVarName( "u_cc3LightSpecularColor", kCC3SemanticLightColorSpecular );					/**< (vec4[]) Specular color of each light. */
	mapVarName( "u_cc3LightAttenuation", kCC3SemanticLightAttenuation );						/**< (vec3[]) Distance attenuation coefficients for each light. */
	mapVarName( "u_cc3LightSpotDirectionEyeSpace", kCC3SemanticLightSpotDirectionEyeSpace );	/**< (vec3[]) Direction of each spotlight in eye space. */
	mapVarName( "u_cc3LightSpotDirectionGlobal", kCC3SemanticLightSpotDirectionGlobal );		/**< (vec3[]) Direction of each spotlight in global coordinates. */
	mapVarName( "u_cc3LightSpotDirectionModel", kCC3SemanticLightSpotDirectionModelSpace );	/**< (vec3[]) Direction of each spotlight in local coordinates of the model (not light). */
	mapVarName( "u_cc3LightSpotExponent", kCC3SemanticLightSpotExponent );						/**< (float[]) Fade-off exponent of each spotlight. */
	mapVarName( "u_cc3LightSpotCutoffAngle", kCC3SemanticLightSpotCutoffAngle );				/**< (float[]) Cutoff angle of each spotlight (degrees). */
	mapVarName( "u_cc3LightSpotCutoffAngleCosine", kCC3SemanticLightSpotCutoffAngleCosine );	/**< (float[]) Cosine of cutoff angle of each spotlight. */

	mapVarName( "u_cc3LightIsUsingLightProbes", kCC3SemanticIsUsingLightProbes );					/**< (bool) Whether the model is using light probes for lighting, instead of lights. */
	mapVarName( "u_cc3LightProbeCount", kCC3SemanticLightProbeCount );								/**< (int) Min of number of active light probes in the scene, and the number used by the shader program. */
	mapVarName( "u_cc3LightProbeIsEnabled", kCC3SemanticLightProbeIsEnabled );						/**< (bool[]) Whether each light probe is available and enabled. */
	mapVarName( "u_cc3LightProbeLocationGlobal", kCC3SemanticLightProbeLocationGlobal );			/**< (vec3[]) Location of each light probe in global coordinates. */
	mapVarName( "u_cc3LightProbeLocationEyeSpace", kCC3SemanticLightProbeLocationEyeSpace );		/**< (vec3[]) Location of each light probe in eye space. */
	mapVarName( "u_cc3LightProbeLocationModelSpace", kCC3SemanticLightProbeLocationModelSpace );	/**< (vec3[]) Location of each light probe in local coordinates of the model (not light probe). */
	mapVarName( "u_cc3LightProbeColorDiffuse", kCC3SemanticLightProbeColorDiffuse );				/**< (vec4) Diffuse color of each light probe. */
	
	mapVarName( "u_cc3FogIsEnabled", kCC3SemanticFogIsEnabled );				/**< (bool) Whether scene fogging is enabled. */
	mapVarName( "u_cc3FogColor", kCC3SemanticFogColor );						/**< (vec4) Fog color. */
	mapVarName( "u_cc3FogAttenuationMode", kCC3SemanticFogAttenuationMode );	/**< (int) Fog attenuation mode (one of GL_LINEAR, GL_EXP or GL_EXP2). */
	mapVarName( "u_cc3FogDensity", kCC3SemanticFogDensity );					/**< (float) Fog density. */
	mapVarName( "u_cc3FogStartDistance", kCC3SemanticFogStartDistance );		/**< (float) Distance from camera at which fogging effect starts. */
	mapVarName( "u_cc3FogEndDistance", kCC3SemanticFogEndDistance );			/**< (float) Distance from camera at which fogging effect ends. */

	// TEXTURES --------------
	mapVarName( "u_cc3TextureCount", kCC3SemanticTextureCount );			/**< (int) Number of active textures of any type. */
	mapVarName( "s_cc3Texture", kCC3SemanticTextureSampler );				/**< (sampler2D/sampler3D) Single texture sampler of any type. */
	mapVarName( "s_cc3Textures", kCC3SemanticTextureSampler );				/**< (sampler2D[]/sampler3D) Array of texture samplers of any single type. */

	mapVarName( "u_cc3Texture2DCount", kCC3SemanticTexture2DCount );		/**< (int) Number of active 2D textures on current model. */
	mapVarName( "s_cc3Texture2D", kCC3SemanticTexture2DSampler );			/**< (sampler2D) Single 2D texture sampler. */
	mapVarName( "s_cc3Texture2Ds", kCC3SemanticTexture2DSampler );			/**< (sampler2D[]) Array of 2D texture samplers. */

	mapVarName( "u_cc3TextureCubeCount", kCC3SemanticTextureCubeCount );	/**< (int) Number of active cube textures on current model. */
	mapVarName( "s_cc3TextureCube", kCC3SemanticTextureCubeSampler );		/**< (samplerCube) Single cube texture sampler. */
	mapVarName( "s_cc3TextureCubes", kCC3SemanticTextureCubeSampler );		/**< (samplerCube[]) Array of cube texture samplers. */
	
	mapVarName( "s_cc3LightProbeTexture", kCC3SemanticTextureLightProbeSampler );		/**< (samplerCube or sampler2D) Single light probe texture sampler. */
	mapVarName( "s_cc3LightProbeTextures", kCC3SemanticTextureLightProbeSampler );		/**< (samplerCube[] or sampler2D[]) Array of light probe texture samplers. */

	// The semantics below mimic OpenGL ES 1.1 configuration functionality for combining texture units.
	// In most shaders, these will be left unused in favor of customized the texture combining in GLSL code.
	mapVarName( "u_cc3TextureUnitColor", kCC3SemanticTexUnitConstantColor );						/**< (vec4[]) The constant color of each texture unit. */
	mapVarName( "u_cc3TextureUnitMode", kCC3SemanticTexUnitMode );									/**< (int[]) Environment mode of each texture unit. */
	mapVarName( "u_cc3TextureUnitCombineRGBFunction", kCC3SemanticTexUnitCombineRGBFunction );		/**< (int[]) RBG combiner function of each texture unit. */
	mapVarName( "u_cc3TextureUnitRGBSource0", kCC3SemanticTexUnitSource0RGB  );						/**< (int[]) The RGB of source 0 of each texture unit. */
	mapVarName( "u_cc3TextureUnitRGBSource1", kCC3SemanticTexUnitSource1RGB );						/**< (int[]) The RGB of source 1 of each texture unit. */
	mapVarName( "u_cc3TextureUnitRGBSource2", kCC3SemanticTexUnitSource2RGB );						/**< (int[]) The RGB of source 2 of each texture unit. */
	mapVarName( "u_cc3TextureUnitRGBOperand0", kCC3SemanticTexUnitOperand0RGB );					/**< (int[]) The RGB combining operand of source 0 of each texture unit. */
	mapVarName( "u_cc3TextureUnitRGBOperand1", kCC3SemanticTexUnitOperand1RGB );					/**< (int[]) The RGB combining operand of source 1 of each texture unit. */
	mapVarName( "u_cc3TextureUnitRGBOperand2", kCC3SemanticTexUnitOperand2RGB );					/**< (int[]) The RGB combining operand of source 2 of each texture unit. */
	mapVarName( "u_cc3TextureUnitCombineAlphaFunction", kCC3SemanticTexUnitCombineAlphaFunction );	/**< (int[]) Alpha combiner function of each texture unit. */
	mapVarName( "u_cc3TextureUnitAlphaSource0", kCC3SemanticTexUnitSource0Alpha );					/**< (int[]) The alpha of source 0 of each texture unit. */
	mapVarName( "u_cc3TextureUnitAlphaSource1", kCC3SemanticTexUnitSource1Alpha );					/**< (int[]) The alpha of source 1 of each texture unit. */
	mapVarName( "u_cc3TextureUnitAlphaSource2", kCC3SemanticTexUnitSource2Alpha );					/**< (int[]) The alpha of source 2 of each texture unit. */
	mapVarName( "u_cc3TextureUnitAlphaOperand0", kCC3SemanticTexUnitOperand0Alpha );				/**< (int[]) The alpha combining operand of source 0 of each texture unit. */
	mapVarName( "u_cc3TextureUnitAlphaOperand1", kCC3SemanticTexUnitOperand1Alpha );				/**< (int[]) The alpha combining operand of source 1 of each texture unit. */
	mapVarName( "u_cc3TextureUnitAlphaOperand2", kCC3SemanticTexUnitOperand2Alpha );				/**< (int[]) The alpha combining operand of source 2 of each texture unit. */
	
	// MODEL ----------------
	mapVarName( "u_cc3ModelCenterOfGeometry", kCC3SemanticCenterOfGeometry );		/**< (vec3) The center of geometry of the model in the model's local coordinates. */
	mapVarName( "u_cc3ModelBoundingRadius", kCC3SemanticBoundingRadius );			/**< (float) The radius of a sphere, located at the center of geometry, that encompasses all of the vertices, in the model's local coordinates. */
	mapVarName( "u_cc3ModelBoundingBoxMinimum", kCC3SemanticBoundingBoxMin );		/**< (vec3) The maximum corner of the model's bounding box in the model's local coordinates. */
	mapVarName( "u_cc3ModelBoundingBoxMaximum", kCC3SemanticBoundingBoxMax );		/**< (vec3) The dimensions of the model's bounding box in the model's local coordinates. */
	mapVarName( "u_cc3ModelBoundingBoxSize", kCC3SemanticBoundingBoxSize );		/**< (float) The radius of the model's bounding sphere in the model's local coordinates. */
	mapVarName( "u_cc3ModelAnimationFraction", kCC3SemanticAnimationFraction );	/**< (float) Fraction of the model's animation that has been viewed (range 0-1). */
	
	// PARTICLES ------------
	mapVarName( "u_cc3IsDrawingPoints", kCC3SemanticIsDrawingPoints );						/**< (bool) Whether the vertices are being drawn as points. */
	mapVarName( "u_cc3PointSize", kCC3SemanticPointSize );									/**< (float) Default size of points, if not specified per-vertex in a vertex attribute array. */
	mapVarName( "u_cc3PointSizeAttenuation", kCC3SemanticPointSizeAttenuation );			/**< (vec3) Point size distance attenuation coefficients. */
	mapVarName( "u_cc3PointMinimumSize", kCC3SemanticPointSizeMinimum );					/**< (float) Minimum size points will be allowed to shrink to. */
	mapVarName( "u_cc3PointMaximumSize", kCC3SemanticPointSizeMaximum );					/**< (float) Maximum size points will be allowed to grow to. */
	mapVarName( "u_cc3PointShouldDisplayAsSprites", kCC3SemanticPointSpritesIsEnabled );	/**< (bool) Whether points should be interpeted as textured sprites. */
	
	// TIME ------------------
	mapVarName( "u_cc3FrameTime", kCC3SemanticFrameTime );				/**< (float) The time in seconds since the last frame. */
	mapVarName( "u_cc3SceneTime", kCC3SemanticSceneTime );				/**< (vec2) The real time, in seconds, since the scene was opened, and the fractional part of that time (T, fmod(T, 1)). */
	mapVarName( "u_cc3SceneTimeSin", kCC3SemanticSceneTimeSine );		/**< (vec4) Sine of the scene time (sin(T), sin(T/2), sin(T/4), sin(T/8)). */
	mapVarName( "u_cc3SceneTimeCos", kCC3SemanticSceneTimeCosine );	/**< (vec4) Cosine of the scene time (cos(T), cos(T/2), cos(T/4), cos(T/8)). */
	mapVarName( "u_cc3SceneTimeTan", kCC3SemanticSceneTimeTangent );	/**< (vec4) Tangent of the scene time (tan(T), tan(T/2), tan(T/4), tan(T/8)). */

	// MISC ENVIRONMENT --------
	mapVarName( "u_cc3DrawCount", kCC3SemanticDrawCountCurrentFrame );		/**< (int) The number of draw calls so far in this frame. */
	mapVarName( "u_cc3Random", kCC3SemanticRandomNumber );					/**< (float) A random number between 0 and 1. */

	// DEPRECATED ------------------
	mapVarName( "u_cc3BonesPerVertex", kCC3SemanticVertexBoneCount );	/**< @deprecated Replaced with u_cc3VertexBoneCount. */
	mapVarName( "u_cc3BoneCount", kCC3SemanticBatchBoneCount );			/**< @deprecated Replaced with u_cc3BatchBoneCount. */
	mapVarName( "u_cc3BoneMatrixCount", kCC3SemanticBatchBoneCount );	/**< @deprecated Replaced with u_cc3BatchBoneCount. */
	mapVarName( "u_cc3AppTime", kCC3SemanticSceneTime );				/**< @deprecated Use u_cc3SceneTime instead. */
	mapVarName( "u_cc3AppTimeSine", kCC3SemanticSceneTimeSine );		/**< @deprecated Use u_cc3SceneTimeSin instead. */
	mapVarName( "u_cc3AppTimeCosine", kCC3SemanticSceneTimeCosine );	/**< @deprecated Use u_cc3SceneTimeCos instead. */
	mapVarName( "u_cc3AppTimeTangent", kCC3SemanticSceneTimeTangent );	/**< @deprecated Use u_cc3SceneTimeTan instead. */
}

void CC3ShaderSemanticsByVarName::populateWithStructuredVariableNameMappings()
{
	// VETEX ATTRIBUTES --------------
	mapVarName( "a_cc3Position", kCC3SemanticVertexLocation );				/**< Vertex location. */
	mapVarName( "a_cc3Normal", kCC3SemanticVertexNormal );					/**< Vertex normal. */
	mapVarName( "a_cc3Tangent", kCC3SemanticVertexTangent );				/**< Vertex tangent. */
	mapVarName( "a_cc3Bitangent", kCC3SemanticVertexBitangent );			/**< Vertex bitangent (aka binormal). */
	mapVarName( "a_cc3Color", kCC3SemanticVertexColor );					/**< Vertex color. */
	mapVarName( "a_cc3BoneWeights", kCC3SemanticVertexBoneWeights );		/**< Vertex skinning bone weights (each an array of length specified by u_cc3BonesPerVertex). */
	mapVarName( "a_cc3BoneIndices", kCC3SemanticVertexBoneIndices );		/**< Vertex skinning bone indices (each an array of length specified by u_cc3BonesPerVertex). */
	mapVarName( "a_cc3PointSize", kCC3SemanticVertexPointSize );			/**< Vertex point size. */
	
	// If only one texture coordinate attribute is used, the index suffix ("a_cc3TexCoordN") is optional.
	mapVarName( "a_cc3TexCoord", kCC3SemanticVertexTexture );				/**< Vertex texture coordinate for the first texture unit. */
	GLuint maxTexUnits = CC3OpenGL::sharedGL()->getMaxNumberOfTextureUnits();
	for (GLuint tuIdx = 0; tuIdx < maxTexUnits; tuIdx++)
		mapVarName( CC3String::stringWithFormat( (char*)"a_cc3TexCoord%u", tuIdx ), kCC3SemanticVertexTexture, tuIdx );	/**< Vertex texture coordinate for a texture unit. */
	
	// VERTEX STATE --------------
	mapVarName( "u_cc3Vertex.hasVertexNormal", kCC3SemanticHasVertexNormal );					/**< (bool) Whether a vertex normal is available. */
	mapVarName( "u_cc3Vertex.hasVertexTangent", kCC3SemanticHasVertexTangent );				/**< (bool) Whether a vertex tangent is available. */
	mapVarName( "u_cc3Vertex.hasVertexBitangent", kCC3SemanticHasVertexBitangent );			/**< (bool) Whether a vertex bitangent is available. */
	mapVarName( "u_cc3Vertex.hasVertexColor", kCC3SemanticHasVertexColor );					/**< (bool) Whether a vertex color is available. */
	mapVarName( "u_cc3Vertex.hasVertexWeight", kCC3SemanticHasVertexWeight );					/**< (bool) Whether a vertex weight is available. */
	mapVarName( "u_cc3Vertex.hasVertexMatrixIndex", kCC3SemanticHasVertexMatrixIndex );		/**< (bool) Whether a vertex matrix index is available. */
	mapVarName( "u_cc3Vertex.hasVertexTexCoord", kCC3SemanticHasVertexTextureCoordinate );	/**< (bool) Whether a vertex texture coordinate is available. */
	mapVarName( "u_cc3Vertex.hasVertexPointSize", kCC3SemanticHasVertexPointSize );			/**< (bool) Whether a vertex point size is available. */
	mapVarName( "u_cc3Vertex.isDrawingPoints", kCC3SemanticIsDrawingPoints );					/**< (bool) Whether the vertices are being drawn as points. */
	mapVarName( "u_cc3Vertex.shouldNormalizeNormal", kCC3SemanticShouldNormalizeVertexNormal );	/**< (bool) Whether vertex normals should be normalized. */
	mapVarName( "u_cc3Vertex.shouldRescaleNormal", kCC3SemanticShouldRescaleVertexNormal );	/**< (bool) Whether vertex normals should be rescaled. */
	
	// ENVIRONMENT MATRICES --------------
	mapVarName( "u_cc3Matrices.modelLocal", kCC3SemanticModelLocalMatrix );					/**< (mat4) Current model-to-parent matrix. */
	mapVarName( "u_cc3Matrices.modelLocalInv", kCC3SemanticModelLocalMatrixInv );				/**< (mat4) Inverse of current model-to-parent matrix. */
	mapVarName( "u_cc3Matrices.modelLocalInvTran", kCC3SemanticModelLocalMatrixInvTran );		/**< (mat3) Inverse-transpose of current model-to-parent matrix. */
	
	mapVarName( "u_cc3Matrices.model", kCC3SemanticModelMatrix );								/**< (mat4) Current model-to-world matrix. */
	mapVarName( "u_cc3Matrices.modelInv", kCC3SemanticModelMatrixInv );						/**< (mat4) Inverse of current model-to-world matrix. */
	mapVarName( "u_cc3Matrices.modelInvTran", kCC3SemanticModelMatrixInvTran );				/**< (mat3) Inverse-transpose of current model-to-world matrix. */
	
	mapVarName( "u_cc3Matrices.view", kCC3SemanticViewMatrix );								/**< (mat4) Camera view matrix. */
	mapVarName( "u_cc3Matrices.viewInv", kCC3SemanticViewMatrixInv );							/**< (mat4) Inverse of camera view matrix. */
	mapVarName( "u_cc3Matrices.viewInvTran", kCC3SemanticViewMatrixInvTran );					/**< (mat3) Inverse-transpose of camera view matrix. */
	
	mapVarName( "u_cc3Matrices.modelView", kCC3SemanticModelViewMatrix );						/**< (mat4) Current model-view matrix. */
	mapVarName( "u_cc3Matrices.modelViewInv", kCC3SemanticModelViewMatrixInv );				/**< (mat4) Inverse of current model-view matrix. */
	mapVarName( "u_cc3Matrices.modelViewInvTran", kCC3SemanticModelViewMatrixInvTran );		/**< (mat3) Inverse-transpose of current model-view matrix. */
	
	mapVarName( "u_cc3Matrices.proj", kCC3SemanticProjMatrix );								/**< (mat4) Camera projection matrix. */
	mapVarName( "u_cc3Matrices.projInv", kCC3SemanticProjMatrixInv );							/**< (mat4) Inverse of camera projection matrix. */
	mapVarName( "u_cc3Matrices.projInvTran", kCC3SemanticProjMatrixInvTran );					/**< (mat3) Inverse-transpose of camera projection matrix. */
	
	mapVarName( "u_cc3Matrices.viewProj", kCC3SemanticViewProjMatrix );						/**< (mat4) Camera view and projection matrix. */
	mapVarName( "u_cc3Matrices.viewProjInv", kCC3SemanticViewProjMatrixInv );					/**< (mat4) Inverse of camera view and projection matrix. */
	mapVarName( "u_cc3Matrices.viewProjInvTran", kCC3SemanticViewProjMatrixInvTran );			/**< (mat3) Inverse-transpose of camera view and projection matrix. */
	
	mapVarName( "u_cc3Matrices.modelViewProj", kCC3SemanticModelViewProjMatrix );				/**< (mat4) Current model-view-projection matrix. */
	mapVarName( "u_cc3Matrices.modelViewProjInv", kCC3SemanticModelViewProjMatrixInv );		/**< (mat4) Inverse of current model-view-projection matrix. */
	mapVarName( "u_cc3Matrices.modelViewProjInvTran", kCC3SemanticModelViewProjMatrixInvTran );/**< (mat3) Inverse-transpose of current model-view-projection matrix. */
	
	// SKINNING ----------------
	mapVarName( "u_cc3Bones.bonesPerVertex", kCC3SemanticVertexBoneCount );							/**< (int) Number of bones influencing each vertex (ie- number of weights/matrices specified on each vertex) */
	mapVarName( "u_cc3Bones.matrixCount", kCC3SemanticBatchBoneCount );							/**< (int) Number of matrices in the matrix arrays in this structure. */
	mapVarName( "u_cc3Bones.matricesEyeSpace", kCC3SemanticBoneMatricesEyeSpace );					/**< (mat4[]) Array of bone matrices in the current mesh skin section in eye space. */
	mapVarName( "u_cc3Bones.matricesInvTranEyeSpace", kCC3SemanticBoneMatricesInvTranEyeSpace );	/**< (mat3[]) Array of inverse-transposes of the bone matrices in the current mesh skin section in eye space. */
	mapVarName( "u_cc3Bones.matricesGlobal", kCC3SemanticBoneMatricesGlobal );						/**< (mat4[]) Array of bone matrices in the current mesh skin section in global coordinates. */
	mapVarName( "u_cc3Bones.matricesInvTranGlobal", kCC3SemanticBoneMatricesInvTranGlobal );		/**< (mat3[]) Array of inverse-transposes of the bone matrices in the current mesh skin section in global coordinates. */
	
	// CAMERA -----------------
	mapVarName( "u_cc3Camera.positionGlobal", kCC3SemanticCameraLocationGlobal );		/**< (vec3) Location of the camera in global coordinates. */
	mapVarName( "u_cc3Camera.positionModel", kCC3SemanticCameraLocationModelSpace );	/**< (vec3) Location of the camera in local coordinates of model (not camera). */
	mapVarName( "u_cc3Camera.frustum", kCC3SemanticCameraFrustum );					/**< (vec4) Dimensions of the camera frustum (FOV width (radians), FOV height (radians), near clip, far clip). */
	mapVarName( "u_cc3Camera.viewport", kCC3SemanticViewport );						/**< (vec4 or ivec4) The viewport rectangle in pixels (x, y, width, height). */
	
	// MATERIALS --------------
	mapVarName( "u_cc3Color", kCC3SemanticColor );										/**< (vec4) Color when lighting & materials are not in use. */
	mapVarName( "u_cc3Material.ambientColor", kCC3SemanticMaterialColorAmbient );		/**< (vec4) Ambient color of the material. */
	mapVarName( "u_cc3Material.diffuseColor", kCC3SemanticMaterialColorDiffuse );		/**< (vec4) Diffuse color of the material. */
	mapVarName( "u_cc3Material.specularColor", kCC3SemanticMaterialColorSpecular );	/**< (vec4) Specular color of the material. */
	mapVarName( "u_cc3Material.emissionColor", kCC3SemanticMaterialColorEmission );	/**< (vec4) Emission color of the material. */
	mapVarName( "u_cc3Material.opacity", kCC3SemanticMaterialOpacity );				/**< (float) Opacity of the material (0.0 - 1.0). */
	mapVarName( "u_cc3Material.shininess", kCC3SemanticMaterialShininess );			/**< (float) Shininess of the material. */
	mapVarName( "u_cc3Material.minimumDrawnAlpha", kCC3SemanticMinimumDrawnAlpha );	/**< (float) Minimum alpha value to be drawn, otherwise will be discarded. */
	
	// LIGHTING --------------
	// With multiple lights, most of the structure elements is an array.
	mapVarName( "u_cc3Lighting.isUsingLighting", kCC3SemanticIsUsingLighting );					/**< (bool) Whether any lighting is enabled. */
	mapVarName( "u_cc3Lighting.sceneAmbientLightColor", kCC3SemanticSceneLightColorAmbient );		/**< (vec4) Ambient light color of the scene. */
	mapVarName( "u_cc3Lighting.isLightEnabled", kCC3SemanticLightIsEnabled );						/**< (bool[]) Whether each light is enabled. */
	mapVarName( "u_cc3Lighting.positionEyeSpace", kCC3SemanticLightPositionEyeSpace );				/**< (vec4[]) Location of each light in eye space. */
	mapVarName( "u_cc3Lighting.positionGlobal", kCC3SemanticLightPositionGlobal );					/**< (vec4[]) Location of each light in global coordinates. */
	mapVarName( "u_cc3Lighting.positionModel", kCC3SemanticLightPositionModelSpace );				/**< (vec4[]) Location of each light in local coordinates of model (not light). */
	mapVarName( "u_cc3Lighting.ambientColor", kCC3SemanticLightColorAmbient );						/**< (vec4[]) Ambient color of each light. */
	mapVarName( "u_cc3Lighting.diffuseColor", kCC3SemanticLightColorDiffuse );						/**< (vec4[]) Diffuse color of each light. */
	mapVarName( "u_cc3Lighting.specularColor", kCC3SemanticLightColorSpecular );					/**< (vec4[]) Specular color of each light. */
	mapVarName( "u_cc3Lighting.attenuation", kCC3SemanticLightAttenuation );						/**< (vec3[]) Distance attenuation coefficients for each light. */
	mapVarName( "u_cc3Lighting.spotDirectionEyeSpace", kCC3SemanticLightSpotDirectionEyeSpace );	/**< (vec3[]) Direction of each spotlight in eye space. */
	mapVarName( "u_cc3Lighting.spotDirectionGlobal", kCC3SemanticLightSpotDirectionGlobal );		/**< (vec3[]) Direction of each spotlight in global coordinates. */
	mapVarName( "u_cc3Lighting.spotDirectionModel", kCC3SemanticLightSpotDirectionModelSpace );	/**< (vec3[]) Direction of each spotlight in local coordinates of the model (not light). */
	mapVarName( "u_cc3Lighting.spotExponent", kCC3SemanticLightSpotExponent );						/**< (float[]) Fade-off exponent of each spotlight. */
	mapVarName( "u_cc3Lighting.spotCutoffAngle", kCC3SemanticLightSpotCutoffAngle );				/**< (float[]) Cutoff angle of each spotlight (degrees). */
	mapVarName( "u_cc3Lighting.spotCutoffAngleCosine", kCC3SemanticLightSpotCutoffAngleCosine );	/**< (float[]) Cosine of cutoff angle of each spotlight. */
	
	mapVarName( "u_cc3Fog.isEnabled", kCC3SemanticFogIsEnabled );				/**< (bool) Whether scene fogging is enabled. */
	mapVarName( "u_cc3Fog.color", kCC3SemanticFogColor );						/**< (vec4) Fog color. */
	mapVarName( "u_cc3Fog.attenuationMode", kCC3SemanticFogAttenuationMode );	/**< (int) Fog attenuation mode (one of GL_LINEAR, GL_EXP or GL_EXP2). */
	mapVarName( "u_cc3Fog.density", kCC3SemanticFogDensity );					/**< (float) Fog density. */
	mapVarName( "u_cc3Fog.startDistance", kCC3SemanticFogStartDistance );		/**< (float) Distance from camera at which fogging effect starts. */
	mapVarName( "u_cc3Fog.endDistance", kCC3SemanticFogEndDistance );			/**< (float) Distance from camera at which fogging effect ends. */
	
	// TEXTURES --------------
	mapVarName( "u_cc3TextureCount", kCC3SemanticTextureCount );	/**< (int) Number of active textures. */
	mapVarName( "s_cc3Texture", kCC3SemanticTextureSampler );		/**< (sampler2D) Single texture sampler (texture unit 0). */
	mapVarName( "s_cc3Textures", kCC3SemanticTextureSampler );		/**< (sampler2D[]) Array of texture samplers. */
	
	// The semantics below mimic OpenGL ES 1.1 configuration functionality for combining texture units.
	// In most shaders, these will be left unused in favor of customized the texture combining in GLSL code.
	mapVarName( "u_cc3TextureUnits.color", kCC3SemanticTexUnitConstantColor );							/**< (vec4[]) The constant color of each texture unit. */
	mapVarName( "u_cc3TextureUnits.mode", kCC3SemanticTexUnitMode );									/**< (int[]) Environment mode of each texture unit. */
	mapVarName( "u_cc3TextureUnits.combineRGBFunction", kCC3SemanticTexUnitCombineRGBFunction );		/**< (int[]) RBG combiner function of each texture unit. */
	mapVarName( "u_cc3TextureUnits.rgbSource0", kCC3SemanticTexUnitSource0RGB );						/**< (int[]) The RGB of source 0 of each texture unit. */
	mapVarName( "u_cc3TextureUnits.rgbSource1", kCC3SemanticTexUnitSource1RGB );						/**< (int[]) The RGB of source 1 of each texture unit. */
	mapVarName( "u_cc3TextureUnits.rgbSource2", kCC3SemanticTexUnitSource2RGB );						/**< (int[]) The RGB of source 2 of each texture unit. */
	mapVarName( "u_cc3TextureUnits.rgbOperand0", kCC3SemanticTexUnitOperand0RGB );						/**< (int[]) The RGB combining operand of source 0 of each texture unit. */
	mapVarName( "u_cc3TextureUnits.rgbOperand1", kCC3SemanticTexUnitOperand1RGB );						/**< (int[]) The RGB combining operand of source 1 of each texture unit. */
	mapVarName( "u_cc3TextureUnits.rgbOperand2", kCC3SemanticTexUnitOperand2RGB );						/**< (int[]) The RGB combining operand of source 2 of each texture unit. */
	mapVarName( "u_cc3TextureUnits.combineAlphaFunction", kCC3SemanticTexUnitCombineAlphaFunction );	/**< (int[]) Alpha combiner function of each texture unit. */
	mapVarName( "u_cc3TextureUnits.alphaSource0", kCC3SemanticTexUnitSource0Alpha );					/**< (int[]) The alpha of source 0 of each texture unit. */
	mapVarName( "u_cc3TextureUnits.alphaSource1", kCC3SemanticTexUnitSource1Alpha );					/**< (int[]) The alpha of source 1 of each texture unit. */
	mapVarName( "u_cc3TextureUnits.alphaSource2", kCC3SemanticTexUnitSource2Alpha );					/**< (int[]) The alpha of source 2 of each texture unit. */
	mapVarName( "u_cc3TextureUnits.alphaOperand0", kCC3SemanticTexUnitOperand0Alpha );					/**< (int[]) The alpha combining operand of source 0 of each texture unit. */
	mapVarName( "u_cc3TextureUnits.alphaOperand1", kCC3SemanticTexUnitOperand1Alpha );					/**< (int[]) The alpha combining operand of source 1 of each texture unit. */
	mapVarName( "u_cc3TextureUnits.alphaOperand2", kCC3SemanticTexUnitOperand2Alpha );					/**< (int[]) The alpha combining operand of source 2 of each texture unit. */
	
	// MODEL ----------------
	mapVarName( "u_cc3Model.centerOfGeometry", kCC3SemanticCenterOfGeometry );		/**< (vec3) The center of geometry of the model in the model's local coordinates. */
	mapVarName( "u_cc3Model.boundingRadius", kCC3SemanticBoundingRadius );			/**< (vec3) The minimum corner of the model's bounding box in the model's local coordinates. */
	mapVarName( "u_cc3Model.boundingBoxMinimum", kCC3SemanticBoundingBoxMin );		/**< (vec3) The maximum corner of the model's bounding box in the model's local coordinates. */
	mapVarName( "u_cc3Model.boundingBoxMaximum", kCC3SemanticBoundingBoxMax );		/**< (vec3) The dimensions of the model's bounding box in the model's local coordinates. */
	mapVarName( "u_cc3Model.boundingBoxSize", kCC3SemanticBoundingBoxSize );		/**< (float) The radius of the model's bounding sphere in the model's local coordinates. */
	mapVarName( "u_cc3Model.animationFraction", kCC3SemanticAnimationFraction );	/**< (float) Fraction of the model's animation that has been viewed (range 0-1). */
	
	// PARTICLES ------------
	mapVarName( "u_cc3Points.isDrawingPoints", kCC3SemanticIsDrawingPoints );				/**< (bool) Whether the vertices are being drawn as points (alias for u_cc3IsDrawingPoints). */
	mapVarName( "u_cc3Points.hasVertexPointSize", kCC3SemanticHasVertexPointSize );		/**< (bool) Whether the vertex point size is available (alias for u_cc3HasVertexPointSize). */
	mapVarName( "u_cc3Points.size", kCC3SemanticPointSize );								/**< (float) Default size of points, if not specified per-vertex in a vertex attribute array. */
	mapVarName( "u_cc3Points.sizeAttenuation", kCC3SemanticPointSizeAttenuation );			/**< (vec3) Point size distance attenuation coefficients. */
	mapVarName( "u_cc3Points.minimumSize", kCC3SemanticPointSizeMinimum );					/**< (float) Minimum size points will be allowed to shrink to. */
	mapVarName( "u_cc3Points.maximumSize", kCC3SemanticPointSizeMaximum );					/**< (float) Maximum size points will be allowed to grow to. */
	mapVarName( "u_cc3Points.shouldDisplayAsSprites", kCC3SemanticPointSpritesIsEnabled );	/**< (bool) Whether points should be interpeted as textured sprites. */
	
	// TIME ------------------
	mapVarName( "u_cc3Time.appTime", kCC3SemanticSceneTime );					/**< @deprecated Use sceneTime instead. */
	mapVarName( "u_cc3Time.appTimeSine", kCC3SemanticSceneTimeSine );			/**< @deprecated Use sceneTimeSin instead. */
	mapVarName( "u_cc3Time.appTimeCosine", kCC3SemanticSceneTimeCosine );		/**< @deprecated Use sceneTimeCos instead. */
	mapVarName( "u_cc3Time.appTimeTangent", kCC3SemanticSceneTimeTangent );	/**< @deprecated Use sceneTimeTan instead. */
	
	// MISC ENVIRONMENT ---------
	mapVarName( "u_cc3DrawCount", kCC3SemanticDrawCountCurrentFrame );		/**< (int) The number of draw calls so far in this frame. */
	mapVarName( "u_cc3Random", kCC3SemanticRandomNumber );					/**< (float) A random number between 0 and 1. */
	
}

void CC3ShaderSemanticsByVarName::populateWithLegacyVariableNameMappings()
{	
	// VETEX ATTRIBUTES --------------
	mapVarName( "a_cc3Position", kCC3SemanticVertexLocation );				/**< Vertex location. */
	mapVarName( "a_cc3Normal", kCC3SemanticVertexNormal );					/**< Vertex normal. */
	mapVarName( "a_cc3Tangent", kCC3SemanticVertexTangent );				/**< Vertex tangent. */
	mapVarName( "a_cc3Bitangent", kCC3SemanticVertexBitangent );			/**< Vertex bitangent (aka binormal). */
	mapVarName( "a_cc3Color", kCC3SemanticVertexColor );					/**< Vertex color. */
	mapVarName( "a_cc3BoneWeights", kCC3SemanticVertexBoneWeights );		/**< Vertex skinning bone weights (each an array of length specified by u_cc3BonesPerVertex). */
	mapVarName( "a_cc3BoneIndices", kCC3SemanticVertexBoneIndices );		/**< Vertex skinning bone indices (each an array of length specified by u_cc3BonesPerVertex). */
	mapVarName( "a_cc3PointSize", kCC3SemanticVertexPointSize );			/**< Vertex point size. */
	
	// If only one texture coordinate attribute is used, the index suffix ("a_cc3TexCoordN") is optional.
	mapVarName( "a_cc3TexCoord", kCC3SemanticVertexTexture );				/**< Vertex texture coordinate for the first texture unit. */
	GLuint maxTexUnits = CC3OpenGL::sharedGL()->getMaxNumberOfTextureUnits();
	for (GLuint tuIdx = 0; tuIdx < maxTexUnits; tuIdx++)
		mapVarName( CC3String::stringWithFormat((char*)"a_cc3TexCoord%u", tuIdx), kCC3SemanticVertexTexture, tuIdx );	/**< Vertex texture coordinate for a texture unit. */
	
	// ATTRIBUTE QUALIFIERS --------------
	mapVarName( "u_cc3HasVertexNormal", kCC3SemanticHasVertexNormal );					/**< (bool) Whether a vertex normal is available. */
	mapVarName( "u_cc3ShouldNormalizeNormal", kCC3SemanticShouldNormalizeVertexNormal );	/**< (bool) Whether vertex normals should be normalized. */
	mapVarName( "u_cc3ShouldRescaleNormal", kCC3SemanticShouldRescaleVertexNormal );	/**< (bool) Whether vertex normals should be rescaled. */
	mapVarName( "u_cc3HasVertexTangent", kCC3SemanticHasVertexTangent );				/**< (bool) Whether a vertex tangent is available. */
	mapVarName( "u_cc3HasVertexBitangent", kCC3SemanticHasVertexBitangent );			/**< (bool) Whether a vertex bitangent is available. */
	mapVarName( "u_cc3HasVertexColor", kCC3SemanticHasVertexColor );					/**< (bool) Whether a vertex color is available. */
	mapVarName( "u_cc3HasVertexWeight", kCC3SemanticHasVertexWeight );					/**< (bool) Whether a vertex weight is available. */
	mapVarName( "u_cc3HasVertexMatrixIndex", kCC3SemanticHasVertexMatrixIndex );		/**< (bool) Whether a vertex matrix index is available. */
	mapVarName( "u_cc3HasVertexTexCoord", kCC3SemanticHasVertexTextureCoordinate );	/**< (bool) Whether a vertex texture coordinate is available. */
	mapVarName( "u_cc3HasVertexPointSize", kCC3SemanticHasVertexPointSize );			/**< (bool) Whether a vertex point size is available. */
	mapVarName( "u_cc3IsDrawingPoints", kCC3SemanticIsDrawingPoints );					/**< (bool) Whether the vertices are being drawn as points. */
	
	// ENVIRONMENT MATRICES --------------
	mapVarName( "u_cc3MtxModelLocal", kCC3SemanticModelLocalMatrix );					/**< (mat4) Current model-to-parent matrix. */
	mapVarName( "u_cc3MtxModelLocalInv", kCC3SemanticModelLocalMatrixInv );			/**< (mat4) Inverse of current model-to-parent matrix. */
	mapVarName( "u_cc3MtxModelLocalInvTran", kCC3SemanticModelLocalMatrixInvTran );	/**< (mat3) Inverse-transpose of current model-to-parent matrix. */
	
	mapVarName( "u_cc3MtxModel", kCC3SemanticModelMatrix );							/**< (mat4) Current model-to-world matrix. */
	mapVarName( "u_cc3MtxModelInv", kCC3SemanticModelMatrixInv );						/**< (mat4) Inverse of current model-to-world matrix. */
	mapVarName( "u_cc3MtxModelInvTran", kCC3SemanticModelMatrixInvTran );				/**< (mat3) Inverse-transpose of current model-to-world matrix. */
	
	mapVarName( "u_cc3MtxView", kCC3SemanticViewMatrix );								/**< (mat4) Camera view matrix. */
	mapVarName( "u_cc3MtxViewInv", kCC3SemanticViewMatrixInv );						/**< (mat4) Inverse of camera view matrix. */
	mapVarName( "u_cc3MtxViewInvTran", kCC3SemanticViewMatrixInvTran );				/**< (mat3) Inverse-transpose of camera view matrix. */
	
	mapVarName( "u_cc3MtxModelView", kCC3SemanticModelViewMatrix );					/**< (mat4) Current model-view matrix. */
	mapVarName( "u_cc3MtxModelViewInv", kCC3SemanticModelViewMatrixInv );				/**< (mat4) Inverse of current model-view matrix. */
	mapVarName( "u_cc3MtxModelViewInvTran", kCC3SemanticModelViewMatrixInvTran );		/**< (mat3) Inverse-transpose of current model-view matrix. */
	
	mapVarName( "u_cc3MtxProj", kCC3SemanticProjMatrix );								/**< (mat4) Camera projection matrix. */
	mapVarName( "u_cc3MtxProjInv", kCC3SemanticProjMatrixInv );						/**< (mat4) Inverse of camera projection matrix. */
	mapVarName( "u_cc3MtxProjInvTran", kCC3SemanticProjMatrixInvTran );				/**< (mat3) Inverse-transpose of camera projection matrix. */
	
	mapVarName( "u_cc3MtxViewProj", kCC3SemanticViewProjMatrix );						/**< (mat4) Camera view and projection matrix. */
	mapVarName( "u_cc3MtxViewProjInv", kCC3SemanticViewProjMatrixInv );				/**< (mat4) Inverse of camera view and projection matrix. */
	mapVarName( "u_cc3MtxViewProjInvTran", kCC3SemanticViewProjMatrixInvTran );		/**< (mat3) Inverse-transpose of camera view and projection matrix. */
	
	mapVarName( "u_cc3MtxModelViewProj", kCC3SemanticModelViewProjMatrix );			/**< (mat4) Current model-view-projection matrix. */
	mapVarName( "u_cc3MtxModelViewProjInv", kCC3SemanticModelViewProjMatrixInv );		/**< (mat4) Inverse of current model-view-projection matrix. */
	mapVarName( "u_cc3MtxModelViewProjInvTran", kCC3SemanticModelViewProjMatrixInvTran );	/**< (mat3) Inverse-transpose of current model-view-projection matrix. */
	
	// CAMERA -----------------
	mapVarName( "u_cc3Camera.positionGlobal", kCC3SemanticCameraLocationGlobal );		/**< (vec3) Location of the camera in global coordinates. */
	mapVarName( "u_cc3Camera.positionModel", kCC3SemanticCameraLocationModelSpace );	/**< (vec3) Location of the camera in local coordinates of model (not camera). */
	mapVarName( "u_cc3Camera.frustum", kCC3SemanticCameraFrustum );					/**< (vec4) Dimensions of the camera frustum (FOV width (radians), FOV height (radians), near clip, far clip). */
	mapVarName( "u_cc3Camera.viewport", kCC3SemanticViewport );						/**< (vec4 or ivec4) The viewport rectangle in pixels (x, y, width, height). */
	
	// MATERIALS --------------
	mapVarName( "u_cc3Color", kCC3SemanticColor );										/**< (vec4) Color when lighting & materials are not in use. */
	mapVarName( "u_cc3Material.ambientColor", kCC3SemanticMaterialColorAmbient );		/**< (vec4) Ambient color of the material. */
	mapVarName( "u_cc3Material.diffuseColor", kCC3SemanticMaterialColorDiffuse );		/**< (vec4) Diffuse color of the material. */
	mapVarName( "u_cc3Material.specularColor", kCC3SemanticMaterialColorSpecular );	/**< (vec4) Specular color of the material. */
	mapVarName( "u_cc3Material.emissionColor", kCC3SemanticMaterialColorEmission );	/**< (vec4) Emission color of the material. */
	mapVarName( "u_cc3Material.opacity", kCC3SemanticMaterialOpacity );				/**< (float) Opacity of the material (0.0 - 1.0). */
	mapVarName( "u_cc3Material.shininess", kCC3SemanticMaterialShininess );			/**< (float) Shininess of the material. */
	mapVarName( "u_cc3Material.minimumDrawnAlpha", kCC3SemanticMinimumDrawnAlpha );	/**< (float) Minimum alpha value to be drawn, otherwise will be discarded. */
	
	// LIGHTING --------------
	mapVarName( "u_cc3IsUsingLighting", kCC3SemanticIsUsingLighting );					/**< (bool) Whether any lighting is enabled. */
	mapVarName( "u_cc3SceneLightColorAmbient", kCC3SemanticSceneLightColorAmbient );	/**< (vec4) Ambient light color of the scene. */
	
	// If only one light is used it can be declared as a single variable structure without the index.
	mapVarName( "u_cc3Light.isEnabled", kCC3SemanticLightIsEnabled );					/**< (bool) Whether the first light is enabled. */
	mapVarName( "u_cc3Light.positionEyeSpace", kCC3SemanticLightPositionEyeSpace );	/**< (vec4) Location of the first light in eye space. */
	mapVarName( "u_cc3Light.positionGlobal", kCC3SemanticLightPositionGlobal );		/**< (vec4) Location of the first light in global coordinates. */
	mapVarName( "u_cc3Light.positionModel", kCC3SemanticLightPositionModelSpace );		/**< (vec4) Location of the first light in local coordinates of model (not light). */
	mapVarName( "u_cc3Light.ambientColor", kCC3SemanticLightColorAmbient );			/**< (vec4) Ambient color of the first light. */
	mapVarName( "u_cc3Light.diffuseColor", kCC3SemanticLightColorDiffuse );			/**< (vec4) Diffuse color of the first light. */
	mapVarName( "u_cc3Light.specularColor", kCC3SemanticLightColorSpecular );			/**< (vec4) Specular color of the first light. */
	mapVarName( "u_cc3Light.attenuation", kCC3SemanticLightAttenuation );				/**< (vec3) Distance attenuation coefficients for the first light. */
	mapVarName( "u_cc3Light.spotDirectionEyeSpace", kCC3SemanticLightSpotDirectionEyeSpace );	/**< (vec3) Direction of the first spotlight in eye space. */
	mapVarName( "u_cc3Light.spotDirectionGlobal", kCC3SemanticLightSpotDirectionGlobal );		/**< (vec3) Direction of the first spotlight in global coordinates. */
	mapVarName( "u_cc3Light.spotDirectionModel", kCC3SemanticLightSpotDirectionModelSpace );	/**< (vec3) Direction of the first spotlight in local coordinates of the model (not light). */
	mapVarName( "u_cc3Light.spotExponent", kCC3SemanticLightSpotExponent );					/**< (float) Fade-off exponent of the first spotlight. */
	mapVarName( "u_cc3Light.spotCutoffAngle", kCC3SemanticLightSpotCutoffAngle );				/**< (float) Cutoff angle of the first spotlight (degrees). */
	mapVarName( "u_cc3Light.spotCutoffAngleCosine", kCC3SemanticLightSpotCutoffAngleCosine );	/**< (float) Cosine of cutoff angle of the first spotlight. */
	
	// Multiple lights are indexed
	for (GLuint ltIdx = 0; ltIdx < 4; ltIdx++) 
	{
		mapVarName( CC3String::stringWithFormat( (char*)"u_cc3Lights[%d].isEnabled", ltIdx ), kCC3SemanticLightIsEnabled, ltIdx );						/**< (bool) Whether a light is enabled. */
		mapVarName( CC3String::stringWithFormat( (char*)"u_cc3Lights[%d].positionEyeSpace", ltIdx ), kCC3SemanticLightPositionEyeSpace, ltIdx );			/**< (vec4) Homogeneous position (location or direction) of a light in eye space. */
		mapVarName( CC3String::stringWithFormat( (char*)"u_cc3Lights[%d].positionGlobal", ltIdx ), kCC3SemanticLightPositionGlobal, ltIdx );				/**< (vec4) Homogeneous position (location or direction) of a light in global coordinates. */
		mapVarName( CC3String::stringWithFormat( (char*)"u_cc3Lights[%d].positionModel", ltIdx ), kCC3SemanticLightPositionModelSpace, ltIdx );			/**< (vec4) Homogeneous position (location or direction) of a light in local coordinates of model (not light). */
		mapVarName( CC3String::stringWithFormat( (char*)"u_cc3Lights[%d].ambientColor", ltIdx ), kCC3SemanticLightColorAmbient, ltIdx );					/**< (vec4) Ambient color of a light. */
		mapVarName( CC3String::stringWithFormat( (char*)"u_cc3Lights[%d].diffuseColor", ltIdx ), kCC3SemanticLightColorDiffuse, ltIdx );					/**< (vec4) Diffuse color of a light. */
		mapVarName( CC3String::stringWithFormat( (char*)"u_cc3Lights[%d].specularColor", ltIdx ), kCC3SemanticLightColorSpecular, ltIdx );				/**< (vec4) Specular color of a light. */
		mapVarName( CC3String::stringWithFormat( (char*)"u_cc3Lights[%d].attenuation", ltIdx ), kCC3SemanticLightAttenuation, ltIdx );					/**< (vec3) Distance attenuation coefficients for a light. */
		mapVarName( CC3String::stringWithFormat( (char*)"u_cc3Lights[%d].spotDirectionEyeSpace", ltIdx ), kCC3SemanticLightSpotDirectionEyeSpace, ltIdx );	/**< (vec3) Direction of a spotlight in eye space. */
		mapVarName( CC3String::stringWithFormat( (char*)"u_cc3Lights[%d].spotDirectionGlobal", ltIdx ), kCC3SemanticLightSpotDirectionGlobal, ltIdx );		/**< (vec3) Direction of a spotlight in global coordinates. */
		mapVarName( CC3String::stringWithFormat( (char*)"u_cc3Lights[%d].spotDirectionModel", ltIdx ), kCC3SemanticLightSpotDirectionModelSpace, ltIdx );	/**< (vec3) Direction of a spotlight in local coordinates of the model (not light). */
		mapVarName( CC3String::stringWithFormat( (char*)"u_cc3Lights[%d].spotExponent", ltIdx ), kCC3SemanticLightSpotExponent, ltIdx );						/**< (float) Fade-off exponent of a spotlight. */
		mapVarName( CC3String::stringWithFormat( (char*)"u_cc3Lights[%d].spotCutoffAngle", ltIdx ), kCC3SemanticLightSpotCutoffAngle, ltIdx );				/**< (float) Cutoff angle of a spotlight (degrees). */
		mapVarName( CC3String::stringWithFormat( (char*)"u_cc3Lights[%d].spotCutoffAngleCosine", ltIdx ), kCC3SemanticLightSpotCutoffAngleCosine, ltIdx );	/**< (float) Cosine of cutoff angle of a spotlight. */
	}
	
	mapVarName( "u_cc3Fog.isEnabled", kCC3SemanticFogIsEnabled );				/**< (bool) Whether scene fogging is enabled. */
	mapVarName( "u_cc3Fog.color", kCC3SemanticFogColor );						/**< (vec4) Fog color. */
	mapVarName( "u_cc3Fog.attenuationMode", kCC3SemanticFogAttenuationMode );	/**< (int) Fog attenuation mode (one of GL_LINEAR, GL_EXP or GL_EXP2). */
	mapVarName( "u_cc3Fog.density", kCC3SemanticFogDensity );					/**< (float) Fog density. */
	mapVarName( "u_cc3Fog.startDistance", kCC3SemanticFogStartDistance );		/**< (float) Distance from camera at which fogging effect starts. */
	mapVarName( "u_cc3Fog.endDistance", kCC3SemanticFogEndDistance );			/**< (float) Distance from camera at which fogging effect ends. */
	
	// TEXTURES --------------
	mapVarName( "u_cc3TextureCount", kCC3SemanticTextureCount );	/**< (int) Number of active textures. */
	mapVarName( "s_cc3Texture", kCC3SemanticTextureSampler );		/**< (sampler2D) Texture sampler. */
	mapVarName( "s_cc3Textures", kCC3SemanticTextureSampler );		/**< (sampler2D[]) Array of texture samplers. */
	
	// The semantics below mimic OpenGL ES 1.1 configuration functionality for combining texture units.
	// In most shaders, these will be left unused in favor of customized the texture combining in code.
	for (GLuint tuIdx = 0; tuIdx < 4; tuIdx++) 
	{
		mapVarName( CC3String::stringWithFormat( (char*)"u_cc3TextureUnits[%d].color", tuIdx), kCC3SemanticTexUnitConstantColor, tuIdx );						/**< (vec4) The constant color of a texture unit. */
		mapVarName( CC3String::stringWithFormat( (char*)"u_cc3TextureUnits[%d].mode", tuIdx), kCC3SemanticTexUnitMode, tuIdx );									/**< (int) Environment mode of a texture unit. */
		mapVarName( CC3String::stringWithFormat( (char*)"u_cc3TextureUnits[%d].combineRGBFunction", tuIdx), kCC3SemanticTexUnitCombineRGBFunction, tuIdx );		/**< (int) RBG combiner function of a texture unit. */
		mapVarName( CC3String::stringWithFormat( (char*)"u_cc3TextureUnits[%d].rgbSource0", tuIdx), kCC3SemanticTexUnitSource0RGB, tuIdx );						/**< (int) The RGB of source 0 of a texture unit. */
		mapVarName( CC3String::stringWithFormat( (char*)"u_cc3TextureUnits[%d].rgbSource1", tuIdx), kCC3SemanticTexUnitSource1RGB, tuIdx );						/**< (int) The RGB of source 1 of a texture unit. */
		mapVarName( CC3String::stringWithFormat( (char*)"u_cc3TextureUnits[%d].rgbSource2", tuIdx), kCC3SemanticTexUnitSource2RGB, tuIdx );						/**< (int) The RGB of source 2 of a texture unit. */
		mapVarName( CC3String::stringWithFormat( (char*)"u_cc3TextureUnits[%d].rgbOperand0", tuIdx), kCC3SemanticTexUnitOperand0RGB, tuIdx );					/**< (int) The RGB combining operand of source 0 of a texture unit. */
		mapVarName( CC3String::stringWithFormat( (char*)"u_cc3TextureUnits[%d].rgbOperand1", tuIdx), kCC3SemanticTexUnitOperand1RGB, tuIdx );					/**< (int) The RGB combining operand of source 1 of a texture unit. */
		mapVarName( CC3String::stringWithFormat( (char*)"u_cc3TextureUnits[%d].rgbOperand2", tuIdx), kCC3SemanticTexUnitOperand2RGB, tuIdx );					/**< (int) The RGB combining operand of source 2 of a texture unit. */
		mapVarName( CC3String::stringWithFormat( (char*)"u_cc3TextureUnits[%d].combineAlphaFunction", tuIdx), kCC3SemanticTexUnitCombineAlphaFunction, tuIdx );	/**< (int) Alpha combiner function of a texture unit. */
		mapVarName( CC3String::stringWithFormat( (char*)"u_cc3TextureUnits[%d].alphaSource0", tuIdx), kCC3SemanticTexUnitSource0Alpha, tuIdx );					/**< (int) The alpha of source 0 of a texture unit. */
		mapVarName( CC3String::stringWithFormat( (char*)"u_cc3TextureUnits[%d].alphaSource1", tuIdx), kCC3SemanticTexUnitSource1Alpha, tuIdx );					/**< (int) The alpha of source 1 of a texture unit. */
		mapVarName( CC3String::stringWithFormat( (char*)"u_cc3TextureUnits[%d].alphaSource2", tuIdx), kCC3SemanticTexUnitSource2Alpha, tuIdx );					/**< (int) The alpha of source 2 of a texture unit. */
		mapVarName( CC3String::stringWithFormat( (char*)"u_cc3TextureUnits[%d].alphaOperand0", tuIdx), kCC3SemanticTexUnitOperand0Alpha, tuIdx );				/**< (int) The alpha combining operand of source 0 of a texture unit. */
		mapVarName( CC3String::stringWithFormat( (char*)"u_cc3TextureUnits[%d].alphaOperand1", tuIdx), kCC3SemanticTexUnitOperand1Alpha, tuIdx );				/**< (int) The alpha combining operand of source 1 of a texture unit. */
		mapVarName( CC3String::stringWithFormat( (char*)"u_cc3TextureUnits[%d].alphaOperand2", tuIdx), kCC3SemanticTexUnitOperand2Alpha, tuIdx );				/**< (int) The alpha combining operand of source 2 of a texture unit. */
	}
	
	// MODEL ----------------
	mapVarName( "u_cc3Model.centerOfGeometry", kCC3SemanticCenterOfGeometry );		/**< (vec3) The center of geometry of the model in the model's local coordinates. */
	mapVarName( "u_cc3Model.boundingRadius", kCC3SemanticBoundingRadius );			/**< (vec3) The minimum corner of the model's bounding box in the model's local coordinates. */
	mapVarName( "u_cc3Model.boundingBoxMinimum", kCC3SemanticBoundingBoxMin );		/**< (vec3) The maximum corner of the model's bounding box in the model's local coordinates. */
	mapVarName( "u_cc3Model.boundingBoxMaximum", kCC3SemanticBoundingBoxMax );		/**< (vec3) The dimensions of the model's bounding box in the model's local coordinates. */
	mapVarName( "u_cc3Model.boundingBoxSize", kCC3SemanticBoundingBoxSize );		/**< (float) The radius of the model's bounding sphere in the model's local coordinates. */
	mapVarName( "u_cc3Model.animationFraction", kCC3SemanticAnimationFraction );	/**< (float) Fraction of the model's animation that has been viewed (range 0-1). */
	
	// SKINNING ----------------
	mapVarName( "u_cc3BonesPerVertex", kCC3SemanticVertexBoneCount );							/**< (int) Number of bones influencing each vertex (ie- number of weights/matrices specified on each vertex) */
	mapVarName( "u_cc3BoneMatrixCount", kCC3SemanticBatchBoneCount );							/**< (int) Length of the u_cc3BoneMatricesEyeSpace and u_cc3BoneMatricesInvTranEyeSpace arrays. */
	mapVarName( "u_cc3BoneMatricesEyeSpace", kCC3SemanticBoneMatricesEyeSpace );				/**< (mat4[]) Array of bone matrices in the current mesh skin section in eye space. */
	mapVarName( "u_cc3BoneMatricesInvTranEyeSpace", kCC3SemanticBoneMatricesInvTranEyeSpace );	/**< (mat3[]) Array of inverse-transposes of the bone matrices in the current mesh skin section  in eye space. */
	mapVarName( "u_cc3BoneMatricesGlobal", kCC3SemanticBoneMatricesGlobal );					/**< (mat4[]) Array of bone matrices in the current mesh skin section in global coordinates. */
	mapVarName( "u_cc3BoneMatricesInvTranGlobal", kCC3SemanticBoneMatricesInvTranGlobal );		/**< (mat3[]) Array of inverse-transposes of the bone matrices in the current mesh skin section in global coordinates. */
	
	// PARTICLES ------------
	mapVarName( "u_cc3Points.isDrawingPoints", kCC3SemanticIsDrawingPoints );				/**< (bool) Whether the vertices are being drawn as points (alias for u_cc3IsDrawingPoints). */
	mapVarName( "u_cc3Points.hasVertexPointSize", kCC3SemanticHasVertexPointSize );		/**< (bool) Whether the vertex point size is available (alias for u_cc3HasVertexPointSize). */
	mapVarName( "u_cc3Points.size", kCC3SemanticPointSize );								/**< (float) Default size of points, if not specified per-vertex in a vertex attribute array. */
	mapVarName( "u_cc3Points.sizeAttenuation", kCC3SemanticPointSizeAttenuation );			/**< (vec3) Point size distance attenuation coefficients. */
	mapVarName( "u_cc3Points.minimumSize", kCC3SemanticPointSizeMinimum );					/**< (float) Minimum size points will be allowed to shrink to. */
	mapVarName( "u_cc3Points.maximumSize", kCC3SemanticPointSizeMaximum );					/**< (float) Maximum size points will be allowed to grow to. */
	mapVarName( "u_cc3Points.shouldDisplayAsSprites", kCC3SemanticPointSpritesIsEnabled );	/**< (bool) Whether points should be interpeted as textured sprites. */
	
	// TIME ------------------
	mapVarName( "u_cc3Time.frameTime", kCC3SemanticFrameTime );						/**< (float) The time in seconds since the last frame. */
	mapVarName( "u_cc3Time.appTime", kCC3SemanticSceneTime );					/**< (float) The application time in seconds. */
	mapVarName( "u_cc3Time.appTimeSine", kCC3SemanticSceneTimeSine );			/**< (vec4) The sine of the application time (sin(T), sin(T/2), sin(T/4), sin(T/8)). */
	mapVarName( "u_cc3Time.appTimeCosine", kCC3SemanticSceneTimeCosine );		/**< (vec4) The cosine of the application time (cos(T), cos(T/2), cos(T/4), cos(T/8)). */
	mapVarName( "u_cc3Time.appTimeTangent", kCC3SemanticSceneTimeTangent );		/**< (vec4) The tangent of the application time (tan(T), tan(T/2), tan(T/4), tan(T/8)). */
	
	// MISC ENVIRONMENT ---------
	mapVarName( "u_cc3DrawCount", kCC3SemanticDrawCountCurrentFrame );		/**< (int) The number of draw calls so far in this frame. */
	mapVarName( "u_cc3Random", kCC3SemanticRandomNumber );					/**< (float) A random number between 0 and 1. */
}

NS_COCOS3D_END
