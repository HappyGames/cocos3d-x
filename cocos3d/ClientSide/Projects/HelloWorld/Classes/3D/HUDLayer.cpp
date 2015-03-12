#include "HUDLayer.h"

#define kHUDEventPriority	- 64

void CHUDLayer::initializeControls()
{
	ignoreAnchorPointForPosition( false );
	setAnchorPoint( ccp(0.5f, 0.5f) );

	_isTracking = false;
	//this->setUserInteractionEnabled( true );
	//this->setMousePriority( kHUDEventPriority );

	scheduleUpdate();
}

CHUDLayer* CHUDLayer::layer()
{
	CHUDLayer* pLayer = new CHUDLayer;
	pLayer->init();
	pLayer->autorelease();

	return pLayer;
}