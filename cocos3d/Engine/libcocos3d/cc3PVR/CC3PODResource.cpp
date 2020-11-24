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
#include "cocos3d.h"

NS_COCOS3D_BEGIN

enum PodResourceNodeType
{
    kTypeMeshNode       = 1,
    kTypeSkinMeshNode   = 2,
    kTypeLight          = 3,
    kTypeCamera         = 4,
    kTypeBone           = 5,
    kTypeNode           = 6,
};

CC3PODResource::CC3PODResource()
{
	_allNodes = NULL;
	_meshes = NULL;
	_materials = NULL;
	_textures = NULL;
	_pvrtModel = NULL;
}

CC3PODResource::~CC3PODResource()
{
	if ( _allNodes )
		_allNodes->removeAllObjects();
	CC_SAFE_RELEASE(_allNodes);

	if ( _meshes )
		_meshes->removeAllObjects();
	CC_SAFE_RELEASE(_meshes);

	if ( _materials )
		_materials->removeAllObjects();
	CC_SAFE_RELEASE(_materials);

	if ( _textures )
		_textures->removeAllObjects();
	CC_SAFE_RELEASE(_textures);

	deleteCPVRTModelPOD();
}

CPVRTModelPOD* CC3PODResource::getPvrtModelImpl()
{
	return (CPVRTModelPOD*)_pvrtModel; 
}

void CC3PODResource::createCPVRTModelPOD()
{
	if ( !_pvrtModel ) 
		_pvrtModel = new CPVRTModelPOD(); 
}

void CC3PODResource::deleteCPVRTModelPOD()
{
	if (_pvrtModel) 
		delete getPvrtModelImpl();
	_pvrtModel = NULL;
}

bool CC3PODResource::init()
{
	if ( super::init() )
	{
		_pvrtModel = NULL;
		_allNodes = CCArray::create();		// retain
		_allNodes->retain();
		_meshes = CCArray::create();			// retain
		_meshes->retain();
		_materials = CCArray::create();		// retain
		_materials->retain();
		_textures = CCArray::create();		// retain
		_textures->retain();
		_textureParameters = CC3Texture::defaultTextureParameters();
		_shouldAutoBuild = true;

		return true;
	}

	return false;
}

bool CC3PODResource::processFile( const std::string& anAbsoluteFilePath )
{
	// Split the path into directory and file names and set the PVR read path to the directory and
	// pass the unqualified file name to the parser. This allows the parser to locate any additional
	// files that might be read as part of the parsing.
	std::string fileName = CC3String::getFileName( anAbsoluteFilePath );
	std::string dirName = CC3String::getDirectory( anAbsoluteFilePath );
	CPVRTResourceFile::SetReadPath( dirName.c_str() );
	
	createCPVRTModelPOD();
	bool wasLoaded = (getPvrtModelImpl()->ReadFromFile(fileName.c_str()) == PVR_SUCCESS);
	
	if (wasLoaded && _shouldAutoBuild) 
		build();
	
	return wasLoaded;
}

void CC3PODResource::build()
{
	buildSceneInfo();
	//LogRez(@"Building %@", self.fullDescription);
	buildTextures();
	buildMaterials();
	buildMeshes();
	buildNodes();
	buildSoftBodyNode();
	deleteCPVRTModelPOD();
}

bool CC3PODResource::saveToFile( const std::string& filePath )
{	
#pragma _NOTE_TODO( "saveToFile( const std::string& filePath )" )
#if 0
	CCAssert(_pvrtModel, "CC3PODResource cannot be saved because the POD file content has been built and released from memory."
			  " Set the shouldAutoBuild property to NO before loading the POD file content in order to be able to save it back to a file.");
	
	// Ensure the path is absolute, converting it if needed.
	std::string absFilePath = CC3EnsureAbsoluteFilePath(filePath);
	
	bool wasSaved = (getPvrtModelImpl()->SavePOD(absFilePath.c_str()) == PVR_SUCCESS);
	
	/*if (wasSaved)
	LogRez(@"%@ saved resources to file '%@' in %.3f ms", self, filePath, GetRezActivityDuration() * 1000);
	else
	LogError(@"%@ could not save resources to file '%@'", self, absFilePath);*/
	
	return wasSaved;
#endif

	return false;
}

bool CC3PODResource::saveAnimationToFile( const std::string& filePath )
{	
	CCAssert(_pvrtModel, "CC3PODResource could not save animation content because the POD file content has been built and released from memory."
			  " Set the shouldAutoBuild property to NO before loading the POD file content in order to be able to save the animation content to a file.");
	
	// Ensure the path is absolute, converting it if needed.
	std::string absFilePath = filePath;//CC3EnsureAbsoluteFilePath(filePath);
#pragma _NOTE_TODO( "CC3EnsureAbsoluteFilePath" )

	CPVRTModelPOD* myPod = getPvrtModelImpl();
	CPVRTModelPOD* pod = new CPVRTModelPOD();
	
	// Scene animation content
	pod->nNumFrame = myPod->nNumFrame;
	pod->nFPS = myPod->nFPS;
	pod->nFlags = myPod->nFlags;

	// Allocate enough memory for the nodes and copy them over,
	// except for the content and material references
	pod->pNode = NULL;
	GLuint nodeCnt = myPod->nNumNode;
	pod->nNumNode = nodeCnt;
	if (nodeCnt) {
		pod->pNode = (SPODNode*)calloc(nodeCnt, sizeof(SPODNode));
		for(GLuint nodeIdx = 0; nodeIdx < nodeCnt; nodeIdx++) {
			PVRTModelPODCopyNode(myPod->pNode[nodeIdx], pod->pNode[nodeIdx], myPod->nNumFrame);
			pod->pNode[nodeIdx].nIdx = -1;
			pod->pNode[nodeIdx].nIdxMaterial = -1;
		}
	}
	
	// Ensure remaining content is blank. Constructor does not initialize to safe empty state!!
	pod->pfColourBackground[0] = pod->pfColourBackground[1] = pod->pfColourBackground[2] = 0.0;
	pod->pfColourAmbient[0] = pod->pfColourAmbient[1] = pod->pfColourAmbient[2] = 0.0;

	pod->nNumMeshNode = 0;
	
	pod->nNumCamera = 0;
	pod->pCamera = NULL;
	
	pod->nNumLight = 0;
	pod->pLight = NULL;
	
	pod->nNumMesh = 0;
	pod->pMesh = NULL;
	
	pod->nNumMaterial = 0;
	pod->pMaterial = NULL;

	pod->nNumTexture = 0;
	pod->pTexture = NULL;
	
	pod->nUserDataSize = 0;
	pod->pUserData = NULL;
	
	pod->InitImpl();
	
	bool wasSaved = (pod->SavePOD(absFilePath.c_str()) == PVR_SUCCESS);

	delete pod;
	
	/*if (wasSaved)
		LogRez(@"%@ saved animation content to file '%@' in %.3f ms", self, filePath, GetRezActivityDuration() * 1000.0);
	else
		LogError(@"%@ could not save animation content to file '%@'", self, absFilePath);*/
	
	return wasSaved;
}

void CC3PODResource::buildSceneInfo()
{
	CPVRTModelPOD* pod = getPvrtModelImpl();
	GLfloat* rgb;
	
	rgb = pod->pfColourAmbient;
	_ambientLight = ccc4f(rgb[0], rgb[1], rgb[2], 1.0);

	rgb = pod->pfColourBackground;
	_backgroundColor = ccc4f(rgb[0], rgb[1], rgb[2], 1.0);

	_animationFrameCount = pod->nNumFrame;
	_animationFrameRate = (GLfloat)pod->nFPS;
	
#pragma _NOTE_TODO( "Assign any user data and take ownership of managing its memory" )

#if 0
	// Assign any user data and take ownership of managing its memory
	if (pod->pUserData && pod->nUserDataSize > 0) 
	{
		self.userData = [NSData dataWithBytesNoCopy: pod->pUserData length: pod->nUserDataSize];
		pod->pUserData = NULL;		// Clear reference so SPODNode won't try to free it.
	}
#endif
}

std::string CC3PODResource::fullDescription()
{
	return "CC3PODResource";
}

GLuint CC3PODResource::getNodeCount()
{
	return _pvrtModel ? getPvrtModelImpl()->nNumNode : 0; 
}

CC3Node* CC3PODResource::getNodeAtIndex( GLuint nodeIndex )
{
	return (CC3Node*)_allNodes->objectAtIndex(nodeIndex);
}

CC3Node* CC3PODResource::getNodeNamed( const std::string& aName )
{
	std::string lcName = aName;
	CC3String::makeLowercase( lcName );
	GLuint nCnt = getNodeCount();
	for (GLuint i = 0; i < nCnt; i++) 
	{
		CC3Node* aNode = getNodeAtIndex(i);

		std::string nodeName = aNode->getName();
		CC3String::makeLowercase( nodeName );
		if ( nodeName == lcName ) 
			return aNode;
	}
	return NULL;
}

void CC3PODResource::buildNodes()
{
	GLuint nCount = getNodeCount();

	// Build the array containing ALL nodes in the PVRT structure
	for (GLuint i = 0; i < nCount; i++) 
		_allNodes->addObject( buildNodeAtIndex(i) );

	// Link the nodes with each other. This includes assembling the nodes into a structural
	// parent-child hierarchy, and connecting targetting nodes with their targets.
	// Base nodes, which have no parent, form the entries of the nodes array.    
    for (GLuint i = 0; i < nCount; i++)
    {
        int nodeType = getNodeType( i );
        switch (nodeType)
        {
            case kTypeMeshNode:
            {
                CC3PODMeshNode* pNode = dynamic_cast<CC3PODMeshNode*>( _allNodes->objectAtIndex(i) );
                if ( pNode )
                {
                    pNode->linkToPODNodes( _allNodes );
                    if ( pNode->getPodParentIndex() < 0 )
                        addNode( pNode );
                }
                
                break;
            }
            case kTypeSkinMeshNode:
            {
                CC3PODSkinMeshNode* pNode = dynamic_cast<CC3PODSkinMeshNode*>( _allNodes->objectAtIndex(i) );
                if ( pNode )
                {
                    pNode->linkToPODNodes( _allNodes );
                    if ( pNode->getPodParentIndex() < 0 )
                        addNode( pNode );
                }

                break;
            }
            case kTypeCamera:
            {
                CC3PODCamera* pNode = dynamic_cast<CC3PODCamera*>( _allNodes->objectAtIndex(i) );
                if ( pNode )
                {
                    pNode->linkToPODNodes( _allNodes );
                    if ( pNode->getPodParentIndex() < 0 )
                        addNode( pNode );
                }

                break;
            }
            case kTypeLight:
            {
                CC3PODLight* pNode = dynamic_cast<CC3PODLight*>( _allNodes->objectAtIndex(i) );
                if ( pNode )
                {
                    pNode->linkToPODNodes( _allNodes );
                    if ( pNode->getPodParentIndex() < 0 )
                        addNode( pNode );
                }

                break;
            }
            case kTypeBone:
            {
                CC3PODBone* pNode = dynamic_cast<CC3PODBone*>( _allNodes->objectAtIndex(i) );
                if ( pNode )
                {
                    pNode->linkToPODNodes( _allNodes );
                    if ( pNode->getPodParentIndex() < 0 )
                        addNode( pNode );
                }
                
                break;
            }
            case kTypeNode:
            {
                CC3PODNode* pNode = dynamic_cast<CC3PODNode*>( _allNodes->objectAtIndex(i) );
                if ( pNode )
                {
                    pNode->linkToPODNodes( _allNodes );
                    if ( pNode->getPodParentIndex() < 0 )
                        addNode( pNode );
                }
                
                break;
            }
            default:
                break;
        }
    }
}

CC3Node* CC3PODResource::buildNodeAtIndex( GLuint nodeIndex )
{
	// Mesh nodes are arranged first
	if ( nodeIndex < getMeshNodeCount() ) 
		return buildMeshNodeAtIndex(nodeIndex);

	// Then light nodes
	if ( nodeIndex < getMeshNodeCount() + getLightCount() )
		return buildLightAtIndex(nodeIndex - getMeshNodeCount());
	
	// Then camera nodes
	if ( nodeIndex < getMeshNodeCount() + getLightCount() + getCameraCount() )
		return buildCameraAtIndex(nodeIndex - (getMeshNodeCount() + getLightCount()));

	// Finally general nodes, including structural nodes or targets for lights or cameras
	return buildStructuralNodeAtIndex( nodeIndex );
}

CC3Node* CC3PODResource::buildStructuralNodeAtIndex( GLuint nodeIndex )
{
	if ( isBoneNode(nodeIndex) )
		return CC3PODBone::nodeAtIndex( nodeIndex, this );

	return CC3PODNode::nodeAtIndex( nodeIndex, this );
}

PODStructPtr CC3PODResource::getNodePODStructAtIndex( GLuint nodeIndex )
{
	return &getPvrtModelImpl()->pNode[nodeIndex]; 
}

bool CC3PODResource::isNodeIndex( GLint aNodeIndex, GLint childIndex )
{
	// Return YES if nodes are the same
	if (aNodeIndex == childIndex) 
		return true;

	// Get the SPOD structure of the child, and extract the index of its parent node.
	// Return no parent
	SPODNode* psn = (SPODNode*)getNodePODStructAtIndex(childIndex);
	GLint parentIndex = psn->nIdxParent;
	if (parentIndex < 0) 
		return false;

	// Invoke recursion on the index of the parent node
	return isNodeIndex( aNodeIndex, parentIndex );
}

bool CC3PODResource::isBoneNode( GLuint aNodeIndex )
{
	GLuint mCount = getMeshCount();
	// Cycle through the meshes
	for (GLuint mi = 0; mi < mCount; mi++) 
	{
		SPODMesh* psm = (SPODMesh*)getMeshPODStructAtIndex(mi);
		CPVRTBoneBatches* pbb = &psm->sBoneBatches;

		// Cycle through the bone batches within each mesh
		for (GLint batchIndex = 0; batchIndex < pbb->nBatchCnt; batchIndex++) 
		{
			GLint boneCount = pbb->pnBatchBoneCnt[batchIndex];
			GLint* boneNodeIndices = &(pbb->pnBatches[batchIndex * pbb->nBatchBoneMax]);

			// Cycle through the bones of each batch. If the bone node is a child of
			// the specified node, then the specified node is a bone as well.
			for (GLint boneIndex = 0; boneIndex < boneCount; boneIndex++) 
			{
				if ( isNodeIndex(aNodeIndex, boneNodeIndices[boneIndex]) ) 
					return true;
			}
		}
	}
	return false;
}

void CC3PODResource::buildSoftBodyNode()
{
	CCArray* myNodes = getNodes();
	CCArray* softBodyComponents = CCArray::createWithCapacity( myNodes->count() );

	CCObject* pObj = NULL;
	CCARRAY_FOREACH( myNodes, pObj )
	{
		CC3Node* baseNode = (CC3Node*)pObj;
		if ( baseNode->hasSoftBodyContent() )
			softBodyComponents->addObject( baseNode );
	}
	
	if (softBodyComponents->count() > 0) 
	{
		std::string sbName = CC3String::stringWithFormat( (char*)"%s-SoftBody", getName().c_str() );
		CC3SoftBodyNode* sbn = CC3SoftBodyNode::nodeWithName(sbName);
		
		CCObject* pObj = NULL;
		CCARRAY_FOREACH( softBodyComponents, pObj )
		{
			CC3Node* sbc = (CC3Node*)pObj;
			sbn->addChild( sbc );
			removeNode( sbc );
		}

		sbn->bindRestPose();
		addNode( sbn );
	}
}

GLuint CC3PODResource::getMeshNodeCount()
{
	return _pvrtModel ? getPvrtModelImpl()->nNumMeshNode : 0; 
}

// mesh nodes appear first in the node array
CC3MeshNode* CC3PODResource::getMeshNodeAtIndex( GLuint meshIndex )
{
	return (CC3MeshNode*)getNodeAtIndex(meshIndex); 
}

/** If we are vertex skinning, return a skin mesh node, otherwise return a generic mesh node. */
CC3MeshNode* CC3PODResource::buildMeshNodeAtIndex( GLuint meshNodeIndex )
{
	SPODNode* psn = (SPODNode*)getMeshNodePODStructAtIndex(meshNodeIndex);
	SPODMesh* psm = (SPODMesh*)getMeshPODStructAtIndex(psn->nIdx);
	if ( psm->sBoneBatches.nBatchCnt )
		return CC3PODSkinMeshNode::nodeAtIndex( meshNodeIndex, this );

	return CC3PODMeshNode::nodeAtIndex( meshNodeIndex, this );
}

// mesh nodes appear first in the node array
PODStructPtr CC3PODResource::getMeshNodePODStructAtIndex( GLuint meshIndex )
{
	return getNodePODStructAtIndex(meshIndex); 
}

GLuint CC3PODResource::getMeshCount()
{
	return _pvrtModel ? getPvrtModelImpl()->nNumMesh : 0; 
}

// Build the array containing all materials in the PVRT structure
void CC3PODResource::buildMeshes()
{
	GLuint mCount = getMeshCount();
	for (GLuint i = 0; i < mCount; i++) 
		_meshes->addObject( buildMeshAtIndex(i) );
}

CC3Mesh* CC3PODResource::getMeshAtIndex( GLuint meshIndex )
{
	return (CC3Mesh*)_meshes->objectAtIndex(meshIndex); 
}

CC3Mesh* CC3PODResource::buildMeshAtIndex( GLuint meshIndex )
{
	return CC3PODMesh::meshAtIndex( meshIndex, this );
}

PODStructPtr CC3PODResource::getMeshPODStructAtIndex( GLuint meshIndex )
{
	return &getPvrtModelImpl()->pMesh[meshIndex]; 
}

GLuint CC3PODResource::getLightCount()
{
	return _pvrtModel ? getPvrtModelImpl()->nNumLight : 0; 
}

// light nodes appear after all the mesh nodes in the node array
CC3Light* CC3PODResource::getLightAtIndex( GLuint lightIndex )
{
	return (CC3Light*)getNodeAtIndex( getMeshNodeCount() + lightIndex );
}

CC3Light* CC3PODResource::buildLightAtIndex( GLuint lightIndex )
{
	return CC3PODLight::nodeAtIndex( lightIndex, this );
}

// light nodes appear after all the mesh nodes in the node array
PODStructPtr CC3PODResource::getLightNodePODStructAtIndex( GLuint lightIndex )
{
	return getNodePODStructAtIndex( getMeshNodeCount() + lightIndex );
}

PODStructPtr CC3PODResource::getLightPODStructAtIndex( GLuint lightIndex )
{
	return &getPvrtModelImpl()->pLight[lightIndex]; 
}

GLuint CC3PODResource::getCameraCount()
{
	return _pvrtModel ? getPvrtModelImpl()->nNumCamera : 0; 
}

CC3Camera* CC3PODResource::getCameraAtIndex( GLuint cameraIndex )
{
	return (CC3Camera*)getNodeAtIndex( getMeshNodeCount() + getLightCount() + cameraIndex );
}

CC3Camera* CC3PODResource::buildCameraAtIndex( GLuint cameraIndex )
{
	return CC3PODCamera::nodeAtIndex( cameraIndex, this );
}

// camera nodes appear after all the mesh nodes and light nodes in the node array
PODStructPtr CC3PODResource::getCameraNodePODStructAtIndex( GLuint cameraIndex )
{
	return getNodePODStructAtIndex( getMeshNodeCount() + getLightCount() + cameraIndex );
}

PODStructPtr CC3PODResource::getCameraPODStructAtIndex( GLuint cameraIndex )
{
	return &getPvrtModelImpl()->pCamera[cameraIndex];
}

GLuint CC3PODResource::getMaterialCount()
{
	return _pvrtModel ? getPvrtModelImpl()->nNumMaterial : 0; 
}

// Fail safely when node references a material that is not in the POD
CC3Material* CC3PODResource::getMaterialAtIndex( GLuint materialIndex )
{
	if (materialIndex >= _materials->count()) 
	{
		//LogRez(@"This POD has no material at index %i", materialIndex);
		return NULL;
	}
	return (CC3Material*)_materials->objectAtIndex(materialIndex);
}

CC3Material* CC3PODResource::getMaterialNamed( const std::string& aName )
{
	std::string lcName = aName;
	CC3String::makeLowercase( lcName );
	GLuint mCnt = getMaterialCount();
	for (GLuint i = 0; i < mCnt; i++) 
	{
		CC3Material* aMat = getMaterialAtIndex(i);

		std::string matName = aMat->getName();
		CC3String::makeLowercase( matName );
		if (matName == lcName) 
			return aMat;
	}

	return NULL;
}

// Build the array containing all materials in the PVRT structure
void CC3PODResource::buildMaterials()
{
	GLuint mCount = getMaterialCount();
	for (GLuint i = 0; i < mCount; i++) 
		_materials->addObject( buildMaterialAtIndex(i) );
}

CC3Material* CC3PODResource::buildMaterialAtIndex( GLuint materialIndex )
{
	return CC3PODMaterial::materialAtIndex( materialIndex, this );
}

PODStructPtr CC3PODResource::getMaterialPODStructAtIndex( GLuint materialIndex )
{
	return &getPvrtModelImpl()->pMaterial[materialIndex];
}

GLuint CC3PODResource::getTextureCount()
{
	return _pvrtModel ? getPvrtModelImpl()->nNumTexture : 0; 
}

GLfloat CC3PODResource::getAnimationFrameRate()
{
	return _animationFrameRate;
}

GLuint CC3PODResource::getAnimationFrameCount()
{
	return _animationFrameCount;
}

CC3Texture* CC3PODResource::getTextureAtIndex( GLuint textureIndex )
{
	CCObject* tex = _textures->objectAtIndex(textureIndex);
	return (tex == NULL ) ? NULL : (CC3Texture*)tex;
}

void CC3PODResource::buildTextures()
{
	GLuint tCount = getTextureCount();
	
	// Build the array containing all textures in the PVRT structure
	for (GLuint i = 0; i < tCount; i++) 
	{
		CC3Texture* tex = buildTextureAtIndex(i);
		// Add the texture, or if it couldn't be built, an null placeholder
		_textures->addObject(tex ? tex : NULL);
	}
}

/** Loads the texture file from the directory indicated by the directory property. */
CC3Texture* CC3PODResource::buildTextureAtIndex( GLuint textureIndex )
{
	SPODTexture* pst = (SPODTexture*)getTexturePODStructAtIndex(textureIndex);
	std::string texFile = pst->pszName;

#pragma _NOTE_TODO( "pvr and bmp hack here, currently not supported" )
	if ( texFile.find( ".pvr" ) != std::string::npos )
		texFile = "Cocos3D.png";

	if ( texFile.find( ".bmp" ) != std::string::npos )
		texFile = "Cocos3D.png";

	std::string texPath = getDirectory() + texFile;
	CC3Texture* tex = CC3Texture::textureFromFile(texPath.c_str());
	if ( tex )
		tex->setTextureParameters( _textureParameters );

	// LogRez(@"Creating %@ at POD index %u from: '%@'", tex, textureIndex, texPath);
	return tex;
}

PODStructPtr CC3PODResource::getTexturePODStructAtIndex( GLuint textureIndex )
{
	return &getPvrtModelImpl()->pTexture[textureIndex];
}

int CC3PODResource::getNodeType( GLuint podIndex )
{
    // Mesh nodes are arranged first
    if ( podIndex < getMeshNodeCount() )
    {
        SPODNode* psn = (SPODNode*)getMeshNodePODStructAtIndex( podIndex );
        SPODMesh* psm = (SPODMesh*)getMeshPODStructAtIndex( psn->nIdx );
        if ( psm->sBoneBatches.nBatchCnt )
            return kTypeSkinMeshNode;
        
        return kTypeMeshNode;
    }
    
    // Then light nodes
    if ( podIndex < getMeshNodeCount() + getLightCount() )
        return kTypeLight;
    
    // Then camera nodes
    if ( podIndex < getMeshNodeCount() + getLightCount() + getCameraCount() )
        return kTypeCamera;
    
    // Finally general nodes, including structural nodes or targets for lights or cameras
    if ( isBoneNode( podIndex ) )
        return kTypeBone;
    
    return kTypeNode;
}

void CC3PODResource::linkToPODNodes( CC3Node *pNode, int parentIndex, int targetIndex, CCArray* nodeArray )
{
    if ( parentIndex >= 0 )
    {
        //LogTrace(@"Linking %@ with parent index %i", self, self.podParentIndex);
        CC3Node* parentNode = (CC3Node*)nodeArray->objectAtIndex( parentIndex );
        parentNode->addChild( pNode );
    }
    if ( targetIndex >= 0 )
    {
        //LogTrace(@"Linking %@ with target index %i", self, self.podTargetIndex);
        pNode->setTarget( (CC3Node*)nodeArray->objectAtIndex( targetIndex ) );
    }
}

CC3PODResource* CC3PODResource::resourceFromFile( const std::string& filePath )
{
	CC3PODResource* rez = (CC3PODResource*)getResourceNamed( resourceNameFromFilePath(filePath) );
	if (rez) 
		return rez;

	rez = new CC3PODResource;
	rez->initFromFile( filePath );
	addResource( rez );
	rez->autorelease();

	return rez;
}

NS_COCOS3D_END
