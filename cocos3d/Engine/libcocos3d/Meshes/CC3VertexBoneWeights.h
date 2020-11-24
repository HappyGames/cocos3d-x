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
#ifndef _CC3_VERTEX_BONE_WEIGHTS_H_
#define _CC3_VERTEX_BONE_WEIGHTS_H_


NS_COCOS3D_BEGIN

/**
 * A CC3VertexArray that manages a collection of bone weights for each vertex. Each bone weight
 * indicates how much that particular bone influences the movement of the vertex for a mesh that
 * uses vertex skinning. Vertex skinning is the manipulation of a soft-body mesh under control
 * of a skeleton of bone nodes.
 *
 * For each vertex, the bone to which the weight should be applied is identified by the bone
 * index specified in the corresponding entry in the CC3VertexBoneIndices vertex array.
 *
 * This vertex array works together with an instace of a CC3VertexBoneIndices vertex array.
 * The elementSize property of the two vertex arrays must be equal, and under OpenGL ES 1.1,
 * must not be larger than the maximum number of available bone influences allowed by the 
 * platform, which can be retreived from CC3OpenGL.sharedGL.maxNumberOfBoneInfluencesPerVertex.
*/
class CC3VertexBoneWeights : public CC3VertexArray
{
	DECLARE_SUPER( CC3VertexArray );
public:
	static CC3VertexBoneWeights* vertexArray();
	/**
	 * Returns the weight value, for the specified influence index within the vertex, for the
	 * vertex at the specified index within the underlying vertex content.
	 *
	 * The weight indicates how much a particular bone influences the movement of the particular 
	 * vertex. Several weights are stored for each vertex, one for each bone that influences the 
	 * movement of that vertex. The specified influenceIndex parameter must be between zero, and
	 * the elementSize property (inclusive/exclusive respectively).
	 *
	 * The vertex index refers to vertices, not bytes. The implementation takes into consideration
	 * the vertexStride and elementOffset properties to access the correct element.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	GLfloat						getWeightForBoneInfluence( GLuint influenceIndex, GLuint vtxIndex );

	/**
	 * Sets the weight value, for the specified influence index within the vertex, for the
	 * vertex at the specified index within the underlying vertex content.
	 *
	 * The weight indicates how much a particular bone influences the movement of the particular
	 * vertex. Several weights are stored for each vertex, one for each bone that influences the
	 * movement of that vertex. The specified influenceIndex parameter must be between zero, and
	 * the elementSize property (inclusive/exclusive respectively).
	 *
	 * The vertex index refers to vertices, not bytes. The implementation takes into consideration
	 * the vertexStride and elementOffset properties to access the correct element.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	void						setWeight( GLfloat weight, GLuint influenceIndex, GLuint vtxIndex );

	/**
	 * Returns the weights of all of the bones that influence the movement of the vertex at the
	 * specified index within the underlying vertex content.
	 *
	 * Several weights are stored for each vertex, one for each bone that influences the movement
	 * of the vertex. The number of elements in the returned array is the same for each vertex
	 * in this vertex array, as defined by the elementSize property.
	 *
	 * The vertex index refers to vertices, not bytes. The implementation takes into consideration
	 * the vertexStride and elementOffset properties to access the correct vertices.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	GLfloat*					getBoneWeightsAt( GLuint vtxIndex );

	/**
	 * Sets the weights of all of the bones that influence the movement of the vertex at the
	 * specified index within the underlying vertex content.
	 *
	 * Several weights are stored for each vertex, one for each bone that influences the movement
	 * of the vertex. The number of elements in the specified input array must therefore be at 
	 * least as large as the value of the elementSize property.
	 *
	 * The vertex index refers to vertices, not bytes. The implementation takes into consideration
	 * the vertexStride and elementOffset properties to access the correct vertices.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	void						setBoneWeights( GLfloat* weights, GLuint vtxIndex );

	std::string					getNameSuffix();
	void						initWithTag( GLuint aTag, const std::string& aName );
	GLenum						defaultSemantic();
};

#define CC3VertexWeights CC3VertexBoneWeights

NS_COCOS3D_END

#endif
