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
#ifndef _CC3_UTILITY_MESH_NODES_H_
#define _CC3_UTILITY_MESH_NODES_H_

NS_COCOS3D_BEGIN

/**
 * CC3PlaneNode is a type of CC3MeshNode that is specialized to display planes and
 * simple rectanglular meshes.
 *
 * Since a plane is a mesh like any other mesh, the functionality required to create
 * and manipulate plane meshes is present in the CC3MeshNode class, and if you choose,
 * you can create and manage plane meshes using that class alone. Some plane-specific
 * functionality is defined within this class.
 * 
 * Several convenience methods exist in the CC3MeshNode class to aid in constructing a
 * CC3PlaneNode instance:
 *   - populateAsCenteredRectangleWithSize:
 *   - populateAsRectangleWithSize:andPivot:
 */
class CC3PlaneNode : public CC3MeshNode
{
public:
	static CC3PlaneNode*		nodeWithName( const std::string& aName );
	
	/**
	 * Returns a CC3Plane structure corresponding to this plane.
	 *
	 * This structure is built from the location vertices of three of the corners
	 * of the bounding box of the mesh.
	 */
	CC3Plane					getPlane();
};

/**
 * CC3LineNode is a type of CC3MeshNode that is specialized to display lines.
 *
 * Since lines are a mesh like any other mesh, the functionality required to create and manipulate
 * line meshes is present in the CC3MeshNode class, and if you choose, you can create and manage line
 * meshes using that class alone. At present, CC3LineNode exists for the most part simply to identify
 * box meshes as such. However, in future, additional state or behaviour may be added to this class.
 *
 * To draw lines, you must make sure that the drawingMode property is set to one of GL_LINES,
 * GL_LINE_STRIP or GL_LINE_LOOP. This property must be set after the mesh is attached.
 * Other than that, you configure the mesh node and its mesh as you would with any mesh node.
 *
 * Several convenience methods exist in the CC3MeshNode class to aid in constructing a
 * CC3LineNode instance:
 *   - populateAsLineStripWith:vertices:andRetain:
 *   - populateAsWireBox:  - a simple wire box
 */
class CC3LineNode : public CC3MeshNode
{
public:

};

/**
 * CC3SimpleLineNode simplifies the creation of a simple two-point straight line.
 *
 * You can create a single simple straight line model by instantiating an instance of this
 * class and then setting either or both of the lineStart and lineEnd properties.
 *
 * The mesh underlying this node is automatically populated as a simple two-vertex line.
 * When using this class, you do not need to use any of the populateAs... methods to generate
 * and populate the mesh.
 */
class CC3SimpleLineNode : public CC3LineNode 
{
	DECLARE_SUPER( CC3LineNode );
public:
	/**
	 * Indicates the start of the line in the local coordinate system of this node.
	 *
	 * The initial value is kCC3VectorZero, indicating that the line starts at the origin of
	 * the local coordinate system.
	 */
	CC3Vector					getLineStart();
	void						setLineStart( const CC3Vector& aLocation );

	/**
	 * Indicates the end of the line in the local coordinate system of this node.
	 *
	 * The initial value is kCC3VectorZero, indicating that the line ends at the origin of
	 * the local coordinate system.
	 */
	CC3Vector					getLineEnd();
	void						setLineEnd( const CC3Vector& aLocation );

	void						initWithTag( GLuint aTag, const std::string& aName );


protected:
	CC3Vector					_lineVertices[2];
};

/**
 * CC3BoxNode is a type of CC3MeshNode that is specialized to display simple box or cube meshes.
 *
 * Since a cube or box is a mesh like any other mesh, the functionality required to create and
 * manipulate box meshes is present in the CC3MeshNode class, and if you choose, you can create
 * and manage box meshes using that class alone. At present, CC3BoxNode exists for the most part
 * simply to identify box meshes as such. However, in future, additional state or behaviour may
 * be added to this class.
 *
 * You can use one of the following convenience methods to aid in constructing a CC3BoxNode instance:
 *   - populateAsSolidBox:
 *   - populateAsSolidBox:withCorner:
 *   - populateAsWireBox:
 */
class CC3BoxNode : public CC3MeshNode
{
public:
	static CC3BoxNode*			nodeWithName( const std::string& aName );
	static CC3BoxNode*			node();
};

/**
 * CC3TouchBox is a specialized node that creates an invisible box mesh that can be used to
 * define a 3D region for touch activity.
 *
 * If you do not set the box property explicitly, when you add an instance of this class as a child
 * of another CC3Node, this node will automatically be populated as a box the same size as the
 * bounding box of that parent. If the parent node contains other nodes, its bounding box will
 * include its descendants, resulting in this mesh being populated to encapsulate all descendant
 * nodes of its parent. The effect is to define a box-shaped touch region around a composite node
 * that might be comprised of a number of smaller nodes with space in between them.
 *
 * If the parent node contains descendants that are moving around, the bounding box of the parent
 * node may be dynamic and constantly changing. If you want the touch box to track changes to the
 * parent bounding box, set the shouldAlwaysMeasureParentBoundingBox property to YES.
 *
 * You can also set the box property directly to create a box that is shaped differently than the
 * bounding box of the parent. For example, you might want to do this if you want the touch box to
 * be larger than the actual visible nodes, in order to make it easier to touch.
 *
 * The mesh underlying this node is automatically populated when you set the box property, or when
 * you add this node to a parent. You do not need to invoke any of the populateAs... methods directly.
 *
 * Since this node is intended to be used as an invisible touch pad, the visible property of this node
 * is initially set to NO, and the shouldAllowTouchableWhenInvisible property is initially set to YES.
 * In addition, the bounding box of this mesh will not contribute to the bounding box of the parent.
 */
class CC3TouchBox : public CC3BoxNode 
{
	DECLARE_SUPER( CC3BoxNode );
public:
	/**
	 * Indicates the size of the touch box.
	 *
	 * Setting this property populates this node with a box mesh of the specified extent.
	 *
	 * Instead of setting this property directly, you can automatically create the box mesh by simply
	 * adding this node to a parent CC3Node. If this property has not already been set when this node
	 * is added to a parent, the value of this property will automatically be set to the value of the
	 * boundingBox property of the parent.
	 *
	 * If the parent node contains descendants that are moving around, the bounding box of the parent
	 * node may be dynamic and constantly changing. If you want the touch box to track changes to the
	 * parent bounding box, set the shouldAlwaysMeasureParentBoundingBox property to YES.
	 *
	 * If you set this property directly, and then subsequently add this node to a parent, the value
	 * of this property will not change, and the underlying mesh will not be repopulated. By setting
	 * the value of this property directly, you can create a mesh box that is of a different size
	 * than the parent bounding box.
	 *
	 * Setting this property to kCC3BoxNull will remove the underlying mesh.
	 *
	 * The initial value of this property is kCC3BoxNull.
	 */
	CC3Box						getBox();
	void						setBox( const CC3Box& box );

	/**
	 * Indicates whether the dimensions of this node should automatically be remeasured on each update pass.
	 *
	 * If this property is set to YES, the box will automatically be resized to account for movements by
	 * any descendant nodes of the parent node. To create a dynamic touch box that automatically adjusts
	 * as the descendants of the parent node move around, this property should be set to YES.
	 *
	 * It is not necessary to set this property to YES to account for changes in the transform properties
	 * of the parent node itself.
	 *
	 * When setting this property, be aware that dynamically measuring the bounding box of the parent node
	 * can be an expensive operation if the parent contains a number of descendant nodes.
	 *
	 * The initial value of this property is NO.
	 */
	bool						shouldAlwaysMeasureParentBoundingBox();
	void						setShouldAlwaysMeasureParentBoundingBox( bool shouldMeasure );

	void						populateBox( const CC3Box& aBox );
	void						setParent( CC3Node* aNode );

	CC3Box						getParentBoundingBox();
	std::string					getNameSuffix();
	bool						shouldContributeToParentBoundingBox();
	/** Overridden because we only need vertex locations, and to allocate and populate indices. */
	CC3Mesh*					prepareParametricMesh();
	/** If we should remeasure and update the bounding box dimensions, do so. */
	void						updateAfterTransform( CC3NodeUpdatingVisitor* visitor );
	void						initWithTag( GLuint aTag, const std::string& aName );

protected:
	bool						_shouldAlwaysMeasureParentBoundingBox : 1;
};

/**
 * CC3SphereNode is a type of CC3MeshNode that is specialized to display a simple sphere mesh.
 *
 * Since a sphere is a mesh like any other mesh, the functionality required to create and
 * manipulate sphere meshes is present in the CC3MeshNode class, and if you choose, you can
 * create and manage sphere meshes using that class alone.
 *
 * However, when using bounding volumes, CC3SphereNode returns a spherical bounding volume
 * from the defaultBoundingVolume method, instead of the default bounding volume for a
 * standard mesh node. This provides a better fit of the bounding volume around the mesh.
 *
 * You can use the following convenience method to aid in constructing a CC3SphereNode instance:
 *   - populateAsSphereWithRadius:andTessellation:
 */
class CC3SphereNode : public CC3MeshNode
{
public:
	CC3NodeBoundingVolume*		defaultBoundingVolume();
	static CC3SphereNode*		nodeWithName( const std::string& aName );
	static CC3SphereNode*		node();
};


/**
 * CC3ClipSpaceNode simplifies the creation of a simple rectangular node that can be used
 * in the clip-space of the view in order to cover the view with a rectangular image. This
 * provides an easy and convenient mechanism for creating backdrops and post-processing effects.
 *
 * Any mesh node can be configured for rendeirng in the clip-space by setting the shouldDrawInClipSpace
 * property is set to YES. This subclass is a convenience class that sets that property to YES during
 * instance initialization.
 *
 * See the notes of the shouldDrawInClipSpace property for further information about drawing
 * a node in clip-space.
 */
class CC3ClipSpaceNode : public CC3MeshNode
{
	DECLARE_SUPER( CC3MeshNode );
public:
	/**
	 * Allocates and initializes and autoreleased instance covered with the specified texture.
	 *
	 * This is a convenience method for a common use of this class.
	 */
	static CC3ClipSpaceNode*	nodeWithTexture( CC3Texture* texture );

	/**
	 * Allocates and initializes and autoreleased instance covered with the specified color.
	 *
	 * This is a convenience method for a common use of this class.
	 */
	static CC3ClipSpaceNode*	nodeWithColor( const ccColor4F& color );

	/**
	 * Allocates and initializes and autoreleased instance covered with the specified texture.
	 *
	 * The name property will be set to the specified name.
	 *
	 * This is a convenience method for a common use of this class.
	 */
	static CC3ClipSpaceNode*	nodeWithName( const std::string& name, CC3Texture* texture );

	/**
	 * Allocates and initializes and autoreleased instance covered with the specified color.
	 *
	 * The name property will be set to the specified name.
	 *
	 * This is a convenience method for a common use of this class.
	 */
	static CC3ClipSpaceNode*	nodeWithName( const std::string& name, const ccColor4F& color );
	static CC3ClipSpaceNode*	nodeWithName( const std::string& name );

	void						initWithTag( GLuint aTag, const std::string& aName );
	/** The camera frustum has no meaning in clip-space. */
	CC3NodeBoundingVolume*		defaultBoundingVolume();
};

/**
 * CC3Backdrop represents a simple full-view static backdrop that is rendered in clip-space.
 * The backdrop can be created as a solid color, or a texture, by using either the nodeWithColor:
 * or nodeWithTexture: method inherited from the CC3ClipSpaceNode superclass.
 *
 * See the class notes for the CC3ClipSpaceNode superclass, and the notes of the 
 * shouldDrawInClipSpace property for further information about drawing a node in clip-space.
 */
class CC3Backdrop : public CC3ClipSpaceNode
{
	DECLARE_SUPER( CC3ClipSpaceNode );
public:
	static CC3Backdrop*			node();
	/**
	 * Allocates and initializes and autoreleased instance covered with the specified texture.
	 *
	 * This is a convenience method for a common use of this class.
	 */
	static CC3Backdrop*			nodeWithTexture( CC3Texture* texture );

	/**
	 * Allocates and initializes and autoreleased instance covered with the specified color.
	 *
	 * This is a convenience method for a common use of this class.
	 */
	static CC3Backdrop*			nodeWithColor( const ccColor4F& color );

	static CC3Backdrop*			nodeWithName( const std::string& name, const ccColor4F& color );
	static CC3Backdrop*			nodeWithName( const std::string& name, CC3Texture* texture );
	static CC3Backdrop*			nodeWithName( const std::string& name );
};

/**
 * CC3WireframeBoundingBoxNode is a type of CC3LineNode specialized for drawing
 * a wireframe bounding box around another node. A CC3WireframeBoundingBoxNode
 * is typically added as a child node to the node whose bounding box is to
 * be displayed.
 *
 * The CC3WireframeBoundingBoxNode node can be set to automatically track
 * the dynamic nature of the boundingBox of the parent node by setting
 * the shouldAlwaysMeasureParentBoundingBox property to YES.
 *
 * Since we don't want to add descriptor labels or wireframe boxes to
 * wireframe nodes, the shouldDrawDescriptor, shouldDrawWireframeBox,
 * and shouldDrawLocalContentWireframeBox properties are overridden to
 * do nothing when set, and to always return YES.
 *
 * Similarly, CC3WireframeBoundingBoxNode node does not participate in calculating
 * the bounding box of the node whose bounding box it is drawing, since, as a child
 * of that node, it would interfere with accurate measurement of the bounding box.
 *
 * The shouldIncludeInDeepCopy property returns NO, so that the CC3WireframeBoundingBoxNode
 * will not be copied when the parent node is copied. A bounding box node for the copy
 * will be created automatically when each of the shouldDrawLocalContentWireframeBox
 * and shouldDrawWireframeBox properties are copied, if they are set to YES on the
 * original node that is copied.
 * 
 * A CC3WireframeBoundingBoxNode will continue to be visible even when its ancestor
 * nodes are invisible, unless the CC3WireframeBoundingBoxNode itself is made invisible.
 */
class CC3WireframeBoundingBoxNode : public CC3LineNode 
{
	DECLARE_SUPER( CC3LineNode );
public:
	/**
	 * Indicates whether the dimensions of this node should automatically be
	 * remeasured on each update pass.
	 *
	 * If this property is set to YES, the box will automatically be resized
	 * to account for movements by any descendant nodes of the parent node.
	 * For bounding box nodes that track the overall boundingBox of a parent
	 * node, this property should be set to YES.
	 *
	 * It is not necessary to set this property to YES to account for changes in
	 * the transform properties of the parent node itself, or if this node is
	 * tracking the bounding box of local content of the parent node. Generally,
	 * changes to that will automatically be handled by the transform updates.
	 *
	 * When setting this property, be aware that measuring the bounding box of
	 * the parent node can be an expensive operation.
	 *
	 * The initial value of this property is NO.
	 */
	virtual bool				shouldAlwaysMeasureParentBoundingBox();
	virtual void				setShouldAlwaysMeasureParentBoundingBox( bool alwaysMeasure );

	/**
	 * Updates this wireframe box from the bounding box of the parent node.
	 *
	 * The extent of the wireframe box is usually set automatically when first created, and is not
	 * automatically updated if the parent bounding box changes. If you want this wireframe to update
	 * automatically on each update frame, set the shouldAlwaysMeasureParentBoundingBox property to YES.
	 *
	 * However, updating on each frame can be a drag on performance, so if the parent bounding box
	 * changes under app control, you can invoke this method whenever the bounding box of the parent
	 * node changes to keep the wireframe box synchronized with its parent. 
	 */
	virtual void				updateFromParentBoundingBox();
	
	virtual bool				shouldIncludeInDeepCopy();
	virtual bool				shouldDrawDescriptor();
	virtual void				setShouldDrawDescriptor( bool shouldDraw );
	virtual bool				shouldDrawWireframeBox();
	virtual void				setShouldDrawWireframeBox( bool shouldDraw );
	virtual bool				shouldDrawLocalContentWireframeBox();
	virtual void				setShouldDrawLocalContentWireframeBox( bool shouldDraw );
	virtual bool				shouldContributeToParentBoundingBox();
	virtual bool				shouldDrawBoundingVolume();
	virtual void				setShouldDrawBoundingVolume( bool shouldDraw );
	/** Overridden so that not touchable unless specifically set as such. */
	virtual bool				isTouchable();
	/** Overridden so that can still be visible if parent is invisible, unless explicitly turned off. */
	virtual bool				isVisible();
	/** For wireframe lines, if material is created dynamically, make sure it ignores lighting. */
	CC3Material*				makeMaterial();
	virtual void				initWithTag( GLuint aTag, const std::string& aName );

	void						populateFrom( CC3WireframeBoundingBoxNode* another );
	virtual CCObject*			copyWithZone( CCZone* zone );

	void						releaseRedundantContent();
	/** If we should remeasure and update the bounding box dimensions, do so. */
	virtual void				updateAfterTransform( CC3NodeUpdatingVisitor* visitor );
	/** Measures the bounding box of the parent node and updates the vertex locations. */
	virtual void				updateFromParentBoundingBoxWithVisitor( CC3NodeUpdatingVisitor* visitor );
	/**
	 * Returns the parent's bounding box, or kCC3BoxZero if no parent,
	 * or if parent doesn't have a bounding box.
	 */
	virtual CC3Box				getParentBoundingBox();

	static CC3WireframeBoundingBoxNode*	nodeWithName( const std::string& aName );

protected:
	bool						_shouldAlwaysMeasureParentBoundingBox : 1;
};

/**
 * CC3WireframeLocalContentBoundingBoxNode is a CC3WireframeBoundingBoxNode that
 * further specializes in drawing a bounding box around the local content of another
 * node with local content. A CC3WireframeLocalContentBoundingBoxNode is typically
 * added as a child node to the node whose bounding box is to be displayed.
 *
 * Since for almost all nodes, the local content generally does not change, the
 * shouldAlwaysMeasureParentBoundingBox property is usually left at NO, to avoid
 * unnecessary remeasuring of the bounding box of the local content of the parent
 * node when we know it will not be changing. However, this property can be set to
 * YES when adding a CC3WireframeLocalContentBoundingBoxNode to a node whose local
 * content does change frequently.
 */
class  CC3WireframeLocalContentBoundingBoxNode : public CC3WireframeBoundingBoxNode
{
public:
	static CC3WireframeLocalContentBoundingBoxNode* nodeWithName( const std::string& aName );

	/**
	 * Overridden to return the parent's local content bounding box,
	 * or kCC3BoxZero if no parent, or if parent doesn't have a bounding box.
	 */
	CC3Box						getParentBoundingBox();
};

/**
 * CC3DirectionMarkerNode is a type of CC3LineNode specialized for drawing a line from the origin
 * of its parent node to a point outside the bounding box of the parent node, in a particular
 * direction. A CC3DirectionMarkerNode is typically added as a child node to the node to visibly
 * indicate the orientation of the parent node.
 *
 * The CC3DirectionMarkerNode node can be set to automatically track the dynamic nature of the
 * boundingBox of the parent node by setting the shouldAlwaysMeasureParentBoundingBox property to YES.
 *
 * Since we don't want to add descriptor labels or wireframe boxes to direction marker nodes, the
 * shouldDrawDescriptor, shouldDrawWireframeBox, and shouldDrawLocalContentWireframeBox properties
 * are overridden to do nothing when set, and to always return YES.
 *
 * Similarly, CC3DirectionMarkerNode node does not participate in calculating the bounding box of
 * the node whose bounding box it is drawing, since, as a child of that node, it would interfere
 * with accurate measurement of the bounding box.
 *
 * The shouldIncludeInDeepCopy property returns YES by default, so that the
 * CC3DirectionMarkerNode will be copied when the parent node is copied.
 * 
 * A CC3DirectionMarkerNode will continue to be visible even when its ancestor
 * nodes are invisible, unless the CC3DirectionMarkerNode itself is made invisible.
 */
class CC3DirectionMarkerNode : public CC3WireframeBoundingBoxNode 
{
	DECLARE_SUPER( CC3WireframeBoundingBoxNode );
public:
	/**
	 * Indicates the unit direction towards which this line marker will point from
	 * the origin of the parent node.
	 *
	 * When setting the value of this property, the incoming vector will be normalized to a unit vector.
	 *
	 * The value of this property defaults to kCC3VectorUnitZNegative, a unit vector
	 * in the direction of the negative Z-axis, which is the OpenGL ES default direction.
	 */
	CC3Vector					getMarkerDirection();
	void						setMarkerDirection( const CC3Vector& direction );

	/**
	 * Returns the proportional distance that the direction marker line should protrude from the parent
	 * node. This is measured in proportion to the distance from the origin of the parent node to the
	 * side of the bounding box through which the line is protruding.
	 *
	 * The initial value of this property is 1.5.
	 */
	static GLfloat				getDirectionMarkerScale();

	/**
	 * Sets the proportional distance that the direction marker line should protrude from the parent node.
	 * This is measured in proportion to the distance from the origin of the parent node to the side of
	 * the bounding box through which the line is protruding.
	 *
	 * The initial value of this property is 1.5.
	 */
	static void					setDirectionMarkerScale( GLfloat scale );

	/**
	 * Returns the minimum length of a direction marker line, expressed in the global
	 * coordinate system.
	 *
	 * Setting a value for this property can be useful for adding direction markers
	 * to very small nodes, or nodes that do not have volume, such as a camera or light.
	 *
	 * The initial value of this property is zero.
	 */
	static GLfloat				getDirectionMarkerMinimumLength();

	/**
	 * Sets the minimum length of a direction marker line, expressed in the global
	 * coordinate system.
	 *
	 * Setting a value for this property can be useful for adding direction markers
	 * to very small nodes, or nodes that do not have volume, such as a camera or light.
	 *
	 * The initial value of this property is zero.
	 */
	static void					setDirectionMarkerMinimumLength( GLfloat len );

	void						setParent( CC3Node* aNode );

	/** 
	 * Overridden to establish a default parent bounding box for parents that have no bounding
	 * box, such as cameras and lights. The default parent box is calculated as 5% of the size
	 * of the entire scene.
	 */
	CC3Box						getParentBoundingBox();

	void						initWithTag( GLuint aTag, const std::string& aName );

	void						populateFrom( CC3DirectionMarkerNode* another );
	virtual CCObject*			copyWithZone( CCZone* zone );

	/** Measures the bounding box of the parent node and updates the vertex locations. */
	void						updateFromParentBoundingBoxWithVisitor( CC3NodeUpdatingVisitor* visitor );

	/**
	 * Calculates the scale to use, along a single axis, for the length of the directional marker.
	 * Divide the distance from the origin, along this axis, to each of two opposite sides of the
	 * bounding box, by the length of the directional marker in this axis.
	 *
	 * Taking into consideration the sign of the direction, the real distance along this axis to
	 * the side it will intersect will be the maximum of these two values.
	 *
	 * Finally, in case the origin is on, or very close to, one side, make sure the length of the
	 * directional marker is at least 1/4 of the length of the distance between the two sides.
	 */
	GLfloat						calcScale( GLfloat markerAxis, GLfloat minBBAxis, GLfloat maxBBAxis );

	/**
	 * Calculate the end of the directonal marker line.
	 *
	 * This is done by calculating the scale we need to multiply the directional marker by to
	 * reach each of the three sides of the bounding box, then take the smallest of these,
	 * because that is the side it will intersect. Finally, multiply by an overall scale factor.
	 */
	CC3Vector					calculateLineEnd();

	static CC3DirectionMarkerNode* nodeWithName( const std::string& aName );
	
protected:
	CC3Vector					_markerDirection;
};

/**
 * CC3BoundingVolumeDisplayNode is a type of CC3MeshNode specialized for displaying
 * the bounding volume of its parent node. A CC3BoundingVolumeDisplayNode is typically
 * added as a child node to the node whose bounding volume is to be displayed.
 */
class CC3BoundingVolumeDisplayNode : public CC3MeshNode
{
	DECLARE_SUPER( CC3MeshNode );
public:
	/**
	 * Allocates and initializes an autoreleased instance with the specified name and an
	 * automatically generated unique tag value. The tag value is generated using a call to nextTag.
	 */
	static CC3BoundingVolumeDisplayNode* nodeWithName( const std::string& aName );

	/** Forces the color to always remain the same, even when the primary node is tinted to some other color. */
	void						setColor( const CCColorRef& color );
	
	/** 
	 * Limit the opacity of the bounding volume display, so it doesn't obscure the primary node,
	 * even when opacity of the parent is changed, as in a fade-in.
	 */
	void						setOpacity( CCOpacity opacity );
	bool						shouldIncludeInDeepCopy();
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
	// Overridden so that can still be visible if parent is invisible, unless explicitly turned off.
	bool						isVisible();
};

/**
 * CC3Fog is a mesh node that can render fog in the 3D scene.
 *
 * Typically, instances of this class are not generally used within the node assembly of a 
 * scene. Instead, a single instance of this class is used in the fog property of the CC3Scene.
 *
 * Fog color is controlled by the diffuseColor property.
 *
 * The style of attenuation imposed by the fog is set by the attenuationMode property.
 * See the notes of that property for information about how fog attenuates visibility.
 *
 * Using the performanceHint property, you can direct the GL engine to trade off between
 * faster or nicer rendering quality.
 *
 * Under OpenGL ES 1.1, fog is implemented as a direct feature of the GL engine, and this
 * class establishes the GL state for that fog.
 *
 * Under OpenGL versions that support GLSL, fog is rendered as a post-processing effect,
 * typically by rendering the scene to a surface that has both color and depth textures.
 * Add the color and depth textures from the scene-rendering surface to this node, and a
 * shader program that can render the node in clip-space, and provide fog effects. A good
 * choice is the combination of the CC3ClipSpaceTexturable.vsh vertex shader and the
 * CC3Fog.fsh fragment shader.
 */
class CC3Fog : public CC3MeshNode 
{
	DECLARE_SUPER( CC3MeshNode );
public:
	/**
	 * Indicates how the fog attenuates visibility with distance.
	 *
	 * The value of this property must be one of the following sybolic constants:
	 * GL_LINEAR, GL_EXP or GL_EXP2.
	 *
	 * When the value of this property is GL_LINEAR, the relative visibility of an object
	 * in the fog will be determined by the linear function ((e - z) / (e - s)), where
	 * s is the value of the start property, e is the value of the end property, and z is
	 * the distance of the object from the camera
	 *
	 * When the value of this property is GL_EXP, the relative visibility of an object in
	 * the fog will be determined by the exponential function e^(-(d - z)), where d is the
	 * value of the density property and z is the distance of the object from the camera.
	 *
	 * When the value of this property is GL_EXP2, the relative visibility of an object in
	 * the fog will be determined by the exponential function e^(-(d - z)^2), where d is the
	 * value of the density property and z is the distance of the object from the camera.
	 *
	 * The initial value of this property is GL_EXP2.
	 */
	GLenum						getAttenuationMode();
	void						setAttenuationMode( GLenum mode );

	/**
	 * Indicates how the GL engine should trade off between rendering quality and speed.
	 * The value of this property should be one of GL_FASTEST, GL_NICEST, or GL_DONT_CARE.
	 *
	 * The initial value of this property is GL_DONT_CARE.
	 */
	GLenum						getPerformanceHint();
	void						setPerformanceHint( GLenum hint );

	/**
	 * The density value used in the exponential functions. This property is only used
	 * when the attenuationMode property is set to GL_EXP or GL_EXP2.
	 *
	 * See the description of the attenuationMode for a discussion of how the exponential
	 * functions determine visibility.
	 *
	 * The initial value of this property is 1.0.
	 */
	GLfloat						getDensity();
	void						setDensity( GLfloat density );

	/**
	 * The distance from the camera, at which linear attenuation starts. Objects between
	 * this distance and the near clipping plane of the camera will be completly visible.
	 *
	 * This property is only used when the attenuationMode property is set to GL_LINEAR.
	 *
	 * See the description of the attenuationMode for a discussion of how the linear
	 * function determine visibility.
	 *
	 * The initial value of this property is 0.0.
	 */
	GLfloat						getStartDistance();
	void						setStartDistance( GLfloat startDistance );

	/**
	 * The distance from the camera, at which linear attenuation ends. Objects between
	 * this distance and the far clipping plane of the camera will be completely obscured.
	 *
	 * This property is only used when the attenuationMode property is set to GL_LINEAR.
	 *
	 * See the description of the attenuationMode for a discussion of how the linear
	 * function determine visibility.
	 *
	 * The initial value of this property is 1.0.
	 */
	GLfloat						getEndDistance();
	void						setEndDistance( GLfloat endDistance );

	/** Allocates and initializes an autoreleased instance. */
	static CC3Fog*				fog();

	virtual	bool				init();

	void						populateFrom( CC3Fog* another );
	virtual CCObject*			copyWithZone( CCZone* zone );

protected:
	GLenum						_attenuationMode;
	GLenum						_performanceHint;
	GLfloat						_density;
	GLfloat						_startDistance;
	GLfloat						_endDistance;
};

NS_COCOS3D_END

#endif
