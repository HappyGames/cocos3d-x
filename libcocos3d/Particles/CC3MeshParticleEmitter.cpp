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
#include "CC3MeshParticleEmitter.h"
#include "CC3MeshParticle.h"
#include "../Nodes/CC3Camera.h"
#include "../Platforms/CC3OSExtensions.h"
#include "support/CCPointExtension.h"
#include "../cc3Helper/Logging.h"

NS_COCOS3D_BEGIN

CC3MeshParticleEmitter::CC3MeshParticleEmitter()
{
	_particleTemplateMesh = NULL;
}

CC3MeshParticleEmitter::~CC3MeshParticleEmitter()
{
	CC_SAFE_RELEASE( _particleTemplateMesh );
}

CC3Mesh* CC3MeshParticleEmitter::getParticleTemplateMesh()
{
	return _particleTemplateMesh; 
}

void CC3MeshParticleEmitter::setParticleTemplateMesh( CC3Mesh* aMesh )
{
	if (aMesh == _particleTemplateMesh) 
		return;
	
	CC_SAFE_RELEASE( _particleTemplateMesh );
	_particleTemplateMesh = aMesh;
	CC_SAFE_RETAIN( aMesh );

	// Add vertex content if not already set, and align the drawing mode
	if ( getVertexContentTypes() == kCC3VertexContentNone )
		setVertexContentTypes( aMesh->getVertexContentTypes() );

	setDrawingMode( aMesh->getDrawingMode() );

	CC3_TRACE( "[ptc]Particle template mesh of CC3MeshParticleEmitter set to %s drawing %s with %d vertices and %d vertex indices",
			 aMesh->fullDescription().c_str(), stringFromGLEnum(getDrawingMode()).c_str(),
			 aMesh->getVertexCount(), aMesh->getVertexIndexCount() );
}

CC3MeshNode* CC3MeshParticleEmitter::getParticleTemplate()
{
	return NULL; 
}

void CC3MeshParticleEmitter::setParticleTemplate( CC3MeshNode* aParticleTemplate )
{
	setParticleTemplateMesh( aParticleTemplate->getMesh() );
	setMaterial( (CC3Material*)aParticleTemplate->getMaterial()->copy()->autorelease() );
}

void CC3MeshParticleEmitter::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	_particleTemplateMesh = NULL;
	_isParticleTransformDirty = false;
	_shouldTransformUnseenParticles = true;
}

void CC3MeshParticleEmitter::populateFrom( CC3MeshParticleEmitter* another )
{
	super::populateFrom( another );
	
	setParticleTemplateMesh( another->getParticleTemplateMesh() );
	_isParticleTransformDirty = another->isParticleTransformDirty();
	_shouldTransformUnseenParticles = another->shouldTransformUnseenParticles();
}

CCObject* CC3MeshParticleEmitter::copyWithZone( CCZone* zone )
{
	CC3MeshParticleEmitter* pVal = new CC3MeshParticleEmitter;
	pVal->init();
	pVal->populateFrom( this );
	pVal->addCopiesOfChildrenFrom( this );

	return pVal;
}

void CC3MeshParticleEmitter::copyTemplateContentToParticle( CC3MeshParticle* aParticle )
{	
	// Get the particle template mesh
	CC3Mesh* templateMesh = aParticle->getTemplateMesh();
	
	// Copy vertex content
	GLuint vtxCount = aParticle->getVertexCount();
	GLuint firstVtx = aParticle->getFirstVertexOffset();
	getMesh()->copyVertices( vtxCount, 0, templateMesh, firstVtx );

	// If this mesh does not have vertex indices, we're done
	if ( !getMesh()->hasVertexIndices() ) 
		return;

	// Copy vertex indices, taking into consideration the staring index of the vertex content in this mesh.
	GLuint vtxIdxCount = aParticle->getVertexIndexCount();
	GLuint firstVtxIdx = aParticle->getFirstVertexIndexOffset();
	getMesh()->copyVertexIndices( vtxIdxCount, 0, templateMesh, firstVtxIdx, firstVtx );
	addDirtyVertexIndexRange( CCRangeMake(firstVtxIdx, vtxIdxCount) );
}

CC3Particle* CC3MeshParticleEmitter::emitParticle()
{
	return super::emitParticle(); 
}

bool CC3MeshParticleEmitter::emitParticle( CC3Particle* aParticle )
{
	if ( !((CC3MeshParticle*)aParticle)->getTemplateMesh() ) 
		assignTemplateMeshToParticle( ((CC3MeshParticle*)aParticle) );

	return super::emitParticle( aParticle );
}

CC3Particle* CC3MeshParticleEmitter::acquireParticle()
{
	CC3MeshParticle* aParticle = (CC3MeshParticle*)super::acquireParticle();
	assignTemplateMeshToParticle( aParticle );
	return aParticle;
}

CC3Particle* CC3MeshParticleEmitter::makeParticle()
{
	CC3MeshParticle* aParticle = CC3MeshParticle::particle();
	assignTemplateMeshToParticle( aParticle );
	return aParticle;
}

void CC3MeshParticleEmitter::assignTemplateMeshToParticle( CC3MeshParticle* aParticle )
{
	CCAssert(_particleTemplateMesh, "The particleTemplateMesh property of CC3MeshParticleEmitter must be set before particles can be emitted.");
	aParticle->setTemplateMesh( _particleTemplateMesh );
}

void CC3MeshParticleEmitter::initializeParticle( CC3Particle* aParticle )
{
	// The vertex offsets depend on particleCount, which has not yet been incremented.
	((CC3MeshParticle*)aParticle)->setFirstVertexOffset( getVertexCount() );
	((CC3MeshParticle*)aParticle)->setFirstVertexIndexOffset( getVertexIndexCount() );
	copyTemplateContentToParticle( ((CC3MeshParticle*)aParticle) );
}

/** If the particles need to be transformed, do so before updating the particle mesh. */
void CC3MeshParticleEmitter::updateParticleMeshWithVisitor( CC3NodeUpdatingVisitor* visitor )
{
	if ( shouldTransformParticles( visitor ) ) 
		transformParticles();

	super::updateParticleMeshWithVisitor( visitor );
}

CC3MeshParticle* CC3MeshParticleEmitter::getMeshParticleAt( GLuint aParticleIndex )
{
	return (CC3MeshParticle*)getParticleAt( aParticleIndex );
}

bool CC3MeshParticleEmitter::shouldTransformUnseenParticles()
{
	return _shouldTransformUnseenParticles;
}

void CC3MeshParticleEmitter::setShouldTransformUnseenParticles( bool transformUnseenParticles )
{
	_shouldTransformUnseenParticles = transformUnseenParticles;
}

bool CC3MeshParticleEmitter::isParticleTransformDirty()
{
	return _isParticleTransformDirty;
}

void CC3MeshParticleEmitter::removeParticle( CC3Particle* aParticle, GLuint anIndex )
{
	super::removeParticle( aParticle,  anIndex );		// Decrements particleCount and vertexCount
	
	GLuint partCount = getParticleCount();	// Get the decremented particleCount
	
	// Particle being removed
	CC3MeshParticle* deadParticle = (CC3MeshParticle*)aParticle;
	GLuint deadFirstVtx = deadParticle->getFirstVertexOffset();
	GLuint deadVtxCount = deadParticle->getVertexCount();
	GLuint deadFirstVtxIdx = deadParticle->getFirstVertexIndexOffset();
	GLuint deadVtxIdxCount = deadParticle->getVertexIndexCount();
	
	// Last living particle
	CC3MeshParticle* lastParticle = getMeshParticleAt( partCount );
	GLuint lastFirstVtx = lastParticle->getFirstVertexOffset();
	GLuint lastVtxCount = lastParticle->getVertexCount();
	GLuint lastFirstVtxIdx = lastParticle->getFirstVertexIndexOffset();
	GLuint lastVtxIdxCount = lastParticle->getVertexIndexCount();

	// Remove the template mesh from the particle, even if the particle will be reused.
	// This gives the emitter a chance to use a different template mesh when it reuses the particle.
	// Clear it before removing the particle, because the particle may disappear when removed from
	// this emitter. First, take note of whether the last particle has the same template mesh as the
	// last particle. This knowledge is used below when copying vertex indices.
	bool isSameTemplateMesh = (deadParticle->getTemplateMesh() == lastParticle->getTemplateMesh());
	deadParticle->setTemplateMesh( NULL );
	
	if (anIndex >= partCount) 
	{
		CC3_TRACE("[ptc]Removing particle at %d by doing nothing, since particle count is now %d.", anIndex, partCount);
	} 
	else if (deadVtxCount == lastVtxCount && deadVtxIdxCount == lastVtxIdxCount) 
	{
		// If the two particles have the same number of vertices and vertex indices, we can swap them.
		CC3_TRACE("[ptc]Removing particle at %d by swapping particles of identical size.", anIndex);
		
		// Move the last living particle into the slot that is being vacated
		_particles->exchangeObjectAtIndex( anIndex, partCount );
		
		// Swap the vertex offsets of the two particles
		deadParticle->setFirstVertexOffset( lastFirstVtx );
		deadParticle->setFirstVertexIndexOffset( lastFirstVtxIdx );
		lastParticle->setFirstVertexOffset( deadFirstVtx );
		lastParticle->setFirstVertexIndexOffset( deadFirstVtxIdx );
		
		// Update the underlying mesh vertex content and mark the updated vertex dirty
		getMesh()->copyVertices( deadVtxCount, lastFirstVtx, deadFirstVtx );
		addDirtyVertexRange( deadParticle->getVertexRange() );

		// If the template meshes are the same, we don't need to update the vertex indices.
		if ( !isSameTemplateMesh ) 
		{
			getMesh()->getVertexIndices()->copyVertices( lastVtxIdxCount, lastFirstVtxIdx, deadFirstVtxIdx, (deadFirstVtx - lastFirstVtx) );
			addDirtyVertexIndexRange( deadParticle->getVertexIndexRange() );
		}
		
	} 
	else 
	{
		CC3_TRACE("[ptc]Removing particle at %d by removing particle with %d vertices from collection.", anIndex, deadVtxCount);
		
		// Move the vertices in the mesh to fill the gap created by the removed particle
		GLuint srcVtxStart = (deadFirstVtx + deadVtxCount);	// Start after removed particle
		GLuint srcVtxEnd = (lastFirstVtx + lastVtxCount);		// End after last living particle
		GLuint vtxCount = srcVtxEnd - srcVtxStart;
		GLuint dstVtxStart = deadFirstVtx;
		getMesh()->copyVertices( vtxCount, srcVtxStart, dstVtxStart );
		addDirtyVertexRange( CCRangeMake(dstVtxStart, vtxCount) );
		
		// If the mesh has vertex indices, move them to fill the gap created by the removed particle
		// and adjust their values to fill the gap created in the vertex content.
		GLuint srcVtxIdxStart = (deadFirstVtxIdx + deadVtxIdxCount);	// Start after removed particle
		GLuint srcVtxIdxEnd = (lastFirstVtxIdx + lastVtxIdxCount);	// End after last living particle
		GLuint vtxIdxCount = srcVtxIdxEnd - srcVtxIdxStart;
		GLuint dstVtxIdxStart = deadFirstVtxIdx;
		getMesh()->copyVertexIndices( vtxIdxCount, srcVtxIdxStart, dstVtxIdxStart, -(GLint)deadVtxCount );
		addDirtyVertexIndexRange( CCRangeMake(dstVtxIdxStart, vtxIdxCount) );
		
		// Remove the particle from particles collection,
		// Do this last in case the particle is only being held by this collection.
		_particles->removeObjectAtIndex( anIndex );
		
		// Adjust the firstVertexOffset and firstVertexIndexOffset properties of each remaining
		// particle to fill in the gap created by removing the particle from the mesh arrays.
		// Do this after the dead particle has been removed from the collection.
		for (GLuint partIdx = anIndex; partIdx < partCount; partIdx++) 
		{
			CC3MeshParticle* mp = getMeshParticleAt( partIdx );
			GLuint firstVertexOffset = mp->getFirstVertexOffset();
			GLuint firstVertexIndexOffset = mp->getFirstVertexIndexOffset();
			firstVertexOffset -= deadVtxCount;
			firstVertexIndexOffset -= deadVtxIdxCount;
			mp->setFirstVertexOffset( firstVertexOffset );
			mp->setFirstVertexIndexOffset( firstVertexIndexOffset );
		}
	}
}

/** Overridden so that the transform is considered dirty if any of the particles need to be transformed. */
bool CC3MeshParticleEmitter::isTransformDirty()
{
	return super::isTransformDirty() || isParticleTransformDirty(); 
}

void CC3MeshParticleEmitter::markParticleTransformDirty()
{
	_isParticleTransformDirty = true; 
}

/**
 * Template method that returns whether the particles should be tranformed.
 *
 * Particles are only transformed if they are dirty and either visible or touchable.
 * If the bounding volume is fixed, and the emitter is not in the camera's field of view,
 * then the particles are not transformed.
 * 
 * Subclasses may override this methods to change how this decision is made.
 */
bool CC3MeshParticleEmitter::shouldTransformParticles( CC3NodeUpdatingVisitor* visitor )
{
	if ( !isParticleTransformDirty() ) 
		return false;

	if ( !(isVisible() || isTouchable()) )
		return false;

	if ( (shouldUseFixedBoundingVolume() || !shouldTransformUnseenParticles()) &&
		!(doesIntersectFrustum( visitor->getCamera()->getFrustum())) )
		return false;

	return true;
}

void CC3MeshParticleEmitter::transformParticles()
{
	GLuint partCount = getParticleCount();
	CC3_TRACE("CC3MeshParticleEmitter transforming %d particles", _particleCount);

	for (GLuint partIdx = 0; partIdx < partCount; partIdx++) 
	{
		CC3MeshParticle* mp = (CC3MeshParticle*)_particles->objectAtIndex( partIdx );
		mp->transformVertices();
	}

	_isParticleTransformDirty = false;
}

CC3MeshParticleEmitter* CC3MeshParticleEmitter::nodeWithName( const std::string& aName )
{
	CC3MeshParticleEmitter* pVal = new CC3MeshParticleEmitter;
	pVal->initWithName( aName );
	pVal->autorelease();

	return pVal;
}


NS_COCOS3D_END
