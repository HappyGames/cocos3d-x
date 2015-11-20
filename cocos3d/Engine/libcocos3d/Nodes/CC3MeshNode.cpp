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

CC3MeshNode::CC3MeshNode()
{	
	m_pMaterial = NULL;
	m_pMesh = NULL;
	m_pShaderContext = NULL;
	m_decalOffsetFactor = 0.f;
	m_decalOffsetUnits = 0.f;
	m_hasRigidSkeleton = false;
}

CC3MeshNode::~CC3MeshNode()
{
	CC_SAFE_RELEASE( m_pMaterial );
	CC_SAFE_RELEASE( m_pMesh );
	CC_SAFE_RELEASE( m_pShaderContext );
}

void CC3MeshNode::setName( const std::string& aName )
{
	super::setName( aName );

	if ( m_pMesh )
		m_pMesh->deriveNameFrom( this );
	
	if ( m_pMaterial )
		m_pMaterial->deriveNameFrom( this );		// Don't lazy init material yet
}

CC3Mesh* CC3MeshNode::getMesh()
{
	return m_pMesh;
}

// Sets the name of the mesh if needed and marks the bounding volume as dirty.
void CC3MeshNode::setMesh( CC3Mesh * aMesh )
{
	if (aMesh == m_pMesh) 
		return;
	
	CC_SAFE_RELEASE( m_pMesh );
	CC_SAFE_RETAIN( aMesh );
	m_pMesh = aMesh;
	
	if ( m_pMesh )
		m_pMesh->deriveNameFrom( this );

	alignMaterialAndMesh();
	markBoundingVolumeDirty();
}

/** If a mesh does not yet exist, create it as a CC3Mesh with interleaved vertices. */
CC3Mesh* CC3MeshNode::ensureMesh()
{
	if ( !m_pMesh ) 
		setMesh( makeMesh() );

	return m_pMesh;
}

CC3Mesh* CC3MeshNode::makeMesh()
{
	return CC3Mesh::mesh(); 
}

/** Lazily init the material */
CC3Material* CC3MeshNode::getMaterial()
{
	if ( !m_pMaterial ) 
		setMaterial( makeMaterial() );

	return m_pMaterial;
}

void CC3MeshNode::setMaterial( CC3Material* aMaterial )
{
	if (aMaterial == m_pMaterial) 
		return;

	CC_SAFE_RELEASE( m_pMaterial );
	CC_SAFE_RETAIN( aMaterial );
	m_pMaterial = aMaterial;

	if ( m_pMaterial )
		m_pMaterial->deriveNameFrom( this );
	alignMaterialAndMesh();
	alignTextureUnits();
}


CC3Material* CC3MeshNode::makeMaterial()
{
	return CC3Material::material();
}

/** Align the material properties to those of the mesh. */
void CC3MeshNode::alignMaterialAndMesh()
{
	if ( m_pMesh && !m_pMesh->hasVertexNormals() ) 
	{
		if ( m_pMaterial )
			m_pMaterial->setShouldUseLighting( false );	// Only if material exists
	}
	
	if ( m_pMesh && !m_pMesh->hasVertexTextureCoordinates() ) 
	{
		if ( m_pMaterial )
			m_pMaterial->setTexture( NULL );	// Only if material exists
	}
}

/**
 * Returns a bounding volume that first checks against the spherical boundary, and then checks
 * against a bounding box. The spherical boundary is fast to check, but is not as accurate as
 * the bounding box for many meshes. The bounding box is more accurate, but is more expensive
 * to check than the spherical boundary. The bounding box is only checked if the spherical
 * boundary does not indicate that the mesh is outside the frustum.
 */
CC3NodeBoundingVolume* CC3MeshNode::defaultBoundingVolume()
{
	return CC3NodeSphereThenBoxBoundingVolume::boundingVolume();
}

bool CC3MeshNode::shouldDrawInClipSpace()
{
	return m_shouldDrawInClipSpace; 
}

void CC3MeshNode::setShouldDrawInClipSpace( bool shouldClip )
{
	if (shouldClip != m_shouldDrawInClipSpace) 
	{
		m_shouldDrawInClipSpace = shouldClip;
		
		if (m_shouldDrawInClipSpace) 
		{
			populateAsCenteredRectangleWithSize( CCSizeMake(2.0f, 2.0f) );
			setShouldDisableDepthTest( true );
			setShouldDisableDepthMask( true );
			setShouldUseLighting( false );
			setBoundingVolume( NULL );
		}
	}

	super::setShouldDrawInClipSpace( shouldClip );
}

bool CC3MeshNode::shouldCullBackFaces()
{
	return m_shouldCullBackFaces; 
}

void CC3MeshNode::setShouldCullBackFaces( bool shouldCull )
{
	m_shouldCullBackFaces = shouldCull;
	super::setShouldCullBackFaces( shouldCull );
}

bool CC3MeshNode::shouldCullFrontFaces()
{
	return m_shouldCullFrontFaces; 
}

void CC3MeshNode::setShouldCullFrontFaces( bool shouldCull )
{
	m_shouldCullFrontFaces = shouldCull;
	super::setShouldCullFrontFaces( shouldCull );
}

bool CC3MeshNode::shouldUseClockwiseFrontFaceWinding()
{
	return m_shouldUseClockwiseFrontFaceWinding; 
}

void CC3MeshNode::setShouldUseClockwiseFrontFaceWinding( bool shouldWindCW )
{
	m_shouldUseClockwiseFrontFaceWinding = shouldWindCW;
	super::setShouldUseClockwiseFrontFaceWinding( shouldWindCW );
}

bool CC3MeshNode::shouldUseSmoothShading()
{
	return m_shouldUseSmoothShading; 
}

void CC3MeshNode::setShouldUseSmoothShading( bool shouldSmooth )
{
	m_shouldUseSmoothShading = shouldSmooth;
	super::setShouldUseSmoothShading( shouldSmooth );
}

bool CC3MeshNode::shouldCastShadowsWhenInvisible()
{
	return m_shouldCastShadowsWhenInvisible; 
}

void CC3MeshNode::setShouldCastShadowsWhenInvisible( bool shouldCast )
{
	m_shouldCastShadowsWhenInvisible = shouldCast;
	super::setShouldCastShadowsWhenInvisible( shouldCast );
}

CC3NormalScaling CC3MeshNode::getNormalScalingMethod()
{
	return m_normalScalingMethod; 
}

void CC3MeshNode::setNormalScalingMethod( CC3NormalScaling nsMethod )
{
	m_normalScalingMethod = nsMethod;
	super::setNormalScalingMethod( nsMethod );
}

bool CC3MeshNode::shouldDisableDepthMask()
{
	return m_shouldDisableDepthMask; 
}

void CC3MeshNode::setShouldDisableDepthMask( bool shouldDisable )
{
	m_shouldDisableDepthMask = shouldDisable;
	super::setShouldDisableDepthMask( shouldDisable );
}

bool CC3MeshNode::shouldDisableDepthTest()
{
	return m_shouldDisableDepthTest; 
}

void CC3MeshNode::setShouldDisableDepthTest( bool shouldDisable )
{
	m_shouldDisableDepthTest = shouldDisable;
	super::setShouldDisableDepthTest( shouldDisable );
}

GLenum CC3MeshNode::getDepthFunction()
{
	return (m_depthFunction != GL_NEVER) ? m_depthFunction : super::getDepthFunction(); 
}

void CC3MeshNode::setDepthFunction( GLenum depthFunc )
{
	m_depthFunction = depthFunc;
	super::setDepthFunction( depthFunc );
}

GLfloat CC3MeshNode::getDecalOffsetFactor()
{
	return m_decalOffsetFactor ? m_decalOffsetFactor : super::getDecalOffsetFactor(); 
}

void CC3MeshNode::setDecalOffsetFactor( GLfloat factor )
{
	m_decalOffsetFactor = factor;
	super::setDecalOffsetFactor( factor );
}

GLfloat CC3MeshNode::getDecalOffsetUnits()
{
	return m_decalOffsetUnits ? m_decalOffsetUnits : super::getDecalOffsetUnits(); 
}

void CC3MeshNode::setDecalOffsetUnits( GLfloat units )
{
	m_decalOffsetUnits = units;
	super::setDecalOffsetUnits( units );
}


bool CC3MeshNode::shouldUseLighting()
{
	return m_pMaterial ? m_pMaterial->shouldUseLighting() : false; 
}

void CC3MeshNode::setShouldUseLighting( bool useLighting )
{
	getMaterial()->setShouldUseLighting( useLighting );
	super::setShouldUseLighting( useLighting );	// pass along to any children
}

bool CC3MeshNode::shouldUseLightProbes()
{
	return m_shouldUseLightProbes; 
}

void CC3MeshNode::setShouldUseLightProbes( bool shouldUse )
{
	m_shouldUseLightProbes = shouldUse;
	super::setShouldUseLightProbes( shouldUse );	// pass along to any children
}

ccColor4F CC3MeshNode::getAmbientColor()
{
	return getMaterial()->getAmbientColor(); 
}

void CC3MeshNode::setAmbientColor( const ccColor4F& aColor )
{
	getMaterial()->setAmbientColor( aColor );
	super::setAmbientColor( aColor );	// pass along to any children
}

ccColor4F CC3MeshNode::getDiffuseColor()
{
	return getMaterial()->getDiffuseColor(); 
}

void CC3MeshNode::setDiffuseColor( const ccColor4F& aColor )
{
	getMaterial()->setDiffuseColor( aColor );
	super::setDiffuseColor( aColor );	// pass along to any children
}

ccColor4F CC3MeshNode::getSpecularColor()
{
	return getMaterial()->getSpecularColor(); 
}

void CC3MeshNode::setSpecularColor( const ccColor4F& aColor )
{
	getMaterial()->setSpecularColor( aColor );
	super::setSpecularColor( aColor );	// pass along to any children
}

ccColor4F CC3MeshNode::getEmissionColor()
{
	return getMaterial()->getEmissionColor(); 
}

void CC3MeshNode::setEmissionColor( const ccColor4F& aColor )
{
	getMaterial()->setEmissionColor( aColor );
	super::setEmissionColor( aColor );	// pass along to any children
}

GLfloat CC3MeshNode::getShininess()
{
	return getMaterial()->getShininess(); 
}

void CC3MeshNode::setShininess( GLfloat shininess )
{
	getMaterial()->setShininess( shininess );
	super::setShininess( shininess );	// pass along to any children
}

GLfloat CC3MeshNode::getReflectivity()
{
	return getMaterial()->getReflectivity(); 
}

void CC3MeshNode::setReflectivity( GLfloat reflectivity )
{
	getMaterial()->setReflectivity( reflectivity );
	super::setReflectivity( reflectivity );	// pass along to any children
}

CC3Vector4 CC3MeshNode::getGlobalLightPosition()
{
	return (m_pMaterial && m_pMaterial->hasBumpMap())
				? getGlobalTransformMatrix()->transformHomogeneousVector( CC3Vector4().fromDirection(m_pMaterial->getLightDirection()) )
				: super::getGlobalLightPosition();
}

void CC3MeshNode::setGlobalLightPosition( const CC3Vector4& aPosition )
{
	CC3Vector4 localLtPos = getGlobalTransformMatrixInverted()->transformHomogeneousVector( aPosition );
	getMaterial()->setLightDirection( localLtPos.cc3Vector() );
	super::setGlobalLightPosition( aPosition );
}

CC3ShaderContext* CC3MeshNode::getShaderContext()
{
	if ( !m_pShaderContext ) 
		m_pShaderContext = new CC3ShaderContext;		// retained - don't use setter

	return m_pShaderContext;
}

// Set shader context if not the same, and pass along to descendants
void CC3MeshNode::setShaderContext( CC3ShaderContext* shaderContext )
{
	if (shaderContext == m_pShaderContext) 
		return;

	CC_SAFE_RELEASE( m_pShaderContext );
	m_pShaderContext = shaderContext;
	CC_SAFE_RETAIN( shaderContext );

	super::setShaderContext( shaderContext );	// pass along to any children
}

CC3ShaderProgram* CC3MeshNode::getShaderProgram()
{
	CC3ShaderProgram* sp = getShaderContext()->getProgram();
	if ( !sp )
	{
		sp = CC3ShaderProgram::getShaderMatcher()->getProgramForMeshNode( this );
		getShaderContext()->setProgram( sp );		// Use shaderContext, so doesn't set descendants
		//LogRez(@"Shader program %@ automatically selected for %@", sp, self);
	}
	return sp;
}

void CC3MeshNode::setShaderProgram( CC3ShaderProgram* shaderProgram )
{
	getShaderContext()->setProgram( shaderProgram );
	super::setShaderProgram( shaderProgram );	// pass along to any children
}

CC3ShaderProgram* CC3MeshNode::selectShaderProgram()
{
	return getShaderProgram(); 
}

void CC3MeshNode::selectShaders()
{
	selectShaderProgram();
	super::selectShaders();
}

void CC3MeshNode::removeLocalShaders()
{
	setShaderProgram( NULL );
}

void CC3MeshNode::removeShaders()
{
	removeLocalShaders();
	super::removeShaders();
}

ccColor3B CC3MeshNode::getColor()
{
	return getMaterial()->getColor(); 
}

void CC3MeshNode::setColor( const ccColor3B& color )
{
	getMaterial()->setColor( color );

	if (m_shouldApplyOpacityAndColorToMeshContent) 
	{
		if ( m_pMesh )
			m_pMesh->setColor( color );	// for meshes with colored vertices
	}

	super::setColor( color );	// pass along to any children
}

CCOpacity CC3MeshNode::getOpacity()
{
	return getMaterial()->getOpacity(); 
}

void CC3MeshNode::setOpacity( CCOpacity opacity )
{
	getMaterial()->setOpacity( opacity );
	if (m_shouldApplyOpacityAndColorToMeshContent) 
		m_pMesh->setOpacity( opacity );	// for meshes with colored vertices

	super::setOpacity( opacity );	// pass along to any children
}

bool CC3MeshNode::shouldBlendAtFullOpacity()
{
	return getMaterial()->shouldBlendAtFullOpacity(); 
}

void CC3MeshNode::setShouldBlendAtFullOpacity( bool shouldBlend )
{
	getMaterial()->setShouldBlendAtFullOpacity( shouldBlend );
	
	super::setShouldBlendAtFullOpacity( shouldBlend );	// pass along to any children
}

bool CC3MeshNode::isOpaque()
{
	return getMaterial()->isOpaque(); 
}

void CC3MeshNode::setIsOpaque( bool opaque )
{
	getMaterial()->setIsOpaque( opaque );
	super::setIsOpaque( opaque );	// pass along to any children
}

ccBlendFunc CC3MeshNode::getBlendFunc()
{
	return getMaterial()->getBlendFunc(); 
}

void CC3MeshNode::setBlendFunc( ccBlendFunc aBlendFunc )
{
	getMaterial()->setBlendFunc( aBlendFunc );
	super::setBlendFunc( aBlendFunc );
}

bool CC3MeshNode::shouldDrawLowAlpha()
{
	return getMaterial()->shouldDrawLowAlpha(); 
}

void CC3MeshNode::setShouldDrawLowAlpha( bool shouldDraw )
{
	getMaterial()->setShouldDrawLowAlpha( shouldDraw );
}

bool CC3MeshNode::shouldApplyOpacityAndColorToMeshContent()
{
	return m_shouldApplyOpacityAndColorToMeshContent; 
}

void CC3MeshNode::setShouldApplyOpacityAndColorToMeshContent( bool shouldApply )
{
	m_shouldApplyOpacityAndColorToMeshContent = shouldApply;
	super::setShouldApplyOpacityAndColorToMeshContent( shouldApply );
}

GLfloat CC3MeshNode::getLineWidth()
{
	return m_lineWidth; 
}

void CC3MeshNode::setLineWidth( GLfloat aLineWidth )
{
	m_lineWidth = aLineWidth;
	super::setLineWidth( aLineWidth );
}

bool CC3MeshNode::shouldSmoothLines()
{
	return m_shouldSmoothLines; 
}

void CC3MeshNode::setShouldSmoothLines( bool shouldSmooth )
{
	m_shouldSmoothLines = shouldSmooth;
	super::setShouldSmoothLines( shouldSmooth );
}

GLenum CC3MeshNode::getLineSmoothingHint()
{
	return m_lineSmoothingHint; 
}

void CC3MeshNode::setLineSmoothingHint( GLenum aHint )
{
	m_lineSmoothingHint = aHint;
	super::setLineSmoothingHint( aHint );
}


void CC3MeshNode::setTexture( CC3Texture* aTexture )
{
	getMaterial()->setTexture( aTexture );
	alignTextureUnit( 0 );
	super::setTexture( aTexture );
}

void CC3MeshNode::addTexture( CC3Texture* aTexture )
{
	getMaterial()->addTexture( aTexture );
	GLuint texCount = getTextureCount();
	if (texCount > 0) 
		alignTextureUnit( texCount - 1 );

	super::addTexture( aTexture );
}

void CC3MeshNode::removeAllTextures()
{
	if ( m_pMaterial )
		m_pMaterial->removeAllTextures(); 
}

void CC3MeshNode::setTexture( CC3Texture* aTexture, GLuint texUnit )
{
	getMaterial()->setTexture( aTexture, texUnit );
	alignTextureUnit( texUnit );
}

bool CC3MeshNode::expectsVerticallyFlippedTextures()
{
	return m_pMesh->expectsVerticallyFlippedTextures(); 
}

void CC3MeshNode::setExpectsVerticallyFlippedTextures( bool expectsFlipped )
{
	m_pMesh->setExpectsVerticallyFlippedTextures( expectsFlipped );
	super::setExpectsVerticallyFlippedTextures( expectsFlipped );
}

bool CC3MeshNode::expectsVerticallyFlippedTextureInTextureUnit( GLuint texUnit )
{
	if ( m_pMesh )
		return m_pMesh->expectsVerticallyFlippedTextureInTextureUnit( texUnit );

	return false;
}

void CC3MeshNode::expectsVerticallyFlippedTexture( bool expectsFlipped, GLuint texUnit )
{
	m_pMesh->setExpectsVerticallyFlippedTexture( expectsFlipped, texUnit );
}

void CC3MeshNode::flipVerticallyTextureUnit( GLuint texUnit )
{
	m_pMesh->flipVerticallyTextureUnit( texUnit ); 
}

void CC3MeshNode::flipTexturesVertically()
{
	m_pMesh->flipTexturesVertically();
	super::flipTexturesVertically();
}

void CC3MeshNode::flipHorizontallyTextureUnit( GLuint texUnit )
{
	m_pMesh->flipHorizontallyTextureUnit( texUnit ); 
}

void CC3MeshNode::flipTexturesHorizontally()
{
	m_pMesh->flipTexturesHorizontally();
	super::flipTexturesHorizontally();
}

void CC3MeshNode::repeatTexture( ccTex2F repeatFactor, GLuint texUnit )
{
	m_pMesh->repeatTexture( repeatFactor, texUnit );
}

void CC3MeshNode::repeatTexture( ccTex2F repeatFactor )
{
	m_pMesh->repeatTexture( repeatFactor ); 
}

CCRect CC3MeshNode::getTextureRectangle()
{
	return m_pMesh ? m_pMesh->getTextureRectangle() : kCC3UnitTextureRectangle;
}

void CC3MeshNode::setTextureRectangle( const CCRect& aRect )
{
	m_pMesh->setTextureRectangle( aRect );  
}

CCRect CC3MeshNode::getTextureRectangleForTextureUnit( GLuint texUnit )
{
	return m_pMesh ? m_pMesh->getTextureRectangleForTextureUnit(texUnit) : kCC3UnitTextureRectangle;
}

void CC3MeshNode::setTextureRectangle( const CCRect& aRect, GLuint texUnit )
{
	m_pMesh->setTextureRectangle( aRect, texUnit );
}


bool CC3MeshNode::hasTextureAlpha()
{
	return m_pMaterial ? m_pMaterial->hasTextureAlpha() : false; 
}

bool CC3MeshNode::hasTexturePremultipliedAlpha()
{
	return m_pMaterial ? m_pMaterial->hasTexturePremultipliedAlpha() : false; 
}

bool CC3MeshNode::hasPremultipliedAlpha()
{
	return hasTexturePremultipliedAlpha();
}		// Deprecated

bool CC3MeshNode::shouldApplyOpacityToColor()
{
	return m_pMaterial ? m_pMaterial->shouldApplyOpacityToColor() : false; 
}

void CC3MeshNode::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	m_pMesh = NULL;
	m_pMaterial = NULL;
	m_pShaderContext = NULL;
	m_renderStreamGroupMarker = "";
	m_shouldUseLightProbes = false;
	m_shouldUseSmoothShading = true;
	m_shouldCullBackFaces = true;
	m_shouldCullFrontFaces = false;
	m_shouldUseClockwiseFrontFaceWinding = false;
	m_shouldDisableDepthMask = false;
	m_shouldDisableDepthTest = false;
	m_shouldCastShadowsWhenInvisible = false;
	m_depthFunction = GL_LEQUAL;
	m_normalScalingMethod = kCC3NormalScalingAutomatic;
	m_lineWidth = 1.0f;
	m_shouldSmoothLines = false;
	m_lineSmoothingHint = GL_DONT_CARE;
	m_shouldApplyOpacityAndColorToMeshContent = false;
	m_shouldDrawInClipSpace = false;
	m_hasRigidSkeleton = false;
}

void CC3MeshNode::populateFrom( CC3MeshNode* another )
{
	super::populateFrom( another );
	
	// Don't use setters, to avoid side effects, including to bounding volume and tex coords.
	CC_SAFE_RELEASE( m_pMesh );
	m_pMesh = another->getMesh();
	CC_SAFE_RETAIN( m_pMesh );					// retained - Mesh shared between original and copy

	CC_SAFE_RELEASE( m_pMaterial );
	m_pMaterial = (CC3Material*)another->getMaterial()->copy();					// retained
	
	CC_SAFE_RELEASE( m_pShaderContext );
	m_pShaderContext = (CC3ShaderContext*)another->getShaderContext()->copy();	// retained
	
	m_shouldUseSmoothShading = another->shouldUseSmoothShading();
	m_shouldCullBackFaces = another->shouldCullBackFaces();
	m_shouldCullFrontFaces = another->shouldCullFrontFaces();
	m_shouldUseClockwiseFrontFaceWinding = another->shouldUseClockwiseFrontFaceWinding();
	m_shouldDisableDepthMask = another->shouldDisableDepthMask();
	m_shouldDisableDepthTest = another->shouldDisableDepthTest();
	m_shouldCastShadowsWhenInvisible = another->shouldCastShadowsWhenInvisible();
	m_depthFunction = another->getDepthFunction();
	m_normalScalingMethod = another->getNormalScalingMethod();
	m_lineWidth = another->getLineWidth();
	m_shouldSmoothLines = another->shouldSmoothLines();
	m_lineSmoothingHint = another->getLineSmoothingHint();
	m_shouldApplyOpacityAndColorToMeshContent = another->shouldApplyOpacityAndColorToMeshContent();
}

CCObject* CC3MeshNode::copyWithZone( CCZone* zone )
{
	CC3MeshNode* pMeshNode = new CC3MeshNode;
	pMeshNode->init();
	pMeshNode->populateFrom( this );
	pMeshNode->addCopiesOfChildrenFrom( this );

	return pMeshNode;
}

void CC3MeshNode::createGLBuffers()
{
	//LogTrace(@"%@ creating GL server buffers", self);
	if ( m_pMesh )
		m_pMesh->createGLBuffers();
	
	super::createGLBuffers();
}

void CC3MeshNode::deleteGLBuffers()
{
	if ( m_pMesh )
		m_pMesh->deleteGLBuffers();
	
	super::deleteGLBuffers();
}

bool CC3MeshNode::isUsingGLBuffers()
{
	if ( m_pMesh )
		return m_pMesh->isUsingGLBuffers(); 

	return false;
}

void CC3MeshNode::releaseRedundantContent()
{
	if ( m_pMesh )
		m_pMesh->releaseRedundantContent();

	super::releaseRedundantContent();
}

void CC3MeshNode::retainVertexContent()
{
	if ( m_pMesh )
		m_pMesh->retainVertexContent();

	super::retainVertexContent();
}

void CC3MeshNode::retainVertexLocations()
{
	if ( m_pMesh )
		m_pMesh->retainVertexLocations();
	
	super::retainVertexLocations();
}

void CC3MeshNode::retainVertexNormals()
{
	if ( m_pMesh )
		m_pMesh->retainVertexNormals();
	
	super::retainVertexNormals();
}

void CC3MeshNode::retainVertexTangents()
{
	if ( m_pMesh )
		m_pMesh->retainVertexTangents();
	
	super::retainVertexTangents();
}

void CC3MeshNode::retainVertexBitangents()
{
	if ( m_pMesh )
		m_pMesh->retainVertexBitangents();
	
	super::retainVertexBitangents();
}

void CC3MeshNode::retainVertexColors()
{
	if ( m_pMesh )
		m_pMesh->retainVertexColors();
	
	super::retainVertexColors();
}

void CC3MeshNode::retainVertexBoneIndices()
{
	if ( m_pMesh )
		m_pMesh->retainVertexBoneIndices();
	
	super::retainVertexBoneIndices();
}

void CC3MeshNode::retainVertexBoneWeights()
{
	if ( m_pMesh )
		m_pMesh->retainVertexBoneWeights();
	
	super::retainVertexBoneWeights();
}

void CC3MeshNode::retainVertexTextureCoordinates()
{
	if ( m_pMesh )
		m_pMesh->retainVertexTextureCoordinates();
	
	super::retainVertexTextureCoordinates();
}

void CC3MeshNode::retainVertexIndices()
{
	if ( m_pMesh )
		m_pMesh->retainVertexIndices();
	
	super::retainVertexIndices();
}

void CC3MeshNode::doNotBufferVertexContent()
{
	if ( m_pMesh )
		m_pMesh->doNotBufferVertexContent();
	
	super::doNotBufferVertexContent();
}

void CC3MeshNode::doNotBufferVertexLocations()
{
	if ( m_pMesh )
		m_pMesh->doNotBufferVertexLocations();
	
	super::doNotBufferVertexLocations();
}

void CC3MeshNode::doNotBufferVertexNormals()
{
	if ( m_pMesh )
		m_pMesh->doNotBufferVertexNormals();
	
	super::doNotBufferVertexNormals();
}

void CC3MeshNode::doNotBufferVertexTangents()
{
	if ( m_pMesh )
		m_pMesh->doNotBufferVertexTangents();
	
	super::doNotBufferVertexTangents();
}

void CC3MeshNode::doNotBufferVertexBitangents()
{
	if ( m_pMesh )
		m_pMesh->doNotBufferVertexBitangents();
	
	super::doNotBufferVertexBitangents();
}

void CC3MeshNode::doNotBufferVertexColors()
{
	if ( m_pMesh )
		m_pMesh->doNotBufferVertexColors();
	
	super::doNotBufferVertexColors();
}

void CC3MeshNode::doNotBufferVertexBoneIndices()
{
	if ( m_pMesh )
		m_pMesh->doNotBufferVertexBoneIndices();
	
	super::doNotBufferVertexBoneIndices();
}

void CC3MeshNode::doNotBufferVertexBoneWeights()
{
	if ( m_pMesh )
		m_pMesh->doNotBufferVertexBoneWeights();
	
	super::doNotBufferVertexBoneWeights();
}

void CC3MeshNode::doNotBufferVertexTextureCoordinates()
{
	if ( m_pMesh )
		m_pMesh->doNotBufferVertexTextureCoordinates();
	
	super::doNotBufferVertexTextureCoordinates();
}

void CC3MeshNode::doNotBufferVertexIndices()
{
	if ( m_pMesh )
		m_pMesh->doNotBufferVertexIndices();
	
	super::doNotBufferVertexIndices();
}

bool CC3MeshNode::isMeshNode()
{
	return true; 
}

/**
 * Template method that uses template methods to configure drawing parameters
 * and the material, draws the mesh, and cleans up the drawing state.
 */
void CC3MeshNode::drawWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	//LogTrace(@"Drawing %@", self);
	configureDrawingParameters( visitor );		// Before material is applied.
	applyMaterialWithVisitor( visitor );
	applyShaderProgramWithVisitor( visitor );
	drawMeshWithVisitor( visitor );
	cleanupDrawingParameters( visitor );
}

/**
 * Template method to configure the drawing parameters.
 *
 * Subclasses may override to add additional drawing parameters.
 */
void CC3MeshNode::configureDrawingParameters( CC3NodeDrawingVisitor* visitor )
{
	configureFaceCulling( visitor );
	configureNormalization( visitor );
	configureColoring( visitor );
	configureDepthTesting( visitor );
	configureDecalParameters( visitor );
	configureLineProperties( visitor );
}

/**
 * Template method configures GL face culling based on the shouldCullBackFaces,
 * shouldCullBackFaces, and shouldUseClockwiseFrontFaceWinding properties.
 */
void CC3MeshNode::configureFaceCulling( CC3NodeDrawingVisitor* visitor )
{
	CC3OpenGL* gl = visitor->getGL();

	// Enable culling if either back or front should be culled.
	gl->enableCullFace(m_shouldCullBackFaces || m_shouldCullFrontFaces);

	// Set whether back, front or both should be culled.
	// If neither should be culled, handled by capability so leave it as back culling.
	gl->setCullFace( m_shouldCullBackFaces
						? (m_shouldCullFrontFaces ? GL_FRONT_AND_BACK : GL_BACK)
						: (m_shouldCullFrontFaces ? GL_FRONT : GL_BACK) );

	// If back faces are not being culled, then enable two-sided lighting,
	// so that the lighting of the back faces uses negated normals.
	gl->enableTwoSidedLighting( !m_shouldCullBackFaces );
	
	// Set the front face winding
	gl->setFrontFace( m_shouldUseClockwiseFrontFaceWinding ? GL_CW : GL_CCW );
}

/**
 * Template method configures GL scaling of normals, based on
 * whether the scaling of this node is uniform or not.
 */
void CC3MeshNode::configureNormalization( CC3NodeDrawingVisitor* visitor )
{
	CC3OpenGL* gl = visitor->getGL();
	switch (getEffectiveNormalScalingMethod())
	{
		case kCC3NormalScalingNormalize:		// Enable normalizing & disable re-scaling
			gl->enableNormalize( true );
			gl->enableRescaleNormal( false );
			break;
		case kCC3NormalScalingRescale:			// Enable rescaling & disable normalizing
			gl->enableNormalize( false );
			gl->enableRescaleNormal( true );
			break;
		case kCC3NormalScalingNone:				// Disable both rescaling & normalizing
		default:
			gl->enableNormalize( false );
			gl->enableRescaleNormal( false );
			break;
	}
}

/**
 * Configures the GL state for smooth shading, and to support vertex coloring.
 * This must be invoked every time, because both the material and mesh influence
 * the colorMaterial property, and the mesh will not be re-bound if it does not
 * need to be switched. And this method must be invoked before material colors
 * are set, otherwise material colors will not stick.
 */
void CC3MeshNode::configureColoring( CC3NodeDrawingVisitor* visitor )
{
	CC3OpenGL* gl = visitor->getGL();

	// Set the smoothing model
	gl->setShadeModel( m_shouldUseSmoothShading ? GL_SMOOTH : GL_FLAT );

	// If per-vertex coloring is being used, attach it to the material
	gl->enableColorMaterial( visitor->shouldDecorateNode() && m_pMesh && m_pMesh->hasVertexColors() );
}

/**
 * Template method disables depth testing and/or writing to the depth buffer if the
 * shouldDisableDepthTest and shouldDisableDepthMask property is set to YES, respectively,
 * and set the depth function.
 */
void CC3MeshNode::configureDepthTesting( CC3NodeDrawingVisitor* visitor )
{
	CC3OpenGL* gl = visitor->getGL();
	gl->enableDepthTest( !m_shouldDisableDepthTest );
	gl->setDepthMask( !m_shouldDisableDepthMask );
	gl->setDepthFunc( m_depthFunction );
}

/**
 * Template method that establishes the decal offset parameters to cause the depth
 * of the content being drawn to be offset relative to the depth of the content
 * that has already been drawn.
 */
void CC3MeshNode::configureDecalParameters( CC3NodeDrawingVisitor* visitor )
{
	CC3OpenGL* gl = visitor->getGL();
	bool hasDecalOffset = m_decalOffsetFactor || m_decalOffsetUnits;
	gl->enablePolygonOffset( hasDecalOffset );
	gl->setPolygonOffsetFactor( m_decalOffsetFactor, m_decalOffsetUnits );
}

/** Template method to configure line drawing properties. */
void CC3MeshNode::configureLineProperties( CC3NodeDrawingVisitor* visitor )
{
	CC3OpenGL* gl = visitor->getGL();
	gl->setLineWidth( m_lineWidth );
	gl->enableLineSmoothing( m_shouldSmoothLines );
	gl->setLineSmoothingHint( m_lineSmoothingHint );
}


/** Checks if this node is tracking a global light position (for bump mapping) and update if needed. */
void CC3MeshNode::updateLightPosition()
{
	if (shouldUpdateToTarget() && isTrackingForBumpMapping())
		setGlobalLightPosition( getTarget()->getGlobalHomogeneousPosition() );
}

/** Template method to draw the mesh to the GL engine. */
void CC3MeshNode::drawMeshWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	if ( m_pMesh )
		m_pMesh->drawWithVisitor( visitor ); 
}

CC3VertexContent CC3MeshNode::getVertexContentTypes() 
{ 
	return m_pMesh ? m_pMesh->getVertexContentTypes() : kCC3VertexContentNone; 
}

void CC3MeshNode::setVertexContentTypes( CC3VertexContent vtxContentTypes )
{
	ensureMesh();
	m_pMesh->setVertexContentTypes( vtxContentTypes );
	alignMaterialAndMesh();
}

CC3Vector CC3MeshNode::getCenterOfGeometry()
{
	return m_pChildren ? super::getCenterOfGeometry() : getLocalContentCenterOfGeometry();
}

CC3Vector CC3MeshNode::getLocalContentCenterOfGeometry()
{
	return m_pMesh ? m_pMesh->getCenterOfGeometry() : CC3Vector::kCC3VectorZero;
}

CC3Box CC3MeshNode::getLocalContentBoundingBox()
{
	return m_pMesh
			? CC3BoxAddUniformPadding(m_pMesh->getBoundingBox(), m_fBoundingVolumePadding)
			: CC3Box::kCC3BoxNull;
}

void CC3MeshNode::moveMeshOriginTo( const CC3Vector& aLocation )
{
	m_pMesh->moveMeshOriginTo( aLocation );
	markBoundingVolumeDirty();
}

void CC3MeshNode::moveMeshOriginToCenterOfGeometry()
{
	m_pMesh->moveMeshOriginToCenterOfGeometry();
	markBoundingVolumeDirty();
}

GLuint CC3MeshNode::getVertexCount()
{
	return m_pMesh ? m_pMesh->getVertexCount() : 0;
}

void CC3MeshNode::setVertexCount( GLuint vCount )
{
	m_pMesh->setVertexCount( vCount );  
}

GLuint CC3MeshNode::getVertexIndexCount()
{
	return m_pMesh ? m_pMesh->getVertexIndexCount() : 0; 
}

void CC3MeshNode::setVertexIndexCount( GLuint vCount )
{
	m_pMesh->setVertexIndexCount( vCount );  
}

CC3Vector CC3MeshNode::getVertexLocationAt( GLuint index )
{
	return m_pMesh ? m_pMesh->getVertexLocationAt(index) : CC3Vector::kCC3VectorZero;
}

void CC3MeshNode::setVertexLocation( const CC3Vector& aLocation, GLuint index )
{
	m_pMesh->setVertexLocation( aLocation, index );
	markBoundingVolumeDirty();
}

CC3Vector4 CC3MeshNode::getVertexHomogeneousLocationAt( GLuint index )
{
	return m_pMesh ? m_pMesh->getVertexHomogeneousLocationAt(index) : CC3Vector4::kCC3Vector4ZeroLocation;
}

void CC3MeshNode::setVertexHomogeneousLocation( const CC3Vector4& aLocation, GLuint index )
{
	m_pMesh->setVertexHomogeneousLocation( aLocation, index );
	markBoundingVolumeDirty();
}

CC3Vector CC3MeshNode::getVertexNormalAt( GLuint index )
{
	return m_pMesh ? m_pMesh->getVertexNormalAt(index) : CC3Vector::kCC3VectorUnitZPositive;
}

void CC3MeshNode::setVertexNormal( const CC3Vector& aNormal, GLuint index )
{
	m_pMesh->setVertexNormal( aNormal, index );
}

void CC3MeshNode::flipNormals()
{
	m_pMesh->flipNormals();
	super::flipNormals();
}

CC3Vector CC3MeshNode::getVertexTangentAt( GLuint index )
{
	return m_pMesh ? m_pMesh->getVertexTangentAt(index) : CC3Vector::kCC3VectorUnitXPositive;
}

void CC3MeshNode::setVertexTangent( const CC3Vector& aTangent, GLuint index )
{
	m_pMesh->setVertexTangent( aTangent, index );
}

CC3Vector CC3MeshNode::getVertexBitangentAt( GLuint index )
{
	return m_pMesh ? m_pMesh->getVertexBitangentAt(index) : CC3Vector::kCC3VectorUnitYPositive;
}

void CC3MeshNode::setVertexBitangent( const CC3Vector& aTangent, GLuint index )
{
	m_pMesh->setVertexBitangent( aTangent, index );
}

GLenum CC3MeshNode::getVertexColorType()
{
	return m_pMesh ? m_pMesh->getVertexColorType() : GL_FALSE; 
}

ccColor4F CC3MeshNode::getVertexColor4FAt( GLuint index )
{
	return m_pMesh ? m_pMesh->getVertexColor4FAt(index) : kCCC4FBlackTransparent;
}

void CC3MeshNode::setVertexColor4F( const ccColor4F& aColor, GLuint index )
{
	ccColor4F color = aColor;
	if ( shouldApplyOpacityToColor() ) 
		color = CCC4FBlendAlpha( color );
	m_pMesh->setVertexColor4F( color, index );
}

ccColor4B CC3MeshNode::getVertexColor4BAt( GLuint index )
{
	return m_pMesh ? m_pMesh->getVertexColor4BAt(index) : ccc4(  0, 0, 0, 0  );
}

void CC3MeshNode::setVertexColor4B( const ccColor4B& aColor, GLuint index )
{
	ccColor4B color = aColor;
	if ( shouldApplyOpacityToColor() ) 
		color = CCC4BBlendAlpha(aColor);

	m_pMesh->setVertexColor4B( color, index );
}

GLuint CC3MeshNode::getVertexBoneCount()
{
	return m_pMesh ? m_pMesh->getVertexBoneCount() : 0; 
}

GLfloat CC3MeshNode::getVertexWeightForBoneInfluence( GLuint influenceIndex, GLuint vtxIndex )
{
	return m_pMesh ? m_pMesh->getVertexWeightForBoneInfluence( influenceIndex, vtxIndex ) : 0.0f;
}

GLfloat* CC3MeshNode::getVertexBoneWeightsAt( GLuint vtxIndex )
{
	return m_pMesh ? m_pMesh->getVertexBoneWeightsAt(vtxIndex) : NULL; 
}

void CC3MeshNode::setVertexBoneWeights( GLfloat* weights, GLuint vtxIndex )
{
	m_pMesh->setVertexBoneWeights( weights, vtxIndex );
}

GLuint CC3MeshNode::getVertexBoneIndexForBoneInfluence( GLuint influenceIndex, GLuint vtxIndex )
{
	return m_pMesh ? m_pMesh->getVertexBoneIndexForBoneInfluence( influenceIndex, vtxIndex ) : 0;
}

void CC3MeshNode::setVertexBoneIndex( GLuint boneIndex, GLuint influenceIndex, GLuint vtxIndex )
{
	m_pMesh->setVertexBoneIndex( boneIndex, influenceIndex, vtxIndex );
}

GLvoid* CC3MeshNode::getVertexBoneIndicesAt( GLuint vtxIndex )
{
	return m_pMesh ? m_pMesh->getVertexBoneIndicesAt(vtxIndex) : NULL;
}

void CC3MeshNode::setVertexBoneIndices( GLvoid* boneIndices, GLuint vtxIndex )
{
	m_pMesh->setVertexBoneIndices( boneIndices, vtxIndex );
}

GLenum CC3MeshNode::getVertexBoneIndexType()
{
	return m_pMesh->getVertexBoneIndexType(); 
}

ccTex2F CC3MeshNode::getVertexTexCoord2FForTextureUnit( GLuint texUnit, GLuint index )
{
	return m_pMesh ? m_pMesh->getVertexTexCoord2FForTextureUnit(texUnit, index) : tex2( 0.0f, 0.0f );
}

void CC3MeshNode::setVertexTexCoord2F( ccTex2F aTex2F, GLuint texUnit, GLuint index )
{
	if ( m_pMesh )
		m_pMesh->setVertexTexCoord2F( aTex2F, texUnit, index );
}

ccTex2F CC3MeshNode::getVertexTexCoord2FAt( GLuint index )
{
	return getVertexTexCoord2FForTextureUnit( 0, index );
}

void CC3MeshNode::setVertexTexCoord2F(ccTex2F aTex2F, GLuint index )
{
	setVertexTexCoord2F( aTex2F, 0, index );
}

GLuint CC3MeshNode::getVertexIndexAt( GLuint index )
{
	return m_pMesh ? m_pMesh->getVertexIndexAt(index) : 0; 
}

void CC3MeshNode::setVertexIndex( GLuint vertexIndex, GLuint index )
{
	if ( m_pMesh )
		m_pMesh->setVertexIndex( vertexIndex, index );
}

void CC3MeshNode::updateVertexLocationsGLBuffer()
{
	if ( m_pMesh )
		m_pMesh->updateVertexLocationsGLBuffer(); 
}

void CC3MeshNode::updateVertexNormalsGLBuffer()
{
	if ( m_pMesh )
		m_pMesh->updateVertexNormalsGLBuffer(); 
}

void CC3MeshNode::updateVertexTangentsGLBuffer()
{
	if ( m_pMesh )
		m_pMesh->updateVertexTangentsGLBuffer(); 
}

void CC3MeshNode::updateVertexBitangentsGLBuffer()
{
	if ( m_pMesh )
		m_pMesh->updateVertexBitangentsGLBuffer(); 
}

void CC3MeshNode::updateVertexColorsGLBuffer()
{
	if ( m_pMesh )
		m_pMesh->updateVertexColorsGLBuffer(); 
}

void CC3MeshNode::updateVertexBoneWeightsGLBuffer()
{
	if ( m_pMesh )
		m_pMesh->updateVertexBoneWeightsGLBuffer(); 
}

void CC3MeshNode::updateVertexBoneIndicesGLBuffer()
{
	if ( m_pMesh )
		m_pMesh->updateVertexBoneIndicesGLBuffer(); 
}

void CC3MeshNode::updateVertexTextureCoordinatesGLBufferForTextureUnit( GLuint texUnit )
{
	if ( m_pMesh )
		m_pMesh->updateVertexTextureCoordinatesGLBufferForTextureUnit( texUnit );
}

void CC3MeshNode::updateVertexTextureCoordinatesGLBuffer()
{
	updateVertexTextureCoordinatesGLBufferForTextureUnit(0);
}

void CC3MeshNode::updateGLBuffers()
{
	if ( m_pMesh )
		m_pMesh->updateGLBuffers(); 
}

void CC3MeshNode::updateVertexIndicesGLBuffer()
{
	if ( m_pMesh )
		m_pMesh->updateVertexIndicesGLBuffer(); 
}


bool CC3MeshNode::shouldCacheFaces()
{
	return m_pMesh ? m_pMesh->shouldCacheFaces() : false; 
}


GLuint CC3MeshNode::getFaceCountFromVertexIndexCount( GLuint vc )
{
	if (m_pMesh) 
		return m_pMesh->getFaceCountFromVertexIndexCount( vc );
	
	CCAssert(false, "CC3MeshNode has no mesh and cannot convert vertex count to face count.");
	return 0;
}

GLuint CC3MeshNode::getVertexIndexCountFromFaceCount( GLuint fc )
{
	if (m_pMesh) 
		return m_pMesh->getVertexIndexCountFromFaceCount(fc);

	CCAssert(false, "CC3MeshNode has no mesh and cannot convert face count to vertex count.");
	return 0;
}


GLuint CC3MeshNode::findFirst( GLuint maxHitCount, CC3MeshIntersection* intersectons, CC3Ray aRay, bool acceptBackFaces, bool acceptBehind )
{
	if ( !m_pMesh ) 
		return 0;

	return m_pMesh->findFirst( maxHitCount, intersectons, aRay, acceptBackFaces, acceptBehind );
}

GLuint CC3MeshNode::findFirstGlobal( GLuint maxHitCount, CC3MeshIntersection* intersections, CC3Ray aRay, bool acceptBackFaces, bool acceptBehind )
{
	if ( !m_pMesh ) 
		return 0;

	// Convert the array to local coordinates and find intersections.
	CC3Ray localRay = getGlobalTransformMatrixInverted()->transformRay( aRay );
	GLuint hitCount = findFirst( maxHitCount, intersections, localRay, acceptBackFaces, acceptBehind );

	// Convert the intersections to global coordinates.
	for (GLuint hitIdx = 0; hitIdx < hitCount; hitIdx++) 
	{
		CC3MeshIntersection* hit = &intersections[hitIdx];
		hit->location = getGlobalTransformMatrix()->transformLocation( hit->location );
		hit->distance = hit->location.distance( aRay.startLocation );
	}

	return hitCount;
}

void CC3MeshNode::setVertexWeight( GLfloat aWeight, GLuint vertexUnit, GLuint index )
{
	if ( m_pMesh )
		m_pMesh->setVertexWeight( aWeight, vertexUnit, index );
}

/// CC3Node method
//-(CC3MeshNode*) getMeshNodeNamed: (NSString*) aName {
//	CC3Node* retrievedNode = [self getNodeNamed: aName];
//	CC3Assert( !retrievedNode || retrievedNode.isMeshNode, @"Retrieved node %@ is not a CC3MeshNode.", retrievedNode);
//	return (CC3MeshNode*)retrievedNode;
//}

bool CC3MeshNode::hasSkeleton()
{
	return false; 
}

bool CC3MeshNode::hasRigidSkeleton()
{
	return false; 
}

CC3Face CC3MeshNode::getDeformedFaceAt( GLuint faceIndex )
{
	return getFaceAt( faceIndex ); 
}

CC3Vector CC3MeshNode::getDeformedFaceCenterAt( GLuint faceIndex )
{
	return getFaceCenterAt( faceIndex ); 
}

CC3Vector CC3MeshNode::getDeformedFaceNormalAt( GLuint faceIndex )
{
	return getFaceNormalAt( faceIndex ); 
}

CC3Plane CC3MeshNode::getDeformedFacePlaneAt( GLuint faceIndex )
{
	return getFacePlaneAt( faceIndex ); 
}

CC3Vector CC3MeshNode::getDeformedVertexLocationAt( GLuint vertexIndex, GLuint faceIndex )
{
	return getVertexLocationAt( vertexIndex );
}

CC3MeshNode* CC3MeshNode::nodeWithName( const std::string& aName )
{
	CC3MeshNode* pNode = new CC3MeshNode;
	pNode->initWithName( aName );
	pNode->autorelease();

	return pNode;
}


void CC3MeshNode::addShadowVolumesForLight( CC3Light* aLight )
{
	// If I shouldn't cast a shadow, or if I already have a shadow volume, just leave.
	if ( !shouldCastShadows() || getShadowVolumeForLight( aLight ) )
		return;

	std::string svName = CC3String::stringWithFormat( "%s-SV-%s", getName().c_str(), aLight->getName().c_str() );
	CC3ShadowVolumeMeshNode* sv = CC3ShadowVolumeMeshNode::nodeWithName( svName );
	sv->selectShaders();

	// Retain data required to build shadow volume mesh
	retainVertexLocations();
	retainVertexIndices();
	setShouldCacheFaces( true );

	prewarmForShadowVolumes();		// Force heavy face calcs now instead of lazily during drawing.

	// Set the active camera to infinite depth of field to accomodate infinite shadow volumes
	getActiveCamera()->setHasInfiniteDepthOfField( true );

	aLight->addShadow( sv );			// Add to light before notifying scene a descendant has been added
	addChild( sv );			// The last thing we do is add the SV to the scene...
	// ...because we might be doing this on a background thread.
	//LogTrace(@"Added shadow volume %@ to %@", sv, self);

	super::addShadowVolumesForLight( aLight );
}

void CC3MeshNode::prewarmForShadowVolumes()
{
	if (getFaceCount() == 0) 
		return;

	// Sample each face characteristics used during shadow volume creation,
	// in order to force the face characteristics to be lazily populated.
	getDeformedFaceAt( 0 );
	getDeformedFacePlaneAt( 0 );
	getFaceNeighboursAt( 0 );

	super::prewarmForShadowVolumes();
}

CC3NormalScaling CC3MeshNode::getEffectiveNormalScalingMethod()
{
	if ( !(m_pMesh && m_pMesh->hasVertexNormals()) ) 
		return kCC3NormalScalingNone;

	switch (m_normalScalingMethod) 
	{
	case kCC3NormalScalingNormalize: 
		return kCC3NormalScalingNormalize;
	case kCC3NormalScalingRescale: 
		return kCC3NormalScalingRescale;
	case kCC3NormalScalingAutomatic:
		if (isTransformRigid()) 
			return kCC3NormalScalingNone;
		else if (isUniformlyScaledGlobally()) 
			return kCC3NormalScalingRescale;
		else return kCC3NormalScalingNormalize;
	case kCC3NormalScalingNone:
	default:
		return kCC3NormalScalingNone;
	}
}

unsigned int CC3MeshNode::getDrawingMode()
{
	return m_pMesh ? m_pMesh->getDrawingMode() : GL_TRIANGLES;
}

void CC3MeshNode::setDrawingMode( GLenum drawingMode )
{
	m_pMesh->setDrawingMode( drawingMode );
}

bool CC3MeshNode::isDrawingPointSprites()
{
	return (getDrawingMode() == GL_POINTS) && (getTextureCount() > 0);
}

void CC3MeshNode::setShouldCacheFaces( bool cacheFaces )
{
	if ( m_pMesh )
		m_pMesh->setShouldCacheFaces( cacheFaces );

	super::setShouldCacheFaces(cacheFaces);
}

void CC3MeshNode::populateAsTriangle( const CC3Face& face, ccTex2F* texCoords, GLuint divsPerSide )
{
	prepareParametricMesh()->populateAsTriangle( face, texCoords, divsPerSide );
}

CC3Mesh* CC3MeshNode::prepareParametricMesh()
{
	if (getVertexContentTypes() == kCC3VertexContentNone) 
	{
		setVertexContentTypes( (CC3VertexContent)(kCC3VertexContentLocation |
			kCC3VertexContentNormal |
			kCC3VertexContentTextureCoordinates) );
	}

	return m_pMesh;
}

void CC3MeshNode::alignTextureUnits()
{
	GLuint texCount = getTextureCount();
	for (GLuint texUnit = 0; texUnit < texCount; texUnit++)
		alignTextureUnit( texUnit );
}

void CC3MeshNode::alignTextureUnit( GLuint texUnit )
{
	if ( m_pMesh )
		m_pMesh->alignTextureUnit( texUnit, getTextureForTextureUnit(texUnit) );
}

CC3Plane CC3MeshNode::getFacePlaneAt( GLuint faceIndex )
{
	return m_pMesh ? m_pMesh->getFacePlaneAt(faceIndex) : CC3Plane( 0.0f, 0.0f, 0.0f, 0.0f );
}

GLuint CC3MeshNode::getFaceCount()
{
	return m_pMesh ? m_pMesh->getFaceCount() : 0; 
}

CC3Face CC3MeshNode::getFaceAt( GLuint faceIndex )
{
	return m_pMesh ? m_pMesh->getFaceAt(faceIndex) : CC3Face::kCC3FaceZero;
}

CC3Face CC3MeshNode::getFaceFromIndices( const CC3FaceIndices& faceIndices )
{
	return m_pMesh ? m_pMesh->getFaceFromIndices( faceIndices ) : CC3Face::kCC3FaceZero;
}

CC3FaceIndices CC3MeshNode::getFaceIndicesAt( GLuint faceIndex )
{
	return m_pMesh ? m_pMesh->getFaceIndicesAt( faceIndex ) : kCC3FaceIndicesZero;
}

CC3Vector CC3MeshNode::getFaceCenterAt( GLuint faceIndex )
{
	return m_pMesh ? m_pMesh->getFaceCenterAt(faceIndex) : CC3Vector::kCC3VectorZero;
}

CC3Vector CC3MeshNode::getFaceNormalAt( GLuint faceIndex )
{
	return m_pMesh ? m_pMesh->getFaceNormalAt( faceIndex ) : CC3Vector::kCC3VectorZero;
}

CC3FaceNeighbours CC3MeshNode::getFaceNeighboursAt( GLuint faceIndex )
{
	CC3FaceNeighbours neighbours;
	neighbours.edges[0] = 0;
	neighbours.edges[1] = 0;
	neighbours.edges[2] = 0;
	return m_pMesh ? m_pMesh->getFaceNeighboursAt(faceIndex) : neighbours;
}

CC3Texture* CC3MeshNode::getTextureForTextureUnit( GLuint texUnit )
{
	return m_pMaterial ? m_pMaterial->getTextureForTextureUnit( texUnit ) : NULL; 
}

CC3Texture* CC3MeshNode::getTexture()
{
	return m_pMaterial ? m_pMaterial->getTexture() : NULL;
}

GLuint CC3MeshNode::getTextureCount()
{
	return m_pMaterial ? m_pMaterial->getTextureCount() : 0;
}


void CC3MeshNode::applyShaderProgramWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	CC3ShaderProgram* pShaderProgram = visitor->getCurrentShaderProgram();
	if ( pShaderProgram )
		pShaderProgram->bindWithVisitor( visitor );
}

void CC3MeshNode::cleanupDrawingParameters( CC3NodeDrawingVisitor* visitor )
{
	
}

void CC3MeshNode::applyMaterialWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	updateLightPosition();
	getMaterial()->drawWithVisitor(visitor);

	// currentColor can be set by material, mesh node, or node picking visitor prior to this method.
	visitor->getGL()->setColor( visitor->getCurrentColor() );
}

void CC3MeshNode::populateAsSphereWithRadius( GLfloat radius, const CC3Tessellation& divsPerAxis )
{
	CC3Mesh* pMesh = prepareParametricMesh();
	if ( pMesh )
		pMesh->populateAsSphereWithRadius( radius, divsPerAxis );
}

void CC3MeshNode::populateAsCenteredRectangleWithSize( const CCSize& rectSize )
{
	CC3Mesh* pMesh = prepareParametricMesh();
	if ( pMesh )
		pMesh->populateAsCenteredRectangleWithSize( rectSize );
}

void CC3MeshNode::populateAsCenteredRectangleWithSize( const CCSize& rectSize, const CC3Tessellation& divsPerAxis )
{
	CC3Mesh* pMesh = prepareParametricMesh();
	if ( pMesh )
		pMesh->populateAsCenteredRectangleWithSize( rectSize, divsPerAxis );
}

void CC3MeshNode::populateAsRectangleWithSize( const CCSize& rectSize, const CCPoint& origin )
{
	CC3Mesh* pMesh = prepareParametricMesh();
	if ( pMesh )
		pMesh->populateAsRectangleWithSize( rectSize, origin );
}

void CC3MeshNode::populateAsRectangleWithSize( const CCSize& rectSize, const CCPoint& origin, const CC3Tessellation& divsPerAxis )
{
	CC3Mesh* pMesh = prepareParametricMesh();
	if ( pMesh )
		pMesh->populateAsRectangleWithSize( rectSize, origin, divsPerAxis );
}

void CC3MeshNode::populateAsLineStripWith( GLuint vertexCount, CC3Vector* vertices, bool shouldRetainVertices )
{
	CC3Mesh* pMesh = prepareParametricMesh();
	if ( pMesh )
		pMesh->populateAsLineStripWith( vertexCount, vertices, shouldRetainVertices );
}

void CC3MeshNode::populateAsWireBox( const CC3Box& box )
{
	CC3Mesh* pMesh = prepareParametricMesh();
	if ( pMesh )
		pMesh->populateAsWireBox( box );
}

void CC3MeshNode::populateAsDiskWithRadius( GLfloat radius, const CC3Tessellation& radialAndAngleDivs )
{
	CC3Mesh* pMesh = prepareParametricMesh();
	if ( pMesh )
		pMesh->populateAsDiskWithRadius( radius, radialAndAngleDivs );
}

void CC3MeshNode::populateAsSolidBox( const CC3Box& box )
{
	CC3Mesh* pMesh = prepareParametricMesh();
	if ( pMesh )
		pMesh->populateAsSolidBox( box );
}

void CC3MeshNode::populateAsSolidBox( const CC3Box& box, const CCPoint& corner )
{
	CC3Mesh* pMesh = prepareParametricMesh();
	if ( pMesh )
		pMesh->populateAsSolidBox( box, corner );
}

void CC3MeshNode::populateAsCubeMappedSolidBox( const CC3Box& box )
{
	CC3Mesh* pMesh = prepareParametricMesh();
	if ( pMesh )
		pMesh->populateAsCubeMappedSolidBox( box );
}

void CC3MeshNode::populateAsHollowConeWithRadius( GLfloat radius, GLfloat height, const CC3Tessellation& angleAndHeightDivs )
{
	CC3Mesh* pMesh = prepareParametricMesh();
	if ( pMesh )
		pMesh->populateAsHollowConeWithRadius( radius, height, angleAndHeightDivs );
}

NS_COCOS3D_END
