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

#ifndef _CCL_CC3NODEANIMATIONSEGMENT_H_
#define _CCL_CC3NODEANIMATIONSEGMENT_H_

NS_COCOS3D_BEGIN

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
	CC3NodeAnimation*               getBaseAnimation();

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
	float                           getStartTime();
	void                            setStartTime( float time );
		 
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
	float                           getEndTime();
	void                            setEndTime( float time );

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
	GLuint                          getStartFrameIndex();
	void                            setStartFrameIndex( GLuint startFrameIndex );

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
	GLuint                          getEndFrameIndex();
	void                            setEndFrameIndex( GLuint endFrameIndex );

	/** 
	 * Initializes this instance to animate a segment of the specified base animation.
	 *
	 * Initially, this animation will use the entire base animation. You can limit the range
	 * to a segment of the full animation by setting the startTime and endTime properties.
	 */
	void                            initOnAnimation( CC3NodeAnimation* baseAnimation );

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
	void                            initOnAnimation( CC3NodeAnimation* baseAnimation, float startTime, float endTime );

	/**
	 * Allocates and initializes an autoreleased instance to animate a segment of the specified
	 * base animation, and with the startTime and endTime properties set to the specified value.
	 */
	static CC3NodeAnimationSegment* animationOnAnimation( CC3NodeAnimation* baseAnimation, float startTime, float endTime );

	/**
	 * Initializes this instance to animate a segment of the specified base animation, and with
	 * the startFrameIndex and endFrameIndex properties set to the specified value.
	 */
	void                            initOnAnimation( CC3NodeAnimation* baseAnimation, GLuint startFrameIndex, GLuint endFrameIndex );
	static CC3NodeAnimationSegment* animationOnAnimation( CC3NodeAnimation* baseAnimation, GLuint startFrameIndex, GLuint endFrameIndex );

	virtual GLuint                  getFrameCount();
	virtual bool                    shouldInterpolate();
	virtual void                    setShouldInterpolate( bool _shouldInterpolate );

	virtual bool                    isAnimatingLocation();
	virtual bool                    isAnimatingQuaternion();
	virtual bool                    isAnimatingScale();
	virtual bool                    hasVariableFrameTiming();

	// Will raise assertion because base animation cannot be nil.
	virtual void                    init();

	/**
	 * Overridden to interpret the specified time as within the range specified by the startTime
	 * and endTime properties, and then to retrieve the corresponding frame index from the base
	 * animation.
	 */
	virtual GLuint                  getFrameIndexAt( float t );
	virtual float                   timeAtFrame( GLuint frameIndex );
	virtual CC3Vector               getLocationAtFrame( GLuint frameIndex );
	virtual CC3Quaternion           getQuaternionAtFrame( GLuint frameIndex );
	virtual CC3Vector               getScaleAtFrame( GLuint frameIndex  );

protected:
	CC3NodeAnimation*               m_pBaseAnimation;
	float                           m_startTime;
	float                           m_endTime;
};

NS_COCOS3D_END

#endif
