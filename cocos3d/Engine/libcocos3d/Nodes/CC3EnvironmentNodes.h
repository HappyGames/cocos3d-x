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
#ifndef _CC3_ENVIRONMENT_NODES_H_
#define _CC3_ENVIRONMENT_NODES_H_

NS_COCOS3D_BEGIN

/**
 * CC3EnvironmentNode is an abstract superclass of a family of node classes that hold a 
 * texture that can be used as an environment map by other nodes.
 *
 * Different subclasses provide specialized types of environment maps, such as light probes
 * and reflection surfaces.
 *
 * Environment maps require shaders to interpret the contents of the texture, and are 
 * therefore not compatible with OpenGL ES 1.1, and instances of CC3EnvironmentNode will
 * have no effect if included in a scene while running under OpenGL ES 1.1.
 */
class CC3EnvironmentNode : public CC3Node 
{
	DECLARE_SUPER( CC3Node );
public:
	CC3EnvironmentNode();
	~CC3EnvironmentNode();
	/** 
	 * The texture that provides the environment map.
	 *
	 * Typically, this texture is a cube-map, to provide a map in all six directions.
	 */
	CC3Texture*					getTexture();
	void						setTexture( CC3Texture* texture );

	/** Initializes this instance with the specified name and environment texture. */
	void						initWithName( const std::string& name, CC3Texture* texture );
	void						initWithName( const std::string& name );

	/** Allocates and initializes an autoreleased instance with the specified name and environment texture. */
	static CC3EnvironmentNode*	nodeWithName( const std::string& name, CC3Texture* texture );

	/**
	 * Initializes this instance with the specified texture.
	 *
	 * The name of this instance will be set to that of the specified texture.
	 */
	void						initWithTexture( CC3Texture* texture );

	/**
	 * Allocates and initializes an autoreleased instance with the specified texture.
	 *
	 * The name of the returned instance will be set to that of the specified texture.
	 */
	static CC3EnvironmentNode*	nodeWithTexture( CC3Texture* texture );
	virtual bool				isLightProbe();
	void						initWithTag( GLuint aTag, const std::string& aName );

protected:
	CC3Texture*					m_pTexture;
};

/**
 * CC3LightProbe is a type of light that uses a texture to define the
 * light intensity in any direction at the light's location.
 */
class CC3LightProbe : public CC3EnvironmentNode 
{
	DECLARE_SUPER( CC3EnvironmentNode );
public:
	/**
	 * The diffuse color of this light.
	 *
	 * The initial value of this propery is kCCC4FWhite.
	 */
	ccColor4F					getDiffuseColor();
	void						setDiffuseColor( const ccColor4F& color );

	virtual bool				isLightProbe();
	void						initWithTag( GLuint aTag, const std::string& aName );

	static CC3LightProbe*		nodeWithTexture( CC3Texture* texture );

protected:
	ccColor4F					m_diffuseColor;
};

NS_COCOS3D_END

#endif
