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
/** @file */	// Doxygen marker
#ifndef _CC3_POD_LIGHT_H_
#define _CC3_POD_LIGHT_H_
#include "../Nodes/CC3Light.h"
#include "CC3PODNode.h"

NS_COCOS3D_BEGIN
/**
 * A CC3Light whose content originates from POD resource data.
 *
 * This is a concrete implementation of the CC3Node category PVRPOD. 
 */
class CC3PODLight : public CC3Light 
{
	DECLARE_SUPER( CC3Light );
public:
	virtual GLint				getPodIndex();
	virtual void				setPodIndex( GLint index );
	virtual GLint				getPodContentIndex();
	virtual void				setPodContentIndex( GLint index );
	virtual GLint				getPodParentIndex();
	virtual void				setPodParentIndex( GLint index );
	virtual GLint				getPodTargetIndex();
	virtual void				setPodTargetIndex( GLint index );
	virtual PODStructPtr		getNodePODStructAtIndex( GLuint aPODIndex, CC3PODResource* aPODRez );
	virtual void				initAtIndex( GLint aPODIndex, CC3PODResource* aPODRez );

	void						populateFrom( CC3PODLight* another );
	virtual CCObject*			copyWithZone( CCZone* zone );

	// The direction of a light in a POD file is taken from the transform of the up direction!
	CC3Vector4					getGlobalHomogeneousPosition();
	std::string					description();

	static CC3PODLight*			nodeAtIndex( GLint aPODIndex, CC3PODResource* aPODRez );

protected:
	GLint						_podIndex;
	GLint						_podContentIndex;
	GLint						_podParentIndex;
	GLint						_podTargetIndex;
};

NS_COCOS3D_END

#endif
