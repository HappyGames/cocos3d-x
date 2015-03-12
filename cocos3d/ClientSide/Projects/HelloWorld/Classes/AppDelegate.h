#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_
#include "cc3Helper/LogDelegate.h"

#define SCREEN_WIDTH	960
#define SCREEN_HEIGHT	640

/**
@brief    The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by CCDirector.
*/
class CGameEntryScene;
class  AppDelegate : private cocos2d::CCApplication, 
	public cocos3d::CLogDelegate
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    /**
    @brief    Implement CCDirector and CCScene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool		applicationDidFinishLaunching();

    /**
    @brief  The function be called when the application enter background
    @param  the pointer of the application
    */
    virtual void		applicationDidEnterBackground();

    /**
    @brief  The function be called when the application enter foreground
    @param  the pointer of the application
    */
    virtual void		applicationWillEnterForeground();

	virtual void		logMessage( const std::string& msg );
    
protected:
    void				onStartUp();
};

#endif // _APP_DELEGATE_H_
