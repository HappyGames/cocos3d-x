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
#include "cocos3d.h"

NS_COCOS3D_BEGIN

int CCNodeAdornmentBase::getZOrder()
{
	return super::getZOrder(); 
}

void CCNodeAdornmentBase::setZOrder( int z )
{
	m_nZOrder = z;
}

// Abstract implementation does nothing.
void CCNodeAdornmentBase::activate()
{

}

// Abstract implementation does nothing.
void CCNodeAdornmentBase::deactivate()
{

}

bool CCNodeAdornmentBase::init()
{
	initWithActionDuration( 0.0f ); 
	return true;
}

float CCNodeAdornmentBase::getActionDuration()
{
	return m_fActionDuration;
}

void CCNodeAdornmentBase::setActionDuration( float actionDuration )
{
	m_fActionDuration = actionDuration;
}

CCNodeAdornmentBase* CCNodeAdornmentBase::adornment()
{
	CCNodeAdornmentBase* pVal = new CCNodeAdornmentBase;
	pVal->init();
	pVal->autorelease();

	return pVal;
}

void CCNodeAdornmentBase::initWithActionDuration( float aDuration )
{
	if ( super::init() ) 
	{
		m_fActionDuration = aDuration;
		setZOrder( kAdornmentOverZOrder );
	}
}

CCNodeAdornmentBase* CCNodeAdornmentBase::adornmentWithActionDuration( float aDuration )
{
	CCNodeAdornmentBase* pVal = new CCNodeAdornmentBase;
	pVal->initWithActionDuration( aDuration );
	pVal->autorelease();

	return pVal;
}

// A (hopefully) unique tag that identifies the currently activated fade-in
// or fade-out action. This tag is used to cancel the action if needed. 
#define kFadeActionTag		(unsigned int)0xfade0001

void CCNodeAdornmentOverlayFader::initWithSprite( CCSprite* sprite, CCOpacity opacity, float aDuration )
{
	CCAssert( sprite, "Sprite must not be nil" );

	super::initWithActionDuration( aDuration );

	_peakOpacity = opacity;
	setContentSize ( sprite->getContentSize() );
	sprite->setVisible( false );
	sprite->setOpacity( 0 );
	_sprite = sprite;
	addChild( sprite );
}

CCNodeAdornmentOverlayFader* CCNodeAdornmentOverlayFader::adornmentWithSprite( CCSprite* sprite, CCOpacity opacity, float aDuration )
{
	CCNodeAdornmentOverlayFader* pVal = new CCNodeAdornmentOverlayFader;
	pVal->initWithSprite( sprite, opacity, aDuration );
	pVal->autorelease();

	return pVal;
}

void CCNodeAdornmentOverlayFader::initWithSprite( CCSprite* sprite, CCOpacity opacity )
{
	initWithSprite( sprite, opacity, kDefaultFadeDuration );
}

CCNodeAdornmentOverlayFader* CCNodeAdornmentOverlayFader::adornmentWithSprite( CCSprite* sprite, CCOpacity opacity )
{
	CCNodeAdornmentOverlayFader* pVal = new CCNodeAdornmentOverlayFader;
	pVal->initWithSprite( sprite, opacity );
	pVal->autorelease();

	return pVal;
}

void CCNodeAdornmentOverlayFader::initWithSprite( CCSprite* sprite )
{
	initWithSprite( sprite, kCCOpacityFull );
}

CCNodeAdornmentOverlayFader* CCNodeAdornmentOverlayFader::adornmentWithSprite( CCSprite* sprite )
{
	CCNodeAdornmentOverlayFader* pVal = new CCNodeAdornmentOverlayFader;
	pVal->initWithSprite( sprite );
	pVal->autorelease();

	return pVal;
}

// When activated, make the adornment node visible and establish an action
// to fade it in up to the peak opacity. The action is tagged so that it
// can be easily found if it needs to be cancelled.
void CCNodeAdornmentOverlayFader::activate()
{
	if ( _sprite )
	{
		_sprite->stopActionByTag( kFadeActionTag );	// Cancel any existing fade action
	
		CCAction* fadeAction = CCActionFadeTo::create( getActionDuration(), _peakOpacity );
		fadeAction->setTag( kFadeActionTag );
		_sprite->setVisible( true );
		_sprite->runAction( fadeAction );
	}
}

// When deactivated, establish an action sequence to first fade the adornment node
// back to fully transparent, and then explicitly hide the adornment. Although this
// last step is visually redundant, it makes subsequent drawing of the invisible
// adornment node more efficient. The action is tagged so that it can be easily found
// if it needs to be cancelled.
void CCNodeAdornmentOverlayFader::deactivate()
{
	if ( _sprite )
	{
		_sprite->stopActionByTag( kFadeActionTag );	// Cancel any existing fade action
		CCActionInterval* fadeAction = CCActionFadeOut::create( getActionDuration() );
		CCFiniteTimeAction* hideAction = CCActionHide::create();
		CCActionInterval* fadeThenHideAction = CCActionSequence::createWithTwoActions( fadeAction, hideAction );
		fadeThenHideAction->setTag( kFadeActionTag );
		_sprite->runAction( fadeThenHideAction );
	}
}

CCOpacity CCNodeAdornmentOverlayFader::getPeakOpacity()
{
	return _peakOpacity;
}

void CCNodeAdornmentOverlayFader::setPeakOpacity( CCOpacity peakOpacity )
{
	_peakOpacity = peakOpacity;
}

CCSprite* CCNodeAdornmentOverlayFader::getSprite()
{
	return _sprite;
}

// A (hopefully) unique tag that identifies the currently activated scaling action.
// This tag is used to cancel the action if needed. 
#define kScaleActionTag 0x5ca1e001

void CCNodeAdornmentScaler::initToScaleBy( const CCSize& aScale, float aDuration )
{
	if ( super::init() ) 
	{
		setActionDuration( aDuration );
		_activatedScale = aScale;
	}
}

CCNodeAdornmentScaler* CCNodeAdornmentScaler::adornmentToScaleBy( const CCSize& aScale, float aDuration )
{
	CCNodeAdornmentScaler* pVal = new CCNodeAdornmentScaler;
	pVal->initToScaleBy( aScale, aDuration );
	pVal->autorelease();

	return pVal;
}

void CCNodeAdornmentScaler::initToScaleBy( const CCSize& aScale )
{
	initToScaleBy( aScale, kDefaultScalingDuration );
}

CCNodeAdornmentScaler* CCNodeAdornmentScaler::adornmentToScaleBy( const CCSize& aScale )
{
	CCNodeAdornmentScaler* pVal = new CCNodeAdornmentScaler;
	pVal->initToScaleBy( aScale );
	pVal->autorelease();

	return pVal;
}

void CCNodeAdornmentScaler::initToScaleUniformlyBy( float aScale, float aDuration )
{
	initToScaleBy( CCSizeMake(aScale, aScale), aDuration );
}

CCNodeAdornmentScaler* CCNodeAdornmentScaler::adornmentToScaleUniformlyBy( float aScale, float aDuration )
{
	CCNodeAdornmentScaler* pVal = new CCNodeAdornmentScaler;
	pVal->initToScaleUniformlyBy( aScale, aDuration );
	pVal->autorelease();

	return pVal;
}

void CCNodeAdornmentScaler::initToScaleUniformlyBy( float aScale )
{
	initToScaleUniformlyBy( aScale, kDefaultScalingDuration );
}

CCNodeAdornmentScaler* CCNodeAdornmentScaler::adornmentToScaleUniformlyBy( float aScale )
{
	CCNodeAdornmentScaler* pVal = new CCNodeAdornmentScaler;
	pVal->initToScaleUniformlyBy( aScale );
	pVal->autorelease();

	return pVal;
}

// Sets the value of originalScale from the current parent.
void CCNodeAdornmentScaler::setOriginalScaleFromParent()
{
	CCNode* p = getParent();
	if ( p )
		_originalScale = CCSizeMake(p->getScaleX(), p->getScaleY());
}

// Overridden to cache the parent's current scale
void CCNodeAdornmentScaler::setParent( CCNode* aNode )
{
	super::setParent( aNode );
	setOriginalScaleFromParent();
}

// When activated, scale the parent CCNode by the value of the activatedScale property.
// The current scale value of the parent is cached again, in case that scale had been
// changed since this adornment was added to the parent. We do not simply use a deactivation
// scale of 1 / activationScale in case the activation scaling is interrupted by the
// deactivation, and has not fully scaled up at the time the deactivation starts.
// The action is tagged so that it can be easily found if it needs to be cancelled.
void CCNodeAdornmentScaler::activate()
{
	CCNode* p = getParent();
	if ( p == NULL )
		return;

	CCAction* currAction = p->getActionByTag( kScaleActionTag );
	if ( currAction ) 
	{
		// if we already have an active action, cancel it
		p->stopAction( currAction );
	} 
	else
	{
		// only cache scale if a scaling action is not active
		// because otherwise scale will be evolvin and we'll cache something halfway
		setOriginalScaleFromParent();
	}
	// use scaleTo instead of scaleBy so that final size is deterministic in the case
	// where we have interrupted an active scaling action above
	float finalScaleX = _originalScale.width * _activatedScale.width;
	float finalScaleY = _originalScale.height * _activatedScale.height;
	CCAction* scaleAction = CCActionScaleTo::create( getActionDuration(), finalScaleX, finalScaleY );
	scaleAction->setTag( kScaleActionTag );
	p->runAction( scaleAction );
}

// When activated, scale the parent CCNode back to its original scale.
// The action is tagged so that it can be easily found if it needs to be cancelled.
void CCNodeAdornmentScaler::deactivate()
{
	CCNode* p = getParent();
	if ( p == NULL )
		return;

	p->stopActionByTag( kScaleActionTag );		// Cancel any existing scaling action
	CCAction* scaleAction = CCActionScaleTo::create( getActionDuration(), _originalScale.width, _originalScale.height );
	scaleAction->setTag( kScaleActionTag );
	p->runAction( scaleAction );
}

CCSize CCNodeAdornmentScaler::getActivatedScale()
{
	return _activatedScale;
}

void CCNodeAdornmentScaler::setActivatedScale( const CCSize& activatedScale )
{
	_activatedScale = activatedScale;
}

CCNodeAdornment* AdornableMenuItemToggle::getAdornment()
{
	return _adornment; 
}

// Add the adornment as a child, removing any previous adornment.
void AdornableMenuItemToggle::setAdornment( CCNodeAdornment* aNode )
{
	if ( _adornment )
		removeChild( _adornment, true );
	
	_adornment = aNode;
	
	if ( aNode ) 
		addChild( aNode );
}

// When this menu item is selected, activate the adornment
void AdornableMenuItemToggle::selected()
{
	super::selected();
	_adornment->activate();
}

// When this menu item is unselected, deactivate the adornment
void AdornableMenuItemToggle::unselected()
{
	super::unselected();
	_adornment->deactivate();
}

AdornableMenuItemToggle* AdornableMenuItemToggle::createWithTarget(CCObject* target, SEL_MenuHandler selector, CCMenuItem* item, ...)
{
	va_list args;
	va_start(args, item);
	AdornableMenuItemToggle *pRet = new AdornableMenuItemToggle();
	pRet->initWithTarget(target, selector, item, args);
	pRet->autorelease();
	va_end(args);
	return pRet;
}

CCNodeAdornment* AdornableMenuItemImage::getAdornment()
{
	return _adornment; 
}

// Add the adornment as a child, removing any previous adornment.
void AdornableMenuItemImage::setAdornment( CCNodeAdornment* aNode )
{
	if ( _adornment )
		removeChild( _adornment, true );
	
	_adornment = aNode;
	if ( aNode ) 
		addChild( aNode );
}

// When this menu item is selected, activate the adornment
void AdornableMenuItemImage::selected()
{
	super::selected();
	
	if ( _adornment )
		_adornment->activate();
}

// When this menu item is unselected, deactivate the adornment
void AdornableMenuItemImage::unselected() 
{
	super::unselected();

	if ( _adornment )
		_adornment->deactivate();
}

AdornableMenuItemImage* AdornableMenuItemImage::create(const char *normalImage, const char *selectedImage, CCObject* target, SEL_MenuHandler selector)
{
	AdornableMenuItemImage *pRet = new AdornableMenuItemImage();
	if (pRet && pRet->initWithNormalImage(normalImage, selectedImage, NULL, target, selector))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

NS_COCOS3D_END
