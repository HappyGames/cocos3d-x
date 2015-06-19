#include "CC3MashUpScene.h"
#include "label_nodes/CCLabelTTF.h"
#include "support/CCPointExtension.h"
#include "CC3MashUpLayer.h"
#include "textures/CCTextureCache.h"

USING_NS_COCOS3D;

// File names
#define kRobotPODFile					"IntroducingPOD_float.pod"
#define kBeachBallPODFile				"models/BeachBall.pod"
#define kGlobeTextureFile				"earthmap1k.jpg"
#define kMascotPODFile					"cocos3dMascot.pod"
#define kDieCubePODFile					"models/DieCube.pod"
#define kGroundTextureFile				"Grass.jpg"
#define kSignTextureFile				"Crate.png"
#define kSignStampTextureFile			"Stamp.png"
#define kSignStampNormalsTextureFile	"Stamp-nm.png"
#define kHeadPODFile					"Head.pod"
#define kHeadTextureFile				"Head_diffuse.png"
#define kHeadBumpFile					"Head_clonespace.png"
#define kCubeTextureFile				"BoxTexture.png"
#define kBrickTextureFile				"Bricks-Red.jpg"
#define kRunningManPODFile				"RunningMen/man.pod"
#define kMalletPODFile					"mallet.pod"
#define kPointParticleTextureFile		"fire.ppng"
#define kMeshParticleTextureFile		"BallBoxTexture.png"
#define kReflectiveMaskPODFile			"Masks/ReflectiveMask.pod"
#define kEtchedMaskPODFile				"Masks/EtchedMask.pod"
#define kReflectivePFXFile				"Masks/ReflectiveEffects.pfx"
#define kEtchedPFXFile					"EtchedEffects.pfx"
#define kEtchedMaskPFXEffect			"EtchedEffect"
#define kTVPODFile						"TV/samsung_tv-med.pod"
#define kTVTestCardFile					"TV/TVTestCard.jpg"
#define kPostProcPFXFile				"PostProc.pfx"

// Model names
#define kLandingCraftName				"LandingCraft"
#define kPODRobotRezNodeName			"RobotPODRez"
#define kPODLightName					"FDirect01"
#define kPODCameraName					"Camera01"
#define kRobotTopArm					"TopArm"
#define kRobotBottomArm					"BottomArm"
#define kRobotCylinder					"Cylinder01"
#define kRobotBase						"GeoSphere01"
#define kRobotCameraName				"Camera01"
#define kBeachBallRezNodeName			"BeachBallPODRez"
#define kBeachBallName					"BeachBall"
#define kBeachBallWhiteSegment			"BeachBall-submesh0"
#define kGlobeName						"Globe"
#define kDieCubeName					"DieCube"
#define kDieCubePODName					"Cube"
#define kTexturedTeapotName				"Teapot"
#define kRainbowTeapotName				"RainbowTeapot"
#define kTeapotHolderName				"TeapotHolder"
#define kTeapotRedName					"TeapotRed"
#define kTeapotGreenName				"TeapotGreen"
#define kTeapotBlueName					"TeapotBlue"
#define kTeapotWhiteName				"TeapotWhite"
#define kTeapotOrangeName				"TeapotOrange"
#define	kBillboardName					"DizzyLabel"
#define kBitmapLabelName				"BitmapLabel"
#define	kSunName						"Sun"
#define kSpotlightName					"Spotlight"
#define kBeachName						"Beach"
#define kGroundName						"Ground"
#define kFloaterName					"Floater"
#define kMascotName						"cocos2d_3dmodel_unsubdivided"
#define kDistractedMascotName			"DistractedMascot"
#define kSignName						"MultiTextureSign"
#define kSignLabelName					"SignLabel"
#define kPODHeadRezNodeName				"HeadPODRez"
#define kFloatingHeadName				"head03low01"
#define kBumpMapLightTrackerName		"BumpMapLightTracker"
#define kExplosionName					"Explosion"
#define kPointHoseEmitterName			"PointHose"
#define kMeshHoseEmitterName			"MeshHose"
#define kTexturedCubeName				"TexturedCube"
#define kBrickWallName					"BrickWall"
#define kMalletName						"Ellipse01"
#define kRunningTrackName				"RunningTrack"
#define kRunnerName						"Runner"
#define kRunnerCameraName				"Camera01"
#define kRunnerLampName					"Spot01"
#define kLittleBrotherName				"LittleBrother"
#define kTVName							"Television"
#define kTVScreenName					"TVScreen"

#define	kMultiTextureCombinerLabel		"Multi-texture combiner function: %s"

#define kCameraMoveDuration				3.0f
#define kTeapotRotationActionTag		1
#define kSkyColor						ccc4f(0.4f, 0.5f, 0.9f, 1.0f)
#define kFadeInDuration					1.0f
#define kNoFadeIn						0.0f

#define kFlappingActionTag				77
#define kGlidingActionTag				78


// Size of the television
#define kTVScale 40
static CC3IntSize kTVTexSize = { (16 * kTVScale), (9 * kTVScale) };

// Locations for the brick wall in open and closed position
static CC3Vector kBrickWallOpenLocation( -190, 150, -840 );
static CC3Vector kBrickWallClosedLocation( -115, 150, -765 );

CC3MashUpScene::CC3MashUpScene()
{
	_robotLamp = NULL;
	_teapotWhite = NULL;
	_teapotSatellite = NULL;
	_teapotTextured = NULL;
	_bumpMapLightTracker = NULL;
	_brickWall = NULL;
}

CC3MashUpScene::~CC3MashUpScene()
{
	CC_SAFE_RELEASE( _tvDrawingVisitor );
}

CC3MashUpLayer* CC3MashUpScene::getPrimaryCC3MashUpLayer()
{
	return _primaryCC3MashUpLayer;
}

void CC3MashUpScene::setPrimaryCC3MashUpLayer( CC3MashUpLayer* pLayer )
{
	_primaryCC3MashUpLayer = pLayer;
}

void CC3MashUpScene::initializeScene()
{	
	// Create the camera, place it back a bit, and add it to the scene
	CC3Camera* cam = CC3Camera::nodeWithName( "Camera" );
	cam->setLocation( cc3v( 0.0, 50.0, 250.0 ) );
	cam->setFarClippingDistance( 4000 );

//	addChild( cam );

	initCustomState();			// Set up any initial state tracked by this subclass
	
//	preloadAssets();			// Loads, compiles, links, and pre-warms all shader programs
								// used by this scene, and certain textures.
	
	addBackdrop();				// Add a static solid sky-blue backdrop, or optional textured backdrop.

	addGround();				// Add a ground plane to provide some perspective to the user

//	addSkyBox();				// Add a skybox around the scene. This is the skybox that is reflected
								// in the reflective runner added in the addSkinnedRunners method

	addRobot();					// Add an animated robot arm, a light, and a camera. This POD file
								// contains the primary camera of this scene.
	addProjectedLabel();		// Attach a text label to the hand of the animated robot.
	
//	addPointParticles();		// Uncomment to add a platform of multi-colored, light-interactive,
							// point particles hanging in the scene.
	
//	addMeshParticles();		// Uncomment to add a platform of multi-colored, mesh particles
							// hanging in the scene.
	
	addPointHose();			// Attach a point particle hose to the hand of the animated robot.
							// The hose is turned on and off when the robot arm is touched.
	
	addMeshHose();				// Attach a point particle hose to the hand of the animated robot.
							// The hose is turned on and off when the robot arm is touched.
	
	addSun();					// Add a Cocos2D particle emitter as the sun in the sky.
	
	addSpotlight();			// Add a spotlight to the camera.
							// This spotlight will be turned on when the sun is turned off.
	
	addLightProbes();			// Adds light probes to the scene, as an alternate to using lights.
							// Using the light probes can be turned on and off.
	
	configureLighting();		// Set up the lighting

	configureCamera();			// Check out some interesting camera options.
	
	// Configure all content added so far in a standard manner. This illustrates how CC3Node
	// properties and methods can be applied to large assemblies of nodes, and even the entire
	// scene itself, allowing us to perform this only once, for all current scene content.
	// For content that is added dynamically after this initial content, this method will also
	// be invoked on each new content component.
	configureForScene( this, kNoFadeIn );
}

void CC3MashUpScene::addSceneContentAsynchronously()
{
	pauseDramatically();
	addAxisMarkers();			// Add colored teapots to mark each coordinate axis
		
	pauseDramatically();
	addLightMarker();			// Add a small white teapot to show the direction toward the light

	pauseDramatically();
	addBitmapLabel();			// Add a bitmapped string label

	pauseDramatically();
	addSkinnedMallet();			// Adds a flexible mallet to the scene, showing bone skinning.

	pauseDramatically();
	addSkinnedRunners();		// Adds two running figures to the scene, showing bone skinning.

	pauseDramatically();
	addDieCube();				// Add a game die whose rotation is controlled by touch-swipe user action

	pauseDramatically();
	addTexturedCube();			// Add another cube, this one textured, below the die cube.

	pauseDramatically();
	addGlobe();					// Add a rotating globe from a parametric sphere covered by a texture

	pauseDramatically();
	addFloatingRing();			// Add a large yellow band floating above the ground, using a texture
								// containing transparency. The band as a whole fades in and out
								// periodically. This demonstrates managing opacity and translucency
								// at both the texture and material level.


	pauseDramatically();
	addBeachBall();				// Add a transparent bouncing beach ball...exported from Blender

	pauseDramatically();
	addTelevision();			// Add a television showing the view from the runner camera
								// This demonstrates dynamic rendering-to-texture capabilities.
								// Must be added after the skinned runners.

	pauseDramatically();
	addTeapotAndSatellite();	// Add a large textured teapot with a smaller satellite teapot

	pauseDramatically();
	addBrickWall();				// Add a brick wall that can block the path of the satellite teapot
								// This must happen after camera is loaded (in addRobot).

	pauseDramatically();
	addWoodenSign();			// Add the multi-texture wooden sign.
								// This must happen after camera is loaded (in addRobot).

	pauseDramatically();
	addFloatingHead();			// Add the bump-mapped floating head.
								// This must happen after camera is loaded (in addRobot).

	pauseDramatically();
	addReflectiveMask();		// Adds a floating mask that uses GLSL shaders loaded via a PowerVR
								// PFX file. Under OpenGL ES 1.1, mask appears with a default texture.

	pauseDramatically();
	addEtchedMask();			// Adds a floating mask that uses GLSL shaders loaded via a PowerVR
								// PFX file. Under OpenGL ES 1.1, mask appears with a default texture.

	pauseDramatically();
	addMascots();				// Add the Cocos3D mascot.

	pauseDramatically();
	addDragon();				// Add a flying dragon that demos blending between animation tracks

	// Log a list of the shader programs that are being used by the scene. During development,
	// we can use this list as a starting point for populating the preloadAssets method.
	CC3_TRACE("The following list contains the shader programs currently in use in this scene."
		   " You can copy and paste much of this list into the preloadAssets method"
		   " in order to pre-load the shader programs during scene initialization. %s",
		   CC3ShaderProgram::loadedProgramsDescription().c_str());

	// Log a list of the PFX resources that are being used by the scene. During development, we can
	// use this list as a starting point for adding PFX files to the preloadAssets method.
	// When initially building this list, set the CC3Resource.isPreloading to YES and leave it there.
	CC3_TRACE("The following list contains the resource files currently in use in this scene."
		   " You can copy the PFX resources from this list and paste them into the"
		   " preloadAssets method, in order to pre-load additional shader programs"
		   " that originate in PFX files, during scene initialization. %s",
		   CC3PFXResource::cachedResourcesDescription().c_str());

	// Remove the pre-loaded PFX resources, now that we no longer need them.
	// Other weakly-cached PFX resources will have been automatically removed already.
	CC3PFXResource::removeAllResources();

	CC3_TRACE( "Finished loading on background thread!" );
}

void CC3MashUpScene::pauseDramatically()
{
#if 0
	if (!CC3OpenGL.sharedGL.isRenderingContext) {
		NSTimeInterval pauseDuration = 0.25f;
		LogRez(@"Pausing for %i milliseconds before loading next resource", (int)(pauseDuration * 1000));
		[NSThread sleepForTimeInterval: pauseDuration];
	}
#endif
}

void CC3MashUpScene::initCustomState()
{
	_isManagingShadows = false;
	_playerDirectionControl = CCPointZero;
	_playerLocationControl = CCPointZero;
	
	// The order in which meshes are drawn to the GL engine can be tailored to your needs.
	// The default is to draw opaque objects first, then alpha-blended objects in reverse Z-order.
	// ([CC3BTreeNodeSequencer sequencerLocalContentOpaqueFirst]).
	//
	// To experiment with an alternate drawing order, set a different node sequence sorter
	// by uncommenting one of the lines here and commenting out the others. The last option
	// does not use a drawing sequencer, and draws the objects hierarchically instead.
	// With this, notice that the transparent beach ball now appears opaque, because it
	// was added first, and is traversed ahead of other objects in the hierarchical assembly,
	// resulting it in being drawn first, and so it cannot blend with the background objects.
	//
	// You can of course write your own node sequencers to customize to your specific
	// app needs. Best to change the node sequencer before any model objects are added.
//	self.drawingSequencer = [CC3BTreeNodeSequencer sequencerLocalContentOpaqueFirstGroupMeshes];
//	self.drawingSequencer = [CC3BTreeNodeSequencer sequencerLocalContentOpaqueFirstGroupTextures];
//	self.drawingSequencer = nil;
}

void CC3MashUpScene::preloadAssets()
{
	// Strongly cache the shader programs loaded here, so they'll be availble
	// when models are loaded on the background loading thread.
	CC3ShaderProgram::setIsPreloading( true );

	CC3ShaderProgram::programFromVertexShaderFile( "CC3ClipSpaceTexturable.vsh", "CC3ClipSpaceNoTexture.fsh" );
	CC3ShaderProgram::programFromVertexShaderFile( "CC3ClipSpaceTexturable.vsh", "CC3Fog.fsh" );
	CC3ShaderProgram::programFromVertexShaderFile( "CC3PointSprites.vsh", "CC3PointSprites.fsh" );
	CC3ShaderProgram::programFromVertexShaderFile( "CC3Texturable.vsh", "CC3BumpMapObjectSpace.fsh" );
	CC3ShaderProgram::programFromVertexShaderFile( "CC3Texturable.vsh", "CC3BumpMapTangentSpace.fsh" );
	CC3ShaderProgram::programFromVertexShaderFile( "CC3Texturable.vsh", "CC3NoTexture.fsh" );
	CC3ShaderProgram::programFromVertexShaderFile( "CC3Texturable.vsh", "CC3SingleTexture.fsh" );
	CC3ShaderProgram::programFromVertexShaderFile( "CC3Texturable.vsh", "CC3SingleTextureAlphaTest.fsh" );
	CC3ShaderProgram::programFromVertexShaderFile( "CC3Texturable.vsh", "CC3SingleTextureReflect.fsh" );
	CC3ShaderProgram::programFromVertexShaderFile( "CC3TexturableBones.vsh", "CC3SingleTexture.fsh" );
	CC3ShaderProgram::programFromVertexShaderFile( "CC3TexturableBones.vsh", "CC3SingleTextureReflect.fsh" );
	CC3ShaderProgram::programFromVertexShaderFile( "CC3TexturableRigidBones.vsh", "CC3BumpMapTangentSpace.fsh" );
	CC3ShaderProgram::programFromVertexShaderFile( "CC3TexturableRigidBones.vsh", "CC3NoTexture.fsh" );
	CC3ShaderProgram::programFromVertexShaderFile( "CC3TexturableRigidBones.vsh", "CC3SingleTexture.fsh" );

	// Now pre-load shader programs that originate in PFX resources.
	// Leave the shader program preloading on too...since effects will also load shaders.
	CC3Resource::setIsPreloading( true );

	CC3PFXResource::resourceFromFile( kPostProcPFXFile );
	CC3PFXResource::resourceFromFile( kReflectivePFXFile );
	
	// All done with shader pre-loading...let me know in the logs if any further shader programs
	// are loaded during the scene operation.
	CC3Resource::setIsPreloading( false );
	CC3ShaderProgram::setIsPreloading( false );

	
	// The automatic generation of mipmap in the environment map texture on the background
	// thread causes a short delay in rendering on the main thread. The text glyph texture
	// also requires substantial time for mipmap generation. For such textures, by loading
	// the texture, creating the mipmap, and caching the texture here, we can avoid the delay.
	// All other textures are loaded on the background thread.
	CC3Texture::setIsPreloading( true );
	CC3Texture::textureFromFile( "Arial32BMGlyph.png" );
	CC3Texture::textureCubeFromFilePattern( "EnvMap/EnvMap%s.jpg" );
	CC3Texture::setIsPreloading( false );
}

/** Various options for configuring interesting camera behaviours. */
void CC3MashUpScene::configureCamera()
{
	CC3Camera* cam = getActiveCamera();

	// Camera starts out embedded in the scene.
	_cameraZoomType = kCameraZoomNone;
	
	// The camera comes from the POD file and is actually animated.
	// Stop the camera from being animated so the user can control it via the user interface.
	cam->disableAnimation();
	
	// Keep track of which object the camera is pointing at
	_origCamTarget = cam->getTarget();
	_camTarget = _origCamTarget;
	
	// Set the field of view orientation to diagonal, to give a good overall average view of
	// the scene, regardless of the shape of the viewing screen. As loaded from the POD file,
	// the FOV is measured horizontally.
	cam->setFieldOfViewOrientation( CC3FieldOfViewOrientationDiagonal );

	// For cameras, the scale property determines camera zooming, and the effective field of view.
	// You can adjust this value to play with camera zooming. Conversely, if you find that objects
	// in the periphery of your view appear elongated, you can adjust the fieldOfView and/or
	// uniformScale properties to reduce this "fish-eye" effect. See the notes of the CC3Camera
	// fieldOfView property for more on this.
	cam->setUniformScale( 0.5f );
	
	// You can configure the camera to use orthographic projection instead of the default
	// perspective projection by setting the isUsingParallelProjection property to YES.
	// You will also need to adjust the scale to match the different projection.
//	cam.isUsingParallelProjection = YES;
//	cam.uniformScale = 0.015;
	
	// To see the effect of mounting a camera on a moving object, uncomment the following
	// lines to mount the camera on a virtual boom attached to the beach ball.
	// Since the beach ball rotates as it bounces, you might also want to comment out the
	// CC3ActionRotateForever action that is run on the beach ball in the addBeachBall method!
//	[_beachBall addChild: cam];				// Mount the camera on the beach ball
//	cam.location = cc3v(2.0, 1.0, 0.0);		// Relative to the parent beach ball
//	cam.rotation = cc3v(0.0, 90.0, 0.0);	// Point camera out over the beach ball

	// To see the effect of mounting a camera on a moving object AND having the camera track a
	// location or object, even as the moving object bounces and rotates, uncomment the following
	// lines to mount the camera on a virtual boom attached to the beach ball, but stay pointed at
	// the moving rainbow teapot, even as the beach ball that the camera is mounted on bounces and
	// rotates. In this case, you do not need to comment out the CC3ActionRotateForever action that
	// is run on the beach ball in the addBeachBall method
//	[_beachBall addChild: cam];				// Mount the camera on the beach ball
//	cam.location = cc3v(2.0, 1.0, 0.0);		// Relative to the parent beach ball
//	cam.target = teapotSatellite;			// Look toward the rainbow teapot...
//	cam.shouldTrackTarget = YES;			// ...and track it as it moves
}

/** Configure the lighting. */
void CC3MashUpScene::configureLighting()
{	
	// Start out with a sunny day
	_lightingType = kLightingSun;

	// Set the ambient scene lighting.
	setAmbientLight( ccc4f(0.3f, 0.3f, 0.3f, 1.0f) );

	if ( _robotLamp )
	{
		// Adjust the relative ambient and diffuse lighting of the main light to
		// improve realisim, particularly on shadow effects.
		_robotLamp->setDiffuseColor( ccc4f(0.8f, 0.8f, 0.8f, 1.0f) );

		// Another mechansim for adjusting shadow intensities is shadowIntensityFactor.
		// For better effect, set here to a value less than one to lighten the shadows
		// cast by the main light.
		_robotLamp->setShadowIntensityFactor( 0.75f );

		// The light from the robot POD file is animated to move back and forth, changing
		// the lighting of the scene as it moves. To turn this animation off, comment out
		// the following line. This can be useful when reviewing shadowing.
		//	[_robotLamp disableAnimation];
	}
}

void CC3MashUpScene::configureForScene( CC3Node* aNode, GLfloat duration )
{	
	// This scene is quite complex, containing many objects. As the user moves the camera
	// around the scene, objects move in and out of the camera's field of view. At any time,
	// there may be a number of objects that are out of view of the camera. With such a scene
	// layout, we can save significant GPU processing by not drawing those objects. To make
	// that happen, we assign a bounding volume to each mesh node. Once that is done, only
	// those objects whose bounding volumes intersect the camera frustum will be drawn.
	// Bounding volumes can also be used for collision detection between nodes. You can see
	// the effect of not using bounding volumes on drawing perfomance by commenting out the
	// following line and taking note of the drop in performance for this scene. However,
	// testing bounding volumes against the camera's frustum does take some CPU processing,
	// and in scenes where all or most of the objects are in front of the camera at all times,
	// using bounding volumes may actually result in slightly lower performance. By including
	// or not including the line below, you can test both scenarios and decide which approach
	// is best for your particular scene. Bounding volumes are not automatically created for
	// skinned meshes, such as the runners and mallet. See the addSkinnedRunners and
	// addSkinnedMallet methods to see how those bounding volumes are added manually.
	aNode->createBoundingVolumes();
	
	// Create OpenGL buffers for the vertex arrays to keep things fast and efficient, and
	// to save memory, release the vertex data in main memory because it is now redundant.
	// However, because we can add shadow volumes dynamically to any node, we need to keep the
	// vertex location, index and skinning data of all meshes around to build shadow volumes.
	// If we had added the shadow volumes before here, we wouldn't have to retain this data.
	aNode->retainVertexLocations();
	aNode->retainVertexIndices();
	aNode->retainVertexBoneWeights();
	aNode->retainVertexBoneIndices();
	aNode->createGLBuffers();
	aNode->releaseRedundantContent();
	
	// The following line displays the bounding volumes of each node. The bounding volume of
	// all mesh nodes, except the globe, contains both a spherical and bounding-box bounding
	// volume, to optimize testing. For something extra cool, touch the robot arm to see the
	// bounding volume of the particle emitter grow and shrink dynamically. Use the joystick
	// controls or gestures to back the camera away to get the full effect. You can also turn
	// on this property on individual nodes or node structures. See the notes for this property
	// and the shouldDrawBoundingVolume property in the CC3Node class notes.
//	aNode.shouldDrawAllBoundingVolumes = YES;
	
	// Select the appropriate shaders for each mesh node descendent now. If this step is omitted,
	// shaders will be selected for each mesh node the first time that mesh node is drawn.
	// Doing it now adds some additional time up front, but avoids potential pauses as the
	// shaders are loaded, compiled, and linked, the first time it is needed during drawing.
	// Shader selection is driven by the characteristics of each mesh node and its material,
	// including the number of textures, whether alpha testing is used, etc. To have the
	// correct shaders selected, it is important that you finish configuring the mesh nodes
	// prior to invoking this method. If you change any of these characteristics that affect
	// the shader selection, you can invoke the removeShaders method to cause different shaders
	// to be selected, based on the new mesh node and material characteristics.
	aNode->selectShaders();
	
	// For an interesting effect, to draw text descriptors and/or bounding boxes on every node
	// during debugging, uncomment one or more of the following lines. The first line displays
	// short descriptive text for each node (including class, node name & tag). The second line
	// displays bounding boxes of only those nodes with local content (eg- meshes). The third
	// line shows the bounding boxes of all nodes, including those with local content AND
	// structural nodes. You can also turn on any of these properties at a more granular level
	// by using these and similar methods on individual nodes or node structures. See the CC3Node
	// class notes. This family of properties can be particularly useful during development to
	// track down display issues.
//	aNode.shouldDrawAllDescriptors = YES;
//	aNode.shouldDrawAllLocalContentWireframeBoxes = YES;
//	aNode.shouldDrawAllWireframeBoxes = YES;
	
	// Use a standard CCActionFadeIn to fade the node in over the specified duration
	if ( duration > 0.0f )
	{
//		aNode->setOpacity( 0 );	// Needed for Cocos2D 1.x, which doesn't start fade-in from zero opacity
//		aNode->runAction( CCActionFadeIn::create( duration ) );
	}
}

void CC3MashUpScene::addBackdrop()
{
	setBackdrop( CC3Backdrop::nodeWithName( "Backdrop", kSkyColor ) );
//	self.backdrop = [CC3Backdrop nodeWithName: @"Backdrop"
//								  withTexture: [CC3Texture textureFromFile: kBrickTextureFile]];
}

void CC3MashUpScene::addGround()
{
	_ground = CC3PlaneNode::nodeWithName( kGroundName );
	_ground->populateAsDiskWithRadius( 1500, CC3TessellationMake(8, 32) );

	// To demonstrate that a Cocos3D CC3Texture can be created from an existing Cocos2D CCTexture,
	// we first load a CCTexture, and create the CC3Texture from it. We then assign the CC3Texture
	// a unique name and add it to the texture cache it so it will be available for later use.
	/*CCTexture* tex2D = CCTextureCache::sharedTextureCache()->addImage( kGroundTextureFile );
	CC3Texture* tex3D = CC3Texture::textureWithCCTexture( tex2D );
	tex3D->setName( kGroundTextureFile );
	CC3Texture::addTexture( tex3D );*/
	_ground->setTexture( CC3Texture::textureFromFile( kGroundTextureFile ) );

	// To simply load a Cocos3D texture directly, without first loading a Cocos2D texture,
	// comment out the lines above, and uncomment the following line.
//	_ground.texture = [CC3Texture textureFromFile: kGroundTextureFile];

	// The ground uses a repeating texture
	_ground->repeatTexture( tex2( 10, 10 ) );		// Grass
//	[_ground repeatTexture: (ccTex2F){3, 3}];		// MountainGrass
	
	_ground->setLocation( cc3v(0.0, -100.0, 0.0) );
	_ground->setRotation( cc3v(-90.0, 180.0, 0.0) );
	_ground->setShouldCullBackFaces( false );	// Show the ground from below as well.
	_ground->setTouchEnabled( true );			// Allow the ground to be selected by touch events.
	_ground->retainVertexLocations();	// Retain location data in main memory, even when it
										// is buffered to a GL VBO via releaseRedundantContent,
										// so that it may be accessed for further calculations
										// when dropping objects on the ground.
	addChild( _ground );
}

void CC3MashUpScene::addFloatingRing()
{
	CC3MeshNode* floater = CC3PlaneNode::nodeWithName( kFloaterName );
	floater->populateAsCenteredRectangleWithSize( CCSizeMake(120.0, 120.0) );

	// The OrangeRing.ppng texture will be loaded without pre-multiplied alpha.
	// The OrangeRing.png texture will be loaded without pre-multiplied alpha.
	// Comment out one or other of the following lines to see the difference. The effect
	// on the material blending that is automatically assigned is output in the logs.
	floater->setTexture( CC3Texture::textureFromFile( "OrangeRing.ppng" ) );
//	floater.texture = [CC3Texture textureFromFile: @"OrangeRing.png"];
	floater->setIsOpaque( false );		// Not strictly needed, because will be set automatically
								// during fading action, but set here to allow the blending
								// function to be logged on the next line.
	/*LogInfo(@"%@ with %@ blending (%@/%@) and %@ %@ pre-multiplied alpha.",
			floater, floater.material,
			NSStringFromGLEnum(floater.material.sourceBlend),
			NSStringFromGLEnum(floater.material.destinationBlend),
			floater.texture, (floater.texture.hasPremultipliedAlpha ? @"with" : @"without"));*/

	// This is a simple plane node. To make this object visible from behind, we need
	// to show the back sides of the faces as well.
	floater->setShouldCullBackFaces( false );			// Show from behind as well.
	
	// This object has some unexpected behaviour when using fog using GLSL. Since fog is dependent
	// on the depth buffer, the fog intensity will be that of this object, even though the farther
	// objects can be see through the transparent parts of this ring. To help with this, we
	// can cause the transparent fragments to be discarded, which helps because the transparent
	// fragments will not be written to the depth buffer. This works well except for the fact
	// that we also fade the entire object in and out, which causes issues as the opqaue areas
	// of the ring approach full transparency under fading. In general, opaque areas that have
	// been faded almost away will not play well with GLSL fog.
	floater->setShouldDrawLowAlpha( false );
	floater->getMaterial()->setAlphaTestReference( 0.05f );
	
	floater->setLocation( cc3v(400.0, 150.0, -250.0 ) );
	floater->setTouchEnabled( true );
	floater->setShouldDrawLocalContentWireframeBox( true );	// Draw an box around texture

	// Ring is added on on background thread. Configure it for the scene, and fade it in slowly.
	configureForScene( floater, kFadeInDuration );
	addChild( floater );

	floater->setOpacity( 255 );

	// Fade the floating ring in and out
	CCActionInterval* fadeOut = CCActionFadeOut::create( 5.0f );
	CCActionInterval* fadeIn = CCActionFadeIn::create( 5.0f );
//	floater->runAction( CCRepeatForever::create( CCActionSequence::createWithTwoActions( fadeOut, fadeIn ) ) );
	
	// Rotate the floating ring to see the effect on the orientation of the plane normals
	floater->runAction( CC3ActionRotateForever::actionWithRotationRate( cc3v(0.0, 30.0, 0.0) ) );
}

#if 0
/** Utility method to copy a file from the resources directory to the Documents directory */
-(BOOL) copyResourceToDocuments: (NSString*) fileName {
	NSString* srcDir = [[NSBundle mainBundle] resourcePath];
	NSString* srcPath = [srcDir stringByAppendingPathComponent: fileName];
	NSString* dstDir = [NSHomeDirectory() stringByAppendingPathComponent: @"Documents"];
	NSString* dstPath = [dstDir stringByAppendingPathComponent: fileName]; 
	
	NSError* err = nil;
	NSFileManager* fileMgr = [NSFileManager defaultManager];
	[fileMgr removeItemAtPath: dstPath error: &err];
	if ( [fileMgr copyItemAtPath: srcPath toPath: dstPath error: &err] ) {
		LogRez(@"Copied %@ to %@", srcPath, dstPath);
		return YES;
	} else {
		LogError(@"Could not copy %@ to %@ because (%li) in %@: %@",
					  srcPath, dstPath, (long)err.code, err.domain, err.userInfo);
		return NO;
	}
}
#endif

void CC3MashUpScene::addBeachBall()
{	
	// To show it is possible to load model files from other directories,
	// we copy the POD file to the application Document directory.
	//[self copyResourceToDocuments: kBeachBallPODFile];
	//NSString* docDir = [NSHomeDirectory() stringByAppendingPathComponent: @"Documents"];
	//NSString* podPath = [docDir stringByAppendingPathComponent: kBeachBallPODFile];
	
	// Load the POD file from the application Documents directory. It will also
	// load any needed textures from that directory as well.
	CC3ResourceNode* bbRez = CC3PODResourceNode::nodeWithName( kBeachBallRezNodeName, kBeachBallPODFile );
	
	// Configure the bouncing beach ball
	_beachBall = bbRez->getNodeNamed( kBeachBallName );
	_beachBall->setLocation( cc3v(200.0, 200.0, -400.0) );
	_beachBall->setUniformScale( 50.0f );
	
	// Allow this beach ball node to be selected by touch events.
	// The beach ball is actually a structural assembly containing four child nodes,
	// one for each separately colored mesh. By marking the node assembly as touch-enabled,
	// and NOT marking each component mesh node as touch-enabled, when any of the component
	// nodes is touched, the entire beach ball structural node will be selected.
	_beachBall->setTouchEnabled( true );
	
	// Bounce the beach ball...simply...we're not trying for realistic physics here,
	// but we can still do some fun and interesting stuff with Ease-actions.
	GLfloat hangTime = 3.0f;
	CC3Vector dropLocation = _beachBall->getLocation();
	CC3Vector landingLocation = dropLocation;
	landingLocation.y = _ground->getLocation().y + 30.0f;
	
	CC3ActionInterval* dropAction = CC3ActionMoveTo::actionWithDuration( hangTime, landingLocation );
//	dropAction = CCActionEaseOut::create( CCActionEaseIn::create( dropAction, 4.0f ), 1.6f );
	
	CC3ActionInterval* riseAction = CC3ActionMoveTo::actionWithDuration( hangTime, dropLocation );
//	riseAction = CCActionEaseIn::create( CCActionEaseOut::create( riseAction, 4.0f ), 1.6f );
	
//	_beachBall->runAction( CC3RepeatForever::create( CC3ActionSequence::createWithTwoActions( dropAction, riseAction ) ) );
	
	// For extra realism, also rotate the beach ball as it bounces.
	_beachBall->runAction( CC3ActionRotateForever::actionWithRotationRate( cc3v(30.0, 0.0, 45.0) ) );

	_beachBall->setOpacity( 255 );
	
	// Beach ball is added on on background thread. Configure it for the scene, and fade it in slowly.
	configureForScene( _beachBall, kFadeInDuration );
	addChild( _beachBall );
}

void CC3MashUpScene::addGlobe()
{	
	// To show it is possible to load texture files from other directories,
	// we copy the texture file to the application Document directory.
	//[self copyResourceToDocuments: kGlobeTextureFile];
	//NSString* docDir = [NSHomeDirectory() stringByAppendingPathComponent: @"Documents"];
	//NSString* texPath = [docDir stringByAppendingPathComponent: kGlobeTextureFile];
	
	// Configure the rotating globe
	_globe = CC3SphereNode::nodeWithName( kGlobeName );		// weak reference
	_globe->populateAsSphereWithRadius( 1.0f, CC3TessellationMake(32, 32) );
	_globe->setTexture( CC3Texture::textureFromFile( kGlobeTextureFile ) );
	_globe->setLocation( cc3v(150.0, 200.0, -150.0) );
	_globe->setUniformScale( 50.0f );
	_globe->setAmbientColor( kCCC4FLightGray );		// Increase the ambient reflection
	_globe->setTouchEnabled( true );				// allow this node to be selected by touch events
	
	// Rotate the globe
	_globe->runAction( CC3ActionRotateForever::actionWithRotationRate( cc3v(0.0, 30.0, 0.0) ) );
	
	// Cube is added on on background thread. Configure it for the scene, and fade it in slowly.
	configureForScene( _globe, kFadeInDuration );
	addChild( _globe );
	
	// For something interesting, uncomment the following lines to make the
	// globe invisible, but still touchable, and still able to cast a shadow.
//	_globe.visible = NO;
//	_globe.shouldAllowTouchableWhenInvisible = YES;
//	_globe.shouldCastShadowsWhenInvisible = YES;
}

void CC3MashUpScene::addDieCube()
{
	// Fetch the die cube model from the POD file.
	CC3ResourceNode* podRezNode = CC3PODResourceNode::nodeFromFile( kDieCubePODFile );
	CC3Node* podDieCube = podRezNode->getNodeNamed( kDieCubePODName );
	
	// We want this node to be a SpinningNode class instead of the CC3PODNode class that
	// is loaded from the POD file. We can swap it out by creating a copy of the loaded
	// POD node, using a different node class as the base.
	CC3Node* pNode = (CC3Node*)podDieCube->copyWithName( kDieCubeName );

	_dieCube = new SpinningNode;
	_dieCube->init();
	_dieCube->populateFrom( pNode );
	_dieCube->addCopiesOfChildrenFrom( pNode );
	// Now set some properties, including the friction, and add the die cube to the scene
	_dieCube->setUniformScale( 30.0f );
	_dieCube->setLocation( cc3v(-200.0, 200.0, 0.0) );
	_dieCube->setTouchEnabled( true );
	_dieCube->setFriction( 1.0f );
	
	// Cube is added on on background thread. Configure it for the scene, and fade it in slowly.
	configureForScene( _dieCube, kFadeInDuration );
	addChild( _dieCube );

	pNode->release();
	_dieCube->release();
}

void CC3MashUpScene::addTexturedCube()
{
	std::string itemName = "";
	
	// Create a parametric textured cube, centered on the local origin.
	CC3BoxNode* texCube = CC3BoxNode::nodeWithName( kTexturedCubeName );
	texCube->populateAsSolidBox( CC3BoxMake(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f) );
	texCube->setUniformScale( 30.0f );

	// Add a texture to the textured cube. This creates a material automatically.
	// For kicks, we use a texture that contains two distinct images, one for a box and
	// one for a ball, and set a texture rectangle on the node so it will use only one
	// part of the texture to cover the box.
	texCube->setTexture( CC3Texture::textureFromFile( kMeshParticleTextureFile ) );
	texCube->setTextureRectangle( CCRectMake(0, 0, 1, 0.75) );
//	texCube->setTexture( CC3Texture::textureFromFile(kCubeTextureFile) );	// Alternately, use a full texture

	texCube->setAmbientColor( ccc4f(0.6f, 0.6f, 0.6f, 1.0f) );		// Increase the ambient reflection
	
	// Add direction markers to demonstrate how the sides are oriented. In the local coordinate
	// system of the cube node, the red marker point in the direction of the positive-X axis,
	// the green marker in the direction of the positive-Y axis, and the blue marker in the
	// direction of the positive-Z axis. As these demonstrate, the front faces the positive-Z
	// direction, and the top faces the positive-Y direction.
	texCube->addAxesDirectionMarkers();
	
	// Wrap the cube in a spinner node to allow it to be rotated by touch swipes.
	// Give the spinner some friction so that it slows down over time one released.
	itemName = CC3String::stringWithFormat( (char*)"%s-Spinner", texCube->getName().c_str() );
	_texCubeSpinner = SpinningNode::nodeWithName( itemName );
	_texCubeSpinner->setFriction( 1.0f );
	_texCubeSpinner->setLocation( cc3v(-200, 75.0, 0.0) );
	_texCubeSpinner->setTouchEnabled( true );

	// Add the cube to the spinner and the spinner to the scene.
	_texCubeSpinner->addChild( texCube );
	
	// Cube is added on on background thread. Configure it for the scene, and fade it in slowly.
	configureForScene( _texCubeSpinner, 0 );
	addChild( _texCubeSpinner );
}

void CC3MashUpScene::addTeapotAndSatellite()
{
	_teapotTextured = CC3ModelSampleFactory::factory()->makeTexturableTeapotNamed( kTexturedTeapotName );
	_teapotTextured->setTouchEnabled( true );		// allow this node to be selected by touch events
	
	// If cube-maps are available, add two textures to the teapot. The first is a cube-map texture
	// showing the six sides of a real-time reflective environmental cube surrounding the teapot,
	// viewed from the teapot's perspective. The reflection is dynamically generated as objects
	// move around the scene. A second texture is added to provide an optional surface material
	// (eg- brushed metal). The material reflectivity property adjusts how reflective the surface
	// is, by adjusting the blend between the two textures. Lower the reflectivity towards zero to
	// show some of the underlying material. Since the environment map texture renders the scene,
	// it requires a depth buffer. This is created automatically during the initialization of the
	// environment texture. However, if we had multiple reflective objects, we could use the same
	// depth buffer for all of them if the textures are the same size, by using a different
	// creation method for the environment texture. Since generating an environment map texture
	// requires rendering the scene from each of the six axis directions, it can be quite costly.
	// You can use the numberOfFacesPerSnapshot property to adjust how often the reflective faces
	// are updated, to trade off real-time accuracy and performance.
	_envMapTex = CC3EnvironmentMapTexture::textureCubeWithSideLength( 256 );
	_envMapTex->setName( "TeapotMirror" );				// Give it a name to help with troubleshooting
	_envMapTex->setNumberOfFacesPerSnapshot( 1.0f );	// Update only one side of the cube in each frame

	_teapotTextured->addTexture( _envMapTex );
	_teapotTextured->setReflectivity( 0.7f );			// Modify this (0-1) to change how reflective the teapot is
	_teapotTextured->setShouldUseLighting( false );		// Ignore lighting to highlight reflections demo

	// Add a brushed metal texture (with or without the reflective texture added above).
	_teapotTextured->addTexture( CC3Texture::textureFromFile( "tex_base.png" ) );
	
	// Add a second rainbow-colored teapot as a satellite of the reflective teapot.
	_teapotSatellite = PhysicsMeshNode::nodeWithName( kRainbowTeapotName );
	_teapotSatellite->setMesh( CC3ModelSampleFactory::factory()->getMulticoloredTeapotMesh() );
	_teapotSatellite->setMaterial( CC3Material::shiny() );
	_teapotSatellite->setLocation( cc3v(0.3f, 0.1f, 0.0f) );
	_teapotSatellite->setUniformScale( 0.4f );
	_teapotSatellite->setTouchEnabled( true );		// allow this node to be selected by touch events
	_teapotTextured->addChild( _teapotSatellite );

	_teapotTextured->setLocation( cc3v(0.0, 150.0, -650.0) );
	_teapotTextured->setUniformScale( 500.0f );

	_teapotTextured->setOpacity( 255 );
	
	// Teapots are added on on background thread. Configure it for the scene, and fade it in slowly.
	configureForScene( _teapotTextured, kFadeInDuration );
	addChild( _teapotTextured );
	
	// Rotate the teapots. The satellite orbits the reflective teapot because it is a child node of the
	// reflective teapot, and orbits as the parent node rotates. We give the rotation action a tag so we can
	// find it again when the satellite teapot collides with the brick wall and we need to change the motion.
	CC3Action* teapotSpinAction =  CC3ActionRotateForever::actionWithRotationRate( cc3v(0.0, 60.0, 0.0) );
	teapotSpinAction->setTag( kTeapotRotationActionTag );
	_teapotTextured->runAction( teapotSpinAction );

	 // For effect, also rotate the satellite around its own axes.
	_teapotSatellite->runAction( CC3ActionRotateForever::actionWithRotationRate( cc3v(30.0, 0.0, 45.0) ) );

//	getActiveCamera()->setTarget( _teapotTextured );
}

void CC3MashUpScene::addSkyBox()
{
	CC3MeshNode* skyBox = CC3SphereNode::nodeWithName( "SkyBox" );
	skyBox->populateAsSphereWithRadius( 1600.0f, CC3TessellationMake(24, 24) );
	skyBox->setShouldCullBackFaces( false );
	skyBox->setTexture( CC3Texture::textureCubeFromFilePattern( "EnvMap%@.jpg" ) ); 
	skyBox->applyEffectNamedFromFile( "SkyBox", "EnvMap.pfx" );
	addChild( skyBox );

	// PVR files can contain an entire cube-map (and all the mipmaps too) in a single file.
	// To try it out when running on iOS, uncomment the following line.
//	skyBox.texture = [CC3Texture textureFromFile: @"Skybox.pvr"];

	_ground->remove();	// Remove the ground, because the skybox already includes a ground
}

void CC3MashUpScene::addBrickWall()
{
	// Create a parametric textured box as an open door.
	_brickWall = DoorMeshNode::nodeWithName( kBrickWallName );
	_brickWall->setTouchEnabled( true );
	_brickWall->populateAsSolidBox( CC3BoxMake(-1.5f, 0.f, -0.3f, 1.5f, 2.5f, 0.3f) );
	_brickWall->setUniformScale( 40.0f );
	
	// Add a texture to the wall and repeat it. This creates a material automatically.
	_brickWall->setTexture( CC3Texture::textureFromFile( kBrickTextureFile ) );
	_brickWall->repeatTexture( tex2(4, 2) );
	_brickWall->setAmbientColor( kCCC4FWhite );			// Increase the ambient reflection so the backside is visible
	
	// Start with the wall in the open position
	_brickWall->setIsOpen( true );
	_brickWall->setLocation( kBrickWallOpenLocation );
	_brickWall->setRotation( cc3v(0, -45, 0) );
	
	// Wall is added on on background thread. Configure it for the scene, and fade it in slowly.
	configureForScene( _brickWall, kFadeInDuration );
	addChild( _brickWall );
}

/** Loads a POD file containing an animated robot arm, a camera, and an animated light. */
void CC3MashUpScene::addRobot()
{
	// We introduce a specialized resource subclass, not because it is needed in general,
	// but because the original PVR demo app ignores some data in the POD file. To replicate
	// the PVR demo faithfully, we must do the same, by tweaking the loader to act accordingly
	// by creating a specialized subclass.
	CC3ResourceNode* podRezNode = CC3PODResourceNode::nodeWithName( kPODRobotRezNodeName );
	podRezNode->populateFromResource( IntroducingPODResource::resourceFromFile(kRobotPODFile) );
	
	// If you want to stop the robot arm from being animated, uncomment the following line.
//	[podRezNode disableAllAnimation];
	
	podRezNode->setTouchEnabled( true );
	addChild( podRezNode );
	
	// Retrieve the camera in the POD and cache it for later access.
	_robotCam = (CC3Camera*)podRezNode->getNodeNamed( kRobotCameraName );
	
	// Retrieve the light from the POD resource so we can track its location as it moves via animation
	_robotLamp = (CC3Light*)podRezNode->getNodeNamed( kPODLightName );
	
	// Start the animation of the robot arm and bouncing lamp from the PVR POD file contents.
	// But we'll have a bit of fun with the animation, as follows.
	// The basic animation in the POD pirouettes the robot arm in a complex movement...
	CC3ActionInterval* pirouette = CC3ActionAnimate::actionWithDuration( 5.0f );
	podRezNode->runAction( CC3RepeatForever::create( pirouette ) );
	return;

	// Extract only the initial bending-down motion from the animation, reverse it to create
	// a stand-up motion, and paste the two actions together to create a bowing motion.
	CC3ActionInterval* bendDown = CC3ActionAnimate::actionWithDuration( 1.8f, 0.0f, 0.15f );
	CC3ActionInterval* standUp = bendDown->reverse();
	CC3ActionInterval* takeABow = CC3ActionSequence::createWithTwoActions( bendDown, standUp );
	
	// Now...put it all together. The robot arm performs its pirouette, and then takes a bow,
	// over and over again.
	podRezNode->runAction( CC3RepeatForever::create( CC3ActionSequence::createWithTwoActions( pirouette, takeABow ) ) );
}

/**
 * Add small red, green and blue teapots to mark the X, Y & Z axes respectively.
 * The teapots appear at location 100.0 on each of the axes.
 */
void CC3MashUpScene::addAxisMarkers()
{	
	// To exhibit node creation options, we create the red teapot using a factory method.
	// But then we create the blue and green teapots by copying the red teapot.

	// Red teapot is at postion 100 on the X-axis
	CC3Node* teapotRed = CC3ModelSampleFactory::factory()->makeUniColoredTeapotNamed( kTeapotRedName, ccc4f(0.7f, 0.0f, 0.0f, 1.0f) );
	teapotRed->setLocation( cc3v(100.0, 0.0, 0.0) );
	teapotRed->setUniformScale( 100.0f );
	teapotRed->setTouchEnabled( true );		// allow this node to be selected by touch events
	
	// Teapot is added on on background thread. Configure it for the scene, and fade it in slowly.
	configureForScene( teapotRed, kFadeInDuration );
	addChild( teapotRed );
	
	// Green teapot is at postion 100 on the Y-axis
	// Create it by copying the red teapot.
	CC3Node* teapotGreen = (CC3Node*)teapotRed->copyWithName( kTeapotGreenName );
	teapotGreen->setDiffuseColor( ccc4f(0.0f, 0.7f, 0.0f, 1.0f) );
	teapotGreen->setLocation( cc3v(0.0, 100.0, 0.0) );
	// Teapot is added on on background thread. Configure it for the scene, and fade it in slowly.
	configureForScene( teapotGreen, kFadeInDuration );
	addChild( teapotGreen );
	/// Should release manualy here
	teapotGreen->release();

	// Blue teapot is at postion 100 on the Z-axis
	// Create it by copying the red teapot.
	CC3Node* teapotBlue = (CC3Node*)teapotRed->copyWithName( kTeapotBlueName );
	teapotBlue->setDiffuseColor( ccc4f(0.0f, 0.0f, 0.7f, 1.0f) );
	teapotBlue->setLocation( cc3v(0.0, 0.0, 100.0) );
	// Teapot is added on on background thread. Configure it for the scene, and fade it in slowly.
	configureForScene( teapotBlue, kFadeInDuration );
	addChild( teapotBlue );
	/// Should release manualy here
	teapotBlue->release();
}

void CC3MashUpScene::addLightMarker()
{
	_teapotWhite = CC3ModelSampleFactory::factory()->makeUniColoredTeapotNamed( kTeapotWhiteName, kCCC4FWhite );
	_teapotWhite->setUniformScale( 200.0f );
	_teapotWhite->setTouchEnabled( true );		// allow this node to be selected by touch events
	
	// Teapot is added on on background thread. Configure it for the scene, and fade it in slowly.
	configureForScene( _teapotWhite, kFadeInDuration );
	addChild( _teapotWhite );
}

void CC3MashUpScene::addBitmapLabel()
{
	CylinderLabel* bmLabel = CylinderLabel::nodeWithName( kBitmapLabelName );
	bmLabel->setRadius( 50.f );
	bmLabel->setTextAlignment( kCCTextAlignmentCenter );
	bmLabel->setRelativeOrigin( ccp(0.5, 0.5) );	
	bmLabel->setTessellation( CC3TessellationMake(4, 1) );
	bmLabel->setFontFileName( "Arial32BMGlyph.fnt" );
	bmLabel->setLabelString( "Hello, world!" );
	_bmLabelMessageIndex = 0;	// Keep track of which message is being displayed
	
	bmLabel->setLocation( cc3v(-150.0, 75.0, 500.0) );
	bmLabel->setRotation( cc3v(0.0, 180.0, 0.0) );
	bmLabel->setUniformScale( 2.0f );
	bmLabel->setColor( CCColorRefFromCCC4F(ccc4f(0.0f, 0.85f, 0.45f, 1.0f)) );
	bmLabel->setShouldCullBackFaces( false );			// Show from behind as well.
	bmLabel->setTouchEnabled( true );

	// Label is added on on background thread. Configure it for the scene, and fade it in slowly.
	configureForScene( bmLabel, kFadeInDuration );
	addChild( bmLabel );
	bmLabel->runAction( CC3ActionRotateForever::actionWithRotationRate(cc3v(0, 30, 0)) );
}

void CC3MashUpScene::addProjectedLabel()
{
	CCLabelTTF* bbLabel = CCLabelTTF::create( "Whoa...I'm dizzy!", "Felt", 18.0f );
	CC3Billboard* bb = CC3Billboard::nodeWithName( kBillboardName, bbLabel );
	bb->setColor( CCColorRefFromCCC4F(kCCC4FYellow) );
	
	// The billboard is a one-sided rectangular mesh, and would not normally be visible from the
	// back side. This is not an issue, since it is configured to always face the camera. However,
	// this also affects its ability to cast a shadow when the light is behind it. Set the back
	// faces to draw so that a shadow will be cast when the light is behind the billboard.
//	bb.shouldCullBackFaces = NO;
	
	// As the hose emitter moves around, it is sometimes in front of this billboard, but emits
	// some particles behind this billboard. The result is that those particles are blocked by
	// the transparent parts of the billboard and appear to inappropriately disappear. To compensate,
	// set the explicit Z-order of the billboard to be either always in-front of (< 0) or always
	// behind (> 0) the particle emitter. You can experiment with both options here. Or set the
	// Z-order to zero (the default and same as the emitter), and see what the problem is in the
	// first place! The problem is more evident when the emitter is set to a wide dispersion angle.
	bb->setZOrder( -1 );
	
	// Uncomment to see the extent of the label as it moves in the 3D scene
//	bb.shouldDrawLocalContentWireframeBox = YES;

	
	// A billboard can be drawn either as part of the 3D scene, or as an overlay
	// above the 3D scene. By commenting out one of the following sections of code,
	// you can choose which method to use.
	
	// 1) In the 3D scene:
	//    Find the camera and track it, so that it always faces the camera.
	bb->setShouldAutotargetCamera( true );
	
	// 2) Overlaid above the 3D scene:
	//    The following lines add the emitter billboard as a 2D overlay that draws above
	//    the 3D scene. The label text will not be occluded by any other 3D nodes.
	//    Comment out the lines just above, and uncomment the following lines:
//	bb.shouldDrawAs2DOverlay = YES;
//	bb.unityScaleDistance = 425.0;
//	bb.offsetPosition = ccp( 0.0, 15.0 );
	
	// Billboards with transparency don't shadow well, so don't let this billboard cast a shadow.
	bb->setShouldCastShadows( false );
	
	// Locate the billboard at the end of the robot's arm
	bb->setLocation( cc3v( 0.0, 90.0, 0.0 ) );

	getNodeNamed( kRobotTopArm )->addChild( bb );
}

void CC3MashUpScene::addWoodenSign()
{

}

// Text to hold in userData of floating head and then log when the head is poked.
static std::string kDontPokeMe = "Owww! Don't poke me!";

void CC3MashUpScene::addFloatingHead()
{
	CC3ResourceNode* podRezNode = CC3PODResourceNode::nodeWithName( kPODHeadRezNodeName, kHeadPODFile );

	// Extract the floating head mesh node and set it to be touch enabled
	_floatingHead = (CC3MeshNode*)podRezNode->getNodeNamed( kFloatingHeadName );	
	_floatingHead->setTouchEnabled( true );
	_floatingHead->setDiffuseColor( kCCC4FWhite );
	_floatingHead->setAmbientColor( kCCC4FGray );
	
	// Demonstrate the use of application-specific data attached to a node in the userData property.
	//_floatingHead->setUserData( kDontPokeMe );
	
	// The floating head normal texture was created in a left-handed coordinate
	// system (eg- DirectX). OpenGL uses a right-handed coordinate system.
	// The difference means that the bump-map normals interact as if the light
	// was coming from the opposite direction. We can correct for this by flipping
	// the normal texture horizontally by flipping the textue mapping coordinates
	// of the mesh horizontally.
	_floatingHead->flipTexturesHorizontally();

	// The origin of the floating head mesh is at the bottom. It suits our purposes better to
	// have the origin of the mesh at the center of geometry. The method we invoke here changes
	// the value of every vertex in the mesh. So we should only ever want to do this once per mesh.
	_floatingHead->moveMeshOriginToCenterOfGeometry();
	
	// Texture that has a bump-map stamp, whose pixels contain normals instead of colors.
	// Give it a texture unit configured for bump-mapping. The rgbNormalMap indicates how
	// the X,Y & Z components of the normal are stored in the texture RGB components.
	_headBumpTex = CC3TextureUnitTexture::textureFromFile( kHeadBumpFile );
	_headBumpTex->setTextureUnit( CC3BumpMapTextureUnit::textureUnit() );
	_headBumpTex->getTextureUnit()->setRgbNormalMap( kCC3DOT3RGB_YZX );

	// Load the visible texture of the floating head, and add it as an overlay on the bump map texture.
	_headTex = CC3Texture::textureFromFile( kHeadTextureFile );

	// The two textures are PVR textures pre-loaded with mipmaps.
	// However, using the mipmap for this mesh creates a visual artifact around the
	// fringe of the model. So we'll just use linear filtering on the main texture.
	// Comment out these two lines if you want to see the difference.
	_headBumpTex->getTexture()->setMinifyingFunction( GL_LINEAR );
	_headTex->setMinifyingFunction( GL_LINEAR );

	// Add the bump-map texture and the color texture to the material.
	_floatingHead->getMaterial()->setTexture( _headBumpTex );		// replace the dummy texture
	_floatingHead->getMaterial()->addTexture( _headTex );
	
	// The bump-map texture uses an OpenGL ES 1.1-compatible object-space bump-map that uses
	// a light direction that is held in the texture unit. The mesh node tracks the direction
	// to the light, and sets it in the texture unit. Because of this, the mesh node cannot
	// also track the camera, to face the camera. But that's okay, because we have already
	// wrapped the floating head mesh node in an orienting wrapper.
	_floatingHead->setTarget( _robotLamp );
	_floatingHead->setShouldTrackTarget( true );
	_floatingHead->setIsTrackingForBumpMapping( true );
	
	// Put the head node in an orienting wrapper so that we can orient it to face the camera.
	// First turn the floating head to face right so that it points towards the side of the
	// wrapper that will be kept facing the camera, and move the head to the origin of the wrapper.
	_floatingHead->setRotation( cc3v(0, -90, 0) );
	_floatingHead->setLocation( CC3Vector::kCC3VectorZero );
	CC3Node* headHolder = _floatingHead->asCameraTrackingWrapper();
	headHolder->setLocation( cc3v(-500.0, 200.0, 0.0) );

	// Head is added on on background thread. Configure it for the scene, and fade it in slowly.
	configureForScene( headHolder, kFadeInDuration );
	addChild( headHolder );
}

void CC3MashUpScene::addMascots()
{
	// Create the mascots. Load the first from file, then copy to create the second.
	// The texture coordinates of the mascot POD file expect the texture to be loaded
	// upside down. By telling the resource this, it will compensate during loading.
	CC3ResourceNode* podRezNode = CC3PODResourceNode::nodeFromFile( kMascotPODFile, true );
	_mascot = (CC3MeshNode*)podRezNode->getNodeNamed( kMascotName );
	CC3MeshNode* distractedMascot = (CC3MeshNode*)_mascot->copyWithName( kDistractedMascotName );
	
	// Allow the mascots to be selected by touch events.
	_mascot->setTouchEnabled( true );
	distractedMascot->setTouchEnabled( true );

	// Scale the mascots
	_mascot->setUniformScale( 22.0f );
	distractedMascot->setUniformScale( 22.0f );
	
	// Create the wrapper for the mascot that stares back at the camera.
	// Rotate the mascot in the wrapper so that the correct side faces the camera
	// as the wrapper tracks the camera.
	CC3Node* mascotHolder = _mascot->asCameraTrackingWrapper();
	_mascot->setRotation( cc3v(0, -90, 0) );
	mascotHolder->setLocation( cc3v(-450.0, 100.0, -575.0) );

	// Mascot is added on on background thread. Configure it for the scene, and fade it in slowly.
	configureForScene( mascotHolder, kFadeInDuration );
	addChild( mascotHolder );

	// Arrange for the mascot to be distracted by the rainbow teapot's movements.
	// We could have the mascot track the teapot itself. But, the wrong side would
	// face the teapot. To have the correct side of the mascot face the teapot, we
	// add it to a wrapper, turn it to look to the right within the wrapper, and
	// then arrange for the wrapper to track the rainbow teapot.
	CC3Node* distractedMascotHolder = distractedMascot->asTrackingWrapper();
	distractedMascot->setRotation( cc3v(0, -90, 0) );
	distractedMascotHolder->setLocation( cc3v(-375.0, 100.0, -700.0) );
	distractedMascotHolder->setTarget( _teapotSatellite );

	// Mascot is added on on background thread. Configure it for the scene, and fade it in slowly.
	configureForScene( distractedMascotHolder, kFadeInDuration );
	addChild( distractedMascotHolder );

	// If you want to restrict the mascot to only rotating side-to-side around the
	// Y-axis, but not up and down, uncomment the following line.
//	distractedMascotHolder.targettingConstraint = kCC3TargettingConstraintLocalYAxis;
	
	// To see the orientation of the mascot within the orienting wrapper, uncomment
	// the first line below. This will show that the front of the mascot faces down
	// the positive X-axis. The 90-degree rotation above serves to rotate this X-axis
	// of the mascot to align with the positive-Z axis of the wrapper, which is the
	// axis that points towards the rainbox teapot.
	// Uncomment the second line to see that the Z-axis of the wrapper points towards
	// the target. If you uncomment both lines to see both sets of axes at the same
	// time, then also uncomment the third line below to give the two sets of axes
	// an offset, so you can see both.
//	[distractedMascot addAxesDirectionMarkers];
//	[distractedMascotHolder addAxesDirectionMarkers];
//	distractedMascot.location = cc3v(0, 20, 0);

	distractedMascot->release();
}

void CC3MashUpScene::addSun()
{
	// Create the Cocos2D 2D particle emitter.
	CCParticleSystem* emitter = CCParticleSun::create();
	emitter->setPosition( ccp(0.0, 0.0) );
	
	// Create the 3D billboard node to hold the 2D particle emitter.
	CC3Billboard* bb = CC3ParticleSystemBillboard::nodeWithName( kSunName, emitter );

	// A billboard can be drawn either as part of the 3D scene, or as an overlay
	// above the 3D scene. By commenting out one of the following sections of code,
	// you can choose which method to use.
	
	// 1) In the 3D scene.
	// The following lines wrap the emitter billboard in a wrapper that will find
	// and track the camera in 3D. The sun can be occluded by other nodes between
	// it and the camera.

	bb->setUniformScale( 3.0f );			// Find a suitable size

	// 2D particle systems do not have a real contentSize and boundingBox, so we need to
	// calculate it dynamically on each update pass, or assign one that will cover the
	// area that will be used by this particular particle system. This bounding rectangle
	// is specified in terms of the local coordinate system of the particle system and
	// will be scaled and transformed as the node is transformed. By setting this once,
	// we don't need to calculate it while running the particle system.
	// To calculate it dynamically on each update instead, comment out the following line,
	// and uncomment the line after.
	bb->setBillboardBoundingRect( CCRectMake(-30.0, -30.0, 60.0, 60.0) );
//	bb.shouldAlwaysMeasureBillboardBoundingRect = YES;
	
	// How did we determine the billboardBoundingRect? This can be done by trial and
	// error, by uncommenting culling logging in the CC3Billboard doesIntersectBoundingVolume:
	// method. Or it is better done by changing LogTrace to LogDebug in the CC3Billboard
	// billboardBoundingRect property accessor method, commenting out the line above this
	// comment, and uncommenting the following line. Doing so will cause an ever expanding
	// bounding box to be logged, the maximum size of which can be used as the value to
	// set in the billboardBoundingRect property.
//	bb.shouldMaximizeBillboardBoundingRect = YES;

	// Locate the sun way up in the sky, and set it to find and track the camera
	// so that it appears to be spherical from wherever the camera is.
	bb->setLocation( cc3v(1000.0, 1000.0, -100.0) );
	bb->setShouldAutotargetCamera( true );
	addChild( bb );

	// 2) Overlaid above the 3D scene.
	// The following lines add the emitter billboard as a 2D overlay that draws above
	// the 3D scene. The flames will not be occluded by any other 3D nodes.
	// Comment out the lines in section (1) just above, and uncomment the following lines:
//	emitter.positionType = kCCPositionTypeGrouped;
//	bb.shouldDrawAs2DOverlay = YES;
//	bb.location = cc3v(3000.0, 3000.0, -100.0);
//	bb.unityScaleDistance = 9000.0;
//	[self addChild: bb];
}

void CC3MashUpScene::addSpotlight()
{
	CC3Light* spotLight = CC3Light::nodeWithName( kSpotlightName );
	spotLight->setVisible( false );
	spotLight->setSpotExponent( 30.0f );
	spotLight->setSpotCutoffAngle( 60.0 );
	spotLight->setAttenuation( CC3AttenuationCoefficientsMake(0.0f, 0.002f, 0.000001f) );
	spotLight->setIsDirectionalOnly( false );

	if ( getActiveCamera() ) 
		getActiveCamera()->addChild( spotLight );
}

void CC3MashUpScene::addLightProbes()
{
	// Load the cube texture that contains the lighting incident from all directions.
	// Alternately, for an interesting effect, you can comment out the first line and
	// uncomment the second line, to load a cube-texture that contains a different
	// solid color per side. This color-coded clearly demonstrates how the cube texture
	// is being mapped to the normals of the model.
	CC3Texture* lpTex = CC3Texture::textureCubeFromFilePattern( "LightProbe/cubelight_%s.png" );
//	CC3Texture* lpTex = [CC3Texture textureCubeColoredForAxes];

	addChild( CC3LightProbe::nodeWithTexture( lpTex ) );
}

void CC3MashUpScene::addFog()
{
	setFog( CC3Fog::fog() );

	_fog->addTexture( _postProcSurface->getColorTexture() );
	_fog->addTexture( _postProcSurface->getDepthTexture() );
	_fog->setShaderProgram( CC3ShaderProgram::programFromVertexShaderFile( "CC3ClipSpaceTexturable.vsh", "CC3Fog.fsh" ) );

	_fog->setVisible( false );
	_fog->setColor( CCColorRefFromCCC4F(ccc4f(0.5, 0.5, 0.75, 1.0)) );	// A slightly bluish fog.

	// Choose one of GL_LINEAR, GL_EXP or GL_EXP2
	_fog->setAttenuationMode( GL_EXP2 );

	// If using GL_EXP or GL_EXP2, the density property will have effect.
	_fog->setDensity( 0.0017f );
	
	// If using GL_LINEAR, the start and end distance properties will have effect.
	_fog->setStartDistance( 200.0f );
	_fog->setEndDistance( 1500.0f );

	// To make things a bit more interesting, set up a repeating up/down cycle to
	// change the color of the fog from the original bluish to reddish, and back again.
	GLfloat tintTime = 4.0f;
	CCColorRef startColor = _fog->getColor();
	CCColorRef endColor = CCColorRefFromCCC4F(ccc4f(0.75, 0.5, 0.5, 1.0));		// A slightly redish fog.
	CCActionInterval* tintDown = CCActionTintTo::create( tintTime, endColor.r, endColor.g, endColor.b );
	CCActionInterval* tintUp   = CCActionTintTo::create( tintTime, startColor.r, startColor.g, startColor.b );
//	_fog->runAction( CCActionRepeatForever::create( CCActionSequence::createWithTwoActions( tintDown, tintUp ) ) );
}

void CC3MashUpScene::addSkinnedMallet()
{
	// Load the POD file and remove its camera since we won't need it.
	// This is not actually necessary, but demonstrates that the resources loaded from
	// a POD file, including the resource node, are just nodes that can be manipulated
	// like any other node assembly.
	CC3ResourceNode* malletAndAnvils = CC3PODResourceNode::nodeFromFile( kMalletPODFile );
	malletAndAnvils->getNodeNamed( "Camera01" )->remove();
	malletAndAnvils->getNodeNamed( "Camera01Target" )->remove();

	CC3MeshNode* mallet = (CC3MeshNode*)malletAndAnvils->getNodeNamed( kMalletName );
	
	// Mallet normal transforms are scaled too far during transforms, so force
	// the normals to be individually re-normalized after being transformed.
	mallet->setNormalScalingMethod( kCC3NormalScalingNormalize );

	// Ensure the bones in the mallet are rigid (no scale applied). Doing this allows the
	// shader program that is optimized for that to be automatically selected for the mallet
	// skinned mesh node. Many more active bones are possible with a rigid skeleton.
	// The model must be designed as a rigid model, otherwise it won't animate correctly.
	mallet->ensureRigidSkeleton();
	
	// Because the mallet is a skinned model, it is not automatically assigned a bounding volume,
	// and will be be drawn even if it is not in front of the camera. We can leave it like this,
	// however, because the mallet is a complex model and is often out of view of the camera, we
	// can reduce processing costs by giving it a fixed bounding volume whose size we determine at
	// development time. We do this here by letting the mallet determine its own bounding volume,
	// which will be a spherical bounding volume that encompasses the mesh in its rest pose.
	// By using the shouldDrawAllBoundingVolumes property, we can check if this bounding volume
	// is appropriate. It turns out to be a bit too small, so we scale the bounding volume, to
	// increase its radius, so that it encompasses the mallet regardless of how the mallet is flexed.
	mallet->createSkinnedBoundingVolumes();
	mallet->getBoundingVolume()->scaleBy( 1.6f );
//	malletAndAnvils.shouldDrawAllBoundingVolumes = YES;	// Verify visually and adjust scale accordingly

	malletAndAnvils->setTouchEnabled( true );		// make the mallet touchable
	malletAndAnvils->setLocation( cc3v(300.0, 95.0, 300.0) );
	malletAndAnvils->setRotation( cc3v(0.0, -45.0, 0.0) );
	malletAndAnvils->setUniformScale( 0.15f );
	
	malletAndAnvils->runAction( CC3RepeatForever::create( CC3ActionAnimate::actionWithDuration(3.0f) ) );

	// Spin the mallet and anvils around for effect
	malletAndAnvils->runAction( CC3ActionRotateForever::actionWithRotationRate(cc3v(0.0, 10.0, 0.0)) );

	// Mallet is added on on background thread. Configure it for the scene, and fade it in slowly.
	configureForScene( malletAndAnvils, kFadeInDuration );
	addChild( malletAndAnvils );
}

void CC3MashUpScene::addSkinnedRunners()
{
	// Load the first running man from the POD file.
	CC3ResourceNode* runner = CC3PODResourceNode::nodeWithName( kRunnerName, kRunningManPODFile );
	
	// Retrieve the camera in the POD and cache it for later access. Adjust the camera
	// frustum to values that are more useful for this demo. Set the field of view to
	// be measured in the vertical orientation, because the runner is vertically oriented
	// and we want him to fit any shape surface in the vertical direction. This corresponds
	// to the common Hor+ FOV approach.
	_runnerCam = (CC3Camera*)runner->getNodeNamed( kRunnerCameraName );
	_runnerCam->setFieldOfViewOrientation( CC3FieldOfViewOrientationVertical );
	_runnerCam->setFarClippingDistance( getActiveCamera()->getFarClippingDistance() );
	_runnerCam->setHasInfiniteDepthOfField( true );

	// Retrieve the lamp in the POD, cache it for later access, and turn it off for now.
	_runnerLamp = (CC3Light*)runner->getNodeNamed( kRunnerLampName );
	_runnerLamp->setVisible( false );

	// Make the runner a little more visible underl lighting
	runner->setDiffuseColor( kCCC4FWhite );
	runner->setAmbientColor( kCCC4FWhite );

	runner->setTouchEnabled( true );		// make the runner touchable
	
	// Create a running track at the scene's center.
	// This "running track" is really just a structural node on which we can place the man and
	// then rotate the "track" to move the man. It's really just an invisible boom holding the man.
	CC3Node* runningTrack = CC3Node::nodeWithName( kRunningTrackName );
	runningTrack->setLocation( _ground->getLocation() );

	// Place the man on the track, near the edge of the ground frame
	runner->setLocation( cc3v(0, 0, 1100) );
	runner->setRotation( cc3v(0, 90, 0) );	// Rotate the entire POD resource so camera rotates as well
	runningTrack->addChild( runner );
	
	// The bones of the runner are fairly self-contained, and do not move beyond the sphere
	// that encompasses the rest pose of the mesh they are controlling. Because of this, we
	// can let each skin mesh node in the runner (there are 3 skin meshes) create its own
	// spherical bounding volume. If the initial bounding volume of any skin mesh node needs
	// to be adjusted in size, you can retrieve the skin mesh node from the runner model,
	// retrieve its bounding volume using the boundingVolume property, and scale it using
	// the scaleBy: method on the bounding volume. If you want to see the bounding volumes,
	// uncomment the second line below.
	runner->createSkinnedBoundingVolumes();
//	runner.shouldDrawAllBoundingVolumes = YES;	// Verify visually
	
	// Alternately, we can manually create a more accurate bounding volume manually as follows:
	//   - Start by extracting the bounding box of the rest pose of the model. Because this is
	//     happening before the scene has been updated, it is not always safe to invoke the
	//     boundingBox property at runtime because it can mess with node target alignment.
	//     If this issue arises (as it does here with the POD camera target), we can log the
	//     result at dev time and hardcode it.
	//   - Use the bounding box to create a fixed bounding volume around the model
	//   - Set the bounding volume into the root node of the skeleton within the model.
	//     This is the node that you want the bounding volume to track as it moves.
	//   - Use the shouldUseFixedBoundingVolume property to mark the bounding volume as
	//     having a fixed size.
	//   - Set the shouldDrawBoundingVolume property to YES to visualize the bounding volume.
	//   - Use the setSkeletalBoundingVolume: method on the entire model to force all skinned
	//     mesh nodes within the model to use the bounding volume being controlled by the skeleton.
	//   - Visually check the bounding volume. If okay, go with it.
	//   - Use CC3BoxTranslate and CC3BoxScale to modify the bounding box
	//     extracted from the model (or just hardcode a modified bounding box) to position
	//     and size the bounding volume around the model and verify visually.
//	LogTrace(@"Runner box: %@", NSStringFromCC3Box(runner.boundingBox));	// Extract bounding box
//	CC3Box bb = CC3BoxFromMinMax(cc3v(-76.982, 18.777, -125.259), cc3v(61.138, 268.000, 96.993));
//	bb = CC3BoxTranslateFractionally(bb, cc3v(0.0f, -0.1f, 0.1f));	// Move it if necessary
//	bb = CC3BoxScale(bb, cc3v(1.0f, 1.1f, 1.0f));					// Size it if necessary
//	CC3NodeBoundingVolume* bv = [CC3NodeSphereThenBoxBoundingVolume boundingVolumeCircumscribingBox: bb];
//	CC3Node* skeleton = [runner getNodeNamed: @"D_CharacterControl"];
//	skeleton.boundingVolume = bv;						// BV is controlled by skeleton root
//	skeleton.shouldUseFixedBoundingVolume = YES;		// Don't want the BV to change
////	skeleton.shouldDrawBoundingVolume = YES;			// Visualize the BV
////	[skeleton addAxesDirectionMarkers];					// Indicate the orientation
//	runner.skeletalBoundingVolume = bv;					// All skinned mesh nodes in model should use BV

	// Run, man, run!
	// The POD node contains animation to move the skinned character through a running stride.
	runner->runAction( CC3RepeatForever::create( CC3ActionAnimate::actionWithDuration(2.4f) ) );

	// Make him run around a circular track by rotating the "track" around the scene's center,
	// and it will carry the man around in a circle with it. By trial and error, set the rotation
	// to take 15 seconds for a full circle, to match the man's stride.
	runningTrack->runAction( CC3ActionRotateForever::actionWithRotationRate( cc3v(0.0, 12.0, 0.0) ) );

	// To demonstrate copying of skinned nodes, add another runner that is a copy, but smaller and
	// with a faster stride. We don't want the runner's POD camera or light, so we'll retrieve the
	// running figure from the POD resource and just copy that. This demonstrates how we can animate
	// either the whole POD resource node, or the specific soft-body node.
	std::string runnerFigureName = "man.pod-SoftBody";
	CC3Node* runnerFigure = runner->getNodeNamed( runnerFigureName.c_str() );
	CC3Node* littleBrother = (CC3Node*)runnerFigure->copyWithName( kLittleBrotherName );
	littleBrother->setUniformScale( 0.75f );
	littleBrother->setLocation( cc3v(0, 0, 1000) );
	littleBrother->setRotation( cc3v(0, 90, 0) );	// Copied runner was not rotated (its parent was)
	littleBrother->setTouchEnabled( true );			// make the runner touchable
	
	runningTrack->addChild( littleBrother );
	littleBrother->runAction( CC3RepeatForever::create( CC3ActionAnimate::actionWithDuration(1.6f) ) );

	// If cube-maps are available, give the little runner a reflective coating. This is done
	// by adding a static cube-map environment-map texture to, and setting the reflectivity of,
	// each mesh node contained within the smaller runner. You can adjust the value of the
	// reflectivity property to shor more or less of the runner's suit. Moving the reflectivity
	// towards 1 will make him appear like a little liquid-metal Terminator 2!
	littleBrother->addTexture( CC3Texture::textureCubeFromFilePattern( "EnvMap/EnvMap%s.jpg" ) );
	littleBrother->setReflectivity( 0.4f );
	littleBrother->release();

	// Runners are added on on background thread. Configure it for the scene, and fade it in slowly.
	configureForScene( runningTrack, kFadeInDuration );
	addChild( runningTrack );
}

void CC3MashUpScene::addTelevision()
{
	// Create an off-screen framebuffer surface, of a size and aspect (16x9) useful in an
	// HDTV model, backed by a blank color texture to which we can render. Alpha is not
	// required for the underlying texture, so we indicate the texture is opaque, which
	// uses a more memory-efficient 16-bit RGB format. Similarly, since stencils will not
	// be used, we allow a default 16-bit depth buffer to be used for this surface.
	CC3GLFramebuffer*  tvSurface = CC3GLFramebuffer::colorTextureSurfaceIsOpaque( true );
	tvSurface->setName( "Television" );
	tvSurface->setSize( kTVTexSize );

	// Now create a drawing visitor that will coordinate the drawing of the the TV screen
	// Since the aspect of the TV screen surface is different than the main display, we don't
	// want to reuse either the main camera, or the runner's camera. Instead, we create a
	// dedicated drawing visitor, with it's own camera, which we copy from the runner's camera.
	// and add it beside the runner's camera. We clear the new camera's existing viewport
	// so that it will be set to match the aspect of the TV screen.
	_tvDrawingVisitor = CC3NodeDrawingVisitor::visitor();
	_tvDrawingVisitor->retain();
	_tvDrawingVisitor->setRenderSurface( tvSurface );
	CC3Camera* tvCam = (CC3Camera*)_runnerCam->copy();
	_runnerCam->getParent()->addChild( tvCam );
	tvCam->setViewport( kCC3ViewportZero );		// Clear the camera viewport, so it will be set to match the TV surface
	_tvDrawingVisitor->setCamera( tvCam );
	tvCam->release();
	
	// Load a television model, extract the mesh node corresponding to the screen, and attach
	// the TV test card image as its texture. Since this is a TV, it should not interact with
	// lighting. Since we want to frequently access the TV screen mesh node, it is given a
	// useful name, and cached in an instance variable of this scene.
	CC3ResourceNode* tv = CC3PODResourceNode::nodeWithName( kTVName, kTVPODFile );
	tv->setLocation( cc3v(1000, -100.0, -1000) );	
	tv->setRotation( cc3v(0, -45, 0) );
	tv->setUniformScale( 750.0f );
	tv->setTouchEnabled( true );
	tv->setShouldCullBackFaces( false );				// Faces winding on decimated model is inconsistent
	_tvScreen = (CC3MeshNode*)tv->getNodeNamed( "tv_set-submesh3" );	// Auto-named by PVRGeoPOD
	_tvScreen->setName( kTVScreenName );							// Give it a more friendly name
	_tvScreen->setShouldUseLighting( false );
	_tvScreen->setEmissionColor( kCCC4FWhite );
	_tvScreen->setShouldCullFrontFaces( true );		// Don't paint both front and back of screen
	
	// Start with a test card displayed on the TV
	// Keep the mesh texture coordinates in memory so we can swap textures of different sizes
	_tvTestCardTex = CC3Texture::textureFromFile( kTVTestCardFile );
	_tvScreen->setTexture( _tvTestCardTex );
	_tvScreen->retainVertexTextureCoordinates();
	
	// TV is added on on background thread. Configure it for the scene, and fade it in slowly.
	configureForScene( tv, kFadeInDuration );
	addChild( tv );
	
	_isTVOn = false;		// Indicate TV is displaying test card


	// Start tv
	toggleTelevision();

	// Demonstrate the ability to extract a CCTexture from a CC3Texture.
	// For some interesting fun, extract a Cocos2D CCTexture instance from the texture
	// underpinning the TV surface, and replace the Cocos2D label node in the billboard held
	// by the robot arm with a CCSprite holding the CCTexture. The robot arm ends up holding
	// a smaller version of the TV screen. You have to touch the TV screen to activate it.
	// Because the TV screen is only updated with new rendered content when the big-screen
	// TV is viewable by the active camera, the portable TV held by the robot arm will only
	// display dynamic video if the larger TV is in the camera view as well. To view both
	// the big TV and the portable TV updating together, move behind the robot arm and look
	// back towards the big TV so that you can see both TV's displaying live video. You can
	// also change the optimization line in the drawSceneContentWithVisitor: method, and invoke
	// the drawToTVScreen method on each loop, to have both TV's show live video at all times.
//	CCSprite* portableTV = [CCSprite spriteWithTexture: [tvSurface.colorTexture ccTexture]];
//	portableTV.flipY = CCTexture.texturesAreLoadedUpsideDown;		// Cocos2D 3.1 & above takes care of flipping
//	CC3Billboard* bb = (CC3Billboard*)[self getNodeNamed: kBillboardName];
//	bb.uniformScale = 0.1;
//	bb.billboard = portableTV;
}

/** Convenience property that returns the rendering surface of the TV, cast to the correct class. */
CC3GLFramebuffer* CC3MashUpScene::getTVSurface() 
{
	return (CC3GLFramebuffer*)_tvDrawingVisitor->getRenderSurface(); 
}

void CC3MashUpScene::addPostProcessing() 
{
	// Create the off-screen framebuffer surface to render the scene to for post-processing effects.
	// We create the off-screen surface with the same size and format characteristics as the view's
	// rendering surface. We specifically use a renderable texture as the depth buffer, so that we
	// can use it to display the contents of the depth buffer as one post-processing option.
	// Otherwise, we could have just used the simpler renderbuffer option for the depth buffer.
	// Finally, we register the off-screen surface with the view's surface manager, so that the
	// off-screen surface will be resized automatically whenever the view is resized.
	CC3ViewSurfaceManager* surfMgr = CC3ViewSurfaceManager::sharedViewSurfaceManager();
	
	// depth format and type
	GLenum depthFormat = surfMgr->getDepthTexelFormat();
	GLenum depthType = surfMgr->getDepthTexelType();
	
	// color format and type
	GLenum colorFormat = surfMgr->getColorTexelFormat();
	GLenum colorType = surfMgr->getColorTexelType();

	// create depth attachment
	CC3Texture* pDepthTexture = CC3Texture::textureWithPixelFormat( depthFormat, depthType );
	CC3TextureFramebufferAttachment* pDepthAttachment = CC3TextureFramebufferAttachment::attachmentWithTexture( pDepthTexture );

	_postProcSurface = CC3GLFramebuffer::colorTextureSurfaceWithPixelFormat( colorFormat, colorType, pDepthAttachment );
	_postProcSurface->setName( "Post-proc surface" );

	surfMgr->addSurface( _postProcSurface );
	
	// Create a clip-space node that will render the off-screen color texture to the screen.
	// Load the node with shaders that convert the image into greyscale, making the scene
	// appear as if it was filmed with black & white film.
	_grayscaleNode = CC3ClipSpaceNode::nodeWithName( "Grayscale post-processor", _postProcSurface->getColorTexture() );
	_grayscaleNode->applyEffectNamedFromFile( "Grayscale", kPostProcPFXFile );
	
	// Create a clip-space node that will render the off-screen depth texture to the screen.
	// Load the node with shaders that convert the depth values into greyscale, visualizing
	// the depth of field as a grayscale gradient.
	_depthImageNode = CC3ClipSpaceNode::nodeWithName( "Depth-map post-processor", _postProcSurface->getDepthTexture() );
	_depthImageNode->applyEffectNamedFromFile( "Depth", kPostProcPFXFile );
}

void CC3MashUpScene::addPointParticles() 
{
	// Set up the emitter for 1000 particle, each an instance of the HangingParticle class.
	// Each particle has an individual location, color, and size.
	CC3PointParticleEmitter* emitter = CC3PointParticleEmitter::nodeWithName( "Particles" );
	//emitter.particleClass = [HangingPointParticle class];
	emitter->setMaximumParticleCapacity( 1000 );
	emitter->setVertexContentTypes( (CC3VertexContent)(kCC3VertexContentLocation |
								 kCC3VertexContentColor |
								 kCC3VertexContentPointSize) );
	
	// Set the emission characteristics
	emitter->setTexture( CC3Texture::textureFromFile(kPointParticleTextureFile) );
	emitter->setEmissionInterval( 0.01f );

	// Combination of particleSize and unity scale distance determine visible size
	// of each particle relative to the distance from the particle to the camera.
	emitter->setParticleSize( 70.0f );
	emitter->setUnityScaleDistance( 200.0f );

	// You can play with the following depth and blending parameters to see the effects
//	emitter.shouldDisableDepthTest = YES;
//	emitter.shouldDisableDepthMask = NO;
	ccBlendFunc blendFunc = {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};
	emitter->setBlendFunc( blendFunc );
//	emitter.blendFunc = (ccBlendFunc){GL_SRC_ALPHA, GL_ONE};	// Additive particles

	// Point particles assume lighting will not be used by default. Turn it on.
	emitter->setShouldUseLighting( true );

	// Shows the bounding volume. The boundingVolumePadding gives the boundary some depth
	// so that the emitter doesn't disappear if particles are still on-screen.
	emitter->setBoundingVolumePadding( 20.0f );
	emitter->setShouldDrawLocalContentWireframeBox( true );
	emitter->setShouldUseFixedBoundingVolume( false );

	emitter->setTouchEnabled( true );		// Shows the emitter name when touched
	
	// Set the location of the emitter, and optionally set it rotating for effect.
	emitter->setLocation( cc3v(0.0, 150.0, kParticlesPerSide * kParticlesSpacing / 2.0f) );
//	[emitter runAction: [CC3ActionRotateForever actionWithRotationRate: cc3v(0.0, 15.0, 0.0)]];
	addChild( emitter );

//	[emitter emitParticles: emitter.maximumParticleCapacity];	// Uncomment to get them all out at once
	emitter->play();
}

void CC3MashUpScene::addMeshParticles() 
{
#define kMeshParticleCubeExtent 10.0f
	CC3BoxNode* templateModel = CC3BoxNode::nodeWithName( kTexturedCubeName );
	templateModel->populateAsSolidBox( CC3BoxMake(-kMeshParticleCubeExtent,
														  -kMeshParticleCubeExtent, 
														  -kMeshParticleCubeExtent,
														   kMeshParticleCubeExtent,
														   kMeshParticleCubeExtent,
														   kMeshParticleCubeExtent) );
	// We get fancy here for the sake of it!
	// The texture file is actually a composite of two textures. We're only interested in the
	// bottom part of the texture, so we assign a texture rectangle to the template mesh.
	// Each HangingMeshParticle also assigns itself a smaller texture rectangle within the
	// texture rectangle of this template mesh. This demonstrates that particles can nest
	// an individual texture rectangle within the texture rectangle of the tempalte mesh.
	templateModel->setTexture( CC3Texture::textureFromFile( kMeshParticleTextureFile ) );
	templateModel->setTextureRectangle( CCRectMake(0, 0, 1, 0.75) );
	
	// Set up the emitter for 1000 particle, each an instance of the HangingParticle class.
	// Each particle has an individual location, color, size, and normal vector so that it
	// interacts with light sources. You can change this parameter to see different options.
	CC3MeshParticleEmitter* emitter = CC3MeshParticleEmitter::nodeWithName( "Particles" );
#pragma _NOTE_TODO( "Set particle class" )
	//emitter.particleClass = [HangingMeshParticle class];
	emitter->setParticleTemplate( templateModel );
	emitter->setMaximumParticleCapacity( 1000 );
	emitter->setVertexContentTypes( (CC3VertexContent)(kCC3VertexContentLocation |
								 kCC3VertexContentNormal |
								 kCC3VertexContentTextureCoordinates) );
	
	// Set the emission characteristics
//	emitter.texture = [CC3Texture textureFromFile: kCubeTextureFile];
	emitter->setEmissionInterval( 0.01f );
	
	// You can play with the following depth and blending parameters to see the effects
//	emitter.shouldDisableDepthTest = YES;
//	emitter.shouldDisableDepthMask = NO;
//	emitter.blendFunc = (ccBlendFunc){GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};
//	emitter.blendFunc = (ccBlendFunc){GL_SRC_ALPHA, GL_ONE};	// Additive particles
	
	// Uncomment to see effect of not using lighting.
	// Can also get same effect by not including kCC3VertexContentNormal in particle content above.
//	emitter.shouldUseLighting = NO;
//	emitter.emissionColor = kCCC4FWhite;
	
	// Shows the bounding volume. The boundingVolumePadding gives the boundary some depth
	// so that the emitter doesn't disappear if particles are still on-screen.
	emitter->setBoundingVolumePadding( 20.0f );
	emitter->setShouldDrawLocalContentWireframeBox( true );
	emitter->setShouldUseFixedBoundingVolume( false );
	
	emitter->setTouchEnabled( true );		// Shows the emitter name when touched
	
	// Set the location of the emitter, and optionally set it rotating for effect.
	emitter->setLocation( cc3v(0.0, 150.0, kParticlesPerSide * kParticlesSpacing / 2.0f) );
//	[emitter runAction: [CC3ActionRotateForever actionWithRotationRate: cc3v(0.0, 15.0, 0.0)]];
	addChild( emitter );
	
//	[emitter emitParticles: emitter.maximumParticleCapacity];	// Uncomment to get them all out at once
	emitter->play();
}

void CC3MashUpScene::addPointHose() 
{
	// Set up the emitter for CC3UniformlyEvolvingPointParticle particles. This specialized emitter
	// already comes with an appropriate particleNavigator and particleClass, which we do not need
	// to change for this usage. We don't need to set up a maxiumum capacity, because this emitter
	// reaches steady-state around 350 particles Each particle has an individual location, color
	// and size, but have no normal, so they do not interact with lighting. You can change this
	// property to see different options.
	CC3VariegatedPointParticleHoseEmitter* emitter = CC3VariegatedPointParticleHoseEmitter::nodeWithName( kPointHoseEmitterName );
	emitter->setVertexContentTypes( (CC3VertexContent)(kCC3VertexContentLocation |
								 kCC3VertexContentColor |
								 kCC3VertexContentPointSize) );
	
	// Set the emission characteristics
	emitter->setTexture( CC3Texture::textureFromFile( kPointParticleTextureFile ) );
	emitter->setEmissionRate( 100.0f );			// Per second
	
	// Combination of particleSize and unity scale distance determine visible size
	// of each particle relative to the distance from the particle to the camera.
	emitter->setUnityScaleDistance( 200.0 );

	// Optionally, set up for additive blending to create bright fire effect,
	// by uncommenting second line.
	ccBlendFunc blendFunc = {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};
	emitter->setBlendFunc( blendFunc );
//	emitter.blendFunc = (ccBlendFunc){GL_SRC_ALPHA, GL_ONE};
	
	// Point particles assume lighting will not be used by default.
	// To see the effect of lighting on the particles, uncomment the following line.
//	emitter.shouldUseLighting = YES;
	
	// The boundingVolumePadding gives the boundary some depth so that the emitter doesn't
	// disappear if particles are still on-screen.
	emitter->setBoundingVolumePadding( 20.0f );

	// Change this to YES to make boundary visible, and watch it change as the particles move around.
	emitter->setShouldDrawLocalContentWireframeBox( false );

	// By default, the bounding volume of the emitter is calculated dynamically as the particles
	// move around. You can avoid having to calculate the bounding volume on each frame, and
	// improve performance (a little bit), by setting a fixed boundary. To see the effect,
	// uncomment the following lines. The boundary below was determined by running with a
	// dynamic boundary and setting the shouldMaximize property of the bounding volume to YES
	// in order to determine the maximum range of the particles.
//	emitter.shouldUseFixedBoundingVolume = YES;
//	CC3NodeSphereThenBoxBoundingVolume* bv = (CC3NodeSphereThenBoxBoundingVolume*)emitter.boundingVolume;
//	bv.sphericalBoundingVolume.radius = 900.0;
//	bv.boxBoundingVolume.boundingBox = CC3BoxMake(-659.821, -408.596, -657.981, 651.606, 806.223, 637.516);

	// Shows the emitter name and location when the particles are touched
	emitter->setTouchEnabled( true );
	
	// We don't want the emitter's bounding volume (which will be quite large)
	// participating in any ray casting.
	emitter->setShouldIgnoreRayIntersection( true );

	// Configure the ranges for the beginning and ending particle size and color.
	emitter->setMinParticleStartingSize( 20.0f );
	emitter->setMaxParticleStartingSize( 40.0f );					// Starting at 20-40 pixels wide
	emitter->setMinParticleEndingSize( kCC3ParticleConstantSize );
	emitter->setMaxParticleEndingSize( kCC3ParticleConstantSize );	// Stay same size while alive
	emitter->setMinParticleStartingColor( kCCC4FDarkGray );
	emitter->setMaxParticleStartingColor( kCCC4FWhite );	 			// Mix of light colors
	emitter->setMinParticleEndingColor( kCC3ParticleFadeOut );
	emitter->setMaxParticleEndingColor( kCC3ParticleFadeOut );		// Fade out, but don't change color
	
	// Emitters can be assigned a particle navigator, which is responsible for configuring the life-cycle
	// and trajectory of the particle. A particle navigator is only involved in the initial configuration of the
	// particle. It does not interact with the particle once it has been emitted, unless the particle
	// accesses it directly. In this case, we establish a particle navigator to configure the starting
	// and ending state for the life-span, speed, size and color, to establish the evolution of the
	// particle throughout its life.
	CC3HoseParticleNavigator* particleNavigator = (CC3HoseParticleNavigator*)emitter->getParticleNavigator();
	particleNavigator->setMinParticleLifeSpan( 3.0f );
	particleNavigator->setMaxParticleLifeSpan( 4.0f );			// Each lives for 2-3 seconds
	particleNavigator->setMinParticleSpeed( 100.0f );
	particleNavigator->setMaxParticleSpeed( 200.0f );			// Travelling 100-200 units/second
	
	// Set the emitter to emit a thin stream. Since it's a small dispersion angle, the 
	// shouldPrecalculateNozzleTangents property will automatically be set to YES to avoid
	// performing two tanf function calls every time a particle is emitted.
	particleNavigator->setDispersionAngle( CCSizeMake(10.0, 10.0) );
	
	// Try a wider dispersion. This will automatically set the shouldPrecalculateNozzleTangents to NO.
//	particleNavigator.dispersionAngle = CGSizeMake(180.0, 180.0);
	
	// The hose navigator has a nozzle to direct the flow of the particles. The nozzle is actually
	// a CC3Node that can be located anywhere in the scene, can be attached to another node, and can
	// be oriented so as to to direct the stream of particles in a particular direction. In this case,
	// we'll place the hose navigator nozzle at the end of the robot arm so that it moves with the arm,
	// and point the nozzle out the end of the arm.
	CC3Node* nozzle = particleNavigator->getNozzle();
	nozzle->setLocation( cc3v( 0.0, 90.0, 0.0 ) );
	nozzle->setRotation( cc3v( -90.0, 30.0, 0.0 ) );

	getNodeNamed( kRobotTopArm )->addChild( nozzle );
	
	// Add the emitter to the scene
	addChild( emitter );

	// To see the bounding volume of the emitter, set this shouldDrawBoundingVolume property to YES.
	// The bounding volume of most nodes contains both a spherical and bounding-box bounding volume
	// to optimize intersection testing. Touch the robot arm to see the bounding volume of the
	// particle emitter grow and shrink dynamically. Back the camera away to get the full effect.
	emitter->setShouldDrawBoundingVolume( false );

	emitter->play();
}

void CC3MashUpScene::addMeshHose() 
{
	#define kPartMeshDim 3.0f
	
	// The template mesh node defines the mesh (and optionally the material) that is used
	// for each particle. Each particle is constucted as a transformed copy of the template mesh.
	// This particle emitter can have multiple template meshes, to allow more than one particle
	// shape. In this case, we create an emitter that emits both spheres and boxes.
	
	// Because an emitter draws all particles with a single GL draw call, both template meshes
	// must use the same texture. We create a single texture file that is a combination of
	// two textures, and assign a different textureRectangle to each template mesh. The particles
	// that use each mesh will inherit their texture rectangle. We can even assign an individual
	// texture rectangle to each particle (although that is not done in this example), to allow
	// each particle to use a different part of the textureRectangle assigned to the template mesh,
	// allowing each partile to appear to be textured individually.

	// Box template mesh
	CC3BoxNode* boxModel = CC3BoxNode::node();
	boxModel->populateAsSolidBox( CC3BoxMake(-kPartMeshDim, -kPartMeshDim, -kPartMeshDim,
													  kPartMeshDim, kPartMeshDim, kPartMeshDim) );
	boxModel->setTexture( CC3Texture::textureFromFile( kMeshParticleTextureFile ) );
	boxModel->setTextureRectangle( CCRectMake(0, 0, 1, 0.75) );	// Bottom part of texture is box texture
	CC3Mesh* boxMesh = boxModel->getMesh();

	// Sphere template mesh
	CC3MeshNode* ballModel = CC3SphereNode::node();
	ballModel->populateAsSphereWithRadius( (kPartMeshDim * 1.5f), CC3TessellationMake(8, 7) );
	ballModel->setTexture( CC3Texture::textureFromFile(kMeshParticleTextureFile) );
	ballModel->setTextureRectangle( CCRectMake(0, 0.75, 1, 0.25) );	// Top part of texture is ball texture
	CC3Mesh* ballMesh = ballModel->getMesh();
	
	// Set up the emitter to emit mesh particles of type CC3UniformlyEvolvingMeshParticle constructed as
	// copies of the particle template mesh. We don't need to set up a maxiumum capacity, because this
	// emitter reaches steady-state after a couple of hundred particles. We include color content type,
	// and set an alpha-blending function on the emitter material so that the particles can be faded.
	// We assign two separate template meshes. Each particle will be assigned one of these meshes at
	// random as it is emitted.
	CC3MultiTemplateMeshParticleEmitter* emitter = CC3MultiTemplateMeshParticleEmitter::nodeWithName( kMeshHoseEmitterName );
	emitter->setVertexContentTypes(( CC3VertexContent)(kCC3VertexContentLocation |
								 kCC3VertexContentNormal |
								 kCC3VertexContentColor |
								 kCC3VertexContentTextureCoordinates) );
	//emitter.particleClass = [RotatingFadingMeshParticle class];
	emitter->addParticleTemplateMesh( boxMesh );
	emitter->addParticleTemplateMesh( ballMesh );
	emitter->setTexture( CC3Texture::textureFromFile(kMeshParticleTextureFile) );
	ccBlendFunc blendFunc = {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};
	emitter->setBlendFunc( blendFunc );
	
	// Set the emission characteristics
	emitter->setEmissionRate( 50.0f );				// Per second

	// The boundingVolumePadding gives the boundary some depth so that the emitter doesn't
	// disappear if particles are still on-screen.
	emitter->setBoundingVolumePadding( 20.0f );
	
	// Change this to YES to make boundary visible, and watch it change as the particles move around.
	emitter->setShouldDrawLocalContentWireframeBox( false );
	
	// By default, the bounding volume of the emitter is calculated dynamically as the particles
	// move around. By using a fixed bounding volume, you can improve performance in two ways:
	// A fixed bounding volume avoids the need to have to calculate the bounding volume on each frame,
	// as the particles move around, and if the bounding volume is outside the camera's frustum,
	// the vertices of the paricles do not need to be transformed.
	// To see the effect, uncomment the following lines. The boundary below was determined by running
	// with a dynamic boundary and setting the shouldMaximize property of the bounding volume to YES
	// in order to determine the maximum range of the particles (and logging the result).
//	emitter.shouldUseFixedBoundingVolume = YES;
//	CC3NodeSphereThenBoxBoundingVolume* bv = (CC3NodeSphereThenBoxBoundingVolume*)emitter.boundingVolume;
//	bv.sphericalBoundingVolume.radius = 750.0;
//	bv.boxBoundingVolume.boundingBox = CC3BoxMake(-400.0, -100.0, -500.0, 500.0, 750.0, 500.0);
	
	// Even without a fixed bounding volume, you can still improve performance when the particles
	// are not in view of the camera by setting the following property to NO.
	// However, this can sometimes create a visually jarring effect when the particles come back
	// into view. See the notes of the shouldTransformUnseenParticles property for more info.
	emitter->setShouldTransformUnseenParticles( true );
	
	// Shows the emitter name and location when the particles are touched
	emitter->setTouchEnabled( true );
	
	// We don't want the emitter's bounding volume (which will be quite large)
	// participating in any ray casting.
	emitter->setShouldIgnoreRayIntersection( true );
	
	// Emitters can be assigned a particle navigator, which is responsible for configuring the
	// life-cycle and trajectory of the particle. A particle navigator is only involved in the
	// initial configuration of the particle. It does not interact with the particle once it has
	// been emitted, unless the particle accesses it directly. In this case, we establish a particle
	// navigator to configure the starting and ending state for the life-span and speed, to establish
	// the evolution of the particle throughout its life.
	CC3HoseParticleNavigator* particleNavigator = CC3HoseParticleNavigator::navigator();
	particleNavigator->setMinParticleLifeSpan( 4.0f );
	particleNavigator->setMaxParticleLifeSpan( 6.0f );			// Each lives for a few seconds
	particleNavigator->setMinParticleSpeed( 50.0f );
	particleNavigator->setMaxParticleSpeed( 100.0f );			// Travelling units/second
	
	// Set the emitter to emit a thin stream. Since it's a small dispersion angle, the 
	// shouldPrecalculateNozzleTangents property will automatically be set to YES to avoid
	// performing two tanf function calls every time a particle is emitted.
	particleNavigator->setDispersionAngle( CCSizeMake(10.0, 10.0) );
	
	// Try a wider dispersion. This will automatically set the shouldPrecalculateNozzleTangents to NO.
//	particleNavigator.dispersionAngle = CGSizeMake(180.0, 180.0);
	
	// Assign the navigator to the emitter.
	emitter->setParticleNavigator( particleNavigator );
	
	// The hose navigator has a nozzle to direct the flow of the particles. The nozzle is actually
	// a CC3Node that can be located anywhere in the scene, can be attached to another node, and can
	// be oriented so as to to direct the stream of particles in a particular direction. In this case,
	// we'll place the hose navigator nozzle at the end of the robot arm so that it moves with the arm,
	// and point the nozzle out the end of the arm.
	CC3Node* nozzle = particleNavigator->getNozzle();
	nozzle->setLocation( cc3v( 0.0, 90.0, 0.0 ) );
	nozzle->setRotation( cc3v( -90.0, 30.0, 0.0 ) );

	getNodeNamed( kRobotTopArm )->addChild( nozzle );
	
	// Add the emitter to the scene
	addChild( emitter );
	
	// To see the bounding volumes of the emitter, set the shouldDrawBoundingVolume to YES.
	// The bounding volume of most nodes contains both a spherical and bounding-box
	// bounding volume to optimize intersection testing. Touch the robot arm to see
	// the bounding volume of the particle emitter grow and shrink dynamically.
	// Use the joystick controls to back the camera away to get the full effect.
	emitter->setShouldDrawBoundingVolume( false );
}

void CC3MashUpScene::addReflectiveMask() 
{
	// To show it is possible to load PFX files from other directories, we copy the POD and
	// PFX and texture files to the application Document directory. Actually, by this time,
	// the PFX file has already been loaded in the preloadAssets method, so to have this work,
	// you need to comment out the line in the preloadAssets method that preloads the
	// kReflectivePFXFile PFX file.
	//[self copyResourceToDocuments: kReflectiveMaskPODFile];
	//[self copyResourceToDocuments: kReflectivePFXFile];
	//[self copyResourceToDocuments: @"BrushedSteel.png"];
	//[self copyResourceToDocuments: @"tex_base.bmp"];	// Not really needed, but referenced in the POD file
	//NSString* docDir = [NSHomeDirectory() stringByAppendingPathComponent: @"Documents"];
	//NSString* podPath = [docDir stringByAppendingPathComponent: kReflectiveMaskPODFile];
	CC3ResourceNode* podRezNode = CC3PODResourceNode::nodeFromFile( kReflectiveMaskPODFile );
	CC3MeshNode* mask = (CC3MeshNode*)podRezNode->getNodeNamed( "maskmain" );

	
	// The mask animation locates the mask at a distant location and scale. Wrap it in a holder
	// to move it to a more convenient location and scale. Remember that the location of the mask
	// within the holder (and therefore the required offset) scales as the holder scales!!
	CC3Node* maskHolder = mask->asOrientingWrapper();
	maskHolder->setUniformScale( 4.0f );
	CC3Vector maskOffset = mask->getLocation().scaleUniform( maskHolder->getUniformScale() );
	maskHolder->setLocation( cc3v(-750.0, 100.0, -500.0).difference( maskOffset ) );

	// Mask is added on on background thread. Configure it for the scene, and fade it in slowly.
	configureForScene( maskHolder, kFadeInDuration );
	addChild( maskHolder );
	
	// The vertex shader defines a uniform named "CustomMatrix" which uses an app-supplied
	// 4x4 matrix to adjust the position of the vertices. This "CustomMatrix" does not map
	// to a standard semantic, so it must be populated directly by the application, or use
	// a default value. Setting the uniform directly is done by creating an override uniform
	// on the shader context of a specific mesh node. Each node can set a different value for
	// the customized variable. This technique can also be used to override the value of a
	// uniform variable whose content can actually be retrieved from the environment.
	// Alternately, the shader program can use a default value for the uniform. By default,
	// this is disabled, but can be enabled by setting the shouldAllowDefaultVariableValues
	// property to YES. Since this app uses picking nodes from touch events, the property must
	// also be enabled in both the shader program and the pure color shader program used for
	// picking nodes. To use a default value instead, comment out the first four lines here,
	// and uncomment the last two.
	CC3GLSLUniform* progUniform = mask->getShaderContext()->getUniformOverrideNamed( "CustomMatrix" );
	CC3Matrix4x4 customMtx;
	CC3Matrix4x4PopulateIdentity(&customMtx);	// Could be any matrix...but just make it an identity
	if ( progUniform ) 
		progUniform->setMatrix4x4( &customMtx );
//	mask.shaderProgram.shouldAllowDefaultVariableValues = YES;
//	mask.shaderContext.pureColorProgram.shouldAllowDefaultVariableValues = YES;

	// Make the mask touchable and animate it.
	mask->setTouchEnabled( true );
	mask->runAction( CC3RepeatForever::create(CC3ActionAnimate::actionWithDuration( 10.0f )) ); 
}

void CC3MashUpScene::addEtchedMask() 
{
	CC3ResourceNode* podRezNode = CC3PODResourceNode::nodeFromFile( kEtchedMaskPODFile );
	CC3MeshNode* mask = (CC3MeshNode*)podRezNode->getNodeNamed( "objmaskmain" );
	
	// Load the textures into the material (bump-mapped texture first), and allow the default shaders
	// to perform the bump-mapping. The default shaders include lighting and material coloring,
	// and so are more realistic than the shaders defined in the PFX file.
	mask->addTexture( CC3Texture::textureFromFile( "Masks/NormalMap.png" ) );
	mask->addTexture( CC3Texture::textureFromFile( "Masks/BaseTex.png" ) );
	
	// Alternately, you can apply a PFX effect to the mask node. This will attach the GL program and
	// texture defined in the PFX effect file, which will run shaders dedicated to tangent-space
	// bump-mapping. The shaders in the PFX file are simpler than the default shaders and do not
	// interact with material and lighting as effectively, and are therefore not as accurate.
	// To load the PFX file, uncomment the following line.
//	[mask applyEffectNamed: kEtchedMaskPFXEffect inPFXResourceFile: kEtchedPFXFile];
	
	mask->setUniformScale( 4.0f );
	mask->setLocation( cc3v(-750.0, 50.0, -500.0) );
	
	// Mask is added on on background thread. Configure it for the scene, and fade it in slowly.
	configureForScene( mask, kFadeInDuration );
	addChild( mask );
	
	// Make the mask touchable and animate it.
	mask->setTouchEnabled( true );
	mask->runAction( CC3ActionRotateForever::actionWithRotationRate( cc3v(0.0, 30.0, 0.0) ) );
}

void CC3MashUpScene::addDragon() 
{
	CC3ResourceNode* dgnRez = CC3PODResourceNode::nodeFromFile( "models/Dragon.pod" );
	_dragon = dgnRez->getNodeNamed( "Dragon.pod-SoftBody" );
	_dragon->setTouchEnabled( true );
	
	// The bones of the dragon are fairly self-contained, and do not move beyond the sphere
	// that encompasses the mesh rest pose. Because of this, we can let the dragon skin mesh
	// node create its own spherical bounding volume. We then use that same bounding volume
	// for the other skinned mesh nodes within the dragon model (ie- the mouth), by using the
	// setSkeletalBoundingVolume: method on the entire model.
	// If you want to see this bounding volume, uncomment the third line below.
	CC3MeshNode* dgnBody = (CC3MeshNode*)_dragon->getNodeNamed( "Dragon" );
	dgnBody->createBoundingVolume();
//	dgnBody.shouldDrawBoundingVolume = YES;
	_dragon->setSkeletalBoundingVolume( dgnBody->getBoundingVolume() );

	// Ensure the bones in the dragon are rigid (no scale applied). Doing this allows the
	// shader program that is optimized for that to be automatically selected for the skinned
	// mesh nodes in the dragon. Many more active bones are possible with a rigid skeleton.
	// The model must be designed as a rigid model, otherwise it won't animate correctly.
	_dragon->ensureRigidSkeleton();
	
	// The model animation that was loaded from the POD into track zero is a concatenation of
	// several separate movements, such as gliding and flapping. Extract the distinct movements
	// from the base animation and add those distinct movement animations as separate tracks.
	_dragonGlideTrack = _dragon->addAnimationFromFrame( 0, 60 );
	_dragonFlapTrack = _dragon->addAnimationFromFrame( 61, 108 );
	
	// The dragon model now contains three animation tracks: a gliding track, a flapping track,
	// and the original concatenation of animation loaded from the POD file into track zero.
	// Any of these tracks can be played or blended by adjusting the relative weightings of each
	// track. We want to start with the dragon flying and flapping its wings. So, we give the
	// flapping track a weight of one, and the gliding and original tracks a weighting of zero.
	// In general, once the movement tracks have been created, you will set track zero, containing
	// the original animation concatenation, to a weighting of zero, and leave it there.
	_dragon->setAnimationBlendingWeight( 0.0f, 0 );
	_dragon->setAnimationBlendingWeight( 0.0f, _dragonGlideTrack );
	_dragon->setAnimationBlendingWeight( 1.0f, _dragonFlapTrack );
	
	// Now create an animate action to actually run the flapping animation, and make it repeat
	// in a loop. We give it a known tag so that we can identify it to stop it later, after
	// we transition to a different movement.
	CC3Action* flapping = CC3RepeatForever::create( CC3ActionAnimate::actionWithDuration( 1.5f,_dragonFlapTrack ) );
	flapping->setTag( kFlappingActionTag );
	_dragon->runAction( flapping );
	_dragonMotion = kDragonFlapping;	// Keep track of which animation is currently active
	
	// Add the dragon to a wrapper node that can be rotated to make the dragon fly in a circular
	// path. Locate the dragon up and away from the point of rotation of the wrapper, and rotate
	// the dragon itself within the wrapper, to make it face the direction of rotation, which is
	// the direction it is flying. Then add the flight path wrapper to the scene.
	CC3Node* flightPath = _dragon->asOrientingWrapper();
	_dragon->setLocation( cc3v(0, 500, 1300) );
	_dragon->setRotation( cc3v(0, -90, 15) );
	_dragon->setUniformScale( 5.0 );
	flightPath->runAction( CC3ActionRotateForever::actionWithRotationRate(cc3v(0, -15, 0)) );

	// Dragon is added on on background thread. Configure it for the scene, and fade it in slowly.
	configureForScene( _dragon, kFadeInDuration );
	addChild( flightPath );
}

/**
 * Adds a temporary fiery explosion on top of the specified node, using a Cocos2D
 * CCParticleSystem. The explosion is set to a short duration, and when the particle
 * system has exhausted, the CC3ParticleSystem node along with the CCParticleSystem
 * billboard it contains are automatically removed from the 3D scene.
 */
void CC3MashUpScene::addExplosionTo( CC3Node* aNode )
{
	// Create the particle emitter with a finite duration, and set it to auto-remove
	// once it is exhausted.
	CCParticleSystem* emitter = CCParticleFire::create();
	emitter->setPosition( ccp(0.0, 0.0) );
	emitter->setDuration( 0.75f );
	emitter->setAutoRemoveOnFinish( true );
	
	// Create the 3D billboard node to hold the 2D particle emitter.
	// The bounding volume is removed so that the flames will not be culled as the
	// camera pans away from the flames. This is suitable since the particle system
	// only exists for a short duration.
	CC3ParticleSystemBillboard* bb = CC3ParticleSystemBillboard::nodeWithName( kExplosionName, emitter );
	
	// A billboard can be drawn either as part of the 3D scene, or as an overlay
	// above the 3D scene. By commenting out one of the following sections of code,
	// you can choose which method to use.
	
	// 1) In the 3D scene.
	// The following lines wrap the emitter billboard in a wrapper that will find
	// and track the camera in 3D. The flames can be occluded by other nodes between
	// the explosion and the camera.

	bb->setUniformScale( 0.25f * (1.0f / aNode->getUniformScale()) );	// Find a suitable scale
	bb->setShouldInheritTouchability( false );						// Don't allow flames to be touched

	// If the 2D particle system uses point particles instead of quads, attenuate the
	// particle sizes with distance realistically. This is not needed if the particle
	// system will always use quads, but it doesn't hurt to set it.
	bb->setParticleSizeAttenuation( CC3AttenuationCoefficientsMake(0.05f, 0.02f, 0.0001f) );
	
	// 2D particle systems do not have a real contentSize and boundingBox, so we need to
	// calculate it dynamically on each update pass, or assign one that will cover the
	// area that will be used by this particular particle system. This bounding rectangle
	// is specified in terms of the local coordinate system of the particle system and
	// will be scaled and transformed as the node is transformed. By setting this once,
	// we don't need to calculate it while running the particle system.
	// To calculate it dynamically on each update instead, comment out the following line,
	// and uncomment the line after. And also uncomment the third line to see the bounding
	// box drawn and updated on each frame.
	bb->setBillboardBoundingRect( CCRectMake(-90.0, -50.0, 190.0, 340.0) );
//	bb.shouldAlwaysMeasureBillboardBoundingRect = YES;
//	bb.shouldDrawLocalContentWireframeBox = YES;

	// How did we determine the billboardBoundingRect? This can be done by trial and
	// error, by uncommenting culling logging in the CC3Billboard doesIntersectBoundingVolume:
	// method. Or it is better done by changing LogTrace to LogDebug in the CC3Billboard
	// billboardBoundingRect property accessor method, commenting out the line above this
	// comment, and uncommenting the following line. Doing so will cause an ever expanding
	// bounding box to be logged, the maximum size of which can be used as the value to
	// set in the billboardBoundingRect property.
//	bb.shouldMaximizeBillboardBoundingRect = YES;

	// We want to locate the explosion between the node and the camera, so that it
	// appears to engulf the node. To do this, wrap the billboard in an orientating
	// wrapper, give the explosion a location offset, and make the wrapper track
	// the camera. This will keep the explosion between the node and the camera,
	// regardless of where they are.
	// If we didn't need the locational offset to place the explosion in front
	// of the camera, we could have the billboard itself track the camera
	// using the shouldAutotargetCamera property of the billboard itself.
	bb->setLocation( cc3v(0.0, 0.0, 0.5) );
	aNode->addChild( bb->asCameraTrackingWrapper() );

	// 2) Overlaid above the 3D scene.
	// The following lines add the emitter billboard as a 2D overlay that draws above
	// the 3D scene. The flames will not be occluded by any other 3D nodes.
	// Comment out the lines in section (1) just above, and uncomment the following lines:
//	emitter.positionType = kCCPositionTypeGrouped;
//	bb.shouldDrawAs2DOverlay = YES;
//	bb.unityScaleDistance = 180.0;
//	[aNode addChild: bb];
}

/**
 * This scene has custom drawing requirements, perfoming multiple passes, based on user interaction.
 *
 * If the user has turned on the TV in the scene, we render one pass of the scene from the
 * point of view of the camera that travels with the runners into a texture that is then
 * displayed on the TV screen during the main scene rendering pass.
 *
 * If the user has selected one of the post-processing options via the lighting button, the 
 * primary rendering pass is rendered to a texture, which is then presented to the view surface
 * as a quad via a single-node rendering pass.
 */
void CC3MashUpScene::drawSceneContentWithVisitor( CC3NodeDrawingVisitor* visitor ) 
{
	illuminateWithVisitor( visitor );			// Light up your world!
	
	// If the TV is on and the TV is in the field of view of the primary camera viewing
	// the scene, draw the scene to the TV screen.
	if ( _isTVOn && _tvScreen->doesIntersectFrustum( visitor->getCamera()->getFrustum() ) )
		drawToTVScreen();
	
	// As a pre-processing pass, if the reflective metal teapot is visible, generate an
	// environment-map cube-map texture for it by taking snapshots of the scene in all
	// six axis directions from its position.
	generateTeapotEnvironmentMapWithVisitor( visitor );

	// When post-processing, we render to a temporary off-screen surface.
	// We remember the original surface in this variable.
	CC3RenderSurface* origSurface = NULL;
	
	// If we are post-processing the rendered scene image, draw to an off-screen surface,
	// clearing it first. Otherwise, draw to view surface directly, without clearing because
	// it was done at the beginning of the rendering cycle.
	if ( isPostProcessing() )
	{
		origSurface = visitor->getRenderSurface();
		visitor->setRenderSurface( _postProcSurface );
		_postProcSurface->clearColorAndDepthContent();
	}
	
	drawBackdropWithVisitor( visitor );	// Draw the backdrop if it exists
	visitor->visit( this );				// Draw the scene components
	
	// Shadows are drawn with a specialized visitor

	if ( getShadowVisitor() )
	{
		getShadowVisitor()->alignShotWith( visitor );
		drawShadowsWithVisitor( getShadowVisitor() );
	}

	// If we are post-processing the rendered scene image, draw the appropriate off-screen
	// surface to the view surface.
	if ( isPostProcessing() ) 
	{
		// If the layer is not full-screen, rendering the layer again will further reduce
		// its shape. To compensate, temporarily clear the camera viewport so that it will
		// be set to the size of the surface when the surface is attached to the visitor
		// so that it draws the post-processed image to the entire view surface.
		CC3Viewport vvp = visitor->getCamera()->getViewport();
		visitor->getCamera()->setViewport( kCC3ViewportZero );
		
		visitor->setRenderSurface( origSurface );		// Ensure drawing to the original surface
		visitor->visit( getPostProcessingNode() );

		visitor->getCamera()->setViewport( vvp );		// Now set the viewport back to the layer's size.
	}
}

/**
 * When drawing an environment map, don't bother with shadows, avoid all the post-rendering
 * processing, don't redraw the TV. And avoid an infinitely recursive issue where rendering
 * the scene for the texture triggers a recursive nested scene render!
 */
void CC3MashUpScene::drawSceneContentForEnvironmentMapWithVisitor( CC3NodeDrawingVisitor* visitor ) 
{
	visitor->getRenderSurface()->clearColorAndDepthContent();
	drawBackdropWithVisitor( visitor );			// Draw the backdrop if it exists
	visitor->visit( this );						// Draw the scene components
}

/**
 * Draws the scene from the runners' camera perspective to the TV screen.
 *
 * The drawing is performed by a dedicated drawing visitor that contains its own camera
 * in the same location as the camera loaded with the runners. A dedicated camera is used
 * because the aspect of the TV screen (HDTV) is different than the main scene view.
 */
void CC3MashUpScene::drawToTVScreen() 
{
	//LogTrace(@"Drawing to TV");

	_tvDrawingVisitor->getRenderSurface()->clearColorAndDepthContent();	// Clear color & depth of TV surface.
	drawBackdropWithVisitor( _tvDrawingVisitor );				// Draw the backdrop if it exists
	_tvDrawingVisitor->visit( this );								// Draw the scene components
	
	pictureInPicture();		// Add a small PiP image in the bottom right of the TV screen
}

/**
 * Adds a small picture-in-picture window in the TV screen by directly changing the TV screen texture.
 *
 * This is done by copying a rectangle of pixels from the rendering surface used to render
 * the TV image, and then pasting them into a different location in the surface. Along the
 * way, a border is added to the PiP image by directly setting pixel values.
 */
void CC3MashUpScene::pictureInPicture() 
{
	// Define the source and destination rectangles within the surface
	CC3IntSize pipSize = CC3IntSizeMake(kTVScale * 2, kTVScale * 2);
	CC3IntPoint pipSrcOrg = CC3IntPointMake((10 * kTVScale), (5 * kTVScale));
	CC3IntPoint pipDstOrg = CC3IntPointMake((13.5 * kTVScale), (0.5 * kTVScale));
	CC3Viewport pipSrc = CC3ViewportFromOriginAndSize(pipSrcOrg, pipSize);
	CC3Viewport pipDst = CC3ViewportFromOriginAndSize(pipDstOrg, pipSize);
	
	// Allocate a temporary array to hold the extracted image content
	ccColor4B* colorArray = new ccColor4B[pipSize.width * pipSize.height];
	
	// Copy a rectangle of image content from the surface, add the border,
	// and paste it to a different location on the surface.
	getTVSurface()->readColorContentFrom( pipSrc, colorArray );
	addBorderToImage( colorArray, pipSize );
	getTVSurface()->replaceColorPixels( pipDst, colorArray );

	CC_SAFE_DELETE_ARRAY( colorArray );
}

/** 
 * Adds a border around the specified image array, which is of the specified rectangular size.
 *
 * Content in the specified image array is ordered from left to right across each row of
 * pixels, starting the row at the bottom, and ending at the row at the top.
 */
void CC3MashUpScene::addBorderToImage( ccColor4B* colorArray, const CC3IntSize& imgSize )
{
	ccColor4B borderColor = ccc4(128, 0, 0, 255);

	// Add the top and bottom borders
	GLuint topRowStart = imgSize.width * (imgSize.height - 1);
	for (GLuint colIdx = 0; colIdx < (GLuint)imgSize.width; colIdx++) 
	{
		colorArray[colIdx] = borderColor;					// Bottom row
		colorArray[topRowStart + colIdx] = borderColor;		// First row
	}
	
	// Add the left and right borders
	for (GLuint rowIdx = 0; rowIdx < (GLuint)imgSize.height; rowIdx++) 
	{
		colorArray[imgSize.width * rowIdx] = borderColor;				// First column in row
		colorArray[imgSize.width * (rowIdx + 1) - 1] = borderColor;		// Last column in row
	}
}

/** Returns whether post-processing of the scene view is active. */
bool CC3MashUpScene::isPostProcessing() 
{
	switch (_lightingType) 
	{
		case kLightingFog:
			return CC3_GLSL;		// Fog is performed as a post-processing if shaders are available
		case kLightingGrayscale:
			return true;
		case kLightingDepth:
			return true;
		default:
			return false;
	}
}

/** Returns the appropriate full-screen rendering node for the current lighting conditions. */
CC3MeshNode* CC3MashUpScene::getPostProcessingNode()
{
	switch (_lightingType) 
	{
		case kLightingFog:
			return _fog;
		case kLightingGrayscale:
			return _grayscaleNode;
		case kLightingDepth:
			return _depthImageNode;
		default:
			return NULL;
	}
}

/** 
 * If we're not already in the middle of generating an environment map, and the reflective metal
 * teapot is visible, generate an environment-map cube-map texture for it by taking snapshots of
 * the scene in all six axis directions from its position. We don't want the teapot to
 * self-reflect, so we make it invisible while we are rendering the scene from the teapot's center.
 */
void CC3MashUpScene::generateTeapotEnvironmentMapWithVisitor( CC3NodeDrawingVisitor* visitor ) 
{
	if ( _teapotTextured == NULL )
		return;

	if ( visitor->isDrawingEnvironmentMap() || !_teapotTextured->doesIntersectFrustum( visitor->getCamera()->getFrustum() ) )
		return;

	bool isVis = _teapotTextured->isVisible();
	_teapotTextured->setVisible( false );			// Hide the teapot from itself
	_envMapTex->generateSnapshotOfScene( this, _teapotTextured->getGlobalCenterOfGeometry() );
	_teapotTextured->setVisible( isVis );
}

/**
 * This method is invoked when the scene is opened within the layer (or when the
 * scene is added to an existing running layer). We can use this method to perform
 * set-up that depends on the layer being attached and sized, such as camera actions
 * that require the dimensions of the viewport to be established.
 *
 * During development, we can use this opportunity to move the camera to view the
 * entire scene, or some section of the scene, in order to troubleshoot the scene.
 */
void CC3MashUpScene::onOpen() 
{
	// Add post-processing capabilities, demonstrating render-to-texture and post-rendering
	// image processing. This is performed here, rather than in initializeScene, so that
	// we can make the off-screen surfaces the same size as the view surface, which is not
	// available during initializeScene.
	addPostProcessing();
	
	// Adds fog to the scene. This is initially invisible.
	// This is performed here, rather than in initializeScene, because under OpenGL ES 2.0
	// and OpenGL OSX, fog is created as a post-processing effect, and so we need access to
	// the off-screen surfaces created in addPostProcessing.
//	addFog();
	
	// Here, we add additional scene content dynamically and asynchronously on a background thread
	// using the CC3Backgrounder singleton. Asynchronous loading must be initiated after the scene
	// has been attached to the view, and should not be started in the initializeScene method.
	// When running on Android, background loading can cause threading conflicts within the GL engine,
	// depending on the device, and must be handled with extreme care. Because of this, if running
	// under Android, we turn background loading off here and the addSceneContentAsynchronously
	// method will run immediately on this thread, before further processing is performed.
//#if APPORTABLE
//	CC3Backgrounder.sharedBackgrounder.shouldRunTasksOnRequestingThread = YES;
//#endif
//	[CC3Backgrounder.sharedBackgrounder runBlock: ^{ [self addSceneContentAsynchronously]; }];

	// Uncomment the first line to have the camera move to show the entire scene.
	// Uncomment the second line to draw the bounding box of the scene.
//	[self.activeCamera moveWithDuration: kCameraMoveDuration toShowAllOf: self];
//	self.shouldDrawWireframeBox = YES;

	// Or uncomment this line to have the camera pan and zoom to focus on the Cocos3D mascot.
//	[self.activeCamera moveWithDuration: kCameraMoveDuration toShowAllOf: mascot];
	addSceneContentAsynchronously();
}

/** 
 * Called periodically as part of the CCLayer scheduled update mechanism.
 * This is where model objects are updated.
 *
 * For this scene, the camera direction and location are updated
 * under control of the user interface, and the location of the white teapot that
 * indicates the direction of the light is updated as the light moves. All other motion
 * in the scene is handled by CCActions.
 */
void CC3MashUpScene::updateBeforeTransform( CC3NodeUpdatingVisitor* visitor )
{
	updateCameraFromControls( visitor->getDeltaTime() );
	
	// To show where the POD light is, track the small white teapot to the light location.
	if ( _teapotWhite )
		_teapotWhite->setLocation( _robotLamp->getLocation() );
 }

/** After all the nodes have been updated, check for collisions. */
void CC3MashUpScene::updateAfterTransform( CC3NodeUpdatingVisitor* visitor ) 
{
	checkForCollisions();
}

/**
 * Check for collisions.
 *
 * Invoke the doesIntersectNode: method to determine whether the rainbow teapot has
 * collided with the brick wall.
 *
 * If the teapot is colliding with the wall, it may do so for several update frames.
 * On each frame, we need to determine whether it is heading towards the wall, or
 * away from it. If it's heading towards the wall we turn it around. If it's already
 * been turned around and is heading away from the wall, we let it continue.
 *
 * All movement is handled by CCActions.
 * 
 * The effect is to see the teapot collide with the wall, bounce off it,
 * and head the other way.
 */
void CC3MashUpScene::checkForCollisions()
{
	// Test whether the rainbow teapot intersects the brick wall.
	if ( _teapotSatellite && _teapotSatellite->doesIntersectNode( _brickWall ) ) 
	{
		// Get the direction from the teapot to the wall.
		CC3Vector tpDir = _brickWall->getGlobalLocation().difference( _teapotSatellite->getGlobalLocation() );		

		// If the teapot velocity is in the same direction as the vector from the
		// teapot to the wall, it is heading towards the wall. If so, turn it around
		// by getting the current spin action on the teapot holder and replacing it
		// with the reverse spin.
		if ( _teapotSatellite->getVelocity().dot( tpDir ) > 0.0f) 
		{
			//LogInfo(@"BANG! %@ hit %@", _teapotSatellite, _brickWall);
			
			// Get the current spinning action.
			CCAction* spinAction = _teapotTextured->getActionByTag( kTeapotRotationActionTag );
			
			// Reverse it and give it a tag so we can find it again.
			CCAction* revSpinAction = ((CCFiniteTimeAction*)spinAction)->reverse();
			revSpinAction->setTag( kTeapotRotationActionTag );

			// Stop the old action and start the new one. 
//			_teapotTextured->stopAction( spinAction );
//			_teapotTextured->runAction( revSpinAction );
		}
	}
}

/** Update the location and direction of looking of the 3D camera */
void CC3MashUpScene::updateCameraFromControls( GLfloat dt )
{
	CC3Camera* cam = getActiveCamera();
	
	// Update the location of the player (the camera)
	if ( _playerLocationControl.x || _playerLocationControl.y ) 
	{
		// Get the X-Y delta value of the control and scale it to something suitable
		CCPoint delta = ccpMult(_playerLocationControl, dt * 100.0);

		// We want to move the camera forward and backward, and side-to-side,
		// from the camera's (the user's) point of view.
		// Forward and backward will be along the globalForwardDirection of the camera,
		// and side-to-side will be along the globalRightDirection of the camera.
		// These two directions are scaled by Y and X delta components respectively, which
		// in turn are set by the joystick, and combined into a single directional vector.
		// This represents the movement of the camera. The new location is simply the old
		// camera location plus the movement.
		CC3Vector moveVector = cam->getGlobalRightDirection().scaleUniform(delta.x).add(
											cam->getGlobalForwardDirection().scaleUniform(delta.y));
		cam->setLocation( cam->getLocation().add( moveVector ) );
	}

	// Update the direction the camera is pointing by panning and inclining using rotation.
	if ( _playerDirectionControl.x || _playerDirectionControl.y ) 
	{
		CCPoint delta = ccpMult(_playerDirectionControl, dt * 30.0);		// Factor to set speed of rotation.
		CC3Vector camRot = cam->getRotation();
		camRot.y -= delta.x;
		camRot.x += delta.y;
		cam->setRotation( camRot );	
	}
}

/**
 * When the user hits the switch-camera-target button, cycle through a series of four
 * different camera targets. The actual movement of the camera to home in on a new target
 * is handled by a CCActionInterval, so that the movement appears smooth and animated.
 */
void CC3MashUpScene::switchCameraTarget() 
{
	if (_camTarget == _origCamTarget)
		_camTarget = _globe;
	else if (_camTarget == _globe)
		_camTarget = _beachBall;
	else if (_camTarget == _beachBall)
		_camTarget = _teapotTextured;
	else if (_camTarget == _teapotTextured)
		_camTarget = _mascot;
	else if (_camTarget == _mascot)
		_camTarget = _floatingHead;
	else if (_camTarget == _floatingHead)
		_camTarget = _dieCube;
	else
		_camTarget = _origCamTarget;
	
	CC3Camera* cam = getActiveCamera();
	cam->setTarget( NULL );			// Ensure the camera is not locked to the original target
	cam->stopAllActions();
	cam->runAction( CC3ActionRotateToLookAt::actionWithDuration( 2.0f, _camTarget->getGlobalLocation() ) );
	//LogInfo(@"Camera target toggled to %@", _camTarget);
}

void CC3MashUpScene::setIsManagingShadows( bool isManaging )
{
	_isManagingShadows = isManaging;
}

bool CC3MashUpScene::isManagingShadows()
{
	return _isManagingShadows;
}

CCPoint CC3MashUpScene::getPlayerDirectionControl()
{
	return _playerDirectionControl;
}

void CC3MashUpScene::setPlayerDirectionControl( const CCPoint& dirControl )
{
	_playerDirectionControl = dirControl;
}

CCPoint CC3MashUpScene::getPlayerLocationControl()
{
	return _playerLocationControl;
}

void CC3MashUpScene::setPlayerLocationControl( const CCPoint& locControl )
{
	_playerLocationControl = locControl;
}

/**
 * Launches an invasion of an army of robots...or removes them if they are here.
 * You can change the robots to teapots by swapping out the last commented line with the one above it.
 */
void CC3MashUpScene::invade() 
{
	LandingCraft* landingCraft = (LandingCraft*)getNodeNamed( kLandingCraftName );
	if (landingCraft) 
	{
		landingCraft->evaporate();
	} 
	else 
	{
		invadeWithRobotArmy();
//		invadeWithTeapotArmy();	// Or if robot armies frighten you, invade with teapots instead.
	}
}

/** Create a landing craft and populate it with an army of robots. */
void CC3MashUpScene::invadeWithRobotArmy() 
{
	// First create a template node by copying the POD resource node.
	// We copy it so we can mofify it.
	// Remove the camera and light that it includes, and since the billboard
	// 2D CCNode can't easily be copied, we'll remove the billboard as well.
	CC3Node* robotTemplate = (CC3Node*)getNodeNamed( kPODRobotRezNodeName )->copy();
	robotTemplate->getNodeNamed( kPODLightName )->remove();
	robotTemplate->getNodeNamed( kPODCameraName )->remove();
	robotTemplate->getNodeNamed( kBillboardName )->remove();

	// In the original robot arm, each component is individually selectable.
	// For the army, we wont bother with this level of detail, and we'll just
	// select the whole assembly (actually the resource node) whenever any part
	// of the robot is touched. This is done by first removing the individual
	// enablement that we set on the original, and then just enabling the top level.
	robotTemplate->touchDisableAll();
	robotTemplate->setTouchEnabled( true );

	// Make these robots smaller to distinguish them from the original
	robotTemplate->setUniformScale( 0.5f );
	
	invadeWithArmyOf( robotTemplate );
}

/** Create a landing craft and populate it with an army of teapots. */
void CC3MashUpScene::invadeWithTeapotArmy() 
{
	// First create a template node by copying the POD resource node.
	CC3Node* teapotTemplate = (CC3Node*)getNodeNamed(kTeapotWhiteName)->copy();
	teapotTemplate->setUniformScale( teapotTemplate->getUniformScale() * 3.0f );

	invadeWithArmyOf( teapotTemplate );
}

/**
 * Invade with multiple copies of the specified template node. Instantiates a landing
 * craft and populates it with an army copied from the specified template node.
 */
void CC3MashUpScene::invadeWithArmyOf( CC3Node* invaderTemplate )
{
	LandingCraft* landingCraft = LandingCraft::nodeWithName( kLandingCraftName );
	landingCraft->populateArmyWith( invaderTemplate );

	// We want to add the landing craft as a child of the ground when it lands.
	// But the ground has been rotated in two dimensions, and if we simply add the landing
	// craft as a child, it will be rotated along with the ground. The result will be that
	// the army will appear to land horizontally and be deployed vertically. However, adding
	// the landing craft to the ground, and locaizing it to the ground, will compensate for
	// the existing transformations that have been applied to the ground. The result will be
	// that the army will appear to land vertically and deploy horizontally, as expected.
	landingCraft->setLocation( _ground->getLocation() );
	_ground->addAndLocalizeChild( landingCraft );
}

void CC3MashUpScene::cycleLights() 
{
	CC3Node* sun = getNodeNamed( kSunName );
	CC3Node* flashLight = getNodeNamed( kSpotlightName );

	// Cycle to the next lighting type, based on current lighting type
	switch (_lightingType) 
	{
		case kLightingSun:
			_lightingType = kLightingFlashlight;
			break;
		case kLightingFlashlight:

			_lightingType = kLightingLightProbe;
			break;
		case kLightingLightProbe:
			_lightingType = kLightingFog;
			break;
		case kLightingFog:
			_lightingType = kLightingGrayscale;
			break;
		case kLightingGrayscale:
			_lightingType = kLightingDepth;
			break;
		case kLightingDepth:
			_lightingType = kLightingSun;
			break;
	}

	// Configure the scene for the new lighting conditions
	switch (_lightingType) 
	{
		case kLightingSun:
			sun->setVisible( true );
			_fog->setVisible( false );
			_robotLamp->setVisible( true );
			flashLight->setVisible( false );
			if ( _bumpMapLightTracker )
				_bumpMapLightTracker->setTarget( _robotLamp );
			_backdrop->setEmissionColor( kSkyColor );
			setShouldUseLightProbes( false );
			break;
		case kLightingFlashlight:
			sun->setVisible( false );
			_fog->setVisible( false );
			_robotLamp->setVisible( false );
			flashLight->setVisible( true );
			if ( _bumpMapLightTracker )
				_bumpMapLightTracker->setTarget( flashLight );
			_backdrop->setEmissionColor( kCCC4FBlack );
			setShouldUseLightProbes( false );
			break;
		case kLightingLightProbe:
			sun->setVisible( false );
			_fog->setVisible( false );
			_robotLamp->setVisible( true );
			flashLight->setVisible( false );
			if ( _bumpMapLightTracker )
				_bumpMapLightTracker->setTarget( _robotLamp );
			_backdrop->setEmissionColor( kSkyColor );
			setShouldUseLightProbes( false );
			break;
		case kLightingFog:
			sun->setVisible( true );
			_fog->setVisible( true );
			_robotLamp->setVisible( true );
			flashLight->setVisible( false );
			if ( _bumpMapLightTracker )
				_bumpMapLightTracker->setTarget( _robotLamp );
			_backdrop->setEmissionColor( kSkyColor );
			setShouldUseLightProbes( false );
			break;
		case kLightingGrayscale:
			sun->setVisible( true );
			_fog->setVisible( false );
			_robotLamp->setVisible( true );
			flashLight->setVisible( false );
			if ( _bumpMapLightTracker )
				_bumpMapLightTracker->setTarget( _robotLamp );
			_backdrop->setEmissionColor( kSkyColor );
			setShouldUseLightProbes( false );
			break;
		case kLightingDepth:
			sun->setVisible( true );
			_fog->setVisible( false );
			_robotLamp->setVisible( true );
			flashLight->setVisible( false );
			if ( _bumpMapLightTracker )
				_bumpMapLightTracker->setTarget( _robotLamp );
			_backdrop->setEmissionColor( kSkyColor );
			setShouldUseLightProbes( false );
			break;
	}
}

/**
 * Cycle between current camera view and two views showing the complete scene.
 * When the full scene is showing, a wireframe is drawn so we can easily see its extent.
 */
void CC3MashUpScene::cycleZoom() 
{
	CC3Camera* cam = getActiveCamera();
	cam->stopAllActions();						// Stop any current camera motion
	switch (_cameraZoomType) 
	{
		// Currently in normal view. Remember orientation of camera, turn on wireframe
		// and move away from the scene along the line between the center of the scene
		// and the camera until everything in the scene is visible.
		case kCameraZoomNone:
			_lastCameraOrientation = CC3RayFromLocDir(cam->getGlobalLocation(), cam->getGlobalForwardDirection());
			setShouldDrawWireframeBox( true );
			cam->moveWithDuration( kCameraMoveDuration, this );
			_cameraZoomType = kCameraZoomStraightBack;	// Mark new state
			break;
		
		// Currently looking at the full scene.
		// Move to view the scene from a different direction.
		case kCameraZoomStraightBack:
			setShouldDrawWireframeBox( true );
			cam->moveWithDuration( kCameraMoveDuration, this, cc3v(-1.0, 1.0, 1.0) );
			_cameraZoomType = kCameraZoomBackTopRight;	// Mark new state
			break;

		// Currently in second full-scene view.
		// Turn off wireframe and move back to the original location and orientation.
		case kCameraZoomBackTopRight:
		default:
			setShouldDrawDescriptor( false );
			setShouldDrawWireframeBox( false );
			cam->runAction( CC3ActionMoveTo::actionWithDuration( kCameraMoveDuration, _lastCameraOrientation.startLocation ) );
			cam->runAction( CC3ActionRotateToLookTowards::actionWithDuration( kCameraMoveDuration, _lastCameraOrientation.direction ) );
			_cameraZoomType = kCameraZoomNone;	// Mark new state
			break;
	}
}

void CC3MashUpScene::startMovingCamera()
{
	_cameraMoveStartLocation = getActiveCamera()->getLocation(); 
}

void CC3MashUpScene::stopMovingCamera()
{

}

/** Set this parameter to adjust the rate of camera movement during a pinch gesture. */
#define kCamPinchMovementUnit		250

void CC3MashUpScene::moveCameraBy( GLfloat aMovement )
{
	CC3Camera* cam = getActiveCamera();

	// Convert to a logarithmic scale, zero is backwards, one is unity, and above one is forward.
	GLfloat camMoveDist = logf(aMovement) * kCamPinchMovementUnit;

	CC3Vector moveVector = cam->getGlobalForwardDirection().scaleUniform(camMoveDist);
	cam->setLocation( _cameraMoveStartLocation.add(moveVector) );
}

void CC3MashUpScene::startPanningCamera()
{
	_cameraPanStartRotation = getActiveCamera()->getRotation(); 
}

void CC3MashUpScene::stopPanningCamera()
{

}

void CC3MashUpScene::panCameraBy( const CCPoint& aMovement )
{
	CC3Vector camRot = _cameraPanStartRotation;
	CCPoint panRot = ccpMult(aMovement, 90);		// Full pan swipe is 90 degrees
	camRot.y += panRot.x;
	camRot.x -= panRot.y;
	getActiveCamera()->setRotation( camRot );
}

void CC3MashUpScene::startDraggingAt( const CCPoint& touchPoint )
{
	pickNodeFromTapAt( touchPoint ); 
}

void CC3MashUpScene::dragBy( const CCPoint& aMovement, const CCPoint& aVelocity )
{
	if (_selectedNode == _dieCube || _selectedNode == _texCubeSpinner)
		rotate( (SpinningNode*)_selectedNode, aVelocity );
}

void CC3MashUpScene::stopDragging()
{
	_selectedNode = NULL; 
}

/** Set this parameter to adjust the rate of rotation from the length of swipe gesture. */
#define kSwipeVelocityScale		400

/**
 * Rotates the specified spinning node by setting its rotation axis
 * and spin speed from the specified 2D drag velocity.
 */
void CC3MashUpScene::rotate( SpinningNode* aNode, const CCPoint& swipeVelocity )
{
	// The 2D rotation axis is perpendicular to the drag velocity.
	CCPoint axis2d = ccpPerp(swipeVelocity);
	
	// Project the 2D rotation axis into a 3D axis by mapping the 2D X & Y screen
	// coords to the camera's rightDirection and upDirection, respectively.
	CC3Camera* cam = getActiveCamera();
	aNode->setSpinAxis( cam->getRightDirection().scaleUniform(axis2d.x).add( cam->getUpDirection().scaleUniform(axis2d.y)) );

	// Set the spin speed from the scaled drag velocity.
	aNode->setSpinSpeed( ccpLength(swipeVelocity) * kSwipeVelocityScale );

	// Mark the spinning node as free-wheeling, so that it will start spinning.
	aNode->setIsFreeWheeling( true );
}

/**
 * Handle touch events in the scene:
 *   - Touch-down events are used to select nodes. Forward these to the touched node picker.
 *   - Touch-move events are used to generate a swipe gesture to rotate the die cube
 *   - Touch-up events are used to mark the die cube as freewheeling if the touch-up event
 *     occurred while the finger is moving.
 * This is a poor UI. We really should be using the touch-stationary event to mark definitively
 * whether the finger stopped before being lifted. But we're just working with what we have handy.
 *
 * If gestures are being used (see the setting of the touchEnabled property in the initializeControls
 * method of CC3DemoMashUpLayer), this method will not be invoked. Instead, the gestures invoke handler
 * methods on the CC3DemoMashUpLayer, which then issues higher-level control messages to this scene.
 *
 * It is generally recommended that you use gestures to provide user interaction with the 3D scene.
 */
void CC3MashUpScene::touchEvent( GLuint touchType, const CCPoint& touchPoint )
{
	struct timeval now;
	gettimeofday(&now, NULL);

	// Time since last event
	GLfloat dt = (now.tv_sec - _lastTouchEventTime.tv_sec) + (now.tv_usec - _lastTouchEventTime.tv_usec) / 1000000.0f;

	switch (touchType) 
	{
		case kCCTouchBegan:
			pickNodeFromTouchEvent( touchType, touchPoint );
			break;
		case kCCTouchMoved:
			{
#if 0
				if (_selectedNode == _dieCube || _selectedNode == _texCubeSpinner) 
				{
					rotate( ((SpinningNode*)_selectedNode), touchPoint, dt );
				}
#endif

				CC3Camera* activeCam = getActiveCamera();
				if ( activeCam )
				{
					float diffX = touchPoint.x - _lastTouchEventPoint.x;
					float diffY = touchPoint.y - _lastTouchEventPoint.y;
				//	activeCam->rotateBy( cc3v( diffY, 0, 0 ) );
				//	activeCam->rotateBy( cc3v(0, -diffX, 0 ) );
				}
			}
			break;
		case kCCTouchEnded:

#if 0
			if (_selectedNode == _dieCube || _selectedNode == _texCubeSpinner) 
			{
				// If the user lifted the finger while in motion, let the cubes know
				// that they can freewheel now. But if the user paused before lifting
				// the finger, consider it stopped.
				((SpinningNode*)_selectedNode)->setIsFreeWheeling( (dt < 0.5) );
			}

#endif
			_selectedNode = NULL;
			break;
		default:
			break;
	}
	
	// For all event types, remember when and where the touchpoint was, for subsequent events.
	_lastTouchEventPoint = touchPoint;
	_lastTouchEventTime = now;
}

/** Set this parameter to adjust the rate of rotation from the length of touch-move swipe. */
#define kSwipeScale 0.6

/**
 * Rotates the specified node, by determining the direction of each touch move event.
 *
 * The touch-move swipe is measured in 2D screen coordinates, which are mapped to
 * 3D coordinates by recognizing that the screen's X-coordinate maps to the camera's
 * rightDirection vector, and the screen's Y-coordinates maps to the camera's upDirection.
 *
 * The node rotates around an axis perpendicular to the swipe. The rotation angle is
 * determined by the length of the touch-move swipe.
 *
 * To allow freewheeling after the finger is lifted, we set the spin speed and spin axis
 * in the node. We indicate for now that the node is not freewheeling.
 */
void CC3MashUpScene::rotate( SpinningNode* aNode, const CCPoint& touchPoint, GLfloat dt )
{	
	CC3Camera* cam = getActiveCamera();

	// Get the direction and length of the movement since the last touch move event, in
	// 2D screen coordinates. The 2D rotation axis is perpendicular to this movement.
	CCPoint swipe2d = ccpSub(touchPoint, _lastTouchEventPoint);
	CCPoint axis2d = ccpPerp(swipe2d);
	
	// Project the 2D axis into a 3D axis by mapping the 2D X & Y screen coords
	// to the camera's rightDirection and upDirection, respectively.
	CC3Vector axis = cam->getRightDirection().scaleUniform(axis2d.x).add( cam->getUpDirection().scaleUniform(axis2d.y) );
	GLfloat angle = ccpLength(swipe2d) * kSwipeScale;

	// Rotate the cube under direct finger control, by directly rotating by the angle
	// and axis determined by the swipe. If the die cube is just to be directly controlled
	// by finger movement, and is not to freewheel, this is all we have to do.
	aNode->rotateByAngle( angle, axis );

	// To allow the cube to freewheel after lifting the finger, have the cube remember
	// the spin axis and spin speed. The spin speed is based on the angle rotated on
	// this event and the interval of time since the last event. Also mark that the
	// die cube is not freewheeling until the finger is lifted.
	aNode->setIsFreeWheeling( false );
	aNode->setSpinAxis( axis );
	aNode->setSpinSpeed( angle / dt );
}

/** 
 * This callback method is automatically invoked when a touchable 3D node is picked
 * by the user. If the touch event indicates that the user has raised the finger,
 * thus completing the touch action.
 *
 * If the UI is in "managing shadows" mode, each touch of an object cycles through
 * various shadowing options for the touched node. If not in "managing shadows" mode,
 * the actions described here occur.
 *
 * Most nodes are simply temporarily highlighted by running a Cocos2D tinting action on
 * the emission color property of the node (which affects the emission color property of
 * the materials underlying the node).
 *
 * Some nodes have other, or additional, behaviour. Nodes with special behaviour include
 * the ground, the die cube, the beach ball, the textured and rainbow teapots, and the wooden sign.
 */
void CC3MashUpScene::nodeSelected( CC3Node* aNode, GLuint touchType, const CCPoint& touchPoint )
{
	// If in "managing shadows" mode, cycle through a variety of shadowing techniques.
	if (_isManagingShadows) 
	{
		cycleShadowFor( aNode );
		return;
	}
	
	// Remember the node that was selected
	_selectedNode = aNode;
	
	// Uncomment to toggle the display of a descriptor label on the node
//	aNode.shouldDrawDescriptor = !aNode.shouldDrawDescriptor;

	// Briefly highlight the location where the node was touched.
	markTouchPoint( touchPoint, aNode );
	
	if (!aNode) return;

	if (aNode == _ground) 
	{
		touchGroundAt( touchPoint );
	}
	else if (aNode == _beachBall) 
	{
		touchBeachBallAt( touchPoint );
	}
	else if (aNode == _brickWall) 
	{
		touchBrickWallAt( touchPoint );
	}
	else if (aNode == _woodenSign) 
	{
		switchWoodenSign();
	}
	else if (aNode == _floatingHead) 
	{
		toggleFloatingHeadConfiguration();
	} 
	else if (aNode == _dieCube || aNode == _texCubeSpinner) 
	{
		// These are spun by touch movement. Do nothing...and don't highlight
	}
	else if (aNode == getNodeNamed(kRunnerName)) 
	{
		toggleActiveCamera();
	}
	else if (aNode == getNodeNamed(kLittleBrotherName) ) 
	{
		toggleActiveCamera();
	}
	else if (aNode == getNodeNamed(kBitmapLabelName)) 
	{
		cycleLabelOf( (CC3BitmapLabelNode*)aNode );
	} 
	else if (aNode == getNodeNamed( "Particles" )) 
	{
		((CC3ParticleEmitter*)aNode)->emitParticle();
	}
	else if (aNode == getNodeNamed(kTVName)) 
	{
		toggleTelevision();
	}
	else if (aNode == _teapotTextured || aNode == _teapotSatellite) 
	{
		
		// Toggle wireframe box around the touched teapot's mesh
		CC3LocalContentNode* lcNode = (CC3LocalContentNode*)aNode;
		lcNode->setShouldDrawLocalContentWireframeBox( !lcNode->shouldDrawLocalContentWireframeBox() );
		
		// Toggle the large wireframe box around both teapots
		_teapotTextured->setShouldDrawWireframeBox( !_teapotTextured->shouldDrawWireframeBox() );

	// If the robot was touched, cycle through three particle hose options.
	// If no particles are being emitting, turn on the point particle hose.
	// If the point particle hose is emitting, turn it off and turn on the mesh particle hose.
	// If the mesh particle hose is emitting, turn it off so neither hose is emitting.
	} 
	else if (aNode == getNodeNamed( kPODRobotRezNodeName ) ) 
	{
		CC3ParticleEmitter* pointHose = (CC3ParticleEmitter*)getNodeNamed( kPointHoseEmitterName );
		CC3ParticleEmitter* meshHose = (CC3ParticleEmitter*)getNodeNamed( kMeshHoseEmitterName );
		if (pointHose->isEmitting()) 
		{
			pointHose->pause();
			meshHose->play();
		}
		else if (meshHose->isEmitting()) 
		{
			meshHose->pause();
		}
		else 
		{
			pointHose->play();
		}
		
	// If the dragon was touched, cycle through several different animations for the dragon
	} 
	else if (aNode == _dragon ) 
	{
		cycleDragonMotion();

	// If the globe was touched, toggle the opening of a HUD window displaying it up close.
	} 
	else if (aNode == _globe ) 
	{
		CC3MashUpLayer* layer = getPrimaryCC3MashUpLayer();		// Create strong reference to weak property
		layer->toggleGlobeHUDFromTouchAt(touchPoint);
	}
}

/**
 * Unproject the 2D touch point into a 3D global-coordinate ray running from the camera through
 * the touched node. Find the node that is punctured by the ray, the location at which the ray
 * punctures the node's bounding volume in the local coordinates of the node, and add a temporary
 * visible marker at that local location that fades in and out, and then removes itself.
 */
void CC3MashUpScene::markTouchPoint( const CCPoint& touchPoint, CC3Node* aNode )
{
	if ( !aNode )
	{
		CC3_TRACE( "You selected no node." );
		return;
	}

	// Get the location where the node was touched, in its local coordinates.
	// Normally, in this case, you would invoke nodesIntersectedByGlobalRay:
	// on the touched node, not on this CC3Scene. We do so here, to show that
	// all of the nodes under the ray will be detected, not just the touched node.
	CC3Ray touchRay = getActiveCamera()->unprojectPoint( touchPoint );
	CC3NodePuncturingVisitor* puncturedNodes = nodesIntersectedByGlobalRay( touchRay );
	
	// The reported touched node may be a parent. We want to find the descendant node that
	// was actually pierced by the touch ray, so that we can attached a descriptor to it.
	CC3Node* localNode = puncturedNodes->getClosestPuncturedNode();
	CC3Vector nodeTouchLoc = puncturedNodes->getClosestPunctureLocation();

	// Create a descriptor node to display the location on the node
	std::string touchLocStr = CC3String::stringWithFormat( (char*)"(%.1f, %.1f, %.1f)", nodeTouchLoc.x, nodeTouchLoc.y, nodeTouchLoc.z );
	CCLabelTTF* dnLabel = CCLabelTTF::create( touchLocStr.c_str(), "Arial", 8 );
	CC3Node* dn = CC3NodeDescriptor::nodeWithName( CC3String::stringWithFormat( (char*)"%s-TP", localNode->getName().c_str() ), dnLabel );
	dn->setColor( localNode->getInitialDescriptorColor() );

	// Use actions to fade the descriptor node in and then out, and remove it when done.
	CCActionInterval* fadeIn = CCActionFadeIn::create( 0.2f );
	CCActionInterval* fadeOut = CCActionFadeOut::create( 5.0f );
	CCActionInstant* remove = CC3ActionRemove::create(); 
	dn->setOpacity( 0 );		// Start invisible
//	dn->runAction( CCActionSequence::create( fadeIn, fadeOut, remove, NULL ) );
	
	// Set the location of the descriptor node to the touch location,
	// which are in the touched node's local coordinates, and add the
	// descriptor node to the touched node.
	dn->setLocation( nodeTouchLoc );
	localNode->addChild( dn );

#if 0
	// Log everything that happened.
	NSMutableString* desc = [NSMutableString stringWithCapacity: 500];
	[desc appendFormat: @"You selected %@", aNode];
	[desc appendFormat: @" located at %@", NSStringFromCC3Vector(aNode.globalLocation)];
	[desc appendFormat: @", or at %@ in 2D.", NSStringFromCC3Vector([self.activeCamera projectNode: aNode])];
	[desc appendFormat: @"\nThe actual node touched was %@", localNode];
	[desc appendFormat: @" at %@ on its boundary", NSStringFromCC3Vector(nodeTouchLoc)];
	[desc appendFormat: @" (%@ globally).", NSStringFromCC3Vector(puncturedNodes.closestGlobalPunctureLocation)];
	[desc appendFormat: @"\nThe nodes punctured by the ray %@ were:", NSStringFromCC3Ray(touchRay)];
	NSUInteger puncturedNodeCount = puncturedNodes.nodeCount;
	for (NSUInteger i = 0; i < puncturedNodeCount; i++) {
		[desc appendFormat: @"\n\t%@", [puncturedNodes puncturedNodeAt: i]];
		[desc appendFormat: @" at %@ on its boundary.", NSStringFromCC3Vector([puncturedNodes punctureLocationAt: i])];
		[desc appendFormat: @" (%@ globally).", NSStringFromCC3Vector([puncturedNodes globalPunctureLocationAt: i])];
	}
	LogInfo(@"%@", desc);
#endif
}

/**
 * If the touched node is the ground, place a little orange teapot at the location
 * on the ground corresponding to the touch-point. As the teapot is placed, we set off
 * a fiery explosion using a 2D particle system for dramatic effect. This demonstrates
 * the ability to drop objects into the 3D scene using touch events, along with the
 * ability to add Cocos2D CCParticleSystems into the 3D scene.
 */
void CC3MashUpScene::touchGroundAt( const CCPoint& touchPoint )
{
	CC3Plane groundPlane = _ground->getPlane();
	CC3Vector4 touchLoc = getActiveCamera()->unprojectPoint( touchPoint, groundPlane );

	// Make sure the projected touch is in front of the camera, not behind it
	// (ie- cam is facing towards, not away from, the ground)
	if (touchLoc.w > 0.0f) 
	{
		CC3MeshNode* tp = CC3ModelSampleFactory::factory()->makeUniColoredTeapotNamed( kTeapotOrangeName, kCCC4FOrange );
		tp->setUniformScale( 200.0f );
		tp->setLocation( touchLoc.cc3Vector() );
		
		addExplosionTo( tp );	// For effect, add an explosion as the teapot is placed
		
		// We've set the teapot location to the global 3D point that was derived from the
		// touch point, and the teapot has a global rotation of zero, and a global scale.
		// When we add it to the ground plane, we don't want those properties to be further
		// transformed by the ground plane's existing transform. Therefore, the teapot
		// transform properties must be localized to properties that are relative to those
		// of the ground plane. We can do that using the addAndLocalizeChild: method.
		_ground->addAndLocalizeChild( tp );
	}
}

/** If the node is the beach ball, toggle it between opaque and translucent. */
void CC3MashUpScene::touchBeachBallAt( const CCPoint& touchPoint )
{
	// Because the beach ball is a composite node, and we add a fading touch location
	// descriptor node as a child to it when the beach ball is touched we can't trust
	// the value of the opacity property of the beach ball parent node. Instead, we
	// need to dig into one of its mesh node segments to determine its opacity.
	CCOpacity bbOpacity = _beachBall->getNodeNamed(kBeachBallWhiteSegment)->getOpacity();
	_beachBall->setOpacity( (bbOpacity == kCCOpacityFull) ? (kCCOpacityFull * 0.75) : kCCOpacityFull );

	// For fun, uncomment the following line to draw wireframe boxes around the beachball component meshes
//	_beachBall.shouldDrawAllLocalContentWireframeBoxes = !_beachBall.shouldDrawAllLocalContentWireframeBoxes;
}

/** When the brick wall is touched, slide it back and forth to open or close it. */
void CC3MashUpScene::touchBrickWallAt( const CCPoint& touchPoint )
{
	CC3Vector destination = _brickWall->isOpen() ? kBrickWallClosedLocation : kBrickWallOpenLocation;
	CCActionInterval* moveAction = CC3ActionMoveTo::actionWithDuration( 3.0f, destination );
	// Add a little bounce for realism.
	moveAction = CCActionEaseElasticOut::create( moveAction, 0.5f );
	_brickWall->stopAllActions();
//	_brickWall->runAction( moveAction );
//	_brickWall->setIsOpen( !_brickWall->isOpen() );
}

/**
 * Switch the multi-texture displayed on the wooden sign node to the next texture combination
 * function in the cycle. There are two basic examples of texture combining demonstrated here.
 * The first is a series of methods of combining regular RGB textures. The second is DOT3
 * bump-mapping which uses the main texture as a normal map to interact with the lighting,
 * and then overlaying the wooden sign texture onto it. The effect of this last type of
 * combining is to add perceived embossing to the wooden texture.
 *
 * Once the multi-texture combining function is determined, the name of it is set in
 * the label that hovers above the wooden sign.
 *
 * This functionality is available only under OpenGL ES 1.1.
 */
void CC3MashUpScene::switchWoodenSign()
{

}

/** 
 * Toggle the floating head between a detailed bump-mapped texture and a mesh-only texture,
 * illustrating the use of bump-mapping to increase the visbible surface detail on a very
 * low-poly mesh.
 */
void CC3MashUpScene::toggleFloatingHeadConfiguration()
{
	// Clear all textures then, if moving to bump-mapping, add the bump-map texture first,
	// then add the visible texture. Otherwise, skip the bump-map texture and just add the
	// visible texture.
	bool shouldBumpMap = (_floatingHead->getTexture() != _headBumpTex);
	_floatingHead->removeAllTextures();
	if (shouldBumpMap) 
		_floatingHead->addTexture( _headBumpTex );
	_floatingHead->addTexture( _headTex );

	// If running shaders under OpenGL ES 2.0, clear the shader program so that a different
	// shader program will automatically be selected for the new texture configuration.
	_floatingHead->removeLocalShaders();
}

/**
 * Turn the TV on or off by toggle the image on the TV between a static test pattern card and a
 * texture generated dynamically by rendering the scene from the runner's camera into a texture.
 *
 * To demonstrate extracting an iOS or OSX image from a render surface, every time we turn the
 * TV off, we create a CGImageRef from the TV screen image, and save it to a JPEG file.
 */
void CC3MashUpScene::toggleTelevision()
{
	_isTVOn = !_isTVOn;
	_tvScreen->setTexture( _isTVOn ? getTVSurface()->getColorTexture() : _tvTestCardTex );
	
	if (!_isTVOn)
		saveTVImage();
}

/** 
 * Extracts the image from the TV screen, converts it into an OS image,
 * and saves it to a JPEG file in the Documents directory.
 */
void CC3MashUpScene::saveTVImage()
{
#if 0
	NSString* docDir = [NSHomeDirectory() stringByAppendingPathComponent: @"Documents"];
	NSString* imgPath = [docDir stringByAppendingPathComponent: @"TV.jpg"];

	// Extract a CGImageRef from either the entire TV surface, or just a section (by uncommenting below)
	CGImageRef tvImgRef = self.tvSurface.createCGImage;
//	CGImageRef tvImgRef = [_tvSurface createCGImageFrom: CC3ViewportMake(230, 100, 256, 256)];
	
#if CC3_IOS
	// Convert the CGImageRef to a UIImage and save it as a JPEG file.
	UIImage* tvImg	= [UIImage imageWithCGImage: tvImgRef];
	NSData* imgData = UIImageJPEGRepresentation(tvImg, 0.9f);
	[imgData writeToFile: imgPath atomically: YES];
#endif	// CC3_IOS

#if CC3_OSX
	// Create an image destination and save the CGImageRef to it.
	CFURLRef url = (__bridge CFURLRef)[NSURL fileURLWithPath:imgPath];
	CGImageDestinationRef dest = CGImageDestinationCreateWithURL(url, kUTTypeJPEG, 1, NULL);
	CGImageDestinationAddImage(dest, tvImgRef, nil);
	CGImageDestinationFinalize(dest);
	CFRelease(dest);
#endif	// CC3_OSX
	
	// Don't forget to release the CGImageRef!
	CGImageRelease(tvImgRef);
	LogInfo(@"TV image saved to file: %@", imgPath);
#endif
}

/** Toggle between the main scene camera and the camera running along with the runner. */
void CC3MashUpScene::toggleActiveCamera()
{
	setActiveCamera( getActiveCamera() == _robotCam ? _runnerCam : _robotCam );
}

/** Cycles the specified bitmapped label node through a selection of label strings. */
void CC3MashUpScene::cycleLabelOf( CC3BitmapLabelNode* bmLabel )
{
	switch (_bmLabelMessageIndex) 
	{
		case 0:
			bmLabel->setLabelString( "Goodbye,\ncruel world." );
			bmLabel->setColor( CCColorRefFromCCC4F(kCCC4FRed) );
			_bmLabelMessageIndex++;
			break;
		default:
			bmLabel->setLabelString( "Hello, world!" );
			bmLabel->setColor( CCColorRefFromCCC4F(ccc4f(0.f, 0.85f, 0.5f, 1.0f)) );
			_bmLabelMessageIndex = 0;
			break;
	}
}

/** Cycles through a variety of shadowing options for the specified node. */
void CC3MashUpScene::cycleShadowFor( CC3Node* aNode )
{	
	// Don't add a shadow to the ground
	if (aNode == _ground) 
		return;

	// If the node already has a shadow volume, remove it, otherwise add one.
	if ( aNode->hasShadowVolumesForLight( _robotLamp ) ) 
	{
		aNode->removeShadowVolumesForLight( _robotLamp );
		CC3_TRACE("Removed shadow from: %s", aNode->getName().c_str());
	} 
	else 
	{
		// Normally, shadow volumes interact with the stencil buffer, and are not visible
		// themselves. You can change the following property to YES, to make the shadow
		// volumes themselves during development.
		CC3ShadowVolumeMeshNode::setDefaultVisible( false );
		
		// Since we're already running, spawn a background threaded task to create and
		// populate the shadow volume, in order to reduce any unwanted animation pauses.
		/*[CC3Backgrounder.sharedBackgrounder runBlock: ^{
			[aNode addShadowVolumesForLight: _robotLamp];
			LogInfo(@"Added shadow to: %@", aNode);
		}];*/
		aNode->addShadowVolumesForLight( _robotLamp );
	}
}

/**
 * When the dragon node is touched, cycle through several different animations, smoothly
 * transitioning between the current and new animations by using animation cross-fading
 * which blends the two animations together using animated blending weights.
 */
void CC3MashUpScene::cycleDragonMotion()
{
	switch (_dragonMotion) 
	{
		case kDragonFlapping:
			dragonTransitionToGliding();
			break;
		
		case kDragonStill:
		case kDragonGliding:
		default:
			dragonTransitionToFlapping();
			break;
	}
}

/**
 * Smoothly transitions the dragon from flapping animation to gliding animation.
 *
 * Flapping and gliding are handled by two separate tracks of animation, and the transition
 * is done by animating the relative blending weights beween the two tracks by using an
 * animation cross-fade action. Animated cross-fading ensures a smooth transition between
 * the distinct animations, regardless of where in each animation cycle it is started.
 *
 * In addition, once the cross-fading transition has finished, the old animation will no longer
 * be visibly affecting the dragon, and can be shut down to save unnecessary processing. We do
 * this after the transition has complted to avoid an abrupt transtion that would occur if the
 * old animation track was stopped while it was still visible.
 */
void CC3MashUpScene::dragonTransitionToGliding()
{
	// Create the gliding animation action and start it running on the dragon
	CC3Action* gliding = CC3RepeatForever::create( CC3ActionAnimate::actionWithDuration( 2.0f, _dragonFlapTrack ) );
	gliding->setTag( kGlidingActionTag );
	_dragon->runAction( gliding );
	
	// The dragon is currently running the flapping animation at full weight and the gliding animation
	// at zero weight. Cross-fade from flapping to gliding over a short time period, then shut
	// down the flapping animation, so we're not wasting time animating it when it's not visible.
	CCActionInterval* crossFade = CC3ActionAnimationCrossFade::actionWithDuration( 0.5f, _dragonGlideTrack, _dragonFlapTrack );
	CCActionCallFunc* stopFlapping = CCActionCallFunc::create( this, SEL_CallFunc(&CC3MashUpScene::dragonTransitionToFlapping) );
//	_dragon->runAction( CCActionSequence::createWithTwoActions( crossFade, stopFlapping ) );
	
	_dragonMotion = kDragonGliding;		// Dragon is gliding now
}

/** 
 * Smoothly transitions the dragon from gliding animation to flapping animation.
 *
 * Flapping and gliding are handled by two separate tracks of animation, and the transition
 * is done by animating the relative blending weights beween the two tracks by using an
 * animation cross-fade action. Animated cross-fading ensures a smooth transition between
 * the distinct animations, regardless of where in each animation cycle it is started.
 *
 * In addition, once the cross-fading transition has finished, the old animation will no longer
 * be visibly affecting the dragon, and can be shut down to save unnecessary processing. We do 
 * this after the transition has complted to avoid an abrupt transtion that would occur if the
 * old animation track was stopped while it was still visible.
 */
void CC3MashUpScene::dragonTransitionToFlapping()
{
	// Create the flapping animation action and start it running on the dragon
	CC3Action* flapping = CC3RepeatForever::create( CC3ActionAnimate::actionWithDuration( 2.0f, _dragonFlapTrack ) );
	flapping->setTag( kFlappingActionTag );
	_dragon->runAction( flapping );
	
	// The dragon is currently running the gliding animation at full weight and the flapping animation
	// at zero weight. Cross-fade from gliding to flapping over a short time period, then shut
	// down the gliding animation, so we're not wasting time animating it when it's not visible.
	CCActionInterval* crossFade = CC3ActionAnimationCrossFade::actionWithDuration( 0.5f, _dragonGlideTrack, _dragonFlapTrack );
	CCActionCallFunc* stopGliding = CCActionCallFunc::create( this, SEL_CallFunc(&CC3MashUpScene::dragonStopGliding) );
//	_dragon->runAction( CCActionSequence::createWithTwoActions( crossFade, stopGliding ) );
	
	_dragonMotion = kDragonFlapping;		// Dragon is flapping now
}

/** Stop the CC3ActionAnimate action that is running the dragon's flapping animation. */
void CC3MashUpScene::dragonStopFlapping() 
{
	_dragon->stopActionByTag( kFlappingActionTag ); 
}

/** Stop the CC3ActionAnimate action that is running the dragon's gliding animation. */
void CC3MashUpScene::dragonStopGliding()
{
	_dragon->stopActionByTag( kGlidingActionTag ); 
}
