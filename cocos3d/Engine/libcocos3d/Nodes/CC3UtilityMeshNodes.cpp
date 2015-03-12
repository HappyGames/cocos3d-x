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
#include "CC3UtilityMeshNodes.h"
#include "../Scenes/CC3Scene.h"

NS_COCOS3D_BEGIN

CC3PlaneNode* CC3PlaneNode::nodeWithName( const std::string& aName )
{
	CC3PlaneNode* pNode = new CC3PlaneNode;
	pNode->initWithName( aName );
	pNode->autorelease();

	return pNode;
}

CC3Plane CC3PlaneNode::getPlane()
{
	CC3Box bb = getMesh()->getBoundingBox();
	
	// Get three points on the plane by using three corners of the mesh bounding box.
	CC3Vector p1 = bb.minimum;
	CC3Vector p2 = bb.maximum;
	CC3Vector p3 = bb.minimum;
	p3.x = bb.maximum.x;
	
	// Transform these points.
	p1 = getGlobalTransformMatrix()->transformLocation( p1 );
	p2 = getGlobalTransformMatrix()->transformLocation( p2 );
	p3 = getGlobalTransformMatrix()->transformLocation( p3 );
	
	// Create and return a plane from these points.
	return CC3PlaneFromLocations(p1, p2, p3);
}

void CC3SimpleLineNode::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	{
		_lineVertices[0] = kCC3VectorZero;
		_lineVertices[1] = kCC3VectorZero;
		populateAsLineStripWith( 2, _lineVertices, false );
		retainVertexLocations();
	}
}

CC3Vector CC3SimpleLineNode::getLineStart()
{
	return getVertexLocationAt(0); 
}

void CC3SimpleLineNode::setLineStart( const CC3Vector& aLocation )
{
	setVertexLocation( aLocation, 0 );
	updateVertexLocationsGLBuffer();
}

CC3Vector CC3SimpleLineNode::getLineEnd()
{
	return getVertexLocationAt( 1 ); 
}

void CC3SimpleLineNode::setLineEnd( const CC3Vector& aLocation )
{
	setVertexLocation( aLocation, 1 );
	updateVertexLocationsGLBuffer();
}

CC3BoxNode* CC3BoxNode::nodeWithName( const std::string& aName )
{
	CC3BoxNode* pNode = new CC3BoxNode;
	pNode->initWithName( aName );
	pNode->autorelease();

	return pNode;
}

CC3BoxNode* CC3BoxNode::node()
{
	CC3BoxNode* pNode = new CC3BoxNode;
	pNode->init();
	pNode->autorelease();

	return pNode;
}

CC3Box CC3TouchBox::getBox()
{
	return getLocalContentBoundingBox(); 
}

void CC3TouchBox::setBox( const CC3Box& aBox )
{
	if (CC3BoxIsNull(aBox)) {
		setMesh( NULL );
	} else {
		populateBox( aBox );
	}
}

void CC3TouchBox::setParent( CC3Node* aNode )
{
	super::setParent( aNode );
	deriveNameFrom( aNode );
	if ( !_mesh ) 
		setBox( getParentBoundingBox() );
}

std::string CC3TouchBox::getNameSuffix()
{
	return "TouchBox"; 
}

CC3Box CC3TouchBox::getParentBoundingBox()
{
	return m_pParent ? m_pParent->getBoundingBox() : kCC3BoxNull; 
}

bool CC3TouchBox::shouldContributeToParentBoundingBox()
{
	return false; 
}

void CC3TouchBox::populateBox( const CC3Box& aBox )
{
	CC3Mesh* mesh = prepareParametricMesh();
	
	// Now update the vertex locations with the box data
	GLuint vIdx = 0;
	CC3Vector bbMin = aBox.minimum;
	CC3Vector bbMax = aBox.maximum;
	mesh->setVertexLocation( cc3v(bbMin.x, bbMin.y, bbMin.z), vIdx++ );
	mesh->setVertexLocation( cc3v(bbMin.x, bbMin.y, bbMax.z), vIdx++ );
	mesh->setVertexLocation( cc3v(bbMin.x, bbMax.y, bbMin.z), vIdx++ );
	mesh->setVertexLocation( cc3v(bbMin.x, bbMax.y, bbMax.z), vIdx++ );
	mesh->setVertexLocation( cc3v(bbMax.x, bbMin.y, bbMin.z), vIdx++ );
	mesh->setVertexLocation( cc3v(bbMax.x, bbMin.y, bbMax.z), vIdx++ );
	mesh->setVertexLocation( cc3v(bbMax.x, bbMax.y, bbMin.z), vIdx++ );
	mesh->setVertexLocation( cc3v(bbMax.x, bbMax.y, bbMax.z), vIdx++ );
	
	mesh->updateVertexLocationsGLBuffer();
	markBoundingVolumeDirty();
}

/** Overridden because we only need vertex locations, and to allocate and populate indices. */
CC3Mesh* CC3TouchBox::prepareParametricMesh()
{
	if (_mesh) 
		return _mesh;
	
	if (getVertexContentTypes() == kCC3VertexContentNone)
		setVertexContentTypes( kCC3VertexContentLocation );
	
	// Prepare the vertex content and allocate space for vertices and indices.
	_mesh->setAllocatedVertexCapacity( 8 );
	_mesh->setAllocatedVertexIndexCapacity( 36 );
	
	GLuint vIdx = 0;
	
	// Front
	_mesh->setVertexIndex( 1, vIdx++ );
	_mesh->setVertexIndex( 5, vIdx++ );
	_mesh->setVertexIndex( 7, vIdx++ );
	_mesh->setVertexIndex( 7, vIdx++ );
	_mesh->setVertexIndex( 3, vIdx++ );
	_mesh->setVertexIndex( 1, vIdx++ );
						 	 		 
	// Back				 	 		 
	_mesh->setVertexIndex( 0, vIdx++ );
	_mesh->setVertexIndex( 2, vIdx++ );
	_mesh->setVertexIndex( 6, vIdx++ );
	_mesh->setVertexIndex( 6, vIdx++ );
	_mesh->setVertexIndex( 4, vIdx++ );
	_mesh->setVertexIndex( 0, vIdx++ );
						 			 
	// Left				 	 		 
	_mesh->setVertexIndex( 0, vIdx++ );
	_mesh->setVertexIndex( 1, vIdx++ );
	_mesh->setVertexIndex( 3, vIdx++ );
	_mesh->setVertexIndex( 3, vIdx++ );
	_mesh->setVertexIndex( 2, vIdx++ );
	_mesh->setVertexIndex( 0, vIdx++ );
						 			 
	// Right			 	 		 
	_mesh->setVertexIndex( 4, vIdx++ );
	_mesh->setVertexIndex( 6, vIdx++ );
	_mesh->setVertexIndex( 7, vIdx++ );
	_mesh->setVertexIndex( 7, vIdx++ );
	_mesh->setVertexIndex( 5, vIdx++ );
	_mesh->setVertexIndex( 4, vIdx++ );
							 		 
	// Top					 		 
	_mesh->setVertexIndex( 2, vIdx++ );
	_mesh->setVertexIndex( 3, vIdx++ );
	_mesh->setVertexIndex( 7, vIdx++ );
	_mesh->setVertexIndex( 7, vIdx++ );
	_mesh->setVertexIndex( 6, vIdx++ );
	_mesh->setVertexIndex( 2, vIdx++ );
									 
	// Bottom				 		 
	_mesh->setVertexIndex( 0, vIdx++ );
	_mesh->setVertexIndex( 4, vIdx++ );
	_mesh->setVertexIndex( 5, vIdx++ );
	_mesh->setVertexIndex( 5, vIdx++ );
	_mesh->setVertexIndex( 1, vIdx++ );
	_mesh->setVertexIndex( 0, vIdx++ );
	
	return _mesh;
}

/** If we should remeasure and update the bounding box dimensions, do so. */
void CC3TouchBox::updateAfterTransform( CC3NodeUpdatingVisitor* visitor )
{
	if (_shouldAlwaysMeasureParentBoundingBox)
		setBox( getParentBoundingBox() );
}

void CC3TouchBox::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	{
		_shouldAlwaysMeasureParentBoundingBox = false;
		setVisible( false );
		setShouldAllowTouchableWhenInvisible( true );
	}
}

CC3NodeBoundingVolume* CC3SphereNode::defaultBoundingVolume()
{
	return CC3NodeSphericalBoundingVolume::boundingVolume();
}

CC3SphereNode* CC3SphereNode::nodeWithName( const std::string& aName )
{
	CC3SphereNode* pNode = new CC3SphereNode;
	pNode->initWithName( aName );
	pNode->autorelease();

	return pNode;
}

CC3SphereNode* CC3SphereNode::node()
{
	CC3SphereNode* pNode = new CC3SphereNode;
	pNode->init();
	pNode->autorelease();

	return pNode;
}


void CC3ClipSpaceNode::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	{
		setShouldDrawInClipSpace( true );
		setShouldUseLighting( false );
	}
}

CC3ClipSpaceNode* CC3ClipSpaceNode::nodeWithTexture( CC3Texture* texture )
{
	CC3ClipSpaceNode* csn = nodeWithColor( kCCC4FWhite );
	csn->setTexture( texture );
	return csn;
}

CC3ClipSpaceNode* CC3ClipSpaceNode::nodeWithColor( const ccColor4F& color )
{
	CC3ClipSpaceNode* csn = new CC3ClipSpaceNode;
	csn->init();
	csn->setEmissionColor( color );
	csn->autorelease();
	return csn;
}

CC3ClipSpaceNode* CC3ClipSpaceNode::nodeWithName( const std::string& name, CC3Texture* texture )
{
	CC3ClipSpaceNode* csn = nodeWithName( name, kCCC4FWhite );
	csn->setTexture( texture );
	return csn;
}

CC3ClipSpaceNode* CC3ClipSpaceNode::nodeWithName( const std::string& name )
{
	CC3ClipSpaceNode* csn = new CC3ClipSpaceNode;
	csn->initWithName( name );
	csn->autorelease();

	return csn;
}

CC3ClipSpaceNode* CC3ClipSpaceNode::nodeWithName( const std::string& name, const ccColor4F& color )
{
	CC3ClipSpaceNode* csn = nodeWithName( name );
	csn->setEmissionColor( color );
	return csn;
}

/** The camera frustum has no meaning in clip-space. */
CC3NodeBoundingVolume* CC3ClipSpaceNode::defaultBoundingVolume()
{
	return NULL; 
}

CC3Backdrop* CC3Backdrop::node()
{
	CC3Backdrop* pNode = new CC3Backdrop;
	pNode->init();
	pNode->autorelease();
	return pNode;
}

CC3Backdrop* CC3Backdrop::nodeWithTexture( CC3Texture* texture )
{
	CC3Backdrop* backdrop = nodeWithColor( kCCC4FWhite );
	backdrop->setTexture( texture );
	return backdrop;
}

CC3Backdrop* CC3Backdrop::nodeWithColor( const ccColor4F& color )
{
	CC3Backdrop* backdrop = new CC3Backdrop;
	backdrop->init();
	backdrop->setEmissionColor( color );
	backdrop->autorelease();
	return backdrop;
}

CC3Backdrop* CC3Backdrop::nodeWithName( const std::string& name, const ccColor4F& color )
{
	CC3Backdrop* backdrop = nodeWithName( name );
	backdrop->setEmissionColor( color );
	return backdrop;
}

CC3Backdrop* CC3Backdrop::nodeWithName( const std::string& name, CC3Texture* texture )
{
	CC3Backdrop* backdrop = nodeWithName( name, kCCC4FWhite );
	backdrop->setTexture( texture );
	return backdrop;
}

CC3Backdrop* CC3Backdrop::nodeWithName( const std::string& name )
{
	CC3Backdrop* backdrop = new CC3Backdrop;
	backdrop->initWithName( name );
	backdrop->autorelease();
	return backdrop;
}

bool CC3WireframeBoundingBoxNode::shouldIncludeInDeepCopy()
{
	return false; 
}

bool CC3WireframeBoundingBoxNode::shouldDrawDescriptor()
{
	return true; 
}

void CC3WireframeBoundingBoxNode::setShouldDrawDescriptor( bool shouldDraw )
{

}

bool CC3WireframeBoundingBoxNode::shouldDrawWireframeBox()
{
	return true; 
}

void CC3WireframeBoundingBoxNode::setShouldDrawWireframeBox( bool shouldDraw )
{

}

bool CC3WireframeBoundingBoxNode::shouldDrawLocalContentWireframeBox()
{
	return true; 
}

void CC3WireframeBoundingBoxNode::setShouldDrawLocalContentWireframeBox( bool shouldDraw )
{
}

bool CC3WireframeBoundingBoxNode::shouldContributeToParentBoundingBox()
{
	return false; 
}

bool CC3WireframeBoundingBoxNode::shouldDrawBoundingVolume()
{
	return false; 
}

void CC3WireframeBoundingBoxNode::setShouldDrawBoundingVolume( bool shouldDraw )
{
}

/** Overridden so that not touchable unless specifically set as such. */
bool CC3WireframeBoundingBoxNode::isTouchable()
{
	return (isVisible() || m_shouldAllowTouchableWhenInvisible) && isTouchEnabled();
}

/** Overridden so that can still be visible if parent is invisible, unless explicitly turned off. */
bool CC3WireframeBoundingBoxNode::isVisible()
{
	return m_visible; 
}

/** For wireframe lines, if material is created dynamically, make sure it ignores lighting. */
CC3Material* CC3WireframeBoundingBoxNode::makeMaterial()
{
	CC3Material* mat = super::makeMaterial();
	mat->setShouldUseLighting( false );
	return mat;
}

void CC3WireframeBoundingBoxNode::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	_shouldAlwaysMeasureParentBoundingBox = false;
}

bool CC3WireframeBoundingBoxNode::shouldAlwaysMeasureParentBoundingBox()
{
	return _shouldAlwaysMeasureParentBoundingBox;
}

void CC3WireframeBoundingBoxNode::setShouldAlwaysMeasureParentBoundingBox( bool alwaysMeasure )
{
	_shouldAlwaysMeasureParentBoundingBox = alwaysMeasure;
}

void CC3WireframeBoundingBoxNode::populateFrom( CC3WireframeBoundingBoxNode* another )
{
	super::populateFrom( another );
	
	_shouldAlwaysMeasureParentBoundingBox = another->shouldAlwaysMeasureParentBoundingBox();
}

CCObject* CC3WireframeBoundingBoxNode::copyWithZone( CCZone* zone )
{
	CC3WireframeBoundingBoxNode* pVal = new CC3WireframeBoundingBoxNode;
	pVal->init();
	pVal->populateFrom( this );
	pVal->addCopiesOfChildrenFrom( this );

	return pVal;
}

void CC3WireframeBoundingBoxNode::releaseRedundantContent()
{
	retainVertexLocations();
	super::releaseRedundantContent();
}

void CC3WireframeBoundingBoxNode::updateFromParentBoundingBox()
{
	updateFromParentBoundingBoxWithVisitor( NULL ); 
}

/** If we should remeasure and update the bounding box dimensions, do so. */
void CC3WireframeBoundingBoxNode::updateAfterTransform( CC3NodeUpdatingVisitor* visitor )
{
	if (_shouldAlwaysMeasureParentBoundingBox)
		updateFromParentBoundingBoxWithVisitor( visitor );
}

/** Measures the bounding box of the parent node and updates the vertex locations. */
void CC3WireframeBoundingBoxNode::updateFromParentBoundingBoxWithVisitor( CC3NodeUpdatingVisitor* visitor )
{
	CC3Box pbb = getParentBoundingBox();
	setVertexLocation( cc3v(pbb.minimum.x, pbb.minimum.y, pbb.minimum.z), 0 );
	setVertexLocation( cc3v(pbb.minimum.x, pbb.minimum.y, pbb.maximum.z), 1 );
	setVertexLocation( cc3v(pbb.minimum.x, pbb.maximum.y, pbb.minimum.z), 2 );
	setVertexLocation( cc3v(pbb.minimum.x, pbb.maximum.y, pbb.maximum.z), 3 );
	setVertexLocation( cc3v(pbb.maximum.x, pbb.minimum.y, pbb.minimum.z), 4 );
	setVertexLocation( cc3v(pbb.maximum.x, pbb.minimum.y, pbb.maximum.z), 5 );
	setVertexLocation( cc3v(pbb.maximum.x, pbb.maximum.y, pbb.minimum.z), 6 );
	setVertexLocation( cc3v(pbb.maximum.x, pbb.maximum.y, pbb.maximum.z), 7 );
	updateVertexLocationsGLBuffer();
}

/**
 * Returns the parent's bounding box, or kCC3BoxZero if no parent,
 * or if parent doesn't have a bounding box.
 */
CC3Box CC3WireframeBoundingBoxNode::getParentBoundingBox()
{
	if (m_pParent) {
		CC3Box pbb = m_pParent->getBoundingBox();
		if (!CC3BoxIsNull(pbb)) return pbb;
	}
	return kCC3BoxZero;
}

CC3WireframeBoundingBoxNode* CC3WireframeBoundingBoxNode::nodeWithName( const std::string& aName )
{
	CC3WireframeBoundingBoxNode* pNode = new CC3WireframeBoundingBoxNode;
	pNode->initWithName( aName );
	pNode->autorelease();

	return pNode;
}

CC3WireframeLocalContentBoundingBoxNode* CC3WireframeLocalContentBoundingBoxNode::nodeWithName( const std::string& aName )
{
	CC3WireframeLocalContentBoundingBoxNode* pNode = new CC3WireframeLocalContentBoundingBoxNode;
	pNode->initWithName( aName );
	pNode->autorelease();

	return pNode;
}

/**
 * Overridden to return the parent's local content bounding box,
 * or kCC3BoxZero if no parent, or if parent doesn't have a bounding box.
 */
CC3Box CC3WireframeLocalContentBoundingBoxNode::getParentBoundingBox()
{
	if (m_pParent && m_pParent->hasLocalContent()) 
	{
		CC3Box pbb = ((CC3LocalContentNode*)m_pParent)->getLocalContentBoundingBox();
		if (!CC3BoxIsNull(pbb)) 
			return pbb;
	}
	return kCC3BoxZero;
}

CC3Vector CC3DirectionMarkerNode::getMarkerDirection()
{
	return _markerDirection; 
}

void CC3DirectionMarkerNode::setMarkerDirection( const CC3Vector& aDirection )
{
	_markerDirection = CC3VectorNormalize(aDirection); 
}

void CC3DirectionMarkerNode::setParent( CC3Node* aNode )
{
	super::setParent( aNode );
	updateFromParentBoundingBox();
}

/** 
 * Overridden to establish a default parent bounding box for parents that have no bounding
 * box, such as cameras and lights. The default parent box is calculated as 5% of the size
 * of the entire scene.
 */
CC3Box CC3DirectionMarkerNode::getParentBoundingBox()
{
	CC3Box pbb = super::getParentBoundingBox();
	if ( !CC3BoxIsZero(pbb) ) 
		return pbb;

	CC3Vector bbDim = kCC3VectorZero;
	CC3Scene* pScene = getScene();
	if ( pScene )
		bbDim = CC3VectorScaleUniform(CC3BoxSize(getScene()->getBoundingBox()), 0.05f);
		
	return CC3BoxFromMinMax(CC3VectorNegate(bbDim), bbDim);
}

void CC3DirectionMarkerNode::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	_markerDirection = kCC3VectorUnitZNegative;
}

void CC3DirectionMarkerNode::populateFrom( CC3DirectionMarkerNode* another )
{
	super::populateFrom( another );
	
	_markerDirection = another->getMarkerDirection();
}

CCObject* CC3DirectionMarkerNode::copyWithZone( CCZone* zone )
{
	CC3DirectionMarkerNode* pVal = new CC3DirectionMarkerNode;
	pVal->init();
	pVal->populateFrom( this );
	pVal->addCopiesOfChildrenFrom( this );
	
	return pVal;
}

/** Measures the bounding box of the parent node and updates the vertex locations. */
void CC3DirectionMarkerNode::updateFromParentBoundingBoxWithVisitor( CC3NodeUpdatingVisitor* visitor )
{
	setVertexLocation( calculateLineEnd(), 1 );
	updateVertexLocationsGLBuffer();
}

#define kCC3DirMarkerLineScale 1.5
#define kCC3DirMarkerMinAbsoluteScale (0.25 / kCC3DirMarkerLineScale)

/**
 * Calculates the scale to use, along a single axis, for the length of the directional marker.
 * Divide the distance from the origin, along this axis, to each of two opposite sides of the
 * bounding box, by the length of the directional marker in this axis.
 *
 * Taking into consideration the sign of the direction, the real distance along this axis to
 * the side it will intersect will be the maximum of these two values.
 *
 * Finally, in case the origin is on, or very close to, one side, make sure the length of the
 * directional marker is at least 1/4 of the length of the distance between the two sides.
 */
GLfloat CC3DirectionMarkerNode::calcScale( GLfloat markerAxis, GLfloat minBBAxis, GLfloat maxBBAxis )
{
	if (markerAxis == 0.0f) 
		return kCC3MaxGLfloat;
	
	GLfloat scaleToMaxSide = maxBBAxis / markerAxis;
	GLfloat scaleToMinSide = minBBAxis / markerAxis;
	GLfloat minAbsoluteScale = (GLfloat)(fabsf((maxBBAxis - minBBAxis) / markerAxis) * kCC3DirMarkerMinAbsoluteScale);
//	CC3_PUSH_NOSHADOW
	return MAX(MAX(scaleToMaxSide, scaleToMinSide), minAbsoluteScale);
//	CC3_POP_NOSHADOW
}

// The proportional distance that the direction should protrude from the parent node
static GLfloat directionMarkerScale = 1.5f;

GLfloat CC3DirectionMarkerNode::getDirectionMarkerScale()
{
	return directionMarkerScale; 
}

void CC3DirectionMarkerNode::setDirectionMarkerScale( GLfloat aScale )
{
	directionMarkerScale = aScale; 
}

// The minimum length of a direction marker, in the global coordinate system.
static GLfloat directionMarkerMinimumLength = 0.f;

GLfloat CC3DirectionMarkerNode::getDirectionMarkerMinimumLength()
{
	return directionMarkerMinimumLength; 
}

void CC3DirectionMarkerNode::setDirectionMarkerMinimumLength( GLfloat len )
{
	directionMarkerMinimumLength = len; 
}

/**
 * Calculate the end of the directonal marker line.
 *
 * This is done by calculating the scale we need to multiply the directional marker by to
 * reach each of the three sides of the bounding box, then take the smallest of these,
 * because that is the side it will intersect. Finally, multiply by an overall scale factor.
 */
CC3Vector CC3DirectionMarkerNode::calculateLineEnd()
{
	CC3Box pbb = getParentBoundingBox();
	CC3Vector md = getMarkerDirection();
	
	CC3Vector pbbDirScale = cc3v(calcScale( md.x, pbb.minimum.x, pbb.maximum.x ),
								 calcScale( md.y, pbb.minimum.y, pbb.maximum.y ),
								 calcScale( md.z, pbb.minimum.z, pbb.maximum.z ));
	//CC3_PUSH_NOSHADOW
	GLfloat dirScale = MIN(pbbDirScale.x, MIN(pbbDirScale.y, pbbDirScale.z));
	dirScale = dirScale * getDirectionMarkerScale();
	//CC3_POP_NOSHADOW

	// Ensure that the direction marker has the minimum length specified by directionMarkerMinimumLength
	if (directionMarkerMinimumLength) {
		GLfloat gblUniScale = CC3VectorLength(getGlobalScale()) / kCC3VectorUnitCubeLength;
		GLfloat minScale = directionMarkerMinimumLength / gblUniScale;
		dirScale = MAX(dirScale, minScale);
	}

	CC3Vector lineEnd = CC3VectorScaleUniform(md, dirScale);
	//LogTrace(@"%@ calculated line end %@ from pbb scale %@ and dir scale %.3f and min global length: %.3f", self,
	//		 NSStringFromCC3Vector(lineEnd), NSStringFromCC3Vector(pbbDirScale), dirScale, directionMarkerMinimumLength);
	return lineEnd;
}

CC3DirectionMarkerNode* CC3DirectionMarkerNode::nodeWithName( const std::string& aName )
{
	CC3DirectionMarkerNode* pNode = new CC3DirectionMarkerNode;
	pNode->initWithName( aName );
	pNode->autorelease();

	return pNode;
}

CC3BoundingVolumeDisplayNode* CC3BoundingVolumeDisplayNode::nodeWithName( const std::string& aName )
{
	CC3BoundingVolumeDisplayNode* pNode = new CC3BoundingVolumeDisplayNode;
	pNode->initWithName( aName );
	pNode->autorelease();

	return pNode;
}

/** Forces the color to always remain the same, even when the primary node is tinted to some other color. */
void CC3BoundingVolumeDisplayNode::setColor( const CCColorRef& color )
{
	CC3NodeBoundingVolume* bv = NULL;
	CC3Node* parent = getParent();
	if ( parent )
		bv = parent->getBoundingVolume();

	super::setColor( bv ? bv->getDisplayNodeColor() : color );
}

/** 
 * Limit the opacity of the bounding volume display, so it doesn't obscure the primary node,
 * even when opacity of the parent is changed, as in a fade-in.
 */
void CC3BoundingVolumeDisplayNode::setOpacity( CCOpacity opacity )
{
	CC3NodeBoundingVolume* bv = NULL;
	CC3Node* parent = getParent();
	if ( parent )
		bv = parent->getBoundingVolume();

	super::setOpacity( bv ? MIN(opacity, bv->getDisplayNodeOpacity()) : opacity );
}

bool CC3BoundingVolumeDisplayNode::shouldIncludeInDeepCopy()
{
	return false; 
}

bool CC3BoundingVolumeDisplayNode::shouldDrawDescriptor()
{
	return true; 
}

void CC3BoundingVolumeDisplayNode::setShouldDrawDescriptor( bool shouldDraw )
{

}

bool CC3BoundingVolumeDisplayNode::shouldDrawWireframeBox()
{
	return true; 
}

void CC3BoundingVolumeDisplayNode::setShouldDrawWireframeBox( bool shouldDraw )
{

}

bool CC3BoundingVolumeDisplayNode::shouldDrawLocalContentWireframeBox()
{
	return true; 
}

void CC3BoundingVolumeDisplayNode::setShouldDrawLocalContentWireframeBox( bool shouldDraw )
{

}

bool CC3BoundingVolumeDisplayNode::shouldContributeToParentBoundingBox()
{
	return false; 
}

bool CC3BoundingVolumeDisplayNode::shouldDrawBoundingVolume()
{
	return false; 
}

void CC3BoundingVolumeDisplayNode::setShouldDrawBoundingVolume( bool shouldDraw )
{

}

// Overridden so that not touchable unless specifically set as such
bool CC3BoundingVolumeDisplayNode::isTouchable()
{
	return (isVisible() || m_shouldAllowTouchableWhenInvisible) && isTouchEnabled();
}

// Overridden so that can still be visible if parent is invisible, unless explicitly turned off.
bool CC3BoundingVolumeDisplayNode::isVisible()
{
	return m_visible; 
}

bool CC3Fog::init()
{
	_attenuationMode = GL_EXP2;
	_performanceHint = GL_DONT_CARE;
	_density = 1.0f;
	_startDistance = 0.0f;
	_endDistance = 1.0f;
	setDiffuseColor( kCCC4FLightGray );
	setShouldDrawInClipSpace( true );

	return true;
}

CC3Fog* CC3Fog::fog()
{
	CC3Fog* pFog = new CC3Fog;
	pFog->init();
	pFog->autorelease();

	return pFog;
}

void CC3Fog::populateFrom( CC3Fog* another )
{
	super::populateFrom( another );
	
	_attenuationMode = another->getAttenuationMode();
	_performanceHint = another->getPerformanceHint();
	_density = another->getDensity();
	_startDistance = another->getStartDistance();
	_endDistance = another->getEndDistance();
}

CCObject* CC3Fog::copyWithZone( CCZone* zone )
{
	CC3Fog* pVal = new CC3Fog;
	pVal->init();
	pVal->populateFrom( this );
	pVal->addCopiesOfChildrenFrom( this ); 

	return pVal;
}

GLenum	CC3Fog::getAttenuationMode()
{
	return _attenuationMode;
}

void CC3Fog::setAttenuationMode( GLenum mode )
{
	_attenuationMode = mode;
}

GLenum	CC3Fog::getPerformanceHint()
{
	return _performanceHint;
}

void CC3Fog::setPerformanceHint( GLenum hint )
{
	_performanceHint = hint;
}

GLfloat	CC3Fog::getDensity()
{
	return _density;
}

void CC3Fog::setDensity( GLfloat density )
{
	_density = density;
}

GLfloat CC3Fog::getStartDistance()
{
	return _startDistance;
}

void CC3Fog::setStartDistance( GLfloat startDistance )
{
	_startDistance = startDistance;
}

GLfloat CC3Fog::getEndDistance()
{
	return _endDistance;
}

void CC3Fog::setEndDistance( GLfloat endDistance )
{
	_endDistance = endDistance;
}

NS_COCOS3D_END
