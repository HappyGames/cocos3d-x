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
#ifndef _CCL_CC3PROJECTIONMATRIX_H_
#define _CCL_CC3PROJECTIONMATRIX_H_

NS_COCOS3D_BEGIN

/**
 * CC3ProjectionMatrix is a 4x4 matrix that can represent many non-linear 3D transforms,
 * including perspective projection and orthographic projection.
 *
 * Although its primary intended use is to act as a projection matrix for the 3D camera (hence its
 * name), as a full 4x4 matrix, a number of other non-linear transforms may be created with this class.
 *
 * Internally, the dimensions of this matrix are four columns by four rows. Data is held
 * in a CC3Matrix4x4 structure of 16 GLfloat elements in column-major order.
 */
class CC3ProjectionMatrix : public CC3Matrix 
{
	DECLARE_SUPER( CC3Matrix );

public:
	CC3ProjectionMatrix();

	static CC3ProjectionMatrix* matrix();

	virtual void			transpose();
	virtual CC3Vector		extractRotation();
	virtual CC3Quaternion	extractQuaternion();
	virtual CC3Vector		extractForwardDirection();
	virtual CC3Vector		extractUpDirection();
	virtual CC3Vector		extractRightDirection();
	virtual CC3Vector		extractTranslation();
	virtual bool			invertAdjoint();
	virtual void			invertRigid();
	virtual void			populateFromCC3Matrix3x3( CC3Matrix3x3* mtx );
	virtual void			populateCC3Matrix3x3( CC3Matrix3x3* mtx );
	virtual void			populateFromCC3Matrix4x3( CC3Matrix4x3* mtx );
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
	virtual CC3Vector		transformLocation( CC3Vector aLocation );
	virtual CC3Vector		transformDirection( CC3Vector aDirection );
	virtual CC3Vector4		transformHomogeneousVector( CC3Vector4 aVector );

protected:
	virtual void			implPopulateZero();
	virtual void			implPopulateIdentity();
	virtual void			implPopulateFrom( CC3Matrix* aMatrix );
	virtual void			implPopulateFromCC3Matrix3x3( CC3Matrix3x3* mtx );
	virtual void			implPopulateFromCC3Matrix4x3( CC3Matrix4x3* mtx );
	virtual void			implPopulateFromCC3Matrix4x4( CC3Matrix4x4* mtx );
	virtual void			implPopulateFromRotation( CC3Vector aRotation );
	virtual void			implPopulateFromQuaternion( CC3Quaternion aQuaternion );
	virtual void			implPopulateFromScale( CC3Vector aScale );

	virtual void			implPopulateFromTranslation( CC3Vector aTranslation );
	virtual void			implPopulateToPointTowards( CC3Vector fwdDirection, CC3Vector upDirection );
	virtual void			implPopulateFromFrustumLeft( GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat nearval, GLfloat farval );
	virtual void			implPopulateFromFrustumLeft( GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat nearval );
	virtual void			implPopulateOrthoFromFrustumLeft(GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat nearval, GLfloat farval );
	virtual void			implPopulateOrthoFromFrustumLeft(GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat nearval );
	virtual void			implRotateBy( CC3Vector aRotation );
	virtual void			implRotateByQuaternion( CC3Quaternion aQuaternion );
	virtual void			implScaleBy( CC3Vector aScale );
	virtual void			implTranslateBy( CC3Vector aTranslation );
	virtual void			implMultiplyBy( CC3Matrix* aMatrix );
	virtual void			implLeftMultiplyBy( CC3Matrix* aMatrix );

protected:
	CC3Matrix4x4			_contents;
};

NS_COCOS3D_END

#endif
