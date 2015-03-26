#include "CC3HelloWorldScene.h"
#include "cocos2d.h"
#include "platform/CCPlatformMacros.h"
#include "Models.h"

USING_NS_CC;

#define kPointHoseEmitterName			"PointHose"
#define kRobotTopArm					"TopArm"


/**
 * When used as an ending size or component of an ending color for a
 * CC3VariegatedPointParticleHoseEmitter, indicates that that component
 * should stay constant at the value at which it started.
 */
#define kCC3ParticleConstantComponent	-1.0


/**
 * Constructs the 3D scene prior to the scene being displayed.
 *
 * Adds 3D objects to the scene, loading a 3D 'hello, world' message
 * from a POD file, and creating the camera and light programatically.
 *
 * When adapting this template to your application, remove all of the content
 * of this method, and add your own to construct your 3D model scene.
 *
 * You can also load scene content asynchronously while the scene is being displayed by
 * loading on a background thread. You can add code that loads content to the 
 * addSceneContentAsynchronously method, and it will automatically be loaded immediately
 * after the scene is opened, and smoothly inserted into the scene, as the existing scene
 * content is being displayed.
 *
 * NOTES:
 *
 * 1) To help you find your scene content once it is loaded, the onOpen method below contains
 *    code to automatically move the camera so that it frames the scene. You can remove that
 *    code once you know where you want to place your camera.
 *
 * 2) The POD file used for the 'hello, world' message model is fairly large, because converting a
 *    font to a mesh results in a LOT of triangles. When adapting this template project for your own
 *    application, REMOVE the POD file 'hello-world.pod' from the Resources folder of your project.
 */
void CC3HelloWorldScene::initializeScene()
{
	//CC3Texture::setShouldGenerateMipmaps( false );
	m_pTargetNode = NULL;

	_lastTouchEventPoint = CCPointZero;

	// Optionally add a static textured, or solid-color, backdrop, by uncommenting one of these lines
	setBackdrop( CC3Backdrop::nodeWithTexture( CC3Texture::textureFromFile("3d/BrushedSteel.png") ) );
	//setBackdrop( CC3Backdrop::nodeWithColor(ccc4f(0.4, 0.5, 0.9, 1.0)) );

//	addSkyBox();

	// Create the camera, place it back a bit, and add it to the scene
	CC3Camera* cam = CC3Camera::nodeWithName( "Camera" );
	cam->setLocation( cc3v( 0.0, 0.0, 50.0 ) );
	cam->setFarClippingDistance( 4000 );
	cam->setFieldOfView( 60.f );

	addChild( cam ); 

	// Create a light, place it back and to the left at a specific
	// position (not just directional lighting), and add it to the scene
	CC3Light* lamp = CC3Light::nodeWithName( "Lamp" );
	lamp->setLocation( cc3v( -2.0, 0.0, 0.0 ) );
	lamp->setIsDirectionalOnly( false );
	cam->addChild( lamp );

	/*lamp = CC3Light::nodeWithName( "Lamp1" );
	lamp->setLocation( cc3v( 2.0, -2.0, 0.0 ) );
	lamp->setIsDirectionalOnly( false );
	cam->addChild( lamp );*/
	
	// Create and load a POD resource file and add its entire contents to the scene.
	// If needed, prior to adding the loaded content to the scene, you can customize the
	// nodes in the resource, remove unwanted nodes from the resource (eg- extra cameras),
	// or extract only specific nodes from the resource to add them directly to the scene,
	// instead of adding the entire contents.
	
	/// Displaye Model
	CC3ResourceNode* rezNode = CC3PODResourceNode::nodeFromFile( "3d/gtr_run.pod" );
	addChild( rezNode );
	rezNode->setLocation( cc3v( -20, 0, 0 ) );
	rezNode->setUniformScale( 5 );
	rezNode->runAction( CC3RepeatForever::create( CC3ActionAnimate::actionWithDuration(2.f) ) );

	rezNode->setRotation( cc3v(90, 0, 0) );

//	addFloatingRing();
//	addGround();
//	addBrickWall();
//	addAxesDirectionMarkers();
//	addSkinnedRunner();
	addRobot();
	
//	addPointHose();

	addProjectedLabel();

	addBitmapLabel();

	CC3Node* pNode = getNodeNamed( "Floater" );

	/// Add a bitmap label
//	m_pTargetNode = pNode;
	        
	
	// Or, if you don't need to modify the resource node at all before adding its content,
	// you can simply use the following as a shortcut, instead of the previous lines.
//	[self addContentFromPODFile: @"hello-world.pod"];
	
	// In some cases, PODs are created with opacity turned off by mistake. To avoid the possible
	// surprise of an empty scene, the following line ensures that all nodes loaded so far will
	// be visible. However, it also removes any translucency or transparency from the nodes, which
	// may not be what you want. If your model contains transparency or translucency, remove this line.
	setOpacity( kCCOpacityFull );
	
	// Select the appropriate shaders for each mesh node in this scene now. If this step is
	// omitted, a shaders will be selected for each mesh node the first time that mesh node is
	// drawn. Doing it now adds some additional time up front, but avoids potential pauses as
	// the shaders are loaded, compiled, and linked, the first time it is needed during drawing.
	// This is not so important for content loaded in this initializeScene method, but it is
	// very important for content loaded in the addSceneContentAsynchronously method.
	// Shader selection is driven by the characteristics of each mesh node and its material,
	// including the number of textures, whether alpha testing is used, etc. To have the
	// correct shaders selected, it is important that you finish configuring the mesh nodes
	// prior to invoking this method. If you change any of these characteristics that affect
	// the shader selection, you can invoke the removeShaders method to cause different shaders
	// to be selected, based on the new mesh node and material characteristics.
	selectShaders();

	// With complex scenes, the drawing of objects that are not within view of the camera will
	// consume GPU resources unnecessarily, and potentially degrading app performance. We can
	// avoid drawing objects that are not within view of the camera by assigning a bounding
	// volume to each mesh node. Once assigned, the bounding volume is automatically checked
	// to see if it intersects the camera's frustum before the mesh node is drawn. If the node's
	// bounding volume intersects the camera frustum, the node will be drawn. If the bounding
	// volume does not intersect the camera's frustum, the node will not be visible to the camera,
	// and the node will not be drawn. Bounding volumes can also be used for collision detection
	// between nodes. You can create bounding volumes automatically for most rigid (non-skinned)
	// objects by using the createBoundingVolumes on a node. This will create bounding volumes
	// for all decendant rigid mesh nodes of that node. Invoking the method on your scene will
	// create bounding volumes for all rigid mesh nodes in the scene. Bounding volumes are not
	// automatically created for skinned meshes that modify vertices using bones. Because the
	// vertices can be moved arbitrarily by the bones, you must create and assign bounding
	// volumes to skinned mesh nodes yourself, by determining the extent of the bounding
	// volume you need, and creating a bounding volume that matches it. Finally, checking
	// bounding volumes involves a small computation cost. For objects that you know will be
	// in front of the camera at all times, you can skip creating a bounding volume for that
	// node, letting it be drawn on each frame. Since the automatic creation of bounding
	// volumes depends on having the vertex location content in memory, be sure to invoke
	// this method before invoking the releaseRedundantContent method.
	createBoundingVolumes();
	
	// Create OpenGL buffers for the vertex arrays to keep things fast and efficient, and to
	// save memory, release the vertex content in main memory because it is now redundant.
	createGLBuffers();
	releaseRedundantContent();

	// ------------------------------------------

	// That's it! The scene is now constructed and is good to go.
	
	// To help you find your scene content once it is loaded, the onOpen method below contains
	// code to automatically move the camera so that it frames the scene. You can remove that
	// code once you know where you want to place your camera.
	
	// If you encounter problems displaying your models, you can uncomment one or more of the
	// following lines to help you troubleshoot. You can also use these features on a single node,
	// or a structure of nodes. See the CC3Node notes for more explanation of these properties.
	// Also, the onOpen method below contains additional troubleshooting code you can comment
	// out to move the camera so that it will display the entire scene automatically.
	
	// Displays short descriptive text for each node (including class, node name & tag).
	// The text is displayed centered on the pivot point (origin) of the node.
//	self.shouldDrawAllDescriptors = YES;
	
	// Displays bounding boxes around those nodes with local content (eg- meshes).
//	self.shouldDrawAllLocalContentWireframeBoxes = YES;
	
	// Displays bounding boxes around all nodes. The bounding box for each node
	// will encompass its child nodes.
//	self.shouldDrawAllWireframeBoxes = YES;
	
	// If you encounter issues creating and adding nodes, or loading models from
	// files, the following line is used to log the full structure of the scene.
//	LogInfo(@"The structure of this scene is: %@", [self structureDescription]);
	
	// ------------------------------------------

	// And to add some dynamism, we'll animate the 'hello, world' message
	// using a couple of actions...
	
	// Fetch the 'hello, world' object that was loaded from the POD file and start it rotating
	CC3MeshNode* helloTxt = (CC3MeshNode*)getNodeNamed( "Hello" );
	//helloTxt->runAction( CC3ActionRotateForever::actionWithRotationRate( cc3v(0, 30, 0) ) );

	m_pTargetNode = helloTxt;
	//m_pTargetNode->setUniformScale( 2.f );
	//m_pTargetNode->setLocation( cc3v( -10, 0, 0 ) );
	
	// To make things a bit more appealing, set up a repeating up/down cycle to
	// change the color of the text from the original red to blue, and back again.
	/*GLfloat tintTime = 8.0f;
	CCColorRef startColor = helloTxt->getColor();
	CCColorRef endColor = CCColorRefFromCCC4F(ccc4f(0.2, 0.0, 0.8, 1.0));
	CCActionInterval* tintDown = CCActionTintTo::create( tintTime, endColor.r, endColor.g, endColor.b );
	CCActionInterval* tintUp   = CCActionTintTo::create( tintTime, startColor.r, startColor.g, startColor.b );
	helloTxt->runAction( CCActionRepeatForever::create( CCActionSequence::createWithTwoActions( tintDown, tintUp ) ) );*/

	// And let's make this interactive, by allowing the hello text to be touched.
	// When the node is touched, it will be passed to the nodeSelected:byTouchEvent:at: method below.
	//helloTxt->setTouchEnabled( true );
}

/**
 * By populating this method, you can add add additional scene content dynamically and
 * asynchronously after the scene is open.
 *
 * This method is invoked from a code block defined in the onOpen method, that is run on a
 * background thread by the CC3Backgrounder available through the backgrounder property.
 * It adds content dynamically and asynchronously while rendering is running on the main
 * rendering thread.
 *
 * You can add content on the background thread at any time while your scene is running, by
 * defining a code block and running it on the backgrounder. The example provided in the
 * onOpen method is a template for how to do this, but it does not need to be invoked only
 * from the onOpen method.
 *
 * Certain assets, notably shader programs, will cause short, but unavoidable, delays in the
 * rendering of the scene, because certain finalization steps from shader compilation occur on
 * the main thread when the shader is first used. Shaders and certain other critical assets can
 * be pre-loaded and cached in the initializeScene method, prior to the opening of this scene.
 */
void CC3HelloWorldScene::addSceneContentAsynchronously()
{

}

/**
 * This template method is invoked periodically whenever the 3D nodes are to be updated.
 *
 * This method provides your app with an opportunity to perform update activities before
 * any changes are applied to the transformMatrix of the 3D nodes in the scene.
 *
 * For more info, read the notes of this method on CC3Node.
 */
void CC3HelloWorldScene::updateBeforeTransform( CC3NodeUpdatingVisitor* visitor )
{

}

/**
 * This template method is invoked periodically whenever the 3D nodes are to be updated.
 *
 * This method provides your app with an opportunity to perform update activities after
 * the transformMatrix of the 3D nodes in the scen have been recalculated.
 *
 * For more info, read the notes of this method on CC3Node.
 */
void CC3HelloWorldScene::updateAfterTransform( CC3NodeUpdatingVisitor* visitor )
{
	if ( m_pTargetNode )  
		m_pTargetNode->rotateBy( cc3v(0, 0.5f, 0)  );
}

/**
 * Callback template method that is invoked automatically when the CC3Layer that
 * holds this scene is first displayed.
 *
 * This method is a good place to invoke one of CC3Camera moveToShowAllOf:... family
 * of methods, used to cause the camera to automatically focus on and frame a particular
 * node, or the entire scene.
 *
 * For more info, read the notes of this method on CC3Scene.
 */
void CC3HelloWorldScene::onOpen()
{
	// Add additional scene content dynamically and asynchronously, on a background thread
	// after rendering has begun on the rendering thread, using the CC3Backgrounder singleton.
	// Asynchronous loading must be initiated after the scene has been attached to the view.
	// It cannot be started in the initializeScene method. However, it does not need to be
	// invoked only from the onOpen method. You can use the code in the line here as a template
	// for use whenever your app requires background content loading after the scene has opened.
//	[CC3Backgrounder.sharedBackgrounder runBlock: ^{ [self addSceneContentAsynchronously]; }];

	// Move the camera to frame the scene. The resulting configuration of the camera is output as
	// an [info] log message, so you know where the camera needs to be in order to view your scene.
//	[self.activeCamera moveWithDuration: 3.0 toShowAllOf: self withPadding: 0.5f];

	// Uncomment this line to draw the bounding box of the scene.
//	self.shouldDrawWireframeBox = YES;
}

/**
 * Callback template method that is invoked automatically when the CC3Layer that
 * holds this scene has been removed from display.
 *
 * For more info, read the notes of this method on CC3Scene.
 */
void CC3HelloWorldScene::onClose()
{

}


/**
 * Template method that draws the content of the scene.
 *
 * This method is invoked automatically by the drawScene method, once the 3D environment has
 * been established. Once this method is complete, the 2D rendering environment will be
 * re-established automatically, and any 2D billboard overlays will be rendered. This method
 * does not need to take care of any of this set-up and tear-down.
 *
 * This implementation simply invokes the default parent behaviour, which turns on the lighting
 * contained within the scene, and performs a single rendering pass of the nodes in the scene 
 * by invoking the visit: method on the specified visitor, with this scene as the argument.
 * Review the source code of the CC3Scene drawSceneContentWithVisitor: to understand the
 * implementation details, and as a starting point for customization.
 *
 * You can override this method to customize the scene rendering flow, such as performing
 * multiple rendering passes on different surfaces, or adding post-processing effects, using
 * the template methods mentioned above.
 *
 * Rendering output is directed to the render surface held in the renderSurface property of
 * the visitor. By default, that is set to the render surface held in the viewSurface property
 * of this scene. If you override this method, you can set the renderSurface property of the
 * visitor to another surface, and then invoke this superclass implementation, to render this
 * scene to a texture for later processing.
 *
 * When overriding the drawSceneContentWithVisitor: method with your own specialized rendering,
 * steps, be careful to avoid recursive loops when rendering to textures and environment maps.
 * For example, you might typically override drawSceneContentWithVisitor: to include steps to
 * render environment maps for reflections, etc. In that case, you should also override the
 * drawSceneContentForEnvironmentMapWithVisitor: to render the scene without those additional
 * steps, to avoid the inadvertenly invoking an infinite recursive rendering of a scene to a
 * texture while the scene is already being rendered to that texture.
 *
 * To maintain performance, by default, the depth buffer of the surface is not specifically
 * cleared when 3D drawing begins. If this scene is drawing to a surface that already has
 * depth information rendered, you can override this method and clear the depth buffer before
 * continuing with 3D drawing, by invoking clearDepthContent on the renderSurface of the visitor,
 * and then invoking this superclass implementation, or continuing with your own drawing logic.
 *
 * Examples of when the depth buffer should be cleared are when this scene is being drawn
 * on top of other 3D content (as in a sub-window), or when any 2D content that is rendered
 * behind the scene makes use of depth drawing. See also the closeDepthTestWithVisitor:
 * method for more info about managing the depth buffer.
 */
void CC3HelloWorldScene::drawSceneContentWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	super::drawSceneContentWithVisitor( visitor );
}

/**
 * This method is invoked from the CC3Layer whenever a touch event occurs, if that layer
 * has indicated that it is interested in user interaction.
 *
 * This default implementation simply delegates to the superclass behaviour, which selects a 3D node
 * on each touch-down event. You can modify this method to perform more sophisticated touch handling.
 *
 * This method is not invoked when gestures are used for user interaction. When gestures are used,
 * your custom CC3Layer should process them and invoke higher-level application-defined behaviour
 * on this customized CC3Scene subclass.
 *
 * For more info, read the notes of this method on CC3Scene.
 */
void CC3HelloWorldScene::touchEvent( GLuint touchType, const CCPoint& touchPoint )
{
	// super::touchEvent( touchType, touchPoint );

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
			CC3Camera* activeCam = getActiveCamera();
			if ( activeCam )
			{
				float diffX = touchPoint.x - _lastTouchEventPoint.x;
				float diffY = touchPoint.y - _lastTouchEventPoint.y;
				activeCam->rotateBy( cc3v( diffY, 0, 0 ) );
				activeCam->rotateBy( cc3v(0, -diffX, 0 ) );
			}
		}
		break;
	case kCCTouchEnded:
		break;
	default:
		break;
	}

	// For all event types, remember when and where the touchpoint was, for subsequent events.
	_lastTouchEventPoint = touchPoint;
	_lastTouchEventTime = now;
}

/**
 * This callback template method is invoked automatically when a node has been picked
 * by the invocation of the pickNodeFromTapAt: or pickNodeFromTouchEvent:at: methods,
 * as a result of a touch event or tap gesture.
 *
 * Modify this method to perform activities on 3D nodes that have been selected by the user.
 *
 * For more info, read the notes of this method on CC3Scene.
 */
void CC3HelloWorldScene::nodeSelected( CC3Node* aNode, GLuint touchType, const CCPoint& touchPoint )
{	
	if ( aNode == NULL )
		return;

	// Provide some user feedback by "pulsing" the touched node. We do this by temporarily making
	// it grow larger and then shrink down again. Because the user might touch the node again while
	// it is in the middle of a pulse, we stop the previous pulse action before creating a new one.
	// At this point the hello text node has several actions running on it simultaneously. We can
	// locate the pulse action we want to stop by giving it a unique tag when it is added to the node.
	
	unsigned int pulseActionTag = 19;				// Can be any integer that will be unique among actions on this node.
	aNode->stopActionByTag( pulseActionTag );	// Remove any existing pulse action first.
	
	// Now, create a new pulse action and run it with the same tag so we can find it later if we want to stop it.
	/*CCActionInterval* grow = CC3ActionScaleTo::actionWithDuration( 0.25f, 1.2f );
	CCActionInterval* shrink = CC3ActionScaleTo::actionWithDuration( 0.25f, 1.0f );
	CCAction* pulse = CCActionSequence actionOne: grow two: shrink];
	aNode->runAction( pulse, pulseActionTag );*/
}

void CC3HelloWorldScene::addFloatingRing()
{
	CC3MeshNode* floater = CC3PlaneNode::nodeWithName( "Floater" );
	floater->populateAsCenteredRectangleWithSize( CCSizeMake( 5.f, 5.f ) );
//	floater->setColor( ccc3( 255, 0, 255 ) );
	floater->setTexture( CC3Texture::textureFromFile( "images/menuTitle.png" ) );
	floater->setIsOpaque( false );
	floater->setShouldCullBackFaces( false );
	floater->setShouldDrawLowAlpha( true );
	floater->getMaterial()->setAlphaTestReference( 0.05f );
//	floater->setLocation( cc3v(400.f, 150.f, 10.f) );
//	floater->setTouchEnabled( true );
	floater->setShouldDrawLocalContentWireframeBox( false );

	//floater->setRotation( cc3v( 45, 0, 0 ) );
	floater->setUniformScale( 0.5f );

	addChild( floater );
}

void CC3HelloWorldScene::addGround()
{
	CC3MeshNode*_ground = CC3PlaneNode::nodeWithName( "Ground" );
	_ground->populateAsDiskWithRadius( 1500, CC3TessellationMake(8, 32) );

	// To demonstrate that a Cocos3D CC3Texture can be created from an existing Cocos2D CCTexture,
	// we first load a CCTexture, and create the CC3Texture from it. We then assign the CC3Texture
	// a unique name and add it to the texture cache it so it will be available for later use.
	/*CCTexture* tex2D = CCTextureCache::sharedTextureCache()->addImage( kGroundTextureFile );
	CC3Texture* tex3D = CC3Texture::textureWithCCTexture( tex2D );
	tex3D->setName( kGroundTextureFile );
	CC3Texture::addTexture( tex3D );*/
	_ground->setTexture( CC3Texture::textureFromFile( "Grass.jpg" ) );

	// To simply load a Cocos3D texture directly, without first loading a Cocos2D texture,
	// comment out the lines above, and uncomment the following line.
//	_ground.texture = [CC3Texture textureFromFile: kGroundTextureFile];

	// The ground uses a repeating texture
	_ground->repeatTexture( tex2( 10, 10 ) );		// Grass
//	[_ground repeatTexture: (ccTex2F){3, 3}];		// MountainGrass
	
	_ground->setLocation( cc3v(0.0, -20, 0.0) );
	_ground->setRotation( cc3v(-90.0, 180.0, 0.0) );
	_ground->setShouldCullBackFaces( false );	// Show the ground from below as well.
	_ground->setTouchEnabled( true );			// Allow the ground to be selected by touch events.
	_ground->retainVertexLocations();	// Retain location data in main memory, even when it
										// is buffered to a GL VBO via releaseRedundantContent,
										// so that it may be accessed for further calculations
										// when dropping objects on the ground.
	addChild( _ground );

	_ground->setUniformScale( 0.1f ); 
}

void CC3HelloWorldScene::addBrickWall()
{
	DoorMeshNode* _brickWall = DoorMeshNode::nodeWithName( "BrickWall" );
	_brickWall->setTouchEnabled( true );
	_brickWall->populateAsSolidBox( CC3BoxMake(-1.5f, 0.f, -0.3f, 1.5f, 2.5f, 0.3f) );
	_brickWall->setUniformScale( 40.0f );

	// Add a texture to the wall and repeat it. This creates a material automatically.
	_brickWall->setTexture( CC3Texture::textureFromFile( "Bricks-Red.jpg" ) );
	_brickWall->repeatTexture( tex2(4, 2) );
	_brickWall->setAmbientColor( kCCC4FWhite );			// Increase the ambient reflection so the backside is visible

	// Start with the wall in the open position
	_brickWall->setIsOpen( true );
	//_brickWall->setLocation( kBrickWallOpenLocation );
	_brickWall->setRotation( cc3v(0, -45, 0) );

	// Wall is added on on background thread. Configure it for the scene, and fade it in slowly.
//	configureForScene( _brickWall, kFadeInDuration );
	addChild( _brickWall );

	_brickWall->setLocation( cc3v(0.0, -20, -110.0) );

	_brickWall->setUniformScale( 10.f );
}

void CC3HelloWorldScene::addAxesDirectionMarkers()
{
	CC3Node* teapotRed = CC3ModelSampleFactory::factory()->makeUniColoredTeapotNamed( "TeapotRed", ccc4f(0.7f, 0.0f, 0.0f, 1.0f) );
	teapotRed->setLocation( cc3v(0, -2.0, 5.0) );
	teapotRed->setTouchEnabled( true );		// allow this node to be selected by touch events

	// Teapot is added on on background thread. Configure it for the scene, and fade it in slowly.
	addChild( teapotRed );

	// Green teapot is at postion 100 on the Y-axis
	// Create it by copying the red teapot.

	teapotRed->setUniformScale( 5.f );
}

void CC3HelloWorldScene::configNode( CC3Node* aNode )
{
	aNode->createBoundingVolumes();
	aNode->retainVertexLocations();
	aNode->retainVertexIndices();
	aNode->retainVertexBoneWeights();
	aNode->retainVertexBoneIndices();
	aNode->createGLBuffers();
	aNode->releaseRedundantContent();
}

void CC3HelloWorldScene::addRobot()
{
	CC3ResourceNode* podRezNode = CC3PODResourceNode::nodeWithName( "PODRobot" );
	podRezNode->populateFromResource( IntroducingPODResource::resourceFromFile("IntroducingPOD_float.pod") );

	// If you want to stop the robot arm from being animated, uncomment the following line.
	//	[podRezNode disableAllAnimation];

	podRezNode->setTouchEnabled( true );
	addChild( podRezNode );

	podRezNode->setLocation( cc3v(0, 0, -10.0) );
	podRezNode->setUniformScale( 0.05f );

	// Start the animation of the robot arm and bouncing lamp from the PVR POD file contents.
	// But we'll have a bit of fun with the animation, as follows.
	// The basic animation in the POD pirouettes the robot arm in a complex movement...
	CC3ActionInterval* pirouette = CC3ActionAnimate::actionWithDuration( 5.0f );

	podRezNode->runAction( CC3RepeatForever::create( pirouette ) );
#if 0
	// Extract only the initial bending-down motion from the animation, reverse it to create
	// a stand-up motion, and paste the two actions together to create a bowing motion.
	CCActionInterval* bendDown = CC3ActionAnimate::actionWithDuration( 1.8f, 0.0f, 0.15f );
	CCActionInterval* standUp = bendDown->reverse();
	CCActionInterval* takeABow = CCActionSequence::createWithTwoActions( bendDown, standUp );

	// Now...put it all together. The robot arm performs its pirouette, and then takes a bow,
	// over and over again.
	podRezNode->runAction( CCActionRepeatForever::create( CCActionSequence::createWithTwoActions( pirouette, takeABow ) ) );
#endif
}

void CC3HelloWorldScene::addSkinnedRunner()
{
	CC3ResourceNode* runner = CC3PODResourceNode::nodeWithName( "Runner", "RunningMen/man.pod" );

	// Make the runner a little more visible underl lighting
	runner->setDiffuseColor( kCCC4FWhite );
	runner->setAmbientColor( kCCC4FWhite );

	runner->setTouchEnabled( true );		// make the runner touchable

	// Place the man on the track, near the edge of the ground frame
	//	runner->setLocation( cc3v(0, 0, 1100) );
	runner->setLocation( cc3v(0, -20, 0) );
	//	runner->setRotation( cc3v(0, 90, 0) );	// Rotate the entire POD resource so camera rotates as well
	runner->setUniformScale( 0.03f );
	runner->runAction( CC3RepeatForever::create( CC3ActionAnimate::actionWithDuration(1.4f) ) );

	addChild( runner );
	configNode( runner );
}

void CC3HelloWorldScene::addSkyBox()
{
	CC3MeshNode* skyBox = CC3SphereNode::nodeWithName( "SkyBox" );
//	skyBox->populateAsSphereWithRadius( 800, CC3TessellationMake(24, 24) );

	GLfloat boxSideLen = 1600;
	skyBox->populateAsSolidBox( CC3BoxMake( -boxSideLen, -boxSideLen, -boxSideLen, boxSideLen, boxSideLen, boxSideLen ) );
	skyBox->setShouldCullBackFaces( false );
//	skyBox->setDiffuseColor( ccc4f(0.4, 0.5, 0.9, 1.0) );
	skyBox->setTexture( CC3Texture::textureCubeFromFilePattern( "EnvMap/sp3/sp3%s.jpg" ) ); 
	skyBox->applyEffectNamedFromFile( "SkyBox", "EnvMap/EnvMap.pfx" );
	skyBox->setShouldCullFrontFaces( true );
	skyBox->setDepthFunction( GL_LEQUAL );
	skyBox->setShouldDisableDepthTest( false );
	skyBox->setShouldUseClockwiseFrontFaceWinding( false );
	skyBox->setShouldDisableDepthMask( true );

	addChild( skyBox );
//	configNode( skyBox );
}

void CC3HelloWorldScene::addPointHose() 
{
#if 0
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
	emitter->setTexture( CC3Texture::textureFromFile( "fire.png" ) );
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
	emitter->setMinParticleStartingColor( kCCC4FCyan );
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

	configNode( emitter );

#else
	std::string itemName = "";

	// Create a parametric textured cube, centered on the local origin.
	CC3BoxNode* texCube = CC3BoxNode::nodeWithName( "TexturedCube" );
	texCube->populateAsSolidBox( CC3BoxMake(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f) );
	texCube->setUniformScale( 30.0f );
	texCube->setShouldDrawBoundingVolume( true );

	// Add a texture to the textured cube. This creates a material automatically.
	// For kicks, we use a texture that contains two distinct images, one for a box and
	// one for a ball, and set a texture rectangle on the node so it will use only one
	// part of the texture to cover the box.
	texCube->setTexture( CC3Texture::textureFromFile( "BallBoxTexture.png" ) );
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
	SpinningNode* _texCubeSpinner = SpinningNode::nodeWithName( itemName );
	_texCubeSpinner->setFriction( 1.0f );
	_texCubeSpinner->setLocation( cc3v(-0, 75.0, 0.0) );
	_texCubeSpinner->setLocation( cc3v(-10.0, -20, -150.0) );
	_texCubeSpinner->setTouchEnabled( true );

	// Add the cube to the spinner and the spinner to the scene.
	_texCubeSpinner->addChild( texCube );

	// Cube is added on on background thread. Configure it for the scene, and fade it in slowly.
	//configureForScene( _texCubeSpinner, = );
	addChild( _texCubeSpinner );

	configNode( _texCubeSpinner );
	_texCubeSpinner->setIsFreeWheeling( true );
	_texCubeSpinner->setSpinSpeed( 10 );
	_texCubeSpinner->setSpinAxis( cc3v(0, 1, 0) );

#endif
}

void CC3HelloWorldScene::addProjectedLabel()
{
	CCLabelTTF* bbLabel = CCLabelTTF::create( "Whoa...I'm dizzy!", "Marker Felt", 18.0f );
	CC3Billboard* bb = CC3Billboard::nodeWithName( "PojectLabel", bbLabel );
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

void CC3HelloWorldScene::addBitmapLabel()
{
	CylinderLabel* bmLabel = CylinderLabel::nodeWithName( "BitmapLabel" );
	bmLabel->setRadius( 50.f );
	bmLabel->setTextAlignment( kCCTextAlignmentCenter );
	bmLabel->setRelativeOrigin( ccp(0.5, 0.5) );	
	bmLabel->setTessellation( CC3TessellationMake(4, 1) );
	bmLabel->setFontFileName( "Arial32BMGlyph.fnt" );
	bmLabel->setLabelString( "Hello, world!" );

	bmLabel->setLocation( cc3v(0, 0, -20) );
	bmLabel->setLocation( cc3v(-10.0, -20, -150.0) );
	bmLabel->setRotation( cc3v(0.0, 180.0, 0.0) );
	bmLabel->setUniformScale( 1.0f );
	bmLabel->setColor( CCColorRefFromCCC4F(ccc4f(1.0f, 0, 0.45f, 1.0f)) );
	bmLabel->setShouldCullBackFaces( false );			// Show from behind as well.
	bmLabel->setTouchEnabled( true );

	// Label is added on on background thread. Configure it for the scene, and fade it in slowly.
	configNode( bmLabel );
	addChild( bmLabel );
	bmLabel->runAction( CC3ActionRotateForever::actionWithRotationRate(cc3v(0, 30, 0)) );
}
