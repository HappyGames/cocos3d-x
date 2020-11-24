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
#ifndef _CC3_SKINMESH_NODE_H_
#define _CC3_SKINMESH_NODE_H_

NS_COCOS3D_BEGIN

class CC3Bone;
class CC3SkinSection;
class CC3DeformedFaceArray;

/**
 * CC3SkinMeshNode is a CC3MeshNode specialized to use vertex skinning to draw the contents
 * of its mesh. It is one of the key structural descendant nodes of a CC3SoftBodyNode instance.
 *
 * The CC3SkinMeshNode contains a collection of skin sections, in the form of CC3SkinSection
 * instances. Each CC3SkinSection instance relates a section of the mesh, in the form of a 
 * range of vertices, to a set of bones in the skeleton.
 * 
 * Each CC3SkinSection applies the transformations in the referenced bones to the the vertices
 * in the section of the mesh that it controls, and draws that section of the mesh by drawing
 * the vertices within its range in a single GL call.
 * 
 * After copying a CC3SkinMeshNode, the newly created copy will still be influenced by the 
 * original skeleton. The result is that both the original mesh and the copy will move and 
 * be deformed in tandem as the skeleton moves.
 *
 * If you are creating a chorus line of dancing characters, this may be the effect you are after.
 * However, if you are creating a squadron of similar, but independently moving, characters, each
 * CC3SkinMeshNode copy should be controlled by a separate skeleton. To do this, after creating
 * a copy of the skeleton bone node assembly as well, you can use the reattachBonesFrom: method
 * to attach the skin mesh node to the new skeleton.
 *
 * When copying a CC3SkinMeshNode as part of copying a CC3SoftBodyNode instance, a copy of 
 * the skeleton is also created, and the reattachBonesFrom: method is automatically invoked.
 * When copying CC3SoftBodyNode, you do not need to invoke the reattachBonesFrom: method on
 * the new CC3SkinMeshNode directly.
 *
 * The use of bounding volumes with skinned meshes can be tricky, because the locations of
 * the vertices are affected both by the location of the mesh node, as with any mesh, but
 * also by the location of the bones. In addition, bone transformation is handled by the
 * GPU, and unless the CPU is also tasked with transforming each vertex, it is difficult
 * for the app to know the true range of the vertices.
 *
 * Because of this, the createBoundingVolumes method will be ignored by instances of this
 * class, and a bounding volume will not automatically be assigned to this node, to ensure
 * that the mesh will not be culled when it shouldn't if the automatic bounding volume is
 * not the correct shape. This mesh will therefore be drawn for each frame, even if it is
 * not in front of the camera (ie- inside the camera's frustum).
 *
 * It is left to the application to determine the best approach to managing the assignment
 * of a bounding volume, possibly using one of the following approaches:
 *
 *   - You can choose to leave this node with no bounding volume, and allow it to be drawn
 *     on each frame. This may be the easiest approach if performance is not critical.
 *
 *   - For a CC3SkinMeshNode, the bounding volume that is created automatically when the
 *     createBoundingVolume method is invoked forms a sphere around the rest pose of the mesh.
 *     A spherical bounding volume is used because, for many bone-rigged characters, the bones
 *     remain within a sphere determmined by the rest pose. If you know that the vertices of 
 *     the skinned mesh node will not move beyond the static bounding volume defined by the
 *     vertices in the rest pose, you can invoke the createBoundingVolume method to have a
 *     spherical bounding volume created automatically from the rest pose of this node. 
 *     If this is a common requirement, you can also use the createSkinnedBoundingVolumes
 *     method on any ancestor node to have a spherical bounding volume automatically created
 *     for each descendant skinned mesh node.
 *
 *   - You can manually create a bounding volume of the right size and shape for the movement
 *     of the vertices from the perspective of a root bone of the skeleton. Assign the bounding
 *     volume to the root bone by using the boundingVolume property on the root bone and,
 *     once it has been assigned a root bone of the skeleton, use the setSkeletalBoundingVolume:
 *     method on an ancestor node of all of the CC3SkinMeshNodes that are to use that bounding
 *     volume, to assign that bounding volume to all of the appropriate CC3SkinMeshNodes.
 *     A good choice to target for the invocation of this method might be the CC3SoftBodyNode
 *     of the model, or even the CC3ResourceNode above it, if loaded from a file. During
 *     development, you can use the shouldDrawBoundingVolume property to make the bounding
 *     volume visible, to aid in determining and setting the right size and shape for it.
 */
class CC3SkinMeshNode : public CC3MeshNode 
{
	DECLARE_SUPER( CC3MeshNode );
public:
	CC3SkinMeshNode();
	virtual ~CC3SkinMeshNode();

	/** The collection of CC3SkinSections that are managed by this node. */
	CCArray*					getSkinSections();

	/**
	 * Returns the skin section that deforms the specified vertex.
	 *
	 * Each skin section operates on a consecutive array of vertex indices.
	 * If this mesh uses vertex indexing, the specified index should be an
	 * index into the vertex index array.
	 *
	 * If this mesh does not use vertex indexing, then the specified index
	 * should be the index of the vertex in the vertex locations array.
	 */
	CC3SkinSection*				getSkinSectionForVertexIndexAt( GLint index );

	/**
	 * Returns the skin section that deforms the specified face.
	 *
	 * The specified faceIndex value refers to the index of the face, not the vertices
	 * themselves. So, a value of 5 will retrieve the three vertices that make up the
	 * fifth triangular face in this mesh. The specified index must be between zero,
	 * inclusive, and the value of the faceCount property, exclusive.
	 */
	CC3SkinSection*				getSkinSectionForFaceIndex( GLint faceIndex );

	/**
	 * Returns the transform matrix of this skin mesh node, relative to the coordinate system 
	 * of the skeleton that influences the vertices of this mesh node. The root of the skeleton
	 * is the nearest ancestor CC3SoftBodyNode.
	 */
	CC3Matrix*					getSkeletalTransformMatrix();

	/** Returns the inverse of the matrix in the skeletalTransformMatrix property. */
	CC3Matrix*					getSkeletalTransformMatrixInverted();

	/**
	 * Callback method indicating the specified bone has been transformed.
	 *
	 * This implementation clears the internal mesh faces cache, because the faces of the skinned
	 * mesh are deformed by the movement of the bones that influence this skin mesh node.
	 */
	void						boneWasTransformed( CC3Bone* aBone );

	bool						hasSkeleton();
	bool						hasRigidSkeleton();
	void						ensureRigidSkeleton();
	void						setShouldCacheFaces( bool shouldCache );
	CC3DeformedFaceArray*		getDeformedFaces();
	void						setDeformedFaces( CC3DeformedFaceArray* aFaceArray );
	CC3Face						getDeformedFaceAt( GLuint faceIndex );
	CC3Vector					getDeformedFaceCenterAt( GLuint faceIndex );
	CC3Vector					getDeformedFaceNormalAt( GLuint faceIndex );
	CC3Plane					getDeformedFacePlaneAt( GLuint faceIndex );
	CC3Vector					getDeformedVertexLocationAt( GLuint vertexIndex, GLuint faceIndex );
	void						initWithTag( GLuint aTag, const std::string& aName );

	void						populateFrom( CC3SkinMeshNode* another );
	virtual CCObject*			copyWithZone( CCZone* zone );

	void						reattachBonesFrom( CC3Node* aNode );
	bool						hasSoftBodyContent();
	void						markTransformDirty();
	void						addShadowVolumesForLight( CC3Light* aLight );

	/**
	 * Returns a spherical bounding volume that will be sized to encompass the vertices of the
	 * skin mesh in its bind pose. A sphere is used because for many bone-rigged characters, 
	 * the bones remain within the sphere determmined by the rest pose.
	 */
	CC3NodeBoundingVolume*		defaultBoundingVolume();
	/** Overridden to skip auto-creating a bounding volume. */
	void						createBoundingVolumes();
	/** Overridden to auto-create a bounding volume. */
	void						createSkinnedBoundingVolumes();
	/** Use this bounding volume, then pass along to my descendants. */
	void						setSkeletalBoundingVolume( CC3NodeBoundingVolume* boundingVolume );
	/** 
	 * Draws the mesh vertices to the GL engine.
	 *
	 * Enables palette matrices, binds the mesh to the GL engine, delegates to the contained
	 * collection of CC3SkinSections to draw the mesh in batches, then disables palette matrices again.
	 */
	void						drawMeshWithVisitor( CC3NodeDrawingVisitor* visitor );
	
protected:
	CCArray*					m_skinSections;
	CC3Matrix*					m_pSkeletalTransformMatrix;
	CC3Matrix*					m_pSkeletalTransformMatrixInverted;
	CC3DeformedFaceArray*		m_deformedFaces;
};

NS_COCOS3D_END

#endif
