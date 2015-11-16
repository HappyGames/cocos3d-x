/*
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
/** @file */	// Doxygen marker
#ifndef _CC3_POD_SHADER_H_
#define _CC3_POD_SHADER_H_

NS_COCOS3D_BEGIN

	/**
	 * Create shader instance compiled from GLSL source code identified by the specified PFX shader
	 * specification in the specified PFX resource loader.
	 *
	 * Shaders loaded through this method are cached. If the shader was already loaded and is in
	 * the cache, it is retrieved and returned. If the shader has not in the cache, it is created
	 * compiled from GLSL code identified by the specified PFX shader specification, and added to
	 * the shader cache. It is safe to invoke this method any time the shader is needed, without
	 * having to worry that the shader will be repeatedly loaded and compiled.
	 *
	 * If the shader is created and compiled, the GLSL code may be embedded in the PFX file, or
	 * may be contained in a separate GLSL source code file, as defined by the PFX shader spec.
	 *
	 * To clear a shader instance from the cache, use the removeShader: method.
	 */
class CC3PODShader
{
public:
	static CC3VertexShader*				createVertexShader( PFXClassPtr pSPVRTPFXParserShader, CC3PFXResource* pfxRez );
	static CC3FragmentShader*			createFragmentShader( PFXClassPtr pSPVRTPFXParserShader, CC3PFXResource* pfxRez );
};

NS_COCOS3D_END

#endif
