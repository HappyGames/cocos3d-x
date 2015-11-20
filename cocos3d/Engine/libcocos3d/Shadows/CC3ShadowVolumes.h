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
#ifndef _CC3_SHADOWVOLUMES_H_
#define _CC3_SHADOWVOLUMES_H_

/** The suggested default shadow volume vertex offset factor. */
static const GLfloat kCC3DefaultShadowVolumeVertexOffsetFactor = 0.001f;

NS_COCOS3D_BEGIN
/**
 * The mesh node used to build a shadow volume. A single CC3ShadowVolumeMeshNode
 * instance represents the shadow from a single light for a single shadow-casting node.
 *
 * As a mesh node, the CC3ShadowVolumeMeshNode instance is added as a child to the node
 * whose shadow is to be represented. To automatically create a CC3ShadowVolumeMeshNode
 * and add it to the shadow-casting node, use the addShadowVolumesForLight: method on
 * the shadow-casting node (or any structural ancestor of that node).
 *
 * CC3ShadowVolumeMeshNode implements the CC3ShadowProtocol. The implementation of the
 * updateShadow method populates a shadow volume mesh that encompasses the volume of
 * space shadowed by the shadow-casting node. Any other object with this shadow volume
 * will be shadowed by that node.
 *
 * The shadow volume mesh of this node is invisible in itself, but by depth-testing
 * against other drawn nodes, a stencil is created indicating which view pixels will
 * be in shadow. Those view pixels are then darkened accordingly.
 *
 * Of all shadowing techniques, shadow volumes result in the most accurate shadows,
 * but are also the most computationally intensive.
 *
 * Shadow volumes use a stencil buffer to determine the areas that require shading. The stencil
 * buffer must be allocated within the EAGLView when the view is created and initialized. Under
 * iOS, the sencil buffer is combined with the depth buffer, and you create a stencil buffer by
 * passing the value GL_DEPTH24_STENCIL8 as the depth format argument in the CCGLView method
 * viewWithFrame:pixelFormat:depthFormat:preserveBackbuffer:sharegroup:multiSampling:numberOfSamples:.
 */
 /// <CC3ShadowProtocol>
class CC3ShadowVolumeMeshNode : public CC3MeshNode, public CC3ShadowProtocol
{
	DECLARE_SUPER( CC3MeshNode );
public:
	CC3ShadowVolumeMeshNode();
	virtual ~CC3ShadowVolumeMeshNode();

	/**
	 * Indicates that this should display the terminator line of the shadow-casting node.
	 *
	 * The terminator line is the line that separates the illuminated side of the
	 * shadow-casting object from the dark side. It defines the start of the shadow
	 * volume mesh that is attached to the shadow-casting node.
	 *
	 * This property can be useful for diagnostics during development. This property
	 * only has effect if the visible property is set to YES for this shadow-volume node.
	 */
	bool						shouldDrawTerminator();
	void						setShouldDrawTerminator( bool shouldDraw );

	// TODO: will change when polymorphism has been figured out
	/**
	 * Draws this node to a stencil. The stencil is marked wherever another node
	 * intersects the mesh volume of this node, and is therefore in shadow.
	 *
	 * The application should not use this method. The method signature, and use of
	 * this method will change as additional shadow-casting techniques are introduced.
	 */
	void						drawToStencilWithVisitor( CC3NodeDrawingVisitor* visitor );

	/**
	 * Returns the default value to which the visible property will be set when an instance is
	 * created and initialized.
	 *
	 * The initial value of this property is NO. Normally, shadow volumes affect the contents of
	 * the stencil buffer, but are not directly visible themselves. However, during development
	 * debugging, you can set this property to YES to make the shadow volumes visible within the
	 * scene, to help visualize how the shadow volumes are interacting with the scene.
	 */
	static bool					defaultVisible();

	/**
	 * Sets the default value to which the visible property will be set when an instance is 
	 * created and initialized.
	 *
	 * The initial value of this property is NO. Normally, shadow volumes affect the contents of
	 * the stencil buffer, but are not directly visible themselves. However, during development
	 * debugging, you can set this property to YES to make the shadow volumes visible within the
	 * scene, to help visualize how the shadow volumes are interacting with the scene.
	 */
	static void					setDefaultVisible( bool _defaultVisible );

	/** Create the shadow volume mesh once the parent is attached. */
	void						setParent( CC3Node* aNode );
	bool						isShadowVolume();
	/**
	 * If shadow volume should be visible, add a material
	 * to display the volume, otherwise get rid of it.
	 */
	void						setVisible( bool _isVisible );

	void						createShadowMesh();
	/** A shadow volume only uses a material when it is to be visible during development. */
	void						checkShadowMaterial();

	GLushort					getShadowLagFactor();
	void						setShadowLagFactor( GLushort lagFactor );
	GLushort					getShadowLagCount();
	void						setShadowLagCount( GLushort lagCount );
	bool						shouldShadowFrontFaces();
	void						setShouldShadowFrontFaces( bool shouldShadow );
	bool						shouldShadowBackFaces();
	void						setShouldShadowBackFaces( bool shouldShadow );
	GLfloat						getShadowOffsetFactor();
	void						setShadowOffsetFactor( GLfloat factor );
	GLfloat						shadowOffsetUnits();
	void						setShadowOffsetUnits( GLfloat units );
	GLfloat						getShadowVolumeVertexOffsetFactor();
	void						setShadowVolumeVertexOffsetFactor( GLfloat factor );
	GLfloat						getShadowExpansionLimitFactor();
	void						setShadowExpansionLimitFactor( GLfloat factor );
	bool						shouldAddShadowVolumeEndCapsOnlyWhenNeeded();
	void						setShouldAddShadowVolumeEndCapsOnlyWhenNeeded( bool onlyWhenNeeded );
	bool						hasShadowVolumesForLight( CC3Light* aLight );
	bool						hasShadowVolumes();
	void						initWithTag( GLuint aTag, const std::string& aName );

	static CC3ShadowVolumeMeshNode*	nodeWithName( const std::string& aName );

	void						populateFrom( CC3ShadowVolumeMeshNode* another );
	virtual CCObject*			copyWithZone( CCZone* zone );

	bool						isShadowDirty();
	bool						useDepthFailAlgorithm();

	/**
	 * Overridden to return nil so that the shadow volume
	 * will always be drawn when made visible during development.
	 */
	CC3NodeBoundingVolume*		defaultBoundingVolume();
	/** Returns this node's parent, cast as a mesh node. */
	CC3MeshNode*				getShadowCaster();
	/**
	 * Returns a 4D directional vector which can be added to each vertex when creating
	 * the shadow volume vertices from the corresponding shadow caster vertices.
	 *
	 * The returned vector is in the local coordinate system of the shadow caster.
	 *
	 * The returned directional vector is a small offset vector in the direction away
	 * from the light. A unit vector in that direction is scaled by both the distance
	 * from the center of the shadow casting node to the camera and the
	 * shadowVolumeVertexOffsetFactor property. Hence, if the shadow caster is farther
	 * away from the camera, the returned value will be larger, to reduce the chance
	 * of Z-fighting between the faces of the shadow volume and the shadow caster.
	 */
	CC3Vector4					getShadowVolumeVertexOffsetForLightAt( const CC3Vector4& localLightPos );

	/**
	 * Populates the shadow volume mesh by iterating through all the faces in the mesh of
	 * the shadow casting node, looking for all pairs of neighbouring faces where one face
	 * is in illuminated (facing towards the light) and the other is dark (facing away from
	 * the light). The set of edges between these pairs forms the terminator of the mesh,
	 * where the mesh on one side of the terminator is illuminated and the other is dark.
	 *
	 * The shadow volume is then constructed by extruding each edge line segment in the
	 * terminator out to infinity in the direction away from the light source, forming a
	 * tube of infinite length.
	 *
	 * Uses the 4D homogeneous location of the light in the global coordinate system.
	 * When using the light location this method transforms this location to the local
	 * coordinates system of the shadow caster.
	 */
	void						populateShadowMesh();

	/**
	 * Adds a face to the cap at the near end of the shadow volume.
	 *
	 * The winding order of the end-cap faces is determined from the winding order of
	 * the model face, taking into consideration whether the face is lit or not.
	 */
	bool						addShadowVolumeCapFor( bool isFaceLit, CC3Vector4* vertices, const CC3Vector4& lightPosition, GLuint* shdwVtxIdx );

	/**
	 * When drawing the terminator line of the mesh, just add the two line
	 * endpoints, and don't make use of infinitely extruded endpoints.
	 */
	bool						addTerminatorLineFrom( const CC3Vector4& edgeStartLoc, const CC3Vector4& edgeEndLoc, GLuint* shdwVtxIdx );

	/**
	 * Adds a side to the shadow volume, by extruding the specified terminator edge of
	 * the specified shadow caster mesh to infinity. The light source is directional
	 * at the specified position. The shadow volume vertices are added starting at the
	 * specified vertex index.
	 * 
	 * For a directional light, the shadow volume sides are parallel and can therefore
	 * be described as meeting at a single point at infinity. We therefore only need to
	 * add a single triangle, whose far point is in the opposite direction of the light.
	 */
	bool						addShadowVolumeSideFrom( const CC3Vector4& edgeStartLoc, const CC3Vector4& edgeEndLoc, const CC3Vector4& lightPosition, GLuint* shdwVtxIdx );

	
	/**
	 * Adds a side to the shadow volume, by extruding the specified terminator edge of
	 * the specified shadow caster mesh to infinity. The light source is positional at
	 * the specified position. The shadow volume vertices are added starting at the
	 * specified vertex index.
	 * 
	 * For a locational light, the shadow volume sides are not parallel and expand as
	 * they extend away from the shadow casting object. If the shadow volume does not
	 * need to be capped off at the far end, the shadow expands to infinity.
	 *
	 * However, if the shadow volume needs to be capped off at the far end, the shadow
	 * is allowed to expand behind the shadow-caster to a distance equivalent to the
	 * distance from the light to the shadow-caster, multiplied by the value of the
	 * shadowExpansionLimitFactor property. At that point, the shadow volume will
	 * extend out to infinity at that same size.
	 * 
	 * For the shadow volume segment that expands in size, each side is a trapezoid
	 * formed by projecting a vector from the light, through each terminator edge
	 * vertex, out to either infinity of the distance determined by the value of the
	 * shadowExpansionLimitFactor property. Then, from that distance to infinity,
	 * the shadow volume side behaves as if it originated from a directional light,
	 * and is constructed from a single triangle, extending out to infinity in the
	 * opposite direction of the light.
	 */
	bool						addShadowVolumeSideFrom( const CC3Vector4& edgeStartLoc, const CC3Vector4& edgeEndLoc, bool  doesRequireCapping, const CC3Vector4& lightPosition, GLuint* shdwVtxIdx );

	/** 
	 * Expands the location of an terminator edge vertex in the direction away from the locational
	 * light at the specified location. The vertex is moved away from the light along the vector
	 * from the light to the vertex, a distance equal to the distance between the light and the
	 * vertex, multiplied by the value of the shadowExpansionLimitFactor property.
	 */
	CC3Vector4					expand( const CC3Vector4& edgeLoc, const CC3Vector4& lightLoc );

	/** Overridden to decrement the shadow lag count on each update. */
	void						processUpdateBeforeTransform( CC3NodeUpdatingVisitor* visitor );

	/** Returns whether the shadow cast by this shadow volume will be visible. */
	bool						isShadowVisible();

	/**
	 * Returns whether this shadow volume is ready to be updated.
	 * It is if the lag count has been decremented to zero.
	 */
	bool						isReadyToUpdate();

	/**
	 * If the shadow is ready to be updated, check if the shadow is both
	 * visible and dirty, and re-populate the shadow mesh if needed.
	 *
	 * To keep the shadow lag count synchronized across all shadow-casting nodes,
	 * the shadow lag count will be reset to the value of the shadow lag factor
	 * if the shadow is ready to be updated, even if it is not actually updated
	 * due to it being invisible, or not dirty.
	 */
	void						updateShadow();

	/**
	 * Selects whether to use the depth-fail or depth-pass algorithm,
	 * based on whether this shadow falls across the camera.
	 *
	 * The depth-fail algorithm requires end caps on this shadow volume.
	 * The depth-pass algorithm does not. Rendering end-caps when not needed
	 * creates a performance penalty, so the depth-pass algorithm can be used
	 * by setting the shouldAddEndCapsOnlyWhenNeeded property to YES.
	 *
	 * If the selected stencil algorithm changes, this shadow volume is marked
	 * as dirty so that the end caps will be added or removed appropriately.
	 */
	void						updateStencilAlgorithm();

	/** Overridden to remove this shadow node from the light. */
	void						wasRemoved();
	void						markTransformDirty();
	/** A node that affects this shadow (generally the light) was transformed. Mark the shadow as dirty. */
	virtual void				nodeWasTransformed( CC3Node* aNode );
	virtual void				nodeWasDestroyed( CC3Node* node );

	/** Overridden to set the line properties in addition to other configuration. */
	void						configureDrawingParameters( CC3NodeDrawingVisitor* visitor );
	void						drawToStencilIncrementing( bool isIncrementing, CC3NodeDrawingVisitor* visitor );

	// Shadows are not copied, because each shadow connects
	// one-and-only-one shadow casting node to one-and-only-one light.
	bool						shouldIncludeInDeepCopy();
	void						addShadowVolumesForLight( CC3Light* aLight );
	bool						shouldDrawDescriptor();
	void						setShouldDrawDescriptor( bool shouldDraw );
	bool						shouldDrawWireframeBox();
	void						setShouldDrawWireframeBox( bool shouldDraw );
	bool						shouldDrawLocalContentWireframeBox();
	void						setShouldDrawLocalContentWireframeBox( bool shouldDraw );
	bool						shouldContributeToParentBoundingBox();
	bool						shouldDrawBoundingVolume();
	void						setShouldDrawBoundingVolume( bool shouldDraw );
	// Overridden so that not touchable unless specifically set as such
	bool						isTouchable();

protected:
	CC3Light*					m_pLight;
	GLushort					m_shadowLagFactor;
	GLushort					m_shadowLagCount;
	GLfloat						m_shadowVolumeVertexOffsetFactor;
	GLfloat						m_shadowExpansionLimitFactor;
	bool						m_isShadowDirty : 1;
	bool						m_shouldDrawTerminator : 1;
	bool						m_shouldShadowFrontFaces : 1;
	bool						m_shouldShadowBackFaces : 1;
	bool						m_useDepthFailAlgorithm : 1;
	bool						m_shouldAddEndCapsOnlyWhenNeeded : 1;
};

/**
 * The mesh node used to paint the shadows cast by shadow volumes.
 *
 * Shadow volumes are used to define a stencil that is then used to draw dark areas onto the
 * viewport in clip-space, where scene mesh nodes are casting shadows. This painter is used
 * to draw those dark areas where the stencil indicates.
 */
/// <CC3ShadowProtocol>
class CC3StencilledShadowPainterNode : public CC3ClipSpaceNode, public CC3ShadowProtocol
{
public:
	/** The shadow painter is always drawn. */
	bool						isShadowVisible();
	void						updateShadow();
	CC3Light*					getLight();
	void						setLight( CC3Light* light );

	virtual void				nodeWasTransformed( CC3Node* aNode );
	virtual void				nodeWasDestroyed( CC3Node* aNode );
	static CC3StencilledShadowPainterNode* nodeWithName( const std::string& name, const ccColor4F& color );
};

/**
 * CC3ShadowDrawingVisitor is a CC3NodeDrawingVisitor that is passed
 * to a shadow node for drawing shadows.
 */
class CC3ShadowDrawingVisitor : public CC3NodeDrawingVisitor
{
	DECLARE_SUPER( CC3NodeDrawingVisitor );
public:
	void						init();
	bool						shouldDrawNode( CC3Node* aNode );

	static CC3ShadowDrawingVisitor* visitor();
};

NS_COCOS3D_END

#endif