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

CC3ShadowVolumeMeshNode::CC3ShadowVolumeMeshNode()
{
	_light = NULL;
}

CC3ShadowVolumeMeshNode::~CC3ShadowVolumeMeshNode()
{
	if ( _light )
	{
		_light->removeShadow( this );
		_light = NULL;
	}
}

bool CC3ShadowVolumeMeshNode::isShadowVolume()
{
	return true; 
}

/** Create the shadow volume mesh once the parent is attached. */
void CC3ShadowVolumeMeshNode::setParent( CC3Node* aNode )
{	
	super::setParent( aNode );
	createShadowMesh();
}

/**
 * If shadow volume should be visible, add a material
 * to display the volume, otherwise get rid of it.
 */
void CC3ShadowVolumeMeshNode::setVisible( bool _isVisible )
{
	super::setVisible( _isVisible );
	checkShadowMaterial();
}

void CC3ShadowVolumeMeshNode::setShouldDrawTerminator( bool shouldDraw )
{
	_shouldDrawTerminator = shouldDraw;
	setDrawingMode( _shouldDrawTerminator ? GL_LINES : GL_TRIANGLES );
	checkShadowMaterial();
}

GLushort CC3ShadowVolumeMeshNode::getShadowLagFactor()
{
	return _shadowLagFactor; 
}

void CC3ShadowVolumeMeshNode::setShadowLagFactor( GLushort lagFactor )
{
	_shadowLagFactor = MAX(lagFactor, 1);
	super::setShadowLagFactor( lagFactor );
}

GLushort CC3ShadowVolumeMeshNode::getShadowLagCount()
{
	return _shadowLagCount; 
}

void CC3ShadowVolumeMeshNode::setShadowLagCount( GLushort lagCount )
{
	_shadowLagCount = lagCount;
	super::setShadowLagCount( lagCount );
}

bool CC3ShadowVolumeMeshNode::shouldShadowFrontFaces()
{
	return _shouldShadowFrontFaces; 
}

void CC3ShadowVolumeMeshNode::setShouldShadowFrontFaces( bool shouldShadow )
{
	_shouldShadowFrontFaces = shouldShadow;
	super::setShouldShadowFrontFaces( shouldShadow );
}

bool CC3ShadowVolumeMeshNode::shouldShadowBackFaces()
{
	return _shouldShadowBackFaces; 
}

void CC3ShadowVolumeMeshNode::setShouldShadowBackFaces( bool shouldShadow )
{
	_shouldShadowBackFaces = shouldShadow;
	super::setShouldShadowBackFaces( shouldShadow );
}

GLfloat CC3ShadowVolumeMeshNode::getShadowOffsetFactor()
{
	return _decalOffsetFactor ? _decalOffsetFactor : super::getShadowOffsetFactor(); 
}

void CC3ShadowVolumeMeshNode::setShadowOffsetFactor( GLfloat factor )
{
	_decalOffsetFactor = factor;
	super::setShadowOffsetFactor( factor );
}

GLfloat CC3ShadowVolumeMeshNode::shadowOffsetUnits()
{
	return _decalOffsetUnits ? _decalOffsetUnits : super::getShadowOffsetUnits(); 
}

void CC3ShadowVolumeMeshNode::setShadowOffsetUnits( GLfloat units )
{
	_decalOffsetUnits = units;
	super::setShadowOffsetUnits( units );
}

GLfloat CC3ShadowVolumeMeshNode::getShadowVolumeVertexOffsetFactor()
{
	return _shadowVolumeVertexOffsetFactor ? _shadowVolumeVertexOffsetFactor : super::getShadowVolumeVertexOffsetFactor();
}

void CC3ShadowVolumeMeshNode::setShadowVolumeVertexOffsetFactor( GLfloat factor )
{
	_shadowVolumeVertexOffsetFactor = factor;
	super::setShadowVolumeVertexOffsetFactor( factor );
}

GLfloat CC3ShadowVolumeMeshNode::getShadowExpansionLimitFactor()
{
	return _shadowExpansionLimitFactor; 
}

void CC3ShadowVolumeMeshNode::setShadowExpansionLimitFactor( GLfloat factor )
{
	_shadowExpansionLimitFactor = factor;
	super::setShadowExpansionLimitFactor( factor );
}

bool CC3ShadowVolumeMeshNode::shouldAddShadowVolumeEndCapsOnlyWhenNeeded()
{
	return _shouldAddEndCapsOnlyWhenNeeded; 
}

void CC3ShadowVolumeMeshNode::setShouldAddShadowVolumeEndCapsOnlyWhenNeeded( bool onlyWhenNeeded )
{
	_shouldAddEndCapsOnlyWhenNeeded = onlyWhenNeeded;
	super::setShouldAddShadowVolumeEndCapsOnlyWhenNeeded( onlyWhenNeeded );
}

bool CC3ShadowVolumeMeshNode::hasShadowVolumesForLight( CC3Light* aLight )
{
	return true; 
}

bool CC3ShadowVolumeMeshNode::hasShadowVolumes()
{
	return true; 
}

void CC3ShadowVolumeMeshNode::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	_light = NULL;
	_isShadowDirty = true;
	_shouldDrawTerminator = false;
	_shouldShadowFrontFaces = true;
	_shouldShadowBackFaces = false;
	_shouldAddEndCapsOnlyWhenNeeded = false;
	_useDepthFailAlgorithm = false;
	setShouldUseLighting( false );
	setShouldDisableDepthMask( true );
	_shadowLagFactor = 1;
	_shadowLagCount = 1;
	setShadowOffsetFactor( 0 );
	setShadowOffsetUnits( -1 );
	_shadowVolumeVertexOffsetFactor = 0;
	_shadowExpansionLimitFactor = 100;
	setVisible( defaultVisible() );		// Use setter
	setEmissionColor( kCCC4FYellow );				// For terminator lines
}

void CC3ShadowVolumeMeshNode::populateFrom( CC3ShadowVolumeMeshNode* another )
{
	super::populateFrom( another );
	
	setLight( another->getLight() );						// weak reference
	_isShadowDirty = another->isShadowDirty();
	_shouldShadowFrontFaces = another->shouldShadowFrontFaces();
	_shouldShadowBackFaces = another->shouldShadowBackFaces();
	_shouldDrawTerminator = another->shouldDrawTerminator();
	_shouldAddEndCapsOnlyWhenNeeded = another->shouldAddShadowVolumeEndCapsOnlyWhenNeeded();
	_useDepthFailAlgorithm = another->useDepthFailAlgorithm();
	_shadowLagFactor = another->getShadowLagFactor();
	_shadowLagCount = another->getShadowLagCount();
	_shadowVolumeVertexOffsetFactor = another->getShadowVolumeVertexOffsetFactor();
	_shadowExpansionLimitFactor = another->getShadowExpansionLimitFactor();
}

CCObject* CC3ShadowVolumeMeshNode::copyWithZone( CCZone* zone )
{
	CC3ShadowVolumeMeshNode* pVal = new CC3ShadowVolumeMeshNode;
	pVal->init();
	pVal->populateFrom( this );
	pVal->addCopiesOfChildrenFrom( this );

	return pVal;
}

/**
 * Overridden to return nil so that the shadow volume
 * will always be drawn when made visible during development.
 */
CC3NodeBoundingVolume* CC3ShadowVolumeMeshNode::defaultBoundingVolume()
{
	return NULL; 
}

/** Returns this node's parent, cast as a mesh node. */
CC3MeshNode* CC3ShadowVolumeMeshNode::getShadowCaster()
{
	return (CC3MeshNode*)m_pParent; 
}

/** A shadow volume only uses a material when it is to be visible during development. */
void CC3ShadowVolumeMeshNode::checkShadowMaterial()
{
	if ( !_shouldDrawTerminator && isVisible() ) 
	{
		setColor( CCColorRefFromCCC4F(ccc4f(kCC3OneThird, kCC3OneThird, kCC3OneThird, 1.0)) );	// Will lazily init material if needed
		setOpacity( (CCOpacity)(kCCOpacityFull * kCC3OneThird) );
	} else
		setMaterial( NULL );
}

void CC3ShadowVolumeMeshNode::createShadowMesh()
{
	GLuint vertexCount = getShadowCaster()->getVertexCount();
	
	// Create vertexLocation array.
	CC3VertexLocations* locArray = CC3VertexLocations::vertexArray();
	locArray->setDrawingMode( GL_TRIANGLES );
	locArray->setElementSize( 4 );						// We're using homogeneous coordinates!
	locArray->setAllocatedVertexCapacity( vertexCount );
	locArray->setVertexCount( 0 );						// Will be populated dynamically
	locArray->setShouldReleaseRedundantContent( false );	// Shadow vertex data is dynamic
	
	CC3Mesh* aMesh = CC3Mesh::mesh();
	aMesh->setVertexLocations( locArray );
	setMesh( aMesh );
}

/**
 * Returns a 4D directional vector which can be added to each vertex when creating
 * the shadow volume vertices from the corresponding shadow caster vertices.
 *
 * The returned vector is in the local coordinate system of the shadow caster.
 *
 * The returned directional vector is a small offset vector in the direction away
 * from the light. A unit vector in that direction is scaled by both the distance
 * from the center of the shadow casting node to the camera and the
 * shadowVolumeVertexOffsetFactor property. Hence, if the shadow caster is farther
 * away from the camera, the returned value will be larger, to reduce the chance
 * of Z-fighting between the faces of the shadow volume and the shadow caster.
 */
CC3Vector4 CC3ShadowVolumeMeshNode::getShadowVolumeVertexOffsetForLightAt( const CC3Vector4& localLightPos )
{
	CC3Vector scLoc = getShadowCaster()->getLocalContentCenterOfGeometry();
	CC3Vector lgtLoc = localLightPos.cc3Vector();
	CC3Vector camLoc = getShadowCaster()->getGlobalTransformMatrixInverted()->transformLocation( getActiveCamera()->getGlobalLocation() );	

	// Get a unit offset vector in the direction away from the light
	CC3Vector offsetDir = (_light->isDirectionalOnly()
												? lgtLoc.negate() 
												: scLoc.difference( lgtLoc )).normalize();

	// Get the distance from the shadow caster CoG and the camera, and scale the
	// unit offset vector by that distance and the shadowVolumeVertexOffsetFactor
	GLfloat camDist = scLoc.distance( camLoc );
	CC3Vector offset = offsetDir.scaleUniform( camDist * _shadowVolumeVertexOffsetFactor );
	CC3_TRACE("CC3ShadowVolumeMeshNode nudging vertices by %s", offset.stringfy().c_str());

	// Create and return a 4D directional vector from the offset
	return CC3Vector4().fromDirection(offset);
}

/**
 * Populates the shadow volume mesh by iterating through all the faces in the mesh of
 * the shadow casting node, looking for all pairs of neighbouring faces where one face
 * is in illuminated (facing towards the light) and the other is dark (facing away from
 * the light). The set of edges between these pairs forms the terminator of the mesh,
 * where the mesh on one side of the terminator is illuminated and the other is dark.
 *
 * The shadow volume is then constructed by extruding each edge line segment in the
 * terminator out to infinity in the direction away from the light source, forming a
 * tube of infinite length.
 *
 * Uses the 4D homogeneous location of the light in the global coordinate system.
 * When using the light location this method transforms this location to the local
 * coordinates system of the shadow caster.
 */
void CC3ShadowVolumeMeshNode::populateShadowMesh()
{
	CC3MeshNode* scNode = getShadowCaster();
	GLuint faceCnt = scNode->getFaceCount();
	GLuint shdwVtxIdx = 0;
	bool wasMeshExpanded = false;
	bool doesRequireCapping = _useDepthFailAlgorithm || !_shouldAddEndCapsOnlyWhenNeeded;
	
	// Transform the 4D position of the light into the local coordinates of the shadow caster.
	CC3Vector4 lightPosition = _light->getGlobalHomogeneousPosition();
	CC3Vector4 localLightPosition = scNode->getGlobalTransformMatrixInverted()->transformHomogeneousVector( lightPosition );
	
	// Determine whether we want to nudge the shadow volume vertices away from the shadow caster
	bool isNudgingVertices = (_shadowVolumeVertexOffsetFactor != 0.0f);
	CC3Vector4 svVtxNudge = isNudgingVertices
								? getShadowVolumeVertexOffsetForLightAt(localLightPosition)
								: CC3Vector4::kCC3Vector4Zero;
	
	//	if (doesRequireCapping) LogDebug(@"Populating %@ with end caps", self);
	
	//	if ( [scNode.name isEqualToString: @"GeoSphere01"] ) {
	//		LogDebug(@"Populating %@ with %i faces for light at %@ and %@ end caps",
	//					  self, faceCnt, NSStringFromCC3Vector4(lightPosition),
	//					  (doesRequireCapping ? @"including" : @"excluding"));
	//	}
	
	/*LogTrace(@"Populating %@ with %i faces for light at %@ and %@ end caps",
				  self, faceCnt, NSStringFromCC3Vector4(lightPosition),
				  (doesRequireCapping ? @"including" : @"excluding"));
	
	LogTrace(@"%@ global light location: %@ shadow local light: %@ %@ inverted: %@",
				  self, NSStringFromCC3Vector4(lightPosition),
				  NSStringFromCC3Vector4(localLightPosition),
				  scNode.globalTransformMatrix,
				  scNode.globalTransformMatrixInverted);*/
	
	// Iterate through all the faces in the mesh of the shadow caster.
	for (GLuint faceIdx = 0; faceIdx < faceCnt; faceIdx++) 
	{	
		// Retrieve the current face, convert it to 4D homogeneous locations
		CC3Face face = scNode->getDeformedFaceAt( faceIdx );
		CC3Vector4 vertices4d[3];
		vertices4d[0].fromLocation(face.vertices[0]);
		vertices4d[1].fromLocation(face.vertices[1]);
		vertices4d[2].fromLocation(face.vertices[2]);
		
		// If needed, nudge the shadow volume face away from the
		// shadow caster face in the direction away from the light
		if (isNudgingVertices) {
			vertices4d[0] = vertices4d[0].add( svVtxNudge );
			vertices4d[1] = vertices4d[1].add( svVtxNudge );
			vertices4d[2] = vertices4d[2].add( svVtxNudge );
		}
		
		// Determine whether the face is illuminated.
		bool isFaceLit = CC3Vector4IsInFrontOfPlane(localLightPosition, scNode->getDeformedFacePlaneAt(faceIdx));
		
		/*LogTrace(@"Face %i of %@ is %@. Indices: %@, Vertices: %@, plane: %@, neighbours: %@",
					  faceIdx, scNode, (isFaceLit ? @"illuminated" : @"dark"),
					  NSStringFromCC3FaceIndices([scNode faceIndicesAt: faceIdx]),
					  NSStringFromCC3Face([scNode deformedFaceAt: faceIdx]),
					  NSStringFromCC3Plane([scNode deformedFacePlaneAt: faceIdx]),
					  NSStringFromCC3FaceNeighbours([scNode faceNeighboursAt: faceIdx]));*/
		
		// If we're drawing end-caps, and this face is part of an end-cap, draw it.
		// It's part of an end-cap if it's a dark face and shadowing is based on front
		// faces (typical), or it's a lit face and shadowing is (also) based on back faces
		// (as with some open meshes).
		if (doesRequireCapping &&
			(isFaceLit ? _shouldShadowBackFaces : _shouldShadowFrontFaces) &&
			!_shouldDrawTerminator) {
			/*LogTrace(@"%@ adding end cap for face %i", self, faceIdx);*/
			wasMeshExpanded |= addShadowVolumeCapFor( isFaceLit,  vertices4d, localLightPosition, &shdwVtxIdx );
		}
		
		// Now check the neighbouring face on the other side of each edge of this face.
		CC3FaceNeighbours neighbours = scNode->getFaceNeighboursAt( faceIdx );
		for (int edgeIdx = 0; edgeIdx < 3; edgeIdx++) 
		{	
			// Get the index of the face on the other side of this edge
			GLuint neighbourFaceIdx = neighbours.edges[edgeIdx];
			
			// Check if this edge is part of the terminator. It is if either:
			//   - There is no neighbouring face on this edge, and either the face is lit
			//     and front faces are being shadowed, or the face is dark and back faces
			//     are being shadowed.
			//   - The neighbour has the opposite illumination than the current face
			//     (ie- lit/dark or dark/lit) AND we haven't encountered this edge before
			//     during this iteration, (ie- don't double count). The double-count test
			//     is accomplished by only accepting the neighbouring face if it has a
			//     larger index than the current face.
			bool isTerminatorEdge = false;
			if (neighbourFaceIdx == kCC3FaceNoNeighbour) {
				isTerminatorEdge = isFaceLit ? _shouldShadowFrontFaces : _shouldShadowBackFaces;
			} else if (neighbourFaceIdx > faceIdx) {		// Don't double count edges
				bool isNeighbourFaceLit = CC3Vector4IsInFrontOfPlane(localLightPosition, scNode->getDeformedFacePlaneAt(neighbourFaceIdx));
				isTerminatorEdge = (isNeighbourFaceLit != isFaceLit);
			}
			
			if (isTerminatorEdge) {
				
				// We've found a terminator edge!
				/*LogTrace(@"\tNeighbouring face %u is %@. We have a terminator edge.",
				neighbourFaceIdx, ((neighbourFaceIdx == kCC3FaceNoNeighbour)
				? @"missing" 
				: (isFaceLit ? @"dark" : @"illuminated")));*/
				
				// Get the end points of the terminator edge that we will be extruding.
				// To have the normals of the shadow volume mesh point outwards, we want the
				// winding of the extruded face to be the same as the dark face. So, choose
				// the start and end of the edge based on which face of this pair is illuminated.
				CC3Vector4 edgeStartLoc, edgeEndLoc;
				if (isFaceLit) {
					edgeStartLoc = vertices4d[edgeIdx];
					edgeEndLoc = vertices4d[(edgeIdx < 2) ? (edgeIdx + 1) : 0];
				} else {
					edgeStartLoc = vertices4d[(edgeIdx < 2) ? (edgeIdx + 1) : 0];
					edgeEndLoc = vertices4d[edgeIdx];
				}
				
				if (shouldDrawTerminator() && isVisible()) {
					// Draw the terminator line instead of a shadow
					wasMeshExpanded |= addTerminatorLineFrom( edgeStartLoc, edgeEndLoc, &shdwVtxIdx );
				} else if ( localLightPosition.isDirectional() ) {
					// Draw the shadow from a directional light
					wasMeshExpanded |= addShadowVolumeSideFrom( edgeStartLoc, edgeEndLoc, localLightPosition, &shdwVtxIdx );
				} else {
					// Draw the shadow from a locational light, possibly closing off the far end
					wasMeshExpanded |= addShadowVolumeSideFrom( edgeStartLoc, edgeEndLoc, doesRequireCapping, localLightPosition, &shdwVtxIdx );
				}
			} else {
				/*LogTrace(@"\tNeighbouring face %u is %@. Not a terminator edge.",
							  neighbourFaceIdx, (isFaceLit ? @"illuminated" : @"dark"));*/
			}
		}
	}
	
	// Update the vertex count of the shadow volume mesh, based on how many sides we've added.
	_mesh->setVertexCount( shdwVtxIdx );
	//LogTrace(@"%@ setting vertex count to %u", self, shdwVtxIdx);
	
	// If the mesh is using GL VBO's, update them. If the mesh was expanded,
	// recreate the VBO's, otherwise update them.
	if (_mesh->isUsingGLBuffers()) 
	{
		if (wasMeshExpanded)
		{
			_mesh->deleteGLBuffers();
			_mesh->createGLBuffers();
		} else {
			_mesh->updateVertexLocationsGLBuffer();
		}
	}
	// LogTrace(@"Finshed populating %@", self);
}

/**
 * Adds a side to the shadow volume, by extruding the specified terminator edge of
 * the specified shadow caster mesh to infinity. The light source is directional
 * at the specified position. The shadow volume vertices are added starting at the
 * specified vertex index.
 * 
 * For a directional light, the shadow volume sides are parallel and can therefore
 * be described as meeting at a single point at infinity. We therefore only need to
 * add a single triangle, whose far point is in the opposite direction of the light.
 */
bool CC3ShadowVolumeMeshNode::addShadowVolumeSideFrom( const CC3Vector4& edgeStartLoc, const CC3Vector4& edgeEndLoc, const CC3Vector4& lightPosition, GLuint* shdwVtxIdx )
{	
	// Get the location of the single point at infinity from the light direction.
	CC3Vector4 farLoc = lightPosition.homogeneousNegate();
	
	// Ensure the mesh has enough capacity for another triangle.
	bool wasMeshExpanded = getMesh()->ensureVertexCapacity( *shdwVtxIdx + 3 );
	
	// Add a single triangle from the edge to a single point at infinity,
	// with the same winding as the dark face.
	_mesh->setVertexHomogeneousLocation( edgeStartLoc, (*shdwVtxIdx)++ );
	_mesh->setVertexHomogeneousLocation( farLoc, (*shdwVtxIdx)++ );
	_mesh->setVertexHomogeneousLocation( edgeEndLoc, (*shdwVtxIdx)++ );
	
	//LogTrace(@"%@ drawing shadow volume side face for directional light", self);
	
	return wasMeshExpanded;
}

/**
 * Adds a side to the shadow volume, by extruding the specified terminator edge of
 * the specified shadow caster mesh to infinity. The light source is positional at
 * the specified position. The shadow volume vertices are added starting at the
 * specified vertex index.
 * 
 * For a locational light, the shadow volume sides are not parallel and expand as
 * they extend away from the shadow casting object. If the shadow volume does not
 * need to be capped off at the far end, the shadow expands to infinity.
 *
 * However, if the shadow volume needs to be capped off at the far end, the shadow
 * is allowed to expand behind the shadow-caster to a distance equivalent to the
 * distance from the light to the shadow-caster, multiplied by the value of the
 * shadowExpansionLimitFactor property. At that point, the shadow volume will
 * extend out to infinity at that same size.
 * 
 * For the shadow volume segment that expands in size, each side is a trapezoid
 * formed by projecting a vector from the light, through each terminator edge
 * vertex, out to either infinity of the distance determined by the value of the
 * shadowExpansionLimitFactor property. Then, from that distance to infinity,
 * the shadow volume side behaves as if it originated from a directional light,
 * and is constructed from a single triangle, extending out to infinity in the
 * opposite direction of the light.
 */
bool CC3ShadowVolumeMeshNode::addShadowVolumeSideFrom( const CC3Vector4& edgeStartLoc, const CC3Vector4& edgeEndLoc, bool  doesRequireCapping, const CC3Vector4& lightPosition, GLuint* shdwVtxIdx )
{
	CC3Vector4 farStartLoc, farEndLoc;

	if (doesRequireCapping) {
		// We need to cap this shadow volume at infinity, so allow the shadow volume
		// to expand only for a distance equivalent to the distance from the light to
		// the vertex, mulitiplied by the shadowExpansionLimitFactor property.
		farStartLoc = expand( edgeStartLoc, lightPosition );
		farEndLoc = expand( edgeEndLoc, lightPosition );
	} else {
		// We don't need to cap this shadow volume, so allow the shadow volume to expand
		// to infinity in a direction away from the light, through the edge points.
		// The W component of the result of each subtraction will be zero, indicating
		// a point at infinity.
		farStartLoc = edgeStartLoc.difference( lightPosition );
		farEndLoc = edgeEndLoc.difference( lightPosition );
	}
	
	// Ensure the mesh has enough capacity for another two triangles.
	bool wasMeshExpanded = getMesh()->ensureVertexCapacity( *shdwVtxIdx + 6 );
	
	// The shadow volume faces have the same winding as the dark face.
	// First triangular face:
	_mesh->setVertexHomogeneousLocation( edgeStartLoc, (*shdwVtxIdx)++ );
	_mesh->setVertexHomogeneousLocation( farStartLoc, (*shdwVtxIdx)++ );
	_mesh->setVertexHomogeneousLocation( farEndLoc, (*shdwVtxIdx)++ );
	
	// Second triangular face:
	_mesh->setVertexHomogeneousLocation( edgeStartLoc, (*shdwVtxIdx)++ );
	_mesh->setVertexHomogeneousLocation( farEndLoc, (*shdwVtxIdx)++ );
	_mesh->setVertexHomogeneousLocation( edgeEndLoc, (*shdwVtxIdx)++ );

	if (doesRequireCapping) {
		// To cap, extend from the limited expansion points out to infinity in
		// a direction away from the light, as if the light was directional.
		// These segments will be parallel to each other, and the shadow will
		// expand no further.
		wasMeshExpanded |= addShadowVolumeSideFrom( farStartLoc, farEndLoc, lightPosition, shdwVtxIdx );
	}
	
	//LogTrace(@"%@ drawing shadow volume side face for positional light", self);
	
	return wasMeshExpanded;
}

/** 
 * Expands the location of an terminator edge vertex in the direction away from the locational
 * light at the specified location. The vertex is moved away from the light along the vector
 * from the light to the vertex, a distance equal to the distance between the light and the
 * vertex, multiplied by the value of the shadowExpansionLimitFactor property.
 */
CC3Vector4 CC3ShadowVolumeMeshNode::expand( const CC3Vector4& edgeLoc, const CC3Vector4& lightLoc )
{
	CC3Vector4 extDir = edgeLoc.difference( lightLoc );
	CC3Vector4 extrusion = extDir.scaleUniform( _shadowExpansionLimitFactor );
	return edgeLoc.add( extrusion );
}

/**
 * Adds a face to the cap at the near end of the shadow volume.
 *
 * The winding order of the end-cap faces is determined from the winding order of
 * the model face, taking into consideration whether the face is lit or not.
 */
bool CC3ShadowVolumeMeshNode::addShadowVolumeCapFor( bool isFaceLit, CC3Vector4* vertices, const CC3Vector4& lightPosition, GLuint* shdwVtxIdx )
{	
	// Ensure the mesh has enough capacity for another triangle.
	bool wasMeshExpanded = getMesh()->ensureVertexCapacity( *shdwVtxIdx + 3 );
	
	// Add a single triangle face to the cap at the near end, built from the vertices
	// of the shadow caster face at the specified index. If the face is lit, use the
	// same winding order. If the face is dark, use the opposite winding.
	if (isFaceLit) {
		_mesh->setVertexHomogeneousLocation( vertices[0], (*shdwVtxIdx)++ );
		_mesh->setVertexHomogeneousLocation( vertices[1], (*shdwVtxIdx)++ );
		_mesh->setVertexHomogeneousLocation( vertices[2], (*shdwVtxIdx)++ );
	} else {															  
		_mesh->setVertexHomogeneousLocation( vertices[0], (*shdwVtxIdx)++ );
		_mesh->setVertexHomogeneousLocation( vertices[2], (*shdwVtxIdx)++ );
		_mesh->setVertexHomogeneousLocation( vertices[1], (*shdwVtxIdx)++ );
	}

	/*LogTrace(@"%@ drawing shadow volume near %@end cap face (%@, %@, %@)",
				  self, (CC3Vector4IsLocational(lightPosition) ? @"and far " : @""),
				  NSStringFromCC3Vector4(vertices[0]),
				  NSStringFromCC3Vector4(vertices[1]),
				  NSStringFromCC3Vector4(vertices[2]));*/
	
	return wasMeshExpanded;
}

/**
 * When drawing the terminator line of the mesh, just add the two line
 * endpoints, and don't make use of infinitely extruded endpoints.
 */
bool CC3ShadowVolumeMeshNode::addTerminatorLineFrom( const CC3Vector4& edgeStartLoc, const CC3Vector4& edgeEndLoc, GLuint* shdwVtxIdx )
{	
	// Ensure the mesh has enough capacity for another line
	bool wasMeshExpanded = getMesh()->ensureVertexCapacity( *shdwVtxIdx + 2 );
	
	// Add just the two end points of the terminator edge
	_mesh->setVertexHomogeneousLocation( edgeStartLoc, (*shdwVtxIdx)++ );
	_mesh->setVertexHomogeneousLocation( edgeEndLoc, (*shdwVtxIdx)++ );
	
	//LogTrace(@"%@ drawing terminator line", self);
	
	return wasMeshExpanded;
}


/** Overridden to decrement the shadow lag count on each update. */
void CC3ShadowVolumeMeshNode::processUpdateBeforeTransform( CC3NodeUpdatingVisitor* visitor )
{
	_shadowLagCount = MAX(_shadowLagCount - 1, 0);
	super::processUpdateBeforeTransform( visitor );
}

/** Returns whether the shadow cast by this shadow volume will be visible. */
bool CC3ShadowVolumeMeshNode::isShadowVisible()
{
	CC3MeshNode* scNode = getShadowCaster();
	return (_light->isVisible() || _light->shouldCastShadowsWhenInvisible()) &&
			(scNode->isVisible() || scNode->shouldCastShadowsWhenInvisible() || isVisible()) &&
			scNode->doesIntersectBoundingVolume( _light->getShadowCastingVolume() );
}

/**
 * Returns whether this shadow volume is ready to be updated.
 * It is if the lag count has been decremented to zero.
 */
bool CC3ShadowVolumeMeshNode::isReadyToUpdate()
{
	return (_shadowLagCount == 0); 
}

/**
 * If the shadow is ready to be updated, check if the shadow is both
 * visible and dirty, and re-populate the shadow mesh if needed.
 *
 * To keep the shadow lag count synchronized across all shadow-casting nodes,
 * the shadow lag count will be reset to the value of the shadow lag factor
 * if the shadow is ready to be updated, even if it is not actually updated
 * due to it being invisible, or not dirty.
 */
void CC3ShadowVolumeMeshNode::updateShadow()
{
	//LogTrace(@"Testing to update %@ with shadow lag count %i", self, _shadowLagCount);
	if (isReadyToUpdate())
	{
		if (isShadowVisible())
		{
			updateStencilAlgorithm();
			if (_isShadowDirty) 
			{
				//LogTrace(@"Updating %@", self);
				populateShadowMesh();
				_isShadowDirty = false;
			}
		}
		_shadowLagCount = _shadowLagFactor;
	}
}

/**
 * Selects whether to use the depth-fail or depth-pass algorithm,
 * based on whether this shadow falls across the camera.
 *
 * The depth-fail algorithm requires end caps on this shadow volume.
 * The depth-pass algorithm does not. Rendering end-caps when not needed
 * creates a performance penalty, so the depth-pass algorithm can be used
 * by setting the shouldAddEndCapsOnlyWhenNeeded property to YES.
 *
 * If the selected stencil algorithm changes, this shadow volume is marked
 * as dirty so that the end caps will be added or removed appropriately.
 */
void CC3ShadowVolumeMeshNode::updateStencilAlgorithm()
{
	bool oldAlgo = _useDepthFailAlgorithm;

	_useDepthFailAlgorithm = !_shouldAddEndCapsOnlyWhenNeeded ||
							getShadowCaster()->doesIntersectBoundingVolume( _light->getCameraShadowVolume() );
	
	// If the stencil algo was changed, mark this shadow as dirty,
	// so that end caps will be added or removed.
	if (_useDepthFailAlgorithm != oldAlgo) 
		_isShadowDirty = true;
}

/** Overridden to remove this shadow node from the light. */
void CC3ShadowVolumeMeshNode::wasRemoved()
{
	_light->removeShadow( this );
	super::wasRemoved();
}

void CC3ShadowVolumeMeshNode::markTransformDirty()
{
	super::markTransformDirty();
	_isShadowDirty = true;
}

bool CC3ShadowVolumeMeshNode::isShadowDirty()
{
	return _isShadowDirty;
}

bool CC3ShadowVolumeMeshNode::shouldDrawTerminator()
{
	return _shouldDrawTerminator;
}

bool CC3ShadowVolumeMeshNode::useDepthFailAlgorithm()
{
	return _useDepthFailAlgorithm;
}

/** A node that affects this shadow (generally the light) was transformed. Mark the shadow as dirty. */
void CC3ShadowVolumeMeshNode::nodeWasTransformed( CC3Node* aNode )
{ 
	super::nodeWasTransformed( aNode );
	_isShadowDirty = true;
}

void CC3ShadowVolumeMeshNode::nodeWasDestroyed( CC3Node* node )
{
	super::nodeWasDestroyed( node ); 
}

/** Overridden to set the line properties in addition to other configuration. */
void CC3ShadowVolumeMeshNode::configureDrawingParameters( CC3NodeDrawingVisitor* visitor )
{
	super::configureDrawingParameters( visitor );
	if (_shouldDrawTerminator) 
		visitor->getGL()->setLineWidth( 1.0f );
}

void CC3ShadowVolumeMeshNode::drawToStencilWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	//LogTrace(@"Drawing %@ using depth %@ algo", self, (_useDepthFailAlgorithm ? @"fail" : @"pass"));
	drawToStencilIncrementing( true, visitor );
	drawToStencilIncrementing( false, visitor );
}

void CC3ShadowVolumeMeshNode::drawToStencilIncrementing( bool isIncrementing, CC3NodeDrawingVisitor* visitor )
{
	GLenum zFailOp, zPassOp;
	bool useFrontFaces;

	// Set the stencil operation based on whether we are incrementing or decrementing the stencil.
	GLenum stencilOp = isIncrementing ? GL_INCR_WRAP : GL_DECR_WRAP;
	
	// Depending on whether we are using the depth-fail, or depth-pass algorithm, perform the
	// increment/decrement stencil operation when the depth test fails or passes, respectively,
	// and simply retain the current stencil value otherwise. Also, determine whether we want
	// to cull either the front or back faces, depending on which stencil algorithm we are
	// using, and whether we are on the incrementing or decrementing pass.
	if (_useDepthFailAlgorithm) {
		zFailOp = stencilOp;				// Increment/decrment the stencil on depth fail...
		zPassOp = GL_KEEP;					// ...otherwise keep the current stencil value.
		useFrontFaces = !isIncrementing;	// Cull front faces when incrementing, back faces when decrementing
	} else {
		zPassOp = stencilOp;				// Increment/decrment the stencil on depth pass...
		zFailOp = GL_KEEP;					// ...otherwise keep the current stencil value.
		useFrontFaces = isIncrementing;		// Cull back faces when incrementing, front faces when decrementing
	}

	// Configure the stencil buffer operations
	visitor->getGL()->setOpOnStencilFail( GL_KEEP, zFailOp, zPassOp );
	
	// Remember current culling configuration for this shadow volume
	bool wasCullingBackFaces = shouldCullBackFaces();
	bool wasCullingFrontFaces = shouldCullFrontFaces();

	// Set culling appropriate for stencil operation
	setShouldCullBackFaces( useFrontFaces );
	setShouldCullFrontFaces( !useFrontFaces );
		
	visitor->visit( this );		// Draw the shadow volume to the stencil buffer
		
	// Restore current culling configuration for this shadow volume
	setShouldCullBackFaces( wasCullingBackFaces );
	setShouldCullFrontFaces( wasCullingFrontFaces );
}

static bool _shadowVolumeDefaultVisible = false;

bool CC3ShadowVolumeMeshNode::defaultVisible()
{
	return _shadowVolumeDefaultVisible; 
}

void CC3ShadowVolumeMeshNode::setDefaultVisible( bool _defaultVisible )
{
	_shadowVolumeDefaultVisible = _defaultVisible; 
}

// Shadows are not copied, because each shadow connects
// one-and-only-one shadow casting node to one-and-only-one light.
bool CC3ShadowVolumeMeshNode::shouldIncludeInDeepCopy()
{
	return false; 
}

void CC3ShadowVolumeMeshNode::addShadowVolumesForLight( CC3Light* aLight )
{

}

bool CC3ShadowVolumeMeshNode::shouldDrawDescriptor()
{
	return true; 
}

void CC3ShadowVolumeMeshNode::setShouldDrawDescriptor( bool shouldDraw )
{

}

bool CC3ShadowVolumeMeshNode::shouldDrawWireframeBox() 
{
	return true; 
}

void CC3ShadowVolumeMeshNode::setShouldDrawWireframeBox( bool shouldDraw )
{

}

bool CC3ShadowVolumeMeshNode::shouldDrawLocalContentWireframeBox()
{
	return true; 
}

void CC3ShadowVolumeMeshNode::setShouldDrawLocalContentWireframeBox( bool shouldDraw )
{

}

bool CC3ShadowVolumeMeshNode::shouldContributeToParentBoundingBox()
{
	return false; 
}

bool CC3ShadowVolumeMeshNode::shouldDrawBoundingVolume()
{
	return false; 
}

void CC3ShadowVolumeMeshNode::setShouldDrawBoundingVolume( bool shouldDraw )
{

}

// Overridden so that not touchable unless specifically set as such
bool CC3ShadowVolumeMeshNode::isTouchable()
{
	return (isVisible() || m_shouldAllowTouchableWhenInvisible) && isTouchEnabled(); 
}


/** The shadow painter is always drawn. */
bool CC3StencilledShadowPainterNode::isShadowVisible()
{
	return true; 
}

void CC3StencilledShadowPainterNode::updateShadow()
{
}

CC3Light* CC3StencilledShadowPainterNode::getLight()
{
	return NULL; 
}

void CC3StencilledShadowPainterNode::setLight( CC3Light* light )
{

}

CC3StencilledShadowPainterNode* CC3StencilledShadowPainterNode::nodeWithName( const std::string& name, const ccColor4F& color )
{
	CC3StencilledShadowPainterNode* pNode = new CC3StencilledShadowPainterNode;
	pNode->initWithName( name );
	pNode->setEmissionColor( color );

	return pNode;
}

void CC3StencilledShadowPainterNode::nodeWasTransformed( CC3Node* aNode )
{

}

void CC3StencilledShadowPainterNode::nodeWasDestroyed( CC3Node* aNode )
{

}

void CC3ShadowDrawingVisitor::init()
{
	super::init();
	_shouldVisitChildren = false;
}

bool CC3ShadowDrawingVisitor::shouldDrawNode( CC3Node* aNode )
{
	return aNode->isShadowVisible(); 
}

CC3ShadowDrawingVisitor* CC3ShadowDrawingVisitor::visitor()
{
	CC3ShadowDrawingVisitor* pVal = new CC3ShadowDrawingVisitor;
	pVal->init();
	pVal->autorelease();

	return pVal;
}

NS_COCOS3D_END
