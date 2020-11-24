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

CC3Node::CC3Node()
{
	m_pParent = NULL;
	m_pChildren = NULL;
	m_localTransformMatrix = NULL;
	m_globalTransformMatrix = NULL;
	m_globalTransformMatrixInverted = NULL;
	m_globalRotationMatrix = NULL;
	m_pBoundingVolume = NULL;
	m_pTransformListeners = NULL;
	m_rotator = NULL;
	m_pAnimationStates = NULL;

	m_scale = cc3v( 1.f, 1.f, 1.f );
	m_location = CC3Vector::kCC3VectorZero;
	m_projectedLocation = CC3Vector::kCC3VectorZero;
	
	m_fBoundingVolumePadding = 0.f;
	m_fCameraDistanceProduct = 0.f;

	m_touchEnabled = false;
	m_shouldInheritTouchability = true;
	m_shouldAllowTouchableWhenInvisible = false;
	m_visible = true;
	m_isRunning = false;
	m_shouldAutoremoveWhenEmpty = false;
	m_shouldUseFixedBoundingVolume = false;
	m_isAnimationDirty = true;
	m_cascadeColorEnabled = true;
	m_cascadeOpacityEnabled = true;
	m_isBeingAdded = true;
	m_shouldCastShadows = false;

	m_globalLocation = CC3Vector::kCC3VectorZero;
	m_globalRotation = CC3Vector::kCC3VectorZero;
	m_globalScale = cc3v(1.f, 1.f, 1.f);
	m_isTransformDirty = true;
	m_isTransformRigid = false;
	m_pActionManager = NULL;
}

CC3Node::~CC3Node()
{
	setTarget( NULL );
	removeAllChildren();
	notifyDestructionListeners();

	m_pParent = NULL;

	CC_SAFE_RELEASE( m_localTransformMatrix );
	CC_SAFE_RELEASE( m_globalTransformMatrix );
	CC_SAFE_RELEASE( m_globalTransformMatrixInverted );
	CC_SAFE_RELEASE( m_globalRotationMatrix );
	CC_SAFE_RELEASE( m_rotator );
	CC_SAFE_RELEASE( m_pBoundingVolume );
	CC_SAFE_RELEASE( m_pAnimationStates );
	CC_SAFE_RELEASE( m_pTransformListeners );
}

void CC3Node::setLocation( const CC3Vector& location )
{
	m_location = location;
	markTransformDirty();
}

CC3Vector CC3Node::getLocation()
{
	return m_location;
}

CC3Vector CC3Node::getGlobalLocation()
{
	if ( !m_pParent )
		return CC3Vector::kCC3VectorZero;
	
	if ( m_pParent )
	{
		CC3Matrix* pGlobalTransformMatrix = m_pParent->getGlobalTransformMatrix();
		if ( pGlobalTransformMatrix )
			return pGlobalTransformMatrix->transformLocation( m_location );
	}

	return CC3Vector::kCC3VectorZero;
}

CC3Vector4 CC3Node::getGlobalHomogeneousPosition() 
{ 
	return CC3Vector4().fromLocation( getGlobalLocation() ); 
}

void CC3Node::translateBy( const CC3Vector& aVector )
{ 
	m_location = m_location.add( aVector ); 
}

void CC3Node::setRotator( CC3Rotator* rotator )
{
	CC_SAFE_RELEASE(m_rotator);
	CC_SAFE_RETAIN(rotator);
	m_rotator = rotator;
}

CC3Rotator* CC3Node::getRotator()
{
	return m_rotator;
}

CC3Vector CC3Node::getRotation()
{ 
	if ( m_rotator )
		return m_rotator->getRotation(); 

	return CC3Vector::kCC3VectorZero;
}

void CC3Node::setRotation( const CC3Vector& aRotation )
{
	if ( isTrackingTargetDirection() || aRotation.equals( getRotation() ) ) 
		return;

	getMutableRotator()->setRotation( aRotation );
	markTransformDirty();
}

CC3Vector CC3Node::getGlobalRotation() 
{ 
	return getGlobalRotationMatrix()->extractRotation(); 
}

void CC3Node::rotateBy( const CC3Vector& aRotation )
{
	if ( isTrackingTargetDirection() ) 
		return;

	getMutableRotator()->rotateBy( aRotation );
	markTransformDirty();
}

CC3Quaternion CC3Node::getQuaternion()
{ 
	if ( m_rotator )
		return m_rotator->getQuaternion(); 

	return CC3Quaternion::kCC3QuaternionNull;
}

void CC3Node::setQuaternion( const CC3Quaternion& aQuaternion )
{
	if ( isTrackingTargetDirection() || aQuaternion.equals(getQuaternion()) ) 
		return;

	getMutableRotator()->setQuaternion( aQuaternion );
	markTransformDirty();
}

void CC3Node::rotateByQuaternion( const CC3Quaternion& aQuaternion )
{
	if ( isTrackingTargetDirection() ) 
		return;

	getMutableRotator()->rotateByQuaternion( aQuaternion );
	markTransformDirty();
}

CC3Vector CC3Node::getRotationAxis() 
{ 
	return m_rotator->getRotationAxis(); 
}

void CC3Node::setRotationAxis( const CC3Vector& aDirection )
{
	if ( isTrackingTargetDirection() || aDirection.equals( getRotationAxis() ) ) 
		return;

	getMutableRotator()->setRotationAxis( aDirection );
	markTransformDirty();
}

GLfloat CC3Node::getRotationAngle()
{ 
	return m_rotator->getRotationAngle(); 
}

void CC3Node::setRotationAngle( GLfloat anAngle )
{
	if ( isTrackingTargetDirection() || (anAngle == getRotationAngle()) ) 
		return;

	getMutableRotator()->setRotationAngle( anAngle );
	markTransformDirty();
}

void CC3Node::rotateByAngle( GLfloat angle, const CC3Vector& axis )
{
	if ( isTrackingTargetDirection() )
		return;

	getMutableRotator()->rotateByAngle( angle, axis );
	markTransformDirty();
}

/**
 * Rotate this node, then determine the amount the pivot location has been translated by the
 * rotation action, and translate this node back to compensate. This moves the pivot location
 * back to where it was before the rotation action, effectively making the rotation action
 * occur around that pivot location.
 */
void CC3Node::rotateByAngle( GLfloat angle, CC3Vector axis, const CC3Vector& pivotLocation )
{
	CC3Vector pivotBefore = getLocalTransformMatrix()->transformLocation( pivotLocation );
	rotateByAngle( angle, axis );
	CC3Vector pivotAfter = getLocalTransformMatrix()->transformLocation( pivotLocation );
	translateBy( pivotBefore.difference( pivotAfter ) );
}

CC3Vector CC3Node::getForwardDirection()
{ 
	return getDirectionalRotator()->getForwardDirection(); 
}

void CC3Node::setForwardDirection( const CC3Vector& aDirection )
{
	if ( isTrackingTargetDirection() ) 
		return;

	getDirectionalRotator()->setForwardDirection( aDirection );
	markTransformDirty();
}

CC3Vector CC3Node::getGlobalForwardDirection() 
{ 
	return getGlobalRotationMatrix()->extractForwardDirection(); 
}

CC3Vector CC3Node::getUpDirection()
{ 
	return getDirectionalRotator()->getUpDirection(); 
}

CC3Vector CC3Node::getGlobalUpDirection() 
{ 
	return getGlobalRotationMatrix()->extractUpDirection();
}

CC3Vector CC3Node::getReferenceUpDirection()
{
	return getDirectionalRotator()->getReferenceUpDirection(); 
}

void CC3Node::setReferenceUpDirection( const CC3Vector& aDirection )
{
	getDirectionalRotator()->setReferenceUpDirection( aDirection );
	markTransformDirty();
}

CC3Vector CC3Node::getRightDirection()
{ 
	return getDirectionalRotator()->getRightDirection();
}

CC3Vector CC3Node::getGlobalRightDirection() 
{ 
	return getGlobalRotationMatrix()->extractRightDirection(); 
}

void CC3Node::setScale( const CC3Vector& aScale )
{
	m_scale = aScale;
	markTransformDirty();
}

CC3Vector CC3Node::getScale()
{
	return m_scale;
}

GLfloat CC3Node::getUniformScale()
{
	return (isUniformlyScaledLocally())
					? m_scale.x 
					: m_scale.length() / CC3Vector::kCC3VectorUnitCubeLength;
}

void CC3Node::setUniformScale( GLfloat aValue )
{
	setScale( cc3v(aValue, aValue, aValue) ); 
}

bool CC3Node::isUniformlyScaledLocally()
{ 
	return (m_scale.x == m_scale.y) && (m_scale.x == m_scale.z); 
}

CC3Vector CC3Node::getGlobalScale() 
{ 
	return m_pParent ? m_pParent->getGlobalScale().scale( m_scale ) : m_scale; 
}

bool CC3Node::isUniformlyScaledGlobally() 
{
	return isUniformlyScaledLocally() && (m_pParent ? m_pParent->isUniformlyScaledGlobally() : true);
}

bool CC3Node::isTransformRigid() 
{ 
	return getGlobalTransformMatrix()->isRigid();
}

CC3Vector CC3Node::getProjectedLocation() 
{ 
	return m_projectedLocation; 
}

// Protected setter
void CC3Node::setProjectedLocation( const CC3Vector& projectedLocation )
{ 
	m_projectedLocation = projectedLocation; 
}

// Derived from projected location, but only if in front of the camera
CCPoint CC3Node::getProjectedPosition() 
{
	CC3Vector projLoc = m_projectedLocation;
	return (projLoc.z > 0.0)
				? ccp(projLoc.x, projLoc.y)
				: ccp(-kCC3MaxGLfloat, -kCC3MaxGLfloat);
}

void CC3Node::setIsRunning( bool shouldRun )
{
	if ( !m_isRunning && shouldRun ) 
		resumeAllActions();
	if ( m_isRunning && !shouldRun )
		pauseAllActions();

	m_isRunning = shouldRun;
	
	CCObject* child;
	CCARRAY_FOREACH(m_pChildren, child)
	{
		CC3Node* pChild = (CC3Node*) child;
		if (pChild)
		{
			pChild->setIsRunning( shouldRun );
		}
	}
}

bool CC3Node::isRunning()
{
	return m_isRunning;
}

CC3Vector CC3Node::getTargetLocation()
{
	CC3Vector targLoc = m_rotator->getTargetLocation();
	return targLoc.isNull() ? getGlobalLocation().add( getForwardDirection() ) : targLoc;
}

/** Apply any rotational axis restrictions to the target location before setting it. */
void CC3Node::setTargetLocation( const CC3Vector& aLocation )
{
	getTargettingRotator()->setTargetLocation( aLocation );
	markTransformDirty();
}

CC3Node* CC3Node::getTarget() 
{ 
	if ( m_rotator )
		return m_rotator->getTarget(); 

	return NULL;
}

/** Set the new target and notify that I am now tracking a target. */
void CC3Node::setTarget( CC3Node* aNode )
{
	if ( aNode == getTarget() ) 
		return;

	if ( getTarget() )
		getTarget()->removeTransformListener( this );

	getTargettingRotator()->setTarget( aNode );

	if ( getTarget() )
		getTarget()->addTransformListener( this );
}

bool CC3Node::hasTarget()
{
	return (getTarget() != NULL); 
}

bool CC3Node::shouldTrackTarget() 
{ 
	return m_rotator->shouldTrackTarget(); 
}

/**
 * Check if the property was off and is now being turned on. In the case where this property was
 * temporarily turned off to perform some movement or rotation, we must force an update of the
 * target location when this property is turned back on.
 */
void CC3Node::setShouldTrackTarget( bool shouldTrack )
{
	bool wasAlreadyTracking = shouldTrackTarget();
	getTargettingRotator()->setShouldTrackTarget( shouldTrack );
	if ( shouldTrack && !wasAlreadyTracking) 
		markTransformDirty();
}

bool CC3Node::shouldAutotargetCamera()
{ 
	if ( m_rotator )
		return m_rotator->shouldAutotargetCamera(); 

	return false;
}

void CC3Node::setShouldAutotargetCamera( bool shouldAutotarg )
{
	getTargettingRotator()->setShouldAutotargetCamera( shouldAutotarg );
	setShouldTrackTarget( shouldAutotarg );
}

CC3TargettingConstraint CC3Node::getTargettingConstraint()
{ 
	return m_rotator->getTargettingConstraint(); 
}

void CC3Node::setTargettingConstraint( CC3TargettingConstraint targContraint )
{
	getTargettingRotator()->setTargettingConstraint( targContraint );
}

bool CC3Node::isTrackingForBumpMapping() 
{ 
	return m_rotator->isTrackingForBumpMapping();
}

void CC3Node::setIsTrackingForBumpMapping( bool isBumpMapping )
{
	getTargettingRotator()->setIsTrackingForBumpMapping( isBumpMapping );
}

bool CC3Node::isTrackingTargetDirection() 
{
	if ( m_rotator )
		return m_rotator->isTrackingTargetDirection();

	return false;
}

/**
 * Checks if the camera should be a target, and if so,
 * ensures that the target is the currently active camera.
 */
void CC3Node::checkCameraTarget()
{
	if ( shouldAutotargetCamera() )
	{
		CC3Camera* cam = getActiveCamera();
		if ( cam && (getTarget() != (CC3Node*)cam) ) 
		{
			setTarget( cam ); 
			getTargettingRotator()->setShouldAutotargetCamera( false );
		}
	}
}


/**
 * Returns the rotator property, cast as a CC3MutableRotator.
 *
 * If the rotator is not already a CC3MutableRotator, a new CC3MutableRotator
 * is created and its state is copied from the current rotator.
 *
 * This design allows nodes that do not require rotation to use the empty and smaller
 * CC3Rotator instance, but allows an automatic upgrade to a mutable rotator
 * when the node needs to make changes to the rotational properties.
 *
 * This property should only be accessed if the intention is to swap the existing
 * rotator with a directional rotator.
 */
CC3MutableRotator* CC3Node::getMutableRotator() 
{
	if ( !m_rotator || !m_rotator->isMutable() ) 
	{
		CC3MutableRotator* mRotator = CC3MutableRotator::rotator();
		if ( m_rotator->isMutable() )
			mRotator->populateFrom( (CC3MutableRotator*)m_rotator );

		CC3_TRACE( "CC3Node swapping CC3MutableRotator for existing rotator" );
		setRotator( mRotator );
	}

	return (CC3MutableRotator*)m_rotator;
}

CC3DirectionalRotator* CC3Node::getDirectionalRotator() 
{
	if ( !m_rotator || !m_rotator->isDirectional() ) 
	{
		CC3DirectionalRotator* dRotator = CC3DirectionalRotator::rotator();
		if ( m_rotator->isMutable() )
			dRotator->populateFrom( (CC3MutableRotator*)m_rotator );

		dRotator->setShouldReverseForwardDirection( shouldReverseForwardDirection() );
		CC3_TRACE( "CC3Node swapping CC3DirectionalRotator for existing rotator" );
		setRotator( dRotator );
	}

	return (CC3DirectionalRotator*)m_rotator;
}

CC3TargettingRotator*  CC3Node::getTargettingRotator()
{
	if ( !m_rotator || !m_rotator->isTargettable() )
	{
		CC3TargettingRotator* tRotator = CC3TargettingRotator::rotator();
		if ( m_rotator->isMutable() )
			tRotator->populateFrom( (CC3MutableRotator*)m_rotator );

		tRotator->setShouldReverseForwardDirection( shouldReverseForwardDirection() );
		CC3_TRACE( "CC3Node swapping CC3TargettingRotator for existing rotator" );
		setRotator( tRotator );
	}
	return (CC3TargettingRotator*)m_rotator;
}

bool CC3Node::shouldReverseForwardDirection()
{ 
	return true; 
}


bool CC3Node::shouldDrawInClipSpace() 
{
	CCObject* child;
	CCARRAY_FOREACH( m_pChildren, child )
	{
		CC3Node* pChild = (CC3Node*)child;
		if ( pChild && pChild->shouldDrawInClipSpace() )
			return true;
	}

	return false;
}

void CC3Node::setShouldDrawInClipSpace( bool shouldClip )
{
	CCObject* child;
	CCARRAY_FOREACH( m_pChildren, child )
	{
		CC3Node* pChild = (CC3Node*)child;
		if ( pChild )
			pChild->setShouldDrawInClipSpace( shouldClip );
	}
}

bool CC3Node::shouldCullBackFaces() 
{
	CCObject* child;
	CCARRAY_FOREACH( m_pChildren, child )
	{
		CC3Node* pChild = (CC3Node*)child;
		if ( pChild && pChild->shouldCullBackFaces() == false )
			return false;
	}

	return true;
}

void CC3Node::setShouldCullBackFaces( bool shouldCull )
{
	CCObject* child;
	CCARRAY_FOREACH( m_pChildren, child )
	{
		CC3Node* pChild = (CC3Node*)child;
		if ( pChild )
			pChild->setShouldCullBackFaces( shouldCull );
	}
}

bool CC3Node::shouldCullFrontFaces()
{
	CCObject* child;
	CCARRAY_FOREACH( m_pChildren, child )
	{
		CC3Node* pChild = (CC3Node*)child;
		if ( pChild && pChild->shouldCullFrontFaces() )
			return true;
	}

	return false;
}

void CC3Node::setShouldCullFrontFaces( bool shouldCull )
{
	CCObject* child;
	CCARRAY_FOREACH( m_pChildren, child )
	{
		CC3Node* pChild = (CC3Node*)child;
		if ( pChild )
			pChild->setShouldCullFrontFaces( shouldCull );
	}
}

bool CC3Node::shouldUseClockwiseFrontFaceWinding() 
{
	CCObject* child;
	CCARRAY_FOREACH( m_pChildren, child )
	{
		CC3Node* pChild = (CC3Node*)child;
		if ( pChild && pChild->shouldUseClockwiseFrontFaceWinding() )
			return true;
	}

	return false;
}

void CC3Node::setShouldUseClockwiseFrontFaceWinding( bool shouldWindCW )
{
	CCObject* child;
	CCARRAY_FOREACH( m_pChildren, child )
	{
		CC3Node* pChild = (CC3Node*)child;
		if ( pChild )
			pChild->setShouldUseClockwiseFrontFaceWinding( shouldWindCW );
	}
}

bool CC3Node::shouldUseSmoothShading() 
{
	CCObject* child;
	CCARRAY_FOREACH( m_pChildren, child )
	{
		CC3Node* pChild = (CC3Node*)child;
		if ( pChild && pChild->shouldUseSmoothShading() == false )
			return false;
	}

	return true;
}

void CC3Node::setShouldUseSmoothShading( bool shouldSmooth )
{
	CCObject* child;
	CCARRAY_FOREACH( m_pChildren, child )
	{
		CC3Node* pChild = (CC3Node*) child;
		if ( pChild )
			pChild->setShouldUseSmoothShading( shouldSmooth );
	}
}

CC3NormalScaling CC3Node::getNormalScalingMethod()
{
	CCObject* child;
	CCARRAY_FOREACH( m_pChildren, child )
	{
		CC3Node* pChild = (CC3Node*) child;
		if ( pChild )
		{
			CC3NormalScaling csm = pChild->getNormalScalingMethod();
			if (csm != kCC3NormalScalingNone) 
				return csm;
		}
	}

	return kCC3NormalScalingNone;
}

void CC3Node::setNormalScalingMethod( CC3NormalScaling nsMethod )
{
	CCObject* child;
	CCARRAY_FOREACH( m_pChildren, child )
	{
		CC3Node* pChild = (CC3Node*) child;
		if ( pChild )
			pChild->setNormalScalingMethod( nsMethod );
	}
}

bool CC3Node::shouldCacheFaces()
{
	CCObject* child;
	CCARRAY_FOREACH( m_pChildren, child )
	{
		CC3Node* pChild = (CC3Node*) child;
		if ( pChild && pChild->shouldCacheFaces() )
			return true;
	}

	return false;
}

void CC3Node::setShouldCacheFaces( bool shouldCache )
{
	CCObject* child;
	CCARRAY_FOREACH( m_pChildren, child )
	{
		CC3Node* pChild = (CC3Node*) child;
		if ( pChild )
			pChild->setShouldCacheFaces( shouldCache );
	}
}

bool CC3Node::shouldCastShadowsWhenInvisible()
{
	CCObject* child;
	CCARRAY_FOREACH( m_pChildren, child )
	{
		CC3Node* pChild = (CC3Node*) child;
		if (pChild && pChild->shouldCastShadowsWhenInvisible())
			return true;
	}

	return false;
}

void CC3Node::setShouldCastShadowsWhenInvisible( bool shouldCast )
{
	CCObject* child;
	CCARRAY_FOREACH( m_pChildren, child )
	{
		CC3Node* pChild = (CC3Node*) child;
		if ( pChild )
			pChild->setShouldCastShadowsWhenInvisible( shouldCast );
	}
}

bool CC3Node::shouldDisableDepthMask() 
{
	CCObject* child;
	CCARRAY_FOREACH( m_pChildren, child )
	{
		CC3Node* pChild = (CC3Node*) child;
		if ( pChild && pChild->shouldDisableDepthMask() )
			return true;
	}

	return false;
}

void CC3Node::setShouldDisableDepthMask( bool shouldDisable )
{
	CCObject* child;
	CCARRAY_FOREACH( m_pChildren, child )
	{
		CC3Node* pChild = (CC3Node*) child;
		if ( pChild )
			pChild->setShouldDisableDepthMask( shouldDisable );
	}
}

bool CC3Node::shouldDisableDepthTest() 
{
	CCObject* child;
	CCARRAY_FOREACH( m_pChildren, child )
	{
		CC3Node* pChild = (CC3Node*)child;
		if ( pChild && pChild->shouldDisableDepthTest() )
			return true;
	}

	return false;
}

void CC3Node::setShouldDisableDepthTest( bool shouldDisable )
{
	CCObject* child;
	CCARRAY_FOREACH( m_pChildren, child )
	{
		CC3Node* pChild = (CC3Node*) child;
		if ( pChild )
			pChild->setShouldDisableDepthTest( shouldDisable );
	}
}

GLenum CC3Node::getDepthFunction() 
{
	CCObject* child;
	CCARRAY_FOREACH( m_pChildren, child )
	{
		CC3Node* pChild = (CC3Node*) child;
		if ( pChild )
		{
			GLenum df = pChild->getDepthFunction();
			if ( df != GL_NEVER )
				return df;
		}
	}
	
	return GL_NEVER;
}

void CC3Node::setDepthFunction( GLenum depthFunc )
{
	CCObject* child;
	CCARRAY_FOREACH( m_pChildren, child )
	{
		CC3Node* pChild = (CC3Node*) child;
		if ( pChild )
			pChild->setDepthFunction( depthFunc );
	}
}

GLfloat CC3Node::getDecalOffsetFactor() 
{
	CCObject* child;
	CCARRAY_FOREACH( m_pChildren, child )
	{
		CC3Node* pChild = (CC3Node*) child;
		if ( pChild )
		{
			GLfloat df = pChild->getDecalOffsetFactor();
			if ( df )
				return df;
		}
	}
	
	return 0.0f;
}

void CC3Node::setDecalOffsetFactor( GLfloat factor )
{
	CCObject* child;
	CCARRAY_FOREACH( m_pChildren, child )
	{
		CC3Node* pChild = (CC3Node*) child;
		if ( pChild )
			pChild->setDecalOffsetFactor( factor );
	}
}

GLfloat CC3Node::getDecalOffsetUnits() 
{
	CCObject* child;
	CCARRAY_FOREACH( m_pChildren, child )
	{
		CC3Node* pChild = (CC3Node*) child;
		if ( pChild )
		{
			GLfloat du = pChild->getDecalOffsetUnits();
			if ( du )
				return du;
		}
	}
	return 0.0f;
}

void CC3Node::setDecalOffsetUnits( GLfloat units )
{
	CCObject* child;
	CCARRAY_FOREACH(m_pChildren, child)
	{
		CC3Node* pChild = (CC3Node*) child;
		if ( pChild )
			pChild->setDecalOffsetUnits( units );
	}
}

bool CC3Node::shouldUseFixedBoundingVolume()
{
	return m_shouldUseFixedBoundingVolume;
}

void CC3Node::setShouldUseFixedBoundingVolume( bool use )
{
	m_shouldUseFixedBoundingVolume = use;
}

CC3Box CC3Node::getBoundingBox()
{ 
	return getBoundingBoxRelativeTo( this );
}

CC3Box CC3Node::getGlobalBoundingBox()
{
	return getBoundingBoxRelativeTo( NULL ); 
}

CC3Box CC3Node::getBoundingBoxRelativeTo( CC3Node* ancestor )
{
	CC3Box bb = CC3Box::kCC3BoxNull;
	CCObject* child;
	CCARRAY_FOREACH( m_pChildren, child )
	{
		CC3Node* pChild = (CC3Node*) child;
		if (pChild )
			bb = bb.boxUnion( pChild->getBoundingBoxRelativeTo( ancestor ) );
	}

	return bb;
}

CC3Vector CC3Node::getCenterOfGeometry() 
{
	CC3Box bb = getBoundingBox();
	return bb.isNull() ? CC3Vector::kCC3VectorZero : bb.getCenter();
}

CC3Vector CC3Node::getGlobalCenterOfGeometry()
{
	return m_globalTransformMatrix->transformLocation( getCenterOfGeometry() );
}

// By default, individual nodes do not collect their own performance statistics
CC3PerformanceStatistics* CC3Node::getPerformanceStatistics()
{ 
	return NULL; 
}

std::string CC3Node::getStructureDescription()
{
	return "";
}

std::string CC3Node::appendStructureDescriptionTo( const std::string& desc, unsigned int indentLevel )
{
	return "";
}

bool CC3Node::isCamera()
{
	return false;
}

GLfloat CC3Node::getCameraDistanceProduct()
{
	return m_fCameraDistanceProduct;
}

void CC3Node::setCameraDistanceProduct( GLfloat product )
{
	m_fCameraDistanceProduct = product;
}

void CC3Node::setPerformanceStatistics( CC3PerformanceStatistics* aPerfStats )
{

}

bool CC3Node::shouldUseLighting()
{
	CCObject* child;
	CCARRAY_FOREACH( m_pChildren, child )
	{
		CC3Node* pChild = (CC3Node*) child;
		if (pChild && pChild->shouldUseLighting())
			return true;
	}

	return false;
}

void CC3Node::setShouldUseLighting( bool useLighting )
{
	CCObject* child;
	CCARRAY_FOREACH( m_pChildren, child )
	{
		CC3Node* pChild = (CC3Node*) child;
		if (pChild )
			pChild->setShouldUseLighting( useLighting );
	}
}

bool CC3Node::shouldUseLightProbes() 
{
	CCObject* child;
	CCARRAY_FOREACH( m_pChildren, child )
	{
		CC3Node* pChild = (CC3Node*) child;
		if (pChild && pChild->shouldUseLightProbes())
			return true;
	}

	return false;
}

void CC3Node::setShouldUseLightProbes( bool shouldUseLightProbes )
{
	CCObject* child;
	CCARRAY_FOREACH( m_pChildren, child )
	{
		CC3Node* pChild = (CC3Node*) child;
		if (pChild )
			pChild->setShouldUseLightProbes( shouldUseLightProbes );
	}
}

ccColor4F CC3Node::getAmbientColor()
{
	ccColor4F col = kCCC4FBlackTransparent;
	unsigned int childCnt = 0;
	if ( m_pChildren && (childCnt = m_pChildren->count()) > 0 ) 
	{
		GLfloat rSum, bSum, gSum, aSum;
		rSum = bSum = gSum = aSum = 0.0f;

		CCObject* child;
		CCARRAY_FOREACH(m_pChildren, child)
		{
			CC3Node* pChild = (CC3Node*) child;
			if (pChild )
			{
				ccColor4F childColor = pChild->getAmbientColor();
				rSum += childColor.r;
				gSum += childColor.g;
				bSum += childColor.b;
				aSum += childColor.a;
			}
		}
		col = ccc4f(rSum / childCnt, gSum / childCnt, bSum / childCnt, aSum / childCnt);
	}
	return col;
}

void CC3Node::setAmbientColor( const ccColor4F& color )
{
	CCObject* child;
	CCARRAY_FOREACH( m_pChildren, child )
	{
		CC3Node* pChild = (CC3Node*) child;
		if (pChild )
			pChild->setAmbientColor( color );
	}
}

ccColor4F CC3Node::getDiffuseColor() 
{
	ccColor4F col = kCCC4FBlackTransparent;
	unsigned int childCnt = 0;
	if ( m_pChildren && (childCnt = m_pChildren->count()) > 0 ) 
	{
		GLfloat rSum, bSum, gSum, aSum;
		rSum = bSum = gSum = aSum = 0.0f;
		CCObject* child;
		CCARRAY_FOREACH(m_pChildren, child)
		{
			CC3Node* pChild = (CC3Node*) child;
			if (pChild )
			{
				ccColor4F childColor = pChild->getDiffuseColor();
				rSum += childColor.r;
				gSum += childColor.g;
				bSum += childColor.b;
				aSum += childColor.a;
			}
		}
		col = ccc4f(rSum / childCnt, gSum / childCnt, bSum / childCnt, aSum / childCnt);
	}
	return col;
}

void CC3Node::setDiffuseColor( const ccColor4F& color )
{
	CCObject* child;
	CCARRAY_FOREACH(m_pChildren, child)
	{
		CC3Node* pChild = (CC3Node*) child;
		if (pChild )
			pChild->setDiffuseColor( color );
	}
}

ccColor4F CC3Node::getSpecularColor() 
{
	ccColor4F col = kCCC4FBlackTransparent;
	unsigned int childCnt = 0;
	if ( m_pChildren && (childCnt = m_pChildren->count()) > 0 ) 
	{
		GLfloat rSum, bSum, gSum, aSum;
		rSum = bSum = gSum = aSum = 0.0f;
		CCObject* child;
		CCARRAY_FOREACH(m_pChildren, child)
		{
			CC3Node* pChild = (CC3Node*) child;
			if (pChild )
			{
				ccColor4F childColor = pChild->getSpecularColor();
				rSum += childColor.r;
				gSum += childColor.g;
				bSum += childColor.b;
				aSum += childColor.a;
			}
		}
		col = ccc4f(rSum / childCnt, gSum / childCnt, bSum / childCnt, aSum / childCnt);
	}
	
	return col;
}

void CC3Node::setSpecularColor( const ccColor4F& color )
{
	CCObject* child;
	CCARRAY_FOREACH(m_pChildren, child)
	{
		CC3Node* pChild = (CC3Node*) child;
		if (pChild )
			pChild->setSpecularColor( color );
	}
}

ccColor4F CC3Node::getEmissionColor() 
{
	ccColor4F col = kCCC4FBlackTransparent;
	unsigned int childCnt = 0;
	if (m_pChildren && (childCnt = m_pChildren->count()) > 0) 
	{
		GLfloat rSum, bSum, gSum, aSum;
		rSum = bSum = gSum = aSum = 0.0f;
		CCObject* child;
		CCARRAY_FOREACH(m_pChildren, child)
		{
			CC3Node* pChild = (CC3Node*) child;
			if (pChild )
			{
				ccColor4F childColor = pChild->getEmissionColor();
				rSum += childColor.r;
				gSum += childColor.g;
				bSum += childColor.b;
				aSum += childColor.a;
			}
		}
		col = ccc4f(rSum / childCnt, gSum / childCnt, bSum / childCnt, aSum / childCnt);
	}

	return col;
}

void CC3Node::setEmissionColor( const ccColor4F& color )
{
	CCObject* child;
	CCARRAY_FOREACH(m_pChildren, child)
	{
		CC3Node* pChild = (CC3Node*) child;
		if (pChild )
			pChild->setEmissionColor( color );
	}
}

GLfloat CC3Node::getShininess()
{
	GLfloat shininess = 0.0f;
	unsigned int childCnt = 0;
	if (m_pChildren && (childCnt = m_pChildren->count()) > 0) 
	{
		GLfloat sSum = 0.0f;
		CCObject* child;
		CCARRAY_FOREACH(m_pChildren, child)
		{
			CC3Node* pChild = (CC3Node*) child;
			if (pChild )
				sSum += pChild->getShininess();
		}
		shininess = sSum / childCnt;
	}
	return shininess;
}

void CC3Node::setShininess( GLfloat shininess )
{
	CCObject* child;
	CCARRAY_FOREACH(m_pChildren, child)
	{
		CC3Node* pChild = (CC3Node*) child;
		if (pChild )
			pChild->setShininess( shininess );
	}
}

GLfloat CC3Node::getReflectivity()
{
	GLfloat reflectivity = 0.0f;
	unsigned int childCnt = 0;
	if (m_pChildren && (childCnt = m_pChildren->count()) > 0) 
	{
		GLfloat rSum = 0.0f;
		CCObject* child;
		CCARRAY_FOREACH(m_pChildren, child)
		{
			CC3Node* pChild = (CC3Node*) child;
			if (pChild )
				rSum += pChild->getReflectivity();
		}
		reflectivity = rSum / childCnt;
	}
	return reflectivity;
}

void CC3Node::setReflectivity( GLfloat reflectivity )
{
	CCObject* child;
	CCARRAY_FOREACH(m_pChildren, child)
	{
		CC3Node* pChild = (CC3Node*) child;
		if (pChild )
			pChild->setReflectivity( reflectivity );
	}
}

CC3Texture* CC3Node::getTexture()
{
	CCObject* child;
	CCARRAY_FOREACH(m_pChildren, child)
	{
		CC3Node* pChild = (CC3Node*) child;
		if (pChild  )
		{
			CC3Texture* tex = pChild->getTexture();
			if ( tex )
				return tex;
		}
	}
	return NULL;
}

void CC3Node::setTexture( CC3Texture* texture )
{
	CCObject* child;
	CCARRAY_FOREACH(m_pChildren, child)
	{
		CC3Node* pChild = (CC3Node*) child;
		if (pChild )
			pChild->setTexture( texture );
	}
}

void CC3Node::addTexture( CC3Texture* texture )
{
	CCObject* child;
	CCARRAY_FOREACH(m_pChildren, child)
	{
		CC3Node* pChild = (CC3Node*) child;
		if (pChild )
			pChild->addTexture( texture );
	}
}

CC3Vector4 CC3Node::getGlobalLightPosition()
{
	CCObject* child;
	CCARRAY_FOREACH(m_pChildren, child)
	{
		CC3Node* pChild = (CC3Node*) child;
		if (pChild )
		{
			CC3Vector4 glp = pChild->getGlobalLightPosition();
			if ( !glp.isZero() ) 
				return glp;
		}
	}

	return CC3Vector4::kCC3Vector4Zero;
}

void CC3Node::setGlobalLightPosition( const CC3Vector4& aPosition )
{
	CCObject* child;
	CCARRAY_FOREACH(m_pChildren, child)
	{
		CC3Node* pChild = (CC3Node*) child;
		if (pChild )
			pChild->setGlobalLightPosition( aPosition );
	}
}


CC3ShaderContext* CC3Node::getShaderContext()
{
	CCObject* child;
	CCARRAY_FOREACH(m_pChildren, child)
	{
		CC3Node* pChild = (CC3Node*) child;
		if (pChild  )
		{
			CC3ShaderContext* ctx = pChild->getShaderContext();
			if ( ctx )
				return ctx;
		}
	}
	return NULL;
}

void CC3Node::setShaderContext( CC3ShaderContext* shaderContext )
{
	CCObject* child;
	CCARRAY_FOREACH(m_pChildren, child)
	{
		CC3Node* pChild = (CC3Node*) child;
		if (pChild )
			pChild->setShaderContext( shaderContext );
	}
}

CC3ShaderProgram* CC3Node::getShaderProgram()
{
	CCObject* child;
	CCARRAY_FOREACH(m_pChildren, child)
	{
		CC3Node* pChild = (CC3Node*) child;
		if (pChild  )
		{
			CC3ShaderProgram* shader = pChild->getShaderProgram();
			if ( shader )
				return shader;
		}
	}
	return NULL;
}

void CC3Node::setShaderProgram( CC3ShaderProgram* shaderProgram )
{
	CCObject* child;
	CCARRAY_FOREACH(m_pChildren, child)
	{
		CC3Node* pChild = (CC3Node*) child;
		if (pChild )
			pChild->setShaderProgram( shaderProgram );
	}
}

void CC3Node::selectShaders()
{
	CCObject* child;
	CCARRAY_FOREACH(m_pChildren, child)
	{
		CC3Node* pChild = (CC3Node*) child;
		if (pChild )
			pChild->selectShaders();
	}
}

void CC3Node::removeShaders()
{ 
	CCObject* child;
	CCARRAY_FOREACH(m_pChildren, child)
	{
		CC3Node* pChild = (CC3Node*) child;
		if (pChild )
			pChild->removeShaders();
	}
}

ccColor3B CC3Node::getColor()
{
	ccColor4F colorSum = kCCC4FBlackTransparent;
	unsigned int childCnt = 0;
	if (m_pChildren && (childCnt = m_pChildren->count()) > 0) 
	{
		CCObject* child;
		CCARRAY_FOREACH(m_pChildren, child)
		{
			CC3Node* pChild = (CC3Node*) child;
			if (pChild )
			{
				ccColor4F childColor = CCC4FFromCCColorRef(pChild->getColor());
				colorSum.r += childColor.r;
				colorSum.g += childColor.g;
				colorSum.b += childColor.b;
			}
		}

		colorSum = CCC4FUniformScale(colorSum, (1.0f / (GLfloat)childCnt));
	}

	return CCColorRefFromCCC4F(colorSum);
}

void CC3Node::setColor( const ccColor3B& color )
{
	if (m_cascadeColorEnabled) 
	{
		CCObject* child;
		CCARRAY_FOREACH(m_pChildren, child)
		{
			CC3Node* pChild = (CC3Node*) child;
			if (pChild )
				pChild->setColor( color );
		}
	}
}

CCOpacity CC3Node::getOpacity()
{
	CCOpacity opc = 0;
	unsigned int childCnt = 0;
	if (m_pChildren && (childCnt = m_pChildren->count()) > 0) 
	{
		GLfloat oSum = 0;
		CCObject* child;
		CCARRAY_FOREACH(m_pChildren, child)
		{
			CC3Node* pChild = (CC3Node*) child;
			if (pChild )
				 oSum += pChild->getOpacity();
		}
		opc = (CCOpacity)(oSum / childCnt);
	}
	return opc;
}

void CC3Node::setOpacity( CCOpacity opacity )
{
	if (m_cascadeColorEnabled) 
	{
		CCObject* child;
		CCARRAY_FOREACH(m_pChildren, child)
		{
			CC3Node* pChild = (CC3Node*) child;
			if (pChild )
				pChild->setOpacity( opacity );
		}
	}
}

ccColor3B CC3Node::getDisplayedColor()
{ 
	return getColor(); 
}

bool CC3Node::isCascadeColorEnabled() 
{ 
	return m_cascadeColorEnabled; 
}

void CC3Node::setCascadeColorEnabled( bool cascadeColorEnabled )
{
	m_cascadeColorEnabled = cascadeColorEnabled;
}

void CC3Node::updateDisplayedColor( CCColorRef color )
{

}

CCOpacity CC3Node::getDisplayedOpacity() 
{ 
	return getOpacity(); 
}

bool CC3Node::isCascadeOpacityEnabled() 
{ 
	return m_cascadeOpacityEnabled; 
}

void CC3Node::setCascadeOpacityEnabled( bool cascadeOpacityEnabled )
{
	m_cascadeOpacityEnabled = cascadeOpacityEnabled;
}

void CC3Node::updateDisplayedOpacity( CCOpacity opacity )
{

}

ccBlendFunc CC3Node::getBlendFunc()
{
	// From first child if exists
	CCObject* child;
	CCARRAY_FOREACH(m_pChildren, child)
	{
		CC3Node* pChild = (CC3Node*) child;
		if (pChild )
			return pChild->getBlendFunc();
	}

	ccBlendFunc blendFunc;
	blendFunc.src = GL_ONE;
	blendFunc.dst = GL_ZERO;
	return blendFunc;
}

void CC3Node::setBlendFunc( ccBlendFunc aBlendFunc )
{
	CCObject* child;
	CCARRAY_FOREACH(m_pChildren, child)
	{
		CC3Node* pChild = (CC3Node*) child;
		if (pChild )
			pChild->setBlendFunc( aBlendFunc );
	}
}

bool CC3Node::isOpaque()
{
	CCObject* child;
	CCARRAY_FOREACH(m_pChildren, child)
	{
		CC3Node* pChild = (CC3Node*) child;
		if (pChild && !pChild->isOpaque())
			return false;
	}
	
	return true;
}

void CC3Node::setIsOpaque( bool opaque )
{
	CCObject* child;
	CCARRAY_FOREACH(m_pChildren, child)
	{
		CC3Node* pChild = (CC3Node*) child;
		if (pChild )
			pChild->setIsOpaque( opaque );
	}
}

bool CC3Node::shouldBlendAtFullOpacity()
{
	CCObject* child;
	CCARRAY_FOREACH(m_pChildren, child)
	{
		CC3Node* pChild = (CC3Node*) child;
		if (pChild && pChild->shouldBlendAtFullOpacity())
			return true;
	}

	return false;
}

void CC3Node::setShouldBlendAtFullOpacity( bool shouldBlend )
{
	CCObject* child;
	CCARRAY_FOREACH(m_pChildren, child)
	{
		CC3Node* pChild = (CC3Node*) child;
		if (pChild )
			pChild->setShouldBlendAtFullOpacity( shouldBlend );
	}
}

bool CC3Node::shouldApplyOpacityAndColorToMeshContent()
{
	CCObject* child;
	CCARRAY_FOREACH(m_pChildren, child)
	{
		CC3Node* pChild = (CC3Node*) child;
		if (pChild )
			return pChild->shouldApplyOpacityAndColorToMeshContent();
	}

	return false;
}

void CC3Node::setShouldApplyOpacityAndColorToMeshContent( bool shouldApply )
{
	CCObject* child;
	CCARRAY_FOREACH(m_pChildren, child)
	{
		CC3Node* pChild = (CC3Node*) child;
		if (pChild )
			pChild->setShouldApplyOpacityAndColorToMeshContent( shouldApply );
	}
}


GLfloat CC3Node::getLineWidth()
{
	CCObject* child;
	CCARRAY_FOREACH(m_pChildren, child)
	{
		CC3Node* pChild = (CC3Node*) child;
		if (pChild )
			return pChild->getLineWidth();
	}

	return 1.f;
}

void CC3Node::setLineWidth( GLfloat aLineWidth )
{
	CCObject* child;
	CCARRAY_FOREACH(m_pChildren, child)
	{
		CC3Node* pChild = (CC3Node*) child;
		if (pChild )
			pChild->setLineWidth( aLineWidth );
	}
}

bool CC3Node::shouldSmoothLines() 
{
	// From first child if exists
	CCObject* child;
	CCARRAY_FOREACH(m_pChildren, child)
	{
		CC3Node* pChild = (CC3Node*) child;
		if (pChild )
			return pChild->shouldSmoothLines();
	}	

	return false;
}

void CC3Node::setShouldSmoothLines( bool shouldSmooth )
{
	CCObject* child;
	CCARRAY_FOREACH(m_pChildren, child)
	{
		CC3Node* pChild = (CC3Node*) child;
		if (pChild )
			pChild->setShouldSmoothLines( shouldSmooth );
	}
}

GLenum CC3Node::getLineSmoothingHint()
{
	// From first child if exists
	CCObject* child;
	CCARRAY_FOREACH(m_pChildren, child)
	{
		CC3Node* pChild = (CC3Node*) child;
		if (pChild )
			return pChild->getLineSmoothingHint();
	}	

	return GL_DONT_CARE;
}

void CC3Node::setLineSmoothingHint( GLenum aHint )
{
	CCObject* child;
	CCARRAY_FOREACH(m_pChildren, child)
	{
		CC3Node* pChild = (CC3Node*) child;
		if (pChild )
			pChild->setLineSmoothingHint( aHint );
	}
}


void CC3Node::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );

	m_localTransformMatrix          = NULL;
	m_globalTransformMatrix         = new CC3AffineMatrix;		// retained
	m_globalTransformMatrix->init();
	m_globalTransformMatrixInverted = NULL;
	m_globalRotationMatrix          = NULL;
	m_rotator                       = new CC3Rotator;			// retained
	m_pTransformListeners           = NULL;
	m_pAnimationStates              = NULL;
	m_isAnimationDirty              = false;
	m_pBoundingVolume               = NULL;
	m_fBoundingVolumePadding        = 0.0f;
	m_shouldUseFixedBoundingVolume  = false;
	m_location                      = CC3Vector::kCC3VectorZero;
	m_projectedLocation             = CC3Vector::kCC3VectorZero;
	m_scale                         = CC3Vector::kCC3VectorUnitCube;
	m_fCameraDistanceProduct        = 0.0f;
	m_touchEnabled                  = false;
	m_shouldInheritTouchability     = true;
	m_shouldAllowTouchableWhenInvisible = false;
	m_visible                       = true;
	m_isRunning                     = false;
	m_shouldStopActionsWhenRemoved  = true;
	m_shouldAutoremoveWhenEmpty     = false;
	m_cascadeColorEnabled           = true;
	m_cascadeOpacityEnabled         = true;
	m_shouldCastShadows             = true;
	m_isBeingAdded                  = false;
}

CC3Node* CC3Node::node() 
{
	CC3Node* pNode = new CC3Node;
	pNode->init();
	pNode->autorelease();
	
	return pNode;
}

CC3Node* CC3Node::nodeWithTag( int aTag ) 
{ 
	CC3Node* pNode = new CC3Node;
	pNode->initWithTag( aTag, "" );
	pNode->autorelease();

	return pNode;
}

CC3Node* CC3Node::nodeWithName( const std::string& aName ) 
{ 
	CC3Node* pNode = new CC3Node;
	pNode->initWithName( aName );
	pNode->autorelease();

	return pNode;
}

CC3Node* CC3Node::nodeWithTag( int aTag, const std::string& aName )
{
	CC3Node* pNode = new CC3Node;
	pNode->initWithTag( aTag, aName );
	pNode->autorelease();

	return pNode;
}

// Protected properties for copying
CCArray* CC3Node::getAnimationStates()
{ 
	return m_pAnimationStates; 
}

void CC3Node::populateFrom( CC3Node* another )
{
	super::populateFrom( another );
	
	// Transform matrices are not copied, but are built from copied transform properties
	m_location = another->getLocation();
	m_projectedLocation = another->getProjectedLocation();
	m_scale = another->getScale();
	markTransformDirty();

	CC_SAFE_RELEASE( m_rotator );
	if ( another->getRotator() )
	{
		m_rotator = (CC3Rotator*)another->getRotator()->copy();					
		//CC_SAFE_RETAIN( m_rotator );
	}

	CC_SAFE_RELEASE( m_pBoundingVolume );
	if ( another->getBoundingVolume() )
	{
		m_pBoundingVolume = (CC3NodeBoundingVolume*)another->getBoundingVolume()->copy();
		m_pBoundingVolume->setNode( this );
		//CC_SAFE_RETAIN( m_pBoundingVolume );
	}
	m_fBoundingVolumePadding = another->getBoundingVolumePadding();
	m_shouldUseFixedBoundingVolume = another->shouldUseFixedBoundingVolume();

	m_isAnimationDirty = false;
	CCObject* object;
	CCARRAY_FOREACH( another->getAnimationStates(), object )
	{
		CC3NodeAnimationState* state = (CC3NodeAnimationState*)object;
		addAnimation( state->getAnimation(), state->getTrackID() );
	}
	
	// Transform listeners are not copied. Managing listeners must be deliberate.

	m_touchEnabled = another->isTouchEnabled();
	m_shouldInheritTouchability = another->shouldInheritTouchability();
	m_shouldAllowTouchableWhenInvisible = another->shouldAllowTouchableWhenInvisible();
	m_visible = another->m_visible;
	m_isRunning = another->isRunning();
	m_shouldStopActionsWhenRemoved = another->shouldStopActionsWhenRemoved();
	m_shouldAutoremoveWhenEmpty = another->shouldAutoremoveWhenEmpty();
	m_cascadeColorEnabled = another->isCascadeColorEnabled();
	m_cascadeOpacityEnabled = another->isCascadeOpacityEnabled();
	m_fCameraDistanceProduct = another->getCameraDistanceProduct();
	m_shouldCastShadows = another->shouldCastShadows();
	
	setShouldDrawDescriptor( another->shouldDrawDescriptor() );		// May create a child node
	setShouldDrawWireframeBox( another->shouldDrawWireframeBox() );	// May create a child node
}

/**
 * Copying of children is performed here instead of in populateFrom:
 * so that subclasses will be completely configured before children are added.
 * Subclasses that extend copying should not override this method,
 * but should override the populateFrom: method instead.
 */
CCObject* CC3Node::copyWithZone( CCZone* zone )
{
	CC3Node* aCopy = new CC3Node;
    aCopy->init();
	aCopy->addCopiesOfChildrenFrom( this );
	return aCopy;
}

/**
 * Adds copies of the child nodes of the other node to this node.
 * Only the children that have the shouldIncludeInDeepCopy property set to YES are copied.
 * The children from the other node are added to the children that already exist in this node
 * (which were possibly added during instantiation init).
 */
void CC3Node::addCopiesOfChildrenFrom( CC3Node* another )
{
	CCArray* otherKids = another->getChildren();
	CCObject* object;
	CCARRAY_FOREACH( otherKids, object )
	{
		CC3Node* otherKid = (CC3Node*)object;
		if ( otherKid->shouldIncludeInDeepCopy() )
		{
			CC3Node* myKid = (CC3Node*)otherKid->copy();
			addChild( myKid );
			myKid->release();
		}
	}
}

void CC3Node::createGLBuffers() 
{ 
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->createGLBuffers();
		}
	}
}

void CC3Node::deleteGLBuffers()
{
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->deleteGLBuffers();
		}
	}
}

void CC3Node::releaseRedundantContent()
{
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->releaseRedundantContent();
		}
	}
}

void CC3Node::retainVertexContent()
{ 
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->retainVertexContent();
		}
	}
}

void CC3Node::retainVertexLocations()
{
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->retainVertexLocations();
		}
	}
}

void CC3Node::retainVertexNormals()
{
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->retainVertexNormals();
		}
	}
}

void CC3Node::retainVertexTangents()
{ 
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->retainVertexTangents();
		}
	}
}

void CC3Node::retainVertexBitangents()
{
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->retainVertexBitangents();
		}
	}
}

void CC3Node::retainVertexColors()
{
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->retainVertexColors();
		}
	}
}

void CC3Node::retainVertexBoneWeights()
{
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->retainVertexBoneWeights();
		}
	}
}

void CC3Node::retainVertexBoneIndices()
{
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->retainVertexBoneIndices();
		}
	}
}

void CC3Node::retainVertexPointSizes()
{
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->retainVertexPointSizes();
		}
	}
}

void CC3Node::retainVertexTextureCoordinates()
{
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->retainVertexTextureCoordinates();
		}
	}
}

void CC3Node::retainVertexIndices()
{
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->retainVertexIndices();
		}
	}
}

void CC3Node::doNotBufferVertexContent()
{
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->doNotBufferVertexContent();
		}
	}
}

void CC3Node::doNotBufferVertexLocations()
{
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->doNotBufferVertexLocations();
		}
	}
}

void CC3Node::doNotBufferVertexNormals()
{
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->doNotBufferVertexNormals();
		}
	}
}

void CC3Node::doNotBufferVertexTangents()
{
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->doNotBufferVertexTangents();
		}
	}
}

void CC3Node::doNotBufferVertexBitangents()
{
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->doNotBufferVertexBitangents();
		}
	}
}

void CC3Node::doNotBufferVertexColors()
{
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->doNotBufferVertexColors();
		}
	}
}

void CC3Node::doNotBufferVertexBoneWeights()
{
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->doNotBufferVertexBoneWeights();
		}
	}
}

void CC3Node::doNotBufferVertexBoneIndices()
{
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->doNotBufferVertexBoneIndices();
		}
	}
}

void CC3Node::doNotBufferVertexPointSizes()
{
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->doNotBufferVertexPointSizes();
		}
	}
}

void CC3Node::doNotBufferVertexTextureCoordinates()
{
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->doNotBufferVertexTextureCoordinates();
		}
	}
}

void CC3Node::doNotBufferVertexIndices()
{
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->doNotBufferVertexIndices();
		}
	}
}

void CC3Node::flipNormals()
{
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->flipNormals();
		}
	}
}

bool CC3Node::expectsVerticallyFlippedTextures()
{
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child && child->expectsVerticallyFlippedTextures() )
			return true;
	}

	return false;
}

void CC3Node::setExpectsVerticallyFlippedTextures( bool expectsFlipped )
{
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->setExpectsVerticallyFlippedTextures( expectsFlipped );
		}
	}
}

void CC3Node::flipTexturesVertically()
{
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->flipTexturesVertically();
		}
	}
}

void CC3Node::flipTexturesHorizontally()
{
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->flipTexturesHorizontally();
		}
	}
}

// Class variable tracking the most recent tag value assigned for CC3Nodes.
// This class variable is automatically incremented whenever the method nextTag is called.
static GLuint lastAssignedNodeTag = 0;

GLuint CC3Node::nextTag()
{ 
	return ++lastAssignedNodeTag; 
}

void CC3Node::resetTagAllocation() 
{ 
	lastAssignedNodeTag = 0; 
}

bool CC3Node::hasLocalContent()
{
	return false;
}

bool CC3Node::isVisible() 
{ 
	return m_visible && (!m_pParent || m_pParent->isVisible()); 
}

void CC3Node::setVisible( bool visible )
{
	m_visible = visible;
}

void CC3Node::show()
{
	setVisible( true );
}

void CC3Node::hide()
{
	setVisible( false );
}

GLint CC3Node::getZOrder()
{
	unsigned int childCount = m_pChildren ? m_pChildren->count() : 0;
	if (childCount > 0) {
		GLint zoSum = 0;

		CCObject* object;
		CCARRAY_FOREACH( m_pChildren, object )
		{
			CC3Node* child = (CC3Node*)object;
			if ( child )
			{
				zoSum += child->getZOrder();
			}
		}

		return zoSum / childCount;
	}
	return 0;
}

void CC3Node::setZOrder( GLint zOrder )
{
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->setZOrder( zOrder );
		}
	}
}

/** Protected template method invoked from the update visitor just before updating the transform. */
void CC3Node::processUpdateBeforeTransform( CC3NodeUpdatingVisitor* visitor )
{
	checkCameraTarget();
	updateFromAnimationState();
	updateBeforeTransform( visitor );
}

// Default invokes legacy updateBeforeChildren: and update: methods, for backwards compatibility.
void CC3Node::updateBeforeTransform( CC3NodeUpdatingVisitor* visitor )
{
	updateBeforeChildren( visitor );
	update( visitor->getDeltaTime() );
}

/**
 * Protected template method invoked from the update visitor just after updating
 * the transform.
 *
 * This implementation simply invokes the application callback updateAfterTransform:
 * method. Framework subclasses that want to perform other activity may override, but
 * should invoke this superclass method to ensure that the updateAfterTransform:
 * will be invoked.
 */
void CC3Node::processUpdateAfterTransform( CC3NodeUpdatingVisitor* visitor )
{
	updateAfterTransform( visitor );
}

// Default invokes legacy updateAfterChildren: method, for backwards compatibility.
void CC3Node::updateAfterTransform( CC3NodeUpdatingVisitor* visitor )
{
	updateAfterChildren( visitor );
}

void CC3Node::update( float dt )
{

}

bool CC3Node::getTransformListeners( std::vector<CC3NodeTransformListenerProtocol*>& listeners )
{
	if ( m_pTransformListeners )
	{
		listeners = m_pTransformListeners->transformListeners();
		return true;
	}

	return false;
}

void CC3Node::addTransformListener( CC3NodeTransformListenerProtocol* aListener )
{
	if ( !aListener ) 
		return;
	
	// Lazily create the transform listeners cache, and add the listener
	if( !m_pTransformListeners ) 
	{
		m_pTransformListeners = CC3NodeTransformListeners::listenersForNode( this );
		m_pTransformListeners->retain();
	}

	m_pTransformListeners->addTransformListener( aListener );
	
	// Notify immediately, to ensure the listener is aware of current state.
	aListener->nodeWasTransformed( this );
}

void CC3Node::removeTransformListener( CC3NodeTransformListenerProtocol* aListener )
{
	// Remove the listener, then remove the transform listeners cache if no further listeners	
	if ( m_pTransformListeners )
	{
		m_pTransformListeners->removeTransformListener( aListener );
		if ( m_pTransformListeners->isEmpty() ) 
		{
			m_pTransformListeners->release();
			m_pTransformListeners = NULL;
		}
	}
}

void CC3Node::removeAllTransformListeners()
{
	if ( m_pTransformListeners )
		m_pTransformListeners->removeAllTransformListeners();
}

void CC3Node::notifyTransformListeners()
{ 
	if ( m_pTransformListeners )
		m_pTransformListeners->notifyTransformListeners();
}

void CC3Node::notifyDestructionListeners() 
{
	if ( m_pTransformListeners )
		m_pTransformListeners->notifyDestructionListeners();
}

void CC3Node::nodeWasTransformed( CC3Node* aNode )
{
	if ( (aNode == getTarget()) && shouldUpdateToTarget() ) 
		markTransformDirty();
}

void CC3Node::nodeWasDestroyed( CC3Node* aNode )
{
	// Can't retrieve target from rotator, because it might be deallocated already,
	// and ARC will attempt to retain and autorelease it, resulting in a zombie.
	// Maybe revisit need for this once using true weak refs under minimum iOS 5.
	if ( m_rotator )
		m_rotator->clearIfTarget( aNode );
}

bool CC3Node::shouldUpdateToTarget()
{
	if ( m_rotator )
		return m_rotator->shouldUpdateToTarget();

	return false;
}

bool CC3Node::isTransformDirty() 
{ 
	return m_globalTransformMatrix->isDirty(); 
}

void CC3Node::updateBoundingVolume()
{

}

void CC3Node::markTransformDirty() 
{
	// Mark the local matrix as dirty always, since it is independent of the globalTransformMatrix
	if ( m_localTransformMatrix )
		m_localTransformMatrix->setIsDirty(  true );

	// All other transform activity is global, and is dependent on the globalTransformMatrix,
	// so don't continue if it is already dirty, including marking descendants.
	if ( m_globalTransformMatrix->isDirty() ) 
			return;

	m_globalTransformMatrix->setIsDirty(  true );

	if ( m_globalTransformMatrixInverted )
		m_globalTransformMatrixInverted->setIsDirty(  true );

	if ( m_globalRotationMatrix )
		m_globalRotationMatrix->setIsDirty(  true );
	
	if ( m_pBoundingVolume )
		m_pBoundingVolume->markTransformDirty();
	
	notifyTransformListeners();
	
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->markTransformDirty();
		}
	}
}

CC3Matrix* CC3Node::getLocalTransformMatrix()
{
	if ( !m_localTransformMatrix ) 
	{
		setLocalTransformMatrix( CC3AffineMatrix::matrix() );		// retained
		m_localTransformMatrix->setIsDirty( true );
	}

	if ( m_localTransformMatrix->isDirty() )  
		buildLocalTransformMatrix();

	return m_localTransformMatrix;
}

void CC3Node::buildLocalTransformMatrix()
{
	m_localTransformMatrix->populateIdentity();
	applyLocalTransformsTo( m_localTransformMatrix );
	m_localTransformMatrix->setIsDirty( false );
}

void CC3Node::setLocalTransformMatrix( CC3Matrix* localTransformMatrix )
{
	if ( localTransformMatrix == m_localTransformMatrix ) 
		return;
	
	CC_SAFE_RELEASE(m_localTransformMatrix);
	CC_SAFE_RETAIN(localTransformMatrix);
	m_localTransformMatrix = localTransformMatrix;
	
	// Changing this matrix affects all other matrices, but be sure to mark this matrix
	// as NOT dirty, so it won't get rebuilt from the transform properties.
	markTransformDirty();

	if ( m_localTransformMatrix )
		m_localTransformMatrix->setIsDirty( false );
}

CC3Matrix* CC3Node::getGlobalTransformMatrix() 
{
	if ( m_globalTransformMatrix && m_globalTransformMatrix->isDirty() ) 
		buildGlobalTransformMatrix();

	return m_globalTransformMatrix;
}

void CC3Node::buildGlobalTransformMatrix()
{
	m_globalTransformMatrix->populateFrom( m_pParent ? m_pParent->getGlobalTransformMatrix() : NULL );

	// If local transform matrix exists, use it.
	// otherwise, apply transforms directly to global matrix.
	if ( m_localTransformMatrix )
		m_globalTransformMatrix->multiplyBy( getLocalTransformMatrix() );
	else
		applyLocalTransformsTo( m_globalTransformMatrix );

	m_globalTransformMatrix->setIsDirty( false );
}

/**
 * Template method that applies the local location, rotation and scale properties to
 * the specified matrix. Subclasses may override to enhance or modify this behaviour.
 */
void CC3Node::applyLocalTransformsTo( CC3Matrix* matrix )
{
	applyTranslationTo( matrix );
	applyRotationTo( matrix );
	applyScalingTo( matrix );
}

/** Template method that applies the local location property to the transform matrix. */
void CC3Node::applyTranslationTo( CC3Matrix* matrix )
{
	matrix->translateBy( m_location );
}

/**
 * Template method that applies the rotation in the rotator to the specified matrix.
 *
 * Target location can only be applied once translation is complete, because the direction to
 * the target depends on the transformed global location of both this node and the target location.
 */
void CC3Node::applyRotationTo( CC3Matrix* matrix )
{
	updateTargetLocation();
	if ( shouldRotateToTargetLocation() )
		applyTargetLocation();
	applyRotatorTo( matrix );
}

/** Check if target location needs to be updated from target, and do so if needed. */
void CC3Node::updateTargetLocation()
{
	if (shouldUpdateToTarget() && !isTrackingForBumpMapping())
		setTargetLocation( getTarget()->getGlobalLocation() );
}

/**
 * Template property simply delegates to rotator.
 * Subclasses can override to change target tracking behaviour.
 */
bool CC3Node::shouldRotateToTargetLocation()
{ 
	return m_rotator->shouldRotateToTargetLocation();
}

/** Template method to update the rotator transform from the targetLocation. */
void CC3Node::applyTargetLocation()
{
	if ( isTargettingConstraintLocal() )
		applyTargetLocationAsLocal();
	else
		applyTargetLocationAsGlobal();
}

/**
 * Rotates the node to point to the target location, by converting the target location to the
 * local coordinate system, treating the referenceUpDirection as a local direction, and rotating
 * in the local coordinate system.
 */
void CC3Node::applyTargetLocationAsLocal()
{
	CC3Vector targLoc = getTargetLocation();
	if (m_pParent) 
		targLoc = m_pParent->getGlobalTransformMatrixInverted()->transformLocation( targLoc );
	targLoc = rotationallyRestrictTargetLocation( targLoc );
	getTargettingRotator()->rotateToTargetLocation( targLoc, getLocation(), getReferenceUpDirection() );
}

/**
 * Rotates the node to point to the target location, by treating the referenceUpDirection as a
 * global direction, and rotating in the global coordinate system.
 */
void CC3Node::applyTargetLocationAsGlobal()
{
	CC3Vector targLoc = getTargetLocation();
	targLoc = rotationallyRestrictTargetLocation( targLoc );
	getTargettingRotator()->rotateToTargetLocation( targLoc, getGlobalLocation(), getReferenceUpDirection() );
	convertRotatorGlobalToLocal();
}

/**
 * Returns whether the targettingConstraint is specified in the local coordinate system
 * or the global coordinate system.
 */
bool CC3Node::isTargettingConstraintLocal()
{
	switch ( getTargettingConstraint() ) 
	{
		case kCC3TargettingConstraintGlobalUnconstrained:
		case kCC3TargettingConstraintGlobalXAxis:
		case kCC3TargettingConstraintGlobalYAxis:
		case kCC3TargettingConstraintGlobalZAxis:
			return false;
		case kCC3TargettingConstraintLocalUnconstrained:
		case kCC3TargettingConstraintLocalXAxis:
		case kCC3TargettingConstraintLocalYAxis:
		case kCC3TargettingConstraintLocalZAxis:
		default:
			return true;
	}
}

/**
 * Constrains rotation to the specified target location by changing the cooresponding coordinate
 * of the location to be the same as this node, so that the node will not rotate out of that plane.
 */
CC3Vector CC3Node::rotationallyRestrictTargetLocation( const CC3Vector& loc )
{
	CC3Vector aLocation = loc;
	switch ( getTargettingConstraint() ) 
	{
		case kCC3TargettingConstraintLocalXAxis:
			aLocation.x = getLocation().x;
			break;
		case kCC3TargettingConstraintLocalYAxis:
			aLocation.y = getLocation().y;
			break;
		case kCC3TargettingConstraintLocalZAxis:
			aLocation.z = getLocation().z;
			break;
		case kCC3TargettingConstraintGlobalXAxis:
			aLocation.x = getGlobalLocation().x;
			break;
		case kCC3TargettingConstraintGlobalYAxis:
			aLocation.y = getGlobalLocation().y;
			break;
		case kCC3TargettingConstraintGlobalZAxis:
			aLocation.z = getGlobalLocation().z;
			break;
		default:
			break;
	}
	return aLocation;
}

/**
 * Converts the rotator's rotation matrix from global to local coordinates,
 * by applying an inverse of the parent's global rotation matrix.
 *
 * If Mc is the local rotation of the child, Mp is the global rotation of
 * the parent node, and Mg is the global rotation of this child node:
 *   Mg = Mp.Mc
 *   Mp(-1).Mg = Mp(-1).Mp.Mc
 *   Mp(-1).Mg = Mc
 *
 * Therefore, we can determine the local rotation of this node by multiplying
 * its global rotation by the inverse of the parent's global rotation.
 */
 void CC3Node::convertRotatorGlobalToLocal()
 {
	if ( !m_pParent ) 
		return;		// No transform needed if no parent
	
	CC3Matrix3x3 parentInvRotMtx;
	m_pParent->getGlobalRotationMatrix()->populateCC3Matrix3x3( &parentInvRotMtx );
	CC3Matrix3x3InvertRigid( &parentInvRotMtx );
	m_rotator->getRotationMatrix()->leftMultiplyByCC3Matrix3x3( &parentInvRotMtx );
}

/** Apply the rotational state of the rotator to the specified matrix. */
void CC3Node::applyRotatorTo( CC3Matrix* matrix )
{
	m_rotator->applyRotationTo( matrix );
}

/** Template method that applies the local scale property to the specified matrix. */
void CC3Node::applyScalingTo( CC3Matrix* matrix )
{ 
	matrix->scaleBy( CC3EnsureMinScaleVector(m_scale) ); 
}

/**
 * Returns the inverse of the globalTransformMatrix.
 *
 * Since this inverse matrix is not commonly used, and is often expensive to compute,
 * it is only calculated when the globalTransformMatrix has changed, and then only on demand.
 */
CC3Matrix* CC3Node::getGlobalTransformMatrixInverted()
{
	if ( !m_globalTransformMatrixInverted ) 
	{
		m_globalTransformMatrixInverted = new CC3AffineMatrix;		// retained
		m_globalTransformMatrixInverted->init();
		m_globalTransformMatrixInverted->setIsDirty( true );
	}

	if ( m_globalTransformMatrixInverted->isDirty() )
		buildGlobalTransformMatrixInverted();

	return m_globalTransformMatrixInverted;
}

void CC3Node::buildGlobalTransformMatrixInverted()
{
	if ( m_globalTransformMatrixInverted )
	{
		m_globalTransformMatrixInverted->populateFrom( getGlobalTransformMatrix() );
		m_globalTransformMatrixInverted->invert();
		m_globalTransformMatrixInverted->setIsDirty( false );
	}	
}

/**
 * Returns a matrix representing all of the rotations that make up this node,
 * including ancestor nodes.
 *
 * Since this matrix is not commonly used, and is expensive to compute, it is only
 * calculated when the globalTransformMatrix has changed, and then only on demand.
 * When the globalTransformMatrix is marked as dirty, the globalRotationMatrix is marked
 * as dirty as well. It is then recalculated the next time this property is accessed,
 * and is cached until it is marked dirty again.
 */
CC3Matrix* CC3Node::getGlobalRotationMatrix()
{
	if ( !m_globalRotationMatrix ) 
	{
		m_globalRotationMatrix = CC3LinearMatrix::matrix();		// retained
		m_globalRotationMatrix->retain();
		m_globalRotationMatrix->setIsDirty( true );
	}

	if ( m_globalRotationMatrix->isDirty() ) 
		buildGlobalRotationMatrix();
	
	return m_globalRotationMatrix;
}

void CC3Node::buildGlobalRotationMatrix()
{
	// Ensure main global transform matrix is updated as well. It is not needed here, but it
	// tracks whether the node's transform is dirty, and it and this matrix need to be in sync.
	getGlobalTransformMatrix();
	
	if ( m_pParent )
		m_globalRotationMatrix->populateFrom(  m_pParent->getGlobalRotationMatrix() );

	if ( m_rotator )
		m_globalRotationMatrix->multiplyBy( m_rotator->getRotationMatrix() );

	m_globalRotationMatrix->setIsDirty( false );
}


void CC3Node::setBoundingVolume( CC3NodeBoundingVolume * aBoundingVolume )
{
	if ( aBoundingVolume == m_pBoundingVolume ) 
		return;
	
	if ( m_pBoundingVolume && aBoundingVolume )
		aBoundingVolume->setShouldIgnoreRayIntersection( m_pBoundingVolume->shouldIgnoreRayIntersection() );
	
	CC_SAFE_RELEASE( m_pBoundingVolume );
	m_pBoundingVolume = aBoundingVolume;
	CC_SAFE_RETAIN( aBoundingVolume );
	
	if ( m_pBoundingVolume )
	{
		// If the bounding volume has not been assigned to a node yet, this node will be the primary
		// node of the bounding volume. Otherwise, this node is a secondary node, is tracking the
		// bounding volume, but should not influence the bounding volume characteristics.
		if ( !m_pBoundingVolume->getNode() ) 
		{
			m_pBoundingVolume->setNode( this );
			markBoundingVolumeDirty();
		}
		else
		{
			m_shouldUseFixedBoundingVolume = true;
		}
	}
}

CC3NodeBoundingVolume* CC3Node::getBoundingVolume()
{
	return m_pBoundingVolume;
}

void CC3Node::createBoundingVolume()
{
	if ( !m_pBoundingVolume ) 
		setBoundingVolume( defaultBoundingVolume() );
}

void CC3Node::createBoundingVolumes()
{
	createBoundingVolume();
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->createBoundingVolumes();
		}
	}
}

void CC3Node::deleteBoundingVolumes() 
{
	m_pBoundingVolume = NULL;

	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->deleteBoundingVolumes();
		}
	}
}

CC3NodeBoundingVolume* CC3Node::defaultBoundingVolume()
{ 
	return NULL; 
}

void CC3Node::setBoundingVolumePadding( GLfloat padding )
{
	m_fBoundingVolumePadding = padding;
}

GLfloat CC3Node::getBoundingVolumePadding()
{
	return m_fBoundingVolumePadding;
}

void CC3Node::markBoundingVolumeDirty()
{ 
	if ( !m_shouldUseFixedBoundingVolume ) 
	{
		if ( m_pBoundingVolume )
			m_pBoundingVolume->markDirty();
	}
}


bool CC3Node::shouldDrawBoundingVolume()
{ 
	return m_pBoundingVolume ? m_pBoundingVolume->shouldDraw() : false; 
}

void CC3Node::setShouldDrawBoundingVolume( bool shouldDraw )
{
	if ( shouldDraw ) 
		createBoundingVolume();

	if ( m_pBoundingVolume )
		m_pBoundingVolume->setShouldDraw( shouldDraw );
}

void CC3Node::draw()
{

}

void CC3Node::drawWithVisitor( CC3NodeDrawingVisitor* visitor )
{

}

bool CC3Node::doesIntersectFrustum( CC3Frustum* aFrustum )
{
	if ( !m_pBoundingVolume || !aFrustum ) 
		return true;

	return doesIntersectBoundingVolume( aFrustum );
}

void CC3Node::transformAndDrawWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	CC3OpenGL* gl = visitor->getGL();
	gl->pushModelviewMatrixStack();

	visitor->populateModelMatrixFrom( getGlobalTransformMatrix() );
	visitor->draw( this );

	gl->popModelviewMatrixStack();
}

void CC3Node::checkDrawingOrder() 
{ 
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->checkDrawingOrder();
		}
	}
}

bool CC3Node::shouldCastShadows() 
{ 
	return m_shouldCastShadows; 
}

void CC3Node::setShouldCastShadows( bool shouldCastShadows )
{
	m_shouldCastShadows = shouldCastShadows;
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->setShouldCastShadows( shouldCastShadows );
		}
	}
}

CCArray* CC3Node::getChildren()
{
	return m_pChildren;
}

CC3Node* CC3Node::getParent()
{
	return m_pParent;
}

/**
 * When assigned to a new parent, ensure that the transform will be recalculated,
 * since it changes this child's overall transform. Parent is weakly referenced.
 */
void CC3Node::setParent( CC3Node* aNode )
{
	if ( aNode == m_pParent ) 
		return;
	
	m_pParent = aNode;					// weak reference.
	if ( aNode )
		setIsRunning( aNode->isRunning() );

	markTransformDirty();
}

CC3Node* CC3Node::getRootAncestor()
{ 
	return m_pParent ? m_pParent->getRootAncestor() :this; 
}

bool CC3Node::isScene()
{
	return false;
}

CC3Scene* CC3Node::getScene()
{ 
	if ( m_pParent )
		return m_pParent->getScene(); 

	return NULL;
}

CC3Camera* CC3Node::getActiveCamera()
{
	CC3Scene* scene = getScene();
	if ( scene )
		return scene->getActiveCamera();

	return NULL;
}

/** 
 * If this action is occuring on a background thread, and this node is already part of the
 * scene being rendered, the operation is queued for execution on the rendering thread, to
 * avoid the possibility of adding a node in the middle of a render iteration.
 */
void CC3Node::addChild( CC3Node* aNode )
{
	// Don't add if child is nil or is already a child of this node
	CCAssert(aNode, "Child node cannot be nil");
	if ( aNode->getParent() == this ) 
		return;

	aNode->markAddBegin();	// Mark that this node is being added

	if ( !CC3OpenGL::sharedGL()->isRenderingContext() && getScene() )
		addChildFromBackgroundThread( aNode );
	else
		addChildNow( aNode );
}

/** Adds the specified node as a child of this node without queuing. */
void CC3Node::addChildNow( CC3Node* aNode )
{	
	aNode->remove();		// Remove node from its existing parent

	// Lazily create the children array, if needed, and add the node to it
	if ( !m_pChildren ) 
	{
		m_pChildren = CCArray::createWithCapacity( 4 );
		m_pChildren->retain();
	}	
	m_pChildren->addObject( aNode );

	aNode->setParent( this );
	didAddDescendant( aNode );
	aNode->markAddEnd();
	aNode->wasAdded();
}

/** 
 * Invoked when a child is being added on a background thread, and this parent node is
 * already part of the scene.
 *
 * Since the scene may be in the process of being rendered, the child is not added immediately.
 * Instead, all GL activity on this thread is allowed to finish, to ensure all GL components
 * of the node are in place, and then an operation to add the specified node as a child of
 * this node is queued to the thread that is performing rendering.
 */
void CC3Node::addChildFromBackgroundThread( CC3Node* aNode )
{
	/*[CC3OpenGL.sharedGL finish];
	[CC3OpenGL.renderThread runBlockAsync: ^{ [self addChildNow: aNode]; } ];*/
	
	// A better design would be to use dispatch queues, but OSX typically
	// renders using a DisplayLink thread instead of the main thread.
//	dispatch_async(dispatch_get_main_queue(), ^{ [self addChildNow: aNode]; });
}

/**
 * To transform location and rotation, we invert the matrix of this node, and multiply it by the
 * matrix of the child node. The incoming child's matrix is in global form. We want a local form
 * that will provide the local location and rotation. We can then extract local location, rotation,
 * and scale from the local matrix.
 *
 * Mathematically, if Mcg is the global matrix of the child node, Mpg is the
 * matrix of this parent, and Mcl is the desired local matrix, we have:
 *     Normally: Mcg = Mpg.Mcl
 * Multiplying both sides by  Mpg(-1), the inverse of the parent's matrix:
 *     Mpg(-1).Mcg = Mpg(-1).Mpg.Mcl
 *     Mcl = Mpg(-1).Mcg
 */
void CC3Node::addAndLocalizeChild( CC3Node* aNode )
{
	CC3Matrix4x3 g2LMtx;
	CC3Matrix3x3 g2LRotMtx;
	
	// Localize the child node's location by finding the right local matrix, and then translating
	// the child node's local origin by the resulting matrix. This is what the location property
	// does. It instructs the local matrix to move the node's origin. By transforming the origin,
	// we determine what that location property needs to be.
	getGlobalTransformMatrixInverted()->populateCC3Matrix4x3( &g2LMtx );
	aNode->getGlobalTransformMatrix()->multiplyIntoCC3Matrix4x3( &g2LMtx );
	CC3Vector4 nodeLoc4 = CC3Matrix4x3TransformCC3Vector4(&g2LMtx, CC3Vector4::kCC3Vector4ZeroLocation);
	aNode->setLocation( nodeLoc4.cc3Vector() );
	
	// Localize the child node's rotation by finding the right rotation matrix. For rotation, we use
	// the globalRotationMatrix, which is free of scale and translation content. Otherwise it would
	// be impossible to extract he local rotation from an arbitrarily scaled and translated matrix.
	getGlobalRotationMatrix()->populateCC3Matrix3x3( &g2LRotMtx );
	CC3Matrix3x3InvertRigid(&g2LRotMtx);	// Contains only rotation
	aNode->getGlobalRotationMatrix()->multiplyIntoCC3Matrix3x3( &g2LRotMtx );
	aNode->setRotation( CC3Matrix3x3ExtractRotationYXZ(&g2LRotMtx) );
	
	// Scale cannot readily be extracted from the inverted and multiplied matrix, but we can get
	// it by scaling the node's scale down by the globalScale of this parent, so that when they
	// are recombined, the original globalScale of the child node.
	aNode->setScale( aNode->getGlobalScale().scale( getGlobalScale().invert() ) );
	
	addChild( aNode );		// Finally, add the child node to this parent
}

/** 
 * Marks that this node is being added to a parent.
 *
 * Since the add operation can be transferred to another thread, its possible for this node
 * to be removed from its current parent before being added to another parent. By marking
 * the add operation as being in progress, the node can avoid standard removal activities
 * such as stopping all actions.
 */
void CC3Node::markAddBegin()
{ 
	m_isBeingAdded = true; 
}

/**
 * Marks that the operation of adding this node to a parent has finished.
 *
 * Since the add operation can be transferred to another thread, its possible for this node
 * to be removed from its current parent before being added to another parent. By marking
 * the add operation as being in progress, the node can avoid standard removal activities
 * such as stopping all actions.
 */
void CC3Node::markAddEnd() 
{ 
	m_isBeingAdded = false;
}

void CC3Node::wasAdded()
{

}

/**
 * Removes a child node and invokes didRemoveDescendant: so action can be taken by subclasses.
 * First locates the object to make sure it is in the child node collection, and only performs
 * the removal and related actions if the specified node really is a child of this node.
 * Also removes this node if the shouldAutoremoveWhenEmpty property is YES, and the last
 * child has just been removed.
 */
void CC3Node::removeChild( CC3Node* aNode )
{
	if ( !m_pChildren || !aNode ) 
		return;

	unsigned int indx = m_pChildren->indexOfObject( aNode );
	if (indx == CC_INVALID_INDEX) 
		return;

	// If the children collection is the only thing referencing the child node, the
	// child node will be deallocated as soon as it is removed, and will be invalid
	// when passed to the didRemoveDescendant: method, or to other activities that
	// it may be subject to in the processing loop. To avoid problems, retain it for
	// the duration of this processing loop, so that it will still be valid until
	// we're done with it.
	
	// If the children collection is the only thing referencing the child node, the child
	// node will be deallocated as soon as it is removed, and will be invalid when passed
	// to the didRemoveDescendant: method, or to other activities that it may be subject
	// to in the processing loop. To avoid problems, retain it for the duration of this
	// processing loop, so that it will still be valid until we're done with it.
	aNode->retain();
	aNode->autorelease();
	aNode->setParent( NULL );

	/// remove this child
	m_pChildren->removeObjectAtIndex( indx );
	if (m_pChildren->count() == 0) 
	{
		m_pChildren->release();
		m_pChildren = NULL;
	}

	aNode->wasRemoved();						// Invoke before didRemoveDesc notification
	didRemoveDescendant( aNode );
	
	// If the last child has been removed, and this instance should autoremove when
	// that occurs, remove this node from the hierarchy as well. This must be performed
	// after everything else is done, particularly only after the didRemoveDescendant:
	// has been invoked so notification can propagate up the node hierarchy.
	if (!m_pChildren && m_shouldAutoremoveWhenEmpty) 
		remove();
}

void CC3Node::removeAllChildren()
{
#if 0
	/*if ( m_pChildren )
	{
		CCArray* myKids = (CCArray*)m_pChildren->copy();
		CCObject* object;
		CCARRAY_FOREACH( myKids, object )
		{
			CC3Node* child = (CC3Node*)object;
			if ( child )
			{
				removeChild( child );
			}
		}
		myKids->release();
	}
	*/
#else
	while ( m_pChildren && m_pChildren->count() > 0 )
	{
		removeChild( (CC3Node*)m_pChildren->objectAtIndex( 0 ) );
	}
#endif
}

void CC3Node::remove()
{ 
	if ( m_pParent )
		m_pParent->removeChild( this );
}

void CC3Node::wasRemoved()
{
	if (m_shouldStopActionsWhenRemoved && !m_isBeingAdded) 
		stopAllActions();

	setIsRunning( false );
}

bool CC3Node::isDescendantOf( CC3Node* aNode )
{
	return (aNode == this) || (m_pParent && m_pParent->isDescendantOf( aNode ));
}

/**
 * Invoked automatically when a node is added as a child somewhere in the descendant structural
 * hierarchy of this node. The method is not only invoked on the immediate parent of the newly
 * added node, but is actually invoked on all ancestors as well (parents of the parent).
 * This default implementation simply passes the notification up the parental ancestor chain.
 * Subclasses may override to take a specific interest in which nodes are being added below them.
 */
void CC3Node::didAddDescendant( CC3Node* aNode )
{
	if ( m_pParent )
		m_pParent->didAddDescendant( aNode );
}

/**
 * Invoked automatically when a node is removed as a child somewhere in the descendant structural
 * hierarchy of this node. The method is not only invoked on the immediate parent of the removed
 * node, but is actually invoked on all ancestors as well (parents of the parent).
 * This default implementation simply passes the notification up the parental ancestor chain.
 * Subclasses may override to take a specific interest in which nodes are being removed below them.
 */
void CC3Node::didRemoveDescendant( CC3Node* aNode )
{
	if ( m_pParent )
		m_pParent->didRemoveDescendant( aNode );
}

/**
 * Invoked automatically when a property was modified on a descendant node that potentially
 * affects its drawing order, relative to other nodes. This default implementation simply
 * passes the notification up the parental ancestor chain. Subclasses may override to take
 * a specific interest in which nodes need resorting below them.
 */
void CC3Node::descendantDidModifySequencingCriteria( CC3Node* aNode )
{
	if ( m_pParent )
		m_pParent->descendantDidModifySequencingCriteria( aNode );
}

CC3Node* CC3Node::getNodeNamed( const char* aName )
{
	// First see if it's me
	if (m_sName.compare(aName) == 0 || (m_sName.empty() && !aName)) 
		return this;

	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			CC3Node* childResult = child->getNodeNamed( aName );
			if (childResult) 
				return childResult;
		}
	}
	
	return NULL;
}

CC3Node* CC3Node::getNodeTagged( GLuint aTag )
{
	if ( m_nTag == aTag ) 
		return this;

	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			CC3Node* childResult = child->getNodeTagged( aTag );
			if (childResult) 
				return childResult;
		}
	}

	return NULL;
}

CCArray* CC3Node::flatten()
{
	CCArray* allNodes = CCArray::create();
	flattenInto( allNodes );
	return allNodes;
}

void CC3Node::flattenInto( CCArray* anArray )
{
	anArray->addObject( this );
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->flattenInto( anArray );
		}
	}
}

CC3Node* CC3Node::asOrientingWrapper() 
{
	std::string sName = "";
	CC3String::stringWithFormat( sName, (char*)"%s-OW", m_sName.c_str() );
	CC3Node* wrap = CC3Node::nodeWithName( sName.c_str() );
	wrap->setShouldAutoremoveWhenEmpty( true );
	wrap->addChild( this );
	return wrap;
}

CC3Node* CC3Node::asTrackingWrapper()
{
	CC3Node* wrap = asOrientingWrapper();
	wrap->setShouldTrackTarget( true );
	return wrap;
}

CC3Node* CC3Node::asCameraTrackingWrapper()
{
	CC3Node* wrap = asOrientingWrapper();
	wrap->setShouldAutotargetCamera( true );
	return wrap;
}

CC3Node* CC3Node::asBumpMapLightTrackingWrapper()
{
	CC3Node* wrap = asTrackingWrapper();
	wrap->setIsTrackingForBumpMapping( true );
	return wrap;
}

bool CC3Node::shouldStopActionsWhenRemoved()
{
	return m_shouldStopActionsWhenRemoved;
}

void CC3Node::setShouldStopActionsWhenRemoved( bool stop )
{
	m_shouldStopActionsWhenRemoved = stop;
}

bool CC3Node::shouldAllowTouchableWhenInvisible()
{
	return m_shouldAllowTouchableWhenInvisible;
}

void CC3Node::setShouldAllowTouchableWhenInvisible( bool allow )
{
	m_shouldAllowTouchableWhenInvisible = allow;
}

bool CC3Node::shouldInheritTouchability()
{
	return m_shouldInheritTouchability;
}

void CC3Node::setShouldInheritTouchability( bool inherit )
{
	m_shouldInheritTouchability = inherit;
}

CC3Action* CC3Node::runAction( CC3Action* action )
{
	CCAssert( action != NULL, "Argument must be non-nil" );
	
	CC3ActionManager* pActionMgr = (CC3ActionManager*)CCDirector::sharedDirector()->getActionManager();
	pActionMgr->addAction( action, this, !m_isRunning );

	return action;
}

CC3Action* CC3Node::runAction( CC3Action* action, unsigned int aTag )
{
	stopActionByTag( aTag );
	action->setTag( aTag );
	return runAction( action );
}

void CC3Node::stopAllActions()
{
	CCDirector::sharedDirector()->getActionManager()->removeAllActionsFromTarget( this );
}

void CC3Node::stopAction( CC3Action* action )
{ 
	CCDirector::sharedDirector()->getActionManager()->removeAction( action );
}

void CC3Node::stopActionByTag( unsigned int aTag )
{
	CCAssert( aTag != kCCActionTagInvalid, "Invalid tag" );
	CCDirector::sharedDirector()->getActionManager()->removeActionByTag( aTag, this );
}

CCAction* CC3Node::getActionByTag( unsigned int aTag )
{
	CCAssert( aTag != kCCActionTagInvalid, "Invalid tag" );
	return CCDirector::sharedDirector()->getActionManager()->getActionByTag( aTag, this );
}

int CC3Node::numberOfRunningActions() 
{
	return CCDirector::sharedDirector()->getActionManager()->numberOfRunningActionsInTarget( this );
}

void CC3Node::resumeAllActions()
{
	CCDirector::sharedDirector()->getActionManager()->resumeTarget( this );
}

void CC3Node::pauseAllActions()
{ 
	CCDirector::sharedDirector()->getActionManager()->pauseTarget( this );
}

void CC3Node::cleanupActions()
{
	stopAllActions();
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->cleanupActions();
		}
	}
}

bool CC3Node::isTouchEnabled()
{ 
	return m_touchEnabled; 
}

void CC3Node::setTouchEnabled( bool canTouch )
{ 
	m_touchEnabled = canTouch; 
}

bool CC3Node::isTouchable()
{
	return (m_visible || m_shouldAllowTouchableWhenInvisible)
			&& (m_touchEnabled || ((m_pParent && m_shouldInheritTouchability) ? m_pParent->isTouchable() : false));
}

CC3Node* CC3Node::touchableNode()
{ 
	return m_touchEnabled ? this : (m_pParent ? m_pParent->touchableNode() : NULL); 
}

void CC3Node::touchEnableAll()
{
	m_touchEnabled = true;
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->touchEnableAll();
		}
	}
}

void CC3Node::touchDisableAll() 
{
	m_touchEnabled = false;
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->touchDisableAll();
		}
	}
}

bool CC3Node::shouldIgnoreRayIntersection()
{
	return m_pBoundingVolume ? m_pBoundingVolume->shouldIgnoreRayIntersection() : true;
}

void CC3Node::setShouldIgnoreRayIntersection( bool shouldIgnore )
{
	if ( m_pBoundingVolume )
		m_pBoundingVolume->setShouldIgnoreRayIntersection( shouldIgnore );
}

bool CC3Node::doesIntersectBoundingVolume( CC3BoundingVolume* otherBoundingVolume )
{
	return m_pBoundingVolume && m_pBoundingVolume->doesIntersect( otherBoundingVolume );
}

bool CC3Node::doesIntersectNode( CC3Node* otherNode )
{
	if ( otherNode )
		return doesIntersectBoundingVolume( otherNode->getBoundingVolume() );
	
	return false;
}

bool CC3Node::doesIntersectGlobalRay( const CC3Ray& aRay )
{
	return m_pBoundingVolume && m_pBoundingVolume->doesIntersectRay( aRay );
}

CC3Vector CC3Node::getLocationOfGlobalRayIntesection( const CC3Ray& aRay )
{
	if ( shouldIgnoreRayIntersection() ) 
		return CC3Vector::kCC3VectorNull;

	CC3Ray localRay = getGlobalTransformMatrixInverted()->transformRay( aRay );
	return m_pBoundingVolume->getLocationOfRayIntesection( localRay );
}

CC3Vector CC3Node::getGlobalLocationOfGlobalRayIntesection( const CC3Ray& aRay )
{
	if ( !m_pBoundingVolume ) 
		return CC3Vector::kCC3VectorNull;

	return m_pBoundingVolume->getGlobalLocationOfGlobalRayIntesection( aRay );
}

CC3NodePuncturingVisitor* CC3Node::nodesIntersectedByGlobalRay( const CC3Ray& aRay )
{
	CC3NodePuncturingVisitor* pnv = CC3NodePuncturingVisitor::visitorWithRay( aRay );
	pnv->visit( this );
	return pnv;
}

CC3Node* CC3Node::closestNodeIntersectedByGlobalRay( const CC3Ray& aRay )
{
	return nodesIntersectedByGlobalRay( aRay )->getClosestPuncturedNode();
}

/** Suffix used to name the descriptor child node. */
#define kDescriptorSuffix "DESC"

std::string CC3Node::getDescriptorName()
{
	std::string sName = "";
	CC3String::stringWithFormat( sName, (char*)"%s-%s", m_sName.c_str(), kDescriptorSuffix );
	return sName;
}

CC3NodeDescriptor* CC3Node::getDescriptorNode()
{
	std::string sName = getDescriptorName();
	return (CC3NodeDescriptor*)getNodeNamed( sName.c_str() );
}

CCColorRef CC3Node::getInitialDescriptorColor()
{ 
	return CCColorRefFromCCC4F(getInitialWireframeBoxColor()); 
}

bool CC3Node::shouldDrawDescriptor() 
{ 
	return getDescriptorNode() != NULL; 
}

void CC3Node::setShouldDrawDescriptor( bool shouldDraw )
{
	// Fetch the descriptor node from the child nodes.
	CC3NodeDescriptor* dn = getDescriptorNode();
	
	// If the descriptor node exists, but should not, remove it
	if ( dn && !shouldDraw )
		dn->remove();
	
	// If there is no descriptor node, but there should be, add it by creating a
	// CC3NodeDescriptor from the description property and add it as a child of this node.
	if ( !dn && shouldDraw )
	{
		CCLabelTTF* dnLabel = CCLabelTTF::create( description().c_str(), "Arial", descriptorFontSize() );
		dn = CC3NodeDescriptor::nodeWithName( getDescriptorName().c_str() , dnLabel );
		dn->setColor( getInitialDescriptorColor() );
		addChild( dn );
	}
}

bool CC3Node::shouldDrawAllDescriptors()
{
	if ( !shouldDrawDescriptor() ) 
		return false;
	
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child && !child->shouldDrawAllDescriptors() )
		{
			return false;
		}
	}

	return true;
}

void CC3Node::setShouldDrawAllDescriptors( bool shouldDraw )
{
	setShouldDrawDescriptor( shouldDraw );
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			setShouldDrawAllDescriptors( shouldDraw );
		}
	}
}

// Initial font size for any new descriptors
static float s_descriptorFontSize = 8.0f;

float CC3Node::descriptorFontSize()
{ 
	return s_descriptorFontSize; 
}

void CC3Node::setDescriptorFontSize( float fontSize )
{
	s_descriptorFontSize = fontSize; 
}

/** Suffix used to name the wireframe child node. */
#define kWireframeBoxSuffix "WFB"

/** The name to use when creating or retrieving the wireframe child node of this node. */
std::string CC3Node::getWireframeBoxName()
{
	std::string sName = "";
	CC3String::stringWithFormat( sName, (char*)"%s-%s", m_sName.c_str(), kWireframeBoxSuffix );
	return sName;
}

CC3WireframeBoundingBoxNode* CC3Node::getWireframeBoxNode()
{
	return (CC3WireframeBoundingBoxNode*)getNodeNamed( getWireframeBoxName().c_str() );
}

bool CC3Node::shouldDrawWireframeBox()
{ 
	return getWireframeBoxNode() != NULL;
}

void CC3Node::setShouldDrawWireframeBox( bool shouldDraw )
{
	// Fetch the wireframe node from the child nodes.
	CC3WireframeBoundingBoxNode* wf = getWireframeBoxNode();
	
	// If the wireframe exists, but should not, remove it
	if ( wf && !shouldDraw ) 
		wf->remove();
	
	// If there is no wireframe, but there should be, add it by creating a
	// CC3WireframeBoundingBoxNode from the boundingBox property and add it as a
	// child of this node. If the bounding box is null, don't create a wireframe.
	// The bounding box is set to update its vertices from the bounding box of
	// this node on each update pass to allow the wireframe to grow and shrink
	// along with the bounding box of this node and its descendants
	if ( !wf && shouldDraw ) 
	{
		CC3Box bb = getBoundingBox();
		if ( !bb.isNull() )
		{
			wf = CC3WireframeBoundingBoxNode::nodeWithName( getWireframeBoxName().c_str() );
			wf->populateAsWireBox( bb );
			wf->setEmissionColor( getInitialWireframeBoxColor() );
			wf->setShouldAlwaysMeasureParentBoundingBox( true );
			addChild( wf );
		}
	}
}

/** If default is transparent black, use the color of the node. */
ccColor4F CC3Node::getInitialWireframeBoxColor()
{
	ccColor4F defaultColor = getWireframeBoxColor();
	return CCC4FAreEqual(defaultColor, kCCC4FBlackTransparent)
				? CCC4FFromCCColorRef(getColor())
				: defaultColor;
}

// The default color to use when drawing the wireframes
static ccColor4F s_wireframeBoxColor = { 1.0, 1.0, 0.0, 1.0 };	// kCCC4FYellow

ccColor4F CC3Node::getWireframeBoxColor()
{ 
	return s_wireframeBoxColor; 
}

void CC3Node::setWireframeBoxColor( const ccColor4F& aColor )
{ 
	s_wireframeBoxColor = aColor; 
}

bool CC3Node::shouldDrawAllWireframeBoxes()
{
	if ( !shouldDrawWireframeBox() ) 
		return false;

	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child && !child->shouldDrawAllWireframeBoxes() )
		{
			return false;
		}
	}

	return true;
}

void CC3Node::setShouldDrawAllWireframeBoxes( bool shouldDraw )
{
	setShouldDrawWireframeBox( shouldDraw ); 

	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->setShouldDrawAllWireframeBoxes( shouldDraw );
		}
	}
}

bool CC3Node::shouldDrawAllLocalContentWireframeBoxes()
{
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child && !child->shouldDrawAllLocalContentWireframeBoxes() )
		{
			return false;
		}
	}

	return true;
}

void CC3Node::setShouldDrawAllLocalContentWireframeBoxes( bool shouldDraw )
{
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->setShouldDrawAllLocalContentWireframeBoxes( shouldDraw );
		}
	}
}

void CC3Node::addDirectionMarkerColored( const ccColor4F& aColor, const CC3Vector& aDirection )
{
	std::string dmName = CC3String::stringWithFormat( (char*)"%s-DM-%s", m_sName.c_str(), aDirection.stringfy().c_str() );
	CC3DirectionMarkerNode* dm = CC3DirectionMarkerNode::nodeWithName( dmName.c_str() );

	CC3Vector lineVertices[2] = { CC3Vector::kCC3VectorZero, CC3Vector::kCC3VectorZero };
	dm->populateAsLineStripWith( 2, lineVertices, true );

	dm->setMarkerDirection( aDirection );
	dm->setLineWidth( 2.0f );
	dm->setEmissionColor( aColor );
	addChild( dm );
}

void CC3Node::addDirectionMarker()
{
	addDirectionMarkerColored( getDirectionMarkerColor(), CC3Vector::kCC3VectorUnitZNegative );
}

void CC3Node::addAxesDirectionMarkers() 
{
	addDirectionMarkerColored( kCCC4FRed, CC3Vector::kCC3VectorUnitXPositive );
	addDirectionMarkerColored( kCCC4FGreen, CC3Vector::kCC3VectorUnitYPositive );
	addDirectionMarkerColored( kCCC4FBlue, CC3Vector::kCC3VectorUnitZPositive );
}

void CC3Node::removeAllDirectionMarkers()
{
	CCArray* dirMks = getDirectionMarkers();
	CCObject* object = NULL;
	CCARRAY_FOREACH( dirMks, object )
	{
		CC3Node* dm = (CC3Node*)object;
		if ( dm )
		{
			dm->remove();
		}
	}

	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->removeAllDirectionMarkers();
		}
	}
}

CCArray* CC3Node::getDirectionMarkers()
{
	CCArray* dirMks = CCArray::create();
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;

		//if ( child && child->getNodeType() == KCC3DirectionMarkerNode )
		//{
		//	dirMks->addObject( child );
		//}
#pragma _NOTE_TODO( "CC3Node RTTI" )
	}

	return dirMks;
}

/** If default is transparent black, use the color of the node. */
ccColor4F CC3Node::getInitialDirectionMarkerColor()
{
	ccColor4F defaultColor = getDirectionMarkerColor();
	return CCC4FAreEqual(defaultColor, kCCC4FBlackTransparent)
				? CCC4FFromCCColorRef(getColor())
				: defaultColor;
}

// The default color to use when drawing the direction markers
static ccColor4F s_directionMarkerColor = { 1.0, 0.0, 0.0, 1.0 };		// kCCC4FRed

ccColor4F CC3Node::getDirectionMarkerColor() 
{ 
	return s_directionMarkerColor; 
}

void CC3Node::setDirectionMarkerColor( const ccColor4F& aColor )
{
	s_directionMarkerColor = aColor; 
}

bool CC3Node::shouldContributeToParentBoundingBox() 
{
	return false; 
}

bool CC3Node::shouldDrawAllBoundingVolumes()
{
	if ( shouldDrawBoundingVolume() ) 
		return true;

	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child && child->shouldDrawAllBoundingVolumes() )
		{
			return true;
		}
	}

	return false;
}

void CC3Node:: setShouldDrawAllBoundingVolumes( bool shouldDraw )
{
	setShouldDrawBoundingVolume( shouldDraw );
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->setShouldDrawAllBoundingVolumes( shouldDraw );
		}
	}
}

bool CC3Node::shouldLogIntersections()
{
	if ( m_pBoundingVolume && m_pBoundingVolume->shouldLogIntersections() ) 
		return true;

	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child && child->shouldLogIntersections() )
		{
			return true;
		}
	}

	return false;
}

void CC3Node::setShouldLogIntersections( bool shouldLog )
{
	if ( shouldLog ) 
		createBoundingVolume();
	m_pBoundingVolume->setShouldLogIntersections( shouldLog );

	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->setShouldLogIntersections( shouldLog );
		}
	}
}

bool CC3Node::shouldLogIntersectionMisses()
{
	if ( m_pBoundingVolume && m_pBoundingVolume->shouldLogIntersectionMisses() ) 
		return true;

	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child && child->shouldLogIntersections() )
		{
			return true;
		}
	}

	return false;
}

void CC3Node::setShouldLogIntersectionMisses( bool shouldLog )
{
	if ( shouldLog ) 
		createBoundingVolume();

	m_pBoundingVolume->setShouldLogIntersectionMisses( shouldLog );

	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->setShouldLogIntersectionMisses( shouldLog );
		}
	}
}

std::string CC3Node::getRenderStreamGroupMarker()
{ 
	return ""; 
}

CC3Vector CC3Node::getSkeletalScale()
{
	return m_pParent ? m_pParent->getSkeletalScale().scale( m_scale ) : m_scale; 
}

void CC3Node::bindRestPose()
{
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->bindRestPose();
		}
	}
}

void CC3Node::reattachBonesFrom( CC3Node* aNode )
{
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->reattachBonesFrom( aNode );
		}
	}
}

bool CC3Node::hasSoftBodyContent()
{
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child && child->hasSoftBodyContent() )
			return true;
	}
	
	return false;
}

void CC3Node::ensureRigidSkeleton()
{ 
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->ensureRigidSkeleton();
		}
	}
}

CC3SoftBodyNode* CC3Node::getSoftBodyNode()
{
	return m_pParent->getSoftBodyNode(); 
}

void CC3Node::createSkinnedBoundingVolumes()
{
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->createSkinnedBoundingVolumes();
		}
	}
}

void CC3Node::setSkeletalBoundingVolume( CC3NodeBoundingVolume* boundingVolume )
{
	CCObject* object;
	CCARRAY_FOREACH( m_pChildren, object )
	{
		CC3Node* child = (CC3Node*)object;
		if ( child )
		{
			child->setSkeletalBoundingVolume( boundingVolume );
		}
	}
}

bool CC3Node::isLightProbe()
{
	return false;
}

bool CC3Node::isBillboard()
{
	return false;
}

bool CC3Node::isMeshNode()
{
	return false;
}

void CC3Node::addAnimationInResource( CC3NodesResource* rez, GLuint trackID )
{
	CC3Node* matchingRezNode = rez->getNodeMatching( this );
	if ( matchingRezNode ) 
		addAnimation( matchingRezNode->getAnimation(), trackID );

	CCObject* pObj = NULL;
	CCARRAY_FOREACH( getChildren(), pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		child->addAnimationInResource( rez, trackID );
	}
}

GLuint CC3Node::addAnimationInResource( CC3NodesResource* rez )
{
	GLuint trackID = CC3NodeAnimationState::generateTrackID();
	addAnimationInResource( rez, trackID );
	return trackID;
}

CC3NodeAnimationState* CC3Node::getAnimationStateOnTrack( GLuint trackID )
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pAnimationStates, pObj )
	{
		CC3NodeAnimationState* as = (CC3NodeAnimationState*)pObj;
		if ( as->getTrackID() == trackID )
			return as;
	}
	
	return NULL;
}

void CC3Node::addAnimationState( CC3NodeAnimationState* animationState )
{
	CC3NodeAnimationState* currAnim = getAnimationStateOnTrack( animationState->getTrackID() );
	if ( !animationState || animationState == currAnim )
		return;		// leave if not changing

	if ( !m_pAnimationStates ) 
	{
		m_pAnimationStates = CCArray::create();		// ensure array exists - retained
		m_pAnimationStates->retain();
	}

	m_pAnimationStates->removeObject( currAnim );							// remove existing
	m_pAnimationStates->addObject( animationState );						// add to array
}

void CC3Node::removeAnimationState( CC3NodeAnimationState* animationState )
{
	m_pAnimationStates->removeObject(animationState);
	if ( m_pAnimationStates->count() == 0 ) 
	{
		m_pAnimationStates->release();
		m_pAnimationStates = NULL;
	}
}

CC3NodeAnimationState* CC3Node::getAnimationState()
{
	return getAnimationStateOnTrack( 0 ); 
}

CC3NodeAnimation* CC3Node::getAnimationOnTrack( GLuint trackID )
{
	CC3NodeAnimationState* pState = getAnimationStateOnTrack( trackID );
	if ( pState )
		return pState->getAnimation();

	return NULL;
}

void CC3Node::addAnimation( CC3NodeAnimation* animation, GLuint trackID )
{
	CC3NodeAnimation* currAnim = getAnimationOnTrack(trackID);
	if ( !animation || animation == currAnim) 
		return;		// leave if not changing

	addAnimationState( CC3NodeAnimationState::animationStateWithAnimation( animation, trackID, this ) );
}

GLuint CC3Node::addAnimationFrom( float startTime, float endTime )
{
	return addAnimationFrom( startTime, endTime, 0 );
}

GLuint CC3Node::addAnimationFrom( float startTime, float endTime, GLuint baseTrackID )
{
	GLuint trackID = CC3NodeAnimationState::generateTrackID();
	addAnimationFrom( startTime, endTime, baseTrackID, trackID );
	return trackID;
}

void CC3Node::addAnimationFrom( float startTime, float endTime, GLuint baseTrackID, GLuint trackID )
{
	// Retrieve the base animation, and contruct a partial animation on it
	CC3NodeAnimation* baseAnim = getAnimationOnTrack( baseTrackID );
	if ( baseAnim ) 
	{
		addAnimation( CC3NodeAnimationSegment::animationOnAnimation( baseAnim, startTime, endTime ), trackID );
	}

	// Propagate to children
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( getChildren(), pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		child->addAnimationFrom( startTime, endTime, baseTrackID, trackID );
	}
}

GLuint CC3Node::addAnimationFromFrame( GLuint startFrameIndex, GLuint endFrameIndex )
{
	return addAnimationFromFrameWithBaseTrackID( startFrameIndex, endFrameIndex, 0 );
}

GLuint CC3Node::addAnimationFromFrameWithBaseTrackID( GLuint startFrameIndex, GLuint endFrameIndex, GLuint baseTrackID )
{
	GLuint trackID = CC3NodeAnimationState::generateTrackID();
	addAnimationFromFrame( startFrameIndex, endFrameIndex, baseTrackID, trackID );

	return trackID;
}

void CC3Node::addAnimationFromFrameWithTrackID( GLuint startFrameIndex, GLuint endFrameIndex, GLuint trackID )
{
	addAnimationFromFrame( startFrameIndex, endFrameIndex, 0, trackID );
}

void CC3Node::addAnimationFromFrame( GLuint startFrameIndex, GLuint endFrameIndex, GLuint baseTrackID, GLuint trackID )
{
	// Retrieve the base animation, and contruct a partial animation on it
	CC3NodeAnimation* baseAnim = getAnimationOnTrack( baseTrackID );
	if ( baseAnim ) 
	{
		addAnimation( CC3NodeAnimationSegment::animationOnAnimation(baseAnim, startFrameIndex, endFrameIndex), trackID );
	}

	// Propagate to children
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( getChildren(), pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		child->addAnimationFromFrame( startFrameIndex, endFrameIndex, baseTrackID, trackID );
	}
}

CC3NodeAnimationState* CC3Node::getAnimationStateForAnimation( CC3NodeAnimation* animation )
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pAnimationStates, pObj )
	{
		CC3NodeAnimationState* as = (CC3NodeAnimationState*)pObj;
		if ( as->getAnimation() == animation )
			return as;
	}
	
	return NULL;
}

void CC3Node::removeAnimation( CC3NodeAnimation* animation )
{
	removeAnimationState( getAnimationStateForAnimation(animation) );
}

void CC3Node::removeAnimationTrack( GLuint trackID )
{
	removeAnimationState( getAnimationStateOnTrack(trackID) );

	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		child->removeAnimationTrack( trackID );
	}
}

CC3NodeAnimation* CC3Node::getAnimation()
{
	return getAnimationOnTrack( 0 ); 
}

void CC3Node::setAnimation( CC3NodeAnimation* animation )
{
	addAnimation( animation, 0 ); 
}

bool CC3Node::containsAnimationOnTrack( GLuint trackID )
{
	if ( getAnimationStateOnTrack(trackID) != NULL ) 
		return true;

	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		if ( child->containsAnimationOnTrack( trackID ) )
			return true;
	}
	
	return false;
}

bool CC3Node::containsAnimation()
{
	if ( m_pAnimationStates && m_pAnimationStates->count() > 0 ) 
		return true;

	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		if ( child->containsAnimation() )
			return true;
	}

	return false;
}

float CC3Node::animationTimeOnTrack( GLuint trackID )
{
	CC3NodeAnimationState* as = getAnimationStateOnTrack( trackID );
	if ( as ) 
		return as->getAnimationTime();

	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		float animTime = child->animationTimeOnTrack( trackID );
		if ( animTime > 0.f )
			return animTime;
	}

	return 0.0f;
}

GLfloat CC3Node::animationBlendingWeightOnTrack( GLuint trackID )
{
	CC3NodeAnimationState* as = getAnimationStateOnTrack( trackID );
	if ( as ) 
		return as->getBlendingWeight();

	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		float animBlend = child->animationBlendingWeightOnTrack( trackID );
		if ( animBlend > 0.f )
			return animBlend;
	}
	
	return 0.0f;
}

void CC3Node::setAnimationBlendingWeight( GLfloat blendWeight, GLuint trackID )
{
	CC3NodeAnimationState* as = getAnimationStateOnTrack( trackID );
	if ( as )
		as->setBlendingWeight( blendWeight );

	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		child->setAnimationBlendingWeight( blendWeight, trackID );
	}
}

void CC3Node::freezeIfInanimateOnTrack( GLuint trackID )
{
	/*CC3NodeAnimation* anim = getAnimationOnTrack( trackID );
	if ( [anim isKindOfClass: CC3FrozenNodeAnimation.class] )
		[((CC3FrozenNodeAnimation*)anim) populateFromNodeState: self];
	else if ( !anim )
		[self addAnimation: [CC3FrozenNodeAnimation animationFromNodeState: self] asTrack: trackID];*/
}

void CC3Node::freezeAllInanimatesOnTrack( GLuint trackID )
{
	freezeIfInanimateOnTrack( trackID );

	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		child->freezeAllInanimatesOnTrack( trackID );
	}
}

void CC3Node::enableAnimation()
{
	setIsAnimationEnabled( true ); 
}

void CC3Node::disableAnimation()
{
	setIsAnimationEnabled( false ); 
}

bool CC3Node::isAnimationEnabled()
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pAnimationStates, pObj )
	{
		CC3NodeAnimationState* as = (CC3NodeAnimationState*)pObj;
		if ( as->isEnabled() )
			return true;
	}
 
	return false;
}

void CC3Node::setIsAnimationEnabled( bool _isAnimationEnabled )
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pAnimationStates, pObj )
	{
		CC3NodeAnimationState* as = (CC3NodeAnimationState*)pObj;
		as->setIsEnabled( _isAnimationEnabled ); 
	}
}

void CC3Node::enableAnimationOnTrack( GLuint trackID )
{
	getAnimationStateOnTrack(trackID)->enable();
}

void CC3Node::disableAnimationOnTrack( GLuint trackID )
{
	getAnimationStateOnTrack(trackID)->disable();
}

bool CC3Node::isAnimationEnabledOnTrack( GLuint trackID )
{
	CC3NodeAnimationState* as = getAnimationStateOnTrack( trackID );
	return as ? as->isEnabled() : false;
}
void CC3Node::enableAllAnimationOnTrack( GLuint trackID )
{
	enableAnimationOnTrack( trackID );

	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		child->enableAllAnimationOnTrack( trackID );
	}
}

void CC3Node::disableAllAnimationOnTrack( GLuint trackID )
{
	disableAnimationOnTrack( trackID );

	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		child->disableAllAnimationOnTrack( trackID );
	}
}

void CC3Node::enableAllAnimation()
{
	enableAnimation();
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		child->enableAllAnimation();
	}
}

void CC3Node::disableAllAnimation()
{
	disableAnimation();
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		child->disableAllAnimation();
	}
}

void CC3Node::enableLocationAnimation()
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pAnimationStates, pObj )
	{
		CC3NodeAnimationState* as = (CC3NodeAnimationState*)pObj;
		as->setIsLocationAnimationEnabled( true );
	}
}

void CC3Node::disableLocationAnimation()
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pAnimationStates, pObj )
	{
		CC3NodeAnimationState* as = (CC3NodeAnimationState*)pObj;
		as->setIsLocationAnimationEnabled( false );
	}
}

void CC3Node::enableQuaternionAnimation()
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pAnimationStates, pObj )
	{
		CC3NodeAnimationState* as = (CC3NodeAnimationState*)pObj;
		as->setIsQuaternionAnimationEnabled( true );
	}
}

void CC3Node::disableQuaternionAnimation()
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pAnimationStates, pObj )
	{
		CC3NodeAnimationState* as = (CC3NodeAnimationState*)pObj;
		as->setIsQuaternionAnimationEnabled( false );
	}
}

void CC3Node::enableScaleAnimation()
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pAnimationStates, pObj )
	{
		CC3NodeAnimationState* as = (CC3NodeAnimationState*)pObj;
		as->setIsScaleAnimationEnabled( true );
	}
}

void CC3Node::disableScaleAnimation()
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pAnimationStates, pObj )
	{
		CC3NodeAnimationState* as = (CC3NodeAnimationState*)pObj;
		as->setIsScaleAnimationEnabled( false );
	}
}

void CC3Node::enableAllLocationAnimation()
{
	enableLocationAnimation();
	
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		child->enableAllLocationAnimation();
	}
}

void CC3Node::disableAllLocationAnimation() 
{
	disableLocationAnimation();
	
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		child->enableAllLocationAnimation();
	}
}

void CC3Node::enableAllQuaternionAnimation()
{
	enableQuaternionAnimation();
	
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		child->enableAllQuaternionAnimation();
	}
}

void CC3Node::disableAllQuaternionAnimation()
{
	disableQuaternionAnimation();
	
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		child->disableAllQuaternionAnimation();
	}
}

void CC3Node::enableAllScaleAnimation()
{
	enableScaleAnimation();
	
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		child->enableAllScaleAnimation();
	}
}

void CC3Node::disableAllScaleAnimation()
{
	disableScaleAnimation();
	
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		child->disableAllScaleAnimation();
	}
}

void CC3Node::markAnimationDirty()
{
	m_isAnimationDirty = true; 
}

void CC3Node::establishAnimationFrameAt( float t, GLuint trackID )
{
	CC3NodeAnimationState* pAnimState = getAnimationStateOnTrack(trackID);
	if ( pAnimState )
		pAnimState->establishFrameAt( t );

	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		child->establishAnimationFrameAt( t, trackID );
	}
}

/** Updates this node from a blending of any contained animation. */
void CC3Node::updateFromAnimationState()
{
	if ( !m_isAnimationDirty ) 
		return;

	// Start with identity transforms
	CC3Vector blendedLoc = CC3Vector::kCC3VectorZero;
	CC3Vector blendedRot = CC3Vector::kCC3VectorZero;
	CC3Quaternion blendedQuat = CC3Quaternion::kCC3QuaternionIdentity;
	CC3Vector blendedScale = CC3Vector::kCC3VectorUnitCube;

	// Accumulated weights
	GLfloat totWtL = 0.0f;		// Accumulated location weight
	GLfloat totWtR = 0.0f;		// Accumulated rotation weight
	GLfloat totWtQ = 0.0f;		// Accumulated quaternion weight
	GLfloat totWtS = 0.0f;		// Accumulated scale weight

	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pAnimationStates, pObj )
	{
		CC3NodeAnimationState* as = (CC3NodeAnimationState*)pObj;
		GLfloat currWt = as->getBlendingWeight();
		if (currWt && as->isEnabled()) 
		{	
			// Don't blend if disabled or zero weight
			// Blend the location
			if ( as->isAnimatingLocation() ) 
			{
				totWtL += currWt;
				blendedLoc = blendedLoc.lerp( as->getLocation(), (currWt / totWtL) );
			}

			// Blend the quaternion
			if ( as->isAnimatingQuaternion() )
			{
				totWtQ += currWt;
				blendedQuat = blendedQuat.slerp(as->getQuaternion(), (currWt / totWtQ));
			}

			// Blend the scale
			if ( as->isAnimatingScale() ) 
			{
				totWtS += currWt;
				blendedScale = blendedScale.lerp( as->getScale(), (currWt / totWtS) );
			}
		}
	}

	if ( totWtL ) 
		setLocation( blendedLoc );
	
	if ( totWtR ) 
		setRotation( blendedRot );
	
	if ( totWtQ ) 
		setQuaternion( blendedQuat );
	
	if ( totWtS ) 
		setScale( blendedScale );

	m_isAnimationDirty = false;
}

std::string CC3Node::describeCurrentAnimationState()
{
	std::string desc = "";

	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pAnimationStates, pObj )
	{
		CC3NodeAnimationState* as = (CC3NodeAnimationState*)pObj;
		desc += CC3String::stringWithFormat( (char*)"\n%s", as->describeCurrentState().c_str() );
	}

	return desc;
}

std::string CC3Node::describeAnimationStateForFrames( GLuint frameCount, float startTime, float endTime )
{
	std::string desc = "";
	
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pAnimationStates, pObj )
	{
		CC3NodeAnimationState* as = (CC3NodeAnimationState*)pObj;
		desc += CC3String::stringWithFormat( (char*)"\n%s", as->describeStateForFrames( frameCount, startTime, endTime ).c_str() );
	}

	return desc;
}

std::string CC3Node::describeAnimationStateForFrames( GLuint frameCount )
{
	return describeAnimationStateForFrames( frameCount, 0.0f, 1.0f );
} 

bool CC3Node::isLight()
{
	return false;
}

bool CC3Node::isShadowVolume()
{
	return false;
}

CC3Node* CC3Node::getTouchableNode()
{
	return this->m_touchEnabled ? this : (getParent() ? getParent()->getTouchableNode() : NULL);
}

void CC3Node::addShadowVolumes()
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( getScene()->getLights(), pObj )
	{
		CC3Light* lt = (CC3Light*)pObj;
		addShadowVolumesForLight( lt );
	}
}

void CC3Node::addShadowVolumesForLight( CC3Light* aLight )
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		child->addShadowVolumesForLight( aLight );
	}
}

CCArray* CC3Node::getShadowVolumes()
{
	CCArray* svs = CCArray::create();
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		if ( child->isShadowVolume() )
			svs->addObject( child );
	}
	
	return svs;
}

CC3ShadowVolumeMeshNode* CC3Node::getShadowVolumeForLight( CC3Light* aLight )
{
	CCObject* pObj = NULL;
    CCArray* shadows = getShadowVolumes();
	CCARRAY_FOREACH( shadows, pObj )
	{
		CC3ShadowVolumeMeshNode* sv = (CC3ShadowVolumeMeshNode*)pObj;
		 if (sv->getLight() == aLight) 
			 return sv;
	}

	return NULL;
}

bool CC3Node::hasShadowVolumesForLight( CC3Light* aLight )
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		if ( child->hasShadowVolumesForLight( aLight ) )
			return true;
	}
	
	return false;
}

bool CC3Node::hasShadowVolumes()
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		if ( child->hasShadowVolumes() )
			return true;
	}

	return false;
}

void CC3Node::removeShadowVolumesForLight( CC3Light* aLight )
{
    CC3Node* pShadow = getShadowVolumeForLight( aLight );
    if ( pShadow )
        pShadow->remove();

	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		child->removeShadowVolumesForLight( aLight );
	}
}

void CC3Node::removeShadowVolumes()
{
	CCObject* pObj = NULL;
    CCArray* shadows = getShadowVolumes();
	CCARRAY_FOREACH( shadows, pObj )
	{
		CC3Node* sv = (CC3Node*)pObj;
		sv->remove();
	}
	
	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		child->removeShadowVolumes();
	}
}

bool CC3Node::shouldShadowFrontFaces()
{
	CCObject* pObj = NULL;
	
	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		if ( !child->shouldShadowFrontFaces() )
			return false;
	}
	
	return true;
}

void CC3Node::setShouldShadowFrontFaces( bool shouldShadow )
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		child->setShouldShadowFrontFaces( shouldShadow );
	}
}

bool CC3Node::shouldShadowBackFaces()
{
	CCObject* pObj = NULL;

	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		if ( !child->shouldShadowBackFaces() )
			return true;
	}
	
	return false;
}

void CC3Node::setShouldShadowBackFaces( bool shouldShadow )
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		child->setShouldShadowBackFaces( shouldShadow );
	}
}

GLfloat CC3Node::getShadowOffsetFactor()
{
	CCObject* pObj = NULL;

	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		GLfloat sf = child->getShadowOffsetFactor();
		if ( sf > 0.f )
			return sf;
	}

	return 0.0f;
}

void CC3Node::setShadowOffsetFactor( GLfloat factor )
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		child->setShadowOffsetFactor( factor );
	}
}

GLfloat CC3Node::getShadowOffsetUnits()
{
	CCObject* pObj = NULL;

	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		GLfloat su = child->getShadowOffsetUnits();
		if ( su > 0.f )
			return su;
	}

	return 0.0f;
}

void CC3Node::setShadowOffsetUnits( GLfloat units )
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		child->setShadowOffsetUnits( units );
	}
}

GLfloat CC3Node::getShadowVolumeVertexOffsetFactor()
{
	CCObject* pObj = NULL;

	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		GLfloat svf = child->getShadowVolumeVertexOffsetFactor();
		if ( svf > 0.f )
			return svf;
	}
	
	return 0.0f;
}

void CC3Node::setShadowVolumeVertexOffsetFactor( GLfloat voFactor )
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		child->setShadowVolumeVertexOffsetFactor( voFactor );
	}
}

GLfloat CC3Node::getShadowExpansionLimitFactor()
{
	CCObject* pObj = NULL;

	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		GLfloat svf = child->getShadowExpansionLimitFactor();
		if ( svf > 0.f )
			return svf;
	}

	return 0.0f;
}

void CC3Node::setShadowExpansionLimitFactor( GLfloat limFactor )
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		child->setShadowExpansionLimitFactor( limFactor );
	}
}

GLushort CC3Node::getShadowLagFactor()
{
	CCObject* pObj = NULL;

	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		GLushort slf = child->getShadowLagFactor();
		if ( slf > 1 )
			return slf;
	}
	
	return 1;
}

/**
 * After setting the lag factor in all descendants, pick a random lag count
 * to start counting from. The same lag factor value and the same lag count
 * value will be set in all descendants.
 */
void CC3Node::setShadowLagFactor( GLushort lagFactor )
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		child->setShadowLagFactor( lagFactor );
	}

	setShadowLagCount( CC3RandomUIntBelow(lagFactor) + 1 );
}

GLushort CC3Node::getShadowLagCount()
{
	CCObject* pObj = NULL;

	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		GLushort slc = child->getShadowLagCount();
		if ( slc > 0 )
			return slc;
	}
	
	return 0;
}

void CC3Node::setShadowLagCount( GLushort lagCount )
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		child->setShadowLagCount( lagCount );
	}
}

bool CC3Node::shouldAddShadowVolumeEndCapsOnlyWhenNeeded()
{
	CCObject* pObj = NULL;

	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		if ( !child->shouldAddShadowVolumeEndCapsOnlyWhenNeeded() )
			return false;
	}
	
	return true;
}

void CC3Node::setShouldAddShadowVolumeEndCapsOnlyWhenNeeded( bool onlyWhenNeeded )
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		child->setShouldAddShadowVolumeEndCapsOnlyWhenNeeded( onlyWhenNeeded );
	}
}

void CC3Node::prewarmForShadowVolumes()
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( m_pChildren, pObj )
	{
		CC3Node* child = (CC3Node*)pObj;
		child->prewarmForShadowVolumes();
	}
}

bool CC3Node::isShadowVisible()
{
	return false; 
}

bool CC3Node::shouldAutoremoveWhenEmpty()
{
	return m_shouldAutoremoveWhenEmpty;
}

void CC3Node::setShouldAutoremoveWhenEmpty( bool shouldAuto )
{
	m_shouldAutoremoveWhenEmpty = shouldAuto;
}

std::string CC3Node::description()
{
	return "";
}

NS_COCOS3D_END
