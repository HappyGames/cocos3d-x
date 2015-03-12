#ifndef _CC3_HELLOWORLD_LAYER_H_
#define _CC3_HELLOWORLD_LAYER_H_
#include "Scenes/CC3Layer.h"
USING_NS_COCOS3D;

class CC3HelloWorldLayer : public CC3Layer
{
public:
	CREATE_FUNC( CC3HelloWorldLayer );

public:
	virtual void		initializeControls();
	virtual void		onOpenCC3Layer();
	virtual void		onCloseCC3Layer();

	virtual void		registerWithTouchDispatcher();
};

#endif
