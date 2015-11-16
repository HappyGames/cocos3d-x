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
/** @file */	// Doxygen marker
#ifndef _CC3_PFX_RESOURCE_H_
#define _CC3_PFX_RESOURCE_H_

NS_COCOS3D_BEGIN
class CC3PFXEffect;
// Legacy naming support
#define CC3PFXGLProgramSemantics	CC3PFXShaderSemantics.


/**
 * CC3PFXResource is a CC3Resource that wraps a PVR PFX data structure loaded from a file.
 * It handles loading object data from PFX files, and creating content from that data.
 * This class is the cornerstone of PFX file management.
 */
class CC3PFXResource : public CC3Resource
{
	DECLARE_SUPER( CC3Resource );
public:
	CC3PFXResource();
	~CC3PFXResource();

	/** Returns the PFX effect with the specified name, or nil if it doesn't exist. */
	CC3PFXEffect*				getEffectNamed( const std::string& name );

	/**
	 * Returns the PFX effect with the specified name, found in the cached CC3PFXResource with
	 * the specifed name. Returns nil if a PFX resource with the specified name cannot be found
	 * in the PFX resource cache, or if that PFX resource does not contain an effect with the
	 * specified effect name.
	 */
	static CC3PFXEffect*		getEffectNamedFromRez( const std::string& effectName, const std::string& rezName );

	/**
	 * Returns the PFX effect with the specified name, found in the CC3PFXResource loaded from
	 * the specfied file. Returns nil if the PFX resource file could not be loaded, or if that
	 * PFX resource does not contain an effect with the specified effect name.
	 */
	static CC3PFXEffect*		getEffectNamedFromFile( const std::string& effectName, const std::string& aFilePath );

	/** 
	 * The class used to instantiate the semantic delegate for the GLSL programs created for
	 * the PFX effects defined in this PFX resource. The returned class must be a subclass of
	 * CC3PFXShaderSemantics.
	 *
	 * The initial value is set from the class-side defaultSemanticDelegateClass property.
	 */
	//@property(nonatomic, retain) Class semanticDelegateClass;

	/**
	 * The default class used to instantiate the semantic delegate for the GLSL programs created
	 * for the PFX effects defined in instances of this class. The value of this property determines
	 * the initial value of the semanticDelegateClass property of any instances. The returned class
	 * must be a subclass of CC3PFXShaderSemantics.
	 *
	 * The initial value is the CC3PVRShamanShaderSemantics class.
	 */
	//+(Class) defaultSemanticDelegateClass;

	/**
	 * The default class used to instantiate the semantic delegate for the GLSL programs created
	 * for the PFX effects defined in instances of this class. The value of this property determines
	 * the initial value of the semanticDelegateClass property of any instances. The class must be
	 * a subclass of CC3PFXShaderSemantics.
	 *
	 * The initial value is the CC3PVRShamanShaderSemantics class.
	 */
	//+(void) setDefaultSemanticDelegateClass: (Class) aClass;

	virtual	bool				init();

	/** Load the file, and if successful build this resource from the contents. */
	bool						processFile( const std::string& anAbsoluteFilePath );
	/** Build this instance from the contents of the resource. */
	void						buildFromPFXParser( CPVRTPFXParser* pfxParser );
	/** Extracts the texture definitions and loads them from files. */
	void						buildTexturesFromPFXParser( CPVRTPFXParser* pfxParser );
	/** Builds the effects from the shaders and textures defined in this resource. */
	void						buildEffectsFromPFXParser( CPVRTPFXParser* pfxParser );
	/**
	 * Builds the rendering passes.
	 *
	 * Multi-pass rendering is not currently supported.
	 * For now, just log the render passes described by this PFX resource.
	 */
	void						buildRenderPassesFromPFXParser( CPVRTPFXParser* pfxParser );

	/** Returns the texture that was assigned the specified name in the PFX resource file. */
	CC3Texture*					getTextureNamed( const std::string& texName );

	std::string					fullDescription();

	static CC3PFXResource*		resourceFromFile( const std::string& filePath );

protected:
	CCDictionary*				_texturesByName;
	CCDictionary*				_effectsByName;
	// Class _semanticDelegateClass;
};

/**
 * CC3PFXEffect represents a single effect within a PFX resource file. It combines the shader code
 * referenced by the effect into a CC3ShaderProgram, and the textures to apply to the material.
 */
class CC3PFXEffectTexture;
class CC3PFXShaderSemantics;
class CC3PFXEffect : public CCObject 
{
public:
	CC3PFXEffect();
	~CC3PFXEffect();

	/** Returns the name of this effect. */
	std::string					getName();

	/** The shader program used to render this effect. */
	CC3ShaderProgram*			getShaderProgram();

	/**
	 * The textures used in this effect. Each element of this array is an instance of CC3PFXEffectTexture
	 * that contains the texture and the index of the texture unit to which the texture should be applied.
	 */
	CCArray*					getTextures();

	/**
	 * This array contains a configuration spec for each attribute and uniform variable used in
	 * the shaders. Each element of this array is an instance of CC3PFXGLSLVariableConfiguration.
	 */
	CCArray*					getVariables();

	/** Populates the specfied mesh node with the shader program in this effect. */
	void						populateMeshNode( CC3MeshNode* meshNode );

	/** Populates the specfied material with the textures in this effect. */
	void						populateMaterial( CC3Material* material );

	/**
	 * Initializes this instance from the specified SPVRTPFXParserEffect C++ class, retrieved
	 * from the specified CPVRTPFXParser C++ class as loaded from the specfied PFX resource.
	 */
	void						initFromSPVRTPFXParserEffect( PFXClassPtr pSPVRTPFXParserEffect, PFXClassPtr pCPVRTPFXParser, CC3PFXResource* pfxRez );

	/**
	 * Returns the effect texture to be applied to the specified texture unit,
	 * or nil if no texture is defined for that texture unit.
	 */
	CC3PFXEffectTexture*		getEffectTextureForTextureUnit( GLuint tuIdx );
	/** Initializes the effect textures in the textures property.  */
	void						initTexturesForPFXEffect( SPVRTPFXParserEffect* pfxEffect, CPVRTPFXParser* pfxParser, CC3PFXResource* pfxRez );
	/** Initializes the variables configurations in the variables property. */
	void						initVariablesForPFXEffect( SPVRTPFXParserEffect* pfxEffect, CPVRTPFXParser* pfxParser, CC3PFXResource* pfxRez );
	/** Initializes the CC3ShaderProgram built from the shaders defined for this effect. */
	void						initShaderProgramForPFXEffect( SPVRTPFXParserEffect* pfxEffect, CPVRTPFXParser* pfxParser, CC3PFXResource* pfxRez );
	/** Adds a variable configuration for each semantic spec in the specified array. */
	void						addVariablesFrom( CPVRTArray<SPVRTPFXParserSemantic> pfxVariables );
	/** Returns the PFX vertex shader that was assigned the specified name in the PFX resource file. */
	SPVRTPFXParserShader*		getPFXVertexShaderForPFXEffect( SPVRTPFXParserEffect* pfxEffect, CPVRTPFXParser* pfxParser );
	SPVRTPFXParserShader*		getPFXFragmentShaderForPFXEffect( SPVRTPFXParserEffect* pfxEffect, CPVRTPFXParser* pfxParser );

	/**
	 * Template property to determine the class of shader program to instantiate.
	 * The returned class must be a subclass of CC3ShaderProgram.
	 */
	//-(Class) shaderProgramClass { return [CC3ShaderProgram class]; }

	/** Template method to create, populate, and return the semantic delegate to use in the GL program. */
	CC3PFXShaderSemantics*		getSemanticDelegateFrom( SPVRTPFXParserEffect* pfxEffect, CPVRTPFXParser* pfxParser, CC3PFXResource* pfxRez );

	/** Returns the shader code for the specified shader. */
	GLchar*						getShaderCode( SPVRTPFXParserShader* pfxShader );
	/** Returns a string description of this effect. */
	std::string					description();
    
	/** Applies the effect to a CC3MesnNode */
    void                        applyTo( CC3MeshNode* pMeshNode );

protected:
	std::string					_name;
	CC3ShaderProgram*			_shaderProgram;
	CCArray*					_textures;
	CCArray*					_variables;
};

/** A CC3PFXGLSLVariableConfiguration that includes a semantic name retrieved from a PFX effect. */
class CC3PFXGLSLVariableConfiguration : public CC3GLSLVariableConfiguration 
{
public:
	/** The semantic name as retrieved from the PFX effect. */
	std::string					getPFXSemanticName();
	void						setPFXSemanticName( const std::string& name );

protected:
	std::string					_pfxSemanticName;
};

/**
 * CC3PFXShaderSemantics provides a mapping from the PFX semantic names declared in a PFX
 * effect within a PFX effects file, and the standard semantics from the CC3Semantic enumeration.
 *
 * GLSL shader code loaded from a PFX effect can mix custom semantics defined within the PFX effect
 * with standard default semantics defined by the semantic delegate associated with the program matcher.
 * If a GLSL variable cannot be configured based on a semantic definition for its name within the
 * PFX effect, configuration of the variable is delegated to the standard semantic delegate at
 * CC3ShaderProgram.shaderMatcher.semanticDelegate. It is even possible to load shaders that use only
 * standard semantic naming, without having to define any semantics within the PFX effect.
 *
 * This is an abstract implementation. Subclasses can override the semanticForPFXSemanticName:
 * method for simple name-based mapping, or can override the resolveSemanticForVariableConfiguration:
 * for more complex mapping.
 */
class CC3PFXShaderSemantics : public CC3ShaderSemanticsByVarName
{
	DECLARE_SUPER( CC3ShaderSemanticsByVarName );
public:
	/**
	 * Populates this instance with the mappings between variable names and semantics defined
	 * in the specified PFX effect. In the process of doing so, the semantic of each variable
	 * is resolved from the PFX semantic name of the variable configuration.
	 *
	 * For each variable configuration in the variables property of the specified PFX effect, this
	 * method invokes the resolveSemanticForVariableConfiguration: and addVariableConfiguration:
	 * methods to resolve the variable configuration and add it to this semantic mapping.
	 *
	 * This method is invoked automatically during the parsing of the PFX file.
	 */
	virtual void				populateWithVariableNameMappingsFromPFXEffect( CC3PFXEffect* pfxEffect );

	/**
	 * If the semantic property of the specified variable configuration has not already been set,
	 * it is set by resolving it from the PFX semantic name of the specified variable configuration.
	 *
	 * Returns whether the semantic has been resolved. Subclasses that override this method can first
	 * invoke this superclass implementation, and then use the return value to resolve any custom semantics.
	 *
	 * The default behaviour is to invoke the semanticForPFXSemanticName: method with the value of
	 * the pfxSemanticName property of the specified variable configuration, and if it returns a
	 * valid semantic value, the semantic value is set in the specified variable configuration and
	 * this method returns YES. If the semanticForPFXSemanticName: method returns kCC3SemanticNone,
	 * the semantic of the specified variable configuration is not set, and this method returns NO.
	 */
	virtual bool				resolveSemanticForVariableConfiguration( CC3PFXGLSLVariableConfiguration* pfxVarConfig );

	/**
	 * Returns the semantic value corresponding the the specified PFX semantic name, or returns
	 * kCC3SemanticNone if the semantic could not be determined from the PFX semantic name.
	 *
	 * This implementation does nothing and simply returns kCC3SemanticNone. Subclasses will override.
	 */
	virtual GLenum				getSemanticForPFXSemanticName( const std::string& semanticName );

	/** Overridden to allow default naming semantics to be combined with PFX-defined semantics. */
	virtual bool				configureVariable( CC3GLSLVariable* variable );
};

/** CC3PFXEffectTexture is a simple object that links a texture with a particular texture unit. */
class CC3PFXEffectTexture : public CCObject 
{
public:
	CC3PFXEffectTexture();
	~CC3PFXEffectTexture();

	/** The texture being linked to a particular texture unit. */
	CC3Texture*					getTexture();
	void						setTexture( CC3Texture* texture );

	/** The name of the texture as declared in the PFX file. */
	std::string					getName();
	void						setName( const std::string& name );

	/** The index of the texture unit to which the texture should be applied. */
	GLuint						getTextureUnitIndex();
	void						setTextureUnitIndex( GLuint unitIndex );

protected:
	CC3Texture*					_texture;
	std::string					_name;
	GLuint						_textureUnitIndex;
};

NS_COCOS3D_END

#endif
