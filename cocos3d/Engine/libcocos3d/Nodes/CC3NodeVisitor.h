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

NS_COCOS3D_BEGIN
class CC3Node;
class CC3MeshNode;
class CC3Camera;
class CC3Light;
class CC3LightProbe;
class CC3ShaderProgram;
class CC3Material;
class CC3TextureUnit;
class CC3Mesh;

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

NS_COCOS3D_END

#endif
