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
#ifndef _CC3_PVR_SHAMAN_SHADER_SEMANTICS_H_
#define _CC3_PVR_SHAMAN_SHADER_SEMANTICS_H_

NS_COCOS3D_BEGIN
// Legacy naming support
#define CC3PVRShamanGLProgramSemantics		CC3PVRShamanShaderSemantics.

/**
 * Extension of the semantics enumeration to add semantics for content that is specific to
 * PVRShaman, either in content or form.
 *
 * Since these semantics extend the standard semantics, this enumeration starts at kCC3SemanticAppBase.
 * You can add custom semantics starting at kCC3PVRShamanSemanticAppBase.
 */
typedef enum {
	kCC3PVRShamanSemanticNone = kCC3SemanticAppBase,	/**< No defined semantic usage. */

	kCC3PVRShamanSemanticLightSpotFalloff,				/**< Cutoff angle and exponent of a spotlight (vec2). */
	kCC3PVRShamanSemanticViewportSize,					/**< Viewport size. */
	kCC3PVRShamanSemanticViewportClipping,				/**< Near distance, far distance, width angle (radians), height angle (radians). */
	kCC3PVRShamanSemanticElapsedTimeLastFrame,			/**< The elapsed time since the app started, measured at the last frame, in seconds. */

	kCC3PVRShamanSemanticAppBase,						/**< First semantic of app-specific custom semantics. */
} CC3PVRShamanSemantic;

/** Returns a string representation of the specified semantic. */
std::string NSStringFromCC3PVRShamanSemantic(CC3PVRShamanSemantic semantic);

/**
 * CC3PVRShamanShaderSemantics provides a mapping from the PVRShaman names declared in a PFX
 * effect within a PFX effects file, and the standard semantics from the CC3Semantic enumeration.
 */
class CC3PVRShamanShaderSemantics : public CC3PFXShaderSemantics
{
	DECLARE_SUPER( CC3PFXShaderSemantics );
public:
	/**
	 * Returns the semantic value corresponding the the specified PFX semantic name, or returns
	 * kCC3SemanticNone if the semantic could not be determined from the PFX semantic name.
	 *
	 * This implementation simply delegates to the class-side semanticForPVRShamanSemanticName: method.
	 */
	GLenum						getSemanticForPFXSemanticName( const std::string& semanticName );

	/**
	 * Returns the semantic value corresponding the the specified PVRShaman semantic name, or returns
	 * kCC3SemanticNone if the semantic could not be determined from the PVRShaman semantic name.
	 */
	static GLenum				getSemanticForPVRShamanSemanticName( const std::string& semanticName );

	/**
	 * By default, this class populates the standard mappings between PVRShaman semantic names and semantic
	 * values. You can use this method to add an additional mapping, or to change an existing mapping.
	 */
	static void					addSemantic( GLenum semantic, const std::string& semanticName );

	/** Handles populating PVRShaman-specific content and delegates remainder to the standard population mechanisms.  */
	bool						populateUniform( CC3GLSLUniform* uniform, CC3NodeDrawingVisitor* visitor );

	static void					ensurePVRShamanSemanticMap();
	static void					purgePVRShamanSemanticMap();
};

NS_COCOS3D_END

#endif
