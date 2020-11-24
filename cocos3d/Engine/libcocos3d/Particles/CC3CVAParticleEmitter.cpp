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

void CC3CommonVertexArrayParticleEmitter::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	clearDirtyVertexRanges();
	m_wasVertexCapacityChanged = false;
}

void CC3CommonVertexArrayParticleEmitter::populateFrom( CC3CommonVertexArrayParticleEmitter* another )
{
	super::populateFrom( another );
	m_dirtyVertexRange = another->m_dirtyVertexRange;
	m_dirtyVertexIndexRange = another->m_dirtyVertexIndexRange;
}

CCObject* CC3CommonVertexArrayParticleEmitter::copyWithZone( CCZone* zone )
{
	CC3CommonVertexArrayParticleEmitter* pVal = new CC3CommonVertexArrayParticleEmitter;
	pVal->init();
	pVal->populateFrom( this );
	pVal->addCopiesOfChildrenFrom( this );

	return pVal;
}

std::string CC3CommonVertexArrayParticleEmitter::fullDescription()
{
	return "CC3CommonVertexArrayParticleEmitter";
	/*return [NSString stringWithFormat: @"%@, using %i of %i vertices and %i of %i vertex indices",
			[super fullDescription],
			self.vertexCount, self.mesh.allocatedVertexCapacity,
			self.vertexIndexCount, self.mesh.allocatedVertexIndexCapacity];*/
}

// Overridden to retain all vertex content in memory and dynamically write to GL buffer.
void CC3CommonVertexArrayParticleEmitter::setVertexContentTypes( CC3VertexContent vtxContentTypes )
{
	super::setVertexContentTypes( vtxContentTypes );
	retainVertexContent();
	getMesh()->getVertexLocations()->setBufferUsage( GL_DYNAMIC_DRAW );
}

void CC3CommonVertexArrayParticleEmitter::setVertexLocation( const CC3Vector& aLocation, GLuint vtxIndex )
{
	super::setVertexLocation( aLocation, vtxIndex );
	addDirtyVertex( vtxIndex );
}

void CC3CommonVertexArrayParticleEmitter::setVertexHomogeneousLocation( const CC3Vector4& aLocation, GLuint vtxIndex )
{
	super::setVertexHomogeneousLocation( aLocation, vtxIndex );
	addDirtyVertex( vtxIndex );
}

void CC3CommonVertexArrayParticleEmitter::setVertexNormal( const CC3Vector& aNormal, GLuint vtxIndex )
{
	super::setVertexNormal( aNormal, vtxIndex );
	addDirtyVertex( vtxIndex );
}

void CC3CommonVertexArrayParticleEmitter::setVertexColor4F( const ccColor4F& aColor, GLuint vtxIndex )
{
	super::setVertexColor4F( aColor, vtxIndex );
	addDirtyVertex( vtxIndex );
}

void CC3CommonVertexArrayParticleEmitter::setVertexColor4B( const ccColor4B& aColor, GLuint vtxIndex )
{
	super::setVertexColor4B( aColor, vtxIndex );
	addDirtyVertex( vtxIndex );
}

void CC3CommonVertexArrayParticleEmitter::setVertexTexCoord2F( const ccTex2F& aTex2F, GLuint texUnit, GLuint vtxIndex )
{
	super::setVertexTexCoord2F( aTex2F, texUnit, vtxIndex );
	addDirtyVertex( vtxIndex );
}

/** Ensures space has been allocated for the specified particle. */
bool CC3CommonVertexArrayParticleEmitter::ensureParticleCapacityFor( CC3Particle* aParticle )
{
	if ( !super::ensureParticleCapacityFor( aParticle ) ) 
		return false;
	
	GLuint currCap, newRqmt, newCap, partVtxCount, meshVtxCount, meshVtxIdxCount;
	CC3Mesh* vaMesh = getMesh();
	meshVtxCount = vaMesh->getVertexCount();

	// Ensure that the vertex content arrays have room, and if not, expand them.
	// Expanding the vertex capacity does not change the value of the vertexCount property.
	currCap = vaMesh->getAllocatedVertexCapacity();
	partVtxCount = ((CC3CommonVertexArrayParticle*)aParticle)->getVertexCount();
	newRqmt = meshVtxCount + partVtxCount;
	if (newRqmt > currCap) 
	{		// Needs expansion...so expand by a large chunk
		if (m_particleCapacityExpansionIncrement == 0) 
			return false;		// Oops...can't expand
		newCap = currCap + (partVtxCount * m_particleCapacityExpansionIncrement);
		vaMesh->setAllocatedVertexCapacity( newCap );
		vaMesh->setVertexCount( meshVtxCount );							// Leave the vertex count unchanged
		if ( vaMesh->getAllocatedVertexCapacity() != newCap ) 
			return false;	// Expansion failed
		m_wasVertexCapacityChanged = true;
		CC3_TRACE( "[ptc]CC3CommonVertexArrayParticleEmitter changed capacity to %d vertices", vaMesh->getAllocatedVertexCapacity() );
	}

	// If the underlying mesh uses vertex indices, ensure it has space for the new particle,
	// and if the underlying mesh does not use vertex indices, but the incoming particle does,
	// synthesize vertex indices for all the existing particles. In either case, expanding the
	// vertex index capacity does not change the value of the vertexIndexCount property.
	if ( vaMesh->hasVertexIndices() )
	{
		// Ensure that the vertex index array has room, and if not, expand it.
		currCap = vaMesh->getAllocatedVertexIndexCapacity();
		partVtxCount = ((CC3CommonVertexArrayParticle*)aParticle)->getVertexIndexCount();
		newRqmt = vaMesh->getVertexIndexCount() + partVtxCount;
		if ( newRqmt > currCap ) 
		{		// Needs expansion...so expand by a large chunk
			if ( m_particleCapacityExpansionIncrement == 0 ) 
				return false;			// Oops...can't expand
			newCap = currCap + (partVtxCount * m_particleCapacityExpansionIncrement);
			meshVtxIdxCount = vaMesh->getVertexIndexCount();
			vaMesh->setAllocatedVertexIndexCapacity( newCap );
			vaMesh->setVertexIndexCount( meshVtxIdxCount );			// Leave the vertex count unchanged
			vaMesh->getVertexIndices()->setBufferUsage( GL_DYNAMIC_DRAW );	// Make sure to use dynamic draw
			vaMesh->retainVertexIndices();						// Make sure the indices stick around to be modified
			if ( vaMesh->getAllocatedVertexIndexCapacity() != newCap ) 
				return false;	// Expansion failed
			m_wasVertexCapacityChanged = true;
			CC3_TRACE("[ptc]CC3CommonVertexArrayParticleEmitter changed capacity to %d vertex indices", vaMesh->getAllocatedVertexIndexCapacity());
		}
	} 
	else if ( ((CC3CommonVertexArrayParticle*)aParticle)->hasVertexIndices() )
	{
		// The underlying mesh does not yet have vertex indices, but the particle requires them.
		// Add a new vertex indices array, with enough capacity for one vertex index per vertex,
		// plus an expansion component.
		if ( m_particleCapacityExpansionIncrement == 0 ) 
			return false;			// Oops...can't expand
		partVtxCount = ((CC3CommonVertexArrayParticle*)aParticle)->getVertexIndexCount();
		newCap = meshVtxCount + (partVtxCount * m_particleCapacityExpansionIncrement);
		meshVtxIdxCount = vaMesh->getVertexIndexCount();
		vaMesh->setAllocatedVertexIndexCapacity( newCap );
		vaMesh->setVertexIndexCount( meshVtxIdxCount );		// Leave the vertex count unchanged
		vaMesh->getVertexIndices()->setBufferUsage( GL_DYNAMIC_DRAW );	// Make sure to use dynamic draw
		vaMesh->retainVertexIndices();					// Make sure the indices stick around to be modified
		if ( vaMesh->getAllocatedVertexIndexCapacity() != newCap ) 
			return false;	// Expansion failed
		m_wasVertexCapacityChanged = true;
		CC3_TRACE("[ptc]CC3CommonVertexArrayParticleEmitter created new capacity for %d vertex indices", vaMesh->getAllocatedVertexIndexCapacity());

		// Synthesize vertex indices for the existing vertex content
		for ( GLuint vtxIdx = 0; vtxIdx < meshVtxCount; vtxIdx++ )
			vaMesh->setVertexIndex( vtxIdx, vtxIdx );
	}
	
	return true;
}

/**
 * Adds the specified range to the range of dirty vertices.
 * The result is to form a union of the specified range and the current range.
 */
void CC3CommonVertexArrayParticleEmitter::addDirtyVertexRange( const CCRange& aRange )
{
	m_dirtyVertexRange = m_dirtyVertexRange.unionRange( aRange );
}

/**
 * Adds the specified vertex to the range of dirty vertices.
 * The result is to form a union of the specified vertex and the current range.
 */
void CC3CommonVertexArrayParticleEmitter::addDirtyVertex( GLuint vtxIdx )
{
	addDirtyVertexRange( CCRangeMake( vtxIdx, 1 ) ); 
}

/**
 * Adds the specified range to the range of dirty vertex indices.
 * The result is to form a union of the specified range and the current range.
 */
void CC3CommonVertexArrayParticleEmitter::addDirtyVertexIndexRange( const CCRange& aRange )
{
	m_dirtyVertexIndexRange = m_dirtyVertexIndexRange.unionRange( aRange );
}

/**
 * Adds the specified vertex index to the range of dirty vertex indices.
 * The result is to form a union of the specified vertex index and the current range.
 */
void CC3CommonVertexArrayParticleEmitter::addDirtyVertexIndex( GLuint vtxIdx )
{
	addDirtyVertexIndexRange( CCRangeMake( vtxIdx, 1 ) ); 
}

/** Returns whether any vertices are dirty, by being either expanded or changed. */
bool CC3CommonVertexArrayParticleEmitter::verticesAreDirty()
{
	return m_wasVertexCapacityChanged || (m_dirtyVertexRange.length > 0); 
}

/** Returns whether any vertex indices are dirty, by being either expanded or changed. */
bool CC3CommonVertexArrayParticleEmitter::vertexIndicesAreDirty()
{
	return m_wasVertexCapacityChanged || (m_dirtyVertexIndexRange.length > 0); 
}

/** Clears the range of dirty vertices and vertex indices. */
void CC3CommonVertexArrayParticleEmitter::clearDirtyVertexRanges()
{
	m_dirtyVertexIndexRange = m_dirtyVertexRange = CCRangeMake( 0, 0 ); 
}

/**
 * Process the transform if the vertices have been changed at all,
 * to ensure that particle vertices are transformed.
 */
bool CC3CommonVertexArrayParticleEmitter::isTransformDirty()
{
	return verticesAreDirty() || super::isTransformDirty(); 
}

/** Updates the mesh vertex counts and marks the range of vertices that are affected by this particle. */
void CC3CommonVertexArrayParticleEmitter::acceptParticle( CC3Particle* aParticle )
{
	super::acceptParticle( aParticle );

	CCRange vtxRange = ((CC3CommonVertexArrayParticle*)aParticle)->getVertexRange();
	setVertexCount( (GLuint)vtxRange.maxRange() );
	addDirtyVertexRange( vtxRange );

	CCRange vtxIdxRange = ((CC3CommonVertexArrayParticle*)aParticle)->getVertexIndexRange();
	setVertexIndexCount( (GLuint)vtxIdxRange.maxRange() );
	addDirtyVertexIndexRange( vtxIdxRange );
}

/** Updates the mesh after particles have been updated.  */
void CC3CommonVertexArrayParticleEmitter::processUpdateBeforeTransform( CC3NodeUpdatingVisitor* visitor )
{
	super::processUpdateBeforeTransform( visitor );
	updateParticleMeshWithVisitor( visitor );
}

/**
 * Updates the particle mesh by updating the particle count, copying the particle
 * vertex data to the GL buffer, and updating the bounding volume of this node.
 */
void CC3CommonVertexArrayParticleEmitter::updateParticleMeshWithVisitor( CC3NodeUpdatingVisitor* visitor )
{
	if ( verticesAreDirty() )
	{
//		CCLOG_TRACE("[ptc]CC3CommonVertexArrayParticleEmitter updating mesh with %d particles", _particleCount);
		updateParticleMeshGLBuffers();
		markBoundingVolumeDirty();
		clearDirtyVertexRanges();
		m_wasVertexCapacityChanged = false;
	}
}

/**
 * If the mesh is using GL VBO's, update them. If the mesh was expanded,
 * recreate the VBO's, otherwise update them.
 */
void CC3CommonVertexArrayParticleEmitter::updateParticleMeshGLBuffers()
{
	if ( isUsingGLBuffers() ) 
	{
		CC3Mesh* vaMesh = getMesh();
		if ( m_wasVertexCapacityChanged ) 
		{
			vaMesh->deleteGLBuffers();
			vaMesh->createGLBuffers();
			CC3_TRACE("[ptc]CC3CommonVertexArrayParticleEmitter re-created GL buffers because buffer capacity has changed to %d vertices and %d vertex indices.",
					 vaMesh->getAllocatedVertexCapacity(), vaMesh->getAllocatedVertexIndexCapacity());
		} 
		else 
		{
			vaMesh->updateGLBuffersStartingAt( (GLuint)m_dirtyVertexRange.location, (GLuint)m_dirtyVertexRange.length );
			
			if ( vaMesh->hasVertexIndices() && vertexIndicesAreDirty() )
				vaMesh->getVertexIndices()->updateGLBufferStartingAt( (GLuint)m_dirtyVertexIndexRange.location, (GLuint)m_dirtyVertexIndexRange.length );
		
			/*CCLOG_TRACE( "[ptc]CC3CommonVertexArrayParticleEmitter updated vertex content GL buffer (ID %d) "
				"range (%ld, %ld) of %d vertices (out of %d allocated as %s) and index GL buffer (ID %d) range (%ld, %ld) of %d indices "
				"(out of %d allocated as %s) for %d particles",
					 vaMesh->getVertexLocations()->getBufferID(), (unsigned long)_dirtyVertexRange.location, (unsigned long)_dirtyVertexRange.length, 
					 getVertexCount(), vaMesh->getAllocatedVertexCapacity(), stringFromGLEnum(vaMesh->getVertexLocations()->getBufferUsage()).c_str(),
					 vaMesh->getVertexIndices()->getBufferID(), 
					 (unsigned long)_dirtyVertexIndexRange.location, (unsigned long)_dirtyVertexIndexRange.length, 
					 getVertexIndexCount(), vaMesh->getAllocatedVertexIndexCapacity(), stringFromGLEnum(vaMesh->getVertexIndices()->getBufferUsage()).c_str(), 
					 _particleCount );*/
		}
	} 
	else 
	{
		CC3_TRACE( "[ptc]CC3CommonVertexArrayParticleEmitter not updating GL buffers because they are not in use for this mesh." );
	}
}

CC3CommonVertexArrayParticle* CC3CommonVertexArrayParticleEmitter::getCommonVertexArrayParticleAt( GLuint aParticleIndex )
{
	return (CC3CommonVertexArrayParticle*)getParticleAt( aParticleIndex );
}

CC3Particle* CC3CommonVertexArrayParticleEmitter::getParticleWithVertexAt( GLuint vtxIndex )
{
	GLuint pCnt = getParticleCount();
	for (GLuint pIdx = 0; pIdx < pCnt; pIdx++) 
	{
		CC3CommonVertexArrayParticle* cvap = getCommonVertexArrayParticleAt( pIdx );
		if ( cvap->getVertexRange().isInRange(vtxIndex) )
			return cvap;
	}
	return NULL;
}

CC3Particle* CC3CommonVertexArrayParticleEmitter::getParticleWithVertexIndexAt( GLuint index )
{
	GLuint pCnt = getParticleCount();
	for (GLuint pIdx = 0; pIdx < pCnt; pIdx++) 
	{
		CC3CommonVertexArrayParticle* cvap = getCommonVertexArrayParticleAt( pIdx );
		if ( cvap->getVertexIndexRange().isInRange(index) )
			return cvap;
	}
	return NULL;
}

/** Shrinks the mesh vertex count by the vertex count of the particle. */
void CC3CommonVertexArrayParticleEmitter::removeParticle( CC3Particle* aParticle, GLuint anIndex )
{
	super::removeParticle( aParticle, anIndex );		// Decrements particleCount
	GLuint vertexCount = getVertexCount();

	setVertexCount( vertexCount - ((CC3CommonVertexArrayParticle*)aParticle)->getVertexCount() );

	GLuint indexCount = getVertexIndexCount();
	setVertexIndexCount( indexCount - ((CC3CommonVertexArrayParticle*)aParticle)->getVertexIndexCount() );
}	

void CC3CommonVertexArrayParticleEmitter::removeAllParticles()
{
	super::removeAllParticles();

	addDirtyVertexRange( CCRangeMake(0, getVertexCount()) );
	setVertexCount( 0 );		// After setting dirty range

	addDirtyVertexIndexRange( CCRangeMake(0, getVertexIndexCount()) );
	setVertexIndexCount( 0 );		// After setting dirty range
}

NS_COCOS3D_END
