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

CC3Shader::CC3Shader()
{
	m_pShaderPreamble = NULL;
}

CC3Shader::~CC3Shader()
{
	remove();
	deleteGLShader();
	CC_SAFE_RELEASE( m_pShaderPreamble );
}

GLuint CC3Shader::getShaderID()
{
	if ( !m_shaderID ) 
		m_shaderID = CC3OpenGL::sharedGL()->createShader( getShaderType() );

	return m_shaderID;
}

void CC3Shader::deleteGLShader()
{
	CC3OpenGL::sharedGL()->deleteShader( m_shaderID );
	m_shaderID = 0;
}

GLenum CC3Shader::getShaderType()
{
	CCAssert( false, "getShaderType is not implmented");
	return GL_ZERO;
}

std::string CC3Shader::getShaderPreambleString()
{
	return getShaderPreamble()->getSourceCodeString(); 
}

void CC3Shader::setShaderPreambleString( const std::string& shaderPreambleString )
{
	std::string preambleName = CC3String::stringWithFormat( (char*)"%s-Preamble", getName().c_str() );
	setShaderPreamble( CC3ShaderSourceCode::shaderSourceCodeWithName( preambleName, shaderPreambleString ) );
}

std::string CC3Shader::defaultShaderPreambleString()
{
	return CC3OpenGL::sharedGL()->defaultShaderPreamble(); 
}

static CC3ShaderSourceCode* _defaultShaderPreamble = NULL;

CC3ShaderSourceCode* CC3Shader::defaultShaderPreamble()
{
	if ( !_defaultShaderPreamble ) 
	{
		_defaultShaderPreamble = CC3ShaderSourceCode::shaderSourceCodeWithName( "DefaultShaderPreamble", defaultShaderPreambleString() );
		_defaultShaderPreamble->retain();
	}

	return _defaultShaderPreamble;
}

void CC3Shader::compileFromSourceCode( CC3ShaderSourceCode* shSrcCode )
{
	CCAssert(shSrcCode, "CC3Shader cannot complile NULL GLSL source.");
	
	m_wasLoadedFromFile = shSrcCode->wasLoadedFromFile();

	// Allocate an array of source code strings
	GLuint scCnt = getShaderPreamble()->getSourceStringCount() + shSrcCode->getSourceStringCount();
	GLchar** scStrings = new GLchar*[scCnt];

	// Populate the source code strings from the preamble and specified source code
	CC3ShaderSourceCodeCompilationStringVisitor* visitor = CC3ShaderSourceCodeCompilationStringVisitor::visitorWithCompilationStrings( (const GLchar**)scStrings );
	getShaderPreamble()->accumulateSourceCompilationStringsWithVisitor( visitor );
	shSrcCode->accumulateSourceCompilationStringsWithVisitor( visitor );
	
	// Double-check the accummulation logic
	CCAssert(visitor->getSourceCompilationStringCount() == scCnt, "CC3Shader mismatch between sourceStringCount number of accumulated source stings");
	
	// Submit the source code strings to the compiler
	CC3OpenGL* gl = CC3OpenGL::sharedGL();
	gl->compileShader( getShaderID(), scCnt, (const GLchar**)scStrings );
	
	if ( !gl->getShaderWasCompiled( getShaderID() ) )
	{
		std::string srcCode = "\n";
		for (unsigned int i = 0; i < scCnt; i++)
		{
			srcCode += scStrings[i];
			srcCode += "\n";
		}
		
		CLoggers::sharedLoggers()->logMessage( CC3_LOG_ERROR, "============Shader [%s] was not compiled=============", getName().c_str() );
		CLoggers::sharedLoggers()->logMessageDirectly( CC3_LOG_ERROR, "ShaderCode follows " + srcCode );
		CLoggers::sharedLoggers()->logMessageDirectly( CC3_LOG_ERROR, getLocalizeCompileErrors( gl->getLogForShader( getShaderID() ), shSrcCode ).c_str() );

		CCAssert( false, "CC3Shader failed to compile" );
	}
	
	gl->setShaderDebugLabel( getName().c_str(), getShaderID() );

	for (unsigned int i = 0; i < scCnt; i++)
	{
		CC_SAFE_DELETE_ARRAY( scStrings[i] );
	}
	
	CC_SAFE_DELETE_ARRAY( scStrings );
}

void CC3Shader::compileFromSourceCodeString( const std::string& glslSource )
{
	compileFromSourceCode( CC3ShaderSourceCode::shaderSourceCodeWithName( getName(), glslSource ) );
}

CC3ShaderSourceCode* CC3Shader::getShaderPreamble()
{
	return m_pShaderPreamble;
}

void CC3Shader::setShaderPreamble( CC3ShaderSourceCode* shaderPreamble )
{
	CC_SAFE_RETAIN( shaderPreamble );
	CC_SAFE_RELEASE( m_pShaderPreamble );
	m_pShaderPreamble = shaderPreamble;
}

/**
 * Inserts a reference to the original source file and line number for each compile error found
 * in the specified error text.
 *
 * The compiler concatenates all of the shader source code into one long string when referencing line
 * numbers. However, the source code can originate from multiple imported source files. This method 
 * extracts the line number referenced in the shader compiler error log, determines which original
 * source file that line number refereneces, inserts the name of that source file and the corresponding
 * line number within that source file into the error text, and returns the modified error text.
 */
std::string CC3Shader::getLocalizeCompileErrors( const std::string& logTxt, CC3ShaderSourceCode* shSrcCode )
{
	// Platform-dependent content
	std::string fieldSeparatorStr = ":";
	std::string errIndicatorStr = "ERROR";
	unsigned int errIndicatorFieldIdx = 0;
	unsigned int lineNumFieldIdx = 2;

	// Create a new log string to contain the localized error text
	std::string localizedLogTxt = "";

	// Proceed line by line
	CLineScanner lineScanner;
	LineArray lines = lineScanner.scan( (char*)logTxt.c_str(), logTxt.size() );
	for (unsigned int i = 0; i < lines.size(); i++ )
	{ 
		const std::string& line = lines[i];
		bool isErrLine = false;

		LineArray fields;
		CC3String::splitString( line, fields, fieldSeparatorStr );
		
		unsigned int fieldCount = fields.size();
		for (unsigned int fieldIdx = 0; fieldIdx < fieldCount; fieldIdx++) 
		{
			const std::string& field = fields[fieldIdx];
			
			// Write the existing field out to the new log entry
			localizedLogTxt += field;
			
			std::string uppercasedStr = field;
			CC3String::makeUppercase( uppercasedStr );
			// If the first field contains the error indicator, this line is a compile error
			if (fieldIdx == errIndicatorFieldIdx && uppercasedStr.find( errIndicatorStr ) != std::string::npos )
				isErrLine = true;
			
			// If this line does contain an error, extract the global line number from the
			// third field, and localize it, first to the preamble, and then to the specified
			// shader source, and write the localized line number to the new log entry.
			if (isErrLine && fieldIdx == lineNumFieldIdx) 
			{
				CC3ShaderSourceCodeLineNumberLocalizingVisitor* visitor;
				visitor = CC3ShaderSourceCodeLineNumberLocalizingVisitor::visitorWithLineNumber( CC3String::toInteger(field) );
				if ( getShaderPreamble()->localizeLineNumberWithVisitor(visitor) ||
					shSrcCode->localizeLineNumberWithVisitor(visitor) )
					localizedLogTxt += CC3String::stringWithFormat( (char*)"(at %s)", "visitor"/*visitor->fullDescription().c_str()*/ );
			}
			
			// Write the log field separator at the end of each field
			localizedLogTxt += fieldSeparatorStr;
		}
		
		// Strip the last field separator and append a newline
		unsigned int fieldSepLen = fieldSeparatorStr.length();
		localizedLogTxt.replace( localizedLogTxt.length() - fieldSepLen, fieldSepLen, "" );
		localizedLogTxt += "\n";
	}
	
	return localizedLogTxt;
}


void CC3Shader::initWithTag( GLuint aTag, const std::string& aName )
{
	CCAssert(!aName.empty(), "CC3Shader cannot be created without a name");
	super::initWithTag( aTag, aName );
	{
		m_shaderID = 0;
		m_wasLoadedFromFile = false;
		setShaderPreamble( defaultShaderPreamble() );
	}
}

void CC3Shader::initFromSourceCode( CC3ShaderSourceCode* shSrcCode )
{
	super::initWithName( shSrcCode->getName() );
	
	compileFromSourceCode( shSrcCode );
}

CC3Shader* CC3Shader::shaderFromSourceCode( CC3ShaderSourceCode* shSrcCode )
{
	CC3Shader* shader = getShaderNamed( shSrcCode->getName() );
	if (shader) 
		return shader;
	
	shader = new CC3Shader;
	shader->initFromSourceCode( shSrcCode );
	shader->autorelease();
	addShader( shader );
	return shader;
}

bool CC3Shader::wasLoadedFromFile()
{
	return m_wasLoadedFromFile;
}

void CC3Shader::initWithName( const std::string& name, const std::string& glslSource )
{
	return initFromSourceCode( CC3ShaderSourceCode::shaderSourceCodeWithName( name, glslSource ) );
}

// We don't delegate to shaderFromShaderSource: by retrieving the shader source, because the
// shader source may have been dropped from its cache, even though the shader is still in its
// cache. The result would be to constantly create and cache the shader source unnecessarily.
CC3Shader* CC3Shader::shaderWithName( const std::string& name, const std::string& srcCodeString )
{
	CC3Shader* shader = getShaderNamed( name );
	if (shader) 
		return shader;
	
	shader = new CC3Shader;
	shader->initWithName( name, srcCodeString );
	shader->autorelease();
	addShader( shader );
	return shader;
}

void CC3Shader::initFromSourceCodeFile( const std::string& filePath )
{
	return initFromSourceCode( CC3ShaderSourceCode::shaderSourceCodeFromFile(filePath) );
}

// We don't delegate to shaderFromShaderSource: by retrieving the shader source, because the
// shader source may have been dropped from its cache, even though the shader is still in its
// cache. The result would be to constantly create and cache the shader source unnecessarily.
CC3Shader* CC3Shader::shaderFromSourceCodeFile( const std::string& filePath )
{
	CC3Shader* shader = getShaderNamed( CC3ShaderSourceCode::shaderSourceCodeNameFromFilePath( filePath ) );
	if (shader) 
		return shader;
	
	shader = new CC3Shader;
	shader->initFromSourceCodeFile( filePath );
	shader->autorelease();
	addShader( shader );
	return shader;
}

static GLuint _lastAssignedShaderTag = 0;

GLuint CC3Shader::nextTag()
{
	return ++_lastAssignedShaderTag; 
}

void CC3Shader::resetTagAllocation()
{
	_lastAssignedShaderTag = 0; 
}

void CC3Shader::remove()
{
	removeShader( this );
}

static CC3Cache* _shaderCache = NULL;

void CC3Shader::ensureCache()
{
	if ( !_shaderCache ) 
	{
		_shaderCache = CC3Cache::weakCacheForType( "shader" ); 
		_shaderCache->retain();
	}
}

void CC3Shader::addShader( CC3Shader* shader )
{
	ensureCache();
	_shaderCache->addObject( shader );
}

CC3Shader* CC3Shader::getShaderNamed( const std::string& name )
{
	ensureCache();
	return (CC3Shader*)(_shaderCache->getObjectNamed(name));
}

void CC3Shader::removeShader( CC3Shader* shader )
{ 
	ensureCache();
	_shaderCache->removeObject( shader );
}

void CC3Shader::removeShaderNamed( const std::string& name )
{
	ensureCache();
	_shaderCache->removeObjectNamed( name ); 
}

void CC3Shader::removeAllShaders()
{
	ensureCache();
	_shaderCache->removeAllObjects();
	CC_SAFE_RELEASE( _defaultShaderPreamble );
}

bool CC3Shader::isPreloading()
{
	return _shaderCache ? !_shaderCache->isWeak() : false; 
}

void CC3Shader::setIsPreloading( bool _isPreloading )
{
	ensureCache();
	_shaderCache->setIsWeak( !_isPreloading );
}

std::string CC3Shader::loadedShadersDescription()
{
	//NSMutableString* desc = [NSMutableString stringWithCapacity: 500];
	//NSArray* sortedCache = [_shaderCache objectsSortedByName];
	//for (CC3Shader* shdr in sortedCache) {
	//	if ( [shdr isKindOfClass: self] && shdr.wasLoadedFromFile )
	//		[desc appendFormat: @"\n\t%@", shdr.constructorDescription];
	//}
	//return desc;
	return "";
}

CC3VertexShader* CC3VertexShader::shaderFromSourceCodeFile( const std::string& filePath )
{
	CC3VertexShader* shader = (CC3VertexShader*)getShaderNamed( CC3ShaderSourceCode::shaderSourceCodeNameFromFilePath( filePath ) );
	if (shader) 
		return shader;

	shader = new CC3VertexShader;
	shader->initFromSourceCodeFile( filePath );
	shader->autorelease();

	CHECK_GL_ERROR_DEBUG();

	addShader( shader );
	
	CHECK_GL_ERROR_DEBUG();
	
	return shader;
}

GLenum CC3VertexShader::getShaderType()
{
	return GL_VERTEX_SHADER; 
}

// We don't delegate to shaderFromShaderSource: by retrieving the shader source, because the
// shader source may have been dropped from its cache, even though the shader is still in its
// cache. The result would be to constantly create and cache the shader source unnecessarily.
CC3VertexShader* CC3VertexShader::shaderWithName( const std::string& name, const std::string& srcCodeString )
{
	CC3VertexShader* shader = (CC3VertexShader*)getShaderNamed( name );
	if (shader) 
		return shader;

	shader = new CC3VertexShader;
	shader->initWithName( name, srcCodeString );
	shader->autorelease();
	addShader( shader );
	return shader;
}

CC3FragmentShader* CC3FragmentShader::shaderFromSourceCodeFile( const std::string& filePath )
{
	CC3FragmentShader* shader = (CC3FragmentShader*)getShaderNamed( CC3ShaderSourceCode::shaderSourceCodeNameFromFilePath( filePath ) );
	if (shader) 
		return shader;

	shader = new CC3FragmentShader;
	shader->initFromSourceCodeFile( filePath );
	shader->autorelease();

	CHECK_GL_ERROR_DEBUG();

	addShader( shader );

	CHECK_GL_ERROR_DEBUG();

	return shader;
}

GLenum CC3FragmentShader::getShaderType()
{
	return GL_FRAGMENT_SHADER; 
}

// We don't delegate to shaderFromShaderSource: by retrieving the shader source, because the
// shader source may have been dropped from its cache, even though the shader is still in its
// cache. The result would be to constantly create and cache the shader source unnecessarily.
CC3FragmentShader* CC3FragmentShader::shaderWithName( const std::string& name, const std::string& srcCodeString )
{
	CC3FragmentShader* shader = (CC3FragmentShader*)getShaderNamed( name );
	if (shader) 
		return shader;

	shader = new CC3FragmentShader;
	shader->initWithName( name, srcCodeString );
	shader->autorelease();
	addShader( shader );
	return shader;
}

CC3ShaderProgram::CC3ShaderProgram()
{
	m_attributes = NULL;
	m_uniformsSceneScope = NULL;
	m_uniformsNodeScope = NULL;
	m_uniformsDrawScope = NULL;
	m_pSemanticDelegate = NULL;
	m_programID = 0;
}

CC3ShaderProgram::~CC3ShaderProgram()
{
	remove();

	setVertexShader( NULL );		// use setter to detach shader from program
	setFragmentShader( NULL );		// use setter to detach shader from program
	setSemanticDelegate( NULL );

	deleteGLProgram();

	if ( m_attributes )
		m_attributes->removeAllObjects();
	CC_SAFE_RELEASE( m_attributes );

	if ( m_uniformsSceneScope )
		m_uniformsSceneScope->removeAllObjects();
	CC_SAFE_RELEASE( m_uniformsSceneScope );

	if ( m_uniformsNodeScope )
		m_uniformsNodeScope->removeAllObjects();
	CC_SAFE_RELEASE( m_uniformsNodeScope );

	if ( m_uniformsDrawScope )
		m_uniformsDrawScope->removeAllObjects();
	CC_SAFE_RELEASE( m_uniformsDrawScope );
}

static CC3Cache* _programCache = NULL;

void CC3ShaderProgram::ensureCache()
{
	if ( !_programCache ) 
	{
		_programCache = CC3Cache::weakCacheForType( "shader program" );
		_programCache->retain();
	}
}

GLuint CC3ShaderProgram::getProgramID()
{
	if ( !m_programID ) 
		m_programID = CC3OpenGL::sharedGL()->createShaderProgram();
	return m_programID;
}

void CC3ShaderProgram::deleteGLProgram()
{
	CC3OpenGL::sharedGL()->deleteShaderProgram( m_programID );
	m_programID = 0;
}

CC3VertexShader* CC3ShaderProgram::getVertexShader()
{
	return m_pVertexShader; 
}

void CC3ShaderProgram::setVertexShader( CC3VertexShader* vertexShader )
{
	if (vertexShader == m_pVertexShader) 
		return;
	
	detachShader( m_pVertexShader );
	
	CC_SAFE_RELEASE( m_pVertexShader );
	m_pVertexShader = vertexShader;
	CC_SAFE_RETAIN( vertexShader );
	
	attachShader( m_pVertexShader );
}

CC3FragmentShader* CC3ShaderProgram::getFragmentShader()
{
	return m_pFragmentShader; 
}

void CC3ShaderProgram::setFragmentShader( CC3FragmentShader* fragmentShader )
{
	if (fragmentShader == m_pFragmentShader) 
		return;
	
	detachShader( m_pFragmentShader );
	
	CC_SAFE_RELEASE( m_pFragmentShader );
	m_pFragmentShader = fragmentShader;
	CC_SAFE_RETAIN(m_pFragmentShader);

	attachShader( m_pFragmentShader );
}

GLint CC3ShaderProgram::getMaxUniformNameLength()
{
	return m_maxUniformNameLength;
}

GLint CC3ShaderProgram::getMaxAttributeNameLength()
{
	return m_maxAttributeNameLength;
}

CC3ShaderSemanticsDelegate* CC3ShaderProgram::getSemanticDelegate()
{
	return m_pSemanticDelegate;
}

void CC3ShaderProgram::setSemanticDelegate( CC3ShaderSemanticsDelegate* pDelegate )
{
	CC_SAFE_RELEASE( m_pSemanticDelegate );
	m_pSemanticDelegate = pDelegate;
	CC_SAFE_RETAIN( pDelegate );
}

void CC3ShaderProgram::attachShader( CC3Shader* shader )
{
	if ( shader )
		CC3OpenGL::sharedGL()->attachShader( shader->getShaderID(), getProgramID() );
}

void CC3ShaderProgram::detachShader( CC3Shader* shader )
{
	if ( shader )
		CC3OpenGL::sharedGL()->detachShader( shader->getShaderID(), getProgramID() );
}

GLuint CC3ShaderProgram::getUniformCount()
{
	return (GLuint)(m_uniformsSceneScope->count() + m_uniformsNodeScope->count() + m_uniformsDrawScope->count());
}

CCArray* CC3ShaderProgram::getUniforms()
{
	CCArray* uniforms = CCArray::createWithCapacity( getUniformCount() );
	uniforms->addObjectsFromArray( m_uniformsSceneScope );
	uniforms->addObjectsFromArray( m_uniformsNodeScope );
	uniforms->addObjectsFromArray( m_uniformsDrawScope );
	return uniforms;
}

GLuint CC3ShaderProgram::getUniformStorageElementCount()
{
	GLuint seCnt = 0;
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_uniformsSceneScope, pObj )
	{
		CC3GLSLUniform* var = (CC3GLSLUniform*)pObj;
		seCnt  += var->getStorageElementCount();
	}

	CCARRAY_FOREACH( m_uniformsNodeScope, pObj )
	{
		CC3GLSLUniform* var = (CC3GLSLUniform*)pObj;
		seCnt  += var->getStorageElementCount();
	}

	CCARRAY_FOREACH( m_uniformsDrawScope, pObj )
	{
		CC3GLSLUniform* var = (CC3GLSLUniform*)pObj;
		seCnt  += var->getStorageElementCount();
	}
	
	return seCnt;
}

CC3GLSLUniform* CC3ShaderProgram::getUniformNamed( const std::string& varName )
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_uniformsSceneScope, pObj )
	{
		CC3GLSLUniform* var = (CC3GLSLUniform*)pObj;
		if ( var->getName() == varName )
			return var;
	}

	CCARRAY_FOREACH( m_uniformsNodeScope, pObj )
	{
		CC3GLSLUniform* var = (CC3GLSLUniform*)pObj;
		if ( var->getName() == varName )
			return var;
	}

	CCARRAY_FOREACH( m_uniformsDrawScope, pObj )
	{
		CC3GLSLUniform* var = (CC3GLSLUniform*)pObj;
		if ( var->getName() == varName )
			return var;
	}

	return NULL;
}

CC3GLSLUniform* CC3ShaderProgram::getUniformAtLocation( GLint uniformLocation )
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_uniformsSceneScope, pObj )
	{
		CC3GLSLUniform* var = (CC3GLSLUniform*)pObj;
		if ( var->getLocation() == uniformLocation )
			return var;
	}

	CCARRAY_FOREACH( m_uniformsNodeScope, pObj )
	{
		CC3GLSLUniform* var = (CC3GLSLUniform*)pObj;
		if ( var->getLocation() == uniformLocation )
			return var;
	}

	CCARRAY_FOREACH( m_uniformsDrawScope, pObj )
	{
		CC3GLSLUniform* var = (CC3GLSLUniform*)pObj;
		if ( var->getLocation() == uniformLocation )
			return var;
	}

	return NULL;
}

CC3GLSLUniform* CC3ShaderProgram::getUniformForSemantic( GLenum semantic )
{
	return getUniformForSemantic( semantic, 0 );
}

CC3GLSLUniform* CC3ShaderProgram::getUniformForSemantic( GLenum semantic, GLuint semanticIndex )
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_uniformsSceneScope, pObj )
	{
		CC3GLSLUniform* var = (CC3GLSLUniform*)pObj;
		if ( var->getSemantic() == semantic && var->getSemanticIndex() == semanticIndex )
			return var;
	}

	CCARRAY_FOREACH( m_uniformsNodeScope, pObj )
	{
		CC3GLSLUniform* var = (CC3GLSLUniform*)pObj;
		if ( var->getSemantic() == semantic && var->getSemanticIndex() == semanticIndex )
			return var;
	}

	CCARRAY_FOREACH( m_uniformsDrawScope, pObj )
	{
		CC3GLSLUniform* var = (CC3GLSLUniform*)pObj;
		if ( var->getSemantic() == semantic && var->getSemanticIndex() == semanticIndex )
			return var;
	}
	
	return NULL;
}

GLuint CC3ShaderProgram::getAttributeCount()
{
	return (GLuint)m_attributes->count(); 
}

CC3GLSLAttribute* CC3ShaderProgram::getAttributeNamed( const std::string& varName )
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_attributes, pObj )
	{
		CC3GLSLAttribute* var = (CC3GLSLAttribute*)pObj;
		if ( var->getName() == varName )
			return var;

	}
	
	return NULL;
}

CC3GLSLAttribute* CC3ShaderProgram::getAttributeAtLocation( GLint attrLocation )
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_attributes, pObj )
	{
		CC3GLSLAttribute* var = (CC3GLSLAttribute*)pObj;
		if ( var->getLocation() == attrLocation )
			return var;
	}

	return NULL;
}

CC3GLSLAttribute* CC3ShaderProgram::getAttributeForSemantic( GLenum semantic )
{
	return getAttributeForSemantic( semantic, 0 );
}

CC3GLSLAttribute* CC3ShaderProgram::getAttributeForSemantic( GLenum semantic, GLuint semanticIndex )
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_attributes, pObj )
	{
		CC3GLSLAttribute* var = (CC3GLSLAttribute*)pObj;
		if ( var->getSemantic() == semantic && var->getSemanticIndex() == semanticIndex )
			return var;
	}

	return NULL;
}

void CC3ShaderProgram::markSceneScopeDirty()
{
	m_isSceneScopeDirty = true; 
}

void CC3ShaderProgram::willBeginDrawingScene()
{
	markSceneScopeDirty(); 
}

void CC3ShaderProgram::setWillBeginDrawingScene()
{
	if ( _programCache == NULL )
		return;

	CCDictionary* shaders = _programCache->getAllObjects();
	CCArray* keys = shaders->allKeys();
	if ( keys == NULL )
		return;

	for (unsigned int i = 0; i < keys->count(); i++)
	{
		CCString* key = (CCString*)keys->objectAtIndex( i );
		if ( key )
		{
			CC3ShaderProgram* program = ( CC3ShaderProgram* )_programCache->getObjectNamed( key->getCString() );
			if ( program ) 
				program->willBeginDrawingScene();
		}
	}
}

void CC3ShaderProgram::resetGLState()
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_uniformsSceneScope, pObj )
	{
		CC3GLSLUniform* var = (CC3GLSLUniform*)pObj;
		var->setIsGLStateKnown( false );
	}

	CCARRAY_FOREACH( m_uniformsNodeScope, pObj )
	{
		CC3GLSLUniform* var = (CC3GLSLUniform*)pObj;
		var->setIsGLStateKnown( false );
	}

	CCARRAY_FOREACH( m_uniformsDrawScope, pObj )
	{
		CC3GLSLUniform* var = (CC3GLSLUniform*)pObj;
		var->setIsGLStateKnown( false );
	}
}

bool CC3ShaderProgram::shouldAllowDefaultVariableValues()
{
	return m_shouldAllowDefaultVariableValues;
}

void CC3ShaderProgram::setShouldAllowDefaultVariableValues( bool shouldAllow )
{
	m_shouldAllowDefaultVariableValues = shouldAllow;
}

static bool _defaultShouldAllowDefaultVariableValues = false;

bool CC3ShaderProgram::defaultShouldAllowDefaultVariableValues()
{
	return _defaultShouldAllowDefaultVariableValues; 
}

void CC3ShaderProgram::setDefaultShouldAllowDefaultVariableValues( bool shouldAllow )
{
	_defaultShouldAllowDefaultVariableValues = shouldAllow;
}

GLuint CC3ShaderProgram::getTexture2DStart()
{
	return 0; 
}

GLuint CC3ShaderProgram::getTextureCubeStart()
{
	return m_texture2DCount; 
}

GLuint CC3ShaderProgram::getTexture2DCount()
{
	return m_texture2DCount;
}

GLuint CC3ShaderProgram::getTextureCubeCount()
{
	return m_textureCubeCount;
}

GLuint CC3ShaderProgram::getTextureLightProbeStart()
{
	return m_texture2DCount + m_textureCubeCount; 
}

GLuint CC3ShaderProgram::getTextureLightProbeCount()
{
	return m_textureLightProbeCount;
}

void CC3ShaderProgram::link()
{
	CCAssert(m_pVertexShader && m_pFragmentShader, "CC3Shader requires both vertex and fragment shaders to be assigned before linking.");
	CCAssert(m_pSemanticDelegate, "CC3Shader requires the semanticDelegate property be set before linking.");
	
	CC3OpenGL* gl = CC3OpenGL::sharedGL();
	gl->linkShaderProgram( getProgramID() );
	
	if ( !gl->getShaderProgramWasLinked( getProgramID() ) )
	{
		CCLOGERROR( "[shd]CC3Shader could not be linked because:\n%s", gl->getLogForShaderProgram(getProgramID()).c_str() );
		CCAssert( false, "Shader is not linked" );
		return;
	}

	gl->setProgramDebugLabel( getName().c_str(), getProgramID() );
	
	configureUniforms();
	configureAttributes();
}

/**
 * Extracts information about the program uniform variables from the GL engine
 * and creates a configuration instance for each.
 */
void CC3ShaderProgram::configureUniforms()
{
	clearUniforms();

	CC3OpenGL* gl = CC3OpenGL::sharedGL();
	GLuint progID = getProgramID();
	GLint varCnt = gl->getIntegerParameterForShaderProgram( GL_ACTIVE_UNIFORMS, progID );
	m_maxUniformNameLength = gl->getIntegerParameterForShaderProgram( GL_ACTIVE_UNIFORM_MAX_LENGTH, progID );
	for (GLint varIdx = 0; varIdx < varCnt; varIdx++) 
	{
		CC3GLSLUniform* var = CC3GLSLUniform::variableInProgram( this, varIdx );
		CCAssert(var->getLocation() >= 0, "CC3Shader has an invalid location. Make sure the maximum number of program uniforms for this platform has not been exceeded.");
		if (var->getSemantic() != kCC3SemanticRedundant) 
		{
			configureUniform( var );
			addUniform( var );
		} 
		else 
		{
			CC3_TRACE("[shd]CC3GLSLUniform is redundant and was not added to CC3Shader");
		}
	}
}

void CC3ShaderProgram::clearUniforms()
{
	m_uniformsSceneScope->removeAllObjects();
	m_uniformsNodeScope->removeAllObjects();
	m_uniformsDrawScope->removeAllObjects();
	m_texture2DCount = 0;
	m_textureCubeCount = 0;
	m_textureLightProbeCount = 0;
}

/** Let the delegate configure the uniform, and then update the texture counts. */
void CC3ShaderProgram::configureUniform( CC3GLSLUniform* var )
{
	m_pSemanticDelegate->configureVariable( var );
	
	if (var->getSemantic() == kCC3SemanticTextureSampler) 
		m_texture2DCount += var->getSize();
	if (var->getSemantic() == kCC3SemanticTexture2DSampler) 
		m_texture2DCount += var->getSize();
	if (var->getSemantic() == kCC3SemanticTextureCubeSampler) 
		m_textureCubeCount += var->getSize();
	if (var->getSemantic() == kCC3SemanticTextureLightProbeSampler) 
		m_textureLightProbeCount += var->getSize();
}

/** Adds the specified uniform to the appropriate internal collection, based on variable scope. */
void CC3ShaderProgram::addUniform( CC3GLSLUniform* var )
{
	switch (var->getScope()) 
	{
		case kCC3GLSLVariableScopeScene:
			m_uniformsSceneScope->addObject(var);
			return;
		case kCC3GLSLVariableScopeDraw:
			m_uniformsDrawScope->addObject(var);
			return;
		default:
			m_uniformsNodeScope->addObject(var);
			return;
	}
}

/**
 * Extracts information about the program vertex attribute variables from the GL engine
 * and creates a configuration instance for each.
 */
void CC3ShaderProgram::configureAttributes()
{
	clearAttributes();
	
	CC3OpenGL* gl = CC3OpenGL::sharedGL();
	GLuint progID = getProgramID();
	GLint varCnt = gl->getIntegerParameterForShaderProgram( GL_ACTIVE_ATTRIBUTES, progID );
	m_maxAttributeNameLength = gl->getIntegerParameterForShaderProgram( GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, progID );
	for (GLint varIdx = 0; varIdx < varCnt; varIdx++) 
	{
		CC3GLSLAttribute* var = CC3GLSLAttribute::variableInProgram( this, varIdx );
		CCAssert(var->getLocation() >= 0, "CC3GLSLAttribute has an invalid location. Make sure the maximum number of program attributes for this platform has not been exceeded.");
		if (var->getSemantic() != kCC3SemanticRedundant) 
		{
			configureAttribute( var );
			addAttribute( var );
		} else {
			CC3_TRACE("[shd]CC3GLSLAttribute is redundant and was not added to CC3Shader");
		}
	}
}

void CC3ShaderProgram::clearAttributes()
{
	m_attributes->removeAllObjects(); 
}

/** Let the delegate configure the attribute. */
void CC3ShaderProgram::configureAttribute( CC3GLSLAttribute* var )
{
	m_pSemanticDelegate->configureVariable(var); 
}

/** Adds the specified attribute to the internal collection. */
void CC3ShaderProgram::addAttribute( CC3GLSLAttribute* var )
{
	m_attributes->addObject(var); 
}

void CC3ShaderProgram::prewarm()
{
	CC3OpenGL* gl = CC3OpenGL::sharedGL();
	if ( !gl->isRenderingContext() || isPreloading()) 
	{
		gl->getShaderProgramPrewarmer()->prewarmShaderProgram( this );
	}
}

void CC3ShaderProgram::bindWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	CC3OpenGL* gl = visitor->getGL();
	gl->useShaderProgram( getProgramID() );
	gl->clearUnboundVertexAttributes();
	populateVertexAttributesWithVisitor( visitor );
	gl->enableBoundVertexAttributes();
	populateNodeScopeUniformsWithVisitor( visitor );
}

void CC3ShaderProgram::populateVertexAttributesWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	CC3OpenGL* gl = visitor->getGL();

	CCObject* pObj = NULL;
	CCARRAY_FOREACH ( m_attributes, pObj )
	{
		CC3GLSLAttribute* var = (CC3GLSLAttribute*)pObj;
		gl->bindVertexAttribute( var, visitor );
	}
}

void CC3ShaderProgram::populateSceneScopeUniformsWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	if ( m_isSceneScopeDirty ) 
	{
		populateUniforms( m_uniformsSceneScope, visitor );
		m_isSceneScopeDirty = false;
	}
}

void CC3ShaderProgram::populateNodeScopeUniformsWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	populateSceneScopeUniformsWithVisitor( visitor );
	populateUniforms( m_uniformsNodeScope, visitor );
}

void CC3ShaderProgram::populateDrawScopeUniformsWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	populateUniforms( m_uniformsDrawScope, visitor );
}

void CC3ShaderProgram::populateUniforms( CCArray* uniforms, CC3NodeDrawingVisitor* visitor )
{
	CC3ShaderContext* progCtx = visitor->getCurrentMeshNode()->getShaderContext();

	CCObject* pObj = NULL;
	CCARRAY_FOREACH ( uniforms, pObj )
	{
		CC3GLSLUniform* var = (CC3GLSLUniform*)pObj;
		bool wasSet = (progCtx->populateUniform(var, visitor) ||
					   m_pSemanticDelegate->populateUniform(var, visitor));

		if ( !wasSet ) 
		{
			CCAssert(shouldAllowDefaultVariableValues(),
					  "CC3Shader could not resolve the value of uniform %s with semantic %s."
					  " If this is a valid uniform, you should create a uniform override in the"
					  " shader context in your mesh node to set the value of the uniform directly."
					  " Or you can allow a default uniform value to be used, and avoid this message,"
					  " by setting the shouldAllowDefaultVariableValues property of the shader program to YES."/*,
					  var->getName().c_str(), stringFromCC3Semantic(var->getSemantic()).c_str()*/);
		}
		
		var->updateGLValueWithVisitor( visitor );
	}
}

void CC3ShaderProgram::initWithTag( GLuint aTag, const std::string& aName )
{
	CCAssert(!aName.empty(), "CC3Shader cannot be created without a name");
	super::initWithTag( aTag, aName );
	{
		m_attributes = CCArray::create();				// retained
		m_attributes->retain();
		m_uniformsSceneScope = CCArray::create();		// retained
		m_uniformsSceneScope->retain();
		m_uniformsNodeScope = CCArray::create();		// retained
		m_uniformsNodeScope->retain();
		m_uniformsDrawScope = CCArray::create();		// retained
		m_uniformsDrawScope->retain();
		m_pVertexShader = NULL;
		m_pFragmentShader = NULL;
		m_maxUniformNameLength = 0;
		m_maxAttributeNameLength = 0;
		m_texture2DCount = 0;
		m_textureCubeCount = 0;
		m_textureLightProbeCount = 0;
		m_isSceneScopeDirty = true;	// start out dirty for auto-loaded programs
		m_pSemanticDelegate = NULL;
		m_shouldAllowDefaultVariableValues = defaultShouldAllowDefaultVariableValues();
	}
}

void CC3ShaderProgram::initWithVertexShader( CC3VertexShader* vertexShader, CC3FragmentShader* fragmentShader )
{
	return initWithSemanticDelegate( getShaderMatcher()->getSemanticDelegate(), vertexShader, fragmentShader );
}

CC3ShaderProgram* CC3ShaderProgram::programWithVertexShader( CC3VertexShader* vertexShader, CC3FragmentShader* fragmentShader )
{
	return programWithSemanticDelegate( getShaderMatcher()->getSemanticDelegate(), vertexShader, fragmentShader );
}

void CC3ShaderProgram::initFromVertexShaderFile( const std::string& vshFilePath, const std::string& fshFilePath )
{
	return initWithSemanticDelegate( getShaderMatcher()->getSemanticDelegate(), vshFilePath, fshFilePath );
}

CC3ShaderProgram* CC3ShaderProgram::programFromVertexShaderFile( const std::string& vshFilePath, const std::string& fshFilePath )
{
	return programWithSemanticDelegate( getShaderMatcher()->getSemanticDelegate(), vshFilePath, fshFilePath );
}

void CC3ShaderProgram::initWithSemanticDelegate( CC3ShaderSemanticsDelegate* semanticDelegate, CC3VertexShader* vertexShader, CC3FragmentShader* fragmentShader )
{
	std::string progName = programNameFromVertexShaderName( vertexShader->getName(), fragmentShader->getName() );
	initWithName( progName );
	setSemanticDelegate( semanticDelegate );
	setVertexShader( vertexShader );
	setFragmentShader( fragmentShader );
	link();
	prewarm();
}

CC3ShaderProgram* CC3ShaderProgram::programWithSemanticDelegate( CC3ShaderSemanticsDelegate* semanticDelegate, CC3VertexShader* vertexShader, CC3FragmentShader* fragmentShader )
{
	std::string progName = programNameFromVertexShaderName( vertexShader->getName(), fragmentShader->getName() );
	CC3ShaderProgram* program = getProgramNamed(progName);
	if (program) 
		return program;
	
	program = new CC3ShaderProgram; 
	program->initWithSemanticDelegate( semanticDelegate, vertexShader, fragmentShader );
	program->autorelease();
	addProgram( program );
	
	return program;
}

void CC3ShaderProgram::initWithSemanticDelegate( CC3ShaderSemanticsDelegate* semanticDelegate, const std::string& vshFilePath, const std::string& fshFilePath )
{
	return initWithSemanticDelegate( semanticDelegate, CC3VertexShader::shaderFromSourceCodeFile(vshFilePath),
						CC3FragmentShader::shaderFromSourceCodeFile(fshFilePath));
}

CC3ShaderProgram* CC3ShaderProgram::programWithSemanticDelegate( CC3ShaderSemanticsDelegate* semanticDelegate, const std::string& vshFilePath, const std::string& fshFilePath )
{
	return programWithSemanticDelegate( semanticDelegate, CC3VertexShader::shaderFromSourceCodeFile(vshFilePath),
		CC3FragmentShader::shaderFromSourceCodeFile(fshFilePath));
}

std::string CC3ShaderProgram::programNameFromVertexShaderName( const std::string& vertexShaderName, const std::string& fragmentShaderName )
{
	return CC3String::stringWithFormat( (char*)"%s-%s", vertexShaderName.c_str(), fragmentShaderName.c_str() );
}

bool CC3ShaderProgram::wasLoadedFromFile()
{
	return m_pVertexShader->wasLoadedFromFile() && m_pFragmentShader->wasLoadedFromFile();
}

static GLuint _lastAssignedProgramTag = 0;

GLuint CC3ShaderProgram::nextTag()
{
	return ++_lastAssignedProgramTag; 
}

void CC3ShaderProgram::resetTagAllocation()
{
	_lastAssignedProgramTag = 0; 
}

void CC3ShaderProgram::remove()
{
	removeProgram( this ); 
}

void CC3ShaderProgram::addProgram( CC3ShaderProgram* program )
{
	if ( !isPreloading() ) 
	{
		CC3_TRACE("[shd]CC3ShaderProgram is likely being compiled and linked outside of a pre-load stage, which"
				"	may cause a short, unexpected pause in the drawing of the scene, while the"
				"	final stages of compilation and configuration are carried out. If this is"
				"	the case, consider pre-loading this shader program during scene initialization."
				"	See the notes of the CC3ShaderProgram setIsPreloading: class-side method for more information"
				"	on pre-loading.");
	}

	ensureCache();
	_programCache->addObject( program );
	
	// If appropriate, ensure that a matching pure-color program is added as well.
	if ( isPreloading() && shouldAutomaticallyPreloadMatchingPureColorPrograms() )
		getShaderMatcher()->getPureColorProgramMatching( program );
}

CC3ShaderProgram* CC3ShaderProgram::getProgramNamed( const std::string& name )
{
	ensureCache();
	return (CC3ShaderProgram*)(_programCache->getObjectNamed(name));
}

void CC3ShaderProgram::removeProgram( CC3ShaderProgram* program )
{
	if ( _programCache )
		_programCache->removeObject(program); 
}

void CC3ShaderProgram::removeProgramNamed( const std::string& name )
{
	if ( _programCache )
		_programCache->removeObjectNamed( name ); 
}

void CC3ShaderProgram::removeAllPrograms()
{
	if ( _programCache )
		_programCache->removeAllObjects();

	setShaderMatcher( NULL );

	CC_SAFE_RELEASE( _shaderCache );
	CC_SAFE_RELEASE( _programCache );
}

static bool _shouldAutomaticallyPreloadMatchingPureColorPrograms = true;

bool CC3ShaderProgram::shouldAutomaticallyPreloadMatchingPureColorPrograms()
{
	return _shouldAutomaticallyPreloadMatchingPureColorPrograms; 
}

void CC3ShaderProgram::setShouldAutomaticallyPreloadMatchingPureColorPrograms( bool shouldAdd )
{
	_shouldAutomaticallyPreloadMatchingPureColorPrograms = shouldAdd;
}

bool CC3ShaderProgram::isPreloading()
{
	return _programCache ? !_programCache->isWeak() : false; 
}

void CC3ShaderProgram::setIsPreloading( bool _isPreloading )
{
	ensureCache();
	_programCache->setIsWeak( !_isPreloading );
}

std::string CC3ShaderProgram::loadedProgramsDescription()
{
	//NSMutableString* desc = [NSMutableString stringWithCapacity: 500];
	//NSArray* sortedCache = [_programCache objectsSortedByName];
	//for (CC3ShaderProgram* prog in sortedCache) {
	//	if ( [prog isKindOfClass: self] && prog.wasLoadedFromFile )
	//		[desc appendFormat: @"\n\t%@", prog.constructorDescription];
	//}
	//return desc;
	return "";
}

static CC3ShaderMatcher* _shaderMatcher = NULL;

CC3ShaderMatcher* CC3ShaderProgram::getShaderMatcher()
{
	if ( !_shaderMatcher ) 
	{
		CC3ShaderMatcher* pMatcher = new CC3ShaderMatcherBase;
		pMatcher->init();
		pMatcher->autorelease();

		setShaderMatcher( pMatcher );

		CHECK_GL_ERROR_DEBUG();
	}
	return _shaderMatcher;
}

void CC3ShaderProgram::setShaderMatcher( CC3ShaderMatcher* shaderMatcher )
{
	if (shaderMatcher == _shaderMatcher) 
		return;
	CC_SAFE_RELEASE( _shaderMatcher );
	_shaderMatcher = shaderMatcher;
	CC_SAFE_RETAIN( _shaderMatcher );
}

std::string CC3ShaderProgram::fullDescription()
{
	return "CC3ShaderProgram: " + m_sName;
}

CC3ShaderSourceCode::CC3ShaderSourceCode()
{

}

CC3ShaderSourceCode::~CC3ShaderSourceCode()
{
	remove();
}

GLuint CC3ShaderSourceCode::getLineCount()
{
	CCAssert( false, "getLineCount() is not implemented" );
	return 0;
}

GLuint CC3ShaderSourceCode::getSourceStringCount()
{
	CC3ShaderSourceCodeCompilationStringCountVisitor* visitor = CC3ShaderSourceCodeCompilationStringCountVisitor::visitor();
	accumulateSourceCompilationStringCountWithVisitor(visitor);
	return visitor->getSourceCompilationStringCount();
}

std::string CC3ShaderSourceCode::getSourceCodeString()
{
	CCAssert( false, "getSourceCodeString() is not implemented" );
	return NULL;
}

std::string CC3ShaderSourceCode::getImportableSourceCodeString()
{
	return (wasLoadedFromFile()
				? CC3String::stringWithFormat( (char*)"#import \"%s\"\n", getName().c_str() )
				: getSourceCodeString());
}

CCArray* CC3ShaderSourceCode::getSubsections()
{
	return NULL; 
}

void CC3ShaderSourceCode::appendSourceCodeString( const std::string& srcCode )
{
	CCAssert( false, "appendSourceCodeString is not implemented" ); 
}

void CC3ShaderSourceCode::accumulateSourceCompilationStringsWithVisitor( CC3ShaderSourceCodeCompilationStringVisitor* visitor )
{
	CCAssert( false, "accumulateSourceCompilationStringsWithVisitor is not implemented");
}

void CC3ShaderSourceCode::accumulateSourceCompilationStringCountWithVisitor( CC3ShaderSourceCodeCompilationStringCountVisitor* visitor )
{
	CCAssert( false, "accumulateSourceCompilationStringCountWithVisitor is not implemented");
}

bool CC3ShaderSourceCode::localizeLineNumberWithVisitor( CC3ShaderSourceCodeLineNumberLocalizingVisitor* visitor )
{
	GLuint lineNumber = visitor->getLineNumber();
	GLuint lineCount = getLineCount();
	if (lineNumber <= lineCount) {
		return true;
	} else {
		lineNumber -= lineCount;
		visitor->setLineNumber( lineNumber );
		visitor->addLineNumberOffset( lineCount );
		return false;
	}
}

void CC3ShaderSourceCode::initWithTag( GLuint tag, const std::string& name )
{
	CCAssert(!name.empty(), "CC3ShaderSourceCode cannot be created without a name");
	super::initWithTag(tag, name);
	m_wasLoadedFromFile = false;
}

CC3ShaderSourceCode* CC3ShaderSourceCode::shaderSourceCodeWithName( const std::string& name, const std::string& srcCodeString )
{
	CC3ShaderSourceCode* shSrc = getShaderSourceCodeNamed( name );
	if (shSrc) 
		return shSrc;

	CC3ShaderSourceCodeGroup* srcGroup = new CC3ShaderSourceCodeGroup;
	srcGroup->initWithName( name );
	srcGroup->autorelease();
	
	CC3ShaderSourceCode* srcSection = NULL;

	GLuint lineCount = 1;			// Running count of the lines parsed
	GLuint sectionCount = 0;		// Running count of the number of compilable sections within this file.

	CLineScanner lineScanner;
	LineArray srcLines = lineScanner.scan( (char*)srcCodeString.c_str(), srcCodeString.size() );
	for (unsigned int i = 0; i < srcLines.size(); i++ )
	{
		const std::string& srcLine = srcLines[i];

		if ( lineContainsImportDirective( srcLine ) )
		{
 			LineArray importLineComponents;
			CC3String::splitString( srcLine, importLineComponents, "\"" );
			CCAssert(importLineComponents.size() > 1, "Shader source file %s missing import target at line %d"/*, name, lineCount*/);

			// Add the source code read since the previous import directive
			srcGroup->addSubsection( srcSection );
			srcSection = NULL;

			// Add the source code from the imported file
			std::string importFileName = importLineComponents[1];
			srcGroup->addSubsection( shaderSourceCodeFromFile(importFileName));
		}
		else 
		{
			if ( !srcSection ) 
			{
				std::string sectionName = name + CC3String::stringWithFormat( (char*)"-Section-%d", ++sectionCount );
				srcSection = new CC3ShaderSourceCodeString; 
				srcSection->initWithName( sectionName );
				srcSection->autorelease();
			}
			srcSection->appendSourceCodeString( srcLine );
		}
		lineCount++;
	}
	
	// Add the source code read since the final import directive
	srcGroup->addSubsection( srcSection );
	
	// Add extracted source code group to the cache and return it.
	addShaderSourceCode( srcGroup );
	//
	//LogRez(@"Parsed GLSL source named %@ in %.3f ms", name, GetRezActivityDuration() * 1000);
	//
	return srcGroup;
}

/** Returns whether the specified source code line contains an #import or #include directive. */
bool CC3ShaderSourceCode::lineContainsImportDirective( const std::string& srcLine )
{
	std::string strLine = srcLine;
	std::string trimmedLine = CC3String::trim( strLine );
	bool bRet = CC3String::startsWith( trimmedLine.c_str(), "#import" );
	bRet |= CC3String::startsWith( trimmedLine.c_str(), "#include" );

	return bRet;
}

CC3ShaderSourceCode* CC3ShaderSourceCode::shaderSourceCodeFromFile( const std::string& filePath )
{
	std::string shSrcName = shaderSourceCodeNameFromFilePath( filePath );
	CC3ShaderSourceCode* shSrc = getShaderSourceCodeNamed( shSrcName );
	if (shSrc) 
		return shSrc;
	
	std::string absFilePath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filePath.c_str()); 
	CCAssert(!absFilePath.empty(), "Could not locate GLSL file '%s' in either the application resources or the Cocos3D library resources"/*, filePath.c_str()*/);

	CCString* pStr = CCString::createWithContentsOfFile( absFilePath.c_str() );
	shSrc = shaderSourceCodeWithName( shSrcName, pStr->getCString() );
	shSrc->setWasLoadedFromFile( true );
	/// Done here
	pStr->release();
	
	return shSrc;
}
//
std::string CC3ShaderSourceCode::shaderSourceCodeNameFromFilePath( const std::string& filePath )
{
	return CC3String::getFileName( filePath );
}

void CC3ShaderSourceCode::remove()
{
	removeShaderSourceCode( this ); 
}

static CC3Cache* _shaderSourceCodeCache = NULL;

void CC3ShaderSourceCode::ensureCache()
{
	if ( !_shaderSourceCodeCache ) 
	{
		_shaderSourceCodeCache = CC3Cache::weakCacheForType( "shader source" );
		_shaderSourceCodeCache->retain();
	}
}

void CC3ShaderSourceCode::addShaderSourceCode( CC3ShaderSourceCode* shSrcCode )
{
	ensureCache();
	_shaderSourceCodeCache->addObject( shSrcCode );
}

CC3ShaderSourceCode* CC3ShaderSourceCode::getShaderSourceCodeNamed( const std::string& name )
{
	ensureCache();
	return (CC3ShaderSourceCode*)(_shaderSourceCodeCache->getObjectNamed(name));
}

void CC3ShaderSourceCode::removeShaderSourceCode( CC3ShaderSourceCode* shSrcCode )
{
	_shaderSourceCodeCache->removeObject(shSrcCode); 
}

void CC3ShaderSourceCode::removeShaderSourceCodeNamed( const std::string& name )
{
	_shaderSourceCodeCache->removeObjectNamed( name ); 
}

void CC3ShaderSourceCode::removeAllShaderSourceCode()
{
	_shaderSourceCodeCache->removeAllObjects();
	CC_SAFE_RELEASE( _shaderSourceCodeCache );
}

bool CC3ShaderSourceCode::isPreloading()
{
	return _shaderSourceCodeCache ? !_shaderSourceCodeCache->isWeak() : false; 
}

void CC3ShaderSourceCode::setIsPreloading( bool _isPreloading )
{
	ensureCache();
	_shaderSourceCodeCache->setIsWeak( !_isPreloading );
}

std::string CC3ShaderSourceCode::loadedShaderSourceCodeDescription()
{
	//NSMutableString* desc = [NSMutableString stringWithCapacity: 500];
	//NSArray* sortedCache = [_shaderSourceCodeCache objectsSortedByName];
	//for (CC3ShaderSourceCode* shSrc in sortedCache) {
	//	if ( [shSrc isKindOfClass: self] && shSrc.wasLoadedFromFile )
	//		[desc appendFormat: @"\n\t%@", shSrc.constructorDescription];
	//}
	//return desc;
	return "";
}

void CC3ShaderSourceCode::setWasLoadedFromFile( bool fromFile )
{
	m_wasLoadedFromFile = fromFile;
}

bool CC3ShaderSourceCode::wasLoadedFromFile()
{
	return m_wasLoadedFromFile;
}

std::string CC3ShaderSourceCodeString::getSourceCodeString()
{
	return m_sourceCodeString; 
}

GLuint CC3ShaderSourceCodeString::getLineCount()
{
	// return (GLuint)_sourceCodeString.lineCount; 
	return 0;
}

void CC3ShaderSourceCodeString::appendSourceCodeString( const std::string& srcCode )
{
	m_sourceCodeString += srcCode; 
	m_sourceCodeString += "\n";
}

void CC3ShaderSourceCodeString::accumulateSourceCompilationStringsWithVisitor( CC3ShaderSourceCodeCompilationStringVisitor* visitor )
{
	if ( visitor->hasAlreadyVisited( this ) ) 
		return;

	visitor->addSourceCompilationString( m_sourceCodeString.c_str() );
}

void CC3ShaderSourceCodeString::accumulateSourceCompilationStringCountWithVisitor( CC3ShaderSourceCodeCompilationStringCountVisitor* visitor )
{
	if ( visitor->hasAlreadyVisited( this ) ) 
		return;

	visitor->addSourceCompilationStringCount( 1 );
}

void CC3ShaderSourceCodeString::initWithTag( GLuint tag, const std::string& name )
{
	super::initWithTag( tag, name );
	m_sourceCodeString = "";
}

CC3ShaderSourceCodeLines::CC3ShaderSourceCodeLines()
{
	m_sourceCodeLines = NULL;
}

CC3ShaderSourceCodeLines::~CC3ShaderSourceCodeLines()
{
	if ( m_sourceCodeLines )
		m_sourceCodeLines->removeAllObjects();

	CC_SAFE_RELEASE( m_sourceCodeLines );
}


std::string CC3ShaderSourceCodeLines::getSourceCodeString()
{
	/*NSMutableString* srcCodeString = [NSMutableString stringWithCapacity: 1000];
	for (NSData* srcLine in _sourceCodeLines)
		[srcCodeString appendString: [NSString stringWithUTF8String: srcLine.bytes]];
	return srcCodeString;*/
	return "";
}

GLuint CC3ShaderSourceCodeLines::getLineCount()
{
	return (GLuint)m_sourceCodeLines->count(); 
}

void CC3ShaderSourceCodeLines::appendSourceCodeString( const std::string& srcCode )
{
	//_sourceCodeLines addObject: [NSData dataWithBytes: srcCode.UTF8String
	//											length: (srcCode.length + 1)]];	// Plus null-terminator
}


void CC3ShaderSourceCodeLines::accumulateSourceCompilationStringsWithVisitor( CC3ShaderSourceCodeCompilationStringVisitor* visitor )
{
	if ( visitor->hasAlreadyVisited(this) ) 
		return;
	
	/*for (NSData* srcLine in _sourceCodeLines) 
		[visitor addSourceCompilationString: srcLine.bytes];*/
}

void CC3ShaderSourceCodeLines::accumulateSourceCompilationStringCountWithVisitor( CC3ShaderSourceCodeCompilationStringCountVisitor* visitor )
{
	if ( visitor->hasAlreadyVisited(this) ) 
		return;
	
	visitor->addSourceCompilationStringCount( (GLuint)m_sourceCodeLines->count() );
}

void CC3ShaderSourceCodeLines::initWithTag( GLuint tag, const std::string& name )
{
	super::initWithTag( tag, name );
	m_sourceCodeLines = CCArray::create();	// retained
	m_sourceCodeLines->retain();
}

CC3ShaderSourceCodeGroup::CC3ShaderSourceCodeGroup()
{
	m_subsections = NULL;
}

CC3ShaderSourceCodeGroup::~CC3ShaderSourceCodeGroup()
{
	if ( m_subsections )
		m_subsections->removeAllObjects();

	CC_SAFE_RELEASE( m_subsections );
}

CCArray* CC3ShaderSourceCodeGroup::getSubsections()
{
	return m_subsections; 
}

void CC3ShaderSourceCodeGroup::addSubsection( CC3ShaderSourceCode* shSrcCode )
{
	if (shSrcCode) 
		m_subsections->addObject( shSrcCode );
}

GLuint CC3ShaderSourceCodeGroup::getLineCount() 
{
	GLuint lineCnt = 0;

	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_subsections, pObj )
	{
		CC3ShaderSourceCode* shSrc = (CC3ShaderSourceCode*)pObj;
		lineCnt += shSrc->getLineCount();
	}
	
	return lineCnt;
}

std::string CC3ShaderSourceCodeGroup::getSourceCodeString()
{
	/*NSMutableString* srcCode = [NSMutableString stringWithCapacity: 500];
	for (CC3ShaderSourceCode* shSrc in _subsections)
	[srcCode appendString: shSrc.importableSourceCodeString];
	return srcCode;*/
	return "";
}

void CC3ShaderSourceCodeGroup::appendSourceCodeString( const std::string& srcCode )
{
	CC3ShaderSourceCode* sscs = new CC3ShaderSourceCodeString;
	sscs->appendSourceCodeString( srcCode );
	addSubsection( sscs );
	sscs->release();
}

void CC3ShaderSourceCodeGroup::accumulateSourceCompilationStringsWithVisitor( CC3ShaderSourceCodeCompilationStringVisitor* visitor )
{
	if ( visitor->hasAlreadyVisited(this) ) 
		return;

	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_subsections, pObj )
	{
		CC3ShaderSourceCode* shSrc = (CC3ShaderSourceCode*)pObj;
		shSrc->accumulateSourceCompilationStringsWithVisitor( visitor );
	}
}

void CC3ShaderSourceCodeGroup::accumulateSourceCompilationStringCountWithVisitor( CC3ShaderSourceCodeCompilationStringCountVisitor* visitor )
{
	if ( visitor->hasAlreadyVisited(this) ) 
		return;

	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_subsections, pObj )
	{
		CC3ShaderSourceCode* shSrc = (CC3ShaderSourceCode*)pObj;
		shSrc->accumulateSourceCompilationStringCountWithVisitor( visitor );
	}
}

bool CC3ShaderSourceCodeGroup::localizeLineNumberWithVisitor( CC3ShaderSourceCodeLineNumberLocalizingVisitor* visitor )
{
	// Increment the line offset to compensate for the #import/include statement that is being skipped.
	if ( visitor->hasAlreadyVisited(this) ) 
	{
		visitor->addLineNumberOffset(1);
		return false;
	}

	// Traverse the subsections, and return right away if the line number was resolved.
	// If the subsection did not claim to be the source of the line number, identify it as this file.
	visitor->pushLineNumberOffset(0);

	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_subsections, pObj )
	{
		CC3ShaderSourceCode* shSrc = (CC3ShaderSourceCode*)pObj;
		if ( shSrc->localizeLineNumberWithVisitor( visitor ) )
		{
			if ( !visitor->getLocalizedSourceCode() )
			{
				visitor->setLocalizedSourceCode( this );
			}

			return true;
		}
	}
	
	// Done with this group, so pop up to the next level for line number offset tracking,
	// and increment to cover the #import/include statement that invoked this group.
	visitor->popLineNumberOffset();
	visitor->addLineNumberOffset( 1 );
	return false;
}

void CC3ShaderSourceCodeGroup::initWithTag( GLuint tag, const std::string& name )
{
	super::initWithTag( tag, name );
	m_subsections = CCArray::create();	// retained
	m_subsections->retain();
}

CC3ShaderSourceCodeVisitor::CC3ShaderSourceCodeVisitor()
{
	m_sourceCodeNamesTraversed.clear();
}

CC3ShaderSourceCodeVisitor::~CC3ShaderSourceCodeVisitor()
{
	m_sourceCodeNamesTraversed.clear();
}

bool CC3ShaderSourceCodeVisitor::hasAlreadyVisited( CC3ShaderSourceCode* srcCode )
{
	std::string ssName = srcCode->getName();
	if ( ssName.empty() || m_sourceCodeNamesTraversed.find(ssName) != m_sourceCodeNamesTraversed.end() )
		return true;

	m_sourceCodeNamesTraversed[ssName] = true;
	return false;
}

void CC3ShaderSourceCodeVisitor::init()
{
	
}

CC3ShaderSourceCodeVisitor* CC3ShaderSourceCodeVisitor::visitor()
{
	CC3ShaderSourceCodeVisitor* pVisitor = new CC3ShaderSourceCodeVisitor;
	pVisitor->init();
	pVisitor->autorelease();
	return pVisitor;
}

CC3ShaderSourceCodeCompilationStringCountVisitor* CC3ShaderSourceCodeCompilationStringCountVisitor::visitor()
{
	CC3ShaderSourceCodeCompilationStringCountVisitor* pVisitor = new CC3ShaderSourceCodeCompilationStringCountVisitor;
	pVisitor->init();
	pVisitor->autorelease();
	return pVisitor;
}


void CC3ShaderSourceCodeCompilationStringCountVisitor::addSourceCompilationStringCount( GLuint sourceStringCount )
{
	m_sourceCompilationStringCount += sourceStringCount;
}

void CC3ShaderSourceCodeCompilationStringCountVisitor::init()
{
	super::init();
	m_sourceCompilationStringCount = 0;
}

GLuint CC3ShaderSourceCodeCompilationStringCountVisitor::getSourceCompilationStringCount()
{
	return m_sourceCompilationStringCount;
}

void CC3ShaderSourceCodeCompilationStringVisitor::addSourceCompilationString( const GLchar* sourceCompilationString )
{
	unsigned int strLen = strlen( sourceCompilationString );
	m_sourceCompilationStrings[m_sourceCompilationStringCount] = new GLchar[strLen+1];
	memset( (void*)m_sourceCompilationStrings[m_sourceCompilationStringCount], 0, strLen+1 );
	memcpy( (void*)m_sourceCompilationStrings[m_sourceCompilationStringCount], sourceCompilationString, strLen );

	m_sourceCompilationStringCount ++;
}

void CC3ShaderSourceCodeCompilationStringVisitor::initWithCompilationStrings( const GLchar** sourceCompilationStrings )
{
	super::init();
	m_sourceCompilationStrings = sourceCompilationStrings;
}

CC3ShaderSourceCodeCompilationStringVisitor* CC3ShaderSourceCodeCompilationStringVisitor::visitorWithCompilationStrings( const GLchar** sourceCompilationStrings )
{
	CC3ShaderSourceCodeCompilationStringVisitor* pVisitor = new CC3ShaderSourceCodeCompilationStringVisitor;
	pVisitor->initWithCompilationStrings( sourceCompilationStrings );
	pVisitor->autorelease();

	return pVisitor;
}

CC3ShaderSourceCodeLineNumberLocalizingVisitor::CC3ShaderSourceCodeLineNumberLocalizingVisitor()
{
	m_localizedSourceCode = NULL;
	m_lineNumberOffsets = NULL;
}

CC3ShaderSourceCodeLineNumberLocalizingVisitor::~CC3ShaderSourceCodeLineNumberLocalizingVisitor()
{
	CC_SAFE_RELEASE( m_localizedSourceCode );
	CC_SAFE_RELEASE( m_lineNumberOffsets );
}

CC3ShaderSourceCode* CC3ShaderSourceCodeLineNumberLocalizingVisitor::getLocalizedSourceCode()
{
	return m_localizedSourceCode;
}

void CC3ShaderSourceCodeLineNumberLocalizingVisitor::setLocalizedSourceCode( CC3ShaderSourceCode* code )
{
	CC_SAFE_RELEASE( m_localizedSourceCode );
	CC_SAFE_RETAIN( code );
	m_localizedSourceCode = code;
}

GLuint CC3ShaderSourceCodeLineNumberLocalizingVisitor::getLineNumberOffset()
{
	if ( m_lineNumberOffsets && m_lineNumberOffsets->count() > 0 )
		return 0;

	return ((CCInteger*)m_lineNumberOffsets->lastObject())->getValue(); 
}

void CC3ShaderSourceCodeLineNumberLocalizingVisitor::pushLineNumberOffset( GLuint lineNumberOffset )
{
	m_lineNumberOffsets->addObject( CCInteger::create( lineNumberOffset ) );
}

void CC3ShaderSourceCodeLineNumberLocalizingVisitor::popLineNumberOffset()
{
	m_lineNumberOffsets->removeLastObject(); 
}

void CC3ShaderSourceCodeLineNumberLocalizingVisitor::addLineNumberOffset( GLuint lineNumberOffset )
{
	GLuint currOffset = getLineNumberOffset();
	popLineNumberOffset();
	pushLineNumberOffset( currOffset + lineNumberOffset );
}

void CC3ShaderSourceCodeLineNumberLocalizingVisitor::initWithLineNumber( GLuint lineNumber )
{
	super::init();
	{
		m_lineNumber = lineNumber;
		m_lineNumberOffsets = CCArray::create();		// retained
		m_lineNumberOffsets->retain();
		pushLineNumberOffset( 0 );					// Init stack with a base value
	}
}

GLuint CC3ShaderSourceCodeLineNumberLocalizingVisitor::getLineNumber()
{
	return m_lineNumber;
}

void CC3ShaderSourceCodeLineNumberLocalizingVisitor::setLineNumber( GLuint number )
{
	m_lineNumber = number;
}

CC3ShaderSourceCodeLineNumberLocalizingVisitor* CC3ShaderSourceCodeLineNumberLocalizingVisitor::visitorWithLineNumber( GLuint lineNumber )
{
	CC3ShaderSourceCodeLineNumberLocalizingVisitor* pVisitor = new CC3ShaderSourceCodeLineNumberLocalizingVisitor;
	pVisitor->initWithLineNumber( lineNumber );
	pVisitor->autorelease();

	return pVisitor;
}

CC3ShaderPrewarmer::CC3ShaderPrewarmer()
{
	m_prewarmingSurface = NULL;
	m_prewarmingMeshNode = NULL;
	m_drawingVisitor = NULL;
}

CC3ShaderPrewarmer::~CC3ShaderPrewarmer()
{
	CC_SAFE_RELEASE( m_prewarmingSurface );
	CC_SAFE_RELEASE( m_prewarmingMeshNode );
	CC_SAFE_RELEASE( m_drawingVisitor );
}

CC3RenderSurface* CC3ShaderPrewarmer::getPrewarmingSurface()
{
	if ( !m_prewarmingSurface ) 
	{
		setPrewarmingSurface( CC3GLFramebuffer::surface() );
		CC3GLFramebuffer* prewarmingSurface = (CC3GLFramebuffer*)m_prewarmingSurface;
		prewarmingSurface->setName( "Prewarming surface" );
		prewarmingSurface->setColorAttachment( CC3GLRenderbuffer::renderbufferWithPixelFormat(GL_RGBA4) );
		prewarmingSurface->setSize( CC3IntSizeMake(4, 4) );
	}
	return m_prewarmingSurface;
}

void CC3ShaderPrewarmer::setPrewarmingSurface( CC3RenderSurface* renderSurface )
{
	CC_SAFE_RETAIN( renderSurface );
	CC_SAFE_RELEASE( m_prewarmingSurface );
	m_prewarmingSurface = renderSurface;
}

CC3MeshNode* CC3ShaderPrewarmer::getPrewarmingMeshNode()
{
	if ( !m_prewarmingMeshNode ) 
	{	
		// Create mesh node that only has vertex locations
		setPrewarmingMeshNode( CC3MeshNode::nodeWithName( "ShaderPrewarmer" ) );
		m_prewarmingMeshNode->setVertexContentTypes( kCC3VertexContentLocation );
		
		// Populate the mesh as a single triangular face of zero dimensions
		CC3Face triangle = CC3Face(CC3Vector::kCC3VectorZero, CC3Vector::kCC3VectorZero, CC3Vector::kCC3VectorZero);
		ccTex2F texCoords[3] = { {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0} };				// unused
		m_prewarmingMeshNode->populateAsTriangle( triangle, texCoords, 1 );

		m_prewarmingMeshNode->getShaderContext()->setShouldEnforceCustomOverrides( false );
		m_prewarmingMeshNode->getShaderContext()->setShouldEnforceVertexAttributes( false );

		/// Fix OpenGL error 0x0506
		//_prewarmingMeshNode->createGLBuffers();
	}

	return m_prewarmingMeshNode;
}

void CC3ShaderPrewarmer::setPrewarmingMeshNode( CC3MeshNode* node )
{
	CC_SAFE_RETAIN( node );
	CC_SAFE_RELEASE( m_prewarmingMeshNode );
	m_prewarmingMeshNode = node;
}

CC3NodeDrawingVisitor* CC3ShaderPrewarmer::getDrawingVisitor()
{
	if ( !m_drawingVisitor ) 
		setDrawingVisitor( CC3NodeDrawingVisitor::visitor() );
	return m_drawingVisitor;
}

void CC3ShaderPrewarmer::setDrawingVisitor( CC3NodeDrawingVisitor* visitor )
{
	CC_SAFE_RETAIN( visitor );
	CC_SAFE_RELEASE( m_drawingVisitor );
	m_drawingVisitor = visitor;
}

void CC3ShaderPrewarmer::prewarmShaderProgram( CC3ShaderProgram* program )
{
	CC3MeshNode* pwNode = getPrewarmingMeshNode();
	CC3RenderSurface* pwSurface = getPrewarmingSurface();
	CC3NodeDrawingVisitor* pwVisitor = getDrawingVisitor();

	pwNode->setShaderProgram( program );
	pwVisitor->setRenderSurface( pwSurface );

	pwVisitor->visit( pwNode );

	// Release visitor state so it won't interfere with later deallocations
	pwVisitor->setRenderSurface( NULL );
	pwVisitor->clearGL();
	pwNode->setShaderProgram( NULL );	// Release the program immediately, since it is only used once.
	program->resetGLState();		// Reset GL state. Needed if pre-warming in background context...
								// ...since state is different between contexts.
}

void CC3ShaderPrewarmer::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName  );
	{
		m_prewarmingSurface = NULL;
		m_prewarmingMeshNode = NULL;
		m_drawingVisitor = NULL;
	}
}

CC3ShaderPrewarmer* CC3ShaderPrewarmer::prewarmerWithName( const std::string& name )
{
	CC3ShaderPrewarmer* prewarmer = new CC3ShaderPrewarmer;
	prewarmer->initWithName( name );
	prewarmer->autorelease();

	return prewarmer;
}


NS_COCOS3D_END
