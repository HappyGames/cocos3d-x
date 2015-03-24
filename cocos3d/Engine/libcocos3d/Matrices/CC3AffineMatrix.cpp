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

CC3AffineMatrix::CC3AffineMatrix()
{

}

CC3AffineMatrix* CC3AffineMatrix::matrix()
{
	CC3AffineMatrix* pMat = new CC3AffineMatrix;
	pMat->init();
	pMat->autorelease();

	return pMat;
}

void CC3AffineMatrix::implPopulateZero()
{ 
	CC3Matrix4x3PopulateZero( &m_contents ); 
}

void CC3AffineMatrix::implPopulateIdentity()
{
	CC3Matrix4x3PopulateIdentity( &m_contents ); 
}

// Double-dispatch to the other matrix
void CC3AffineMatrix::implPopulateFrom( CC3Matrix* aMatrix )
{ 
	aMatrix->populateCC3Matrix4x3( &m_contents ); 
}

void CC3AffineMatrix::implPopulateFromCC3Matrix3x3( CC3Matrix3x3* mtx )
{ 
	CC3Matrix4x3PopulateFrom3x3( &m_contents, mtx );
}

void CC3AffineMatrix::populateCC3Matrix3x3( CC3Matrix3x3* mtx )
{ 
	CC3Matrix3x3PopulateFrom4x3( mtx, &m_contents ); 
}

void CC3AffineMatrix::implPopulateFromCC3Matrix4x3( CC3Matrix4x3* mtx )
{ 
	CC3Matrix4x3PopulateFrom4x3( &m_contents, mtx ); 
}

void CC3AffineMatrix::populateCC3Matrix4x3( CC3Matrix4x3* mtx )
{ 
	CC3Matrix4x3PopulateFrom4x3( mtx, &m_contents ); 
}

void CC3AffineMatrix::implPopulateFromCC3Matrix4x4( CC3Matrix4x4* mtx )
{ 
	CC3Matrix4x3PopulateFrom4x4( &m_contents, mtx ); 
}

void CC3AffineMatrix::populateCC3Matrix4x4( CC3Matrix4x4* mtx )
{
	CC3Matrix4x4PopulateFrom4x3( mtx, &m_contents ); 
}

void CC3AffineMatrix::implPopulateFromRotation( CC3Vector aRotation )
{
	CC3Matrix4x3PopulateFromRotationYXZ( &m_contents, aRotation );
}

void CC3AffineMatrix::implPopulateFromQuaternion( CC3Quaternion aQuaternion )
{
	CC3Matrix4x3PopulateFromQuaternion( &m_contents, aQuaternion );
}

void CC3AffineMatrix::implPopulateFromScale( CC3Vector aScale )
{
	CC3Matrix4x3PopulateFromScale( &m_contents, aScale );
}

void CC3AffineMatrix::implPopulateFromTranslation( CC3Vector aTranslation )
{
	CC3Matrix4x3PopulateFromTranslation( &m_contents, aTranslation );
}

void CC3AffineMatrix::implPopulateToPointTowards( CC3Vector fwdDirection, CC3Vector upDirection )
{
	CC3Matrix4x3PopulateToPointTowards( &m_contents, fwdDirection, upDirection );
}

void CC3AffineMatrix::implPopulateOrthoFromFrustumLeft( GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat nearval, GLfloat farval )
{
	CC3Matrix4x3PopulateOrthoFrustum( &m_contents, left, right, top, bottom, nearval, farval );
}

void CC3AffineMatrix::implPopulateOrthoFromFrustumLeft( GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat nearval ) 
{
	CC3Matrix4x3PopulateInfiniteOrthoFrustum( &m_contents, left, right, top, bottom, nearval);
}

CC3Vector CC3AffineMatrix::extractRotation()
{ 
	return CC3Matrix4x3ExtractRotationYXZ( &m_contents ); 
}

CC3Quaternion CC3AffineMatrix::extractQuaternion()
{ 
	return CC3Matrix4x3ExtractQuaternion( &m_contents ); 
}

CC3Vector CC3AffineMatrix::extractForwardDirection() 
{ 
	return CC3Matrix4x3ExtractForwardDirection( &m_contents ); 
}

CC3Vector CC3AffineMatrix::extractUpDirection() 
{ 
	return CC3Matrix4x3ExtractUpDirection( &m_contents ); 
}

CC3Vector CC3AffineMatrix::extractRightDirection()
{ 
	return CC3Matrix4x3ExtractRightDirection( &m_contents ); 
}

CC3Vector CC3AffineMatrix::extractTranslation()
{
	return CC3Matrix4x3ExtractTranslation( &m_contents ); 
}

void CC3AffineMatrix::implRotateBy( CC3Vector aRotation )
{ 
	CC3Matrix4x3RotateYXZBy( &m_contents, aRotation ); 
}

void CC3AffineMatrix::implRotateByQuaternion( CC3Quaternion aQuaternion )
{
	CC3Matrix4x3RotateByQuaternion( &m_contents, aQuaternion );
}

void CC3AffineMatrix::orthonormalizeRotationStartingWith( unsigned int startColNum )
{
	CC3Matrix4x3Orthonormalize( &m_contents, startColNum );
}

void CC3AffineMatrix::implScaleBy( CC3Vector aScale )
{ 
	CC3Matrix4x3ScaleBy( &m_contents, aScale ); 
}

void CC3AffineMatrix::implTranslateBy( CC3Vector aTranslation )
{ 
	CC3Matrix4x3TranslateBy( &m_contents, aTranslation ); 
}

void CC3AffineMatrix::implMultiplyBy( CC3Matrix* aMatrix )
{
	aMatrix->multiplyIntoCC3Matrix4x3( &m_contents );
}

void CC3AffineMatrix::multiplyIntoCC3Matrix3x3( CC3Matrix3x3* mtx )
{
	if (m_isIdentity) 
		return;
	CC3Matrix4x3 mRslt, mtx4;
	CC3Matrix4x3PopulateFrom3x3(&mtx4, mtx);
	CC3Matrix4x3Multiply(&mRslt, &mtx4, &m_contents);
	CC3Matrix3x3PopulateFrom4x3(mtx, &mRslt);
}

void CC3AffineMatrix::multiplyByCC3Matrix3x3( CC3Matrix3x3* mtx )
{
	if (m_isIdentity) 
	{
		CC3Matrix4x3PopulateFrom3x3(&m_contents, mtx);
	} 
	else
	{
		CC3Matrix4x3 mRslt, mtx4;
		CC3Matrix4x3PopulateFrom3x3(&mtx4, mtx);
		CC3Matrix4x3Multiply(&mRslt, &m_contents, &mtx4);
		CC3Matrix4x3PopulateFrom4x3(&m_contents, &mRslt);
	}
}

void CC3AffineMatrix::multiplyIntoCC3Matrix4x3( CC3Matrix4x3* mtx )
{
	if (m_isIdentity) 
		return;
	CC3Matrix4x3 mRslt;
	CC3Matrix4x3Multiply(&mRslt, mtx, &m_contents);
	CC3Matrix4x3PopulateFrom4x3(mtx, &mRslt);
}

void CC3AffineMatrix::multiplyByCC3Matrix4x3( CC3Matrix4x3* mtx )
{
	if (m_isIdentity)
	{
		CC3Matrix4x3PopulateFrom4x3(&m_contents, mtx);
	} 
	else 
	{
		CC3Matrix4x3 mRslt;
		CC3Matrix4x3Multiply(&mRslt, &m_contents, mtx);
		CC3Matrix4x3PopulateFrom4x3(&m_contents, &mRslt);
	}
}

void CC3AffineMatrix::multiplyIntoCC3Matrix4x4( CC3Matrix4x4* mtx )
{
	if (m_isIdentity) 
		return;
	CC3Matrix4x4 mRslt, mMine;
	CC3Matrix4x4PopulateFrom4x3(&mMine, &m_contents);
	CC3Matrix4x4Multiply(&mRslt, mtx, &mMine);
	CC3Matrix4x4PopulateFrom4x4(mtx, &mRslt);
}

void CC3AffineMatrix::multiplyByCC3Matrix4x4( CC3Matrix4x4* mtx )
{
	if (m_isIdentity) 
	{
		CC3Matrix4x3PopulateFrom4x4(&m_contents, mtx);
	} 
	else 
	{
		CC3Matrix4x4 mRslt, mMine;
		CC3Matrix4x4PopulateFrom4x3(&mMine, &m_contents);
		CC3Matrix4x4Multiply(&mRslt, &mMine, mtx);
		CC3Matrix4x3PopulateFrom4x4(&m_contents, &mRslt);
	}
}

void CC3AffineMatrix::implLeftMultiplyBy( CC3Matrix* aMatrix )
{
	aMatrix->leftMultiplyIntoCC3Matrix4x3( &m_contents );
}

void CC3AffineMatrix::leftMultiplyIntoCC3Matrix3x3( CC3Matrix3x3* mtx )
{
	if (m_isIdentity) 
		return;

	CC3Matrix4x3 mRslt, mtx4;
	CC3Matrix4x3PopulateFrom3x3(&mtx4, mtx);
	CC3Matrix4x3Multiply(&mRslt, &m_contents, &mtx4);
	CC3Matrix3x3PopulateFrom4x3(mtx, &mRslt);
}

void CC3AffineMatrix::leftMultiplyByCC3Matrix3x3( CC3Matrix3x3* mtx )
{
	if (m_isIdentity) 
	{
		CC3Matrix4x3PopulateFrom3x3(&m_contents, mtx);
	} 
	else 
	{
		CC3Matrix4x3 mRslt, mtx4;
		CC3Matrix4x3PopulateFrom3x3(&mtx4, mtx);
		CC3Matrix4x3Multiply(&mRslt, &mtx4, &m_contents);
		CC3Matrix4x3PopulateFrom4x3(&m_contents, &mRslt);
	}
}

void CC3AffineMatrix::leftMultiplyIntoCC3Matrix4x3( CC3Matrix4x3* mtx )
{
	if (m_isIdentity) 
		return;
	CC3Matrix4x3 mRslt;
	CC3Matrix4x3Multiply(&mRslt, &m_contents, mtx);
	CC3Matrix4x3PopulateFrom4x3(mtx, &mRslt);
}

void CC3AffineMatrix::leftMultiplyByCC3Matrix4x3( CC3Matrix4x3* mtx )
{
	if (m_isIdentity) 
	{
		CC3Matrix4x3PopulateFrom4x3(&m_contents, mtx);
	}
	else
	{
		CC3Matrix4x3 mRslt;
		CC3Matrix4x3Multiply(&mRslt, mtx, &m_contents);
		CC3Matrix4x3PopulateFrom4x3(&m_contents, &mRslt);
	}
}

void CC3AffineMatrix::leftMultiplyIntoCC3Matrix4x4( CC3Matrix4x4* mtx )
{
	if (m_isIdentity) 
		return;
	CC3Matrix4x4 mRslt, mMine;
	CC3Matrix4x4PopulateFrom4x3(&mMine, &m_contents);
	CC3Matrix4x4Multiply(&mRslt, &mMine, mtx);
	CC3Matrix4x4PopulateFrom4x4(mtx, &mRslt);
}

void CC3AffineMatrix::leftMultiplyByCC3Matrix4x4( CC3Matrix4x4* mtx )
{
	if (m_isIdentity) 
	{
		CC3Matrix4x3PopulateFrom4x4(&m_contents, mtx);
	} 
	else 
	{
		CC3Matrix4x4 mRslt, mMine;
		CC3Matrix4x4PopulateFrom4x3(&mMine, &m_contents);
		CC3Matrix4x4Multiply(&mRslt, mtx, &mMine);
		CC3Matrix4x3PopulateFrom4x4(&m_contents, &mRslt);
	}
}

// Short-circuit if this is an identity matrix
CC3Vector CC3AffineMatrix::transformLocation( CC3Vector v )
{
	if (m_isIdentity) 
		return v;

	return CC3Matrix4x3TransformLocation(&m_contents, v);
}

// Short-circuit if this is an identity matrix
CC3Vector CC3AffineMatrix::transformDirection( CC3Vector v )
{
	if (m_isIdentity) 
		return v;

	return CC3Matrix4x3TransformDirection(&m_contents, v);
}

// Short-circuit if this is an identity matrix
CC3Vector4 CC3AffineMatrix::transformHomogeneousVector( CC3Vector4 aVector )
{
	if (m_isIdentity) 
		return aVector;

	return CC3Matrix4x3TransformCC3Vector4(&m_contents, aVector);
}

// Short-circuit if this is an identity matrix
void CC3AffineMatrix::transpose() 
{ 
	if ( !m_isIdentity ) 
		CC3Matrix4x3Transpose(&m_contents); 
}

// Short-circuit if this is an identity matrix
bool CC3AffineMatrix::invertAdjoint() 
{
	if (m_isIdentity) 
		return true;

	return CC3Matrix4x3InvertAdjoint(&m_contents);
}

// Short-circuit if this is an identity matrix
void CC3AffineMatrix::invertRigid() 
{ 
	if ( !m_isIdentity ) 
		CC3Matrix4x3InvertRigid(&m_contents); 
}

NS_COCOS3D_END
