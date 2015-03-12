#ifndef _MENU_SCENE_H_
#define _MENU_SCENE_H_
#include "cocos2d.h"

USING_NS_CC;

class CMenuLayer : public CCLayerColor
{
public:
	CMenuLayer() {};
	virtual ~CMenuLayer();

	CREATE_FUNC(CMenuLayer);

public:
	bool					init();

	virtual void			ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void			ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void			ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);

	virtual void			onEnter();

protected:
	void					createMenu();
	void					onQuit( CCObject* pSender );
	void					onPlay( CCObject* pSender );
	void					update(float delta);

private:
	float					m_fRotation;
	CCPoint					m_tBeginPos;
	CCPoint					m_preLocation;
};

class CMainMenu : public CCScene
{
public:
	CMainMenu() : m_pLayer(NULL) {};
	~CMainMenu();

	CREATE_FUNC( CMainMenu );

public:
	CC_SYNTHESIZE_READONLY( CMenuLayer*, m_pLayer, Layer );

	bool					init();
};

#endif
