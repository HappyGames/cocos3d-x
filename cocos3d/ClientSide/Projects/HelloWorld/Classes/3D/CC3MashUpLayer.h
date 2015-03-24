#ifndef _CC3_MASHUP_LAYER_H_
#define _CC3_MASHUP_LAYER_H_
#include "cocos3d.h"
USING_NS_COCOS3D;

class CC3MashUpScene;

/// Class forward declaration
NS_COCOS3D_BEGIN
class Joystick;
class AdornableMenuItemImage;
class AdornableMenuItemToggle;
class CCNodeAdornment;
NS_COCOS3D_END

class CC3MashUpLayer : public CC3Layer
{
	DECLARE_SUPER( CC3Layer );
public:
	CREATE_FUNC( CC3MashUpLayer );
	/**
	 * Returns the contained CC3Scene, cast into the appropriate type.
	 * This is a convenience method to perform automatic casting.
	 */
	CC3MashUpScene*				getMashUpScene();
	/**
	 * Opens a secondary heads-up-display CC3Layer that holds a CC3Scene that
	 * contains only the globe. The opening of this secondary layer is animated.
	 */
	void						openGlobeHUDFromTouchAt( const CCPoint& touchPoint );

	/** Closes the secondary HUD layer. The closing is animated. */
	void						closeGlobeHUDFromTouchAt( const CCPoint& touchPoint );

	/**
	 * If the HUD window is not open, opens it by invoking the openGlobeHUDFromTouchAt:
	 * method. If the HUD window is already open, closes it by invoking the
	 * closeGlobeHUDFromTouchAt: method.
	 *
	 * This is invoked from a touch-handing event when the globe is touched.
	 */
	void						toggleGlobeHUDFromTouchAt( const CCPoint& touchPoint );

	/**
	 * Removes the secondary HUD window from this layer once it has closed.
	 *
	 * This is invoked automatically from the CCAction that animates the closing of the HUD window.
	 */
	void						removeGlobeHUD();

	void						setCC3Scene( CC3MashUpScene* aScene );
	void						initializeControls();

	virtual void				registerWithTouchDispatcher();

	/**
	 * Updates the player (camera) direction and location from the joystick controls
	 * and then updates the 3D scene.
	 */
	void						update( GLfloat dt );

	/** The user has pressed the switch camera view button. Tell the 3D scene so it can move the camera. */
	void						switchViewSelected( CCObject* pSender );

	/** The user has pressed the invade button. Tell the 3D scene. */
	void						invade( CCObject* pSender );

	/** The user has pressed the cycle lights button. Tell the 3D scene. */
	void						cycleLights( CCObject* pSender );

	/** The user has pressed the zoom button. Tell the 3D scene. */
	void						cycleZoom( CCObject* pSender );

	/** The user has pressed the shadow button. Tell the 3D scene. */
	void						toggleShadows( CCObject* pSender );

	/**
	 * Called automatically when the contentSize has changed.
	 * Move the location joystick to keep it in the bottom right corner of this layer
	 * and the switch view button to keep it centered between the two joysticks.
	 */
	void						onContentSizeChanged();

	/**
	 * Returns a new CC3Scene containing a copy of the globe from the main scene.
	 * Sets the globe rotating and makes it semi-transparent.
	 */
	CC3Scene*					makeHUDScene();

protected:
	/** Creates the two joysticks that control the 3D camera direction and location. */
	void						addJoysticks();
	/**
	 * Creates a button that will allow the user to switch between different views of the 3D scene.
	 * The type of button depends on whether we are using Cocos2D v3, or the legacy Cocos2D v2/v1.
	 */
	void						addSwitchViewButton();
	/**
	 * Creates a button that will allow the user to create a robot invasion.
	 * The type of button depends on whether we are using Cocos2D v3, or the legacy Cocos2D v2/v1.
	 */
	void						addInvasionButton();
	/**
	 * Creates a button that will allow the user to turn the sun on or off.
	 * The type of button depends on whether we are using Cocos2D v3, or the legacy Cocos2D v2/v1.
	 */
	void						addSunlightButton();
	/**
	 * Creates a button that will allow the user to move between viewing the whole scene and
	 * viewing from the previous position.
	 * The type of button depends on whether we are using Cocos2D v3, or the legacy Cocos2D v2/v1.
	 */
	void						addZoomButton();
	/**
	 * Creates a toggle button that will allow the user to toggle shadows on and off for a selected node.
	 * The type of button depends on whether we are using Cocos2D v3, or the legacy Cocos2D v2/v1.
	 */
	void						addShadowButton();
	/**
	 * Positions the buttons between the two joysticks.
	 * This is called at initialization, and anytime the content size of the layer changes
	 * to keep the button in the correct location within the new layer dimensions.
	 */
	void						positionButtons();

	/**
	 * Positions the right-side location joystick at the right of the layer.
	 * This is called at initialization, and anytime the content size of the layer changes
	 * to keep the joystick in the correct location within the new layer dimensions.
	 */
	void						positionLocationJoystick();

	/**
	 * Adds a UI button to this layer, and returns the button. The button will display the image
	 * in the specified file, and is adorned with the specified visual adornment that will be
	 * activated when the button is touched. The button will invoke the specified callback method
	 * on this instance when the button is pressed and released by the user. The type of button
	 * used depends on whether we are using Cocos2D v3, or Cocos2D v2/v1.
	 */
	 AdornableMenuItemImage*	addButtonWithCallbackSelector( SEL_MenuHandler callBackSelector, const char* imgFileName, 
		 CCNodeAdornment* adornment );

	/**
	 * Adds a toggle UI button to this layer, and returns the button. The button will display
	 * the image in the specified file, and is adorned with the specified visual adornment that
	 * will be activated when the button is touched. The button will invoke the specified callback
	 * method on this instance when the button is pressed and released by the user. The type of
	 * button used depends on whether we are using Cocos2D v3, or Cocos2D v2/v1.
	 */
	 AdornableMenuItemToggle*	addToggleButtonWithCallbackSelector( SEL_MenuHandler callBackSelector, const char* imgFileName,
		 const char* imgLatchFileName, CCNodeAdornment* adornment );
 

protected:
	CC3Layer*					_hudLayer;
	Joystick*					_directionJoystick;
	Joystick*					_locationJoystick;

	AdornableMenuItemImage*		_switchViewButton;
	AdornableMenuItemImage*		_invasionButton;
	AdornableMenuItemImage*		_sunlightButton;
	AdornableMenuItemImage*		_zoomButton;
	AdornableMenuItemToggle*	_shadowButton;
};

#endif
