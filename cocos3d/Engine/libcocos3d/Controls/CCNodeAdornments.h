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
#ifndef _CCNODE_ADORNMENTS_H_
#define _CCNODE_ADORNMENTS_H_

NS_COCOS3D_BEGIN

/**
 * Constants that can be used to set the Z-Order of an adornment
 * to place it under or over the base node.
 */
#define kAdornmentOverZOrder 10
#define kAdornmentUnderZOrder -10

/**
 * Adornments are CCNodes that support the CCNodeAdornmentProtocol, and can be added to other
 * CCNodes that support the AdornableCCNodeProtocol protocol in order to add transient visual
 * characteristics to the adorned CCNode. Examples of adornments might be a shine, a surrounding
 * ring, an overlay label, or an action that scales, rotates, or otherwise temporarily changes
 * the adorned CCNode.
 */
class CCNodeAdornmentProtocol
{
public:
	/**
	 * Activates this adornment. Typically this will trigger a CCAction to animate the
	 * appearance of the adornment over the base CCNode, or will alter the base node in some way.
	 */
	virtual void				activate() { }

	/**
	 * Deactivates this adornment. Typically this will trigger a CCAction to animate the
	 * reversion to the unadorned base CCNode.
	 */
	virtual void				deactivate() { }

};

class CCNodeAdornment : public CCNode, public CCNodeAdornmentProtocol
{
public:
	CCNodeAdornment() 
	{
	
	}
};

/**
 * Describes the behaviour required for CCNodes that can be adorned with an adornment.
 * Adornments are themselves CCNodes that support the CCNodeAdornmentProtocol protocol,
 * and typical implementations of AdornableCCNodeProtocol will add the adornment as
 * a child. Concrete implementations of AdornableCCNodeProtocol will be subclassed from
 * a specific subclass of CCNode (eg: CCLayer, CCMenuItem, etc).
 */
class AdornableCCNodeProtocol
{
public:
	virtual CCNodeAdornment*	getAdornment() = 0;
	virtual void				setAdornment( CCNodeAdornment* adornment ) = 0;
};

/**
 * A base implementation of the CCNodeAdornmentProtocol as a CCNode extension.
 * This base class can be used to create concrete adornment subclasses.
 * Initial zOrder is set to kAdornmentOverZOrder. This places the adornment above the
 * content of the adorned node (which may have other children aside from the adornment).
 * The implementation of activate and deactivate in this abstract class do nothing.
 */
class CCNodeAdornmentBase : public CCNodeAdornment
{
	DECLARE_SUPER( CCNodeAdornment );
public:
	// Abstract implementation does nothing.
	virtual	void				activate();
	virtual void				deactivate();

	/**
	 * Overridden to expose the zOrder property to be assignable for CCNodeAdornments.
	 * Setting zOrder in an adornment before adding it to the base CCNode allows the
	 * adornment to be placed either over or under the base CCNode and its other children.
	 * Initial value is kAdornmentOverZOrder, which places the adornment above the content
	 * of the adorned CCNode.
	 */
	virtual int					getZOrder();
	virtual void				setZOrder( int zOrder );

	/**
	 * Most adornments will invoke a CCActionInterval when activated and deactivated in
	 * order to animate the display or reversion of the adornment. The duration of these
	 * actions is controlled by the value of this property.
	 */
	virtual float				getActionDuration();
	virtual void				setActionDuration( float actionDuration );

	/** Initializes this instance with a value of zero for the actionDuration property. */
	virtual bool				init();

	/**
	 * Allocates and initializes an autoreleased instance with a value of zero
	 * for the actionDuration property.
	 */
	static CCNodeAdornmentBase* adornment();

	/** Initializes this instance with the specified value for the actionDuration property. */
	void						initWithActionDuration( float aDuration );

	/**
	 * Allocates and initializes an autoreleased instance with the specified value
	 * for the actionDuration property.
	 */
	static CCNodeAdornmentBase* adornmentWithActionDuration( float aDuration );

protected:
	float						_actionDuration;
};

/** The default duration of the fade-in and fade-out actions of the CCNodeAdornmentOverlayFader */
#define kDefaultFadeDuration 0.25


/**
 * An adornment CCNode that wraps a separate visible child CCSprite, and implements the
 * CCNodeAdornmentProtocol such that, when activated, will fade the visible child CCSprite
 * in over the adorned CCNode, and, when deactivated, will fade that child CCSprite out to
 * make it invisible again. It is the child CCSprite that defines what the actual adornment
 * will look like. That child may be any CCSprite.
 */
class CCNodeAdornmentOverlayFader : public CCNodeAdornmentBase 
{
	DECLARE_SUPER( CCNodeAdornmentBase );
public:
	// When activated, make the adornment node visible and establish an action
	// to fade it in up to the peak opacity. The action is tagged so that it
	// can be easily found if it needs to be cancelled.
	virtual void				activate();

	// When deactivated, establish an action sequence to first fade the adornment node
	// back to fully transparent, and then explicitly hide the adornment. Although this
	// last step is visually redundant, it makes subsequent drawing of the invisible
	// adornment node more efficient. The action is tagged so that it can be easily found
	// if it needs to be cancelled.
	virtual void				deactivate();

	/** The visible CCSprite to be displayed as the visual child adornment. */
	virtual CCSprite*			getSprite();

	/**
	 * When activated, this CCNodeAdornmentOverlayFader instance will fade the sprite in from 
	 * zero opacity to the value of this peakOpacity property, and when deactivated, will fade 
	 * the adornmentNode back to zero opacity.
	 *
	 * This value defaults to kCCOpacityFull, indicating that the adornmentNode should fade in from 
	 * fully transparent to fully opaque. Setting this property to a lower value will allow the base 
	 * CCNode to remain partially visible, making the sprite appear as a "shine" over the base CCNode.
	 */
	virtual CCOpacity			getPeakOpacity();
	virtual void				setPeakOpacity( CCOpacity peakOpacity );

	/**
	 * Initializes this instance to use the specified CCSprite as the visual sprite and, when 
	 * activated, to fade the sprite in from fully transparent up to the specified opacity, 
	 * over the specified duration.
	 */
	virtual void				initWithSprite( CCSprite* sprite, CCOpacity opacity, float aDuration );

	/**
	 * Allocates and initializes an autoreleased instance to use the specified CCSprite as the
	 * visual sprite and, when activated, to fade the sprite in from fully transparent up to 
	 * the specified opacity, over the specified duration.
	 */
	static CCNodeAdornmentOverlayFader* adornmentWithSprite( CCSprite* sprite, CCOpacity opacity, float aDuration );

	/**
	 * Initializes this instance to use the specified CCSprite as the visual sprite and, when 
	 * activated, to fade the sprite in from fully transparent up to the specified opacity. 
	 * The fade duration will default to kDefaultFadeDuration.
	 */
	virtual void				initWithSprite( CCSprite* sprite, CCOpacity opacity );

	/**
	 * Allocates and initializes an autoreleased instance to use the specified CCSprite as the
	 * visual sprite and, when activated, to fade the sprite in from fully transparent up to 
	 * the specified opacity. The fade duration will default to kDefaultFadeDuration.
	 */
	static CCNodeAdornmentOverlayFader* adornmentWithSprite( CCSprite* aNode, CCOpacity opacity );

	/**
	 * Initializes this instance to use the specified CCSprite as the visual sprite and,
	 * when activated, to fade the sprite in from fully transparent to fully opaque.
	 * The fade duration will default to kDefaultFadeDuration.
	 */
	virtual void				initWithSprite( CCSprite* aNode );

	/**
	 * Allocates and initializes an autoreleased instance to use the specified CCSprite as the
	 * visual sprite and, when activated, to fade the sprite in from fully transparent fully opaque. 
	 * The fade duration will default to kDefaultFadeDuration.
	 */
	static CCNodeAdornmentOverlayFader* adornmentWithSprite( CCSprite* aNode );

protected:
	CCSprite*					_sprite;
	CCOpacity					_peakOpacity;

	CCSize						_contentSize;
};

/** The default duration of the scaling up and down actions of the CCNodeAdornmentScaler */
#define kDefaultScalingDuration 0.25

/**
 * An adornment CCNode that implements the CCNodeAdornmentProtocol such that, when activated,
 * will scale the adorned CCNode (the parent of this adornment), making it appear either
 * larger or smaller and, when deactivated, will return the adorned CCNode to its original scale.
 */
class CCNodeAdornmentScaler : public CCNodeAdornmentBase 
{
	DECLARE_SUPER( CCNodeAdornmentBase );
public:
	// When activated, scale the parent CCNode by the value of the activatedScale property.
	// The current scale value of the parent is cached again, in case that scale had been
	// changed since this adornment was added to the parent. We do not simply use a deactivation
	// scale of 1 / activationScale in case the activation scaling is interrupted by the
	// deactivation, and has not fully scaled up at the time the deactivation starts.
	// The action is tagged so that it can be easily found if it needs to be cancelled.
	virtual void				activate();

	// When activated, scale the parent CCNode back to its original scale.
	// The action is tagged so that it can be easily found if it needs to be cancelled.
	virtual void				deactivate();

	/**
	 * When activated, this CCNodeAdornmentScaler instance will scale the adorned CCNode by this
	 * scaling factor. This scaling is applied on top of any scaling applied to the adorned CCNode
	 * at the time this adornment is activated. For example, if the adorned CCNode is already scaled
	 * by a factor of two and this activatedScale property is set to a value of three, then the final
	 * scale of the adorned CCNode when the adornment is activated will be six. If needed, different
	 * scaling values can be set for the width and height to cause non-uniform scaling.
	 */
	virtual CCSize				getActivatedScale();
	virtual void				setActivatedScale( const CCSize& activatedScale );

	/**
	 * Initializes this instance to scale the adorned CCNode by the specified factor over
	 * the specified duration. The width and height scaling components may be set to
	 * different values to cause non-uniform scaling.
	 */
	virtual void				initToScaleBy( const CCSize& aScale, float aDuration );

	/**
	 * Allocates and initializes an autoreleased instance to scale the adorned CCNode by
	 * the specified factor over the specified duration. The width and height scaling
	 * components may be set to different values to cause non-uniform scaling.
	 */
	static CCNodeAdornmentScaler* adornmentToScaleBy( const CCSize& aScale, float aDuration );

	/**
	 * Initializes this instance to scale the adorned CCNode by the specified factor.
	 * The duration of this scaling will default to kDefaultScalingDuration. The width and
	 * height scaling components may be set to different values to cause non-uniform scaling.
	 */
	virtual void				initToScaleBy( const CCSize& aScale );

	/**
	 * Allocates and initializes an autoreleased instance to scale the adorned CCNode by the
	 * specified factor. The duration of this scaling will default to kDefaultScalingDuration.
	 * The width and height scaling components may be set to different values to cause
	 * non-uniform scaling.
	 */
	static CCNodeAdornmentScaler* adornmentToScaleBy( const CCSize& aScale );

	/**
	 * Initializes this instance to scale the adorned CCNode by the specified factor over
	 * the specified duration. Scaling will be uniform in both width and height.
	 */
	virtual void				initToScaleUniformlyBy( float aScale, float aDuration );

	/**
	 * Allocates and initializes an autoreleased instance to scale the adorned CCNode by
	 * the specified factor over the specified duration. Scaling will be uniform in both
	 * width and height.
	 */
	static CCNodeAdornmentScaler* adornmentToScaleUniformlyBy( float aScale, float aDuration );

	/**
	 * Initializes this instance to scale the adorned CCNode by the specified factor.
	 * The duration of this scaling will default to kDefaultScalingDuration.
	 * Scaling will be uniform in both width and height.
	 */
	virtual void				initToScaleUniformlyBy( float aScale );

	/**
	 * Allocates and initializes an autoreleased instance to scale the adorned CCNode by the
	 * specified factor. The duration of this scaling will default to kDefaultScalingDuration.
	 * Scaling will be uniform in both width and height.
	 */
	static CCNodeAdornmentScaler* adornmentToScaleUniformlyBy( float aScale );

	// Sets the value of originalScale from the current parent.
	virtual void				setOriginalScaleFromParent();

	// Overridden to cache the parent's current scale
	virtual void				setParent( CCNode* aNode );

protected:
	CCSize						_activatedScale;
	CCSize						_originalScale;
};

/**
 * A concrete subclass of CCMenuItemToggle that supports a visual adornment via the 
 * AdornableCCNodeProtocol. When the menu item is selected by the user, the adornment will be
 * activated, and when the item is deselected by the user, the adornment will be deactivated.
 *
 * This control can only be used with Cocos2D v1 & v2.
 */
class AdornableMenuItemToggle : public CCMenuItemToggle, public AdornableCCNodeProtocol
{
	DECLARE_SUPER( CCMenuItemToggle );
public:
	virtual CCNodeAdornment*	getAdornment();

	// Add the adornment as a child, removing any previous adornment.
	virtual void				setAdornment( CCNodeAdornment* aNode );

	// When this menu item is selected, activate the adornment
	virtual void				selected();

	// When this menu item is unselected, deactivate the adornment
	virtual void				unselected();

	/** creates a menu item from a list of items with a target/selector */
	static AdornableMenuItemToggle* createWithTarget(CCObject* target, SEL_MenuHandler selector, CCMenuItem* item, ...);  

protected:
	CCNodeAdornment*			_adornment;
};

/**
 * A concrete subclass of CCMenuItemImage that supports a visual adornment via the
 * AdornableCCNodeProtocol. When the menu item is selected by the user, the adornment will be
 * activated, and when the item is deselected by the user, the adornment will be deactivated.
 *
 * This control can only be used with Cocos2D v1 & v2.
 */
class AdornableMenuItemImage : public CCMenuItemImage, public AdornableCCNodeProtocol
{
	DECLARE_SUPER( CCMenuItemImage );
public:
	virtual CCNodeAdornment*	getAdornment();

	// Add the adornment as a child, removing any previous adornment.
	virtual void				setAdornment( CCNodeAdornment* aNode );

	// When this menu item is selected, activate the adornment
	virtual void				selected();

	// When this menu item is unselected, deactivate the adornment
	virtual void				unselected();

	/** creates a menu item with a normal and selected image with target/selector */
	static AdornableMenuItemImage* create(const char *normalImage, const char *selectedImage, CCObject* target, SEL_MenuHandler selector);

protected:
	CCNodeAdornment*			_adornment;
};

NS_COCOS3D_END

#endif
