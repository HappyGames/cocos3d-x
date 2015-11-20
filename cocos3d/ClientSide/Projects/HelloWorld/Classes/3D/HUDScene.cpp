#include "HUDScene.h"

#define kGlobeName					"Globe"
#define kBackDropOpacityFactor		0.4f


void CHUDScene::initializeScene()
{
	setBackdrop( CC3Backdrop::node() );
	
	// Create the camera, place it back a bit, and add it to the scene
	CC3Camera* cam = CC3Camera::nodeWithName( "Camera" );
	cam->setLocation( cc3v( 0.0f, 0.0f, 1.0f ) );
	addChild( cam );
	
	// Create a light and attach it to the camera.
	CC3Light* lamp = CC3Light::nodeWithName( "Lamp" );
	cam->addChild( lamp );
}

void CHUDScene::drawSceneContentWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	visitor->getRenderSurface()->clearDepthContent();
	super::drawSceneContentWithVisitor( visitor );
}

CCOpacity CHUDScene::getOpacity()
{
	return m_pBackdrop->getOpacity() / kBackDropOpacityFactor;
}

void CHUDScene::setOpacity( CCOpacity opacity )
{
	super::setOpacity( opacity );
	m_pBackdrop->setOpacity( (opacity * kBackDropOpacityFactor) );
}

void CHUDScene::onOpen()
{
	getActiveCamera()->moveToShowAllOf( getNodeNamed( kGlobeName ), -0.1f );
}

CHUDScene* CHUDScene::scene()
{
	CHUDScene* pScene = new CHUDScene;
	pScene->init();
	pScene->autorelease();

	return pScene;
}