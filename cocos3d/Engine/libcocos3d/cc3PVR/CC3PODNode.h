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
#ifndef _CC3_NODEPODEXTENSIONS_H_
#define _CC3_NODEPODEXTENSIONS_H_

NS_COCOS3D_BEGIN

// The stride for each type of animation content
#define kPODAnimationLocationStride 3
#define kPODAnimationQuaternionStride 4
#define kPODAnimationScaleStride 7

class CC3PODNode : public CC3Node
{
	DECLARE_SUPER( CC3Node );
public:
	virtual GLint				getPodIndex();
	virtual void				setPodIndex( GLint index );

	/**
	 * The index of the POD data that forms the type-specific content of this node.
	 *
	 * This is distinct from the podIndex property, which is the index of the data
	 * for the node, which may be of any node type. Once the type is established,
	 * the type-specific content is indexed by the podContentIndex property.
	 *
	 * This abstract implementation does not map this property to an instance
	 * variable, and always returns kCC3PODNilIndex. Concrete subclasses must
	 * override to map to an actual instance variable.
	 */
	virtual GLint				getPodContentIndex();
	virtual void				setPodContentIndex( GLint index );

	/**
	 * The index of the parent node of this node.
	 * This will be -1 if this node has no parent.
	 *
	 * This abstract implementation does not map this property to an instance
	 * variable, and always returns kCC3PODNilIndex. Concrete subclasses must
	 * override to map to an actual instance variable.
	 */
	virtual GLint				getPodParentIndex();
	virtual void				setPodParentIndex( GLint index );

	/**
	 * Allocates and initializes an autoreleased instance from the data of
	 * this type at the specified index within the specified POD resource.
	 */
	static CC3PODNode*			nodeAtIndex( GLint aPODIndex, CC3PODResource* aPODRez );

	void						populateFrom( CC3PODNode* another );
	virtual CCObject*			copyWithZone( CCZone* zone );

	std::string					description();

	/**
	 * Adds the animation contained in the specified POD file to this node and all its descendants.
	 * The animation is added as the specified track.
	 *
	 * If the specified POD file has already been loaded, it is retrieved from the resource cache.
	 * If the POD file has not been loaded, it will be loaded and placed in the resource cache.
	 */
	virtual void				addAnimationFromPODFile( const std::string& podFilePath, GLuint trackID );

	/**
	 * Adds the animation contained in the specified POD file to this node and all its descendants.
	 * The animation is added in a new track, whose ID is returned from this method.
	 *
	 * If the specified POD file has already been loaded, it is retrieved from the resource cache.
	 * If the POD file has not been loaded, it will be loaded and placed in the resource cache.
	 */
	virtual GLuint				addAnimationFromPODFile( const std::string& podFilePath );
	
	/**
	 * Instantiates an instance of CC3PODResourceNode, loads it from the POD file at
	 * the specified path, and adds the CC3PODResourceNode instance as a child node
	 * to this CC3Scene instance.
	 *
	 * The specified file path may be either an absolute path, or a path relative to the
	 * application resource directory. If the file is located directly in the application
	 * resources directory, the specified file path can simply be the name of the file.
	 * 
	 * The name of the resource node will be that of the file.
	 *
	 * If this method is being invoked from a background thread (ie- not the main rendering thread)
	 * AND this node is already part of a scene, this operation will automatically be deferred and
	 * queued onto the main operation queue, to be performed on teh main rendering thread prior to
	 * the next rendeirng cycle. This is to ensure that the node is not added while the scene is
	 * being rendered, to avoid race conditions.
	 *
	 * In this situation, subsequent code executed on the background thread should not rely on the
	 * specified node, or any of its descendants, having been added to the receiver or the scene.
	 * For example, invoking this method on the background thread, followed by getNodeNamed: to the
	 * receiver (or any of its ancestors), will almost certainly return nil, because this method
	 * will have been deferred to the main thread, and will, almost certainly, not have been run by
	 * the time the subsequent getNodeNamed: method is run on the background thread.
	 */
	virtual void				addContentFromPODFile( const std::string& aFilepath );

	/**
	 * Instantiates an instance of CC3PODResourceNode with the specified name, loads it
	 * from the POD file at the specified path, and adds the CC3PODResourceNode instance
	 * as a child node to this CC3Scene instance.
	 *
	 * The specified file path may be either an absolute path, or a path relative to the
	 * application resource directory. If the file is located directly in the application
	 * resources directory, the specified file path can simply be the name of the file.
	 *
	 * If this method is being invoked from a background thread (ie- not the main rendering thread)
	 * AND this node is already part of a scene, this operation will automatically be deferred and
	 * queued onto the main operation queue, to be performed on teh main rendering thread prior to
	 * the next rendeirng cycle. This is to ensure that the node is not added while the scene is
	 * being rendered, to avoid race conditions.
	 *
	 * In this situation, subsequent code executed on the background thread should not rely on the
	 * specified node, or any of its descendants, having been added to the receiver or the scene.
	 * For example, invoking this method on the background thread, followed by getNodeNamed: to the
	 * receiver (or any of its ancestors), will almost certainly return nil, because this method
	 * will have been deferred to the main thread, and will, almost certainly, not have been run by
	 * the time the subsequent getNodeNamed: method is run on the background thread.
	 */
	virtual void				addContentFromPODFile( const std::string& aFilepath, const std::string& aName );

protected:
	GLint						_podIndex;
	GLint						_podContentIndex;
	GLint						_podParentIndex;
};

/** 
 * POD files can contain information to animate the nodes.
 * A CC3PODNodeAnimation instance manages the animation of a single node.
 * It is held by the node itself, in the animation property, and is activated
 * when the establishAnimationFrameAt: method is invoked on the node.
 */
class CC3PODNodeAnimation : public CC3NodeAnimation 
{
	DECLARE_SUPER( CC3NodeAnimation );
public:
	CC3PODNodeAnimation();
	~CC3PODNodeAnimation();
	/**
	 * Initializes this instance to animate nodes using animation data found in the specified
	 * SPODNode structure, containing the specified number of animation frames.
	 *
	 * Usually it's only worth instantiating an instance of this class if
	 * the SPODNode actually contains animation data. This can be checked
	 * with the sPODNodeDoesContainAnimation: class method.
	 */
	void						initFromSPODNode( PODStructPtr pSPODNode, GLuint numFrames );

	/**
	 * Allocates and initializes an autoreleased instance to animate nodes using animation
	 * data found in the specified SPODNode structure, containing the specified number of
	 * animation frames.
	 *
	 * Usually it's only worth instantiating an instance of this class if
	 * the SPODNode actually contains animation data. This can be checked
	 * with the sPODNodeDoesContainAnimation: class method.
	 */
	static CC3PODNodeAnimation* animationFromSPODNode( PODStructPtr pSPODNode, GLuint numFrames );

	/** Returns whether the specified SPODNode structure contains animation data. */
	static bool					sPODNodeDoesContainAnimation( PODStructPtr pSPODNode );

	virtual bool				isAnimatingLocation();
	virtual bool				isAnimatingQuaternion();
	virtual bool				isAnimatingScale();
	virtual CC3Vector			getLocationAtFrame( GLuint frameIndex );
	virtual CC3Quaternion		getQuaternionAtFrame( GLuint frameIndex );
	virtual CC3Vector			getScaleAtFrame( GLuint frameIndex );

protected:
	GLuint*						_animatedLocationIndices;
	GLfloat*					_animatedLocations;			// 3 floats per frame of animation.
	GLuint*						_animatedQuaternionsIndices;
	GLfloat*					_animatedQuaternions;			// 4 floats per frame of animation.
	GLuint*						_animatedScaleIndices;
	GLfloat*					_animatedScales;				// 7 floats per frame of animation.
};


NS_COCOS3D_END

#endif
