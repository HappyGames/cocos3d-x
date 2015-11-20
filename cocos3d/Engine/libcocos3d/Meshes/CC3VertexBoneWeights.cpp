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

CC3VertexBoneWeights* CC3VertexBoneWeights::vertexArray()
{
	CC3VertexBoneWeights* weights = new CC3VertexBoneWeights;
	weights->init();
	weights->autorelease();

	return weights;
}

GLfloat CC3VertexBoneWeights::getWeightForBoneInfluence( GLuint influenceIndex, GLuint vtxIndex )
{
	return getBoneWeightsAt(vtxIndex)[influenceIndex];
}

void CC3VertexBoneWeights::setWeight( GLfloat weight, GLuint influenceIndex, GLuint vtxIndex )
{
	getBoneWeightsAt(vtxIndex)[influenceIndex] = weight;
}

GLfloat* CC3VertexBoneWeights::getBoneWeightsAt( GLuint vtxIndex )
{
	return (GLfloat*)getAddressOfElement(vtxIndex); 
}

void CC3VertexBoneWeights::setBoneWeights( GLfloat* weights, GLuint vtxIndex )
{
	GLfloat* vtxWeights = getBoneWeightsAt(vtxIndex);
	GLint numWts = getElementSize();
	for (int i = 0; i < numWts; i++) 
		vtxWeights[i] = weights[i];
}

std::string CC3VertexBoneWeights::getNameSuffix()
{
	return "Weights"; 
}

void CC3VertexBoneWeights::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	{
		m_elementType = GL_FLOAT;
		m_elementSize = 0;
	}
}

GLenum CC3VertexBoneWeights::defaultSemantic()
{
	return kCC3SemanticVertexBoneWeights; 
}

NS_COCOS3D_END
