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
#ifndef _CCL_CC3OPENGL_H_
#define _CCL_CC3OPENGL_H_

NS_COCOS3D_BEGIN

class CC3NodeDrawingVisitor;
class CC3Mesh;
class CC3Fog;
class CC3GLSLVariable;
class CC3GLSLUniform;
class CC3GLSLAttribute;
class CC3ShaderProgram;
class CC3ShaderPrewarmer;
class CC3OpenGLDelegate;
class CC3GLContext;
class CC3ShaderSemanticsDelegate;

/** Indicates that vertex attribute array is not available. */
#define kCC3VertexAttributeIndexUnavailable		-1

/** GL state tracking for vertex attributes. */
typedef struct {
	GLenum semantic;			/**< The Cocos3D semantic of this content array, under OGLES 1.1. */
	GLenum glName;				/**< The GL content name, used to enable a vertex array under OGLES 1.1. */
	GLenum elementType;			/**< The data type of each element. */
	GLint elementSize;			/**< The number of elements in each vertex. */
	GLsizei vertexStride;		/**< The stride in bytes between vertices. */
	GLvoid* vertices;			/**< A pointer to the vertex content. */
	bool shouldNormalize : 1;	/**< Indicates whether the vertex content should be normalized by the GL engine. */
	bool isKnown : 1;			/**< Indicates whether the GL state value are known. */
	bool isEnabled : 1;			/**< Indicates whether these attributes are enabled in the GL engine. */
	bool isEnabledKnown : 1;	/**< Indicates whether it is known if these attributes are enabled in the GL engine. */
	bool wasBound : 1;			/**< Indicates whether the attributes have been bound to the GL engine. */
} CC3VertexAttr;

/**
 * CC3OpenGL manages the OpenGL or OpenGL ES state for a single GL context.
 *
 * CC3OpenGL is implemented as a class cluster. The abstract CC3OpenGL class supports
 * a superset of functionality for OpenGL, OpenGL ES 1.1, or OpenGL ES 2.0. Concrete subclass
 * implementations provide functionality tailored to each specific GL implementation.
 *
 * OpenGL is designed to be a state machine that operates asynchronously from the application
 * code that calls its functions. Calls to most gl* functions queue up commands to the GL engine
 * that are processed by the GL engine asynchronously from the gl* call.
 *
 * This design allows GL command execution to be run on a different processor than the application
 * is running on, specifically a hardware-assisted GPU.
 *
 * To maximize the throughput and performance of this design, it is important that GL state is
 * changed only when necessary, and that querying of the GL state machine is avoided wherever possible.
 * 
 * By routing all GL requests through CC3OpenGL, this class can keep track of the GL state
 * change requests made to the GL engine, and will only forward such requests to the GL engine
 * if the state really is changing.
 */
class CC3OpenGL : public CC3Identifiable 
{
	DECLARE_SUPER( CC3Identifiable );
public:
	CC3OpenGL();
	virtual ~CC3OpenGL();

public:
	
	/** 
	 * The OpenGL engine context.
	 *
	 * The value of this property is automatically retrieved from the CCGLView.
	 */
	CC3GLContext*				getContext();
	void						setContext( CC3GLContext* context );

	/**
	 * Returns whether this instance is tracking state for the primary rendering GL context
	 * on the rendering thread.
	 */
	bool						isRenderingContext();

	/**
	 * Returns the CC3OpenGLDelegate delegate that will receive callback notifications for
	 * asynchronous OpenGL activities.
	 */
	static CC3OpenGLDelegate*	getDelegate();

	/**
	 * Sets the CC3OpenGLDelegate delegate that will receive callback notifications for
	 * asynchronous OpenGL activities.
	 */
	static void					setDelegate( CC3OpenGLDelegate* _delegate );

	/** Enable/disable alpha testing. */
	virtual void				enableAlphaTesting( bool onOff );

	/** Enable/disable blending. */
	virtual void				enableBlend( bool onOff );

	/**
	 * Enable/disable the user clipping plane at the specified index, which must be a value
	 * between zero and the maximum number of clipping planes supported by the platform.
	 */
	virtual void				enableClipPlane( bool onOff, GLuint clipIdx );

	/** Enable/disable the current color logic operation. */
	virtual void				enableColorLogicOp( bool onOff );

	/** Enable/disable the ambient & diffuse material colors to track vertex color. */
	virtual void				enableColorMaterial( bool onOff );

	/** Enable/disable polygon face culling. */
	virtual void				enableCullFace( bool onOff );

	/** Enable/disable depth testing. */
	virtual void				enableDepthTest( bool onOff );

	/** Enable/disable dithering. */
	virtual void				enableDither( bool onOff );

	/** Enable/disable fogging. */
	virtual void				enableFog( bool onOff );

	/** Enable/disable line smoothing. */
	virtual void				enableLineSmoothing( bool onOff );

	/** Enable/disable bone skinning using matrix palettes. */
	virtual void				enableMatrixPalette( bool onOff );

	/** Enable/disable sampling multiple fragments per pixel. */
	virtual void				enableMultisampling( bool onOff );

	/** Enable/disable the re-normalizing of normals when they are transformed. */
	virtual void				enableNormalize( bool onOff );

	/** Enable/disable point smoothing. */
	virtual void				enablePointSmoothing( bool onOff );

	/** Enable/disable displaying points as textured point sprites. */
	virtual void				enablePointSprites( bool onOff );

	/** Enable/disable displaying points as textured point sprites. */
	virtual void				enableShaderPointSize( bool onOff );

	/** Enable/disable offsetting fragment depth when comparing depths. */
	virtual void				enablePolygonOffset( bool onOff );

	/** Enable/disable the re-scaling of normals when they are transformed. */
	virtual void				enableRescaleNormal( bool onOff );

	/** Enable/disable alpha coverage in multisampling. */
	virtual void				enableSampleAlphaToCoverage( bool onOff );

	/** Enable/disable setting alpha to one when multisampling. */
	virtual void				enableSampleAlphaToOne( bool onOff );

	/** Enable/disable sample coverage. */
	virtual void				enableSampleCoverage( bool onOff );

	/** Enable/disable discarding pixels that are outside to a scissor rectangle. */
	virtual void				enableScissorTest( bool onOff );

	/** Enable/disable discarding pixels that are not part of a defined stencil. */
	virtual void				enableStencilTest( bool onOff );

	/** Binds the vertex attributes in the specified mesh to the GL engine. */
	virtual void				bindMesh( CC3Mesh* mesh, CC3NodeDrawingVisitor* visitor );

	/**
	 * Retrieves the vertex array that should be bound to the specified attribute from the mesh
	 * of the current node and binds the content of the vertex array to the GLSL attribute. Does
	 * nothing if the mesh does not contain vertex content for the specified attribute.
	 */
	virtual void				bindVertexAttribute( CC3GLSLAttribute* attribute, CC3NodeDrawingVisitor* visitor );

	/**
	 * Enable/disable the vertex attributes at the specified index, which must be a value
	 * between zero and the maximum number of vertex attributes supported by the platform.
	 *
	 * It is safe to submit a negative index. It will be ignored, and no changes will be made.
	 */
	virtual void				enableVertexAttribute( bool onOff, GLint vaIdx );

	virtual void				setVertexAttributeEnablementAt( GLint vaIdx );

	/**
	 * Binds the content pointer, size, type, stride, and normalization requirements value together
	 * for the vertex attribute at the specified index, which should be below the maximum number of
	 * vertex attributes supported by the platform.
	 *
	 * The values will be set in the GL engine only if at least one of the values has actually changed.
	 *
	 * It is safe to submit a negative index. It will be ignored, and no changes will be made.
	 */
	virtual void				bindVertexContent( GLvoid* pData, GLint elemSize, GLenum elemType, GLsizei vtxStride, bool shldNorm, GLint vaIdx );

	virtual void				bindVertexContentToAttributeAt( GLint vaIdx );

	/** Clears the tracking of unbound vertex attribute arrays. */
	virtual void				clearUnboundVertexAttributes();

	/** Enables the vertex attributes that have been bound and disables the rest. */
	virtual void				enableBoundVertexAttributes();

	/** Enables the vertex attribute needed for drawing Cocos2D 2D artifacts, and disables all the rest. */
	virtual void				enable2DVertexAttributes();

	/**
	 * Generates and returns a GL buffer ID.
	 *
	 * This is a wrapper for the GL function glGenBuffers.
	 */
	virtual GLuint				generateBuffer();

	/**
	 * Deletes the GL buffer with the specifid buffer ID.
	 *
	 * This is a wrapper for the GL function glDeleteBuffers.
	 */
	virtual void				deleteBuffer( GLuint buffID );

	/** Binds the buffer with the specified ID to the specified buffer target. */
	virtual void				bindBuffer( GLuint buffId, GLenum target );

	/** 
	 * Unbinds all buffers from the specified buffer target. 
	 *
	 * This is equivalent to invoking the bindBuffer:toTarget: method with a zero buffID parameter.
	 */
	virtual void				unbindBufferTarget( GLenum target );

	/**
	 * Loads data into the GL buffer currently bound to the specified target, starting at the
	 * specified buffer pointer, and extending for the specified length. The buffer usage is a
	 * hint for the GL engine, and must be a valid GL buffer usage enumeration value.
	 */
	virtual void				loadBufferTarget( GLenum target, GLvoid* buffPtr, GLsizeiptr buffLen, GLenum buffUsage );

	/**
	 * Updates data in the GL buffer currently bound to the specified target, from data starting
	 * at the specified offset to the specified pointer, and extending for the specified length.
	 */
	virtual void				updateBufferTarget( GLenum target, GLvoid* buffPtr, GLintptr offset, GLsizeiptr length );

	/** Binds the VAO with the specified ID. */
	virtual void				bindVertexArrayObject( GLuint vaoId );
	 
	/**
	 * Draws vertices bound by the vertex pointers using the specified draw mode,
	 * starting at the specified index, and drawing the specified number of verticies.
	 *
	 * This is a wrapper for the GL function glDrawArrays.
	 */
	virtual void				drawVerticiesAs( GLenum drawMode, GLuint start, GLuint len );

	/**
	 * Draws the vertices indexed by the specified indices, to the specified number of indices,
	 * each of the specified GL type, and using the specified draw mode.
	 *
	 * This is a wrapper for the GL function glDrawElements.
	 */
	virtual void				drawIndicies( GLvoid* indicies, GLuint len, GLenum type, GLenum drawMode );

	/** Sets the color used to clear the color buffer. */
	virtual void				setClearColor( const ccColor4F& color );

	/** Sets the value used to clear the depth buffer. */
	virtual void				setClearDepth( GLfloat val );

	/** Sets the value used to clear the stencil buffer. */
	virtual void				setClearStencil( GLint val );

	/** Sets the color used to paint in the absence of materials and textures. */
	virtual void				setColor( const ccColor4F& color );

	/** Sets the color mask indicating which of R, G, B & A should be written to the color buffer. */
	virtual void				setColorMask( const ccColor4B& mask );

	/** Sets the faces to cull. */
	virtual void				setCullFace( GLenum val );

	/** Sets the depth function to use when comparing depths. */
	virtual void				setDepthFunc( GLenum val );

	/** Sets whether the depth buffer is enabled for writing. */
	virtual void				setDepthMask( bool writable );

	/** Sets which face winding is considered to be the front face. */
	virtual void				setFrontFace( GLenum val );

	/** Sets the width used to draw lines. */
	virtual void				setLineWidth( GLfloat val );

	/** Sets the size used to draw points. */
	virtual void				setPointSize( GLfloat val );

	/** Sets the point size attenuation coefficients. */
	virtual void				setPointSizeAttenuation( const CC3AttenuationCoefficients& ac );

	/** Sets the point size below which points will be faded away. */
	virtual void				setPointSizeFadeThreshold( GLfloat val );

	/** Sets the minimum size at which points will be drawn. */
	virtual void				setPointSizeMinimum( GLfloat val );

	/** Sets the maximum size at which points will be drawn. */
	virtual void				setPointSizeMaximum( GLfloat val );

	/** Sets the polygon offset factor and units. */
	virtual void				setPolygonOffsetFactor( GLfloat factor, GLfloat units );

	/** Sets the scissor clipping rectangle. */
	virtual void				setScissor( const CC3Viewport& vp );

	/** Sets the shading model. */
	virtual void				setShadeModel( GLenum val );

	/** Sets the stencil function parameters. */
	virtual void				setStencilFunc( GLenum func, GLint ref, GLuint mask );

	/** Sets mask for enabling writing of individual bits in the stencil buffer. */
	virtual void				setStencilMask( GLuint mask );

	/** Sets the operations when the stencil fails, the depth test fails, or the depth test passes. */
	virtual void				setOpOnStencilFail( GLenum sFail, GLenum dFail, GLenum dPass );

	/** Sets the viewport rectangle. */
	virtual void				setViewport( const CC3Viewport& vp );

	/** Enable/disable lighting. */
	virtual void				enableLighting( bool onOff );

	/** Enable/disable two-sided lighting. */
	virtual void				enableTwoSidedLighting( bool onOff );

	/** Sets the color of the ambient scene lighting. */
	virtual void				setSceneAmbientLightColor( const ccColor4F& color );

	/**
	 * Enable/disable the light at the specified index, which must be a value
	 * between zero and the maximum number of lights supported by the platform.
	 */
	virtual void				enableLight( bool onOff, GLuint ltIdx );

	/**
	 * Sets the ambient color of the light at the specified index, which must be a
	 * value between zero and the maximum number of lights supported by the platform.
	 */
	virtual void				setLightAmbientColor( const ccColor4F& color, GLuint ltIdx );

	/**
	 * Sets the diffuse color of the light at the specified index, which must be a
	 * value between zero and the maximum number of lights supported by the platform.
	 */
	virtual void				setLightDiffuseColor( const ccColor4F& color, GLuint ltIdx );

	/**
	 * Sets the specular color of the light at the specified index, which must be a
	 * value between zero and the maximum number of lights supported by the platform.
	 */
	virtual void				setLightSpecularColor( const ccColor4F& color, GLuint ltIdx );

	/**
	 * Sets the homogeneous position of the light at the specified index, which must be a
	 * value between zero and the maximum number of lights supported by the platform.
	 */
	virtual void				setLightPosition( const CC3Vector4& pos, GLuint ltIdx );

	/**
	 * Sets the distance attenuation coefficients of the light at the specified index, which 
	 * must be a value between zero and the maximum number of lights supported by the platform.
	 */
	virtual void				setLightAttenuation( const CC3AttenuationCoefficients& ac, GLuint ltIdx );

	/**
	 * Sets the direction of the spot light at the specified index, which must be a
	 * value between zero and the maximum number of lights supported by the platform.
	 */
	virtual void				setSpotlightDirection( const CC3Vector& dir, GLuint ltIdx );

	/**
	 * Sets the angular fading exponent of the spot light at the specified index, which must
	 * be a value between zero and the maximum number of lights supported by the platform.
	 */
	virtual void				setSpotlightFadeExponent( GLfloat val, GLuint ltIdx );

	/**
	 * Sets the cutoff angle of the spot light at the specified index, which must be a
	 * value between zero and the maximum number of lights supported by the platform.
	 */
	virtual void				setSpotlightCutoffAngle( GLfloat val, GLuint ltIdx );

	/** Binds the specified fog configuration to the GL engine. */
	virtual void				bindFog( CC3Fog* fog, CC3NodeDrawingVisitor* visitor );

	/** Sets the color of the fog. */
	virtual void				setFogColor( const ccColor4F& color );

	/** Sets the type of the fog. */
	virtual void				setFogMode( GLenum mode );

	/** Sets the density of the fog. */
	virtual void				setFogDensity( GLfloat val );

	/** Sets the start distance of the fog. */
	virtual void				setFogStart( GLfloat val );

	/** Sets the end distance of the fog. */
	virtual void				setFogEnd( GLfloat val );

	/** Sets the ambient color of the material. */
	virtual void				setMaterialAmbientColor( const ccColor4F& color );

	/** Sets the diffuse color of the material. */
	virtual void				setMaterialDiffuseColor( const ccColor4F& color );

	/** Sets the specular color of the material. */
	virtual void				setMaterialSpecularColor( const ccColor4F& color );

	/** Sets the emission color of the material. */
	virtual void				setMaterialEmissionColor( const ccColor4F& color );

	/** Sets the shininess of the material. */
	virtual void				setMaterialShininess( GLfloat val );

	/** Sets the alpha function and reference value. */
	virtual void				setAlphaFunc( GLenum func, GLfloat ref );

	/** Sets the blend function, forcing RGB and alpha blending to use the same blending function. */
	virtual void				setBlendFunc( GLenum src, GLenum dst );

	/** Sets the blend function, allowing RGB and alpha blending to be set separately. */
	virtual void				setBlendFunc( GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha );

	/** Generates a new texture and returns its ID. */
	virtual GLuint				generateTexture();

	/** Deletes the texture with the specified ID from the GL engine. */
	virtual void				deleteTexture( GLuint texID );

	/** 
	 * Clears the tracking of the specified texture.
	 *
	 * For each texture unit whose state tracking indicates that it is bound to the specified
	 * texture, sets the tracking state for that texture unit to the default texture ID (0),
	 * to ensure that the state tracking no longer expects to be bound to that texture.
	 *
	 * This method is invoked automatically whenever a GL texture is deleted, or whenever a 
	 * GL texture is removed from Cocos3D, but may still be in use by Cocos2D.
	 */
	virtual void				clearTextureBinding( GLuint texID );

	/**
	 * Loads the specified texture image data, with the specified characteristics,
	 * into the specified target at the specified texture unit, in GL memory.
	 */
	virtual void				loadTexureImage( const GLvoid* imageData, GLenum target, GLint mipmapLevel, 
		const CC3IntSize& size, GLenum texelFormat, GLenum texelType, GLint byteAlignment, GLuint tuIdx );

	/**
	 * Loads the specified texture image data, with the specified characteristics, into the
	 * specified rectangular area within the texture at the specified target and texture unit,
	 * in GL memory. The image data replaces the texture data within the specified bounds.
	 */
	virtual void				loadTexureSubImage( const GLvoid* imageData, GLenum target, GLint mipmapLevel, 
		const CC3Viewport& rect, GLenum texelFormat, GLenum texelType, GLint byteAlignment, GLuint tuIdx );

	/** 
	 * Sets the specified texture unit as the active texture unit. Subsequent changes made to
	 * texture unit properties will affect only this texture unit. The specified texture unit must
	 * be a value between zero and the maximum number of texture units supported by the platform.
	 *
	 * This method is invoked automatically for any texture action method that identifies the
	 * texture unit on which the action should be made. Normally, this method does not need to
	 * be invoked directly.
	 */
	virtual void				activateTextureUnit( GLuint tuIdx );

	/**
	 * Sets the specified texture unit as the active texture unit for client actions. 
	 * Subsequent changes made to texture unit client properties will affect only this texture unit.
	 * The specified texture unit must be a value between zero and the maximum number of texture
	 * units supported by the platform.
	 *
	 * This method is invoked automatically for any texture action method that identifies the
	 * texture unit on which the action should be made. Normally, this method does not need to
	 * be invoked directly.
	 */
	virtual void				activateClientTextureUnit( GLuint tuIdx );

	/**
	 * Enable/disable texturing for the specified target in the specified texture unit index, which
	 * must be a value between zero and the maximum number of texture units supported by the platform.
	 */
	virtual void				enableTexturing( bool onOff, GLenum target, GLuint tuIdx );

	/**
	 * Disables texturing for all targets in the specified texture unit index, which must be a value
	 * between zero and the maximum number of texture units supported by the platform.
	 */
	virtual void				disableTexturingAt( GLuint tuIdx );

	/**
	 * Disables texturing for all targets in all texture units starting at, and above, the specified
	 * texture unit index, which must be a value between zero and the maximum number of texture units
	 * supported by the platform.
	 */
	virtual void				disableTexturingFrom( GLuint tuIdx );

	/**
	 * Binds the texture with the specified ID to the specified target at the specified texture
	 * unit index, which must be a value between zero and the maximum number of texture units
	 * supported by the platform.
	 */
	virtual void				bindTexture( GLuint texID, GLenum target, GLuint tuIdx );

	/** Unbind all targets in the specified texture unit except the specified target. */
	virtual void				unbindTexturesExceptTarget( GLenum target, GLuint tuIdx );

	/** Sets the specified texture parameter for the specified texture unit, without checking a cache. */
	virtual void				setTexParamEnum( GLenum pName, GLenum target, GLenum val, GLuint tuIdx );

	/** 
	 * Sets the texture minifying function in the specified target of the specified texture
	 * unit index, which must be a value between zero and the maximum number of texture units
	 * supported by the platform.
	 */
	virtual void				setTextureMinifyFunc( GLenum func, GLenum target, GLuint  tuIdx );

	/**
	 * Sets the texture magnifying function in the specified target of the specified texture
	 * unit index, which must be a value between zero and the maximum number of texture units 
	 * supported by the platform.
	 */
	virtual void				setTextureMagnifyFunc( GLenum func, GLenum target, GLuint  tuIdx );

	/**
	 * Sets the texture horizontal wrapping function in the specified target of the specified
	 * texture unit index, which must be a value between zero and the maximum number of texture
	 * units supported by the platform.
	 */
	virtual void				setTextureHorizWrapFunc( GLenum func, GLenum target, GLuint  tuIdx );

	/**
	 * Sets the texture vertical wrapping function in the specified target of the specified
	 * texture unit index, which must be a value between zero and the maximum number of texture
	 * units supported by the platform.
	 */
	virtual void				setTextureVertWrapFunc( GLenum func, GLenum target, GLuint  tuIdx );

	/** Generates a mipmap for the specified target for the texture bound to the specified texture unit. */
	virtual void				generateMipmapForTarget( GLenum target, GLuint tuIdx );

	/**
	 * Sets the texture environment mode of the specified texture unit index, which must
	 * be a value between zero and the maximum number of texture units supported by the platform.
	 */
	virtual void				setTextureEnvMode( GLenum mode, GLuint tuIdx );

	/**
	 * Sets the texture environment color of the specified texture unit index, which must
	 * be a value between zero and the maximum number of texture units supported by the platform.
	 */
	virtual void				setTextureEnvColor( const ccColor4F& color, GLuint tuIdx );

	/**
	 * Enable/disable point sprite texture coordinate replacement for the specified texture unit index,
	 * which must be a value between zero and the maximum number of texture units supported by the platform.
	 */
	virtual void				enablePointSpriteCoordReplace( bool onOff, GLuint tuIdx );

	/** Returns a string description of the current texture object bindings for each texture unit. */
	virtual std::string			dumpTextureBindings();

	virtual std::string			dumpTextureBindingsAt( GLuint tuIdx );

	/** Activates the specified matrix stack to make it the current matrix stack. */
	virtual void				activateMatrixStack( GLenum mode );

	/** Activates the specified palette matrix stack to make it the current matrix stack. */
	virtual void				activatePaletteMatrixStack( GLuint pmIdx );

	/** Activates the modelview matrix stack and replaces the current matrix with the specified matrix. */
	virtual void				loadModelviewMatrix( const CC3Matrix4x3* mtx );

	/** Activates the projection matrix stack and replaces the current matrix with the specified matrix. */
	virtual void				loadProjectionMatrix( const CC3Matrix4x4* mtx );

	/** Activates the specified palette matrix stack and replaces the current matrix with the specified matrix. */
	virtual void				loadPaletteMatrix( const CC3Matrix4x3* mtx, GLuint pmIdx );
	 
	/** Activates the modelview matrix stack, pushes it down one level, and copies the old top to the new top. */
	virtual void				pushModelviewMatrixStack();

	/** Activates the modelview matrix stack and pops off the current top level. */
	virtual void				popModelviewMatrixStack();

	/** Activates the projection matrix stack, pushes it down one level, and copies the old top to the new top. */
	virtual void				pushProjectionMatrixStack();

	/** Activates the projection matrix stack and pops off the current top level. */
	virtual void				popProjectionMatrixStack();

	/** Sets the fog hint. */
	virtual void				setFogHint( GLenum hint );

	/** Sets the mipmap generation hint. */
	virtual void				setGenerateMipmapHint( GLenum hint );

	/** Sets the line smooting hint. */
	virtual void				setLineSmoothingHint( GLenum hint );

	/** Sets the perspective correction hint. */
	virtual void				setPerspectiveCorrectionHint( GLenum hint );

	/** Sets the point smooting hint. */
	virtual void				setPointSmoothingHint( GLenum hint );

	/** Generates and returns a new framebuffer ID. */
	virtual GLuint				generateFramebuffer();

	/** Deletes the framebuffer with the specified ID from the GL engine. */
	virtual void				deleteFramebuffer( GLuint fbID );

	/** Returns whether the specified framebuffer ID is the currently bound value. */
	virtual bool				checkGLFramebuffer( GLuint fbID );

	/** Returns whether the specified framebuffer target is the currently bound value. */
	virtual bool				checkGLFramebufferTarget( GLenum fbTarg );

	virtual void				bindFramebuffer( GLuint fbID, GLenum fbTarget );

	/** Makes the framebuffer with the specified ID the current framebuffer in the GL engine. */
	virtual void				bindFramebuffer( GLuint fbID );

	/** 
	 * Resolves the content in the specified multisample framebuffer into the specified framebuffer,
	 * and leaves the multisample framebuffer bound to the GL_FRAMEBUFFER target for further rendering.
	 */
	virtual void				resolveMultisampleFramebuffer( GLuint fbSrcID, GLuint fbDstID );

	/**
	 * Discards the specified attachments from the specified framebuffer.
	 *
	 * The attachments parameter is an array of framebuffer attachments enums that is may include:
	 *  - GL_COLOR_ATTACHMENT0
	 *  - GL_DEPTH_ATTACHMENT
	 *  - GL_STENCIL_ATTACHMENT
	 *
	 * The count parameter indicates the length of this array.
	 */
	virtual void				discard( GLsizei count, const GLenum* attachments, GLuint fbID );

	/** Generates and returns a new renderbuffer ID. */
	virtual GLuint				generateRenderbuffer();

	/** Deletes the renderbuffer with the specified ID from the GL engine. */
	virtual void				deleteRenderbuffer( GLuint rbID );

	/** Makes the renderbuffer with the specified ID the current renderbuffer in the GL engine. */
	virtual void				bindRenderbuffer( GLuint rbID );

	/** 
	 * Allocates storage for the specified renderbuffer, sufficient to render an image of the
	 * specified size, in the specified pixel format, and with the specified number of samples
	 * per pixel, which will be a value larger than one if antialiasing multisampling is in use.
	 * If multi-sampling is not supported by the platform, the pixelSamples value is ignored.
	 */
	virtual void				allocateStorageForRenderbuffer( GLuint rbID, const CC3IntSize& size, GLenum format, GLuint pixelSamples );

	/** Returns the current value in the GL engine of the specified integer renderbuffer parameter. */
	virtual GLint				getRenderbufferParameterInteger( GLenum param );

	/** Binds the specified renderbuffer to the specified framebuffer as the specified attachement. */
	virtual void				bindRenderbuffer( GLuint rbID, GLuint fbID, GLenum attachment );

	/** 
	 * Binds the specified mipmap level of the specified face of the specified texture to the
	 * specified framebuffer as the specified attachement.
	 */
	virtual void				bindTexture2D( GLuint texID, GLenum face, GLint mipmapLevel, GLuint fbID, GLenum attachment );

	/**
	 * Checks the completeness status of the specified framebuffer, and returns YES if the framebuffer
	 * is complete and ready to be drawn to, or NO if the framebuffer is not ready to be drawn to.
	 *
	 * If the framebuffer is not complete, an error is logged, and, if the GL_ERROR_ASSERTION_ENABLED
	 * compiler build setting is set, an assertion error is raised.
	 */
	virtual bool				checkFramebufferStatus( GLuint fbID );

	/**
	 * Clears the buffers identified by the specified bitmask, which is a bitwise OR
	 * combination of one or more of the following masks: GL_COLOR_BUFFER_BIT,
	 * GL_DEPTH_BUFFER_BIT, and GL_STENCIL_BUFFER_BIT
	 */
	virtual void				clearBuffers( GLbitfield mask );

	/**
	 * Reads the color content of the range of pixels defined by the specified rectangle from the
	 * GL color buffer of the currently bound framebuffer, into the specified array, which must be
	 * large enough to accommodate the number of pixels covered by the specified rectangle.
	 *
	 * Content is written to memory left to right across each row, starting at the row at the bottom
	 * of the image, and ending at the row at the top of the image. The pixel content is packed
	 * tightly into the specified array, with no gaps left at the end of each row. In memory, the
	 * last pixel of one row is immediately followed by the first pixel of the next row.
	 *
	 * If the specified framebuffer is not the active framebuffer, it is temporarily activated,
	 * long enough to read the contents, then the current framebuffer is reactivated. This allows
	 * pixels to be read from a secondary framebuffer while rendering to the active framebuffer.
	 *
	 * This method should be used with care, since it involves making a synchronous call to
	 * query the state of the GL engine. This method will not return until the GL engine has
	 * executed all previous drawing commands in the pipeline. Excessive use of this method
	 * will reduce GL throughput and performance.
	 */
	virtual void				readPixelsIn( const CC3Viewport& rect, GLuint fbID, ccColor4B* colorArray );

	/**
	 * Sets the packing alignment when writing pixel content from the GL engine into application
	 * memory to the specified alignment, which may be 1, 2, 4 or 8.
	 *
	 * This value indicates whether each row of pixels should start at a 1, 2, 4 or 8 byte boundary.
	 * Depending on the width of the image, a value other than 1 may result in additional bytes being
	 * added at the end of each row of pixels, in order to maintain the specified byte alignment.
	 * The contents of those additional bytes is undefined.
	 */
	virtual void				setPixelPackingAlignment( GLint byteAlignment );

	/**
	 * Sets the unpacking alignment when reading pixel content from application memory for copying
	 * into the GL engine to the specified alignment, which may be 1, 2, 4 or 8.
	 *
	 * This value indicates whether each row of pixels should start at a 1, 2, 4 or 8 byte boundary.
	 * Depending on the width of the image, a value other than 1 may require that the application
	 * add additional bytes to the end of each row of pixels, in order to maintain the specified
	 * byte alignment. The contents of those additional bytes is not copied into the GL engine.
	 */
	virtual void				setPixelUnpackingAlignment( GLint byteAlignment );

	/** Flushes the GL buffer to the GL hardware. */
	virtual void				flush();

	/** Flushes the GL buffer to the GL hardware, and returns only when all GL commands have finished. */
	virtual void				finish();

	/** Returns the current value in the GL engine of the specified integer parameter. */
	virtual GLint				getInteger( GLenum param );

	/** Returns the current value in the GL engine of the specified float parameter. */
	virtual GLfloat				getFloat( GLenum param );

	/** Returns the current value in the GL engine of the specified string parameter. */
	virtual std::string			getString( GLenum param );

	/** Returns the maximum number of lights supported by the platform. */
	virtual GLuint				getMaxNumberOfLights();

	/** Returns the maximum number of clip planes supported by the platform. */
	virtual GLuint				getMaxNumberOfClipPlanes();

	/**
	 * Returns the maximum number of vertex skinning palette matrices supported by
	 * the platform, or zero if the platform does not impose a limit.
	 */
	virtual GLuint				getMaxNumberOfPaletteMatrices();

	/**
	 * Returns the maximum number of texture units supported by
	 * the platform, or zero if the platform does not impose a limit.
	 */
	virtual GLuint				getMaxNumberOfTextureUnits();

	/**
	 * Returns the maximum number of vertex attributes supported by
	 * the platform, or zero if the platform does not impose a limit.
	 */
	virtual GLuint				getMaxNumberOfVertexAttributes();

	/**
	 * Returns the maximum number of vertex skinning bone influences per vertex
	 * supported by the platform, or zero if the platform does not impose a limit.
	 */
	virtual GLuint				getMaxNumberOfBoneInfluencesPerVertex();

	virtual GLuint				getMaxNumberOfVertexUnits();

	/**
	 * Returns the maximum number of pixel samples supported by the platform,
	 * or zero if the platform does not impose a limit.
	 */
	virtual GLuint				getMaxNumberOfPixelSamples();

	/** Returns the maximum size for a renderbuffer supported by the platform. */
	virtual GLuint				getMaxRenderbufferSize();

	/** Returns the maximum size for a 2D texture supported by the platform. */
	virtual GLuint				getMaxTextureSize();

	/** Returns the maximum size for a cube-map texture supported by the platform. */
	virtual GLuint				getMaxCubeMapTextureSize();

	/** Returns the maximum size for a texture used for the specified target supported by the platform. */
	virtual GLuint				getMaxTextureSizeForTarget( GLenum target );

	/** Returns the maximum number of GLSL uniform vectors allowed in each vertex shader. */
	virtual GLuint				getMaxNumberOfVertexShaderUniformVectors();

	/** Returns the maximum number of GLSL uniform vectors allowed in each fragment shader. */
	virtual GLuint				getMaxNumberOfFragmentShaderUniformVectors();

	/** Returns the maximum number of GLSL varying vectors allowed in each shader program. */
	virtual GLuint				getMaxNumberOfShaderProgramVaryingVectors();

	/**
	 * Returns the minimum precision value of the shader variable of the specified type for a
	 * vertex shader, or returns zero if the platform does not support shader precision modifiers.
	 *
	 * For float variable types, this value is the (+/-) minimum resolvable value.
	 * For int variable types, this is the absolute minimum negative value.
	 * 
	 * The precisionType argument must be one of:
	 *  - GL_LOW_FLOAT
	 *  - GL_MEDIUM_FLOAT
	 *  - GL_HIGH_FLOAT
	 *  - GL_LOW_INT
	 *  - GL_MEDIUM_INT
	 *  - GL_HIGH_INT
	 */
	virtual GLfloat				getVertexShaderVarRangeMin( GLenum precisionType );

	/**
	 * Returns the maximum precision value of the shader variable of the specified type for a
	 * vertex shader, or returns zero if the platform does not support shader precision modifiers.
	 *
	 * For float variable types, this value is the (+/-) maximum value.
	 * For int variable types, this is the absolute maximum positive value.
	 *
	 * The precisionType argument must be one of:
	 *  - GL_LOW_FLOAT
	 *  - GL_MEDIUM_FLOAT
	 *  - GL_HIGH_FLOAT
	 *  - GL_LOW_INT
	 *  - GL_MEDIUM_INT
	 *  - GL_HIGH_INT
	 */
	virtual GLfloat				getVertexShaderVarRangeMax( GLenum precisionType );

	/**
	 * Returns the resolvable precision of the shader variable of the specified type within a
	 * vertex shader, or returns zero if the platform does not support shader precision modifiers.
	 *
	 * The precisionType argument must be one of:
	 *  - GL_LOW_FLOAT
	 *  - GL_MEDIUM_FLOAT
	 *  - GL_HIGH_FLOAT
	 *  - GL_LOW_INT
	 *  - GL_MEDIUM_INT
	 *  - GL_HIGH_INT
	 */
	virtual GLfloat				getVertexShaderVarPrecision( GLenum precisionType );

	/**
	 * Returns the minimum precision value of the shader variable of the specified type for a
	 * fragment shader, or returns zero if the platform does not support shader precision modifiers.
	 *
	 * For float variable types, this value is the (+/-) minimum resolvable value.
	 * For int variable types, this is the absolute minimum negative value.
	 *
	 * The precisionType argument must be one of:
	 *  - GL_LOW_FLOAT
	 *  - GL_MEDIUM_FLOAT
	 *  - GL_HIGH_FLOAT
	 *  - GL_LOW_INT
	 *  - GL_MEDIUM_INT
	 *  - GL_HIGH_INT
	 */
	virtual GLfloat				getFragmentShaderVarRangeMin( GLenum precisionType );

	/**
	 * Returns the maximum precision value of the shader variable of the specified type for a
	 * fragment shader, or returns zero if the platform does not support shader precision modifiers.
	 *
	 * For float variable types, this value is the (+/-) maximum value.
	 * For int variable types, this is the absolute maximum positive value.
	 *
	 * The precisionType argument must be one of:
	 *  - GL_LOW_FLOAT
	 *  - GL_MEDIUM_FLOAT
	 *  - GL_HIGH_FLOAT
	 *  - GL_LOW_INT
	 *  - GL_MEDIUM_INT
	 *  - GL_HIGH_INT
	 */
	virtual GLfloat				getFragmentShaderVarRangeMax( GLenum precisionType );

	/**
	 * Returns the resolvable precision of the shader variable of the specified type within a
	 * fragment shader, or returns zero if the platform does not support shader precision modifiers.
	 *
	 * The precisionType argument must be one of:
	 *  - GL_LOW_FLOAT
	 *  - GL_MEDIUM_FLOAT
	 *  - GL_HIGH_FLOAT
	 *  - GL_LOW_INT
	 *  - GL_MEDIUM_INT
	 *  - GL_HIGH_INT
	 */
	virtual GLfloat				getFragmentShaderVarPrecision( GLenum precisionType );

	/** Returns the specified extension name, stripped of any optional GL_ prefix. */
	virtual std::string			trimGLPrefix( const char* extensionName );

	/** Returns a collection of names of the GL extensions supported by the platform. */
	virtual CCSet*				getExtensions();

	/**
	 * Returns whether this platform supports the GL extension with the specified name, which
	 * should be the name of the GL extension, as registered with the OpenGL standards bodies,
	 * or as specified by the GPU driver manufacturer. 
	 *
	 * You may specify the name either with or without a "GL_" prefix 
	 * (eg. both @"OES_packed_depth_stencil" and @"GL_OES_packed_depth_stencil" will work if
	 * that extension is supported).
	 *
	 * This method checks the extensions collection for the presence of the specified name.
	 * Although this is an optimized hash test, you should generally not use this test in 
	 * time-critical code. If you need to frequently test for the presence of an extension
	 * (for example, within the render loop), you should invoke this method once at the 
	 * beginning of your app, and cache the resulting boolean value elsewhere in your code.
	 */
	virtual bool				supportsExtension( const char* extensionName );

	/** 
	 * Creates a new shader of the specifed type and returns its ID.
	 *
	 * The shaderType parameter must be one of the following values:
	 *   - GL_VERTEX_SHADER
	 *   - GL_FRAGMENT_SHADER
	 */
	virtual GLuint				createShader( GLenum shaderType );

	/** Deletes the shader with the specified ID from the GL engine. */
	virtual void				deleteShader( GLuint shaderID );

	/**
	 * Compiles the specified shader from the specified number of GLSL source code strings, 
	 * which is an array of null-terminated UTF8 strings. The number of source strings in 
	 * the source string array must be at least as large as the specified count.
	 *
	 * You can use the getShaderWasCompiled: method to determine whether compilation was successful,
	 * and the getLogForShader: method to retrieve the reason for any unsuccessful compilation.
	 */
	virtual void				compileShader( GLuint shaderID, GLuint srcStrCount, const GLchar** srcCodeStrings );

	/** Returns whether the specified shader was successfully compiled. */
	virtual bool				getShaderWasCompiled( GLuint shaderID );

	/** Returns the integer value of the specified GL engine parameter for the specified shader. */
	virtual GLint				getIntegerParameterForShader( GLenum param, GLuint shaderID );
	
	/** Returns the GL status info log for the specified shader. */
	virtual std::string			getLogForShader( GLuint shaderID );

	/** Returns the GLSL source code for the specified shader. */
	virtual std::string			getSourceCodeForShader( GLuint shaderID );

	/**
	 * Returns a string containing platform-specific GLSL source code to be used as a
	 * preamble for the vertex and fragment shader source code when compiling the shaders.
	 */
	virtual std::string			defaultShaderPreamble();

	/** Creates a new GLSL program and returns its ID. */
	virtual GLuint				createShaderProgram();

	/** Deletes the shader program with the specified ID from the GL engine. */
	virtual void				deleteShaderProgram( GLuint programID );

	/** Attaches the specified shader to the specified shader program. */
	virtual void				attachShader( GLuint shaderID, GLuint programID );

	/** Detaches the specified shader from the specified shader program. */
	virtual void				detachShader( GLuint shaderID, GLuint programID );

	/** 
	 * Links the specified shader program.
	 *
	 * You can use the getShaderProgramWasLinked: method to determine whether linking was successful,
	 * and the getLogForShaderProgram: method to retrieve the reason for any unsuccessful link attempt.
	 */
	virtual void				linkShaderProgram( GLuint programID );

	/**
	 * The shader prewarmer for this context.
	 *
	 * When loading, compiling and linking a shader program, some of the steps are deferred,
	 * within the GL engine, until the shader is first used to draw a mesh. This can result
	 * in a significant, unexpected, and undesired pause during the GL draw call.
	 *
	 * This prewarmer can be used to force that first draw call to be made immediately,
	 * and to an off-screen surface, so it won't be visible.
	 */
	virtual CC3ShaderPrewarmer*	getShaderProgramPrewarmer();
	virtual void				setShaderProgramPrewarmer( CC3ShaderPrewarmer* shaderPrewarmer );

	CC3ShaderSemanticsDelegate*	getSemanticDelegate();
	void						setSemanticDelegate( CC3ShaderSemanticsDelegate* semanticDelegate );

	/** Returns whether the specified shader was successfully linked. */
	virtual bool				getShaderProgramWasLinked( GLuint programID );

	/** Returns the integer value of the specified GL engine parameter for the specified shader program. */
	virtual GLint				getIntegerParameterForShaderProgram( GLenum param, GLuint programID );

	/** Binds the specified GLSL program as the program to be used for subsequent rendering. */
	virtual void				useShaderProgram( GLuint programID );

	/** Returns the GL status info log for the GL program. */
	virtual std::string			getLogForShaderProgram( GLuint programID );

	/** Populates the specified GLSL variable with info retrieved from the GL engine. */
	virtual void				populateShaderProgramVariable( CC3GLSLVariable* var );

	/** 
	 * Ensures that the shader program for the specified GLSL uniform is active, 
	 * then sets the value of the uniform into the GL engine.
	 */
	virtual void				setShaderProgramUniformValue( CC3GLSLUniform* uniform );

	/**
	 * Releases the shader compiler and its resources from the GL engine.
	 *
	 * It will be restored automatically on the next shader compilation request.
	 */
	virtual void				releaseShaderCompiler();

	/**
	 * Pushes the specified group marker into the GL command stream. This marker can be used
	 * by the debugger to organize the presentation of the commands in an OpenGL frame.
	 *
	 * This version must convert the specified marker string into a 'C' string in order to
	 * send it to the GL engine. For better performance, use the pushGroupMarkerC: version
	 * of this method, and consider using a static 'C' string, or caching the 'C' string to
	 * avoid creating it on each frame.
	 */
	virtual void				pushGroupMarker( const char* marker );

	/**
	 * Pushes the specified group marker into the GL command stream. This marker can be used
	 * by the debugger to organize the presentation of the commands in an OpenGL frame.
	 *
	 * For best performance, consider using a static string, or caching the string to avoid
	 * creating it on each frame.
	 */
	virtual void				pushGroupMarkerC( const char* marker );

	/**
	 * Pops the current group marker from the GL command stream. 
	 *
	 * This is the complement to the pushGroupMarker: or pushGroupMarkerC: methods, 
	 * and you can use this method in conjunction with either of those methods.
	 */
	virtual void				popGroupMarker();

	/**
	 * Inserts the specified marker into the GL command stream. This marker can be used
	 * by the debugger to organize the presentation of the commands in an OpenGL frame.
	 *
	 * This version must convert the specified marker string into a 'C' string in order to
	 * send it to the GL engine. For better performance, use the pushGroupMarkerC: version
	 * of this method, and consider using a static 'C' string, or caching the 'C' string to
	 * avoid creating it on each frame.
	 */
	virtual void				insertEventMarker( const char* marker );

	/**
	 * Inserts the specified marker into the GL command stream. This marker can be used
	 * by the debugger to organize the presentation of the commands in an OpenGL frame.
	 *
	 * For best performance, consider using a static string, or caching the string to avoid
	 * creating it on each frame.
	 */
	virtual void				insertEventMarkerC( const char* marker );

	/** Capture the current OpenGL command stream frame, starting at this point. */
	virtual void				captureOpenGLFrame();

	/** Sets the debug label for the specified GL object of the specified type. */
	virtual void				setDebugLabel( const char* label, GLuint objID, GLenum objType );

	/** Sets the debug label for the specified texture. */
	virtual void				setTextureDebugLabel( const char* label, GLuint texID );

	/** Sets the debug label for the specified buffer. */
	virtual void				setBufferDebugLabel( const char* label, GLuint buffID );

	/** Sets the debug label for the specified shader. */
	virtual void				setShaderDebugLabel( const char* label, GLuint shaderID );

	/** Sets the debug label for the specified shader program. */
	virtual void				setProgramDebugLabel( const char* label, GLuint progID );

	/** Sets the debug label for the specified framebuffer. */
	virtual void				setFrameBufferDebugLabel( const char* label, GLuint fbID );

	/** Sets the debug label for the specified renderbuffer. */
	virtual void				setRenderBufferDebugLabel( const char* label, GLuint rbID );

	/** Sets the debug label for the specified vertex array. */
	virtual void				setVertexArrayDebugLabel( const char* label, GLuint vaID );

	/**
	 * Aligns the state within the GL engine to be suitable for 2D drawing by Cocos2D.
	 *
	 * This is invoked automatically during the transition from 3D to 2D drawing. You can also  invoke
	 * this method if you perform 3D activities outside of the normal drawing loop, and you find that
	 * it interferes with subsequent 2D rendering by Cocos2D. However, such occurrances should be rare,
	 * and in most circumstances you should never need to invoke this method.
	 */
	virtual void				alignFor2DDrawingWithVisitor( CC3NodeDrawingVisitor* visitor );

	/**
	 * Aligns the state within the GL engine to be suitable for 3D drawing by Cocos3D.
	 *
	 * This is invoked automatically during the transition from 2D to 3D drawing.
	 */
	virtual void				alignFor3DDrawingWithVisitor( CC3NodeDrawingVisitor* visitor );

	virtual void				align2DStateCacheWithVisitor( CC3NodeDrawingVisitor* visitor );
	virtual void				align3DStateCacheWithVisitor( CC3NodeDrawingVisitor* visitor );
	virtual void				align3DVertexAttributeStateWithVisitor( CC3NodeDrawingVisitor* visitor );

	/**
	 * Clears content and resource caches that use OpenGL, including the CC3ShaderPrewarmer
	 * instance in the shaderProgramPrewarmer property, and the following OpenGL resource caches:
	 *
	 * 	 - CC3Resource
	 *   - CC3Texture
	 *   - CC3ShaderProgram
	 *   - CC3Shader
	 *   - CC3ShaderSourceCode
	 *   - CC3ShaderSourceCode
	 */
	virtual void				clearOpenGLResourceCaches();

	void						initWithTag( GLuint aTag, const std::string& aName );
	/** Template method to establish the initial value of the deletionDelay property. */
	virtual void				initDeletionDelay();
	/** Template method to establish the OpenGL engine context. */
	virtual void				initGLContext();
	/** Template method to create and return the primary rendering OpenGL context. */
	virtual CC3GLContext*		makeRenderingGLContext();

	/**
	 * Template method to create and return the background OpenGL context.
	 *
	 * This implementation creates the background GL context from the primary rendering context,
	 * using a sharegroup, so that the two can share GL objects.
	 */
	virtual CC3GLContext*		makeBackgroundGLContext();
	/** Template method to retrieve the GL platform limits. */
	virtual void				initPlatformLimits();

	/** Initializes surfaces frameworks. */
	virtual void				initSurfaces();

	/** Initializes vertex attributes. This must be invoked after the initPlatformLimits. */
	virtual void				initVertexAttributes();

	/** Initializes the texture units. This must be invoked after the initPlatformLimits. */
	virtual void				initTextureUnits();

	/** Performs any required initialization for GL extensions supported by this platform. */
	virtual void				initExtensions();

	/** 
	 * Returns the shared singleton instance for the currently running thread, creating it if necessary.
	 *
	 * Within OpenGL, the state of the GL engine is tracked per thread. To support this, although the
	 * interface is as a singleton, this implementation actually keeps track of a CC3OpenGL instance
	 * per thread, and will return the appropriate instance according to which thread the invocation
	 * of this method is occuring.
	 *
	 * Currently, a maximum of two instances are supported, one for the primary rendering thread, and
	 * one for a single background thread that can be used for loading resources, textures, and shaders.
	 */
	static CC3OpenGL*			sharedGL();

	/** Returns the thread that is being used for primary rendering. */
	static CCThread*			renderThread();

	/** Returns whether the current thread is being used for primary rendering. */
	static bool					isRenderThread();

	/** 
	 * Terminates the current use of OpenGL by this application.
	 *
	 * Terminates the CCDirector.sharedDirector singleton. Terminates OpenGL and deletes all GL contexts,
	 * serving all threads. Also clears all caches that contain content that uses OpenGL, including:
	 * 	 - CC3Resource
	 *   - CC3Texture
	 *   - CC3ShaderProgram
	 *   - CC3Shader
	 *   - CC3ShaderSourceCode
	 *
	 * You can invoke this method when your app no longer needs support for OpenGL, or will not
	 * use OpenGL for a significant amount of time, in order to free up app and OpenGL memory
	 * used by your application.
	 *
	 * To ensure that further OpenGL calls are not attempted, before invoking this method, you should
	 * release all CC3Scenes that you have created or loaded, along with any Cocos2D components.
	 *
	 * To ensure that that the current GL activity has finished before pulling the rug out from
	 * under it, this request is queued for each existing context, on the thread for which the
	 * context was created, and will only be executed once any currently running tasks on the 
	 * queue have been completed.
	 *
	 * In addition, once dequeued, a short delay is imposed, before the context instance is 
	 * actually released and deallocated, to provide time for object deallocation and cleanup 
	 * after the caches have been cleared, and autorelease pools have been drained. The length
	 * of this delay may be different for each context instance, and is specified by the 
	 * deletionDelay property of each instance.
	 *
	 * Since much of the processing of this method is handled through queued operations, as 
	 * described above, this method will return as soon as the requests are queued, and well
	 * before the operations have completed, and OpenGL has been terminated. 
	 *
	 * You can choose to be notified once all operations triggered by this method have completed,
	 * and OpenGL has been terminated, by registering a delegate object using the setDelegate: 
	 * class method. The delegate object will be sent the didTerminateOpenGL method once  all 
	 * operations triggered by this method have completed, and OpenGL has been terminated. 
	 * You should use this delegate notification if you intend to make use of OpenGL again, 
	 * as you must wait for one OpenGL session to terminate before starting another.
	 *
	 * Note that, in order to ensure that OpenGL is free to shutdown, this method forces the
	 * CC3Texture shouldCacheAssociatedCCTextures class-side property to NO, so that any
	 * background loading that is currently occurring will not cache Cocos2D textures.
	 * If you had set this property to YES, and intend to restart OpenGL at some point, then
	 * you might want to set it back to YES before reloading 3D resources again.
	 *
	 * Use this method with caution, as creating the GL contexts again will require significant overhead.
	 */
	static void					terminateOpenGL();

	/**
	 * Indicates the length of time, in seconds, that this instance will wait after the terminateOpenGL
	 * method is invoked, before this instance is actually deleted. This delay is intended to provide
	 * time for object deallocation and cleanup after the caches have been cleared, and autorelease
	 * pools have been drained.
	 *
	 * The value of this property is specified in seconds. The initial value of this is 0 for the
	 * instance that is used on the primary rendering thread, and 0.25 for the instance that is used
	 * for loading resources in the background.
	 */
	float						getDeletionDelay();
	void						setDeletionDelay( float delay );

	void						terminate();
	void						terminateSoon();
	void						terminateNow();

	/** If BOTH the render context AND the background context have been deleted, release the render thread. */
	void						checkClearRenderThread();

	/** If BOTH the render context AND the background context have been deleted, notify the delegate. */
	void						checkTerminationNotify();

protected:
	CC3GLContext*				_context;
	CCSet*						_extensions;
	float						_deletionDelay;

public:
	std::string					 value_GL_VENDOR;
	std::string					 value_GL_RENDERER;
	std::string					 value_GL_VERSION;
	
	CC3VertexAttr*				vertexAttributes;
	GLuint						value_MaxVertexAttribsUsed;
	
	GLuint*						value_GL_TEXTURE_BINDING_2D;
	GLbitfield					isKnown_GL_TEXTURE_BINDING_2D;		// Track up to 32 texture units
	
	GLuint*						value_GL_TEXTURE_BINDING_CUBE_MAP;
	GLbitfield					isKnown_GL_TEXTURE_BINDING_CUBE_MAP;	// Track up to 32 texture units
	
	GLbitfield					value_GL_COORD_REPLACE;				// Track up to 32 texture units
	GLbitfield					isKnownCap_GL_COORD_REPLACE;			// Track up to 32 texture units
	
	GLenum						value_GL_BLEND_SRC_RGB;
	GLenum						value_GL_BLEND_DST_RGB;
	GLenum						value_GL_BLEND_SRC_ALPHA;
	GLenum						value_GL_BLEND_DST_ALPHA;
	
	ccColor4F					value_GL_COLOR_CLEAR_VALUE;
	GLfloat						value_GL_DEPTH_CLEAR_VALUE;
	GLint						value_GL_STENCIL_CLEAR_VALUE;
	ccColor4B					value_GL_COLOR_WRITEMASK;
	GLenum						value_GL_CULL_FACE_MODE;
	GLenum						value_GL_DEPTH_FUNC;
	GLenum						value_GL_FRONT_FACE;
	GLfloat						value_GL_LINE_WIDTH;
	GLfloat						value_GL_POLYGON_OFFSET_FACTOR;
	GLfloat						value_GL_POLYGON_OFFSET_UNITS;
	CC3Viewport					value_GL_SCISSOR_BOX;
	GLenum						value_GL_STENCIL_FUNC;
	GLint						value_GL_STENCIL_REF;
	GLuint						value_GL_STENCIL_VALUE_MASK;
	GLuint						value_GL_STENCIL_WRITEMASK;
	GLenum						value_GL_STENCIL_FAIL;
	GLenum						value_GL_STENCIL_PASS_DEPTH_FAIL;
	GLenum						value_GL_STENCIL_PASS_DEPTH_PASS;
	CC3Viewport					value_GL_VIEWPORT;

	GLenum						value_GL_GENERATE_MIPMAP_HINT;

	GLint						value_GL_MAX_CLIP_PLANES;
	GLint						value_GL_MAX_LIGHTS;
	GLint						value_GL_MAX_PALETTE_MATRICES;
	GLint						value_GL_MAX_SAMPLES;
	GLint						value_GL_MAX_TEXTURE_UNITS;
	GLint						value_GL_MAX_VERTEX_ATTRIBS;
	GLint						valueMaxBoneInfluencesPerVertex;
	GLint						value_GL_MAX_TEXTURE_SIZE;
	GLint						value_GL_MAX_CUBE_MAP_TEXTURE_SIZE;
	GLint						value_GL_MAX_RENDERBUFFER_SIZE;

	GLuint						value_GL_ARRAY_BUFFER_BINDING;
	GLuint						value_GL_ELEMENT_ARRAY_BUFFER_BINDING;
	GLuint						value_GL_VERTEX_ARRAY_BINDING;

	GLuint						value_GL_ACTIVE_TEXTURE;
	GLuint						value_MaxTextureUnitsUsed;
	
	GLuint						value_GL_FRAMEBUFFER_BINDING;
	GLenum						value_GL_FRAMEBUFFER_Target;
	GLuint						value_GL_RENDERBUFFER_BINDING;
	GLuint						value_GL_PACK_ALIGNMENT;
	GLuint						value_GL_UNPACK_ALIGNMENT;

	GLfloat						value_GL_MAX_POINT_SIZE;

	bool						valueCap_GL_BLEND : 1;
	bool						valueCap_GL_CULL_FACE : 1;
	bool						valueCap_GL_DEPTH_TEST : 1;
	bool						valueCap_GL_DITHER : 1;
	bool						valueCap_GL_POLYGON_OFFSET_FILL : 1;
	bool						valueCap_GL_SAMPLE_ALPHA_TO_COVERAGE : 1;
	bool						valueCap_GL_SAMPLE_COVERAGE : 1;
	bool						valueCap_GL_SCISSOR_TEST : 1;
	bool						valueCap_GL_STENCIL_TEST : 1;
	bool						valueCap_GL_POINT_SPRITE : 1;
	bool						value_GL_DEPTH_WRITEMASK : 1;
	bool						isKnownBlendFunc : 1;
	bool						isKnownCap_GL_BLEND : 1;
	bool						isKnownCap_GL_CULL_FACE : 1;
	bool						isKnownCap_GL_DEPTH_TEST : 1;
	bool						isKnownCap_GL_DITHER : 1;
	bool						isKnownCap_GL_POLYGON_OFFSET_FILL : 1;
	bool						isKnownCap_GL_SAMPLE_ALPHA_TO_COVERAGE : 1;
	bool						isKnownCap_GL_SAMPLE_COVERAGE : 1;
	bool						isKnownCap_GL_SCISSOR_TEST : 1;
	bool						isKnownCap_GL_STENCIL_TEST : 1;
	bool						isKnownCap_GL_POINT_SPRITE : 1;
	bool						isKnown_GL_COLOR_CLEAR_VALUE : 1;
	bool						isKnown_GL_DEPTH_CLEAR_VALUE : 1;
	bool						isKnown_GL_STENCIL_CLEAR_VALUE : 1;
	bool						isKnown_GL_COLOR_WRITEMASK : 1;
	bool						isKnown_GL_CULL_FACE_MODE : 1;
	bool						isKnown_GL_DEPTH_FUNC : 1;
	bool						isKnown_GL_DEPTH_WRITEMASK : 1;
	bool						isKnown_GL_FRONT_FACE : 1;
	bool						isKnown_GL_LINE_WIDTH : 1;
	bool						isKnownPolygonOffset : 1;
	bool						isKnown_GL_SCISSOR_BOX : 1;
	bool						isKnownStencilFunc : 1;
	bool						isKnown_GL_STENCIL_WRITEMASK : 1;
	bool						isKnownStencilOp : 1;
	bool						isKnown_GL_VIEWPORT : 1;
	bool						isKnownMat_GL_AMBIENT : 1;
	bool						isKnownMat_GL_DIFFUSE : 1;
	bool						isKnown_GL_GENERATE_MIPMAP_HINT : 1;
	bool						isKnown_GL_ARRAY_BUFFER_BINDING : 1;
	bool						isKnown_GL_ELEMENT_ARRAY_BUFFER_BINDING : 1;
	bool						isKnown_GL_VERTEX_ARRAY_BINDING : 1;
	bool						isKnown_GL_ACTIVE_TEXTURE : 1;
	bool						isKnown_GL_FRAMEBUFFER_BINDING : 1;
	bool						isKnown_GL_FRAMEBUFFER_Target : 1;
	bool						isKnown_GL_RENDERBUFFER_BINDING : 1;
	bool						isKnown_GL_PACK_ALIGNMENT : 1;
	bool						isKnown_GL_UNPACK_ALIGNMENT : 1;
};

/**
 * This protocol specifies methods that will be invoked by certain asynchronous operations
 * performed by instances of CC3OpenGL.
 *
 * All callback notification methods are invoked on the main application thread.
 */
class CC3OpenGLDelegate
{
public:
	/** 
	 * This method is invoked once all of the operations triggered by invoking the CC3OpenGL
	 * terminateOpenGL class method have completed, and OpenGL has been terminated.
	 */
	virtual void				didTerminateOpenGL() { };

};

/**
 * Checks whether the specified boolean value changes the state of the GL engine for the state
 * tracked at the specified index in the specified stateBits. The isKnownBits bitfield keeps
 * track of whether or not the GL state is known at this time.
 *
 * If either the GL state is not known, or the specified value is different than the value currently
 * being tracked, this method updates the new value in the state cache, and marks the state value
 * as known, and returns YES, indicating that the state in the GL engine should be updated.
 * If the specified value is the same as the currently tracked state, this method returns NO.
 */
bool CC3CheckGLBooleanAt(GLuint idx, bool val, GLbitfield* stateBits, GLbitfield* isKnownBits);

/**
 * Checks whether the specified uint value changes the state of the GL engine for the state
 * tracked at the specified index in the specified stateArray. The isKnownBits bitfield keeps
 * track of whether or not the GL state is known at this time.
 *
 * If either the GL state is not known, or the specified value is different than the value currently
 * being tracked, this method updates the new value in the state cache, and marks the state value
 * as known, and returns YES, indicating that the state in the GL engine should be updated.
 * If the specified value is the same as the currently tracked state, this method returns NO.
 */
bool CC3CheckGLuintAt(GLuint idx, GLuint val, GLuint* stateArray, GLbitfield* isKnownBits);

/**
 * Checks whether the specified float value changes the state of the GL engine for the state
 * tracked at the specified index in the specified stateArray. The isKnownBits bitfield keeps
 * track of whether or not the GL state is known at this time.
 *
 * If either the GL state is not known, or the specified value is different than the value currently
 * being tracked, this method updates the new value in the state cache, and marks the state value
 * as known, and returns YES, indicating that the state in the GL engine should be updated.
 * If the specified value is the same as the currently tracked state, this method returns NO.
 */
bool CC3CheckGLfloatAt(GLuint idx, GLfloat val, GLfloat* stateArray, GLbitfield* isKnownBits);

/**
 * Checks whether the specified vector value changes the state of the GL engine for the state
 * tracked at the specified index in the specified stateArray. The isKnownBits bitfield keeps
 * track of whether or not the GL state is known at this time.
 *
 * If either the GL state is not known, or the specified value is different than the value currently
 * being tracked, this method updates the new value in the state cache, and marks the state value
 * as known, and returns YES, indicating that the state in the GL engine should be updated.
 * If the specified value is the same as the currently tracked state, this method returns NO.
 */
bool CC3CheckGLVectorAt(GLuint idx, CC3Vector val, CC3Vector* stateArray, GLbitfield* isKnownBits);

/**
 * Checks whether the specified 4D vector value changes the state of the GL engine for the state
 * tracked at the specified index in the specified stateArray. The isKnownBits bitfield keeps
 * track of whether or not the GL state is known at this time.
 *
 * If either the GL state is not known, or the specified value is different than the value currently
 * being tracked, this method updates the new value in the state cache, and marks the state value
 * as known, and returns YES, indicating that the state in the GL engine should be updated.
 * If the specified value is the same as the currently tracked state, this method returns NO.
 */
bool CC3CheckGLVector4At(GLuint idx, CC3Vector4 val, CC3Vector4* stateArray, GLbitfield* isKnownBits);

/**
 * Checks whether the specified color value changes the state of the GL engine for the state
 * tracked at the specified index in the specified stateArray. The isKnownBits bitfield keeps
 * track of whether or not the GL state is known at this time.
 *
 * If either the GL state is not known, or the specified value is different than the value currently
 * being tracked, this method updates the new value in the state cache, and marks the state value
 * as known, and returns YES, indicating that the state in the GL engine should be updated.
 * If the specified value is the same as the currently tracked state, this method returns NO.
 */
bool CC3CheckGLColorAt(GLuint idx, ccColor4F val, ccColor4F* stateArray, GLbitfield* isKnownBits);

/**
 * Checks whether the specified value changes the state of the GL engine for the capability
 * tracked at the specified index in the specified stateBits, and updates the GL engine with
 * the new value if it has changed. The isKnownBits bitfield keeps track of whether or not
 * the GL state is known at this time.
 */
void CC3SetGLCapAt(GLenum cap, GLuint idx, bool val, GLbitfield* stateBits, GLbitfield* isKnownBits);

/**
 * Macro for checking the state of a single state value and updating the cached value if needed.
 *
 * - val is the value to test.
 * - var is the instance variable used to cache the state value. May be updated.
 * - isKnown is the boolean instance variable that indicates whether the value is known. May be updated.
 * - equal contains a logical expression that determines whether val and var are equal.
 *
 * Defines and sets a local variable called  needsUpdate, to indicate whether the GL engine state
 * should be updated by the method or function that invoked this macro. This needsUpdate flag is
 * set to YES if the equal expression evaluates to NO, or the isKnown variable is set to NO.
 *
 * Both the var and isKnown instance variables are updated.
 *
 * This macro does not update the GL engine state. The calling function or method should do so
 * if the needsUpdate local variable is YES.
 */
#define cc3_CheckGLValue(val, equal, var, isKnown)		\
	bool needsUpdate = false;							\
	if ( !(equal) || !isKnown) {						\
		var = (val);									\
		isKnown = true;									\
		needsUpdate = true;								\
	}

/**
 * Macro for checking the state of a single state primitive variable and updating the cached
 * value if needed. Evaluates the cc3_CheckGLValue macro, passing a simple ((var) == (val))
 * test as the equal expression.
 */
#define cc3_CheckGLPrim(val, var, isKnown)  cc3_CheckGLValue((val), ((var) == (val)), var, isKnown)

/** Macro for checking the state of a single capability and setting it in GL engine if needed. */
#define cc3_SetGLCap(cap, val, var, isKnown)				\
	if ( !CC3BooleansAreEqual(val, var) || !isKnown) {		\
		isKnown = true;										\
		var = val;											\
		if (val) glEnable(cap);								\
		else glDisable(cap);								\
	}
		// MARK_TRACE_HERE									\
		//	CCLog("gl%sable(%s)", (onOff ? "En" : "Dis"), stringFromGLEnum(cap).c_str());	\
	//}

/** Macro for returning the state of a capability, retriving it from the GL engine if needed. */
#define cc3_GetGLCap(cap, var, isKnown)			\
	if (!isKnown) var = glIsEnabled(cap);		\
	return var;

/**
 * Macro for checking the state of a single state primitive value contained with an indexed
 * array of structs, and updating the cached value if needed. The sArray parameter is an array
 * of structs, and the VAR and IS_KNOWN parameters are the names of the struct elements holding
 * the value and isKnown indicator for the state, respectively. The idx parameter indexes into
 * a particualr struct in the array.
 *
 * Defines and sets a local variable called  needsUpdate, to indicate whether the GL engine state
 * should be updated by the method or function that invoked this macro. This needsUpdate flag is
 * set to YES if the equal expression evaluates to NO, or the isKnown variable is set to NO.
 *
 * Both the var and isKnown instance variables are updated.
 *
 * This macro does not update the GL engine state. The calling function or method should do so
 * if the needsUpdate local variable is YES.
 */
#define cc3_CheckGLStructValue(sArray, idx, val, VAR, IS_KNOWN)		\
	bool needsUpdate = false;										\
	if ( !((sArray[idx].VAR) == (val)) || !sArray[idx].IS_KNOWN) {	\
		sArray[idx].VAR = (val);									\
		sArray[idx].IS_KNOWN = true;								\
		needsUpdate = true;											\
	}

/**
 * If info logging is enabled AND this is the primary rendering context, logs the specified
 * info message, otherwise does nothing.
 */
#define LogInfoIfPrimary(fmt, ...)	//LogInfoIf(self.isRenderingContext, fmt, ##__VA_ARGS__)


NS_COCOS3D_END

#endif
