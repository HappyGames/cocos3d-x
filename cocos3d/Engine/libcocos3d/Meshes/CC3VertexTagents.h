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
#ifndef _CC3_VERTEX_TANGENTS_H_
#define _CC3_VERTEX_TANGENTS_H_

NS_COCOS3D_BEGIN

/** A CC3VertexArray that manages the tangent or bitangent aspect of an array of vertices. */
class CC3VertexTangents : public CC3VertexArray
{
public:
	static CC3VertexTangents*	vertexArray();
	/**
	 * Returns the tangent element at the specified index in the underlying vertex content.
	 *
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * the vertexStride and elementOffset properties to access the correct element.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	CC3Vector					getTangentAt( GLuint index );

	/**
	 * Sets the tangent element at the specified index in the underlying vertex content to
	 * the specified tangent value.
	 *
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * the vertexStride and elementOffset properties to access the correct element.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	void						setTangent( const CC3Vector& aTangent, GLuint index );

	std::string					getNameSuffix();
	GLenum						defaultSemantic();
};

NS_COCOS3D_END

#endif
