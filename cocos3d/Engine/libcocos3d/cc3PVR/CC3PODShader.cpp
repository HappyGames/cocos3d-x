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
#include "CC3PVRTModelPOD.h"
#include "CC3PVRFoundation.h"

NS_COCOS3D_BEGIN

CC3VertexShader* CC3PODShader::createVertexShader( PFXClassPtr pSPVRTPFXParserShader, CC3PFXResource* pfxRez )
{
	SPVRTPFXParserShader* pfxShader = (SPVRTPFXParserShader*)pSPVRTPFXParserShader;
	if ( pfxShader->bUseFileName ) 
	{
		// Load the shader from the file
		std::string shaderFilePath = pfxShader->pszGLSLfile;
		return CC3VertexShader::shaderFromSourceCodeFile( shaderFilePath );
	}
	else 
	{
		// Derive the shader name as a combination of the PFX resource name and the local shader name.
		std::string shaderName = CC3String::stringWithFormat( (char*)"%s-%s", pfxRez->getName().c_str(), pfxShader->Name.c_str() );
		std::string shSrcStr = pfxShader->pszGLSLcode;
		return CC3VertexShader::shaderWithName( shaderName, shSrcStr );
	}

}

CC3FragmentShader* CC3PODShader::createFragmentShader( PFXClassPtr pSPVRTPFXParserShader, CC3PFXResource* pfxRez )
{
	SPVRTPFXParserShader* pfxShader = (SPVRTPFXParserShader*)pSPVRTPFXParserShader;
	if ( pfxShader->bUseFileName ) 
	{
		// Load the shader from the file
		std::string shaderFilePath = pfxShader->pszGLSLfile;
		return CC3FragmentShader::shaderFromSourceCodeFile( shaderFilePath );
	} 
	else 
	{
		// Derive the shader name as a combination of the PFX resource name and the local shader name.
		std::string shaderName = CC3String::stringWithFormat( (char*)"%s-%s", pfxRez->getName().c_str(), pfxShader->Name.c_str() );
		std::string shSrcStr = pfxShader->pszGLSLcode;
		return CC3FragmentShader::shaderWithName( shaderName, shSrcStr );
	}
}

NS_COCOS3D_END
