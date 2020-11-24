/*
 * Cocos3D-X 1.0.0
 * Author: Bill Hollings
 * Copyright (c) 2010-2014 The Brenwill Workshop Ltd. All rights reserved.
 * http://www.brenwill.com
 *
 * Copyright (c) 2014-2015 Jason Wang
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
#ifndef _CCL_CC3MESHNODE_H_
#define _CCL_CC3MESHNODE_H_

NS_COCOS3D_BEGIN
class CC3Material;

/**
 * A CC3Node that draws a 3D mesh.
 * This class forms the base of all visible 3D mesh models in the 3D scene.
 *
 * CC3MeshNode is a type of CC3Node, and will often participate in a structural node assembly.
 * An instance can be the child of another node, and the mesh node itself can have child nodes.
 *
 * CC3MeshNodes encapsulate a CC3Mesh instance, and a CC3Material instance. The CC3Mesh instance
 * contains the mesh vertex content. The CC3Material instance describes the material and texture
 * properties covering the mesh, which can be affected by lighting conditions.
 *
 * If it is not explicitly set beforehand, the material will automatically be created and assigned 
 * to the mesh node when a texture is added to the mesh node through the texture property or the
 * addTexture: method, or if any of the material properties of the mesh node are set or accessed,
 * including color, opacity, ambientColor, diffuseColor, specularColor, emissionColor, blendFunc,
 * isOpaque, shouldUseLighting, or shouldDrawLowAlpha.
 *
 * There are a number of populateAs... parametric population methods available in the CC3MeshNode
 * (ParametricShapes) category extension. These methods can be used to populate the vertices of the
 * mesh contained in a new mesh node to create interesting and useful parametric shapes and surfaces.
 *
 * When this node is drawn, it delegates to the mesh instance to render the mesh vertices. 
 * Before drawing the mesh, it delegates to the material to configure the covering of the mesh.
 *
 * Each CC3MeshNode can have only one material. For large, complicated meshes that are 
 * covered by more than one material, the mesh must be broken into smaller meshes, each
 * of which is covered by a single material. These smaller sub-meshes are sometimes 
 * referred to as "vertex groups". Each such sub-mesh is then wrapped in its own CC3MeshNode
 * instance, along with the material that covers that sub-mesh.
 *
 * These CC3MeshNode instances can then be added as child nodes to a single parent CC3Node instance.
 * This parent CC3Node can then be moved, rotated and scaled, and all of its child nodes will
 * transform in sync. The assembly will behave and be seen as a single object.
 *
 * When the mesh is set in the mesh property, the CC3MeshNode instance creates and builds a
 * CC3NodeBoundingVolume instance from the mesh data, and sets it into its boundingVolume property. 
 *
 * When a copy is made of a CC3MeshNode instance using the copy method, a copy is made of the material,
 * but the mesh is simply assigned by reference, and is not copied. The result is that the the new and
 * original nodes will have different materials, but will share the same mesh. This design avoids
 * creating multiple copies of volumnious and static mesh data when creating copies of nodes.
 *
 * Normally, the front faces of a mesh are displayed, and the back faces are culled and not displayed.
 * You can change this behaviour if you need to be changing the values of the shouldCullFrontFaces and
 * shouldCullBackFaces properties. An example might be if you wanted to show the back-side of a planar
 * sign, or if you wanted to show the inside faces of a skybox.
 *
 * However, be aware that culling is a significant performance-improving technique. You should avoid
 * disabling backface culling except where specifically needed for visual effect. And when you do,
 * if you only need the back faces, turn on front face culling for that mesh by setting the
 * shouldCullFrontFaces property to YES.
 */
class CC3ShaderContext;
class CC3ShaderProgram;
class CC3Texture;
class CC3Mesh;

class CC3MeshNode : public CC3LocalContentNode
{
	DECLARE_SUPER( CC3LocalContentNode );
public:
	CC3MeshNode();
	virtual ~CC3MeshNode();

public:
	virtual void				setName( const std::string& aName );
	/**
	 * The mesh that holds the vertex content for this mesh node.
	 *
	 * When this property is set, if this node has a boundingVolume, it is forced to rebuild itself,
	 * otherwise, if this node does not have a boundingVolume, a default bounding volume is created
	 * from the mesh. In addition, if the mesh does not have normals, the shouldUseLighting property
	 * of this node is set to NO, and if the mesh does not have texture coordinates, the texture
	 * property of this node is set to nil.
	 */
	virtual CC3Mesh*			getMesh();
	virtual void				setMesh( CC3Mesh* mesh );

	/**
	 * If a mesh does not yet exist, this method invokes the makeMesh method to create
	 * a suitable mesh, and sets it into the mesh property. Does nothing if this mesh
	 * node already has a mesh. Returns the mesh (existing or new).
	 *
	 * This method is invoked whenever a property is set that would affect the mesh.
	 * Usually, you will never need to invoke this method.
	 */
	virtual CC3Mesh*			ensureMesh();

	/**
	 * This template method creates a suitable mesh for this mesh node.
	 *
	 * This implementation invokes [CC3Mesh mesh], and returns the result.
	 * Subclasses may override to provide a different mesh.
	 *
	 * This method is invoked automatically by the ensureMesh method if a mesh is needed,
	 * but has not yet been established. Usually, you will never need to invoke this method.
	 */
	virtual CC3Mesh*			makeMesh();

	/**
	 * Returns whether the underlying vertex content has been loaded into GL engine vertex
	 * buffer objects. Vertex buffer objects are engaged via the createGLBuffers method.
	 */
	virtual bool				isUsingGLBuffers();

	/**
	 * The normal scaling method that is currently in use for this mesh node.
	 *
	 * This property differs from the normalScalingMethod. The normalScalingMethod is a settable
	 * property that is used to indicate the desired scaling method to be used for normals, and
	 * can include a setting of kCC3NormalScalingAutomatic, to allow the mesh node to resolve
	 * which method to use. This property returns that resolved value.
	 *
	 * If the mesh has vertex normals, this property will match the normalScalingMethod for values
	 * kCC3NormalScalingNone, kCC3NormalScalingRescale & kCC3NormalScalingNormalize. If the mesh
	 * does not contain vertex normals, this property will always return kCC3NormalScalingNone.
	 */
	CC3NormalScaling			getEffectiveNormalScalingMethod();

	/**
	 * The material covering this mesh node.
	 *
	 * When querying this property, if a material does not yet exist, this method invokes the 
	 * makeMaterial method to create a suitable material, and sets it into this property. 
	 */
	virtual void				setMaterial( CC3Material* material );
	virtual CC3Material*		getMaterial();

	/**
	 * If this value is set to YES, current lighting conditions (from either lights or light probes)
	 * will be taken into consideration when drawing colors and textures.
	 *
	 * If this value is set to NO, lighting conditions will be ignored when drawing colors and
	 * textures, and the emissionColor will be applied to the mesh surface, without regard to
	 * lighting. Blending will still occur, but the other material aspects, including ambientColor,
	 * diffuseColor, specularColor, and shininess will be ignored. This is useful for a cartoon
	 * effect, where you want a pure color, or the natural colors of the texture, to be included
	 * in blending calculations, without having to arrange lighting, or if you want those colors
	 * to be displayed in their natural values despite current lighting conditions.
	 *
	 * Be aware that the initial value of the emissionColor property is normally black. If you
	 * find your node disappears or turns black when you set this property to NO, try changing
	 * the value of the emissionColor property.
	 *
	 * Setting the value of this property sets the same property in the contained material.
	 * Reading the value of this property returns the value of the same property in the contained material.
	 *
	 * The initial value of this property is YES.
	 */
	virtual void				setShouldUseLighting( bool use );
	virtual bool				shouldUseLighting();

	/**
	 * If this value is set to YES, this mesh node will ignore the lights in the scene and will, instead,
	 * determine the lighting of the mesh node using textures held by light probes in the scene.
	 *
	 * This property only has effect if the shouldUseLighting property is set to YES.
	 *
	 * The initial value of this property is NO.
	 *
	 * See the notes of the CC3LightProbe class to learn more about using light probes to
	 * illuminate models within the scene.
	 */
	virtual void				setShouldUseLightProbes( bool use );
	virtual bool				shouldUseLightProbes();

	/**
	 * The ambient color of the material of this mesh node.
	 *
	 * Material color is initially set to kCC3DefaultMaterialColorAmbient.
	 * If this instance has no material, this property will return kCCC4FBlackTransparent.
	 *
	 * The value of this property is also affected by changes to the color and opacity
	 * properties. See the notes for those properties for more information.
	 */
	virtual void				setAmbientColor( const ccColor4F& color );
	virtual ccColor4F			getAmbientColor();

	/**
	 * The diffuse color of the material of this mesh node.
	 *
	 * Material color is initially set to kCC3DefaultMaterialColorDiffuse.
	 * If this instance has no material, this property will return kCCC4FBlackTransparent.
	 *
	 * The value of this property is also affected by changes to the color and opacity
	 * properties. See the notes for those properties for more information.
	 */
	virtual void				setDiffuseColor( const ccColor4F& color );
	virtual ccColor4F			getDiffuseColor();

	/**
	 * The specular color of the material of this mesh node.
	 *
	 * Material color is initially set to kCC3DefaultMaterialColorSpecular.
	 * If this instance has no material, this property will return kCCC4FBlackTransparent.
	 *
	 * The value of this property is also affected by changes to the opacity property.
	 * See the notes for the opacity property for more information.
	 */
	virtual void				setSpecularColor( const ccColor4F& color );
	virtual ccColor4F			getSpecularColor();

	/**
	 * The emission color of the material of this mesh node.
	 *
	 * Material color is initially set to kCC3DefaultMaterialColorEmission.
	 * If this instance has no material, this property will return kCCC4FBlackTransparent.
	 *
	 * The value of this property is also affected by changes to the opacity property.
	 * See the notes for the opacity property for more information.
	 */
	virtual void				setEmissionColor( const ccColor4F& color );
	virtual ccColor4F			getEmissionColor();
	
	/**
	 * The shininess of the material of this mesh node.
	 *
	 * The value of this property is clamped to between zero and kCC3MaximumMaterialShininess.
	 * The initial value of this property is kCC3DefaultMaterialShininess (zero).
	 */
	virtual void				setShininess( GLfloat shininess );
	virtual GLfloat				getShininess();

	/**
	 * The reflectivity of the material of this mesh node.
	 *
	 * This property can be used when the material is covered by an environmental reflection cube-map
	 * texture to indicate the weighting that should be applied to the reflection texture, relative to
	 * any other textures on the material. A value of zero indicates that the surface should be
	 * completely unreflective, and a value of one indicates that the surface is entirely reflective.
	 *
	 * This property requires a programmable pipeline and has no effect when running OpenGL ES 1.1.
	 *
	 * The value of this property is clamped to between zero and one.
	 * The initial value of this property is kCC3DefaultMaterialReflectivity (zero).
	 */
	virtual void				setReflectivity( GLfloat reflectivity );
	virtual GLfloat				getReflectivity();

	/**
	 * Creates and returns a suitable material for this mesh node.
	 *
	 * This method is invoked automatically by the material property accessor method if a material
	 * is needed, but has not yet been established.
	 *
	 * This implementation returns an instance of CC3Material initialized with the default 
	 * initializer method. Subclasses may override to provide a different material.
	 */
	virtual CC3Material*		makeMaterial();

	/**
	 * The GLSL shader program context containing the GLSL program (vertex & fragment shaders) 
	 * used to draw this node.
	 *
	 * A single CC3ShaderProgram object can be used by many nodes. The CC3ShaderContext
	 * instance in this property contains state and behaviour specific to the use of the shader
	 * program by this mesh node.
	 *
	 * Each shader program typically makes use of many uniform variables. In most, or many, cases,
	 * each uniform will have a semantic defined, and the content of the uniform will automatically
	 * be extracted from the environment, including from this mesh node itself. So, in most cases,
	 * once the semantic is defined, the application needs pay no further attention to the uniform.
	 *
	 * The shader context can be used to modify this standard semanitic-driven behaviour in two
	 * ways. This shader context can be used to assign a value to a specialized or custom shader
	 * uniform whose value is not derived semantically from the node or the environment, and it 
	 * can be used to override the value of an otherwise semantically-derived uniform, if needed.
	 *
	 * If this property is not set directly, it is automatically initialized to a new shader 
	 * context instance on first access (typically when the shaderProgram property is established,
	 * or a uniform override is added). Unless you have a need to set the value of this property
	 * directly, you can simply let it be managed automatically.
	 *
	 * This property is used only when running under OpenGL ES 2.
	 */
	virtual void				setShaderContext( CC3ShaderContext* context );
	virtual CC3ShaderContext*	getShaderContext();

	/**
	 * The GLSL program (vertex & fragment shaders) used to draw this node.
	 *
	 * The program is held in the shader context in the shaderContext property. This is a 
	 * convenience property that allows the shader program to be accessed from the shaderContext.
	 *
	 * Setting the value of this property will set the specified program into the context in the
	 * shaderContext property, creating a new shader context if necessary.
	 *
	 * As an alternative to setting this property directly, you can either access this property,
	 * or invoke the selectShaderProgram method (or let it be invoked automatically during the 
	 * first draw), to have an appropriate shader program automatically selected for use by this
	 * node, and assigned to this property,
	 *
	 * This property is used only when running under OpenGL ES 2.
	 */
	virtual void				setShaderProgram( CC3ShaderProgram* program );
	virtual CC3ShaderProgram*	getShaderProgram();

	/**
	 * Selects an appropriate shader program for this mesh node, and returns that shader program.
	 *
	 * When running under a programmable rendering pipeline, such as OpenGL ES 2.0 or OpenGL, all
	 * mesh nodes require shaders to be assigned. This can be done directly using the shaderProgram
	 * property. Or a shader program can be selected automatically based on the characteristics of
	 * the mesh node by invoking this method.
	 *
	 * Since all mesh nodes require shaders, if this method is not invoked, and a shader program 
	 * was not manually assigned via the shaderProgram property, a shaders will be automatically 
	 * assigned to each mesh node the first time it is rendered. The automatic selection is the 
	 * same, whether this method is invoked, or the selection is made lazily. However, if the 
	 * shaders must be loaded and compiled, there can be a noticable pause in drawing a mesh node
	 * for the first time if lazy assignment is used.
	 *
	 * Shader selection is driven by the characteristics of the mesh node and its material,
	 * including the number of textures, whether alpha testing is used, etc. If you change
	 * any of these characteristics that affect the shader selection, you can invoke the
	 * removeLocalShaders method to cause a different shader program to be selected, based
	 * on the new mesh node and material characteristics.
	 *
	 * Shader selection is handled by an implementation of the CC3ShaderMatcher held in the
	 * CC3ShaderProgram shaderMatcher class-side property. The application can therefore 
	 * customize shader program selection by establishing a custom instance in the 
	 * CC3ShaderProgram shaderMatcher class-side property
	 *
	 * This method differs from the selectShaders method in that this method does not
	 * propagate to any descendant nodes.
	 */
	virtual CC3ShaderProgram*	selectShaderProgram();

	/**
	 * Removes the shaders from this mesh node, allowing new shaders to be selected, either directly
	 * by subsequently invoking the selectShaderProgram method, or automatically the next time this
	 * mesh node is drawn.
	 *
	 * Shader selection is driven by the characteristics of the mesh node and its material,
	 * including the number of textures, whether alpha testing is used, etc. If you change
	 * any of these characteristics that affect the shader selection, you can invoke the
	 * removeLocalShaders method to cause a different shader program to be selected, based
	 * on the new mesh node and material characteristics.
	 *
	 * This method is equivalent to setting the shaderProgram property to nil.
	 *
	 * This method differs from the removeShaders method in that this method does not
	 * propagate to any descendant nodes.
	 */
	virtual void				removeLocalShaders();

	/**
	 * The color of this node, as described by the color of the material covering this node.
	 *
	 * Querying this property returns the value of the same property of the enclosed material.
	 *
	 * Setting this property sets the same property on the enclosed material, and on the mesh,
	 * if the shouldApplyOpacityAndColorToMeshContent property is set to YES, and also sets the
	 * same property on all descendant nodes.
	 */
	//CCColorRef				getColor();
	//void					setColor( const CCColorRef& color );

	/**
	 * The opacity of this node, as described by the opacity of the material covering this mesh.
	 *
	 * Querying this property returns the value of the same property of the enclosed material.
	 *
	 * Setting this property sets the same property on the enclosed material, and on the mesh,
	 * if the shouldApplyOpacityAndColorToMeshContent property is set to YES, and also sets the
	 * same property on all descendant nodes.
	 *
	 * See the notes for this property on CC3Material for more information on how this
	 * property interacts with the other material properties.
	 *
	 * Setting this property should be thought of as a convenient way to switch between the
	 * two most common types of blending combinations. For finer control of blending, set
	 * specific blending properties on the CC3Material instance directly, and avoid making
	 * changes to this property.
	 */
	virtual void				setOpacity( CCOpacity opacity );
	virtual CCOpacity			getOpacity();

	/**
	 * Indicates whether the material of this mesh node is opaque.
	 *
	 * If this node has a material, returns the value of the same property on the material,
	 * otherwise return YES.
	 *
	 * Setting this property sets the same property in the material and in all descendants.
	 *
	 * See the notes for this property on CC3Material for more information on how this
	 * property interacts with the other material properties.
	 *
	 * Setting this property should be thought of as a convenient way to switch between the
	 * two most common types of blending combinations. For finer control of blending, set
	 * specific blending properties on the CC3Material instance directly, and avoid making
	 * changes to this property.
	 */
	virtual void				setIsOpaque( bool opaque );
	virtual bool				isOpaque();

	/**
	 * Implementation of the CCBlendProtocol blendFunc property.
	 *
	 * This is a convenience property that gets and sets both the sourceBlend and
	 * destinationBlend properties of the material used by this node using a single
	 * structure. Changes to this property is also passed along to any child nodes.
	 * Querying this property returns {GL_ONE, GL_ZERO} if this node has no material.
	 */
	virtual void				setBlendFunc( ccBlendFunc func );
	virtual ccBlendFunc			getBlendFunc();

	/**
	 * Indicates whether alpha testing should be used to determine if pixels with
	 * lower alpha values should be drawn.
	 *
	 * Setting or reading the value of this property will set or return the value of the
	 * same property on the material covering this mesh.
	 *
	 * If the value of this property is set to YES, each pixel will be drawn regardless
	 * of the value of its alpha component. If the value of this property is set to NO,
	 * the value of the alpha component of each pixel will be compared against the value
	 * in the alphaTestReference property of the material, and only those pixel alpha
	 * values that are greater than that reference value will be drawn. You can set the
	 * value of the alphaTestReference property of the material to determine the cutoff
	 * level.
	 *
	 * The initial value of this property is YES, indicating that pixels with lower
	 * alpha values will be drawn.
	 * 
	 * For most situations, alpha testing is not necessary, and you can leave the value
	 * of this property set to YES. Alpha testing can sometimes be useful when drawing
	 * overlapping objects that each contain transparency, and it is not possible to rely
	 * only on drawing order and depth testing to mediate whether a pixel should be drawn.
	 */
	virtual void                setShouldDrawLowAlpha( bool draw );
	virtual bool                shouldDrawLowAlpha();

	/**
	 * Returns the number of textures covering this mesh, regardless of whether the
	 * textures were attached using the texture property or the addTexture: method.
	 */
	GLuint                      getTextureCount();

	/**
	 * When the material covering this mesh contains a single texture, this property
	 * references that texture. When multi-texturing is in use, and the material holds
	 * more than one texture, this property references the texture that will be processed
	 * by GL texture unit zero.
	 *
	 * If a material does not yet exist in this mesh node, a new material will be
	 * created and the texture will be attached to it.
	 * 
	 * Under iOS, during loading, textures are padded to dimensions of a power-of-two
	 * (POT) and, because vertical OpenGL coordinates are inverted relative to iOS
	 * view coordinates, most texture formats are loaded updside-down.
	 *
	 * To compensate, when a texture is attached to a mesh node, the texture coordinates
	 * of the mesh are automatically adjusted to correctly display the texture, taking
	 * into consideration POT padding and vertical orientation.
	 *
	 * When building for iOS, raw PNG and TGA images are pre-processed by Xcode to pre-multiply
	 * alpha, and to reorder the pixel component byte order, to optimize the image for the iOS
	 * platform. If you want to avoid this pre-processing for PNG or TGA files, for textures
	 * such as normal maps or lighting maps, that you don't want to be modified, you can prepend
	 * a 'p' to the file extension ("ppng" or "ptga") to cause Xcode to skip this pre-processing
	 * and to use a loader that does not pre-multiply the alpha. You can also use this for other
	 * file types as well. See the notes for the CC3STBImage useForFileExtensions class-side
	 * property for more info.
	 */
	virtual void				setTexture( CC3Texture* texture );
	virtual CC3Texture*			getTexture();

	/**
	 * In most situations, the material will use a single CC3Texture in the texture property.
	 * However, if multi-texturing is used, additional CC3Texture instances can be provided
	 * by adding them using this method.
	 *
	 * If a material does not yet exist in this mesh node, a new material will be
	 * created and the texture will be attached to it.
	 *
	 * When multiple textures are attached to a material, when drawing, the material will
	 * combine these textures together using configurations contained in the textureUnit
	 * property of each texture.
	 *
	 * As a consistency convenience, if the texture property has not yet been set directly,
	 * the first texture added using this method will appear in that property.
	 *
	 * Textures are processed by GL texture units in the order they are added to the material.
	 * The first texture added (or set directly into the texture property) will be processed
	 * by GL texture unit zero. Subsequent textures added with this method will be processed
	 * by subsequent texture units, in the order they were added.
	 *
	 * The maximum number of texture units available is platform dependent, but will be
	 * at least two. The maximum number of texture units available can be read from the
	 * CC3OpenGL.sharedGL.maxNumberOfTextureUnits property. If you attempt to add more than
	 * this number of textures to the material, the additional textures will be ignored,
	 * and an informational message to that fact will be logged.
	 *
	 * Under iOS, during loading, textures are padded to dimensions of a power-of-two
	 * (POT) and, because vertical OpenGL coordinates are inverted relative to iOS
	 * view coordinates, most texture formats are loaded updside-down.
	 *
	 * To compensate, when a texture is attached to a mesh node, the texture coordinates
	 * of the mesh are automatically adjusted to correctly display the texture, taking
	 * into consideration POT padding and vertical orientation.
	 *
	 * When building for iOS, raw PNG and TGA images are pre-processed by Xcode to pre-multiply
	 * alpha, and to reorder the pixel component byte order, to optimize the image for the iOS
	 * platform. If you want to avoid this pre-processing for PNG or TGA files, for textures
	 * such as normal maps or lighting maps, that you don't want to be modified, you can prepend
	 * a 'p' to the file extension ("ppng" or "ptga") to cause Xcode to skip this pre-processing
	 * and to use a loader that does not pre-multiply the alpha. You can also use this for other
	 * file types as well. See the notes for the CC3STBImage useForFileExtensions class-side
	 * property for more info.
	 */
	virtual void				addTexture( CC3Texture* texture );

	/** Removes all textures from the material covering this mesh. */
	virtual void				removeAllTextures();

	/**
	 * Returns the texture that will be processed by the texture unit with the specified
	 * index, which should be a number between zero, and one less than the value of the
	 * textureCount property.
	 *
	 * The value returned will be nil if this node has no material,
	 * or if that material has no textures.
	 *
	 * This method is a convenience. It simply delegates to the same method on the
	 * material covering this mesh node, creating the material first, if needed.
	 */
	virtual CC3Texture*			getTextureForTextureUnit( GLuint texUnit );

	/**
	 * Sets the texture that will be processed by the texture unit with the specified index,
	 * which should be a number between zero, and the value of the textureCount property.
	 * 
	 * If the specified index is less than the number of texture units added already, the
	 * specified texture will replace the one assigned to that texture unit. Otherwise, this
	 * implementation will invoke the addTexture: method to add the texture to this material.
	 *
	 * If the specified texture unit index is zero, the value of the texture property will
	 * be changed to the specified texture.
	 *
	 * If a material does not yet exist in this mesh node, a new material will be
	 * created and the texture will be attached to it.
	 * 
	 * Under iOS, during loading, textures are padded to dimensions of a power-of-two
	 * (POT) and, because vertical OpenGL coordinates are inverted relative to iOS
	 * view coordinates, most texture formats are loaded updside-down.
	 *
	 * To compensate, when a texture is attached to a mesh node, the texture coordinates
	 * of the mesh are automatically adjusted to correctly display the texture, taking
	 * into consideration POT padding and vertical orientation.
	 */
	virtual void				setTexture( CC3Texture* texture, GLuint texUnit );

	/**
	 * Indicates whether the texture coordinates of this mesh expects that the texture
	 * was flipped upside-down during texture loading.
	 * 
	 * The vertical axis of the coordinate system of OpenGL is inverted relative to the
	 * CoreGraphics view coordinate system. As a result, some texture file formats may be
	 * loaded upside down. Most common file formats, including JPG, PNG & PVR are loaded
	 * right-way up, but using proprietary texture formats developed for other platforms
	 * may result in textures being loaded upside-down.
	 *
	 * The value of this property is used in combination with the value of the 
	 * isUpsideDown property of a texture to determine whether the texture
	 * will be oriented correctly when displayed using these texture coordinates.
	 *
	 * When a texture or material is assigned to this mesh node, the value of this
	 * property is compared with the isUpsideDown property of the texture to
	 * automatically determine whether these texture coordinates need to be flipped
	 * vertically in order to display the texture correctly. If needed, the texture
	 * coordinates will be flipped automatically. As part of that inversion, the
	 * value of this property will also be flipped, to indicate that the texture
	 * coordinates are now aligned differently.
	 *
	 * If you need to adjust the value of this property, you sould do so before
	 * setting a texture or material into this mesh node.
	 *
	 * When multi-texturing is being used on this mesh, you can use the
	 * expectsVerticallyFlippedTexture:inTextureUnit: method for finer control
	 * of orienting textures for each texture unit.
	 *
	 * When multi-texturing is being used, setting this value of this property will
	 * use the expectsVerticallyFlippedTexture:inTextureUnit: method to set
	 * the same value for each texture unit.
	 *
	 * Reading the value of this property will return YES if the property-reading
	 * method expectsVerticallyFlippedTextureInTextureUnit: returns YES for
	 * any texture unit, otherwise this property will return NO.
	 * 
	 * The initial value of this property is set when the underlying mesh texture
	 * coordinates are built or loaded. See the same property on the CC3NodesResource
	 * class to understand how this property is set during mesh resource loading.
	 * 
	 * When building meshes programmatically, you should endeavour to design the
	 * mesh so that this property will be YES if you will be using vertically-flipped
	 * textures (all texture file formats except PVR). This avoids the texture
	 * coordinate having to be flipped automatically when a texture or material
	 * is assigned to this mesh node.
	 */
	virtual void				setExpectsVerticallyFlippedTextures( bool expect );
	virtual bool				expectsVerticallyFlippedTextures();

	/**
	 * Returns whether the texture coordinates for the specfied texture unit expects
	 * that the texture was flipped upside-down during texture loading.
	 * 
	 * The vertical axis of the coordinate system of OpenGL is inverted relative to
	 * the iOS view coordinate system. This results in textures from most file formats
	 * being oriented upside-down, relative to the OpenGL coordinate system. All file
	 * formats except PVR format will be oriented upside-down after loading.
	 *
	 * The value of this property is used in combination with the value of the 
	 * isUpsideDown property of a texture to determine whether the texture
	 * will be oriented correctly when displayed using these texture coordinates.
	 *
	 * When a texture or material is assigned to this mesh node, the value of this
	 * property is compared with the isUpsideDown property of the texture to
	 * automatically determine whether these texture coordinates need to be flipped
	 * vertically in order to display the texture correctly, and if needed, the
	 * texture coordinates will be flipped automatically. As part of that inversion,
	 * the value of this property will also be flipped, to indicate that the texture
	 * coordinates are now aligned differently.
	 *
	 * If you need to adjust the value of this property, you sould do so before
	 * setting a texture or material into this mesh node.
	 * 
	 * The initial value of this property is set when the underlying mesh texture
	 * coordinates are built or loaded. See the expectsVerticallyFlippedTextures
	 * property on the CC3NodesResource class to understand how this property is set
	 * during mesh resource loading from model files.
	 * 
	 * When building meshes programmatically, you should endeavour to design the
	 * mesh so that this property will be YES if you will be using vertically-flipped
	 * textures (all texture file formats except PVR).
	 */
	virtual bool				expectsVerticallyFlippedTextureInTextureUnit( GLuint texUnit );
	
	/**
	 * Sets whether the texture coordinates for the specfied texture unit expects
	 * that the texture was flipped upside-down during texture loading.
	 *
	 * See the notes of the expectsVerticallyFlippedTextureInTextureUnit: method
	 * for a discussion of texture coordinate orientation.
	 *
	 * Setting the value of this property will change the way the texture coordinates
	 * are aligned when a texture is assigned to cover this texture unit for this mesh.
	 */
	virtual void				expectsVerticallyFlippedTexture( bool expectsFlipped, GLuint texUnit );

	/**
	 * Convenience method that flips the texture coordinate mapping vertically
	 * for the specified texture channels. This has the effect of flipping the
	 * texture for that texture channel vertically on the model. and can be
	 * useful for creating interesting effects, or mirror images.
	 *
	 * This implementation flips correctly if the mesh is mapped
	 * to only a section of the texture (a texture atlas).
	 */
	virtual void				flipVerticallyTextureUnit( GLuint texUnit );

	/**
	 * Convenience method that flips the texture coordinate mapping vertically
	 * for all texture units. This has the effect of flipping the textures
	 * vertically on the model. and can be useful for creating interesting
	 * effects, or mirror images.
	 *
	 * This implementation flips correctly if the mesh is mapped
	 * to only a section of the texture (a texture atlas).
	 *
	 * This has the same effect as invoking the flipVerticallyTextureUnit:
	 * method for all texture units.
	 *
	 * This method will also invoke the superclass behaviour to invoke
	 * the same method on each child node.
	 */
	virtual void				flipTexturesVertically();

	/**
	 * Convenience method that flips the texture coordinate mapping horizontally
	 * for the specified texture channels. This has the effect of flipping the
	 * texture for that texture channel horizontally on the model. and can be
	 * useful for creating interesting effects, or mirror images.
	 *
	 * This implementation flips correctly if the mesh is mapped
	 * to only a section of the texture (a texture atlas).
	 */
	virtual void				flipHorizontallyTextureUnit( GLuint texUnit );

	/**
	 * Convenience method that flips the texture coordinate mapping horizontally
	 * for all texture units. This has the effect of flipping the textures
	 * horizontally on the model. and can be useful for creating interesting
	 * effects, or mirror images.
	 *
	 * This implementation flips correctly if the mesh is mapped
	 * to only a section of the texture (a texture atlas).
	 *
	 * This has the same effect as invoking the flipHorizontallyTextureUnit:
	 * method for all texture units.
	 *
	 * This method will also invoke the superclass behaviour to invoke
	 * the same method on each child node.
	 */
	virtual void				flipTexturesHorizontally();

	/**
	 * Configures the mesh so that a texture applied to the specified texture unit will
	 * be repeated the specified number of times across the mesh, in each dimension.
	 * The repeatFactor argument contains two numbers, corresponding to how many times
	 * in each dimension the texture should be repeated.
	 * 
	 * As an example, a value of (1, 2) for the repeatValue indicates that the texture
	 * should repeat twice vertically, but not repeat horizontally.
	 * 
	 * When a texture is repeated, the corresponding side of the texture covering this
	 * mesh must have a length that is a power-of-two, otherwise the padding added by
	 * iOS to convert it to a power-of-two length internally will be visible in the
	 * repeating pattern across the mesh.
	 *
	 * For a side that is not repeating, the corresponding side of the texture covering
	 * this mesh does not require a length that is a power-of-two.
	 *
	 * The textureParameters property of any texture covering this mesh should include
	 * the GL_REPEAT setting in each of its texture wrap components that correspond to
	 * a repeatFactor greater than one. The GL_REPEAT setting is the default setting
	 * for CC3Texture.
	 *
	 * For example, if you want to repeat your texture twice in one dimension, but only
	 * once in the other, then you would use a repeatFactor of (1, 2) or (2, 1). For the
	 * side that is repeating twice, the length of that side of the texture must be a
	 * power-of-two. But the other side may have any dimension. The textureParameters
	 * property of the CC3Texture should include the GL_REPEAT setting for the
	 * corresponding texture dimension.
	 *
	 * You can specify a fractional value for either of the components of the repeatFactor
	 * to expand the texture in that dimension so that only part of the texture appears
	 * in that dimension, while potentially repeating multiple times in the other dimension.
	 */
	virtual void				repeatTexture( ccTex2F repeatFactor, GLuint texUnit );

	/**
	 * Configures the mesh so that the textures in all texture units will be repeated the
	 * specified number of times across the mesh, in each dimension. The repeatFactor
	 * argument contains two numbers, corresponding to how many times in each dimension
	 * the texture should be repeated.
	 *
	 * This has the same effect as invoking the repeatTexture:forTextureUnit: method
	 * for each texture unit.
	 */
	virtual void				repeatTexture( ccTex2F repeatFactor );

	/**
	 * Defines the rectangular area of the textures, for all texture units, that should
	 * be mapped to the mesh used by this node.
	 *
	 * This property facilitates the use of sprite-sheets, where the mesh is covered
	 * by a small fraction of a larger texture. This technique has many uses, including
	 * animating a texture onto a mesh, where each section of the full texture is really
	 * a different frame of a texture animation, or simply loading one larger texture
	 * and using parts of it to texture many different meshes.
	 *
	 * The dimensions of this rectangle are taken as fractional portions of the full
	 * area of the texture. Therefore, a rectangle with zero origin, and unit size
	 * ((0.0, 0.0), (1.0, 1.0)) indicates that the mesh should be covered with the
	 * complete texture.
	 * 
	 * A rectangle of smaller size, and/or a non-zero origin, indicates that the mesh
	 * should be covered by a fractional area of the texture. For example, a rectangular
	 * value for this property with origin at (0.5, 0.5), and size of (0.5, 0.5) indicates
	 * that only the top-right quarter of the texture will be used to cover this mesh.
	 *
	 * The bounds of the texture rectangle must fit within a unit rectangle. Both the
	 * bottom-left and top-right corners must lie between zero and one in both the
	 * X and Y directions.
	 *
	 * This property affects all texture units used by this mesh, to query or change
	 * this property for a single texture unit only, use the textureRectangleForTextureUnit:
	 * and setTextureRectangle:forTextureUnit: methods.
	 *
	 * The initial value of this property is a rectangle with origin at zero, and unit
	 * size, indicating that the mesh will be covered with the complete usable area of
	 * the texture.
	 */
	virtual void				setTextureRectangle( const CCRect& rect );
	virtual CCRect				getTextureRectangle();

	/**
	 * Returns the textureRectangle property from the texture coordinates that are
	 * mapping the specified texture unit index.
	 *
	 * See the notes for the textureRectangle property of this class for an explanation
	 * of the use of this property.
	 */
	virtual CCRect				getTextureRectangleForTextureUnit( GLuint texUnit );

	/**
	 * Sets the textureRectangle property from the texture coordinates that are
	 * mapping the specified texture unit index.
	 *
	 * See the notes for the textureRectangle property of this class for an explanation
	 * of the use of this property.
	 */
	virtual void				setTextureRectangle( const CCRect& aRect, GLuint texUnit );

	/**
	 * Returns whether this mesh is being drawn as point sprites.
	 *
	 * This property returns YES if this mesh node has a texture and the drawingMode property
	 * is set to GL_POINTS, otherwise this property returns NO.
	 */
	virtual bool				isDrawingPointSprites();

	/**
	 * Returns whether any of the textures used by this material have an alpha channel, representing opacity.
	 *
	 * Returns YES if any of the textures contained in this instance has an alpha channel.
	 *
	 * See also the notes of the shouldBlendAtFullOpacity property for the effects of using a
	 * texture with an alpha channel.
	 */
	virtual bool				hasTextureAlpha();

	/**
	 * Returns whether the alpha channel has already been multiplied into each of the RGB
	 * color channels, in any of the textures used by this material.
	 *
	 * Returns YES if any of the textures contained in this instance has pre-mulitiplied alpha.
	 *
	 * See also the notes of the shouldApplyOpacityToColor property for the effects of using textures
	 * with pre-multiplied alpha.
	 */
	virtual bool				hasTexturePremultipliedAlpha();

	/**
	 * Returns whether the opacity of each of the material colors (ambient, diffuse, specular and emission)
	 * should be blended (multiplied) by its alpha value prior to being submitted to the GL engine,
	 * and whether the alpha component of any vertex color should be blended into the vertex color.
	 *
	 * This property returns the value of the same property of the material of this node.
	 *
	 * If this property returns YES, each of the material colors will automatically be blended with its
	 * alpha component prior to being submitted to the GL engine, and any vertex color set using the
	 * setVertexColor4B:at: or setVertexColor4F:at: methods will automatically have its alpha value
	 * blended into (multiplied into) each of the red, green and blue components of that vertex color,
	 * before the color is set into the vertex.
	 */
	virtual bool				shouldApplyOpacityToColor();
	
	/**
	 * Indicates the types of content contained in each vertex of this mesh.
	 *
	 * Each vertex can contain several types of content, optionally including location, normal,
	 * color, texture coordinates, along with other specialized content for certain specialized
	 * meshes. To identify this various content, this property is a bitwise-OR of flags that
	 * enumerate the types of content contained in each vertex of this mesh.
	 *
	 * Valid component flags of this property include:
	 *   - kCC3VertexContentLocation
	 *   - kCC3VertexContentNormal
	 *   - kCC3VertexContentTangent
	 *   - kCC3VertexContentBitangent
	 *   - kCC3VertexContentColor
	 *   - kCC3VertexContentTextureCoordinates
	 *   - kCC3VertexContentBoneWeights
	 *   - kCC3VertexContentBoneIndices
	 *   - kCC3VertexContentPointSize
	 *
	 * To indicate that this mesh should contain particular vertex content, construct a
	 * bitwise-OR combination of one or more of the component types listed above, and set
	 * this property to that combined value.
	 *
	 * Setting this property affects the underlying mesh. When this property is set, if a mesh
	 * has not yet been set in the mesh property of this node, a new CC3Mesh, set to interleave
	 * vertex content, will automatically be created and set into the mesh property of this node.
	 *
	 * When setting this property, if the kCC3VertexContentTextureCoordinates component is not
	 * included, the texture property will be set to nil. If the kCC3VertexContentNormal component
	 * is not included, the shouldUseLighting property will be set to NO automatically.
	 *
	 * This property is a convenience property. You can also construct the mesh by managing the
	 * vertex content directly by assigning specific vertex arrays to the appropriate properties
	 * on the underlying mesh.
	 *
	 * The mesh constructed by this property will be configured to use interleaved data if the
	 * shouldInterleaveVertices property of the mesh is set to YES. You should ensure the value
	 * of the shouldInterleaveVertices property of the underlying mesh is set to the desired value
	 * before setting the value of this property. The initial value of the shouldInterleaveVertices
	 * property is YES.
	 *
	 * If the content is interleaved, for each vertex, the content is held in the structures identified in
	 * the list above, in the order that they appear in the list. You can use this consistent organization
	 * to create an enclosing structure to access all data for a single vertex, if it makes it easier to
	 * access vertex content that way. If vertex content is not specified, it is simply absent, and the content
	 * from the following type will be concatenated directly to the content from the previous type.
	 *
	 * For instance, in a typical textured and illuminated mesh, you might not require per-vertex
	 * color, tangent and bitangent content. You would therefore omit the kCC3VertexContentColor,
	 * kCC3VertexContentTangent and kCC3VertexContentBitangent values in the bitmask when setting
	 * this property, and the resulting structure for each vertex would be a location CC3Vector,
	 * followed by a normal CC3Vector, followed immediately by a texture coordinate ccTex2F.
	 * You can then define an enclosing structure to hold and manage all content for a single vertex.
	 * In this particular example, this is already done for you with the CC3TexturedVertex structure.
	 *
	 * You can declare and use such a custom vertex structure even if you have constructed the vertex
	 * arrays directly, without using this property. The structure of the content of a single vertex
	 * is the same in either case.
	 *
	 * The vertex arrays created in the underlying mesh by this property cover the most common use
	 * cases and data formats. If you require more customized vertex arrays, you can use this property
	 * to create the typical mesh content, and then customize the mesh, by accessing the vertex arrays
	 * individually through their respective properties on the mesh. After doing so, if the vertex
	 * content is interleaved, you should invoke the updateVertexStride method on the mesh to
	 * automatically align the elementOffset and vertexStride properties of all of the vertex arrays.
	 * After setting this property, you do not need to invoke the updateVertexStride method unless
	 * you subsequently make changes to the constructed vertex arrays.
	 *
	 * It is safe to set this property more than once. Doing so will remove any existing vertex arrays
	 * and replace them with those indicated by this property.
	 *
	 * When reading this property, the appropriate bitwise-OR values are returned, corresponding to
	 * the mesh vertex arrays, even if those arrays were constructed directly, instead of through
	 * setting this property. If this mesh contains no vertex arrays, this property will return
	 * kCC3VertexContentNone.
	 */
	virtual CC3VertexContent	getVertexContentTypes();
	virtual void				setVertexContentTypes( CC3VertexContent content );

	/**
	 * Changes the mesh vertices so that the origin of the mesh is at the specified location.
	 *
	 * The origin of the mesh is the location (0,0,0) in the local coordinate system, and is the
	 * location around which all transforms are performed.
	 *
	 * This method can be used to adjust the mesh structure to make it easier to apply transformations,
	 * by moving the origin of the transformations to a more convenient location in the mesh.
	 *
	 * This method changes the location component of every vertex in the mesh. This can be quite costly,
	 * and should only be performed once, to adjust a mesh so that it is easier to manipulate. As an
	 * alternate, you should consider changing the origin of the mesh at development time using a 3D editor.
	 * 
	 * Do not use this method to move your model around. Instead, use the transform properties
	 * (location, rotation and scale) of this node, and let the GL engine do the heavy lifting of
	 * transforming the mesh vertices.
	 * 
	 * This method automatically invokes the markBoundingVolumeDirty method, to ensure that the
	 * boundingVolume encompasses the new vertex locations.
	 *
	 * This method also ensures that the GL VBO that holds the vertex content is updated.
	 */
	virtual void				moveMeshOriginTo( const CC3Vector& location );

	/**
	 * Changes the mesh vertices so that the origin of the mesh is at the center of geometry of the mesh.
	 *
	 * The origin of the mesh is the location (0,0,0) in the local coordinate system, and is the
	 * location around which all transforms are performed.
	 *
	 * This method can be used to adjust the mesh structure to make it easier to apply transformations,
	 * by moving the origin of the transformations to the center of the mesh.
	 *
	 * This method changes the location component of every vertex in the mesh. This can be quite costly,
	 * and should only be performed once, to adjust a mesh so that it is easier to manipulate. As an
	 * alternate, you should consider changing the origin of the mesh at development time using a 3D editor.
	 * 
	 * Do not use this method to move your model around. Instead, use the transform properties
	 * (location, rotation and scale) of this node, and let the GL engine do the heavy lifting of
	 * transforming the mesh vertices.
	 * 
	 * This method automatically invokes the markBoundingVolumeDirty method, to ensure that the
	 * boundingVolume encompasses the new vertex locations.
	 *
	 * This method also ensures that the GL VBO that holds the vertex content is updated.
	 */
	virtual void				moveMeshOriginToCenterOfGeometry();

	/**
	 * Indicates the number of vertices in this mesh.
	 *
	 * Usually, you should treat this property as read-only. However, there may be
	 * occasions with meshes that contain dynamic content, such as particle systems,
	 * where it may be appropriate to set the value of this property.
	 *
	 * Setting the value of this property changes the amount of vertex content that
	 * will be submitted to the GL engine during drawing.
	 *
	 * When setting this property, care should be taken to ensure that the value is
	 * not set larger than the number of vertices that were allocated for this mesh.
	 */
	virtual void				setVertexCount( GLuint vertexCount );
	virtual GLuint				getVertexCount();

	/**
	 * If indexed drawing is used by this mesh, indicates the number of vertex indices in the mesh.
	 *
	 * If indexed drawing is not used by this mesh, this property has no effect, and reading it
	 * will return zero.
	 *
	 * Usually, you should treat this property as read-only. However, there may be
	 * occasions with meshes that contain dynamic content, such as particle systems,
	 * where it may be appropriate to set the value of this property.
	 *
	 * Setting the value of this property changes the amount of vertex content that
	 * will be submitted to the GL engine during drawing.
	 *
	 * When setting this property, care should be taken to ensure that the value is
	 * not set larger than the number of vertices that were allocated for this mesh.
	 */
	virtual void				setVertexIndexCount( GLuint vertexIndexCount );
	virtual GLuint				getVertexIndexCount();

	/**
	 * Returns the location element at the specified index from the vertex content.
	 *
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * whether the vertex content is interleaved to access the correct vertex content component.
	 *
	 * This implementation takes into consideration the dimensionality of the underlying
	 * vertex content. If the dimensionality is 2, the returned vector will contain zero in
	 * the Z component.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	virtual CC3Vector			getVertexLocationAt( GLuint index );

	/**
	 * Sets the location element at the specified index in the vertex content to the specified value.
	 * 
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * whether the vertex content is interleaved to access the correct vertex content component.
	 *
	 * This implementation takes into consideration the dimensionality of the underlying
	 * vertex content. If the dimensionality is 2, the Z component of the specified vector
	 * will be ignored. If the dimensionality is 4, the specified vector will be converted
	 * to a 4D vector, with the W component set to one, before storing.
	 *
	 * When all vertex changes have been made, be sure to invoke the updateVertexLocationsGLBuffer
	 * method to ensure that the GL VBO that holds the vertex content is updated.
	 * 
	 * This method automatically invokes the markBoundingVolumeDirty method, to ensure that the
	 * boundingVolume encompasses the new vertex locations.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	virtual void				setVertexLocation( const CC3Vector& location, GLuint index );

	/**
	 * Returns the location element at the specified index in the underlying vertex content,
	 * as a four-dimensional location in the 4D homogeneous coordinate space.
	 *
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * whether the vertex content is interleaved to access the correct vertex content component.
	 *
	 * This implementation takes into consideration the elementSize property. If the
	 * value of the elementSize property is 3, the returned vector will contain one
	 * in the W component. If the value of the elementSize property is 2, the returned
	 * vector will contain zero in the Z component and one in the W component.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	virtual CC3Vector4			getVertexHomogeneousLocationAt( GLuint index );

	/**
	 * Sets the location element at the specified index in the underlying vertex content to
	 * the specified four-dimensional location in the 4D homogeneous coordinate space.
	 * 
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * whether the vertex content is interleaved to access the correct vertex content component.
	 *
	 * This implementation takes into consideration the dimensionality of the underlying
	 * data. If the dimensionality is 3, the W component of the specified vector will be
	 * ignored. If the dimensionality is 2, both the W and Z components of the specified
	 * vector will be ignored.
	 * 
	 * When all vertex changes have been made, be sure to invoke the updateVertexLocationsGLBuffer
	 * method to ensure that the GL VBO that holds the vertex content is updated.
	 * 
	 * This method automatically invokes the markBoundingVolumeDirty method, to ensure that the
	 * boundingVolume encompasses the new vertex locations.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	virtual void				setVertexHomogeneousLocation( const CC3Vector4& location, GLuint index );

	/**
	 * Returns the normal element at the specified index from the vertex content.
	 *
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * the vertexStride and elementOffset properties to access the correct element.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	virtual CC3Vector			getVertexNormalAt( GLuint index );

	/**
	 * Sets the normal element at the specified index in the vertex content to the specified value.
	 * 
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * the vertexStride and elementOffset properties to access the correct element.
	 *
	 * When all vertex changes have been made, be sure to invoke the
	 * updateVertexNormalsGLBuffer method to ensure that the GL VBO
	 * that holds the vertex content is updated.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	virtual void				setVertexNormal( const CC3Vector& normal, GLuint index );

	/**
	 * Returns the tangent element at the specified index from the vertex content.
	 *
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * the vertexStride and elementOffset properties to access the correct element.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	virtual CC3Vector			getVertexTangentAt( GLuint index );

	/**
	 * Sets the tangent element at the specified index in the vertex content to the specified value.
	 *
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * the vertexStride and elementOffset properties to access the correct element.
	 *
	 * When all vertex changes have been made, be sure to invoke the
	 * updateVertexNormalsGLBuffer method to ensure that the GL VBO
	 * that holds the vertex content is updated.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	virtual void				setVertexTangent( const CC3Vector& normal, GLuint index );

	/**
	 * Returns the tangent element at the specified index from the vertex content.
	 *
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * the vertexStride and elementOffset properties to access the correct element.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	virtual CC3Vector			getVertexBitangentAt( GLuint index );

	/**
	 * Sets the bitangent element at the specified index in the vertex content to the specified value.
	 *
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * the vertexStride and elementOffset properties to access the correct element.
	 *
	 * When all vertex changes have been made, be sure to invoke the
	 * updateVertexNormalsGLBuffer method to ensure that the GL VBO
	 * that holds the vertex content is updated.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	virtual void				setVertexBitangent( const CC3Vector& normal, GLuint index );

	/**
	 * Returns the symbolic content type of the vertex color, which indicates the range of values
	 * stored for each vertex color.
	 *
	 * This property will return one of the values: GL_FLOAT, GL_UNSIGNED_BYTE, or GL_FIXED, or will return
	 * GL_FALSE if this node does not have a mesh, or if that mesh does not support individual vertex colors.
	 *
	 * You can use the value returned by this property to select whether to access individual vertex
	 * color content as bytes or floats, in order to retain accuracy and avoid unnecessary type conversions.
	 */
	virtual GLenum				getVertexColorType();

	/**
	 * Returns the color element at the specified index from the vertex content.
	 *
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * the vertexStride and elementOffset properties to access the correct element.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	virtual ccColor4F			getVertexColor4FAt( GLuint index );

	/**
	 * Sets the color element at the specified index in the vertex content to the specified value.
	 * 
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * the vertexStride and elementOffset properties to access the correct element.
	 *
	 * If the shouldApplyOpacityToColor property of this node returns YES, each of the red, green
	 * and blue components of the specified color will be multiplied by the alpha component of the
	 * specified color before the color is set into the vertex. This occurs when the texture attached
	 * to this mesh contains pre-multiplied alpha. When this occurs, the value retrieved using the
	 * vertexColor4F: method will not be the same as the value set with this method, if the color
	 * contained an alpha value less than one. See the notes of the shouldApplyOpacityToColor
	 * property for more on using pre-multiplied alpha.
	 *
	 * When all vertex changes have been made, be sure to invoke the updateVertexColorsGLBuffer
	 * method to ensure that the GL VBO that holds the vertex content is updated.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	virtual void				setVertexColor4F( const ccColor4F& color, GLuint index );

	/**
	 * Returns the color element at the specified index from the vertex content.
	 *
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * the vertexStride and elementOffset properties to access the correct element.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	virtual ccColor4B			getVertexColor4BAt( GLuint index );

	/**
	 * Sets the color element at the specified index in the vertex content to the specified value.
	 * 
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * the vertexStride and elementOffset properties to access the correct element.
	 *
	 * If the shouldApplyOpacityToColor property of this node returns YES, each of the red, green
	 * and blue components of the specified color will be multiplied by the alpha component of the
	 * specified color before the color is set into the vertex. This occurs when the texture attached
	 * to this mesh contains pre-multiplied alpha. When this occurs, the value retrieved using the
	 * vertexColor4F: method will not be the same as the value set with this method, if the color
	 * contained an alpha value less than 255. See the notes of the shouldApplyOpacityToColor
	 * property for more on using pre-multiplied alpha.
	 *
	 * When all vertex changes have been made, be sure to invoke the updateVertexColorsGLBuffer
	 * method to ensure that the GL VBO that holds the vertex content is updated.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	virtual void				setVertexColor4B( const ccColor4B& color, GLuint index );

	/**
	 * Returns the number of bones that influence each vertex in this mesh. This value defines
	 * the number of bone weights and bone indices that are attached to each vertex.
	 */
	virtual GLuint				getVertexBoneCount();

	/**
	 * Returns the weight value, for the specified influence index within the vertex, for the
	 * vertex at the specified index within the underlying vertex content.
	 *
	 * The weight indicates how much a particular bone influences the movement of the particular
	 * vertex. Several weights are stored for each vertex, one for each bone that influences the
	 * movement of that vertex. The specified influenceIndex parameter must be between zero, and
	 * the vertexBoneCount property (inclusive/exclusive respectively).
	 *
	 * The vertex index refers to vertices, not bytes. The implementation takes into consideration
	 * whether the vertex content is interleaved to access the correct vertex content component.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	virtual GLfloat				getVertexWeightForBoneInfluence( GLuint influenceIndex, GLuint vtxIndex );

	/**
	 * Sets the weight value, for the specified influence index within the vertex, for the
	 * vertex at the specified index within the underlying vertex content.
	 *
	 * The weight indicates how much a particular bone influences the movement of the particular
	 * vertex. Several weights are stored for each vertex, one for each bone that influences the
	 * movement of that vertex. The specified influenceIndex parameter must be between zero, and
	 * the vertexBoneCount property (inclusive/exclusive respectively).
	 *
	 * The vertex index refers to vertices, not bytes. The implementation takes into consideration
	 * whether the vertex content is interleaved to access the correct vertex content component.
	 *
	 * When all vertex changes have been made, be sure to invoke the updateVertexBoneWeightsGLBuffer
	 * method to ensure that the GL VBO that holds the vertex content is updated.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	virtual void				setVertexWeight( GLfloat weight, GLuint influenceIndex, GLuint vtxIndex );

	/**
	 * Returns the weights of all of the bones that influence the movement of the vertex at the
	 * specified index within the underlying vertex content.
	 *
	 * Several weights are stored for each vertex, one for each bone that influences the movement
	 * of the vertex. The number of elements in the returned array is the same for each vertex
	 * in this vertex array, as defined by the vertexBoneCount property.
	 *
	 * The vertex index refers to vertices, not bytes. The implementation takes into consideration
	 * whether the vertex content is interleaved to access the correct vertex content component.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	virtual GLfloat*			getVertexBoneWeightsAt( GLuint vtxIndex );

	/**
	 * Sets the weights of all of the bones that influence the movement of the vertex at the
	 * specified index within the underlying vertex content.
	 *
	 * Several weights are stored for each vertex, one for each bone that influences the movement
	 * of the vertex. The number of elements in the specified input array must therefore be at
	 * least as large as the value of the vertexBoneCount property.
	 *
	 * The vertex index refers to vertices, not bytes. The implementation takes into consideration
	 * whether the vertex content is interleaved to access the correct vertex content component.
	 *
	 * When all vertex changes have been made, be sure to invoke the updateVertexBoneWeightsGLBuffer
	 * method to ensure that the GL VBO that holds the vertex content is updated.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	virtual void				setVertexBoneWeights( GLfloat* weights, GLuint vtxIndex );

	/**
	 * Returns the index of the bone, that provides the influence at the specified influence index
	 * within a vertex, for the vertex at the specified index within the underlying vertex content.
	 *
	 * The bone index indicates which bone provides the particular influence for the movement of
	 * the particular vertex. Several bone indices are stored for each vertex, one for each bone
	 * that influences the movement of that vertex. The specified influenceIndex parameter must
	 * be between zero, and the vertexBoneCount property (inclusive/exclusive respectively).
	 *
	 * The vertex index refers to vertices, not bytes. The implementation takes into consideration
	 * whether the vertex content is interleaved to access the correct vertex content component.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	virtual GLuint				getVertexBoneIndexForBoneInfluence( GLuint iinfluenceIndex, GLuint vtxIndex );

	/**
	 * Sets the index of the bone, that provides the influence at the specified influence index
	 * within a vertex, for the vertex at the specified index within the underlying vertex content.
	 *
	 * The bone index indicates which bone provides the particular influence for the movement of
	 * the particular vertex. Several bone indices are stored for each vertex, one for each bone
	 * that influences the movement of that vertex. The specified influenceIndex parameter must
	 * be between zero, and the vertexBoneCount property (inclusive/exclusive respectively).
	 *
	 * The vertex index refers to vertices, not bytes. The implementation takes into consideration
	 * whether the vertex content is interleaved to access the correct vertex content component.
	 *
	 * When all vertex changes have been made, be sure to invoke the updateVertexBoneIndicesGLBuffer
	 * method to ensure that the GL VBO that holds the vertex content is updated.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	virtual void				setVertexBoneIndex( GLuint boneIndex, GLuint influenceIndex, GLuint vtxIndex );

	/**
	 * Returns the indices of all of the bones that influence the movement of the vertex at the
	 * specified index within the underlying vertex content.
	 *
	 * Several indices are stored for each vertex, one for each bone that influences the movement
	 * of the vertex. The number of elements in the returned array is the same for each vertex
	 * in this vertex array, as defined by the vertexBoneCount property.
	 *
	 * The bone indices can be stored in each vertex as either type GLushort or type GLubyte.
	 * The returned array will be of the type of index stored by the verties in this mesh, and it
	 * is up to the application to know which type will be returned, and cast the returned array
	 * accordingly. The type can be determined by the vertexBoneIndexType property of this mesh,
	 * which will return one of GL_UNSIGNED_SHORT or GL_UNSIGNED_BYTE, respectively.
	 *
	 * The vertex index refers to vertices, not bytes. The implementation takes into consideration
	 * whether the vertex content is interleaved to access the correct vertex content component.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	virtual GLvoid*				getVertexBoneIndicesAt( GLuint vtxIndex );

	/**
	 * Sets the indices of all of the bones that influence the movement of the vertex at the
	 * specified index within the underlying vertex content.
	 *
	 * Several indices are stored for each vertex, one for each bone that influences the movement
	 * of the vertex. The number of elements in the specified input array must therefore be at
	 * least as large as the value of the vertexBoneCount property.
	 *
	 * The bone indices can be stored in each vertx as either type GLushort or type GLubyte.
	 * The specified array must be of the type of index stored by the verties in this mesh, and
	 * it is up to the application to know which type is required, and provide that type of array
	 * accordingly. The type can be determined by the vertexBoneIndexType property of this mesh,
	 * which will return one of GL_UNSIGNED_SHORT or GL_UNSIGNED_BYTE, respectively.
	 *
	 * To avoid checking the elementType altogether, you can use the setVertxBoneIndex:forBoneInfluence:at:
	 * method, which sets the bone index values one at a time, and automatically converts the input type to
	 * the correct stored type.
	 *
	 * The vertex index refers to vertices, not bytes. The implementation takes into consideration
	 * whether the vertex content is interleaved to access the correct vertex content component.
	 *
	 * When all vertex changes have been made, be sure to invoke the updateVertexBoneIndicesGLBuffer
	 * method to ensure that the GL VBO that holds the vertex content is updated.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	virtual void				setVertexBoneIndices( GLvoid* boneIndices, GLuint vtxIndex );

	/**
	 * Returns the type of data element used to store each bone index.
	 *
	 * The value returned by this property will be either GL_UNSIGNED_SHORT or GL_UNSIGNED_BYTE,
	 * corresponding to each bone index being stored in either a type GLushort or type GLubyte,
	 * respectively.
	 *
	 * You can use the value of this property to determine how to cast the data arrays used by
	 * the vertexBoneIndicesAt: and setVertexBoneIndices:at: methods.
	 */
	virtual GLenum				getVertexBoneIndexType();

	/**
	 * Returns the texture coordinate element at the specified index from the vertex content
	 * at the specified texture unit index.
	 *
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * the vertexStride and elementOffset properties to access the correct element.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	virtual ccTex2F				getVertexTexCoord2FForTextureUnit( GLuint texUnit, GLuint index );

	/**
	 * Sets the texture coordinate element at the specified index in the vertex content,
	 * at the specified texture unit index, to the specified texture coordinate value.
	 * 
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * the vertexStride and elementOffset properties to access the correct element.
	 *
	 * When all vertex changes have been made, be sure to invoke the
	 * updateVertexTextureCoordinatesGLBufferForTextureUnit: method
	 * to ensure that the GL VBO that holds the vertex content is updated.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	virtual void				setVertexTexCoord2F( ccTex2F tex2f, GLuint texUnit, GLuint index );

	/**
	 * Returns the texture coordinate element at the specified index from the vertex content
	 * at the commonly used texture unit zero.
	 *
	 * This is a convenience method that is equivalent to invoking the
	 * vertexTexCoord2FForTextureUnit:at: method, with zero as the texture unit index.
	 *
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * the vertexStride and elementOffset properties to access the correct element.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	virtual ccTex2F				getVertexTexCoord2FAt( GLuint index );

	/**
	 * Sets the texture coordinate element at the specified index in the vertex content,
	 * at the commonly used texture unit zero, to the specified texture coordinate value.
	 *
	 * This is a convenience method that delegates to the setVertexTexCoord2F:forTextureUnit:at:
	 * method, passing in zero for the texture unit index.
	 * 
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * the vertexStride and elementOffset properties to access the correct element.
	 *
	 * When all vertex changes have been made, be sure to invoke the
	 * updateVertexTextureCoordinatesGLBuffer method to ensure that
	 * the GL VBO that holds the vertex content is updated.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	virtual void				setVertexTexCoord2F( ccTex2F tex2f, GLuint index );

	/**
	 * Returns the index element at the specified index from the vertex content.
	 *
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * the vertexStride and elementOffset properties to access the correct element.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	virtual GLuint				getVertexIndexAt( GLuint index );

	/**
	 * Sets the index element at the specified index in the vertex content to the specified value.
	 * 
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * the vertexStride and elementOffset properties to access the correct element.
	 *
	 * When all vertex changes have been made, be sure to invoke the
	 * updateVertexIndicesGLBuffer method to ensure that the GL VBO
	 * that holds the vertex content is updated.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	virtual void				setVertexIndex( GLuint vertexIndex, GLuint index );

	/** Updates the GL engine buffer with the vertex location content in this mesh. */
	virtual void				updateVertexLocationsGLBuffer();
	
	/** Updates the GL engine buffer with the vertex normal content in this mesh. */
	virtual void				updateVertexNormalsGLBuffer();

	/** Updates the GL engine buffer with the vertex tangent content in this mesh. */
	virtual void				updateVertexTangentsGLBuffer();

	/** Updates the GL engine buffer with the vertex tangent content in this mesh. */
	virtual void				updateVertexBitangentsGLBuffer();

	/** Updates the GL engine buffer with the vertex color content in this mesh. */
	virtual void				updateVertexColorsGLBuffer();

	/** Updates the GL engine buffer with the vertex bone weight content in this mesh. */
	virtual void				updateVertexBoneWeightsGLBuffer();

	/** Updates the GL engine buffer with the vertex bone indices content in this mesh. */
	virtual void				updateVertexBoneIndicesGLBuffer();

	/**
	 * Updates the GL engine buffer with the vertex texture coord content from the
	 * specified texture unit in this mesh.
	 */
	virtual void				updateVertexTextureCoordinatesGLBufferForTextureUnit( GLuint texUnit );

	/**
	 * Updates the GL engine buffer with the vertex texture coord content from
	 * texture unit zero in this mesh.
	 */
	virtual void				updateVertexTextureCoordinatesGLBuffer();

	/**
	 * Convenience method to update the GL engine buffers with the vertex content in this mesh.
	 *
	 * This updates the content of each vertex. It does not update the vertex indices. To update
	 * the vertex index data to the GL engine, use the updateVertexIndicesGLBuffer method.
	 */
	virtual void				updateGLBuffers();

	/** Updates the GL engine buffer with the vertex index content in this mesh. */
	virtual void				updateVertexIndicesGLBuffer();
							
	/**
	 * Indicates whether information about the faces of this mesh should be cached.
	 *
	 * If this property is set to NO, accessing information about the faces through the
	 * methods faceAt:, faceIndicesAt:, faceCenterAt:, faceNormalAt:, or facePlaneAt:,
	 * will be calculated dynamically from the mesh data.
	 *
	 * If such data will be accessed frequently, this repeated dynamic calculation may
	 * cause a noticable impact to performance. In such a case, this property can be
	 * set to YES to cause the data to be calculated once and cached, improving the
	 * performance of subsequent accesses to information about the faces.
	 *
	 * However, caching information about the faces will increase the amount of memory
	 * required by the mesh, sometimes significantly. To avoid this additional memory
	 * overhead, in general, you should leave this property set to NO, unless intensive
	 * access to face information is causing a performance impact.
	 *
	 * An example of a situation where the use of this property may be noticable,
	 * is when adding shadow volumes to nodes. Shadow volumes make intense use of
	 * accessing face information about the mesh that is casting the shadow.
	 *
	 * When the value of this property is set to NO, any data cached during previous
	 * access through the faceIndicesAt:, faceCenterAt:, faceNormalAt:, or facePlaneAt:,
	 * methods will be cleared.
	 *
	 * The initial value of this property is NO.
	 */
	virtual void				setShouldCacheFaces( bool cacheFaces );
	virtual bool				shouldCacheFaces();

	/**
	 * Returns the number of faces in this mesh.
	 *
	 * This is calculated from the number of vertices, taking into
	 * consideration the type of primitives that this mesh is drawing.
	 */
	virtual GLuint				getFaceCount();
	
	/**
	 * Returns the number of faces to be drawn from the specified number of
	 * vertex indices, based on the type of primitives that this mesh is drawing.
	 */ 
	GLuint						getFaceCountFromVertexIndexCount( GLuint vc );

	/**
	 * Returns the number of vertex indices required to draw the specified number
	 * of faces, based on the type of primitives that this mesh is drawing.
	 */ 
	GLuint						getVertexIndexCountFromFaceCount( GLuint fc );

	/**
	 * Returns the face from the mesh at the specified index.
	 *
	 * The specified faceIndex value refers to the index of the face, not the vertices
	 * themselves. So, a value of 5 will retrieve the three vertices that make up the
	 * fifth triangular face in this mesh. The specified index must be between zero,
	 * inclusive, and the value of the faceCount property, exclusive.
	 *
	 * The returned face structure contains only the locations of the vertices. If the vertex
	 * locations are interleaved with other vertex content, such as color or texture coordinates,
	 * or other padding, that data will not appear in the returned face structure. For that
	 * remaining vertex content, you can use the faceIndicesAt: method to retrieve the indices
	 * of the vertex content, and then use the vertex accessor methods to retrieve the individual
	 * vertex content components.
	 *
	 * If you will be invoking this method frequently, you can optionally set the
	 * shouldCacheFaces property to YES to speed access, and possibly improve performance.
	 * However, be aware that setting the shouldCacheFaces property to YES can significantly
	 * increase the amount of memory used by the mesh.
	 */
	CC3Face						getFaceAt( GLuint faceIndex );

	/**
	 * Returns the mesh face that is made up of the three vertices at the three indices
	 * within the specified face indices structure.
	 *
	 * The returned face structure contains only the locations of the vertices. If the vertex
	 * locations are interleaved with other vertex content, such as color or texture coordinates,
	 * or other padding, that data will not appear in the returned face structure. For that
	 * remaining vertex content, you can use the faceIndicesAt: method to retrieve the indices
	 * of the vertex content, and then use the vertex accessor methods to retrieve the individual
	 * vertex content components.
	 */
	CC3Face						getFaceFromIndices( const CC3FaceIndices& faceIndices );

	/**
	 * Returns the face from the mesh at the specified index, as indices into the mesh vertices.
	 *
	 * The specified faceIndex value refers to the index of the face, not the vertices
	 * themselves. So, a value of 5 will retrieve the three vertices that make up the
	 * fifth triangular face in this mesh. The specified index must be between zero,
	 * inclusive, and the value of the faceCount property, exclusive.
	 *
	 * The returned structure reference contains the indices of the three vertices that
	 * make up the triangular face. These indices index into the actual vertex content within
	 * the layout of the mesh.
	 *
	 * This method takes into consideration any padding (stride) between the vertex indices.
	 *
	 * If you will be invoking this method frequently, you can optionally set the
	 * shouldCacheFaces property to YES to speed access, and possibly improve performance.
	 * However, be aware that setting the shouldCacheFaces property to YES can significantly
	 * increase the amount of memory used by the mesh.
	 */
	CC3FaceIndices				getFaceIndicesAt( GLuint faceIndex );

	/**
	 * Returns the center of the mesh face at the specified index.
	 *
	 * If you will be invoking this method frequently, you can optionally set the
	 * shouldCacheFaces property to YES to speed access, and possibly improve performance.
	 * However, be aware that setting the shouldCacheFaces property to YES can significantly
	 * increase the amount of memory used by the mesh.
	 */
	CC3Vector					getFaceCenterAt( GLuint faceIndex );

	/**
	 * Returns the normal of the mesh face at the specified index.
	 *
	 * If you will be invoking this method frequently, you can optionally set the
	 * shouldCacheFaces property to YES to speed access, and possibly improve performance.
	 * However, be aware that setting the shouldCacheFaces property to YES can significantly
	 * increase the amount of memory used by the mesh.
	 */
	CC3Vector					getFaceNormalAt( GLuint faceIndex );

	/**
	 * Returns the plane of the mesh face at the specified index.
	 *
	 * If you will be invoking this method frequently, you can optionally set the
	 * shouldCacheFaces property to YES to speed access, and possibly improve performance.
	 * However, be aware that setting the shouldCacheFaces property to YES can significantly
	 * increase the amount of memory used by the mesh.
	 */
	CC3Plane					getFacePlaneAt( GLuint faceIndex );

	/** Returns the indices of the neighbours of the mesh face at the specified index. */
	CC3FaceNeighbours			getFaceNeighboursAt( GLuint faceIndex );

	/**
	 * Populates the specified array with information about the intersections of the specified ray
	 * and this mesh, up to the specified maximum number of intersections.
	 *
	 * This method returns the actual number of intersections found (up to the specified maximum).
	 * This value indicates how many of the elements of the specifed intesections array were populated
	 * during the execution of this method. The contents of elements beyond that number are undefined.
	 *
	 * Each of the populated elements of the intersections array contains information about the face
	 * on which the intersection occurred, the location of the intersection, and the distance from the
	 * ray startLocation where the intersection occurred. The location and distance components are
	 * specified in the local coordinates system of this mesh.
	 *
	 * The intersections array is not sorted in any way. In particular, when the array contains multiple
	 * entries, the first element in the array does not necessily contain the closest intersection.
	 * If you need to determine the closest intersection, you can iterate the intersections array and
	 * compare the values of the location element of each intersection.
	 *
	 * To use this method, allocate an array of CC3MeshIntersection structures, pass a reference to it
	 * in the intersections parameter, and indicate the size of that array in the maxHitCount parameter.
	 *
	 * The method iterates through the faces in the mesh until the indicated number of intersections are
	 * found, or until all the faces in the mesh have been inspected. Therefore, to keep performance high,
	 * you should set the maxHitCount parameter no higher than the number of intersections that are useful
	 * to you. For example, specifiying a value of one for the maxHitCount parameter will cause this method
	 * to return as soon as the first intersection is found. In most cases, this is all that is needed.
	 * 
	 * The allowBackFaces parameter is used to indicate whether to include intersections where the ray
	 * pierces a face from its back face. Typically, this means that the ray has intersected the face
	 * as the ray exits on the far side of the mesh. In most cases you will interested only where the
	 * ray intersects the near side of the mesh, in which case you can set this parameter to NO.
	 *
	 * The allowBehind parameter is used to indicate whether to include intersections that occur behind
	 * the startLocation of the ray, in the direction opposite to the direction of the ray. Typically,
	 * this might mean the mesh is located behind the ray startLocation, or it might mean the ray starts
	 * inside the mesh. Again,in most cases, you will be interested only in intersections that occur in
	 * the direction the ray is pointing, and can ususally set this parameter to NO.
	 */
	GLuint						findFirst( GLuint maxHitCount, CC3MeshIntersection* intersectons, CC3Ray aRay, bool acceptBackFaces, bool acceptBehind );

	/**
	 * Populates the specified array with information about the intersections of the specified ray
	 * and this mesh, up to the specified maximum number of intersections.
	 *
	 * This is a convenience method that converts the specified global ray to the local coordinate system
	 * of this node, and invokes the findFirst:intersections:ofLocalRay:acceptBackFaces:acceptBehindRay:
	 * method, and converts the location and distance components of each of the elements in the intersections
	 * array to the global coordinate system.
	 *
	 * See the notes for the findFirst:intersections:ofLocalRay:acceptBackFaces:acceptBehindRay: method
	 * to understand more about how to use this method.
	 */
	GLuint						findFirstGlobal( GLuint maxHitCount, CC3MeshIntersection* intersections, CC3Ray aGlobalRay, bool acceptBackFaces, bool acceptBehind );
	/**
	 * The drawing mode indicating how the vertices are connected (points, lines, triangles...).
	 *
	 * This must be set with a valid GL drawing mode enumeration. The default value is GL_TRIANGLES.
	 */
	void						setDrawingMode( GLenum drawingMode ); 
	GLenum						getDrawingMode();
		
	/**
	 * Draws the local content of this mesh node by following these steps:
	 *   -# If the shouldDecorateNode property of the visitor is YES, and this node
	 *      has a material, invokes the drawWithVisitor method of the material.
	 *      Otherwise, invokes the CC3Material class-side unbind method.
	 *   -# Invokes the drawWithVisitor: method of the encapsulated mesh.
	 *
	 * This method is called automatically from the transformAndDrawWithVisitor: method
	 * of this node. Usually, the application never needs to invoke this method directly.
	 */
	virtual void				drawWithVisitor( CC3NodeDrawingVisitor* visitor );

	/**
	 * Returns whether the vertices of this mesh node are influenced by a skeleton of bones.
	 *
	 * Vertex skinning (also known as bone-rigging) is a technique of manipulating, or deforming,
	 * the vertices of a mesh through the movement of a skeleton of bones.
	 *
	 * This implementation returns NO. Subclasses that support vertex skinning (notably the CC3SkinMeshNode
	 * subclass) will return YES if the vertices are influenced by a skeleton of bone nodes.
	 */
	virtual bool				hasSkeleton();

	/**
	 * Returns whether the skeletal structures controlling any descendant skinned mesh nodes are
	 * composed of bones that undergo only rigid transforms, relative to their nearest ancestor
	 * soft-body node.
	 *
	 * The initial value of this property is NO. It is set to YES when the ensureRigidSkeleton
	 * method is invoked on this node, or an ancestor node, and this mesh node has a skeleton
	 * (as indicated by the hasSkeleton property of this node).
	 *
	 * When the bones in a skeleton contain only rigid transforms, the vertices in the skin-mesh
	 * node can be transformed within a shader using rotations and transforms, instead of a full
	 * transform matrix. This allows for many more bones to be transferred to the shader program
	 * during a single GL draw call, which increases performance in many larger meshes.
	 *
	 * Consequently, the value of this property affects the choice of shader that will be selected
	 * automatically for this node.
	 */
	virtual bool				hasRigidSkeleton();

	/**
	 * Returns the face from the mesh at the specified index.
	 * 
	 * If the vertices of this mesh node represent the skin covering the bones of a 
	 * soft-body, the vertex locations of the returned face take into consideration the
	 * current deformation caused by motion of the bones underlying the this skin mesh.
	 * Otherwise, this method returns the same value as the faceAt: method.
	 *
	 * In either case, the vertex locations of the returned face are specified in the
	 * local coordinate system of this node.
	 *
	 * The specified faceIndex value refers to the index of the face, not the vertices
	 * themselves. So, a value of 5 will retrieve the three vertices that make up the
	 * fifth triangular face in this mesh. The specified index must be between zero,
	 * inclusive, and the value of the faceCount property, exclusive.
	 *
	 * The returned face structure contains only the locations of the vertices. If the vertex
	 * locations are interleaved with other vertex data, such as color or texture coordinates,
	 * or other padding, that data will not appear in the returned face structure. For that
	 * remaining vertex data, you can use the faceIndicesAt: method to retrieve the indices
	 * of the vertex data, and then use the vertex accessor methods to retrieve the individual
	 * vertex data components.
	 */
	virtual CC3Face				getDeformedFaceAt( GLuint faceIndex );

	/**
	 * Returns the center of the mesh face at the specified index.
	 * 
	 * If the vertices of this mesh node represent the skin covering the bones of a  soft-body, the
	 * returned location takes into consideration the current deformation caused by motion of the 
	 * bones underlying the this skin mesh. The returned location is the center of the face in its 
	 * location and orientation after the skin has been deformed by the current position of the 
	 * underlying bones. Otherwise, this method returns the same value as the faceCenterAt: method.
	 *
	 * In either case, the returned face center is specified in the local coordinate system of this node.
	 */
	virtual CC3Vector			getDeformedFaceCenterAt( GLuint faceIndex );

	/**
	 * Returns the normal of the mesh face at the specified index.
	 * 
	 * If the vertices of this mesh node represent the skin covering the bones of a  soft-body, the
	 * returned normal takes into consideration the current deformation caused by motion of the bones
	 * underlying the this skin mesh. The returned vector is the normal of the face in its orientation
	 * after the skin has been deformed by the current position of the underlying bones. Otherwise,
	 * this method returns the same value as the faceNormalAt: method.
	 * 
	 * In either case, the returned face normal is specified in the local coordinate system of this node.
	 */
	virtual CC3Vector			getDeformedFaceNormalAt( GLuint faceIndex );

	/**
	 * Returns the plane of the mesh face at the specified index.
	 * 
	 * If the vertices of this mesh node represent the skin covering the bones of a soft-body, the
	 * returned plane takes into consideration the current deformation caused by motion of the bones
	 * underlying the this skin mesh. The returned plane is the plane of the face in its location and
	 * orientation after the skin has been deformed by the current position of the underlying bones. 
	 * Otherwise, this method returns the same value as the facePlaneAt: method.
	 * 
	 * In either case, the returned face plane is specified in the local coordinate system of this node.
	 */
	virtual CC3Plane			getDeformedFacePlaneAt( GLuint faceIndex );

	/**
	 * Returns the vertex from the mesh at the specified vtxIndex, that is within the face at 
	 * the specified faceIndex.
	 * 
	 * If the vertices of this mesh node represent the skin covering the bones of a soft-body, the
	 * returned vertex location takes into consideration the current deformation caused by motion 
	 * of the bones underlying the this skin mesh. Otherwise, this method returns the same value as
	 * the vertexLocationAt: method.
	 *
	 * In either case, the returned vertex location is specified in the local coordinate system of this node.
	 *
	 * The specified faceIndex value refers to the index of the face that contains the vertex. It is
	 * required to determine the skin section whose bones are deforming the vertex location at the
	 * specified vertex index. The specified faceIndex must be between zero, inclusive, and the value
	 * of the faceCount property, exclusive.
	 *
	 * The specified vtxIndex must be between zero, inclusive, and the value of the vertexCount 
	 * property, exclusive.
	 */
	virtual CC3Vector			getDeformedVertexLocationAt( GLuint vertexIndex, GLuint faceIndex );

	/**
	 * Allocates and initializes an autoreleased instance with the specified name and an
	 * automatically generated unique tag value. The tag value is generated using a call to nextTag.
	 */
	static CC3MeshNode*			nodeWithName( const std::string& aName );

	/**
	 * Ensures that the contained mesh has been created and has vertexContentType defined.
	 *
	 * This method is invoked by each of the populateAs... family of methods, prior to populating
	 * the mesh contents.
	 *
	 * The vertexContentType property of this mesh node may be set prior to invoking any of the
	 * populateAs... family of methods, to define the content type for each vertex.
	 *
	 * If the vertexContentType property has not already been set, that property is set to a value
	 * of (kCC3VertexContentLocation | kCC3VertexContentNormal | kCC3VertexContentTextureCoordinate).
	 * and the mesh will be populated with location, normal and texture coordinates for each vertex.
	 *
	 * If you do not need either of the normal or texture coordinates, set the vertexContentTypes
	 * property accordingly prior to invoking any of the populateAs... methods.
	 */
	virtual CC3Mesh*			prepareParametricMesh();

	/**
	 * Populates this instance as a simple triangular mesh.
	 *
	 * The specified face defines the three vertices at the corners of the triangular mesh in 3D space.
	 * The vertices within the CC3Face structure are specified in the winding order of the triangular
	 * face. The winding order of the specified face determines the winding order of the vertices in
	 * the mesh, and the direction of the normal vector applied to each of the vertices. Since the
	 * resulting triangular mesh is flat, all vertices will have the same normal vector.
 
	 * Although the triangle can be created with the corners can be anywhere in 3D space, for
	 * simplicity of construction, it is common practice, when using this method, to specify the
	 * mesh in the X-Y plane (where all three corners have a zero Z-component), and then rotate
	 * this node to an orientation in 3D space.
	 *
	 * The texCoords parameter is an array of ccTex2F structures, providing the texture coordinates for
	 * the cooresponding vertices of the face. This array must have three elements, one for each vertex
	 * in the specified face. If the mesh will not be covered with a texture, you can pass in any values
	 * in the elements of this array.
	 *
	 * The tessellation property determines how the mesh will be tessellated into smaller faces. The
	 * specified tessellation value indicates how many divisions each side of the main triangle should
	 * be divided into. Each side of the triangular mesh is tessellated into the same number of divisions.
	 *
	 * This mesh can be covered with a solid material or a single texture. If this mesh is to be covered
	 * with a texture, use the texture property of this mesh to set the texture. If a solid color is
	 * desired, leave the texture property unassigned.
	 *
	 * The vertexContentType property of this mesh may be set prior to invoking this method, to define the
	 * content type for each vertex. Content types kCC3VertexContentLocation, kCC3VertexContentNormal, and
	 * kCC3VertexContentTextureCoordinate are populated by this method.
	 *
	 * If the vertexContentType property has not already been set, that property is set to a value of
	 * (kCC3VertexContentLocation | kCC3VertexContentNormal | kCC3VertexContentTextureCoordinate), and
	 * the mesh will be populated with location, normal and texture coordinates for each vertex.
	 */
	virtual void				populateAsTriangle( const CC3Face& face, ccTex2F* texCoords, GLuint divsPerSide );

	/**
	 * Populates this instance as a simple rectangular mesh of the specified size,
	 * centered at the origin, and laid out on the X-Y plane.
	 *
	 * The rectangular mesh contains only one face with two triangles. The result is
	 * the same as invoking populateAsCenteredRectangleWithSize:andTessellation: with
	 * the divsPerAxis argument set to {1,1}.
	 *
	 * This mesh can be covered with a solid material or a single texture. If this mesh
	 * is to be covered with a texture, use the texture property of this node to set
	 * the texture. If a solid color is desired, leave the texture property unassigned.
	 *
	 * The vertexContentType property of this mesh node may be set prior to invoking this method,
	 * to define the content type for each vertex. Content types kCC3VertexContentLocation,
	 * kCC3VertexContentNormal, and kCC3VertexContentTextureCoordinate are populated by this method.
	 *
	 * If the vertexContentType property has not already been set, that property is set to a value
	 * of (kCC3VertexContentLocation | kCC3VertexContentNormal | kCC3VertexContentTextureCoordinate).
	 * and the mesh will be populated with location, normal and texture coordinates for each vertex.
	 */
	virtual void				populateAsCenteredRectangleWithSize( const CCSize& rectSize );

	/**
	 * Populates this instance as a simple rectangular mesh of the specified size,
	 * centered at the origin, and laid out on the X-Y plane.
	 *
	 * The large rectangle can be divided into many smaller divisions. Building a rectanglular
	 * surface from more than one division can dramatically improve realism when the surface is
	 * illuminated with specular lighting or a tightly focused spotlight, because increasing the
	 * face count increases the number of vertices that interact with the specular or spot lighting.
	 *
	 * The divsPerAxis argument indicates how to break this large rectangle into multiple faces.
	 * The X & Y elements of the divsPerAxis argument indicate how each axis if the rectangle
	 * should be divided into faces. The total number of faces in the rectangle will therefore
	 * be the multiplicative product of the X & Y elements of the divsPerAxis argument.
	 *
	 * For example, a value of {5,5} for the divsPerAxis argument will result in the rectangle
	 * being divided into 25 faces, arranged into a 5x5 grid.
	 *
	 * This mesh can be covered with a solid material or a single texture. If this mesh
	 * is to be covered with a texture, use the texture property of this node to set
	 * the texture. If a solid color is desired, leave the texture property unassigned.
	 *
	 * The vertexContentType property of this mesh node may be set prior to invoking this method,
	 * to define the content type for each vertex. Content types kCC3VertexContentLocation,
	 * kCC3VertexContentNormal, and kCC3VertexContentTextureCoordinate are populated by this method.
	 *
	 * If the vertexContentType property has not already been set, that property is set to a value
	 * of (kCC3VertexContentLocation | kCC3VertexContentNormal | kCC3VertexContentTextureCoordinate).
	 * and the mesh will be populated with location, normal and texture coordinates for each vertex.
	 */
	virtual void				populateAsCenteredRectangleWithSize( const CCSize& rectSize, const CC3Tessellation& divsPerAxis );

	/**
	 * Populates this instance as a simple rectangular mesh of the specified size, with the specified
	 * relative origin, and laid out on the X-Y plane.
	 *
	 * The rectangular mesh contains only one face with two triangles. The result is the same as invoking
	 * the populateAsRectangleWithSize:andRelativeOrigin:andTessellation: with the divsPerAxis argument
	 * set to {1,1}.
	 *
	 * The relative origin is a fractional point that is relative to the rectangle's extent, and indicates
	 * where the origin of the rectangular mesh is to be located. The mesh origin is the origin of the
	 * local coordinate system of the mesh, and is the basis for all transforms applied to the mesh
	 * (including the location and rotation properties).
	 *
	 * The specified relative origin should be a fractional value. If it is {0, 0}, the rectangle will
	 * be laid out so that the bottom-left corner is at the origin. If it is {1, 1}, the rectangle will
	 * be laid out so that the top-right corner of the rectangle is at the origin. If it is {0.5, 0.5},
	 * the rectangle will be laid out with the origin at the center, as in the
	 * populateAsCenteredRectangleWithSize: method.
	 *
	 * This mesh can be covered with a solid material or a single texture. If this mesh is to be covered
	 * with a texture, use the texture property of this mesh to set the texture. If a solid color is
	 * desired, leave the texture property unassigned.
	 *
	 * The vertexContentType property of this mesh may be set prior to invoking this method, to define
	 * the content type for each vertex. Content types kCC3VertexContentLocation, kCC3VertexContentNormal,
	 * and kCC3VertexContentTextureCoordinate are populated by this method.
	 *
	 * If the vertexContentType property has not already been set, that property is set to a value of
	 * (kCC3VertexContentLocation | kCC3VertexContentNormal | kCC3VertexContentTextureCoordinate), and
	 * the mesh will be populated with location, normal and texture coordinates for each vertex.
	 */
	virtual void				populateAsRectangleWithSize( const CCSize& rectSize, const CCPoint& origin );

	/**
	 * Populates this instance as a simple rectangular mesh of the specified size, with the specified
	 * relative origin, and laid out on the X-Y plane.
	 *
	 * The large rectangle can be divided into many smaller divisions. Building a rectanglular surface
	 * from more than one division can dramatically improve realism when the surface is illuminated with
	 * specular lighting or a tightly focused spotlight, because increasing the face count increases the
	 * number of vertices that interact with the specular or spot lighting.
	 *
	 * The divsPerAxis argument indicates how to break this large rectangle into multiple faces. The X & Y
	 * elements of the divsPerAxis argument indicate how each axis if the rectangle should be divided into
	 * faces. The total number of faces in the rectangle will therefore be the multiplicative product of
	 * the X & Y elements of the divsPerAxis argument.
	 *
	 * For example, a value of {5,5} for the divsPerAxis argument will result in the rectangle being
	 * divided into 25 faces, arranged into a 5x5 grid.
	 *
	 * The relative origin is a fractional point that is relative to the rectangle's extent, and indicates
	 * where the origin of the rectangular mesh is to be located. The mesh origin is the origin of the
	 * local coordinate system of the mesh, and is the basis for all transforms applied to the mesh
	 * (including the location and rotation properties).
	 *
	 * The specified relative origin should be a fractional value. If it is {0, 0}, the rectangle will
	 * be laid out so that the bottom-left corner is at the origin. If it is {1, 1}, the rectangle will
	 * be laid out so that the top-right corner of the rectangle is at the origin. If it is {0.5, 0.5},
	 * the rectangle will be laid out with the origin at the center, as in the
	 * populateAsCenteredRectangleWithSize: method.
	 *
	 * This mesh can be covered with a solid material or a single texture. If this mesh is to be covered
	 * with a texture, use the texture property of this mesh to set the texture. If a solid color is desired,
	 * leave the texture property unassigned.
	 *
	 * The vertexContentType property of this mesh may be set prior to invoking this method, to define the
	 * content type for each vertex. Content types kCC3VertexContentLocation, kCC3VertexContentNormal, and
	 * kCC3VertexContentTextureCoordinate are populated by this method.
	 *
	 * If the vertexContentType property has not already been set, that property is set to a value of
	 * (kCC3VertexContentLocation | kCC3VertexContentNormal | kCC3VertexContentTextureCoordinate), and
	 * the mesh will be populated with location, normal and texture coordinates for each vertex.
	 */
	virtual void				populateAsRectangleWithSize( const CCSize& rectSize, const CCPoint& origin, const CC3Tessellation& divsPerAxis );

	/**
	 * Populates this instance as a flat, single-sided circular disk mesh of the specified
	 * radius, centered at the origin, and laid out on the X-Y plane.
	 *
	 * The surface of the disk is divided into many smaller divisions, both in the radial and
	 * angular dimensions.
	 *
	 * The radialAndAngleDivs argument indicates how to divide the surface of the disks into
	 * divisions. The X element of the radialAndAngleDivs argument indicates how many radial
	 * divisions will occur from the center and the circuferential edge. A value of one means
	 * that the mesh will consist of a series of radial triangles from the center of the
	 * circle to the edge. A larger value for the X element of the radialAndAngleDivs argument
	 * will structure the mesh as a series of concentric rings. This value must be at least one.
	 * 
	 * The Y element of the radialAndAngleDivs argument indicates how many angular divisions
	 * will occur around the circumference. This value must be at least three, which will
	 * essentially render the circle as a triangle. But, typically, this value will be larger.
	 * 
	 * For example, a value of {4,24} for the radialAndAngleDivs argument will result in the
	 * disk being divided into four concentric rings, each divided into 24 segments around the
	 * circumference of the circle.
	 * 
	 * Each segement, except those in the innermost disk is trapezoidal, and will be constructed
	 * from two triangular mesh faces. Therefore, the number of triangles in the mesh will be
	 * (2X - 1) * Y, where X = radialAndAngleDivs.x and Y = radialAndAngleDivs.y.
	 *
	 * This mesh can be covered with a solid material or a single texture. If this mesh
	 * is to be covered with a texture, use the texture property of this node to set
	 * the texture. If a solid color is desired, leave the texture property unassigned.
	 *
	 * The texture is mapped to the tessellated disk as if a tagential square was overlaid over
	 * the circle, starting from the lower left corner, where both X and Y are at a minimum.
	 * The center of the disk maps to the center of the texture.
	 *
	 * The vertexContentType property of this mesh node may be set prior to invoking this method,
	 * to define the content type for each vertex. Content types kCC3VertexContentLocation,
	 * kCC3VertexContentNormal, and kCC3VertexContentTextureCoordinate are populated by this method.
	 *
	 * If the vertexContentType property has not already been set, that property is set to a value
	 * of (kCC3VertexContentLocation | kCC3VertexContentNormal | kCC3VertexContentTextureCoordinate).
	 * and the mesh will be populated with location, normal and texture coordinates for each vertex.
	 */
	virtual void				populateAsDiskWithRadius( GLfloat radius, const CC3Tessellation& radialAndAngleDivs );

	/**
	 * Populates this instance as a simple rectangular box mesh from the specified
	 * bounding box, which contains two of the diagonal corners of the box.
	 *
	 * This mesh can be covered with a solid material or a single texture. If this mesh
	 * is to be covered with a texture, use the texture property of this node to set
	 * the texture. If a solid color is desired, leave the texture property unassigned.
	 *
	 * The vertexContentType property of this mesh node may be set prior to invoking this method,
	 * to define the content type for each vertex. Content types kCC3VertexContentLocation,
	 * kCC3VertexContentNormal, and kCC3VertexContentTextureCoordinate are populated by this method.
	 *
	 * If the vertexContentType property has not already been set, that property is set to a value
	 * of (kCC3VertexContentLocation | kCC3VertexContentNormal | kCC3VertexContentTextureCoordinate).
	 * and the mesh will be populated with location, normal and texture coordinates for each vertex.
	 *
	 * If a texture is to be wrapped around this mesh, since the single texture is wrapped
	 * around all six sides of the box, the texture will be mapped according to the layout
	 * illustrated in the texture file BoxTexture.png, included in the distribution.
	 *
	 * The "front" of the box is the side that faces towards the positive-Z axis, the "top"
	 * of the box is the side that faces towards the positive-Y axis, and the "right" side
	 * of the box is the side that faces towards the positive-X axis.
	 *
	 * For the purposes of wrapping a texture around the box, the texture will wrap
	 * uniformly around all sides, and the texture will not appear stretched between any
	 * two adjacent sides. This is useful when you are texturing the box with a simple
	 * rectangular repeating pattern and want the texture to appear consistent across
	 * the sides, for example, a brick pattern wrapping around all four sides of a house.
	 *
	 * Depending on the relative aspect of the height and width of the box, the texture
	 * may appear distorted horizontal or vertically. If you need to correct that, you
	 * can use the repeatTexture: method, and adjust one of the dimensions.
	 *
	 * For higher fidelity in applying textures to non-cube boxes, so that the texture
	 * will not be stretched to fit, use the populateAsSolidBox:withCorner: method.
	 *
	 * Thanks to Cocos3D user andyman for contributing the prototype code and texture
	 * template file for this method.
	 */
	virtual void				populateAsSolidBox( const CC3Box& box );

	/**
	 * Populates this instance as a simple rectangular box mesh from the specified
	 * bounding box, which contains two of the diagonal corners of the box.
	 *
	 * This mesh can be covered with a solid material or a single texture. If this mesh
	 * is to be covered with a texture, use the texture property of this node to set
	 * the texture. If a solid color is desired, leave the texture property unassigned.
	 *
	 * The vertexContentType property of this mesh node may be set prior to invoking this method,
	 * to define the content type for each vertex. Content types kCC3VertexContentLocation,
	 * kCC3VertexContentNormal, and kCC3VertexContentTextureCoordinate are populated by this method.
	 *
	 * If the vertexContentType property has not already been set, that property is set to a value
	 * of (kCC3VertexContentLocation | kCC3VertexContentNormal | kCC3VertexContentTextureCoordinate).
	 * and the mesh will be populated with location, normal and texture coordinates for each vertex.
	 *
	 * If a texture is to be wrapped around this mesh, since the single texture is wrapped
	 * around all six sides of the box, the texture will be mapped according to the layout
	 * illustrated in the texture file BoxTexture.png, included in the distribution.
	 *
	 * The "front" of the box is the side that faces towards the positive-Z axis, the "top"
	 * of the box is the side that faces towards the positive-Y axis, and the "right" side
	 * of the box is the side that faces towards the positive-X axis.
	 *
	 * For the purposes of wrapping the texture around the box, this method assumes that
	 * the texture is an unfolded cube. The box can be created with any relative dimensions,
	 * but if it is not a cube, the texture may appear stretched or shrunk on two or more
	 * sides. The texture will still fully wrap all six sides of the box, but the texture
	 * is stretched or shrunk to fit each side according to its dimension relative to the
	 * other sides. The appearance will be as if you had started with a textured cube and
	 * then pulled one or two of the dimensions out further.
	 *
	 * For higher fidelity in applying textures to non-cube boxes, so that the texture
	 * will not be stretched to fit, use either of the populateAsSolidBox: or
	 * populateAsSolidBox:withCorner: methods, with a texture whose layout is compatible
	 * with the aspect ratio of the box.
	 *
	 * Thanks to Cocos3D user andyman for contributing the prototype code and texture
	 * template file for this method.
	 */
	virtual void				populateAsCubeMappedSolidBox( const CC3Box& box );

	/**
	 * Populates this instance as a simple rectangular box mesh from the specified
	 * bounding box, which contains two of the diagonal corners of the box, and
	 * configures the mesh texture coordinates so that the entire box can be wrapped
	 * in a single texture.
	 *
	 * This mesh can be covered with a solid material or a single texture. If this mesh
	 * is to be covered with a texture, use the texture property of this node to set
	 * the texture. If a solid color is desired, leave the texture property unassigned.
	 *
	 * The vertexContentType property of this mesh node may be set prior to invoking this method,
	 * to define the content type for each vertex. Content types kCC3VertexContentLocation,
	 * kCC3VertexContentNormal, and kCC3VertexContentTextureCoordinate are populated by this method.
	 *
	 * If the vertexContentType property has not already been set, that property is set to a value
	 * of (kCC3VertexContentLocation | kCC3VertexContentNormal | kCC3VertexContentTextureCoordinate).
	 * and the mesh will be populated with location, normal and texture coordinates for each vertex.
	 *
	 * If a texture is to be wrapped around this mesh, since the single texture is wrapped
	 * around all six sides of the box, the texture will be mapped according to the layout
	 * illustrated in the texture file BoxTexture.png, included in the distribution.
	 *
	 * The "front" of the box is the side that faces towards the positive-Z axis, the
	 * "top" of the box is the side that faces towards the positive-Y axis, and the
	 * "right" side of the box is the side that faces towards the positive-X axis.
	 *
	 * For the purposes of wrapping the texture around the box, the corner argument
	 * specifies the relative point in the texture that will map to the corner of the
	 * box that is at the juncture of the "left", "front" and "bottom" sides (see the
	 * BoxTexture.png image for a better understanding of this point). The corner
	 * argument is specified as a fraction in each of the S & T dimensions of the texture.
	 * In the CGPoint that specifies the corner, the x & y elements of the CGPoint
	 * correspond to the S & T dimensions of this left-front-bottom corner mapping,
	 * with each value being between zero and one.
	 *
	 * Since, by definition, opposite sides of the box have the same dimensions, this
	 * single corner point identifies the S & T dimensions of all six of the sides of
	 * the box. A value of (1/4, 1/3) for the corner is used when the box is a cube.
	 * A smaller value for the x-element would move the corner to the left in the
	 * texture layout, indicating that the left and right sides are shallower than
	 * they are in a cube, and that the front and back are wider than in a cube, and
	 * vice-versa for a larger value in the x-element of the corner. Similarly for
	 * the y-element. A y-element that is smaller than 1/3, moves the corner point
	 * downwards on the texture, indicating that the bottom and top are shallower than
	 * they are in a cube, or that the front and back are higher than they are in a cube.
	 *
	 * The two axes defined by the corner are interrelated, because the sides need to
	 * be the same depth as the top and bottom. The best way to determine the values to
	 * use in the corner is to use the measure of this point (where the "left", "front",
	 * and "bottom" sides meet) from the layout of the texture. If the aspect of the
	 * corner on the texture does not align with the aspect of the width, height and
	 * depth of the box, the texture will appear stretched on one or two sides relative
	 * to the others.
	 *
	 * Thanks to Cocos3D user andyman for contributing the prototype code and texture
	 * template file for this method.
	 */
	virtual void				populateAsSolidBox( const CC3Box& box, const CCPoint& corner );

	/**
	 * Populates this instance as a wire-frame box with the specified dimensions.
	 *
	 * As this node is translated, rotate and scaled, the wire-frame box will be re-oriented
	 * in 3D space.
	 *
	 * This is a convenience method for creating a simple, but useful, shape.
	 */
	virtual void				populateAsWireBox( const CC3Box& box );

	/**
	 * Populates this instance as a spherical mesh of the specified radius, centered at the origin.
	 *
	 * The surface of the sphere is divided into many smaller divisions, similar to latitude and
	 * longtitude divisions. The sphere mesh contains two poles, where the surface intersects the
	 * positive and negative Y-axis.
	 *
	 * The divsPerAxis argument indicates how to divide the surface of the sphere into divisions.
	 * The X element of the divsPerAxis argument indicates how many longtitude divisions will
	 * occur around one circumnavigation of the equator. The Y element of the divsPerAxis argument
	 * indicates how many latitude divisions will occur between the north pole and the south pole.
	 * 
	 * For example, a value of {12,8} for the divsPerAxis argument will result in the sphere being
	 * divided into twelve divisions of longtitude around the equator, and eight divisions of
	 * latitude between the north and south poles.
	 *
	 * Except at the poles, each division is roughly trapezoidal and is drawn as two triangles.
	 * At the poles, each division is a single triangle.
	 *
	 * This mesh can be covered with a solid material or a single texture. If this mesh
	 * is to be covered with a texture, use the texture property of this node to set
	 * the texture. If a solid color is desired, leave the texture property unassigned.
	 *
	 * The vertexContentType property of this mesh node may be set prior to invoking this method,
	 * to define the content type for each vertex. Content types kCC3VertexContentLocation,
	 * kCC3VertexContentNormal, and kCC3VertexContentTextureCoordinate are populated by this method.
	 *
	 * If the vertexContentType property has not already been set, that property is set to a value
	 * of (kCC3VertexContentLocation | kCC3VertexContentNormal | kCC3VertexContentTextureCoordinate).
	 * and the mesh will be populated with location, normal and texture coordinates for each vertex.
	 *
	 * If a texture is applied to this mesh, it is mapped to the sphere with a simple cylindrical
	 * projection around the equator (similar to Mercator projection without the north-south stretching).
	 * This type of projection is typical of maps of the earth taken from space, and results in the
	 * smooth curving of any texture around the sphere from the equator to the poles. Texture wrapping
	 * begins at the negative Z-axis, so the center of the texture will be positioned at the point where
	 * the sphere intersects the positive Z-axis, and the conceptual seam (where the left and right
	 * edges of the texture are stitched together) will occur where the sphere intersects the plane
	 * (X = 0) along the negative-Z axis. This texture orientation means that the center of the texture
	 * will face the forwardDirection of the sphere node.
	 *
	 * The boundingVolume of this node is automatically set to a spherical shape (an instance of 
	 * CC3NodeSphericalBoundingVolume) to match the shape of this mesh.
	 */
	virtual void				populateAsSphereWithRadius( GLfloat radius, const CC3Tessellation& divsPerAxis );

	/**
	 * Populates this instance as a conical mesh of the specified radius and height.
	 *
	 * The mesh is constructed so that the base of the cone is centered on the origin of the X-Z plane,
	 * and the apex is on the positive Y-axis at the specified height. The cone is open and does not
	 * have a bottom.
	 *
	 * The surface of the cone is divided into many smaller divisions, as specified by the
	 * angleAndHeightsDivs parameter. The X-coordinate of this parameter indicates how many angular
	 * divisions are created around the circumference of the base, and the Y-coordinate of this
	 * parameter indicates how many vertical divisions are created between the base and the apex.
	 * 
	 * For example, a value of {12,8} for the angleAndHeightsDivs parameter will result in a cone with
	 * 12 divisions around the circumference of the base, and 8 divisions along the Y-axis to the apex.
	 *
	 * By reducing the number of angular divisions to 3 or 4, you can use this method to create a
	 * tetrahedron or square pyramid, respectively.
	 *
	 * This mesh can be covered with a solid material or a single texture. If this mesh
	 * is to be covered with a texture, use the texture property of this mesh to set
	 * the texture. If a solid color is desired, leave the texture property unassigned.
	 *
	 * The vertexContentType property of this mesh may be set prior to invoking this method,
	 * to define the content type for each vertex. Content types kCC3VertexContentLocation,
	 * kCC3VertexContentNormal, and kCC3VertexContentTextureCoordinate are populated by this method.
	 *
	 * If the vertexContentType property has not already been set, that property is set to a value
	 * of (kCC3VertexContentLocation | kCC3VertexContentNormal | kCC3VertexContentTextureCoordinate).
	 * and the mesh will be populated with location, normal and texture coordinates for each vertex.
	 *
	 * If a texture is applied to this mesh, it is mapped to the cone with a simple horizontal
	 * projection. Horizontal lines in the texture will remain parallel, but vertical lines will
	 * converge at the apex. Texture wrapping begins at the negative Z-axis, so the center of the
	 * texture will be positioned at the point where the cone intersects the positive Z-axis, and
	 * the conceptual seam (where the left and right edges of the texture are stitched together)
	 * will occur where the cone intersects the negative-Z axis. This texture orientation means
	 * that the center of the texture will face the forwardDirection of the cone node.
	 */
	virtual void				populateAsHollowConeWithRadius( GLfloat radius, GLfloat height, const CC3Tessellation& angleAndHeightDivs );

	/**
	 * Populates this instance as a line strip with the specified number of vertex points.
	 * The data for the points that define the end-points of the lines are contained within the
	 * specified vertices array. The vertices array must contain at least vertexCount elements.
	 *
	 * The lines are specified and rendered as a strip, where each line is connected to the
	 * previous and following lines. Each line starts at the point where the previous line ended,
	 * and that point is defined only once in the vertices array. Therefore, the number of lines
	 * drawn is equal to one less than the specified vertexCount.
	 * 
	 * The shouldRetainVertices flag indicates whether the data in the vertices array should
	 * be retained by this instance. If this flag is set to YES, the data in the vertices array
	 * will be copied to an internal array that is managed by this instance. If this flag is
	 * set to NO, the data is not copied internally and, instead, a reference to the vertices
	 * data is established. In this case, it is up to you to manage the lifespan of the data
	 * contained in the vertices array.
	 *
	 * If you are defining the vertices data dynamically in another method, you may want to
	 * set this flag to YES to have this instance copy and manage the data. If the vertices
	 * array is a static array, you can set this flag to NO.
	 *
	 * This is a convenience method for creating a simple, but useful, shape.
	 */
	virtual void				populateAsLineStripWith( GLuint vertexCount, CC3Vector* vertices, bool shouldRetainVertices );

	virtual void				prewarmForShadowVolumes();
	virtual void				addShadowVolumesForLight( CC3Light* aLight );

	virtual void				applyMaterialWithVisitor( CC3NodeDrawingVisitor* visitor );
	virtual void				cleanupDrawingParameters( CC3NodeDrawingVisitor* visitor );
	/** The Cocos2D CCNode will supply its own shaders, but still need shader during node picking. */
	virtual void				applyShaderProgramWithVisitor( CC3NodeDrawingVisitor* visitor );

	virtual void				alignTextureUnits();
	virtual void				alignTextureUnit( GLuint texUnit );
	/** Align the material properties to those of the mesh. */
	virtual void				alignMaterialAndMesh();

	virtual void				updateLightPosition();

	/**
	 * Returns a bounding volume that first checks against the spherical boundary, and then checks
	 * against a bounding box. The spherical boundary is fast to check, but is not as accurate as
	 * the bounding box for many meshes. The bounding box is more accurate, but is more expensive
	 * to check than the spherical boundary. The bounding box is only checked if the spherical
	 * boundary does not indicate that the mesh is outside the frustum.
	 */
	CC3NodeBoundingVolume*		defaultBoundingVolume();
	virtual bool				shouldDrawInClipSpace();
	virtual void				setShouldDrawInClipSpace( bool shouldClip );
	virtual bool				shouldCullBackFaces();
	virtual void				setShouldCullBackFaces( bool shouldCull );
	virtual bool				shouldCullFrontFaces();
	virtual void				setShouldCullFrontFaces( bool shouldCull );
	virtual bool				shouldUseClockwiseFrontFaceWinding();
	virtual void				setShouldUseClockwiseFrontFaceWinding( bool shouldWindCW );
	virtual bool				shouldUseSmoothShading();
	virtual void				setShouldUseSmoothShading( bool shouldSmooth );
	virtual bool				shouldCastShadowsWhenInvisible();
	virtual void				setShouldCastShadowsWhenInvisible( bool shouldCast );
	virtual CC3NormalScaling	getNormalScalingMethod();
	virtual void				setNormalScalingMethod( CC3NormalScaling nsMethod );
	virtual bool				shouldDisableDepthMask();
	virtual void				setShouldDisableDepthMask( bool shouldDisable );

	virtual bool				shouldDisableDepthTest();
	virtual void				setShouldDisableDepthTest( bool shouldDisable );
	virtual GLenum				getDepthFunction();
	virtual void				setDepthFunction( GLenum depthFunc );
	virtual GLfloat				getDecalOffsetFactor();
	virtual void				setDecalOffsetFactor( GLfloat factor );
	virtual GLfloat				getDecalOffsetUnits();
	virtual void				setDecalOffsetUnits( GLfloat units );
	virtual CC3Vector4			getGlobalLightPosition();
	virtual void				setGlobalLightPosition( const CC3Vector4& aPosition );
	virtual void				selectShaders();
	virtual void				removeShaders();
	virtual ccColor3B			getColor();
	virtual void				setColor( const ccColor3B& color );
	virtual bool				shouldBlendAtFullOpacity();
	virtual void				setShouldBlendAtFullOpacity( bool shouldBlend );
	virtual bool				shouldApplyOpacityAndColorToMeshContent();
	virtual void				setShouldApplyOpacityAndColorToMeshContent( bool shouldApply );

	virtual GLfloat				getLineWidth();
	virtual void				setLineWidth( GLfloat aLineWidth );
	virtual bool				shouldSmoothLines();
	virtual void				setShouldSmoothLines( bool shouldSmooth );
	virtual GLenum				getLineSmoothingHint();
	virtual void				setLineSmoothingHint( GLenum aHint );
	virtual bool				hasPremultipliedAlpha();
	virtual void				initWithTag( GLuint aTag, const std::string& aName );
	virtual void				populateFrom( CC3MeshNode* another );
	virtual void				createGLBuffers();
	virtual void				deleteGLBuffers();
	virtual void				releaseRedundantContent();
	virtual void				retainVertexContent();
	virtual void				retainVertexLocations();
	virtual void				retainVertexNormals();
	virtual void				retainVertexTangents();

	virtual void				retainVertexBitangents();
	virtual void				retainVertexColors();
	virtual void				retainVertexBoneIndices();
	virtual void				retainVertexBoneWeights();
	virtual void				retainVertexTextureCoordinates();
	virtual void				retainVertexIndices();
	virtual void				doNotBufferVertexContent();
	virtual void				doNotBufferVertexLocations();
	virtual void				doNotBufferVertexNormals();
	virtual void				doNotBufferVertexTangents();
	virtual void				doNotBufferVertexBitangents();
	virtual void				doNotBufferVertexColors();
	virtual void				doNotBufferVertexBoneIndices();
	virtual void				doNotBufferVertexBoneWeights();
	virtual void				doNotBufferVertexTextureCoordinates();
	virtual void				doNotBufferVertexIndices();
	virtual bool				isMeshNode();

	virtual CCObject*			copyWithZone( CCZone* zone );

	/** Template method to draw the mesh to the GL engine. */
	virtual void				drawMeshWithVisitor( CC3NodeDrawingVisitor* visitor );
	/**
	 * Template method to configure the drawing parameters.
	 *
	 * Subclasses may override to add additional drawing parameters.
	 */
	virtual void				configureDrawingParameters( CC3NodeDrawingVisitor* visitor );
	/**
	 * Template method configures GL scaling of normals, based on
	 * whether the scaling of this node is uniform or not.
	 */
	virtual void				configureNormalization( CC3NodeDrawingVisitor* visitor );
	/**
	 * Template method configures GL face culling based on the shouldCullBackFaces,
	 * shouldCullBackFaces, and shouldUseClockwiseFrontFaceWinding properties.
	 */
	virtual void				configureFaceCulling( CC3NodeDrawingVisitor* visitor );

	/**
	 * Configures the GL state for smooth shading, and to support vertex coloring.
	 * This must be invoked every time, because both the material and mesh influence
	 * the colorMaterial property, and the mesh will not be re-bound if it does not
     * need to be switched. And this method must be invoked before material colors
     * are set, otherwise material colors will not stick.
    */
	virtual void				configureColoring( CC3NodeDrawingVisitor* visitor );

	/**
	 * Template method disables depth testing and/or writing to the depth buffer if the
	 * shouldDisableDepthTest and shouldDisableDepthMask property is set to YES, respectively,
	 * and set the depth function.
	 */
	virtual void				configureDepthTesting( CC3NodeDrawingVisitor* visitor );
	/**
	 * Template method that establishes the decal offset parameters to cause the depth
	 * of the content being drawn to be offset relative to the depth of the content
	 * that has already been drawn.
	 */
	void						configureDecalParameters( CC3NodeDrawingVisitor* visitor );

	/** Template method to configure line drawing properties. */
	virtual void				configureLineProperties( CC3NodeDrawingVisitor* visitor );

	virtual CC3Vector			getCenterOfGeometry();
	virtual CC3Vector			getLocalContentCenterOfGeometry();
	virtual CC3Box				getLocalContentBoundingBox();
	virtual void				flipNormals();

protected:
	CC3Mesh*					m_pMesh;
	CC3Material*				m_pMaterial;
	CC3ShaderContext*			m_pShaderContext;
	std::string					m_renderStreamGroupMarker;
	GLenum						m_depthFunction;
	GLfloat						m_decalOffsetFactor;
	GLfloat						m_decalOffsetUnits;
	GLfloat						m_lineWidth;
	GLenum						m_lineSmoothingHint;

	CC3NormalScaling			m_normalScalingMethod : 4;
	bool						m_shouldUseLightProbes : 1;
	bool						m_shouldSmoothLines : 1;
	bool						m_shouldDisableDepthMask : 1;
	bool						m_shouldDisableDepthTest : 1;
	bool						m_shouldCullFrontFaces : 1;
	bool						m_shouldCullBackFaces : 1;
	bool						m_shouldDrawInClipSpace : 1;
	bool						m_shouldUseClockwiseFrontFaceWinding : 1;
	bool						m_shouldUseSmoothShading : 1;
	bool						m_shouldCastShadowsWhenInvisible : 1;
	bool						m_shouldApplyOpacityAndColorToMeshContent : 1;
	bool						m_hasRigidSkeleton : 1;		// Used by skinned mesh node subclasses
};

NS_COCOS3D_END

#endif