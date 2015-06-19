#include "SimpleAudioEngine.h"
#include "AppDelegate.h"
#include "MenuScene.h"

USING_NS_COCOS3D;
USING_NS_CC;


AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate() 
{
	ObjectFactory::getInstance()->destroyInstance();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->end();
	CCScriptEngineManager::purgeSharedManager();
	CLoggers::purge();
}

bool AppDelegate::applicationDidFinishLaunching()
{
	/// Add logger
	CLoggers* logMgr = CLoggers::sharedLoggers();
	logMgr->addLogger( this );
	//logMgr->setShowTime( false );

	// initialize director
	CCDirector *pDirector = CCDirector::sharedDirector();
	pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

	CCSize viewSize = CCDirector::sharedDirector()->getWinSizeInPixels();
	GLfloat deviceFactor = MAX(viewSize.height, viewSize.width) / 480.f;
	CCLOG( "Device Factor: %.2f", deviceFactor );

	std::string sBuilld = Cocos3d::version();
	CCLOG( sBuilld.c_str() );

	//////////////////////////////////////////////////////////////////////////
	// Use a customized action manager
	CC3ActionManager::active();

	TargetPlatform target = getTargetPlatform();

	if (target == kTargetIpad)
	{
		// ipad
		CCFileUtils::sharedFileUtils()->addSearchPath("hd");
		// don't enable retina because we don't have ipad hd resource
		CCEGLView::sharedOpenGLView()->setDesignResolutionSize(SCREEN_WIDTH, SCREEN_HEIGHT, kResolutionNoBorder);
	}
	else if (target == kTargetIphone)
	{        
		CCFileUtils::sharedFileUtils()->addSearchPath("sd");
		CCEGLView::sharedOpenGLView()->setDesignResolutionSize(SCREEN_WIDTH, SCREEN_HEIGHT, kResolutionExactFit);
	}
	else 
	{
		// android, windows, blackberry, linux or mac
		// use 960*640 resources as design resolution size
		CCFileUtils::sharedFileUtils()->addSearchPath("sd");
		CCEGLView::sharedOpenGLView()->setDesignResolutionSize(SCREEN_WIDTH, SCREEN_HEIGHT, kResolutionShowAll);
	}

	CCFileUtils::sharedFileUtils()->addSearchPath( "gameui" );
	CCFileUtils::sharedFileUtils()->addSearchPath( "3d" );

	// turn on display FPS
	pDirector->setDisplayStats( true );

	// set FPS. the default value is 1.0/60 if you don't call this
	pDirector->setAnimationInterval(1.0 / 60);

	onStartUp();

	// create a scene. it's an autorelease object
	CCScene* pScene = CMainMenu::create();

	// run
	pDirector->runWithScene( pScene );
	 
	return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
	CCDirector::sharedDirector()->pause();
	// if you use SimpleAudioEngine, it must be pause
	CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
	CCDirector::sharedDirector()->resume();
	// if you use SimpleAudioEngine, it must resume here
	CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

void AppDelegate::onStartUp()
{
	Cocos3d::startUp( false );
	Cocos3d::addInternalShaderPaths();
}

void AppDelegate::logMessage( const std::string& msg )
{
	CCLOG( msg.c_str() );
}
