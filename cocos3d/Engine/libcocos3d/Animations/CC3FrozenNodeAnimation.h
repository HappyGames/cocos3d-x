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

#ifndef _CCL_CC3FROZENNODEANIMATION_H_
#define _CCL_CC3FROZENNODEANIMATION_H_

NS_COCOS3D_BEGIN

/**
 * A concrete CC3NodeAnimation that holds animation a single, frozen animation frame.
 *
 * A node containing a CC3FrozenNodeAnimation will have its location, quaternion, and scale
 * properties frozen to the values of the corresponding properties of this instance, and
 * every frame of animation will use the same values.
 *
 * This freezing behaviour is different than if the node had no animation at all. A node with
 * no animation content can have its location, quaternion, and scale properties freely set,
 * even while animation is running. By contrast, while an animation is running on the node
 * containing instance of CC3FrozenNodeAnimation, the values of the location, quaternion,
 * and scale properties will each be locked to a single value.
 *
 * Instances of this class can be useful if a node is not really animated, but you want to
 * ensure that, when a particular animation is playing on a node assembly, the node is forced
 * to a particular location, rotation, and scale.
 *
 * You do not need to use all of these animation properties. If you don't want to force an
 * animation component to a particular value, set the corresponding property to a null value
 * (kCC3VectorNull or kCC3QuaternionNull). The corresponding isAnimatingLocation, 
 * isAnimatingQuaternion or isAnimatingScale will thereafter return NO.
 *
 * The frameCount property will always return 1. The shouldInterpolate property is ignored.
 */
class CC3FrozenNodeAnimation : public CC3NodeAnimation 
{
	DECLARE_SUPER( CC3NodeAnimation );
public:
	/**
	 * A single location to which the node will be frozen throughout the animation.
	 *
	 * If you don't want to force the node to a particular location during the animation, set this
	 * property to kCC3VectorNull. The corresponding isAnimatingLocation property will thereafter
	 * return NO, and the location of the node will be left unchanged during animation.
	 *
	 * The initial value of this property is kCC3VectorNull.
	 */
	CC3Vector					getLocation();
	void						setLocation( const CC3Vector& location ); 

	/**
	 * A single rotation quaterion to which the node will be frozen throughout the animation.
	 *
	 * If you don't want to force the node to a particular rotation during the animation, set this
	 * property to kCC3QuaternionNull. The corresponding isAnimatingQuaternion property will thereafter
	 * return NO, and the rotation of the node will be left unchanged during animation.
	 *
	 * The initial value of this property is kCC3QuaternionNull.
	 */
	CC3Quaternion				getQuaternion();
	void						setQuaternion( const CC3Quaternion& quaternion );

	/**
	 * A single scale to which the node will be frozen throughout the animation.
	 *
	 * If you don't want to force the node to a particular scale during the animation, set this
	 * property to kCC3VectorNull. The corresponding isAnimatingScale property will thereafter
	 * return NO, and the scale of the node will be left unchanged during animation.
	 *
	 * The initial value of this property is kCC3VectorNull.
	 */
	CC3Vector					getScale();
	void						setScale( const CC3Vector& scale );

	/**
	 * Populates the location, quaternion and scale properties from the current values of the 
	 * corresponding properties of the specfied node.
	 */
	void						populateFromNodeState( CC3Node* aNode );

	/** Allocates and initializes an instance with null location, quaternion and scale properties. */
	static CC3FrozenNodeAnimation* animation();

	/**
	 * Initializes this instance with location, quaternion and scale properties set
	 * from the current values of the corresponding properties of the specfied node.
	 */
	void						initFromNodeState( CC3Node* aNode );

	/** 
	 * Allocates and initializes an instance with location, quaternion and scale properties set
	 * from the current values of the corresponding properties of the specfied node. 
	 */
	static CC3FrozenNodeAnimation*	animationFromNodeState( CC3Node* aNode ); 

	virtual bool				isAnimatingLocation();
	virtual bool				isAnimatingQuaternion();
	virtual bool				isAnimatingScale();
	virtual void				establishFrameAt( float t, CC3NodeAnimationState* animState );
	virtual void				init();
	virtual void				initWithFrameCount( GLuint numFrames );

protected:
	CC3Vector					m_location;
	CC3Quaternion				m_quaternion;
	CC3Vector					m_scale;
};

NS_COCOS3D_END

#endif
