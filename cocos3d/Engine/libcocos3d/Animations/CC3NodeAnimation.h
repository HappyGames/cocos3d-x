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

#ifndef _CCL_CC3NODEANIMATION_H_
#define _CCL_CC3NODEANIMATION_H_

NS_COCOS3D_BEGIN

class CC3NodeAnimationState;
/** 
 * A CC3NodeAnimation manages the animation of a node.
 *
 * An instance is held in the animation property of the node itself, and the node delegates
 * to its CC3NodeAnimation when the establishAnimationFrameAt: method is invoked on the node.
 *
 * Animations define animated content in in a series of frames (often called key-frames),
 * and can be configured to interpolate the animated state between these frames if necessary,
 * ensuring smooth animation, regardless of how many, or how widely spaced, the frames of
 * actual animated content are.
 *
 * A single CC3NodeAnimation instance can be shared by multiple nodes. This is a typical
 * situation when creating many copies of a node that is animated.
 *
 * CC3NodeAnimation is an abstract class. Subclasses define concrete animation data storage.
 */
class CC3NodeAnimation : public CCObject 
{
public:
	CC3NodeAnimation();
	virtual ~CC3NodeAnimation();
	/**
	 * The number of frames of animated content.
	 *
	 * This property indicates the number of frames for which animated content is available (often
	 * called key-frames). Because animations can be configured to interpolate between frames, it
	 * is quite common for the effective number of animated frames to be substantially higher than
	 * the number of frames of available animated content.
	 *
	 * As an extreme example, this property might indicate only two frames of animated content
	 * (a beginning and end state). If that animation played out over 10 seconds, it would interpolate
	 * several hundred "tween-frames", creating a smooth transition from the beginning to end state.
	 * More commonly, animations will specify a number of frames of content, to ensure sophisticated
	 * and realistic animation.
	 */
	virtual GLuint				getFrameCount();
	/**
	 * Indicates whether this animation should interpolate between frames, to ensure smooth
	 * transitions between frame content.
	 *
	 * The initial value of this property is YES.
	 */
	virtual bool				shouldInterpolate();
	virtual void				setShouldInterpolate( bool inter ); 

	
	/** Indicates whether location animated content is available and is enabled.  */
	virtual bool				isAnimatingLocation();

	/** Indicates whether rotation quaternion animated content is available and is enabled. */
	virtual bool				isAnimatingQuaternion();

	/** Indicates whether scale animated content is available and is enabled. */
	virtual bool				isAnimatingScale();

	/** 
	 * Indicates whether animation is enabled and any animated content (location, quaternion,
	 * or scale) is available and enabled.
	 */
	virtual bool				isAnimating();

	/**
	 * Indicates whether the time interval between frames can vary from frame to frame, or whether
	 * the time interval between frames is constant across all frames.
	 *
	 * If this property returns NO, the frames of this animation are equally spaced in time.
	 */
	virtual bool				hasVariableFrameTiming();
	
	/**
	 * Indicates a fractional value of a frame below which interpolation will not be performed.
	 *
	 * If an animation time is within this fraction above or below an exact frame time (relative to
	 * the previous or next frame), the frame time itself is used, instead of interpolating between
	 * that frame time and the next or previous frame time. This allows the animation to avoid an
	 * interpolation calculation if the difference will be unnoticable when compared to simply using
	 * the values for that specific frame.
	 *
	 * This value is specified as a fraction of a frame (between zero and one). The initial value is
	 * set at 0.1, indicating that no interpolation will be performed if the animation time is within
	 * 10% of the duration between the frame and the frame before or after it.
	 *
	 * Setting the value of this property to zero will cause interpolation to always be performed,
	 * and setting the value to one will cause interpolation to never be performed.
	 *
	 * The value of this class-side property affects all animation.
	 */
	static float				getInterpolationEpsilon();
	static void					setInterpolationEpsilon( float epsilon );

	/** Initializes this instance to animate with the specified number of animation frames. */
	virtual void				initWithFrameCount( GLuint numFrames );

	/**
	 * Allocates and initializes an autoreleased instance to animate
	 * with the specified number of animation frames.
	 */
	static CC3NodeAnimation*	animationWithFrameCount( GLuint numFrames );

	
	/**
	 * Updates the location, quaternion, and scale of the specified animation state based on the
	 * animation frame located at the specified time, which should be a value between zero and one,
	 * with zero indicating the first animation frame, and one indicating the last animation frame.
	 *
	 * Only those properties of the animation state for which there is animation data will be changed.
	 * If the shouldInterpolate property is set to YES, linear interpolation of the frame
	 * data is performed, based on the frameCount and the specified time.
	 */
	virtual void				establishFrameAt( float t, CC3NodeAnimationState* animState );

	/**
	 * Returns the time at which the frame at the specified index occurs. The returned time
	 * value will be between zero and one, where zero represents the time of the first frame
	 * and one represents the time of the last frame.
	 *
	 * This base implementation assumes a constant time between each frame and the next, so the
	 * returned value is calculated as (frameIndex / (frameCount - 1)), which is then clamped to
	 * the range between zero and one. Subclasses that allow variable times between frames will
	 * override to return the appropriate value.
	 */
	virtual float				timeAtFrame( GLuint frameIndex );

	/**
	 * Template method that returns the index of the frame within which the specified time occurs.
	 * The specified time will lie between the time of the animation frame at the returned index
	 * and the time of the animation frame following that frame.
	 */
	virtual GLuint				getFrameIndexAt( float t );

	/**
	 * Updates the location, quaternion, and scale of the specified node animation state based on the
	 * animation frame located at the specified frame, plus an interpolation amount towards the next frame.
	 */
	virtual void				establishFrame( GLuint frameIndex, GLfloat frameInterpolation, CC3NodeAnimationState* animState );

	/**
	 * Updates the location of the node animation state by interpolating between the animation
	 * content at the specified frame index and that at the next frame index, using the specified
	 * interpolation fraction value, which will be between zero and one.
	 * If frameInterpolation is zero, the lerp function will immediately return first frame.
	 */
	virtual void				establishLocationAtFrame( GLuint frameIndex, GLfloat frameInterpolation, CC3NodeAnimationState* animState );

	/**
	 * Updates the rotation quaternion of the node animation state by interpolating between the
	 * animation content at the specified frame index and that at the next frame index, using
	 * the specified interpolation fraction value, which will be between zero and one.
	 * If frameInterpolation is zero, the slerp function will immediately return first frame.
	 */
	virtual void				establishQuaternionAtFrame( GLuint frameIndex, GLfloat frameInterpolation, CC3NodeAnimationState* animState );

	/**
	 * Updates the scale of the node animation state by interpolating between the the animation
	 * content at the specified frame index and that at the next frame index, using the specified
	 * interpolation fraction value, which will be between zero and one.
	 * If frameInterpolation is zero, the lerp function will immediately return first frame.
	 */
	virtual void				establishScaleAtFrame( GLuint frameIndex, GLfloat frameInterpolation, CC3NodeAnimationState* animState );

	/**
	 * Template method that returns the location at the specified animation frame.
	 * Frame index numbering starts at zero.
	 *
	 * Default returns zero location. Subclasses with animation data should override.
	 * Subclasses should ensure that if frameIndex is larger than (frameCount - 1),
	 * the last frame will be returned.
	 */
	virtual CC3Vector			getLocationAtFrame( GLuint frameIndex );

	/**
	 * Template method that returns the location at the specified animation frame.
	 * Frame index numbering starts at zero.
	 *
	 * Default returns the identity quaternion. Subclasses with animation data should override.
	 * Subclasses should ensure that if frameIndex is larger than (frameCount - 1), the last
	 * frame will be returned.
	 */
	virtual CC3Quaternion		getQuaternionAtFrame( GLuint frameIndex );

	/**
	 * Template method that returns the scale at the specified animation frame.
	 * Frame index numbering starts at zero.
	 *
	 * Default returns unit scale. Subclasses with animation data should override.
	 * Subclasses should ensure that if frameIndex is larger than (frameCount - 1),
	 * the last frame will be returned.
	 */
	virtual CC3Vector			getScaleAtFrame( GLuint frameIndex );

protected:
	GLuint						_frameCount;
	bool						_shouldInterpolate : 1;
};

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
	CC3Vector					_location;
	CC3Quaternion				_quaternion;
	CC3Vector					_scale;
};

/**
 * A CC3NodeAnimationSegment plays a segment of the animation data contained within another
 * CC3NodeAnimation.
 *
 * An instance of CC3NodeAnimationSegment is constructed with a reference to an underlying
 * base animation, along with references to start and end times within that underlying animation.
 *
 * The CC3NodeAnimationSegment maps its standard zero-to-one animation range to the segment
 * of the base animation defined by the start and end times of the CC3NodeAnimationSegment.
 *
 * As an example, a CC3NodeAnimationSegment with a startTime of 0.2 and endTime of 0.5 maps
 * the full 0.0 - 1.0 animation range to the frames contained within the range of 0.2 - 0.5
 * in the base animation. In this case, requesting the animation of frames at times 0.0, 0.4
 * and 1.0 from the CC3NodeAnimationSegment instance will result in the animation of the
 * frames at times 0.2, 0.32, 0.5 from the base animation (0.32 = 0.2 + (0.5 - 0.2) * 0.4).
 *
 * The values of all read-only properties and methods are retrieved from the underlying base animation.
 */
class CC3NodeAnimationSegment : public CC3NodeAnimation 
{
	DECLARE_SUPER( CC3NodeAnimation );
public:
	CC3NodeAnimationSegment();
	~CC3NodeAnimationSegment();

	/** The CC3NodeAnimation containing the underlying animation data. */
	CC3NodeAnimation*			getBaseAnimation();

	/**
	 * The time within the underlying animation data that corresponds to the first frame of animation
	 * that will be animated by this instance.
	 *
	 * The value of this property must be between zero and one, with zero and one indicating the
	 * beginning and end of the underlying animation data, respectively.
	 *
	 * See the class notes for more information about how to set the values of the startTime and
	 * endTime properties to create an animation segment from the underlying animation data.
	 */
	float						getStartTime();
	void						setStartTime( float time );
		 
	/**
	 * The time within the underlying animation data that corresponds to the last frame of animation
	 * that will be animated by this instance.
	 *
	 * The value of this property must be between zero and one, with zero and one indicating the
	 * beginning and end of the underlying animation data, respectively.
	 *
	 * See the class notes for more information about how to set the values of the startTime and
	 * endTime properties to create an animation segment from the underlying animation data.
	 */
	float						getEndTime();
	void						setEndTime( float time );

	/**
	 * The index of the first frame that will be animated from the underlying animation data.
	 *
	 * The value of this property will be between zero and one less than the value of the frameCount
	 * property.
	 *
	 * This is a convenience property. Setting the value of this property sets the value of the 
	 * startTime property by determining the time of the frame in the underlying base animation 
	 * data corresponding to the frame index. The value derived depends on the number of frames
	 * of animation in the underlying animation data, and whether it has linear or variable frame
	 * timing. The use of this property makes most sense when the frame timing is linear (a constant
	 * time between each pair of consecutive frames).
	 *
	 * See the class notes for more information about how to set the values of the startTime and
	 * endTime properties to create an animation segment from the underlying animation data.
	 */
	GLuint						getStartFrameIndex();
	void						setStartFrameIndex( GLuint startFrameIndex );

	/**
	 * The index of the last frame that will be animated from the underlying animation data.
	 *
	 * The value of this property will be between zero and one less than the value of the frameCount
	 * property.
	 *
	 * This is a convenience property. Setting the value of this property sets the value of the
	 * endTime property by determining the time of the frame in the underlying base animation
	 * data corresponding to the frame index. The value derived depends on the number of frames
	 * of animation in the underlying animation data, and whether it has linear or variable frame
	 * timing. The use of this property makes most sense when the frame timing is linear (a constant
	 * time between each pair of consecutive frames).
	 *
	 * See the class notes for more information about how to set the values of the startTime and
	 * endTime properties to create an animation segment from the underlying animation data.
	 */
	GLuint						getEndFrameIndex();
	void						setEndFrameIndex( GLuint endFrameIndex );

	/** 
	 * Initializes this instance to animate a segment of the specified base animation.
	 *
	 * Initially, this animation will use the entire base animation. You can limit the range
	 * to a segment of the full animation by setting the startTime and endTime properties.
	 */
	void						initOnAnimation( CC3NodeAnimation* baseAnimation );

	/**
	 * Allocates and initializes an autoreleased instance to animate a segment of the specified
	 * base animation.
	 *
	 * Initially, this animation will use the entire base animation. You can limit the range
	 * to a segment of the full animation by setting the startTime and endTime properties.
	 */
	static CC3NodeAnimationSegment*	animationOnAnimation( CC3NodeAnimation* baseAnimation );

	/**
	 * Initializes this instance to animate a segment of the specified base animation, and with
	 * the startTime and endTime properties set to the specified value.
	 */
	void						initOnAnimation( CC3NodeAnimation* baseAnimation, float startTime, float endTime );

	/**
	 * Allocates and initializes an autoreleased instance to animate a segment of the specified
	 * base animation, and with the startTime and endTime properties set to the specified value.
	 */
	static CC3NodeAnimationSegment* animationOnAnimation( CC3NodeAnimation* baseAnimation, float startTime, float endTime );

	/**
	 * Initializes this instance to animate a segment of the specified base animation, and with
	 * the startFrameIndex and endFrameIndex properties set to the specified value.
	 */
	void						initOnAnimation( CC3NodeAnimation* baseAnimation, GLuint startFrameIndex, GLuint endFrameIndex );
	static CC3NodeAnimationSegment* animationOnAnimation( CC3NodeAnimation* baseAnimation, GLuint startFrameIndex, GLuint endFrameIndex );

	virtual GLuint				getFrameCount();
	virtual bool				shouldInterpolate();
	virtual void				setShouldInterpolate( bool _shouldInterpolate );

	virtual bool				isAnimatingLocation();
	virtual bool				isAnimatingQuaternion();
	virtual bool				isAnimatingScale();
	virtual bool				hasVariableFrameTiming();

	// Will raise assertion because base animation cannot be nil.
	virtual void				init();

	/**
	 * Overridden to interpret the specified time as within the range specified by the startTime
	 * and endTime properties, and then to retrieve the corresponding frame index from the base
	 * animation.
	 */
	virtual GLuint				getFrameIndexAt( float t );
	virtual float				timeAtFrame( GLuint frameIndex );
	virtual CC3Vector			getLocationAtFrame( GLuint frameIndex );
	virtual CC3Quaternion		getQuaternionAtFrame( GLuint frameIndex );
	virtual CC3Vector			getScaleAtFrame( GLuint frameIndex  );

protected:
	CC3NodeAnimation*			_baseAnimation;
	float						_startTime;
	float						_endTime;
};


/**
 * CC3NodeAnimationState holds the state associated with the animation of a single node on a single track.
 *
 * Each node can participate in multiple tracks of animation, and during animation, these tracks
 * can be mixed to perform sophisticated animation blending techniques.
 *
 * Each instance of this class bridges a single CC3Node with an CC3NodeAnimation running on
 * on a particular track, and keeps track of the animation state on behalf of the node.
 */
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
	CC3Node*					_node;
	CC3NodeAnimation*			_animation;
	float						_animationTime;
	CC3Vector					_location;
	CC3Quaternion				_quaternion;
	CC3Vector					_scale;
	GLuint						_trackID;
	GLfloat						_blendingWeight;
	bool						_isEnabled : 1;
	bool						_isLocationAnimationEnabled : 1;
	bool						_isQuaternionAnimationEnabled : 1;
	bool						_isScaleAnimationEnabled : 1;
};

NS_COCOS3D_END

#endif
