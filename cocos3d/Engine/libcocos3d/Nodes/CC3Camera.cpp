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
/** The maximum allowed effective field of view. */
#define kMaxEffectiveFOV 179.9

bool isDeviceOrientationLandscape( CC3UIInterfaceOrientation orientation )
{
	return ( orientation == CC3UIInterfaceOrientationLandscapeLeft || 
		orientation == CC3UIInterfaceOrientationLandscapeRight );
}

bool isDeviceOrientationPortrait( CC3UIInterfaceOrientation orientation )
{
	return ( orientation == CC3UIInterfaceOrientationPortrait || 
		orientation == CC3UIInterfaceOrientationPortraitUpsideDown );
}

CC3Camera::CC3Camera()
{
	m_frustum = NULL;
	m_fieldOfViewAspectOrientation = CC3UIInterfaceOrientationUndifined;
}

CC3Camera::~CC3Camera()
{
	CC_SAFE_RELEASE( m_frustum );
}

bool CC3Camera::isCamera()
{ 
	return true; 
}

/** Overridden to return NO so that the forwardDirection aligns with the negative-Z-axis. */
bool CC3Camera::shouldReverseForwardDirection()
{ 
	return false; 
}

bool CC3Camera::shouldClipToViewport()
{
	return m_shouldClipToViewport;
}

void CC3Camera::setShouldClipToViewport( bool shouldClip )
{
	m_shouldClipToViewport = shouldClip;
}

CC3Frustum* CC3Camera::getFrustum()
{
	buildProjection();
	return m_frustum;
}

/** Establish backpointer from frustum. */
void CC3Camera::setFrustum( CC3Frustum* frustum )
{
	if (frustum == m_frustum) 
		return;
	
	CC_SAFE_RELEASE( m_frustum );
	m_frustum = frustum;
	CC_SAFE_RETAIN( frustum );

	if ( m_frustum )
		m_frustum->setCamera( this );
}

CC3Matrix* CC3Camera::getProjectionMatrix()
{
	buildProjection();
	return m_hasInfiniteDepthOfField
				? m_frustum->getInfiniteProjectionMatrix()
				: m_frustum->getFiniteProjectionMatrix();
}

GLfloat CC3Camera::getFieldOfView()
{
	return m_fieldOfView; 
}

void CC3Camera::setFieldOfView( GLfloat anAngle )
{
	if (anAngle == m_fieldOfView) 
		return;

	m_fieldOfView = anAngle;

	markProjectionDirty();
}

GLfloat CC3Camera::getEffectiveFieldOfView()
{
	return (GLfloat)MIN(getFieldOfView() / getUniformScale(), kMaxEffectiveFOV); 
}

CC3FieldOfViewOrientation CC3Camera::getFieldOfViewOrientation()
{
	return m_fieldOfViewOrientation; 
}

void CC3Camera::setFieldOfViewOrientation( CC3FieldOfViewOrientation fieldOfViewOrientation )
{
	if (fieldOfViewOrientation == m_fieldOfViewOrientation) 
		return;

	m_fieldOfViewOrientation = fieldOfViewOrientation;
	markProjectionDirty();
}

CC3UIInterfaceOrientation CC3Camera::getFieldOfViewAspectOrientation()
{
	return m_fieldOfViewAspectOrientation; 
}

void CC3Camera::setFieldOfViewAspectOrientation( CC3UIInterfaceOrientation fieldOfViewAspectOrientation )
{
	if ( fieldOfViewAspectOrientation == m_fieldOfViewAspectOrientation ) 
		return;

	m_fieldOfViewAspectOrientation = fieldOfViewAspectOrientation;
	markProjectionDirty();
}

GLfloat CC3Camera::getNearClippingDistance()
{
	return m_nearClippingDistance; 
}

void CC3Camera::setNearClippingDistance( GLfloat aDistance )
{
	if (aDistance == m_nearClippingDistance) 
		return;

	m_nearClippingDistance = aDistance;
	markProjectionDirty();
}

GLfloat CC3Camera::getFarClippingDistance()
{
	return m_farClippingDistance; 
}

void CC3Camera::setFarClippingDistance( GLfloat aDistance )
{
	if (aDistance == m_farClippingDistance) 
		return;

	m_farClippingDistance = aDistance;
	markProjectionDirty();
}

CC3Viewport CC3Camera::getViewport()
{
	return m_viewport; 
}

void CC3Camera::setViewport( const CC3Viewport& viewport )
{
	if (CC3ViewportsAreEqual(viewport, m_viewport)) 
		return;

	m_viewport = viewport;
	markProjectionDirty();
}


// Overridden to mark the frustum's projection matrix dirty instead of the
// globalTransformMatrix. This is because for a camera, scale acts as a zoom to change
// the effective FOV, which is a projection quality, not a transformation quality.
void CC3Camera::setScale( const CC3Vector& aScale )
{
	m_scale = aScale;
	markProjectionDirty();
}

bool CC3Camera::hasInfiniteDepthOfField()
{
	return m_hasInfiniteDepthOfField;
}

void CC3Camera::setHasInfiniteDepthOfField( bool infinite )
{
	m_hasInfiniteDepthOfField = infinite;
}

bool CC3Camera::isUsingParallelProjection()
{ 
	return m_frustum->isUsingParallelProjection(); 
}

void CC3Camera::setIsUsingParallelProjection( bool shouldUseParallelProjection )
{
	m_frustum->setIsUsingParallelProjection( shouldUseParallelProjection );
	markProjectionDirty();
}

void CC3Camera::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	{
		setFrustum( CC3Frustum::frustum() );		// use setter for backpointer
		m_isProjectionDirty = true;
		m_fieldOfView = kCC3DefaultFieldOfView;
		m_fieldOfViewOrientation = CC3FieldOfViewOrientationDiagonal;
		m_fieldOfViewAspectOrientation = CC3UIInterfaceOrientationLandscapeLeft;
		m_nearClippingDistance = kCC3DefaultNearClippingDistance;
		m_farClippingDistance = kCC3DefaultFarClippingDistance;
		m_viewport = CC3ViewportMake(0, 0, 0, 0);
		m_shouldClipToViewport = false;
		m_hasInfiniteDepthOfField = false;
		m_isOpen = false;
	}
}

void CC3Camera::populateFrom( CC3Camera* another )
{
	super::populateFrom( another );

	setFrustum( (CC3Frustum*)another->m_frustum->copy()->autorelease() ) ;

	m_fieldOfView = another->getFieldOfView();
	m_fieldOfViewOrientation = another->getFieldOfViewOrientation();
	m_fieldOfViewAspectOrientation = another->getFieldOfViewAspectOrientation();
	m_nearClippingDistance = another->getNearClippingDistance();
	m_farClippingDistance = another->getFarClippingDistance();
	m_isProjectionDirty = another->m_isProjectionDirty;
	m_isOpen = another->isOpen();
}

CCObject* CC3Camera::copyWithZone( CCZone* zone )
{
	CC3Camera* pVal = new CC3Camera;
	pVal->init();
	pVal->populateFrom( this );

	return pVal;
}

bool CC3Camera::isOpen()
{
	return m_isOpen;
}

std::string CC3Camera::fullDescription()
{
	/*return [NSString stringWithFormat: @"%@, FOV: %.2f, near: %.2f, far: %.2f",
			[super fullDescription], _fieldOfView, _nearClippingDistance, _farClippingDistance];*/
	return "";
}

/** Overridden to also force the frustum to be rebuilt. */
void CC3Camera::markTransformDirty()
{
	super::markTransformDirty();
	m_frustum->markDirty();
}

void CC3Camera::markProjectionDirty()
{
	m_isProjectionDirty = true; 
}

/**
 * Scaling the camera is a null operation because it scales everything, including the size
 * of objects, but also the distance from the camera to those objects. The effects cancel
 * out, and visually, it appears that nothing has changed. Therefore, the scale property
 * is not applied to the transform matrix of the camera. Instead it is used to adjust the
 * field of view to create a zooming effect. See the notes for the fieldOfView property.
 *
 * This implementation uses the globalScale property to unwind all scaling from the camera,
 * globally, because any inherited scaling will scale the frustum, and cause undesirable
 * clipping artifacts, particularly at the near clipping plane.
 *
 * For example, if the camera is mounted on another node that is scaled to ten times, the
 * near clipping plane of the camera will be scaled away from the camera by ten times,
 * resulting in unwanted clipping around the fringes of the view. For this reason, an inverse
 * scale of 1/10 is applied to the transform to counteract this effect.
 */
void CC3Camera::applyScalingTo( CC3Matrix* matrix )
{
	matrix->scaleBy( getGlobalScale().invert() ); 
}

/**
 * Scaling does not apply to cameras. Return the globalScale of the parent node, 
 * or unit scaling if no parent.
 */
CC3Vector CC3Camera::getGlobalScale()
{
	return m_pParent ? m_pParent->getGlobalScale() : CC3Vector::kCC3VectorUnitCube; 
}

CC3Matrix* CC3Camera::getViewMatrix()
{
	return getGlobalTransformMatrixInverted(); 
}

/**
 * Template method to rebuild the frustum's projection matrix if the
 * projection parameters have been changed since the last rebuild.
 */
void CC3Camera::buildProjection()
{
	if ( !m_isProjectionDirty ) 
		return;
	
	CCAssert( m_viewport.h > 0 && m_viewport.w > 0, "%CC3Camera does not have a valid viewport" );
	
	CCPoint fovAspect = getOrientedFieldOfViewAspect();
	m_frustum->populateRight( m_nearClippingDistance * fovAspect.x, m_nearClippingDistance * fovAspect.y, m_nearClippingDistance, m_farClippingDistance );
	
	m_isProjectionDirty = false;
	
	notifyTransformListeners();	// Notify the transform listeners that the projection has changed
}

/**
 * Returns a point representing the top-right corner of the near clipping plane,
 * expressed as a proportional multiple of the nearClippingDistance.
 *
 * The returned point will have the same aspect ratio as the viewport. The component 
 * values of the point are calculated taking into consideration the effectiveFieldOfView,
 * fieldOfViewOrientation, and fieldOfViewAspectOrientation properties.
 */
CCPoint CC3Camera::getOrientedFieldOfViewAspect()
{
	GLfloat halfFOV = getEffectiveFieldOfView() / 2.0f;
	GLfloat aspect = ((GLfloat) m_viewport.w / (GLfloat) m_viewport.h);
	GLfloat right, top, diag, orientationCorrection;

	switch (m_fieldOfViewOrientation) 
	{
		case CC3FieldOfViewOrientationVertical:
			top = tanf(CC3DegToRad(halfFOV));
			right = top * aspect;
			orientationCorrection = 1.0f / aspect;
			break;

		case CC3FieldOfViewOrientationDiagonal:
			diag = tanf(CC3DegToRad(halfFOV));
			top = diag / sqrtf((aspect * aspect) + 1.0f);
			right = top * aspect;
			orientationCorrection = 1.0f;
			break;
		
		case CC3FieldOfViewOrientationHorizontal:
		default:
			right = tanf(CC3DegToRad(halfFOV));
			top = right / aspect;
			orientationCorrection = aspect;
			break;
	}

	// If the aspect doesn't match the intended orientation,
	// bring them in alignment by scaling by the orientation correction.
	if ((isDeviceOrientationLandscape(m_fieldOfViewAspectOrientation) && (aspect < 1.0f)) ||
		(isDeviceOrientationPortrait(m_fieldOfViewAspectOrientation ) && (aspect > 1.0f))) 
	{
		right *= orientationCorrection;
		top *= orientationCorrection;
	}

	return ccp(right, top);
}

void CC3Camera::openWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	//LogTrace(@"%@ opening with %@", self, visitor);
	m_isOpen = true;
	openProjectionWithVisitor( visitor );
	openViewWithVisitor( visitor );
}

void CC3Camera::closeWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	//LogTrace(@"%@ closing with %@", self, visitor);
	m_isOpen = false;
	closeViewWithVisitor( visitor );
	closeProjectionWithVisitor( visitor );
}

/** Template method that pushes the GL projection matrix stack, and loads the projectionMatrix into it. */
void CC3Camera::openProjectionWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	// LogTrace(@"%@ opening projection with %@", self, visitor);
	visitor->getGL()->pushProjectionMatrixStack();
	loadProjectionMatrixWithVisitor( visitor );
}

/** Template method that pops the projectionMatrix from the GL projection matrix stack. */
void CC3Camera::closeProjectionWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	//LogTrace(@"%@ closing projection with %@", self, visitor);
	visitor->getGL()->popProjectionMatrixStack();
}

/** Template method that pushes the GL modelview matrix stack, and loads the viewMatrix into it. */
void CC3Camera::openViewWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	//LogTrace(@"%@ opening modelview with %@", self, visitor);
	visitor->getGL()->pushModelviewMatrixStack();
	loadViewMatrixWithVisitor( visitor );
}

/** Template method that pops the viewMatrix from the GL modelview matrix stack. */
void CC3Camera::closeViewWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	//LogTrace(@"%@ closing modelview with %@", self, visitor);
	visitor->getGL()->popModelviewMatrixStack();
}

/** Template method that loads the viewMatrix into the current GL modelview matrix. */
void CC3Camera::loadViewMatrixWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	//LogTrace(@"%@ loading modelview matrix into GL: %@", self, self.viewMatrix);
	visitor->populateViewMatrixFrom( getViewMatrix() );
}

/**
 * Template method that loads either the projectionMatrix or the
 * infiniteProjectionMatrix into the current GL projection matrix,
 * depending on the currents state of the hasInfiniteDepthOfField property.
 */
void CC3Camera::loadProjectionMatrixWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	// LogTrace(@"%@ loading %@finite projection matrix into GL: %@",
	//		 self, (_hasInfiniteDepthOfField ? @"in" : @""), self.projectionMatrix);
	visitor->populateProjMatrixFrom( getProjectionMatrix() );
}

void CC3Camera::moveToShowAllOf( CC3Node* aNode )
{
	moveToShowAllOf( aNode, kCC3DefaultFrustumFitPadding );
}

void CC3Camera::moveToShowAllOf( CC3Node* aNode, GLfloat padding )
{
	ensureSceneUpdated( true );
	CC3Vector moveDir = getGlobalLocation() - aNode->getGlobalLocation();
	moveToShowAllOfLookAt( aNode, CC3Vector::kCC3VectorNull, moveDir, padding, false );
}

void CC3Camera::moveToShowAllOf( CC3Node* aNode, const CC3Vector& aDirection )
{
	moveToShowAllOf( aNode, aDirection, kCC3DefaultFrustumFitPadding );
}

void CC3Camera::moveToShowAllOf( CC3Node* aNode, const CC3Vector& aDirection, GLfloat padding )
{
	moveToShowAllOfLookAt( aNode, CC3Vector::kCC3VectorNull, aDirection, padding, true );
}

void CC3Camera::moveToShowAllOfLookAt( CC3Node* aNode, const CC3Vector& targetLoc )
{
	moveToShowAllOfLookAt( aNode, targetLoc, kCC3DefaultFrustumFitPadding );
}

void CC3Camera::moveToShowAllOfLookAt( CC3Node* aNode, const CC3Vector& targetLoc, GLfloat padding )
{
	ensureSceneUpdated( true );
	CC3Vector moveDir = getGlobalLocation() - aNode->getGlobalLocation();
	moveToShowAllOfLookAt( aNode, targetLoc, moveDir, padding, false );
}

void CC3Camera::moveToShowAllOfLookAt( CC3Node* aNode, const CC3Vector& targetLoc, const CC3Vector& aDirection )
{
	moveToShowAllOfLookAt( aNode, targetLoc, aDirection, kCC3DefaultFrustumFitPadding );
}

void CC3Camera::moveToShowAllOfLookAt( CC3Node* aNode, const CC3Vector& targetLoc, const CC3Vector& aDirection, GLfloat padding )
{
	moveToShowAllOfLookAt( aNode, targetLoc, aDirection, padding, true );
}

void CC3Camera::moveToShowAllOfLookAt( CC3Node* aNode, const CC3Vector& targetLoc, const CC3Vector& aDirection, GLfloat padding, bool checkScene )
{
	moveWithDuration( 0.0f, aNode, targetLoc, aDirection, padding, checkScene );
}

void CC3Camera::moveWithDuration( float t, CC3Node* aNode )
{
	moveWithDuration( t, aNode, kCC3DefaultFrustumFitPadding );
}

void CC3Camera::moveWithDuration( float t, CC3Node* aNode, GLfloat padding )
{
	ensureSceneUpdated( true );
	CC3Vector moveDir = getGlobalLocation() - aNode->getGlobalLocation();
	moveWithDuration( t, aNode, CC3Vector::kCC3VectorNull, moveDir, padding, false );
}

void CC3Camera::moveWithDuration( float t, CC3Node* aNode, const CC3Vector& aDirection )
{
	moveWithDuration( t, aNode, aDirection, kCC3DefaultFrustumFitPadding );
}

void CC3Camera::moveWithDuration( float t, CC3Node* aNode, const CC3Vector& aDirection, GLfloat padding )
{
	moveWithDuration( t, aNode, CC3Vector::kCC3VectorNull, aDirection, padding, true );
}

void CC3Camera::moveWithDurationLookAt( float t, const CC3Vector& targetLoc, CC3Node* aNode )
{
	moveWithDurationLookAt( t, aNode, targetLoc, kCC3DefaultFrustumFitPadding );
}

void CC3Camera::moveWithDurationLookAt( float t, CC3Node* aNode, const CC3Vector& targetLoc, GLfloat padding )
{
	ensureSceneUpdated( true );
	CC3Vector moveDir = getGlobalLocation() - aNode->getGlobalLocation();
	moveWithDuration( t, aNode, targetLoc, moveDir, padding, false );
}

void CC3Camera::moveWithDurationLookAt( float t, CC3Node* aNode, const CC3Vector& targetLoc, const CC3Vector& aDirection )
{
	moveWithDurationLookAt( t, aNode, targetLoc, aDirection, kCC3DefaultFrustumFitPadding );
}

void CC3Camera::moveWithDurationLookAt( float t, CC3Node* aNode, const CC3Vector& targetLoc, const CC3Vector& aDirection, GLfloat padding )
{
	moveWithDuration( t, aNode, targetLoc, aDirection, padding, true );
}

void CC3Camera::moveWithDuration( float t, CC3Node* aNode, const CC3Vector& targetLoc, const CC3Vector& aDirection, GLfloat padding, bool checkScene )
{
	CC3Vector newLoc = calculateLocationToShowAllOf( aNode, targetLoc, aDirection, padding, checkScene );
	CC3Vector newFwdDir = aDirection.negate();
	//LogInfo(@"%@ \n\tmoving to: %@ \n\tpointing towards: %@ \n\tnear clipping distance: %.3f"
	//		 @"\n\tfar clipping distance: %.3f \n\tto show all of: %@",
	//		 self, NSStringFromCC3Vector(newLoc), NSStringFromCC3Vector(newFwdDir),
	//		 self.nearClippingDistance, self.farClippingDistance, aNode);

	ensureAtRootAncestor();
	if (t > 0.0f) 
	{
		runAction( CC3ActionMoveTo::actionWithDuration( t, newLoc ) );
		runAction( CC3ActionRotateToLookTowards::actionWithDuration( t, newFwdDir ));
	} 
	else 
	{
		setLocation( newLoc );
		setForwardDirection( newFwdDir );
	}
}

/**
 * Padding to add to the near & far clipping plane when it is adjusted as a result of showing
 * all of a node, to ensure that all of the node is within the far end of the frustum.
 */
#define kCC3FrustumFitPadding 0.01

CC3Vector CC3Camera::calculateLocationToShowAllOf( CC3Node* aNode, const CC3Vector& aDirection, GLfloat padding )
{
	return calculateLocationToShowAllOf( aNode, CC3Vector::kCC3VectorNull, aDirection, padding );
}

CC3Vector CC3Camera::calculateLocationToShowAllOf( CC3Node* aNode, const CC3Vector& targetLoc, const CC3Vector& aDirection, GLfloat padding )
{
	return calculateLocationToShowAllOf( aNode, targetLoc, aDirection, padding, true );
}

CC3Vector CC3Camera::calculateLocationToShowAllOf( CC3Node* aNode, const CC3Vector& targLoc, const CC3Vector& aDirection, GLfloat padding, bool checkScene )
{
	ensureSceneUpdated( checkScene );
	
	// Complementary unit vectors pointing towards camera from node, and vice versa
	CC3Vector camDir = aDirection.normalize();
	CC3Vector viewDir = camDir.negate();
	
	// The camera's new forward direction will be viewDir. Use a matrix to detrmine
	// the camera's new up and right directions assuming the same scene up direction.
	CC3Matrix3x3 rotMtx;
	CC3Matrix3x3PopulateToPointTowards(&rotMtx, viewDir, getReferenceUpDirection());
	CC3Vector upDir = CC3Matrix3x3ExtractUpDirection(&rotMtx);
	CC3Vector rtDir = CC3Matrix3x3ExtractRightDirection(&rotMtx);
	
	// Determine the eight vertices, of the node's bounding box, in the global coordinate system
	CC3Box gbb = aNode->getGlobalBoundingBox();

	CC3Vector targetLoc = targLoc;
	// If a target location has not been specified, use the center of the node's global bounding box
	if ( targetLoc.isNull() ) 
		targetLoc = gbb.getCenter();

	CC3Vector bbMin = gbb.minimum;
	CC3Vector bbMax = gbb.maximum;
	CC3Vector bbVertices[8];
	bbVertices[0] = cc3v(bbMin.x, bbMin.y, bbMin.z);
	bbVertices[1] = cc3v(bbMin.x, bbMin.y, bbMax.z);
	bbVertices[2] = cc3v(bbMin.x, bbMax.y, bbMin.z);
	bbVertices[3] = cc3v(bbMin.x, bbMax.y, bbMax.z);
	bbVertices[4] = cc3v(bbMax.x, bbMin.y, bbMin.z);
	bbVertices[5] = cc3v(bbMax.x, bbMin.y, bbMax.z);
	bbVertices[6] = cc3v(bbMax.x, bbMax.y, bbMin.z);
	bbVertices[7] = cc3v(bbMax.x, bbMax.y, bbMax.z);
	
	// Express the camera's FOV in terms of ratios of the near clip bounds to
	// the near clip distance, so we can determine distances using similar triangles.
	CCSize fovRatios = getFovRatios();
	
	// Iterate through all eight vertices of the node's bounding box, and calculate
	// the largest distance required to place the camera away from the center of the
	// node in order to fit all eight vertices within the camera's frustum.
	// Simultaneously, calculate the extra distance from the center of the node to
	// the vertex that will be farthest from the camera, so we can ensure that all
	// vertices will fall within the frustum's far end.
	GLfloat maxCtrDist = 0;
	GLfloat maxVtxDeltaDist = 0;
	GLfloat minVtxDeltaDist = 0;
	for (int i = 0; i < 8; i++) 
	{
		// Get a vector from the target location to the vertex 
		CC3Vector relVtx = bbVertices[i] - targetLoc;
		
		// Project that vector onto each of the camera's new up and right directions,
		// and use similar triangles to determine the distance at which to place the
		// camera so that the vertex will fit in both the up and right directions.
		GLfloat vtxDistUp = fabs(relVtx.dot( upDir ) / fovRatios.height);
		GLfloat vtxDistRt = fabs(relVtx.dot( rtDir ) / fovRatios.width);
		GLfloat vtxDist = MAX(vtxDistUp, vtxDistRt);
		
		// Calculate how far along the view direction the vertex is from the center
		GLfloat vtxDeltaDist = relVtx.dot( viewDir );
		GLfloat ctrDist = vtxDist - vtxDeltaDist;
		
		// Accumulate the maximum distance from the node's center to the camera
		// required to fit all eight points, and the distance from the node's
		// center to the vertex that will be farthest away from the camera. 
		maxCtrDist = MAX(maxCtrDist, ctrDist);
		maxVtxDeltaDist = MAX(maxVtxDeltaDist, vtxDeltaDist);
		minVtxDeltaDist = MIN(minVtxDeltaDist, vtxDeltaDist);
	}
	
	// Add some padding so we will have a bit of space around the node when it fills the view.
	maxCtrDist *= (1 + padding);
	
	// Determine if we need to move the far end of the camera frustum farther away
	GLfloat farClip = viewDir.scaleUniform(maxCtrDist + maxVtxDeltaDist).length();
	farClip *= (GLfloat)(1 + kCC3FrustumFitPadding);		// Include a little bit of padding
	if (farClip > getFarClippingDistance())
		setFarClippingDistance( farClip );
	
	// Determine if we need to move the near end of the camera frustum closer
	GLfloat nearClip = viewDir.scaleUniform(maxCtrDist + minVtxDeltaDist).length();
	nearClip *= (GLfloat)(1 - kCC3FrustumFitPadding);		// Include a little bit of padding
	if (nearClip < getNearClippingDistance()) 
		setNearClippingDistance( nearClip );
	
	//LogTrace(@"%@ moving to %@ to show %@ at %@ within %@ with new farClip: %.3f", self,
	//			  NSStringFromCC3Vector(CC3VectorAdd(targLoc, CC3VectorScaleUniform(camDir, maxCtrDist))),
	//			  aNode, NSStringFromCC3Vector(targLoc), _frustum, self.farClippingDistance);
	
	// Return the new location of the camera,
	return targetLoc.add( camDir.scaleUniform( maxCtrDist ) );
}

/**
 * If the checkScene arg is YES, and the scene is not running, force an update
 * to ensure that all nodes are transformed to their global coordinates.
 */
void CC3Camera::ensureSceneUpdated( bool checkScene )
{
	if (checkScene) 
	{
		CC3Scene* myScene = getScene();
		if ( !myScene->isRunning() ) 
			myScene->updateScene();
	}
}

/**
 * Returns the camera's FOV in terms of ratios of the near clip bounds
 * (width & height) to the near clip distance.
 */
CCSize CC3Camera::getFovRatios()
{
	buildProjection();

	/// TODO: get device orientation
	const int orientation = CC3UIInterfaceOrientationLandscapeLeft;

	switch(/*CCDirector.sharedDirector.getDeviceOrientation*/ orientation)
	{
	case CC3UIInterfaceOrientationLandscapeLeft:
	case CC3UIInterfaceOrientationLandscapeRight:
		return CCSizeMake(m_frustum->getTop() / m_frustum->getNear(), m_frustum->getRight() / m_frustum->getNear());
	case CC3UIInterfaceOrientationPortrait:
	case CC3UIInterfaceOrientationPortraitUpsideDown:
	default:
		return CCSizeMake(m_frustum->getRight() / m_frustum->getNear(), m_frustum->getTop() / m_frustum->getNear());
	}
}

/**
 * Ensures that this camera is a direct child of its root ancestor, which in almost all
 * cases will be your CC3Scene. This is done by simply adding this camera to the root ancestor.
 * The request will be ignored if this camera is already a direct child of the root ancestor.
 */
void CC3Camera::ensureAtRootAncestor()
{
	getRootAncestor()->addChild( this ); 
}

CC3Vector CC3Camera::getProjectLocation( const CC3Vector& a3DLocation )
{
	// Convert specified location to a 4D homogeneous location vector
	// and transform it using the modelview and projection matrices.
	CC3Vector4 hLoc;
	hLoc.fromLocation(a3DLocation);
	hLoc = getViewMatrix()->transformHomogeneousVector( hLoc );
	hLoc = getProjectionMatrix()->transformHomogeneousVector( hLoc );
	
	// Convert projected 4D vector back to 3D.
	CC3Vector projectedLoc = hLoc.homogenizedCC3Vector();

	// The projected vector is in a projection coordinate space between -1 and +1 on all axes.
	// Normalize the vector so that each component is between 0 and 1 by calculating ( v = (v + 1) / 2 ).
	projectedLoc = projectedLoc.average( CC3Vector::kCC3VectorUnitCube );
	
	CCAssert(m_viewport.h > 0 && m_viewport.w > 0, "%CC3Camera does not have a valid viewport");
	
	// Map the X & Y components of the projected location (now between 0 and 1) to display coordinates.
	GLfloat g2p = 1.0f / CCDirector::sharedDirector()->getContentScaleFactor();
	projectedLoc.x *= ((GLfloat)m_viewport.w * g2p);
	projectedLoc.y *= ((GLfloat)m_viewport.h * g2p);
	
	// Using the vector from the camera to the 3D location, determine whether or not the
	// 3D location is in front of the camera by using the dot-product of that vector and
	// the direction the camera is pointing. Set the Z-component of the projected location
	// to be the signed distance from the camera to the 3D location, with a positive sign
	// indicating the location is in front of the camera, and a negative sign indicating
	// the location is behind the camera.
	CC3Vector camToLocVector = a3DLocation - getGlobalLocation();
	GLfloat camToLocDist = camToLocVector.length();
	GLfloat frontOrBack = (GLfloat)SIGN( camToLocVector.dot( getGlobalForwardDirection() ) );
	projectedLoc.z = frontOrBack * camToLocDist;
	
	//LogTrace(@"%@ projecting location %@ to %@ and orienting with device to %@ using viewport %@",
	//		 self, NSStringFromCC3Vector(a3DLocation), NSStringFromCC3Vector(projectedLoc),
	//		 NSStringFromCC3Vector(orientedLoc), NSStringFromCC3Viewport(_viewport));
	return projectedLoc;
}

CC3Vector CC3Camera::getProjectLocation( const CC3Vector& aLocal3DLocation, CC3Node* aNode )
{
	return getProjectLocation( aNode->getGlobalTransformMatrix()->transformLocation( aLocal3DLocation ) );
}

CC3Vector CC3Camera::projectNode( CC3Node* aNode )
{
	CCAssert(aNode, "Camera cannot project a nil node.");
	CC3Vector pLoc = getProjectLocation( aNode->getGlobalLocation() );
	aNode->setProjectedLocation( pLoc );
	return pLoc;
}

CC3Ray CC3Camera::unprojectPoint( const CCPoint& cc2Point )
{
#if 0
	// Scale from UI points to GL points
	CCPoint glPoint = ccpMult(cc2Point, CCDirector::sharedDirector()->getContentScaleFactor());
	
	// Express the glPoint X & Y as proportion of the viewport dimensions.
	CC3Viewport vp = getViewport();
	GLfloat xp = ((2.0f * glPoint.x) / vp.w) - 1;
	GLfloat yp = ((2.0f * glPoint.y) / vp.h) - 1;

	// Ensure that the camera's frustum is up to date, and then map the proportional point
	// on the viewport to its position on the near clipping rectangle. The Z-coordinate is
	// negative because the camera points down the negative Z axis in its local coordinates.
	buildProjection();
	CC3Vector pointLocNear = cc3v(m_frustum->getRight() * xp, m_frustum->getTop() * yp, -m_frustum->getNear());

	CC3Ray ray;
	if ( isUsingParallelProjection() )
	{
		// The location on the near clipping plane is relative to the camera's
		// local coordinates. Convert it to global coordinates before returning.
		// The ray direction is straight out from that global location in the 
		// camera's globalForwardDirection.
		ray.startLocation =  getGlobalTransformMatrix()->transformLocation( pointLocNear );
		ray.direction = getGlobalForwardDirection();
	} 
	else
	{
		// The location on the near clipping plane is relative to the camera's local
		// coordinates. Since the camera's origin is zero in its local coordinates,
		// this point on the near clipping plane forms a directional vector from the
		// camera's origin. Rotate this directional vector with the camera's rotation
		// matrix to convert it to a global direction vector in global coordinates.
		// Thanks to Cocos3D forum user Rogs for suggesting the use of the globalRotationMatrix.
		ray.startLocation = getGlobalLocation();
		ray.direction = getGlobalRotationMatrix()->transformDirection( pointLocNear );
	}
	
	// Ensure the direction component is normalized before returning.
	ray.direction = ray.direction.normalize();
	
	//LogTrace(@"%@ unprojecting point %@ to near plane location %@ and to ray starting at %@ and pointing towards %@",
	//			  [self class], NSStringFromCGPoint(glPoint), NSStringFromCC3Vector(pointLocNear),
	//			  NSStringFromCC3Vector(ray.startLocation), NSStringFromCC3Vector(ray.direction));

	return ray;
#else
	// Ensure that the camera's frustum is up to date, and then map the proportional point
	// on the viewport to its position on the near clipping rectangle. The Z-coordinate is
	// negative because the camera points down the negative Z axis in its local coordinates.
	buildProjection();

	// Scale from UI points to GL points
	CCPoint glPoint = ccpMult(cc2Point, CCDirector::sharedDirector()->getContentScaleFactor());

	// Express the glPoint X & Y as proportion of the viewport dimensions.
	CC3Viewport vp = getViewport();
	float xp = ((2.0f * glPoint.x) / vp.w) - 1;
	float yp = ((2.0f * glPoint.y) / vp.h) - 1;

	// Projection
	float e = 1.f / tanf(CC3DegToRad(getFieldOfView() * 0.5f));
	float a = (float)vp.h / (float)vp.w;
	xp /= e;
	yp /= e / a;

	// View
	CC3Matrix* pView = getGlobalTransformMatrix();

	CC3Ray ray( CC3Vector::kCC3VectorZero, CC3Vector(xp, yp, -1.f) );
	ray.startLocation = pView->transformLocation( ray.startLocation );
	ray.direction = pView->transformDirection( ray.direction );
	ray.direction = ray.direction.normalize();

	return ray;
#endif
}

CC3Vector4 CC3Camera::unprojectPoint( const CCPoint& cc2Point, const CC3Plane& plane )
{
	return CC3RayIntersectionWithPlane( unprojectPoint( cc2Point ), plane );
}

CC3Camera* CC3Camera::nodeWithName( const std::string& name )
{
	CC3Camera* pCam = new CC3Camera;
	pCam->initWithName( name );
	pCam->autorelease();

	return pCam;
}

// Indices of the six boundary planes
#define kCC3TopIdx		0
#define kCC3BotmIdx		1
#define kCC3LeftIdx		2
#define kCC3RgtIdx		3
#define kCC3NearIdx		4
#define kCC3FarIdx		5

// Indices of the eight boundary vertices
#define kCC3NearTopLeftIdx	0
#define kCC3NearTopRgtIdx	1
#define kCC3NearBtmLeftIdx	2
#define kCC3NearBtmRgtIdx	3
#define kCC3FarTopLeftIdx	4
#define kCC3FarTopRgtIdx	5
#define kCC3FarBtmLeftIdx	6
#define kCC3FarBtmRgtIdx	7

CC3Frustum::CC3Frustum()
{
	m_finiteProjectionMatrix = NULL;
	m_infiniteProjectionMatrix = NULL;

	m_camera = NULL;
	m_top = m_bottom = m_left = m_right = m_near = m_far = 0.0f;
	m_finiteProjectionMatrix = CC3ProjectionMatrix::matrix();
	m_finiteProjectionMatrix->retain();

	m_infiniteProjectionMatrix = NULL;
	m_isUsingParallelProjection = false;
}

CC3Frustum::~CC3Frustum()
{
	m_camera = NULL;			// weak reference
	CC_SAFE_RELEASE( m_finiteProjectionMatrix );
	CC_SAFE_RELEASE(m_infiniteProjectionMatrix );
}

GLfloat CC3Frustum::getTop()
{
	return m_top;
}

void CC3Frustum::setTop( GLfloat aValue )
{
	m_top = aValue;
	markProjectionDirty();
}

GLfloat CC3Frustum::getBottom()
{
	return m_bottom;
}

void CC3Frustum::setBottom( GLfloat aValue )
{
	m_bottom = aValue;
	markProjectionDirty();
}

GLfloat CC3Frustum::getLeft()
{
	return m_left;
}

void CC3Frustum::setLeft( GLfloat aValue )
{
	m_left = aValue;
	markProjectionDirty();
}

GLfloat CC3Frustum::getRight()
{
	return m_right;
}

void CC3Frustum::setRight( GLfloat aValue )
{
	m_right = aValue;
	markProjectionDirty();
}

GLfloat CC3Frustum::getNear()
{
	return m_near;
}

void CC3Frustum::setNear( GLfloat aValue )
{
	m_near = aValue;
	markProjectionDirty();
}

GLfloat CC3Frustum::getFar()
{
	return m_far;
}

void CC3Frustum::setFar( GLfloat aValue )
{
	m_far = aValue;
	markProjectionDirty();
}

CC3Plane* CC3Frustum::getPlanes()
{
	updateIfNeeded();
	return m_planes;
}

GLuint CC3Frustum::getPlaneCount()
{
	return 6; 
}

CC3Vector* CC3Frustum::getVertices()
{
	updateIfNeeded();
	return m_vertices;
}

GLuint CC3Frustum::getVertexCount()
{
	return 8; 
}

CC3Plane CC3Frustum::getTopPlane()
{
	return getPlanes()[kCC3TopIdx]; 
}

CC3Plane CC3Frustum::getBottomPlane()
{
	return getPlanes()[kCC3BotmIdx]; 
}

CC3Plane CC3Frustum::getLeftPlane()
{
	return getPlanes()[kCC3LeftIdx]; 
}

CC3Plane CC3Frustum::getRightPlane()
{
	return getPlanes()[kCC3RgtIdx]; 
}

CC3Plane CC3Frustum::getNearPlane()
{
	return getPlanes()[kCC3NearIdx]; 
}

CC3Plane CC3Frustum::getFarPlane()
{
	return getPlanes()[kCC3FarIdx]; 
}

CC3Vector CC3Frustum::getNearTopLeft()
{
	return getVertices()[kCC3NearTopLeftIdx]; 
}

CC3Vector CC3Frustum::getNearTopRight()
{
	return getVertices()[kCC3NearTopRgtIdx]; 
}

CC3Vector CC3Frustum::getNearBottomLeft()
{
	return getVertices()[kCC3NearBtmLeftIdx]; 
}

CC3Vector CC3Frustum::getNearBottomRight() 
{
	return getVertices()[kCC3NearBtmRgtIdx]; 
}

CC3Vector CC3Frustum::getFarTopLeft() 
{
	return getVertices()[kCC3FarTopLeftIdx]; 
}

CC3Vector CC3Frustum::getFarTopRight() 
{
	return getVertices()[kCC3FarTopRgtIdx]; 
}

CC3Vector CC3Frustum::getFarBottomLeft()
{ 
	return getVertices()[kCC3FarBtmLeftIdx]; 
}

CC3Vector CC3Frustum::getFarBottomRight() 
{
	return getVertices()[kCC3FarBtmRgtIdx]; 
}

CC3Camera* CC3Frustum::getCamera()
{
	return m_camera;
}

void CC3Frustum::setCamera( CC3Camera* camera )
{
	m_camera = camera;
}

bool CC3Frustum::isUsingParallelProjection()
{
	return m_isUsingParallelProjection;
}

void CC3Frustum::setIsUsingParallelProjection( bool isUse )
{
	m_isUsingParallelProjection = isUse;
}

bool CC3Frustum::init()
{
	super::init();
	return true;
}

CC3Frustum* CC3Frustum::frustum()
{
	CC3Frustum* pFrustum = new CC3Frustum;
	pFrustum->init();
	pFrustum->autorelease();

	return pFrustum;
}

void CC3Frustum::populateFrom( CC3Frustum* another )
{
	super::populateFrom( another );

	m_isUsingParallelProjection = another->isUsingParallelProjection();
	populateRight( another->getRight(), another->getTop(), another->getNear(), another->getFar() );
}

CCObject* CC3Frustum::copyWithZone( CCZone* pZone )
{
	CC3Frustum* pVal = new CC3Frustum;
	pVal->init();
	pVal->populateFrom( this );

	return pVal;
}

void CC3Frustum::populateRight( GLfloat right, GLfloat top, GLfloat fnear, GLfloat ffar )
{
	m_right = right;
	m_left = -right;
	m_top = top;
	m_bottom = -top;
	m_near = fnear;
	m_far = ffar;
	
	markProjectionDirty();
	
	// LogTrace(@"%@ updated from right: %.3f, top: %.3f, near: %.3f, far: %.3f", self, right, top, near, far);
}

void CC3Frustum::populateFrom( GLfloat fieldOfView, GLfloat aspect, GLfloat nearClip, GLfloat farClip )
{
	GLfloat rightClip, topClip;
	GLfloat halfFOV = fieldOfView / 2.0f;

	// Apply the field of view angle to the narrower aspect.
	if ( aspect >= 1.0f ) 
	{	// Landscape
		topClip = nearClip * tanf(CC3DegToRad(halfFOV));
		rightClip = topClip * aspect;
	}
	else 
	{	// Portrait
		rightClip = nearClip * tanf(CC3DegToRad(halfFOV));
		topClip = rightClip / aspect;
	}

	populateRight( rightClip, topClip, nearClip, farClip );
}

std::string CC3Frustum::fullDescription()
{
	return "CC3Frustum";
}

void CC3Frustum::markProjectionDirty()
{
	if ( m_finiteProjectionMatrix )
		m_finiteProjectionMatrix->setIsDirty( true );

	if ( m_infiniteProjectionMatrix )
		m_infiniteProjectionMatrix->setIsDirty( true );

	markDirty();
}

CC3Matrix* CC3Frustum::getFiniteProjectionMatrix()
{
	if ( m_finiteProjectionMatrix->isDirty() ) 
	{
		if (m_isUsingParallelProjection)
			m_finiteProjectionMatrix->populateOrthoFromFrustumLeft(m_left, m_right, m_top, m_bottom, m_near, m_far );
		else
			m_finiteProjectionMatrix->populateFromFrustumLeft( m_left, m_right, m_top, m_bottom, m_near, m_far );

		m_finiteProjectionMatrix->setIsDirty( false );
	}

	return m_finiteProjectionMatrix;
}

CC3Matrix* CC3Frustum::getInfiniteProjectionMatrix()
{
	// Since this matrix is not commonly used, it is only calculated when the
	// finiateProjectionMatrix has changed, and then only on demand.
	if ( !m_infiniteProjectionMatrix ) 
	{
		m_infiniteProjectionMatrix = CC3ProjectionMatrix::matrix();		// retained
		m_infiniteProjectionMatrix->retain();
		m_infiniteProjectionMatrix->setIsDirty( true );
	}
	if ( m_infiniteProjectionMatrix->isDirty() ) 
	{
		if (m_isUsingParallelProjection)
			m_infiniteProjectionMatrix->populateOrthoFromFrustumLeft(m_left, m_right, m_top, m_bottom, m_near, m_far );
		else
			m_infiniteProjectionMatrix->populateFromFrustumLeft( m_left, m_right, m_top, m_bottom, m_near, m_far );

		m_infiniteProjectionMatrix->setIsDirty( false );
	}
	return m_infiniteProjectionMatrix;
}

/**
 * Builds the six planes that define the frustum volume,
 * using the modelview matrix and the finite projection matrix.
 */
void CC3Frustum::buildPlanes()
{
	CC3Matrix4x4 projMtx, viewMtx, m;
	getFiniteProjectionMatrix()->populateCC3Matrix4x4( &projMtx );
	if ( m_camera )
		m_camera->getViewMatrix()->populateCC3Matrix4x4( &viewMtx );
	CC3Matrix4x4Multiply(&m, &projMtx, &viewMtx);
	
    m_planes[kCC3BotmIdx] = CC3Plane::negate(CC3Plane::normalize(CC3Plane((m.c1r4 + m.c1r2), (m.c2r4 + m.c2r2),
																		 (m.c3r4 + m.c3r2), (m.c4r4 + m.c4r2))));
	m_planes[kCC3TopIdx]  = CC3Plane::negate(CC3Plane::normalize(CC3Plane((m.c1r4 - m.c1r2), (m.c2r4 - m.c2r2),
																		 (m.c3r4 - m.c3r2), (m.c4r4 - m.c4r2))));
	
	m_planes[kCC3LeftIdx] = CC3Plane::negate(CC3Plane::normalize(CC3Plane((m.c1r4 + m.c1r1), (m.c2r4 + m.c2r1),
																		 (m.c3r4 + m.c3r1), (m.c4r4 + m.c4r1))));
	m_planes[kCC3RgtIdx]  = CC3Plane::negate(CC3Plane::normalize(CC3Plane((m.c1r4 - m.c1r1), (m.c2r4 - m.c2r1),
																		 (m.c3r4 - m.c3r1), (m.c4r4 - m.c4r1))));
	
	m_planes[kCC3NearIdx] = CC3Plane::negate(CC3Plane::normalize(CC3Plane((m.c1r4 + m.c1r3), (m.c2r4 + m.c2r3),
																		 (m.c3r4 + m.c3r3), (m.c4r4 + m.c4r3))));
	m_planes[kCC3FarIdx]  = CC3Plane::negate(CC3Plane::normalize(CC3Plane((m.c1r4 - m.c1r3), (m.c2r4 - m.c2r3),
																		 (m.c3r4 - m.c3r3), (m.c4r4 - m.c4r3))));
	buildVertices();
	
	//LogTrace(@"Built planes for %@ from projection: %@ and view: %@", self, self.finiteProjectionMatrix, _camera.viewMatrix);
}

void CC3Frustum::buildVertices()
{
	CC3Plane tp = m_planes[kCC3TopIdx];
	CC3Plane bp = m_planes[kCC3BotmIdx];
	CC3Plane lp = m_planes[kCC3LeftIdx];
	CC3Plane rp = m_planes[kCC3RgtIdx];
	CC3Plane np = m_planes[kCC3NearIdx];
	CC3Plane fp = m_planes[kCC3FarIdx];
	
	m_vertices[kCC3NearTopLeftIdx] = CC3TriplePlaneIntersection(np, tp, lp);
	m_vertices[kCC3NearTopRgtIdx] = CC3TriplePlaneIntersection(np, tp, rp);
	
	m_vertices[kCC3NearBtmLeftIdx] = CC3TriplePlaneIntersection(np, bp, lp);
	m_vertices[kCC3NearBtmRgtIdx] = CC3TriplePlaneIntersection(np, bp, rp);
	
	m_vertices[kCC3FarTopLeftIdx] = CC3TriplePlaneIntersection(fp, tp, lp);
	m_vertices[kCC3FarTopRgtIdx] = CC3TriplePlaneIntersection(fp, tp, rp);
	
	m_vertices[kCC3FarBtmLeftIdx] = CC3TriplePlaneIntersection(fp, bp, lp);
	m_vertices[kCC3FarBtmRgtIdx] = CC3TriplePlaneIntersection(fp, bp, rp);
}

NS_COCOS3D_END
