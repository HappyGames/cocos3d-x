/*
 * Cocos3D-X 1.0.0
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
#ifndef _CC3_COCOS_LIB_H_
#define _CC3_COCOS_LIB_H_

/// platforms
#include "Platforms/CC3Environment.h"

/// common
#include "Common/Common.h"
#include "Common/CC3Platform.h"
#include "Common/CC3String.h"
#include "Common/LineScanner.h"
#include "Common/LogDelegate.h"
#include "Common/LogManager.h"
#include "Common/CC3CC2Extensions.h"
#include "Common/CC3Math.h"
#include "Common/CC3Vector.h"
#include "Common/CC3Vector4.h"
#include "Common/CC3Quaternion.h"
#include "Common/CC3Foundation.h"

/// matrices
#include "Matrices/CC3Matrix3x3.h"
#include "Matrices/CC3Matrix4x3.h"
#include "Matrices/CC3Matrix4x4.h"
#include "Matrices/CC3Matrix.h"
#include "Matrices/CC3AffineMatrix.h"
#include "Matrices/CC3LinearMatrix.h"
#include "Matrices/CC3ProjectionMatrix.h"

/// utility
#include "Utility/CC3Backgrounder.h"
#include "Utility/CC3Cache.h"
#include "Utility/CC3DataArray.h"
#include "Utility/CC3Identifiable.h"
#include "Utility/CC3Logging.h"
#include "Utility/CC3PerformanceStatistics.h"
#include "Utility/CC3Rotator.h"

/// Nodes
#include "Nodes/CC3MeshCommon.h"
#include "Nodes/CC3NodeListeners.h"
#include "Nodes/CC3Node.h"
#include "Nodes/CC3BoundingVolumes.h"
#include "Nodes/CC3Camera.h"
#include "Nodes/CC3EnvironmentNodes.h"
#include "Nodes/CC3Light.h"
#include "Nodes/CC3LocalContentNode.h"
#include "Nodes/CC3MeshNode.h"
#include "Nodes/CC3BitmapLabelNode.h"
#include "Nodes/CC3NodeVisitor.h"
#include "Nodes/CC3UtilityMeshNodes.h"
#include "Nodes/CC3Billboard.h"

/// meshes
#include "Meshes/CC3VertexArrays.h"
#include "Meshes/CC3Mesh.h"
#include "Meshes/CC3VertexSkinning.h"

/// Animation
#include "Animations/CC3Actions.h"
#include "Animations/CC3NodeAnimation.h"
#include "Animations/CC3ActionManager.h"

/// resources
#include "Resources/CC3DataStreams.h"
#include "Resources/CC3Resource.h"
#include "Resources/CC3ResourceNode.h"
#include "Resources/CC3NodesResource.h"

/// shaders
#include "Shaders/CC3GLSLVariable.h"
#include "Shaders/CC3ShaderContext.h"
#include "Shaders/CC3ShaderSemantics.h"
#include "Shaders/CC3ShaderMatcher.h"
#include "Shaders/CC3Shaders.h"

/// materials
#include "Materials/CC3Material.h"
#include "Materials/CC3STBImage.h"
#include "Materials/CC3Texture.h"
#include "Materials/CC3TextureUnit.h"

/// cc3PVR
#include "cc3PVR/CC3PVRTPFXParser.h"
#include "cc3PVR/CC3PFXResource.h"
#include "cc3PVR/CC3PODResource.h"
#include "cc3PVR/CC3PODCamera.h"
#include "cc3PVR/CC3PODResourceNode.h"
#include "cc3PVR/CC3PODLight.h"
#include "cc3PVR/CC3PODMesh.h"
#include "cc3PVR/CC3PODNode.h"
#include "cc3PVR/CC3PODResourceNode.h"
#include "cc3PVR/CC3PODVertexSkinning.h"
#include "cc3PVR/CC3PVRFoundation.h"
#include "cc3PVR/CC3PVRShamanShaderSemantics.h"
#include "cc3PVR/CC3PVRTexture.h"
#include "cc3PVR/CC3PVRTModelPOD.h"
#include "cc3PVR/CC3PVRTTexture.h"
#include "cc3PVR/CC3PODMaterial.h"
#include "cc3PVR/CC3PODMeshNode.h"
#include "cc3PVR/CC3PVROpenGLFoundation.h"

/// controls
#include "Controls/CCNodeAdornments.h"
#include "Controls/Joystick.h"

/// opengl
#include "OpenGL/CC3OpenGL.h"
#include "OpenGL/CC3OpenGLFoundation.h"
#include "OpenGL/CC3OpenGLProgPipeline.h"
#include "OpenGL/CC3OpenGLUtility.h"
#include "OpenGL/OpenGL2/CC3OpenGL2.h"
#include "OpenGL/OpenGL2/CC3OpenGLCompatibility.h"
#include "OpenGL/OpenGLES2/CC3OpenGLES2.h"
#include "OpenGL/OpenGLES2/CC3OpenGLES2Compatibility.h"

/// particle system
#include "Particles/CC3Particle.h"
#include "Particles/CC3CVAParticle.h"
#include "Particles/CC3MeshParticle.h"
#include "Particles/CC3PointParticle.h"
#include "Particles/CC3ParticleNavigator.h"
#include "Particles/CC3ParticleEmitter.h"
#include "Particles/CC3CVAParticleEmitter.h"
#include "Particles/CC3MeshParticleEmitter.h"
#include "Particles/CC3PointParticleEmitter.h"

/// cc3Extras
#include "cc3Extras/CC3ParticleSamples.h"
#include "cc3Extras/CC3PointParticleSamples.h"
#include "cc3Extras/CC3MeshParticleSamples.h"
#include "cc3Extras/CC3ModelSampleFactory.h"

/// scenes
#include "Scenes/CC3Layer.h"
#include "Scenes/CC3NodeSequencer.h"
#include "Scenes/CC3RenderSurfaces.h"
#include "Scenes/CC3Scene.h"

/// shadows
#include "Shadows/CC3ShadowVolumes.h"


#endif
