#include "SimpleAudioEngine.h"
#include "MenuScene.h"
#include "3D/CC3HelloWorldLayer.h"
#include "3D/CC3HelloWorldScene.h"
#include "3D/CC3MashUpLayer.h"
#include "3D/CC3MashUpScene.h"

bool CMainMenu::init()
{
	if( CCScene::init() )
	{
		m_pLayer = CMenuLayer::create();
		m_pLayer->retain();
		this->addChild(m_pLayer);

		m_pLayer->setTouchEnabled( true );
		return true;
	}
	else
	{
		return false;
	}
}

CMainMenu::~CMainMenu()
{
	if (m_pLayer)
	{
		m_pLayer->release();
		m_pLayer = NULL;
	}
}


bool CMenuLayer::init()
{
	setTouchEnabled(true);
	m_fRotation = 0;
	// sheduleUpdate();

	 CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this,0,false);

	//if ( CCLayerColor::initWithColor( ccc4(255, 255, 255, 255) ) )
	if ( CCLayerColor::init() )
	{
#if 0
		CC3Layer* cc3Layer = CC3HelloWorldLayer::create();
		CC3Scene* cc3Scene = CC3HelloWorldScene::create();
#else
		CC3Layer* cc3Layer = CC3MashUpLayer::create();
		CC3Scene* cc3Scene = CC3MashUpScene::create();
#endif
		
		cc3Layer->setCC3Scene( cc3Scene );

		addChild( cc3Layer );

		createMenu();

		return true;
	}
	else
	{
		return false;
	}
}

CMenuLayer::~CMenuLayer()
{
	
}

void CMenuLayer::onEnter()
{
	CCLayerColor::onEnter();
}

void CMenuLayer::createMenu()
{
	CCMenuItemFont::setFontSize( 32 );
	CCMenuItemFont::setFontName("Courier New");

	setTouchEnabled(true);

	// Font Item
	CCMenuItem* quit = CCMenuItemImage::create( "images/CloseNormal.png", "images/CloseSelected.png", this, menu_selector(CMenuLayer::onQuit) );
	CCMenu* menu = CCMenu::create( quit, NULL);
	menu->alignItemsVertically();

	addChild( menu );

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSize itemSize = quit->getContentSize();

	menu->setPositionX( winSize.width - itemSize.width / 2 );
	menu->setPositionY( winSize.height - itemSize.height / 2 );
}

void CMenuLayer::onQuit( CCObject* pSender )
{
	CCDirector::sharedDirector()->end();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->end();

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	exit(0);
#endif
}

void CMenuLayer::onPlay( CCObject* pSender )
{

}

void CMenuLayer::update(float delta)
{

}

void CMenuLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
}

void CMenuLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
}

void CMenuLayer::ccTouchesEnded( CCSet *pTouches, CCEvent *pEvent )
{

}
