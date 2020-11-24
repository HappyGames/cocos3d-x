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
#ifndef _CC3_NODES_RESOURCE_H_
#define _CC3_NODES_RESOURCE_H_

NS_COCOS3D_BEGIN
/**
 * CC3NodesResource is a wrapper class around a resource structure loaded from a data file
 * containing 3D resources. It handles loading model and texture data from files, and
 * creating CC3Nodes from that data.
 *
 * The loadFromFile: method is used to load this resource. Once this method has been
 * successfully invoked, access to 3D data objects loaded from the file is through
 * the nodes property, which contains the root nodes of a structural 3D node assembly
 * constructed from the 3D data loaded from the file.
 *
 * As shortcuts, there are also class and instance initialization methods that will
 * invoke the loadFromFile: method automatically during instance initialization.
 *
 * However, before using any of these shortcut methods, you should take into consideration
 * whether you need to set the directory or expectsVerticallyFlippedTextures properties
 * prior to loading, as explained here.
 *
 * By default, additional resources (typically textures), are loaded from the same
 * directory that the file containing the content of this resource is located.
 * If this is not the case, you can set the directory property prior to invoking
 * the loadFromFile: method, in order to establish another directory from which
 * additional resources such as textures will be loaded.
 *
 * You do not need to set the directory property if these additional resources
 * are in the same directory as the file loaded by this resource.
 *
 * Under iOS, a texture whose width and height are not each a power-of-two, will be converted
 * to a size whose width and height are a power-of-two. The result is a texture that can have
 * empty space on the top and right sides. If the texture coordinates of the mesh do not take
 * this into consideration, the result will be that only the lower left of the mesh will be
 * covered by the texture.
 * 
 * In addition, the vertical axis of the coordinate system of OpenGL is inverted
 * relative to the iOS view coordinate system. This results in textures being
 * displayed upside-down, relative to the OpenGL coordinate system.
 *
 * This instance will automatically adjust the meshes to compensate for this. Meshes loaded
 * by this resource loader will have their texture coordinates adjusted to align with the usable
 * area of an NPOT texture, and to vertically flip a texture that has been loaded upside-down.
 *
 * To determine whether textures will need to be vertically flipped, the loader needs to know
 * whether or not the meshes have already been flipped (by the 3D editor or file exporter).
 * The expectsVerticallyFlippedTextures property can be set to indicate to the loader whether
 * the texture coordinates have already been flipped. If the value of this property needs to
 * be changed, it should be set before the file is loaded.
 *
 * The class-side property defaultExpectsVerticallyFlippedTextures can be
 * used to set all instances to load one way or the other.
 *
 * This is an abstract class. Specific subclasses will load files of specific types.
 *
 * Subclasses must override the primitive template method processFile:. All other
 * loading and initialization methods defined by this class are implemented using
 * this primitive method, and subclasses do not need to override any of these other
 * loading and initialization methods.
 *
 * Subclasses should ensure that the nodes array property is fully populated upon
 * successful completion of the processFile: method.
 */
class CC3NodesResource : public CC3Resource 
{
	DECLARE_SUPER( CC3Resource );
public:
	CC3NodesResource();
	~CC3NodesResource();

	/**
	 * A collection of the root nodes of the node assembly extracted from the file.
	 * Each of these nodes will usually contain child nodes.
	 */
	CCArray*					getNodes();

	/**
	 * Returns a node from the hierarchy under the nodes in the nodes array, that matches the
	 * specified node, or returns nil if no match is found.
	 *
	 * The criteria used to match the node against the contained nodes depends on this resource
	 * and the type of nodes it has loaded. This implementation invokes the getNodeNamed: method
	 * on each node in the nodes array to recursively fetch the node that has the same name as
	 * the specified node. Subclasses that load other types of nodes may be able to define more
	 * efficient searching and matching algorithms.
	 */
	virtual CC3Node*			getNodeMatching( CC3Node* node );

	/**
	 * Adds the specified node to the collection of nodes loaded by this resource.
	 *
	 * This method is used by subclasses during loading to add a node to the nodes collection.
	 *
	 * The application may also use this method to add a node manually, if appropriate. However,
	 * be aware that the nodes collection of a resource object is generally only accessed once
	 * to populate the descendants of a CC3ResourceNode that wraps this resource. In most cases,
	 * it may be more appropriate for the application to add any manually-generated nodes to the
	 * CC3ResourceNode, instead of this resource.
	 */
	virtual void				addNode( CC3Node* node );

	/**
	 * Removes the specified node from the collection of nodes loaded by this resource.
	 *
	 * This method is used by subclasses during loading to remove a node from the nodes collection.
	 *
	 * The application may also use this method to remove a node manually, if appropriate. However,
	 * be aware that the nodes collection of a resource object is generally only accessed once
	 * to populate the descendants of a CC3ResourceNode that wraps this resource. In most cases,
	 * it may be more appropriate for the application to remove any unwanted nodes from the
	 * CC3ResourceNode, instead of this resource.
	 */
	virtual void				removeNode( CC3Node* node );

	/**
	 * Returns a resource instance loaded from the specified file.
	 *
	 * Resources loaded through this method are cached. If the resource was already loaded and
	 * is in the cache, it is retrieved and returned. If the resource has not in the cache, it
	 * is loaded from the specified file, after being configured for texture orientation, placed
	 * into the cache, and returned.
	 *
	 * See the description of the expectsVerticallyFlippedTextures property for more about texture
	 * orientation. See the description of the resourceFromFile: in the superclass for more about
	 * file loading alternatives.
	 *
	 * The specified file path may be either an absolute path, or a path relative to the
	 * application resource directory. If the file is located directly in the application
	 * resources directory, the specified file path can simply be the name of the file.
	 *
	 * This method will return nil if the file is not in the cache and could not be loaded.
	 */
	static CC3NodesResource*	resourceFromFile( const std::string& aFilePath, bool flipped );

	/**
	 * Indicates whether the texture coordinates of the meshes that will be loaded by this
	 * resource loader expect that the texture will be flipped upside-down during texture loading.
	 * 
	 * The vertical axis of the coordinate system of OpenGL is inverted relative to the  CoreGraphics
	 * view coordinate system. As a result, some texture file formats may be loaded upside down. Most
	 * common file formats, including JPG, PNG & PVR are loaded right-way up, but using proprietary
	 * texture formats developed for other platforms may result in textures being loaded upside-down.
	 *
	 * If the value of this property is YES, the texture coordinates of meshes loaded by this resource
	 * loader will be assumed to have already been flipped vertically, (typically by the 3D editor or
	 * file exporter) to align with textures that will be vertically flipped by the texture loader.
	 *
	 * If the value of this property is NO, the texture coordinates of meshes loaded by this resource
	 * loader will be assumed to have their original orientation, and aligned with textures that have
	 * not been vertically flipped by the texture loader.
	 *
	 * The value of this property is then used to cause the meshes to automatically
	 * correctly align themselves with the orientation of any texture applied to them.
	 *
	 * For meshes that are based on vertex arrays, this property is used to set the same property on
	 * each CC3VertexTextureCoordinates instance created and loaded by this resource. When a texture
	 * is assigned to cover the mesh, the value of that CC3VertexTextureCoordinates property is used
	 * in combination with the value of the isUpsideDown property of a texture to determine
	 * whether the texture coordinates should automatically be reoriented when displaying that texture.
	 * 
	 * The initial value of this property is determined by the value of the class-side
	 * defaultExpectsVerticallyFlippedTextures property at the time an instance of this class
	 * is created and initialized. If you want all meshes to behave the same way, with respect
	 * to this property, set the value of that class-side property.
	 */
	bool						expectsVerticallyFlippedTextures();
	void						setExpectsVerticallyFlippedTextures( bool expects );

	/**
	 * This class-side property determines the initial value of the expectsVerticallyFlippedTextures
	 * property when an instance of this class is created and initialized.
	 *
	 * See the notes for that property for more information.
	 *
	 * The initial value of this class-side property is NO.
	 */
	static bool					defaultExpectsVerticallyFlippedTextures();

	/**
	 * This class-side property determines the initial value of the expectsVerticallyFlippedTextures
	 * property when an instance of this class are created and initialized.
	 *
	 * See the notes for that property for more information.
	 *
	 * The initial value of this class-side property is NO.
	 */
	static void					setDefaultExpectsVerticallyFlippedTextures( bool expectsFlipped );

	/**
	 * Indicates whether any nodes that do not contain animation should be frozen to their initial
	 * location, quaternion, and scale properties when animation is run.
	 *
	 * Setting this property to YES has the same effect as invoking freezeIfInanimateOnTrack:
	 * on each node, immediately after loading the node. See the notes for that method to learn
	 * more about freezing inanimate nodes during animation.
	 *
	 * The initial value of this property is determined by the value of the class-side
	 * defaultShouldFreezeInanimateNodes property at the time of instantiation.
	 */
	bool						shouldFreezeInanimateNodes();
	void						setShouldFreezeInanimateNodes( bool shouldFreeze );

	/**
	 * Indicates the initial value to which the shouldFreezeInanimateNodes property will be set
	 * for each new instance of this class.
	 *
	 * The initial value of this class-side property is NO.
	 */
	static bool					defaultShouldFreezeInanimateNodes();

	/**
	 * Indicates the initial value to which the shouldFreezeInanimateNodes property will be set
	 * for each new instance of this class.
	 *
	 * The initial value of this class-side property is NO.
	 */
	static void					setDefaultShouldFreezeInanimateNodes( bool shouldFreeze );

	virtual bool				init();
	static CC3NodesResource*	resource();

protected:
	CCArray*					m_nodes;
	bool						m_expectsVerticallyFlippedTextures : 1;
	bool						m_shouldFreezeInanimateNodes : 1;
};

NS_COCOS3D_END

#endif
