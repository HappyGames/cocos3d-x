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

CC3Matrix::CC3Matrix()
{
	m_isIdentity = false;	
	m_isRigid = false;
}

void CC3Matrix::init()
{
	populateIdentity();
	m_isDirty = false;
}

bool CC3Matrix::isRigid()
{
	return m_isRigid;
}

bool CC3Matrix::isDirty()
{
	return m_isDirty;
}

void CC3Matrix::setIsDirty( bool dirty )
{
	m_isDirty = dirty;
}

bool CC3Matrix::isIdentity()
{
	return m_isIdentity;
}

CC3Matrix* CC3Matrix::matrix()
{
	CC3Matrix* pMat = new CC3Matrix;
	pMat->init();
	pMat->autorelease();
	return pMat;
}

CC3Matrix* CC3Matrix::matrixByMultiplying( CC3Matrix* mL, CC3Matrix* mR )
{
	CC3Matrix* m = (CC3Matrix*)mL->copy();
	m->multiplyBy( mR );
	m->autorelease();

	return m;
}

CCObject* CC3Matrix::copyWithZone( CCZone* zone )
{
	CC3Matrix* aCopy = new CC3Matrix;
	aCopy->populateFrom( this );
	return aCopy;
}

void CC3Matrix::populateZero() 
{
	implPopulateZero();
	m_isIdentity = false;
	m_isRigid = false;
}

/** Template method. Subclasses will provide implementation. */
void CC3Matrix::implPopulateZero() 
{
	CCAssert(false, "the implPopulateZero method is not implemented");
}

void CC3Matrix::populateIdentity()
{
	if (!m_isIdentity) 
	{
		implPopulateIdentity();
		m_isIdentity = true;
		m_isRigid = true;
	}
}

/** Template method. Subclasses will provide implementation. */
void CC3Matrix::implPopulateIdentity() 
{
	CCAssert(false, "the implPopulateIdentity method is not implemented");
}

void CC3Matrix::populateFrom( CC3Matrix* aMatrix )
{
	if (!aMatrix || aMatrix->isIdentity()) 
	{
		populateIdentity();
	} 
	else 
	{
		implPopulateFrom( aMatrix );
		m_isIdentity = false;
		m_isRigid = aMatrix->isRigid();
	}
}

/**
 * Template method. Subclasses will provide implementation.
 * Subclass implementations will double-dispatch to the other matrix.
 * Subclass implementation does not need to set isIdentity or isRigid.
 */
void CC3Matrix::implPopulateFrom( CC3Matrix* aMatrix )
{
	CCAssert(false, "the implPopulateFrom: method is not implemented");
}

void CC3Matrix::populateFromCC3Matrix3x3( CC3Matrix3x3* mtx )
{
	implPopulateFromCC3Matrix3x3( mtx );
	m_isIdentity = CC3Matrix3x3IsIdentity(mtx);
	m_isRigid = m_isIdentity;
}

void CC3Matrix::implPopulateFromCC3Matrix3x3( CC3Matrix3x3* mtx )
{
	CCAssert(false, "%the implPopulateFromCC3Matrix3x3: method is not implemented");
}

void CC3Matrix::populateCC3Matrix3x3( CC3Matrix3x3* mtx )
{
	CCAssert(false, "%the populateCC3Matrix3x3: method is not implemented");
}

void CC3Matrix::populateFromCC3Matrix4x3( CC3Matrix4x3* mtx )
{
	implPopulateFromCC3Matrix4x3( mtx );
	m_isIdentity = CC3Matrix4x3IsIdentity(mtx);
	m_isRigid = m_isIdentity;
}

void CC3Matrix::implPopulateFromCC3Matrix4x3( CC3Matrix4x3* mtx )
{
	CCAssert(false, "the implPopulateFromCC3Matrix3x3: method is not implemented");
}

void CC3Matrix::populateCC3Matrix4x3( CC3Matrix4x3* mtx ) 
{
	CCAssert(false, "the populateCC3Matrix4x3: method is not implemented");
}

void CC3Matrix::populateFromCC3Matrix4x4( CC3Matrix4x4* mtx )
{
	implPopulateFromCC3Matrix4x4( mtx );
	m_isIdentity = CC3Matrix4x4IsIdentity(mtx);
	m_isRigid = m_isIdentity;
}

void CC3Matrix::implPopulateFromCC3Matrix4x4( CC3Matrix4x4* mtx )
{
	CCAssert(false, "the implPopulateFromCC3Matrix4x4: method is not implemented");
}

void CC3Matrix::populateCC3Matrix4x4( CC3Matrix4x4* mtx )
{
	CCAssert(false, "the populateCC3Matrix4x4: method is not implemented");
}

void CC3Matrix::populateFromRotation( CC3Vector aRotation )
{
	if (CC3VectorsAreEqual(aRotation, kCC3VectorZero)) 
	{
		populateIdentity();
	} 
	else 
	{
		implPopulateFromRotation( aRotation );
		m_isIdentity = false;
		m_isRigid = true;
	}
}

/** Template method. Subclasses will provide implementation. */
void CC3Matrix::implPopulateFromRotation( CC3Vector aRotation )
{
	CCAssert(false, "the implPopulateFromRotation: method is not implemented");
}

void CC3Matrix::populateFromQuaternion( CC3Quaternion aQuaternion )
{
	if (CC3QuaternionsAreEqual(aQuaternion, kCC3QuaternionIdentity)) {
		populateIdentity();
	} else {
		implPopulateFromQuaternion( aQuaternion );
		m_isIdentity = false;
		m_isRigid = true;
	}
}

/** Template method. Subclasses will provide implementation. */
void CC3Matrix::implPopulateFromQuaternion( CC3Quaternion aQuaternion )
{
	CCAssert(false, "the implPopulateFromQuaternion: method is not implemented");
}

void CC3Matrix::populateFromScale( CC3Vector aScale ) 
{
	if (CC3VectorsAreEqual(aScale, kCC3VectorUnitCube)) {
		populateIdentity();
	} else {
		implPopulateFromScale( aScale );
		m_isIdentity = false;
		m_isRigid = false;
	}
}

/** Template method. Subclasses will provide implementation. */
void CC3Matrix::implPopulateFromScale( CC3Vector aScale )
{
	CCAssert(false, "the implPopulateFromScale: method is not implemented");
}

void CC3Matrix::populateFromTranslation( CC3Vector aTranslation ){
	if (CC3VectorsAreEqual(aTranslation, kCC3VectorZero)) {
		populateIdentity();
	} else {
		implPopulateFromTranslation( aTranslation );
		m_isIdentity = false;
		m_isRigid = true;
	}
}

/** Template method. Subclasses will provide implementation. */
void CC3Matrix::implPopulateFromTranslation( CC3Vector aTranslation )
{
	CCAssert(false, "the implPopulateFromTranslation: method is not implemented");
}

void CC3Matrix::populateToPointTowards( CC3Vector fwdDirection, CC3Vector upDirection )
{
	implPopulateToPointTowards( fwdDirection, upDirection );
	m_isIdentity = false;
	m_isRigid = true;
}

/** Template method. Subclasses will provide implementation. */
void CC3Matrix::implPopulateToPointTowards( CC3Vector fwdDirection, CC3Vector upDirection )
{
	CCAssert(false, "the implPopulateToPointTowards:withUp: method is not implemented");
}

void CC3Matrix::populateToLookAt( CC3Vector targetLocation, CC3Vector eyeLocation, CC3Vector upDirection )
{
	CC3Vector fwdDir = CC3VectorDifference(targetLocation, eyeLocation);
	populateToPointTowards( fwdDir, upDirection );
	transpose();		
	translateBy( CC3VectorNegate(eyeLocation) );
	m_isIdentity = false;
	m_isRigid = true;
}

void CC3Matrix::populateFromFrustumLeft( GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat nearval, GLfloat farval )
{
	implPopulateFromFrustumLeft( left, right, top, bottom, nearval, farval );
	m_isIdentity = false;
	m_isRigid = false;
}

void CC3Matrix::implPopulateFromFrustumLeft( GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat nearval, GLfloat farval )
{
	CCAssert(false, "%CC3Matrix does not support perspective projection. Use the CC3ProjectionMatrix instead");
}

void CC3Matrix::populateFromFrustumLeft( GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat nearval )
{
	implPopulateFromFrustumLeft( left, right, top, bottom, nearval );
	m_isIdentity = false;
	m_isRigid = false;
}

void CC3Matrix::implPopulateFromFrustumLeft( GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat nearval ) 
{
	CCAssert(false, "%CC3Matrix does not support perspective projection. Use the CC3ProjectionMatrix instead");
}

void CC3Matrix::populateOrthoFromFrustumLeft( GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat nearval, GLfloat farval )
{
	implPopulateOrthoFromFrustumLeft( left, right, top, bottom, nearval, farval );
	m_isIdentity = false;
	m_isRigid = false;
}

void CC3Matrix::implPopulateOrthoFromFrustumLeft( GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat nearval, GLfloat farval )
{
	CCAssert(false, "CC3Matrix does not support orthographic projection. Use the CC3ProjectionMatrix instead");
}

void CC3Matrix::populateOrthoFromFrustumLeft(GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat nearval )
{
	implPopulateOrthoFromFrustumLeft( left, right, top, bottom, nearval );
	m_isIdentity = false;
	m_isRigid = false;
}

void CC3Matrix::implPopulateOrthoFromFrustumLeft(GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat nearval )
{
	CCAssert(false, "%CC3Matrix not support orthographic projection. Use the CC3ProjectionMatrix instead");
}

CC3Vector CC3Matrix::extractRotation()
{
	CCAssert(false, "the extractRotation method is not implemented");
	return kCC3VectorNull;
}

CC3Quaternion CC3Matrix::extractQuaternion()
{
	CCAssert(false, "the extractQuaternion method is not implemented");
	return kCC3QuaternionNull;
}

CC3Vector CC3Matrix::extractForwardDirection()
{
	CCAssert(false, "the extractForwardDirection method is not implemented");
	return kCC3VectorNull;
}

CC3Vector CC3Matrix::extractUpDirection()
{
	CCAssert(false, "the extractUpDirection method is not implemented");
	return kCC3VectorNull;
}

CC3Vector CC3Matrix::extractRightDirection() 
{
	CCAssert(false, "the extractRightDirection method is not implemented");
	return kCC3VectorNull;
}

CC3Vector CC3Matrix::extractTranslation() {
	CCAssert(false, "the extractTranslation method is not implemented");
	return kCC3VectorNull;
}

// Short-circuit the identity transform. isRigid unchanged under rotation.
void CC3Matrix::rotateBy( CC3Vector aRotation ) {
	if ( !CC3VectorsAreEqual(aRotation, kCC3VectorZero) ) {
		implRotateBy( aRotation );
		m_isIdentity = false;
	}
}

/** Template method. Subclasses will provide implementation. */
void CC3Matrix::implRotateBy( CC3Vector aRotation )
{
	CCAssert(false, "the implRotateBy: method is not implemented");
}

// Short-circuit the identity transform. isRigid unchanged under rotation.
void CC3Matrix::rotateByQuaternion( CC3Quaternion aQuaternion ){
	if ( !CC3QuaternionsAreEqual(aQuaternion, kCC3QuaternionIdentity) ) {
		implRotateByQuaternion( aQuaternion );
		m_isIdentity = false;
	}
}

/** Template method. Subclasses will provide implementation. */
void CC3Matrix::implRotateByQuaternion( CC3Quaternion aQuaternion )
{
	CCAssert(false, "the implRotateByQuaternion: method is not implemented");
}

/** Template method. Subclasses will provide implementation. */
void CC3Matrix::orthonormalizeRotationStartingWith( unsigned int startColNum )
{
	CCAssert(false, "the orthonormalizeRotationStartingWith: method is not implemented");
}

// Short-circuit the identity transform
void CC3Matrix::scaleBy( CC3Vector aScale )
{
	if ( !CC3VectorsAreEqual(aScale, kCC3VectorUnitCube) ) 
	{
		implScaleBy( aScale );
		m_isIdentity = false;
		m_isRigid = false;
	}
}

/** Template method. Subclasses will provide implementation. */
void CC3Matrix::implScaleBy( CC3Vector aScale )
{
	CCAssert(false, "the implScaleBy: method is not implemented");
}

// Short-circuit the identity transform. isRigid unchanged under translation.
void CC3Matrix::translateBy( CC3Vector aTranslation )
{
	if ( !CC3VectorsAreEqual(aTranslation, kCC3VectorZero) ) 
	{
		implTranslateBy( aTranslation );
		m_isIdentity = false;
	}
}

/** Template method. Subclasses will provide implementation. */
void CC3Matrix::implTranslateBy( CC3Vector aTranslation )
{
	CCAssert(false, "the implTranslateBy: method is not implemented");
}


// Includes short-circuits when one of the matrix is an identity matrix
void CC3Matrix::multiplyBy( CC3Matrix* aMatrix )
{
	// If other matrix is identity, this matrix doesn't change, so leave
	if ( !aMatrix || aMatrix->isIdentity() ) 
		return;
	
	// If this matrix is identity, it just becomes the other matrix
	if ( m_isIdentity ) 
	{
		populateFrom( aMatrix );
		return;
	}

	// Otherwise, go through with the multiplication
	implMultiplyBy( aMatrix );
	m_isIdentity = false;
	if ( !aMatrix->isRigid() ) 
		m_isRigid = false;
}

/**
 * Template method for multiplying this matrix by the other, once it is known that neither
 * this matrix nor the other are identity matrices.
 *
 * Subclass implementations will double-dispatch to the other matrix.
 * Subclass implementation does not need to set isIdentity or isRigid.
 */
void CC3Matrix::implMultiplyBy( CC3Matrix* aMatrix )
{
	CCAssert(false, "the implMultiplyBy: method is not implemented");
}

/** Template method. Subclasses will provide implementation. */
void CC3Matrix::multiplyIntoCC3Matrix3x3( CC3Matrix3x3* mtx )
{
	CCAssert(false, "the multiplyIntoCC3Matrix3x3: method is not implemented");
}

/** Template method. Subclasses will provide implementation. */
void CC3Matrix::multiplyByCC3Matrix3x3( CC3Matrix3x3* mtx )
{
	CCAssert(false, "the multiplyByCC3Matrix3x3: method is not implemented");
}

/** Template method. Subclasses will provide implementation. */
void CC3Matrix::multiplyIntoCC3Matrix4x3( CC3Matrix4x3* mtx )
{
	CCAssert(false, "the multiplyIntoCC3Matrix4x3: method is not implemented");
}

/** Template method. Subclasses will provide implementation. */
void CC3Matrix::multiplyByCC3Matrix4x3( CC3Matrix4x3* mtx )
{
	CCAssert(false, "the multiplyByCC3Matrix4x3: method is not implemented");
}

/** Template method. Subclasses will provide implementation. */
void CC3Matrix::multiplyIntoCC3Matrix4x4( CC3Matrix4x4* mtx )
{
	CCAssert(false, "the multiplyIntoCC3Matrix4x4: method is not implemented");
}

/** Template method. Subclasses will provide implementation. */
void CC3Matrix::multiplyByCC3Matrix4x4( CC3Matrix4x4* mtx )
{
	CCAssert(false, "the multiplyByCC3Matrix4x4: method is not implemented");
}

// Includes short-circuits when one of the matrix is an identity matrix
void CC3Matrix::leftMultiplyBy( CC3Matrix* aMatrix )
{
	// If other matrix is identity, this matrix doesn't change, so leave
	if ( !aMatrix || aMatrix->isIdentity() ) 
		return;
	
	// If this matrix is identity, it just becomes the other matrix
	if ( m_isIdentity )
	{
		populateFrom( aMatrix );
		return;
	}
	
	// Otherwise, go through with the multiplication
	implLeftMultiplyBy( aMatrix );
	m_isIdentity = false;
	if ( !aMatrix->isRigid() ) 
		m_isRigid = false;
}

/**
 * Template method for multiplying this matrix by the other, once it is known that neither
 * this matrix nor the other are identity matrices.
 *
 * Subclass implementations will double-dispatch to the other matrix.
 * Subclass implementation does not need to set isIdentity or isRigid.
 */
void CC3Matrix::implLeftMultiplyBy( CC3Matrix* aMatrix )
{
	CCAssert(false, "the implLeftMultiplyBy: method is not implemented");
}

/** Template method. Subclasses will provide implementation. */
void CC3Matrix::leftMultiplyIntoCC3Matrix3x3( CC3Matrix3x3* mtx )
{
	CCAssert(false, "the leftMultiplyIntoCC3Matrix3x3: method is not implemented");
}

/** Template method. Subclasses will provide implementation. */
void CC3Matrix::leftMultiplyByCC3Matrix3x3( CC3Matrix3x3* mtx )
{
	CCAssert(false, "the leftMultiplyByCC3Matrix3x3: method is not implemented");
}

/** Template method. Subclasses will provide implementation. */
void CC3Matrix::leftMultiplyIntoCC3Matrix4x3( CC3Matrix4x3* mtx )
{
	CCAssert(false, "%the leftMultiplyIntoCC3Matrix4x3: method is not implemented");
}

/** Template method. Subclasses will provide implementation. */
void CC3Matrix::leftMultiplyByCC3Matrix4x3( CC3Matrix4x3* mtx ) 
{
	CCAssert(false, "the leftMultiplyByCC3Matrix4x3: method is not implemented");
}

/** Template method. Subclasses will provide implementation. */
void CC3Matrix::leftMultiplyIntoCC3Matrix4x4( CC3Matrix4x4* mtx )
{
	CCAssert(false, "the leftMultiplyIntoCC3Matrix4x4: method is not implemented");
}

/** Template method. Subclasses will provide implementation. */
void CC3Matrix::leftMultiplyByCC3Matrix4x4( CC3Matrix4x4* mtx )
{
	CCAssert(false, "the leftMultiplyByCC3Matrix4x4: method is not implemented");
}

// Short-circuit if this is an identity matrix
CC3Vector CC3Matrix::transformLocation( CC3Vector aLocation )
{
	if (m_isIdentity) 
		return aLocation;

	CCAssert(false, "the transformLocation: method is not implemented");
	return kCC3VectorNull;
}

// Short-circuit if this is an identity matrix
CC3Vector CC3Matrix::transformDirection( CC3Vector aDirection )
{
	if (m_isIdentity) 
		return aDirection;

	CCAssert(false, "the transformDirection: method is not implemented");
	return kCC3VectorNull;
}

// Short-circuit if this is an identity matrix
CC3Vector4 CC3Matrix::transformHomogeneousVector( CC3Vector4 aVector )
{
	if (m_isIdentity) 
		return aVector;

	CCAssert(false, "the transformHomogeneousVector: method is not implemented");
	return kCC3Vector4Null;
}

// Short-circuit if this is an identity matrix
CC3Ray CC3Matrix::transformRay( CC3Ray aRay )
{
	if ( m_isIdentity ) 
		return aRay;

	CC3Ray rayOut;
	rayOut.startLocation = transformLocation( aRay.startLocation );
	rayOut.direction = transformDirection( aRay.direction );
	return rayOut;
}

// Short-circuit if this is an identity matrix
void CC3Matrix::transpose() 
{
	if (m_isIdentity) 
		return;

	CCAssert(false, "the transpose method is not implemented");
}

// Short-circuit if this is an identity or rigid matrix
bool CC3Matrix::invert() 
{
	if ( m_isIdentity ) 
		return true;

	if ( m_isRigid )
	{
		invertRigid();
		return true;
	}

	return invertAdjoint();
}

// Short-circuit if this is an identity or rigid matrix
bool CC3Matrix::invertAdjoint() 
{
	if ( m_isIdentity ) 
		return true;

	CCAssert(false, "the invertAdjoint method is not implemented");
	
	return false;
}

// Short-circuit if this is an identity matrix
void CC3Matrix::invertRigid() 
{
	if ( m_isIdentity ) 
		return;
	
	CCAssert(false, "the invertRigid method is not implemented");
}

NS_COCOS3D_END
