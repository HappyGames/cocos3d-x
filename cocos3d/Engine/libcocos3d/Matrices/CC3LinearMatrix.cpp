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
#include "cocos3d.h"

NS_COCOS3D_BEGIN

CC3LinearMatrix::CC3LinearMatrix()
{

}

CC3LinearMatrix* CC3LinearMatrix::matrix()
{
	CC3LinearMatrix* mat = new CC3LinearMatrix;
	mat->init();
	mat->autorelease();
	return mat;
}

void CC3LinearMatrix::implPopulateZero() 
{ 
	CC3Matrix3x3PopulateZero(&m_contents); 
}

void CC3LinearMatrix::implPopulateIdentity() 
{ 
	CC3Matrix3x3PopulateIdentity(&m_contents); 
}

// Double-dispatch to the other matrix
void CC3LinearMatrix::implPopulateFrom(CC3Matrix* aMatrix )
{ 
	aMatrix->populateCC3Matrix3x3( &m_contents ); 
}

void CC3LinearMatrix::implPopulateFromCC3Matrix3x3( CC3Matrix3x3* mtx )
{ 
	CC3Matrix3x3PopulateFrom3x3(&m_contents, mtx); 
}

void CC3LinearMatrix::populateCC3Matrix3x3( CC3Matrix3x3* mtx )
{ 
	CC3Matrix3x3PopulateFrom3x3(mtx, &m_contents); 
}

void CC3LinearMatrix::implPopulateFromCC3Matrix4x3( CC3Matrix4x3* mtx )
{ 
	CC3Matrix3x3PopulateFrom4x3(&m_contents, mtx); 
}

void CC3LinearMatrix::populateCC3Matrix4x3( CC3Matrix4x3* mtx )
{ 
	CC3Matrix4x3PopulateFrom3x3(mtx, &m_contents); 
}

void CC3LinearMatrix::implPopulateFromCC3Matrix4x4( CC3Matrix4x4* mtx )
{ 
	CC3Matrix3x3PopulateFrom4x4(&m_contents, mtx); 
}

void CC3LinearMatrix::populateCC3Matrix4x4( CC3Matrix4x4* mtx )
{ 
	CC3Matrix4x4PopulateFrom3x3(mtx, &m_contents); 
}

void CC3LinearMatrix::implPopulateFromRotation( const CC3Vector& aRotation )
{
	CC3Matrix3x3PopulateFromRotationYXZ(&m_contents, aRotation);
}

void CC3LinearMatrix::implPopulateFromQuaternion( const CC3Quaternion& aQuaternion )
{
	CC3Matrix3x3PopulateFromQuaternion(&m_contents, aQuaternion);
}

void CC3LinearMatrix::implPopulateFromScale( const CC3Vector& aScale )
{ 
	CC3Matrix3x3PopulateFromScale(&m_contents, aScale); 
}

// Linear matrix unaffected by translation
void CC3LinearMatrix::implPopulateFromTranslation( const CC3Vector& aTranslation )
{ 
	implPopulateIdentity(); 
}

void CC3LinearMatrix::implPopulateToPointTowards( const CC3Vector& fwdDirection, const CC3Vector& upDirection )
{
	CC3Matrix3x3PopulateToPointTowards(&m_contents, fwdDirection, upDirection);
}

CC3Vector CC3LinearMatrix::extractRotation() 
{ 
	return CC3Matrix3x3ExtractRotationYXZ(&m_contents); 
}

CC3Quaternion CC3LinearMatrix::extractQuaternion() 
{ 
	return CC3Matrix3x3ExtractQuaternion(&m_contents); 
}

CC3Vector CC3LinearMatrix::extractForwardDirection() 
{
	return CC3Matrix3x3ExtractForwardDirection(&m_contents); 
}

CC3Vector CC3LinearMatrix::extractUpDirection() 
{ 
	return CC3Matrix3x3ExtractUpDirection(&m_contents); 
}

CC3Vector CC3LinearMatrix::extractRightDirection() 
{ 
	return CC3Matrix3x3ExtractRightDirection(&m_contents); 
}

CC3Vector CC3LinearMatrix::extractTranslation() 
{ 
	return CC3Vector::kCC3VectorZero; 
}

void CC3LinearMatrix::implRotateBy( const CC3Vector& aRotation )
{
	CC3Matrix3x3RotateYXZBy(&m_contents, aRotation); 
}

void CC3LinearMatrix::implRotateByQuaternion( const CC3Quaternion& aQuaternion )
{
	CC3Matrix3x3RotateByQuaternion(&m_contents, aQuaternion);
}

void CC3LinearMatrix::orthonormalizeRotationStartingWith( unsigned int startColNum )
{
	CC3Matrix3x3Orthonormalize(&m_contents, startColNum);
}

void CC3LinearMatrix::implScaleBy( const CC3Vector& aScale )
{ 
	CC3Matrix3x3ScaleBy(&m_contents, aScale); 
}

// Linear matrix unaffected by translation
void CC3LinearMatrix::implTranslateBy( const CC3Vector& aTranslation )
{}

void CC3LinearMatrix::implMultiplyBy( CC3Matrix* aMatrix )
{
	aMatrix->multiplyIntoCC3Matrix3x3( &m_contents );
}

void CC3LinearMatrix::multiplyIntoCC3Matrix3x3( CC3Matrix3x3* mtx )
{
	if (m_isIdentity) return;
	CC3Matrix3x3 mRslt;
	CC3Matrix3x3Multiply(&mRslt, mtx, &m_contents);
	CC3Matrix3x3PopulateFrom3x3(mtx, &mRslt);
}

void CC3LinearMatrix::multiplyByCC3Matrix3x3( CC3Matrix3x3* mtx )
{
	if (m_isIdentity) {
		CC3Matrix3x3PopulateFrom3x3(&m_contents, mtx);
	} else {
		CC3Matrix3x3 mRslt;
		CC3Matrix3x3Multiply(&mRslt, &m_contents, mtx);
		CC3Matrix3x3PopulateFrom3x3(&m_contents, &mRslt);
	}
}

void CC3LinearMatrix::multiplyIntoCC3Matrix4x3( CC3Matrix4x3* mtx )
{
	if (m_isIdentity) return;
	CC3Matrix4x3 mRslt, mMine;
	CC3Matrix4x3PopulateFrom3x3(&mMine, &m_contents);
	CC3Matrix4x3Multiply(&mRslt, mtx, &mMine);
	CC3Matrix4x3PopulateFrom4x3(mtx, &mRslt);
}

void CC3LinearMatrix::multiplyByCC3Matrix4x3( CC3Matrix4x3* mtx )
{
	if (m_isIdentity) {
		CC3Matrix3x3PopulateFrom4x3(&m_contents, mtx);
	} else {
		CC3Matrix4x3 mRslt, mMine;
		CC3Matrix4x3PopulateFrom3x3(&mMine, &m_contents);
		CC3Matrix4x3Multiply(&mRslt, &mMine, mtx);
		CC3Matrix3x3PopulateFrom4x3(&m_contents, &mRslt);
	}
}

void CC3LinearMatrix::multiplyIntoCC3Matrix4x4( CC3Matrix4x4* mtx )
{
	if (m_isIdentity) return;
	CC3Matrix4x4 mRslt, mMine;
	CC3Matrix4x4PopulateFrom3x3(&mMine, &m_contents);
	CC3Matrix4x4Multiply(&mRslt, mtx, &mMine);
	CC3Matrix4x4PopulateFrom4x4(mtx, &mRslt);
}

void CC3LinearMatrix::multiplyByCC3Matrix4x4( CC3Matrix4x4* mtx )
{
	if (m_isIdentity) {
		CC3Matrix3x3PopulateFrom4x4(&m_contents, mtx);
	} else {
		CC3Matrix4x4 mRslt, mMine;
		CC3Matrix4x4PopulateFrom3x3(&mMine, &m_contents);
		CC3Matrix4x4Multiply(&mRslt, &mMine, mtx);
		CC3Matrix3x3PopulateFrom4x4(&m_contents, &mRslt);
	}
}

void CC3LinearMatrix::implLeftMultiplyBy( CC3Matrix* aMatrix )
{
	aMatrix->leftMultiplyIntoCC3Matrix3x3( &m_contents );
}

void CC3LinearMatrix::leftMultiplyIntoCC3Matrix3x3( CC3Matrix3x3* mtx )
{
	if (m_isIdentity) return;
	CC3Matrix3x3 mRslt;
	CC3Matrix3x3Multiply(&mRslt, &m_contents, mtx);
	CC3Matrix3x3PopulateFrom3x3(mtx, &mRslt);
}

void CC3LinearMatrix::leftMultiplyByCC3Matrix3x3( CC3Matrix3x3* mtx )
{
	if (m_isIdentity) {
		CC3Matrix3x3PopulateFrom3x3(&m_contents, mtx);
	} else {
		CC3Matrix3x3 mRslt;
		CC3Matrix3x3Multiply(&mRslt, mtx, &m_contents);
		CC3Matrix3x3PopulateFrom3x3(&m_contents, &mRslt);
	}
}

void CC3LinearMatrix::leftMultiplyIntoCC3Matrix4x3( CC3Matrix4x3* mtx )
{
	if (m_isIdentity) return;
	CC3Matrix4x3 mRslt, mMine;
	CC3Matrix4x3PopulateFrom3x3(&mMine, &m_contents);
	CC3Matrix4x3Multiply(&mRslt, &mMine, mtx);
	CC3Matrix4x3PopulateFrom4x3(mtx, &mRslt);
}

void CC3LinearMatrix::leftMultiplyByCC3Matrix4x3( CC3Matrix4x3* mtx )
{
	if (m_isIdentity) {
		CC3Matrix3x3PopulateFrom4x3(&m_contents, mtx);
	} else {
		CC3Matrix4x3 mRslt, mMine;
		CC3Matrix4x3PopulateFrom3x3(&mMine, &m_contents);
		CC3Matrix4x3Multiply(&mRslt, mtx, &mMine);
		CC3Matrix3x3PopulateFrom4x3(&m_contents, &mRslt);
	}
}

void CC3LinearMatrix::leftMultiplyIntoCC3Matrix4x4( CC3Matrix4x4* mtx )
{
	if (m_isIdentity) return;
	CC3Matrix4x4 mRslt, mMine;
	CC3Matrix4x4PopulateFrom3x3(&mMine, &m_contents);
	CC3Matrix4x4Multiply(&mRslt, &mMine, mtx);
	CC3Matrix4x4PopulateFrom4x4(mtx, &mRslt);
}

void CC3LinearMatrix::leftMultiplyByCC3Matrix4x4( CC3Matrix4x4* mtx )
{
	if (m_isIdentity) {
		CC3Matrix3x3PopulateFrom4x4(&m_contents, mtx);
	} else {
		CC3Matrix4x4 mRslt, mMine;
		CC3Matrix4x4PopulateFrom3x3(&mMine, &m_contents);
		CC3Matrix4x4Multiply(&mRslt, mtx, &mMine);
		CC3Matrix3x3PopulateFrom4x4(&m_contents, &mRslt);
	}
}

// Short-circuit if this is an identity matrix
CC3Vector CC3LinearMatrix::transformLocation( const CC3Vector& aLocation )
{
	if (m_isIdentity) return aLocation;
	return CC3Matrix3x3TransformCC3Vector(&m_contents, aLocation);
}

// Short-circuit if this is an identity matrix
CC3Vector CC3LinearMatrix::transformDirection( const CC3Vector& aDirection )
{
	if (m_isIdentity) return aDirection;
	return CC3Matrix3x3TransformCC3Vector(&m_contents, aDirection);
}

// Short-circuit if this is an identity matrix
CC3Vector4 CC3LinearMatrix::transformHomogeneousVector( const CC3Vector4& v )
{
	if (m_isIdentity) 
		return v;
	
	CC3Vector4 vOut;
	vOut.x = (m_contents.c1r1 * v.x) + (m_contents.c2r1 * v.y) + (m_contents.c3r1 * v.z);
	vOut.y = (m_contents.c1r2 * v.x) + (m_contents.c2r2 * v.y) + (m_contents.c3r2 * v.z);
	vOut.z = (m_contents.c1r3 * v.x) + (m_contents.c2r3 * v.y) + (m_contents.c3r3 * v.z);
	vOut.w = v.w;
	return vOut;
}

// Short-circuit if this is an identity matrix
void CC3LinearMatrix::transpose() 
{
	if (m_isIdentity) return;
	CC3Matrix3x3Transpose(&m_contents);
}

// Short-circuit if this is an identity matrix
bool CC3LinearMatrix::invertAdjoint() 
{
	if (m_isIdentity) return true;
	return CC3Matrix3x3InvertAdjoint(&m_contents);
}

// Short-circuit if this is an identity matrix
// The inverse of a rigid linear matrix is equal to its transpose.
void CC3LinearMatrix::invertRigid() 
{
	if (m_isIdentity) return;
	CC3Matrix3x3Transpose(&m_contents);
}

NS_COCOS3D_END
