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
#include "cocos3d.h"

NS_COCOS3D_BEGIN

CC3VertexBoneIndices* CC3VertexBoneIndices::vertexArray()
{
	CC3VertexBoneIndices* indices = new CC3VertexBoneIndices;
	indices->init();
	indices->autorelease();

	return indices;
}

GLuint CC3VertexBoneIndices::getBoneIndexForBoneInfluence( GLuint influenceIndex, GLuint vtxIndex )
{
	if (_elementType == GL_UNSIGNED_BYTE) 
	{
		GLubyte* boneIndices = (GLubyte*)getAddressOfElement(vtxIndex);
		return boneIndices[influenceIndex];
	} 
	else
	{
		GLushort* boneIndices = (GLushort*)getAddressOfElement(vtxIndex);
		return boneIndices[influenceIndex];
	}
}

void CC3VertexBoneIndices::setBoneIndex( GLuint boneIndex, GLuint influenceIndex, GLuint vtxIndex )
{
	if (_elementType == GL_UNSIGNED_BYTE) 
	{
		GLubyte* boneIndices = (GLubyte*)getAddressOfElement(vtxIndex);
		boneIndices[influenceIndex] = boneIndex;
	} else {
		GLushort* boneIndices = (GLushort*)getAddressOfElement(vtxIndex);
		boneIndices[influenceIndex] = boneIndex;
	}
}

GLvoid* CC3VertexBoneIndices::getBoneIndicesAt( GLuint vtxIndex )
{
	return getAddressOfElement(vtxIndex); 
}

void CC3VertexBoneIndices::setBoneIndices( GLvoid* boneIndices, GLuint vtxIndex )
{
	GLint numMtx = getElementSize();
	if (_elementType == GL_UNSIGNED_BYTE) 
	{
		GLubyte* vtxBoneIndices = (GLubyte*)getAddressOfElement(vtxIndex);
		for (int i = 0; i < numMtx; i++) vtxBoneIndices[i] = ((GLubyte*)boneIndices)[i];
	} else {
		GLushort* vtxBoneIndices = (GLushort*)getAddressOfElement(vtxIndex);
		for (int i = 0; i < numMtx; i++) vtxBoneIndices[i] = ((GLushort*)boneIndices)[i];
	}
}

std::string CC3VertexBoneIndices::getNameSuffix()
{
	return "BoneIndices"; 
}

void CC3VertexBoneIndices::initWithTag( GLuint aTag, const char* aName )
{
	super::initWithTag( aTag, aName );
	{
		_elementType = GL_UNSIGNED_BYTE;
		_elementSize = 0;
	}
}

GLenum CC3VertexBoneIndices::defaultSemantic()
{
	return kCC3SemanticVertexBoneIndices; 
}

NS_COCOS3D_END
