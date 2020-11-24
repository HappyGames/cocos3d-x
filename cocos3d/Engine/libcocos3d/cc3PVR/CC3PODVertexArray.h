/*
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
/** @file */	// Doxygen marker
#ifndef _CC3_POD_VERTEX_ARRAY_H_
#define _CC3_POD_VERTEX_ARRAY_H_

NS_COCOS3D_BEGIN

class CC3PODVertexFactory
{
public:
	static CC3VertexLocations*			createVertexLocations( CC3PODResource* aPODRez, GLint aPODIndex );
	static CC3VertexIndices*			createVertexIndices( CC3PODResource* aPODRez, GLint aPODIndex );
	static CC3VertexNormals*			createVertexNormals( CC3PODResource* aPODRez, GLint aPODIndex );
	static CC3VertexTangents*			createVertexTangents( CC3PODResource* aPODRez, GLint aPODIndex, bool bitangents = false );
	static CC3VertexColors*				createVertexColors( CC3PODResource* aPODRez, GLint aPODIndex );
	static CC3VertexBoneWeights*		createVertexBoneWeights( CC3PODResource* aPODRez, GLint aPODIndex );
	static CC3VertexBoneIndices*		createVertexBoneIndices( CC3PODResource* aPODRez, GLint aPODIndex );
	static CC3VertexTextureCoordinates*	createVertexTextureCoordinates( CC3PODResource* aPODRez, GLint aPODIndex, GLint texUnit );
};

NS_COCOS3D_END

#endif
