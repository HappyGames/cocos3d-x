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

CC3ProjectionMatrix* CC3ProjectionMatrix::matrix()
{
	CC3ProjectionMatrix* pMat = new CC3ProjectionMatrix;
	pMat->init();
	pMat->autorelease();
	return pMat;
}

CC3ProjectionMatrix::CC3ProjectionMatrix()
{
}

void CC3ProjectionMatrix::implPopulateZero() 
{ 
	CC3Matrix4x4PopulateZero(&_contents); 
}

void CC3ProjectionMatrix::implPopulateIdentity() 
{ 
	CC3Matrix4x4PopulateIdentity(&_contents); 
}

// Double-dispatch to the other matrix
void CC3ProjectionMatrix::implPopulateFrom( CC3Matrix* aMatrix )
{ 
	aMatrix->populateCC3Matrix4x4( &_contents ); 
}

void CC3ProjectionMatrix::implPopulateFromCC3Matrix3x3( CC3Matrix3x3* mtx )
{ 
	CC3Matrix4x4PopulateFrom3x3(&_contents, mtx); 
}

void CC3ProjectionMatrix::populateCC3Matrix3x3( CC3Matrix3x3* mtx )
{ 
	CC3Matrix3x3PopulateFrom4x4(mtx, &_contents); 
}

void CC3ProjectionMatrix::implPopulateFromCC3Matrix4x3( CC3Matrix4x3* mtx )
{ 
	CC3Matrix4x4PopulateFrom4x3(&_contents, mtx); 
}

void CC3ProjectionMatrix::populateCC3Matrix4x3( CC3Matrix4x3* mtx )
{ 
	CC3Matrix4x3PopulateFrom4x4(mtx, &_contents); 
}

void CC3ProjectionMatrix::implPopulateFromCC3Matrix4x4( CC3Matrix4x4* mtx )
{ 
	CC3Matrix4x4PopulateFrom4x4(&_contents, mtx);
}

void CC3ProjectionMatrix::populateCC3Matrix4x4( CC3Matrix4x4* mtx )
{ 
	CC3Matrix4x4PopulateFrom4x4(mtx, &_contents); 
}

void CC3ProjectionMatrix::implPopulateFromRotation( CC3Vector aRotation )
{
	CC3Matrix4x4PopulateFromRotationYXZ(&_contents, aRotation);
}

void CC3ProjectionMatrix::implPopulateFromQuaternion( CC3Quaternion aQuaternion )
{
	CC3Matrix4x4PopulateFromQuaternion(&_contents, aQuaternion);
}

void CC3ProjectionMatrix::implPopulateFromScale( CC3Vector aScale )
{
	CC3Matrix4x4PopulateFromScale(&_contents, aScale);
}

void CC3ProjectionMatrix::implPopulateFromTranslation( CC3Vector aTranslation )
{
	CC3Matrix4x4PopulateFromTranslation(&_contents, aTranslation);
}

void CC3ProjectionMatrix::implPopulateToPointTowards( CC3Vector fwdDirection, CC3Vector upDirection )
{
	CC3Matrix4x4PopulateToPointTowards(&_contents, fwdDirection, upDirection);
}

void CC3ProjectionMatrix::implPopulateFromFrustumLeft( GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat nearval, GLfloat farval )
{
	CC3Matrix4x4PopulatePerspectiveFrustum(&_contents, left, right, top, bottom, nearval, farval);
}

void CC3ProjectionMatrix::implPopulateFromFrustumLeft( GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat nearval )
{
	CC3Matrix4x4PopulateInfinitePerspectiveFrustum(&_contents, left, right, top, bottom, nearval);
}

void CC3ProjectionMatrix::implPopulateOrthoFromFrustumLeft( GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat nearval, GLfloat farval )
{
	CC3Matrix4x4PopulateOrthoFrustum(&_contents, left, right, top, bottom, nearval, farval);
}

void CC3ProjectionMatrix::implPopulateOrthoFromFrustumLeft( GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat nearval )
{
	CC3Matrix4x4PopulateInfiniteOrthoFrustum(&_contents, left, right, top, bottom, nearval);
}

CC3Vector CC3ProjectionMatrix::extractRotation() { return CC3Matrix4x4ExtractRotationYXZ(&_contents); }

CC3Quaternion CC3ProjectionMatrix::extractQuaternion() { return CC3Matrix4x4ExtractQuaternion(&_contents); }

CC3Vector CC3ProjectionMatrix::extractForwardDirection() { return CC3Matrix4x4ExtractForwardDirection(&_contents); }

CC3Vector CC3ProjectionMatrix::extractUpDirection() { return CC3Matrix4x4ExtractUpDirection(&_contents); }

CC3Vector CC3ProjectionMatrix::extractRightDirection() { return CC3Matrix4x4ExtractRightDirection(&_contents); }

CC3Vector CC3ProjectionMatrix::extractTranslation() { return CC3Matrix4x4ExtractTranslation(&_contents); }


void CC3ProjectionMatrix::implRotateBy( CC3Vector aRotation ) { CC3Matrix4x4RotateYXZBy(&_contents, aRotation); }

void CC3ProjectionMatrix::implRotateByQuaternion( CC3Quaternion aQuaternion )
{
	CC3Matrix4x4RotateByQuaternion(&_contents, aQuaternion);
}

void CC3ProjectionMatrix::orthonormalizeRotationStartingWith( unsigned int startColNum )
{
	CC3Matrix4x4Orthonormalize(&_contents, startColNum);
}

void CC3ProjectionMatrix::implScaleBy( CC3Vector aScale )
{ 
	CC3Matrix4x4ScaleBy(&_contents, aScale); 
}

void CC3ProjectionMatrix::implTranslateBy( CC3Vector aTranslation )
{ 
	CC3Matrix4x4TranslateBy(&_contents, aTranslation); 
}

void CC3ProjectionMatrix::implMultiplyBy( CC3Matrix* aMatrix )
{
	aMatrix->multiplyIntoCC3Matrix4x4( &_contents );
}

void CC3ProjectionMatrix::multiplyIntoCC3Matrix3x3( CC3Matrix3x3* mtx )
{
	if (m_isIdentity) return;
	CC3Matrix4x4 mRslt, mtx4;
	CC3Matrix4x4PopulateFrom3x3(&mtx4, mtx);
	CC3Matrix4x4Multiply(&mRslt, &mtx4, &_contents);
	CC3Matrix3x3PopulateFrom4x4(mtx, &mRslt);
}

void CC3ProjectionMatrix::multiplyByCC3Matrix3x3( CC3Matrix3x3* mtx )
{
	if (m_isIdentity) {
		CC3Matrix4x4PopulateFrom3x3(&_contents, mtx);
	} else {
		CC3Matrix4x4 mRslt, mtx4;
		CC3Matrix4x4PopulateFrom3x3(&mtx4, mtx);
		CC3Matrix4x4Multiply(&mRslt, &_contents, &mtx4);
		CC3Matrix4x4PopulateFrom4x4(&_contents, &mRslt);
	}
}

void CC3ProjectionMatrix::multiplyIntoCC3Matrix4x3( CC3Matrix4x3* mtx )
{
	if (m_isIdentity) return;
	CC3Matrix4x4 mRslt, mtx4;
	CC3Matrix4x4PopulateFrom4x3(&mtx4, mtx);
	CC3Matrix4x4Multiply(&mRslt, &mtx4, &_contents);
	CC3Matrix4x3PopulateFrom4x4(mtx, &mRslt);
}

void CC3ProjectionMatrix::multiplyByCC3Matrix4x3( CC3Matrix4x3* mtx )
{
	if (m_isIdentity) {
		CC3Matrix4x4PopulateFrom4x3(&_contents, mtx);
	} else {
		CC3Matrix4x4 mRslt, mtx4;
		CC3Matrix4x4PopulateFrom4x3(&mtx4, mtx);
		CC3Matrix4x4Multiply(&mRslt, &_contents, &mtx4);
		CC3Matrix4x4PopulateFrom4x4(&_contents, &mRslt);
	}
}

void CC3ProjectionMatrix::multiplyIntoCC3Matrix4x4( CC3Matrix4x4* mtx )
{
	if (m_isIdentity) return;
	CC3Matrix4x4 mRslt;
	CC3Matrix4x4Multiply(&mRslt, mtx, &_contents);
	CC3Matrix4x4PopulateFrom4x4(mtx, &mRslt);
}

void CC3ProjectionMatrix::multiplyByCC3Matrix4x4( CC3Matrix4x4* mtx )
{
	if (m_isIdentity) {
		CC3Matrix4x4PopulateFrom4x4(&_contents, mtx);
	} else {
		CC3Matrix4x4 mRslt;
		CC3Matrix4x4Multiply(&mRslt, &_contents, mtx);
		CC3Matrix4x4PopulateFrom4x4(&_contents, &mRslt);
	}
}

void CC3ProjectionMatrix::implLeftMultiplyBy( CC3Matrix* aMatrix )
{
	aMatrix->leftMultiplyIntoCC3Matrix4x4( &_contents );
}

void CC3ProjectionMatrix::leftMultiplyIntoCC3Matrix3x3( CC3Matrix3x3* mtx )
{
	if (m_isIdentity) return;
	CC3Matrix4x4 mRslt, mtx4;
	CC3Matrix4x4PopulateFrom3x3(&mtx4, mtx);
	CC3Matrix4x4Multiply(&mRslt, &_contents, &mtx4);
	CC3Matrix3x3PopulateFrom4x4(mtx, &mRslt);
}

void CC3ProjectionMatrix::leftMultiplyByCC3Matrix3x3( CC3Matrix3x3* mtx )
{
	if (m_isIdentity) {
		CC3Matrix4x4PopulateFrom3x3(&_contents, mtx);
	} else {
		CC3Matrix4x4 mRslt, mtx4;
		CC3Matrix4x4PopulateFrom3x3(&mtx4, mtx);
		CC3Matrix4x4Multiply(&mRslt, &mtx4, &_contents);
		CC3Matrix4x4PopulateFrom4x4(&_contents, &mRslt);
	}
}

void CC3ProjectionMatrix::leftMultiplyIntoCC3Matrix4x3( CC3Matrix4x3* mtx )
{
	if (m_isIdentity) return;
	CC3Matrix4x4 mRslt, mtx4;
	CC3Matrix4x4PopulateFrom4x3(&mtx4, mtx);
	CC3Matrix4x4Multiply(&mRslt, &_contents, &mtx4);
	CC3Matrix4x3PopulateFrom4x4(mtx, &mRslt);
}

void CC3ProjectionMatrix::leftMultiplyByCC3Matrix4x3( CC3Matrix4x3* mtx )
{
	if (m_isIdentity) {
		CC3Matrix4x4PopulateFrom4x3(&_contents, mtx);
	} else {
		CC3Matrix4x4 mRslt, mtx4;
		CC3Matrix4x4PopulateFrom4x3(&mtx4, mtx);
		CC3Matrix4x4Multiply(&mRslt, &mtx4, &_contents);
		CC3Matrix4x4PopulateFrom4x4(&_contents, &mRslt);
	}
}

void CC3ProjectionMatrix::leftMultiplyIntoCC3Matrix4x4( CC3Matrix4x4* mtx )
{
	if (m_isIdentity) return;
	CC3Matrix4x4 mRslt;
	CC3Matrix4x4Multiply(&mRslt, &_contents, mtx);
	CC3Matrix4x4PopulateFrom4x4(mtx, &mRslt);
}

void CC3ProjectionMatrix::leftMultiplyByCC3Matrix4x4( CC3Matrix4x4* mtx )
{
	if (m_isIdentity) {
		CC3Matrix4x4PopulateFrom4x4(&_contents, mtx);
	} else {
		CC3Matrix4x4 mRslt;
		CC3Matrix4x4Multiply(&mRslt, mtx, &_contents);
		CC3Matrix4x4PopulateFrom4x4(&_contents, &mRslt);
	}
}

// Short-circuit if this is an identity matrix
CC3Vector CC3ProjectionMatrix::transformLocation( CC3Vector v )
{
	if (m_isIdentity) return v;
	return CC3Matrix4x4TransformLocation(&_contents, v);
}

// Short-circuit if this is an identity matrix
CC3Vector CC3ProjectionMatrix::transformDirection( CC3Vector v )
{
	if (m_isIdentity) return v;
	return CC3Matrix4x4TransformDirection(&_contents, v);
}

// Short-circuit if this is an identity matrix
CC3Vector4 CC3ProjectionMatrix::transformHomogeneousVector( CC3Vector4 aVector )
{
	if (m_isIdentity) return aVector;
	return CC3Matrix4x4TransformCC3Vector4(&_contents, aVector);
}

// Short-circuit if this is an identity matrix
void CC3ProjectionMatrix::transpose() 
{ 
	if ( !m_isIdentity ) 
		CC3Matrix4x4Transpose(&_contents); 
}

// Short-circuit if this is an identity matrix
bool CC3ProjectionMatrix::invertAdjoint() 
{
	if (m_isIdentity) return true;
	return CC3Matrix4x4InvertAdjoint(&_contents);
}

// Short-circuit if this is an identity matrix
void CC3ProjectionMatrix::invertRigid()
{ 
	if ( !m_isIdentity ) 
		CC3Matrix4x4InvertRigid(&_contents); 
}

void CC3ProjectionMatrix::populateFromCC3Matrix3x3( CC3Matrix3x3* mtx )
{
	 CC3Matrix4x4PopulateFrom3x3(&_contents, mtx);
}

void CC3ProjectionMatrix::populateFromCC3Matrix4x3( CC3Matrix4x3* mtx )
{
	CC3Matrix4x4PopulateFrom4x3(&_contents, mtx);
}

NS_COCOS3D_END
