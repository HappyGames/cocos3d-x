#ifndef _CC3_HELLOWORLD_SCENE_H_
#define _CC3_HELLOWORLD_SCENE_H_
#include "Scenes/CC3Scene.h"
USING_NS_COCOS3D;

class CC3HelloWorldScene : public CC3Scene
{
	DECLARE_SUPER( CC3Scene );
public:
	CREATE_FUNC( CC3HelloWorldScene );

	virtual void		initializeScene();
	virtual void		addSceneContentAsynchronously();
	virtual void		updateBeforeTransform( CC3NodeUpdatingVisitor* visitor );
	virtual void		updateAfterTransform( CC3NodeUpdatingVisitor* visitor );
	virtual void		onOpen();
	virtual void		onClose();
	virtual void		drawSceneContentWithVisitor( CC3NodeDrawingVisitor* visitor );
	virtual void		touchEvent( GLuint touchType, const CCPoint& touchPoint );
	virtual void		nodeSelected( CC3Node* aNode, GLuint touchType, const CCPoint& touchPoint );

protected:
	void				addFloatingRing();
	void				addGround();
	void				addBrickWall();
	void				addAxesDirectionMarkers();
	void				addSkinnedRunner();
	void				addRobot();
	void				addSkyBox();
	void				addPointHose();
	void				addProjectedLabel();
	void				addBitmapLabel();

	void				configNode( CC3Node* node ); 

protected:
	CC3Node*			m_pTargetNode;

	CCPoint				_lastTouchEventPoint;
	struct timeval		_lastTouchEventTime;
};

#endif
