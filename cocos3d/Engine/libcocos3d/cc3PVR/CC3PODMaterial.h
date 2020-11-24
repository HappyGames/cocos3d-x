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
/** @file */	// Doxygen marker
#ifndef _CC3_POD_MATERIAL_H_
#define _CC3_POD_MATERIAL_H_
//#include "../Materials/CC3Material.h"

NS_COCOS3D_BEGIN

class CC3PFXEffect;


/** A CC3Material whose content originates from POD resource data. */
class CC3PODMaterial : public CC3Material 
{
	DECLARE_SUPER( CC3Material )
public:
	CC3PODMaterial();
	~CC3PODMaterial();

	GLint						getPodIndex();
	void						setPodIndex( GLint aPODIndex );
	/** Returns the PFX effect used by this material. */
	CC3PFXEffect*				getPfxEffect();

	/**
	 * Allocates and initializes an autoreleased instance from the data of
	 * this type at the specified index within the specified POD resource.
	 */
	static CC3PODMaterial*		materialAtIndex( int aPODIndex, CC3PODResource* aPODRez );

	/**
	 * The shininess of a material as specified in a POD file has a very different scale than the
	 * shininess value used by OpenGL ES. To compensate for this, the shininess value extracted
	 * from a POD file is multiplied by this factor before being applied to the material.
	 *
	 * The initial value of this factor assumes the POD shininess range to be between zero and one.
	 * Since the OpenGL range is zero to 128, the initial value of this property is set to 128.
	 * If your POD files use a different range of shininess values, you can modify the value of
	 * this property to bring that range into the standard OpenGL range of zero to 128.
	 */
	static GLfloat				getShininessExpansionFactor();

	/**
	 * The shininess of a material as specified in a POD file has a very different scale than the
	 * shininess value used by OpenGL ES. To compensate for this, the shininess value extracted
	 * from a POD file is multiplied by this factor before being applied to the material.
	 *
	 * The initial value of this factor assumes the POD shininess range to be between zero and one.
	 * Since the OpenGL range is zero to 128, the initial value of this property is set to 128.
	 * If your POD files use a different range of shininess values, you can modify the value of
	 * this property to bring that range into the standard OpenGL range of zero to 128.
	 */
	static void					setShininessExpansionFactor( GLfloat aFloat );

	virtual void				initAtIndex( GLint aPODIndex, CC3PODResource* aPODRez );
	/**
	 * If the specified texture index is valid, extracts the texture from the POD resource,
	 * configures it as a bump-map texture, and adds it to this material.
	 */
	void						addBumpMapTexture( GLint aPODTexIndex, CC3PODResource* aPODRez );

	/**
	 * If the specified texture index is valid, extracts the texture from the POD resource
	 * and adds it to this material.
	 */
	void						addTexture( GLint aPODTexIndex, CC3PODResource* aPODRez );

	void						populateFrom( CC3PODMaterial* another );
	virtual CCObject*			copyWithZone( CCZone* zone );

protected:
	CC3PFXEffect*				_pfxEffect;
	GLint						_podIndex;
};

NS_COCOS3D_END

#endif
