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
#ifndef _CCL_CC3NODEVISITOR_H_
#define _CCL_CC3NODEVISITOR_H_
#include "../Matrices/CC3Matrix.h"
#include "../Utility/CC3DataArray.h"
#include "../Utility/CC3PerformanceStatistics.h"
#include "../OpenGL/CC3OpenGL.h"

NS_COCOS3D_BEGIN
class CC3Node;
class CC3MeshNode;
class CC3Camera;
class CC3Light;
class CC3LightProbe;
class CC3Scene;
class CC3ShaderProgram;
class CC3SceneDrawingSurfaceManager;
class CC3Material;
class CC3TextureUnit;
class CC3Mesh;
class CC3NodeSequencer;
class CC3SkinSection;
class CC3RenderSurface;

/**
 * A CC3NodeVisitor is a context object that is passed to a node when it is visited
 * during a traversal of the node hierarchy.
 *
 * To initiate a visitation run, invoke the visit: method on any CC3Node.
 *
 * Subclasses will override template methods to customize the behaviour prior to, during,
 * and after the node traversal.
 *
 * If a node is to be removed from the node structural hierarchy during a visitation run,
 * the requestRemovalOf: method can be used instead of directly invoking the remove method
 * on the node itself. A visitation run involves iterating through collections of child
 * nodes, and removing a node during the iteration of a collection raises an error.
 */
class CC3NodeVisitor : public CCObject 
{
public:
	CC3NodeVisitor();
	virtual ~CC3NodeVisitor();

	/**
	 * Indicates whether this visitor should traverse the child nodes of any node it visits.
	 *
	 * The initial value of this property is YES.
	 */
	bool						shouldVisitChildren();
	void						setShouldVisitChildren( bool shouldVisit );

	/**
	 * Visits the specified node, then if the shouldVisitChildren property is set to YES,
	 * invokes this visit: method on each child node as well.
	 *
	 * Returns whether the visitation run was stopped once a desired result was accomplished.
	 * Depending on the type of visitation, this might occur if a particular node was reached,
	 * or some other desired result has been accomplished. The purpose of the returned value
	 * is not to indicate whether all node have been visited, or even that the visitation was
	 * aborted. Instead, you should think of the returned value as a way of indicating that
	 * a desired result has been accomplished, and that there is no need to visit further nodes.
	 * For visitations that normally visit all nodes, such as drawing, or updating, the return
	 * value will generally be NO.
	 *
	 * Subclasses will override several template methods to customize node visitation behaviour.
	 */
	virtual bool				visit( CC3Node* aNode );

	/**
	 * Requests the removal of the specfied node.
	 *
	 * During a visitation run, to remove a node from the hierarchy, you must use this method
	 * instead of directly invoking the remove method on the node itself. Visitation involves
	 * iterating through collections of child nodes, and removing a node during the iteration
	 * of a collection raises an error.
	 *
	 * This method can safely be invoked while a node is being visited. The visitor keeps
	 * track of the requests, and safely removes all requested nodes as part of the close
	 * method, once the visitation of the full node assembly is finished.
	 */
	virtual void				requestRemovalOf( CC3Node* aNode );

	/**
	 * The CC3Node on which this visitation traversal was intitiated. This is the node
	 * on which the visit: method was first invoked to begin a traversal of the node
	 * structural hierarchy.
	 *
	 * This property is only valid during the traversal, and will be nil both before
	 * and after the visit: method is invoked.
	 */
	virtual CC3Node*			getStartingNode();

	/**
	 * Returns the CC3Scene.
	 *
	 * This is a convenience property that returns the scene property of the startingNode property.
	 */
	virtual CC3Scene*			getScene();

	/**
	 * The camera that is viewing the 3D scene.
	 *
	 * If this property is not set in advance, it is lazily initialized to the value
	 * of the defaultCamera property when first accessed during a visitation run.
	 *
	 * The value of this property is not cleared at the end of the visitation run.
	 */
	virtual CC3Camera*			getCamera();
	virtual void				setCamera( CC3Camera* camera );

	/**
	 * The default camera to use when visiting a node assembly.
	 *
	 * This implementation returns the activeCamera property of the starting node.
	 * Subclasses may override.
	 */
	virtual CC3Camera*			getDefaultCamera();

	/**
	 * The CC3Node that is currently being visited.
	 *
	 * This property is only valid during the traversal of the node returned by this property,
	 * and will be nil both before and after the visit: method is invoked on the node.
	 */
	virtual CC3Node*			getCurrentNode();

	/**
	 * Returns the mesh node that is currently being visited.
	 *
	 * This is a convenience property that returns the value of the currentNode property,
	 * cast as a CC3MeshNode. It is up to the invoker to make sure that the current node
	 * actually is a CC3MeshNode.
	 *
	 * This property is only valid during the traversal of the node returned by this property,
	 * and will be nil both before and after the visit: method is invoked on that node.
	 */
	CC3MeshNode*				getCurrentMeshNode();

	/**
	 * Returns the mesh of the mesh node that is currently being visited.
	 *
	 * It is up to the invoker to make sure that the current node actually is a CC3MeshNode.
	 *
	 * This property is only valid during the traversal of the node returned by this property,
	 * and will be nil both before and after the visit: method is invoked on the node.
	 */
	CC3Mesh*					getCurrentMesh();

	/**
	 * Returns the number of textures in the current mesh node.
	 *
	 * It is up to the invoker to make sure that the current node actually is a CC3MeshNode.
	 */
	virtual GLuint				getTextureCount();

	/**
	 * Returns the material of the mesh node that is currently being visited, or returns nil
	 * if that mesh node has no material.
	 *
	 * It is up to the invoker to make sure that the current node actually is a CC3MeshNode.
	 *
	 * This property is only valid during the traversal of the node returned by the currentMeshNode
	 * property, and will be nil both before and after the visit: method is invoked on that node.
	 */
	CC3Material*				getCurrentMaterial();

	/**
	 * Returns the texture unit at the specified index from the mesh node that is currently being
	 * visited, or returns nil if the material covering the node has no corresponding texture unit.
	 *
	 * It is up to the invoker to make sure that the current node actually is a CC3MeshNode.
	 *
	 * The value returned by this method is only valid during the traversal of the node returned
	 * by the currentMeshNode property, and will be nil both before and after the visit: method
	 * is invoked on that node.
	 */
	CC3TextureUnit*				getCurrentTextureUnitAt( GLuint texUnit );

	/**
	 * Under OpenGL ES 2.0 & OpenGL, returns the shader program of the mesh node that is currently being
	 * visited, or returns nil if that mesh node has no shader program, or when using OpenGL ES 1.1.
	 *
	 * It is up to the invoker to make sure that the current node actually is a CC3MeshNode.
	 *
	 * This property is only valid during the traversal of the node returned by the currentMeshNode
	 * property, and will be nil both before and after the visit: method is invoked on that node.
	 */
	virtual  CC3ShaderProgram*	getCurrentShaderProgram();

	/** Number of lights in the scene. */
	GLuint						getLightCount();

	/**
	 * Returns the light indicated by the index, or nil if the specified index is greater than
	 * the number of lights currently existing in the scene.
	 *
	 * The specified index is an index into the lights array of the scene, and is not necessarily
	 * the same as the lightIndex property of the CC3Light.
	 */
	CC3Light*					getLightAt( GLuint index );

	/** Min of number of light probes in the scene, and the number used by the shader program. */
	GLuint						getLightProbeCount();

	/**
	 * Returns the light probe indicated by the index, or nil if the specified index is 
	 * greater than the number of light probes currently existing in the scene.
	 *
	 * The specified index is an index into the lightProbes array of the scene.
	 */
	CC3LightProbe*				getLightProbeAt( GLuint index );

	/**
	 * The performanceStatistics being accumulated during the visitation runs.
	 *
	 * This is extracted from the startingNode, and may be nil if that node
	 * is not collecting statistics.
	 */
	CC3PerformanceStatistics*	getPerformanceStatistics();

	/** Allocates and initializes an autoreleased instance. */
	static CC3NodeVisitor*		visitor();

	/** Returns a more detailed description of this instance. */
	virtual std::string			fullDescription();

	/**
	 * Template method that prepares the visitor to perform a visitation run. This method
	 * is invoked automatically prior to the first node being visited. It is not invoked
	 * for each node visited.
	 *
	 * This implementation does nothing. Subclasses can override to initialize their state,
	 * or to set any external state needed, such as GL state, prior to starting a visitation
	 * run, and should invoke this superclass implementation.
	 */
	virtual void				open();
	/** Template method that is invoked automatically during visitation to process the specified node. */
	virtual bool				process( CC3Node* aNode );
	/**
	 * Invoked automatically after the last node has been visited during a visitation run.
	 * This method is invoked automatically after all nodes have been visited.
	 * It is not invoked for each node visited.
	 *
	 * This implementation processes the removals of any nodes that were requested to
	 * be removed via the requestRemovalOf: method during the visitation run. Subclasses
	 * can override to clean up their state, or to reset any external state, such as GL
	 * state, upon completion of a visitation run, and should invoke this superclass
	 * implementation to process any removal requests.
	 */
	virtual void				close();
	/**
	 * Template method that is invoked automatically to process the specified node when
	 * that node is visited, before the visit: method is invoked on the child nodes of
	 * the specified node.
	 * 
	 * This abstract implementation does nothing. Subclasses will override to process
	 * each node as it is visited.
	 */
	virtual void				processBeforeChildren( CC3Node* aNode );

	/**
	 * If the shouldVisitChildren property is set to YES, this template method is invoked
	 * automatically to cause the visitor to visit the child nodes of the specified node .
	 *
	 * This implementation invokes the visit: method on this visitor for each of the
	 * children of the specified node. This establishes a depth-first traveral of the
	 * node hierarchy.
	 *
	 * Subclasses may override this method to establish a different traversal.
	 */
	virtual bool				processChildrenOf( CC3Node* aNode );

	/**
	 * Invoked automatically to process the specified node when that node is visited,
	 * after the visit: method is invoked on the child nodes of the specified node.
	 * 
	 * This abstract implementation does nothing. Subclasses will override to process
	 * each node as it is visited.
	 */
	virtual void				processAfterChildren( CC3Node* aNode );

	virtual void				processRemovals();
	void						init();

protected:
	CC3Node*					_startingNode;
	CC3Node*					_currentNode;
	CC3Camera*					_camera;
	CCArray*					_pendingRemovals;
	bool						_shouldVisitChildren : 1;
};

/**
 * CC3NodeUpdatingVisitor is a CC3NodeVisitor that is passed to a node when it is visited
 * during updating and transforming operations.
 *
 * This visitor encapsulates the time since the previous update.
 */
class CC3NodeUpdatingVisitor : public CC3NodeVisitor 
{
	DECLARE_SUPER( CC3NodeVisitor );
public:
	static CC3NodeUpdatingVisitor* visitor();
	/**
	 * This property gives the interval, in seconds, since the previous update. This value can be
	 * used to create realistic real-time motion that is independent of specific frame or update rates.
	 * Depending on the setting of the maxUpdateInterval property of the CC3Scene instance, the value
	 * of this property may be clamped to an upper limit. See the description of the CC3Scene
	 * maxUpdateInterval property for more information about clamping the update interval.
	 */
	float						getDeltaTime();
	void						setDeltaTime( float dt );

	virtual void				processBeforeChildren( CC3Node* aNode );
	virtual void				processAfterChildren( CC3Node* aNode );
	std::string					fullDescription();

protected:
	float						_deltaTime;
};

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
	CC3NodeSequencer*			_drawingSequencer;
	CC3SkinSection*				_currentSkinSection;
	CC3SceneDrawingSurfaceManager*	_surfaceManager;
	CC3RenderSurface*			_renderSurface;
	CC3OpenGL*					_gl;
	CC3DataArray*				_boneMatricesGlobal;
	CC3DataArray*				_boneMatricesEyeSpace;
	CC3DataArray*				_boneMatricesModelSpace;
	CC3Matrix4x4				_projMatrix;
	CC3Matrix4x3				_viewMatrix;
	CC3Matrix4x3				_modelMatrix;
	CC3Matrix4x4				_viewProjMatrix;
	CC3Matrix4x3				_modelViewMatrix;
	CC3Matrix4x4				_modelViewProjMatrix;
	CC3Matrix4x4				_layerTransformMatrix;
	ccColor4F					_currentColor;
	CC3TextureBindingMode		_textureBindingMode;
	GLuint						_textureUnitCount;
	GLuint						_current2DTextureUnit;
	GLuint						_currentCubeTextureUnit;
	GLuint						_currentLightProbeTextureUnit;
	float						_deltaTime;
	bool						_shouldDecorateNode : 1;
	bool						_isDrawingEnvironmentMap : 1;
	bool						_isVPMtxDirty : 1;
	bool						_isMVMtxDirty : 1;
	bool						_isMVPMtxDirty : 1;
};

/**
 * CC3NodePickingVisitor is a CC3NodeDrawingVisitor that is passed to a node when
 * it is visited during node picking operations using color-buffer based picking.
 *
 * The visit: method must be invoked with a CC3Scene instance as the argument.
 *
 * Node picking is the act of picking a 3D node from user input, such as a touch.
 * One method of accomplishing this is to draw the scene such that each object is
 * drawn in a unique solid color. Once the scene is drawn, the color of the pixel
 * that has been touched can be read from the OpenGL ES color buffer, and mapped
 * back to the object that was painted with that color.
 */
class CC3NodePickingVisitor : public CC3NodeDrawingVisitor 
{
	DECLARE_SUPER( CC3NodeDrawingVisitor );
public:
	CC3NodePickingVisitor();
	~CC3NodePickingVisitor();
	/** 
	 * Returns the node that was most recently picked.
	 *
	 * This picked node is valid at a point in time. The value of this property is cleared 
	 * immediately after it has been read. Subsequent queries of this property will return nil,
	 * so you should cache the returned value elsewhere if you need it to persist.
	 */
	CC3Node*					getPickedNode();

	/** 
	 * Indicates the value to shift the bits of the value of the tag property of each node to
	 * determine the color to paint that node.
	 *
	 * The initial value of this property is zero, indicating that the node tag value will not
	 * be shifted when converting it to and from a color. Increasing the value will increase the
	 * color separation between different nodes, which can be helpful during development when 
	 * debugging node picking visually (ie- when the shouldDisplayPickingRender property of the
	 * CC3Scene is set to YES), However, increasing the shift value will also decrease the number
	 * of nodes that can be displayed and resolved on screen.
	 *
	 * This value is a shift value that operates on the bits of the tag value. A value of one
	 * will effectively double the tag value before it is converted to a color, a value of two
	 * will quadruple the tag value, etc.
	 */
	GLuint						getTagColorShift();
	void						setTagColorShift( GLuint shift );

	/**
	 * Template property that returns the default value used to automatically
	 * set the value of the renderSurface property.
	 *
	 * Overridden to return the value of the pickingSurface property of the
	 * CC3SceneDrawingSurfaceManager held in the surfaceManager property.
	 */
	CC3RenderSurface*			getDefaultRenderSurface();

	/**
	 * Aligns this visitor to use the same camera as the specified visitor.
	 *
	 * The camera and surfaceManager properties of this visitor are set to those of the specified visitor.
	 *
	 * The renderSurface property is left cleared, so that the defaultRenderSurface property will set
	 * it to the pickingSurface of the surfaceManager, when the renderSurface property is next accessed.
	 */
	void						alignShotWith( CC3NodeDrawingVisitor* otherVisitor );

	/** Overridden to initially set the shouldDecorateNode to NO. */
	void						init();

	/** Clears the render surface and the pickedNode property. */
	void						open();

	/**
	 * Reads the color of the pixel at the touch point, maps that to the tag of the CC3Node
	 * that was touched, and sets the picked node in the pickedNode property.
	 *
	 * Clears the depth buffer in case the primary scene rendering is using the same surface.
	 */
	void						close();

	/**
	 * Maps the specified color to a tag, by combining the bits of the red, green, and blue
	 * colors into a single integer value. This is the compliment of the colorFromNodeTag: method.
	 */
	GLuint						tagFromColor( const ccColor4B& color );

	/**
	 * Overridden because what matters here is not visibility, but touchability.
	 * Invisible nodes will be drawn if touchable.
	 */
	virtual bool				isNodeVisibleForDrawing( CC3Node* aNode );

	/** Overridden to draw the node in a uniquely identifiable color. */
	virtual void				draw( CC3Node* aNode );

	/** Maps the specified node to a unique color, and paints the node with that color. */
	void						paintNode( CC3Node* aNode );

	/**
	 * Maps the specified integer tag to a color, by spreading the bits of the integer across
	 * the red, green and blue unsigned bytes of the color. This permits 2^24 objects to be
	 * encoded by colors. This is the compliment of the tagFromColor: method.
	 */
	ccColor4B					colorFromNodeTag( GLuint tag );

	std::string					fullDescription();

	static CC3NodePickingVisitor* visitor();

protected:
	CC3Node*					_pickedNode;
	GLuint						_tagColorShift;
};


/** Helper class for CC3NodePuncturingVisitor that tracks a node and the location of its puncture. */
class CC3NodePuncture : public CCObject
{
	DECLARE_SUPER( CC3NodePuncture );
public:
	CC3NodePuncture();
	~CC3NodePuncture();

	/** The punctured node. */
	CC3Node*					getNode();

	/** The location of the puncture, in the local coordinate system of the punctured node. */
	CC3Vector					getPunctureLocation();

	/** The location of the puncture, in the global coordinate system. */
	CC3Vector					getGlobalPunctureLocation();

	/**
	 * The square of the distance from the startLocation of the ray to the puncture.
	 * This is used to sort the punctures by distance from the start of the ray.
	 */
	float						getSQGlobalPunctureDistance();

	/** Initializes this instance with the specified node and ray. */
	void						initOnNode( CC3Node* aNode, const CC3Ray& aRay );

	/** Allocates and initializes an autoreleased instance with the specified node and ray. */
	static CC3NodePuncture*		punctureOnNode( CC3Node* aNode, const CC3Ray& aRay );

protected:
	CC3Node*					_node;
	CC3Vector					_punctureLocation;
	CC3Vector					_globalPunctureLocation;
	float						_sqGlobalPunctureDistance;
};

/**
 * CC3NodePuncturingVisitor is a CC3NodeVisitor that is used to collect nodes
 * that are punctured (intersected) by a global ray.
 *
 * For example, you can use the CC3Camera unprojectPoint: method to convert a 2D touch point
 * into a CC3Ray that projects into the 3D scene from the center of the camera. All objects
 * that lie visually below the touch point will be punctured by that projected ray.
 *
 * Or, you may want to know which nodes lie under a targetting reticle, or have been hit by
 * the path of a bullet.
 * 
 * To find the nodes that are punctured by a global CC3Ray, create an instance of this class,
 * and invoke the visit: method on that instance, passing the CC3Scene as the argument. You can
 * also invoke the visit: method with a particular structural node, instead of the full CC3Scene,
 * to limit the range of nodes to inspect (for example, to determine which object in a room, but
 * not outside the room, was hit by a bullet), for design or performance reasons.
 *
 * The visitor will collect the nodes that are punctured by the ray, in order of distance from
 * the startLocation of the CC3Ray. You can access the nodes and the puncture locations using the
 * closestPuncturedNode, punctureNodeAt: closestPunctureLocation, and punctureLocationAt: methods.
 *
 * Only nodes that have a bounding volume will be tested by this visitor. Nodes without a bounding
 * volume, or whose shouldIgnoreRayIntersection property is set to YES will be ignored by this visitor.
 *
 * The shouldPunctureFromInside property can be used to include or exclude nodes where the start
 * location of the ray is within its bounding volume. 
 *
 * To save instantiating a CC3NodePuncturingVisitor each time, you can reuse the visitor instance
 * over and over, through different invocations of the visit: method.
 */
class CC3NodePuncturingVisitor : public CC3NodeVisitor 
{
	DECLARE_SUPER( CC3NodeVisitor );
public:
	CC3NodePuncturingVisitor();
	~CC3NodePuncturingVisitor();
	/**
	 * Indicates whether the visitor should consider the ray to intersect a node's
	 * bounding volume if the ray starts within the bounding volume of the node.
	 *
	 * The initial value of this property is NO, indicating that the visitor
	 * will not collect punctures for any node where the ray starts within
	 * the bounding volume of that node.
	 *
	 * This initial value makes sense for the common use of using the ray to pick
	 * nodes from a touch, as, when the camera is within a node, that node will
	 * not be visible. However, if you have a character within a room, and you
	 * want to know where in the room a thrown object hits the walls, you might
	 * want to set this property to YES to collect nodes that are punctured from
	 * the inside as well as from the outside.
	 */
	bool						shouldPunctureFromInside();
	void						setShouldPunctureFromInside( bool shouldPuncture );

	/**
	 * Indicates whether the visitor should include those nodes that are not
	 * visible (whose visible property returns NO), when collecting the nodes
	 * whose bounding volumes are punctured by the ray.
	 *
	 * The initial value of this property is NO, indicating that invisible
	 * nodes will be ignored by this visitor.
	 */
	bool						shouldPunctureInvisibleNodes();
	void						setShouldPunctureInvisibleNodes( bool shouldPuncture );

	/**
	 * The ray that is to be traced, specified in the global coordinate system.
	 *
	 * This property is set on initialization, but you may set it to another
	 * ray when reusing the same visitor on more than one visitation.
	 */
	CC3Ray						getRay();
	void						setRay( const CC3Ray& ray );

	/** The number of nodes that were punctured by the ray. */
	unsigned int				getNodeCount();

	/**
	 * Returns the node punctured by the ray that is closest to the startLocation
	 * of the ray, or nil if the ray intersects no nodes.
	 *
	 * The result will not include any node that does not have a bounding volume,
	 * or whose shouldIgnoreRayIntersection property is set to YES.
	 */
	CC3Node*					getClosestPuncturedNode();

	/**
	 * Returns the location of the puncture on the node returned by the
	 * closestPuncturedNode property, or kCC3VectorNull if the ray intersects no nodes.
	 *
	 * The returned location is on the bounding volume of the node (or tightest
	 * bounding volume if the node is using a composite bounding volume such as
	 * CC3NodeTighteningBoundingVolumeSequence), and is specified in the local
	 * coordinate system of the node.
	 *
	 * The result will not include any node that does not have a bounding volume,
	 * or whose shouldIgnoreRayIntersection property is set to YES.
	 */
	CC3Vector					getClosestPunctureLocation();

	/**
	 * Returns the location of the puncture on the node returned by the
	 * closestPuncturedNode property, or kCC3VectorNull if the ray intersects no nodes.
	 *
	 * The returned location is on the bounding volume of the node (or tightest
	 * bounding volume if the node is using a composite bounding volume such as
	 * CC3NodeTighteningBoundingVolumeSequence), and is specified in the global
	 * coordinate system.
	 */
	CC3Vector					getClosestGlobalPunctureLocation();

	/**
	 * Returns the node punctured by the ray at the specified order index,
	 * which must be between zero and nodeCount minus one, inclusive.
	 *
	 * When multiple nodes are punctured by the ray, they can be accessed
	 * using the specified positional index, with the order determined by
	 * the distance from the startLocation of the ray to the global location
	 * of the puncture for each node. The index zero represents the node
	 * whose puncture is globally closest to the startLocation of the ray.
	 *
	 * The results will not include nodes that do not have a bounding volume,
	 * or whose shouldIgnoreRayIntersection property is set to YES.
	 */
	CC3Node*					getPuncturedNodeAt( unsigned int index );

	/**
	 * Returns the location of the puncture on the node returned by the
	 * puncturedNodeAt: method. The specified index must be between zero
	 * and nodeCount minus one, inclusive.
	 *
	 * When multiple nodes are punctured by the ray, the location of the
	 * puncture on each can be accessed using the specified positional index,
	 * with the order determined by the distance from the startLocation of
	 * the ray to the global location of the puncture for each node. The
	 * index zero represents the node whose puncture is globally closest
	 * to the startLocation of the ray.
	 *
	 * The returned location is on the bounding volume of the node (or tightest
	 * bounding volume if the node is using a composite bounding volume such as
	 * CC3NodeTighteningBoundingVolumeSequence), and is specified in the local
	 * coordinate system of the node.
	 *
	 * The results will not include nodes that do not have a bounding volume,
	 * or whose shouldIgnoreRayIntersection property is set to YES.
	 */
	CC3Vector					getPunctureLocationAt( unsigned int index );

	/**
	 * Returns the location of the puncture on the node returned by the
	 * puncturedNodeAt: method. The specified index must be between zero
	 * and nodeCount minus one, inclusive.
	 *
	 * When multiple nodes are punctured by the ray, the location of the
	 * puncture on each can be accessed using the specified positional index,
	 * with the order determined by the distance from the startLocation of
	 * the ray to the global location of the puncture for each node. The
	 * index zero represents the node whose puncture is globally closest
	 * to the startLocation of the ray.
	 *
	 * The returned location is on the bounding volume of the node (or tightest
	 * bounding volume if the node is using a composite bounding volume such as
	 * CC3NodeTighteningBoundingVolumeSequence), and is specified in the local
	 * coordinate system of the node.
	 *
	 * The results will not include nodes that do not have a bounding volume,
	 * or whose shouldIgnoreRayIntersection property is set to YES.
	 */
	CC3Vector					getGlobalPunctureLocationAt( unsigned int index );

	/**
	 * Initializes this instance with the specified ray,
	 * which is specified in the global coordinate system.
	 */
	void						initWithRay( const CC3Ray& aRay );

	/**
	 * Allocates and initializes an autoreleased instance with the specified ray,
	 * which is specified in the global coordinate system.
	 */
	static CC3NodePuncturingVisitor* visitorWithRay( const CC3Ray& aRay );

	CC3NodePuncture*			getNodePunctureAt( unsigned int index );
	void						open();
	/**
	 * Utility method that returns whether the specified node is punctured by the ray.
	 *   - Returns NO if the node has no bounding volume.
	 *   - Returns NO if the node is invisible, unless the shouldPunctureInvisibleNodes property
	 *     has been set to YES.
	 *   - Returns NO if the ray starts within the bounding volume, unless the 
	 *     shouldPunctureFromInside property has been set to YES.
	 */
	bool						doesPuncture( CC3Node* aNode );
	void						processBeforeChildren( CC3Node* aNode );

	void						init();

protected:
	CCArray*					_nodePunctures;
	CC3Ray						_ray;
	bool						_shouldPunctureFromInside : 1;
	bool						_shouldPunctureInvisibleNodes : 1;
};

NS_COCOS3D_END

#endif
