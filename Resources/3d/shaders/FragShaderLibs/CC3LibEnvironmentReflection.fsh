/*
 * CC3LibEnvironmentReflection.fsh
 *
 * Cocos3D 2.0.1
 * Author: Bill Hollings
 * Copyright (c) 2011-2014 The Brenwill Workshop Ltd. All rights reserved.
 * http://www.brenwill.com
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

/**
 * This fragment shader library adds an environment reflection on the model.
 *
 * This library requires the following varying variables be declared and populated in the vertex shader:
 *   - varying vec3			v_reflectDirGlobal;			// Fragment reflection vector direction in global coordinates.
 *
 * This library requires the following local variables be declared and populated outside this library:
 *   - lowp vec4			fragColor;					// The fragment color
 *
 * This library declares and uses the following attribute and uniform variables:
 *   - uniform float		u_cc3MaterialReflectivity;	// Reflectivity of the material (0 <> 1).
 *   - uniform samplerCube	s_cc3TextureCube;			// Reflection cube-map texture sampler.
 */

uniform float			u_cc3MaterialReflectivity;	/**< Reflectivity of the material (0 <> 1). */
uniform samplerCube		s_cc3TextureCube;			/**< Reflection cube-map texture sampler. */

varying vec3			v_reflectDirGlobal;			/**< Fragment reflection vector direction in global coordinates. */

/** Mix the fragment color with the reflection color in proportion to the material reflectivity. */
void addEnvironmentReflection() {
	lowp vec4 reflectColor = textureCube(s_cc3TextureCube, v_reflectDirGlobal);
	fragColor = mix(fragColor, reflectColor, u_cc3MaterialReflectivity);
}
