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
#include "CC3ShaderMatcher.h"
#include "../Meshes/CC3VertexSkinning.h"

NS_COCOS3D_BEGIN

CC3ShaderMatcher::CC3ShaderMatcher()
{

}

CC3ShaderMatcher::~CC3ShaderMatcher()
{

}

bool CC3ShaderMatcher::init()
{
	return true;
}

CC3ShaderProgram* CC3ShaderMatcher::getProgramForMeshNode( CC3MeshNode* aMeshNode )
{
	return NULL;
}
	
CC3ShaderProgram* CC3ShaderMatcher::getPureColorProgramMatching( CC3ShaderProgram* shaderProgram )
{
	return NULL;
}

CC3ShaderSemanticsDelegate* CC3ShaderMatcher::getSemanticDelegate()
{
	return NULL;
}

void CC3ShaderMatcher::setSemanticDelegate( CC3ShaderSemanticsDelegate* pDelegate )
{
	
}

CC3ShaderMatcherBase::CC3ShaderMatcherBase()
{
	_semanticDelegate = NULL;
}

CC3ShaderMatcherBase::~CC3ShaderMatcherBase()
{
	CC_SAFE_RELEASE(_semanticDelegate);
}

CC3ShaderProgram* CC3ShaderMatcherBase::getProgramForMeshNode( CC3MeshNode* aMeshNode )
{
	return CC3ShaderProgram::programWithSemanticDelegate( getSemanticDelegate(), vertexShaderFileForMeshNode(aMeshNode), 
		fragmentShaderFileForMeshNode(aMeshNode) );
}

std::string CC3ShaderMatcherBase::vertexShaderFileForMeshNode( CC3MeshNode* aMeshNode )
{
	if (aMeshNode->shouldDrawInClipSpace()) 
		return "CC3ClipSpaceTexturable.vsh";
	
	if (aMeshNode->hasRigidSkeleton()) 
		return "CC3TexturableRigidBones.vsh";
	
	if (aMeshNode->hasSkeleton()) 
		return "CC3TexturableBones.vsh";
	
	if (aMeshNode->isDrawingPointSprites()) 
		return "CC3PointSprites.vsh";
		
	return "CC3Texturable.vsh";
}

std::string CC3ShaderMatcherBase::fragmentShaderFileForMeshNode( CC3MeshNode* aMeshNode )
{	
	CC3Material* mat = aMeshNode->getMaterial();
	GLuint texCnt = mat->getTextureCount();
	bool shouldAlphaTest = !mat->shouldDrawLowAlpha();
	
	if (aMeshNode->shouldDrawInClipSpace())
		return (texCnt > 0) ? "CC3ClipSpaceSingleTexture.fsh" : "CC3ClipSpaceNoTexture.fsh";
	
	if (aMeshNode->isDrawingPointSprites())
		return shouldAlphaTest ? "CC3PointSpritesAlphaTest.fsh" : "CC3PointSprites.fsh";

	// Material without texture
	if (texCnt == 0) return shouldAlphaTest ? "CC3NoTextureAlphaTest.fsh" : "CC3NoTexture.fsh";

	// Reflection using cube-map texture
	if (mat->hasTextureCube()) {
		if (texCnt > 1)
			return shouldAlphaTest ? "CC3SingleTextureReflectAlphaTest.fsh" : "CC3SingleTextureReflect.fsh";
		else
			return shouldAlphaTest ? "CC3NoTextureReflectAlphaTest.fsh" : "CC3NoTextureReflect.fsh";
	}
	
	// Bump-mapping using a tangent-space normal map texture.
	if (texCnt > 1 && aMeshNode->getMesh() && aMeshNode->getMesh()->hasVertexTangents())
		return shouldAlphaTest ? "CC3BumpMapTangentSpaceAlphaTest.fsh" : "CC3BumpMapTangentSpace.fsh";
	
	// Bump-mapping using an object-space normal map texture.
	if (texCnt > 1 && mat->hasBumpMap())
		return shouldAlphaTest ? "CC3BumpMapObjectSpaceAlphaTest.fsh" : "CC3BumpMapObjectSpace.fsh";
	
	// Default to the basic single-texture shader program
	return shouldAlphaTest ? "CC3SingleTextureAlphaTest.fsh" : "CC3SingleTexture.fsh";
}

CC3ShaderProgram* CC3ShaderMatcherBase::getPureColorProgramMatching( CC3ShaderProgram* shaderProgram )
{
	return CC3ShaderProgram::programWithSemanticDelegate( shaderProgram->getSemanticDelegate(),
										 shaderProgram->getVertexShader()->getName(),
										"CC3PureColor.fsh" );
}

bool CC3ShaderMatcherBase::init()
{
	super::init();

	initSemanticDelegate();

	return true;
}

void CC3ShaderMatcherBase::initSemanticDelegate()
{
	CC3ShaderSemanticsByVarName* sd = new CC3ShaderSemanticsByVarName();
	sd->init();
	sd->populateWithDefaultVariableNameMappings();
	sd->autorelease();
	sd->retain();

	_semanticDelegate = sd;			// retained from new
}

CC3ShaderSemanticsDelegate* CC3ShaderMatcherBase::getSemanticDelegate()
{
	return _semanticDelegate;
}

void CC3ShaderMatcherBase::setSemanticDelegate( CC3ShaderSemanticsDelegate* pDelegate )
{
	CC_SAFE_RELEASE( _semanticDelegate );
	_semanticDelegate = pDelegate;
	CC_SAFE_RETAIN( pDelegate );
}

NS_COCOS3D_END
