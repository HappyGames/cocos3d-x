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
#ifndef _CC3_SHADERMATCHER_H_
#define _CC3_SHADERMATCHER_H_

NS_COCOS3D_BEGIN

class CC3MeshNode;
class CC3NodeDrawingVisitor;

// Legacy naming support
#define CC3GLProgramMatcher				CC3ShaderMatcher
#define CC3ShaderProgramMatcher			CC3ShaderMatcher
#define CC3GLProgramMatcherBase			CC3ShaderMatcherBase
#define CC3ShaderProgramMatcherBase		CC3ShaderMatcherBase


/**
 * CC3ShaderMatcher describes the behaviour required to match nodes and materials to an
 * appropriate GL program for rendering a particular node.
 *
 * Under OpenGL ES 2, every drawable mesh node requires a CC3ShaderProgram to be rendered. Typically,
 * the application will deliberately assign a specific GL program to each material, through the
 * shaderProgram or shaderContext properties of the material, and in some cases, this may be
 * defined during model loading from resources.
 *
 * When a model is created or loaded without a specific CC3ShaderProgram assigned, the material will retrieve
 * an appropriate default shader from the shader cache. The shader cache maintains an instance of an
 * implementation of this protocol and delegates to it to match the model to a suitable GL program.
 */
class CC3ShaderMatcher : public CCObject
{
public:
	CC3ShaderMatcher();
	virtual ~CC3ShaderMatcher();
	/**
	 * Returns the shader program to use to draw the specified mesh node.
	 *
	 * Returns a shader program selected from the characteristics of the mesh node and its material.
	 *
	 * The returned program will be compiled and linked, and will have a semantics delegate
	 * assigned in the semanticDelegate property.
	 *
	 * The implementation is responsible for determining how to match the specified mesh node to an
	 * appropriate GL program, and each implementation may have a different matching methodology.
	 *
	 * Implementations are responsible for compiling, linking, and assigning a semantics
	 * delegate to the program.
	 */
	virtual CC3ShaderProgram*		getProgramForMeshNode( CC3MeshNode* aMeshNode );

	/**
	 * Returns a shader program that matches the specified shader program, but renders the mesh
	 * in a single, solid color, instead of taking into consideration lighting, textures, etc.
	 *
	 * The returned shaderProgram will be used for rendering the mesh node during paint-based node picking,
	 * or can be used for simply rendering the mesh while ignoring lighting, material and textures.
	 *
	 * Implementation should ensure that the vertices will be rendered in the same position as the
	 * specified shader program. Typical implementations will return a shader program that uses the
	 * same vertex shader as the specified shader program, but has a fragment shader that renders 
	 * in a single color.
	 */
	virtual CC3ShaderProgram*		getPureColorProgramMatching( CC3ShaderProgram* shaderProgram );

	/** 
	 * The semantic delegate that will be attached to any program created by this matcher.
	 *
	 * The initial value of this property is an instance of CC3ShaderSemanticsByVarName that has
	 * been populated with default semantics by the populateWithDefaultVariableNameMappings method.
	 */
	virtual CC3ShaderSemanticsDelegate* getSemanticDelegate();
	virtual void					setSemanticDelegate( CC3ShaderSemanticsDelegate* pDelegate );

	virtual bool					init();
};

/**
 * CC3ShaderMatcherBase is a basic implementation of the CC3ShaderMatcher protocol.
 *
 * It looks at aspects of the mesh node, such as number of texture units, bump-mapping, etc.
 * To determine the appropriate GL program for a particular mesh node. All programs matched
 * using this implementation will be assigned the semantics delegate from the semanticDelegate
 * property of this instance.
 */
class CC3ShaderMatcherBase : public CC3ShaderMatcher
{
	DECLARE_SUPER( CC3ShaderMatcher );
public:
	CC3ShaderMatcherBase();
	~CC3ShaderMatcherBase();

	CC3ShaderProgram*				getProgramForMeshNode( CC3MeshNode* aMeshNode );
	std::string						vertexShaderFileForMeshNode( CC3MeshNode* aMeshNode );
	std::string						fragmentShaderFileForMeshNode( CC3MeshNode* aMeshNode );
	CC3ShaderProgram*				getPureColorProgramMatching( CC3ShaderProgram* shaderProgram );
	virtual bool					init();
	void							initSemanticDelegate();

	virtual CC3ShaderSemanticsDelegate* getSemanticDelegate();
	virtual void					setSemanticDelegate( CC3ShaderSemanticsDelegate* pDelegate );

protected:
	CC3ShaderSemanticsDelegate*		m_pSemanticDelegate;
};

NS_COCOS3D_END

#endif
