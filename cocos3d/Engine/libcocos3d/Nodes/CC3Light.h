/*
 * Cocos3D-X 1.0.0
 * Author: Bill Hollings
 * Copyright (c) 2010-2014 The Brenwill Workshop Ltd. All rights reserved.
 * http://www.brenwill.com
 *
 * Copyright (c) 2014-2015 Jason Wong
 * http://www.cocos3dx.org/
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * http://en.wikipedia.org/wiki/MIT_License
 */
#ifndef _CC3_LIGHT_H_
#define _CC3_LIGHT_H_

NS_COCOS3D_BEGIN
class CC3ShadowProtocol;
class CC3ShadowCastingVolume;
class CC3CameraShadowVolume;
class CC3StencilledShadowPainterNode;

/** Constant indicating that the light is not directional. */
static const GLfloat kCC3SpotCutoffNone = 180.0f;

/** Default ambient light color. */
static const ccColor4F kCC3DefaultLightColorAmbient = { 0.0, 0.0, 0.0, 1.0 };

/** Default diffuse light color. */
static const ccColor4F kCC3DefaultLightColorDiffuse = { 1.0, 1.0, 1.0, 1.0 };

/** Default specular light color. */
static const ccColor4F kCC3DefaultLightColorSpecular = { 1.0, 1.0, 1.0, 1.0 };

/** Default light attenuation coefficients */
static const CC3AttenuationCoefficients kCC3DefaultLightAttenuationCoefficients = {1.0, 0.0, 0.0};

/**
 * CC3Light represents the light in the 3D scene.
 *
 * CC3Light is a type of CC3Node, and can therefore participate in a structural node
 * assembly. An instance can be the child of another node, and the light itself can
 * have child nodes. For example, a light can be mounted on a boom object or camera,
 * and will move along with the parent node.
 *
 * CC3Light can be pointed so that it shines in a particular direction, or can be
 * made to track a target node as that node moves.
 *
 * To turn a CC3Light on or off, set the visible property.
 *
 * The maximum number of lights available is determined by the platform. That number can be retrieved
 * from the CC3OpenGL.sharedGL.maxNumberOfLights property. All platforms support at least eight lights.
 *
 * Lights in different scenes (different instances of CC3Scene) can have the same
 * GL lightIndex value. Applications that make use of multiple CC3Scenes, either as
 * a sequence of scenes, or as multiple scenes (and multiple CC3Layers) displayed
 * on the screen at once, can reuse a light index across the scenes.
 * The shouldCopyLightIndex property can be used to help copy lights across scenes.
 *
 * If the application uses lights in the 2D scene as well, the indexes of those lights
 * can be reserved by invoking the class method setLightPoolStartIndex:. Light indexes
 * reserved for use by the 2D scene will not be used by the 3D scene.
 */
class CC3Light : public CC3Node 
{
	DECLARE_SUPER( CC3Node );
public:
	CC3Light();
	~CC3Light();
	/** Returns whether this node is a light. Returns YES. */
	bool						isLight();

	/**
	 * The index of this light to identify it to the GL engine. This is automatically assigned
	 * during instance initialization. The value of lightIndex will be between zero and one 
	 * less than the maximium number of available lights, inclusive.
	 *
	 * The maximum number of lights available is determined by the platform. That number can be retrieved
	 * from the CC3OpenGL.sharedGL.maxNumberOfLights property. All platforms support at least eight lights.
	 */
	GLuint						getLightIndex();

	/** The ambient color of this light. Initially set to kCC3DefaultLightColorAmbient. */
	ccColor4F					getAmbientColor();
	void						setAmbientColor( const ccColor4F& color );

	/** The diffuse color of this light. Initially set to kCC3DefaultLightColorDiffuse. */
	ccColor4F					getDiffuseColor();
	void						setDiffuseColor( const ccColor4F& color );

	/** The specular color of this light. Initially set to kCC3DefaultLightColorSpecular. */
	ccColor4F					getSpecularColor();
	void						setSpecularColor( const ccColor4F& color );

	/**
	 * Indicates whether this light is directional and without a specified location.
	 * Directional-only light is good for modeling sunlight, or other flat overhead
	 * lighting. Positional lighting is good for point-source lights like a single
	 * bulb, flare, etc.
	 *
	 * The value of this property impacts features like attenuation, and the angle
	 * of reflection to the user view. A directional-only light is not subject to
	 * attenuation over distance, where an absolutely located light is. In addition,
	 * directional-only light bounces off a flat surface at a single angle, whereas
	 * the angle for a point-source light also depends on the location of the camera.
	 *
	 * The value of this property also impacts performance. Because positional light
	 * involves significantly more calculations within the GL engine, setting this
	 * property to YES (the initial value) will improve lighting performance.
	 * You should only set this property to NO if you need to make use of the
	 * positional features described above.
	 *
	 * The initial value is YES, indicating directional-only lighting.
	 */
	bool						isDirectionalOnly();
	void						setIsDirectionalOnly( bool directionalOnly );

	/**
	 * The position of this light in a global 4D homogeneous coordinate space.
	 *
	 * The X, Y & Z components of the returned 4D vector are the same as those in the globalLocation
	 * property. The W-component will be zero if the isDirectionalOnly property is set to YES, indicating
	 * that this position represents a direction. The W-component will be one if the isDirectionalOnly
	 * property is set to NO, indicating that this position represents a specific location.
	 */
	CC3Vector4					getGlobalHomogeneousPosition();

	/**
	 * Indicates the intensity distribution of the light.
	 *
	 * Effective light intensity is attenuated by the cosine of the angle between the
	 * direction of the light and the direction from the light to the vertex being lighted,
	 * raised to the power of the value of this property. Thus, higher spot exponents result
	 * in a more focused light source, regardless of the value of the spotCutoffAngle property.
	 *
	 * The value of this property must be in the range [0, 128], and is clamped to that
	 * range if an attempt is made to set the value outside this range.
	 *
	 * The initial value of this property is zero, indicating a uniform light distribution.
	 */
	GLfloat						getSpotExponent();
	void						setSpotExponent( GLfloat exponnet );

	/**
	 * Indicates the angle, in degrees, of dispersion of the light from the direction of the light.
	 * Setting this value to any angle between zero and 90 degrees, inclusive, will cause this light
	 * to be treated as a spotlight whose direction is set by the forwardDirection property of this
	 * light, and whose angle of dispersion is controlled by this property. Setting this property to
	 * any value above 90 degrees will cause this light to be treated as an omnidirectional light.
	 *
	 * This property is initially set to kCC3SpotCutoffNone (180 degrees).
	 */
	GLfloat						getSpotCutoffAngle();
	void						setSpotCutoffAngle( GLfloat spotCutoffAngle );

	/**
	 * The coefficients of the attenuation function that reduces the intensity of the light
	 * based on the distance from the light source. The intensity of the light is attenuated
	 * according to the formula 1/sqrt(a + b * r + c * r * r), where r is the radial distance
	 * from the light source, and a, b and c are the coefficients from this property.
	 *
	 * The initial value of this property is kCC3DefaultLightAttenuationCoefficients.
	 */
	CC3AttenuationCoefficients	getAttenuation();
	void						setAttenuation( const CC3AttenuationCoefficients& coefficients );

	/**
	 * When a copy is made of this node, indicates whether this node should copy the value
	 * of the lightIndex property to the new node when performing a copy of this node.
	 *
	 * The initial value of this property is NO.
	 *
	 * When this property is set to NO, and this light node is copied, the new copy will
	 * be assigned its own lightIndex, to identify it to the GL engine. This allows both
	 * lights to illuminate the same scene (instance of CC3Scene), and is the most common
	 * mechanism for assigning the lightIndex property.
	 *
	 * OpenGL ES limits the number of lights available to illuminate a single scene.
	 * Once that limit is reached, additional lights cannot be created, and attempting
	 * to copy this node will fail, returning a nil node.
	 *
	 * The maximum number of lights available is determined by the platform. That number can be retrieved
	 * from the CC3OpenGL.sharedGL.maxNumberOfLights property. All platforms support at least eight lights.
	 *
	 * When this property is set to YES, and this light node is copied, the new copy will
	 * be assigned the same lightIndex as this node. This means that the copy may not be
	 * used in the same scene as the original light, but it may be used in another scene
	 * (another CC3Scene instance).
	 *
	 * Applications that make use of multiple CC3Scenes, either as a sequence of scenes,
	 * or as multiple scenes (and multiple CC3Layers) displayed on the screen at once,
	 * can set this property to YES when making copies of a light to be placed in
	 * different CC3Scene instances.
	 */
	bool						shouldCopyLightIndex();
	void						setShouldCopyLightIndex( bool copyLightIndex );
	
	/**
	 * Initializes this unnamed instance with an automatically generated unique tag value.
	 * The tag value will be generated automatically via the method nextTag.
	 *
	 * The lightIndex property will be set to the next available GL light index.
	 * This method will return nil if all GL light indexes have been consumed.
	 *
	 * The maximum number of lights available is determined by the platform. That number can be retrieved
	 * from the CC3OpenGL.sharedGL.maxNumberOfLights property. All platforms support at least eight lights.
	 */
	virtual bool				init();

	/**
	 * Initializes this unnamed instance with the specified tag.
	 *
	 * The lightIndex property will be set to the next available GL light index.
	 * This method will return nil if all GL light indexes have been consumed.
	 *
	 * The maximum number of lights available is determined by the platform. That number can be retrieved
	 * from the CC3OpenGL.sharedGL.maxNumberOfLights property. All platforms support at least eight lights.
	 */
	void						initWithTag( GLuint aTag );

	/**
	 * Initializes this instance with the specified name and an automatically generated unique
	 * tag value. The tag value will be generated automatically via the method nextTag.
	 *
	 * The lightIndex property will be set to the next available GL light index.
	 * This method will return nil if all GL light indexes have been consumed.
	 *
	 * The maximum number of lights available is determined by the platform. That number can be retrieved
	 * from the CC3OpenGL.sharedGL.maxNumberOfLights property. All platforms support at least eight lights.
	 */
	void						initWithName( const std::string& aName );

	/**
	 * Initializes this instance with the specified tag and name.
	 *
	 * The lightIndex property will be set to the next available GL light index.
	 * This method will return nil if all GL light indexes have been consumed.
	 *
	 * The maximum number of lights available is determined by the platform. That number can be retrieved
	 * from the CC3OpenGL.sharedGL.maxNumberOfLights property. All platforms support at least eight lights.
	 */
	void						initWithTag( GLuint aTag, const std::string& aName );

	/**
	 * Initializes this unnamed instance with the specified GL light index, and an
	 * automatically generated unique tag value. The tag value will be generated
	 * automatically via the method nextTag.
	 *
	 * If multiple lights are used to illumniate a scene (a CC3Scene instance),
	 * each light must have its own GL light index. Do not assign the same light
	 * index to more than one light in a scene.
	 *
	 * This method will return nil if the specified light index is not less than the
	 * maximum number of lights available.
	 *
	 * The maximum number of lights available is determined by the platform. That number can be retrieved
	 * from the CC3OpenGL.sharedGL.maxNumberOfLights property. All platforms support at least eight lights.
	 */
	void						initWithLightIndex( GLuint ltIndx );

	/**
	 * Initializes this unnamed instance with the specified GL light index, and the
	 * specified tag.
	 *
	 * If multiple lights are used to illumniate a scene (a CC3Scene instance),
	 * each light must have its own GL light index. Do not assign the same light
	 * index to more than one light in a scene.
	 *
	 * This method will return nil if the specified light index is not less than the
	 * maximum number of lights available.
	 *
	 * The maximum number of lights available is determined by the platform. That number can be retrieved
	 * from the CC3OpenGL.sharedGL.maxNumberOfLights property. All platforms support at least eight lights.
	 */
	void						initWithTag( GLuint aTag, GLuint ltIndx );

	/**
	 * Initializes this instance with the specified GL light index, the specified name,
	 * and an automatically generated unique tag value. The tag value will be generated
	 * automatically via the method nextTag.
	 *
	 * If multiple lights are used to illumniate a scene (a CC3Scene instance),
	 * each light must have its own GL light index. Do not assign the same light
	 * index to more than one light in a scene.
	 *
	 * This method will return nil if the specified light index is not less than the
	 * maximum number of lights available.
	 *
	 * The maximum number of lights available is determined by the platform. That number can be retrieved
	 * from the CC3OpenGL.sharedGL.maxNumberOfLights property. All platforms support at least eight lights.
	 */
	void						initWithName( const std::string& aName, GLuint ltIndx );

	/**
	 * Initializes this instance with the specified GL light index, the specified name,
	 * and the specified tag.
	 *
	 * If multiple lights are used to illumniate a scene (a CC3Scene instance),
	 * each light must have its own GL light index. Do not assign the same light
	 * index to more than one light in a scene.
	 *
	 * This method will return nil if the specified light index is not less than the
	 * maximum number of lights available.
	 *
	 * The maximum number of lights available is determined by the platform. That number can be retrieved
	 * from the CC3OpenGL.sharedGL.maxNumberOfLights property. All platforms support at least eight lights.
	 */
	void						initWithTag( GLuint aTag, const std::string& aName, GLuint ltIndx );

	/**
	 * Allocates and initializes an autoreleased unnamed instance with the specified
	 * GL light index, and an automatically generated unique tag value. The tag value
	 * will be generated automatically via the method nextTag.
	 *
	 * If multiple lights are used to illumniate a scene (a CC3Scene instance),
	 * each light must have its own GL light index. Do not assign the same light
	 * index to more than one light in a scene.
	 *
	 * This method will return nil if the specified light index is not less than the
	 * maximum number of lights available.
	 *
	 * The maximum number of lights available is determined by the platform. That number can be retrieved
	 * from the CC3OpenGL.sharedGL.maxNumberOfLights property. All platforms support at least eight lights.
	 */
	static CC3Light*			lightWithLightIndex( GLuint ltIndx );

	/**
	 * Allocates and initializes an autoreleased unnamed instance with the specified
	 * GL light index, and the specified tag.
	 *
	 * If multiple lights are used to illumniate a scene (a CC3Scene instance),
	 * each light must have its own GL light index. Do not assign the same light
	 * index to more than one light in a scene.
	 *
	 * This method will return nil if the specified light index is not less than the
	 * maximum number of lights available.
	 *
	 * The maximum number of lights available is determined by the platform. That number can be retrieved
	 * from the CC3OpenGL.sharedGL.maxNumberOfLights property. All platforms support at least eight lights.
	 */
	static CC3Light*			lightWithTag( GLuint aTag, GLuint ltIndx );

	/**
	 * Allocates and initializes an autoreleased instance with the specified GL light
	 * index, the specified name, and an automatically generated unique tag value.
	 * The tag value will be generated automatically via the method nextTag.
	 *
	 * If multiple lights are used to illumniate a scene (a CC3Scene instance),
	 * each light must have its own GL light index. Do not assign the same light
	 * index to more than one light in a scene.
	 *
	 * This method will return nil if the specified light index is not less than the
	 * maximum number of lights available.
	 *
	 * The maximum number of lights available is determined by the platform. That number can be retrieved
	 * from the CC3OpenGL.sharedGL.maxNumberOfLights property. All platforms support at least eight lights.
	 */
	static CC3Light*			lightWithName( const std::string& aName, GLuint ltIndx );

	/**
	 * Allocates and initializes an autoreleased instance with the specified GL light
	 * index, the specified name, and the specified tag.
	 *
	 * If multiple lights are used to illumniate a scene (a CC3Scene instance),
	 * each light must have its own GL light index. Do not assign the same light
	 * index to more than one light in a scene.
	 *
	 * This method will return nil if the specified light index is not less than the
	 * maximum number of lights available.
	 *
	 * The maximum number of lights available is determined by the platform. That number can be retrieved
	 * from the CC3OpenGL.sharedGL.maxNumberOfLights property. All platforms support at least eight lights.
	 */
	static CC3Light*			lightWithTag( GLuint aTag, const std::string& aName, GLuint ltIndx );

	/**
	 * Indicates whether this light should cast shadows even when invisible.
	 *
	 * Normally, when a light is turned off, any shadows cast by that light should
	 * disappear as well. However, there are certain lighting situations where you
	 * might want a light to cast shadows, even when turned off, such as using one
	 * light to accent the shadows cast by another light that has different ambient
	 * or diffuse lighting characteristics.
	 *
	 * The initial value of this propety is NO.
	 *
	 * Setting this value sets the same property on any descendant mesh and light nodes.
	 */
	bool						shouldCastShadowsWhenInvisible();
	void						setShouldCastShadowsWhenInvisible( bool shouldCast );

	/**
	 * The shadows cast by this light.
	 *
	 * If this light is casting no shadows, this property will be nil.
	 */
	CCArray*					getShadows();

	/**
	 * Adds a shadow to the shadows cast by this light.
	 *
	 * This method is invoked automatically when a shadow is added to a mesh node.
	 * Usually, the application never needs to invoke this method directly.
	 */
	void						addShadow( CC3ShadowVolumeMeshNode* shadowNode );

	/** Removes a shadow from the shadows cast by this light. */
	void						removeShadow( CC3ShadowVolumeMeshNode* shadowNode );

	/**
	 * Returns whether this light is casting shadows.
	 *
	 * It is if any shadows have been added and not yet removed.
	 */
	bool						hasShadows();

	/** Update the shadows that are cast by this light. */
	void						updateShadows();

	/** Draws any shadows cast by this light. */
	void						drawShadowsWithVisitor( CC3NodeDrawingVisitor* visitor );

	/**
	 * A specialized bounding volume that encloses a volume that includes the camera
	 * frustum plus the space between the camera frustum and this light.
	 *
	 * Nodes that intersect this volume will cast a shadow from this light into the
	 * camera frustum, and that shadow will be visible. Shadows cast by nodes outside
	 * this volume will not intersect the frustum and will not be visible.
	 *
	 * This volume is used to cull the updating and drawing of shadows that will not
	 * be visible, to enhance performance.
	 *
	 * If not set directly, this property is lazily created when a shadow is added.
	 * If no shadow has been added, this property will return nil.
	 */
	CC3ShadowCastingVolume*		getShadowCastingVolume();
	void						setShadowCastingVolume( CC3ShadowCastingVolume* pVolume );

	/**
	 * A specialized bounding volume that encloses a pyramidal volume between the
	 * view plane (near clipping plane) of the camera, and this light.
	 *
	 * Nodes that intersect this volume will cast a shadow from that light across
	 * the camera. The shadow volume of nodes that cast a shadow across the camera
	 * view plane are rendered differently than shadow volumes for nodes that do
	 * not cast their shadow across the camera.
	 *
	 * If not set directly, this property is lazily created when a shadow is added.
	 * If no shadow has been added, this property will return nil.
	 */
	CC3CameraShadowVolume*		getCameraShadowVolume();
	void						setCameraShadowVolume( CC3CameraShadowVolume* pVolume );

	/**
	 * The mesh node used to draw the shadows cast by any shadow volumes that have
	 * been added to mesh nodes for this light.
	 *
	 * Shadow volumes are used to define a stencil that is then used to draw dark
	 * areas onto the viewport where mesh nodes are casting shadows. This painter
	 * is used to draw those dark areas where the stencil indicates.
	 *
	 * If not set directly, this property is lazily created when a shadow is added.
	 * If no shadow has been added, this property will return nil.
	 */
	CC3StencilledShadowPainterNode*		getStencilledShadowPainter();
	void						setStencilledShadowPainter( CC3StencilledShadowPainterNode* node );

	/**
	 * This property is used to adjust the shadow intensity as calculated when the 
	 * updateRelativeIntensityFrom: method is invoked. This property increases flexibility
	 * by allowing the shadow intensity to be ajusted relative to that calculated value to
	 * improve realisim.
	 *
	 * The intensity of shadows cast by this light is calculated by comparing the intensity of
	 * the diffuse component of this light against the total ambient and diffuse illumination
	 * from all lights, to get a measure of the fraction of total scene illumination that is
	 * contributed by this light.
	 *
	 * Using this technique, the presence of multiple lights, or strong ambient light, will
	 * serve to lighten the shadows cast by any single light. A single light with no ambient
	 * light will cast completely opaque, black shadows.
	 *
	 * That fraction, representing the fraction of overall light coming from this light, is
	 * then multiplied by the value of this property to determine the intensity (opacity) of
	 * the shadows cast by this light.
	 *
	 * This property must be zero or a positive value. A value between zero and one will serve
	 * to to lighten the shadow, relative to the shadow intensity (opacity) calculated from the
	 * relative intensity of this light, and a value of greater than one will serve to darken
	 * the shadow, relative to that calculated intensity.
	 *
	 * The initial value of this property is one, meaning that the shadow intensity
	 * calculated from the relative intensity of this light will be used without adjustment.
	 */
	GLfloat						getShadowIntensityFactor();
	void						setShadowIntensityFactor( GLfloat factor );

	/**
	 * Updates the relative intensity of this light, as compared to the specified
	 * total scene illumination.
	 *
	 * Certain characteristics, such as shadow intensities, depend on the relative
	 * intensity of this light, relative to the total intensity of all lights in
	 * the scene.
	 *
	 * Sets the intensity of shadows cast by this light by comparing the intensity of
	 * the diffuse component of this light against the total ambient and diffuse
	 * illumination from all lights, to get a measure of the fraction of total scene
	 * illumination that is contributed by this light.
	 *
	 * Using this technique, the presence of multiple lights, or strong ambient light,
	 * will serve to lighten the shadows cast by any single light. A single light with
	 * no ambient light will cast completely black opaque shadows.
	 *
	 * That calculated fraction is then multiplied by the value of the shadowIntensityFactor
	 * property to determine the intensity (opacity) of the shadows cast by this light.
	 * The shadowIntensityFactor increases flexibility by allowing the shadow intensity
	 * to be adjusted relative to the calculated value to improve realisim.
	 *
	 * This method is invoked automatically when any of the the ambientColor, diffuseColor,
	 * visible, or shadowIntensityFactor properties of any light in the scene is changed,
	 * or if the ambientLight property of the CC3Scene is changed.
	 */
	void						updateRelativeIntensityFrom( const ccColor4F& totalLight );

	/**
	 * If this light is visible, turns it on by enabling this light in the GL engine,
	 * and then applies the properties of this light to the GL engine.
	 *
	 * This method is invoked automatically by CC3Scene near the beginning of each frame
	 * drawing cycle. Usually, the application never needs to invoke this method directly.
	 */
	void						turnOnWithVisitor( CC3NodeDrawingVisitor* visitor );

	/**
	 * Turns this light off on by disabling this light in the GL engine.
	 *
	 * This method is invoked automatically by CC3Scene at the end of each frame drawing cycle.
	 * Usually, the application never needs to invoke this method directly.
	 */
	void						turnOffWithVisitor( CC3NodeDrawingVisitor* visitor );

	/**
	 * Returns the number of lights that have already been instantiated (and not yet deallocated).
	 *
	 * The maximum number of lights available is determined by the platform. That number can be retrieved
	 * from the CC3OpenGL.sharedGL.maxNumberOfLights property. All platforms support at least eight lights.
	 */
	static GLuint				getLightCount();

	/**
	 * Indicates the smallest index number to assign to a 3D light.
	 * 
	 * See the description of the setLightPoolStartIndex: method for more information on this value.
	 */
	static GLuint				getLightPoolStartIndex();

	/**
	 * Sets the smallest index number to assign to a 3D light. This value should be between
	 * zero inclusive and CC3OpenGL.sharedGL.maxNumberOfLights exclusive.
	 *
	 * If the 2D scene uses lights, setting this value to a number above zero will reserve
	 * the indexes below this number for the 2D scene and those indexes will not be used in
	 * lights in the 3D scene.
	 * 
	 * This value defaults to zero. If your application requires light indexes to be reserved
	 * and not assigned in the 3D scene, set this value.
	 */
	static void					setLightPoolStartIndex( GLuint newStartIndex );

	/**
	 * Disables the lights that were reserved for the 2D scene by setLightPoolStartIndex:.
	 *
	 * This method is invoked automatically by CC3Scene near the beginning of each frame
	 * drawing cycle. Usually, the application never needs to invoke this method directly.
	 */
	static void					disableReservedLightsWithVisitor( CC3NodeDrawingVisitor* visitor );

	/**
	 * Invoked when this light is being deallocated. Removes all associated
	 * shadow nodes from this scene, which also removes the shadows array.
	 */
	void						cleanupShadows();
	/** Returns the specified light index to the pool. */
	void						returnLightIndex( GLuint aLightIndex );

	/** Overridden to return NO so that the forwardDirection aligns with the negative-Z-axis. */
	bool						shouldReverseForwardDirection();

	void						setVisible( bool isVisible );

	/** Returns diffuse color. */
	CCColorRef					getColor();
	// Set both diffuse and ambient colors, retaining the alpha of each
	void						setColor( const CCColorRef& color );

	/** Returns diffuse alpha. */
	CCOpacity					getOpacity();

	/** Set opacity of all colors, retaining the colors of each. */
	void						setOpacity( CCOpacity opacity );

	/**
	 * Assigns and returns the next available light index from the pool.
	 * If no more lights are available, returns kCC3MaxGLuint.
	 */
	GLuint						nextLightIndex();
	static CC3Light*			nodeWithLightIndex( GLuint ltIndx );

	// Template method that populates this instance from the specified other instance.
	// This method is invoked automatically during object copying via the copyWithZone: method.
	// The lightIndex property is NOT copied, since we want each light to have a different value.
	void						populateFrom( CC3Light* another );
	virtual CCObject*			copyWithZone( CCZone* zone );

	/** Scaling does not apply to lights. */
	void						applyScalingTo( CC3Matrix* matrix );

	/**
	 * Scaling does not apply to lights. Return the globalScale of the parent node,
	 * or unit scaling if no parent.
	 */
	CC3Vector					getGlobalScale();

	/** Overridden to update the camera shadow frustum with the global location of this light */
	void						markTransformDirty();

	/**
	 * Template method that sets the position of this light in the GL engine to the value of
	 * the globalHomogeneousPosition property of this node.
	 */	
	virtual void				applyPositionWithVisitor( CC3NodeDrawingVisitor* visitor );

	/**
	 * Template method that sets the spot direction, spot exponent, and spot cutoff angle of this light
	 * in the GL engine to the values of the globalForwardDirection, spotExponent and spotCutoffAngle
	 * properties of this node, respectively. The direction and exponent are only valid if a cutoff has
	 * been specified and less than 90 degrees, otherwise the light is treated as omni directional.
	 * OpenGL ES only supports angles less than 90 degrees, so anything above is treated as omni directional.
	 */
	virtual void				applyDirectionWithVisitor( CC3NodeDrawingVisitor* visitor );

	/**
	 * Template method that sets the light intensity attenuation characteristics
	 * in the GL engine from the attenuation property of this light.
	 */
	virtual void				applyAttenuationWithVisitor( CC3NodeDrawingVisitor* visitor );

	/**
	 * Template method that sets the ambient, diffuse and specular colors of this light
	 * in the GL engine to the values of the ambientColor, diffuseColor and specularColor
	 * properties of this node, respectively.
	 */
	virtual void				applyColorWithVisitor( CC3NodeDrawingVisitor* visitor );

	/**
	 * Invoked when a shadow is being added on a background thread, and this parent node is
	 * already part of the scene.
	 *
	 * Since the scene may be in the process of being rendered, the shadow is not added immediately.
	 * Instead, all GL activity on this thread is allowed to finish, to ensure all GL components of
	 * the shadow node are in place, and then an operation to add the specified shadow is queued to
	 * the thread that is performing rendering.
	 */
	void						addShadowFromBackgroundThread( CC3ShadowVolumeMeshNode* aShadowNode );

	/** Adds the specified shadow to this light without queuing. */
	void						addShadowNow( CC3ShadowVolumeMeshNode* aShadowNode );

	/**
	 * If there are shadows and the shadow casting volume has not been added, add it now,
	 * and tell the camera to let the shadow casting volume know whenever the camera moves
	 * so the shadow casting volume can determine which objects are casting shadows that
	 * are visible within the camera frustum.
	 *
	 * If there are no more shadows, disconnect the shadow casting volume from the camera,
	 * and remove the shadow casting volume.
	 */
	void						checkShadowCastingVolume();
	void						checkCameraShadowVolume();
	void						checkStencilledShadowPainter();
	/** Turns stenciling back off. */
	void						cleanupStencilParameters( CC3NodeDrawingVisitor* visitor );
	/** Draws the clip-space rectangle on the screen, coloring only those pixels where the stencil is non-zero. */
	void						paintStenciledShadowsWithVisitor( CC3NodeDrawingVisitor* visitor );
	/**
	 * Turns on stenciling and ensure the stencil buffer can be updated.
	 * Turns off writing to the color buffer, because the shadow volumes themselves are invisible.
	 */
	void						configureStencilParameters( CC3NodeDrawingVisitor* visitor );

	static CC3Light*			nodeWithName( const std::string& name );

protected:
	CC3ShadowCastingVolume*		_shadowCastingVolume;
	CC3CameraShadowVolume*		_cameraShadowVolume;
	CC3StencilledShadowPainterNode* _stencilledShadowPainter;
	CCArray*					_shadows;
	ccColor4F					_ambientColor;
	ccColor4F					_diffuseColor;
	ccColor4F					_specularColor;
	CC3AttenuationCoefficients	_attenuation;
	GLfloat						_spotExponent;
	GLfloat						_spotCutoffAngle;
	GLfloat						_shadowIntensityFactor;
	GLuint						_lightIndex;
	bool						_isDirectionalOnly : 1;
	bool						_shouldCopyLightIndex : 1;
	bool						_shouldCastShadowsWhenInvisible : 1;
};

/**
 * The behaviour required by objects that represent shadows cast by a light.
 *
 * CAUTION: The signature of this protocol may evolve as additional shadowing
 *          techniques are introduced.
 */
class CC3ShadowProtocol/* : public CC3NodeTransformListenerProtocol*/
{
public:	
	/** The light casting this shadow. */
	virtual CC3Light*			getLight() { return NULL; }
	virtual void				setLight( CC3Light* light ) { }

	/**
	 * Updates the shape and location of the shadow.
	 *
	 * This is invoked automatically by the light during each update frame
	 * to udpate the shape and location of the shadow.
	 */
	virtual void				updateShadow() {  }
};

/**
 * A bounding volume that encloses a volume between a light and all or part of
 * the frustum of the camera. This is an abstract class. Subclasses will define
 * the actual appropriate bounding volume.
 *
 * As a bounding volume, this class supports methods for testing whether
 * locations, rays, shapes, and other bounding volumes intersect its volume.
 */
class CC3LightCameraBridgeVolume : public CC3BoundingVolume, public CC3NodeTransformListenerProtocol
{
	DECLARE_SUPER( CC3BoundingVolume );
public:
	CC3LightCameraBridgeVolume();
	virtual ~CC3LightCameraBridgeVolume();
	/**
	 * Returns the number of vertices in the array returned by the vertices property.
	 *
	 * The value returned depends on whether the light has a specific location,
	 * or is directional. If the light is directional, the location of the light
	 * is at infinity, and is not used when comparing the vertices with other
	 * bounding volumes.
	 *
	 * Consequently, if the light has a specific location, that location will be
	 * included in the array returned by the vertices property, and the value
	 * returned by this property will reflect that. If the light is directional,
	 * the light location will not be included in the array returned by the vertices
	 * property, and the value returned by this property reflects that, and will be
	 * one less than if the light has a specific location.
	 */
	GLuint						getVertexCount();

	CC3Light*					getLight();
	void						setLight( CC3Light* light );

	/** Overridden to include the homogeneous location of the light into the vertex test. */
	bool						areAllVerticesInFrontOf( const CC3Plane& plane );

	/**
	 * Returns whether the light is located in front of the plane.
	 *
	 * Performs a 4D dot-product between the plane definition, and the homegeneous location
	 * of the light, which magically works for both directional and positional lights.
	 */
	bool						isLightInFrontOfPlane( const CC3Plane& aPlane );

	void						nodeWasDestroyed( CC3Node* aNode );
	/**
	 * Callback indicating that the camera has been transformed.
	 * Sets the camera frustum (in case the camera has changed), and marks this volume as dirty.
	 */
	void						nodeWasTransformed( CC3Node* aNode );

	void						populateFrom( CC3LightCameraBridgeVolume* another );
	virtual CCObject*			copyWithZone( CCZone* pZone );

	/**
	 * Returns the position of the light, as a 3D vector.
	 *
	 * This could be a location or direction, depending on whether the
	 * 4D homogeneous location has a definite location, or is directional.
	 */
	CC3Vector					getLightPosition();

protected:
	CC3Light*					_light;
};

/**
 * A bounding volume that encloses a volume that includes the camera frustum plus
 * the space between the camera frustum and a light.
 *
 * Nodes that intersect this volume will cast a shadow from that light into the frustum,
 * and that shadow will be visible. Shadows cast by nodes outside this volume will not
 * intersect the frustum and will not be visible. This volume is used to cull the
 * updating and drawing of shadows, that will not be visible, to improve performance.
 *
 * The number of planes in this bounding volume will be between six and eleven, depending
 * on where the light is located. The number of vertices will be between five and nine.
 *
 * The shadow casting volume is a type of bounding volume and therefore supports methods for
 * testing whether locations, rays, shapes, and other bounding volumes intersect its volume.
 */
class CC3ShadowCastingVolume : public CC3LightCameraBridgeVolume
{
public:
	static CC3ShadowCastingVolume* boundingVolume();
	CC3ShadowCastingVolume();

	void						buildPlanes();

	void						checkPlane( const CC3Plane& aPlane, const CC3Plane& edgePlane1, const CC3Vector& v1, 
		const CC3Plane& edgePlane2, const CC3Vector& v2,
		const CC3Plane& edgePlane3, const CC3Vector& v3,
		const CC3Plane& edgePlane4, const CC3Vector& v4 );
	void						checkPlaneEdge( const CC3Plane& edgePlane, const CC3Vector& v1, const CC3Vector& v2 );

	/** Adds the specified plane to the planes array, and increments the planeCount property. */
	void						addPlane( const CC3Plane& aPlane );

	/**
	 * If the specified vertex does not already exist in the vertices array,
	 * it is added to the array, and the vertexCount property is incremented.
	 */
	void						addUniqueVertex( const CC3Vector& aLocation );

	GLuint						getVertexCount();
	CC3Vector*					getVertices();
	GLuint						getPlaneCount();
	CC3Plane*					getPlanes();

protected:
	CC3Plane					_planes[11];
	CC3Vector					_vertices[9];
	GLuint						_planeCount;
	GLuint						_vertexCount;
};

/**
 * A bounding volume that encloses a pyramidal volume between the view plane
 * (near clipping plane) of the camera, and a light.
 *
 * Nodes that intersect this volume will cast a shadow from that light across the camera.
 * The shadow volume of nodes that cast a shadow across the camera view plane are rendered
 * differently than shadow volumes for nodes that do not cast their shadow across the camera.
 *
 * The camera shadow volume is a type of bounding volume and therefore supports methods for
 * testing whether locations, rays, shapes, and other bounding volumes intersect its volume.
 */
class CC3CameraShadowVolume : public CC3LightCameraBridgeVolume 
{
public:
	static CC3CameraShadowVolume* boundingVolume();

	/** The frustum vertex on the near clipping plane of the camera, at the intersection of the top and left sides. */
	CC3Vector					getTopLeft();

	/** The frustum vertex on the near clipping plane of the camera, at the intersection of the top and right sides. */
	CC3Vector					getTopRight();

	/** The frustum vertex on the near clipping plane of the camera, at the intersection of the bottom and left sides. */
	CC3Vector					getBottomLeft();

	/** The frustum vertex on the near clipping plane of the camera, at the intersection of the bottom and right sides. */
	CC3Vector					getBottomRight();

	/** The clip plane at the top of this frustum, in global coordinates. */
	CC3Plane					getTopPlane();

	/** The clip plane at the bottom of this frustum, in global coordinates. */
	CC3Plane					getBottomPlane();

	/** The clip plane at the left side of this frustum, in global coordinates. */
	CC3Plane					getLeftPlane();

	/** The clip plane at the right side of this frustum, in global coordinates. */
	CC3Plane					getRightPlane();

	/** The clip plane at the near end of this frustum, in global coordinates. */
	CC3Plane					getNearPlane();

	/** The clip plane at the far end of this frustum, in global coordinates. */
	CC3Plane					getFarPlane();

	/**
	 * Builds the planes of the pyramid by taking points on the near clipping plane
	 * as the base of the pyramid, and the light location as the apex of the pyramid.
	 *
	 * If the light is directional, then the pyramid will actually become an elongated
	 * box extending out to infinity, with opposite sides parallel.
	 *
	 * If the light is actually behind the camera, then the planes are adjusted so that
	 * they are oriented correctly for a pyramid behind the near camera plane.
	 *
	 * All planes have their normals pointing outward.
	 */
	void						buildPlanes();

	/** Updates the vertices from the camera frustum. */
	void						buildVolume();

	CC3Plane*					getPlanes();
	GLuint						getPlaneCount();
	CC3Vector*					getVertices();
	GLuint						getVertexCount();

protected:
	CC3Plane					_planes[6];
	CC3Vector					_vertices[5];
};

NS_COCOS3D_END

#endif
