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
#ifndef _CC3_SKIN_SECTION_H_
#define _CC3_SKIN_SECTION_H_

NS_COCOS3D_BEGIN

/**
 * A CC3SkinSection defines a section of the skin mesh, and contains a collection of
 * bones from the skeleton that influence the locations of the vertices in that section.
 *
 * The skin section is expressed as a range of consecutive vertices from the mesh, as
 * specified by the vertexStart and vertexCount properties. These properties define the
 * first vertex of the section and the number of vertices in the section, respectively.
 *
 * The skin section also contains a collection of the bones that influence the vertices in
 * the skin section. The bones are ordered in that collection such that the index of a bone
 * in the collection corresponds to the index held for a vertex in the vertexMatrixIndices
 * vertex array of the mesh.
 *
 * Through the CC3VertexBoneIndices vertex array in the vertexMatrixIndices property of the mesh,
 * each vertex identifies several distinct indices into the bones collection of this skin section.
 * The transform matrices from those bones are combined in a weighted fashion, and used to transform
 * the location of the vertex. Each vertex defines its own set of weights through the CC3VertexWeights
 * vertex array in the vertexWeights property of the mesh.
 */
class CC3SkinSection : public CCObject
{
public:
	CC3SkinSection();
	virtual ~CC3SkinSection();

	/** Returns whether this skin section contains bones. */
	bool						hasSkeleton();

	/** Returns the number of bones in this skin section. */
	GLuint						getBoneCount();

	/**
	 * The collection of bones from the skeleton that influence the mesh vertices that are managed
	 * and drawn by this skin section.
	 * 
	 * Each vertex holds a set of indices into this mesh, to identify the bones that contribute to
	 * the transforming of that vertex. The contribution that each bone makes is weighted by the
	 * corresponding weights held by the vertex.
	 *
	 * Any particular vertex will typically only be directly influenced by two or three bones.
	 * Under OpenGL ES 1.1, the maximum number of bones that any vertex can be directly influenced
	 * by is determined by the maximum number of available bone influences allowed by the platform.
	 * This limit can be retreived from the CC3OpenGL.sharedGL.maxNumberOfBoneInfluencesPerVertex property.
	 *
	 * Because different vertices of the skin section may be influenced by different combinations
	 * of bones, the number of bones in the collection in this property will generally be larger
	 * than the number of bones used per vertex.
	 *
	 * However, when the vertices are drawn, all of the vertices in this skin section are drawn with
	 * a single call to the GL engine. All of the bone transforms that affect any of the vertices
	 * being drawn are loaded into the GL engine by this skin section prior to drawing the vertices.
	 * 
	 * Under OpenGL ES 1.1, the number of transform matrices that can be simultaneously loaded 
	 * into the GL engine matrix palette is limited by the platform, and that limit defines the
	 * maximum number of bones in the collection in this property. This platform limit can be
	 * retrieved from the CC3OpenGL.sharedGL.maxNumberOfPaletteMatrices property.
	 *
	 * The array returned by this property is created anew for each access. Do not add or remove
	 * bones from the returned array directly. To add a bone, use the addBone: method.
	 */
	CCArray*					getBones();

	/** Returns the bone node at the specified index. */
	CC3Bone*					getBoneAt( GLuint boneIdx );

	/**
	 * An index that indicates which vertex in the mesh begins this skin section.
	 *
	 * This value is an index of vertices, not of the underlying primitives (floats or bytes).
	 *
	 * For example, if a mesh has ten vertices, the value of this property can be set to
	 * some value between zero and nine, even though each of the vertices contains several
	 * components of data (locations, normals, texture coordinates, bone indices and bone
	 * weights, making the actual array much longer than ten, in terms of primatives or bytes)
	 */
	GLint						getVertexStart();
	void						setVertexStart( GLint vertexStart );

	/**
	 * Indicates the number of vertices in this skin section.
	 *
	 * This value is a count of the number of vertices, not of the number of underlying
	 * primitives (floats or bytes).
	 *
	 * For example, if a mesh has ten vertices, the value of this property can be set to
	 * some value between zero and ten, even though each of the vertices contains several
	 * components of data (locations, normals, texture coordinates, bone indices and bone
	 * weights, making the actual array much longer than ten, in terms of primatives or bytes)
	 */
	GLint						getVertexCount();
	void						setVertexCount( GLint vertexCount );

	/**
	 * Adds the specified bone node to the collection of bones in the bones property.
	 *
	 * See the notes for the bones property for more information about bones.
	 *
	 * When the vertices are drawn, all of the vertices in this skin section are drawn with a 
	 * single call to the GL engine. All of the bone transforms that affect any of the vertices
	 * being drawn are loaded into the GL engine by this skin section prior to drawing the vertices.
	 * 
	 * Under OpenGL ES 1.1, the number of transform matrices that can be simultaneously loaded
	 * into the GL engine matrix palette is limited by the platform, and that limit defines the
	 * maximum number of bones in the collection in this property. This platform limit can be
	 * retrieved from the CC3OpenGL.sharedGL.maxNumberOfPaletteMatrices property.
	 */
	void						addBone( CC3Bone* aNode );

	/** Returns whether all of the bones in this skin section have a rigid transform. */
	bool						hasRigidSkeleton();

	/**
	 * Returns whether this skin section contains the specified vertex index.
	 *
	 * It does if the vertex index is equal to or greater than the vertexStart property
	 * and less than the the sum of the vertexStart and vertexCount properties.
	 */
	bool						containsVertexIndex( GLint aVertexIndex );

	/**
	 * Returns the location of the vertex at the specified index within the mesh,
	 * after the vertex location has been deformed by the bone transforms.
	 *
	 * This implementation retrieves the vertex location from the mesh and transforms
	 * it using the matrices and weights defined by the bones in this skin section. 
	 */
	CC3Vector					getDeformedVertexLocationAt( GLuint vtxIdx );

	/** Initializes an instance that will be used by the specified skin mesh node. */
	void						initForNode( CC3SkinMeshNode* aNode );

	/** Allocates and initializes an instance that will be used by the specified skin mesh node. */
	static CC3SkinSection*		skinSectionForNode( CC3SkinMeshNode* aNode );

	/** Returns a copy of this skin section, for use by the specified skin mesh node. */
	CCObject*					copyForNode( CC3SkinMeshNode* aNode );

	/** Returns a copy of this skin section, for use by the specified skin mesh node. */
	CCObject*					copyForNode( CC3SkinMeshNode* aNode, CCZone* zone );

	/**
	 * Creating a copy of a skin section is typically done as part of creating a copy of
	 * a skin mesh node. After copying, the newly created copy will still be influenced
	 * by the original skeleton. The result is that both the original mesh and the copy
	 * will move and be deformed in tandem as the skeleton moves.
	 *
	 * If you are creating a chorus line of dancing characters, this may be the effect
	 * you are after. However, if you are creating a squadron of similar, but independently
	 * moving characters, each skin mesh node copy should be controlled by a separate skeleton.
	 * 
	 * After creating a copy of the skeleton bone node assembly, you can use this method
	 * to attach the skin mesh node to the new skeleton. The node that is provided as the
	 * argument to this method is the root bone node of the skeleton, or a structural ancestor
	 * of the skeleton that does not also include the original skeleton as a descendant.
	 *
	 * This method iterates through all the bones referenced by this skin section, and retrieves
	 * a bone with the same name from the structural descendants of the specified node.
	 *
	 * Typically, you would not invoke this method on the skin section directly. Instead, you
	 * would invoke a similar method on the CC3SkinMeshNode, or one of its structural ancestors.
	 *
	 * When copying a CC3SoftBodyNode instance, this method is automatically invoked as part
	 * of the copying of the soft-body object, and you do not need to invoke this method directly.
	 */
	void						reattachBonesFrom( CC3Node* aNode );

	/** Returns a description of this skin section that includes a list of the bones. */
	std::string					fullDescription();

	/**
	 * Draws the mesh vertices of this skin section.
	 *
	 * When running under OpenGL ES 1.1, prior to drawing the vertices, this method iterates
	 * through the CC3Bones in the bones property, and loads a transform matrix into the GL
	 * matrix palette for each bone.
	 *
	 * When running under a programmable rendering pipeline, such as OpenGL ES 2.0 or desktop
	 * OpenGL, the bones transforms are retrieved from the contained bones by the shader program.
	 *
	 * This method is invoked automatically when a CC3SkinMeshNode is drawn. Usually, the
	 * application never needs to invoke this method directly.
	 */
	void						drawVerticesOfMesh( CC3Mesh* mesh, CC3NodeDrawingVisitor* visitor );

	/**
	 * Returns the transform matrix used by the bone at the specified index to influence the 
	 * vertices of the mesh in this skin section. 
	 *
	 * The returned matrix transforms the vertices within the coordinate system of the skin mesh node.
	 *
	 * See the notes of the transformMatrix property of the CC3SkinnedBone class for more details.
	 */
	CC3Matrix*					getTransformMatrixForBoneAt( GLuint boneIdx );

	virtual void				init();
	void						populateFrom( CC3SkinSection* another );
	CCObject*					copyWithZone( CCZone* zone );

protected:
	CC3SkinMeshNode*			m_pNode;
	CCArray*					m_skinnedBones;
	GLint						m_vertexStart;
	GLint						m_vertexCount;
};

NS_COCOS3D_END

#endif
