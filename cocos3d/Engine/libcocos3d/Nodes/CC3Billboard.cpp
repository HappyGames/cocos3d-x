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

CC3Billboard::CC3Billboard()
{
	_billboard = NULL;
	setBillboard( NULL );
}

CC3Billboard::~CC3Billboard()
{
	setBillboard( NULL );
}

bool CC3Billboard::isBillboard()
{
	return true; 
}

CCNode* CC3Billboard::getBillboard()
{
	return _billboard;
}

void CC3Billboard::setBillboard( CCNode* aCCNode )
{
	if (aCCNode == _billboard) 
		return;							// Don't do anything if it's the same 2D billboard...
										// ...otherwise it will be detached from scheduler.
	// Old 2D billboard
	if ( _billboard )
	{
		_billboard->onExit();			// Turn off running state and pause activity.
		_billboard->cleanup();			// Detach billboard from scheduler and actions.
		_billboard->release();
	}

	// New 2D billboard
	_billboard = aCCNode;

	if ( _billboard )
	{
		_billboard->retain();
		_billboard->setVisible( isVisible() );
	}
	
	// Retrieve the blend function from the 2D node and align this 3D node's material with it.
#pragma _NOTE_TODO( "Bad behavior here!" )
	CCBlendProtocol* pVal = dynamic_cast<CCBlendProtocol*>(aCCNode);
	if ( pVal )
		setBlendFunc( pVal->getBlendFunc() );

	normalizeBillboardScaleToDevice();

	if ( isRunning() )
		start2DBillboard();	// If running already, start scheduled activities on new billboard
}

CCPoint CC3Billboard::getOffsetPosition()
{
	return _offsetPosition;
}

GLfloat CC3Billboard::getUnityScaleDistance()
{
	return _unityScaleDistance;
}

CCPoint CC3Billboard::getMaximumBillboardScale()
{
	return _maximumBillboardScale;
}

CCPoint CC3Billboard::getMinimumBillboardScale()
{
	return _minimumBillboardScale;
}

void CC3Billboard::setShouldDrawAs2DOverlay( bool drawAsOverlay )
{
	_shouldDrawAs2DOverlay = drawAsOverlay;
	normalizeBillboardScaleToDevice();
}

bool CC3Billboard::shouldDrawAs2DOverlay()
{
	return _shouldDrawAs2DOverlay;
}

bool CC3Billboard::shouldNormalizeScaleToDevice()
{
	return _shouldNormalizeScaleToDevice;
}

void CC3Billboard::setShouldNormalizeScaleToDevice( bool normalizeToDevice )
{
	_shouldNormalizeScaleToDevice = normalizeToDevice;
	normalizeBillboardScaleToDevice();
}

bool CC3Billboard::shouldAlwaysMeasureBillboardBoundingRect()
{
	return _shouldAlwaysMeasureBillboardBoundingRect;
}

bool CC3Billboard::shouldMaximizeBillboardBoundingRect()
{
	return _shouldAlwaysMeasureBillboardBoundingRect;
}

/**
 * If in 3D mode and should be normalized, force scale of billboard
 * to a factor determined by the type of billboard.
 */
void CC3Billboard::normalizeBillboardScaleToDevice()
{
	if (!_shouldDrawAs2DOverlay && _shouldNormalizeScaleToDevice)
	{
		if ( _billboard )
			_billboard->setScale( /*_billboard->getBillboard3DContentScaleFactor()*/1.f );
	}
}

/** Returns whether the bounding rectangle needs to be measured on each update pass. */
bool CC3Billboard::hasDynamicBoundingRect()
{
	return (_shouldDrawAs2DOverlay
			|| _shouldAlwaysMeasureBillboardBoundingRect
			|| _shouldMaximizeBillboardBoundingRect);
}

CCRect CC3Billboard::getBillboardBoundingRect()
{
	if (_billboard && (hasDynamicBoundingRect() || _billboardBoundingRect.equals(CCRectZero))) 
	{	
		CCRect currRect = measureBillboardBoundingRect();
		
		if (_shouldMaximizeBillboardBoundingRect && !_billboardBoundingRect.equals(CCRectZero))
			setBillboardBoundingRect( CCRectUnion(_billboardBoundingRect, currRect) );
		else
			setBillboardBoundingRect( currRect );
		
		//LogTrace(@"%@ billboard bounding rect updated to %@", [self class], NSStringFromCGRect(_billboardBoundingRect));
	}
	return _billboardBoundingRect;
}

/**
 * If we're drawning in 2D, simply get the 2D node's Cocos2D bounding box.
 * If we're drawing in 3D, measure the 2D nodes bounding box using an extension method.
 */
CCRect CC3Billboard::measureBillboardBoundingRect()
{
	return _shouldDrawAs2DOverlay
		? getBoundingBoxInPixels( _billboard )
		: measureBoundingBoxInPixels( _billboard );

	return CCRectZero;
}

/** If the bounding mesh exists, update it from the new bounding rect. */
void CC3Billboard::setBillboardBoundingRect( const CCRect& aRect )
{
	_billboardBoundingRect = aRect;
	updateBoundingMesh();
}

void CC3Billboard::resetBillboardBoundingRect()
{
	setBillboardBoundingRect( CCRectZero ); 
}

bool CC3Billboard::shouldUpdateUnseenBillboard()
{
	return _shouldUpdateUnseenBillboard;
}

void CC3Billboard::setShouldUpdateUnseenBillboard( bool update )
{
	_shouldUpdateUnseenBillboard = update;
}

/** Calculate bounding box from bounding rect of 2D node. */
CC3Box CC3Billboard::getLocalContentBoundingBox()
{
	CCRect bRect = getBillboardBoundingRect();
	return CC3BoxMake(bRect.getMinX(), bRect.getMinY(), 0.0,
							  bRect.getMaxX(), bRect.getMaxY(), 0.0);
}

void CC3Billboard::setVisible( bool isVisible )
{
	super::setVisible( isVisible );
	_billboard->setVisible( isVisible );
}

/** Only touchable if drawing in 3D. */
bool CC3Billboard::isTouchable()
{
	return (!_shouldDrawAs2DOverlay) && super::isTouchable(); 
}

/** Overridden to ignore lighting, since Cocos2D nodes have no normals. */
void CC3Billboard::setMaterial( CC3Material* aMaterial )
{
	super::setMaterial( aMaterial );
	aMaterial->setShouldUseLighting( false );
}

/** Returns color of billboard if it has a color, otherwise falls back to superclass implementation. */
CCColorRef CC3Billboard::getColor()
{
	if ( _billboard )
	{
		CCSprite* pSprt = dynamic_cast<CCSprite*>( _billboard );
		if ( pSprt )
			return pSprt->getColor();
	}

	return super::getColor();
}
//
/** Also sets color of billboard if it can be set. */
void CC3Billboard::setColor( CCColorRef color )
{
	if ( _billboard )
	{
		CCSprite* pSprt = dynamic_cast<CCSprite*>( _billboard );
		if ( pSprt )
			pSprt->setColor( color );
	}

	super::setColor( color );
}
//
///** Returns opacity of billboard if it has an opacity, otherwise falls back to superclass implementation. */
//-(CCOpacity) opacity {
//	return ([_billboard conformsToProtocol: @protocol(CCRGBAProtocol)])
//				? [((id<CCRGBAProtocol>)_billboard) opacity]
//				: [super opacity];
//}
//
///** Also sets opacity of billboard if it can be set. */
//void setOpacity: (CCOpacity) opacity {
//	if ([_billboard conformsToProtocol: @protocol(CCRGBAProtocol)])
//		[((id<CCRGBAProtocol>)_billboard) setOpacity: opacity];
//	[super setOpacity: opacity];
//}

void CC3Billboard::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	{
		setColor( CCColorRefFromCCC4F(kCCC4FWhite) );
		setBillboard( NULL );
		_billboardBoundingRect = CCRectZero;
		_offsetPosition = CCPointZero;
		_minimumBillboardScale = CCPointZero;
		_maximumBillboardScale = CCPointZero;
		_unityScaleDistance = 0.0;
		_shouldNormalizeScaleToDevice = true;
		_shouldDrawAs2DOverlay = false;
		_shouldAlwaysMeasureBillboardBoundingRect = false;
		_shouldMaximizeBillboardBoundingRect = false;
		_textureUnitIndex = 0;
		_shouldUpdateUnseenBillboard = true;
		_billboardIsPaused = false;
	}
}

void CC3Billboard::initWithTag( GLuint aTag, const std::string& aName, CCNode* a2DNode )
{
	initWithTag( aTag, aName );
	{
		setBillboard( a2DNode );
	}
}

void CC3Billboard::initWithBillboard( CCNode* a2DNode )
{
	init();
	setBillboard( a2DNode );
}

CC3Billboard* CC3Billboard::nodeWithBillboard( CCNode* a2DNode )
{
	CC3Billboard* billboard = new CC3Billboard;
	billboard->initWithBillboard( a2DNode );
	billboard->autorelease();
	return billboard;
}

void CC3Billboard::initWithName( const std::string& aName, CCNode* a2DNode )
{
	initWithName( aName );
	setBillboard( a2DNode );
}

void CC3Billboard::initWithName( const std::string& aName )
{
	super::initWithName( aName );
}

CC3Billboard* CC3Billboard::nodeWithName( const std::string& aName, CCNode* a2DNode )
{
	CC3Billboard* billboard = new CC3Billboard;
	billboard->initWithName( aName, a2DNode );
	billboard->autorelease();
	return billboard;
}

void CC3Billboard::populateFrom( CC3Billboard* another )
{
	super::populateFrom( another );
	
	// Since the billboard can be any kind of CCNode, check if it supports NSCopying.
	// If it does...copy it...otherwise don't attach it. Attaching a single CCNode to multiple
	// CC3Billboards is fraught with peril, because the position and scale of the CCNode will
	// be set by multiple CC3Billboards, and the last one to do so is where the CCNode will be
	// drawn (but over and over, once per CC3Billboard that references it).
	CCNode* bb = another->getBillboard();
	/// setBillboard( (CCNode*)bb->copy()->autorelease() );
	
	_billboardBoundingRect = another->getBillboardBoundingRect();
	_offsetPosition = another->getOffsetPosition();
	_unityScaleDistance = another->getUnityScaleDistance();
	_minimumBillboardScale = another->getMinimumBillboardScale();
	_maximumBillboardScale = another->getMaximumBillboardScale();
	_shouldNormalizeScaleToDevice = another->shouldNormalizeScaleToDevice();
	_shouldDrawAs2DOverlay = another->shouldDrawAs2DOverlay();
	_shouldAlwaysMeasureBillboardBoundingRect = another->shouldAlwaysMeasureBillboardBoundingRect();
	_shouldMaximizeBillboardBoundingRect = another->shouldMaximizeBillboardBoundingRect();
}

CCObject* CC3Billboard::copyWithZone( CCZone* zone )
{
	//CCAssert( false, "Please do not copy CC3Billboard" );

	CC3Billboard* pVal = new CC3Billboard;
	pVal->init();
	pVal->populateFrom( this );
	pVal->addCopiesOfChildrenFrom( this );

	return pVal;
}

/** Ensure that the bounding rectangle mesh has been created. */
void CC3Billboard::ensureBoundingMesh()
{
	if ( !_mesh ) 
		populateAsBoundingRectangle(); 
}

void CC3Billboard::populateAsBoundingRectangle()
{
	CC3Vector* vertices;		// Array of simple vertex location data

	// Start with default initial values
	GLfloat xMin = 0.0f;
	GLfloat xMax = 1.0f;
	GLfloat yMin = 0.0f;
	GLfloat yMax = 1.0f;
	int vCount = 4;
	
	CC3VertexLocations* locArray = CC3VertexLocations::vertexArray();
	locArray->setDrawingMode( GL_TRIANGLE_STRIP );			// Location array will do the drawing as a strip
	locArray->setVertexStride( 0 );							// Tightly packed locations only
	locArray->setElementOffset( 0 );							// Only locations
	locArray->setAllocatedVertexCapacity( vCount );
	vertices = (CC3Vector*)locArray->getVertices();
	
	// Populate vertex locations in the X-Y plane
	vertices[0] = cc3v(xMax, yMax, 0.0f);
	vertices[1] = cc3v(xMin, yMax, 0.0f);
	vertices[2] = cc3v(xMax, yMin, 0.0f);
	vertices[3] = cc3v(xMin, yMin, 0.0f);
	
	// Create mesh with vertex location array
	CC3Mesh* aMesh = CC3Mesh::mesh();
	aMesh->setVertexLocations( locArray );
	setMesh( aMesh );

	updateBoundingMesh();
}

/** Starts the 2D billboard. */
void CC3Billboard::start2DBillboard()
{
	// _billboard->paused = NO;
	if ( _billboard )
		_billboard->onEnter();
}

/** Stops the 2D billboard. */
void CC3Billboard::stop2DBillboard()
{
	//_billboard.paused = YES;
	if ( _billboard )
		_billboard->onExit();
}

void CC3Billboard::alignToCamera( CC3Camera* camera )
{
	if (camera && _billboard) 
	{
		if (_shouldDrawAs2DOverlay)
			align2DToCamera( camera );
		else
			align3DToCamera( camera );
	}
}

/**
 * When drawing in 2D, this method is invoked automatically to dynamically scale the
 * node so that it appears with the correct perspective. This is required because
 * when drawing as a 2D overlay, the node will not otherwise be drawn with the
 * perspective of the 3D billboard's location.
 */
void CC3Billboard::align2DToCamera( CC3Camera* camera )
{
	// Use the camera to project the 3D location of this node
	// into 2D and then set the billboard to that position
	camera->projectNode( this );
	CCPoint pPos = getProjectedPosition();
	_billboard->setPosition( ccpAdd(pPos, _offsetPosition) );
	
	CCPoint newBBScale;
	// If only one non-zero scale is allowed (min == max), ensure that the billboard is set to that scale
	if (!_minimumBillboardScale.equals(CCPointZero)
		&& _maximumBillboardScale.equals(_minimumBillboardScale)) 
	{
		newBBScale = _minimumBillboardScale;
		//LogTrace(@"Projecting billboard %@ to %@ with fixed scaling %@", self,
		//		 NSStringFromCGPoint(pPos), NSStringFromCGPoint(newBBScale));
	} 
	else 
	{
		// Calc how much to scale the billboard by comparing distance from camera to billboard
		// and camera to the defined unity-scale distance. Neither may be smaller than the near
		// clipping plane.
		GLfloat camNear = camera->getNearClippingDistance();
		GLfloat camDist = MAX( getGlobalLocation().distance( camera->getGlobalLocation() ), camNear);
		GLfloat unityDist = MAX(getUnityScaleDistance(), camNear);
		GLfloat distScale = unityDist / camDist;
		newBBScale.x = distScale;
		newBBScale.y = distScale;
		
		// Ensure result is within any defined min and max scales
		newBBScale.x = MAX(newBBScale.x, _minimumBillboardScale.x);
		newBBScale.y = MAX(newBBScale.y, _minimumBillboardScale.y);
		
		newBBScale.x = (_maximumBillboardScale.x != 0.0) ? MIN(newBBScale.x, _maximumBillboardScale.x) : newBBScale.x;
		newBBScale.y = (_maximumBillboardScale.y != 0.0) ? MIN(newBBScale.y, _maximumBillboardScale.y) : newBBScale.y;
		
		// Factor in the scale of this CC3Billboard node.
		CC3Vector myScale = getScale();
		newBBScale.x *= myScale.x;
		newBBScale.y *= myScale.y;
		//LogTrace(@"Projecting billboard %@ to %@, scaled to %@ using distance %.2f and unity distance %.2f",
		//		 self, NSStringFromCGPoint(pPos), NSStringFromCGPoint(newBBScale), camDist, unityDist);
	}
	
	// If consistency across devices is desired, adjust size of 2D billboard so that
	// it appears the same size relative to 3D artifacts across all device resolutions
	if (_shouldNormalizeScaleToDevice) 
		newBBScale = ccpMult(newBBScale, deviceScaleFactor());
	
	// Set the new scale only if it has changed. 
	if (_billboard->getScaleX() != newBBScale.x) 
		_billboard->setScaleX( newBBScale.x );
	if (_billboard->getScaleY() != newBBScale.y) 
		_billboard->setScaleY( newBBScale.y );
}

/**
 * When drawing in 3D, thd 2D node will automatically be drawn with the correct
 * perspective projection, but this method is invoked automatically to enforce
 * the minimum and maximum scales.
 */
void CC3Billboard::align3DToCamera( CC3Camera* camera )
{
	// Don't waste time if no min or max scale has been set.
	if (_minimumBillboardScale.equals(CCPointZero) &&
		_maximumBillboardScale.equals(CCPointZero)) 
		return;

	GLfloat camNear = camera->getNearClippingDistance();
	GLfloat unityDist = MAX(getUnityScaleDistance(), camNear);
	GLfloat camDist = MAX(getGlobalLocation().distance(camera->getGlobalLocation()), camNear);

	CCPoint newBBScale = ccp(_billboard->getScaleX(), _billboard->getScaleY());

	if (_minimumBillboardScale.x > 0.0) {
		GLfloat minScaleDistX = unityDist / _minimumBillboardScale.x;
		newBBScale.x = (camDist > minScaleDistX) ? (camDist / minScaleDistX) : 1.0f;
	}
	
	if (_minimumBillboardScale.y > 0.0) {
		GLfloat minScaleDistY = unityDist / _minimumBillboardScale.y;
		newBBScale.y = (camDist > minScaleDistY) ? (camDist / minScaleDistY) : 1.0f;
	}
	
	if (_maximumBillboardScale.x > 0.0) {
		GLfloat maxScaleDistX = unityDist / _maximumBillboardScale.x;
		newBBScale.x = (camDist < maxScaleDistX) ? (camDist / maxScaleDistX) : 1.0f;
	}
	
	if (_maximumBillboardScale.y > 0.0) {
		GLfloat maxScaleDistY = unityDist / _maximumBillboardScale.y;
		newBBScale.y = (camDist < maxScaleDistY) ? (camDist / maxScaleDistY) : 1.0f;
	}
	
	// Set the new scale only if it has changed. 
	if (_billboard->getScaleX() != newBBScale.x) 
		_billboard->setScaleX( newBBScale.x );

	if (_billboard->getScaleY() != newBBScale.y) 
		_billboard->setScaleY( newBBScale.y );
}

#define kCC3DeviceScaleFactorBase 480.0f
static GLfloat _deviceScaleFactor = 0.0f;

GLfloat CC3Billboard::deviceScaleFactor()
{
	if (_deviceScaleFactor == 0.0f) 
	{
		CCSize winSz = CCDirector::sharedDirector()->getWinSize();
		_deviceScaleFactor = MAX(winSz.height, winSz.width) / kCC3DeviceScaleFactorBase;
	}
	return _deviceScaleFactor;
}

CC3NodeBoundingArea* CC3Billboard::getBoundingVolume()
{
	return (CC3NodeBoundingArea*)super::getBoundingVolume(); 
}

/** Verify that the bounding volume is of the right type. */
void CC3Billboard::setBoundingVolume( CC3NodeBoundingArea* boundingVolume )
{
	//CC3Assert( [boundingVolume isKindOfClass: [CC3NodeBoundingArea class]],
	//		  @"%@ requires that the boundingVolume property be of type CC3NodeBoundingArea.", self);
	super::setBoundingVolume( boundingVolume );
}

CC3NodeBoundingVolume* CC3Billboard::defaultBoundingVolume()
{
	return CC3BillboardBoundingBoxArea::boundingVolume();
}

/** Overridden to return YES only if this billboard should draw in 3D. */
bool CC3Billboard::hasLocalContent()
{
	return !_shouldDrawAs2DOverlay; 
}

/** 
 * Overridden to possibly turn scheduled update activity on or off whenever this node
 * transitions from being inside or outside the camera frustum.
 */
bool CC3Billboard::doesIntersectFrustum( CC3Frustum* aFrustum )
{
	bool doesIntersect = super::doesIntersectFrustum( aFrustum );

	if (_billboard && !_shouldUpdateUnseenBillboard) 
	{
		if (doesIntersect && _billboardIsPaused) 
		{
			CCDirector::sharedDirector()->getScheduler()->resumeTarget( _billboard );
			_billboardIsPaused = false;
		} 
		else if (!doesIntersect && !_billboardIsPaused) 
		{
			CCDirector::sharedDirector()->getScheduler()->pauseTarget( _billboard );
			_billboardIsPaused = true;
		}
	}

	return doesIntersect;
}

/** Only intersect frustum when drawing in 3D mode. */
bool CC3Billboard::doesIntersectBoundingVolume( CC3BoundingVolume* otherBoundingVolume )
{
	return (!_shouldDrawAs2DOverlay) && super::doesIntersectBoundingVolume(otherBoundingVolume);
}

/**
 * During normal drawing, establish 2D drawing environment.
 * Don't configure anything if painting for node picking.
 */
void CC3Billboard::applyMaterialWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	super::applyMaterialWithVisitor( visitor );
	if (visitor->shouldDecorateNode()) 
	{
		CC3OpenGL* gl = visitor->getGL();
		gl->alignFor2DDrawingWithVisitor( visitor );
		gl->setDepthMask( !_shouldDisableDepthMask );
	}
}

/** The Cocos2D CCNode will supply its own shaders, but still need shader during node picking. */
void CC3Billboard::applyShaderProgramWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	if ( visitor->shouldDecorateNode()) 
		return;
	
	super::applyShaderProgramWithVisitor( visitor );
}

/**
 * During normal drawing, draw the Cocos2D node.
 * When painting for node picking, update the bounding box mesh vertices and draw it.
 */
void CC3Billboard::drawMeshWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	if ( visitor->shouldDecorateNode() )
	{
		if ( _billboard )
			_billboard->visit();
	} 
	else 
	{
		// We're drawing a colored box to allow this node to be picked by a touch.
		// This is done by creating and drawing an underlying rectangle mesh that
		// is sized the same as the 2D node.
		ensureBoundingMesh();
		super::drawMeshWithVisitor( visitor );
	}
}

/**
 * During normal drawing, restore 3D drawing environment.
 * Don't configure anything if painting for node picking.
 */
void CC3Billboard::cleanupDrawingParameters( CC3NodeDrawingVisitor* visitor )
{
	if (visitor->shouldDecorateNode()) 
		visitor->getGL()->alignFor3DDrawingWithVisitor(visitor);
	super::cleanupDrawingParameters(visitor);
}

/** If the bounding mesh exists, update its vertices to match the bounding box of the 2D node. */
void CC3Billboard::updateBoundingMesh()
{
	if (_mesh) 
	{
		CCRect bRect = getBillboardBoundingRect();
		GLfloat xMin = bRect.getMinX();
		GLfloat xMax = bRect.getMaxX();
		GLfloat yMin = bRect.getMinX();
		GLfloat yMax = bRect.getMaxY();
		_mesh->setVertexLocation( cc3v(xMax, yMax, 0.0), 0 );
		_mesh->setVertexLocation( cc3v(xMin, yMax, 0.0), 1 );
		_mesh->setVertexLocation( cc3v(xMax, yMin, 0.0), 2 );
		_mesh->setVertexLocation( cc3v(xMin, yMin, 0.0), 3 );
	}
}

bool CC3Billboard::doesIntersectBounds( const CCRect& bounds )
{
	if (m_pBoundingVolume) 
	{
		bool intersects = getBoundingVolume()->doesIntersectBounds( bounds );
		//LogTrace(@"%@ bounded by %@ %@ %@", self, _boundingVolume,
		//			  (intersects ? @"intersects" : @"does not intersect"), NSStringFromCGRect(bounds));

		// Uncomment and change name to verify culling:
//		if (!intersects && ([self.name isEqualToString: @"MyNodeName"])) {
//			LogDebug(@"%@ bounded by %@ does not intersect %@",
//						  self, _boundingVolume, NSStringFromCGRect(bounds));
//		}
		return intersects;
	}
	return true;
}

/*
void CC3Billboard::draw2dWithinBounds( const CCRect& bounds, CCRenderer* renderer, CC3NodeDrawingVisitor* visitor )
{
	if( !(_shouldDrawAs2DOverlay && self.visible && [self doesIntersectBounds: bounds ]) ) 
		return;

	CC3OpenGL* gl = visitor.gl;
	[gl pushGroupMarkerC: "Draw overlay 2D billboard"];
	[_billboard visit: renderer parentTransform: visitor.layerTransformMatrix];
	[gl popGroupMarker];
}
*/

void CC3Billboard::resumeAllActions()
{
	super::resumeAllActions();
	start2DBillboard();
}

void CC3Billboard::pauseAllActions()
{
	super::pauseAllActions();
	stop2DBillboard();
}

void CC3Billboard::setShouldDrawLocalContentWireframeBox( bool shouldDraw )
{
	super::setShouldDrawLocalContentWireframeBox( shouldDraw );

	// If we're adding a wireframe and this node has a dynamic boundary,
	// fetch the new wireframe node from the child nodes and set it to
	// measure the local content of this node on each update.
	if (shouldDraw && hasDynamicBoundingRect()) 
	{
		getLocalContentWireframeBoxNode()->setShouldAlwaysMeasureParentBoundingBox( true );
	}
}


void CC3Billboard::addShadowVolumesForLight( CC3Light* aLight )
{
	if (!_mesh) 
		populateAsBoundingRectangle();

	super::addShadowVolumesForLight( aLight );

	CC3ShadowVolumeMeshNode* sv = getShadowVolumeForLight( aLight );
	if ( sv )
	{
		sv->setShouldShadowBackFaces( true );
		sv->setShadowOffsetUnits( 0 );
		sv->setShadowVolumeVertexOffsetFactor( kCC3DefaultShadowVolumeVertexOffsetFactor );
	}
}

CCRect CC3Billboard::getBoundingBoxInPixels( CCNode* pNode )
{
	return CC_RECT_PIXELS_TO_POINTS( pNode->boundingBox() );
}

CCRect CC3Billboard::measureBoundingBoxInPixels( CCNode* pNode )
{
	return getBoundingBoxInPixels( pNode );
}

static inline CCRect makeRectFromQuad( ccV3F_C4B_T2F_Quad quad )
{
	float fBlx = MIN(quad.bl.vertices.x, MIN(quad.br.vertices.x, MIN(quad.tl.vertices.x, quad.tr.vertices.x)));
	float fBly = MIN(quad.bl.vertices.y, MIN(quad.br.vertices.y, MIN(quad.tl.vertices.y, quad.tr.vertices.y)));
	float fTrx = MAX(quad.bl.vertices.x, MAX(quad.br.vertices.x, MAX(quad.tl.vertices.x, quad.tr.vertices.x)));
	float fTry = MAX(quad.bl.vertices.y, MAX(quad.br.vertices.y, MAX(quad.tl.vertices.y, quad.tr.vertices.y)));

	return CCRectMake( fBlx, fBly, fTrx - fBlx, fTry - fBly );
}

CCRect CC3Billboard::measureBoundingBoxInPixels( CCParticleSystemQuad* pNode )
{
	const ccV3F_C4B_T2F_Quad* pQuads = pNode->getQuads();
	unsigned int particleIndex = pNode->getParticleIndex();

	if ( pQuads == NULL || particleIndex == 0 )
		return CCRectZero;

	// Get the first quad as a starting point
	CCRect boundingRect = makeRectFromQuad( pQuads[0] );

	// Iterate through all the remaining quads, taking the union of the current
	// bounding rect and each quad to find the rectangle that bounds all the quads.
	for( unsigned int i = 1; i < particleIndex; i++ )
		boundingRect = CCRectUnion( boundingRect, makeRectFromQuad( pQuads[i] ) );

	return boundingRect;
}

CC3BillboardBoundingBoxArea* CC3BillboardBoundingBoxArea::boundingVolume()
{
	CC3BillboardBoundingBoxArea* pVolume = new CC3BillboardBoundingBoxArea;
	pVolume->init();
	pVolume->autorelease();

	return pVolume;
}

CC3Plane* CC3BillboardBoundingBoxArea::getPlanes()
{
	updateIfNeeded();
	return _planes;
}

GLuint CC3BillboardBoundingBoxArea::getPlaneCount()
{
	return 6; 
}

CC3Vector* CC3BillboardBoundingBoxArea::getVertices()
{
	updateIfNeeded();
	return _vertices;
}

GLuint CC3BillboardBoundingBoxArea::getVertexCount()
{
	return 4; 
}

/**
 * Return the bounding rectangle of the 2D node held in the CC3Billboard node.
 * If its not valid, return a zero rectangle.
 */
CCRect CC3BillboardBoundingBoxArea::getBillboardBoundingRect()
{
	CCRect bRect = ((CC3Billboard*)_node)->getBillboardBoundingRect();
	//LogTrace(@"%@ bounding rect: %@", _node, NSStringFromCGRect(bRect));
	return bRect.equals(CCRectZero) ? CCRectZero : bRect;
}

/** Transform the bounding rectangle of the 2D node on the X-Y plane into 3D. */
void CC3BillboardBoundingBoxArea::transformVolume()
{
	super::transformVolume();

	// Get the corners of the CCNode bounding box
	CCRect br = getBillboardBoundingRect();
	CCPoint bbMin = ccp(br.getMinX(), br.getMinY());
	CCPoint bbMax = ccp(br.getMaxX(), br.getMaxY());
	
	// Construct all 4 corner vertices of the local bounding box and transform each to global coordinates
	_vertices[0] = _node->getGlobalTransformMatrix()->transformLocation( cc3v(bbMin.x, bbMin.y, 0.0) );
	_vertices[1] = _node->getGlobalTransformMatrix()->transformLocation( cc3v(bbMin.x, bbMax.y, 0.0) );
	_vertices[2] = _node->getGlobalTransformMatrix()->transformLocation( cc3v(bbMax.x, bbMin.y, 0.0) );
	_vertices[3] = _node->getGlobalTransformMatrix()->transformLocation( cc3v(bbMax.x, bbMax.y, 0.0) );
	
	/*LogTrace(@"%@ bounding volume transformed %@ MinMax(%@, %@) to (%@, %@, %@, %@)", _node,
			 NSStringFromCGRect(br),
			 NSStringFromCGPoint(bbMin), NSStringFromCGPoint(bbMax), 
			 NSStringFromCC3Vector(_vertices[0]), NSStringFromCC3Vector(_vertices[1]),
			 NSStringFromCC3Vector(_vertices[2]), NSStringFromCC3Vector(_vertices[3]));*/
}

/**
 * Constructs the six box face planes from normals and vertices.
 * The plane normals are the transformed face normals of the original box.
 * The vertices are the transformed min-max corners of the rectangle.
 */
void CC3BillboardBoundingBoxArea::buildPlanes()
{
	CC3Vector normal;
	CC3Matrix* tMtx = _node->getGlobalTransformMatrix();
	CC3Vector bbMin = _vertices[0];
	CC3Vector bbMax = _vertices[3];
	
	// Front plane
	normal = tMtx->transformDirection(CC3Vector::kCC3VectorUnitZPositive).normalize();
	_planes[0] = CC3PlaneFromNormalAndLocation(normal, bbMax);
	
	// Back plane
	normal = tMtx->transformDirection(CC3Vector::kCC3VectorUnitZNegative).normalize();
	_planes[1] = CC3PlaneFromNormalAndLocation(normal, bbMin);
	
	// Right plane
	normal = tMtx->transformDirection(CC3Vector::kCC3VectorUnitXPositive).normalize();
	_planes[2] = CC3PlaneFromNormalAndLocation(normal, bbMax);
	
	// Left plane
	normal = tMtx->transformDirection(CC3Vector::kCC3VectorUnitXNegative).normalize();
	_planes[3] = CC3PlaneFromNormalAndLocation(normal, bbMin);
	
	// Top plane
	normal = tMtx->transformDirection(CC3Vector::kCC3VectorUnitYPositive).normalize();
	_planes[4] = CC3PlaneFromNormalAndLocation(normal, bbMax);
	
	// Bottom plane
	normal = tMtx->transformDirection(CC3Vector::kCC3VectorUnitYNegative).normalize();
	_planes[5] = CC3PlaneFromNormalAndLocation(normal, bbMin);
}

CC3Vector CC3BillboardBoundingBoxArea::getLocationOfRayIntesection( const CC3Ray& localRay )
{
	if (_shouldIgnoreRayIntersection) 
		return CC3Vector::kCC3VectorNull;

	// Get the location where the ray intersects the plane of the billboard,
	// which is the Z=0 plane, and ensure that the ray is not parallel to that plane.
	CC3Plane bbPlane = CC3PlaneFromNormalAndLocation(CC3Vector::kCC3VectorUnitZPositive, CC3Vector::kCC3VectorZero);
	CC3Vector4 pLoc4 = CC3RayIntersectionWithPlane(localRay, bbPlane);
	if ( pLoc4.isNull() ) 
		return CC3Vector::kCC3VectorNull;
	
	// Convert the location to a 2D point on the Z=0 plane, and check
	// if that point is inside the rectangular bounds of the billboard.
	bool intersects = getBillboardBoundingRect().containsPoint( ccp(pLoc4.x, pLoc4.y) );

	// Return the 3D puncture location, or null if the ray did not intersect the boundary rectangle
	return intersects ? pLoc4.cc3Vector() : CC3Vector::kCC3VectorNull;
}

std::string CC3BillboardBoundingBoxArea::displayNodeNameSuffix()
{
	return "BV-Billboard"; 
}

CCColorRef CC3BillboardBoundingBoxArea::getDisplayNodeColor()
{
	return CCColorRefFromCCC4F(kCCC4FCyan); 
}	// Cyan

CCOpacity CC3BillboardBoundingBoxArea::getDisplayNodeOpacity()
{
	return (CCOpacity)(kCCOpacityFull * 0.25); 
}			// Cyan is heavy...reduce to 25% opacity

/** Get the mesh from the rectangular bounding mesh of the billboard node, which is used for node picking. */
void CC3BillboardBoundingBoxArea::populateDisplayNode()
{
	CC3Billboard* bbNode = (CC3Billboard*)_node;
	bbNode->ensureBoundingMesh();
	CC3BoundingVolumeDisplayNode* dn = displayNode();
	dn->setMesh( bbNode->getMesh() );
	dn->setShouldCullBackFaces( false );	// Make it a two-sided rectangle
}

CC3NodeDescriptor* CC3NodeDescriptor::nodeWithName( const std::string& aName, CCNode* a2DNode )
{
	CC3NodeDescriptor* pDescriptor = new CC3NodeDescriptor;
	pDescriptor->initWithName( aName, a2DNode );
	pDescriptor->autorelease();

	return pDescriptor;
}

void CC3ParticleSystemBillboard::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	_particleSizeAttenuation = kCC3AttenuationNone;
	_shouldDisableDepthMask = true;
}

void CC3ParticleSystemBillboard::populateFrom( CC3ParticleSystemBillboard* another )
{
	super::populateFrom( another );
	
	_particleSizeAttenuation = another->getParticleSizeAttenuation();
}

CCObject* CC3ParticleSystemBillboard::copyWithZone( CCZone* zone )
{
	CC3ParticleSystemBillboard* pVal = new CC3ParticleSystemBillboard;
	pVal->init();
	pVal->populateFrom( this );
	pVal->addCopiesOfChildrenFrom( this );

	return pVal;
}

void CC3ParticleSystemBillboard::setParticleSizeAttenuation( const CC3AttenuationCoefficients& attenuation )
{
	_particleSizeAttenuation = attenuation;
}

CC3AttenuationCoefficients CC3ParticleSystemBillboard::getParticleSizeAttenuation()
{
	return _particleSizeAttenuation;
}

bool CC3ParticleSystemBillboard::shouldTransformUnseenParticles()
{
	return shouldUpdateUnseenBillboard(); 
}

void CC3ParticleSystemBillboard::setShouldTransformUnseenParticles( bool shouldTransform )
{
	setShouldUpdateUnseenBillboard( shouldTransform );
}

/**
 * If the particle system has exhausted and it is set to auto-remove, remove this
 * node from the scene so that this node and the particle system will be released.
 */
void CC3ParticleSystemBillboard::updateBeforeTransform( CC3NodeUpdatingVisitor* visitor )
{
	if ( _billboard ) 
	{
		CCParticleSystem* ps = (CCParticleSystem*)_billboard;
		if ( ps->isAutoRemoveOnFinish() && !ps->isActive() && ps->getParticleCount() == 0 )
		{
			CC3_TRACE("[bbd]2D particle system exhausted. Removing");
			visitor->requestRemovalOf( this );
		}
	}
}

/** Overridden to add setting the point size attenuation parameters. */
void CC3ParticleSystemBillboard::configureDrawingParameters( CC3NodeDrawingVisitor* visitor )
{
	super::configureDrawingParameters( visitor );
	visitor->getGL()->setPointSizeAttenuation( _particleSizeAttenuation );
}

CC3ParticleSystemBillboard* CC3ParticleSystemBillboard::nodeWithName( const std::string& aName, CCNode* a2DNode )
{
	CC3ParticleSystemBillboard* pBillboard = new CC3ParticleSystemBillboard;
	pBillboard->initWithName( aName, a2DNode );
	pBillboard->autorelease();

	return pBillboard;
}
//
//
//#pragma mark -
//#pragma mark CC3NodeDescriptor
//
//@implementation CC3NodeDescriptor
//
//-(CC3Box) localContentBoundingBox { return kCC3BoxNull; }
//
//-(CC3Box) globalLocalContentBoundingBox { return kCC3BoxNull; }
//
//-(BOOL) shouldIncludeInDeepCopy { return NO; }
//
//-(BOOL) shouldDrawDescriptor { return YES; }
//
//void setShouldDrawDescriptor: (BOOL) shouldDraw {}
//
//-(BOOL) shouldDrawWireframeBox { return YES; }
//
//void setShouldDrawWireframeBox: (BOOL) shouldDraw {}
//
//-(BOOL) shouldDrawLocalContentWireframeBox { return YES; }
//
//void setShouldDrawLocalContentWireframeBox: (BOOL) shouldDraw {}
//
//-(BOOL) shouldContributeToParentBoundingBox { return NO; }
//
//-(BOOL) shouldDrawBoundingVolume { return NO; }
//
//void setShouldDrawBoundingVolume: (BOOL) shouldDraw {}
//
//
//// Overridden so that not touchable unless specifically set as such
//-(BOOL) isTouchable { return self.isTouchEnabled; }
//
//// Overridden so that can still be visible if parent is invisible, unless explicitly turned off.
//-(BOOL) visible { return _visible; }
//
//
//#pragma mark Allocation and initialization
//
//-(id) initWithTag: (GLuint) aTag withName: (NSString*) aName {
//	if ( (self = [super initWithTag: aTag withName: aName]) ) {
//		_minimumBillboardScale = ccp(1.0, 1.0);
//		_maximumBillboardScale = ccp(1.0, 1.0);
//		_shouldDrawAs2DOverlay = YES;
//	}
//	return self;
//}
//
//@end
//
//
//#pragma mark -
//#pragma mark CC3Node extension for billboards
//
//@implementation CC3Node (Billboards)
//
//-(BOOL) isBillboard { return NO; }
//
//@end
//
//
//#pragma mark -
//#pragma mark CCNode extensions
//
//@implementation CCNode (CC3Billboard)
//
//-(CGFloat) billboard3DContentScaleFactor {
//#if CC3_CC2_1
//	return 1.0f / CCDirector.sharedDirector.contentScaleFactor;
//#else
//	return 1.0f;
//#endif	// CC3_CC2_1
//}
//
///** Simply return the bounding box of this node. */
//-(CGRect) measureBoundingBoxInPixels { return self.boundingBoxInPixels; }
//
//@end
//
//#if !CC3_CC2_CLASSIC
//#define CCParticleSystemQuad CCParticleSystem
//#endif	// !CC3_CC2_CLASSIC
//
//#pragma mark -
//#pragma mark CCParticleSystemQuad extensions
//
//#if COCOS2D_VERSION >= 0x020100
//#	define CC2_QUADS _quads
//#	define CC2_PARTICLE_IDX _particleIdx
//#else
//#	define CC2_QUADS quads_
//#	define CC2_PARTICLE_IDX particleIdx
//#endif
//
//// Cocos2D 1.0 and below use 2D structures for particle quad vertices
//// Cocos2D 1.1 and above use 3D structures for particle quad vertices
//#if COCOS2D_VERSION >= 0x010100
//#	define CC_PARTICLE_QUAD_TYPE ccV3F_C4B_T2F_Quad
//#else
//#	define CC_PARTICLE_QUAD_TYPE ccV2F_C4B_T2F_Quad
//#endif
//
//@implementation CCParticleSystemQuad (CC3)
//
//#if CC3_CC2_RENDER_QUEUE
//
///** Find the rectangle that envelopes the specified particle. */
//-(CGRect) makeRectFromParticle: (_CCParticle*) pParticle {
//	float ps = pParticle->size;
//	float hs = ps * 0.5f;
//	return CGRectMake(pParticle->pos.x - hs, pParticle->pos.y - hs, ps, ps);
//}
//
///** Build the bounding box to encompass the locations of all of the particles. */
//-(CGRect) measureBoundingBoxInPixels {
//	
//	// Must have at least one particle, otherwise simply return a zero rect
//	if (!_particles || _particleCount == 0) return CGRectZero;
//	
//	// Get the first particle as a starting point
//	CGRect boundingRect = [self makeRectFromParticle: &_particles[0]];
//	
//	// Iterate through all the remaining particles, taking the union of the current bounding
//	// rect and each particle rect to find the rectangle that bounds all the particles.
//	for(NSUInteger i = 1; i < _particleCount; i++)
//		boundingRect = CGRectUnion(boundingRect, [self makeRectFromParticle: &_particles[i]]);
//
//	return boundingRect;
//}
//
//#else
//
///**
// * Find the absolute bottom left and top right from all four vertices in the quad,
// * assuming that the bl and tr of the quad are nominal representations and do not
// * necessarily represent the true corners of the quad. Then create a rectangle from
// * these true bottom left and top right corners.
// */
//-(CGRect) makeRectFromQuad: (CC_PARTICLE_QUAD_TYPE) quad {
//	CC3_PUSH_NOSHADOW
//	CGFloat blx = MIN(quad.bl.vertices.x, MIN(quad.br.vertices.x, MIN(quad.tl.vertices.x, quad.tr.vertices.x)));
//	CGFloat bly = MIN(quad.bl.vertices.y, MIN(quad.br.vertices.y, MIN(quad.tl.vertices.y, quad.tr.vertices.y)));
//	CGFloat trx = MAX(quad.bl.vertices.x, MAX(quad.br.vertices.x, MAX(quad.tl.vertices.x, quad.tr.vertices.x)));
//	CGFloat try = MAX(quad.bl.vertices.y, MAX(quad.br.vertices.y, MAX(quad.tl.vertices.y, quad.tr.vertices.y)));
//	CC3_POP_NOSHADOW
//	return CGRectMake(blx, bly, trx - blx, try - bly);
//}
//
///** Build the bounding box to encompass the locations of all of the particles. */
//-(CGRect) measureBoundingBoxInPixels {
//	
//	// Must have at least one quad, otherwise simply return a zero rect
//	if (!CC2_QUADS || CC2_PARTICLE_IDX == 0) return CGRectZero;
//	
//	// Get the first quad as a starting point
//	CGRect boundingRect = [self makeRectFromQuad: CC2_QUADS[0]];
//	
//	// Iterate through all the remaining quads, taking the union of the current
//	// bounding rect and each quad to find the rectangle that bounds all the quads.
//	for(NSUInteger i = 1; i < CC2_PARTICLE_IDX; i++)
//		boundingRect = CGRectUnion(boundingRect, [self makeRectFromQuad: CC2_QUADS[i]]);
//
//	return boundingRect;
//}
//
//#endif	// CC3_CC2_RENDER_QUEUE
//
//@end
//
//
//#pragma mark -
//#pragma mark CCParticleSystemPoint extensions
//
//#if CC3_CC2_1 && CC3_IOS
//@implementation CCParticleSystemPoint (CC3)
//
///** Constructs a rectangle whose origin is at the specified vertex, and with zero size. */
//-(CGRect) makeRectFromVertex: (ccVertex2F) aVertex {
//	return CGRectMake(aVertex.x, aVertex.y, 0.0, 0.0);
//}
//
///** Build the bounding box to encompass the locations of all of the particles. */
//-(CGRect) measureBoundingBoxInPixels {
//	// Must have at least one particle
//	if (vertices && particleIdx > 0) {
//		// Get the first particle as a starting point
//		CGRect boundingRect = [self makeRectFromVertex: vertices[0].pos];
//		
//		// Iterate through all the remaining particles, taking the union of
//		// the current bounding rect and each particle location to find the
//		// rectangle that bounds all the vertices.
//		for(NSUInteger i = 1; i < particleIdx; i++) {
//			CGRect vertexRect = [self makeRectFromVertex: vertices[i].pos];
//			boundingRect = CGRectUnion(boundingRect, vertexRect);
//		}
//		LogTrace(@"%@ bounding rect measured as %@ across %u active of %u possible particles",
//				 [self class], NSStringFromCGRect(boundingRect), particleIdx, totalParticles);
//		return boundingRect;
//	} else {
//		// Otherwise simply return a zero rect
//		return CGRectZero;
//	}
//}
//
//@end

NS_COCOS3D_END
