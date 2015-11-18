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
#ifndef _CC3_POD_CAMERA_H_
#define _CC3_POD_CAMERA_H_

NS_COCOS3D_BEGIN
/**
 * A CC3Camera whose content originates from POD resource data.
 *
 * This is a concrete implementation of the CC3Node category PVRPOD.
 *
 * In this implementation, the fieldOfViewOrientation is set to CC3FieldOfViewOrientationHorizontal
 * since the fieldOfView value in a POD file is always specified as the horizontal field of view.
 */
class CC3PODCamera : public CC3Camera 
{
	DECLARE_SUPER( CC3Camera );
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
	/**
	 * In Cocos3D, scaling a camera affects the effective field of view. In a POD file, scale
	 * info is meaningless and should be ignored. This is handled here by clearing the scale
	 * animation flag, and clearing the scale animation content before building the camera node.
	 */
	void						clearScaleContentIn( SPODNode* psn );

	/**
	 * Cameras in Cocos3D are oriented to the OpenGL standard of pointing down the -Z axis, with
	 * the UP direction pointing up the +Y axis. However, the camera is a POD file is oriented
	 * so that it points down the -Y axis, with the up direction pointing down the -Z axis.
	 *
	 * The POD orientation can be aligned with the OpenGL orientation by rotating the camera
	 * -90 degrees around the +X axis.
	 *
	 * This method runs through the quaternions in the rotation animation array (including the
	 * initial rotation setting in the first element, even if rotation animation is not used),
	 * and prepreds a fixed -90 degrees rotation around the X-axis to each quaternion. This is
	 * done by creating a -90 degree +X axis rotation quaternion, multiplying it by each of the
	 * quaternions in the rotation animation array, and placing eac of the results back in the
	 * rotation animation array.
	 */
	void						adjustQuaternionsIn( SPODNode* psn, GLuint numFrames );

	void						populateFrom( CC3PODCamera* another );
	virtual CCObject*			copyWithZone( CCZone* zone );

	std::string					description();
    
    void                        linkToPODNodes( CCArray* nodeArray );

	static CC3PODCamera*		nodeAtIndex( GLint aPODIndex, CC3PODResource* aPODRez );

protected:
	GLint						_podIndex;
	GLint						_podContentIndex;
	GLint						_podParentIndex;
	GLint						_podTargetIndex;
};

NS_COCOS3D_END

#endif
