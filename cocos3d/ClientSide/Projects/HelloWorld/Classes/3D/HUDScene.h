#ifndef _HUD_SCENE_H_
#define _HUD_SCENE_H_
#include "cocos3d.h"
USING_NS_COCOS3D;

class CHUDScene : public CC3Scene
{
	DECLARE_SUPER( CC3Scene ); 
public:
	/** Create the camera and light. Other items are added from the main scene. */
	virtual void		initializeScene();
	/** 
	 * Since this scene is drawn as an overlay on top of the main scene, we must clear the depth
	 * buffer before drawing, so objects in this scene won't hide behind objects in the main scene.
	 */
	virtual void		drawSceneContentWithVisitor( CC3NodeDrawingVisitor* visitor );
	virtual CCOpacity	getOpacity();
	/** Whatever the opacity of this scene is set to, make the backdrop more transparent. */
	virtual void		setOpacity( CCOpacity opacity );
	/** Ensure the camera frames the globe. */
	virtual void		onOpen();

	static CHUDScene*	scene();
};


#endif
