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

CC3PointParticleEmitter::CC3PointParticleEmitter()
{

}

CC3PointParticleEmitter::~CC3PointParticleEmitter()
{
	CC3Camera* pCam = getActiveCamera();
	if ( pCam )
		pCam->removeTransformListener( this );
}

GLfloat CC3PointParticleEmitter::getNormalizedParticleSize()
{
	return normalizeParticleSizeToDevice( getParticleSize() );
}

GLfloat CC3PointParticleEmitter::getNormalizedParticleSizeMinimum()
{
	return normalizeParticleSizeToDevice( getParticleSizeMinimum() );
}

GLfloat CC3PointParticleEmitter::getNormalizedParticleSizeMaximum()
{
	return normalizeParticleSizeToDevice( getParticleSizeMaximum() );
}

GLfloat CC3PointParticleEmitter::getUnityScaleDistance()
{
	GLfloat sqDistAtten = _particleSizeAttenuation.c;
	return (sqDistAtten > 0.0f) ? sqrt(1.0f / sqDistAtten) : 0.0f;
}

void CC3PointParticleEmitter::setUnityScaleDistance( GLfloat aDistance )
{
	if (aDistance > 0.0)
		_particleSizeAttenuation = CC3AttenuationCoefficientsMake( 0.0f, 0.0f, 1.0f / (aDistance * aDistance) );
	else
		_particleSizeAttenuation = kCC3AttenuationNone;
}

// Overridden to retain all vertex content in memory.
void CC3PointParticleEmitter::setVertexContentTypes( CC3VertexContent vtxContentTypes )
{
	super::setVertexContentTypes( vtxContentTypes );
	setDrawingMode( GL_POINTS );
}

GLfloat CC3PointParticleEmitter::getParticleSizeAt( GLuint vtxIndex )
{
	return _mesh ? denormalizeParticleSizeFromDevice( _mesh->getVertexPointSizeAt(vtxIndex) ) : 0.0f;
}

void CC3PointParticleEmitter::setParticleSize( GLfloat aSize, GLuint vtxIndex )
{
	_mesh->setVertexPointSize( normalizeParticleSizeToDevice(aSize), vtxIndex );
	addDirtyVertex( vtxIndex );
}

void CC3PointParticleEmitter::setParticleSize( GLfloat particleSize )
{
	_particleSize = particleSize;
}

GLfloat CC3PointParticleEmitter::getParticleSize()
{
	return _particleSize;
}

GLfloat CC3PointParticleEmitter::getParticleSizeMaximum()
{
	return _particleSizeMaximum;
}

GLfloat CC3PointParticleEmitter::getParticleSizeMinimum()
{
	return _particleSizeMinimum;
}

CC3AttenuationCoefficients CC3PointParticleEmitter::getParticleSizeAttenuation()
{		
	return _particleSizeAttenuation;
}

bool CC3PointParticleEmitter::shouldNormalizeParticleSizesToDevice()
{
	return _shouldNormalizeParticleSizesToDevice;
}

bool CC3PointParticleEmitter::shouldSmoothPoints()
{
	return _shouldSmoothPoints;
}

void CC3PointParticleEmitter::updateParticleSizesGLBuffer()
{
	getMesh()->updatePointSizesGLBuffer(); 
}

void CC3PointParticleEmitter::retainVertexPointSizes()
{
	getMesh()->retainVertexPointSizes();
	super::retainVertexPointSizes();
}

void CC3PointParticleEmitter::doNotBufferVertexPointSizes()
{
	getMesh()->doNotBufferVertexPointSizes();
	super::doNotBufferVertexPointSizes();
}

void CC3PointParticleEmitter::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	_globalCameraLocation = CC3Vector::kCC3VectorNull;
	_areParticleNormalsDirty = false;
	setParticleSize( kCC3DefaultParticleSize );
	_particleSizeMinimum = kCC3ParticleSizeMinimumNone;
	_particleSizeMaximum = kCC3ParticleSizeMaximumNone;
	_particleSizeAttenuation = kCC3AttenuationNone;
	_shouldSmoothPoints = false;
	_shouldNormalizeParticleSizesToDevice = true;
	_shouldDisableDepthMask = true;
	deviceScaleFactor();	// Force init the static deviceScaleFactor before accessing it.
}

/** Overridden to configure for blending. */
CC3Material* CC3PointParticleEmitter::makeMaterial()
{
	CC3Material* mat = super::makeMaterial();
	mat->setDiffuseColor( kCCC4FWhite );
	mat->setSourceBlend( GL_SRC_ALPHA );
	mat->setDestinationBlend( GL_ONE_MINUS_SRC_ALPHA );
	return mat;
}
	
void CC3PointParticleEmitter::populateFrom( CC3PointParticleEmitter* another )
{
	super::populateFrom( another );
	
	_particleSize = another->getParticleSize();
	_particleSizeMinimum = another->getParticleSizeMinimum();
	_particleSizeMaximum = another->getParticleSizeMaximum();
	_shouldSmoothPoints = another->shouldSmoothPoints();
	_shouldNormalizeParticleSizesToDevice = another->shouldNormalizeParticleSizesToDevice();
	_particleSizeAttenuation = another->getParticleSizeAttenuation();
}

CCObject* CC3PointParticleEmitter::copyWithZone( CCZone* zone )
{
	CC3PointParticleEmitter* pVal = new CC3PointParticleEmitter;
	pVal->init();
	pVal->populateFrom( this );
	pVal->addCopiesOfChildrenFrom( this );

	return pVal;
}

void CC3PointParticleEmitter::initializeParticle( CC3Particle* aPointParticle )
{
	super::initializeParticle( aPointParticle );
	
	// particleCount not yet incremented, so it points to this particle
	aPointParticle->setParticleIndex( _particleCount );

	// Set the particle size directly so the CC3PointParticleProtocol does not need to support size
	setParticleSize( getParticleSize(), _particleCount );
}

/** Marks the range of vertices in the underlying mesh that are affected by this particle. */
void CC3PointParticleEmitter::acceptParticle( CC3Particle* aParticle )
{
	super::acceptParticle( aParticle );
	setParticleNormal( aParticle );
}

/** Returns whether this mesh is making use of normals and lighting. */
bool CC3PointParticleEmitter::hasIlluminatedNormals()
{
	return _mesh && _mesh->hasVertexNormals() && shouldUseLighting(); 
}

void CC3PointParticleEmitter::markTransformDirty()
{
	super::markTransformDirty();
	_areParticleNormalsDirty = true;
}

void CC3PointParticleEmitter::nodeWasTransformed( CC3Node* aNode )
{
	super::nodeWasTransformed( aNode );
	if ( aNode->isCamera() ) 
	{
		_globalCameraLocation = aNode->getGlobalLocation();
		_areParticleNormalsDirty = true;
	}
}

/** Overridden to update the normals of the particles before the GL mesh is updated by parent.  */
void CC3PointParticleEmitter::processUpdateAfterTransform( CC3NodeUpdatingVisitor* visitor )
{
	updateParticleNormals( visitor );
	super::processUpdateAfterTransform( visitor );
}

/**
 * If the particles have normals that interact with lighting, and the global direction from the
 * emitter to the camera has changed (by a movement of either the emitter or the camera), this
 * method transforms that direction to the local coordinates of the emitter and points the normal
 * vector of each particle to the new local camera direction. Each particle will point in a slightly
 * different direction, depending on how far it is from the origin of the emitter.
 */
void CC3PointParticleEmitter::updateParticleNormals( CC3NodeUpdatingVisitor* visitor )
{
	if ( hasIlluminatedNormals() )
	{
		// If we haven't already registered as a camera listener, do so now.
		// Get the current cam location, because cam might not immediately callback.
		if ( _globalCameraLocation.isNull() ) 
		{
			CC3Camera* cam = getActiveCamera();
			_globalCameraLocation = cam->getGlobalLocation();
			cam->addTransformListener( this );
			CC3_TRACE("[ptc]CC3PointParticleEmitter registered as listener of camera at %s", _globalCameraLocation.stringfy().c_str());
		}

		if (_areParticleNormalsDirty) 
		{
			CC3_TRACE("[ptc]CC3PointParticleEmitter updating particle normals from camera location %s", _globalCameraLocation.stringfy().c_str());
			
			// Get the direction to the camera and transform it to local coordinates
			CC3Vector camDir = _globalCameraLocation.difference( getGlobalLocation() );
			camDir = getGlobalTransformMatrixInverted()->transformDirection( camDir );

			CCObject* pObj = NULL;
			CCARRAY_FOREACH( _particles, pObj )
			{
				CC3Particle* particle = (CC3Particle*)pObj;
				particle->pointNormalAt( camDir );
			}
		}
	}
}

/**
 * Determine the global vector that points from the emitter to the camera,
 * transform it to the local coordinates of the emitter and point the
 * particle normal to the new local camera location.
 
 * Determines the global direction from the emitter to the camera, transforms it to the
 * local coordinates of the emitter and points the normal vector of the particle to the
 * new local camera location. Each particle normal will point in a slightly different
 * direction, depending on how far the particle is from the origin of the emitter.
 */
void CC3PointParticleEmitter::setParticleNormal( CC3Particle* pointParticle )
{
	if ( hasIlluminatedNormals() )
	{
		CC3Vector camDir = _globalCameraLocation.difference( getGlobalLocation() );
		camDir = getGlobalTransformMatrixInverted()->transformDirection( camDir );
		pointParticle->pointNormalAt( camDir );
	}
}

CC3PointParticle* CC3PointParticleEmitter::getPointParticleAt( GLuint aParticleIndex )
{
	return (CC3PointParticle*)getParticleAt( aParticleIndex );
}

/**
 * Remove the current particle from the active particles, but keep it cached for future use.
 * To do this, decrement the particle count and swap the current particle with the last living
 * particle. This is done without releasing either particle from the particles collection.
 *
 * The previously-last particle is now in the slot that the removed particle was taken from,
 * and vice-versa. Update their indices, and move the underlying vertex data.
 */
void CC3PointParticleEmitter::removeParticle( CC3Particle* aParticle, GLuint anIndex )
{
	super::removeParticle( aParticle, anIndex );		// Decrements particleCount and vertexCount
	
	// Get the last living particle
	CC3PointParticle* lastParticle = getPointParticleAt( _particleCount );
	
	// Swap the particles in the particles array
	_particles->exchangeObjectAtIndex( anIndex, _particleCount );
	
	// Update the particle's index. This also updates the vertex indices array, if it exists.
	aParticle->setParticleIndex( _particleCount );
	lastParticle->setParticleIndex( anIndex );
	
	// Update the underlying mesh
	getMesh()->copyVertices( 1, _particleCount, anIndex );
	
	// Mark the vertex and vertex indices as dirty
	addDirtyVertex( anIndex );
	addDirtyVertexIndex( anIndex );
}

/** Overridden to set the particle properties in addition to other configuration. */
void CC3PointParticleEmitter::configureDrawingParameters( CC3NodeDrawingVisitor* visitor )
{
	super::configureDrawingParameters( visitor );
	configurePointProperties( visitor );
}

/**
 * Enable particles in each texture unit being used by the material,
 * and set GL point size, size attenuation and smoothing.
 */
void CC3PointParticleEmitter::configurePointProperties( CC3NodeDrawingVisitor* visitor )
{
	CC3OpenGL* gl = visitor->getGL();

	// Enable point sprites and shader point sizing (gl_PointSize) for OGL
	gl->enablePointSprites( true );
	gl->enableShaderPointSize( true );

	// Enable texture coordinate replacing in each texture unit used by the material.
	GLuint texCount = getTextureCount();
	for (GLuint texUnit = 0; texUnit < texCount; texUnit++)
		gl->enablePointSpriteCoordReplace( true, texUnit );
	
	// Set default point size
	gl->setPointSize( getNormalizedParticleSize() );
	gl->setPointSizeMinimum( getNormalizedParticleSizeMinimum() );
	gl->setPointSizeMaximum( getNormalizedParticleSizeMaximum() );
	gl->setPointSizeAttenuation( _particleSizeAttenuation );
	gl->enablePointSmoothing( _shouldSmoothPoints );
}

void CC3PointParticleEmitter::cleanupDrawingParameters( CC3NodeDrawingVisitor* visitor )
{
	super::cleanupDrawingParameters( visitor );
	cleanupPointProperties( visitor );
}

/** Disable particles again in each texture unit being used by the material. */
void CC3PointParticleEmitter::cleanupPointProperties( CC3NodeDrawingVisitor* visitor )
{
	CC3OpenGL* gl = visitor->getGL();

	// Disable point sprites again, but leave point sizing enabled
	gl->enablePointSprites( false );

	// Disable texture coordinate replacing again in each texture unit used by the material.
	GLuint texCount = getTextureCount();
	for (GLuint texUnit = 0; texUnit < texCount; texUnit++)
		gl->enablePointSpriteCoordReplace( false, texUnit );
}

CC3PointParticleEmitter* CC3PointParticleEmitter::nodeWithName( const std::string& aName )
{
	CC3PointParticleEmitter* pEmitter = new CC3PointParticleEmitter;
	pEmitter->initWithName( aName );
	pEmitter->autorelease();

	return pEmitter;
}

#define kCC3DeviceScaleFactorBase 480.0f
static GLfloat _deviceScaleFactor = 0.0f;

GLfloat CC3PointParticleEmitter::deviceScaleFactor()
{
	if ( _deviceScaleFactor == 0.0f ) 
	{
		CCSize viewSize = CCDirector::sharedDirector()->getWinSizeInPixels();
		_deviceScaleFactor = MAX(viewSize.height, viewSize.width) / kCC3DeviceScaleFactorBase;
	}

	return _deviceScaleFactor;
}

/**
 * Converts the specified nominal particle size to a device-normalized size,
 * if the shouldNormalizeParticleSizesToDevice property is set to YES.
 *
 * For speed, this method accesses the deviceScaleFactor static variable directly.
 * the deviceScaleFactor method must be invoked once before this access occurs
 * in order to initialize this value correctly.
 */
GLfloat CC3PointParticleEmitter::normalizeParticleSizeToDevice( GLfloat aSize )
{
	return _shouldNormalizeParticleSizesToDevice ? (aSize * _deviceScaleFactor) : aSize;
}

/**
 * Converts the specified device-normalized particle size to a consistent nominal size,
 * if the shouldNormalizeParticleSizesToDevice property is set to YES.
 *
 * For speed, this method accesses the deviceScaleFactor static variable directly.
 * the deviceScaleFactor method must be invoked once before this access occurs
 * in order to initialize this value correctly.
 */
GLfloat CC3PointParticleEmitter::denormalizeParticleSizeFromDevice( GLfloat aSize )
{
	return _shouldNormalizeParticleSizesToDevice ? (aSize / _deviceScaleFactor) : aSize;
}

NS_COCOS3D_END
