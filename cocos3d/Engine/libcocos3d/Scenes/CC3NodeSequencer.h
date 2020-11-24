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
#ifndef _CC3NODE_SEQUENCER_H_
#define _CC3NODE_SEQUENCER_H_

NS_COCOS3D_BEGIN
class CC3Scene;
class CC3NodeSequencerVisitor;
/**
 * A CC3NodeEvaluator performs some type of accept/reject evaluation on a CC3Node instance.
 * The type of evaluation performed is determined by the subclass of CC3NodeEvaluator.
 * A wide range of subclasses may be constructed to perform a variety of evaluations.
 *
 * The central evaluation method evaluate: returns YES or NO, indicating whether the
 * evaluator accepts or rejects the node.
 */
class CC3NodeEvaluator : public CCObject
{
public:
	virtual void				init();
	/**
	 * Performs the evaluation defined by this class on the specified node and returns
	 * YES if the node is accepted, or NO if it is rejected.
	 * 
	 * This default implementation always returns NO. Subclasses will override.
	 */
	virtual bool				evaluate( CC3Node* aNode );

	/** Allocates and initializes an autoreleased instance. */
	static CC3NodeEvaluator*	evaluator();

	/**
	 * Template method that populates this instance from the specified other instance.
	 *
	 * This method is invoked automatically during object copying via the copy or
	 * copyWithZone: method. In most situations, the application should use the
	 * copy method, and should never need to invoke this method directly.
	 * 
	 * Subclasses that add additional instance state (instance variables) should extend
	 * copying by overriding this method to copy that additional state. Superclass that
	 * override this method should be sure to invoke the superclass implementation to
	 * ensure that superclass state is copied as well.
	 */
	void						populateFrom( CC3NodeEvaluator* another );
	virtual CCObject*			copyWithZone( CCZone* zone );
};

/** A type of CC3NodeEvaluator that accepts all nodes by always returning YES from the evaluate: method. */
class CC3NodeAcceptor : public CC3NodeEvaluator
{
public:
	/** Allocates and initializes an autoreleased instance. */
	static CC3NodeAcceptor*		evaluator();

	virtual bool				evaluate( CC3Node* aNode );
};

/** A type of CC3NodeEvaluator that rejects all nodes by always returning NO from the evaluate: method. */
class CC3NodeRejector : public CC3NodeEvaluator
{
public:
	/** Allocates and initializes an autoreleased instance. */
	static CC3NodeRejector*		evaluator();
};

/**
 * A type of CC3NodeEvaluator that specializes in evaluating only CC3Nodes with local content.
 * 
 * The evalute: method checks the hasLocalContent property of the node. If the node does not
 * have local content, the evaluate: method returns NO indicating that the evaluation failed.
 *
 * If the node does have local content, the node is cast to an instance of CC3LocalContentNode
 * and passed to the evaluateLocalContentNode: for further evaluation.
 */
class CC3LocalContentNodeEvaluator : public CC3NodeEvaluator
{
public:
	/** Allocates and initializes an autoreleased instance. */
	static CC3LocalContentNodeEvaluator* evaluator();
    
	virtual bool				evaluate( CC3Node* aNode );
	/**
	 * Performs the evaluation defined by this class on the specified node, which must be a type
	 * of CC3LocalContentNode, and returns YES if the node is accepted, or NO if it is rejected.
	 *
	 * This default implementation simply returns NO, meaning that all CC3LocalContentNodes
	 * will be rejected. Since all other nodes have also been rejected by the evaluate: method
	 * prior to invoking this method, the effect of this class is to reject all nodes.
	 */
	virtual bool				evaluateLocalContentNode( CC3LocalContentNode* lcNode );
};

/**
 * A type of CC3LocalContentNodeEvaluator that accepts all nodes with local content,
 * and rejects all other nodes.
 */
class CC3LocalContentNodeAcceptor : public CC3LocalContentNodeEvaluator
{
public:
	static CC3LocalContentNodeAcceptor* evaluator();

	virtual bool				evaluateLocalContentNode( CC3LocalContentNode* lcNode );
};

/**
 * A type of CC3LocalContentNodeEvaluator that accepts only opaque nodes.
 * To do this, the evaluateLocalContentNode: method returns YES if the isOpaque
 * property of the CC3LocalContentNode instance is YES.
 */
class CC3OpaqueNodeAcceptor : public CC3LocalContentNodeEvaluator
{
public:
	static CC3OpaqueNodeAcceptor* evaluator();

	virtual bool				evaluateLocalContentNode( CC3LocalContentNode* lcNode );
};

/**
 * A type of CC3LocalContentNodeEvaluator that accepts only translucent nodes.
 * To do this, the evaluateLocalContentNode: method returns YES if the isOpaque
 * property of the CC3LocalContentNode instance is NO.
 */
class CC3TranslucentNodeAcceptor : public CC3LocalContentNodeEvaluator
{
public:
	static CC3TranslucentNodeAcceptor* evaluator();

	virtual bool				evaluateLocalContentNode( CC3LocalContentNode* lcNode );
};

/**
 * A CC3NodeSequencer instance organizes nodes that are added to it. The node sequencer
 * contains a CC3NodeEvaluator to determine whether it is interested in a node when an
 * attempt is made to add the node. Only nodes that are accepted the evaluator will be
 * added to the sequencer.
 *
 * The type of sequencing performed is determined by the subclass of CC3NodeSequencer.
 * A wide range of subclasses may be constructed to perform a variety of sequencing techniques.
 */
class CC3NodeSequencer : public CCObject 
{
public:
	CC3NodeSequencer();
	virtual ~CC3NodeSequencer();
	/**
	 * The evaluator that determines whether a node should be added to this sequencer.
	 * If no evaluator is attached to this sequencer, no nodes will be added.
	 */
	CC3NodeEvaluator*			getEvaluator();
	void						setEvaluator( CC3NodeEvaluator* evaluator );

	/**
	 * Returns an array of the nodes that have been added to this sequencer,
	 * ordered as this sequencer defines.
	 *
	 * The returned array is a copy of the any internal arrays.
	 * Changing the contents will not change the internal node seqeunce.
	 */
	virtual CCArray*			getNodes();

	/**
	 * Indicates that the sequencer will run the algorithm to relocate misplaced nodes
	 * when the updateSequenceWithVisitor: method is invoked. Setting this property to NO means
	 * that when updateSequenceWithVisitor: is invoked, on this or a parent sequencer, no attempt
	 * will be made to move misplaced nodes in this sequencer to their correct drawing
	 * sequence position.
	 *
	 * Initially, this property is set to YES to ensure nodes are always in their correct
	 * drawing sequence position, to avoid unexpected visual artifacts.
	 *
	 * However, the updateSequenceWithVisitor: method is invoked on each drawing frame, and checks
	 * each drawable node. You may find performance improvements by setting this property to
	 * NO on some sequencers, if you know that the nodes contained in a particular sequencer
	 * will not be moved out of that sequencer, or re-sorted within that sequencer, and you want
	 * to save the overhead of checking each node on each drawing frame.
	 *
	 * If you have set this property to NO, you can still force a node to be re-positioned to its
	 * correct drawing sequence position by invoking the checkDrawingOrder method on the node.
	 */
	virtual bool				allowSequenceUpdates();
	virtual void				setAllowSequenceUpdates( bool allow );

	/**
	 * For sequencers that order nodes based on distance to the camera, indicates whether,
	 * when comparing distances from the nodes to the camera, only the distance component
	 * that is parallel to the camera's forwardDirection should be considered.
	 *
	 * If the value of this property is NO, nodes will be sorted based on the true 3D
	 * straight-line distance from each node to the camera, as if drawing a measuring tape
	 * from the location of the camera to the location of the center of geometry of the node.
	 * This is the most common 3D scenario.
	 *
	 * If the value of this property is YES, nodes will be sorted based on the shortest
	 * distance from the camera to a plane that is perpendicular to the forwardDirection
	 * of the camera and contains the location of the node. This has the effect of sorting
	 * nodes based on their distance "straight-out" from the camera, ignoring distance
	 * contributed by nodes that are "off to the side" of the camera's view. This option
	 * is good for scenes that are built from large planar nodes that move in layers at
	 * fixed distances from a fixed camera, similar to cell-animation techniques.
	 *
	 * The initial value for this property is NO, indicating that the true 3D distance
	 * between the camera and the center of geometry of the node will be used to determine
	 * drawing order. Unless your 3D scene is using special cell-animation techniques with
	 * large planar nodes, you should not change the value of this property.
	 *
	 * In this default abstract implmentation, the value returned is always returned as NO,
	 * and values set in this property are ignored. Subclasses that sort based on Z-order,
	 * and subclasses that contain such other sequencers will override.
	 */
	virtual bool				shouldUseOnlyForwardDistance();
	virtual void				setShouldUseOnlyForwardDistance( bool shouldUse );

	/**
	 * Allocates and initializes an autoreleased instance with no evaluator.
	 * This sequencer will not accept any nodes until an evaluator is attached.
	 */
	static CC3NodeSequencer*	sequencer();

	/** Initializes this instance with the specified evaluator. */
	virtual void				initWithEvaluator( CC3NodeEvaluator* anEvaluator );

	/** Allocates and initializes an autoreleased instance with the specified evaluator. */
	static CC3NodeSequencer*	sequencerWithEvaluator( CC3NodeEvaluator* anEvaluator );

	/**
	 * Template method that populates this instance from the specified other instance.
	 *
	 * This method is invoked automatically during object copying via the copy or
	 * copyWithZone: method. In most situations, the application should use the
	 * copy method, and should never need to invoke this method directly.
	 * 
	 * Subclasses that add additional instance state (instance variables) should extend
	 * copying by overriding this method to copy that additional state. Superclass that
	 * override this method should be sure to invoke the superclass implementation to
	 * ensure that superclass state is copied as well.
	 */
	void						populateFrom( CC3NodeSequencer* another );
	virtual CCObject*			copyWithZone( CCZone* zone );

	/**
	 * Adds the specified node to this sequencer if the node is accepted by the
	 * contained evaluator. If the node is rejected by the evaluator, it is not added.
	 * Returns whether the node was added.
	 */
	virtual bool				add( CC3Node* aNode, CC3NodeSequencerVisitor* visitor );

	/**
	 * Removes the specified node, if it exists within this sequencer,
	 * and returns whether it was removed.
	 */
	virtual bool				remove( CC3Node* aNode, CC3NodeSequencerVisitor* visitor );

	/**
	 * Invokes the identifyMisplacedNodesWithVisitor: method on this sequencer to
	 * look for nodes that are "misplaced", then removes and re-adds those misplaced
	 * nodes back into this sequencer, so that they can be inserted into their correct
	 * sequence position.
	 *
	 * This method is invoked automatically from the CC3Scene on each drawing frame.
	 * The application should never need to invoke this method directly.
	 */
	virtual bool				updateSequenceWithVisitor( CC3NodeSequencerVisitor* visitor );

	/**
	 * If the allowSequenceUpdates is set to YES, looks for nodes that are misplaced in
	 * this sequencer, and adds them to the misplacedNodes property of the specified visitor. 
	 *
	 * What it means for a node to be "misplaced" is defined by the sequencer subclass.
	 * A sequencer subclass may determine that the node no longer meets the criteria
	 * of the sequencer's evaluator, or that the node is now out of order, relative to
	 * the sorting or grouping criteria defined by the sequencer.
	 *
	 * The default behaviour is to do nothing. Subclasses will override as necessary.
	 *
	 * This method is invoked automatically by the updateSequenceWithVisitor: method.
	 * The application should never need to invoke this method directly.
	 */
	virtual void				identifyMisplacedNodesWithVisitor( CC3NodeSequencerVisitor* visitor );

	/**
	 * Visits the nodes contained in this node sequencer with the specified node visitor.
	 * The nodes are visited in the order that they are sequenced by this node sequencer.
	 *
	 * Note that the argument is a CC3NodeVisitor, not a CC3NodeSequencerVisitor as with
	 * other methods on this class.
	 *
	 * The default implementation does nothing. Subclasses that contain nodes, or contain
	 * other sequencers that contain nodes, will override.
	 */
	virtual void				visitNodesWithNodeVisitor( CC3NodeVisitor* aNodeVisitor );

	/** Returns a string containing a more complete description of this object. */
	std::string					fullDescription();

	void						init();

protected:
	CC3NodeEvaluator*			m_pEvaluator;
	bool						m_allowSequenceUpdates : 1;
};

/**
 * An CC3BTreeNodeSequencer is a type of CC3NodeSequencer that separates nodes into a
 * B-tree structure of child sequencers.
 *
 * When a node is added, it is first evaluated by the contained evaluator. If it is accepted,
 * the sequencer iterates through the contained child sequencers, in the order that the
 * child sequencers were added, attempting to add the node to each child sequencer in turn.
 * The node is added to the first child sequencer that accepts it.
 *
 * Instances of CC3BTreeNodeSequencer can be used to group nodes by some parent criteria
 * while allowing the nodes to be further grouped within each child grouping.
 *
 * Setting the property shouldUseOnlyForwardDistance sets the same value in each child sequencer.
 * Reading that property returns YES if any child sequencer returns YES, otherwise it returns NO.
 */
class CC3BTreeNodeSequencer : public CC3NodeSequencer 
{
	DECLARE_SUPER( CC3NodeSequencer );
public:
	CC3BTreeNodeSequencer();
	virtual ~CC3BTreeNodeSequencer();

	/** The array of child sequencers. */
	CCArray*					getSequencers();

	/** Adds the specified sequencer as a child sequencer. */
	void						addSequencer( CC3NodeSequencer* aNodeSequencer );

	virtual CCArray*			getNodes();

	/**
	 * Allocates and initializes an autoreleased instance that accepts only nodes that have
	 * local content to draw, and sequences them so that all the opaque nodes appear before
	 * all the translucent nodes.
	 * 
	 * The opaque nodes are sorted in the order they are added. The translucent nodes are
	 * sorted by their distance from the camera, from furthest from the camera to closest.
	 */
	static CC3BTreeNodeSequencer* sequencerLocalContentOpaqueFirst();

	/**
	 * Allocates and initializes an autoreleased instance that accepts only nodes that have
	 * local content to draw, and sequences them so that all the opaque nodes appear before
	 * all the translucent nodes.
	 * 
	 * The opaque nodes are grouped by texture, so that all nodes with the same texture
	 * appear together. The translucent nodes are sorted by their distance from the camera,
	 * from furthest from the camera to closest.
	 */
	static CC3BTreeNodeSequencer* sequencerLocalContentOpaqueFirstGroupTextures();

	/**
	 * Allocates and initializes an autoreleased instance that accepts only nodes that have
	 * local content to draw, and sequences them so that all the opaque nodes appear before
	 * all the translucent nodes.
	 * 
	 * The opaque nodes are grouped by mesh, so that all nodes with the same mesh appear
	 * together. The translucent nodes are sorted by their distance from the camera, from
	 * furthest from the camera to closest.
	 */
	static CC3BTreeNodeSequencer* sequencerLocalContentOpaqueFirstGroupMeshes();
	static CC3BTreeNodeSequencer* sequencerWithEvaluator( CC3NodeEvaluator* anEvaluator );

	void						initWithEvaluator( CC3NodeEvaluator* anEvaluator );

	void						populateFrom( CC3BTreeNodeSequencer* another );
	virtual CCObject*			copyWithZone( CCZone* zone );

	/** Iterates through the sequencers, adding it to the first one that accepts the node. */
	virtual bool				add( CC3Node* aNode, CC3NodeSequencerVisitor* visitor );
	/** Iterates through the sequencers, asking each to remove the node. */
	virtual bool				remove( CC3Node* aNode, CC3NodeSequencerVisitor* visitor );
	/** Iterates through the sequencers, collecting misplaced nodes in the visitor. */
	virtual void				identifyMisplacedNodesWithVisitor( CC3NodeSequencerVisitor* visitor );
	virtual void				visitNodesWithNodeVisitor( CC3NodeVisitor* aNodeVisitor );

	virtual bool				shouldUseOnlyForwardDistance();
	virtual void				setShouldUseOnlyForwardDistance( bool onlyForward );

protected:
	CCArray*					m_sequencers;
};


/**
 * An CC3NodeArraySequencer is a type of CC3NodeSequencer that arranges nodes into an
 * array, and orders the nodes in the array by some criteria.
 *
 * When a node is added, it is first evaluated by the contained evaluator. If it is
 * accepted, the sequencer iterates through the existing nodes that it holds, invoking
 * the template method shouldInsertNode:between:and:withVisitor: on each pair of
 * sequential existing nodes, looking for the place to insert the new node. The node
 * is inserted the first time that template method returns YES. If not suitable
 * insertion point is found, the node is added to the end of the array.
 *
 * This base class simply arranges the nodes in the order they are presented, by always
 * adding to the end of the contained array of nodes. Subclasses will customize the way
 * that the nodes are ordered and grouped in the array.
 *
 * The contents of the nodes array are not copied when this sequencer is copied.
 */
class CC3NodeArraySequencer : public CC3NodeSequencer 
{
	DECLARE_SUPER( CC3NodeSequencer );
public:
	CC3NodeArraySequencer();
	~CC3NodeArraySequencer();

	/** Allocates and initializes an autoreleased instance with the specified evaluator. */
	static CC3NodeArraySequencer*	sequencerWithEvaluator( CC3NodeEvaluator* anEvaluator );
	/**
	 * Attempts to insert the specified node between two specified nodes that already exist
	 * in the array, and returns whether it was inserted at that location.
	 * 
	 * This default implementation always returns NO, resulting in each node always being added
	 * to the end of the array.
	 */
	virtual bool				shouldInsertNode( CC3Node* aNode, CC3Node* leftNode, CC3Node* rightNode, CC3NodeSequencerVisitor* visitor );

	virtual CCArray*			getNodes();

	/** Initializes this instance with the specified evaluator. */
	virtual void				initWithEvaluator( CC3NodeEvaluator* anEvaluator );

	/**
	 * Iterates through the existing nodes, passing them as sequential pairs, along with the node
	 * to be added, to the shouldInsertNode:between:and:withVisitor: template method. If that method
	 * returns YES, the node is inserted into the array at that point. If that method never returns
	 * YES, the node is added at the end of the array. Returns whether the node was added.
	 */
	virtual bool				add( CC3Node* aNode, CC3NodeSequencerVisitor* visitor );
	virtual bool				remove( CC3Node* aNode, CC3NodeSequencerVisitor* visitor );

	virtual void				identifyMisplacedNodesWithVisitor( CC3NodeSequencerVisitor* visitor );
	virtual void				visitNodesWithNodeVisitor( CC3NodeVisitor* aNodeVisitor );

protected:
	CCArray*					m_nodes;
};

/**
 * An CC3NodeArrayZOrderSequencer is a type of CC3NodeArraySequencer that sorts
 * the contained nodes by their Z-order, which is a combination of the explicit
 * Z-order property of each node, and a measure of the distance from the camera
 * to the globalCenterOfGravity of the node's bounding volume.
 *
 * Use this sequencer for translucent nodes. There is no need to use this sequencer
 * for nodes that are opaque (whose isOpaque property returns YES), and the overhead
 * of testing each node on each update should be avoided in that case.
 *
 * The nodes are sorted using the Z-order property and the cameraDistanceProduct
 * property of the boundingVolume of each node, from furthest from the camera to
 * closest. Nodes without a boundingVolume are added to the end of the array.
 *
 * Explicit Z-order sequence takes priority over distance to camera. However,
 * sorting based on distance to the camera alone is quite effective. In almost all
 * cases, it is not necessary to set the Z-order property of the nodes, and if the
 * nodes are moving around, assigning an explicit Z-order to each node can actually
 * interfere with the dynamic determination of the correct drawing order. Only use
 * the Z-order property if you have reason to force a specific node to be drawn
 * before or after another node for visual effect.
 *
 * The distance between a node and the camera can be measured in one of two ways:
 *   -# The true 3D straight-line distance between the node and the camera.
 *   -# The distance from the camera to the node measured "straight out" from the
 *      camera, ignoring how far the node is away from the center of the camera's view.
 *
 * The value of the shouldUseOnlyForwardDistance property determines which of these two
 * methods will be used. See the notes of that property in the CC3NodeSequencer for more
 * information. By default, the true 3D distance is used.
 *
 * Since all nodes, and the camera, can move around on each update, this sequencer will
 * test and re-order its nodes on each update.
 *
 * Be careful about setting the allowSequenceUpdates property to NO on this sequencer.
 * Since this sequencer will generally only be used to keep translucent nodes in their
 * correct drawing order, setting allowSequenceUpdates to NO will defeat the purpose,
 * and will result in translucent nodes not properly displaying other translucent
 * objects that are behind them.
 */
class CC3NodeArrayZOrderSequencer : public CC3NodeArraySequencer 
{
	DECLARE_SUPER( CC3NodeArraySequencer );
public:
	virtual void				initWithEvaluator( CC3NodeEvaluator* anEvaluator );
	void						populateFrom( CC3NodeArrayZOrderSequencer* another );
	virtual CCObject*			copyWithZone( CCZone* zone );

	/**
	 * If either the Z-order of the node of interest is greater than the Z-order of the rightNode,
	 * or the Z-order is equal, and the distance from the node of interest to the camera is greater
	 * than the distance from the rightNode to the camera, return YES, otherwise return NO.
	 * 
	 * Since the array is traversed from front to back, the node will have already been tested
	 * against the leftNode. Nodes without a boundingVolume are added to the end of the array.
	 */
	virtual bool				shouldInsertNode( CC3Node* aNode, CC3Node* leftNode, CC3Node* rightNode, CC3NodeSequencerVisitor* visitor );
	/**
	 * Identify nodes that do not pass the evaluator or are not in the correct Z-order sequence.
	 * Any node whose distance to the camera is greater than the nodes before it is considered misplaced.
	 */
	virtual void				identifyMisplacedNodesWithVisitor( CC3NodeSequencerVisitor* visitor );

protected:
    bool						m_shouldUseOnlyForwardDistance;
};

/**
 * An CC3MeshNodeArraySequencer is a type of CC3NodeArraySequencer that only accepts
 * mesh nodes, in addition to whatever other evaluation criteria is set by the
 * evaluator property. This is a convenience class that allows many mesh-oriented
 * subclasses to be easily created.
 *
 * For subclass convenience, since this sequencer only accepts mesh nodes, the
 * implementation of the template method shouldInsertNode:between:and:withVisitor:
 * casts the nodes to CC3MeshNode and delegates to the
 * shouldInsertMeshNode:between:and:withVisitor: method.
 *
 * This base class simply arranges the nodes in the order they are presented.
 * Subclasses will customize the way that the nodes are ordered and grouped in the array.
 */
class CC3MeshNodeArraySequencer : public CC3NodeArraySequencer
{
	DECLARE_SUPER( CC3NodeArraySequencer );
public:
	/**
	 * Attempts to insert the specified node between two specified nodes that already exist
	 * in the array, and returns whether it was inserted at that location.
	 * 
	 * This default implementation always returns NO, resulting in each node always being added
	 * to the end of the array.
	 */
	virtual bool				shouldInsertMeshNode( CC3MeshNode* aNode, CC3MeshNode* leftNode, CC3MeshNode* rightNode, CC3NodeSequencerVisitor* visitor );

	virtual bool				add( CC3Node* aNode, CC3NodeSequencerVisitor* visitor );

	/**
	 * Nodes are always instances of CC3MeshNode, so cast them as such
	 * and invoke the shouldInsertMeshNode:between:and:withVisitor: template method.
	 */
	virtual bool				shouldInsertNode( CC3Node* aNode, CC3Node* leftNode, CC3Node* rightNode, CC3NodeSequencerVisitor* visitor );
};

/**
 * An CC3MeshNodeArraySequencerGroupTextures is a type of CC3MeshNodeArraySequencer that
 * groups together nodes that are using the same texture.
 */
class CC3MeshNodeArraySequencerGroupTextures : public CC3MeshNodeArraySequencer
{
public:
	bool						shouldInsertMeshNode( CC3MeshNode* aNode, CC3MeshNode* leftNode, CC3MeshNode* rightNode, CC3NodeSequencerVisitor* visitor );
};

/**
 * An CC3MeshNodeArraySequencerGroupTextures is a type of CC3MeshNodeArraySequencer that
 * groups together nodes that are using the same mesh.
 */
class CC3MeshNodeArraySequencerGroupMeshes : public CC3MeshNodeArraySequencer
{
public:
	virtual bool				shouldInsertMeshNode( CC3MeshNode* aNode, CC3MeshNode* leftNode, CC3MeshNode* rightNode, CC3NodeSequencerVisitor* visitor );
};

/**
 * This visitor is used to visit CC3NodeSequencers to perform operations on nodes
 * within the sequencers.
 *
 * The visitor maintains a reference to the CC3Scene, so that the sequencer may
 * use aspects of the scene during operations.
 *
 * This visitor can be used to visit CC3NodeSequencers to detect and keep track of
 * nodes that are misplaced within the sequencer, using the updateSequenceWithVisitor:
 * method on the sequencer.
 *
 * What it means for a node to be "misplaced" is defined by the sequencer itself.
 * A sequencer may determine that the node no longer meets the criteria of the
 * sequencer's evaluator, or that the node is now out of order, relative to the
 * sorting or grouping criteria defined by the sequencer.
 *
 * A sequencer visitor can either be instantiated for a single visitation of a sequencer,
 * or can be instantiated once and reused to visit different sequencers over and over.
 * In doing so, you should invoke the reset method on the sequencer visitor prior to
 * using it to visit a sequencer.
 */
class CC3NodeSequencerVisitor : public CCObject 
{
public:
	CC3NodeSequencerVisitor();
	virtual ~CC3NodeSequencerVisitor();

	/**
	 * The CC3Scene instance. The sequencer may use aspects of the scene when
	 * performing sequencing operations with a node.
	 */
	CC3Scene*					getScene();
	void						setScene( CC3Scene* scene );

	/** Initializes this instance with the specified CC3Scene. */
	void						initWithScene( CC3Scene* aCC3Scene );

	/** Allocates and initializes an autoreleased instance with the specified CC3Scene. */
	static CC3NodeSequencerVisitor* visitorWithScene( CC3Scene* aCC3Scene );

	/** Indicates whether the misplacedNodes property contains nodes. */
	bool						hasMisplacedNodes();

	/**
	 * Returns an array of nodes that the sequencer deems to be misplaced after
	 * being visited by this visitor.
	 *
	 * The returned array may be nil.
	 */
	CCArray*					getMisplacedNodes();

	/** Adds the specified node to the array of nodes held in the misplacedNodes property */
	void						addMisplacedNode( CC3Node* aNode );

	/** Clears the misplacedNodes array. */
	void						clearMisplacedNodes();

	void						init();

protected:
	CC3Scene*					m_pScene;
	CCArray*					m_misplacedNodes;
};


NS_COCOS3D_END

#endif
