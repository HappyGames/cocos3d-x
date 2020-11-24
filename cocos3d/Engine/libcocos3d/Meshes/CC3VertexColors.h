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
#ifndef _CC3_VERTEX_COLORS_H_
#define _CC3_VERTEX_COLORS_H_

NS_COCOS3D_BEGIN

/** A CC3VertexArray that manages the per-vertex color aspect of an array of vertices. */
class CC3VertexColors : public CC3VertexArray
{
	DECLARE_SUPER( CC3VertexArray );
public:
	static CC3VertexColors*		vertexArray();
	static CC3VertexColors*		vertexArrayWithName( const std::string& aName );
	/**
	 * Returns the color element at the specified index in the underlying vertex content.
	 *
	 * If the underlying vertex content is not of type GLfloat, the color components are
	 * converted to GLfloat before the color value is returned.
	 *
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * the vertexStride and elementOffset properties to access the correct element.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	ccColor4F					getColor4FAt( GLuint index );

	/**
	 * Sets the color element at the specified index in the underlying vertex content to
	 * the specified color value.
	 *
	 * If the underlying vertex content is not of type GLfloat, the color components are
	 * converted to the appropriate type (typically GLubyte) before being set in the
	 * vertex content.
	 * 
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * the vertexStride and elementOffset properties to access the correct element.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	void						setColor4F( const ccColor4F& aColor, GLuint index );

	/**
	 * Returns the color element at the specified index in the underlying vertex content.
	 *
	 * If the underlying vertex content is not of type GLubyte, the color components are
	 * converted to GLubyte before the color value is returned.
	 *
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * the vertexStride and elementOffset properties to access the correct element.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	ccColor4B					getColor4BAt( GLuint index );
		
	/**
	 * Sets the color element at the specified index in the underlying vertex content to
	 * the specified color value.
	 *
	 * If the underlying vertex content is not of type GLubyte, the color components are
	 * converted to the appropriate type (typically GLfloat) before being set in the
	 * vertex content.
	 * 
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * the vertexStride and elementOffset properties to access the correct element.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	void						setColor4B( const ccColor4B& aColor, GLuint index );

	/**
	 * The color of the vertices of this mesh.
	 *
	 * Querying this property returns the RGB components of the first vertex.
	 *
	 * When setting this property, the RGB values of each vertex are set to the specified color,
	 * without affecting the opacity value of each individual vertex. If the content of this vertex
	 * array has been copied to a GL buffer, that buffer is automatically updated.
	 */
	CCColorRef					getColor();
	void						setColor( const CCColorRef& color );

	/**
	 * The opacity of the vertices in this mesh.
	 *
	 * Querying this property returns the alpha component of the first vertex.
	 *
	 * When setting this property, the alpha values of each vertex is set to the specified
	 * opacity, without affecting the RGB color value of each individual vertex. If the content
	 * of this vertex array has been copied to a GL buffer, that buffer is automatically updated. 
	 */
	CCOpacity					getOpacity();
	void						setOpacity( CCOpacity opacity );

	void						setElementType( GLenum elementType );

	/**
	 * Since material color tracking mucks with both ambient and diffuse material colors under
	 * the covers, we won't really know what the ambient and diffuse material color values will
	 * be when we get back to setting them...so indicate that to the corresponding trackers.
	 */
	void						bindContent( GLvoid* pointer, GLint vaIdx, CC3NodeDrawingVisitor* visitor );

	std::string					getNameSuffix();
	void						initWithTag( GLuint aTag, const std::string& aName );
	GLenum						defaultSemantic();
};

NS_COCOS3D_END

#endif
