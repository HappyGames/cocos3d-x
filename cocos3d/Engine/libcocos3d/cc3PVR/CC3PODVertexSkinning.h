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
#ifndef _CC3_POD_VERTEX_SKINNING_H_
#define	_CC3_POD_VERTEX_SKINNING_H_

NS_COCOS3D_BEGIN

/** A CC3SkinMeshNode extracted from a POD file. */
class CC3PODSkinMeshNode : public CC3SkinMeshNode
{
	DECLARE_SUPER( CC3SkinMeshNode );
public:
	static CC3PODSkinMeshNode*	nodeAtIndex( GLint aPODIndex, CC3PODResource* aPODRez );

	virtual GLint				getPodContentIndex();
	virtual void				setPodContentIndex( GLint aPODIndex );
	virtual GLint				getPodParentIndex();
	virtual void				setPodParentIndex( GLint aPODIndex );
	virtual GLint				getPodIndex();
	virtual void				setPodIndex( GLint aPODIndex );
	virtual GLint				getPodMaterialIndex();
	virtual void				setPodMaterialIndex( GLint aPODIndex );
    PODStructPtr				getNodePODStructAtIndex( GLuint aPODIndex, CC3PODResource* aPODRez );

	/** 
	 * Overridden to verify that the mesh is not constructed from triangle strips,
	 * which are not compatible with the way that skin sections render mesh sections.
	 */
	virtual void				setMesh( CC3Mesh* mesh );
	/** Overridden to extract the bone batches from the associated POD mesh structure */
	virtual void				initAtIndex( GLint aPODIndex, CC3PODResource* aPODRez );

	void						populateFrom( CC3PODSkinMeshNode* another );
	virtual CCObject*			copyWithZone( CCZone* zone );

	/** Link the nodes in the bone batches. */
	virtual void				linkToPODNodes( CCArray* nodeArray );
	std::string					description();

protected:
	GLint						_podIndex;
	GLint						_podContentIndex;
	GLint						_podParentIndex;
	GLint						_podMaterialIndex;
};

/**
 * A CC3SkinSection extracted from a POD file.
 * 
 * Since the CC3PODSkinSection may be loaded before the corresponding skeleton,
 * the bones to which this skin section will be attached may not exist during
 * loading. This class keeps track of the bone node indices, and creates links
 * to the bones once the entire POD has been loaded.
 */
class CC3PODSkinSection : public CC3SkinSection 
{
	DECLARE_SUPER( CC3SkinSection );
public:
	/**
	 * Initializes an instance from the specified POD SPODMesh structure, 
	 * and that will be used by the specified skin mesh node.
	 */
	void						initFromBatchAtIndex( int aBatchIndex, PODStructPtr aSPODMesh, CC3SkinMeshNode* aNode );

	/**
	 * Allocates and initializes an autoreleased instance from the specified POD
	 * SPODMesh structure, and that will be used by the specified skin mesh node.
	 */
	static CC3PODSkinSection*	skinSectionFromBatchAtIndex( int aBatchIndex, PODStructPtr aSPODMesh, CC3SkinMeshNode* aNode );

	/**
	 * Create links to the nodes in the specified array.
	 *
	 * This implementation iterates through the indices in the boneNodeIndices array,
	 * retrieves the CC3Bone node at each index in the specified node array, and
	 * adds that bone node to this skin section using the addBone: method.
	 */
	void						linkToPODNodes( CCArray* nodeArray );

	void						init();
	std::string					fullDescription();
	
protected:
	GLuint						_podBoneCount;
	GLint*						_podBoneNodeIndices;
};

class CC3PODNode;
/** A CC3Bone extracted from a POD file. */
class CC3PODBone : public CC3Bone 
{
	DECLARE_SUPER( CC3Bone );
public:
	static CC3PODBone*			nodeAtIndex( GLint aPODIndex, CC3PODResource* aPODRez );

    void                        initAtIndex( GLint aPODIndex, CC3PODResource* aPODRez );
	GLint						getPodIndex();
	void						setPodIndex( GLint aPODIndex );
	GLint						getPodContentIndex();
	void						setPodContentIndex( GLint aPODIndex );
	GLint						getPodParentIndex();
	void						setPodParentIndex( GLint aPODIndex );

	void						populateFrom( CC3PODBone* another );
	virtual CCObject*			copyWithZone( CCZone* zone );

	std::string					description();
    
    void                        linkToPODNodes( CCArray* nodeArray );

protected:
	GLint						_podIndex;
	GLint						_podContentIndex;
	GLint						_podParentIndex;
};

NS_COCOS3D_END

#endif
