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


static CC3OpenGL* _renderGL = NULL;
static CC3OpenGL* _bgGL = NULL;

CC3OpenGL::CC3OpenGL()
{
	_context = NULL;
	_extensions = NULL;
	vertexAttributes = NULL;
	value_GL_TEXTURE_BINDING_2D = NULL;
	value_GL_TEXTURE_BINDING_CUBE_MAP = NULL;
}

CC3OpenGL::~CC3OpenGL()
{
	CC3_TRACE("Deallocating %CC3OpenGL and closing OpenGL on thread id.");

//	CC_SAFE_RELEASE( _context );
#pragma _NOTE_TODO( "Release gl context here" )

	CC_SAFE_RELEASE( _extensions );
	
	CC_SAFE_FREE(vertexAttributes);
	CC_SAFE_FREE(value_GL_TEXTURE_BINDING_2D);
	CC_SAFE_FREE(value_GL_TEXTURE_BINDING_CUBE_MAP);
}

void CC3OpenGL::setContext( CC3GLContext* context )
{

}

CC3GLContext* CC3OpenGL::getContext()
{
	return _context;
}

static CC3OpenGLDelegate* _delegate = NULL;

CC3OpenGLDelegate* CC3OpenGL::getDelegate()
{ 
	return _delegate; 
}
void CC3OpenGL::setDelegate( CC3OpenGLDelegate* pDelegate )
{
	if (pDelegate == _delegate) 
		return;

	delete _delegate;
	_delegate = pDelegate;
}

void CC3OpenGL::enableAlphaTesting( bool onOff )
{

}

void CC3OpenGL::enableBlend( bool onOff )
{ 
	cc3_SetGLCap(GL_BLEND, onOff, valueCap_GL_BLEND, isKnownCap_GL_BLEND); 
}

void CC3OpenGL::enableClipPlane( bool onOff, GLuint clipIdx )
{

}

void CC3OpenGL::enableColorLogicOp( bool onOff )
{

}

void CC3OpenGL::enableColorMaterial( bool onOff )
{

}

void CC3OpenGL::enableCullFace( bool onOff )
{ 
	cc3_SetGLCap(GL_CULL_FACE, onOff, valueCap_GL_CULL_FACE, isKnownCap_GL_CULL_FACE); 
}

void CC3OpenGL::enableDepthTest( bool onOff )
{ 
	cc3_SetGLCap(GL_DEPTH_TEST, onOff, valueCap_GL_DEPTH_TEST, isKnownCap_GL_DEPTH_TEST); 
}

void CC3OpenGL::enableDither( bool onOff )
{ 
	cc3_SetGLCap(GL_DITHER, onOff, valueCap_GL_DITHER, isKnownCap_GL_DITHER); 
}

void CC3OpenGL::enableFog( bool onOff )
{

}

void CC3OpenGL::enableLineSmoothing( bool onOff )
{

}

void CC3OpenGL::enableMatrixPalette( bool onOff )
{

}

void CC3OpenGL::enableMultisampling( bool onOff )
{

}

void CC3OpenGL::enableNormalize( bool onOff )
{

}

void CC3OpenGL::enablePointSmoothing( bool onOff )
{

}

void CC3OpenGL::enablePointSprites( bool onOff )
{

}

void CC3OpenGL::enableShaderPointSize( bool onOff )
{

}

void CC3OpenGL::enablePolygonOffset( bool onOff )
{ 
	cc3_SetGLCap(GL_POLYGON_OFFSET_FILL, onOff, valueCap_GL_POLYGON_OFFSET_FILL, isKnownCap_GL_POLYGON_OFFSET_FILL); 
}

void CC3OpenGL::enableRescaleNormal( bool onOff )
{

}

void CC3OpenGL::enableSampleAlphaToCoverage( bool onOff )
{ 
	cc3_SetGLCap(GL_SAMPLE_ALPHA_TO_COVERAGE, onOff, valueCap_GL_SAMPLE_ALPHA_TO_COVERAGE, isKnownCap_GL_SAMPLE_ALPHA_TO_COVERAGE);
}

void CC3OpenGL::enableSampleAlphaToOne( bool onOff )
{

}

void CC3OpenGL::enableSampleCoverage( bool onOff )
{ 
	cc3_SetGLCap(GL_SAMPLE_COVERAGE, onOff, valueCap_GL_SAMPLE_COVERAGE, isKnownCap_GL_SAMPLE_COVERAGE);
}

void CC3OpenGL::enableScissorTest( bool onOff )
{ 
	cc3_SetGLCap(GL_SCISSOR_TEST, onOff, valueCap_GL_SCISSOR_TEST, isKnownCap_GL_SCISSOR_TEST); 
}

void CC3OpenGL::enableStencilTest( bool onOff )
{ 
	cc3_SetGLCap(GL_STENCIL_TEST, onOff, valueCap_GL_STENCIL_TEST, isKnownCap_GL_STENCIL_TEST);
}

void CC3OpenGL::bindMesh( CC3Mesh* mesh, CC3NodeDrawingVisitor* visitor )
{

}

void CC3OpenGL::bindVertexAttribute( CC3GLSLAttribute* attribute, CC3NodeDrawingVisitor* visitor )
{

}

void CC3OpenGL::enableVertexAttribute( bool onOff, GLint vaIdx )
{
	if (vaIdx < 0)
		return;
	CC3VertexAttr* vaPtr = &vertexAttributes[vaIdx];
	
	// If we know the state and it is not changing, do nothing.
	if (vaPtr->isEnabledKnown && CC3BooleansAreEqual(vaPtr->isEnabled, onOff))
		return;
	
	vaPtr->isEnabled = onOff;
	vaPtr->isEnabledKnown = true;
	
	setVertexAttributeEnablementAt( vaIdx );
}

void CC3OpenGL::setVertexAttributeEnablementAt( GLint vaIdx )
{
	// CC3AssertUnimplemented(@"setVertexAttributeEnablementAt:");
}

void CC3OpenGL::bindVertexContent( GLvoid* pData, GLint elemSize, GLenum elemType, GLsizei vtxStride, bool shldNorm, GLint vaIdx )
{
	if (vaIdx < 0) 
		return;
	CC3VertexAttr* vaPtr = &vertexAttributes[vaIdx];
	
	bool shouldSetGL = true;		// TODO - fix errors drawing multiple line strips when not bound every time (eg teapots)
//	BOOL shouldSetGL = !vaPtr->isKnown;
	shouldSetGL |= (pData != vaPtr->vertices);
	shouldSetGL |= (elemSize != vaPtr->elementSize);
	shouldSetGL |= (elemType != vaPtr->elementType);
	shouldSetGL |= (vtxStride != vaPtr->vertexStride);
	shouldSetGL |= (shldNorm != vaPtr->shouldNormalize);
	if ( shouldSetGL )
	{
		vaPtr->vertices = pData;
		vaPtr->elementSize = elemSize;
		vaPtr->elementType = elemType;
		vaPtr->vertexStride = vtxStride;
		vaPtr->shouldNormalize = shldNorm;
		vaPtr->isKnown = true;
		bindVertexContentToAttributeAt( vaIdx );
	}
	vaPtr->wasBound = true;
	value_MaxVertexAttribsUsed = MAX(value_MaxVertexAttribsUsed, (GLuint)(vaIdx + 1));
}

void CC3OpenGL::bindVertexContentToAttributeAt( GLint vaIdx )
{
	// CC3AssertUnimplemented(@"bindVertexContentToAttributeAt:"); 
}

void CC3OpenGL::clearUnboundVertexAttributes()
{
	for (GLuint vaIdx = 0; vaIdx < value_MaxVertexAttribsUsed; vaIdx++)
		vertexAttributes[vaIdx].wasBound = false;
}

void CC3OpenGL::enableBoundVertexAttributes()
{
	for (GLuint vaIdx = 0; vaIdx < value_MaxVertexAttribsUsed; vaIdx++)
		enableVertexAttribute( vertexAttributes[vaIdx].wasBound, vaIdx );
}

void CC3OpenGL::enable2DVertexAttributes()
{ 
	//CC3AssertUnimplemented(@"enable2DVertexAttributes"); 
}

GLuint CC3OpenGL::generateBuffer()
{
	GLuint buffID;
	glGenBuffers(1, &buffID);
	CC3_TRACE("[ogl]glGenBuffers(%d, %d)", 1, buffID);

	CHECK_GL_ERROR_DEBUG();

	return buffID;
}

void CC3OpenGL::deleteBuffer( GLuint buffID  )
{
	if ( !buffID ) 
		return;		// Silently ignore zero ID

	glDeleteBuffers( 1, &buffID );
	
	CC3_TRACE("[ogl]glDeleteBuffers(%d, %d)", 1, buffID);
	CHECK_GL_ERROR_DEBUG();

	// If the deleted buffer is currently bound, the GL engine will automatically
	// bind to the empty buffer ID (0). Update the state tracking accordingly.
	if ( value_GL_ARRAY_BUFFER_BINDING == buffID )
		value_GL_ARRAY_BUFFER_BINDING = 0;
	if ( value_GL_ELEMENT_ARRAY_BUFFER_BINDING == buffID )
		value_GL_ELEMENT_ARRAY_BUFFER_BINDING = 0;
}

void CC3OpenGL::bindBuffer( GLuint buffId, GLenum target )
{
	if ( target == GL_ELEMENT_ARRAY_BUFFER ) 
	{
		cc3_CheckGLPrim( buffId, value_GL_ELEMENT_ARRAY_BUFFER_BINDING, isKnown_GL_ELEMENT_ARRAY_BUFFER_BINDING );
		if ( !needsUpdate ) 
			return;
	} 
	else 
	{
		cc3_CheckGLPrim( buffId, value_GL_ARRAY_BUFFER_BINDING, isKnown_GL_ARRAY_BUFFER_BINDING );
		if ( !needsUpdate ) 
			return;
	}

	bindVertexArrayObject( 0 );	// Ensure that a VAO was not left in place by Cocos2D
	glBindBuffer( target, buffId );

	CHECK_GL_ERROR_DEBUG();
	//CCLOG("[ogl]glBindBuffer(%s, %d)", stringFromGLEnum(target).c_str(), buffId);
}

void CC3OpenGL::unbindBufferTarget( GLenum target )
{
	bindBuffer( 0, target );
}

void CC3OpenGL::loadBufferTarget( GLenum target, GLvoid* buffPtr, GLsizeiptr buffLen, GLenum buffUsage )				
{
	glBufferData( target, buffLen, buffPtr, buffUsage );
	CC3_TRACE( "[ogl]glBufferData(%s, %d, %s)", stringFromGLEnum(target).c_str(), buffLen, stringFromGLEnum(buffUsage).c_str() );

	CHECK_GL_ERROR_DEBUG();
}

void CC3OpenGL::updateBufferTarget( GLenum target, GLvoid* buffPtr, GLintptr offset, GLsizeiptr length )
{
	glBufferSubData( target, offset, length, buffPtr );
	//CCLOG("glBufferSubData(%s, %d, %d)", stringFromGLEnum(target).c_str(), offset, length);

	CHECK_GL_ERROR_DEBUG();
}

void CC3OpenGL::bindVertexArrayObject( GLuint vaoId )
{
	CHECK_GL_ERROR_DEBUG();

	// Binding to VAO's is not supported on a background thread.
	// iOS & OSX do support background VAO's, but they are not shared across contexts.
	// Android seems to share the VAO binding state across contexts, which causes
	// interference between threads.
	if ( !isRenderingContext() )
		return;

#if CC3_CC2_RENDER_QUEUE || (COCOS2D_VERSION < 0x020100)
	cc3_CheckGLPrim(vaoId, value_GL_VERTEX_ARRAY_BINDING, isKnown_GL_VERTEX_ARRAY_BINDING);
	if ( !needsUpdate ) return;
	glBindVertexArray(vaoId);
	//CCLOG("glBindVertexArray(%d)", vaoId);
#else
	// If available, use Cocos2D state management. This method can be invoked from outside
	// the main rendering path (ie- during buffer loading), so Cocos2D state must be honoured.
	ccGLBindVAO(vaoId);
#endif	// COCOS2D_VERSION >= 0x020100

	CHECK_GL_ERROR_DEBUG();
}

void CC3OpenGL::drawVerticiesAs( GLenum drawMode, GLuint start, GLuint len )
{
	glDrawArrays( drawMode, start, len );
	//CCLOG("glDrawArrays(%s, %d, %d)", stringFromGLEnum(drawMode).c_str(), start, len);
	CC_INCREMENT_GL_DRAWS(1);
}

void CC3OpenGL::drawIndicies( GLvoid* indicies, GLuint len, GLenum type, GLenum drawMode )
{
#if CC3_OGLES
	CCAssert((type == GL_UNSIGNED_SHORT || type == GL_UNSIGNED_BYTE),
			  "OpenGL ES permits drawing a maximum of 65536 indexed vertices, and supports only"
			  " GL_UNSIGNED_SHORT or GL_UNSIGNED_BYTE types for vertex indices");
#endif
	CHECK_GL_ERROR_DEBUG();

	glDrawElements( drawMode, len, type, indicies );
//	CCLOG_TRACE( "glDrawElements(%s, %d, %s)", stringFromGLEnum(drawMode).c_str(), len, stringFromGLEnum(type).c_str() );
	CC_INCREMENT_GL_DRAWS(1);

	CHECK_GL_ERROR_DEBUG();
}

void CC3OpenGL::setClearColor( const ccColor4F& color )
{
	cc3_CheckGLValue(color, CCC4FAreEqual(color, value_GL_COLOR_CLEAR_VALUE),
					 value_GL_COLOR_CLEAR_VALUE, isKnown_GL_COLOR_CLEAR_VALUE);
	if ( !needsUpdate ) return;
	glClearColor(color.r, color.g, color.b, color.a);
	//LogGLErrorTrace(@"glClearColor%@", NSStringFromCCC4F(color));
}

void CC3OpenGL::setClearDepth( GLfloat val )
{
	cc3_CheckGLPrim(val, value_GL_DEPTH_CLEAR_VALUE, isKnown_GL_DEPTH_CLEAR_VALUE);
	if ( !needsUpdate ) return;
	glClearDepth(val);
	//LogGLErrorTrace(@"glClearDepth(%.3f)", val);
}

void CC3OpenGL::setClearStencil( GLint val )
{
	cc3_CheckGLPrim(val, value_GL_STENCIL_CLEAR_VALUE, isKnown_GL_STENCIL_CLEAR_VALUE);
	if ( !needsUpdate ) return;
	glClearStencil(val);
	//LogGLErrorTrace(@"glClearStencil(%i)", val);
}

void CC3OpenGL::setColor( const ccColor4F& color )
{
}

void CC3OpenGL::setColorMask( const ccColor4B& mask )
{
	// Normalize the mask to strictly 0 or 1 in each component.
	ccColor4B maskBools = ccc4(mask.r != 0, mask.g != 0, mask.b != 0, mask.a != 0);
	cc3_CheckGLValue(maskBools, CCC4BAreEqual(maskBools, value_GL_COLOR_WRITEMASK),
					 value_GL_COLOR_WRITEMASK, isKnown_GL_COLOR_WRITEMASK);
	if ( !needsUpdate ) 
		return;

	glColorMask(maskBools.r, maskBools.g, maskBools.b, maskBools.a);
	//LogGLErrorTrace(@"glColorMask%@", NSStringFromCCC4B(mask));
}

void CC3OpenGL::setCullFace( GLenum val )
{
	cc3_CheckGLPrim(val, value_GL_CULL_FACE_MODE, isKnown_GL_CULL_FACE_MODE);
	if ( !needsUpdate ) 
		return;

	glCullFace( val );	
	//LogGLErrorTrace(@"glCullFace(%@)", NSStringFromGLEnum(val));
}

void CC3OpenGL::setDepthFunc( GLenum val )
{
	cc3_CheckGLPrim(val, value_GL_DEPTH_FUNC, isKnown_GL_DEPTH_FUNC);
	if ( !needsUpdate )
		return;

	glDepthFunc( val );
	//LogGLErrorTrace(@"glDepthFunc(%@)", NSStringFromGLEnum(val));
}

void CC3OpenGL::setDepthMask( bool writable )
{
	cc3_CheckGLValue(writable, CC3BooleansAreEqual(writable, value_GL_DEPTH_WRITEMASK),
					 value_GL_DEPTH_WRITEMASK, isKnown_GL_DEPTH_WRITEMASK);
	if ( !needsUpdate ) 
		return;

	glDepthMask(writable);
	//LogGLErrorTrace(@"glDepthMask(%@)", NSStringFromBoolean(writable));
}

void CC3OpenGL::setFrontFace( GLenum val )
{
	cc3_CheckGLPrim(val, value_GL_FRONT_FACE, isKnown_GL_FRONT_FACE);
	if ( !needsUpdate ) 
		return;

	glFrontFace(val);
	//LogGLErrorTrace(@"glFrontFace(%@)", NSStringFromGLEnum(val));
}

void CC3OpenGL::setLineWidth( GLfloat val )
{
	cc3_CheckGLPrim(val, value_GL_LINE_WIDTH, isKnown_GL_LINE_WIDTH);
	if ( !needsUpdate ) 
		return;

	glLineWidth(val);
	//LogGLErrorTrace(@"glLineWidth(%.3f)", val);
}

void CC3OpenGL::setPointSize( GLfloat val )
{

}

void CC3OpenGL::setPointSizeAttenuation( const CC3AttenuationCoefficients& ac )
{

}

void CC3OpenGL::setPointSizeFadeThreshold( GLfloat val )
{
}

void CC3OpenGL::setPointSizeMinimum( GLfloat val )
{
}

void CC3OpenGL::setPointSizeMaximum( GLfloat val )
{

}

void CC3OpenGL::setPolygonOffsetFactor( GLfloat factor, GLfloat units )
{
	if ((factor != value_GL_POLYGON_OFFSET_FACTOR) ||
		(units != value_GL_POLYGON_OFFSET_UNITS) ||
		!isKnownPolygonOffset)
	{
		value_GL_POLYGON_OFFSET_FACTOR = factor;
		value_GL_POLYGON_OFFSET_UNITS = units;
		isKnownPolygonOffset = true;
		glPolygonOffset(factor, units);
		//LogGLErrorTrace(@"glPolygonOffset(%.3f, %.3f)", factor, units);

		CHECK_GL_ERROR_DEBUG();
	}
}

void CC3OpenGL::setScissor( const CC3Viewport& vp )
{
	cc3_CheckGLValue(vp, CC3ViewportsAreEqual(vp, value_GL_SCISSOR_BOX),
					 value_GL_SCISSOR_BOX, isKnown_GL_SCISSOR_BOX);
	if ( !needsUpdate )
		return;

	glScissor(vp.x, vp.y, vp.w, vp.h);
	//LogGLErrorTrace(@"glScissor%@", NSStringFromCC3Viewport(vp));

	CHECK_GL_ERROR_DEBUG();
}

void CC3OpenGL::setShadeModel( GLenum val )
{

}

void CC3OpenGL::setStencilFunc( GLenum func, GLint stencilRef, GLuint mask )
{
	if ((func != value_GL_STENCIL_FUNC) ||
		(stencilRef != value_GL_STENCIL_REF) ||
		(mask != value_GL_STENCIL_VALUE_MASK) ||
		!isKnownStencilFunc) 
	{
		value_GL_STENCIL_FUNC = func;
		value_GL_STENCIL_REF = stencilRef;
		value_GL_STENCIL_VALUE_MASK = mask;
		isKnownStencilFunc = true;
		glStencilFunc(func, stencilRef, mask);
		//LogGLErrorTrace(@"glStencilFunc(%@, %i, %u)", NSStringFromGLEnum(func), ref, mask);

		CHECK_GL_ERROR_DEBUG();
	}
}

void CC3OpenGL::setStencilMask( GLuint mask )
{
	cc3_CheckGLPrim(mask, value_GL_STENCIL_WRITEMASK, isKnown_GL_STENCIL_WRITEMASK);
	if ( !needsUpdate ) 
		return;

	glStencilMask(mask);
	//LogGLErrorTrace(@"glStencilMask(%x)", mask);

	CHECK_GL_ERROR_DEBUG();
}

void CC3OpenGL::setOpOnStencilFail( GLenum sFail, GLenum dFail, GLenum dPass )
{
	if ((sFail != value_GL_STENCIL_FAIL) ||
		(dFail != value_GL_STENCIL_PASS_DEPTH_FAIL) ||
		(dPass != value_GL_STENCIL_PASS_DEPTH_PASS) ||
		!isKnownStencilOp) 
	{
		value_GL_STENCIL_FAIL = sFail;
		value_GL_STENCIL_PASS_DEPTH_FAIL = dFail;
		value_GL_STENCIL_PASS_DEPTH_PASS = dPass;
		isKnownStencilOp = true;
		glStencilOp(sFail, dFail, dPass);
		//LogGLErrorTrace(@"glStencilOp(%@, %@, %@)", NSStringFromGLEnum(sFail), NSStringFromGLEnum(zFail), NSStringFromGLEnum(zPass));
	
		CHECK_GL_ERROR_DEBUG();
	}
}

void CC3OpenGL::setViewport( const CC3Viewport& vp )
{
	cc3_CheckGLValue(vp, CC3ViewportsAreEqual(vp, value_GL_VIEWPORT),
					 value_GL_VIEWPORT, isKnown_GL_VIEWPORT);
	if ( !needsUpdate )
		return;

	glViewport(vp.x, vp.y, vp.w, vp.h);
	//LogGLErrorTrace(@"glViewport%@", NSStringFromCC3Viewport(vp));

	CHECK_GL_ERROR_DEBUG();
}

void CC3OpenGL::enableLighting( bool onOff ){}

void CC3OpenGL::enableTwoSidedLighting( bool onOff ){}

void CC3OpenGL::setSceneAmbientLightColor( const ccColor4F& color ){}

void CC3OpenGL::enableLight( bool onOff, GLuint ltIdx ){}

void CC3OpenGL::setLightAmbientColor( const ccColor4F& color, GLuint ltIdx ){}

void CC3OpenGL::setLightDiffuseColor( const ccColor4F& color, GLuint ltIdx ){}

void CC3OpenGL::setLightSpecularColor( const ccColor4F& color, GLuint ltIdx ){}

void CC3OpenGL::setLightPosition( const CC3Vector4& pos, GLuint ltIdx ){}

void CC3OpenGL::setLightAttenuation( const CC3AttenuationCoefficients& ac, GLuint ltIdx ){}

void CC3OpenGL::setSpotlightDirection( const CC3Vector& dir, GLuint ltIdx ){}

void CC3OpenGL::setSpotlightFadeExponent( GLfloat val, GLuint ltIdx ){}

void CC3OpenGL::setSpotlightCutoffAngle( GLfloat val, GLuint ltIdx ){}

void CC3OpenGL::bindFog( CC3Fog* fog, CC3NodeDrawingVisitor* visitor ){}

void CC3OpenGL::setFogColor( const ccColor4F& color ){}

void CC3OpenGL::setFogMode( GLenum mode ){}

void CC3OpenGL::setFogDensity( GLfloat val ){}

void CC3OpenGL::setFogStart( GLfloat val ){}

void CC3OpenGL::setFogEnd( GLfloat val ){}

void CC3OpenGL::setMaterialAmbientColor(const ccColor4F& color ){}

void CC3OpenGL::setMaterialDiffuseColor(const ccColor4F& color ){}

void CC3OpenGL::setMaterialSpecularColor(const ccColor4F& color ){}

void CC3OpenGL::setMaterialEmissionColor(const ccColor4F& color ){}

void CC3OpenGL::setMaterialShininess( GLfloat val ){}

void CC3OpenGL::setAlphaFunc( GLenum func, GLfloat ref ){}

void CC3OpenGL::setBlendFunc( GLenum src, GLenum dst )
{
	setBlendFunc( src, dst, src, dst );
}

void CC3OpenGL::setBlendFunc( GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha )
{
	if ((srcRGB == value_GL_BLEND_SRC_RGB) &&
		(dstRGB == value_GL_BLEND_DST_RGB) &&
		(srcAlpha == value_GL_BLEND_SRC_ALPHA) &&
		(dstAlpha == value_GL_BLEND_DST_ALPHA) &&
		isKnownBlendFunc) return;
	
	value_GL_BLEND_SRC_RGB = srcRGB;
	value_GL_BLEND_DST_RGB = dstRGB;
	value_GL_BLEND_SRC_ALPHA = srcAlpha;
	value_GL_BLEND_DST_ALPHA = dstAlpha;
	isKnownBlendFunc = true;
	glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
	//LogGLErrorTrace(@"glBlendFuncSeparate(%@, %@, %@, %@)",
	//				NSStringFromGLEnum(srcRGB), NSStringFromGLEnum(dstRGB),
	//				NSStringFromGLEnum(srcAlpha), NSStringFromGLEnum(dstAlpha));
}


GLuint CC3OpenGL::generateTexture()
{
	GLuint texID;
	glGenTextures(1, &texID);
	CC3_TRACE("[ogl]glGenTextures(%d, %d)", 1, texID);

	CHECK_GL_ERROR_DEBUG();

	return texID;
}

void CC3OpenGL::deleteTexture( GLuint texID )
{
	if ( !texID ) 
		return;		// Silently ignore zero texture ID
	glDeleteTextures(1, &texID);
	CC3_TRACE("[ogl]glDeleteTextures(%d, %d)", 1, texID);

    // If the deleted texture is currently bound to a texture unit, the GL engine will automatically
	// bind the default texture ID (0) to that texture unit. Update the state tracking accordingly.
    clearTextureBinding( texID );

	CHECK_GL_ERROR_DEBUG();
}

void CC3OpenGL::clearTextureBinding( GLuint texID )
{
	GLuint maxTexUnits = value_MaxTextureUnitsUsed;
	for (GLuint tuIdx = 0; tuIdx < maxTexUnits; tuIdx++) 
	{
		if (value_GL_TEXTURE_BINDING_2D[tuIdx] == texID)
			value_GL_TEXTURE_BINDING_2D[tuIdx] = 0;
		if (value_GL_TEXTURE_BINDING_CUBE_MAP[tuIdx] == texID)
			value_GL_TEXTURE_BINDING_CUBE_MAP[tuIdx] = 0;
	}
}

void CC3OpenGL::loadTexureImage( const GLvoid* imageData, GLenum target, GLint mipmapLevel, const CC3IntSize& size, GLenum texelFormat, GLenum texelType, GLint byteAlignment, GLuint tuIdx )
{
	CCAssert((GLuint)size.width <= getMaxTextureSizeForTarget(target) && (GLuint)size.height <= getMaxTextureSizeForTarget(target),
			  "%s exceeds the maximum texture size, %d per side, for target %s"/*,
			 stringFromCC3IntSize(size).c_str(), maxTextureSizeForTarget(target), stringFromGLEnum(target).c_str()*/);

	activateTextureUnit( tuIdx );
	setPixelUnpackingAlignment( byteAlignment );
	glTexImage2D(target, mipmapLevel, texelFormat, size.width, size.height, 0, texelFormat, texelType, imageData);
	CC3_TRACE("[ogl]glTexImage2D(%s, %d, %s, %d, %d, %d, %s, %s)",
					stringFromGLEnum(target).c_str(), mipmapLevel, stringFromGLEnum(texelFormat).c_str(), size.width, size.height,
					0, stringFromGLEnum(texelFormat).c_str(), stringFromGLEnum(texelType).c_str(), imageData);

	CHECK_GL_ERROR_DEBUG();

	imageData = NULL;
}

void CC3OpenGL::loadTexureSubImage( const GLvoid* imageData, GLenum target, GLint mipmapLevel, const CC3Viewport& rect, GLenum texelFormat, GLenum texelType, GLint byteAlignment, GLuint tuIdx )
{
	activateTextureUnit( tuIdx );
	setPixelUnpackingAlignment( byteAlignment );
	glTexSubImage2D(target, mipmapLevel, rect.x, rect.y, rect.w, rect.h, texelFormat, texelType, imageData);
//	CCLOG_TRACE("[ogl]glTexSubImage2D(%s, %d, %d, %d, %d, %d, %s, %s)",
//		stringFromGLEnum(target).c_str(), mipmapLevel, rect.x, rect.y, rect.w, rect.h,
//		stringFromGLEnum(texelFormat).c_str(), stringFromGLEnum(texelType).c_str(), imageData);

	CHECK_GL_ERROR_DEBUG();
}

// Activate the current texture unit, and keep track of the maximum
// number of texture units that have been concurrently activated.
void CC3OpenGL::activateTextureUnit( GLuint tuIdx )
{
	cc3_CheckGLPrim(tuIdx, value_GL_ACTIVE_TEXTURE, isKnown_GL_ACTIVE_TEXTURE);
	if ( !needsUpdate ) 
		return;

	CHECK_GL_ERROR_DEBUG();

	glActiveTexture(GL_TEXTURE0 + tuIdx);
//	CCLOG_TRACE("[ogl]glActiveTexture(%s)", stringFromGLEnum(GL_TEXTURE0 + tuIdx).c_str());
	value_MaxTextureUnitsUsed = MAX(value_MaxTextureUnitsUsed, tuIdx + 1);

	CHECK_GL_ERROR_DEBUG();
}

void CC3OpenGL::activateClientTextureUnit( GLuint tuIdx )
{

}

void CC3OpenGL::enableTexturing( bool onOff, GLenum target, GLuint tuIdx )
{

}

void CC3OpenGL::disableTexturingAt( GLuint tuIdx )
{
	//CC3AssertUnimplemented(@"disableTexturingAt:"); 
}

void CC3OpenGL::disableTexturingFrom( GLuint startTexUnitIdx )
{
	GLuint maxTexUnits = value_MaxTextureUnitsUsed;
	for (GLuint tuIdx = startTexUnitIdx; tuIdx < maxTexUnits; tuIdx++)
		disableTexturingAt( tuIdx );
}

void CC3OpenGL::bindTexture( GLuint texID, GLenum target, GLuint tuIdx )
{
	GLuint* stateArray;
	GLbitfield* isKnownBits;

	switch (target) 
	{
		case GL_TEXTURE_2D:
			stateArray = value_GL_TEXTURE_BINDING_2D;
			isKnownBits = &isKnown_GL_TEXTURE_BINDING_2D;
			break;
		case GL_TEXTURE_CUBE_MAP:
			stateArray = value_GL_TEXTURE_BINDING_CUBE_MAP;
			isKnownBits = &isKnown_GL_TEXTURE_BINDING_CUBE_MAP;
			break;
		default:
			CCAssert(false, "Texture target %s is not a valid binding target."/*, stringFromGLEnum(target).c_str()*/);
			return;
	}


	if (CC3CheckGLuintAt(tuIdx, texID, stateArray, isKnownBits)) 
	{
		CHECK_GL_ERROR_DEBUG();

		activateTextureUnit( tuIdx );
		glBindTexture(target, texID);

		CHECK_GL_ERROR_DEBUG();
		//CCLOG("[ogl]glBindTexture(%s, %d)", stringFromGLEnum(target).c_str(), texID);

		// If a real texture was set in this target, unbind all other targets in this texture unit
		if (texID) 
			unbindTexturesExceptTarget( target, tuIdx );
	}
}

/** Unbind all targets in the specified texture unit except the specified target. */
void CC3OpenGL::unbindTexturesExceptTarget( GLenum target, GLuint tuIdx )
{
	//CC3AssertUnimplemented(@"unbindTexturesExceptTarget:at:");
}

/** Sets the specified texture parameter for the specified texture unit, without checking a cache. */
void CC3OpenGL::setTexParamEnum( GLenum pName, GLenum target, GLenum val, GLuint tuIdx )
{
	activateTextureUnit( tuIdx );
	glTexParameteri(target, pName, val);
	//LogGLErrorTrace(@"glTexParameteri(%@, %@, %@)", NSStringFromGLEnum(target),
	//				NSStringFromGLEnum(pName), NSStringFromGLEnum(val));

	CHECK_GL_ERROR_DEBUG();
}

void CC3OpenGL::setTextureMinifyFunc( GLenum func, GLenum target, GLuint tuIdx )
{
	setTexParamEnum( GL_TEXTURE_MIN_FILTER, target, func, tuIdx );
}

void CC3OpenGL::setTextureMagnifyFunc( GLenum func, GLenum target, GLuint tuIdx )
{
	setTexParamEnum( GL_TEXTURE_MAG_FILTER, target, func, tuIdx );
}

void CC3OpenGL::setTextureHorizWrapFunc( GLenum func, GLenum target, GLuint tuIdx )
{
	setTexParamEnum( GL_TEXTURE_WRAP_S, target, func, tuIdx );
}

void CC3OpenGL::setTextureVertWrapFunc( GLenum func, GLenum target, GLuint tuIdx )
{
	setTexParamEnum( GL_TEXTURE_WRAP_T, target, func, tuIdx );
}

void CC3OpenGL::generateMipmapForTarget( GLenum target, GLuint tuIdx )
{
	activateTextureUnit( tuIdx );
	glGenerateMipmap(target);
	CC3_TRACE("[ogl]glGenerateMipmap(%s)", stringFromGLEnum(target).c_str());

	CHECK_GL_ERROR_DEBUG();
}

void CC3OpenGL::setTextureEnvMode( GLenum mode, GLuint tuIdx )
{

}

void CC3OpenGL::setTextureEnvColor( const ccColor4F& color, GLuint tuIdx )
{

}

void CC3OpenGL::enablePointSpriteCoordReplace( bool onOff, GLuint tuIdx )
{

}

std::string CC3OpenGL::dumpTextureBindings()
{
	std::string desc = "";
	desc += "Current GL texture bindings:";
	for (GLuint tuIdx = 0; tuIdx < value_MaxTextureUnitsUsed; tuIdx++) 
	{
		activateTextureUnit( tuIdx );
		desc += CC3String::stringWithFormat( (char*)"\n\tTexture Unit %d: %s", tuIdx, dumpTextureBindingsAt( tuIdx ).c_str() );
	}
	return desc;
}

std::string CC3OpenGL::dumpTextureBindingsAt( GLuint tuIdx )
{
	// CC3AssertUnimplemented(@"dumpTextureBindingsAt:");
	return "";
}

void CC3OpenGL::activateMatrixStack( GLenum mode )
{

}

void CC3OpenGL::activatePaletteMatrixStack( GLuint pmIdx )
{
}

void CC3OpenGL::loadModelviewMatrix( const CC3Matrix4x3* mtx )
{
}

void CC3OpenGL::loadProjectionMatrix( const CC3Matrix4x4* mtx )
{
}

void CC3OpenGL::loadPaletteMatrix( const CC3Matrix4x3* mtx, GLuint pmIdx )
{
}

void CC3OpenGL::pushModelviewMatrixStack()
{
}

void CC3OpenGL::popModelviewMatrixStack() 
{

}

void CC3OpenGL::pushProjectionMatrixStack()
{

}

void CC3OpenGL::popProjectionMatrixStack() 
{

}

void CC3OpenGL::setFogHint( GLenum hint )
{}

void CC3OpenGL::setGenerateMipmapHint( GLenum hint )
{
	cc3_CheckGLPrim(hint, value_GL_GENERATE_MIPMAP_HINT, isKnown_GL_GENERATE_MIPMAP_HINT);
	if ( !needsUpdate )
		return;
	glHint(GL_GENERATE_MIPMAP_HINT, hint);
	//LogGLErrorTrace(@"glHint(%@, %@)", NSStringFromGLEnum(GL_GENERATE_MIPMAP_HINT), NSStringFromGLEnum(hint));

	CHECK_GL_ERROR_DEBUG();
}

void CC3OpenGL::setLineSmoothingHint( GLenum hint )
{

}

void CC3OpenGL::setPerspectiveCorrectionHint( GLenum hint )
{

}

void CC3OpenGL::setPointSmoothingHint( GLenum hint )
{

}

GLuint CC3OpenGL::generateFramebuffer()
{
	GLuint fbID;
	glGenFramebuffers(1, &fbID);
	//LogGLErrorTrace(@"glGenFramebuffers(%i, %u)", 1, fbID);

	CHECK_GL_ERROR_DEBUG();

	return fbID;
}

void CC3OpenGL::deleteFramebuffer( GLuint fbID )
{
	if ( !fbID )
		return;		// Silently ignore zero ID
	glDeleteFramebuffers(1, &fbID);
	//LogGLErrorTrace(@"glDeleteFramebuffers(%i, %u)", 1,fbID);
	
	// If the deleted buffer is currently bound, the GL engine will automatically
	// bind to the empty buffer ID (0). Update the state tracking accordingly.
	if (value_GL_FRAMEBUFFER_BINDING == fbID) 
		value_GL_FRAMEBUFFER_BINDING = 0;

	CHECK_GL_ERROR_DEBUG();
}

/** Returns whether the specified framebuffer ID is the currently bound value. */
bool CC3OpenGL::checkGLFramebuffer( GLuint fbID )
{
	cc3_CheckGLPrim(fbID, value_GL_FRAMEBUFFER_BINDING, isKnown_GL_FRAMEBUFFER_BINDING);
	return !needsUpdate;
}

/** Returns whether the specified framebuffer target is the currently bound value. */
bool CC3OpenGL::checkGLFramebufferTarget( GLenum fbTarg )
{
	cc3_CheckGLPrim(fbTarg, value_GL_FRAMEBUFFER_Target, isKnown_GL_FRAMEBUFFER_Target);
	return !needsUpdate;
}

void CC3OpenGL::bindFramebuffer( GLuint fbID, GLenum fbTarget )
{
	if ( checkGLFramebuffer( fbID ) && checkGLFramebufferTarget( fbTarget ) )
		return;
	
	glBindFramebuffer(fbTarget, fbID);
	//LogGLErrorTrace(@"glBindFramebuffer(%@, %u)", NSStringFromGLEnum(fbTarget), fbID);

	CHECK_GL_ERROR_DEBUG();
}

void CC3OpenGL::bindFramebuffer( GLuint fbID )
{
	bindFramebuffer( fbID, GL_FRAMEBUFFER );
}

void CC3OpenGL::resolveMultisampleFramebuffer( GLuint fbSrcID, GLuint fbDstID )
{
	bindFramebuffer( fbSrcID, GL_FRAMEBUFFER );
}

void CC3OpenGL::discard( GLsizei count, const GLenum* attachments, GLuint fbID )
{

}

GLuint CC3OpenGL::generateRenderbuffer()
{
	GLuint rbID;
	glGenRenderbuffers(1, &rbID);
	//LogGLErrorTrace(@"glGenRenderbuffers(%i, %u)", 1, rbID);

	CHECK_GL_ERROR_DEBUG();

	return rbID;
}

void CC3OpenGL::deleteRenderbuffer( GLuint rbID )
{
	if ( !rbID ) 
		return;		// Silently ignore zero ID
	glDeleteRenderbuffers(1, &rbID);
	
	// If the deleted buffer is currently bound, the GL engine will automatically
	// bind to the empty buffer ID (0). Update the state tracking accordingly.
	if (value_GL_RENDERBUFFER_BINDING == rbID) 
		value_GL_RENDERBUFFER_BINDING = 0;
	CHECK_GL_ERROR_DEBUG();
}

void CC3OpenGL::bindRenderbuffer( GLuint rbID )
{
	cc3_CheckGLPrim(rbID, value_GL_RENDERBUFFER_BINDING, isKnown_GL_RENDERBUFFER_BINDING);
	if ( !needsUpdate ) 
		return;
	glBindRenderbuffer(GL_RENDERBUFFER, rbID);

	CHECK_GL_ERROR_DEBUG();
}

void CC3OpenGL::allocateStorageForRenderbuffer( GLuint rbID, const CC3IntSize& size, GLenum format, GLuint pixelSamples )
{
	CCAssert((GLuint)size.width <= getMaxRenderbufferSize() && (GLuint)size.height <= getMaxRenderbufferSize(),
			  "%s exceeds the maximum renderbuffer size, %d per side"/*,
			  stringFromCC3IntSize(size).c_str(), getMaxRenderbufferSize()*/);
	
	bindRenderbuffer( rbID );
	glRenderbufferStorage(GL_RENDERBUFFER, format, size.width, size.height);
	
	CHECK_GL_ERROR_DEBUG();
}

GLint CC3OpenGL::getRenderbufferParameterInteger( GLenum param )
{
	GLint val;
	glGetRenderbufferParameteriv(GL_RENDERBUFFER, param, &val);

	CHECK_GL_ERROR_DEBUG();

	return val;
}

void CC3OpenGL::bindRenderbuffer( GLuint rbID, GLuint fbID, GLenum attachment )
{
	bindFramebuffer( fbID );	
	bindRenderbuffer( rbID );
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, rbID);
	CHECK_GL_ERROR_DEBUG();
}

void CC3OpenGL::bindTexture2D( GLuint texID, GLenum face, GLint mipmapLevel, GLuint fbID, GLenum attachment )
{
	bindFramebuffer( fbID );
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, face, texID, mipmapLevel);
	CHECK_GL_ERROR_DEBUG();
}

bool CC3OpenGL::checkFramebufferStatus( GLuint fbID )
{
	bindFramebuffer( fbID );
	
	CHECK_GL_ERROR_DEBUG();

	GLenum fbStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	//LogGLErrorTrace(@"glCheckFramebufferStatus(%@)", NSStringFromGLEnum(GL_FRAMEBUFFER));
	if (fbStatus == GL_FRAMEBUFFER_COMPLETE) 
		return true;

	CC3_ERROR( "Framebuffer %u is incomplete: %s", fbID, stringFromGLEnum(fbStatus).c_str() );
	CCAssert(!GL_ERROR_ASSERTION_ENABLED,
			   "%s To disable this assertion and just log the GL error, set the preprocessor macro GL_ERROR_ASSERTION_ENABLED=0 in your project build settings.\n"/*,
			   stringWithFormat( "Framebuffer %d is incomplete: %s", fbID, stringFromGLEnum(fbStatus).c_str() )*/);
	return false;
}

void CC3OpenGL::clearBuffers( GLbitfield mask )
{
	glClear(mask);
	
	CHECK_GL_ERROR_DEBUG();
}

void CC3OpenGL::readPixelsIn( const CC3Viewport& rect, GLuint fbID, ccColor4B* colorArray )
{
	GLuint currFB = value_GL_FRAMEBUFFER_BINDING;
	bindFramebuffer( fbID );
	setPixelPackingAlignment( 1 );
	glReadPixels(rect.x, rect.y, rect.w, rect.h, GL_RGBA, GL_UNSIGNED_BYTE, colorArray);
	
	CHECK_GL_ERROR_DEBUG();

	bindFramebuffer( currFB );
}

void CC3OpenGL::setPixelPackingAlignment( GLint byteAlignment )
{
	cc3_CheckGLPrim(byteAlignment, value_GL_PACK_ALIGNMENT, isKnown_GL_PACK_ALIGNMENT);
	if ( !needsUpdate ) 
		return;
	glPixelStorei(GL_PACK_ALIGNMENT, byteAlignment);
	//LogGLErrorTrace(@"glPixelStorei(%@, %i)", NSStringFromGLEnum(GL_PACK_ALIGNMENT), byteAlignment);
}

void CC3OpenGL::setPixelUnpackingAlignment( GLint byteAlignment )
{
	cc3_CheckGLPrim(byteAlignment, value_GL_UNPACK_ALIGNMENT, isKnown_GL_UNPACK_ALIGNMENT);
	if ( !needsUpdate ) 
		return;
	glPixelStorei(GL_UNPACK_ALIGNMENT, byteAlignment);
	//LogGLErrorTrace(@"glPixelStorei(%@, %i)", NSStringFromGLEnum(GL_UNPACK_ALIGNMENT), byteAlignment);
}

void CC3OpenGL::flush()
{
	glFlush(); 
}

void CC3OpenGL::finish()
{
	glFinish(); 
}

GLint CC3OpenGL::getInteger( GLenum param )
{
	GLint val;
	glGetIntegerv(param, &val);
	
	CHECK_GL_ERROR_DEBUG();

	return val;
}

GLfloat CC3OpenGL::getFloat( GLenum param )
{
	GLfloat val;
	glGetFloatv(param, &val);
	
	CHECK_GL_ERROR_DEBUG();

	return val;
}

std::string CC3OpenGL::getString( GLenum param )
{
	std::string val = "";
		
	const char* cstrVal =  (char*)glGetString( param );
	if ( cstrVal )
		val = cstrVal;
	
	CHECK_GL_ERROR_DEBUG();

	return val;
}

GLuint CC3OpenGL::getMaxNumberOfLights()
{
	return value_GL_MAX_LIGHTS; 
}

GLuint CC3OpenGL::getMaxNumberOfClipPlanes()
{
	return value_GL_MAX_CLIP_PLANES; 
}

GLuint CC3OpenGL::getMaxNumberOfPaletteMatrices()
{
	return value_GL_MAX_PALETTE_MATRICES; 
}

GLuint CC3OpenGL::getMaxNumberOfTextureUnits()
{
	return value_GL_MAX_TEXTURE_UNITS; 
}

GLuint CC3OpenGL::getMaxNumberOfVertexAttributes()
{
	return value_GL_MAX_VERTEX_ATTRIBS; 
}

GLuint CC3OpenGL::getMaxNumberOfBoneInfluencesPerVertex()
{
	return valueMaxBoneInfluencesPerVertex; 
}
	
GLuint CC3OpenGL::getMaxNumberOfVertexUnits()
{
	return getMaxNumberOfBoneInfluencesPerVertex(); 
}

GLuint CC3OpenGL::getMaxNumberOfPixelSamples()
{
	return value_GL_MAX_SAMPLES; 
}

GLuint CC3OpenGL::getMaxTextureSize() 
{ 
	return value_GL_MAX_TEXTURE_SIZE; 
}

GLuint CC3OpenGL::getMaxRenderbufferSize() 
{
	return value_GL_MAX_RENDERBUFFER_SIZE; 
}

GLuint CC3OpenGL::getMaxCubeMapTextureSize()
{
	return value_GL_MAX_CUBE_MAP_TEXTURE_SIZE; 
}

GLuint CC3OpenGL::getMaxTextureSizeForTarget( GLenum target )
{
	switch (target) 
	{
		case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
		case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
		case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
			return getMaxCubeMapTextureSize();
		case GL_TEXTURE_2D:
		default:
			return getMaxTextureSize();
	}
}

GLuint CC3OpenGL::getMaxNumberOfVertexShaderUniformVectors()
{
	return 0; 
}

GLuint CC3OpenGL::getMaxNumberOfFragmentShaderUniformVectors()
{
	return 0; 
}

GLuint CC3OpenGL::getMaxNumberOfShaderProgramVaryingVectors()
{
	return 0; 
}

GLfloat CC3OpenGL::getVertexShaderVarRangeMin( GLenum precisionType )
{ 
	return 0.0f; 
}

GLfloat CC3OpenGL::getVertexShaderVarRangeMax( GLenum precisionType )
{ 
	return 0.0f; 
}

GLfloat CC3OpenGL::getVertexShaderVarPrecision( GLenum precisionType )
{ 
	return 0.0f; 
}

GLfloat CC3OpenGL::getFragmentShaderVarRangeMin( GLenum precisionType )
{ 
	return 0.0f; 
}

GLfloat CC3OpenGL::getFragmentShaderVarRangeMax( GLenum precisionType )
{ 
	return 0.0f; 
}

GLfloat CC3OpenGL::getFragmentShaderVarPrecision( GLenum precisionType )
{ 
	return 0.0f; 
}

/** Returns the specified extension name, stripped of any optional GL_ prefix. */
std::string CC3OpenGL::trimGLPrefix( const char* extensionName )
{
	CCAssert( false, "trimGLPrefix is not implemented" );
	std::string extPfx = "GL_";
	/*return ([extensionName hasPrefix: extPfx]
	? [extensionName substringFromIndex: extPfx.length]
	: extensionName);*/
	return extPfx;
}

CCSet* CC3OpenGL::getExtensions()
{
	if ( !_extensions ) 
	{
		_extensions = CCSet::create();		// retained
		_extensions->retain();

		CCAssert( false, "getExtensions is not implemented" );
		/*CCArray* rawExts = getString( GL_EXTENSIONS );

		componentsSeparatedByCharactersInSet:
		[NSCharacterSet whitespaceCharacterSet]];
		for (NSString* extName in rawExts) {
		NSString* trimmedName = [self trimGLPrefix: extName];
		if (trimmedName.length > 0) [_extensions addObject: trimmedName];
		}*/
	}
	return _extensions;
}

bool CC3OpenGL::supportsExtension( const char* extensionName )
{
	CCAssert( false, "supportsExtension is not implemented" );
	//return [_extensions containsObject: [self trimGLPrefix: extensionName]];
	return true;
}

/** Returns a description of the available extensions. */
//-(NSString*) extensionsDescription {
//	NSMutableString* desc = [NSMutableString stringWithCapacity: 1000];
//	NSSortDescriptor* sorter = [NSSortDescriptor sortDescriptorWithKey: @"description"
//															 ascending: YES
//															  selector: @selector(caseInsensitiveCompare:)];
//	NSArray* sorted = [self.extensions sortedArrayUsingDescriptors: [NSArray arrayWithObject: sorter]];
//	for (NSString* ext in sorted) [desc appendFormat: @"\n\t%@", ext];
//	return desc;
//}


CC3ShaderPrewarmer* CC3OpenGL::getShaderProgramPrewarmer()
{
	return NULL;
}

void CC3OpenGL::setShaderProgramPrewarmer( CC3ShaderPrewarmer* shaderProgramPrewarmer )
{

}

CC3ShaderSemanticsDelegate* CC3OpenGL::getSemanticDelegate()
{
	return NULL; 
}

void CC3OpenGL::setSemanticDelegate( CC3ShaderSemanticsDelegate* semanticDelegate )
{

}

GLuint CC3OpenGL::createShader( GLenum shaderType )
{
	return 0; 
}

void CC3OpenGL::deleteShader( GLuint shaderID )
{

}


void CC3OpenGL::compileShader( GLuint shaderID, GLuint srcStrCount, const GLchar** srcCodeStrings )
{
}

bool CC3OpenGL::getShaderWasCompiled( GLuint shaderID )
{
	return false; 
}

std::string CC3OpenGL::getLogForShader( GLuint shaderID )
{ 
	return ""; 
}

std::string CC3OpenGL::getSourceCodeForShader( GLuint shaderID )
{ 
	return "";
}

std::string CC3OpenGL::defaultShaderPreamble()
{
	return ""; 
}

GLuint CC3OpenGL::createShaderProgram()
{
	return 0; 
}

void CC3OpenGL::deleteShaderProgram( GLuint programID )
{
}

void CC3OpenGL::attachShader( GLuint shaderID, GLuint programID )
{

}

void CC3OpenGL::detachShader( GLuint shaderID, GLuint programID )
{
}

void CC3OpenGL::linkShaderProgram( GLuint programID )
{
}

bool CC3OpenGL::getShaderProgramWasLinked( GLuint programID )
{
	return false; 
}

GLint CC3OpenGL::getIntegerParameterForShaderProgram( GLenum param, GLuint programID )
{
	return 0;
}

GLint CC3OpenGL::getIntegerParameterForShader( GLenum param, GLuint programID )
{
	return 0; 
}

void CC3OpenGL::useShaderProgram( GLuint programID )
{

}

std::string CC3OpenGL::getLogForShaderProgram( GLuint programID )
{
	return ""; 
}

void CC3OpenGL::populateShaderProgramVariable( CC3GLSLVariable* var )
{
}

void CC3OpenGL::setShaderProgramUniformValue( CC3GLSLUniform* uniform )
{
}

void CC3OpenGL::releaseShaderCompiler()
{

}

void CC3OpenGL::pushGroupMarker( const char* marker )
{
	pushGroupMarkerC( marker ); 
}

void CC3OpenGL::pushGroupMarkerC( const char* marker )
{
	//glPushGroupMarkerEXT(0, marker);
	//LogGLErrorTrace(@"glPushGroupMarkerEXT(0, %@)", [NSString stringWithUTF8String: marker]);
}

void CC3OpenGL::popGroupMarker() 
{
	//glPopGroupMarkerEXT();
//	LogGLErrorTrace(@"glPopGroupMarkerEXT()");	// Log appears outside the group, which creates clutter.
}

void CC3OpenGL::insertEventMarker( const char* marker )
{ 
	insertEventMarkerC( marker );
}

void CC3OpenGL::insertEventMarkerC( const char* marker )
{
	//glInsertEventMarkerEXT(0, marker);
	//LogGLErrorTrace(@"glInsertEventMarkerEXT(0, %@)", [NSString stringWithUTF8String: marker]);
}

void CC3OpenGL::captureOpenGLFrame()
{
	insertEventMarkerC( "com.apple.GPUTools.event.debug-frame" ); 
}

#if APPORTABLE
void CC3OpenGL::setDebugLabel( const char* label, GLuint objID, GLenum objType ) {}
void CC3OpenGL::setTextureDebugLabel( const char* label, GLuint texID ) {}
void CC3OpenGL::setBufferDebugLabel( const char* label, GLuint buffID ) {}
void CC3OpenGL::setShaderDebugLabel( const char* label, GLuint shaderID ) {}
void CC3OpenGL::setProgramDebugLabel( const char* label, GLuint progID ) {}
void CC3OpenGL::setFrameBufferDebugLabel( const char* label, GLuint fbID ) {}
void CC3OpenGL::setRenderBufferDebugLabel( const char* label, GLuint rbID ) {}
void CC3OpenGL::setVertexArrayDebugLabel( const char* label, GLuint vaID ) {}
#else
void CC3OpenGL::setDebugLabel( const char* label, GLuint objID, GLenum objType )
{
	//glLabelObjectEXT(objType, objID, 0, label);
	//LogGLErrorTrace(@"glLabelObjectEXT(%@, %u, 0, %@)", NSStringFromGLEnum(objType), objID, label);
}

void CC3OpenGL::setTextureDebugLabel( const char* label, GLuint texID )
{
	setDebugLabel( label, texID, GL_TEXTURE );
}

void CC3OpenGL::setBufferDebugLabel( const char* label, GLuint buffID )
{
	//setDebugLabel( label, buffID, GL_BUFFER_OBJECT_EXT );
}

void CC3OpenGL::setShaderDebugLabel( const char* label, GLuint shaderID )
{
	//setDebugLabel( label, shaderID, GL_SHADER_OBJECT_EXT );
}

void CC3OpenGL::setProgramDebugLabel( const char* label, GLuint progID )
{
	//setDebugLabel( label, progID, GL_PROGRAM_OBJECT_EXT );
}

void CC3OpenGL::setFrameBufferDebugLabel( const char* label, GLuint fbID )
{
	setDebugLabel( label, fbID, GL_FRAMEBUFFER );
}

void CC3OpenGL::setRenderBufferDebugLabel( const char* label, GLuint rbID )
{
	setDebugLabel( label, rbID, GL_RENDERBUFFER );
}

void CC3OpenGL::setVertexArrayDebugLabel( const char* label, GLuint vaID )
{
	//setDebugLabel( label, vaID, GL_VERTEX_ARRAY_OBJECT_EXT );
}
#endif	// APPORTABLE

void CC3OpenGL::alignFor2DDrawingWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	
	// Default GL states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
	
	// Restore 2D standard blending
	enableBlend( true );	// if director setAlphaBlending: NO, needs to be overridden
	setBlendFunc( CC_BLEND_SRC, CC_BLEND_DST );
	enableAlphaTesting( false );
	
	// Enable vertex attributes needed for 2D, disable all others, unbind GL buffers.
	enable2DVertexAttributes();
	unbindBufferTarget( GL_ARRAY_BUFFER );
	unbindBufferTarget( GL_ELEMENT_ARRAY_BUFFER );
	
	// Disable, and remove both 2D & cube texture bindings from, all texture units.
	// Then enable texture unit 0 for 2D textures only, and default texture unit blending.
	disableTexturingFrom( 0 );
	enableTexturing( true, GL_TEXTURE_2D, 0 );
	setTextureEnvMode( GL_MODULATE, 0 );
	setTextureEnvColor( kCCC4FBlackTransparent, 0 );
	
	// Ensure texture unit zero is the active texture unit. Code above might leave another active.
	activateTextureUnit( 0 );
	activateClientTextureUnit( 0 );
	
	// Ensure GL_MODELVIEW matrix is active under OGLES 1.1.
	activateMatrixStack( GL_MODELVIEW );
	
	align2DStateCacheWithVisitor( visitor );	// Align the 2D GL state cache with current settings
}

void CC3OpenGL::align2DStateCacheWithVisitor( CC3NodeDrawingVisitor* visitor )
{

}

void CC3OpenGL::alignFor3DDrawingWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	bindVertexArrayObject( 0 );	// Ensure that a VAO was not left in place by Cocos2D
	align3DStateCacheWithVisitor( visitor );
}

void CC3OpenGL::align3DStateCacheWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	isKnownCap_GL_BLEND = false;
	isKnownBlendFunc = false;
	isKnown_GL_ARRAY_BUFFER_BINDING = false;
	isKnown_GL_ELEMENT_ARRAY_BUFFER_BINDING = false;
	CC3SetBit(&isKnown_GL_TEXTURE_BINDING_2D, 0, false);	// Unknown texture in tex unit zero

	align3DVertexAttributeStateWithVisitor( visitor );
}

void CC3OpenGL::align3DVertexAttributeStateWithVisitor( CC3NodeDrawingVisitor* visitor )
{

}

void CC3OpenGL::clearOpenGLResourceCaches()
{
	//LogInfo(@"Clearing resource caches on thread %@.", NSThread.currentThread);
	
	setShaderProgramPrewarmer( NULL );
	
	CC3Resource::removeAllResources();
	CC3Texture::removeAllTextures();
	CC3ShaderProgram::removeAllPrograms();
	CC3Shader::removeAllShaders();
	CC3ShaderSourceCode::removeAllShaderSourceCode();

	// Dynamically reference model factory class, as it might not be present.
	//[NSClassFromString(@"CC3ModelSampleFactory") deleteFactory];
}

void CC3OpenGL::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );

	//LogInfoIfPrimary(@"Third dimension provided by %@", NSStringFromCC3Version());
	//LogInfo(@"Starting GL context %@ on %@", self, NSThread.currentThread);
	initDeletionDelay();
	initGLContext();
	initPlatformLimits();
	initSurfaces();
	initVertexAttributes();
	initTextureUnits();
	initExtensions();
}

/** Template method to establish the initial value of the deletionDelay property. */
void CC3OpenGL::initDeletionDelay() 
{ 
	_deletionDelay = isRenderingContext() ? 0.0f : 0.25f; 
}

/** Template method to establish the OpenGL engine context. */
void CC3OpenGL::initGLContext() 
{
	setContext( isRenderingContext() ? makeRenderingGLContext() : makeBackgroundGLContext() );
#pragma _NOTE_TODO( "CC3OpenGL::initGLContext() " )
	//_context->ensureCurrentContext();
}

/** Template method to create and return the primary rendering OpenGL context. */
CC3GLContext* CC3OpenGL::makeRenderingGLContext()
{
#pragma _NOTE_TODO( "CC3OpenGL::makeRenderingGLContext()" )
	//CC3GLContext* context = CCDirector::sharedDirector()->getOpenGLView()->getContext();
	//CCAssert(context, "OpenGL context not available. Be sure to install the CCGLView in"
	//		  " the CCDirector before attempting to process any OpenGL calls.");
	//return context;
	
	return NULL;
}

/**
 * Template method to create and return the background OpenGL context.
 *
 * This implementation creates the background GL context from the primary rendering context,
 * using a sharegroup, so that the two can share GL objects.
 */
CC3GLContext* CC3OpenGL::makeBackgroundGLContext()
{
	CCAssert(_renderGL, "The background OpenGL context cannot be created until after the"
			  " rendering OpenGL context has been created on the main rendering thread."
			  " Ensure that you invoke [CC3OpenGL sharedContext] from the main thread,"
			  " prior to invoking it on a background thread.");
	
#pragma _NOTE_TODO( "CC3OpenGL::makeBackgroundGLContext()" )
	//return _renderGL->getContext( asSharedContext() );
	return NULL;
}

/** Template method to retrieve the GL platform limits. */
void CC3OpenGL::initPlatformLimits()
{
	value_GL_VENDOR = getString( GL_VENDOR );
	value_GL_RENDERER = getString( GL_RENDERER );
	value_GL_VERSION = getString( GL_VERSION );
	value_GL_MAX_TEXTURE_SIZE = getInteger( GL_MAX_TEXTURE_SIZE );
	value_GL_MAX_RENDERBUFFER_SIZE = getInteger( GL_MAX_RENDERBUFFER_SIZE );

#if CC3_OGLES_2
	value_GL_MAX_POINT_SIZE = kCC3MaxGLfloat;
#else
	value_GL_MAX_POINT_SIZE = getFloat( GL_POINT_SIZE_MAX );
#endif
}

/** Initializes surfaces frameworks. */
void CC3OpenGL::initSurfaces()
{

}

/** Initializes vertex attributes. This must be invoked after the initPlatformLimits. */
void CC3OpenGL::initVertexAttributes()
{
	vertexAttributes = (CC3VertexAttr*)calloc(value_GL_MAX_VERTEX_ATTRIBS, sizeof(CC3VertexAttr));
}

/** Initializes the texture units. This must be invoked after the initPlatformLimits. */
void CC3OpenGL::initTextureUnits()
{
	value_MaxTextureUnitsUsed = 0;

	value_GL_TEXTURE_BINDING_2D = (GLuint*)calloc(value_GL_MAX_TEXTURE_UNITS, sizeof(GLuint));
	value_GL_TEXTURE_BINDING_CUBE_MAP = (GLuint*)calloc(value_GL_MAX_TEXTURE_UNITS, sizeof(GLuint));
}

/** Performs any required initialization for GL extensions supported by this platform. */
void CC3OpenGL::initExtensions()
{
	//LogInfoIfPrimary(@"GL extensions supported by this platform: %@", self.extensionsDescription);
}

/** Returns the appropriate class cluster subclass instance. */
//+(id) CC3OpenGL::alloc {
//	if (self == [CC3OpenGL class]) return [CC3OpenGLClass alloc];
//	return [super alloc];
//}

bool CC3OpenGL::isRenderingContext()
{ 
	return (this == _renderGL);
}

CC3OpenGL* CC3OpenGL::sharedGL()
{
	// The unconventional separation of alloc & init here is required so the static var is set
	// before the init is run, since several init operations require access to the static var.
	//if ( isRenderThread() )
	//{
		if (!_renderGL) 
		{
			_renderGL = new CC3OpenGLClass;
			_renderGL->initWithName( "Rendering Engine" );
		}

		return _renderGL;
	//} else {
	//	if (!_bgGL) {
	//		_bgGL = [self alloc];			// retained
	//		[_bgGL initWithName: @"Background Engine"];
	//	}
	//	return _bgGL;
	//}
	return NULL;
}

static CCThread* _renderThread = NULL;

CCThread* CC3OpenGL::renderThread()
{
#pragma _NOTE_TODO( "CC3OpenGL::renderThread()" )
	// Retrieve from CCDirector, and cache for fast access, and to allow CCDirector to be shut
	// down, but the render thread to still be accessible for any outstanding background loading
	// that occurs before GL is shut down.
	//if (!_renderThread) 
	//	_renderThread = CCDirector::sharedDirector()->getRunningThread();	// weak reference
	
	return _renderThread;
}

bool CC3OpenGL::isRenderThread()
{
	if (!_renderThread) 
		renderThread();

	//return (NSThread.currentThread == _renderThread) || NSThread.isMainThread;
	return true;
}

void CC3OpenGL::terminateOpenGL()
{
	CC3Texture::setShouldCacheAssociatedCCTextures( false );
	CCDirector::sharedDirector()->end();
	_renderGL->terminate();
	_bgGL->terminate();
}

void CC3OpenGL::terminate()
{
	/*if (isRenderingContext()) 
	{
		[self.class.renderThread runBlockAsync: ^{ [self clearOpenGLResourceCaches]; } ];
		[self.class.renderThread runBlockAsync: ^{ [self terminateSoon]; } ];
	} else {
		[CC3Backgrounder.sharedBackgrounder runBlock: ^{ [self clearOpenGLResourceCaches]; }];
		[CC3Backgrounder.sharedBackgrounder runBlock: ^{ [self terminateSoon]; }];
	}*/
}

void CC3OpenGL::terminateSoon()
{
	/*LogInfo(@"Requesting deletion of %@ on thread %@.", self, NSThread.currentThread);
	if (self.isRenderingContext) {
		[self.class.renderThread runBlock: ^{ [self terminateNow]; } after: _deletionDelay ];
	} else {
		[CC3Backgrounder.sharedBackgrounder runBlock: ^{ [self terminateNow]; } after: _deletionDelay];
	}*/
}

void CC3OpenGL::terminateNow()
{
	/*LogInfo(@"Deleting %@ now on thread %@.", self, NSThread.currentThread);
	[self finish];
	
	if (self == _renderGL) {
		[_renderGL release];
		_renderGL = nil;
	}
	if (self == _bgGL) {
		[_bgGL release];
		_bgGL = nil;
	}
	
	[self.class checkClearRenderThread];
	[self.class checkTerminationNotify];*/
}

/** If BOTH the render context AND the background context have been deleted, release the render thread. */
void CC3OpenGL::checkClearRenderThread()
{
	if (!_renderGL && !_bgGL) 
		_renderThread = NULL;		// weak reference
}

/** If BOTH the render context AND the background context have been deleted, notify the delegate. */
void CC3OpenGL::checkTerminationNotify()
{
	//if (!_renderGL && !_bgGL)
		//[self notifyDelegateOf: @selector(didTerminateOpenGL) withObject: nil];
}

/** 
 * Notifies the delegate by invoking the specified method, with the specified optional argument.
 * If the method does not take an argument, the arg value should be nil. If the delegate does
 * not support the method, then the notification is not sent.
 *
 * The notification is queued to the main thread for execution, and is processed asynchronously.
 */
#pragma _NOTE_TODO( "CC3OpenGL::notifyDelegateOf( SEL selector, void* arg )" )
//void CC3OpenGL::notifyDelegateOf( SEL selector, void* arg )
//{
//	/*if ([_delegate respondsToSelector: selector])
//		[_delegate performSelectorOnMainThread: selector withObject: arg waitUntilDone: NO];*/
//}

bool CC3CheckGLBooleanAt(GLuint idx, bool val, GLbitfield* stateBits, GLbitfield* isKnownBits)
{
	bool needsUpdate = (!CC3BooleansAreEqual(CC3IsBitSet(*stateBits, idx), val)) || CC3IsBitClear(*isKnownBits, idx);
	if (needsUpdate) 
	{
		CC3SetBit(stateBits, idx, val);
		CC3SetBit(isKnownBits, idx, true);
	}
	return needsUpdate;
}

bool CC3CheckGLuintAt(GLuint idx, GLuint val, GLuint* stateArray, GLbitfield* isKnownBits) {
	bool needsUpdate = (stateArray[idx] != val) || CC3IsBitClear(*isKnownBits, idx);
	if (needsUpdate) {
		stateArray[idx] = val;
		CC3SetBit(isKnownBits, idx, true);
	}
	return needsUpdate;
}

bool CC3CheckGLfloatAt(GLuint idx, GLfloat val, GLfloat* stateArray, GLbitfield* isKnownBits) {
	bool needsUpdate = (stateArray[idx] != val) || CC3IsBitClear(*isKnownBits, idx);
	if (needsUpdate) {
		stateArray[idx] = val;
		CC3SetBit(isKnownBits, idx, true);
	}
	return needsUpdate;
}

bool CC3CheckGLVectorAt(GLuint idx, CC3Vector val, CC3Vector* stateArray, GLbitfield* isKnownBits)
{
	bool needsUpdate = !stateArray[idx].equals( val ) || CC3IsBitClear(*isKnownBits, idx);
	if (needsUpdate)
	{
		stateArray[idx] = val;
		CC3SetBit(isKnownBits, idx, true);
	}
	return needsUpdate;
}

bool CC3CheckGLVector4At(GLuint idx, CC3Vector4 val, CC3Vector4* stateArray, GLbitfield* isKnownBits) 
{
	bool needsUpdate = !stateArray[idx].equals(val) || CC3IsBitClear(*isKnownBits, idx);
	if (needsUpdate) 
	{
		stateArray[idx] = val;
		CC3SetBit(isKnownBits, idx, true);
	}
	return needsUpdate;
}

bool CC3CheckGLColorAt(GLuint idx, ccColor4F val, ccColor4F* stateArray, GLbitfield* isKnownBits) {
	bool needsUpdate = !CCC4FAreEqual(stateArray[idx], val) || CC3IsBitClear(*isKnownBits, idx);
	if (needsUpdate) {
		stateArray[idx] = val;
		CC3SetBit(isKnownBits, idx, true);
	}
	return needsUpdate;
}

void CC3SetGLCapAt(GLenum cap, GLuint idx, bool onOff, GLbitfield* stateBits, GLbitfield* isKnownBits) 
{
	if (CC3CheckGLBooleanAt(idx, onOff, stateBits, isKnownBits))
	{
		if (onOff)
			glEnable(cap + idx);
		else
			glDisable(cap + idx);
		//LogGLErrorTrace(@"gl%@able(%@)", (onOff ? @"En" : @"Dis"), NSStringFromGLEnum(cap + idx));

		CHECK_GL_ERROR_DEBUG();
	}
}

NS_COCOS3D_END
