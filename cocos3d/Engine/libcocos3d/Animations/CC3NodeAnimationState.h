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

#ifndef _CCL_CC3NODEANIMATIONSTATE_H_
#define _CCL_CC3NODEANIMATIONSTATE_H_

NS_COCOS3D_BEGIN

/**
 * CC3NodeAnimationState holds the state associated with the animation of a single node on a single track.
 *
 * Each node can participate in multiple tracks of animation, and during animation, these tracks
 * can be mixed to perform sophisticated animation blending techniques.
 *
 * Each instance of this class bridges a single CC3Node with an CC3NodeAnimation running on
 * on a particular track, and keeps track of the animation state on behalf of the node.
 */
class CC3NodeAnimation;
class CC3NodeAnimationState : public CCObject 
{
public:
	CC3NodeAnimationState();
	~CC3NodeAnimationState();

	/** The node whose animation state is being tracked by this instance.  */
	CC3Node*					getNode();
	void						setNode( CC3Node* node ); 

	/** The animation whose state is being tracked by this instance. */
	CC3NodeAnimation*			getAnimation();

	/** The animation track on which the animation runs. */
	GLuint						getTrackID();
	void						setTrackID( GLuint trackID );

	/** 
	 * The relative weight to use when blending this animation track with the other tracks.
	 * For each animation state in a node, this value can be set to a value between zero and one.
	 * During animation, the animated node properties (location, quaternion, scale) are derived
	 * from a weighted average of the contributions from each animation track, as determined by
	 * the relative weights assigned to each animation track, as specified by this property.
	 *
	 * For each track, the blending weight is relative to the blending weights of the other tracks,
	 * and the absolute values used for this property are unimportant. So, for instance, setting the
	 * value of this property to 0.2 on one track and 0.1 on another is equivalent to setting the value
	 * of this property to 1.0 and 0.5 respectively. In both cases, the first animation track will
	 * contribute twice the effect on the node's animated properties than the second animation track.
	 *
	 * It is important to understand that with multi-track animation, each animation track will
	 * contribute to the node's animated properties according to its weight even in the absence of
	 * a CC3ActionAnimate action running on that track. This is to ensure smooth transitions before and
	 * after a CC3ActionAnimate is run. To stop a track from contributing to the animated properties of
	 * the node, either set the value of this property to zero, or set the isEnabled property to NO.
	 *
	 * The initial value of this property is one.
	 */
	GLfloat						getBlendingWeight();
	void						setBlendingWeight( GLfloat weight );

	/**
	 * Indicates whether this animation is enabled, and will participate in animating the
	 * contained node if an animate action is run on the node.
	 *
	 * The initial value of this property is YES.
	 */
	bool						isEnabled();
	void						setIsEnabled( bool enable );

	/**
	 * Indicates whether animation of the location property of the node is enabled.
	 *
	 * The initial value of this property is YES. Setting this property to NO will disable animation
	 * of the node's location property, but will permit other properties to be animated.
	 */
	bool						isLocationAnimationEnabled();
	void						setIsLocationAnimationEnabled( bool enable );

	/**
	 * Indicates whether animation of the quaternion property of the node is enabled.
	 *
	 * The initial value of this property is YES. Setting this property to NO will disable animation
	 * of the node's quaternion property, but will permit other properties to be animated.
	 */
	bool						isQuaternionAnimationEnabled();
	void						setIsQuaternionAnimationEnabled( bool enable );
		
	/**
	 * Indicates whether animation of the scale property of the node is enabled.
	 *
	 * The initial value of this property is YES. Setting this property to NO will disable animation
	 * of the node's scale property, but will permit other properties to be animated.
	 */
	bool						isScaleAnimationEnabled();
	void						setIsScaleAnimationEnabled( bool enable );

	/** Sets the isEnabled property to YES. */
	void						enable();

	/** Sets the isEnabled property to NO. */
	void						disable();

	/**
	 * Returns the current animation time. This is the value submitted to the most recent invocation
	 * of the establishFrameAt: method, or zero if that method has not yet been invoked.
	 */
	float						getAnimationTime();
	void						setAnimationTime( float time );

	/**
	 * The current animated location.
	 *
	 * The value of this property is updated by the animation when the establishFrameAt: is invoked.
	 */
	CC3Vector					getLocation();
	void						setLocation( const CC3Vector& location );

	/**
	 * The current animated rotation quaternion.
	 *
	 * The value of this property is updated by the animation when the establishFrameAt: is invoked.
	 */
	CC3Quaternion				getQuaternion();
	void						setQuaternion( const CC3Quaternion& quaternion );

	/**
	 * The current animated scale.
	 *
	 * The value of this property is updated by the animation when the establishFrameAt: is invoked.
	 */
	CC3Vector					getScale();
	void						setScale( const CC3Vector& scale );

	/**
	 * The number of frames of animated content.
	 *
	 * The value of this property is retrieved from the same property on the contained animation instance.
	 */
	GLuint						getFrameCount();

	/** 
	 * Indicates whether the location property of the node is being animated. It is if both the
	 * isLocationAnimationEnabled property of this instance, and the isAnimatingLocation property
	 * of the contained animation, are set to YES.
	 */
	bool						isAnimatingLocation();

	/**
	 * Indicates whether the quaternion property of the node is being animated. It is if both the
	 * isQuaternionAnimationEnabled property of this instance, and the isAnimatingQuaternion property
	 * of the contained animation, are set to YES.
	 */
	bool						isAnimatingQuaternion();

	/**
	 * Indicates whether the scale property of the node is being animated. It is if both the
	 * isScaleAnimationEnabled property of this instance, and the isAnimatingScale property
	 * of the contained animation, are set to YES.
	 */
	bool						isAnimatingScale();

	/**
	 * Indicates whether any of the properties of the node are being animated. Returns YES if any of
	 * the isAnimatingLocation, isAnimatingQuaternion or isAnimatingScale properties returns YES.
	 */
	bool						isAnimating();

	/**
	 * Indicates whether the time interval between frames can vary from frame to frame, or whether
	 * the time interval between frames is constant across all frames.
	 *
	 * If this property returns NO, the frames of this animation are equally spaced in time.
	 *
	 * The value of this property is retrieved from the same property on the contained animation instance.
	 */
	bool						hasVariableFrameTiming();

	/**
	 * Updates the currentFrame, location, quaternion, and scale of this instance based on the
	 * animation content found in the contained animation at the specified time, which should
	 * be a value between zero and one, with zero indicating the first animation frame, and one
	 * indicating the last animation frame.
	 */
	void						establishFrameAt( float time );

	/**
	 * Initializes this instance tracking the animation state for the specified animation running on
	 * the specified track for the specified node.
	 *
	 * Returns nil if either the animation or the node are nil.
	 */
	void						initWithAnimation( CC3NodeAnimation* animation, GLuint trackID, CC3Node* node );

	/**
	 * Allocates and initializes an autoreleased instance tracking the animation state for the
	 * specified animation running on the specified track for the specified node.
	 *
	 * Returns nil if either the animation or the node are nil.
	 */
	static CC3NodeAnimationState*	animationStateWithAnimation( CC3NodeAnimation* animation, GLuint trackID, CC3Node* node );

	/**
	 * Returns the next available trackID value. The value returned is guaranteed to be different
	 * each time this method is invoked.
	 *
	 * When using multi-track animation in a node assembly, the trackID identifies a particular
	 * animation track within that node assembly. Since any particular track may only affect a
	 * few nodes within the entire node assembly, when adding a new animation track to the node
	 * assembly, it can be difficult to know how to select a track ID that will not conflict with
	 * any existing tracks within that node assembly. This method can be used to generate a unique
	 * track ID to use when adding a new track of animation to a node assembly.
	 */
	static GLuint				generateTrackID();


	/** Returns a description of the current state, including time and animated location, quaternion and scale. */
	std::string					describeCurrentState();

	/** Returns a description of the state at each of frameCount frames over the entire animation. */
	std::string					describeStateForFrames( GLuint frameCount );

	/**
	 * Returns a description of the state at each of frameCount frames between the specified
	 * start and end times, which should each be in the range between zero and one.
	 */
	std::string					describeStateForFrames( GLuint frameCount, float startTime, float endTime );

	std::string					description();

protected:
	void						markDirty();

protected:
	CC3Node*					m_pNode;
	CC3NodeAnimation*			m_pAnimation;
	float						m_fAnimationTime;
	CC3Vector					m_location;
	CC3Quaternion				m_quaternion;
	CC3Vector					m_scale;
	GLuint						m_trackID;
	GLfloat						m_fBlendingWeight;
	bool						m_isEnabled : 1;
	bool						m_isLocationAnimationEnabled : 1;
	bool						m_isQuaternionAnimationEnabled : 1;
	bool						m_isScaleAnimationEnabled : 1;
};

NS_COCOS3D_END

#endif
