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

CC3ShaderContext::CC3ShaderContext()
{
	_program = NULL;
	_pureColorProgram = NULL;
	_uniformOverrides = NULL;
	_uniformOverridesByName = NULL;
}

CC3ShaderContext::~CC3ShaderContext()
{
	CC_SAFE_RELEASE(_program);
	CC_SAFE_RELEASE(_pureColorProgram);

	removeAllUniformOverrides();
}

CC3ShaderProgram* CC3ShaderContext::getProgram()
{
	return _program; 
}

void CC3ShaderContext::setProgram( CC3ShaderProgram* program )
{
	if (program == _program) 
		return;

	CC_SAFE_RELEASE(_program);
	_program = program;
	CC_SAFE_RETAIN( program );
	
	setPureColorProgram( NULL );
	
	removeAllUniformOverrides();
}

CC3ShaderProgram* CC3ShaderContext::getPureColorProgram()
{
	if ( !_pureColorProgram )
		setPureColorProgram( CC3ShaderProgram::getShaderMatcher()->getPureColorProgramMatching( getProgram() ) );

	return _pureColorProgram;
}

void CC3ShaderContext::setPureColorProgram( CC3ShaderProgram* program )
{
	if (program == _pureColorProgram) 
		return;

	CC_SAFE_RELEASE( _pureColorProgram );
	_pureColorProgram = program;
	CC_SAFE_RETAIN( program );
}

bool CC3ShaderContext::shouldEnforceCustomOverrides()
{
	return _shouldEnforceCustomOverrides;
}

void CC3ShaderContext::setShouldEnforceCustomOverrides( bool shouldEnforce )
{
	_shouldEnforceCustomOverrides = shouldEnforce;
}

bool CC3ShaderContext::shouldEnforceVertexAttributes()
{
	return _shouldEnforceVertexAttributes;
}

void CC3ShaderContext::setShouldEnforceVertexAttributes( bool shouldEnforce )
{
	_shouldEnforceVertexAttributes = shouldEnforce;
}

CC3GLSLUniform* CC3ShaderContext::getUniformOverrideNamed( const char* name )
{
	CC3GLSLUniform* rtnVar = _uniformOverridesByName ? (CC3GLSLUniform*)_uniformOverridesByName->objectForKey(name) : NULL;
	return rtnVar ? rtnVar : addUniformOverrideFor( getProgram()->getUniformNamed( name ) );
}

CC3GLSLUniform* CC3ShaderContext::getUniformOverrideForSemantic( GLenum semantic, GLuint semanticIndex )
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( _uniformOverrides, pObj )
	{
		CC3GLSLUniform* var = (CC3GLSLUniform*)pObj;
		if (var->getSemantic() == semantic && var->getSemanticIndex() == semanticIndex) 
			return var;
	}
	
	return addUniformOverrideFor( getProgram()->getUniformForSemantic( semantic, semanticIndex ) );
}

CC3GLSLUniform* CC3ShaderContext::getUniformOverrideForSemantic( GLenum semantic )
{
	return getUniformOverrideForSemantic( semantic, 0 );
}

CC3GLSLUniform* CC3ShaderContext::getUniformOverrideAtLocation( GLint uniformLocation )
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( _uniformOverrides, pObj )
	{
		CC3GLSLUniform* var = (CC3GLSLUniform*)pObj;
		if (var->getLocation() == uniformLocation) 
			return var;
	}

	return addUniformOverrideFor( getProgram()->getUniformAtLocation( uniformLocation) );
}

CC3GLSLUniform*	CC3ShaderContext::addUniformOverrideFor( CC3GLSLUniform* uniform )
{
	if( !uniform ) 
		return NULL;
	
	CC3GLSLUniform* pureColorUniform = getPureColorProgram()->getUniformNamed( uniform->getName() );
	CC3GLSLUniformOverride* uniformOverride = CC3GLSLUniformOverride::uniformOverrideForProgramUniform( uniform, pureColorUniform );
	addUniformOverride( uniformOverride );
	return uniformOverride;
}

void CC3ShaderContext::addUniformOverride( CC3GLSLUniformOverride* uniformOverride )
{
	if( !uniformOverride ) 
		return;
	
	if ( !_uniformOverrides ) 
	{
		_uniformOverrides = CCArray::create();						// retained
		_uniformOverrides->retain();
	}
	if ( !_uniformOverridesByName ) 
	{
		_uniformOverridesByName = CCDictionary::create();	// retained
		_uniformOverridesByName->retain();
	}
		
	_uniformOverridesByName->setObject( uniformOverride, uniformOverride->getName() );
	_uniformOverrides->addObject( uniformOverride );
}

void CC3ShaderContext::removeUniformOverride( CC3GLSLUniform* uniform )
{
	_uniformOverrides->removeObject( uniform );
	_uniformOverridesByName->removeObjectForKey( uniform->getName() );
	CCAssert(_uniformOverrides->count() == _uniformOverridesByName->count(), "uniform was not completely removed from CC3ShaderContext");
	if (_uniformOverrides->count() == 0) 
		removeAllUniformOverrides();	// Remove empty collections
}

void CC3ShaderContext::removeAllUniformOverrides()
{
	if ( _uniformOverrides )
		_uniformOverrides->removeAllObjects();

	if ( _uniformOverridesByName )
		_uniformOverridesByName->removeAllObjects();

	CC_SAFE_RELEASE( _uniformOverridesByName );
	CC_SAFE_RELEASE( _uniformOverrides );
}

bool CC3ShaderContext::populateUniform( CC3GLSLUniform* uniform, CC3NodeDrawingVisitor* visitor )
{
	// If any of the uniform overrides are overriding the uniform, update the value of the
	// uniform from the override, and return that we've done so.
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( _uniformOverrides, pObj )
	{
		CC3GLSLUniformOverride* var = (CC3GLSLUniformOverride*)pObj;
		if ( var->updateIfOverriding( uniform ) )
			return true;
	}

	// If the semantic is unknown, and no override was found, return whether a default is okay
	if (uniform->getSemantic() == kCC3SemanticNone) 
		return !_shouldEnforceCustomOverrides;
	
	return false;
}

void CC3ShaderContext::init()
{
	_program = NULL;
	_pureColorProgram = NULL;
	_uniformOverrides = NULL;
	_uniformOverridesByName = NULL;
	_shouldEnforceCustomOverrides = true;
	_shouldEnforceVertexAttributes = true;
}

CC3ShaderContext* CC3ShaderContext::context()
{
	CC3ShaderContext* pContext = new CC3ShaderContext;
	pContext->init();
	pContext->autorelease();

	return pContext;
}

CCObject* CC3ShaderContext::copyWithZone( CCZone* zone )
{
	CC3ShaderContext* aCopy = new CC3ShaderContext;
	aCopy->init();
	aCopy->populateFrom( this );
	return aCopy;
}

void CC3ShaderContext::populateFrom( CC3ShaderContext* another )
{
	CC_SAFE_RELEASE(_program);
	_program = another->getProgram();
	CC_SAFE_RETAIN( _program );
	
	CC_SAFE_RELEASE(_pureColorProgram);
	_pureColorProgram = another->_pureColorProgram;
	CC_SAFE_RETAIN( _pureColorProgram );

	_shouldEnforceCustomOverrides = another->shouldEnforceCustomOverrides();
	_shouldEnforceVertexAttributes = another->shouldEnforceVertexAttributes();

	CCObject* pObj = NULL;
	CCARRAY_FOREACH( another->_uniformOverrides, pObj )
	{
		CC3GLSLUniformOverride* var = (CC3GLSLUniformOverride*)pObj;
		addUniformOverride( (CC3GLSLUniformOverride*)var->copy()->autorelease() );
	}
}

std::string CC3ShaderContext::fullDescription()
{
	return "CC3ShaderContext";
	//return [NSString stringWithFormat: @"%@ for program %@", self.class, _program.fullDescription];
}

NS_COCOS3D_END
