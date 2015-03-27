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
#ifndef _CCL_CC3AFFINEMATRIX_H_
#define _CCL_CC3AFFINEMATRIX_H_

NS_COCOS3D_BEGIN

/**
 * CC3AffineMatrix is a 4x3 matrix that can represent affine transforms such as rotation,
 * scaling, reflection, shearing, and translation.
 *
 * Affine matrices differ from linear matrices in that affine matrices can perform
 * translation transformations.
 *
 * Internally, the dimensions of this matrix are four columns by three rows. Data is held in a
 * CC3Matrix4x3 structure of 12 GLfloat elements in column-major order. For situations requiring
 * only 3D affine transformations, this offers a storage savings over using a full 4x4 matrix.
 *
 * Although this matrix has only three rows, it behaves like a square matrix with four columns
 * and four rows, with the missing row always taken to contain (0, 0, 0, 1). Since all affine
 * transforms never change this last row, the requirement to store this last row is dropped in
 * order to reduce memory and calculation overhead. Where operations require this last row to
 * be present, it is temporarily generated automatically.
 */
class CC3AffineMatrix : public CC3Matrix 
{
public:
	CC3AffineMatrix();

	static CC3AffineMatrix*	matrix();

	virtual void			transpose();
	virtual CC3Vector		extractRotation();
	virtual CC3Quaternion	extractQuaternion();
	virtual CC3Vector		extractForwardDirection();
	virtual CC3Vector		extractUpDirection();
	virtual CC3Vector		extractRightDirection();
	virtual CC3Vector		extractTranslation();
	virtual bool			invertAdjoint();
	virtual void			invertRigid();
	//virtual void			populateFromCC3Matrix3x3( CC3Matrix3x3* mtx );
	virtual void			populateCC3Matrix3x3( CC3Matrix3x3* mtx );
	//virtual void			populateFromCC3Matrix4x3( CC3Matrix4x3* mtx );
	virtual void			populateCC3Matrix4x3( CC3Matrix4x3* mtx );
	virtual void			populateCC3Matrix4x4( CC3Matrix4x4* mtx );
	virtual void			orthonormalizeRotationStartingWith( unsigned int startColNum );
	virtual void			multiplyIntoCC3Matrix3x3( CC3Matrix3x3* mtx );
	virtual void			multiplyByCC3Matrix3x3( CC3Matrix3x3* mtx );
	virtual void			multiplyIntoCC3Matrix4x3( CC3Matrix4x3* mtx );
	virtual void			multiplyByCC3Matrix4x3( CC3Matrix4x3* mtx );
	virtual void			multiplyIntoCC3Matrix4x4( CC3Matrix4x4* mtx );
	virtual void			multiplyByCC3Matrix4x4( CC3Matrix4x4* mtx );
	//virtual void			leftMultiplyBy( CC3Matrix* aMatrix );
	virtual void			leftMultiplyIntoCC3Matrix3x3( CC3Matrix3x3* mtx );
	virtual void			leftMultiplyByCC3Matrix3x3( CC3Matrix3x3* mtx );
	virtual void			leftMultiplyIntoCC3Matrix4x3( CC3Matrix4x3* mtx );
	virtual void			leftMultiplyByCC3Matrix4x3( CC3Matrix4x3* mtx );
	virtual void			leftMultiplyIntoCC3Matrix4x4( CC3Matrix4x4* mtx );
	virtual void			leftMultiplyByCC3Matrix4x4( CC3Matrix4x4* mtx );
	virtual CC3Vector		transformLocation( const CC3Vector& aLocation );
	virtual CC3Vector		transformDirection( const CC3Vector& aDirection );
	virtual CC3Vector4		transformHomogeneousVector( const CC3Vector4& aVector );

protected:
	virtual void			implPopulateZero();
	virtual void			implPopulateIdentity();
	virtual void			implPopulateFrom( CC3Matrix* aMatrix );
	virtual void			implPopulateFromCC3Matrix3x3( CC3Matrix3x3* mtx );
	virtual void			implPopulateFromCC3Matrix4x3( CC3Matrix4x3* mtx );
	virtual void			implPopulateFromCC3Matrix4x4( CC3Matrix4x4* mtx );
	virtual void			implPopulateFromRotation( const CC3Vector& aRotation );
	virtual void			implPopulateFromQuaternion( const CC3Quaternion& aQuaternion );
	virtual void			implPopulateFromScale( const CC3Vector& aScale );

	virtual void			implPopulateFromTranslation( const CC3Vector& aTranslation );
	virtual void			implPopulateToPointTowards( const CC3Vector& fwdDirection, const CC3Vector& upDirection );
	//virtual void			implPopulateFromFrustumLeft( GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat nearval, GLfloat farval );
	//virtual void			implPopulateFromFrustumLeft( GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat nearval );
	virtual void			implPopulateOrthoFromFrustumLeft(GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat nearval, GLfloat farval );
	virtual void			implPopulateOrthoFromFrustumLeft(GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat nearval );
	virtual void			implRotateBy( const CC3Vector& aRotation );
	virtual void			implRotateByQuaternion( const CC3Quaternion& aQuaternion );
	virtual void			implScaleBy( const CC3Vector& aScale );
	virtual void			implTranslateBy( const CC3Vector& aTranslation );
	virtual void			implMultiplyBy( CC3Matrix* aMatrix );
	virtual void			implLeftMultiplyBy( CC3Matrix* aMatrix );

protected:
	CC3Matrix4x3			m_contents;
};

NS_COCOS3D_END

#endif
