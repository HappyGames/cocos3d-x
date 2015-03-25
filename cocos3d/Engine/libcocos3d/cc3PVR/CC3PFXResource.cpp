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

CC3PFXResource::CC3PFXResource()
{
	_effectsByName = NULL;
	_texturesByName = NULL;
}

CC3PFXResource::~CC3PFXResource()
{
	CC_SAFE_RELEASE( _effectsByName );
	CC_SAFE_RELEASE( _texturesByName );
}

CC3PFXEffect* CC3PFXResource::getEffectNamed( const std::string& effectName )
{
	if ( effectName.empty() ) 
		return NULL;
	
	CC3PFXEffect* pfxEffect = (CC3PFXEffect*)_effectsByName->objectForKey( effectName );
	if ( !pfxEffect ) 
	{
		CC3_ERROR( "CC3PFXResource could not find PFX effect named %s."
							   " Mesh nodes using this PFX effect will revert to default shaders and textures.",
						    effectName.c_str() );
	}

	return pfxEffect;
}

CC3PFXEffect* CC3PFXResource::getEffectNamedFromRez( const std::string& effectName, const std::string& rezName )
{
	if ( effectName.empty() || rezName.empty() ) 
		return NULL;
	
	CC3PFXResource* pfxRez = (CC3PFXResource*)getResourceNamed( rezName );
	if ( !pfxRez ) 
	{
		CC3_ERROR( "CC3PFXResource could not find cached PFX resource named %s to apply effect named %s."
							" Mesh nodes using this PFX effect will revert to default shaders and textures.",
							rezName.c_str(), effectName.c_str() );
	}

	return pfxRez->getEffectNamed( effectName );
}

CC3PFXEffect* CC3PFXResource::getEffectNamedFromFile( const std::string& effectName, const std::string& aFilePath )
{
	if ( effectName.empty() || aFilePath.empty() ) 
		return NULL;
	
	CC3PFXResource* pfxRez = (CC3PFXResource*)resourceFromFile(aFilePath);
	if ( !pfxRez ) 
	{
		CC3_ERROR( "CC3PFXResource could not load PFX resource file %s to apply effect named %s."
							" Mesh nodes using this PFX effect will revert to default shaders and textures.",
							aFilePath.c_str(), effectName.c_str() );
	}
	
	return pfxRez->getEffectNamed( effectName );
}

bool CC3PFXResource::init()
{
	if ( super::init() )
	{
		_effectsByName = CCDictionary::create();
		_effectsByName->retain();
		_texturesByName = CCDictionary::create();
		_texturesByName->retain();

		return true;
	}

	return false;
}

CC3PFXResource* CC3PFXResource::resourceFromFile( const std::string& filePath )
{
	std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename( filePath.c_str() );
	CC3PFXResource* pRez = new CC3PFXResource;
	if ( pRez->initFromFile( fullPath ) )
	{
		pRez->autorelease();
		return pRez;
	}
	
	CC_SAFE_DELETE( pRez );
	return pRez;
}

/** Load the file, and if successful build this resource from the contents. */
bool CC3PFXResource::processFile( const std::string& anAbsoluteFilePath )
{
	// Split the path into directory and file names and set the PVR read path to the directory and
	// pass the unqualified file name to the parser. This allows the parser to locate any additional
	// files that might be read as part of the parsing. For PFX, this will include any shader files
	// referenced by the PFX file.
	std::string fileName = CC3String::getFileName( anAbsoluteFilePath );
	std::string dirName = CC3String::getDirectory( anAbsoluteFilePath );
	CPVRTResourceFile::SetReadPath( dirName.c_str() );

	CPVRTString	error;
	CPVRTPFXParser* pfxParser = new CPVRTPFXParser();
	bool wasLoaded = (pfxParser->ParseFromFile(fileName.c_str(), &error) == PVR_SUCCESS);
	if (wasLoaded)
		buildFromPFXParser( pfxParser );
	else
		CC3_ERROR( "Could not load %s because %s", fileName.c_str(), error.c_str() );

	delete pfxParser;

	return wasLoaded;
}

/** Build this instance from the contents of the resource. */
void  CC3PFXResource::buildFromPFXParser( CPVRTPFXParser* pfxParser )
{
	buildTexturesFromPFXParser( pfxParser );
	buildEffectsFromPFXParser( pfxParser );
	buildRenderPassesFromPFXParser( pfxParser );
}

/** Extracts the texture definitions and loads them from files. */
void CC3PFXResource::buildTexturesFromPFXParser( CPVRTPFXParser* pfxParser )
{
	GLuint texCnt = pfxParser->GetNumberTextures();
	for (GLuint texIdx = 0; texIdx < texCnt; texIdx++) 
	{
		const SPVRTPFXParserTexture* pfxTex = pfxParser->GetTexture(texIdx);
		//LogRez(@"Adding texture %@", NSStringFromSPVRTPFXParserTexture((SPVRTPFXParserTexture*)pfxTex));

		CCAssert(!pfxTex->bRenderToTexture, "%CC3PFXResource rendering to a texture is not supported");
		
		// Load texture and set texture parameters
		std::string texName = pfxTex->Name.c_str();
		std::string texFile = pfxTex->FileName.c_str();
		CC3Texture* tex = CC3Texture::textureFromFile( texFile.c_str() );
		if ( tex == NULL )
			tex = CC3Texture::textureFromFile( (_directory + texFile).c_str() );

		tex->setHorizontalWrappingFunction( GLTextureWrapFromETextureWrap(pfxTex->nWrapS) );
		tex->setVerticalWrappingFunction( GLTextureWrapFromETextureWrap(pfxTex->nWrapT) );
		tex->setMinifyingFunction( GLMinifyingFunctionFromMinAndMipETextureFilters(pfxTex->nMin, pfxTex->nMIP) );
		tex->setMagnifyingFunction( GLMagnifyingFunctionFromETextureFilter(pfxTex->nMag) );
		if (tex)
			_texturesByName->setObject( tex, texName );	// Add to texture dictionary
		else
			CCLOGERROR("CC3PFXResource cannot load texture named %s from file %s", texName.c_str(), texFile.c_str());
	}
}

/** Builds the effects from the shaders and textures defined in this resource. */
void CC3PFXResource::buildEffectsFromPFXParser( CPVRTPFXParser* pfxParser )
{
	GLuint eCnt = pfxParser->GetNumberEffects();
	for (GLuint eIdx = 0; eIdx < eCnt; eIdx++) 
	{
		const SPVRTPFXParserEffect& pfxEffect = pfxParser->GetEffect(eIdx);
		CC3PFXEffect* effect = new CC3PFXEffect;
		effect->initFromSPVRTPFXParserEffect( (SPVRTPFXParserEffect*)&pfxEffect, pfxParser, this );
		_effectsByName->setObject( effect, effect->getName() );
		effect->release();
	}
}

/** Returns the texture that was assigned the specified name in the PFX resource file. */
CC3Texture* CC3PFXResource::getTextureNamed( const std::string& texName )
{
	return (CC3Texture*)_texturesByName->objectForKey(texName); 
}

/**
 * Builds the rendering passes.
 *
 * Multi-pass rendering is not currently supported.
 * For now, just log the render passes described by this PFX resource.
 */
void CC3PFXResource::buildRenderPassesFromPFXParser( CPVRTPFXParser* pfxParser )
{
	GLuint rpCnt = pfxParser->GetNumberRenderPasses();
	for (GLuint rpIdx = 0; rpIdx < rpCnt; rpIdx++) 
	{
		/*const SPVRTPFXRenderPass rendPass = pfxParser->GetRenderPass(rpIdx);
		LogRez(@"Describing render pass %@", NSStringFromSPVRTPFXRenderPass((SPVRTPFXRenderPass*)&rendPass));*/
	}
}

std::string CC3PFXResource::fullDescription()
{
	std::string desc = "";
	desc += "CC3PFXResource";
	desc += CC3String::stringWithFormat( (char*)" containing %ld effects", (unsigned long)_effectsByName->count() );
	desc += CC3String::stringWithFormat( (char*)", %ld textures", (unsigned long)_texturesByName->count() );
	return desc;
}
//
//static Class _defaultSemanticDelegateClass = nil;
//
//+(Class) defaultSemanticDelegateClass {
//	if ( !_defaultSemanticDelegateClass)
//		self.defaultSemanticDelegateClass = [CC3PVRShamanShaderSemantics class];
//	return _defaultSemanticDelegateClass;
//}
//
//+(void) setDefaultSemanticDelegateClass: (Class) aClass {
//	[_defaultSemanticDelegateClass release];
//	_defaultSemanticDelegateClass = [aClass retain];
//}
//
//@end

CC3PFXEffect::CC3PFXEffect()
{
	_shaderProgram = NULL;
	_textures = NULL;
	_variables = NULL;
}

CC3PFXEffect::~CC3PFXEffect()
{
	CC_SAFE_RELEASE( _textures );
	CC_SAFE_RELEASE( _shaderProgram );
	CC_SAFE_RELEASE( _variables );
}

std::string CC3PFXEffect::getName()
{
	return _name;
}

CC3ShaderProgram* CC3PFXEffect::getShaderProgram()
{
	return _shaderProgram;
}

CCArray* CC3PFXEffect::getTextures()
{
	return _textures;
}

CCArray* CC3PFXEffect::getVariables()
{
	return _variables;
}

// Set the GL program into the mesh node
void CC3PFXEffect::populateMeshNode( CC3MeshNode* meshNode )
{
	meshNode->setShaderProgram( _shaderProgram );  
}

// Set each texture into its associated texture unit
// After parsing, the ordering might not be consecutive, so look each up by texture unit index
void CC3PFXEffect::populateMaterial( CC3Material* material )
{
	GLuint tuCnt = _textures->count();
	for (GLuint tuIdx = 0; tuIdx < tuCnt; tuIdx++) 
	{
		CC3PFXEffectTexture* pfxTex = getEffectTextureForTextureUnit( tuIdx );
		if (pfxTex)
			material->setTexture( pfxTex->getTexture(), tuIdx );
		else
			CCLOGERROR("CC3PFXEffect contains no texture for texture unit %d", tuIdx);
	}
}

/**
 * Returns the effect texture to be applied to the specified texture unit,
 * or nil if no texture is defined for that texture unit.
 */
CC3PFXEffectTexture* CC3PFXEffect::getEffectTextureForTextureUnit( GLuint tuIdx )
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( _textures, pObj )
	{
		CC3PFXEffectTexture* effectTex = (CC3PFXEffectTexture*)pObj;
		if ( effectTex->getTextureUnitIndex() == tuIdx )
			return effectTex;
	}
	
	return NULL;
}

void CC3PFXEffect::initFromSPVRTPFXParserEffect( PFXClassPtr pSPVRTPFXParserEffect, PFXClassPtr pCPVRTPFXParser, CC3PFXResource* pfxRez )
{
	//LogRez(@"Creating %@ from: %@", [self class], NSStringFromSPVRTPFXParserEffect(pSPVRTPFXParserEffect));
	CPVRTPFXParser* pfxParser = (CPVRTPFXParser*)pCPVRTPFXParser;
	SPVRTPFXParserEffect* pfxEffect = (SPVRTPFXParserEffect*)pSPVRTPFXParserEffect;
	_name = pfxEffect->Name.c_str();		// retained
	initTexturesForPFXEffect( pfxEffect, pfxParser, pfxRez );
	initVariablesForPFXEffect( pfxEffect, pfxParser, pfxRez );
	initShaderProgramForPFXEffect( pfxEffect, pfxParser, pfxRez );
}

/** Initializes the effect textures in the textures property.  */
void CC3PFXEffect::initTexturesForPFXEffect( SPVRTPFXParserEffect* pfxEffect, CPVRTPFXParser* pfxParser, CC3PFXResource* pfxRez )
{
	_textures = CCArray::create();
	_textures->retain();
	
	CPVRTArray<SPVRTPFXParserEffectTexture> effectTextures = pfxEffect->Textures;
	GLuint texCount = effectTextures.GetSize();
	for(GLuint texIdx = 0; texIdx < texCount; texIdx++) 
	{
		// Extract the texture and texture unit index from the SPVRTPFXParserEffectTexture
		std::string texName = effectTextures[texIdx].Name.c_str();;
		GLuint tuIdx = effectTextures[texIdx].nNumber;
		
		// Retrieve the texture from the PFX resource and add a CC3PFXEffectTexture
		// linking the texture to the texture unit
		CC3Texture* tex = (CC3Texture*)pfxRez->getTextureNamed(texName);
		if (tex) {
			CC3PFXEffectTexture* effectTex = new CC3PFXEffectTexture;
			effectTex->setTexture( tex );
			effectTex->setName( texName );
			effectTex->setTextureUnitIndex( tuIdx );
			_textures->addObject( effectTex );
		} else {
			CCLOGERROR("%CC3PFXEffect could not find texture named %s", texName.c_str() );
		}
	}
}

/** Initializes the variables configurations in the variables property. */
void CC3PFXEffect::initVariablesForPFXEffect( SPVRTPFXParserEffect* pfxEffect, CPVRTPFXParser* pfxParser, CC3PFXResource* pfxRez )
{
	_variables = CCArray::create();
	_variables->retain();

	addVariablesFrom( pfxEffect->Attributes );
	addVariablesFrom( pfxEffect->Uniforms );
}

/** Adds a variable configuration for each semantic spec in the specified array. */
void CC3PFXEffect::addVariablesFrom( CPVRTArray<SPVRTPFXParserSemantic> pfxVariables )
{
	GLuint varCount = pfxVariables.GetSize();
	for(GLuint varIdx = 0; varIdx < varCount; varIdx++) 
	{
		CC3PFXGLSLVariableConfiguration* varConfig = new CC3PFXGLSLVariableConfiguration;
		varConfig->setName( pfxVariables[varIdx].pszName );
		varConfig->setPFXSemanticName( pfxVariables[varIdx].pszValue );
		varConfig->setSemanticIndex( pfxVariables[varIdx].nIdx );
		_variables->addObject( varConfig );
		varConfig->release();
	}
}

/** Initializes the CC3ShaderProgram built from the shaders defined for this effect. */
void CC3PFXEffect::initShaderProgramForPFXEffect( SPVRTPFXParserEffect* pfxEffect, CPVRTPFXParser* pfxParser, CC3PFXResource* pfxRez )
{
#if CC3_GLSL
	// Retrieve or create the vertex shader
	SPVRTPFXParserShader* pfxVtxShader = getPFXVertexShaderForPFXEffect( pfxEffect, pfxParser );
	CC3VertexShader* vtxShader = CC3VertexShader::shaderFromPFXShader( pfxVtxShader, pfxRez );

	// Retrieve or create the fragment shader
	SPVRTPFXParserShader* pfxFragShader = getPFXFragmentShaderForPFXEffect( pfxEffect, pfxParser );
	CC3FragmentShader* fragShader = CC3FragmentShader::shaderFromPFXShader( pfxFragShader, pfxRez );
	
	CC3PFXShaderSemantics* semanticDelegate = getSemanticDelegateFrom( pfxEffect, pfxParser, pfxRez );

	_shaderProgram = CC3ShaderProgram::programWithSemanticDelegate( semanticDelegate, vtxShader, fragShader );
	_shaderProgram->retain();
#endif	// CC3_GLSL
}

/**
 * Template property to determine the class of shader program to instantiate.
 * The returned class must be a subclass of CC3ShaderProgram.
 */
//-(Class) shaderProgramClass { return [CC3ShaderProgram class]; }

/** Template method to create, populate, and return the semantic delegate to use in the GL program. */
CC3PFXShaderSemantics* CC3PFXEffect::getSemanticDelegateFrom( SPVRTPFXParserEffect* pfxEffect, CPVRTPFXParser* pfxParser, CC3PFXResource* pfxRez )
{
	CC3PFXShaderSemantics* semanticDelegate = new CC3PVRShamanShaderSemantics;
	semanticDelegate->init();
	semanticDelegate->populateWithVariableNameMappingsFromPFXEffect( this );
	semanticDelegate->autorelease();

	return semanticDelegate;
}

/** Returns the PFX vertex shader that was assigned the specified name in the PFX resource file. */
SPVRTPFXParserShader* CC3PFXEffect::getPFXVertexShaderForPFXEffect( SPVRTPFXParserEffect* pfxEffect, CPVRTPFXParser* pfxParser )
{
	const char* sName = pfxEffect->VertexShaderName.c_str();
	GLuint sCnt = pfxParser->GetNumberVertexShaders();
	for (GLuint sIdx = 0; sIdx < sCnt; sIdx++) 
	{
		const SPVRTPFXParserShader& pfxShader = pfxParser->GetVertexShader(sIdx);
		if (strcmp(pfxShader.Name.c_str(), sName) == 0) 
			return (SPVRTPFXParserShader*)&pfxShader;
	}

	return NULL;
}

/** Returns the PFX fragment shader that was assigned the specified name in the PFX resource file. */
SPVRTPFXParserShader* CC3PFXEffect::getPFXFragmentShaderForPFXEffect( SPVRTPFXParserEffect* pfxEffect, CPVRTPFXParser* pfxParser )
{
	const char* sName = pfxEffect->FragmentShaderName.c_str();
	GLuint sCnt = pfxParser->GetNumberFragmentShaders();
	for (GLuint sIdx = 0; sIdx < sCnt; sIdx++) {
		const SPVRTPFXParserShader& pfxShader = pfxParser->GetFragmentShader(sIdx);
		if (strcmp(pfxShader.Name.c_str(), sName) == 0) 
			return (SPVRTPFXParserShader*)&pfxShader;
	}
	return NULL;
}

/** Returns the shader code for the specified shader. */
GLchar* CC3PFXEffect::getShaderCode( SPVRTPFXParserShader* pfxShader )
{
	return pfxShader->pszGLSLcode; 
}

/** Returns a string description of this effect. */
std::string CC3PFXEffect::description()
{
	return CC3String::stringWithFormat( (char*)"CC3PFXEffect named %s", _name.c_str() ); 
}

std::string CC3PFXGLSLVariableConfiguration::getPFXSemanticName()
{
	return _pfxSemanticName;
}

void CC3PFXGLSLVariableConfiguration::setPFXSemanticName( const std::string& name )
{
	_pfxSemanticName = name;
}

CC3PFXEffectTexture::CC3PFXEffectTexture()
{
	_texture = NULL;
}

CC3PFXEffectTexture::~CC3PFXEffectTexture()
{
	CC_SAFE_RELEASE( _texture );
}

CC3Texture* CC3PFXEffectTexture::getTexture()
{
	return _texture;
}

void CC3PFXEffectTexture::setTexture( CC3Texture* texture )
{
	CC_SAFE_RETAIN( texture );
	CC_SAFE_RELEASE( _texture );
	_texture = texture;
}

std::string	CC3PFXEffectTexture::getName()
{
	return _name;
}

void CC3PFXEffectTexture::setName( const std::string& name )
{
	_name = name;
}

/** The index of the texture unit to which the texture should be applied. */
GLuint CC3PFXEffectTexture::getTextureUnitIndex()
{
	return _textureUnitIndex;
}

void CC3PFXEffectTexture::setTextureUnitIndex( GLuint unitIndex )
{
	_textureUnitIndex = unitIndex;
}

/** Overridden to allow default naming semantics to be combined with PFX-defined semantics. */
bool CC3PFXShaderSemantics::configureVariable( CC3GLSLVariable* variable )
{
	return super::configureVariable(variable) ||
			CC3ShaderProgram::getShaderMatcher()->getSemanticDelegate()->configureVariable(variable);
}

void CC3PFXShaderSemantics::populateWithVariableNameMappingsFromPFXEffect( CC3PFXEffect* pfxEffect )
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( pfxEffect->getVariables(), pObj )
	{
		CC3PFXGLSLVariableConfiguration* pfxVarConfig = (CC3PFXGLSLVariableConfiguration*)pObj;
		resolveSemanticForVariableConfiguration( pfxVarConfig );
		addVariableConfiguration( pfxVarConfig );
	}
}

bool CC3PFXShaderSemantics::resolveSemanticForVariableConfiguration( CC3PFXGLSLVariableConfiguration* pfxVarConfig )
{
	if (pfxVarConfig->getSemantic() != kCC3SemanticNone) 
		return true;		// Only do it once!
	
	GLenum semantic = getSemanticForPFXSemanticName( pfxVarConfig->getPFXSemanticName() );
	if (semantic == kCC3SemanticNone) 
		return false;

	pfxVarConfig->setSemantic( semantic );

	return true;
}

GLenum CC3PFXShaderSemantics::getSemanticForPFXSemanticName( const std::string& semanticName )
{
	return kCC3SemanticNone; 
}


NS_COCOS3D_END
