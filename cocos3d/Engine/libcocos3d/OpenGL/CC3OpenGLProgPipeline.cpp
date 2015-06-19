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

#if CC3_GLSL

CC3OpenGLProgPipeline::CC3OpenGLProgPipeline()
{
	_shaderProgramPrewarmer = NULL;
}

CC3OpenGLProgPipeline::~CC3OpenGLProgPipeline()
{
}

/** Only need to bind vertex indices. All vertex attributes are bound when program is bound. */
void CC3OpenGLProgPipeline::bindMesh( CC3Mesh* mesh, CC3NodeDrawingVisitor* visitor )
{
	if ( mesh && mesh->getVertexIndices() )
		mesh->getVertexIndices()->bindContentToAttributeAt( kCC3VertexAttributeIndexUnavailable, visitor );
}

void CC3OpenGLProgPipeline::bindVertexAttribute( CC3GLSLAttribute* attribute, CC3NodeDrawingVisitor* visitor )
{
	if ( attribute == NULL )
		return;

	CCAssert(attribute->getSemantic() != kCC3SemanticNone || !visitor->getCurrentMeshNode()->getShaderContext()->shouldEnforceVertexAttributes(),
			  "Cannot bind the attribute named %s to the GL engine because its semantic meaning"
			  " is unknown. Check the attribute name. If the attribute name is correct, but is"
			  " not a standard Cocos3D attribute name, assign a semantic value to the attribute"
			  " in the configureVariable: method of your semantic delegate implementation, or use"
			  " a PFX file to define the semantic for the attribute name."/*, attribute.name*/);

	CC3VertexArray* pVertexArray = getVertexArrayForAttribute( attribute, visitor );
	if ( pVertexArray )
		pVertexArray->bindContentToAttributeAt( attribute->getLocation(), visitor );
}

/** 
 * Returns the vertex array that should be bound to the specified attribute, or nil if the
 * mesh does not contain a vertex array that matches the specified attribute.
 */
CC3VertexArray* CC3OpenGLProgPipeline::getVertexArrayForAttribute( CC3GLSLAttribute* attribute, CC3NodeDrawingVisitor* visitor )
{
	CC3Mesh* pMesh = visitor->getCurrentMesh();
	if ( pMesh == NULL )
		return NULL;

	return pMesh->getVertexArrayForSemantic( attribute->getSemantic(), attribute->getSemanticIndex() );
}

void CC3OpenGLProgPipeline::setVertexAttributeEnablementAt( GLint vaIdx )
{
	if ( vertexAttributes[vaIdx].isEnabled )
		glEnableVertexAttribArray( vaIdx );
	else
		glDisableVertexAttribArray( vaIdx );

	CHECK_GL_ERROR_DEBUG();
}

void CC3OpenGLProgPipeline::bindVertexContentToAttributeAt( GLint vaIdx )
{
	if (vaIdx < 0) 
		return;

	CC3VertexAttr* vaPtr = &vertexAttributes[vaIdx];
	glVertexAttribPointer( vaIdx, vaPtr->elementSize, vaPtr->elementType,
						  vaPtr->shouldNormalize, vaPtr->vertexStride, vaPtr->vertices );
	
	CHECK_GL_ERROR_DEBUG();
}

void CC3OpenGLProgPipeline::enable2DVertexAttributes()
{
	for (GLuint vaIdx = 0; vaIdx < value_MaxVertexAttribsUsed; vaIdx++)
		enableVertexAttribute( (vaIdx < kCCVertexAttrib_MAX), vaIdx );
}

void CC3OpenGLProgPipeline::align3DVertexAttributeStateWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	super::align3DVertexAttributeStateWithVisitor( visitor );

	// Mark position, color & tex coords as unknown
	GLuint maxAttribs = MIN(value_MaxVertexAttribsUsed, kCCVertexAttrib_MAX);
	for (GLuint vaIdx = 0; vaIdx < maxAttribs; vaIdx++) 
	{
		vertexAttributes[vaIdx].isEnabledKnown = false;
		vertexAttributes[vaIdx].isKnown = false;
	}
}

// Don't change matrix state on background thread (which can occur during shader prewarming),
// because it messes with the concurrent rendering of Cocos2D components on the rendering thread.
void CC3OpenGLProgPipeline::activateMatrixStack( GLenum mode )
{
	if ( !isRenderingContext() )
		return;
	
	kmGLMatrixMode(mode);
}

void CC3OpenGLProgPipeline::loadModelviewMatrix( const CC3Matrix4x3* mtx )
{
	if ( !isRenderingContext() )
		return;

	activateMatrixStack( GL_MODELVIEW );
	CC3Matrix4x4 glMtx;
	CC3Matrix4x4PopulateFrom4x3(&glMtx, mtx);
	kmGLLoadMatrix((kmMat4*)&glMtx);
}

void CC3OpenGLProgPipeline::loadProjectionMatrix( const CC3Matrix4x4* mtx )
{
	if ( !isRenderingContext() ) 
		return;
	
	activateMatrixStack( GL_PROJECTION );
	kmGLLoadMatrix((kmMat4*)mtx);
}

void CC3OpenGLProgPipeline::pushModelviewMatrixStack()
{
	if ( !isRenderingContext() ) 
		return;
	
	activateMatrixStack( GL_MODELVIEW );
	kmGLPushMatrix();
}

void CC3OpenGLProgPipeline::popModelviewMatrixStack()
{
	if ( !isRenderingContext() ) 
		return;
	
	activateMatrixStack( GL_MODELVIEW );
	kmGLPopMatrix();
}

void CC3OpenGLProgPipeline::pushProjectionMatrixStack()
{
	if ( !isRenderingContext() ) 
		return;
	
	activateMatrixStack( GL_PROJECTION );
	kmGLPushMatrix();
}

void CC3OpenGLProgPipeline::popProjectionMatrixStack()
{
	if ( !isRenderingContext() ) 
		return;
	
	activateMatrixStack( GL_PROJECTION );
	kmGLPopMatrix();
}

CC3ShaderPrewarmer* CC3OpenGLProgPipeline::getShaderProgramPrewarmer()
{ 
	return _shaderProgramPrewarmer; 
}

void CC3OpenGLProgPipeline::setShaderProgramPrewarmer( CC3ShaderPrewarmer* shaderProgramPrewarmer )
{
	if (shaderProgramPrewarmer == _shaderProgramPrewarmer) 
		return;
	CC_SAFE_RELEASE( _shaderProgramPrewarmer );
	_shaderProgramPrewarmer = shaderProgramPrewarmer;
	CC_SAFE_RETAIN( shaderProgramPrewarmer );
}

GLuint CC3OpenGLProgPipeline::createShader( GLenum shaderType )
{
    GLuint shaderID = glCreateShader(shaderType);

	CHECK_GL_ERROR_DEBUG();

	return shaderID;
}

void CC3OpenGLProgPipeline::deleteShader( GLuint shaderID )
{
	if ( !shaderID ) 
		return;		// Silently ignore zero ID

	glDeleteShader(shaderID);

	CHECK_GL_ERROR_DEBUG();
}

void CC3OpenGLProgPipeline::compileShader( GLuint shaderID, GLuint srcStrCount, const GLchar** srcCodeStrings )
{
	glShaderSource( shaderID, srcStrCount, srcCodeStrings, NULL );

	glCompileShader( shaderID );
	
	CHECK_GL_ERROR_DEBUG();
}

bool CC3OpenGLProgPipeline::getShaderWasCompiled( GLuint shaderID )
{
	if ( !shaderID ) 
		return false;

    return (getIntegerParameterForShader( GL_COMPILE_STATUS, shaderID ) > 0);
}

GLint CC3OpenGLProgPipeline::getIntegerParameterForShader( GLenum param, GLuint shaderID )
{
	GLint val;
    glGetShaderiv(shaderID, param, &val);

	CHECK_GL_ERROR_DEBUG();

	return val;
}

std::string CC3OpenGLProgPipeline::getLogForShader( GLuint shaderID )
{
	GLint strLen = getIntegerParameterForShader( GL_INFO_LOG_LENGTH, shaderID );
	if ( strLen < 1 ) 
		return NULL;
	
	GLint charsRetrieved = 0;
	GLchar* contentBytes = new GLchar[strLen];
	glGetShaderInfoLog(shaderID, strLen, &charsRetrieved, contentBytes);

	CHECK_GL_ERROR_DEBUG();

	std::string sRet = contentBytes;
	CC_SAFE_DELETE_ARRAY(contentBytes);

	return sRet;
}

std::string CC3OpenGLProgPipeline::getSourceCodeForShader( GLuint shaderID )
{
	GLint strLen = getIntegerParameterForShader( GL_SHADER_SOURCE_LENGTH, shaderID );
	if ( strLen < 1 ) 
		return NULL;
	
	GLint charsRetrieved = 0;
	GLchar* contentBytes = new GLchar[strLen];
	glGetShaderSource( shaderID, strLen, &charsRetrieved, contentBytes );

	CHECK_GL_ERROR_DEBUG();

	std::string sRet = contentBytes;
	CC_SAFE_DELETE_ARRAY(contentBytes);

	return sRet;
}

GLuint CC3OpenGLProgPipeline::createShaderProgram()
{
    GLuint programID = glCreateProgram();

	CHECK_GL_ERROR_DEBUG();

	return programID;
}

void CC3OpenGLProgPipeline::deleteShaderProgram( GLuint programID )
{
	if ( !programID ) 
		return;		// Silently ignore zero ID
	
	// If the program to be deleted is currently bound, force it to unbind first. Program deletion
	// is deferred by the GL engine until the program is no longer in use. If the GL state is not
	// updated, the program will not actually be deleted in the GL engine. This can occur, for
	// instance, when closing 3D rendering temporarily within an app. The currently bound program
	// will actually never be deleted. In addition, this state engine will continue to think it is
	// bound, which can cause problems if a new shader program is later created with the same ID.
	// This method can be invoked outside the render loop, including during autorelease deallocations,
	// so, align the 2D state to ensure that 2D components will use the correct program.
	if ( value_GL_CURRENT_PROGRAM == programID )
	{
		useShaderProgram( 0 );
		ccGLUseProgram( 0 );
	}
	
	glDeleteProgram( programID );

	CHECK_GL_ERROR_DEBUG();
}

void CC3OpenGLProgPipeline::attachShader( GLuint shaderID, GLuint programID )
{
	if ( !shaderID || !programID ) 
		return;		// Silently ignore zero IDs
	
	glAttachShader( programID, shaderID );

	CHECK_GL_ERROR_DEBUG();
}

void CC3OpenGLProgPipeline::detachShader( GLuint shaderID, GLuint programID )
{
	if ( !shaderID || !programID ) 
		return;		// Silently ignore zero IDs

	glDetachShader( programID, shaderID );

	CHECK_GL_ERROR_DEBUG();
}

void CC3OpenGLProgPipeline::linkShaderProgram( GLuint programID )
{
	if ( !programID ) 
		return;		// Silently ignore zero ID

	glLinkProgram( programID );
}

bool CC3OpenGLProgPipeline::getShaderProgramWasLinked( GLuint programID )
{
	if ( !programID ) 
		return false;
    
	return (getIntegerParameterForShaderProgram( GL_LINK_STATUS, programID ) > 0);
}

void CC3OpenGLProgPipeline::useShaderProgram( GLuint programID )
{
	cc3_CheckGLPrim(programID, value_GL_CURRENT_PROGRAM, isKnown_GL_CURRENT_PROGRAM);
	if ( !needsUpdate ) 
		return;
	
	glUseProgram( programID );

	CHECK_GL_ERROR_DEBUG();
}

GLint CC3OpenGLProgPipeline::getIntegerParameterForShaderProgram( GLenum param, GLuint programID )
{
	GLint val;
	glGetProgramiv( programID, param, &val );

	CHECK_GL_ERROR_DEBUG();

	return val;
}

std::string CC3OpenGLProgPipeline::getLogForShaderProgram( GLuint programID )
{
	GLint strLen = getIntegerParameterForShaderProgram( GL_INFO_LOG_LENGTH, programID );
	if ( strLen < 1 ) 
		return "";
	
	GLint charsRetrieved = 0;
	GLchar* contentBytes = new GLchar[strLen];
	glGetProgramInfoLog( programID, strLen, &charsRetrieved, contentBytes );

	CHECK_GL_ERROR_DEBUG();

	std::string sRet = contentBytes;
	CC_SAFE_DELETE_ARRAY( contentBytes );

	return sRet;
}

void CC3OpenGLProgPipeline::populateShaderProgramVariable( CC3GLSLVariable* var )
{ 
	var->populateFromGL(); 
}

void CC3OpenGLProgPipeline::setShaderProgramUniformValue( CC3GLSLUniform* uniform )
{
	useShaderProgram( uniform->getProgram()->getProgramID() );
	uniform->setGLValue();
}

GLuint CC3OpenGLProgPipeline::getMaxNumberOfVertexShaderUniformVectors()
{ 
	return value_GL_MAX_VERTEX_UNIFORM_VECTORS; 
}

GLuint CC3OpenGLProgPipeline::getMaxNumberOfFragmentShaderUniformVectors()
{ 
	return value_GL_MAX_FRAGMENT_UNIFORM_VECTORS; 
}

GLuint CC3OpenGLProgPipeline::getMaxNumberOfShaderProgramVaryingVectors()
{
	return value_GL_MAX_VARYING_VECTORS; 
}

void CC3OpenGLProgPipeline::align2DStateCacheWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	super::align2DStateCacheWithVisitor( visitor );
	
	ccGLBlendFunc( value_GL_BLEND_SRC_RGB, value_GL_BLEND_DST_RGB );
	
#define kCCMaxActiveTexture 16		// Max Cocos2D texture units, as defined in ccGLStateCache.m
	GLuint tuCnt = MIN( value_MaxTextureUnitsUsed, kCCMaxActiveTexture );
	for (GLuint tuIdx = 0; tuIdx < tuCnt; tuIdx++)
		ccGLBindTexture2DN( tuIdx, value_GL_TEXTURE_BINDING_2D[tuIdx] );
	
	ccGLEnableVertexAttribs( kCCVertexAttribFlag_None );
	ccGLEnableVertexAttribs( kCCVertexAttribFlag_PosColorTex );
	
	ccGLUseProgram( 0 );
	
#if COCOS2D_VERSION < 0x020100
	if (valueCap_GL_BLEND) 
		ccGLEnable(CC_GL_BLEND);
	else 
		ccGLEnable(0);
#endif

	CHECK_GL_ERROR_DEBUG();
}

void CC3OpenGLProgPipeline::align3DStateCacheWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	super::align3DStateCacheWithVisitor( visitor );
	
	isKnown_GL_CURRENT_PROGRAM = false;
}

void CC3OpenGLProgPipeline::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );

	initShaderProgramPrewarmer();
}

void CC3OpenGLProgPipeline::initPlatformLimits()
{
	super::initPlatformLimits();
	
	value_GL_MAX_TEXTURE_UNITS = getInteger( GL_MAX_TEXTURE_IMAGE_UNITS );
	value_GL_MAX_VERTEX_ATTRIBS = getInteger( GL_MAX_VERTEX_ATTRIBS );
	value_GL_SHADING_LANGUAGE_VERSION = getString( GL_SHADING_LANGUAGE_VERSION );
	
	value_GL_MAX_CLIP_PLANES = kCC3MaxGLSLClipPlanes;
	value_GL_MAX_LIGHTS = kCC3MaxGLSLLights;
	value_GL_MAX_PALETTE_MATRICES = kCC3MaxGLSLPaletteMatrices;
	value_GL_MAX_SAMPLES = 1;				// Assume no multi-sampling support
	valueMaxBoneInfluencesPerVertex = kCC3MaxGLSLBoneInfluencesPerVertex;
}

// Start with at least the Cocos2D attributes so they can be enabled and disabled
void CC3OpenGLProgPipeline::initVertexAttributes()
{
	super::initVertexAttributes();
	value_MaxVertexAttribsUsed = kCCVertexAttrib_MAX;
}

void CC3OpenGLProgPipeline::initShaderProgramPrewarmer()
{
	setShaderProgramPrewarmer( CC3ShaderPrewarmer::prewarmerWithName( getName().c_str() ) );
}

#endif	// CC3_GLSL

NS_COCOS3D_END
