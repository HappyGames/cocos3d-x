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
#ifndef _CC3_POINT_PARTICLES_H_
#define _CC3_POINT_PARTICLES_H_
#include "CC3CVAParticleEmitter.h"

NS_COCOS3D_BEGIN

class CC3PointParticle;
/**
 * CC3PointParticleEmitter emits particles that conform to the CC3PointParticleProtocol protocol.
 *
 * Each particle has its own location, and may optionally be configued with its own color
 * and individual size, and each particle may be configured with a vertex normal so that
 * it can interact with light sources. This particle content is defined by the
 * vertexContentTypes property of this emitter. 
 * 
 * Each point particle emitted displays the same texture, which is determined by the texture
 * property of this emitter node. Be aware that OpenGL point particles use the entire texture,
 * which you should generally ensure has dimensions that are power-of-two. Non-POT textures will
 * be padded by iOS when loaded, for compatibility with the graphics hardware. Although the
 * padding is generally transparent, it may throw off the expected location of your particle.
 *
 * In general, point particles will contain transparent content. As such, you will likely want
 * to set the blendFunc property to one of the following:
 *   - {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA} - Standard realistic translucent blending
 *                                              (this is the initial setting).
 *   - {GL_SRC_ALPHA, GL_ONE} - Additive blending, to have overlapping particles build on,
 *                              and intensify, each other
 *
 * For CC3PointParticleEmitter, the initial value of the shouldDisableDepthMask property
 * is YES, so that the particles do not enage in Z-fighting with each other. You can
 * experiment with changing this to NO if your emitter is better suited to it.
 *
 * You can also experiment with the shouldDisableDepthTest and depthFunction properties
 * to see if change them helps you get the look you are trying to achieve.
 *
 * You can control characteristics about the sizes of the particles, and how that size should
 * change with distance from the camera, using the particleSize, particleSizeMinimum,
 * particleSizeMaximum, particleSizeAttenuation, and unityScaleDistance properties.
 *
 * All memory used by the particles and the underlying vertex mesh is managed by the
 * emitter node, and is deallocated automatically when the emitter is released.
 */
class CC3PointParticleEmitter : public CC3CommonVertexArrayParticleEmitter 
{
	DECLARE_SUPER( CC3CommonVertexArrayParticleEmitter );
public:
	CC3PointParticleEmitter();
	~CC3PointParticleEmitter();
	/**
	 * If the kCC3VertexContentPointSize component was not specified in the vertexContentTypes
	 * property, all particles will be emitted at the same size, as specified by this property.
	 *
	 * If the kCC3VertexContentPointSize component was specified, the size of each particle can
	 * be individually set during the initialization of that particle. The size of each particle
	 * defaults to this value, if not set to something else during its initialization.
	 *
	 * The initial value is kCC3DefaultParticleSize.
	 */
	GLfloat						getParticleSize();
	void						setParticleSize( GLfloat particleSize );

	/**
	 * The miniumum size for point particles. Particle sizes will not be allowed to shrink below this
	 * value when distance attenuation is engaged.
	 *
	 * You can use this property to limit how small particles will become as they recede from the camera.
	 *
	 * The initial value of this property is kCC3ParticleSizeMinimumNone, indicating that particles
	 * will be allowed to shrink to one pixel if needed.
	 */
	GLfloat						getParticleSizeMinimum();
	void						setParticleSizeMinimum( GLfloat minSize );

	/**
	 * The maxiumum size for point particles. Particle sizes will not be allowed to grow beyond this
	 * value when distance attenuation is engaged.
	 *
	 * You can use this property to limit how large particles will become as they approach the camera.
	 *
	 * The initial value of this property is kCC3ParticleSizeMaximumNone, indicating that particles
	 * will be allowed to grow until clamped by any platform limits.
	 */
	GLfloat						getParticleSizeMaximum();
	void						setParticleSizeMaximum( GLfloat maxSize );

	/**
	 * The distance from the camera, in 3D space, at which the particle will be displayed
	 * at unity scale (its natural size).
	 *
	 * The value of this property defines how the apparent size of the particle will change as it
	 * moves closer to, or farther from, the camera. If the particle is closer to the camera than
	 * this distance, the particle will appear proportionally larger than its natural size, and if
	 * the particle is farther away from the camera than this distance, the particle will appear
	 * proportionally smaller than its natural size.
	 * 
	 * The natural size of the particle is expressed in pixels and is set either by the particleSize
	 * property of this emitter, or by the size property of the individual particle if the
	 * vertexContentTypes property of this emitter includes the kCC3VertexContentPointSize value.
	 *
	 * Setting the value of this property to zero indicates that the size of the particles should stay
	 * constant, at their natural size, regardless of how far the particle is from the camera.
	 *
	 * Setting this property replaces the need to set the value of the particleSizeAttenuation property,
	 * which offers a wider range of distance attenuation options, but is more complicated to use.
	 *
	 * The initial value of this property is zero, indicating that distance attenuation is not applied,
	 * and each particle will appear at its natural size regardless of how far it is from the camera.
	 */
	GLfloat						getUnityScaleDistance();
	void						setUnityScaleDistance( GLfloat unityScaleDistance );

	/**
	 * The coefficients of the attenuation function that affects the size of a particle based on its
	 * distance from the camera. The sizes of the particles are attenuated according to the formula
	 * 1/sqrt(a + (b * r) + (c * r * r)), where r is the radial distance from the particle to the camera,
	 * and a, b and c are the coefficients from this property.
	 *
	 * As an alternate to setting this property, you can set the unityScaleDistance property to establish
	 * standard proportional distance attenuation.
	 *
	 * The initial value of this property is kCC3AttenuationNone, indicating no attenuation with distance.
	 */
	CC3AttenuationCoefficients	getParticleSizeAttenuation();
	void						setParticleSizeAttenuation( const CC3AttenuationCoefficients& coefficients );

	/**
	 * Indicates whether the particle sizes should be adjusted so that particles appear
	 * to be a consistent size across all device screen resolutions
	 *
	 * The 3D camera frustum is consistent across all devices, making the view of the 3D scene consistent
	 * across all devices. However, particle size is defined in terms of pixels, and particles will appear
	 * larger or smaller. relative to 3D artifacts, on different screen resolutions.
	 * 
	 * If this property is set to YES, the actual size of each particle, as submitted to the GL engine,
	 * will be adjusted so that it appears to be the same size across all devices, relative to the 3D nodes.
	 *
	 * If this property is set to NO, the actual size of each particle will be drawn in the same absolute
	 * pixel size across all devices, which may make it appear to be smaller or larger, relative to the
	 * 3D artifacts around it, on different devices.
	 *
	 * The initial value of this property is YES.
	 */
	bool						shouldNormalizeParticleSizesToDevice();
	void						setShouldNormalizeParticleSizesToDevice( bool shouldNormalized );

	/**
	 * Returns the value of the particleSize property. If the shouldNormalizeParticleSizesToDevice
	 * property is set to YES, the returned value will be normalized. For further explanation, see
	 * the notes for the shouldNormalizeParticleSizesToDevice property.
	 */
	GLfloat						getNormalizedParticleSize();

	/**
	 * Returns the value of the particleSizeMinimum property. If the shouldNormalizeParticleSizesToDevice
	 * property is set to YES, the returned value will be normalized. For further explanation, see
	 * the notes for the shouldNormalizeParticleSizesToDevice property.
	 */
	GLfloat						getNormalizedParticleSizeMinimum();

	/**
	 * Returns the value of the particleSizeMaximum property. If the shouldNormalizeParticleSizesToDevice
	 * property is set to YES, the returned value will be normalized. For further explanation, see
	 * the notes for the shouldNormalizeParticleSizesToDevice property.
	 */
	GLfloat						getNormalizedParticleSizeMaximum();

	/** Indicates whether points should be smoothed (antialiased). The initial value is NO. */
	bool						shouldSmoothPoints();
	void						setShouldSmoothPoints( bool shouldSmooth );

	/**
	 * Returns the particle size element at the specified index from the vertex data.
	 *
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * the vertexStride and elementOffset properties to access the correct element.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex data has been released, this method will raise an assertion exception.
	 *
	 * You typically do not use this method directly. Instead, use the size property
	 * of the individual particle from within your custom CC3PointParticle subclass.
	 */
	GLfloat						getParticleSizeAt( GLuint vtxIndex );

	/**
	 * Sets the particle size element at the specified index in the vertex data to the specified value.
	 * 
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * the vertexStride and elementOffset properties to access the correct element.
	 *
	 * When all vertex changes have been made, be sure to invoke the updateParticleSizesGLBuffer
	 * method to ensure that the GL VBO that holds the vertex data is updated.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex data has been released, this method will raise an assertion exception.
	 *
	 * You typically do not use this method directly. Instead, use the size property
	 * of the individual particle from within your custom CC3PointParticle subclass.
	 */
	void						setParticleSize( GLfloat aSize, GLuint vtxIndex );

	/**
	 * Updates the GL engine buffer with the particle size data in this mesh.
	 *
	 * For particle emitters, this method is invoked automatically when particles
	 * have been updated from within your CC3PointParticle subclass. Usually, the
	 * application should never have need to invoke this method directly. 
	 */
	void						updateParticleSizesGLBuffer();

	/**
	 * Convenience method to cause the vertex point size data to be retained in application
	 * memory when releaseRedundantContent is invoked, even if it has been buffered to a GL VBO.
	 *
	 * Only the vertex point sizes will be retained. Any other vertex data, such as locations,
	 * or texture coordinates, that has been buffered to GL VBO's, will be released from
	 * application memory when releaseRedundantContent is invoked.
	 *
	 * This method is invoked automatically when the vertexContentTypes property is set.
	 * Usually, the application should never have need to invoke this method directly.
	 */
	void						retainVertexPointSizes();

	/**
	 * Convenience method to cause the vertex point size data to be skipped when createGLBuffers
	 * is invoked. The vertex data is not buffered to a GL VBO, is retained in application memory,
	 * and is submitted to the GL engine on each frame render.
	 *
	 * Only the vertex point sizes will not be buffered to a GL VBO. Any other vertex content, such as
	 * locations, or texture coordinates, will be buffered to a GL VBO when createGLBuffers is invoked.
	 *
	 * This method causes the vertex data to be retained in application memory, so, if you have
	 * invoked this method, you do NOT also need to invoke the retainVertexPointSizes method.
	 */
	void						doNotBufferVertexPointSizes();

	/** Returns the particle at the specified index within the particles array, cast as a point particle. */
	CC3PointParticle*			getPointParticleAt( GLuint aParticleIndex );

	/**
	 * Converts the specified nominal particle size to a device-normalized size,
	 * if the shouldNormalizeParticleSizesToDevice property is set to YES.
	 *
	 * For speed, this method accesses the deviceScaleFactor static variable directly.
	 * the deviceScaleFactor method must be invoked once before this access occurs
	 * in order to initialize this value correctly.
	 */
	GLfloat						normalizeParticleSizeToDevice( GLfloat aSize );

	/**
	 * Converts the specified device-normalized particle size to a consistent nominal size,
	 * if the shouldNormalizeParticleSizesToDevice property is set to YES.
	 *
	 * For speed, this method accesses the deviceScaleFactor static variable directly.
	 * the deviceScaleFactor method must be invoked once before this access occurs
	 * in order to initialize this value correctly.
	 */
	GLfloat						denormalizeParticleSizeFromDevice( GLfloat aSize );

	// Overridden to retain all vertex content in memory.
	void						setVertexContentTypes( CC3VertexContent vtxContentTypes );

	void						initWithTag( GLuint aTag, const std::string& aName );
	/** Overridden to configure for blending. */
	CC3Material*				makeMaterial();

	/**
	 * The scaling factor used to adjust the particle size so that it is drawn at a consistent
	 * size across all device screen resolutions, if the  shouldNormalizeParticleSizesToDevice
	 * property of the emitter is set to YES.
	 *
	 * The value returned depends on the device screen window size and is normalized to the
	 * original iPhone/iPod Touch screen size of 480 x 320. The value returned for an original
	 * iPhone or iPod Touch will be 1.0. The value returned for other devices depends on the
	 * screen resolution, and formally, on the screen height as measured in pixels.
	 * Devices with larger screen heights in pixels will return a value greater than 1.0.
	 * Devices with smaller screen heights in pixels will return a value less than 1.0
	 */
	static GLfloat				deviceScaleFactor();
	
	void						populateFrom( CC3PointParticleEmitter* another );
	virtual CCObject*			copyWithZone( CCZone* zone );

	void						initializeParticle( CC3Particle* aPointParticle );
	/** Marks the range of vertices in the underlying mesh that are affected by this particle. */
	void						acceptParticle( CC3Particle* aParticle );
	/**
	 * Determine the global vector that points from the emitter to the camera,
	 * transform it to the local coordinates of the emitter and point the
	 * particle normal to the new local camera location.
 
	 * Determines the global direction from the emitter to the camera, transforms it to the
	 * local coordinates of the emitter and points the normal vector of the particle to the
	 * new local camera location. Each particle normal will point in a slightly different
	 * direction, depending on how far the particle is from the origin of the emitter.
	 */
	void						setParticleNormal( CC3Particle* pointParticle );
	/** Returns whether this mesh is making use of normals and lighting. */
	bool						hasIlluminatedNormals();
	void						markTransformDirty();
	void						nodeWasTransformed( CC3Node* aNode );

	/** Overridden to update the normals of the particles before the GL mesh is updated by parent.  */
	void						processUpdateAfterTransform( CC3NodeUpdatingVisitor* visitor );

	/**
	 * If the particles have normals that interact with lighting, and the global direction from the
	 * emitter to the camera has changed (by a movement of either the emitter or the camera), this
	 * method transforms that direction to the local coordinates of the emitter and points the normal
	 * vector of each particle to the new local camera direction. Each particle will point in a slightly
	 * different direction, depending on how far it is from the origin of the emitter.
	 */
	void						updateParticleNormals( CC3NodeUpdatingVisitor* visitor );

	/**
	 * Remove the current particle from the active particles, but keep it cached for future use.
	 * To do this, decrement the particle count and swap the current particle with the last living
	 * particle. This is done without releasing either particle from the particles collection.
	 *
	 * The previously-last particle is now in the slot that the removed particle was taken from,
	 * and vice-versa. Update their indices, and move the underlying vertex data.
	 */
	void						removeParticle( CC3Particle* aParticle, GLuint anIndex );

	/** Overridden to set the particle properties in addition to other configuration. */
	void						configureDrawingParameters( CC3NodeDrawingVisitor* visitor );
	
	/**
	 * Enable particles in each texture unit being used by the material,
	 * and set GL point size, size attenuation and smoothing.
	 */
	void						configurePointProperties( CC3NodeDrawingVisitor* visitor );

	void						cleanupDrawingParameters( CC3NodeDrawingVisitor* visitor );

	/** Disable particles again in each texture unit being used by the material. */
	void						cleanupPointProperties( CC3NodeDrawingVisitor* visitor );

	static CC3PointParticleEmitter*	nodeWithName( const std::string& aName );

protected:
	CC3Vector					_globalCameraLocation;
	CC3AttenuationCoefficients	_particleSizeAttenuation;
	GLfloat						_particleSize;
	GLfloat						_particleSizeMinimum;
	GLfloat						_particleSizeMaximum;
	bool						_shouldSmoothPoints : 1;
	bool						_shouldNormalizeParticleSizesToDevice : 1;
	bool						_areParticleNormalsDirty : 1;
};

NS_COCOS3D_END

#endif
