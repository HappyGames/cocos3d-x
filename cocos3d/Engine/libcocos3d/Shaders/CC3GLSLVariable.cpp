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

std::string stringFromCC3GLSLVariableScope( CC3GLSLVariableScope scope ) 
{
	switch (scope) 
	{
		case kCC3GLSLVariableScopeUnknown: return "kCC3GLSLVariableScopeUnknown";
		case kCC3GLSLVariableScopeScene: return "kCC3GLSLVariableScopeScene";
		case kCC3GLSLVariableScopeNode: return "kCC3GLSLVariableScopeNode";
		case kCC3GLSLVariableScopeDraw: return "kCC3GLSLVariableScopeDraw";
			
		default: return CC3String::stringWithFormat( (char*)"Unknown variable scope (%d)", scope );
	}
}

CC3GLSLVariable::CC3GLSLVariable()
{
	_program = NULL;			// weak reference
	_name = "";
}

CC3GLSLVariable::~CC3GLSLVariable()
{
	_program = NULL;			// weak reference
}

GLuint CC3GLSLVariable::getTypeStorageElementCount()
{
	switch (_type) 
	{	// 17
			
		case GL_FLOAT:
		case GL_INT:
		case GL_BOOL:
		case GL_SAMPLER_2D:
		case GL_SAMPLER_CUBE:
			return 1;

		case GL_FLOAT_VEC2:
		case GL_INT_VEC2:
		case GL_BOOL_VEC2:
			return 2;

		case GL_FLOAT_VEC3:
		case GL_INT_VEC3:
		case GL_BOOL_VEC3:
			return 3;

		case GL_FLOAT_VEC4:
		case GL_INT_VEC4:
		case GL_BOOL_VEC4:
		case GL_FLOAT_MAT2:
			return 4;
			
		case GL_FLOAT_MAT3:
			return 9;
			
		case GL_FLOAT_MAT4:
			return 16;
			
		default:
			CCAssert(false, "CC3GLSLVariable could not establish typeStorageElementCount because type %s is not understood"/*,
					  stringFromGLEnum(_type)*/);
			return 0;
	}
}

GLuint CC3GLSLVariable::getStorageElementCount()
{
	return getTypeStorageElementCount() * getSize(); 
}

CC3GLSLVariableScope CC3GLSLVariable::getScope()
{
	return _scope;
}

void CC3GLSLVariable::setScope( CC3GLSLVariableScope scope )
{
	_scope = scope;
}

void CC3GLSLVariable::setSemantic( GLenum semantic )
{
	_semantic = semantic;
}
	
void CC3GLSLVariable::init()
{
	_program = NULL;
	_index = 0;
	_location = 0;
	_name = "";
	_type = GL_ZERO;
	_size = 0;
	_semantic = kCC3SemanticNone;
	_semanticIndex = 0;
	_scope = kCC3GLSLVariableScopeUnknown;
	_isGLStateKnown = false;
}

void CC3GLSLVariable::initInProgram( CC3ShaderProgram* program, GLuint index )
{
	init();
	_index = index;
	_program = program;				// weak reference
	populateFromProgram();
}

CC3GLSLVariable* CC3GLSLVariable::variableInProgram( CC3ShaderProgram* program, GLuint index )
{
	CC3GLSLVariable* pVariable = new CC3GLSLVariable;
	pVariable->initInProgram( program, index );
	pVariable->autorelease();

	return pVariable;
}

void CC3GLSLVariable::populateFrom( CC3GLSLVariable* another )
{
	_program = another->getProgram();		// weak reference
	_name = another->getName();
	
	_index = another->getIndex();
	_location = another->getLocation();
	_type = another->getType();
	_size = another->getSize();
	_semantic = another->getSemantic();
	_semanticIndex = another->getSemanticIndex();
	_scope = another->getScope();
	_isGLStateKnown = another->isGLStateKnown();
}

CCObject* CC3GLSLVariable::copyWithZone( CCZone* pZone )
{
	CC3GLSLVariable* pVal = new CC3GLSLVariable;
	pVal->init();
	pVal->populateFrom( this );

	return pVal;
}

void CC3GLSLVariable::populateFromProgram()
{
	_semantic = kCC3SemanticNone;
	_semanticIndex = 0;
	_isGLStateKnown = false;
	CC3OpenGL::sharedGL()->populateShaderProgramVariable( this );
	normalizeName();
}

void CC3GLSLVariable::normalizeName()
{
	if ( _name.empty() ) 
		return;
	
	unsigned int nameLen = _name.size();
	if ( nameLen < 4 ) 
		return;		// Too short to have a "[n]" subscript

	// No subscript, so don't make any changes to the name
	if ( _name[nameLen - 1] != ']' )  
		return;
	
	// If the subscript is [0], remove it from the name
	unsigned int subStartIdx = nameLen - 3;
	if (( _name[subStartIdx] == '[' ) &&
		( _name[subStartIdx + 1] == '0' )) 
	{
		std::string name = _name.substr( 0, subStartIdx );
		_name = name;		// retained
	}
	else 
	{
		// We have a non-zero subscript. This variable is redundant to the zero-subscript
		// variable, so mark it as such, but don't remove the subscript from the name.
		setSemantic( kCC3SemanticRedundant );
	}
}
//
//-(NSString*) description { return [NSString stringWithFormat: @"%@ named %@", self.class, _name]; }
//
//-(NSString*) fullDescription {
//	NSMutableString* desc = [NSMutableString stringWithCapacity: 200];
//	desc += ( "%@", self.description];
//	desc += ( "\n\t\tLocation: %i", _location];
//	desc += ( "\n\t\tIndex: %u", _index];
//	desc += ( "\n\t\tType: %@", NSStringFromGLEnum(_type)];
//	desc += ( "\n\t\tSize: %i", _size];
//	desc += ( "\n\t\tStorage elements: %i", self.storageElementCount];
//	desc += ( "\n\t\tSemantic: %@ (%u)", self.semanticName, _semantic];
//	desc += ( "\n\t\tSemantic index: %u", _semanticIndex];
//	desc += ( "\n\t\tScope: %@", NSStringFromCC3GLSLVariableScope(_scope)];
//	return desc;
//}

std::string CC3GLSLVariable::getSemanticName()
{
	return _program->getSemanticDelegate()->getNameOfSemantic( _semantic ); 
}

void CC3GLSLVariable::populateFromGL()
{
	CCAssert( false, "CC3GLSLVariable::populateFromGL() is not implemented" );
}

CC3ShaderProgram* CC3GLSLVariable::getProgram()
{
	return _program;
}

unsigned int CC3GLSLVariable::getIndex()
{
	return _index;
}

unsigned int CC3GLSLVariable::getType()
{
	return _type;
}

bool CC3GLSLVariable::isGLStateKnown()
{
	return _isGLStateKnown;
}

void CC3GLSLVariable::setIsGLStateKnown( bool isKnonw )
{
	_isGLStateKnown = isKnonw;
}

unsigned int CC3GLSLVariable::getSemanticIndex()
{
	return _semanticIndex;
}

void CC3GLSLVariable::setSemanticIndex( GLuint semanticIndex )
{
	_semanticIndex = semanticIndex;
}

unsigned int CC3GLSLVariable::getSemantic()
{
	return _semantic;
}

CC3GLSLAttribute* CC3GLSLAttribute::variableInProgram( CC3ShaderProgram* program, GLuint index )
{
	CC3GLSLAttribute* pVariable = new CC3GLSLAttribute;
	pVariable->initInProgram( program, index );
	pVariable->autorelease();

	return pVariable;
}

void CC3GLSLAttribute::populateFromGL()
{
	if ( _program == NULL )
		return;

	GLint maxNameLen = _program->getMaxAttributeNameLength();
	char* cName = new char[maxNameLen];

	glGetActiveAttrib( _program->getProgramID(), _index, maxNameLen, NULL, &_size, &_type, cName );
	
	_location = glGetAttribLocation(_program->getProgramID(), cName);

	_name = cName;

	CC_SAFE_DELETE_ARRAY( cName );
}

int CC3GLSLVariable::getSize()
{
	return _size;
}

int CC3GLSLVariable::getLocation()
{
	return _location;
}

std::string CC3GLSLVariable::getName()
{
	return _name;
}

CC3GLSLUniform::CC3GLSLUniform()
{
	m_varValue = NULL;
	m_glVarValue = NULL;
}

CC3GLSLUniform::~CC3GLSLUniform()
{
	CC_SAFE_FREE( m_varValue );
	CC_SAFE_FREE( m_glVarValue );
}

GLenum CC3GLSLUniform::getType()
{
	return _type;
}

void CC3GLSLUniform::setFloat( GLfloat value )
{
	setFloat( value, 0 ); 
}

void CC3GLSLUniform::setFloat( GLfloat value, GLuint index )
{
	setVector4( CC3Vector4(value, 0.0f, 0.0f, 1.0f), index );
}

void CC3GLSLUniform::setPoint( const CCPoint& value )
{
	setPoint( value, 0 ); 
}

void CC3GLSLUniform::setPoint( const CCPoint& value, GLuint index )
{
	setVector4( CC3Vector4(value.x, value.y, 0.0f, 1.0f), index );
}

void CC3GLSLUniform::setVector( const CC3Vector& value )
{
	setVector( value, 0 ); 
}

void CC3GLSLUniform::setVector( const CC3Vector& value, GLuint index )
{
	setVector4( CC3Vector4(value.x, value.y, value.z, 1.0f), index );
}

void CC3GLSLUniform::setVector4( const CC3Vector4& value )
{
	setVector4( value, 0 ); 
}

void CC3GLSLUniform::setVector4( const CC3Vector4& value, GLuint index )
{
	CCAssert(index < (GLuint)_size, "%CC3GLSLUniform could not set value because index %d is out of bounds"/*, index*/);
	
	switch (_type) 
	{		
		case GL_FLOAT:
			((GLfloat*)m_varValue)[index] = *(GLfloat*)&value;
			return;
		case GL_FLOAT_VEC2:
			((CCPoint*)m_varValue)[index] = *(CCPoint*)&value;
			return;
		case GL_FLOAT_VEC3:
			((CC3Vector*)m_varValue)[index] = *(CC3Vector*)&value;
			return;
		case GL_FLOAT_VEC4:
		case GL_FLOAT_MAT2:
			((CC3Vector4*)m_varValue)[index] = value;
			return;
			
		case GL_FLOAT_MAT3:
		case GL_FLOAT_MAT4:
			CCAssert(false, "%@ attempted to set scalar or vector when matrix type %@ expected."/*,
																								self, NSStringFromGLEnum(_type)*/);
			return;
			
		case GL_INT:
		case GL_INT_VEC2:
		case GL_INT_VEC3:
		case GL_INT_VEC4:
		case GL_SAMPLER_2D:
		case GL_SAMPLER_CUBE:
		case GL_BOOL:
		case GL_BOOL_VEC2:
		case GL_BOOL_VEC3:
		case GL_BOOL_VEC4:
			setIntVector4( CC3IntVector4Make((int)value.x, (int)value.y, (int)value.z, (int)value.w), index );
			return;
			
		default:
			CCAssert(false, "CC3GLSLUniform could not set value because type %@ is not understood"/*, stringFromGLEnum(_type).c_str()*/);
			return;
	}
	
	CC3_TRACE("CC3GLSLUniform setting value to %s", value.stringfy().c_str());
}

void CC3GLSLUniform::setQuaternion( const CC3Quaternion& value )
{
	setQuaternion( value, 0 );
}

void CC3GLSLUniform::setQuaternion( const CC3Quaternion& value, GLuint index )
{
	setVector4( value, index ); 
}

void CC3GLSLUniform::setMatrix3x3( const CC3Matrix3x3* value )
{
	setMatrix3x3( value, 0 ); 
}

void CC3GLSLUniform::setMatrix3x3( const CC3Matrix3x3* value, GLuint index )
{
	CC3Matrix3x3* varMtx = (CC3Matrix3x3*)m_varValue;
	switch (_type) 
	{
		case GL_FLOAT_MAT3:
			varMtx[index] = *value;
			return;
		default:
			CCAssert(false, "CC3GLSLUniform attempted to set 3x3 matrix when matrix type %s expected."/*,
					 stringFromGLEnum(_type).c_str()*/);
			return;
	}
}

void CC3GLSLUniform::setMatrix4x3( const CC3Matrix4x3* value )
{
	setMatrix4x3( value, 0 ); 
}

void CC3GLSLUniform::setMatrix4x3( const CC3Matrix4x3* value, GLuint index )
{
	CC3Matrix4x4* varMtx = (CC3Matrix4x4*)m_varValue;
	switch (_type) 
	{
		case GL_FLOAT_MAT4:
			CC3Matrix4x4PopulateFrom4x3(&(varMtx[index]), value);
			return;
		default:
			CCAssert(false, "CC3GLSLUniform attempted to set 4x4 matrix when matrix type %s expected."/*,
					  stringFromGLEnum(_type).c_str()*/);
			return;
	}
}

void CC3GLSLUniform::setMatrix4x4( const CC3Matrix4x4* value )
{
	setMatrix4x4( value, 0 ); 
}

void CC3GLSLUniform::setMatrix4x4( const CC3Matrix4x4* value, GLuint index )
{
	CC3Matrix4x4* varMtx = (CC3Matrix4x4*)m_varValue;
	switch (_type) 
	{
		case GL_FLOAT_MAT4:
			varMtx[index] = *value;
			return;
		default:
			CCAssert(false, "CC3GLSLUniform attempted to set 4x4 matrix when matrix type %s expected."/*,
					 stringFromGLEnum(_type).c_str()*/);
			return;
	}
}

void CC3GLSLUniform::setInteger( GLint value )
{
	setInteger( value, 0 ); 
}

void CC3GLSLUniform::setInteger( GLint value, GLuint index )
{
	setIntVector4( CC3IntVector4Make(value, 0, 0, 0), index );
}

void CC3GLSLUniform::setIntPoint( const CC3IntPoint& value )
{
	setIntPoint( value, 0 ); 
}

void CC3GLSLUniform::setIntPoint( const CC3IntPoint& value, GLuint index )
{
	setIntVector4( CC3IntVector4Make(value.x, value.y, 0, 0), index );
}

void CC3GLSLUniform::setIntVector( const CC3IntVector& value )
{
	setIntVector( value, 0 ); 
}

void CC3GLSLUniform::setIntVector( const CC3IntVector& value, GLuint index )
{
	setIntVector4( CC3IntVector4Make(value.x, value.y, value.z, 0), index );
}

void CC3GLSLUniform::setIntVector4( const CC3IntVector4& value )
{
	setIntVector4( value, 0 ); 
}

void CC3GLSLUniform::setIntVector4( const CC3IntVector4& value, GLuint index )
{
	CCAssert((GLint)index < _size, "CC3GLSLUniform could not set value because index %d is out of bounds"/*, index*/);
	
	switch (_type) 
	{		
		case GL_FLOAT:
		case GL_FLOAT_VEC2:
		case GL_FLOAT_VEC3:
		case GL_FLOAT_VEC4:
		case GL_FLOAT_MAT2:
			setVector4( CC3Vector4((GLfloat)value.x, (GLfloat)value.y, (GLfloat)value.z, (GLfloat)value.w), index );
			return;
			
		case GL_FLOAT_MAT3:
		case GL_FLOAT_MAT4:
			CCAssert(false, "CC3GLSLUniform attempted to set scalar or vector when matrix type %s expected."/*,
					 stringFromGLEnum(_type).c_str()*/);
			return;
			
		case GL_INT:
		case GL_BOOL:
		case GL_SAMPLER_2D:
		case GL_SAMPLER_CUBE:
			((GLint*)m_varValue)[index] = *(GLint*)&value;
			return;
		case GL_INT_VEC2:
		case GL_BOOL_VEC2:
			((CC3IntPoint*)m_varValue)[index] = *(CC3IntPoint*)&value;
			return;
		case GL_INT_VEC3:
		case GL_BOOL_VEC3:
			((CC3IntVector*)m_varValue)[index] = *(CC3IntVector*)&value;
			return;
		case GL_INT_VEC4:
		case GL_BOOL_VEC4:
			((CC3IntVector4*)m_varValue)[index] = value;
			return;
			
		default:
			CCAssert(false, "CC3GLSLUniform could not set value because type %s is not understood"/*,
					  stringFromGLEnum(_type).c_str()*/);
			return;
	}
	CC3_TRACE("CC3GLSLUniform setting value to (%d, %d, %d, %d)", value.x, value.y, value.z, value.w);
}

void CC3GLSLUniform::setBoolean( bool value )
{ 
	setBoolean( value, 0 ); 
}

void CC3GLSLUniform::setBooleanVector( bool bX, bool bY )
{
	setBooleanVector( bX, bY, 0 );
}

void CC3GLSLUniform::setBooleanVector( bool bX, bool bY, bool bZ )
{
	setBooleanVector( bX, bY, bZ, 0 );
}

void CC3GLSLUniform::setBooleanVector( bool bX, bool bY, bool bZ, bool bW )
{
	setBooleanVectorAt( bX, bY, bZ, bW, 0 );
}

void CC3GLSLUniform::setBoolean( bool value, GLuint index )
{
	setBooleanVectorAt( value, false, false, false, index );
	setInteger( (value != 0), index );
}

void CC3GLSLUniform::setBooleanVectorAt( bool bX, bool bY, GLuint index )
{
	setBooleanVectorAt( bX, bY, false, false, index );
}

void CC3GLSLUniform::setBooleanVectorAt( bool bX, bool bY, bool bZ, GLuint index )
{
	setBooleanVectorAt( bX, bY, bZ, false, index );
}

void CC3GLSLUniform::setBooleanVectorAt( bool bX, bool bY, bool bZ, bool bW, GLuint index )
{
	setIntVector4( CC3IntVector4Make((bX != false), (bY != false), (bZ != false), (bW != false)), index );
}

void CC3GLSLUniform::setColor4B( const ccColor4B& value )
{
	setColor4B( value, 0 ); 
}

void CC3GLSLUniform::setColor4B( const ccColor4B& value, GLuint index )
{
	switch (_type) 
	{
		case GL_FLOAT:
		case GL_FLOAT_VEC2:
		case GL_FLOAT_VEC3:
		case GL_FLOAT_VEC4:
		case GL_FLOAT_MAT2:
			setColor4F( CCC4FFromCCC4B(value) );
			return;
			
		case GL_FLOAT_MAT3:
		case GL_FLOAT_MAT4:
			CCAssert(false, "CC3GLSLUniform attempted to set color when matrix type %s expected."/*,
					 stringFromGLEnum(_type).c_str()*/);
			return;

		case GL_INT:
		case GL_BOOL:
		case GL_INT_VEC2:
		case GL_BOOL_VEC2:
		case GL_INT_VEC3:
		case GL_BOOL_VEC3:
		case GL_INT_VEC4:
		case GL_BOOL_VEC4:
		case GL_SAMPLER_2D:
		case GL_SAMPLER_CUBE:
			setIntVector4( CC3IntVector4Make(value.r, value.g, value.b, value.a), index );
			return;
			
		default:
			CCAssert(false, "CC3GLSLUniform could not set value because type %s is not understood"/*,
					  stringFromGLEnum(_type).c_str()*/);
			return;
	}
}

void CC3GLSLUniform::setColor4F( const ccColor4F& value )
{ 
	setColor4F( value, 0 ); 
}

void CC3GLSLUniform::setColor4F( const ccColor4F& value, GLuint index )
{
	switch (_type) 
	{		
		case GL_FLOAT:
		case GL_FLOAT_VEC2:
		case GL_FLOAT_VEC3:
		case GL_FLOAT_VEC4:
		case GL_FLOAT_MAT2:
			setVector4( CC3Vector4(value.r, value.g, value.b, value.a), index );
			return;

		case GL_FLOAT_MAT3:
		case GL_FLOAT_MAT4:
			CCAssert(false, "CC3GLSLUniform attempted to set color when matrix type %s expected."/*,
					 stringFromGLEnum(_type).c_str()*/);
			return;

		case GL_INT:
		case GL_BOOL:
		case GL_INT_VEC2:
		case GL_BOOL_VEC2:
		case GL_INT_VEC3:
		case GL_BOOL_VEC3:
		case GL_INT_VEC4:
		case GL_BOOL_VEC4:
		case GL_SAMPLER_2D:
		case GL_SAMPLER_CUBE:
			setColor4B( CCC4BFromCCC4F(value) );
			return;

		default:
			CCAssert(false, "CC3GLSLUniform could not set value because type %s is not understood"/*,
					  stringFromGLEnum(_type).c_str()*/ );
			return;
	}
}

void CC3GLSLUniform::setValueFromUniform( CC3GLSLUniform* uniform )
{
	CCAssert(_type == uniform->getType(), "uniforms are not of the same type");
	CCAssert(_size == uniform->getSize(), "uniforms are not of the same size");
	memcpy(m_varValue, uniform->m_varValue, m_varLen);
}

std::string CC3GLSLUniform::valueDescription()
{
	std::string desc = "";
	desc += "[";
	
	for (GLint vIdx = 0 ; vIdx < _size; vIdx++) 
	{
		if (vIdx > 0) 
			desc += ", ";

		switch (_type) 
		{
			case GL_FLOAT:
				desc += CC3String::stringWithFormat( (char*)"%.3f", ((GLfloat*)m_varValue)[vIdx] );
				break;
			case GL_FLOAT_VEC2:
				desc += CC3String::stringWithFormat( (char*)"%s", stringFromCCPoint(((CCPoint*)m_varValue)[vIdx]).c_str() );
				break;
			case GL_FLOAT_VEC3:
				desc += CC3String::stringWithFormat( (char*)"%s", ((CC3Vector*)m_varValue)[vIdx].stringfy().c_str() );
				break;
			case GL_FLOAT_VEC4:
			case GL_FLOAT_MAT2:
				desc += CC3String::stringWithFormat( (char*)"%s", (((CC3Vector4*)m_varValue)[vIdx]).stringfy().c_str() );
				break;
				
			case GL_FLOAT_MAT3:
				desc += CC3String::stringWithFormat( (char*)"%s", stringFromCC3Matrix3x3(&(((CC3Matrix3x3*)m_varValue)[vIdx])).c_str() );
				break;
			case GL_FLOAT_MAT4:
				desc += CC3String::stringWithFormat( (char*)"%s", stringFromCC3Matrix4x4(&(((CC3Matrix4x4*)m_varValue)[vIdx])).c_str() );
				break;
			case GL_SAMPLER_2D:
			case GL_SAMPLER_CUBE:
			case GL_INT:
			case GL_BOOL:
				desc += CC3String::stringWithFormat( (char*)"%d", ((GLint*)m_varValue)[vIdx] );
				break;
			case GL_INT_VEC2:
			case GL_BOOL_VEC2:
				desc += CC3String::stringWithFormat( (char*)"%s", stringFromCC3IntPoint(((CC3IntPoint*)m_varValue)[vIdx]).c_str() );
				break;
			case GL_INT_VEC3:
			case GL_BOOL_VEC3:
				desc += CC3String::stringWithFormat( (char*)"%s", stringFromCC3IntVector(((CC3IntVector*)m_varValue)[vIdx]).c_str() );
				break;
			case GL_INT_VEC4:
			case GL_BOOL_VEC4:
				desc += CC3String::stringWithFormat( (char*)"%s", stringFromCC3IntVector4(((CC3IntVector4*)m_varValue)[vIdx]).c_str() );
				break;
				
			default:
				CCAssert(false, "%CC3GLSLUniform could not set value because type %s is not understood"/*,
						  stringFromGLEnum(_type)*/);
				break;
		}
	}
	desc += "]";
	return desc;
}

/** Also allocate space for the uniform value. */
void CC3GLSLUniform::populateFromProgram()
{
	super::populateFromProgram();
	
	m_varLen = CC3GLElementTypeSize(_type) * _size;
	CC_SAFE_FREE(m_varValue);
	m_varValue = calloc(m_varLen, 1);
	CC_SAFE_FREE(m_glVarValue);
	m_glVarValue = calloc(m_varLen, 1);
	
	populateInitialValue();
}

/** Populates the initial values of the uniform, based on the size and type. */
void CC3GLSLUniform::populateInitialValue()
{
	for (GLint vIdx = 0 ; vIdx < _size; vIdx++) 
	{
		CC3Matrix3x3 m3x3;
		CC3Matrix4x4 m4x4;
		
		switch (_type) 
		{		
			case GL_FLOAT:
			case GL_FLOAT_VEC2:
			case GL_FLOAT_VEC3:
			case GL_FLOAT_VEC4:
				setVector4( CC3Vector4(0.0f, 0.0f, 0.0f, 1.0f), vIdx );
				return;
				
			case GL_FLOAT_MAT2:
				setVector4( CC3Vector4(1.0f, 0.0f, 0.0f, 1.0f), vIdx );
				return;
			case GL_FLOAT_MAT3:
				CC3Matrix3x3PopulateIdentity(&m3x3);
				setMatrix3x3( &m3x3, vIdx );
				return;
			case GL_FLOAT_MAT4:
				CC3Matrix4x4PopulateIdentity(&m4x4);
				setMatrix4x4( &m4x4, vIdx );
				return;
				
			case GL_INT:
			case GL_INT_VEC2:
			case GL_INT_VEC3:
			case GL_INT_VEC4:
			case GL_SAMPLER_2D:
			case GL_SAMPLER_CUBE:
			case GL_BOOL:
			case GL_BOOL_VEC2:
			case GL_BOOL_VEC3:
			case GL_BOOL_VEC4:
				setIntVector4( CC3IntVector4Make(0, 0, 0, 1), vIdx );
				return;
				
			default:
				CCAssert(false, "CC3GLSLUniform could not set value because type %s is not understood"/*,
						  stringFromGLEnum(_type).c_str()*/);
				return;
		}
	}
	
}

bool CC3GLSLUniform::updateGLValueWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	if ( _isGLStateKnown && memcmp(m_glVarValue, m_varValue, m_varLen) == 0 ) 
		return false;

	memcpy(m_glVarValue, m_varValue, m_varLen);
	visitor->getGL()->setShaderProgramUniformValue( this );
	_isGLStateKnown = true;
	return true;
}

void CC3GLSLUniform::init()
{
	super::init();
	{
		m_varLen = 0;
		m_varValue = NULL;
	}
}

void CC3GLSLUniform::populateFrom( CC3GLSLUniform* another )
{
	super::populateFrom( another );
	
	m_varLen = CC3GLElementTypeSize(_type) * _size;
	CC_SAFE_FREE(m_varValue);
	m_varValue = calloc(m_varLen, 1);
	CC_SAFE_FREE(m_glVarValue);
	m_glVarValue = calloc(m_varLen, 1);
	
	setValueFromUniform( another );
}

CCObject* CC3GLSLUniform::copyWithZone( CCZone* pZone )
{
	CC3GLSLUniform* pVal = new CC3GLSLUniform;
	pVal->init();
	pVal->populateFrom( this );

	return pVal;
}

CC3GLSLUniform* CC3GLSLUniform::variableInProgram( CC3ShaderProgram* program, GLuint index )
{
	CC3GLSLUniform* pVariable = new CC3GLSLUniform;
	pVariable->initInProgram( program, index );
	pVariable->autorelease();

	return pVariable;
}

void CC3GLSLUniform::setGLValue()
{
	switch (_type) 
	{
	case GL_FLOAT:
		glUniform1fv(_location, _size, (const GLfloat*)m_glVarValue);
		//LogGLErrorTrace(@"glUniform1fv(%i, %i, %.3f) setting %@", _location, _size, *(GLfloat*)_glVarValue, self.name);
		break;
	case GL_FLOAT_VEC2:
		glUniform2fv(_location, _size, (const GLfloat*)m_glVarValue);
		//LogGLErrorTrace(@"glUniform2fv(%i, %i, %@) setting %@", _location, _size, NSStringFromCGPoint(*(CGPoint*)_glVarValue), self.name);
		break;
	case GL_FLOAT_VEC3:
		glUniform3fv(_location, _size, (const GLfloat*)m_glVarValue);
		//LogGLErrorTrace(@"glUniform3fv(%i, %i, %@) setting %@", _location, _size, NSStringFromCC3Vector(*(CC3Vector*)_glVarValue), self.name);
		break;
	case GL_FLOAT_VEC4:
		glUniform4fv(_location, _size, (const GLfloat*)m_glVarValue);
		//LogGLErrorTrace(@"glUniform4fv(%i, %i, %@) setting %@", _location, _size, NSStringFromCC3Vector4(*(CC3Vector4*)_glVarValue), self.name);
		break;

	case GL_FLOAT_MAT2:
		glUniformMatrix2fv(_location, _size, GL_FALSE, (const GLfloat*)m_glVarValue);
	//	LogGLErrorTrace(@"glUniformMatrix2fv(%i, %i, GL_FALSE, %@) setting %@", _location, _size, NSStringFromCC3Vector4(*(CC3Vector4*)_glVarValue), self.name);
		break;
	case GL_FLOAT_MAT3:
		glUniformMatrix3fv(_location, _size, GL_FALSE, (const GLfloat*)m_glVarValue);
	//	LogGLErrorTrace(@"glUniformMatrix3fv(%i, %i, GL_FALSE, %@) setting %@", _location, _size, NSStringFromCC3Matrix3x3((CC3Matrix3x3*)_glVarValue), self.name);
		break;
	case GL_FLOAT_MAT4:
		glUniformMatrix4fv(_location, _size, GL_FALSE, (const GLfloat*)m_glVarValue);
	//	LogGLErrorTrace(@"glUniformMatrix4fv(%i, %i, GL_FALSE, %@) setting %@", _location, _size, NSStringFromCC3Matrix4x4((CC3Matrix4x4*)_glVarValue), self.name);
		break;

	case GL_INT:
	case GL_SAMPLER_2D:
	case GL_SAMPLER_CUBE:
	case GL_BOOL:
		glUniform1iv(_location, _size, (const GLint*)m_glVarValue);
	//	LogGLErrorTrace(@"glUniform1iv(%i, %i, %i) setting %@", _location, _size, *(GLint*)_glVarValue, self.name);
		break;
	case GL_INT_VEC2:
	case GL_BOOL_VEC2:
		glUniform2iv(_location, _size, (const GLint*)m_glVarValue);
//		LogGLErrorTrace(@"glUniform2iv(%i, %i, %@) setting %@", _location, _size, NSStringFromCC3IntPoint(*(CC3IntPoint*)_glVarValue), self.name);
		break;
	case GL_INT_VEC3:
	case GL_BOOL_VEC3:
		glUniform3iv(_location, _size, (const GLint*)m_glVarValue);
//		LogGLErrorTrace(@"glUniform3iv(%i, %i, %@) setting %@", _location, _size, NSStringFromCC3IntVector(*(CC3IntVector*)_glVarValue), self.name);
		break;
	case GL_INT_VEC4:
	case GL_BOOL_VEC4:
		glUniform4iv(_location, _size, (const GLint*)m_glVarValue);
//		LogGLErrorTrace(@"glUniform4iv(%i, %i, %@) setting %@", _location, _size, NSStringFromCC3IntVector4(*(CC3IntVector4*)_glVarValue), self.name);
		break;

	default:
		CCAssert(false, "could not set GL engine state value because uniform type is not understood"/*,
			self, NSStringFromGLEnum(_type)*/);
		break;
	}
}

void CC3GLSLUniform::populateFromGL()
{
	if ( _program == NULL )
		return;

	GLint maxNameLen = _program->getMaxUniformNameLength();
	char* cName = new char[maxNameLen];

	glGetActiveUniform( _program->getProgramID(), _index, maxNameLen, NULL, &_size, &_type, cName );

	_location = glGetUniformLocation( _program->getProgramID(), cName );

	_name = cName;

	CC_SAFE_DELETE_ARRAY( cName );
}


CC3GLSLUniformOverride::CC3GLSLUniformOverride()
{
	m_pProgramUniform = NULL;
	m_pPureColorProgramUniform = NULL;
}

CC3GLSLUniformOverride::~CC3GLSLUniformOverride()
{
	CC_SAFE_RELEASE( m_pProgramUniform );
	CC_SAFE_RELEASE( m_pPureColorProgramUniform );
}

bool CC3GLSLUniformOverride::updateIfOverriding( CC3GLSLUniform* uniform )
{
	if (uniform == m_pProgramUniform || uniform == m_pPureColorProgramUniform) 
	{
		uniform->setValueFromUniform( this );
		return true;
	}
	return false;
}

bool CC3GLSLUniformOverride::updateGLValueWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	return false; 
}

void CC3GLSLUniformOverride::initForProgramUniform( CC3GLSLUniform* uniform, CC3GLSLUniform* pureColorUniform )
{
	CCAssert(uniform, "The overridden uniform must not be nil.");
	super::init();
	populateFrom( uniform );

	CC_SAFE_RELEASE( m_pProgramUniform ); 
	m_pProgramUniform = uniform;						// retained
	CC_SAFE_RETAIN( uniform );
	CC_SAFE_RELEASE( m_pPureColorProgramUniform );
	m_pPureColorProgramUniform = pureColorUniform;	// retained
	CC_SAFE_RETAIN( pureColorUniform );
}

CC3GLSLUniformOverride* CC3GLSLUniformOverride::uniformOverrideForProgramUniform( CC3GLSLUniform* uniform, CC3GLSLUniform* pureColorUniform )
{
	CC3GLSLUniformOverride* uniformOveride = new CC3GLSLUniformOverride;
	uniformOveride->initForProgramUniform( uniform, pureColorUniform );
	uniformOveride->autorelease();

	return uniformOveride;
}

void CC3GLSLUniformOverride::setGLValue()
{

}

NS_COCOS3D_END
