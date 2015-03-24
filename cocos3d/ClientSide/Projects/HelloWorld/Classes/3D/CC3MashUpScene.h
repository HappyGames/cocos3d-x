#ifndef _CC3_MASHUP_SCENE_H_
#define _CC3_MASHUP_SCENE_H_
#include "cocos3d.h"
#include "Models.h"

USING_NS_COCOS3D;

class CC3MashUpLayer;

/** Enumeration of camera zoom options. */
typedef enum 
{
	kCameraZoomNone,			/**< Inside the scene. */
	kCameraZoomStraightBack,	/**< Zoomed straight out to view complete scene. */
	kCameraZoomBackTopRight,	/**< Zoomed out to back top right view of complete scene. */
} CameraZoomType;

/** Enumeration of lighting options. */
typedef enum 
{
	kLightingSun,				/**< Sunshine. */
	kLightingFlashlight,		/**< Nightime with flashlight. */
	kLightingLightProbe,		/**< Light probe uses cube texture instead of light calcs. */
	kLightingFog,				/**< Sunshine with fog. */
	kLightingGrayscale,			/**< Sunshine with grayscale post-processing filter. */
	kLightingDepth,				/**< Display the depth buffer using a post-processing filter. */
} LightingType;

/** Enumeration of dragon motion options. */
typedef enum 
{
	kDragonStill,			/**< No motion. */
	kDragonFlapping,		/**< Flapping. */
	kDragonGliding,			/**< Gliding. */
} DragonMotion;

/**
 * A sample application-specific CC3Scene subclass that demonstrates a number of 3D features:
 *   - loading mesh models, cameras and lights from 3D model files stored in the PowerVR POD format
 *   - creating mesh models from static header file data
 *   - sharing mesh data across several nodes with different materials
 *   - loading 3D models from a POD file converted from a Collada file created in a 3D editor (Blender)
 *   - assembling nodes into a hierarchical parent-child structual assembly.
 *   - programatic creation of spherical, box and plane meshes using parametric definitions.
 *   - texturing a 3D mesh from a CCTexture image
 *   - transparency and alpha-blending
 *   - translucent and transparent textures
 *   - coloring a mesh with a per-vertex color blend
 *   - multi-texturing an object using texture units by combining several individual textures into overlays
 *   - DOT3 bump-map texturing of an object to provide high-resolution surface detail on a model
 *     with few actual vertices
 *   - Vertex skinning with a soft-body mesh bending and flexing based on the movement of skeleton bone nodes.
 *   - Copying soft-body nodes to create a completely separate character, with its own skeleton, that can be
 *     manipulated independently of the skeleton of the original.
 *   - animating 3D models using a variety of standard Cocos2D CCActionIntervals
 *   - overlaying the 3D scene with 2D Cocos2D controls such as joysticks and buttons
 *   - embedding 2D Cocos2D text labels into the 3D scene
 *   - incorporating 2D Cocos2D CCParticleEmitters into the 3D scene (as a sun and explosion fire)
 *   - emitting 3D point particles from a moving nozzle, with realistic distance attenuation
 *   - emitting two different types of 3D mesh particles, with distinct textures, from a moving nozzle,
 *     with each particle moving, rotating, and fading independently
 *   - creating a tightly focused spotlight whose intensity attenuates with distance
 *   - directing the 3D camera to track a particular target object
 *   - directing an object to track the camera, always facing (looking at) the camera (aka halo objects)
 *   - directing an object to track another object, always facing (looking at) that object
 *   - selecting a 3D object by touching the object on the screen with a finger
 *   - placing a 3D object on another at a point that was touched with a finger
 *   - adding a small CC3Layer/CC3Scene pair as a child window to a larger CC3Layer/CC3Scene pair.
 *   - moving, scaling and fading a CC3Layer and its CC3Scene
 *   - creating parametric boxes and texturing all six sides of the box with a single texture.
 *   - adding an object as a child of another, but keeping the original orientation of the child
 *     (addAndLocalizeChild:)
 *   - handling touch-move events to create swipe gestures to spin a 3D object using rotation
 *     around an arbitrary axis
 *   - toggling between opacity and translucency using the isOpaque property
 *   - choosing to cull or display backfaces (shouldCullBackFaces)
 *   - creating and deploying many independent copies of a node, while sharing the underlying mesh data
 *   - drawing a descriptive text label on a node using CC3Node shouldDrawDescriptor property.
 *   - drawing a wireframe bounding box around a node using CC3Node shouldDrawWireframeBox property.
 *   - automatically zooming the camera out to view all objects in the scene
 *   - constructing and drawing a highly tessellated rectangular plane mesh using CC3PlaneNode
 *   - caching mesh data into GL vertex buffer objects and releasing vertex data from application memory
 *   - retaining vertex location data in application memory (retainVertexLocations) for subsequent calculations
 *   - moving the pivot location (origin) of a mesh to the center of geometry of the mesh.
 *   - attaching application-specific userData to any node
 *   - applying a texture to all six sides of a parametric box
 *   - displaying direction marker lines on a node to clarify its orientation during development.
 *   - displaying a repeating texture pattern across a mesh
 *   - creating and displaying shadow volumes to render shadows for selected nodes
 *   - detecting the local location of where a node was touched using ray tracing
 *   - collision detection between nodes
 *   - texturing a node with only a small section of single texture
 *   - using the CC3Scene onOpen method to initiate activity when a scene opens
 *   - using pinch and pan gestures to control the movement of the 3D camera
 *   - using tap gestures to select 3D objects, and pan gestures to spin 3D objects
 *   - bitmapped font text labels
 *   - moving individual vertex location programmatically
 *   - using OpenGL ES 2.0 shaders.
 *   - loading PowerVR PFX effects files and applying them to materials
 *   - environmental reflections using a cube mapped texture.
 *   - render-to-texture the scene for display within the scene.
 *   - render-to-texture to create additional visual effects using post-rendering image processing.
 *   - render depth-to-texture to visualize the contents of the depth buffer.
 *   - read pixels from a rendered framebuffer
 *   - replace framebuffer and texture pixels with programmatic content
 *   - create CGImageRef from a rendered framebuffer
 *   - dynamically generate an environmental cube-map for creating a real-time dynamic reflective surfaces.
 *   - apply multiple animation tracks to a model, blend them together, and smoothly transition between
 *     animation tracks using a cross-fade action.
 *   - bypassing Xcode/iOS automatic pre-multiplying of texture color content with alpha content
 *   - scene lighting using light probes containing cube-map textures, instead of discrete lights
 *
 * In addition, there are a number of interesting options for you to play with by uncommenting
 * certain lines of code in the methods of this class that build objects in the 3D scene,
 * including experimenting with:
 *   - simple particle generator with multi-colored, light-interactive, particles
 *   - simple particle generator with meshes updated less frequently to conserve performance 
 *   - different options for ordering nodes when drawing, including ordering by mesh or texture
 *   - configuring the camera for parallel/isometric/orthographic projection instead of the default
 *     perpective projection
 *   - mounting the camera on a moving object, in this case a bouncing ball
 *   - mounting the camera on a moving object, in this case a bouncing ball, and having the
 *     camera stay focused on the rainbow teapot as both beach ball and teapot move and rotate
 *   - directing an object to track another object, always facing that object, but only
 *     rotating in one direction (eg- side-to-side, but not up and down).
 *   - displaying 2D labels (eg- health-bars) overlayed on top of the 3D scene at locations projected from the position of 3D objects
 *   - disabling animation for a particular node, in this case the camera and light
 *   - invading with an army of teapots instead of robots
 *   - ignore lighting conditions when drawing a node to draw in pure colors and textures
 *   - Making use of the userData property to add app-specific information to a node.
 *   - displaying descriptive text and wireframe bounding boxes on every node
 *   - displaying a dynamic bounding box on a 3D particle emitter.
 *   - making use of a fixed bounding volume for the 3D particle emitter to improve performance.
 *   - permitting a node to cast a shadow even when the node itself is invisible by using the shouldCastShadowsWhenInvisible property
 *   - Skybox using a cube mapped texture.
 *   - Cocos2D CCSprite displaying the television screen rendered texture
 *
 * The camera initially opens on a scene of an animated robot arm with a 2D label attached to the
 * end of the rotating arm, demonstrating the technique of embedding a 2D CCNode into the 3D scene.
 * The robot arm is loaded from a POD file, along with the moving light and the camera.
 *
 * As you watch, additional objects appear in the scene. These objects are loaded from files
 * on a background thread, and seamlessly inserted into the scene. For dramatic effect, each
 * object is faded in using a fade action. The background thread pauses before addin each new
 * object, to extend the activity over time. This is also strictly for dramatic effect here.
 * The entire scene takes between 10 and 15 seconds to appear.
 *
 * Most of the 3D objects are selectable by touching. Touching any of the 3D objects with
 * your finger will display the location of the touch on the object itself, in the 3D
 * coordinate system of the touched node. This is performed by converting the 2D touch
 * point to a 3D ray, and tracing the ray to detect the nodes that are punctured by the ray.
 *
 * If the ground plane is touched, a little orange teapot will be placed on the ground at the
 * location of the touch point, demonstrating the ability to integrate touch events with object
 * positioning in 3D (sometimes known as unprojecting). For dramatic effect, as the teapot is
 * placed, a fiery explosion is set off using a Cocos2D CCParticleSystem, demonstrating the
 * ability to embed dynamic 2D particle systems into a 3D scene. Once the explosion particle
 * system has exhausted, it is automatically removed as a child of the teapot.
 *
 * Touching the robot arm, or the label it is carrying, turns on a hose that emits a stream of
 * multi-colored 3D point particles from the end of the robot arm. As the robot arm moves, the nozzle
 * moves with it, spraying the stream of particles around the 3D scene. These are true 3D point
 * particles. Each particle has a 3D location, and appears smaller the further it is from the camera.
 *
 * Touching the robot arm again turns off the point hose and turns on a hose that emits a stream
 * of small mesh particles, containing spheres and boxes. All meshes emitted by a single particle
 * emitter must use the same material and texture, but the spheres and boxes use different sections
 * of a single texture, demonstrating the use of textureRectangle property of a particle (or mesh).
 * Each mesh particle moves, rotates, and fades independently.
 *
 * Touching the robot arm or label again will turn off both the point and mesh hoses.
 *
 * The robot arm is surrounded by three small teapots, one red, one green, and one blue.
 * These teapots are positioned at 100 on each of the X, Y and Z axes respectively (so the
 * viewer can appreciate the orientation of the scene.
 *
 * A fourth teapot, this one white, indicates the position of the light source, which is also
 * animated. You can see the effect on the lighting of the scene as it moves back and forth.
 *
 * Behind and to the right of the robot arm is a text label that is wrapped around a circular arc and
 * rotating around the center of that circular arc, as if it was pasted to an invisible cylinder.
 * Touching this text label will set a new text string into the label and change its color. This
 * curved label is different than the label held by the robot arm, in that it is actually constructed
 * as a 3D mesh (whereas the label held by the robot arm is a 2D Cocos2D artifact). Since this rotating
 * label is a 3D mesh, its vertex content can be manipulated programmatically. This is demonstrated here
 * by moving the individual vertices so that they appear to be wrapped around an imaginary cylinder.
 *
 * Behind the to the left of the robot arm is a wooden mallet that is animated to alternately
 * hammer two wooden anvils. The hammer bends and flexes as it bounces back and forth,
 * demonstrating the technique of vertex skinning to deform a soft-body mesh based on the
 * movement of an underlying skeleton constructed of bones and joints.
 *
 * As you watch the scene, two running figures will pass by. These figure run in a circular
 * path around the scene. The runners are also comprised of soft-body meshes that flex and
 * bend realistically based on the movement of an underlying skeleton of bones and joints.
 *
 * Both the mallet and the runners are controlled by skeletons whose bones are moved and
 * rotated using animation data loaded from the POD file. Because of the complexity of
 * controlling multiple joints in a skeleton, animation, as created in a 3D editor, is
 * the most common technique used for controlling vertex skinning using skeletons.
 *
 * However, these skeletons are simply structural node assemblies, with each bone being
 * represented with a separate node. Therefore, the bones and joints of a skeleton can be
 * moved and rotated using programatic control, or through interaction with a physics engine.
 *
 * To see the runners up close, touch one of the runners (which can be a bit tricky, as they
 * are fast). This will switch the view to a camera that is travelling with the runners, giving
 * you a close-up of the meshes that makes up the runners flexing realistically as they run.
 * 
 * Up-close, you'll notice that one runner is smaller than the other and is having to run
 * with a faster stride than the larger runner. This smaller runner was actually created
 * from a copy of the larger runner, and give a different animation rate. This demonstrates
 * the ability to copy soft-body nodes, and that, after copying, each soft-body node will
 * have its own skin and skeleton that can be manipulated separately.
 * 
 * Touching the runners again will switch back to the original camera that is viewing the
 * larger scene. This demonstrates the ability to have more than one camera in the scene
 * and to switch between them using the activeCamera property of the scene.
 *
 * At any time, you can move the camera using the two joysticks. The left joystick controls the
 * direction that the camera is pointing, and the right joystick controls the location of the camera,
 * moving forward, back, left and right. By experimenting with these two joysticks, you should be
 * able to navigate the camera all around the 3D scene, looking behind, above, and below objects.
 *
 * Under iOS, you can also move the camera using gestures directly on the screen. A double-finger
 * drag gesture will pan the camera around the scene. And a pinch gesture will move the camera
 * forwards or backwards. Gestures are not used when running under Android or OSX.
 *
 * Using the left joystick, you can redirect the camera to look far away in the direction
 * of the light source by extrapolating a line from the base of the robot arm through the
 * white teapot. There you will find the sun hanging in the sky, as a dynamic particle
 * emitter. This demonstrates the ability to embed standard Cocos2D particle emitters
 * into a 3D scene. The sun is quite a large particle emitter, and you should notice a
 * drop in frame rate when it is visible.
 *
 * The scene is given perspective by a ground plane constructed from the Cocos2D logo. This ground
 * plane is configured so that, in addition to its front face, its backface will also be drawn.
 * You can verify this by moving the camera down below the ground plane, and looking up.
 *
 * Touching the switch-view button (with the green arrow on it) between the two joysticks
 * will point the camera at a second part of the scene, at a rotating globe, illustrating
 * the creation of a sphere mesh programatically from a parametric definition, and the
 * texturing of that mesh using a rectangular texture.
 *
 * Touching the globe will open a child HUD (Heads-Up-Display) window showing a close-up of
 * the globe (actually a copy of the globe) in a child CC3Layer and CC3Scene. The small window
 * contains another CC3Layer and CC3Scene. The scene contains a copy of the globe, and the camera of
 * the scene automatically frames the globe in its field of view invoking one of the CC3Camera
 * moveToShowAllOf:... family of methods, from the onOpen callback method of the HUDScene.
 * 
 * This small HUD window opens minimized at the point on the globe that was touched, and
 * then smoothly expands and moves to the top-right corner of the screen. The HUD window,
 * and the globe inside it are semi-transparent. As you move the camera around, you can
 * see the main scene behind it. Touching the HUD window or the globe again will cause
 * the HUD window CC3Layer and CC3Scene to fade away.
 *
 * To the left of the globe is a large rotating rectangular orange ring floating above the ground.
 * This ring is created from a plane using a texture that combines transparency and opacity. It
 * demonstrates the use of transparency in textures. You can see through the transparent areas to
 * the scene behind the texture. This is particularly apparent when the runners run behind the
 * ring and can be seen through the middle of the ring. The texture as a whole fades in and out
 * periodically, and rotates around the vertical (Y) axis.
 *
 * The texture loaded for this ring has a special PPNG file extension that will stop Xcode from
 * pre-multipying the alpha component of the texture into the color components, and the PPNG
 * file is loaded with a custom image loader, again, to avoid iOS pre-multiplying the texture
 * content during image loading.
 *
 * As the ring rotates, both sides are visible. This is because the shouldCullBackFaces property is
 * set to NO, so that both sides of each face are rendered.
 *
 * Touching the switch-view button again will point the camera at a bouncing, rotating beach ball.
 * Touching the beach ball will toggle the beach ball between fully opaque and translucent,
 * demonstrating how the opacity property can be used to conveniently change transparency.
 *
 * When the beach ball is translucent, you can see objects through the ball. This is particularly
 * apparent if you move the camera so that it is behind the ball, and look back through the ball
 * at the other objects in the scene.
 *
 * To be multi-colored, the beach ball sports several materials. This is done by constructing the
 * beach ball as a parent node with four child nodes (and meshes), one for each colored material.
 * This breakdown is handled by the POD file exporter, and is automatically reconstructed during
 * standard loading from a POD file here. This demonstrates the parent-child nature of nodes.
 * Moving and rotating the parent beach ball node moves and rotates the children automatically.
 *
 * Although the beach ball is constructed from four separate mesh nodes, touching any part of the
 * beach ball will actually select the node representing the complete beach ball, and the entire
 * beach ball is highlighted.
 *
 * Behind the beach ball is a television. Touching the television will turn it on and have it
 * display a feed from the camera tracking the two running men. This demonstrates the ability
 * to render a scene (or a part of a scene) to a texture, and include that texture into the
 * scene, as well as the abilty to use more than one camera to film the same scene. Take note
 * of the TV image as the runners pass in front of the television. You will see an infinitely
 * recursive image of the runners!
 *
 * In the bottom-right corner of the TV screen is a small picture-in-picture window showing 
 * the head of the smaller runner, and framed by a red border. This effect is created by 
 * copying a rectangle of pixels from the TV screen surface, processing them in the CPU to
 * add the border, and then pasting those pixels back into the surface (and the underlying
 * texture) in a different location. This demonstrates the ability to process the contents
 * of a rendered texture in application memory using the CPU.
 *
 * Touching the television again will turn it off. Rendering to the television screen occurs
 * only when the television is turned on and in view of the active camera. In addition, whenever
 * you turn the television off, the current image on the television screen is extracted and 
 * saved to a JPEG file at Documents/TV.jpg. This demonstrates the ability to extract a 
 * CGImageRef from the contents of a rendered scene.
 *
 * Touching the switch-view button again will point the camera at yet another teapot, this one
 * textured with a metallic texture, and rotating on it's axis. When running GLSL shaders, under
 * either OpenGL ES 2.0 on iOS or OpenGL on OSX, this teapot dynamically reflects the environment.
 * This reflection effect is created by applying a cube-mapped texture to the teapot, and
 * dynamically rendering that texture on each frame. As objects move around in the scene, their
 * reflections in the teapot move correspondingly.
 * 
 * This reflective teapot has another smaller rainbow-colored teapot as a satellite. This satellite
 * is colored with a color gradient using a color array, and orbits around the teapot, and rotates
 * on it's own axes. The rainbow teapot is a child node of the textured teapot node, and rotates
 * along with the textured teapot.
 *
 * Touching either teapot will toggle the display of a wireframe around the mesh of that teapot
 * (orange), and a wireframe around both teapots (yellow). This easily is done by simply setting
 * the shouldDrawLocalContentWireframeBox and shouldDrawWireframeBox properties, respectively.
 * Notice that the wireframes move, rotate, and scale along with the teapots themselves, and
 * notice that the yellow wireframe that surrounds both teapots grows and shrinks automatically
 * as the rainbow teapot rotates and stretches the box around both teapots.
 *
 * Behind the rotating teapots is a brick wall. Touching the brick wall will animate the wall to
 * move into the path of the rainbow teapot. When the teapot collides with the wall, it bounces off
 * and heads in the opposite direction. As long as the brick wall is there, the rainbow teapot will
 * ping-pong back and forth in its orbit around the textured teapot. Touching the brick wall again
 * will move the wall out of the way of the teapot and back to its original location. This demonstrates
 * the ability to perform simple collision detection between nodes using the doesIntersectNode: method.
 * See the checkForCollisions method of this class for an example of how to use this feature.
 *
 * Touching the switch-view button again will point the camera at two copies of Alexandru Barbulescu's
 * 3D Cocos3D mascot. The mascot on the left stares back at the camera, regardless of where you move
 * the camera in the 3D scene (which you do using the right joystick). This kind of object is also
 * known as a halo object, and can be useful when you always want an object to face the camera.
 *
 * The second mascot is distracted by the satellite rainbow teapot. The gaze of this second
 * mascot tracks the location of the rainbow teapot as it orbits the textured teapot.
 *
 * Both mascots make use of targetting behaviour to point themselves at another object. You can
 * add any object as a child to a targetting node, orient the child node so that the side that you
 * consider the front of the object faces in the forwardDirection of the targetting node, and then
 * tell the targetting node to point in a particular direction, or to always point at another node,
 * and track the motion of that other node as it moves around in the scene.
 *
 * By uncommenting documeted code in the configureCamera method, the camera can be targetted
 * at another node, demonstrating an "orbit camera" by simply giving your camera a target to
 * track. As you move the camera around, it will continue to look at the target object.
 *
 * To the left of the mascots are two masks. The visual effects applied to this masks are defined
 * in a PowerVR PFX file that describe and configures the visual effects. Under OpenGL ES 2.0, a
 * PFX file contains "effects" each of which describes a combination of GLSL shaders and textures
 * that should be applied to the material of a mesh node to accomplish a particular visual effect
 * or look. The PFX file also includes declarations of semantics for the GLSL variables, allowing
 * Cocos3D to automatically populate the uniforms from the shaders with active content from the scene.
 *
 * For the upper mask, the PFX file and effect are referenced from the POD file for the mask model,
 * and the effect is loaded and attached to the model automatically when the POD file is loaded.
 * For the lower mask, the PFX effect from the PFX file is manually assigned to the model after
 * the POD model has been loaded.
 *
 * When running under OpenGL ES 2.0, the upper mask has a brightly reflective golden appearance that
 * shimmers as the object moves around. The effect is created by a combination of a base texture, a
 * static environmental reflective texture, and GLSL shaders that combine the two textures and the
 * scene lighting to create the effect.
 *
 * When running under OpenGL ES 2.0, the lower mask has an etched surface that appears three-dimensional.
 * This is accomplished by layering two textures, the first of which is a tangent-space bump-map that
 * treats the texels in the texture as normals, and uses them in combination with the direction of the
 * light source to create a detailed three-dimensional effect at a pixel level. This bump-mapping
 * is a common technique to simulate a very detailed textured surface with relatively few vertices.
 *
 * When running under OpenGL ES 1.1, the texture files described in the PFX files are applied to the
 * masks, but without the associated GLSL shaders to combine the textures, the textures are simply
 * combined in a standard modulation. Under OpenGL ES 1.1, further configuration would have to be
 * applied to the texture unit for each texture in order to enhance the appearance. Nevertheless,
 * this demonstrates the ability, under OpenGL ES 1.1, to use a PFX file to describe the textures
 * that should be applied to a material.
 *
 * Touching the switch-view button again will point the camera at a purple floating head that
 * looks back at the camera, regardless of where the camera moves. This floating head shows
 * quite realistic surface detail and shadowing that changes as the light source moves up
 * and down, or as the head turns to face the camera as it moves around. The surface detail,
 * and interaction with lighting is performed by a bump-map texture. The floating head has
 * two textures applied, the first is a bump-map which contains a surface normal vector in
 * each pixel instead of a color. These per-pixel normals interact with a vector indicating
 * the direction of the light source to determine the luminiosity of each pixel. A second
 * texture containing the purple featuring is then overlaid on, and combined with, the main
 * bump-map texture, to create the overall textured and shadowed effect.
 *
 * Bump-mapping is a technique used to provide complex surface detail without requiring a large
 * number of mesh vertices. The actual mesh underlying the floating head contains only 153 vertices.
 * 
 * Touching the purple floating head removes the bump-map texture, and leaves only the purple
 * texture laid on the raw mesh vertices. The surface detail virtually vanishes, leaving a
 * crude model of a head, and demonstrating that the surface detail and shadowing is contained
 * within the bump-mapped texture, not within the mesh vertices. The effect is quite striking.
 *
 * The light direction that is combined with the per-pixel texture normals to peform this bump-mapping
 * is provided by a orienting node, which holds both the wooden sign and the floating head as child nodes.
 * It keeps track of the location of the light, even as both the light and the models move around, and
 * automatically provides the light direction to the bump-mapped wooden sign and floating head nodes.
 *
 * Touching the purple head also logs an information message using userData that was attached
 * to the floating head an initialization time. The userData property can be used to attach
 * any application specific data that you want to any node, mesh, material, texture, etc.
 *
 * When running under OpenGL ES 1.1, to the right of the purple head is a wooden sign that is 
 * constructed from two separate textures that are loaded separately and applied as a multi-texture
 * to the sign mesh. When multiple textures are applied to a mesh, different techniques can be
 * configured for combining the textures to create interesting effects. The wooden sign is 
 * touchable, and touching the wooden sign will select a different method of combining the two 
 * textures. These methods of combining cycle through the following options when the wooden sign
 * is repeated touched: Modulation, Addition, Signed Addition, Simple Replacement, Subtraction,
 * and DOT3 bump-mapping (also known as normal mapping).
 *
 * The wooden sign is not visible when running under OpenGL ES 2.0 or OpenGL OSX. It makes use
 * of functionality that is provided by OpenGL ES 1.1. The same texture-combining functionality
 * is provided under OpenGL ES 2.0 and OpenGL OSX via GLSL shaders.
 *
 * This wooden sign also demonstrates the use of the textureRectangle property to cover a mesh with
 * a section of a texture. This feature can be used to extract a texture from a texture atlas, so
 * that a single loaded texture can be used to cover multiple meshes, with each mesh covered by a
 * different section fo the texture.
 *
 * Touching the switch-view button again will point the camera at a die cube. You can spin
 * this die cube by touching it and swiping your finger in any direction. The die will
 * spin in the direction of the swipe. The faster and longer you swipe, the faster the
 * die will spin. The spinning die will slow down over time, eventually stopping. This
 * spinning die cube demonstrates a number of useful features in Cocos3D:
 *   - The ability to rotate a 3D object around any axis.
 *   - The ability to convert touch-move events into swipe gestures to interact with a 3D object.
 *   - The separation of touch-event handling for control, and touch-event handling for node selection.
 *   - The behaviour of a node class under internal control using the updateBeforeTransform:
 *     method, in this case, to perform freewheeling and friction behaviour.
 *   - The ability to use the copyWithName:asClass: method to change the class of a node
 *     loaded from a POD file to add additional functionality to that node. This is done here
 *     so that the POD class can be swapped for one that controls the freewheeling and friction.
 *
 * Below the die cube is a multi-colored cube created parametrically and wrapped on all six
 * sides by a single texture. The texture is laid out specifically to wrap around box nodes.
 * See the BoxTexture.png image to see the layout of a texture that will be wrapped around
 * a box. Direction markers have been added to the node to show which side of the box faces
 * each direction in the local coordinate system of the node. Like the die cube, the
 * multi-color cube can be rotated with a swipe gesture.
 *
 * Poking out of the multi-color box are direction marker lines. During development,
 * these lines can be added to any node to help track the orientation of the node, by
 * using any of several convenience methods, including addDirectionMarker, 
 * addDirectionMarkerColored:inDirection: and addAxesDirectionMarkers. These direction
 * marker lines are oriented in the local coordinate system of the node.
 * 
 * Touching the switch-view button one final time will point the camera back at the animated
 * robot arm.
 *
 * Flying high above the scene is an dragon. Each time the dragon is touched it switches to
 * a different animated movement. The transition from one movement to the next is smooth,
 * regardless of when the dragon is touched. This demonstrates the ability to apply multiple
 * animation tracks to a model, blend them together, and to smoothly transition between those
 * tracks by using a cross-fade action to dynamically adjust the relative blending weight of
 * each animation track. 
 *
 * This dragon example also demonstrates the ability to concatenate many discrete animated
 * movements into a single long animation within a 3D editor, and single POD file, and then 
 * to extract each distinct movement into its own animation track within Cocos3D, so that
 * each can be applied by itself to the model, repeated in a loop, or blended with other
 * animated movements.
 *
 * Touching the invasion button (with the grid of dots on it) will unleash an army of robots,
 * by copying the main robot arm many times, and deploying the copies around the grid. Notice
 * that each of the robot arms moves independently. The army drops from the sky like rain.
 * The random rain is intentional, and is not some artifact of performance degredation.
 * Touching the invasion button again will cause the robot army to fade away and be removed.
 *
 * Touching the illumination button  (with the sun on it) turns the sun off and turns on a 
 * spotlight that is attached to the camera. This spotlight is tightly focused. Objects that
 * are away from the center of the spotlight are illuminated less than objects in the center 
 * of the spotlight. The intensity of the spotlight beam also attenuates with distance. 
 * Objects that are farther away from the spotlight are less illumnated than objects that 
 * are closer to the spotlight. Since it is attached to the camera, it moves as the camera
 * moves, as if you were walking through the scene carrying a flashlight.
 *
 * If you shine the spotlight on the purple floating head, you might notice two things.
 * The first is that the head is correctly illuminated from the position of the spotlight.
 * This is because the target of the wrapper holding the floating head and wooden sign is
 * switched from the main sunshine light to the spotlight. The second is that the floating
 * head appears fully illuminated even when the spotlight is not shining on it.
 * this is a funcion of the way that bump-map lighting works. It has no knowledge of the
 * configuration or focus of the spotlight, and therefore does not attenuate the per-pixel
 * illumination outside the beam of the spotlight. This is something to keep in mind when
 * combining the techniques of spotlights and bump-mapping.
 *
 * If the device is running OpenGL ES 2.0 or OpenGL OSX, touching the illumination button 
 * again turns the spotlight and main sun light off, and turns on a light probe. A light 
 * probe contains a cube-map texture that is used to determine the light that hits each 
 * node from all directions. This can be used to create realistic lighting in a 3D editor, 
 * and then apply it to the scene. If you touch one of the runners to view them up-close,
 * you can follow how the lighting on the runners changes as they face different directions,
 * as they run around the track. The light probe was added in the addLightProbe method.
 *
 * Touching the illumination button again turns the sun back on, but envelopes the scene in
 * a fog. The farther away an object is, the less visible it is through the fog. The effect
 * of the fog is best appreciated with the scene is full of the invading robot arms.
 *
 * If the device is running OpenGL ES 2.0 or OpenGL OSX, touching the illumination button 
 * again displays the view in grayscale, as if using black & white film. This effect is 
 * created by rendering the scene in color to a texture and then rendering the texture to
 * the screen using a shader that converts the texture colors to grayscale. This is only 
 * one example of such post-rendering processing. Using the same techique, you could add 
 * bloom effects, blurring, or other specialized colorizations.
 *
 * If the device is running OpenGL ES 2.0 or OpenGL OSX, touching the illumination button 
 * again displays a visualization of the contents of the depth buffer of the scene. 
 * This effect is created by attaching a texture as the depth buffer of an off-screen 
 * framebuffer surface, and then rendering the underlying texture to the screen using a 
 * shader that converts the depth values in the texture to a linearized grayscale image.
 *
 * Touching the illumination button again will bring back the original sunshine.
 *
 * Touching the zoom button (with the plus-sign) rotates the camera so that it points
 * towards the center of the scene, and moves the camera away from the scene along the
 * line between the camera and the center of the scene, until the entire scene is visible.
 * A wireframe is drawn around the entire scene to show its extent and the node descriptor
 * text is displayed to show the center of the scene. This demonstrates the moveToShowAllOf:
 * family of methods on CC3Camera, which, in addition to providing interesting orbit-camera
 * control for the app, can be particularly useful at development time for troubleshooting
 * objects that are not drawing correctly, either are not visible at all, or are unexpectedly
 * out of the camera's field-of-view.
 *
 * The camera now points to the center of the scene. However, the scene may appear to be
 * lying off to one side. This is due to perspective, depending on the location of the
 * camera. The center of the scene is in the center of the screen.
 * 
 * Also, after zooming out, you may notice that the left-most corner of the bounding box
 * is slightly off-screen. This is because the sun is a particle system billboard and
 * rotates as the camera pans out, effectively expanding the bounding box of the scene
 * as it turns. A similar effect will occur if the bounding box of the scene is dynamic
 * due to movement of nodes within the scene.
 *
 * Touching the zoom button a second time moves the camera to view the entire scene from
 * a different direction. Touching it a third time moves the camera back to the view it
 * had before the zoom button was touched the first time.
 *
 * Touching the shadow button puts the user interface into "shadow mode". While in "shadow
 * mode", touching any object will toggle the display of a shadow of that node. The shadows
 * are implemented using shadow volumes, which provide accurate fidelity to the object shape.
 * As the objects, light or camera moves, the shadow volumes are updated automatically.
 * To turn "shadow-mode" off, touch the shadow button a second time.
 *
 * Most of the dynamic motion in this scene is handled by standard Cocos2D CCActionIntervals.
 * User interaction is through buttons, which are 2D child layers on the main CC3DemoMashUpLayer,
 * and either gestures or touch and mouse event handling.
 *
 * Under iOS, you can select to use gestures for user interaction by setting the touchEnabled
 * property of the CC3DemoMashUpLayer to NO in its initializeControls method. If that property
 * is set to NO, the layer will use gestures for input, and if YES, the layer and scene will
 * use basic touch events to interact with the user. Gestures are not used when running under
 * Android or OSX.
 *
 * Under OSX, gestures are not supported, and so mouse events are used for user control.
 *
 * Vertex arrays and meshes are created only once for each mesh type, and are used by several
 * nodes. For exmample, all of the teapots: textured, colored or multi-colored, use the same
 * teapot mesh instance, but can be transformed separately, and covered with different materials.
 */
class CC3MashUpScene : public CC3Scene 
{
	DECLARE_SUPER( CC3Scene );
public:
	CREATE_FUNC( CC3MashUpScene );
	CC3MashUpScene();
	/**
	 * A weak reference back to the primary CC3MashUpLayer that is displaying this scene.
	 *
	 * This back-reference allows activities within the scene to modify user interface interactions, 
	 * such as enabling/disabling controls ro opening other view/layers.
	 *
	 * This property has been fomally deprecated to better support multiple CC3Layers displaying
	 * a single CC3Scene from different perspectives (different cameras). If you want to dedicate
	 * a single CC3Layer to a single CC3Scene, and hold a back reference to that layer within the
	 * scene, you should create and manage that reference in your custom CC3Scene class.
	 *
	 * This property is set automatically when this scene is assigned to the CC3DemoMashUpLayer.
	 */
	CC3MashUpLayer*				getPrimaryCC3MashUpLayer();
	void						setPrimaryCC3MashUpLayer( CC3MashUpLayer* pLayer );

	/**
	 * This property controls the velocity of the change in direction of the 3D camera
	 * (a proxy for the player). This property is set by the CC3Layer, from the velocity
	 * of the corresponding joystick control.
	 *
	 * The initial value of this property is CGPointZero.
	 */
	CCPoint						getPlayerDirectionControl();
	void						setPlayerDirectionControl( const CCPoint& dirControl );

	/**
	 * This property controls the velocity of the change in location of the 3D camera
	 * (a proxy for the player). This property is set by the CC3Layer, from the velocity
	 * of the corresponding joystick control.
	 *
	 * The initial value of this property is CGPointZero.
	 */
	CCPoint						getPlayerLocationControl();
	void						setPlayerLocationControl( const CCPoint& locControl );

	/**
	 * Indicates whether the UI is in "managing shadows" mode. When in this mode,
	 * touching an object will cycle through different shadow options for that
	 * object. When not in "managing shadows" mode, touching an object will take
	 * its normal action.
	 *
	 * The initial value of this property is NO.
	 */
	bool						isManagingShadows();
	void						setIsManagingShadows( bool isManaging );

	/** Launches an invasion of an army of robots. */
	void						invade();

	/**
	 * Cycles between different lighting and post-processing conditions:
	 *   - Sunshine on a clear day.
	 *   - Sunshine on a foggy day.
	 *   - Nighttime with the scene lit by a flashlight
	 *   - Sunshine on a clear day through a black & white camera (grayscale).
	 */
	void						cycleLights();

	/**
	 * Toggles between zooming out to show the entire scene, and zooming back in to the
	 * prevous camera position.
	 */
	void						cycleZoom();

	/**
	 * Start moving the camera using the feedback from a UIPinchGestureRecognizer.
	 *
	 * This method is invoked once at the beginning of each pinch gesture.
	 * The current location of the camera is cached. Subsequent invocations of the
	 * moveCameraBy: method will move the camera relative to this starting location.
	 */
	void						startMovingCamera();

	/**
	 * Moves the camera using the feedback from a UIPinchGestureRecognizer.
	 *
	 * Since the specified movement comes from a pinch gesture, it's value will be a
	 * scale, where one represents the initial pinch size, zero represents a completely
	 * closed pinch, and values larget than one represent an expanded pinch.
	 *
	 * Taking the initial pinch size to reference the initial camera location, the camera
	 * is moved backwards relative to that location as the pinch closes, and forwards as
	 * the pinch opens. Movement is linear and relative to the forwardDirection of the camera.
	 *
	 * This method is invoked repeatedly during a pinching gesture.
	 *
	 * Note that the pinching does not zoom the camera, although the visual effect is
	 * very similar. For this application, moving the camera is more flexible and useful
	 * than zooming. But other application might prefer to use the pinch gesture scale
	 * to modify the uniformScale or fieldOfView properties of the camera, to perform
	 * a true zooming effect.
	 */
	void						moveCameraBy( GLfloat aMovement );

	/**
	 * Stop moving the camera using the feedback from a UIPinchGestureRecognizer.
	 *
	 * This method is invoked once at the end of each pinch gesture.
	 * This method does nothing.
	 */
	void						stopMovingCamera();

	/**
	 * Start panning the camera using the feedback from a UIPanGestureRecognizer.
	 *
	 * This method is invoked once at the beginning of each double-finger pan gesture.
	 * The current orientation of the camera is cached. Subsequent invocations of the
	 * panCameraBy: method will move the camera relative to this starting orientation.
	 */
	void						startPanningCamera();

	/**
	 * Pans the camera using the feedback from a UIPanGestureRecognizer.
	 *
	 * Each component of the specified movement has a value of +/-1 if the user drags two
	 * fingers completely across the width or height of the CC3Layer, or a proportionally
	 * smaller value for shorter drags. The value changes as the panning gesture continues.
	 * At any time, it represents the movement from the initial position when the gesture
	 * began, and the startPanningCamera method was invoked. The movement does not represent
	 * a delta movement from the previous invocation of this method.
	 *
	 * This method is invoked repeatedly during a double-finger panning gesture.
	 */
	void						panCameraBy( const CCPoint& aMovement );

	/**
	 * Stop panning the camera using the feedback from a UIPanGestureRecognizer.
	 *
	 * This method is invoked once at the end of each double-finger pan gesture.
	 * This method does nothing.
	 */
	void						stopPanningCamera();

	/**
	 * Start dragging whatever object is below the touch point of this gesture.
	 *
	 * This method is invoked once at the beginning of each single-finger gesture.
	 * This method invokes the pickNodeFromTapAt: method to pick the node under the
	 * gesture, and cache that node. If that node is either of the two rotating cubes,
	 * subsequent invocations of the dragBy:atVelocity: method will spin that node.
	 */
	void						startDraggingAt( const CCPoint& touchPoint );

	/**
	 * Dragging whatever object was below the initial touch point of this gesture.
	 *
	 * If the selected node is either of the spinning cubes, spin it based on the
	 * specified velocity,
	 * 
	 * Each component of the specified movement has a value of +/-1 if the user drags one
	 * finger completely across the width or height of the CC3Layer, or a proportionally
	 * smaller value for shorter drags. The value changes as the panning gesture continues.
	 * At any time, it represents the movement from the initial position when the gesture
	 * began, and the startDraggingAt: method was invoked. The movement does not represent
	 * a delta movement from the previous invocation of this method.
	 * 
	 * Each component of the specified velocity is also normalized to the CC3Layer, so that
	 * a steady drag completely across the layer taking one second would have a value of
	 * +/-1 in the X or Y components.
	 *
	 * This method is invoked repeatedly during a single-finger panning gesture.
	 */
	void						dragBy( const CCPoint& aMovement, const CCPoint& aVelocity );

	/**
	 * Stop dragging whatever object was below the initial touch point of this gesture.
	 *
	 * This method is invoked once at the end of each single-finger pan gesture.
	 * This method simply clears the cached selected node.
	 */
	void						stopDragging();

	/**
	 * Add the initial content to the scene.
	 *
	 * Once the scene is displayed and running, additional content is added asynchronously in
	 * the addSceneContentAsynchronously method, which is invoked on a background thread by the
	 * CC3Backgrounder singleton.
	 */
	void						initializeScene();

	/**
	 * Adds additional scene content dynamically and asynchronously.
	 *
	 * This method is invoked from a code block that is run on a background thread by the 
	 * CC3Backgrounder singleton. It adds content dynamically and asynchronously after
	 * rendering has begun on the rendering thread.
	 *
	 * To emphasize that the loading is happening on a background thread while the existing scene
	 * is running, this method takes a small pause before loading each model. This pause is purely
	 * for dramatic effect for the purposes of this demo app. Pauses are NOT required before normal
	 * background model loading.
	 *
	 * Certain assets, notably shader programs, will cause short, but unavoidable, delays in the
	 * rendering of the scene, because certain finalization steps from shader compilation occur on
	 * the main thread. Shaders and certain other critical assets are pre-loaded in the preloadAssets
	 * method, which is invoked prior to the opening of this scene.
	 */
	void						addSceneContentAsynchronously();

	/** 
	 * When loading in the background, periodically pause the loading to phase the scene in over time.
	 * We put an explicit test here, because if the CC3Backgrounder shouldRunTasksOnRequestingThread
	 * property is set to YES, the addSceneContentAsynchronously method will be run in the foreground, 
	 * and we don't want to add any unncessary delays in that case.
	 */
	void						pauseDramatically();

	/**
	 * Invoked by the customized initializeScene to set up any initial state for
	 * this customized scene. This is broken into a separate method so that the
	 * initializeScene method can focus on loading the artifacts of the 3D scene.
	 */
	void						initCustomState();

	/**
	 * Pre-loads certain assets, such as shader programs, and certain textures, prior to the
	 * scene being displayed.
	 *
	 * Much of the scene is loaded on a background thread, while the scene is visible. However,
	 * the handling of some assets on the background thread can interrupt the main rendering thread.
	 *
	 * The GL drivers often leave the final stages of shader compilation and configuration until
	 * the first time the shader program is used to render an object. This can often introduce a
	 * short, unwanted pause if the shader program is loaded while the scene is running.
	 *
	 * Unfortunately, although resources such as models, textures, and shader programs can be loaded
	 * on a background thread, the final stages of shader programs compilation must be performed on
	 * the primary rendering thread. Because of this, the only way to avoid an unwanted pause while
	 * a shader program compilation is finalized is to therefore perform all shader program loading
	 * prior to the scene being displayed, including shader programs that may not be required until
	 * additional content is loaded later in the scene on a background thread.
	 *
	 * In order to ensure that the shader programs will be available when the models are loaded
	 * at a later point in the scene (usually via background loading), the cache must be configured
	 * to retain the loaded shader programs even though they will not immediately be used to display
	 * any models. This is done by turning on the value of the class-side isPreloading property.
	 *
	 * In addition, the automatic creation of mipmaps on larger textures, particularly cube-map 
	 * textures (which require a set of six mipmaps), can cause excessive work for the GPU in
	 * the background, which can spill over into a delay on the primary rendering thread.
	 *
	 * As a result, a large cube-map texture is loaded here and cached, for later access once
	 * the model that uses it is loaded in the background.
	 */
	void						preloadAssets();
	/** Various options for configuring interesting camera behaviours. */
	void						configureCamera();

	/** Configure the lighting. */
	void						configureLighting();

	/**
	 * Configures the specified node and all its descendants for use in the scene, and then fades
	 * them in over the specified duration, in seconds. Specifying zero for the duration will 
	 * instantly materialize the node without employing any fading.
	 *
	 * This scene is highly complex, and it helps to configure the nodes within it in a standardized
	 * manner, including whether we use VBO's to manage the vertices, whether the vertices need to
	 * also be retained in main memory, whether bounding volumes are required, and to force early
	 * selection of shaders for use with the nodes.
	 *
	 * The specified node can be the root of an arbitrarily complex node tree, and the behaviour
	 * applied in this method is propagated to all descendant nodes of the specified node, and the
	 * materialization fading will be applied to the entire node tree. The specified node can even
	 * be the entire scene itself.
	 */
	void						configureForScene( CC3Node* aNode, GLfloat duration );

	/** 
	 * Creates a clear-blue-sky backdrop. Or install a textured backdrop by uncommenting the 
	 * 2nd & 3rd lines of this method. See the notes for the backdrop property for more info.
	 */
	void						addBackdrop();

	/**
	 * Add a large circular grass-covered ground to give everything perspective.
	 * The ground is tessellated into many smaller faces to improve realism of spotlight.
	 */
	void						addGround();

	/**
	 * Adds a large rectangular orange ring floating above the ground. This ring is created from a plane
	 * using a texture that combines transparency and opacity. It demonstrates the use of transparency in
	 * textures. You can see through the transparent areas to the scene behind the texture. The texture
	 * as a whole fades in and out periodically, and rotates around the vertical (Y) axis.
	 *
	 * The type of blending function used to blend the transparent/translucent areas of the texture
	 * with the object behind is set automatically, and is influenced by the opacity of the object,
	 * whether the texture contains an alpha channel, and whether the color channels of the texture
	 * have been pre-multiplied by the alpha channel in the texture. Here we provide the option to
	 * demonstrate textures with either pre-multiplied content or non-pre-multiplied content, and
	 * the resulting blending function is logged to help you understand the difference.
	 *
	 * The non-premultiplied alpha texture is a PNG file with the special file-extension PPNG. 
	 * This is a normal PNG file, but the renamed extension will stop Xcode from modifying the file
	 * to pre-multiply the alpha during app building. The PPNG file is loaded with a custom image
	 * loader, again, to avoid iOS pre-multiplying the texture content during image loading. 
	 * This allows the texture to appear exactly as it was created. This is an important feature
	 * when loading textures that contain custom content, such as normal-maps, light-maps, shininess,
	 * weightings, etc. See the notes for the CC3STBImage useForFileExtensions property to learn 
	 * more about these special file extensions.
	 *
	 * As the ring rotates, both sides are visible. This is because the shouldCullBackFaces property is
	 * set to NO, so that both sides of each face are rendered. 
	 *
	 * A border is drawn around the bounding box of the mesh to highlight the extent of the
	 * transparency in the texture.
	 */
	void						addFloatingRing();

	/**
	 * Loads a POD file containing a semi-transparent beach ball
	 * sporting multiple materials, exported from Blender.
	 */
	void						addBeachBall();

	/**
	 * Adds a rotating globe that is created programatically from a prametric sphere,
	 * and is covered with a rectangular texture containing a cylindrical projection
	 * (typical of earth maps taken from space).
	 */
	void						addGlobe();

	/**
	 * Adds a die cube that can be rotated by the user touching it and then swiping in any
	 * direction. The die cube rotates in the direction of the swipe, at a speed proportional
	 * to the speed and length of the swipe, and then steadily slows down over time.
	 *
	 * While the user is touching the cube and moving the finger, the die cube is rotated
	 * under direct finger motion. Once the finger is lifted, the die cube spins in a
	 * freewheel fashion, and slows down over time due to friction.
	 *
	 * This die cube does not use a CCAction to rotate. Instead, a custom SpinningNode class
	 * replaces the node loaded from the POD file. This custom class spins by adjusting its
	 * rotational state on each update pass. It contains a spinSpeed property to indicate how
	 * fast it is currently spinning, and a friction property to adjust the spinSpeed on each
	 * update.
	 *
	 * To handle the behaviour of the node while it is freewheeling, we create it as a
	 * specialized subclass. Since this node is loaded from a POD file, one way to do this
	 * is to load the POD class and then copy it to the subclass we want. That is done here.
	 *
	 * To rotate a node using changes in rotation using the rotateBy... family of methods,
	 * as is done to this node, does NOT requre a specialized class. This specialized class
	 * is required to handle the freewheeling and friction nature of the behaviour after the
	 * rotation has begun.
	 *
	 * The die cube POD file was created from a Blender model available from the Blender
	 * "Two dice" modeling tutorial available online at:
	 * http://wiki.blender.org/index.php/Doc:Tutorials/Modeling/Two_dice
	 */
	void						addDieCube();

	/**
	* Adds a parametric textured cube that rotates by swiping, similar to the die cube.
	*
	* This is a single box mesh (not constructed from six separate plane meshes), and is
	* wrapped by a single texture, that wraps around all six sides of the cube. The texture
	* must be constructed to do this. Have a look at the BoxTexture.png texture file to
	* understand how the texture is wrapped to the different sides.
	*/
	void						addTexturedCube();

	/** 
	 * Adds a large textured teapot and a small multicolored teapot orbiting it.
	 * 
	 * When running with GLSL shaders under OpenGL ES 2.0 on iOS, or OpenGL on OSX, the textured
	 * teapot reflects the surrounding environment dynamically. This is performed by adding a
	 * environmental cube-map texture to the teapot. A cube-map texture actually consists of six
	 * textures, each representing a view of the scene from one of the six scene axes. The reflection
	 * texture is updated each frame (see the generateTeapotEnvironmentMapWithVisitor method), so
	 * the teapot reflects the dynamic scene. As objects move around the scene, they are reflected
	 * in the teapot.
	 *
	 * The default program matcher assigns the GLSL shaders CC3Texturable.vsh and
	 * CC3SingleTextureReflect.fsh shaders to the reflective teapot.
	 *
	 * The textured teapot actually has two textures. The first is the reflective cube-map, and the
	 * second can provide an optional surface material effect (in this case brushed metal), that
	 * blends with the reflection, to more realistically mimic a non-silvered reflective material.
	 * The reflectivity property of the material covering the teapot adjusts the blend between the
	 * reflective and material textures, and can be used to control how reflective the surface is.
	 * For demonstative effect, the reflectivity property is set to the maximum value of 1.0, making
	 * the material fully reflective (like a mirror or chrome), and none of the brushed metal texture
	 * shows through. You can reduce the reflectivity value to dull the reflection and show more of
	 * the brushed metal surface.
	 */
	void						addTeapotAndSatellite();

	/**
	 * When running with GLSL shaders under OpenGL ES 2.0 on iOS, or OpenGL on OSX, adds a large skybox
	 * surrounding the scene, using a cube-map texture. A cube-map texture actually consists of six
	 * textures, each representing a view of the scene from one of the six scene axes. As a convenience,
	 * the six textures are loaded using a file-name pattern.
	 *
	 * In this example, the six cube-map textures include markers to illustrate which texture is which.
	 *
	 * Cube maps can also be used to render environmental reflections on objects. This skybox is reflected
	 * into the reflective runner added in the addSkinnedRunners method. The runner will reflect this skybox
	 * texture even if the skybox itself is not added.
	 */
	void						addSkyBox();

	/**
	 * Adds a parametric textured wall that can be touched to slide it into the path of the
	 * rainbow teapot as it orbits the textured teapot. Once in the path of the teapot, it
	 * causes the teapot to bounce off it and reverse path, demonstating collision detection.
	 */
	void						addBrickWall();

	/** Loads a POD file containing an animated robot arm, a camera, and an animated light. */
	void						addRobot();

	/**
	 * Add small red, green and blue teapots to mark the X, Y & Z axes respectively.
	 * The teapots appear at location 100.0 on each of the axes.
	 */
	void						addAxisMarkers();

	/**
	* Adds a small white teapot that will be used to indicate the current position of the light
	* that illuminates the scene. The light is animated and moves up and down according to
	* animation data from the POD file, and the white teapot tracks its location (actually its
	* direction, since it is a directional light).
	*/
	void						addLightMarker();

	/**
	* Adds a label created from bitmapped font loaded from a font confguration file.
	*
	* The CylinderLabel class is a custom class that further bends the text around the arc of
	* a circle whose center is behind the text. The effect is like a marquee on a round tower.
	* This example demonstrates both the use of bitmapped text labels, and the ability to
	* manipulate the locations of vertices programmatically.
	*/
	void						addBitmapLabel();

	/**
	 * Add a label attached to the robot arm. This is created using a Cocos2D label object wrapped
	 * in a CC3Billboard to turn it into a 3D object.
	 *
	 * Unfortunately, this label will not play nicely with the fog, because it contains transparent parts
	 * that should be discarded by the fragment shader so that the deptth component is not written to the
	 * depth buffer. This would allow the fog to show through (see the cylindrical text example in this 
	 * demo for an example of how that works. But since this is a Cocos2D component, it would require
	 * changing the Cocos2D shaders to get it to work. There's no point in doing that, because it would
	 * be better to simply use 3D text instead of a Cocos2D text component.
	 */
	void						addProjectedLabel();

	void						addWoodenSign();

	/**
	 * Adds a bump-mapped floating purple head to the scene, suitable for running under either
	 * OpenGL ES 1.1 or OpenGL ES 2.0.
	 *
	 * Bump-mapping under OpenGL ES 1.1 is much more limited than the techniques available under
	 * OpenGL ES 2.0. This example demonstrates the technique for OpenGL ES 1.1. Under OpenGL ES 1.1,
	 * the normals contained in the bump-mapping texture must be specified in model-space coordinates.
	 *
	 * This example also runs correctly under OpenGL ES 2.0, but is not the recommended technique.
	 * Under OpenGL ES 2.0, your model should include vertex tangent content, and the normals in
	 * your texture should be specified in tangent-space (which is much easier). When using vertex
	 * tangents, none of the texture unit configuration below is required, nor is the light tracker
	 * required, as the GLSL shader code handles the texture lookups and normal mapping directly.
	 * See the addEtchedMask method for an example of using vertex tangent content.
	 *
	 * Bump-mapping works by applying a texture that contains normal data instead of colors. This allows
	 * us to apply a different normal to every pixel of the texture, instead of only at the vertices.
	 * This allows us to simulate 3D surface at a resolution much higher than the vertices permit.
	 * 
	 * The floating head has much higher 3D definitional resolution that provided by the
	 * relatively low vertex count mesh. The mesh only contains 153 vertices.
	 *
	 * You'll notice that the light shadowing changes correctly as the light moves up and
	 * down, and as you move the camera around, which causes the head to rotate to follow you.
	 */
	void						addFloatingHead();

	/**
	 * Loads a POD file containing the Cocos3D mascot, and creates a copy of it so that we have
	 * two mascots. One mascot always stares back at the camera, regardless of where the camera
	 * moves to. The other is distracted by the rainbow teapot and its gaze follows the teapot
	 * as the rainbow teapot moves.
	 *
	 * The Cocos2D/Cocos3D mascot model was created by Alexandru Barbulescu, and used by permission.
	 */
	void						addMascots();

	/**
	* Adds a sun in the sky, in the form of a standard Cocos2D particle emitter,
	* held in the 3D scene by a CC3Billboard. The sun is a very large particle
	* emitter, and you should notice a drop in frame rate when it is visible.
	*/
	void						addSun();

	/**
	 * Adds a spotlight to the camera. The spotlight is initially turned off, but will be turned
	 * on when the sun is turned off. The spotlight has a focused beam and the intensity of the
	 * light attenuates with distance from the light.
	 */
	void						addSpotlight();

	/** 
	 * Adds light probes to the scene.
	 *
	 * Illuminating models with light probes is an alternate to using individual lights. Light probes 
	 * contain a texture (usually a cube-map texture) that defines the lighting characteristics of an
	 * area of the scene. Within a GLSL shader, the vertex or fragment normal is used to pick a light 
	 * intensity from the texture. Light probes often capture lighting nuances and detail that can be 
	 * difficult to replicate with individual lights, and can improve performance, as they require 
	 * substantially less calculation within a shader.
	 * 
	 * Light probes are not available when using OpenGL ES 1.1.
	 */
	void						addLightProbes();

	/**
	* Adds fog to the scene. The fog is initially turned off, but will be turned on when
	* the sun button is toggled. The fog cycles in color between bluish and reddish tones.
	* Under OpenGL ES 1.1, fog is configured in GL engine. Under OpenGL ES 2.0 or OpenGL OSX,
	* fog provided using shader post-processing.
	*/
	void						addFog();

	/**
	 * Adds a mallet that moves back and forth, alternately hammering two anvils.
	 * The mallet's mesh employs vertex skinning and an animated bone skeleton to
	 * simulate smooth motion and realistic flexibility.
	 */
	void						addSkinnedMallet();

	/**
	 * Adds two running men to the scene. The men run endless laps around the scene. The men's meshes
	 * employ vertex skinning and an animated bone skeleton to simulate smooth motion and realistic
	 * joint flexibility. Under a programmable rendering pipeline, the smaller man also sports a
	 * reflective skin that reflects the environment, using a static cube-map texture. 
	 *
	 * In contrast to the reflective teapot in the addTeapotAndSatellite method, the reflective runner
	 * here has a static cube-map texture. Such a texture is typically generated once, usually at
	 * development time, and does not dynamically update as the scene contents change. The benefit
	 * to using a static environment map is that it does not required repeatitive regeneration, and
	 * is therefore much more efficient than a fully dynamic enviornment map, such as used on the
	 * reflective teapot.
	 *
	 * Cube maps can also be used to draw skyboxes. To see the environment that is being reflected
	 * into the reflective runner, uncomment the addSkyBox invocation in the initializeScene method.
	 */
	void						addSkinnedRunners();

	/**
	 * Adds a television showing the view from the runner camera
	 * This demonstrates dynamic rendering-to-texture capabilities.
	 *
	 * The television model POD file was created by exporting from a low-poly (decimated) version
	 * of a Blender model created by Blend Swap artist "nhumrod", and downloaded from Blend Swap
	 * at http://www.blendswap.com/blends/view/63306 under a CreativeCommons Zero license.
	 */
	void						addTelevision();

	/** Convenience property that returns the rendering surface of the TV, cast to the correct class. */
	CC3GLFramebuffer*			getTVSurface();

	/**
	 * Adds post-rendering image processing capabilities.
	 *
	 * Adds an off-screen framebuffer surface backed by a texture to hold the color and a texture
	 * to hold the depth content. The scene can be rendered to this surface, and, using GLSL shaders,
	 * the textures attached to the surface can be processed by the shaders associated with a
	 * special post-processing node when drawing one or the other of the surface's textures to the
	 * view's rendering surface.
	 *
	 * If we choose to render the color texture, we can process the values to perform visual
	 * operations such as grayscale or blurring. If we choose to render the depth texture, we
	 * can get a visualization of the values held in the depth buffer.
	 *
	 * We want the surface that we create to match the dimensions and characteristics of the view,
	 * and we want it to automatically adjust if the view dimensions change. To do that, we construct
	 * the surface with the same size as the view's surface, and format the textures to be compatible
	 * with the format of the view's surface. And then we register this new surface with the surface
	 * manager of the CC3Layer to have it automatically update the dimensions of the textures whenever
	 * the dimensions of the CC3Layer change.
	 *
	 * Since this method accesses the view's surface manager, it must be invoked after the
	 * view has been created. This method is invoked from the onOpen method of this class,
	 * instead of from the initializeScene method.
	 */
	void						addPostProcessing();

	/**
	 * Adds a platform of 3D point particles to the scene, laid out in a grid, and hanging over
	 * the back part of the ground. Each particle is displayed in a different color, and the
	 * entire platform rotates.
	 *
	 * Each particle has a normal vector so that it interacts with the light sources.
	 * Move the camera around to see the difference between when the camera is looking at the
	 * side of a particle that is facing towards a light source and when the camera is looking
	 * at the side of a particle that is facing away from a light source.
	 *
	 * You can play with the settings below to understand how particles behave.
	 */
	void						addPointParticles();

	/**
	 * Adds a platform of 3D mesh particles to the scene, laid out in a grid, and hanging over
	 * the back part of the ground. Each particle is displayed in a different color, and the
	 * entire platform can be rotated.
	 *
	 * Each particle is a small mesh cube. You can play with the settings below to understand
	 * how mesh particles behave.
	 */
	void						addMeshParticles();

	/**
	 * Adds a point particle emitter, that emits particles as if from a hose, to the end of the robot arm.
	 * The emitter is started and paused by touching the robot arm.
	 */
	void						addPointHose();

	/**
	 * Adds a mesh particle emitter, that emits particles as if from a hose, to the end of the robot arm.
	 * The emitter is started and paused by touching the robot arm.
	 */
	void						addMeshHose();

	/**
	 * Adds a floating mask that uses a PowerVR PFX file to define a GLSL shader program for the
	 * material. The material in the POD file references a PFX effect found in a PFX resource file.
	 * The PFX effect applies two textures to the mask.
	 * 
	 * When running under OpenGL ES 2.0, specialized shaders defined in the PFX effect render the
	 * second texture as an environment reflection.
	 *
	 * When running under OpenGL ES 1.1, only the second texture is visible, due to the default
	 * multi-texturing configuration. Under OpenGL ES 1.1, further texture unit configuration
	 * could be applied to allow the two textures to be combined in a more realistic manner.
	 *
	 * This example also demonstrates the ability to define within a shader a customized uniform
	 * variable, that does not have a semantic mapping to content  within the environment, and have
	 * the application set the value of such a uniform variable directly.
	 */
	void						addReflectiveMask();
		
	/**
	 * Adds a floating mask that uses two textures to create a tangent-space bump-mapped surface
	 * when running under OpenGL ES 2.0.
	 *
	 * When running under OpenGL ES 1.1, only the second (base) texture is visible, due to default
	 * multi-texturing configuration.
	 *
	 * Two techniques for applying bump-mapping GLSL shaders are provided here, and each can be
	 * selected by selectively commenting out code in this method. By default, this method applies
	 * the bump-map and visible textures to the mask and allows the default tanget-space bump-map
	 * shaders to be applied to the mask.
	 *
	 * Alternately, you can apply the textures and specific shaders to the mask by applying a
	 * PowerVR PFX file to determine apply the textures and shaders. The shaders assigned in the
	 * PFX file are different than the default bump-map shaders, and the mask will look different.
	 */
	void						addEtchedMask();

	/**
	 * Adds a dragon flying in a circular path above the scene.
	 *
	 * When first seen, the dragon is flying by flapping its wings. Touching the dragon causes it
	 * to start gliding. Touching it again causes it to go back to flapping. The transition between
	 * flapping and gliding occurs smoothly, regardless of when the dragon is touched.
	 *
	 * Flapping and gliding are handled by two separate tracks of animation, and the transition
	 * is done by animating the relative blending weights beween the two tracks by using an
	 * animation cross-fade action. Animated cross-fading ensures a smooth transition between
	 * the distinct animations, regardless of where in each animation cycle it is started.
	 * Try touching the dragon when its wings are at the top of the flap, or the bottom of the
	 * flap. Either way, the transition to the gliding animation is smoothly blended.
	 *
	 * The dragon model was created in Blender by Aleksandra Sebastian, and is available on
	 * Blend Swap at http://www.blendswap.com/blends/view/67196. It is used here under a
	 * Creative Commons Attribution 3.0 CC-BY license, requiring attribution to the author.
	 * All animation was added to the model after acquisition. The animated and modified
	 * Blender model is available in the Models folder of the Cocos3D distribution.
	 * The dragon POD file was created by exporting directly to POD from within Blender.
	 */
	void						addDragon() ;

	/**
	 * Adds a temporary fiery explosion on top of the specified node, using a Cocos2D
	 * CCParticleSystem. The explosion is set to a short duration, and when the particle
	 * system has exhausted, the CC3ParticleSystem node along with the CCParticleSystem
	 * billboard it contains are automatically removed from the 3D scene.
	 */
	void						addExplosionTo( CC3Node* aNode );

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
	void						drawSceneContentWithVisitor( CC3NodeDrawingVisitor* visitor ) ;

	/**
	 * When drawing an environment map, don't bother with shadows, avoid all the post-rendering
	 * processing, don't redraw the TV. And avoid an infinitely recursive issue where rendering
	 * the scene for the texture triggers a recursive nested scene render!
	 */
	void						drawSceneContentForEnvironmentMapWithVisitor( CC3NodeDrawingVisitor* visitor );

	/**
	 * Draws the scene from the runners' camera perspective to the TV screen.
	 *
	 * The drawing is performed by a dedicated drawing visitor that contains its own camera
	 * in the same location as the camera loaded with the runners. A dedicated camera is used
	 * because the aspect of the TV screen (HDTV) is different than the main scene view.
	 */
	void						drawToTVScreen();

	/**
	 * Adds a small picture-in-picture window in the TV screen by directly changing the TV screen texture.
	 *
	 * This is done by copying a rectangle of pixels from the rendering surface used to render
	 * the TV image, and then pasting them into a different location in the surface. Along the
	 * way, a border is added to the PiP image by directly setting pixel values.
	 */
	void						pictureInPicture();

	/** 
	 * Adds a border around the specified image array, which is of the specified rectangular size.
	 *
	 * Content in the specified image array is ordered from left to right across each row of
	 * pixels, starting the row at the bottom, and ending at the row at the top.
	 */
	void						addBorderToImage( ccColor4B* colorArray, const CC3IntSize& imgSize );

	/** Returns whether post-processing of the scene view is active. */
	bool						isPostProcessing();

	/** Returns the appropriate full-screen rendering node for the current lighting conditions. */
	CC3MeshNode*				getPostProcessingNode();

	/** 
	 * If we're not already in the middle of generating an environment map, and the reflective metal
	 * teapot is visible, generate an environment-map cube-map texture for it by taking snapshots of
	 * the scene in all six axis directions from its position. We don't want the teapot to
	 * self-reflect, so we make it invisible while we are rendering the scene from the teapot's center.
	 */
	void						generateTeapotEnvironmentMapWithVisitor( CC3NodeDrawingVisitor* visitor );

	/**
	 * This method is invoked when the scene is opened within the layer (or when the
	 * scene is added to an existing running layer). We can use this method to perform
	 * set-up that depends on the layer being attached and sized, such as camera actions
	 * that require the dimensions of the viewport to be established.
	 *
	 * During development, we can use this opportunity to move the camera to view the
	 * entire scene, or some section of the scene, in order to troubleshoot the scene.
	 */
	void						onOpen();

	/** 
	 * Called periodically as part of the CCLayer scheduled update mechanism.
	 * This is where model objects are updated.
	 *
	 * For this scene, the camera direction and location are updated
	 * under control of the user interface, and the location of the white teapot that
	 * indicates the direction of the light is updated as the light moves. All other motion
	 * in the scene is handled by CCActions.
	 */
	void						updateBeforeTransform( CC3NodeUpdatingVisitor* visitor );

	/** After all the nodes have been updated, check for collisions. */
	void						updateAfterTransform( CC3NodeUpdatingVisitor* visitor );

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
	void						checkForCollisions();

	/** Update the location and direction of looking of the 3D camera */
	void						updateCameraFromControls( GLfloat dt );

	/**
	 * When the user hits the switch-camera-target button, cycle through a series of four
	 * different camera targets. The actual movement of the camera to home in on a new target
	 * is handled by a CCActionInterval, so that the movement appears smooth and animated.
	 */
	void						switchCameraTarget();

	/** Create a landing craft and populate it with an army of robots. */
	void						invadeWithRobotArmy();

	/** Create a landing craft and populate it with an army of teapots. */
	void						invadeWithTeapotArmy();

	/**
	 * Invade with multiple copies of the specified template node. Instantiates a landing
	 * craft and populates it with an army copied from the specified template node.
	 */
	void						invadeWithArmyOf( CC3Node* invaderTemplate );
	
	/**
	 * Rotates the specified spinning node by setting its rotation axis
	 * and spin speed from the specified 2D drag velocity.
	 */
	void						rotate( SpinningNode* aNode, const CCPoint& swipeVelocity );

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
	void						touchEvent( GLuint touchType, const CCPoint& touchPoint );

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
	void						rotate( SpinningNode* aNode, const CCPoint& touchPoint, GLfloat dt );

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
	void						nodeSelected( CC3Node* aNode, GLuint touchType, const CCPoint& touchPoint );
	/**
	 * Unproject the 2D touch point into a 3D global-coordinate ray running from the camera through
	 * the touched node. Find the node that is punctured by the ray, the location at which the ray
	 * punctures the node's bounding volume in the local coordinates of the node, and add a temporary
	 * visible marker at that local location that fades in and out, and then removes itself.
	 */
	void						markTouchPoint( const CCPoint& touchPoint, CC3Node* aNode );

	/**
	 * If the touched node is the ground, place a little orange teapot at the location
	 * on the ground corresponding to the touch-point. As the teapot is placed, we set off
	 * a fiery explosion using a 2D particle system for dramatic effect. This demonstrates
	 * the ability to drop objects into the 3D scene using touch events, along with the
	 * ability to add Cocos2D CCParticleSystems into the 3D scene.
	 */
	void						touchGroundAt( const CCPoint& touchPoint );

	/** If the node is the beach ball, toggle it between opaque and translucent. */
	void						touchBeachBallAt( const CCPoint& touchPoint );

	/** When the brick wall is touched, slide it back and forth to open or close it. */
	void						touchBrickWallAt( const CCPoint& touchPoint );

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
	void						switchWoodenSign();
	
	/** 
	 * Toggle the floating head between a detailed bump-mapped texture and a mesh-only texture,
	 * illustrating the use of bump-mapping to increase the visbible surface detail on a very
	 * low-poly mesh.
	 */
	void						toggleFloatingHeadConfiguration();
	/**
	 * Turn the TV on or off by toggle the image on the TV between a static test pattern card and a
	 * texture generated dynamically by rendering the scene from the runner's camera into a texture.
	 *
	 * To demonstrate extracting an iOS or OSX image from a render surface, every time we turn the
	 * TV off, we create a CGImageRef from the TV screen image, and save it to a JPEG file.
	 */
	void						toggleTelevision();

	/** 
	 * Extracts the image from the TV screen, converts it into an OS image,
	 * and saves it to a JPEG file in the Documents directory.
	 */
	void						saveTVImage();

	/** Toggle between the main scene camera and the camera running along with the runner. */
	void						toggleActiveCamera();

	/** Cycles the specified bitmapped label node through a selection of label strings. */
	void						cycleLabelOf( CC3BitmapLabelNode* bmLabel );

	/** Cycles through a variety of shadowing options for the specified node. */
	void						cycleShadowFor( CC3Node* aNode );

	/**
	 * When the dragon node is touched, cycle through several different animations, smoothly
	 * transitioning between the current and new animations by using animation cross-fading
	 * which blends the two animations together using animated blending weights.
	 */
	void						cycleDragonMotion();

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
	void						dragonTransitionToGliding();

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
	void						dragonTransitionToFlapping();

	/** Stop the CC3ActionAnimate action that is running the dragon's flapping animation. */
	void						dragonStopFlapping();

	/** Stop the CC3ActionAnimate action that is running the dragon's gliding animation. */
	void						dragonStopGliding();

protected:
	/// @Note: weakref
	CC3MashUpLayer*				_primaryCC3MashUpLayer;
	CCPoint						_playerDirectionControl;
	CCPoint						_playerLocationControl;
	CC3Vector					_cameraMoveStartLocation;
	CC3Vector					_cameraPanStartRotation;
	CC3PlaneNode*				_ground;
	CC3MeshNode*				_teapotWhite;
	CC3MeshNode*				_teapotTextured;
	PhysicsMeshNode*			_teapotSatellite;
	DoorMeshNode*				_brickWall;
	CC3Node*					_beachBall;
	CC3MeshNode*				_globe;
	SpinningNode*				_dieCube;
	SpinningNode*				_texCubeSpinner;
	CC3MeshNode*				_mascot;
	CC3Node*					_bumpMapLightTracker;
	CC3MeshNode*				_woodenSign;
	CC3MeshNode*				_floatingHead;
	CC3Texture*					_signTex;
	CC3TextureUnitTexture*		_stampTex;
	CC3TextureUnitTexture*		_embossedStampTex;
	CC3Texture*					_headTex;
	CC3TextureUnitTexture*		_headBumpTex;
	CC3Camera*					_robotCam;
	CC3Light*					_robotLamp;
	CC3Node*					_origCamTarget;
	CC3Node*					_camTarget;
	CC3Camera*					_runnerCam;
	CC3Light*					_runnerLamp;
	CC3EnvironmentMapTexture*	_envMapTex;
	CC3MeshNode*				_tvScreen;
	CC3Texture*					_tvTestCardTex;
	CC3Node*					_selectedNode;
	CC3NodeDrawingVisitor*		_tvDrawingVisitor;
	CC3GLFramebuffer*			_postProcSurface;
	CC3MeshNode*				_grayscaleNode;
	CC3MeshNode*				_depthImageNode;
	CC3Node*					_dragon;
	GLuint						_dragonGlideTrack;
	GLuint						_dragonFlapTrack;
	DragonMotion				_dragonMotion;
	CCPoint						_lastTouchEventPoint;
	struct timeval				_lastTouchEventTime;
	CameraZoomType				_cameraZoomType;
	LightingType				_lightingType;
	CC3Ray						_lastCameraOrientation;
	GLubyte						_bmLabelMessageIndex;
	bool						_isManagingShadows : 1;
	bool						_isTVOn : 1;
};

#endif
