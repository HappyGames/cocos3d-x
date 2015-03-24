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
#ifndef _CC3_RESOURCENODE_H_
#define _CC3_RESOURCENODE_H_

NS_COCOS3D_BEGIN

/**
 * A CC3ResourceNode is a CC3Node that that can be populated from a CC3NodesResource, and
 * forms the root of the node structural assembly loaded from a resource file.
 *
 * This is an abstract class, and subclasses are specialized for loading different types
 * of resource files.
 *
 * A subclass instance can be populated in one of several ways:
 *   - The most common way is to invoke one of the initialization methods that specify a
 *     path to a resource file.
 *   - Or, an instance can be instantiated, and then populated by invoking one of the
 *     loadFromFile:... methods.
 *   - Or, if a compatible resource has already been loaded, this instance can be instantiated,
 *     and then populated using the populateFromResource: method.
 *
 * Under iOS, a texture whose width and height are not each a power-of-two, will be converted
 * to a size whose width and height are a power-of-two. The result is a texture that can have
 * empty space on the top and right sides. If the texture coordinates of the mesh do not take
 * this into consideration, the result will be that only the lower left of the mesh will be
 * covered by the texture.
 * 
 * In addition, the vertical axis of the coordinate system of OpenGL is inverted relative 
 * to the iOS view coordinate system. This results in textures being displayed upside-down,
 * relative to the OpenGL coordinate system.
 *
 * The CC3NodesResource that actually loads the file content will automatically adjust the 
 * meshes to compensate for this. Meshes loaded by this resource loader will have their 
 * texture coordinates adjusted to align with the usable area of an NPOT texture, and to
 * vertically flip a texture that has been loaded upside-down.
 *
 * To determine whether textures will need to be vertically flipped, the loader needs to know
 * whether or not the meshes have already been flipped (by the 3D editor or file exporter).
 * The initialization and loading methods have an option to pass an indication of whether
 * the texture coordinates have already been flipped.
 */
class CC3ResourceNode : public CC3Node
{
	DECLARE_SUPER( CC3Node );
public:
	CC3ResourceNode();
	~CC3ResourceNode();

	/**
	 * Populates this instance from the specified resource, which must be of the type specified
	 * by the resourceClass property.
	 *
	 * This method removes all child nodes of this instance and replaces them with the nodes
	 * extracted from the nodes property of the specified resource.
	 *
	 * If this node has not yet been assigned a name, it will be set to the name of the specified resource.
	 *
	 * The userData property of this node will be set to the userData property of the resource.
	 *
	 * This method is automatically invoked by the loadFromFile:... methods, and in turn, from
	 * any of the initialization methods that load content from a file.
	 *
	 * Subclass may override to extract additional content from the resource.
	 */
	virtual void				populateFromResource( CC3NodesResource* resource );
		
	/**
	 * Loads the file at the specified file path, extracts the loaded CC3Nodes from the resource,
	 * and adds them as child nodes to this resource node.
	 *
	 * The specified file path may be either an absolute path, or a path relative to the application
	 * resource directory. If the file is located directly in the application resources directory,
	 * the specified file path can simply be the name of the file.
	 *
	 * This method invokes the resourceFromFile: class method on the class returned by the resourceClass
	 * property, and sets the resource property of this node to the returned instance.
	 *
	 * If not already set, the name of this node will be set to that of the resource, which is
	 * usually the name of the file loaded.
	 */
	virtual bool				loadFromFile( const std::string& aFilepath );

	/**
	 * Loads the file at the specified file path, extracts the loaded CC3Nodes from the resource,
	 * and adds them as child nodes to this resource node.
	 *
	 * The specified file path may be either an absolute path, or a path relative to the application
	 * resource directory. If the file is located directly in the application resources directory,
	 * the specified file path can simply be the name of the file.
	 *
	 * This method invokes the resourceFromFile:expectsVerticallyFlippedTextures: class method
	 * on the class returned by the resourceClass property, and sets the resource property of 
	 * this node to the returned instance.
	 *
	 * If not already set, the name of this node will be set to that of the resource, which is
	 * usually the name of the file loaded.
	 */
	virtual bool				loadFromFile( const std::string& aFilepath, bool flipped );

	/**
	 * Initializes this instance, loads the file at the specified file path, extracts the loaded
	 * CC3Nodes from the resource, and adds them as child nodes to this resource node.
	 *
	 * The specified file path may be either an absolute path, or a path relative to the application
	 * resource directory. If the file is located directly in the application resources directory,
	 * the specified file path can simply be the name of the file.
	 *
	 * This method invokes the loadFromFile: method to load the file.
	 *
	 * The name of this node will be set to that of the resource, which is usually the name of
	 * the file loaded.
	 */
	virtual bool				initFromFile( const std::string& aFilepath );

	/**
	 * Allocates and initializes an autoreleased instance, loads the file at the specified file path,
	 * extracts the loaded CC3Nodes from the resource, and adds them as child nodes to this resource node.
	 *
	 * The specified file path may be either an absolute path, or a path relative to the application
	 * resource directory. If the file is located directly in the application resources directory,
	 * the specified file path can simply be the name of the file.
	 *
	 * This method invokes the loadFromFile: method to load the file.
	 *
	 * The name of this node will be set to that of the resource, which is usually the name of
	 * the file loaded.
	 */
	static CC3ResourceNode*		nodeFromFile( const std::string& aFilepath );

	/**
	 * Initializes this instance, loads the file at the specified file path, extracts the loaded
	 * CC3Nodes from the resource, and adds them as child nodes to this resource node.
	 *
	 * The specified file path may be either an absolute path, or a path relative to the application
	 * resource directory. If the file is located directly in the application resources directory,
	 * the specified file path can simply be the name of the file.
	 *
	 * This method invokes the loadFromFile:expectsVerticallyFlippedTextures: method to load the file.
	 *
	 * The name of this node will be set to that of the resource, which is usually the name of
	 * the file loaded.
	 */
	virtual bool				initFromFile( const std::string& aFilepath, bool flipped );

	/**
	 * Allocates and initializes an autoreleased instance, loads the file at the specified file path,
	 * extracts the loaded CC3Nodes from the resource, and adds them as child nodes to this resource node.
	 *
	 * The specified file path may be either an absolute path, or a path relative to the application
	 * resource directory. If the file is located directly in the application resources directory,
	 * the specified file path can simply be the name of the file.
	 *
	 * This method invokes the loadFromFile:expectsVerticallyFlippedTextures: method to load the file.
	 *
	 * The name of this node will be set to that of the resource, which is usually the name of
	 * the file loaded.
	 */
	static CC3ResourceNode*		nodeFromFile( const std::string& aFilepath, bool flipped );

	/**
	 * Initializes this instance, loads the file at the specified file path, extracts the loaded
	 * CC3Nodes from the resource, and adds them as child nodes to this resource node.
	 *
	 * The specified file path may be either an absolute path, or a path relative to the
	 * application resource directory. If the file is located directly in the application
	 * resources directory, the specified file path can simply be the name of the file.
	 *
	 * The name of this node will be set to the specified name.
	 */
	virtual bool				initWithName( const std::string& aName, const std::string& aFilepath );

	/**
	 * Allocates and initializes an autoreleased instance, loads the file at the specified file path,
	 * extracts the loaded CC3Nodes from the resource, and adds them as child nodes to this resource node.
	 *
	 * The specified file path may be either an absolute path, or a path relative to the
	 * application resource directory. If the file is located directly in the application
	 * resources directory, the specified file path can simply be the name of the file.
	 *
	 * The name of this node will be set to the specified name.
	 */
	static CC3ResourceNode*		nodeWithName( const std::string& aName, const std::string& aFilepath );

protected:
	virtual CC3NodesResource*	createResourceFromFile( const std::string& aFilePath );
	virtual CC3NodesResource*	createResourceFromFile( const std::string& aFilePath, bool flipped );
};

NS_COCOS3D_END

#endif
