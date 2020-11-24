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
#include "cocos3d.h"

NS_COCOS3D_BEGIN

CC3VertexNormals* CC3VertexNormals::vertexArray()
{
	CC3VertexNormals* normals = new CC3VertexNormals;
	normals->init();
	normals->autorelease();

	return normals;
}

CC3VertexNormals* CC3VertexNormals::vertexArrayWithName( const std::string& aName )
{
	CC3VertexNormals* normals = new CC3VertexNormals;
	normals->initWithName( aName );
	normals->autorelease();

	return normals;
}

CC3Vector CC3VertexNormals::getNormalAt( GLuint index )
{
	return *(CC3Vector*)getAddressOfElement(index); 
}

void CC3VertexNormals::setNormal( const CC3Vector& aNormal, GLuint index )
{
	*(CC3Vector*)getAddressOfElement(index) = aNormal;
}

void CC3VertexNormals::flipNormals()
{
	GLuint vtxCnt = getVertexCount();
	for (GLuint vtxIdx = 0; vtxIdx < vtxCnt; vtxIdx++) 
	{
		CC3Vector* pn = (CC3Vector*)getAddressOfElement(vtxIdx);
		*pn = (*pn).negate();
	}
}

std::string CC3VertexNormals::getNameSuffix()
{
	return "Normals"; 
}

GLenum CC3VertexNormals::defaultSemantic()
{
	return kCC3SemanticVertexNormal; 
}

NS_COCOS3D_END
