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
#ifndef _CC3_OPENGL_PROGPIPELINE_H_
#define _CC3_OPENGL_PROGPIPELINE_H_

NS_COCOS3D_BEGIN

#if CC3_GLSL

/**
 * Maximum number of lights under when using GLSL.
 *
 * Although under GLSL, there is no explicit maximum number of lights available, this setting
 * defines the number of possible lights that will be allocated and tracked within Cocos3D, and can
 * be set by the application to confirm the maximum number of lights programmed into the shaders.
 *
 * The default value is 32. This can be changed by either setting the value of this compiler
 * build setting, or by setting the value of the value_GL_MAX_LIGHTS public instance variable
 * of the CC3OpenGL instance.
 */
#ifndef kCC3MaxGLSLLights
#	define kCC3MaxGLSLLights				32
#endif

/**
 * Maximum number of user clip planes when using GLSL.
 *
 * Although under GLSL, there is no explicit maximum number of clip planes available, this
 * setting defines the number of possible user clip planes that will be allocated and tracked
 * within Cocos3D, and can be set by the application to confirm the maximum number of user clip
 * planes programmed into the shaders.
 *
 * The default value is 0, indicating that under a programmable rendering pipeline, the platform
 * does not impose a limit. This can be changed by either setting the value of this compiler
 * build setting, or by setting the value of the value_GL_MAX_CLIP_PLANES public instance
 * variable of the CC3OpenGL instance.
 */
#ifndef kCC3MaxGLSLClipPlanes
#	define kCC3MaxGLSLClipPlanes			0
#endif

/**
 * Maximum number of palette matrices used for vertex skinning when using GLSL.
 *
 * Although under GLSL, there is no explicit maximum number of palette matrices available, this
 * setting defines the number of possible matrices that will be allocated and tracked within
 * Cocos3D, and can be set by the application to confirm the maximum number of palettes programmed
 * into the shaders.
 *
 * The default value is 0, indicating that under a programmable rendering pipeline, the platform
 * does not impose a limit. This can be changed by either setting the value of this compiler
 * build setting, or by setting the value of the value_GL_MAX_PALETTE_MATRICES public instance
 * variable of the CC3OpenGL instance.
 */
#ifndef kCC3MaxGLSLPaletteMatrices
#	define kCC3MaxGLSLPaletteMatrices		0
#endif

/** 
 * Maximum number of bones that can be applied to a single vertex during vertex skinning with GLSL.
 *
 * The default value is 0, indicating that under a programmable rendering pipeline, the platform
 * does not impose a limit. This can be changed by either setting the value of this compiler
 * build setting, or by setting the value of the valueMaxBoneInfluencesPerVertex public instance
 * variable of the CC3OpenGL instance.
 */
#ifndef kCC3MaxGLSLBoneInfluencesPerVertex
#	define kCC3MaxGLSLBoneInfluencesPerVertex			0
#endif

class CC3VertexArray;
/** 
 * CC3OpenGLProgPipeline is an abstract class that manages the OpenGL state for a single GL context
 * that supports a programmable pipeline running GLSL.
 */
class CC3OpenGLProgPipeline : public CC3OpenGL
{	
	DECLARE_SUPER( CC3OpenGL );
public:
	CC3OpenGLProgPipeline();
	~CC3OpenGLProgPipeline();

	virtual void			initPlatformLimits();
	virtual void			initVertexAttributes();
	virtual void			align3DVertexAttributeStateWithVisitor( CC3NodeDrawingVisitor* visitor );
	virtual void			align2DStateCacheWithVisitor( CC3NodeDrawingVisitor* visitor );
	virtual void			align3DStateCacheWithVisitor( CC3NodeDrawingVisitor* visitor );

	/** Only need to bind vertex indices. All vertex attributes are bound when program is bound. */
	virtual void			bindMesh( CC3Mesh* mesh, CC3NodeDrawingVisitor* visitor );

	virtual void			bindVertexAttribute( CC3GLSLAttribute* attribute, CC3NodeDrawingVisitor* visitor );

	/** 
	 * Returns the vertex array that should be bound to the specified attribute, or nil if the
	 * mesh does not contain a vertex array that matches the specified attribute.
	 */
	virtual CC3VertexArray*	getVertexArrayForAttribute( CC3GLSLAttribute* attribute, CC3NodeDrawingVisitor* visitor );

	void					setVertexAttributeEnablementAt( GLint vaIdx );
	void					bindVertexContentToAttributeAt( GLint vaIdx );
	void					enable2DVertexAttributes();

	// Don't change matrix state on background thread (which can occur during shader prewarming),
	// because it messes with the concurrent rendering of Cocos2D components on the rendering thread.
	void					activateMatrixStack( GLenum mode );

	void					loadModelviewMatrix( const CC3Matrix4x3* mtx );

	void					loadProjectionMatrix( const CC3Matrix4x4* mtx );
	void					pushModelviewMatrixStack();
	void					popModelviewMatrixStack();
	void					pushProjectionMatrixStack();

	void					popProjectionMatrixStack();

	CC3ShaderPrewarmer*		getShaderProgramPrewarmer();
	void					setShaderProgramPrewarmer( CC3ShaderPrewarmer* shaderProgramPrewarmer );

	GLuint					createShader( GLenum shaderType );
	void					deleteShader( GLuint shaderID );
	void					compileShader( GLuint shaderID, GLuint srcStrCount, const GLchar** srcCodeStrings );

	bool					getShaderWasCompiled( GLuint shaderID );

	GLint					getIntegerParameterForShader( GLenum param, GLuint shaderID );

	std::string				getLogForShader( GLuint shaderID );

	std::string				getSourceCodeForShader( GLuint shaderID );
	GLuint					createShaderProgram();
	void					deleteShaderProgram( GLuint programID );

	void					attachShader( GLuint shaderID, GLuint programID );

	void					detachShader( GLuint shaderID, GLuint programID );

	void					linkShaderProgram( GLuint programID );

	bool					getShaderProgramWasLinked( GLuint programID );

	GLint					getIntegerParameterForShaderProgram( GLenum param, GLuint programID );

	void					useShaderProgram( GLuint programID );

	std::string				getLogForShaderProgram( GLuint programID );
	void					populateShaderProgramVariable( CC3GLSLVariable* var );
	void					setShaderProgramUniformValue( CC3GLSLUniform* uniform );

	GLuint					getMaxNumberOfVertexShaderUniformVectors();
	GLuint					getMaxNumberOfFragmentShaderUniformVectors();
	GLuint					getMaxNumberOfShaderProgramVaryingVectors();

	void					initWithTag( GLuint aTag, const std::string& aName );
	void					initShaderProgramPrewarmer();

public:
	GLint					value_GL_MAX_VARYING_VECTORS;
	GLint					value_GL_MAX_VERTEX_UNIFORM_VECTORS;
	GLint					value_GL_MAX_FRAGMENT_UNIFORM_VECTORS;

	GLuint					value_GL_CURRENT_PROGRAM;

	bool					isKnown_GL_CURRENT_PROGRAM : 1;

protected:
	std::string				value_GL_SHADING_LANGUAGE_VERSION;
	CC3ShaderPrewarmer*		m_pShaderProgramPrewarmer;
};

#endif	// CC3_GLSL

NS_COCOS3D_END

#endif
