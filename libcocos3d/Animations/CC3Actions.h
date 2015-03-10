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

#ifndef _CCL_CC3_ACTIONS_H_
#define _CCL_CC3_ACTIONS_H_
#include "../Nodes/CC3Node.h"
#include "actions/CCActionEase.h"

NS_COCOS3D_BEGIN

/**
 * Constants for use as action tags to identify an action of a particular type on a node.
 *
 * Assigning a tag to an action allows one type of action on a node to be stopped, while allowing
 * other actions on that node to continue. For instance, a node might be both moving and fading
 * in concurrently. If the movement is altered by a user interaction, it might be desirable to
 * stop the movement action and replace it, without affecting the fade action. Using a tag to
 * identify the movement action allows it to be retrieved and stopped (via stopActionByTag:)
 * without affecting the fade action.
 *
 * You can use the CC3Node convenience method  to stop any existing action on a node with a
 * particular tag, assign the tag to the new action, and run that action instead.
 */
typedef enum {							// Don't start at zero to avoid possible confusion with defaults or other action tags
	kCC3ActionTagAnimation = 314,		/**< Use for animation that may combine move, rotate, and scale type actions. */
	kCC3ActionTagAnimationBlending,		/**< Use for changes to animation track blending. */
	kCC3ActionTagMove,					/**< Use for movement type actions. */
	kCC3ActionTagRotation,				/**< Use for rotation type actions. */
	kCC3ActionTagScale,					/**< Use for scaling type actions. */
	kCC3ActionTagFade,					/**< Use for fading type actions. */
	kCC3ActionTagTint,					/**< Use for general tinting type actions. */
	kCC3ActionTagTintAmbient,			/**< Use for tinting ambient color type actions. */
	kCC3ActionTagTintDiffuse,			/**< Use for tinting diffuse color type actions. */
	kCC3ActionTagTintSpecular,			/**< Use for tinting specular color type actions. */
	kCC3ActionTagTintEmission,			/**< Use for tinting emission color type actions. */
} CC3ActionTag;

class CC3Action : public CCActionInterval
{
public:
	CC3Action() { setUserData( (void*)1 ); }
	virtual void				startWithTarget( CC3Node* pNode ) {  }
	virtual CC3Node*			getTargetNode() { return NULL; }
};

class CC3ActionInterval : public CC3Action
{
public:
	virtual void				initWithDuration( float d );
	CCAction*					repeatForever();
	CC3Node*					getTargetNode();
	virtual void				startWithTarget( CC3Node* pNode );
	virtual CC3ActionInterval*	reverse();

protected:
	CC3Node*					m_pTargetCC3Node;
};

class CC3ActionTransformVector : public CC3ActionInterval
{
	DECLARE_SUPER( CC3ActionInterval );
public:
	virtual void				initWithDuration( float t, const CC3Vector& vector );

	CCObject*					copyWithZone( CCZone* pZone );
	virtual CC3ActionInterval*	reverse();
	void						startWithTarget( CC3Node* aTarget );
	virtual CC3Vector			getTargetVector();
	virtual void				setTargetVector( const CC3Vector& aVector );
	void						update( float t );

	static CC3ActionTransformVector* actionWithDuration( float t, const CC3Vector& aVector );

protected:
	CC3Vector					m_startVector;
	CC3Vector					m_diffVector;
};

class CC3ActionTransformBy : public CC3ActionTransformVector
{

};

class CC3ActionMoveBy : public CC3ActionTransformBy
{
public:
	virtual void				initWithDuration( float t, const CC3Vector& translation );
	static CC3ActionMoveBy*		actionWithDuration( float t, const CC3Vector& aTranslation );
	CC3Vector					getTargetVector();
	void						setTargetVector( const CC3Vector& aLocation );
};

class CC3ActionRotateBy : public CC3ActionTransformBy
{
public:
	virtual void				initWithDuration( float t, const CC3Vector& rotation );
	static CC3ActionRotateBy*	actionWithDuration( float t, const CC3Vector& aRotation );
	CC3Vector					getTargetVector();
	void						setTargetVector( const CC3Vector& aRotation );
};


class CC3RepeatForever : public CC3ActionInterval
{
public:
	CC3RepeatForever()
		: m_pInnerAction(NULL)
	{}
	virtual ~CC3RepeatForever();

	/** initializes the action */
	bool						initWithAction(CC3ActionInterval *pAction);
	virtual CCObject*			copyWithZone(CCZone *pZone);
	virtual void				startWithTarget(CC3Node* pTarget);
	virtual void				step(float dt);
	virtual bool				isDone(void);
	virtual CC3ActionInterval*	reverse(void);

	inline void					setInnerAction(CC3ActionInterval *pAction)
	{
		if (m_pInnerAction != pAction)
		{
			CC_SAFE_RELEASE(m_pInnerAction);
			m_pInnerAction = pAction;
			CC_SAFE_RETAIN(m_pInnerAction);
		}
	}

	inline CC3ActionInterval*	getInnerAction()
	{
		return m_pInnerAction;
	}

public:
	/** creates the action */
	static CC3RepeatForever*	create(CC3ActionInterval *pAction);

protected:
	/** Inner action */
	CC3ActionInterval *			m_pInnerAction;
};

class CC3ActionRotateForever : public CC3RepeatForever
{
public:
	void						initWithRotationRate( const CC3Vector& rotationPerSecond );
	static CC3ActionRotateForever* actionWithRotationRate( const CC3Vector& rotationPerSecond );
	CC3ActionInterval*			reverse();
};

class CC3ActionScaleBy : public CC3ActionTransformBy
{
	DECLARE_SUPER( CC3ActionTransformBy )
public:
	void						initWithDuration( float t, const CC3Vector& scale );
	void						initWithDuration( float t, GLfloat scale );
	/**
	 * Scale is multiplicative. Scaling BY 5 means take whatever the current scale is
	 * and multiply it by 5. If the previous scale was 3, then the future scale
	 * will be 15, not 8 if the numbers were simply added as in the superclass.
	 */
	void						startWithTarget( CC3Node* aTarget );
	static CC3ActionScaleBy*	actionWithDuration( float t, GLfloat aScale );
	static CC3ActionScaleBy*	actionWithDuration( float t, const CC3Vector& aScale );

	virtual void				update( float t );
	virtual CC3Vector			getTargetVector();
	virtual void				setTargetVector( const CC3Vector& aVector );

protected:
	CC3Vector					m_scaleDiffVector;
};

class CC3ActionRotateByAngle : public CC3ActionInterval
{
	DECLARE_SUPER( CC3ActionInterval );
public:
	void						initWithDuration( float t, GLfloat angle );
	void						initWithDuration( float t, GLfloat angle, const CC3Vector& anAxis );

	CCObject*					copyWithZone( CCZone* zone );
	CC3ActionInterval*			reverse();

	/** If no explicit rotation axis was set, retrieve it from the target node. */
	void						startWithTarget( CC3Node* aTarget );

	void						update( float t );

	std::string					description();

	static CC3ActionRotateByAngle* actionWithDuration( float t, GLfloat anAngle );
	static CC3ActionRotateByAngle* actionWithDuration( float t, GLfloat anAngle, const CC3Vector& anAxis );

protected:
	CC3Vector					m_rotationAxis;
	CC3Vector					m_activeRotationAxis;
	GLfloat						m_diffAngle;
	float						m_fPrevTime;
};

class CC3ActionRotateOnAxisForever : public CC3RepeatForever
{
public:
	void						initWithRotationRate( float degreesPerSecond, const CC3Vector& anAxis );
	CC3ActionInterval*			reverse();

	static CC3ActionRotateOnAxisForever* actionWithRotationRate( GLfloat degreesPerSecond, const CC3Vector& anAxis );
};

class CC3ActionTransformTo : public CC3ActionTransformVector
{
	DECLARE_SUPER( CC3ActionTransformVector );
public:
	void						initWithDuration( float t, const CC3Vector& endVector );
	CCObject*					copyWithZone( CCZone* zone );

	CC3ActionInterval*			reverse();
	std::string					description();

	void						startWithTarget( CC3Node* aTarget );

	static CC3ActionTransformTo* actionWithDuration( float t, const CC3Vector& aVector );

protected:
	CC3Vector					m_endVector;
};


/** CC3ActionMoveTo moves a target CC3Node to a specific location. */
class CC3ActionMoveTo : public CC3ActionTransformTo
{
	DECLARE_SUPER( CC3ActionTransformTo );
public:
	/**
	 * Initializes this instance to move the target node
	 * to the specified location, within the specified time duration.
	 */
	void						initWithDuration( float t, const CC3Vector& aLocation );

	virtual CC3Vector			getTargetVector();
	virtual void				setTargetVector( const CC3Vector& aLocation );

	static CC3ActionMoveTo*		actionWithDuration( float t, const CC3Vector& aLocation );
};


/**
 * CC3ActionRotateTo rotates a target CC3Node to a specific orientation.
 *
 * The rotational travel will be minimized, taking into consideration the cyclical nature
 * of rotation. For exmaple, a rotation from 10 degrees to 350 degrees in any axis should
 * travel -20 degrees, not the +340 degrees that would result from simple subtraction.
 */
class CC3ActionRotateTo : public CC3ActionTransformTo
{
	DECLARE_SUPER( CC3ActionTransformTo );
public:
	/**
	 * Initializes this instance to move the target node
	 * to the specified rotation, within the specified time duration.
	 */
	virtual void				initWithDuration( float t, const CC3Vector& aRotation );
	static CC3ActionRotateTo*	actionWithDuration( float t, const CC3Vector& aRotation );
	// We want to rotate the minimal angles to get from the startVector to the endVector,
	// taking into consideration the cyclical nature of rotation. Therefore, a rotation
	// from 10 degrees to 350 degrees should travel -20 degrees, not the +340 degrees
	// that would result from simple subtraction.
	void						startWithTarget( CC3Node* aTarget );
	virtual CC3Vector			getTargetVector();
	virtual void				setTargetVector( const CC3Vector& aVector );
};

/** CC3ActionScaleTo scales a target CC3Node to a specific scale. */
class CC3ActionScaleTo : public CC3ActionTransformTo
{
	DECLARE_SUPER( CC3ActionTransformTo );
public:
	/**
	 * Initializes this instance to scale the target node
	 * to the specified scale, within the specified time duration.
	 */
	virtual void				initWithDuration( float t, const CC3Vector& aScale );
	
	/**
	 * Initializes this instance to scale the target node uniformly in all
	 * dimensions to the specified uniformScale, within the specified time duration.
	 */
	virtual void				initWithDuration( float t, GLfloat aScale );

	virtual CC3Vector			getTargetVector();
	virtual void				setTargetVector( const CC3Vector& aVector );

	static CC3ActionScaleTo*	actionWithDuration( float t, const CC3Vector& aScale );
	static CC3ActionScaleTo*	actionWithDuration( float t, GLfloat aScale );
};


/**
 * CC3ActionRotateToAngle rotates a target CC3Node to a specific rotationAngle.
 *
 * The rotationAngle property rotates the node around the axis set in the rotationAxis
 * property of the node. Make sure that you set the rotationAxis property on the node
 * appropriately prior to running this action.
 *
 * The rotational travel will be minimized, taking into consideration the cyclical nature
 * of rotation. For exmaple, a rotation from 10 degrees to 350 degrees in any axis should
 * travel -20 degrees, not the +340 degrees that would result from simple subtraction.
 */
class CC3ActionRotateToAngle : CC3ActionInterval
{
	DECLARE_SUPER( CC3ActionInterval );
public:
	/**
	 * Initializes this instance to move the target node to the
	 * specified rotation angle, within the specified time duration.
	 */
	virtual void				initWithDuration( float t, GLfloat anAngle );
	CCObject*					copyWithZone( CCZone* pZone );
	CC3ActionInterval*			reverse();
	// We want to rotate the minimal angles to get from the startAngle to the endAngle,
	// taking into consideration the cyclical nature of rotation. Therefore, a rotation
	// from 10 degrees to 350 degrees should travel -20 degrees, not the +340 degrees
	// that would result from simple subtraction.
	void						startWithTarget( CC3Node* aTarget );
	void						update( float t );
	std::string					description();

	static CC3ActionRotateToAngle* actionWithDuration( float t, GLfloat anAngle );

protected:
	GLfloat						m_startAngle;
	GLfloat						m_endAngle;
	GLfloat						m_diffAngle;
};

/**
 * CC3ActionRotateToLookTowards rotates a target CC3Node to look towards a specific direction.
 */
class CC3ActionRotateToLookTowards : public CC3ActionTransformTo
{
	DECLARE_SUPER( CC3ActionTransformTo );
public:
	/**
	 * Initializes this instance to rotate the target node to look towards
	 * the specified dirction. within the specified time duration.
	 */
	virtual void				initWithDuration( float t, const CC3Vector& aDirection );
	virtual CC3Vector			getTargetVector();
	virtual void				setTargetVector( const CC3Vector& aDirection );

	static CC3ActionRotateToLookTowards* actionWithDuration( float t, const CC3Vector& aDirection );
};

/**
 * CC3ActionRotateToLookAt rotates a target CC3Node to look at a specific location.
 */
class CC3ActionRotateToLookAt : public CC3ActionRotateToLookTowards
{
	DECLARE_SUPER( CC3ActionRotateToLookTowards );
public:
	/**
	 * Initializes this instance to rotate the target node to look at
	 * the specified location. within the specified time duration.
	 */
	virtual void				initWithDuration( float t, const CC3Vector& aLocation );
	void						startWithTarget( CC3Node* aTarget );

	static CC3ActionRotateToLookAt* actionWithDuration( float t, const CC3Vector& aLocation );
};

/**
 * CC3ActionMoveDirectionallyBy is an abstract subclass of CCActionInterval that is
 * the parent of subclasses that move a target CC3Node by a specific translation
 * distance in a direction relative to the orientation of the node.
 *
 * The direction of movement is evaluated on each update frame. If the node
 * is also being rotated over time, this action will follow the change in
 * orientation of the node, and adjust the direction of movement.
 *
 * This is an abstract class. Subclasses define the actual direction of
 * movement by overriding the targetDirection property.
 */
class CC3ActionMoveDirectionallyBy : public CC3ActionInterval
{
	DECLARE_SUPER( CC3ActionInterval );
public:
	/**
	 * Initializes this instance to move the target node by the specified distance in
	 * the direction, as indicated by the subclass, within the specified time duration.
	 *
	 * The specified distance may be positive or negative, indicating whether the
	 * node should move forward or backward, relative to the direction of movement.
	 *
	 * The direction of movement is evaluated on each update frame. If the node
	 * is also being rotated over time, this action will follow the change in
	 * direction of movement of the node.
	 */
	void						initWithDuration( float t, GLfloat aDistance );
	virtual CC3Vector			getTargetDirection();
	CCObject*					copyWithZone( CCZone* zone );
	CC3ActionInterval*			reverse();
	void						startWithTarget( CC3Node* aTarget );
	void						update( float t );
	std::string					description();

	static CC3ActionMoveDirectionallyBy* actionWithDuration( float t, GLfloat aDistance );

protected:
	float						m_prevTime;
	GLfloat						m_distance;
};

/**
 * CC3ActionMoveForwardBy moves a target CC3Node forward by a specific distance.
 *
 * The direction of movement is taken from the forwardDirection property 
 * of the node, and is evaluated on each update frame. If the node is being
 * separately rotated while this action is active, this action will follow
 * the changes to the forwardDirection property of the node, and the resulting
 * path of the node will be a curve instead of a staight line.
 *
 * The specified distance may be negative, indicating the node should move
 * backward, relative to the direction indicated by the forwardDirection property.
 */
class CC3ActionMoveForwardBy : public CC3ActionMoveDirectionallyBy
{
public:
	CC3Vector					getTargetDirection();
};


/**
 * CC3ActionMoveRightBy moves a target CC3Node to the right by a specific distance.
 *
 * The direction of movement is taken from the rightDirection property of the
 * node, and is evaluated on each update frame. If the node is being separately
 * rotated while this action is active, this action will follow the changes to
 * the rightDirection property of the node, and the resulting path of the node
 * will be a curve instead of a staight line.
 *
 * The specified distance may be negative, indicating the node should move
 * backward, relative to the direction indicated by the rightDirection property.
 */
class CC3ActionMoveRightBy : public CC3ActionMoveDirectionallyBy
{
public:
	CC3Vector					getTargetDirection();
};

/**
 * CC3ActionMoveUpBy moves a target CC3Node up by a specific distance.
 *
 * The direction of movement is taken from the upDirection property of the node,
 * and is evaluated on each update frame. If the node is being separately rotated
 * while this action is active, this action will follow the changes to the
 * upDirection property of the node, and the resulting path of the node will be
 * a curve instead of a staight line.
 *
 * The specified distance may be negative, indicating the node should move
 * backward, relative to the direction indicated by the upDirection property.
 */

class CC3ActionMoveUpBy : public CC3ActionMoveDirectionallyBy
{
public:
	CC3Vector					getTargetDirection();
};

/**
 * CC3ActionTintTo changes the color of a target CC3Node to a particular color.
 *
 * This implementation changes BOTH the ambientColor and diffuseColor properties of the
 * target CC3Node. In addition, CC3ActionTintTo has several subclasses, each dedicated to 
 * changing one particular color property, without affecting the other color properties.
 */
class CC3ActionTintTo : public CC3ActionInterval
{
	DECLARE_SUPER( CC3ActionInterval );
public:
	/**
	 * Initializes this instance to change a color property of the target
	 * node to the specified color, within the specified time duration.
	 */
	virtual ccColor4F			getTargetColor();
	virtual void				setTargetColor( const ccColor4F& aColor );
	virtual void				initWithDuration( float t, const ccColor4F& aColor );
	static CC3ActionTintTo*		actionWithDuration( float t, const ccColor4F& aColor );

	CCObject*					copyWithZone( CCZone* zone );
	void						startWithTarget( CC3Node* aTarget );
	void						update( float t );

protected:
	ccColor4F					m_startColor;
	ccColor4F					m_endColor;
};

/** CC3ActionTintAmbientTo changes only the ambientColor property of the target CC3Node. */
class CC3ActionTintAmbientTo : public CC3ActionTintTo
{
public:
	ccColor4F					getTargetColor();
	void						setTargetColor( const ccColor4F& aColor );
};


/** CC3ActionTintDiffuseTo changes only the diffuseColor property of the target CC3Node. */
class CC3ActionTintDiffuseTo : public CC3ActionTintTo
{
public:
	ccColor4F					getTargetColor();
	void						setTargetColor( const ccColor4F& aColor );
};


/** CC3ActionTintSpecularTo changes only the specularColor property of the target CC3Node. */
class CC3ActionTintSpecularTo : public CC3ActionTintTo
{
public:
	ccColor4F					getTargetColor();
	void						setTargetColor( const ccColor4F& aColor );
};


/** CC3ActionTintEmissionTo changes only the emissionColor property of the target CC3Node. */
class CC3ActionTintEmissionTo : public CC3ActionTintTo
{
public:
	ccColor4F					getTargetColor();
	void						setTargetColor( const ccColor4F& aColor );
};

/**
 * CC3ActionAnimate animates a single track of animation on a CC3Node and its descendants.
 *
 * To animate a node, CC3ActionAnimate invokes the establishAnimationFrameAt:onTrack: method of the
 * target CC3Node. The heavy lifting is performed by the CC3NodeAnimation instance held in the
 * animation property of the node.
 *
 * The establishAnimationFrameAt:onTrack: method of the CC3Node also takes care of propagating
 * the animation to its descendant nodes. A complete assembly of nodes can therefore be animated
 * in concert for one track of information using a single CC3ActionAnimate instance.
 *
 * It is possible to animate only a fraction of the full animation. This can be done using
 * either the actionWithDuration:onTrack:limitFrom:to: or asActionLimitedFrom:to: methods.
 *
 * Doing so will result is an animation action that will perform only part of the animation.
 * This is very useful for an node that contains several different motions in one animation.
 * Using a range-limited CC3ActionAnimate, you can animate one of those distinct motions without having
 * to run the full animation. To do this, set the startOfRange and endOfRange values to the
 * fractional positions (between zero and one) of the start and end frames of the sub-animation.
 *
 * For example, if a character animation contains a punch animation that starts and stops
 * at relative positions 0.67 and 0.78 respectively within the full animation, setting
 * those two values here will result in an animation containing only the punch.
 */
class CC3ActionAnimate : public CC3ActionInterval
{
	DECLARE_SUPER( CC3ActionInterval );
public:
	GLuint						getTrackID();
	bool						isReversed();
	void						setIsReversed( bool reversed ); 
	void						initWithDuration( float t ); 
	/**
	 * Initializes this instance to animate the specified animation track on the target node,
	 * over the specified time duration.
	 */
	void						initWithDuration( float t, GLuint trackID );
	void						update( float t );

	CCObject*					copyWithZone( CCZone* zone );
	CC3ActionInterval*			reverse();

	/**
	 * Wraps this instance in an autoreleased CC3ActionRangeLimit instance that maps the normal
	 * zero-to-one update range to the specified range, and returns the CC3ActionRangeLimit instance
	 *
	 * The effective result is an animation action that will perform only part of the animation.
	 * This is very useful for an node that contains several different motions in one animation.
	 * Using a range-limited CC3ActionAnimate, you can animate one of those distinct motions without having
	 * to run the full animation. To do this, set the startOfRange and endOfRange values of the
	 * fractional positions (between zero and one) of the start and end frames of the sub-animation.
	 *
	 * For example, if a character animation contains a punch animation that starts and stops
	 * at relative positions 0.67 and 0.78 respectively within the full animation, setting
	 * those two values here will result in an animation containing only the punch.
	 */
	CC3ActionInterval*			asActionLimitedFrom( GLfloat startOfRange, GLfloat endOfRange );

	static CC3ActionAnimate*	actionWithDuration( GLfloat duration );
	static CC3ActionAnimate*	actionWithDuration( GLfloat duration, GLuint trackID );

	static CC3ActionAnimate*	actionWithDuration( float t, GLfloat startOfRange, GLfloat endOfRange );
	static CC3ActionAnimate*	actionWithDuration( float t, GLuint trackID, GLfloat startOfRange, GLfloat endOfRange );

protected:
	GLuint						m_trackID;
	bool						m_isReversed : 1;
};


/**
 * CC3ActionAnimationBlendingFadeTrackTo fades the animation blending weight of an animation
 * track in the target CC3Node from its current value to an end value. This allows the animation
 * track to be faded in or out smoothly.
 */
class CC3ActionAnimationBlendingFadeTrackTo : public CC3ActionInterval
{
	DECLARE_SUPER( CC3ActionInterval );
public:
	GLuint						getTrackID();
	/**
	 * Initializes this instance to fade the animation blending weight of the specified animation
	 * track on the target node to the specified value, over the specified time duration.
	 */
	void						initWithDuration( float t, GLuint trackID, GLfloat blendingWeight );

	void						update( float t );
	CC3ActionInterval*			reverse();
	CCObject*					copyWithZone( CCZone* zone );
	void						startWithTarget( CC3Node* aTarget );

	static CC3ActionAnimationBlendingFadeTrackTo* actionWithDuration( float t, GLuint trackID, GLfloat blendingWeight );

protected:
	GLfloat						m_startWeight;
	GLfloat						m_endWeight;
	/** The animation track on which the animation runs. */
	GLuint						m_trackID;
};

/** CC3ActionAnimationCrossFade fades smoothly from one animation track to another. */
class CC3ActionAnimationCrossFade : public CC3ActionInterval
{
	DECLARE_SUPER( CC3ActionInterval );
public:
	CC3ActionInterval*			reverse();
	GLuint						getFromTrackID();
	GLuint						getToTrackID();
	
	/**
	 * Initializes this instance to fade from the specified track to the specified track, over
	 * the specified time duration, and leaving the final track with a blending weight of one.
	 */
	void						initWithDuration( float t, GLuint fromTrackID, GLuint toTrackID );

	/**
	 * Initializes this instance to fade from the specified track to the specified track, over the
	 * specified time duration, and leaving the final track with the specified blending weight.
	 */
	void						initWithDuration( float t, GLuint fromTrackID, GLuint toTrackID, GLfloat toBlendingWeight );

	CCObject*					copyWithZone( CCZone* zone );
	void						startWithTarget( CC3Node* aTarget );
	void						update( float t );

	static CC3ActionAnimationCrossFade* actionWithDuration( float t, GLuint fromTrackID, GLuint toTrackID );
	static CC3ActionAnimationCrossFade* actionWithDuration( float t, GLuint fromTrackID, GLuint toTrackID, GLfloat toBlendingWeight );

protected:
	GLuint						m_fromTrackID;
	GLuint						m_toTrackID;
	GLfloat						m_startWeight;
	GLfloat						m_endWeight;
};

/**
 * CC3ActionAnimationBlendingSetTrackTo immediately sets the animation blending weight of
 * an animation track in the target CC3Node to a specified value.
 *
 * By setting the blending weight to zero, the animation track can be effectively turned off.
 */
class CC3ActionAnimationBlendingSetTrackTo : public CCActionInstant 
{
public:
	GLuint						getTrackID();
	
	/**
	 * Initializes this instance to set the animation blending weight of the specified animation
	 * track on the target node to the specified value.
	 *
	 * By setting the blending weight to zero, the animation track can be effectively turned off.
	 */
	void						initOnTrack( GLuint trackID, GLfloat blendingWeight );
	void						update( float t );

	static CC3ActionAnimationBlendingSetTrackTo* actionOnTrack( GLuint trackID, GLfloat blendingWeight );

protected:
	GLfloat						m_endWeight;
	GLuint						m_trackID;
};

/**
 * CC3EnableAnimation immediately enables a specified animation track on the target node
 * and all of its descendants.
 */
class CC3ActionEnableAnimationTrack : public CCActionInstant 
{
public:
	GLuint						getTrackID();
	/**
	 * Initializes this instance to enable the specified animation track on the target node
	 * and all of its descendants.
	 */
	void						initOnTrack( GLuint trackID );
	void						update( float t );
	static CC3ActionEnableAnimationTrack* actionOnTrack( GLuint trackID );

protected:
	GLuint						m_trackID;
};

/**
 * CC3DisableAnimation immediately disables a specified animation track on the target node
 * and all of its descendants.
 */
class CC3ActionDisableAnimationTrack : public CCActionInstant 
{
	DECLARE_SUPER( CCActionInstant );
public:
	GLuint						getTrackID();
	
	/**
	 * Initializes this instance to disable the specified animation track on the target node
	 * and all of its descendants.
	 */
	void						initOnTrack( GLuint trackID );
	void						update( float t );
	static CC3ActionDisableAnimationTrack* actionOnTrack( GLuint trackID );

protected:
	GLuint						m_trackID;
};

/**
 * A CC3ActionRangeLimit holds another action, and serves to modify the normal zero-to-one
 * range of update values to a smaller range that is presented to the contained action.
 *
 * For example, for an instance that is limited to a range of 0.5 to 0.75, as the 
 * input update value changes from zero to one, the value that is forwarded to the
 * contained action will change from 0.5 to 0.75.
 */
class CC3ActionRangeLimit : public CCActionEase 
{
	DECLARE_SUPER( CCActionEase );
public:
	/**
	 * Initializes this instance to modify the update values that are forwarded to the
	 * specified action so that they remain within the specified range.
	 */
	void						initWithAction( CCActionInterval* action, GLfloat startOfRange, GLfloat endOfRange );
	CCObject*					copyWithZone( CCZone* zone );
	CCActionInterval*			reverse();
	void						update( float t );
	static CC3ActionRangeLimit* actionWithAction( CCActionInterval* action, GLfloat startOfRange, GLfloat endOfRange );

protected:
	GLfloat						m_rangeStart;
	GLfloat						m_rangeSpan;
};

/**
 * CC3ActionRemove immediately removes a CC3Node from the scene, by invoking the remove method of the CC3Node.
 *
 * This action can be used as part of a CCActionSequence to remove a node after some other 
 * kind of action has completed. For example, you might create a CCActionSequence containing 
 * a CCActionFadeOut and a CC3ActionRemove, to fade a node away and then remove it from the scene.
 */
class CC3ActionRemove : public CCActionInstant
{
public:
	void						update( float t );
	static CC3ActionRemove*		create();
};

/** Animates a change to the contentSize of a CCNode. */
class CC3ActionCCNodeSizeTo : public CC3ActionInterval 
{
	DECLARE_SUPER( CC3ActionInterval );
public:
	/**
	 * Initializes this instance to change the contentSize property of the target to the specified
	 * size, within the specified elapsed duration.
	 */
	void						initWithDuration( float dur, const CCSize& endSize );
	void						update( float t );
	void						startWithTarget( CCNode* aTarget );
	CCObject*					copyWithZone( CCZone* zone );

	CC3ActionInterval*			reverse();
	static CC3ActionCCNodeSizeTo*	actionWithDuration( float dur, const CCSize& endSize );
	
protected:
	CCSize						startSize_;
	CCSize						endSize_;
	CCSize						sizeChange_;
};

class CC3ActionSequence : public CC3ActionInterval
{
public:
	~CC3ActionSequence(void);

	/** initializes the action */
	bool						initWithTwoActions(CC3ActionInterval *pActionOne, CC3ActionInterval *pActionTwo);

	virtual CCObject*			copyWithZone(CCZone* pZone);
	virtual void				startWithTarget(CC3Node *pTarget);
	virtual void				stop(void);
	virtual void				update(float t);
	virtual CC3ActionInterval*	reverse(void);

public:

	/** helper constructor to create an array of sequenceable actions */
	static CC3ActionSequence*	create(CC3ActionInterval *pAction1, ...);
	/** helper constructor to create an array of sequenceable actions given an array */
	static CC3ActionSequence*	create(CCArray *arrayOfActions);
	/** helper constructor to create an array of sequence-able actions */
	static CC3ActionSequence*	createWithVariableList(CC3ActionInterval *pAction1, va_list args);
	/** creates the action */
	static CC3ActionSequence*	createWithTwoActions(CC3ActionInterval *pActionOne, CC3ActionInterval *pActionTwo);

protected:
	CC3ActionInterval*			m_pActions[2];
	float						m_split;
	int							m_last;
};


#define CC3TransformVectorAction			CC3ActionTransformVector
#define CC3TransformBy						CC3ActionTransformBy
#define CC3MoveBy							CC3ActionMoveBy
#define CC3RotateBy							CC3ActionRotateBy
#define CC3ScaleBy							CC3ActionScaleBy
#define CC3RotateByAngle					CC3ActionRotateByAngle
#define CC3TransformTo						CC3ActionTransformTo
#define CC3MoveTo							CC3ActionMoveTo
#define CC3RotateTo							CC3ActionRotateTo
#define CC3ScaleTo							CC3ActionScaleTo
#define CC3RotateToAngle					CC3ActionRotateToAngle
#define CC3RotateToLookTowards				CC3ActionRotateToLookTowards
#define CC3RotateToLookAt					CC3ActionRotateToLookAt
#define CC3MoveDirectionallyBy				CC3ActionMoveDirectionallyBy
#define CC3MoveForwardBy					CC3ActionMoveForwardBy
#define CC3MoveRightBy						CC3ActionMoveRightBy
#define CC3MoveUpBy							CC3ActionMoveUpBy
#define CC3TintTo							CC3ActionTintTo
#define CC3TintAmbientTo					CC3ActionTintAmbientTo
#define CC3TintDiffuseTo					CC3ActionTintDiffuseTo
#define CC3TintSpecularTo					CC3ActionTintSpecularTo
#define CC3TintEmissionTo					CC3ActionTintEmissionTo
#define CC3Animate							CC3ActionAnimate
#define CC3AnimationBlendingFadeTrackTo		CC3ActionAnimationBlendingFadeTrackTo
#define CC3AnimationCrossFade				CC3ActionAnimationCrossFade
#define CC3AnimationBlendingSetTrackTo		CC3ActionAnimationBlendingSetTrackTo
#define CC3EnableAnimationTrack				CC3ActionEnableAnimationTrack
#define CC3DisableAnimationTrack			CC3ActionDisableAnimationTrack
#define CC3Remove							CC3ActionRemove
#define CC3CCSizeTo							CC3ActionCCNodeSizeTo

NS_COCOS3D_END

#endif

