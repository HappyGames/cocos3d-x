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
#ifndef _CC3_SKINNED_BONE_H_
#define _CC3_SKINNED_BONE_H_

NS_COCOS3D_BEGIN

/**
 * CC3SkinnedBone combines the transforms of a bone and a skin mesh node, and applies these
 * transforms to deform the vertices during rendering, or when the deformed location of a
 * vertex is accessed programmatically.
 *
 * An instance keeps track of two related transform matrices, a transformMatrix,
 * which is used by the GL engine to deform the vertices during drawing, and a
 * skinTransformMatrix, which is used to deform a vertex into the local coordinate
 * system of the skin mesh node, so that it can be used programmatically.
 *
 * The CC3SkinnedBone instance registers as a transform listener with both the bone and the
 * skin mesh node, and lazily recalculates the transformMatrix and skinTransformMatrix
 * whenever the transform of either the bone or the skin mesh node changes.
 */
class CC3SkinnedBone : public CCObject, public CC3NodeTransformListenerProtocol
{
public:
	CC3SkinnedBone();
	virtual ~CC3SkinnedBone();

	/** Returns the bone whose transforms are being tracked. */
	CC3Bone*					getBone();

	/** Returns the skin mesh node whose transforms are being tracked. */
	CC3SkinMeshNode*			getSkinNode();

	/**
	 * Returns the transform matrix used to draw the deformed vertices during mesh rendering.
	 * This transform matrix combines the transform of the bone, the rest pose of the bone,
	 * and the rest pose of the skin mesh node.
	 *
	 * This transform matrix is lazily recomputed the first time this property is accessed after
	 * the transform is marked dirty via the markTransformDirty method. This occurs automatically
	 * when either the bone or the skin mesh node being tracked by this instance is transformed.
	 *
	 * The matrix in this property (T) is derived from a combination of:
	 *
	 *   - The transform matrix of the skin mesh node, relative to the soft-body node,
	 *     as retrieved by invoking self.skinNode.skeletalTransformMatrix (Ms).
	 *   - The inverse of that matrix, as retrieved by invoking 
	 *     self.skinNode.skeletalTransformMatrixInverted (Ms(-1)).
	 *   - The transform matrix of the bone, relative to the soft-body node,
	 *     as retrieved by invoking self.bone.skeletalTransformMatrix (Bs).
	 *   - The inverse of that matrix, when the skeleton is in its rest pose,
	 *     as retrieved by invoking self.bone.restPoseSkeletalTransformMatrixInverted (Bsrp(-1)).
	 *
	 * as follows, with * representing matrix multiplication:
	 *
	 *   T = Ms(-1) * Bs * Bsrp(-1) * Ms
	 *
	 * Reading this equation right to left, we can see that when used on a vertex in the skin
	 * mesh node, this has the effect of transforming the vertex:
	 *   1) From skin-mesh-space to soft-body-space.
	 *   2) From soft-body-space to the joint-space of the bone in its rest-pose.
	 *   3) From joint-space back to soft-body-space using the transform of the bone in its current pose.
	 *   4) From soft-body-space back to the skin-mesh-space.
	 *
	 * These steps are performed because the soft-body node represents the common root of the
	 * skin mesh nodes and the bones in the skeleton.
	 */
	CC3Matrix*					getTransformMatrix();

	/**
	 * Marks the transform matrix as dirty.
	 *
	 * Once marked as dirty, the matrix will be lazily recalculated the next time the 
	 * transformMatrix property is accessed.
	 *
	 * This method is invoked automatically when the transform of either the bone or the
	 * skin mesh node being tracked by this instance is transformed. The application should
	 * never need to invoke this method directly.
	 */
	void						markTransformDirty();


	/** Initializes this instance to apply the specified bone to the specified skin mesh node. */
	virtual void				initWithSkin( CC3SkinMeshNode* aNode, CC3Bone* aBone );

	/** Allocates and initializes an instance to apply the specified bone to the specified skin mesh node. */
	static CC3SkinnedBone*		skinnedBoneWithSkin( CC3SkinMeshNode* aNode, CC3Bone* aBone );

	// This will raise an assertion without a skin node or bone.
	virtual void				init();

	void						nodeWasTransformed( CC3Node* node );
	void						nodeWasDestroyed( CC3Node* node );

protected:
	CC3SkinMeshNode*			m_pSkinNode;
	CC3Bone*					m_pBone;
	CC3Matrix*					m_transformMatrix;
};

NS_COCOS3D_END

#endif
