/*
 * Cocos3D-X 1.0.0
 * Author: Bill Hollings
 * Copyright (c) 2010-2014 The Brenwill Workshop Ltd. All rights reserved.
 * http://www.brenwill.com
 *
 * Copyright (c) 2014-2015 Jason Wang
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
	m_cameraShadowVolume = NULL;
	m_shadowCastingVolume = NULL;
	m_stencilledShadowPainter = NULL;
	m_shadows = NULL;
}

CC3Light::~CC3Light()
{
	cleanupShadows(); // Includes releasing the shadows array, camera shadow volume & shadow painter
	returnLightIndex( m_lightIndex );
}

bool CC3Light::isLight()
{
	return true; 
}

GLuint CC3Light::getLightIndex()
{
	return m_lightIndex;
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
	m_spotExponent = CLAMP(spotExp, 0.0f, 128.0f); 
}

void CC3Light::setAmbientColor( const ccColor4F& aColor )
{
	m_ambientColor = aColor;

	if ( getScene() )
		getScene()->updateRelativeLightIntensities();
}

void CC3Light::setDiffuseColor( const ccColor4F& aColor )
{
	m_diffuseColor = aColor;

	if ( getScene() )
		getScene()->updateRelativeLightIntensities();
}

void CC3Light::setSpecularColor( const ccColor4F& aColor )
{
	m_specularColor = aColor;

	if ( getScene() )
		getScene()->updateRelativeLightIntensities();
}

ccColor4F CC3Light::getSpecularColor()
{
	return m_specularColor;
}

ccColor4F CC3Light::getDiffuseColor()
{
	return m_diffuseColor;
}

ccColor4F CC3Light::getAmbientColor()
{
	return m_ambientColor;
}

float CC3Light::getSpotExponent()
{
	return m_spotExponent;
}

float CC3Light::getSpotCutoffAngle()
{
	return m_spotCutoffAngle;
}

void CC3Light::setSpotCutoffAngle( GLfloat spotCutoffAngle )
{
	m_spotCutoffAngle = spotCutoffAngle;
}

CC3AttenuationCoefficients CC3Light::getAttenuation()
{
	return m_attenuation;
}

void CC3Light::setAttenuation( const CC3AttenuationCoefficients& coefficients )
{
	m_attenuation = coefficients;
}

bool CC3Light::shouldCopyLightIndex()
{
	return m_shouldCopyLightIndex;
}

void CC3Light::setShouldCopyLightIndex( bool copyLightIndex )
{
	m_shouldCopyLightIndex = copyLightIndex;
}

GLfloat CC3Light::getShadowIntensityFactor()
{
	return m_shadowIntensityFactor;
}

void CC3Light::setShadowIntensityFactor( GLfloat shdwIntFactor )
{
	m_shadowIntensityFactor = shdwIntFactor;

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
	return CCColorRefFromCCC4F(m_diffuseColor); 
}

// Set both diffuse and ambient colors, retaining the alpha of each
void CC3Light::setColor( const CCColorRef& color )
{
	ccColor4F c4f = CCC4FFromCCColorRef(color);
	
	m_ambientColor.r = c4f.r;
	m_ambientColor.g = c4f.g;
	m_ambientColor.b = c4f.b;
	
	m_diffuseColor.r = c4f.r;
	m_diffuseColor.g = c4f.g;
	m_diffuseColor.b = c4f.b;

	super::setColor( color );
}

/** Returns diffuse alpha. */
CCOpacity CC3Light::getOpacity()
{
	return CCOpacityFromGLfloat(m_diffuseColor.a); 
}

/** Set opacity of all colors, retaining the colors of each. */
void CC3Light::setOpacity( CCOpacity opacity )
{
	GLfloat alpha = GLfloatFromCCOpacity(opacity);
	m_ambientColor.a = alpha;
	m_diffuseColor.a = alpha;
	m_specularColor.a = alpha;

	super::setOpacity( opacity );
}

void CC3Light::initWithTag( GLuint aTag, const std::string& aName, GLuint ltIndx )
{
	super::initWithTag( aTag, aName );
	{
		if (ltIndx == kCC3MaxGLuint) 
			return;		// All the lights have been used already.
			
		m_lightIndex = ltIndx;
		m_shadows = NULL;
		m_shadowCastingVolume = NULL;
		m_cameraShadowVolume = NULL;
		m_stencilledShadowPainter = NULL;
		m_ambientColor = kCC3DefaultLightColorAmbient;
		m_diffuseColor = kCC3DefaultLightColorDiffuse;
		m_specularColor = kCC3DefaultLightColorSpecular;
		m_spotExponent = 0;
		m_spotCutoffAngle = kCC3SpotCutoffNone;
		m_attenuation = kCC3DefaultLightAttenuationCoefficients;
		m_shadowIntensityFactor = 1.0f;
		m_isDirectionalOnly = true;
		m_shouldCopyLightIndex = false;
		m_shouldCastShadowsWhenInvisible = false;
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
	
	m_ambientColor = another->getAmbientColor();
	m_diffuseColor = another->getDiffuseColor();
	m_specularColor = another->getSpecularColor();
	m_spotExponent = another->getSpotExponent();
	m_spotCutoffAngle = another->getSpotCutoffAngle();
	m_attenuation = another->getAttenuation();
	m_shadowIntensityFactor = another->getShadowIntensityFactor();
	m_isDirectionalOnly = another->isDirectionalOnly();
	m_shouldCopyLightIndex = another->shouldCopyLightIndex();
	m_shouldCastShadowsWhenInvisible = another->shouldCastShadowsWhenInvisible();
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

	if ( m_shadowCastingVolume )
		m_shadowCastingVolume->markDirty();

	if ( m_cameraShadowVolume )
		m_cameraShadowVolume->markDirty();
}

void CC3Light::turnOnWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	CC3OpenGL* gl = visitor->getGL();
	if ( isVisible() ) 
	{
		// MARK_TRACE_HERE
		//CCLOG( "Turning on CC3Light" );
		gl->enableLight( true, m_lightIndex );
		applyPositionWithVisitor( visitor );
		applyDirectionWithVisitor( visitor );
		applyAttenuationWithVisitor( visitor );
		applyColorWithVisitor( visitor );
	} else {
		gl->enableLight( false, m_lightIndex );
	}
}

void CC3Light::turnOffWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	visitor->getGL()->enableLight( false, m_lightIndex );
}

/**
 * Template method that sets the position of this light in the GL engine to the value of
 * the globalHomogeneousPosition property of this node.
 */	
void CC3Light::applyPositionWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	visitor->getGL()->setLightPosition( getGlobalHomogeneousPosition(), m_lightIndex );
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
	if (m_spotCutoffAngle <= 90.0f) 
	{
		gl->setSpotlightDirection( getGlobalForwardDirection(), m_lightIndex );
		gl->setSpotlightCutoffAngle( m_spotCutoffAngle, m_lightIndex );
		gl->setSpotlightFadeExponent( m_spotExponent, m_lightIndex );
	} else {
		gl->setSpotlightCutoffAngle( kCC3SpotCutoffNone, m_lightIndex );
	}
}

/**
 * Template method that sets the light intensity attenuation characteristics
 * in the GL engine from the attenuation property of this light.
 */
void CC3Light::applyAttenuationWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	if ( !m_isDirectionalOnly ) 
		visitor->getGL()->setLightAttenuation( m_attenuation, m_lightIndex );
}

/**
 * Template method that sets the ambient, diffuse and specular colors of this light
 * in the GL engine to the values of the ambientColor, diffuseColor and specularColor
 * properties of this node, respectively.
 */
void CC3Light::applyColorWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	CC3OpenGL* gl = visitor->getGL();
	gl->setLightAmbientColor( m_ambientColor, m_lightIndex );
	gl->setLightDiffuseColor( m_diffuseColor, m_lightIndex );
	gl->setLightSpecularColor( m_specularColor, m_lightIndex );
}

bool CC3Light::shouldCastShadowsWhenInvisible()
{
	return m_shouldCastShadowsWhenInvisible; 
}

void CC3Light::setShouldCastShadowsWhenInvisible( bool shouldCast )
{
	m_shouldCastShadowsWhenInvisible = shouldCast;
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
	
	if ( !m_shadows ) 
	{
		m_shadows = CCArray::create();			// retained
		m_shadows->retain();
	}
	m_shadows->addObject( aShadowNode );

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
    if ( m_shadows )
        m_shadows->removeObject( aShadowNode );
    
	//_shadows->removeObject( aShadowNode );
	aShadowNode->setLight( NULL );					// So it can't call back here if I'm gone
	if (m_shadows && m_shadows->count() == 0) 
	{
		m_shadows->release();
		m_shadows = NULL;
		checkShadowCastingVolume();		// Remove the shadow casting volume
		checkCameraShadowVolume();			// Remove the camera shadow volume
		checkStencilledShadowPainter();	// Remove the stencilled shadow painter
	}

	removeTransformListener( aShadowNode );
}

bool CC3Light::hasShadows()
{
	return m_shadows && m_shadows->count() > 0; 
}

void CC3Light::updateShadows()
{
	CCObject* pObj = NULL;
    int index = 0;
	CCARRAY_FOREACH( m_shadows, pObj )
	{
		CC3ShadowVolumeMeshNode* sv = (CC3ShadowVolumeMeshNode*)pObj;
        sv->updateShadow();
	}
}

CC3ShadowCastingVolume* CC3Light::getShadowCastingVolume()
{
	return m_shadowCastingVolume; 
}

/** Detaches old as camera listener, attaches new as camera listener, and attaches light. */
void CC3Light::setShadowCastingVolume( CC3ShadowCastingVolume* scVolume )
{
	if (scVolume == m_shadowCastingVolume) 
		return;

	CC3Camera* cam = getActiveCamera();
	cam->removeTransformListener( m_shadowCastingVolume );
	CC_SAFE_RELEASE( m_shadowCastingVolume );
	
	m_shadowCastingVolume = scVolume;
    
    if ( m_shadowCastingVolume )
    {
        m_shadowCastingVolume->retain();
        m_shadowCastingVolume->setLight( this );
        cam->addTransformListener( m_shadowCastingVolume );
    }
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
	if ( hasShadows() ) {
		if ( !m_shadowCastingVolume )
			setShadowCastingVolume( CC3ShadowCastingVolume::boundingVolume() );
	} else {
		setShadowCastingVolume( NULL );
	}
}

CC3CameraShadowVolume* CC3Light::getCameraShadowVolume()
{
	return m_cameraShadowVolume; 
}

/** Detaches old as camera listener, attaches new as camera listener, and attaches light. */
void CC3Light::setCameraShadowVolume( CC3CameraShadowVolume* csVolume )
{
	if (csVolume == m_cameraShadowVolume) 
		return;
		
	CC3Camera* cam = getActiveCamera();
	cam->removeTransformListener( m_cameraShadowVolume );
	CC_SAFE_RELEASE( m_cameraShadowVolume );

	m_cameraShadowVolume = csVolume;
    
    if ( csVolume )
    {
        m_cameraShadowVolume->retain();
        m_cameraShadowVolume->setLight( this );
        cam->addTransformListener( m_cameraShadowVolume );
    }
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
		if (!m_cameraShadowVolume) 
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
		if (!m_stencilledShadowPainter)
			setStencilledShadowPainter( CC3StencilledShadowPainterNode::nodeWithName( "Shadow painter", kCCC4FBlack ) );
	} else {
		setStencilledShadowPainter( NULL );
	}
}

CC3StencilledShadowPainterNode* CC3Light::getStencilledShadowPainter()
{
	return m_stencilledShadowPainter; 
}

void CC3Light::setStencilledShadowPainter( CC3StencilledShadowPainterNode* sspNode )
{
	if (sspNode == m_stencilledShadowPainter) 
		return;
	
	CC_SAFE_RELEASE( m_stencilledShadowPainter );
	CC_SAFE_RETAIN( sspNode );
	m_stencilledShadowPainter = sspNode;

	getScene()->updateRelativeLightIntensities();	//  Must be done after the ivar is set.
}

void CC3Light::updateRelativeIntensityFrom( const ccColor4F& totalLight )
{
	if (m_stencilledShadowPainter) 
	{
		GLfloat dIntensity = CCC4FIntensity(getDiffuseColor());
		GLfloat totIntensity = CCC4FIntensity(totalLight);
		GLfloat shadowIntensity =  (dIntensity / totIntensity) * m_shadowIntensityFactor;
		m_stencilledShadowPainter->setOpacity( CCOpacityFromGLfloat(shadowIntensity) );
		/*LogTrace(@"%@ updated shadow intensity to %.3f from light illumination %@ against total illumination %@ and shadow intensity factor %.3f",
					  self, (float)_stencilledShadowPainter.opacity,
					  NSStringFromCCC4F(self.diffuseColor), NSStringFromCCC4F(self.scene.totalIllumination), _shadowIntensityFactor);*/
	}
}

// TODO - combine with other shadow techniques - how to make polymorphic?
void CC3Light::drawShadowsWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	if (m_shadows && (isVisible() || shouldCastShadowsWhenInvisible()) ) 
	{
		configureStencilParameters( visitor );
		
		CCObject* pObj = NULL;
		CCARRAY_FOREACH( m_shadows, pObj )
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
	visitor->visit( m_stencilledShadowPainter );
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
	if ( m_shadows )
	{
		CCArray* myShadows = (CCArray*)m_shadows->copy();

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
	m_isDirectionalOnly = directionalOnly;
}

bool CC3Light::isDirectionalOnly()
{
	return m_isDirectionalOnly;
}

CC3LightCameraBridgeVolume::CC3LightCameraBridgeVolume()
{
	m_pLight = NULL;
}

CC3LightCameraBridgeVolume::~CC3LightCameraBridgeVolume()
{
	m_pLight = NULL;
}

// Included to satisfy compiler because property appears in interface for documentation purposes
GLuint CC3LightCameraBridgeVolume::getVertexCount()
{
	return super::getVertexCount(); 
}

CC3Light* CC3LightCameraBridgeVolume::getLight()
{
	return m_pLight; 
}

void CC3LightCameraBridgeVolume::setLight( CC3Light* aLight )
{
	m_pLight = aLight;			// weak reference
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
	return m_pLight->getGlobalHomogeneousPosition().cc3Vector(); 
}

void CC3LightCameraBridgeVolume::populateFrom( CC3LightCameraBridgeVolume* another )
{
	super::populateFrom( another );
	
	m_pLight = another->getLight();		// weak reference
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
	return aPlane.isInFront( m_pLight->getGlobalHomogeneousPosition() );
}

/** Overridden to include the homogeneous location of the light into the vertex test. */
bool CC3LightCameraBridgeVolume::areAllVerticesInFrontOf( const CC3Plane& plane )
{
	return isLightInFrontOfPlane(plane) && super::areAllVerticesInFrontOf(plane);
}

CC3ShadowCastingVolume::CC3ShadowCastingVolume()
{
	m_vertexCount = 0; 
	m_planeCount = 0;
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
	return m_planes;
}

GLuint CC3ShadowCastingVolume::getPlaneCount()
{
	updateIfNeeded();
	return m_planeCount;
}

CC3Vector* CC3ShadowCastingVolume::getVertices()
{
	updateIfNeeded();
	return m_vertices;
}

GLuint CC3ShadowCastingVolume::getVertexCount()
{
	updateIfNeeded();
	return m_vertexCount;
}

/**
 * If the specified vertex does not already exist in the vertices array,
 * it is added to the array, and the vertexCount property is incremented.
 */
void CC3ShadowCastingVolume::addUniqueVertex( const CC3Vector& aLocation )
{
	for (GLuint vtxIdx = 0; vtxIdx < m_vertexCount; vtxIdx++)
		if ( aLocation.equals( m_vertices[vtxIdx] ) ) 
			return;

	m_vertices[m_vertexCount++] = aLocation;
}

/** Adds the specified plane to the planes array, and increments the planeCount property. */
void CC3ShadowCastingVolume::addPlane( const CC3Plane& aPlane )
{
	m_planes[m_planeCount++] = aPlane; 
}

void CC3ShadowCastingVolume::checkPlaneEdge( const CC3Plane& edgePlane, const CC3Vector& v1, const CC3Vector& v2 )
{
	if ( isLightInFrontOfPlane( edgePlane ) ) 
	{
		CC3Vector v3 = m_pLight->isDirectionalOnly()
							? v2.add( getLightPosition() ) 
							: getLightPosition();
		addPlane( CC3Plane::planeFromLocations(v1, v2, v3) );
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
	m_planeCount = 0;
	m_vertexCount = 0;
	
    CC3Frustum* cf = m_pLight->getActiveCamera()->getFrustum();
	
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

	if ( !m_pLight->isDirectionalOnly() ) 
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
	return m_planes;
}

GLuint CC3CameraShadowVolume::getPlaneCount()
{
	return 6; 
}

CC3Vector* CC3CameraShadowVolume::getVertices()
{
	updateIfNeeded();
	return m_vertices;
}

GLuint CC3CameraShadowVolume::getVertexCount()
{
	updateIfNeeded();

	return (m_pLight && m_pLight->isDirectionalOnly()) ? 4 : 5;
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
    CC3Frustum* cf = m_pLight->getActiveCamera()->getFrustum();
	m_vertices[kCC3TopLeftIdx] = cf->getNearTopLeft();
	m_vertices[kCC3TopRgtIdx] = cf->getNearTopRight();
	m_vertices[kCC3BtmLeftIdx] = cf->getNearBottomLeft();
	m_vertices[kCC3BtmRgtIdx] = cf->getNearBottomRight();
	m_vertices[kCC3LightIdx] = getLightPosition();
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
	CC3Vector tl = m_vertices[kCC3TopLeftIdx];
	CC3Vector tr = m_vertices[kCC3TopRgtIdx];
	CC3Vector bl = m_vertices[kCC3BtmLeftIdx];
	CC3Vector br = m_vertices[kCC3BtmRgtIdx];
	
	// The near plane does not depend on the light position
	m_planes[kCC3NearIdx] = CC3Plane::planeFromLocations(bl, br, tr);
	
	if (m_pLight->isDirectionalOnly()) 
	{	
		// The light is infinitely far away. The light position is actually a direction to it.
		// Opposite sides are parallel and pointing in the direction of the light source.
		// For each edge of the near clipping rectangle, generate the third location on the
		// plane by adding the light direction to one of the locations on the edge. 
		lightDir = lightPos;
		
		m_planes[kCC3LeftIdx] = CC3Plane::planeFromLocations(bl, tl, tl.add( lightDir ));
		m_planes[kCC3RgtIdx] = CC3Plane::planeFromLocations(tr, br, br.add( lightDir ));
		
		m_planes[kCC3TopIdx] = CC3Plane::planeFromLocations(tl, tr, tr.add( lightDir ));
		m_planes[kCC3BotmIdx] = CC3Plane::planeFromLocations(br, bl, bl.add( lightDir ));
		
		// The far plane is parallel to the near plane, but the normal points in
		// the opposite direction. Locate the far plane at the light position,
		// and then move it out an infinite distance, in the same direction.
        m_planes[kCC3FarIdx] = CC3Plane::negate( m_planes[kCC3NearIdx] );
		m_planes[kCC3FarIdx].d = -lightPos.dot( m_planes[kCC3FarIdx].getNormal() );
		m_planes[kCC3FarIdx].d = SIGN(m_planes[kCC3FarIdx].d) * FLOAT_INFINITY;

	} else {
		
		// The light is at a definite position. All side planes meet at the light position.
		// The direction is taken from the center of the near clipping rectangle.
		lightDir = lightPos.difference( tl.average( br ) );
		
		m_planes[kCC3LeftIdx] = CC3Plane::planeFromLocations(bl, tl, lightPos);
		m_planes[kCC3RgtIdx] = CC3Plane::planeFromLocations(tr, br, lightPos);
		
		m_planes[kCC3TopIdx] = CC3Plane::planeFromLocations(tl, tr, lightPos);
		m_planes[kCC3BotmIdx] = CC3Plane::planeFromLocations(br, bl, lightPos);
		
		// The far plane is parallel to the near plane, but the normal points in
		// the opposite direction. Locate the far plane at the light position.
        m_planes[kCC3FarIdx] = CC3Plane::negate( m_planes[kCC3NearIdx] );
		m_planes[kCC3FarIdx].d = -lightPos.dot( m_planes[kCC3FarIdx].getNormal() );

	}
	
	// Finally, determine if the light source is actually behind the camera, by crossing
	// two sides of the near plane to determine the camera direction, and dotting with a
	// vector from the light position and a point on the near plane.
	CC3Vector left = tl - bl;
	CC3Vector bottom = br - bl;
	CC3Vector camDir = left.cross( bottom );
	BOOL isBehindCamera = (camDir.dot( lightDir ) < 0);
	
	if ( isBehindCamera )
    {
        m_planes[kCC3LeftIdx] = CC3Plane::negate(m_planes[kCC3LeftIdx]);
		m_planes[kCC3RgtIdx] = CC3Plane::negate(m_planes[kCC3RgtIdx]);
		m_planes[kCC3TopIdx] = CC3Plane::negate(m_planes[kCC3TopIdx]);
		m_planes[kCC3BotmIdx] = CC3Plane::negate(m_planes[kCC3BotmIdx]);
		m_planes[kCC3NearIdx] = CC3Plane::negate(m_planes[kCC3NearIdx]);
		m_planes[kCC3FarIdx] = CC3Plane::negate(m_planes[kCC3FarIdx]);
	}
}

NS_COCOS3D_END
