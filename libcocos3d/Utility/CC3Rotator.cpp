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
#include "CC3Rotator.h"
#include "../Matrices/CC3LinearMatrix.h"
#include "../Nodes/CC3Node.h"

NS_COCOS3D_BEGIN

CC3Rotator::CC3Rotator()
{
	init();
}

CC3Rotator::~CC3Rotator()
{

}

bool CC3Rotator::isMutable()
{ 
	return false; 
}

bool CC3Rotator::isDirectional()
{ 
	return false; 
}

bool CC3Rotator::isTargettable()
{ 
	return false; 
}

CC3Vector CC3Rotator::getRotation()
{ 
	return kCC3VectorZero; 
}

CC3Quaternion CC3Rotator::getQuaternion()
{ 
	return kCC3QuaternionIdentity; 
}

CC3Vector CC3Rotator::getRotationAxis() 
{ 
	return kCC3VectorNull;
}

GLfloat CC3Rotator::getRotationAngle()
{ 
	return 0.0f;
}

CC3Matrix* CC3Rotator::getRotationMatrix()
{ 
	return NULL; 
}

CC3Vector CC3Rotator::getTargetLocation()
{ 
	return kCC3VectorNull; 
}

CC3TargettingConstraint CC3Rotator::getTargettingConstraint()
{ 
	return kCC3TargettingConstraintGlobalUnconstrained; 
}

CC3Node* CC3Rotator::getTarget()
{ 
	return NULL; 
}

bool CC3Rotator::shouldTrackTarget()
{ 
	return false; 
}

bool CC3Rotator::shouldUpdateToTarget()
{ 
	return false; 
}

bool CC3Rotator::shouldAutotargetCamera()
{ 
	return false; 
}

bool CC3Rotator::shouldRotateToTargetLocation()
{ 
	return false; 
}

bool CC3Rotator::isTrackingForBumpMapping()
{ 
	return false; 
}

bool CC3Rotator::isTrackingTargetDirection()
{ 
	return false; 
}

bool CC3Rotator::clearIfTarget( CC3Node* aNode )
{ 
	return false; 
}

CC3Rotator* CC3Rotator::rotator()
{
	CC3Rotator* pRotator = new CC3Rotator;
	pRotator->init();
	pRotator->autorelease();
	return pRotator;
}

CCObject* CC3Rotator::copyWithZone( CCZone* zone )
{
	CC3Rotator* aCopy = new CC3Rotator;
	aCopy->init();
	aCopy->populateFrom( this );
	return aCopy;
}

void CC3Rotator::populateFrom( CC3Rotator* another )
{

}

std::string CC3Rotator::description()
{ 
	return stringWithFormat( (char*)"CC3Rotator" ); 
}

std::string CC3Rotator::fullDescription()
{ 
	return description(); 
}

CC3Vector CC3Rotator::transformDirection( CC3Vector aDirection )
{ 
	return aDirection; 
}

bool CC3Rotator::init()
{
	return true;
}

void CC3Rotator::applyRotationTo( CC3Matrix* matrix )
{

}

CC3MutableRotator::CC3MutableRotator()
{
	m_rotationMatrix = NULL;
}

CC3MutableRotator::~CC3MutableRotator()
{
	CC_SAFE_RELEASE( m_rotationMatrix );
}


CC3MutableRotator* CC3MutableRotator::rotator()
{
	CC3MutableRotator* pRotator = new CC3MutableRotator;
	pRotator->init();
	pRotator->autorelease();
	return pRotator;
}

bool CC3MutableRotator::isMutable()
{ 
	return true; 
}

bool CC3MutableRotator::isDirectional()
{ 
	return false; 
}

bool CC3MutableRotator::isRotationDirty()
{ 
	if ( m_rotationMatrix )
		return m_rotationMatrix->isDirty();

	return false;
}

void CC3MutableRotator::markRotationDirty()
{ 
	if ( m_rotationMatrix )
		m_rotationMatrix->setIsDirty( true ); 
}

void CC3MutableRotator::markRotationClean()
{ 
	if ( m_rotationMatrix )
		m_rotationMatrix->setIsDirty( false );
}

CC3Vector CC3MutableRotator::getRotation()
{
	return (m_rotationType == kCC3RotationTypeEuler)
				? m_rotationVector.v
				: getRotationMatrix()->extractRotation();
}

void CC3MutableRotator::setRotation( CC3Vector aRotation )
{
	m_rotationVector = CC3Vector4FromCC3Vector(CC3VectorRotationModulo(aRotation), 0.0f);
	m_rotationType = kCC3RotationTypeEuler;
	markRotationDirty();
}

void CC3MutableRotator::rotateBy( CC3Vector aRotation )
{
	getRotationMatrix()->rotateBy( CC3VectorRotationModulo(aRotation) );
	autoOrthonormalize();
	m_rotationType = kCC3RotationTypeUnknown;
	markRotationClean();
}

CC3Quaternion CC3MutableRotator::getQuaternion()
{
	switch (m_rotationType) 
	{
		case kCC3RotationTypeQuaternion:
			return m_rotationVector;
		case kCC3RotationTypeAxisAngle:
			return CC3QuaternionFromAxisAngle(getRotationAxisAngle());
		default:
			return getRotationMatrix()->extractQuaternion();
	}
}

void CC3MutableRotator::setQuaternion( CC3Quaternion aQuaternion )
{
	m_rotationVector = aQuaternion;
	m_rotationType = kCC3RotationTypeQuaternion;
	markRotationDirty();
}

void CC3MutableRotator::rotateByQuaternion( CC3Quaternion aQuaternion )
{
	getRotationMatrix()->rotateByQuaternion( aQuaternion );
	autoOrthonormalize();
	m_rotationType = kCC3RotationTypeUnknown;
	markRotationClean();
}

CC3Vector4 CC3MutableRotator::getRotationAxisAngle()
{
	return (m_rotationType == kCC3RotationTypeAxisAngle)
				? m_rotationVector
				: CC3AxisAngleFromQuaternion(getQuaternion());
}

CC3Vector CC3MutableRotator::getRotationAxis()
{ 
	return getRotationAxisAngle().v; 
}

void CC3MutableRotator::setRotationAxis( CC3Vector aDirection )
{
	m_rotationVector = CC3Vector4FromCC3Vector(aDirection, getRotationAngle());
	m_rotationType = kCC3RotationTypeAxisAngle;
	markRotationDirty();
}

GLfloat CC3MutableRotator::getRotationAngle() 
{ 
	return getRotationAxisAngle().w; 
}

void CC3MutableRotator::setRotationAngle( GLfloat anAngle )
{
	m_rotationVector = CC3Vector4FromCC3Vector(getRotationAxis(), CC3CyclicAngle(anAngle));
	m_rotationType = kCC3RotationTypeAxisAngle;
	markRotationDirty();
}

void CC3MutableRotator::rotateByAngle( GLfloat anAngle, CC3Vector anAxis )
{
	rotateByQuaternion( CC3QuaternionFromAxisAngle(CC3Vector4FromCC3Vector(anAxis, anAngle)) );
}

CC3Matrix* CC3MutableRotator::getRotationMatrix()
{
	applyRotation();
	return m_rotationMatrix;
}

void CC3MutableRotator::setRotationMatrix( CC3Matrix* aMatrix )
{
	if (aMatrix == m_rotationMatrix) return;
	
	CC_SAFE_RELEASE(m_rotationMatrix);
	m_rotationMatrix = aMatrix;
	CC_SAFE_RETAIN(aMatrix);
	
	m_rotationType = kCC3RotationTypeUnknown;
	markRotationClean();
}

// Orthonormalize the matrix using the current starting basis vector.
// Then cycle the starting vector, so that the next invocation starts with a different column.
void CC3MutableRotator::orthonormalize()
{
	getRotationMatrix()->orthonormalizeRotationStartingWith( m_orthonormalizationStartColumnNumber );

	m_orthonormalizationStartColumnNumber = (m_orthonormalizationStartColumnNumber < 3)
												? (m_orthonormalizationStartColumnNumber + 1) : 1;
}

static GLubyte _autoOrthonormalizeCount = 0;

GLubyte CC3MutableRotator::getAutoOrthonormalizeCount() { return _autoOrthonormalizeCount; }

void CC3MutableRotator::setAutoOrthonormalizeCount( GLubyte aCount )
{
	_autoOrthonormalizeCount = aCount; 
}

void CC3MutableRotator::autoOrthonormalize()
{
	if (_autoOrthonormalizeCount) {
		m_incrementalRotationCount++;
		if (m_incrementalRotationCount >= _autoOrthonormalizeCount) {
			orthonormalize();
			m_incrementalRotationCount = 0;
		}
	}
}

bool CC3MutableRotator::init()
{ 
	initOnRotationMatrix( CC3LinearMatrix::matrix() );
	return true;
}

void CC3MutableRotator::initOnRotationMatrix( CC3Matrix* aMatrix )
{
	if ( super::init() ) 
	{
		m_orthonormalizationStartColumnNumber = 1;
		m_incrementalRotationCount = 0;
		m_rotationVector = kCC3Vector4Zero;
		setRotationMatrix( aMatrix );		// also sets rotation type
	}
}
	
CC3MutableRotator* CC3MutableRotator::rotatorOnRotationMatrix( CC3Matrix* aMatrix )
{
	CC3MutableRotator* rotator = new CC3MutableRotator;
	rotator->initOnRotationMatrix( aMatrix ); 
	rotator->autorelease();
	
	return rotator;
}

// Protected properties for copying
CC3Vector4 CC3MutableRotator::getRotationVector()
{ 
	return m_rotationVector;
}

GLubyte CC3MutableRotator::getRotationType()
{ 
	return m_rotationType; 
}

GLubyte CC3MutableRotator::getOrthonormalizationStartColumnNumber()
{ 
	return m_orthonormalizationStartColumnNumber;
}

GLubyte CC3MutableRotator::getIncrementalRotationCount()
{ 
	return m_incrementalRotationCount;
}

void CC3MutableRotator::populateFrom( CC3MutableRotator* another )
{
	super::populateFrom( another );
	
	// Only populate the following if the other instance is also a mutable rotator.
	{
		m_rotationVector = another->getRotationVector();
		m_rotationType = another->getRotationType();
		m_orthonormalizationStartColumnNumber = another->getOrthonormalizationStartColumnNumber();
		m_incrementalRotationCount = another->getIncrementalRotationCount();
	}

	markRotationDirty();
}

CCObject* CC3MutableRotator::copyWithZone(CCZone* pZone)
{
	CC3MutableRotator* pVal = new CC3MutableRotator;
	pVal->init();
	pVal->populateFrom( this );

	return pVal;
}

std::string CC3MutableRotator::fullDescription()
{
	return stringWithFormat( (char*)"%s with rotation: %s, quaternion: %s, rotation axis: %s, rotation angle %.3f",
			super::fullDescription().c_str(),
			stringFromCC3Vector(getRotation()).c_str(),
			stringFromCC3Vector4(getQuaternion()).c_str(),
			stringFromCC3Vector(getRotationAxis()).c_str(),
			getRotationAngle() );
}


/** Recalculates the rotation matrix from the most recently set rotation property. */
void  CC3MutableRotator::applyRotation()
{
	if ( !isRotationDirty() ) 
		return;

	switch (m_rotationType) 
	{
		case kCC3RotationTypeEuler:
			m_rotationMatrix->populateFromRotation( getRotation() );
			break;
		case kCC3RotationTypeQuaternion:
		case kCC3RotationTypeAxisAngle:
			m_rotationMatrix->populateFromQuaternion( getQuaternion() );
			break;
		default:
			break;
	}
	markRotationClean();
}

// Rotation matrix is built lazily if needed
void CC3MutableRotator::applyRotationTo( CC3Matrix* aMatrix )
{ 
	aMatrix->multiplyBy( getRotationMatrix() );
}

CC3Vector CC3MutableRotator::transformDirection( CC3Vector aDirection )
{
	return getRotationMatrix()->transformDirection( aDirection );
}

CC3DirectionalRotator* CC3DirectionalRotator::rotator()
{
	CC3DirectionalRotator* pRotator = new CC3DirectionalRotator;
	pRotator->init();
	pRotator->autorelease();
	return pRotator;
}

bool CC3DirectionalRotator::isDirectional()
{
	return true; 
}

bool CC3DirectionalRotator::shouldReverseForwardDirection()
{
	return m_shouldReverseForwardDirection; 
}

void CC3DirectionalRotator::setShouldReverseForwardDirection( bool shouldReverse )
{
	m_shouldReverseForwardDirection = shouldReverse; 
}

CC3Vector CC3DirectionalRotator::getForwardDirection()
{
	if (m_rotationType == kCC3RotationTypeDirection) 
	{
		return m_rotationVector.v;
	} 
	else 
	{
		CC3Vector mtxFwdDir = getRotationMatrix()->extractForwardDirection();
		return m_shouldReverseForwardDirection ? CC3VectorNegate(mtxFwdDir) : mtxFwdDir;
	}
}

void CC3DirectionalRotator::setForwardDirection( CC3Vector aDirection )
{
	CCAssert(!CC3VectorsAreEqual(aDirection, kCC3VectorZero),
			 "The forwardDirection may not be set to the zero vector.");
	m_rotationVector = CC3Vector4FromDirection(CC3VectorNormalize(aDirection));
	m_rotationType = kCC3RotationTypeDirection;
	markRotationDirty();
}

CC3Vector CC3DirectionalRotator::getReferenceUpDirection()
{ 
	return m_referenceUpDirection; 
}

/** Does not set the rotation type until the forwardDirection is set. */
void CC3DirectionalRotator::setReferenceUpDirection( CC3Vector aDirection )
{
	CCAssert(!CC3VectorsAreEqual(aDirection, kCC3VectorZero),
			 "The referenceUpDirection may not be set to the zero vector.");

	m_referenceUpDirection = CC3VectorNormalize(aDirection);
}


CC3Vector CC3DirectionalRotator::getUpDirection() 
{ 
	return getRotationMatrix()->extractUpDirection();
}

CC3Vector CC3DirectionalRotator::getRightDirection()
{ 
	return getRotationMatrix()->extractRightDirection(); 
}


void CC3DirectionalRotator::initOnRotationMatrix( CC3Matrix* aMatrix )
{
	super::initOnRotationMatrix( aMatrix );
	m_referenceUpDirection = kCC3VectorUnitYPositive;
	m_shouldReverseForwardDirection = false;	
}

void CC3DirectionalRotator::populateFrom( CC3MutableRotator* another )
{
	super::populateFrom(another);
}

void CC3DirectionalRotator::populateFrom( CC3DirectionalRotator* another )
{
	super::populateFrom( another );

	// Only populate the following if the other instance is also a directional rotator.
	{
		m_referenceUpDirection = another->getReferenceUpDirection();
		m_shouldReverseForwardDirection = another->shouldReverseForwardDirection();
	}
}

CCObject* CC3DirectionalRotator::copyWithZone(CCZone* pZone)
{
	CC3DirectionalRotator* pVal = new CC3DirectionalRotator;
	pVal->init();
	pVal->populateFrom( this );

	return pVal;
}

// If rotation is defined by the forward direction, apply it to the matrix, taking into
// consideration whether the foward direction should be inverted. Otherwise, invoke superclass
// implementation to handle other types of rotation.
void CC3DirectionalRotator::applyRotation()
{
	if ( !isRotationDirty() ) 
		return;
	
	if (m_rotationType == kCC3RotationTypeDirection) 
	{
		CCAssert( !CC3VectorsAreParallel(getForwardDirection(), getReferenceUpDirection()),
				  "The forwardDirection cannot be parallel to the referenceUpDirection."
				  " To use this forwardDirection, you must choose a different referenceUpDirection." );

		CC3Vector mtxFwdDir = m_shouldReverseForwardDirection
									? CC3VectorNegate(getForwardDirection())
									: getForwardDirection();
		m_rotationMatrix->populateToPointTowards( mtxFwdDir, getReferenceUpDirection() );
		markRotationClean();
	} 
	else 
	{
		super::applyRotation();
	}
}

std::string CC3DirectionalRotator::fullDescription()
{
	return stringWithFormat( (char*)"%s, direction: %s, up: %s, scene up: %s",
			super::fullDescription().c_str(),
			stringFromCC3Vector(getForwardDirection()).c_str(),
			stringFromCC3Vector(getUpDirection()).c_str(),
			stringFromCC3Vector(getReferenceUpDirection()).c_str() );
}

CC3TargettingRotator::CC3TargettingRotator()
{
	m_pTarget = NULL;
}

CC3TargettingRotator::~CC3TargettingRotator()
{
	m_pTarget = NULL;
}

CC3TargettingRotator* CC3TargettingRotator::rotator()
{
	CC3TargettingRotator* pRotator = new CC3TargettingRotator;
	pRotator->init();
	pRotator->autorelease();
	return pRotator;
}

bool CC3TargettingRotator::isTrackingForBumpMapping()
{ 
	return m_isTrackingForBumpMapping; 
}

void CC3TargettingRotator::setIsTrackingForBumpMapping( bool  isBM )
{
	m_isTrackingForBumpMapping = isBM; 
}

bool CC3TargettingRotator::shouldTrackTarget()
{
	return m_shouldTrackTarget; 
}

void CC3TargettingRotator::setShouldTrackTarget( bool shouldTrack )
{ 
	m_shouldTrackTarget = shouldTrack; 
}

bool CC3TargettingRotator::shouldAutotargetCamera()
{
	return m_shouldAutotargetCamera; 
}

void CC3TargettingRotator::setShouldAutotargetCamera( bool shouldAutotarget )
{ 
	m_shouldAutotargetCamera = shouldAutotarget; 
}

CC3TargettingConstraint CC3TargettingRotator::getTargettingConstraint()
{
	return (CC3TargettingConstraint)m_targettingConstraint; 
}

void  CC3TargettingRotator::setTargettingConstraint( CC3TargettingConstraint targContraint )
{ 
	m_targettingConstraint = targContraint; 
}

bool CC3TargettingRotator::isTargettable()
{
	return true; 
}

CC3Vector CC3TargettingRotator::getTargetLocation()
{
	return (m_rotationType == kCC3RotationTypeLocation) ? m_rotationVector.v : kCC3VectorNull;
}

void CC3TargettingRotator::setTargetLocation( CC3Vector aLocation )
{
	m_rotationVector = CC3Vector4FromCC3Vector(aLocation, 0.0f);
	m_rotationType = kCC3RotationTypeLocation;
	m_isNewTarget = false;		// Target is no longer new once the location of it has been set.
	markRotationDirty();
}

bool CC3TargettingRotator::isDirtyByTargetLocation()
{ 
	return isRotationDirty() && (m_rotationType == kCC3RotationTypeLocation); 
}

void CC3TargettingRotator::rotateToTargetLocation( CC3Vector targLoc, CC3Vector eyeLoc, CC3Vector upDir )
{
	if ( !CC3VectorsAreEqual(targLoc, eyeLoc) ) 
	{
		CC3Vector mtxDir = m_shouldReverseForwardDirection
								? CC3VectorDifference(eyeLoc, targLoc)
								: CC3VectorDifference(targLoc, eyeLoc);

		m_rotationMatrix->populateToPointTowards( mtxDir, upDir );
		
		markRotationClean();
	}
}

CC3Node* CC3TargettingRotator::getTarget()
{
	return m_pTarget; 
}

/**
 * Set the new target as weak reference and mark whether it has changed.
 * Don't mark if not changed, so that a change persists even if the same target is set again.
 */
void CC3TargettingRotator::setTarget( CC3Node* aNode )
{
	if (aNode != m_pTarget) 
		m_isNewTarget = true;

	m_pTarget = aNode;		// weak reference
}

bool CC3TargettingRotator::shouldUpdateToTarget()
{ 
	return m_pTarget && (m_isNewTarget || m_shouldTrackTarget);
}

bool CC3TargettingRotator::shouldRotateToTargetLocation()
{
	return (isDirtyByTargetLocation() || m_shouldTrackTarget) && !isTrackingForBumpMapping();
}

bool CC3TargettingRotator::isTrackingTargetDirection()
{
	return m_shouldTrackTarget && !m_isTrackingForBumpMapping && (m_pTarget != NULL);
}

bool CC3TargettingRotator::clearIfTarget( CC3Node* aNode )
{
	if (aNode != m_pTarget) 
		return false;

	m_pTarget = NULL;		// weak reference
	return true;
}


void CC3TargettingRotator::initOnRotationMatrix( CC3Matrix* aMatrix )
{
	super::initOnRotationMatrix( aMatrix );

	m_pTarget = NULL;
	m_targettingConstraint = kCC3TargettingConstraintGlobalUnconstrained;
	m_isNewTarget = false;
	m_shouldTrackTarget = false;
	m_shouldAutotargetCamera = false;
	m_isTrackingForBumpMapping = false;
}

// Protected properties used for copying
bool CC3TargettingRotator::isNewTarget()
{ 
	return m_isNewTarget; 
}

void CC3TargettingRotator::populateFrom( CC3MutableRotator* another )
{
	super::populateFrom(another);
}

void  CC3TargettingRotator::populateFrom( CC3TargettingRotator* another )
{
	super::populateFrom( another );

	setTarget( another->getTarget() );		// weak reference...not copied
	m_targettingConstraint = another->getTargettingConstraint();
	m_shouldTrackTarget = another->shouldTrackTarget();
	m_shouldAutotargetCamera = another->shouldAutotargetCamera();
	m_isTrackingForBumpMapping = another->isTrackingForBumpMapping();
}

CCObject* CC3TargettingRotator::copyWithZone(CCZone* pZone)
{
	CC3TargettingRotator* pVal = new CC3TargettingRotator;
	pVal->init();
	pVal->populateFrom( this );

	return pVal;
}

std::string  CC3TargettingRotator::fullDescription()
{
	return stringWithFormat( (char*)"%s, targetted at: %s, from target: %s",
			super::fullDescription().c_str(), stringFromCC3Vector(getTargetLocation()).c_str(),  m_pTarget->getName().c_str() );
}


NS_COCOS3D_END
