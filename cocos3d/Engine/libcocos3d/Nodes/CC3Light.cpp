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

CC3Light::CC3Light()
{
	_cameraShadowVolume = NULL;
	_shadowCastingVolume = NULL;
	_stencilledShadowPainter = NULL;
	_shadows = NULL;
}

CC3Light::~CC3Light()
{
	cleanupShadows(); // Includes releasing the shadows array, camera shadow volume & shadow painter
	returnLightIndex( _lightIndex );
}

bool CC3Light::isLight()
{
	return true; 
}

GLuint CC3Light::getLightIndex()
{
	return _lightIndex;
}

// Overridden to take into consideration the isDirectionalOnly property
CC3Vector4 CC3Light::getGlobalHomogeneousPosition()
{
	return (isDirectionalOnly()
			? CC3Vector4().fromDirection(getGlobalLocation())
			: CC3Vector4().fromLocation(getGlobalLocation()));
}

/** Overridden to return NO so that the forwardDirection aligns with the negative-Z-axis. */
bool CC3Light::shouldReverseForwardDirection()
{
	return false; 
}

// Clamp to valid range.
void CC3Light::setSpotExponent( GLfloat spotExp )
{
	_spotExponent = CLAMP(spotExp, 0.0f, 128.0f); 
}

void CC3Light::setAmbientColor( const ccColor4F& aColor )
{
	_ambientColor = aColor;

	if ( getScene() )
		getScene()->updateRelativeLightIntensities();
}

void CC3Light::setDiffuseColor( const ccColor4F& aColor )
{
	_diffuseColor = aColor;

	if ( getScene() )
		getScene()->updateRelativeLightIntensities();
}

void CC3Light::setSpecularColor( const ccColor4F& aColor )
{
	_specularColor = aColor;

	if ( getScene() )
		getScene()->updateRelativeLightIntensities();
}

ccColor4F CC3Light::getSpecularColor()
{
	return _specularColor;
}

ccColor4F CC3Light::getDiffuseColor()
{
	return _diffuseColor;
}

ccColor4F CC3Light::getAmbientColor()
{
	return _ambientColor;
}

float CC3Light::getSpotExponent()
{
	return _spotExponent;
}

float CC3Light::getSpotCutoffAngle()
{
	return _spotCutoffAngle;
}

void CC3Light::setSpotCutoffAngle( GLfloat spotCutoffAngle )
{
	_spotCutoffAngle = spotCutoffAngle;
}

CC3AttenuationCoefficients CC3Light::getAttenuation()
{
	return _attenuation;
}

void CC3Light::setAttenuation( const CC3AttenuationCoefficients& coefficients )
{
	_attenuation = coefficients;
}

bool CC3Light::shouldCopyLightIndex()
{
	return _shouldCopyLightIndex;
}

void CC3Light::setShouldCopyLightIndex( bool copyLightIndex )
{
	_shouldCopyLightIndex = copyLightIndex;
}

GLfloat CC3Light::getShadowIntensityFactor()
{
	return _shadowIntensityFactor;
}

void CC3Light::setShadowIntensityFactor( GLfloat shdwIntFactor )
{
	_shadowIntensityFactor = shdwIntFactor;

	if ( getScene() )
		getScene()->updateRelativeLightIntensities();
}

void CC3Light::setVisible( bool isVisible )
{
	super::setVisible( isVisible );

	if ( getScene() )
		getScene()->updateRelativeLightIntensities();
}

/** Returns diffuse color. */
CCColorRef CC3Light::getColor()
{
	return CCColorRefFromCCC4F(_diffuseColor); 
}

// Set both diffuse and ambient colors, retaining the alpha of each
void CC3Light::setColor( const CCColorRef& color )
{
	ccColor4F c4f = CCC4FFromCCColorRef(color);
	
	_ambientColor.r = c4f.r;
	_ambientColor.g = c4f.g;
	_ambientColor.b = c4f.b;
	
	_diffuseColor.r = c4f.r;
	_diffuseColor.g = c4f.g;
	_diffuseColor.b = c4f.b;

	super::setColor( color );
}

/** Returns diffuse alpha. */
CCOpacity CC3Light::getOpacity()
{
	return CCOpacityFromGLfloat(_diffuseColor.a); 
}

/** Set opacity of all colors, retaining the colors of each. */
void CC3Light::setOpacity( CCOpacity opacity )
{
	GLfloat alpha = GLfloatFromCCOpacity(opacity);
	_ambientColor.a = alpha;
	_diffuseColor.a = alpha;
	_specularColor.a = alpha;

	super::setOpacity( opacity );
}

void CC3Light::initWithTag( GLuint aTag, const std::string& aName, GLuint ltIndx )
{
	super::initWithTag( aTag, aName );
	{
		if (ltIndx == kCC3MaxGLuint) 
			return;		// All the lights have been used already.
			
		_lightIndex = ltIndx;
		_shadows = NULL;
		_shadowCastingVolume = NULL;
		_cameraShadowVolume = NULL;
		_stencilledShadowPainter = NULL;
		_ambientColor = kCC3DefaultLightColorAmbient;
		_diffuseColor = kCC3DefaultLightColorDiffuse;
		_specularColor = kCC3DefaultLightColorSpecular;
		_spotExponent = 0;
		_spotCutoffAngle = kCC3SpotCutoffNone;
		_attenuation = kCC3DefaultLightAttenuationCoefficients;
		_shadowIntensityFactor = 1.0f;
		_isDirectionalOnly = true;
		_shouldCopyLightIndex = false;
		_shouldCastShadowsWhenInvisible = false;
	}
}

void CC3Light::initWithLightIndex( GLuint ltIndx )
{
	return initWithName( "", ltIndx );
}

void CC3Light::initWithTag( GLuint aTag, GLuint ltIndx )
{
	return initWithTag( aTag, "", ltIndx );
}

void CC3Light::initWithName( const std::string& aName, GLuint ltIndx )
{
	return initWithTag( nextTag(), aName, ltIndx );
}

void CC3Light::initWithTag( GLuint aTag, const std::string& aName )
{
	return initWithTag( aTag, aName, nextLightIndex() );
}

void CC3Light::initWithName( const std::string& aName )
{
	initWithName( aName, nextLightIndex() );
}

void CC3Light::initWithTag( GLuint aTag )
{
	initWithTag( aTag, nextLightIndex() );
}

bool CC3Light::init()
{
	initWithTag( 0 );
	return true;
}

CC3Light* CC3Light::nodeWithLightIndex( GLuint ltIndx )
{
	CC3Light* light = new CC3Light;
	light->initWithLightIndex( ltIndx );
	light->autorelease();

	return light;
}

CC3Light* CC3Light::lightWithLightIndex( GLuint ltIndx )
{
	CC3Light* light = new CC3Light;
	light->initWithLightIndex( ltIndx );
	light->autorelease();

	return light;
}

CC3Light* CC3Light::lightWithTag( GLuint aTag, GLuint ltIndx )
{
	CC3Light* light = new CC3Light;
	light->initWithTag( aTag, ltIndx );
	light->autorelease();

	return light;
}

CC3Light* CC3Light::lightWithName( const std::string& aName, GLuint ltIndx )
{
	CC3Light* light = new CC3Light;
	light->initWithName( aName, ltIndx );
	light->autorelease();

	return light;
}

CC3Light* CC3Light::lightWithTag( GLuint aTag, const std::string& aName, GLuint ltIndx )
{
	CC3Light* light = new CC3Light;
	light->initWithTag( aTag, aName, ltIndx );
	light->autorelease();

	return light;
}

// Template method that populates this instance from the specified other instance.
// This method is invoked automatically during object copying via the copyWithZone: method.
// The lightIndex property is NOT copied, since we want each light to have a different value.
void CC3Light::populateFrom( CC3Light* another )
{
	super::populateFrom( another );

	// Shadows are not copied, because each shadow connects
	// one-and-only-one shadow casting node to one-and-only-one light.
	
	_ambientColor = another->getAmbientColor();
	_diffuseColor = another->getDiffuseColor();
	_specularColor = another->getSpecularColor();
	_spotExponent = another->getSpotExponent();
	_spotCutoffAngle = another->getSpotCutoffAngle();
	_attenuation = another->getAttenuation();
	_shadowIntensityFactor = another->getShadowIntensityFactor();
	_isDirectionalOnly = another->isDirectionalOnly();
	_shouldCopyLightIndex = another->shouldCopyLightIndex();
	_shouldCastShadowsWhenInvisible = another->shouldCastShadowsWhenInvisible();
}

CCObject* CC3Light::copyWithZone( CCZone* zone )
{
	CC3Light* pVal = new CC3Light;
	pVal->init();
	pVal->populateFrom( this );
	pVal->addCopiesOfChildrenFrom( this );

	return pVal;
}

/** Scaling does not apply to lights. */
void CC3Light::applyScalingTo( CC3Matrix* matrix )
{

}

/**
 * Scaling does not apply to lights. Return the globalScale of the parent node,
 * or unit scaling if no parent.
 */
CC3Vector CC3Light::getGlobalScale()
{
	return m_pParent ? m_pParent->getGlobalScale() : CC3Vector::kCC3VectorUnitCube; 
}

/** Overridden to update the camera shadow frustum with the global location of this light */
void CC3Light::markTransformDirty()
{
	super::markTransformDirty();

	if ( _shadowCastingVolume )
		_shadowCastingVolume->markDirty();

	if ( _cameraShadowVolume )
		_cameraShadowVolume->markDirty();
}

void CC3Light::turnOnWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	CC3OpenGL* gl = visitor->getGL();
	if ( isVisible() ) 
	{
		// MARK_TRACE_HERE
		//CCLOG( "Turning on CC3Light" );
		gl->enableLight( true, _lightIndex );
		applyPositionWithVisitor( visitor );
		applyDirectionWithVisitor( visitor );
		applyAttenuationWithVisitor( visitor );
		applyColorWithVisitor( visitor );
	} else {
		gl->enableLight( false, _lightIndex );
	}
}

void CC3Light::turnOffWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	visitor->getGL()->enableLight( false, _lightIndex );
}

/**
 * Template method that sets the position of this light in the GL engine to the value of
 * the globalHomogeneousPosition property of this node.
 */	
void CC3Light::applyPositionWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	visitor->getGL()->setLightPosition( getGlobalHomogeneousPosition(), _lightIndex );
}

/**
 * Template method that sets the spot direction, spot exponent, and spot cutoff angle of this light
 * in the GL engine to the values of the globalForwardDirection, spotExponent and spotCutoffAngle
 * properties of this node, respectively. The direction and exponent are only valid if a cutoff has
 * been specified and less than 90 degrees, otherwise the light is treated as omnidirectional.
 * OpenGL ES only supports angles less than 90 degrees, so anything above is treated as omnidirectional.
 */
void CC3Light::applyDirectionWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	CC3OpenGL* gl = visitor->getGL();
	if (_spotCutoffAngle <= 90.0f) 
	{
		gl->setSpotlightDirection( getGlobalForwardDirection(), _lightIndex );
		gl->setSpotlightCutoffAngle( _spotCutoffAngle, _lightIndex );
		gl->setSpotlightFadeExponent( _spotExponent, _lightIndex );
	} else {
		gl->setSpotlightCutoffAngle( kCC3SpotCutoffNone, _lightIndex );
	}
}

/**
 * Template method that sets the light intensity attenuation characteristics
 * in the GL engine from the attenuation property of this light.
 */
void CC3Light::applyAttenuationWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	if ( !_isDirectionalOnly ) 
		visitor->getGL()->setLightAttenuation( _attenuation, _lightIndex );
}

/**
 * Template method that sets the ambient, diffuse and specular colors of this light
 * in the GL engine to the values of the ambientColor, diffuseColor and specularColor
 * properties of this node, respectively.
 */
void CC3Light::applyColorWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	CC3OpenGL* gl = visitor->getGL();
	gl->setLightAmbientColor( _ambientColor, _lightIndex );
	gl->setLightDiffuseColor( _diffuseColor, _lightIndex );
	gl->setLightSpecularColor( _specularColor, _lightIndex );
}

bool CC3Light::shouldCastShadowsWhenInvisible()
{
	return _shouldCastShadowsWhenInvisible; 
}

void CC3Light::setShouldCastShadowsWhenInvisible( bool shouldCast )
{
	_shouldCastShadowsWhenInvisible = shouldCast;
	super::setShouldCastShadowsWhenInvisible( shouldCast );
}

/**
 * If this action is occuring on a background thread, and this node is already part of the
 * scene being rendered, the operation is queued for execution on the rendering thread, to
 * avoid the possibility of adding a shadow in the middle of a render iteration.
 */
void CC3Light::addShadow( CC3ShadowVolumeMeshNode* aShadowNode )
{
	if ( !CC3OpenGL::sharedGL()->isRenderingContext() && getScene() )
		addShadowFromBackgroundThread( aShadowNode );
	else
		addShadowNow( aShadowNode );
}

/** Adds the specified shadow to this light without queuing. */
void CC3Light::addShadowNow( CC3ShadowVolumeMeshNode* aShadowNode )
{
	CCAssert(aShadowNode, "Shadow cannot be nil");		// Don't add if shadow is nil
	
	if ( !_shadows ) 
	{
		_shadows = CCArray::create();			// retained
		_shadows->retain();
	}
	_shadows->addObject( aShadowNode );

	aShadowNode->setLight( this );
	addTransformListener( aShadowNode );	// Update the shadow when this light moves.
	
	checkShadowCastingVolume();			// Make sure we have the shadow casting volume
	checkCameraShadowVolume();				// Make sure we have the camera shadow volume
	checkStencilledShadowPainter();		// Make sure we have the shadow painter
}

/**
 * Invoked when a shadow is being added on a background thread, and this parent node is
 * already part of the scene.
 *
 * Since the scene may be in the process of being rendered, the shadow is not added immediately.
 * Instead, all GL activity on this thread is allowed to finish, to ensure all GL components of
 * the shadow node are in place, and then an operation to add the specified shadow is queued to
 * the thread that is performing rendering.
 */
void CC3Light::addShadowFromBackgroundThread( CC3ShadowVolumeMeshNode* aShadowNode )
{
	//[CC3OpenGL.sharedGL finish];
	//[CC3OpenGL.renderThread runBlockAsync: ^{ [self addShadowNow: aShadowNode]; } ];
	
	// A better design would be to use dispatch queues, but OSX typically
	// renders using a DisplayLink thread instead of the main thread.
//	dispatch_async(dispatch_get_main_queue(), ^{ [self addShadowNow: aShadowNode]; });
}

void CC3Light::removeShadow( CC3ShadowVolumeMeshNode* aShadowNode )
{
#pragma _NOTE_TODO( "removeShadow( CC3ShadowProtocol* aShadowNode )" )
	//_shadows->removeObject( aShadowNode );
	aShadowNode->setLight( NULL );					// So it can't call back here if I'm gone
	if (_shadows && _shadows->count() == 0) 
	{
		_shadows->release();
		_shadows = NULL;
		checkShadowCastingVolume();		// Remove the shadow casting volume
		checkCameraShadowVolume();			// Remove the camera shadow volume
		checkStencilledShadowPainter();	// Remove the stencilled shadow painter
	}

	removeTransformListener( aShadowNode );
}

bool CC3Light::hasShadows()
{
	return _shadows && _shadows->count() > 0; 
}

void CC3Light::updateShadows()
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH( _shadows, pObj )
	{
		CC3ShadowVolumeMeshNode* sv = (CC3ShadowVolumeMeshNode*)pObj;
		sv->updateShadow();
	}
}

CC3ShadowCastingVolume* CC3Light::getShadowCastingVolume()
{
	return _shadowCastingVolume; 
}

/** Detaches old as camera listener, attaches new as camera listener, and attaches light. */
void CC3Light::setShadowCastingVolume( CC3ShadowCastingVolume* scVolume )
{
	if (scVolume == _shadowCastingVolume) 
		return;

	CC3Camera* cam = getActiveCamera();
	cam->removeTransformListener( _shadowCastingVolume );
	CC_SAFE_RELEASE( _shadowCastingVolume );
	
	_shadowCastingVolume = scVolume;
	_shadowCastingVolume->retain();
	_shadowCastingVolume->setLight( this );
	cam->addTransformListener( _shadowCastingVolume );
}

/**
 * If there are shadows and the shadow casting volume has not been added, add it now,
 * and tell the camera to let the shadow casting volume know whenever the camera moves
 * so the shadow casting volume can determine which objects are casting shadows that
 * are visible within the camera frustum.
 *
 * If there are no more shadows, disconnect the shadow casting volume from the camera,
 * and remove the shadow casting volume.
 */
void CC3Light::checkShadowCastingVolume()
{
	if (hasShadows()) {
		if (!_shadowCastingVolume) 
			setShadowCastingVolume( CC3ShadowCastingVolume::boundingVolume() );
	} else {
		setShadowCastingVolume( NULL );
	}
}

CC3CameraShadowVolume* CC3Light::getCameraShadowVolume()
{
	return _cameraShadowVolume; 
}

/** Detaches old as camera listener, attaches new as camera listener, and attaches light. */
void CC3Light::setCameraShadowVolume( CC3CameraShadowVolume* csVolume )
{
	if (csVolume == _cameraShadowVolume) 
		return;
		
	CC3Camera* cam = getActiveCamera();
	cam->removeTransformListener( _cameraShadowVolume );
	CC_SAFE_RELEASE( _cameraShadowVolume );
	
	_cameraShadowVolume = csVolume;
	_cameraShadowVolume->retain();
	_cameraShadowVolume->setLight( this );
	cam->addTransformListener( _cameraShadowVolume );
}

/**
 * If there are shadows and the camera shadow volume has not been added, add it now,
 * and tell the camera to let the camera shadow volume know whenever the camera moves
 * so the camera shadow volume can determine which objects are shadowing the camera.
 *
 * If there are no more shadows, disconnect the camera shadow volume from the camera,
 * and remove the camera shadow volume.
 */
void CC3Light::checkCameraShadowVolume()
{
	if (hasShadows()) {
		if (!_cameraShadowVolume) 
			setCameraShadowVolume( CC3CameraShadowVolume::boundingVolume() );
	} else {
		setCameraShadowVolume( NULL );
	}
}

/**
 * Creates or removes the stenciled shadow painter mesh node, as needed.
 * 
 * Sets the intensity of the shadow from the intensity of the diffuse component
 * of this light relative to the intensity of all other illumination in the scene.
 */
void CC3Light::checkStencilledShadowPainter()
{
	if (hasShadows()) 
	{
		if (!_stencilledShadowPainter)
			setStencilledShadowPainter( CC3StencilledShadowPainterNode::nodeWithName( "Shadow painter", kCCC4FBlack ) );
	} else {
		setStencilledShadowPainter( NULL );
	}
}

CC3StencilledShadowPainterNode* CC3Light::getStencilledShadowPainter()
{
	return _stencilledShadowPainter; 
}

void CC3Light::setStencilledShadowPainter( CC3StencilledShadowPainterNode* sspNode )
{
	if (sspNode == _stencilledShadowPainter) 
		return;
	
	CC_SAFE_RELEASE( _stencilledShadowPainter );
	CC_SAFE_RETAIN( sspNode );
	_stencilledShadowPainter = sspNode;

	getScene()->updateRelativeLightIntensities();	//  Must be done after the ivar is set.
}

void CC3Light::updateRelativeIntensityFrom( const ccColor4F& totalLight )
{
	if (_stencilledShadowPainter) 
	{
		GLfloat dIntensity = CCC4FIntensity(getDiffuseColor());
		GLfloat totIntensity = CCC4FIntensity(totalLight);
		GLfloat shadowIntensity =  (dIntensity / totIntensity) * _shadowIntensityFactor;
		_stencilledShadowPainter->setOpacity( CCOpacityFromGLfloat(shadowIntensity) );
		/*LogTrace(@"%@ updated shadow intensity to %.3f from light illumination %@ against total illumination %@ and shadow intensity factor %.3f",
					  self, (float)_stencilledShadowPainter.opacity,
					  NSStringFromCCC4F(self.diffuseColor), NSStringFromCCC4F(self.scene.totalIllumination), _shadowIntensityFactor);*/
	}
}

// TODO - combine with other shadow techniques - how to make polymorphic?
void CC3Light::drawShadowsWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	if (_shadows && (isVisible() || shouldCastShadowsWhenInvisible()) ) 
	{
		// MARK_TRACE_HERE
		//CCLOG("CC3Light drawing %d shadows", _shadows->count());
		configureStencilParameters( visitor );
		
		CCObject* pObj = NULL;
		CCARRAY_FOREACH( _shadows, pObj )
		{
			CC3ShadowVolumeMeshNode* sv = (CC3ShadowVolumeMeshNode*)pObj;
			sv->drawToStencilWithVisitor( visitor );
		}
		
		paintStenciledShadowsWithVisitor( visitor );
		cleanupStencilParameters( visitor );
	}
}

/**
 * Turns on stenciling and ensure the stencil buffer can be updated.
 * Turns off writing to the color buffer, because the shadow volumes themselves are invisible.
 */
void CC3Light::configureStencilParameters( CC3NodeDrawingVisitor* visitor )
{
	CC3OpenGL* gl = visitor->getGL();
	gl->enableStencilTest( true );
	gl->setStencilMask( ~0 );
	gl->setColorMask( ccc4(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE) );
	gl->setStencilFunc( GL_ALWAYS, 0, ~0 );
}

/** Draws the clip-space rectangle on the screen, coloring only those pixels where the stencil is non-zero. */
void CC3Light::paintStenciledShadowsWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	CC3OpenGL* gl = visitor->getGL();
	
	// Turn color masking back on so that shadow will be painted on the scene.
	// The depth mask will be turned on by the mesh node
	gl->setColorMask( ccc4(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE) );
	
	// Set the stencil function so that only those pixels that have a non-zero
	// value in the stencil (and that pass the depth test) will be painted.
	gl->setStencilFunc( GL_NOTEQUAL, 0, ~0 );
	
	// Don't waste time updating the stencil buffer now.
	gl->setStencilMask( 0 );
	
	// Paint the shadow to the screen. Only areas that have been marked as being
	// in the stencil buffer as being in the shadow of this light will be shaded.
	visitor->visit( _stencilledShadowPainter );
}

/** Turns stenciling back off. */
void CC3Light::cleanupStencilParameters( CC3NodeDrawingVisitor* visitor )
{
	visitor->getGL()->enableStencilTest( false );
}

/**
 * Invoked when this light is being deallocated. Removes all associated
 * shadow nodes from this scene, which also removes the shadows array.
 */
void CC3Light::cleanupShadows()
{
	if ( _shadows )
	{
		CCArray* myShadows = (CCArray*)_shadows->copy();

		CCObject* pObj = NULL;
		CCARRAY_FOREACH( myShadows, pObj )
		{
			CC3Node* sv = (CC3Node*)pObj;
			sv->remove();
		}

		CC_SAFE_RELEASE( myShadows );
	}
}

// Class variable that tracks the indexes of the lights that are in use.
// When a new instance is instantiated, it's lightIndex property is assigned from the pool
// of indexes. When the instance is deallocated, its index is returned to the pool for use
// by any subsequently instantiated lights.
static bool _lightIndexPool[32] = {false};

static bool* getLightIndexPool()
{
	return _lightIndexPool; 
}

// Indicates the staring index to use when instantiating new lights.
static GLuint lightPoolStartIndex = 0;

/**
 * Assigns and returns the next available light index from the pool.
 * If no more lights are available, returns kCC3MaxGLuint.
 */
GLuint CC3Light::nextLightIndex()
{
	bool* indexPool = getLightIndexPool();
	GLuint platformMaxLights = CC3OpenGL::sharedGL()->getMaxNumberOfLights();
	for (GLuint lgtIdx = lightPoolStartIndex; lgtIdx < platformMaxLights; lgtIdx++) 
	{
		if (!indexPool[lgtIdx]) 
		{
			CC3_TRACE("[lgt]Allocating light index %d", lgtIdx);
			indexPool[lgtIdx] = true;
			return lgtIdx;
		}
	}
	CCAssert(false, "Too many lights. Only %d lights may be created."/*, platformMaxLights*/);
	return kCC3MaxGLuint;
}

/** Returns the specified light index to the pool. */
void CC3Light::returnLightIndex( GLuint aLightIndex )
{
	CC3_TRACE("[lgt]Returning light index %d", aLightIndex);
	bool* indexPool = getLightIndexPool();
	indexPool[aLightIndex] = false;
}

GLuint CC3Light::getLightCount()
{
	GLuint count = 0;
	bool* indexPool = getLightIndexPool();
	GLuint platformMaxLights = CC3OpenGL::sharedGL()->getMaxNumberOfLights();
	for (GLuint i = lightPoolStartIndex; i < platformMaxLights; i++) 
		if (indexPool[i]) 
			count++;

	return lightPoolStartIndex + count;
}

GLuint CC3Light::getLightPoolStartIndex()
{
	return lightPoolStartIndex; 
}

void CC3Light::setLightPoolStartIndex( GLuint newStartIndex )
{
	lightPoolStartIndex = newStartIndex; 
}

void CC3Light::disableReservedLightsWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	for (GLuint ltIdx = 0; ltIdx < lightPoolStartIndex; ltIdx++) 
		visitor->getGL()->enableLight( false, ltIdx );
}

CC3Light* CC3Light::nodeWithName( const std::string& name )
{
	CC3Light* pLight = new CC3Light;
	pLight->initWithName( name );
	pLight->autorelease();

	return pLight;
}

void CC3Light::setIsDirectionalOnly( bool directionalOnly )
{
	_isDirectionalOnly = directionalOnly;
}

bool CC3Light::isDirectionalOnly()
{
	return _isDirectionalOnly;
}

CC3LightCameraBridgeVolume::CC3LightCameraBridgeVolume()
{
	_light = NULL;
}

CC3LightCameraBridgeVolume::~CC3LightCameraBridgeVolume()
{
	_light = NULL;
}

// Included to satisfy compiler because property appears in interface for documentation purposes
GLuint CC3LightCameraBridgeVolume::getVertexCount()
{
	return super::getVertexCount(); 
}

CC3Light* CC3LightCameraBridgeVolume::getLight()
{
	return _light; 
}

void CC3LightCameraBridgeVolume::setLight( CC3Light* aLight )
{
	_light = aLight;			// weak reference
	markDirty();
}

/**
 * Returns the position of the light, as a 3D vector.
 *
 * This could be a location or direction, depending on whether the
 * 4D homogeneous location has a definite location, or is directional.
 */
CC3Vector CC3LightCameraBridgeVolume::getLightPosition()
{
	return _light->getGlobalHomogeneousPosition().cc3Vector(); 
}

void CC3LightCameraBridgeVolume::populateFrom( CC3LightCameraBridgeVolume* another )
{
	super::populateFrom( another );
	
	_light = another->getLight();		// weak reference
}

CCObject* CC3LightCameraBridgeVolume::copyWithZone( CCZone* pZone )
{
	CC3LightCameraBridgeVolume* pVal = new CC3LightCameraBridgeVolume;
	pVal->init();
	pVal->populateFrom( this );

	return pVal;
}

/**
 * Callback indicating that the camera has been transformed.
 * Sets the camera frustum (in case the camera has changed), and marks this volume as dirty.
 */
void CC3LightCameraBridgeVolume::nodeWasTransformed( CC3Node* aNode )
{
	if (aNode->isCamera()) 
		markDirty(); 
}

void CC3LightCameraBridgeVolume::nodeWasDestroyed( CC3Node* aNode )
{

}

/**
 * Returns whether the light is located in front of the plane.
 *
 * Performs a 4D dot-product between the plane definition, and the homegeneous location
 * of the light, which magically works for both directional and positional lights.
 */
bool CC3LightCameraBridgeVolume::isLightInFrontOfPlane( const CC3Plane& aPlane )
{
	return CC3Vector4IsInFrontOfPlane(_light->getGlobalHomogeneousPosition(), aPlane);
}

/** Overridden to include the homogeneous location of the light into the vertex test. */
bool CC3LightCameraBridgeVolume::areAllVerticesInFrontOf( const CC3Plane& plane )
{
	return isLightInFrontOfPlane(plane) && super::areAllVerticesInFrontOf(plane);
}

CC3ShadowCastingVolume::CC3ShadowCastingVolume()
{
	_vertexCount = 0; 
	_planeCount = 0;
}

CC3ShadowCastingVolume* CC3ShadowCastingVolume::boundingVolume()
{
	CC3ShadowCastingVolume* pVolume = new CC3ShadowCastingVolume;
	pVolume->init();
	pVolume->autorelease();

	return pVolume;
}

CC3Plane* CC3ShadowCastingVolume::getPlanes()
{
	updateIfNeeded();
	return _planes;
}

GLuint CC3ShadowCastingVolume::getPlaneCount()
{
	updateIfNeeded();
	return _planeCount;
}

CC3Vector* CC3ShadowCastingVolume::getVertices()
{
	updateIfNeeded();
	return _vertices;
}

GLuint CC3ShadowCastingVolume::getVertexCount()
{
	updateIfNeeded();
	return _vertexCount;
}

/**
 * If the specified vertex does not already exist in the vertices array,
 * it is added to the array, and the vertexCount property is incremented.
 */
void CC3ShadowCastingVolume::addUniqueVertex( const CC3Vector& aLocation )
{
	for (GLuint vtxIdx = 0; vtxIdx < _vertexCount; vtxIdx++)
		if ( aLocation.equals( _vertices[vtxIdx] ) ) 
			return;

	_vertices[_vertexCount++] = aLocation;
}

/** Adds the specified plane to the planes array, and increments the planeCount property. */
void CC3ShadowCastingVolume::addPlane( const CC3Plane& aPlane )
{
	_planes[_planeCount++] = aPlane; 
}

//-(NSString*) fullDescription {
//	NSMutableString* desc = [NSMutableString stringWithCapacity: 200];
//	[desc appendFormat: @"%@", self.description];
//	[desc appendFormat: @" from light at %@", NSStringFromCC3Vector4(_light.globalHomogeneousPosition)];
//	[self appendPlanesTo: desc];
//	[self appendVerticesTo: desc];
//	return desc;
//}


void CC3ShadowCastingVolume::checkPlaneEdge( const CC3Plane& edgePlane, const CC3Vector& v1, const CC3Vector& v2 )
{
	if ( isLightInFrontOfPlane( edgePlane ) ) 
	{
		CC3Vector v3 = _light->isDirectionalOnly()
							? v2.add( getLightPosition() ) 
							: getLightPosition();
		addPlane( CC3PlaneFromLocations(v1, v2, v3) );
	}
}

void CC3ShadowCastingVolume::checkPlane( const CC3Plane& aPlane, const CC3Plane& edgePlane1, const CC3Vector& v1, 
	const CC3Plane& edgePlane2, const CC3Vector& v2,
	const CC3Plane& edgePlane3, const CC3Vector& v3,
	const CC3Plane& edgePlane4, const CC3Vector& v4 )
{
	
	if( !isLightInFrontOfPlane(aPlane) ) 
	{
		addPlane( aPlane );
		
		addUniqueVertex( v1 );
		addUniqueVertex( v2 );
		addUniqueVertex( v3 );
		addUniqueVertex( v4 );
		
		checkPlaneEdge( edgePlane1, v1, v2 );
		checkPlaneEdge( edgePlane2, v2, v3 );
		checkPlaneEdge( edgePlane3, v3, v4 );
		checkPlaneEdge( edgePlane4, v4, v1 );
	}
}

void CC3ShadowCastingVolume::buildPlanes()
{	
	_planeCount = 0;
	_vertexCount = 0;
	
    CC3Frustum* cf = _light->getActiveCamera()->getFrustum();
	
	checkPlane( cf->getLeftPlane(),
			cf->getFarPlane(), cf->getFarBottomLeft(),
			cf->getTopPlane(), cf->getFarTopLeft(),
			cf->getNearPlane(), cf->getNearTopLeft(),
			cf->getBottomPlane(), cf->getNearBottomLeft() );
	
	checkPlane( cf->getRightPlane(),
			cf->getNearPlane(), cf->getNearBottomRight(),
			cf->getTopPlane(), cf->getNearTopRight(),
			cf->getFarPlane(), cf->getFarTopRight(),
			cf->getBottomPlane(), cf->getFarBottomRight() );
	
	checkPlane( cf->getTopPlane(),
			cf->getLeftPlane(), cf->getNearTopLeft(),
			cf->getFarPlane(), cf->getFarTopLeft(),
			cf->getRightPlane(), cf->getFarTopRight(),
			cf->getNearPlane(), cf->getNearTopRight() );
	
	checkPlane( cf->getBottomPlane(),
			cf->getRightPlane(), cf->getNearBottomRight(),
			cf->getFarPlane(), cf->getFarBottomRight(),
			cf->getLeftPlane(), cf->getFarBottomLeft(),
			cf->getNearPlane(), cf->getNearBottomLeft() );
	
	checkPlane( cf->getNearPlane(),
			cf->getLeftPlane(), cf->getNearBottomLeft(),
			cf->getTopPlane(), cf->getNearTopLeft(),
			cf->getRightPlane(), cf->getNearTopRight(),
			cf->getBottomPlane(), cf->getNearBottomRight() );
	
	checkPlane( cf->getFarPlane(),
			cf->getRightPlane(), cf->getFarBottomRight(), 
			cf->getTopPlane(), cf->getFarTopRight(),
			cf->getLeftPlane(), cf->getFarTopLeft(),
			cf->getBottomPlane(), cf->getFarBottomLeft() );

	if ( !_light->isDirectionalOnly() ) 
		addUniqueVertex( getLightPosition() );
}

// Indices of the five boundary vertices
#define kCC3TopLeftIdx	0
#define kCC3TopRgtIdx	1
#define kCC3BtmLeftIdx	2
#define kCC3BtmRgtIdx	3
#define kCC3LightIdx	4

// Indices of the six boundary planes
#define kCC3TopIdx		0
#define kCC3BotmIdx		1
#define kCC3LeftIdx		2
#define kCC3RgtIdx		3
#define kCC3NearIdx		4
#define kCC3FarIdx		5

CC3CameraShadowVolume* CC3CameraShadowVolume::boundingVolume()
{
	CC3CameraShadowVolume* pVolume = new CC3CameraShadowVolume;
	pVolume->init();
	pVolume->autorelease();

	return pVolume;
}

CC3Vector CC3CameraShadowVolume::getTopLeft()
{
	return getVertices()[kCC3TopLeftIdx]; 
}
CC3Vector CC3CameraShadowVolume::getTopRight()
{
	return getVertices()[kCC3TopRgtIdx]; 
}

CC3Vector CC3CameraShadowVolume::getBottomLeft()
{
	return getVertices()[kCC3BtmLeftIdx]; 
}

CC3Vector CC3CameraShadowVolume::getBottomRight()
{
	return getVertices()[kCC3BtmRgtIdx]; 
}

CC3Plane* CC3CameraShadowVolume::getPlanes()
{
	updateIfNeeded();
	return _planes;
}

GLuint CC3CameraShadowVolume::getPlaneCount()
{
	return 6; 
}

CC3Vector* CC3CameraShadowVolume::getVertices()
{
	updateIfNeeded();
	return _vertices;
}

GLuint CC3CameraShadowVolume::getVertexCount()
{
	updateIfNeeded();

	return _light->isDirectionalOnly() ? 4 : 5;
}

CC3Plane CC3CameraShadowVolume::getTopPlane()
{
	return getPlanes()[kCC3TopIdx]; 
}

CC3Plane CC3CameraShadowVolume::getBottomPlane()
{
	return getPlanes()[kCC3BotmIdx]; 
}

CC3Plane CC3CameraShadowVolume::getLeftPlane()
{
	return getPlanes()[kCC3LeftIdx]; 
}

CC3Plane CC3CameraShadowVolume::getRightPlane()
{
	return getPlanes()[kCC3RgtIdx]; 
}

CC3Plane CC3CameraShadowVolume::getNearPlane()
{
	return getPlanes()[kCC3NearIdx]; 
}

CC3Plane CC3CameraShadowVolume::getFarPlane()
{
	return getPlanes()[kCC3FarIdx]; 
}

/** Updates the vertices from the camera frustum. */
void CC3CameraShadowVolume::buildVolume()
{
    CC3Frustum* cf = _light->getActiveCamera()->getFrustum();
	_vertices[kCC3TopLeftIdx] = cf->getNearTopLeft();
	_vertices[kCC3TopRgtIdx] = cf->getNearTopRight();
	_vertices[kCC3BtmLeftIdx] = cf->getNearBottomLeft();
	_vertices[kCC3BtmRgtIdx] = cf->getNearBottomRight();
	_vertices[kCC3LightIdx] = getLightPosition();
}

/**
 * Builds the planes of the pyramid by taking points on the near clipping plane
 * as the base of the pyramid, and the light location as the apex of the pyramid.
 *
 * If the light is directional, then the pyramid will actually become an elongated
 * box extending out to infinity, with opposite sides parallel.
 *
 * If the light is actually behind the camera, then the planes are adjusted so that
 * they are oriented correctly for a pyramid behind the near camera plane.
 *
 * All planes have their normals pointing outward.
 */
void CC3CameraShadowVolume::buildPlanes()
{
	// Get the 3D position that corresponds to either a location or a direction
	CC3Vector lightPos = getLightPosition();
	CC3Vector lightDir;
	CC3Vector tl = _vertices[kCC3TopLeftIdx];
	CC3Vector tr = _vertices[kCC3TopRgtIdx];
	CC3Vector bl = _vertices[kCC3BtmLeftIdx];
	CC3Vector br = _vertices[kCC3BtmRgtIdx];
	
	// The near plane does not depend on the light position
	_planes[kCC3NearIdx] = CC3PlaneFromLocations(bl, br, tr);
	
	if (_light->isDirectionalOnly()) 
	{	
		// The light is infinitely far away. The light position is actually a direction to it.
		// Opposite sides are parallel and pointing in the direction of the light source.
		// For each edge of the near clipping rectangle, generate the third location on the
		// plane by adding the light direction to one of the locations on the edge. 
		lightDir = lightPos;
		
		_planes[kCC3LeftIdx] = CC3PlaneFromLocations(bl, tl, tl.add( lightDir ));
		_planes[kCC3RgtIdx] = CC3PlaneFromLocations(tr, br, br.add( lightDir ));
		
		_planes[kCC3TopIdx] = CC3PlaneFromLocations(tl, tr, tr.add( lightDir ));
		_planes[kCC3BotmIdx] = CC3PlaneFromLocations(br, bl, bl.add( lightDir ));
		
		// The far plane is parallel to the near plane, but the normal points in
		// the opposite direction. Locate the far plane at the light position,
		// and then move it out an infinite distance, in the same direction.
		_planes[kCC3FarIdx] = CC3PlaneNegate(_planes[kCC3NearIdx]);
		_planes[kCC3FarIdx].d = -lightPos.dot( CC3PlaneNormal(_planes[kCC3FarIdx]) );
		_planes[kCC3FarIdx].d = SIGN(_planes[kCC3FarIdx].d) * FLOAT_INFINITY;

	} else {
		
		// The light is at a definite position. All side planes meet at the light position.
		// The direction is taken from the center of the near clipping rectangle.
		lightDir = lightPos.difference( tl.average( br ) );
		
		_planes[kCC3LeftIdx] = CC3PlaneFromLocations(bl, tl, lightPos);
		_planes[kCC3RgtIdx] = CC3PlaneFromLocations(tr, br, lightPos);
		
		_planes[kCC3TopIdx] = CC3PlaneFromLocations(tl, tr, lightPos);
		_planes[kCC3BotmIdx] = CC3PlaneFromLocations(br, bl, lightPos);
		
		// The far plane is parallel to the near plane, but the normal points in
		// the opposite direction. Locate the far plane at the light position.
		_planes[kCC3FarIdx] = CC3PlaneNegate(_planes[kCC3NearIdx]);
		_planes[kCC3FarIdx].d = -lightPos.dot( CC3PlaneNormal(_planes[kCC3FarIdx]) );

	}
	
	// Finally, determine if the light source is actually behind the camera, by crossing
	// two sides of the near plane to determine the camera direction, and dotting with a
	// vector from the light position and a point on the near plane.
	CC3Vector left = tl.difference( bl );
	CC3Vector bottom = br.difference( bl );
	CC3Vector camDir = left.cross( bottom );
	BOOL isBehindCamera = (camDir.dot( lightDir ) < 0);
	
	if ( isBehindCamera ) {
		_planes[kCC3LeftIdx] = CC3PlaneNegate(_planes[kCC3LeftIdx]);
		_planes[kCC3RgtIdx] = CC3PlaneNegate(_planes[kCC3RgtIdx]);
		_planes[kCC3TopIdx] = CC3PlaneNegate(_planes[kCC3TopIdx]);
		_planes[kCC3BotmIdx] = CC3PlaneNegate(_planes[kCC3BotmIdx]);
		_planes[kCC3NearIdx] = CC3PlaneNegate(_planes[kCC3NearIdx]);
		_planes[kCC3FarIdx] = CC3PlaneNegate(_planes[kCC3FarIdx]);
	}
	
	//LogTrace(@"Built %@ from %@ light %@ the camera",
	//			  self.fullDescription, (_light.isDirectionalOnly ? @"directional" : @"positional"),
	//			  (isBehindCamera ? @"behind" : @"in front of"));
}
//
//-(NSString*) fullDescription {
//	NSMutableString* desc = [NSMutableString stringWithCapacity: 500];
//	[desc appendFormat: @"%@", self.description];
//	[desc appendFormat: @" from light at %@", NSStringFromCC3Vector4(_light.globalHomogeneousPosition)];
//	[desc appendFormat: @"\n\tleftPlane: %@", NSStringFromCC3Plane(self.leftPlane)];
//	[desc appendFormat: @"\n\trightPlane: %@", NSStringFromCC3Plane(self.rightPlane)];
//	[desc appendFormat: @"\n\ttopPlane: %@", NSStringFromCC3Plane(self.topPlane)];
//	[desc appendFormat: @"\n\tbottomPlane: %@", NSStringFromCC3Plane(self.bottomPlane)];
//	[desc appendFormat: @"\n\tnearPlane: %@", NSStringFromCC3Plane(self.nearPlane)];
//	[desc appendFormat: @"\n\tfarPlane: %@", NSStringFromCC3Plane(self.farPlane)];
//	[desc appendFormat: @"\n\ttopLeft: %@", NSStringFromCC3Vector(self.topLeft)];
//	[desc appendFormat: @"\n\ttopRight: %@", NSStringFromCC3Vector(self.topRight)];
//	[desc appendFormat: @"\n\tbottomLeft: %@", NSStringFromCC3Vector(self.bottomLeft)];
//	[desc appendFormat: @"\n\tbottomRight: %@", NSStringFromCC3Vector(self.bottomRight)];
//	return desc;
//}

NS_COCOS3D_END
