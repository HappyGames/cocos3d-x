#include "CC3HelloWorldLayer.h"
#include "Nodes/CC3Camera.h"

/**
 * Override to set up your 2D controls and other initial state, and to initialize update processing.
 *
 * For more info, read the notes of this method on CC3Layer.
 */
void CC3HelloWorldLayer::initializeControls()
{
	scheduleUpdate();
#pragma _NOTE_TODO( "setUserInteractionEnabled( bool )" )
	// setUserInteractionEnabled( true );		// Set to NO if using Gesture Recognizers.

	setTouchEnabled( true );
}

/**
 * Override to perform set-up activity prior to the scene being opened on the view, such as 
 * adding Gesture Recognizers. If you add Gesture Recognizers, set the userInteractionEnabled
 * property to NO (it is set to YES in the initializeControls method above).
 *
 * For more info, read the notes of this method on CC3Layer.
 */
void CC3HelloWorldLayer::onOpenCC3Layer()
{
	CC3Camera* cam = CC3Camera::nodeWithName( "Camera" );
}

/**
 * Override to perform tear-down activity prior to the scene disappearing.
 *
 * For more info, read the notes of this method on CC3Layer.
 */
void CC3HelloWorldLayer::onCloseCC3Layer() 
{
	
}

void CC3HelloWorldLayer::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}
