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
#ifndef _CC3_POD_RESOURCE_NODE_H_
#define _CC3_POD_RESOURCE_NODE_H_

NS_COCOS3D_BEGIN
/** 
 * A CC3ResourceNode that can be loaded from a PVR POD resource file. 
 *
 * An instance is loaded using an instance of CC3PODResource. The resourceClass property
 * returns CC3PODResource, and the populateFromResource: method expects to be passed an
 * instance of CC3PODResource.
 */
class CC3PODResourceNode : public CC3ResourceNode 
{
	DECLARE_SUPER( CC3ResourceNode );
public:
	CC3PODResourceNode();
	~CC3PODResourceNode();

	/** The number of frames of animation in the POD file. */
	GLuint						getAnimationFrameCount();

	/** The frame rate of animation in the POD file, in frames per second. */
	GLfloat						getAnimationFrameRate();

	void						populateFromResource( CC3NodesResource* resource );

	void						populateFrom( CC3PODResourceNode* another );
	virtual CCObject*			copyWithZone( CCZone* zone ); 

	virtual void				initWithName( const std::string& aName );
	virtual bool				initWithName( const std::string& aName, const std::string& aFilePath );

	static CC3PODResourceNode*	nodeWithName( const std::string& aName, const std::string& aFilePath );
	static CC3PODResourceNode*  nodeWithName( const std::string& aName );
    static CC3PODResourceNode*	nodeFromFile( const std::string& aFilePath );
    static CC3PODResourceNode*	nodeFromFile( const std::string& aFilepath, bool flipped );

protected:
	virtual CC3NodesResource*	createResourceFromFile( const std::string& aFilePath );
	virtual CC3NodesResource*   createResourceFromFile( const std::string& aFilePath, bool flipped );

protected:
	GLuint						_animationFrameCount;
	GLfloat						_animationFrameRate;
};

NS_COCOS3D_END

#endif
