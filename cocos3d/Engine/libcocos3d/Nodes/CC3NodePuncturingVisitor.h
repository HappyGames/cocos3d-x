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
#ifndef _CCL_CC3NODE_PUNCTURING_VISITOR_H_
#define _CCL_CC3NODE_PUNCTURING_VISITOR_H_

NS_COCOS3D_BEGIN

class CC3Node;

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
	CC3Node*					m_pNode;
	CC3Vector					m_punctureLocation;
	CC3Vector					m_globalPunctureLocation;
	float						m_sqGlobalPunctureDistance;
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
	CCArray*					m_nodePunctures;
	CC3Ray						m_ray;
	bool						m_shouldPunctureFromInside : 1;
	bool						m_shouldPunctureInvisibleNodes : 1;
};

NS_COCOS3D_END

#endif
