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
#ifndef _CC3_LOCALCONTENT_NODE_H_
#define _CC3_LOCALCONTENT_NODE_H_

NS_COCOS3D_BEGIN
/**
 * CC3LocalContentNode is an abstract class that forms the basis for nodes
 * that have local content to draw.
 *
 * You can cause a wireframe box to be drawn around the local content of
 * the node by setting the shouldDrawLocalContentWireframeBox property to YES.
 * This can be particularly useful during development to locate the boundaries
 * of a node, or to locate a node that is not drawing properly.
 * You can set the default color of this wireframe using the class-side
 * defaultLocalContentWireframeBoxColor property.
 */
class CC3LocalContentNode : public CC3Node 
{
	DECLARE_SUPER( CC3Node );
public:
	CC3LocalContentNode();
	~CC3LocalContentNode();
	
	/**
	 * Returns the center of geometry of the local content of this node,
	 * in the local coordinate system of this node.
	 *
	 * If this node has no local content, returns the zero vector.
	 */
	virtual CC3Vector			getLocalContentCenterOfGeometry();

	/**
	 * Returns the center of geometry of the local content of this node,
	 * in the global coordinate system of the 3D scene.
	 *
	 * If this node has no local content, returns the value of the globalLocation property.
	 *
	 * The value of this property is calculated by transforming the value of the
	 * localContentCenterOfGeometry property, using the globalTransformMatrix of this node.
	 */
	virtual CC3Vector			getGlobalLocalContentCenterOfGeometry();

	/**
	 * Returns the smallest axis-aligned bounding box that surrounds the local
	 * content of this node, in the local coordinate system of this node.
	 *
	 * Returns kCC3BoxNull if this node has no local content.
	 */
	virtual CC3Box				getLocalContentBoundingBox();

	/**
	 * Returns the smallest axis-aligned bounding box that surrounds the local
	 * content of this node, in the global coordinate system of the 3D scene.
	 *
	 * If this node has no local content, returns kCC3BoxNull.
	 *
	 * The value of this property is calculated by transforming the eight vertices derived
	 * from the localContentBoundingBox property, using the globalTransformMatrix of this node,
	 * and constructing another bounding box that surrounds all eight transformed vertices.
	 *
	 * Since all bounding boxes are axis-aligned (AABB), if this node is rotated, the
	 * globalLocalContentBoundingBox will generally be significantly larger than the
	 * localContentBoundingBox.
	 */
	virtual CC3Box				getGlobalLocalContentBoundingBox();

	/**
	 * Returns the smallest axis-aligned bounding box that surrounds any local content of this node.
	 *
	 * The returned bounding box is specified in the coordinate system of the specified node,
	 * or in the global coordinate system of the 3D scene if the ancestor is nil.
	 *
	 * Returns kCC3BoxNull if this node has no local content.
	 *
	 * Since the bounding box of a node can change based on the locations, rotations, or
	 * scales of any descendant node, this property is measured dynamically on each access,
	 * by traversing all descendant nodes. This is a computationally expensive method.
	 */
	virtual CC3Box				getLocalContentBoundingBoxRelativeTo( CC3Node* ancestor );

	/**
	 * Checks that this node is in the correct drawing order relative to other nodes.
	 * This implementation forwards this notification up the ancestor chain to the CC3Scene,
	 * which checks if the node is correctly positioned in the drawing sequence, and
	 * repositions the node if needed.
	 *
	 * By default, nodes are automatically repositioned on each drawing frame to optimize
	 * the drawing order, so you should usually have no need to use this method.
	 * 
	 * However, in order to eliminate the overhead of checking each node during each drawing
	 * frame, you can disable this automatic behaviour by setting the allowSequenceUpdates
	 * property of specific drawing sequencers to NO.
	 *
	 * In that case, if you modify the properties of a node or its content, such as mesh or material
	 * opacity, and your CC3Scene drawing sequencer uses that criteria to sort nodes, you can invoke
	 * this method to force the node to be repositioned in the correct drawing order.
	 *
	 * You don't need to invoke this method when initially setting the properties.
	 * You only need to invoke this method if you modify the properties after the node has
	 * been added to the CC3Scene, either by itself, or as part of a node assembly.
	 */
	virtual void				checkDrawingOrder();

	/**
	 * Indicates whether the node should display a wireframe box around the local content
	 * of this node.
	 *
	 * This property is distinct from the inherited shouldDrawWireframeBox property.
	 * The shouldDrawWireframeBox property draws a wireframe that encompasses this node
	 * and any child nodes, where this property draws a wireframe that encompasses just
	 * the local content for this node alone. If this node has no children, then the two
	 * wireframes will surround the same volume.
	 *
	 * The wireframe box is drawn by creating and adding a CC3WireframeBoundingBoxNode as a child node
	 * to this node. The dimensions of the child node are set from the localContentBoundingBox
	 * property of this node. Setting this property to YES adds the wireframe child node, and
	 * setting this property to NO removes the wireframe child node.
	 *
	 * Setting this property to YES can be useful during development in determining the
	 * boundaries of the local drawn content of a node.
	 *
	 * The color of the wireframe box will be the value of the class-side
	 * defaultLocalContentWireframeBoxColor property, or the value of the color
	 * property of this node if defaultLocalContentWireframeBoxColor is equal
	 * to kCCC4FBlackTransparent.
	 */
	virtual bool				shouldDrawLocalContentWireframeBox();
	virtual void				setShouldDrawLocalContentWireframeBox( bool shouldDraw );

	/**
	 * If the shouldDrawLocalContentWireframeBox is set to YES, returns the child node that
	 * draws the wireframe around the local content of this node. Otherwise, returns nil.
	 */
	CC3WireframeBoundingBoxNode* getLocalContentWireframeBoxNode();

	/**
	 * Returns the color that local content wireframe bounding boxes will be drawn
	 * in when created using the shouldDrawLocalContentWireframeBox property.
	 *
	 * Setting this property to kCCC4FBlackTransparent will cause the color
	 * of any new local content wireframe bounding boxes to be set to the value
	 * of the color property of the node instead.
	 * 
	 * The initial value of this class property is kCCC4FMagenta.
	 */
	static ccColor4F			getLocalContentWireframeBoxColor();

	/**
	 * Sets the color that local content wireframes will be drawn in when created
	 * using the shouldDrawWireframeBox property.
	 *
	 * Changing this property will affect the color of any new local content wireframe
	 * bounding boxes created. It does not affect any instances that already have a
	 * wireframe bounding box established.
	 *
	 * Setting this property to kCCC4FBlackTransparent will cause the color
	 * of any new local content wireframe bounding boxes to be set to the value
	 * of the color property of the node instead.
	 * 
	 * The initial value of this class property is kCCC4FMagenta.
	 */
	static void					setLocalContentWireframeBoxColor( const ccColor4F& aColor );

	virtual bool				hasLocalContent();
	virtual GLint				getZOrder();
	virtual void				setZOrder( GLint zo );
	// Overridden to return the localContentBoundingBox if no children.
	virtual CC3Box				getBoundingBox();
	// Overridden to include local content
	virtual CC3Box				getBoundingBoxRelativeTo( CC3Node* ancestor );
	virtual void				initWithTag( GLuint aTag, const std::string& aName );

	void						populateFrom( CC3LocalContentNode* another );
	virtual CCObject*			copyWithZone( CCZone* zone );

	/** Overridden to force a lazy recalculation of the globalLocalContentBoundingBox. */
	virtual void				markTransformDirty();
	/** Overridden to return local content box color */
	virtual CCColorRef			getInitialDescriptorColor();
	/** The name to use when creating or retrieving the wireframe node of this node's local content. */
	std::string					getLocalContentWireframeBoxName();
	/** If default is transparent black, use the color of the node. */
	ccColor4F					getInitialLocalContentWireframeBoxColor();
	virtual bool				shouldDrawAllLocalContentWireframeBoxes();
	virtual void				setShouldDrawAllLocalContentWireframeBoxes( bool shouldDraw );
	virtual bool				shouldContributeToParentBoundingBox();

protected:
	CC3Box						m_globalLocalContentBoundingBox;
	GLint						m_zOrder;
};

NS_COCOS3D_END

#endif
