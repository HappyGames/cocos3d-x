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

std::string NSStringFromSPODNode(PODStructPtr pSPODNode) 
{
	SPODNode* psn = (SPODNode*)pSPODNode;
	std::string desc = "";
	desc += stringWithFormat( (char*)"SPODNode named %s", psn->pszName );
	desc += stringWithFormat( (char*)", parent index: %d ", psn->nIdxParent );
	desc += stringWithFormat( (char*)", content index: %d ", psn->nIdx );
	desc += stringWithFormat( (char*)", material index: %d ", psn->nIdxMaterial );
	desc += stringWithFormat( (char*)",\n\tanimation flags: %d", psn->nAnimFlags );
	bool first = true;
	if (psn->nAnimFlags & ePODHasPositionAni) {
		desc += stringWithFormat( (char*)"%s", first ? " (" : " + " );
		desc += stringWithFormat( (char*)"ePODHasPositionAni" );
		first = false;
	}
	if (psn->nAnimFlags & ePODHasRotationAni) {
		desc += stringWithFormat( (char*)"%s", first ? " (" : " + " );
		desc += stringWithFormat( (char*)"ePODHasRotationAni" );
		first = false;
	}
	if (psn->nAnimFlags & ePODHasScaleAni) {
		desc += stringWithFormat( (char*)"%s", first ? " (" : " + " );
		desc += stringWithFormat( (char*)"ePODHasScaleAni" );
		first = false;
	}
	if (psn->nAnimFlags & ePODHasMatrixAni) {
		desc += stringWithFormat( (char*)"%s", first ? " (" : " + " );
		desc += stringWithFormat( (char*)"ePODHasMatrixAni" );
		first = false;
	}
	desc += stringWithFormat( (char*)"%s", first ? "" : ")" );
	desc += stringWithFormat( (char*)"\n\tposition: %s", (psn->pfAnimPosition ? stringFromCC3Vector(*(CC3Vector*)psn->pfAnimPosition).c_str() : "none") );
	desc += stringWithFormat( (char*)", quaternion: %s", (psn->pfAnimRotation ? stringFromCC3Vector4(*(CC3Vector4*)psn->pfAnimRotation).c_str() : "none") );
	desc += stringWithFormat( (char*)", scale: %s", (psn->pfAnimScale ? stringFromCC3Vector(*(CC3Vector*)psn->pfAnimScale).c_str() : "none") );
	desc += stringWithFormat( (char*)", matrix: %s", (psn->pfAnimMatrix ? stringFromCC3Matrix4x4((CC3Matrix4x4*)psn->pfAnimMatrix).c_str() : "none") );
	desc += stringWithFormat( (char*)", %d bytes of user data at %d", psn->nUserDataSize, psn->pUserData );
	return desc;
}

std::string NSStringFromSPODMesh(PODStructPtr pSPODNode) 
{
	SPODMesh* psm = (SPODMesh*)pSPODNode;
	std::string desc = "";
	desc += "SPODMesh drawing ";
	switch (psm->ePrimitiveType) {
		case ePODTriangles:
			desc += "ePODTriangles";
			break;
//		case ePODLines:				// ePODLines not supported as of PVR 2.09
//			[desc appendFormat: @"ePODLines"];
			break;
		default:
			desc += stringWithFormat( (char*)"unknown ePrimitiveType (%d)", psm->ePrimitiveType );
			break;
	}
	desc += stringWithFormat( (char*)"\n\tvertices: %d (%s)", psm->nNumVertex, NSStringFromCPODData(&psm->sVertex).c_str() );
	desc += stringWithFormat( (char*)"\n\t\tnormals: (%s)", NSStringFromCPODData(&psm->sNormals).c_str() );
	desc += stringWithFormat( (char*)"\n\t\ttangents: (%s)", NSStringFromCPODData(&psm->sTangents).c_str() );
	desc += stringWithFormat( (char*)"\n\t\tbinormals: (%s)", NSStringFromCPODData(&psm->sBinormals).c_str() );
	desc += stringWithFormat( (char*)"\n\t\tcolors: (%s)", NSStringFromCPODData(&psm->sVtxColours).c_str() );
	for (GLuint i = 0; i < psm->nNumUVW; i++) {
		desc += stringWithFormat( (char*)"\n\t\ttexmap%u: (%s)", i, NSStringFromCPODData(&psm->psUVW[i]).c_str() );
	}
	desc += stringWithFormat( (char*)"\n\t\tboneIndices: (%s)", NSStringFromCPODData(&psm->sBoneIdx).c_str() );
	desc += stringWithFormat( (char*)"\n\t\tboneWeights: (%s)", NSStringFromCPODData(&psm->sBoneWeight).c_str() );
	desc += stringWithFormat( (char*)"\n\tfaces: %d (%s)", psm->nNumFaces, NSStringFromCPODData(&psm->sFaces).c_str() );
	desc += stringWithFormat( (char*)"\n\tstrips: %d", psm->nNumStrips );
	desc += stringWithFormat( (char*)", texture channels: %d", psm->nNumUVW );
	desc += stringWithFormat( (char*)", interleaved data: %p", psm->pInterleaved );
	
	int batchCount = psm->sBoneBatches.nBatchCnt;
	desc += stringWithFormat( (char*)", bone batches: %d", batchCount );
	
	for (int bbi = 0; bbi < psm->sBoneBatches.nBatchCnt; bbi++) {
		int boneCount = psm->sBoneBatches.pnBatchBoneCnt[bbi];
		desc += stringWithFormat( (char*)"\n\t\tbatch with %d bone nodes:", boneCount );
		bool firstBone = true;
		for (int bi = 0; bi < boneCount; bi++) {
			desc += stringWithFormat( (char*)"%s", firstBone ? " (" : ", " );
			desc += stringWithFormat( (char*)"%d", psm->sBoneBatches.pnBatches[bbi * psm->sBoneBatches.nBatchBoneMax + bi] );
			firstBone = false;
		}
		desc += stringWithFormat( (char*)"%s", firstBone ? "" : ")" );
	}
	return desc;
}

std::string NSStringFromCPODData(PODClassPtr aCPODData) 
{
	CPODData* pcd = (CPODData*)aCPODData;
	std::string desc = "";
	desc += stringWithFormat( (char*)"CPODData type: %s", NSStringFromEPVRTDataType(pcd->eType).c_str() );
	desc += stringWithFormat( (char*)", size: %d", pcd->n );
	desc += stringWithFormat( (char*)", stride: %d", pcd->nStride );
	desc += stringWithFormat( (char*)", data ptr: %d", pcd->pData );
	return desc;
}

std::string NSStringFromCPVRTBoneBatches(PODClassPtr aCPVRTBoneBatches) {
	CPVRTBoneBatches* pbb = (CPVRTBoneBatches*)aCPVRTBoneBatches;
	int batchCount = pbb->nBatchCnt;
	std::string desc = "";
	desc += stringWithFormat( (char*)"CPVRTBoneBatches with %d batches of max %d bones per batch at: %d",
			batchCount, pbb->nBatchBoneMax, pbb->pnBatches );

	if (batchCount) 
	{
		desc += stringWithFormat( (char*)"\n\t\tbone counts: (%d", pbb->pnBatchBoneCnt[0] );
		for (int i = 1; i < batchCount; i++) {
			desc += stringWithFormat( (char*)", %d", pbb->pnBatchBoneCnt[i] );
		}
		desc += ")";

		desc += stringWithFormat( (char*)"\n\t\tbone vertex offsets: (%d", pbb->pnBatchOffset[0] );
		for (int i = 1; i < batchCount; i++) {
			desc += stringWithFormat( (char*)", %d", pbb->pnBatchOffset[i] );
		}
		desc += ")";
	}
	return desc;
}

GLenum GLElementTypeFromEPVRTDataType(GLuint ePVRTDataType) {
	switch (ePVRTDataType) {
		case EPODDataFloat:
			return GL_FLOAT;
		case EPODDataInt:
		case EPODDataUnsignedInt:
			return GL_FIXED;
		case EPODDataByte:
		case EPODDataByteNorm:
			return GL_BYTE;
		case EPODDataUnsignedByte:
		case EPODDataUnsignedByteNorm:
		case EPODDataARGB:
		case EPODDataRGBA:
		case EPODDataUBYTE4:
			return GL_UNSIGNED_BYTE;
		case EPODDataShort:
		case EPODDataShortNorm:
			return GL_SHORT;
		case EPODDataUnsignedShort:
		case EPODDataUnsignedShortNorm:
			return GL_UNSIGNED_SHORT;
		default:
			CCAssert(false, "Unexpected EPVRTDataType '%s'"/*, NSStringFromEPVRTDataType(ePVRTDataType).c_str()*/);
			return GL_UNSIGNED_BYTE;
	}
}

bool CC3ShouldNormalizeEPVRTDataType(GLuint ePVRTDataType) {
	switch (ePVRTDataType) {
		case EPODDataByteNorm:
		case EPODDataUnsignedByteNorm:
		case EPODDataShortNorm:
		case EPODDataUnsignedShortNorm:
		case EPODDataARGB:
		case EPODDataRGBA:
			return true;

		default: return false;
	}
}

std::string NSStringFromEPVRTDataType(GLuint ePVRTDataType) {
	switch (ePVRTDataType) {
		case EPODDataNone:
			return "EPODDataNone";
		case EPODDataFloat:
			return "EPODDataFloat";
		case EPODDataInt:
			return "EPODDataInt";
		case EPODDataUnsignedInt:
			return "EPODDataUnsignedInt";
		case EPODDataByte:
			return "EPODDataByte";
		case EPODDataByteNorm:
			return "EPODDataByteNorm";
		case EPODDataUnsignedByte:
			return "EPODDataUnsignedByte";
		case EPODDataUnsignedByteNorm:
			return "EPODDataUnsignedByteNorm";
		case EPODDataShort:
			return "EPODDataShort";
		case EPODDataShortNorm:
			return "EPODDataShortNorm";
		case EPODDataUnsignedShort:
			return "EPODDataUnsignedShort";
		case EPODDataUnsignedShortNorm:
			return "EPODDataUnsignedShortNorm";
		case EPODDataRGBA:
			return "EPODDataRGBA";
		case EPODDataARGB:
			return "EPODDataARGB";
		case EPODDataD3DCOLOR:
			return "EPODDataD3DCOLOR";
		case EPODDataUBYTE4:
			return "EPODDataUBYTE4";
		case EPODDataDEC3N:
			return "EPODDataDEC3N";
		case EPODDataFixed16_16:
			return "EPODDataFixed16_16";
		default:
			return stringWithFormat((char*)"unknown EPVRTDataType (%d)", ePVRTDataType);
	}
}

GLenum GLDrawingModeForSPODMesh(PODStructPtr aSPODMesh) {
	SPODMesh* psm = (SPODMesh*)aSPODMesh;
	bool usingStrips = psm->nNumStrips > 0;
	switch (psm->ePrimitiveType) {
		case ePODTriangles:
			return usingStrips ? GL_TRIANGLE_STRIP : GL_TRIANGLES;
//		case ePODLines:								// ePODLines not supported as of PVR 2.09
//			return usingStrips ? GL_LINE_STRIP : GL_LINES;
		default:
			CCLOGERROR("Unknown EPODPrimitiveType %d", psm->ePrimitiveType);
			return GL_TRIANGLES;
	}
}

std::string NSStringFromSPODCamera(PODStructPtr pSPODCamera) {
	SPODCamera* psc = (SPODCamera*)pSPODCamera;
	std::string desc = "";
	desc += stringWithFormat( (char*)"SPODCamera fov: %.2f", psc->fFOV );
	desc += stringWithFormat( (char*)", near: %.2f", psc->fNear );
	desc += stringWithFormat( (char*)", far: %.2f", psc->fFar );
	desc += stringWithFormat( (char*)", target index: %i", psc->nIdxTarget );
	desc += stringWithFormat( (char*)", FOV is %sanimated", (psc->pfAnimFOV ? "" : "not ") );
	return desc;
}

std::string NSStringFromSPODLight(PODStructPtr pSPODLight) {
	SPODLight* psl = (SPODLight*)pSPODLight;
	std::string desc = "";
	desc += stringWithFormat( (char*)"SPODLight type: %s", NSStringFromEPODLight(psl->eType).c_str() );
	desc += stringWithFormat( (char*)", color: (%.3f, %.3f, %.3f)", psl->pfColour[0], psl->pfColour[1], psl->pfColour[2] );
	desc += stringWithFormat( (char*)", falloff angle: %.3f", psl->fFalloffAngle );
	desc += stringWithFormat( (char*)", falloff expo: %.3f", psl->fFalloffExponent );
	desc += stringWithFormat( (char*)", const atten: %.3f", psl->fConstantAttenuation );
	desc += stringWithFormat( (char*)", linear atten: %.3f", psl->fLinearAttenuation );
	desc += stringWithFormat( (char*)", quad atten: %3f", psl->fQuadraticAttenuation );
	desc += stringWithFormat( (char*)", target index: %i", psl->nIdxTarget );
	return desc;
}

std::string NSStringFromEPODLight(GLuint ePODLight) {
	switch (ePODLight) {
		case ePODPoint:
			return "ePODPoint";
		case ePODDirectional:
			return "ePODDirectional";
		case ePODSpot:
			return "ePODSpot";
		default:
			return stringWithFormat( (char*)"unknown EPODLight (%d)", ePODLight );
	}
}

std::string NSStringFromSPODMaterial(PODStructPtr pSPODMaterial) {
	SPODMaterial* psm = (SPODMaterial*)pSPODMaterial;
	std::string desc = "";
	desc += stringWithFormat( (char*)"SPODMaterial named %s", psm->pszName );
	desc += stringWithFormat( (char*)"\n\tambient: (%.2f, %.2f, %.2f)", psm->pfMatAmbient[0], psm->pfMatAmbient[1], psm->pfMatAmbient[2] );
	desc += stringWithFormat( (char*)", diffuse: (%.2f, %.2f, %.2f)", psm->pfMatDiffuse[0], psm->pfMatDiffuse[1], psm->pfMatDiffuse[2] );
	desc += stringWithFormat( (char*)", specular: (%.2f, %.2f, %.2f)", psm->pfMatSpecular[0], psm->pfMatSpecular[1], psm->pfMatSpecular[2] );
	desc += stringWithFormat( (char*)", opacity: %.2f", psm->fMatOpacity );
	desc += stringWithFormat( (char*)", shininess: %.2f", psm->fMatShininess );
	desc += stringWithFormat( (char*)"\n\tsrc RGB blend: %s", NSStringFromEPODBlendFunc(psm->eBlendSrcRGB).c_str() );
	desc += stringWithFormat( (char*)", src alpha blend: %s", NSStringFromEPODBlendFunc(psm->eBlendSrcA).c_str() );
	desc += stringWithFormat( (char*)"\n\tdest RGB blend: %s", NSStringFromEPODBlendFunc(psm->eBlendDstRGB).c_str() );
	desc += stringWithFormat( (char*)", dest alpha blend: %s", NSStringFromEPODBlendFunc(psm->eBlendDstA).c_str() );
	desc += stringWithFormat( (char*)"\n\toperation RGB blend: %s", NSStringFromEPODBlendOp(psm->eBlendOpRGB).c_str() );
	desc += stringWithFormat( (char*)", operation alpha blend: %s", NSStringFromEPODBlendOp(psm->eBlendOpA).c_str() );
	desc += stringWithFormat( (char*)"\n\tblend color: (%.2f, %.2f, %.2f, %.2f)", psm->pfBlendColour[0], psm->pfBlendColour[1], psm->pfBlendColour[2], psm->pfBlendColour[3] );
	desc += stringWithFormat( (char*)", blend factor: (%.2f, %.2f, %.2f, %.2f)", psm->pfBlendFactor[0], psm->pfBlendFactor[1], psm->pfBlendFactor[2], psm->pfBlendFactor[3] );
	desc += stringWithFormat( (char*)"\n\ttexture indices: (diffuse: %d", psm->nIdxTexDiffuse );
	desc += stringWithFormat( (char*)", ambient: %d", psm->nIdxTexAmbient );
	desc += stringWithFormat( (char*)", specular color: %d", psm->nIdxTexSpecularColour );
	desc += stringWithFormat( (char*)", specular level: %d", psm->nIdxTexSpecularLevel );
	desc += stringWithFormat( (char*)", bump: %d", psm->nIdxTexBump );
	desc += stringWithFormat( (char*)", emissive: %d", psm->nIdxTexEmissive );
	desc += stringWithFormat( (char*)", gloss: %d", psm->nIdxTexGlossiness );
	desc += stringWithFormat( (char*)", opacity: %d", psm->nIdxTexOpacity );
	desc += stringWithFormat( (char*)", reflection: %d", psm->nIdxTexReflection );
	desc += stringWithFormat( (char*)", refraction: %d)", psm->nIdxTexRefraction );
	desc += stringWithFormat( (char*)"\n\tflags: %d", psm->nFlags );
	desc += stringWithFormat( (char*)", effect %s in file %s",
			(psm->pszEffectName ? psm->pszEffectName : "none"),
			(psm->pszEffectFile ? psm->pszEffectFile : "none") );
	
	return desc;
}

GLenum GLBlendFuncFromEPODBlendFunc(GLuint ePODBlendFunc) {
	switch (ePODBlendFunc) {
		case ePODBlendFunc_ZERO:
		case ePODBlendFunc_ONE:
		case ePODBlendFunc_SRC_COLOR:
		case ePODBlendFunc_ONE_MINUS_SRC_COLOR:
		case ePODBlendFunc_SRC_ALPHA:
		case ePODBlendFunc_ONE_MINUS_SRC_ALPHA:
		case ePODBlendFunc_DST_ALPHA:
		case ePODBlendFunc_ONE_MINUS_DST_ALPHA:
		case ePODBlendFunc_DST_COLOR:
		case ePODBlendFunc_ONE_MINUS_DST_COLOR:
		case ePODBlendFunc_SRC_ALPHA_SATURATE:
			return (GLenum)ePODBlendFunc;
		default:
			//LogError(@"Unknown EPODBlendFunc %u", ePODBlendFunc);
			return GL_ONE;
	}
}

std::string NSStringFromEPODBlendFunc(GLuint ePODBlendFunc) {
	switch (ePODBlendFunc) {
		case ePODBlendFunc_ZERO:
			return "ePODBlendFunc_ZERO";
		case ePODBlendFunc_ONE:
			return "ePODBlendFunc_ONE";
		case ePODBlendFunc_BLEND_FACTOR:
			return "ePODBlendFunc_BLEND_FACTOR";
		case ePODBlendFunc_ONE_MINUS_BLEND_FACTOR:
			return "ePODBlendFunc_ONE_MINUS_BLEND_FACTOR";
		case ePODBlendFunc_SRC_COLOR:
			return "ePODBlendFunc_SRC_COLOR";
		case ePODBlendFunc_ONE_MINUS_SRC_COLOR:
			return "ePODBlendFunc_ONE_MINUS_SRC_COLOR";
		case ePODBlendFunc_SRC_ALPHA:
			return "ePODBlendFunc_SRC_ALPHA";
		case ePODBlendFunc_ONE_MINUS_SRC_ALPHA:
			return "ePODBlendFunc_ONE_MINUS_SRC_ALPHA";
		case ePODBlendFunc_DST_ALPHA:
			return "ePODBlendFunc_DST_ALPHA";
		case ePODBlendFunc_ONE_MINUS_DST_ALPHA:
			return "ePODBlendFunc_ONE_MINUS_DST_ALPHA";
		case ePODBlendFunc_DST_COLOR:
			return "ePODBlendFunc_DST_COLOR";
		case ePODBlendFunc_ONE_MINUS_DST_COLOR:
			return "ePODBlendFunc_ONE_MINUS_DST_COLOR";
		case ePODBlendFunc_SRC_ALPHA_SATURATE:
			return "ePODBlendFunc_SRC_ALPHA_SATURATE";
		case ePODBlendFunc_CONSTANT_COLOR:
			return "ePODBlendFunc_CONSTANT_COLOR";
		case ePODBlendFunc_ONE_MINUS_CONSTANT_COLOR:
			return "ePODBlendFunc_ONE_MINUS_CONSTANT_COLOR";
		case ePODBlendFunc_CONSTANT_ALPHA:
			return "ePODBlendFunc_CONSTANT_ALPHA";
		case ePODBlendFunc_ONE_MINUS_CONSTANT_ALPHA:
			return "ePODBlendFunc_ONE_MINUS_CONSTANT_ALPHA";
		default:
			return stringWithFormat( (char*)"unknown EPODBlendFunc (%d)", ePODBlendFunc );
	}
}

std::string NSStringFromEPODBlendOp(GLuint ePODBlendOp) {
	switch (ePODBlendOp) {
		case ePODBlendOp_ADD:
			return "ePODBlendOp_ADD";
		case ePODBlendOp_MIN:
			return "ePODBlendOp_MIN";
		case ePODBlendOp_MAX:
			return "ePODBlendOp_MAX";
		case ePODBlendOp_SUBTRACT:
			return "ePODBlendOp_SUBTRACT";
		case ePODBlendOp_REVERSE_SUBTRACT:
			return "ePODBlendOp_REVERSE_SUBTRACT";
		default:
			return stringWithFormat( (char*)"unknown EPODBlendOp (%d)", ePODBlendOp );
	}
}

std::string NSStringFromSPODTexture(PODStructPtr pSPODTexture) {
	SPODTexture* pst = (SPODTexture*)pSPODTexture;
	return stringWithFormat( (char*)"\nSPODTexture filename %s", pst->pszName );
}

std::string NSStringFromSPVRTPFXParserEffect(PFXClassPtr pSPVRTPFXParserEffect) {
	SPVRTPFXParserEffect* pfxEffect = (SPVRTPFXParserEffect*)pSPVRTPFXParserEffect;
	std::string desc = "";
	desc += stringWithFormat( (char*)"SPVRTPFXParserEffect" );
	desc += stringWithFormat( (char*)" named %s", pfxEffect->Name.c_str() );
	desc += stringWithFormat( (char*)"\n\tvertex shader: %s", pfxEffect->VertexShaderName.c_str() );
	desc += stringWithFormat( (char*)"\n\tfragment shader: %s", pfxEffect->FragmentShaderName.c_str() );
	
	CPVRTArray<SPVRTPFXParserSemantic> attributes = pfxEffect->Attributes;
	GLuint attrCount = attributes.GetSize();
	desc += stringWithFormat( (char*)"\n\twith %d attributes:", attrCount );
	for(GLuint i = 0; i < attrCount; i++) {
		desc += stringWithFormat( (char*)"\n\t\t%s:", NSStringFromSPVRTPFXParserSemantic(&attributes[i], "attribute").c_str() );
	}

	CPVRTArray<SPVRTPFXParserSemantic> uniforms = pfxEffect->Uniforms;
	GLuint uniformCount = uniforms.GetSize();
	desc += stringWithFormat( (char*)"\n\twith %d uniforms:", uniformCount );
	for(GLuint i = 0; i < uniformCount; i++) {
		desc += stringWithFormat( (char*)"\n\t\t%s:", NSStringFromSPVRTPFXParserSemantic(&uniforms[i], "uniform").c_str() );
	}
	
	CPVRTArray<SPVRTPFXParserEffectTexture> textures = pfxEffect->Textures;
	GLuint texCount = textures.GetSize();
	desc += stringWithFormat( (char*)"\n\twith %d textures:", texCount );
	for(GLuint i = 0; i < texCount; i++) {
		desc += stringWithFormat( (char*)"\n\t\t%s:", NSStringFromSPVRTPFXParserEffectTexture(&textures[i]).c_str() );
	}
	
	CPVRTArray<SPVRTTargetPair> targets = pfxEffect->Targets;
	GLuint targCount = targets.GetSize();
	desc += stringWithFormat( (char*)"\n\twith %d targets:", targCount );
	for(GLuint i = 0; i < targCount; i++) {
		desc += stringWithFormat( (char*)"\n\t\ttarget named %s of type %s", targets[i].TargetName.c_str(), targets[i].BufferType.c_str() );
	}

	desc += stringWithFormat( (char*)"\n\tannotation: %s", pfxEffect->Annotation.c_str() );
	return desc;
}

std::string NSStringFromSPVRTPFXParserSemantic(PFXClassPtr pSPVRTPFXParserSemantic, std::string typeName) 
{
	SPVRTPFXParserSemantic* pfxSemantic = (SPVRTPFXParserSemantic*)pSPVRTPFXParserSemantic;
	std::string desc = "";
	desc += stringWithFormat( (char*)"SPVRTPFXParserSemantic" );
	desc += stringWithFormat( (char*)" for GLSL %s %s", typeName.c_str(), pfxSemantic->pszName );
	desc += stringWithFormat( (char*)" with semantic %s", pfxSemantic->pszValue );
	desc += stringWithFormat( (char*)" at %d", pfxSemantic->nIdx );
	return desc;
}

std::string NSStringFromSPVRTPFXParserEffectTexture(PFXClassPtr pSPVRTPFXParserEffectTexture) 
{
	SPVRTPFXParserEffectTexture* pfxTex = (SPVRTPFXParserEffectTexture*)pSPVRTPFXParserEffectTexture;
	std::string desc = "";
	desc += stringWithFormat( (char*)"SPVRTPFXParserEffectTexture" );
	desc += stringWithFormat( (char*)" named %s", pfxTex->Name.c_str() );
	desc += stringWithFormat( (char*)" in texture unit %d", pfxTex->nNumber );
	return desc;
}

std::string NSStringFromSPVRTPFXParserShader(PFXClassPtr pSPVRTPFXParserShader) 
{
	SPVRTPFXParserShader* pfxShader = (SPVRTPFXParserShader*)pSPVRTPFXParserShader;
	std::string desc = "";
	desc += stringWithFormat( (char*)"SPVRTPFXParserShader" );
	desc += stringWithFormat( (char*)" named %s", pfxShader->Name.c_str() );
	if (pfxShader->bUseFileName) {
		desc += stringWithFormat( (char*)" from file %s", pfxShader->pszGLSLfile );
	} else {
		desc += stringWithFormat( (char*)" from embedded GLSL source code" );
	}
	return desc;
}

std::string NSStringFromSPVRTPFXParserTexture(PFXClassPtr pSPVRTPFXParserTexture) 
{
	SPVRTPFXParserTexture* pfxTex = (SPVRTPFXParserTexture*)pSPVRTPFXParserTexture;
	std::string desc = "";
	desc += stringWithFormat( (char*)"SPVRTPFXParserTexture" );
	desc += stringWithFormat( (char*)" named %s", pfxTex->Name.c_str() );
	desc += stringWithFormat( (char*)" from file %s", pfxTex->FileName.c_str() );
	desc += stringWithFormat( (char*)" wrap (S,T,R): (%s, %s, %s)", NSStringFromETextureWrap(pfxTex->nWrapS).c_str(),
														NSStringFromETextureWrap(pfxTex->nWrapT).c_str(),
														NSStringFromETextureWrap(pfxTex->nWrapR).c_str() );
	desc += stringWithFormat( (char*)" min: %s", NSStringFromETextureFilter(pfxTex->nMin).c_str() );
	desc += stringWithFormat( (char*)" mag: %s", NSStringFromETextureFilter(pfxTex->nMag).c_str() );
	desc += stringWithFormat( (char*)" mipmap: %s", NSStringFromETextureFilter(pfxTex->nMIP).c_str() );
	desc += stringWithFormat( (char*)" is render target: %s", pfxTex->bRenderToTexture ? "true" : "false" );
	return desc;
}

std::string NSStringFromSPVRTPFXRenderPass(PFXClassPtr pSPVRTPFXRenderPass) 
{
	SPVRTPFXRenderPass* pfxPass = (SPVRTPFXRenderPass*)pSPVRTPFXRenderPass;
	std::string desc = "";
	desc += stringWithFormat( (char*)"SPVRTPFXRenderPass" );
	desc += stringWithFormat( (char*)" to texture %s", pfxPass->pTexture->Name.c_str() );
	return desc;
}

GLenum GLTextureWrapFromETextureWrap(GLuint eTextureWrap) 
{
	switch (eTextureWrap) {
		case eWrap_Clamp:
			return GL_CLAMP_TO_EDGE;
		case eWrap_Repeat:
			return GL_REPEAT;
		default:
			CC3_TRACE("Unknown ETextureWrap '%s'", NSStringFromETextureWrap(eTextureWrap).c_str());
			return GL_REPEAT;
	}
}

std::string NSStringFromETextureWrap(GLuint eTextureWrap) 
{
	switch (eTextureWrap) {
		case eWrap_Clamp:
			return "eWrap_Clamp";
		case eWrap_Repeat:
			return "eWrap_Repeat";
		default:
			return stringWithFormat( (char*)"unknown ETextureWrap (%d)", eTextureWrap );
	}
}

GLenum GLMagnifyingFunctionFromETextureFilter(GLuint eTextureFilter) 
{
	switch (eTextureFilter) 
	{
		case eFilter_Nearest:
			return GL_NEAREST;
		case eFilter_Linear:
			return GL_LINEAR;
		default:
			CC3_TRACE("Unknown ETextureFilter '%s'", NSStringFromETextureFilter(eTextureFilter).c_str());
			return GL_LINEAR;
	}
}

GLenum GLMinifyingFunctionFromMinAndMipETextureFilters(GLuint minETextureFilter, GLuint mipETextureFilter) 
{
	switch(mipETextureFilter) 
	{
		case eFilter_Nearest:							// Standard mipmapping
			switch(minETextureFilter) {
				case eFilter_Nearest:
					return GL_NEAREST_MIPMAP_NEAREST;	// Nearest	- std. Mipmap
				case eFilter_Linear:
				default:
					return GL_LINEAR_MIPMAP_NEAREST;	// Bilinear - std. Mipmap
		}

		case eFilter_Linear:							// Trilinear mipmapping
			switch(minETextureFilter) {
				case eFilter_Nearest:
					return GL_NEAREST_MIPMAP_LINEAR;	// Nearest - Trilinear
				case eFilter_Linear:
				default:
					return GL_LINEAR_MIPMAP_LINEAR;		// Bilinear - Trilinear
		}

		case eFilter_None:								// No mipmapping
		default:
			switch(minETextureFilter) {
				case eFilter_Nearest:
					return GL_NEAREST;					// Nearest - no Mipmap
				case eFilter_Linear:
				default:
					return GL_LINEAR;					// Bilinear - no Mipmap
			}
	}
}

std::string NSStringFromETextureFilter(GLuint eTextureFilter) 
{
	switch (eTextureFilter) {
		case eFilter_Nearest:
			return "eFilter_Nearest";
		case eFilter_Linear:
			return "eFilter_Linear";
		case eFilter_None:
			return "eFilter_None";
		default:
			return stringWithFormat( (char*)"unknown ETextureFilter (%d)", eTextureFilter );
	}
}

NS_COCOS3D_END
