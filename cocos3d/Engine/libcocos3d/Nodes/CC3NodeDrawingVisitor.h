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
#ifndef _CCL_CC3NODE_DRAWING_VISITOR_H_
#define _CCL_CC3NODE_DRAWING_VISITOR_H_

NS_COCOS3D_BEGIN
class CC3Node;
class CC3Camera;
class CC3ShaderProgram;
class CC3SceneDrawingSurfaceManager;
class CC3NodeSequencer;
class CC3SkinSection;
class CC3RenderSurface;
class CC3OpenGL;

/** Enumeration of drawing visitor texture modes. */
typedef enum {
	kCC3TextureBindingModeModel,			/**< Binding model textures. */
	kCC3TextureBindingModeLightProbe,		/**< Binding light probe textures. */
} CC3TextureBindingMode;

/**
 * CC3NodeDrawingVisitor is a CC3NodeVisitor that is passed to a node when it is visited
 * during drawing operations.
 *
 * The visitor uses the camera property to determine which nodes to visit. Only nodes that
 * are within the camera's field of view will be visited. Nodes outside the camera's frustum
 * will neither be visited nor drawn.
 *
 * Drawing operations only visit drawable mesh nodes, so the node access properties defined on
 * the CC3NodeVisitor superclass that rely on the current node being a CC3MeshNode containing
 * a mesh and material will be valid.
 *
 * This visitor maintains access to a number of properties of the node being drawn, and
 * other components in the scene, for access by rendering logic and shaders.
 */
class CC3NodeDrawingVisitor : public CC3NodeVisitor 
{
	DECLARE_SUPER( CC3NodeVisitor );
public:
	CC3NodeDrawingVisitor();
	~CC3NodeDrawingVisitor();

	static CC3NodeDrawingVisitor* visitor();
	/** 
	 * Returns the OpenGL engine context.
	 *
	 * During drawing, all OpenGL commands are invoked through this instance.
	 *
	 * The value of this property is lazily retrieved and set the first time it is accessed.
	 *
	 * The value of this property must be compatible with the GL context of the thread on
	 * which this visitor is used. If you want to use a single visitor instance on different
	 * threads, you can invoke the clearGL method to clear the GL engine reference so it can
	 * be lazily recreated when used on the next thread.
	 */
	CC3OpenGL*					getGL();

	/**
	 * Clears the reference in the gl property, so that it can be retrieved automatically on
	 * the next access of the property. You can use this method before using this visitor on
	 * a thread that is different  (and therefore likely a different GL engine context) than
	 * the last thread on which this visitor was used.
	 */
	void						clearGL();

	/**
	 * The index of the current texture unit holding a 2D texture.
	 *
	 * This value is initialized to zero when starting to draw each material, and is incremented
	 * as each 2D texture in the material is drawn.
	 */
	GLuint						getCurrent2DTextureUnit();

	/** Increments the value fo the current2DTextureUnit property. */
	void						increment2DTextureUnit();

	/**
	 * The index of the current texture unit holding a cube-map texture.
	 *
	 * This value is initialized to zero when starting to draw each material, and is incremented
	 * as each cube-map texture in the material is drawn.
	 */
	GLuint						getCurrentCubeTextureUnit();

	/** Increments the value fo the currentCubeTextureUnit property. */
	void						incrementCubeTextureUnit();

	/** 
	 * Sets the value of the current2DTextureUnit property to zero, and sets the value of the 
	 * currentCubeTextureUnit property to either the value of the textureCubeStart property of
	 * the currentShaderProgram (OpenGL ES 2.0 & OpenGL), or to the same as the textureCount
	 * property of this instance (OpenGL ES 1.1).
	 *
	 * The 2D texture are assigned to the lower texture units, and cube-map textures are assigned
	 * to texture units above all the 2D textures. This ensures that the same texture types are
	 * consistently assigned to the shader samplers, to avoid the shaders recompiling on the
	 * fly to adapt to changing texture types.
	 *
	 * Additional environmental textures, such as light probes, are assigned to the texture units
	 * beyond the model's cube textures.
	 *
	 * GL texture units of each type that were not used by the textures are disabled via the
	 * disabledTextureUnits method.
	 */
	void						resetTextureUnits();
		
	/** Binds environmental textures, such as light probes. */
	void						bindEnvironmentalTextures();

	/** 
	 * Disables all texture units that do not have an associated texture. 
	 *
	 * The 2D texture are assigned to the lower texture units, and cube-map textures are assigned
	 * to texture units above all the 2D textures. This ensures that the same texture types are
	 * consistently assigned to the shader samplers, to avoid the shaders recompiling on the
	 * fly to adapt to changing texture types.
	 *
	 * GL texture units of each type that were not used by the textures are disabled by this method.
	 * Since cube-map textures are assigned to texture units above all 2D textures, for nodes with
	 * fewer 2D textures than expected by the shader, one or more 2D texture units may be disabled
	 * in between the active 2D texture units and any cube-map texture units.
	 */
	void						disableUnusedTextureUnits();

	/**
	 * This property gives the interval, in seconds, since the previous frame.
	 *
	 * See the description of the CC3Scene minUpdateInterval and maxUpdateInterval properties
	 * for more information about clamping the update interval.
	 */
	float						getDeltaTime();
	void						setDeltaTime( float dt );

	/**
	 * Indicates whether nodes should decorate themselves with their configured material, textures,
	 * or color arrays. In most cases, nodes should be drawn decorated. However, specialized visitors
	 * may turn off normal decoration drawing in order to do specialized coloring instead.
	 *
	 * The initial value of this property is YES.
	 */	
	bool						shouldDecorateNode();
	void						setShouldDecorateNode( bool shouldDecorate );

	/**
	 * Indicates whether this visitor is rendering an environment map to a texture.
	 *
	 * Environment maps typically do not require full detail. This property can be used during
	 * drawing to make optimization decisions such as to avoid drawing more complex content 
	 * when creating an environment map.
	 *
	 * The initial value of this property is NO.
	 */
	bool						isDrawingEnvironmentMap();
	void						setIsDrawingEnvironmentMap( bool draw );

	/**
	 * Aligns this visitor to use the same camera and rendering surface as the specified visitor.
	 *
	 * The camera, surfaceManager, and renderSurface properties of this visitor are set to
	 * those of the specified visitor.
	 *
	 * You can use this method to ensure that a secondary visitor (such as a shadow visitor, 
	 * or picking visitor), makes use of the same camera and surface as the primary visitor.
	 */
	void						alignShotWith( CC3NodeDrawingVisitor* otherVisitor );

	/**
	 * Draws the specified node. Invoked by the node itself when the node's local
	 * content is to be drawn.
	 *
	 * This implementation first caches the current lighting enablement state in case
	 * lighting is turned off during drawing of the material, then it double-dispatches
	 * back to the node's drawWithVisitor: method to perform the drawing. Finally, this
	 * implementation updates the drawing performance statistics.
	 *
	 * Subclass may override to enhance or modify this behaviour.
	 */
	virtual void				draw( CC3Node* aNode );

	/**
	 * The surface manager that manages the surfaces to which this visitor can render.
	 *
	 * Setting this property also clears the renderSurface property, so that, if not explicitly set
	 * to another surface, it will be initialized to a surface retrieved from the new surface manager.
	 *
	 * During normal rendering, this property is set by the CC3Layer prior to rendering a CC3Scene.
	 */
	CC3SceneDrawingSurfaceManager*	getSurfaceManager();
	void						setSurfaceManager( CC3SceneDrawingSurfaceManager* manager );

	/**
	 * The rendering surface to which this visitor is rendering.
	 *
	 * The surface will be activated at the beginning of each visitation run.
	 *
	 * You can set this property at any time to direct rendering to any on-screen or off-screen surface. 
	 * If not set directly, this property will be set to the value of the defaultRenderSurface property
	 * the next time it is accessed. If you have set this property to an specific surface temporarily,
	 * you can automatically revert to the defaultRenderSurface by simply setting this property to nil.
	 *
	 * This property is is not cleared at the end of the visitation run. It is retained so that
	 * this visitor can be used to render multiple node assemblies and complete multiple drawing
	 * passes without having to set the surface each time.
	 */
	CC3RenderSurface*			getRenderSurface();
	void						setRenderSurface( CC3RenderSurface* surface );

	/**
	 * Template property that returns the default value used to automatically
	 * set the value of the renderSurface property.
	 *
	 * This implementation returns the value of the viewSurface property of the 
	 * CC3SceneDrawingSurfaceManager held in the surfaceManager property. This
	 * is a subsection of the on-screen view surface.
	 *
	 * Subclasses may override to return a different surface.
	 */
	CC3RenderSurface*			getDefaultRenderSurface();

	/**
	 * During the drawing of nodes that use vertex skinning, this property holds the skin
	 * section that is currently being drawn.
	 *
	 * The value of this property is set by the skin section itself and is only valid
	 * during the drawing of that skin section.
	 */
	CC3SkinSection*				getCurrentSkinSection();

	/**
	 * The current color used during drawing if no materials or lighting are engaged.
	 *
	 * Each of the RGBA components of this color are floating point values between 0 and 1.
	 */
	ccColor4F					getCurrentColor();
	void						setCurrentColor( const ccColor4F& color );

	/**
	 * The current color used during drawing if no materials or lighting are engaged.
	 *
	 * Each of the RGBA components of this color are integer values between 0 and 255.
	 */
	ccColor4B					getCurrentColor4B();
	void						setCurrentColor4B( const ccColor4B& color );

	/** Transforms the specified global location to the coordinate system of the camera (eye space). */
	CC3Vector					transformGlobalLocationToEyeSpace( const CC3Vector& globalLocation );

	/** Transforms the specified global location to the coordinate system of the current node. */
	CC3Vector					transformGlobalLocationToModelSpace( const CC3Vector& globalLocation );

	/** Returns the current projection matrix. */
	const CC3Matrix4x4*			getProjMatrix();

	/** Returns the current view matrix. */
	const CC3Matrix4x3*			getViewMatrix();

	/** Returns the current model-to-global transform matrix. */
	const CC3Matrix4x3*			getModelMatrix();

	/** Returns the current view-projection matrix. */
	const CC3Matrix4x4*			getViewProjMatrix();

	/** Returns the current model-view matrix. */
	const CC3Matrix4x3*			getModelViewMatrix();

	/** Returns the current model-view-projection matrix. */
	const CC3Matrix4x4*			getModelViewProjMatrix();

	/** Returns the current CC3Layer GLKMatrix4 transform matrix. Available when using Cocos2D 3.1 and higher. */
	const CC3Matrix4x4*			getLayerTransformMatrix();

	/**
	 * Populates the current projection matrix from the specified matrix.
	 *
	 * This method is invoked automatically when the camera property is set.
	 */
	void						populateProjMatrixFrom( CC3Matrix* projMtx );

	/**
	 * Populates the current view matrix from the specified matrix.
	 *
	 * This method is invoked automatically when the camera property is set.
	 */
	void						populateViewMatrixFrom( CC3Matrix* viewMtx );

	/** Populates the current model-to-global matrix from the specified matrix. */
	void						populateModelMatrixFrom( CC3Matrix* modelMtx );

	/** Populates the current CC3Layer transform matrix from the specified GLKMatrix4 matrix. */
	void						populateLayerTransformMatrixFrom( const CC3Matrix4x4* layerMtx );

	/**
	 * Returns a pointer to the bone matrix at the specified index, from the currentSkinSection,
	 * in the global coordinate system.
	 * 
	 * This method has meaning only during the drawing of the currentSkinSection. Attempting to
	 * access this method at any other time will produced undefined results.
	 */
	const CC3Matrix4x3*			getGlobalBoneMatrixAt( GLuint index );

	/**
	 * Returns a pointer to the bone matrix at the specified index, from the currentSkinSection,
	 * in the coordinate system of the eye-space of the camera in the camera property.
	 *
	 * This method has meaning only during the drawing of the currentSkinSection. Attempting to
	 * access this method at any other time will produced undefined results.
	 */
	const CC3Matrix4x3*			getEyeSpaceBoneMatrixAt( GLuint index );

	/**
	 * Returns a pointer to the bone matrix at the specified index, from the currentSkinSection,
	 * in the coordinate system of the mesh node in the currentMeshNode property.
	 *
	 * This method has meaning only during the drawing of the currentSkinSection. Attempting to
	 * access this method at any other time will produced undefined results.
	 */
	const CC3Matrix4x3*			getModelSpaceBoneMatrixAt( GLuint index );

	virtual void				alignCameraViewport();
	virtual void				processBeforeChildren( CC3Node* aNode );
	virtual bool				shouldDrawNode( CC3Node* aNode );
	virtual bool				isNodeVisibleForDrawing( CC3Node* aNode );
	virtual bool				doesNodeIntersectFrustum( CC3Node* aNode );
	virtual bool				processChildrenOf( CC3Node* aNode );
	/** Prepares GL programs, activates the rendering surface, and opens the scene and the camera. */
	virtual void				open();
	/** 
	 * Activates the render surface and applies its viewport. 
	 * Subsequent GL drawing will be directed to this surface. 
	 */
	virtual void				activateRenderSurface();

	/** If this visitor was started on a CC3Scene node, set up for drawing an entire scene. */
	virtual void				openScene();

	/** Template method that opens the 3D camera. */
	virtual void				openCamera();

	/** Close the camera. This is the compliment of the openCamera method. */
	virtual void				closeCamera();

	/** Close the camera. */
	virtual void				close();

	/** Retrieve any light probe textures and bind them to the GL engine. */
	void						bindLightProbeTextures();

	void						setCamera( CC3Camera* camera );

	/**
	 * Overridden to return either the real shader program, or the pure-color shader program,
	 * depending on whether the node should be decorated. For example, during node picking,
	 * the node is not decorated, and the pure-color program will be used.
	 * The shaderProgram will be selected if it has not been assigned already.
	 */
	virtual CC3ShaderProgram*	getCurrentShaderProgram();

	void						setCurrentSkinSection( CC3SkinSection* currentSkinSection );

	/** Resets the bone matrices so they will be populated when requested for the current skin section. */
	void						resetBoneMatrices();

	/**
	 * Ensures that the specified bone matrix array has been populated from the bones of the
	 * current skin section. If the specified space matrix is not NULL, it is used to transform
	 * the bone matrices into some other space (eg- global space, eye space), otherwise, the
	 * bone matrices are left in their local space coordinates (model space).
	 * Once populated, the bone matrix array is marked as being ready, so it won't be populated
	 * again until being marked as not ready.
	 */
	void						ensureBoneMatrices( CC3DataArray* boneMatrices, const CC3Matrix4x3* spaceMatrix );

	void						init();
	std::string					fullDescription();

protected:
	CC3NodeSequencer*			m_drawingSequencer;
	CC3SkinSection*				m_currentSkinSection;
	CC3SceneDrawingSurfaceManager*	m_surfaceManager;
	CC3RenderSurface*			m_renderSurface;
	CC3OpenGL*					m_pGL;
	CC3DataArray*				m_boneMatricesGlobal;
	CC3DataArray*				m_boneMatricesEyeSpace;
	CC3DataArray*				m_boneMatricesModelSpace;
	CC3Matrix4x4				m_projMatrix;
	CC3Matrix4x3				m_viewMatrix;
	CC3Matrix4x3				m_modelMatrix;
	CC3Matrix4x4				m_viewProjMatrix;
	CC3Matrix4x3				m_modelViewMatrix;
	CC3Matrix4x4				m_modelViewProjMatrix;
	CC3Matrix4x4				m_layerTransformMatrix;
	ccColor4F					m_currentColor;
	CC3TextureBindingMode		m_textureBindingMode;
	GLuint						m_textureUnitCount;
	GLuint						m_current2DTextureUnit;
	GLuint						m_currentCubeTextureUnit;
	GLuint						m_currentLightProbeTextureUnit;
	float						m_fDeltaTime;
	bool						m_shouldDecorateNode : 1;
	bool						m_isDrawingEnvironmentMap : 1;
	bool						m_isVPMtxDirty : 1;
	bool						m_isMVMtxDirty : 1;
	bool						m_isMVPMtxDirty : 1;
};

NS_COCOS3D_END

#endif
