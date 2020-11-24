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
#ifndef _CC3_SOFT_BODY_NODE_H_
#define _CC3_SOFT_BODY_NODE_H_

NS_COCOS3D_BEGIN

/**
 * CC3SoftBodyNode is the primary structural component for a soft-body object that uses vertex
 * skinning to manipulate and draw mesh vertices.
 *
 * Vertex skinning is a feature of OpenGL that allows the vertices of a mesh to be manipulated
 * or deformed using an underlying skeleton of bones. This feature is also sometimes referred
 * to as bone-rigging. This feature is used to create realistic movement in soft-body, flexible
 * meshes, such as characters or textiles.
 *
 * A soft-body object consists of two primary components: a skeletal structure, and the skin
 * that covers it. The skeletal structure is constructed from an assembly of CC3Bone instances,
 * and the skin is constructed from one or more CC3SkinMeshNode instances. The CC3SoftBodyNode
 * instance then serves to collect together the bones and skin components, and forms the root
 * of the soft-body object.
 * 
 * The vertices of the skin mesh forms the skin that surrounds the bones of the skeleton.
 * During movement and drawing, the location and rotation of each bone in the skeleton 
 * influences the locations of the skin vertices that are attached to that bone. Some skin
 * vertices, particularly those around joints where two bones meet, can be associated with
 * more than one bone, and in that case, the influence that each bone has on the location 
 * of a vertex is determined by a weighting associated with each bone for that vertex.
 *
 * The CC3Bone instances are typically assembled into a structural assembly of bones known
 * as a skeleton. The purpose of this skeletal structure is to allow the bones to move and
 * interact with each other in a hierarchical manner.
 *
 * A CC3SkinMeshNode instance represents the skin that covers the skeleton, and contains the
 * mesh that makes up the skin. This mesh includes the bone assignments and weights for each
 * vertex, which specifies how the location of each vertex is influenced by the location and
 * orientation of each nearby bone.
 *
 * A single soft-body object may be covered by a single skin mesh, but more complicated objects
 * may be covered by several skin meshes. As such, a single CC3SoftBodyNode instance may contain
 * one or more CC3SkinMeshNode instances.
 * 
 * For efficiency and control, each skin mesh is usually broken into sections. These skin 
 * sections are represented by instances of the CC3SkinSection class. A CC3SkinMeshNode
 * typically holds a single mesh and several CC3SkinSection instances to define how that mesh
 * should be divided into sections. Each CC3SkinSection instance contains a range of vertices,
 * and references to the bones in the skeleton that influence the vertices in that range.
 * All of the vertices of a single CC3SkinSection are drawn in a single GL drawing call.
 *
 * Manipulation of the bones in the skeleton will cause the soft-body to move and flex 
 * internally. In addition, like any node, a CC3SoftBodyNode can be moved, rotated and 
 * scaled to move, rotate and scale the entire soft-body assembly of skin and bones as
 * a unit. By combining both internal bone animation with movement of the entire
 * CC3SoftBodyNode, you can create realistic movement of your soft-body objects.
 *
 * For example, if your CC3SoftBodyNode represents a character, you could animate the bones
 * in the skeleton within the node to crouch down and then stand up again. During the standing
 * up animation, you could move the entire CC3SoftBodyNode upwards to create a realistic jumping
 * action. Or, you could simply animate the bones in the skeleton through a loop of a step of a
 * walking motion, while at the same time moving the CC3SoftBodyNode forward, making it appear
 * that the character was walking forward.
 *
 * The initial assembly of CC3Bone nodes should be arranged into what is termed the "rest pose".
 * This is the alignment of the bones that will fit the undeformed positions of the vertices that
 * make up the skin. In the rest pose, the bones have no deforming effect on the skin vertices.
 * 
 * Once the initial skeleton has been assembled into the rest pose, you should invoke 
 * the bindRestPose method on the CC3SoftBodyNode instance (or any ancestor node of the
 * CC3SoftBodyNode instance) to cause the bones to cache this pose.
 * 
 * Subsequent movement of the bones in the skeleton deform the skin vertices relative to this
 * rest pose, affecting the location of the vertices in the mesh.
 *
 * In almost all soft-body objects, all internal movement of the object is handled via
 * manipulation of the bones. The CC3SkinMeshNodes should not be moved or rotated directly,
 * relative to the surrounding CC3SoftBodyNode, otherwise the skin will become detached
 * from the bones. However, if you have reason to move the skin mesh nodes relative to
 * the soft-body node, you should re-establish the rest pose and invoke the bindRestPose
 * method again to re-align the bones with the skin.
 *
 * If the CC3SoftBodyNode has been assembled from a file loader, the bindRestPose method
 * will usually be invoked automatically, and you do not need to invoke it explicitly.
 */
class CC3SoftBodyNode : public CC3Node
{
	DECLARE_SUPER( CC3Node );
public:
	void						addCopiesOfChildrenFrom( CC3Node* another );
	static CC3SoftBodyNode*		nodeWithName( const std::string& aName );

	virtual CC3SoftBodyNode*	getSoftBodyNode();
	virtual CC3Vector			getSkeletalScale();

	virtual CCObject*			copyWithZone( CCZone* zone );
};

NS_COCOS3D_END

#endif
