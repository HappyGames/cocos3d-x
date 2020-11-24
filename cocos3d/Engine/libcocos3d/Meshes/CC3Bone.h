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
#ifndef _CC3_BONE_H_
#define _CC3_BONE_H_

NS_COCOS3D_BEGIN

/**
 * CC3Bone is the building block of skeletons that control the deformation of a skin mesh.
 *
 * When building a skeleton, bones are assembled in a structural assembly, from a root bone
 * out to limb or branch bones. For example, a skeleton for a human character might start
 * with a root spine bone, to which are attached upper-arm and thigh bones, to which are
 * attached, forearm and shin bones, to which are attached hand and foot bones, and so on.
 *
 * In this structual assembly, moving an upper-arm bone to cause the character to reach
 * out, carries the forearm and hand bones along with it. Movement of the forearm bone
 * is then performed relative to the upper-arm bone, and movement of the hand bone is
 * performed relative to the forearm, and so on.
 *
 * CC3Bones are simply specialized structural nodes, and have no content of their own to
 * draw. However, individual bones are referenced by skin sections of the skin mesh node,
 * and the transform matrices of the bones influence the transformations of the vertices
 * of the skin mesh, as the skeleton moves.
 */
class CC3Bone : public CC3Node 
{
	DECLARE_SUPER( CC3Node );
public:
	CC3Bone();
	virtual ~CC3Bone();

	/**
	 * Returns the transform matrix of this bone, relative to the coordinate system of the skeleton
	 * of which this bone is a part. The root of the skeleton is the nearest ancestor CC3SoftBodyNode.
	 */
	CC3Matrix*				getSkeletalTransformMatrix();

	/**
	 * Returns the inverse of the transform matrix of this bone, relative to the coordinate system
	 * of the skeleton of which this bone is a part, that corresponds to the orientation of this
	 * bone when the skeleton is in its rest pose.
	 *
	 * The returned matrix is the inverse of the matrix returned by the skeletalTransformMatrix
	 * property, at the point where the skeleton is in its rest pose (before any further transforms
	 * are applied to the bones in the skeleton). The rest pose of the skeleton corresponds to
	 * the undeformed skin mesh. Changes to the transform of this bone, relative to this rest pose,
	 * will deform the mesh to create soft-body movement of the mesh vertices.
	 *
	 * The value of this property is set from the value of the skeletalTransformMatrix whenever the
	 * bindRestPose method is invoked.
	 */
	CC3Matrix*				getRestPoseSkeletalTransformMatrixInverted();

	bool					hasSoftBodyContent();
	void					ensureRigidSkeleton();
	void					initWithTag( GLuint aTag, const std::string& aName );
	
	void					populateFrom( CC3Bone* another );
	virtual CCObject*		copyWithZone( CCZone* zone );

	void					markTransformDirty();
	void					bindRestPose();
	/** Inverts the transform matrix and caches it as the inverted rest pose matrix. */
	void					cacheRestPoseMatrix();
    
    static CC3Bone*         create();

protected:
	CC3Matrix*				m_skeletalTransformMatrix;
	CC3Matrix*				m_restPoseSkeletalTransformMatrixInverted;
};

NS_COCOS3D_END

#endif
