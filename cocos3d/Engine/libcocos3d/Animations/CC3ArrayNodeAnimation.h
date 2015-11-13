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

#ifndef _CCL_CC3ARRAYNODEANIMATION_H_
#define _CCL_CC3ARRAYNODEANIMATION_H_

NS_COCOS3D_BEGIN

/** 
 * A concrete CC3NodeAnimation that holds animation data in simple arrays.
 * The arrays can be allocated and managed either by the instance, or externally.
 *
 * There are three properties that hold the animated content:
 *   - animatedLocations - location animation content
 *   - animatedQuaternions - rotation quaternion animation content
 *   - animatedScales - scale animation content
 *
 * You do not need to use all of these properties. You can choose to animate any subset of
 * these animation data properties, and leave the remaining animation data properties set to
 * NULL (the default). If you do not set an animation data property, the corresponding property
 * on the node will not be animated, and will retain its originally set value.
 *
 * For example, if you set only the animatedLocations property, and run a CC3ActionAnimate on the node,
 * only the location of the node will move around during the animation. The remaining node
 * properties (quaternion & scale) will remain unchanged by the animation. The effect will be
 * that the node moves around, but remains at a fixed size, and oriented in a fixed rotation.
 *
 * This animation can be configured so that time interval between frames can vary from frame
 * to frame, or that the time interval between frames is constant. To configure for variable
 * frame timing, use the frameTimes property to assign a specific time to each frame. To
 * configure for equally-spaced frames, set the frameTimes property to NULL.
 *
 * You can work with these animation content properties in one of two ways:
 *   - Allocate the arrays outside this class and simply assign them to this instance using the
 *     property accessors. In this case, it is up to you to allocate and deallocate the memory
 *     used by the arrays.
 *   - Invoke one or more of the methods allocateFrameTimes, allocateLocations, allocateQuaternions,
 *     and allocateScales to instruct this instance to allocate and manage the memory for the content
 *     array. You can then access the associated array via the frameTimes, animatedLocations,
 *     animatedQuaternions, and animatedScales properties respectively. This instance will take
 *     care of releasing the arrays when appropriate.
 */
class CC3ArrayNodeAnimation : public CC3NodeAnimation 
{
	DECLARE_SUPER( CC3NodeAnimation );
public:
	CC3ArrayNodeAnimation();
	~CC3ArrayNodeAnimation();
	/**
	 * An array of frame times. Each CCTime in the array indicates the time for one frame. All values
	 * must be within the range from zero and one inclusive. For accurate animation, the value of
	 * the first element of this array should be zero, and the value of the last element should be one.
	 * The array must have at least frameCount elements.
	 *
	 * This property can be set to NULL to indicate that the duration of all of the frames is the same.
	 *
	 * The hasVariableFrameTiming property will return YES if this property is not NULL, and NO otherwise.
	 *
	 * Setting this property will safely free any memory allocated by the allocateFrameTimes method.
	 *
	 * The initial value of this property is NULL, indicating that the frames are equally spaced.
	 */
	 float*						getFrameTimes();
	 void						setFrameTimes( float* frameTimes );

	/**
	 * An array of animated location content. Each CC3Vector in the array holds the location content
	 * for one frame of animation. The array must have at least frameCount elements.
	 *
	 * This property can be set to NULL to indicate that the location is not animated.
	 *
	 * The isAnimatingLocation property will return YES if this property is not NULL, and NO otherwise.
	 *
	 * Setting this property will safely free any memory allocated by the allocateLocations method.
	 *
	 * The initial value of this property is NULL.
	 */
	CC3Vector*					getAnimatedLocations();
	void						setAnimatedLocations( CC3Vector* animatedLocations ); 

	/**
	 * An array of animated rotation quaternion content. Each CC3Quaternion in the array holds the
	 * rotation content for one frame of animation. The array must have at least frameCount elements.
	 *
	 * This property can be set to NULL to indicate that the rotation is not animated.
	 *
	 * The isAnimatingQuaternion property will return YES if this property is not NULL, and NO otherwise.
	 *
	 * Setting this property will safely free any memory allocated by the allocateQuaternions method.
	 *
	 * The initial value of this property is NULL.
	 */
	CC3Quaternion*				getAnimatedQuaternions();
	void						setAnimatedQuaternions( CC3Quaternion* quaternions );

	/**
	 * An array of animated scale content. Each CC3Vector in the array holds the scale content
	 * for one frame of animation. The array must have at least frameCount elements.
	 *
	 * This property can be set to NULL to indicate that the scale is not animated.
	 *
	 * The isAnimatingScale property will return YES if this property is not NULL, and NO otherwise.
	 *
	 * Setting this property will safely free any memory allocated by the allocateScales method.
	 *
	 * The initial value of this property is NULL.
	 */
	CC3Vector*					getAnimatedScales();
	void						setAnimatedScales( CC3Vector* scales );

	/**
	 * Allocates underlying memory for an array of frame times.
	 * All elements of the array are initialized to zero.
	 * The amount of memory allocated will be (frameCount * sizeof(CCTime)) bytes.
	 *
	 * It is safe to invoke this method more than once, but understand that any previously
	 * allocated memory will be safely freed prior to the allocation of the new memory.
	 * The memory allocated earlier will therefore be lost and should not be referenced.
	 */
	float*						allocateFrameTimes();

	/**
	 * Allocates underlying memory for an array of location vectors.
	 * All elements of the array are initialized to zero location.
	 * The amount of memory allocated will be (frameCount * sizeof(CC3Vector)) bytes.
	 *
	 * It is safe to invoke this method more than once, but understand that any previously
	 * allocated memory will be safely freed prior to the allocation of the new memory.
	 * The memory allocated earlier will therefore be lost and should not be referenced.
	 */
	CC3Vector*					allocateLocations();

	/**
	 * Allocates underlying memory for an array of quaternions vectors.
	 * All elements of the array are initialized to the identity quaternion.
	 * The amount of memory allocated will be (frameCount * sizeof(CC3Quaternion)) bytes.
	 *
	 * It is safe to invoke this method more than once, but understand that any previously
	 * allocated memory will be safely freed prior to the allocation of the new memory.
	 * The memory allocated earlier will therefore be lost and should not be referenced.
	 */
	CC3Quaternion*				allocateQuaternions();

	/**
	 * Allocates underlying memory for an array of scale vectors.
	 * All elements of the array are initialized to unity scale.
	 * The amount of memory allocated will be (frameCount * sizeof(CC3Vector)) bytes.
	 *
	 * It is safe to invoke this method more than once, but understand that any previously
	 * allocated memory will be safely freed prior to the allocation of the new memory.
	 * The memory allocated earlier will therefore be lost and should not be referenced.
	 */
	CC3Vector*					allocateScales();

	/**
	 * Deallocates the underlying frame times array allocated with the allocateFrameTimes method.
	 * It is safe to invoke this method more than once, or even if allocateFrameTimes was not
	 * previously invoked.
	 *
	 * This method is invoked automatically when this instance is deallocated.
	 */
	void						deallocateFrameTimes();

	/**
	 * Deallocates the underlying location array allocated with the allocateLocations method.
	 * It is safe to invoke this method more than once, or even if allocateLocations was not
	 * previously invoked.
	 *
	 * This method is invoked automatically when this instance is deallocated.
	 */
	void						deallocateLocations();

	/**
	 * Deallocates the underlying quaternion array allocated with the allocateQuaternions method.
	 * It is safe to invoke this method more than once, or even if allocateQuaternions was not
	 * previously invoked.
	 *
	 * This method is invoked automatically when this instance is deallocated.
	 */
	void						deallocateQuaternions();

	/**
	 * Deallocates the underlying scale array allocated with the allocateScales method.
	 * It is safe to invoke this method more than once, or even if allocateScales was not
	 * previously invoked.
	 *
	 * This method is invoked automatically when this instance is deallocated.
	 */
	void						deallocateScales();

	virtual bool				isAnimatingLocation();
	virtual bool				isAnimatingQuaternion();
	virtual bool				isAnimatingScale();
	virtual bool				hasVariableFrameTiming();
	virtual void				initWithFrameCount( GLuint numFrames );

	// All times should be in range between zero and one
	virtual float				timeAtFrame( GLuint frameIndex );

	// Iterate backwards through the frames looking for the first frame whose time is at or before
	// the specified frame time, and return that frame. If the specified frame is before the first
	// frame, return the first frame.
	virtual GLuint				getFrameIndexAt( float t );

	virtual CC3Vector			getLocationAtFrame( GLuint frameIndex );
	virtual CC3Quaternion		getQuaternionAtFrame( GLuint frameIndex );
	virtual CC3Vector			getScaleAtFrame( GLuint frameIndex );

protected:
	float*						_frameTimes;
	CC3Vector*					_animatedLocations;
	CC3Quaternion*				_animatedQuaternions;
	CC3Vector*					_animatedScales;
	bool						_frameTimesAreRetained : 1;
	bool						_locationsAreRetained : 1;
	bool						_quaternionsAreRetained : 1;
	bool						_scalesAreRetained : 1;
};

NS_COCOS3D_END

#endif
