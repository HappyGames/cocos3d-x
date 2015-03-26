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
	_material = NULL;
	_mesh = NULL;
	_shaderContext = NULL;
	_decalOffsetFactor = 0.f;
	_decalOffsetUnits = 0.f;
	_hasRigidSkeleton = false;
}

CC3MeshNode::~CC3MeshNode()
{
	CC_SAFE_RELEASE( _material );
	CC_SAFE_RELEASE( _mesh );
	CC_SAFE_RELEASE( _shaderContext );
}

void CC3MeshNode::setName( const std::string& aName )
{
	super::setName( aName );

	if ( _mesh )
		_mesh->deriveNameFrom( this );
	
	if ( _material )
		_material->deriveNameFrom( this );		// Don't lazy init material yet
}

CC3Mesh* CC3MeshNode::getMesh()
{
	return _mesh;
}

// Sets the name of the mesh if needed and marks the bounding volume as dirty.
void CC3MeshNode::setMesh( CC3Mesh * aMesh )
{
	if (aMesh == _mesh) 
		return;
	
	CC_SAFE_RELEASE( _mesh );
	CC_SAFE_RETAIN( aMesh );
	_mesh = aMesh;
	
	if ( _mesh )
		_mesh->deriveNameFrom( this );

	alignMaterialAndMesh();
	markBoundingVolumeDirty();
}

/** If a mesh does not yet exist, create it as a CC3Mesh with interleaved vertices. */
CC3Mesh* CC3MeshNode::ensureMesh()
{
	if ( !_mesh ) 
		setMesh( makeMesh() );

	return _mesh;
}

CC3Mesh* CC3MeshNode::makeMesh()
{
	return CC3Mesh::mesh(); 
}

/** Lazily init the material */
CC3Material* CC3MeshNode::getMaterial()
{
	if ( !_material ) 
		setMaterial( makeMaterial() );

	return _material;
}

void CC3MeshNode::setMaterial( CC3Material* aMaterial )
{
	if (aMaterial == _material) 
		return;

	CC_SAFE_RELEASE( _material );
	CC_SAFE_RETAIN( aMaterial );
	_material = aMaterial;

	if ( _material )
		_material->deriveNameFrom( this );
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
	if ( _mesh && !_mesh->hasVertexNormals() ) 
	{
		if ( _material )
			_material->setShouldUseLighting( false );	// Only if material exists
	}
	
	if ( _mesh && !_mesh->hasVertexTextureCoordinates() ) 
	{
		if ( _material )
			_material->setTexture( NULL );	// Only if material exists
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
	return _shouldDrawInClipSpace; 
}

void CC3MeshNode::setShouldDrawInClipSpace( bool shouldClip )
{
	if (shouldClip != _shouldDrawInClipSpace) 
	{
		_shouldDrawInClipSpace = shouldClip;
		
		if (_shouldDrawInClipSpace) 
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
	return _shouldCullBackFaces; 
}

void CC3MeshNode::setShouldCullBackFaces( bool shouldCull )
{
	_shouldCullBackFaces = shouldCull;
	super::setShouldCullBackFaces( shouldCull );
}

bool CC3MeshNode::shouldCullFrontFaces()
{
	return _shouldCullFrontFaces; 
}

void CC3MeshNode::setShouldCullFrontFaces( bool shouldCull )
{
	_shouldCullFrontFaces = shouldCull;
	super::setShouldCullFrontFaces( shouldCull );
}

bool CC3MeshNode::shouldUseClockwiseFrontFaceWinding()
{
	return _shouldUseClockwiseFrontFaceWinding; 
}

void CC3MeshNode::setShouldUseClockwiseFrontFaceWinding( bool shouldWindCW )
{
	_shouldUseClockwiseFrontFaceWinding = shouldWindCW;
	super::setShouldUseClockwiseFrontFaceWinding( shouldWindCW );
}

bool CC3MeshNode::shouldUseSmoothShading()
{
	return _shouldUseSmoothShading; 
}

void CC3MeshNode::setShouldUseSmoothShading( bool shouldSmooth )
{
	_shouldUseSmoothShading = shouldSmooth;
	super::setShouldUseSmoothShading( shouldSmooth );
}

bool CC3MeshNode::shouldCastShadowsWhenInvisible()
{
	return _shouldCastShadowsWhenInvisible; 
}

void CC3MeshNode::setShouldCastShadowsWhenInvisible( bool shouldCast )
{
	_shouldCastShadowsWhenInvisible = shouldCast;
	super::setShouldCastShadowsWhenInvisible( shouldCast );
}

CC3NormalScaling CC3MeshNode::getNormalScalingMethod()
{
	return _normalScalingMethod; 
}

void CC3MeshNode::setNormalScalingMethod( CC3NormalScaling nsMethod )
{
	_normalScalingMethod = nsMethod;
	super::setNormalScalingMethod( nsMethod );
}

bool CC3MeshNode::shouldDisableDepthMask()
{
	return _shouldDisableDepthMask; 
}

void CC3MeshNode::setShouldDisableDepthMask( bool shouldDisable )
{
	_shouldDisableDepthMask = shouldDisable;
	super::setShouldDisableDepthMask( shouldDisable );
}

bool CC3MeshNode::shouldDisableDepthTest()
{
	return _shouldDisableDepthTest; 
}

void CC3MeshNode::setShouldDisableDepthTest( bool shouldDisable )
{
	_shouldDisableDepthTest = shouldDisable;
	super::setShouldDisableDepthTest( shouldDisable );
}

GLenum CC3MeshNode::getDepthFunction()
{
	return (_depthFunction != GL_NEVER) ? _depthFunction : super::getDepthFunction(); 
}

void CC3MeshNode::setDepthFunction( GLenum depthFunc )
{
	_depthFunction = depthFunc;
	super::setDepthFunction( depthFunc );
}

GLfloat CC3MeshNode::getDecalOffsetFactor()
{
	return _decalOffsetFactor ? _decalOffsetFactor : super::getDecalOffsetFactor(); 
}

void CC3MeshNode::setDecalOffsetFactor( GLfloat factor )
{
	_decalOffsetFactor = factor;
	super::setDecalOffsetFactor( factor );
}

GLfloat CC3MeshNode::getDecalOffsetUnits()
{
	return _decalOffsetUnits ? _decalOffsetUnits : super::getDecalOffsetUnits(); 
}

void CC3MeshNode::setDecalOffsetUnits( GLfloat units )
{
	_decalOffsetUnits = units;
	super::setDecalOffsetUnits( units );
}


bool CC3MeshNode::shouldUseLighting()
{
	return _material ? _material->shouldUseLighting() : false; 
}

void CC3MeshNode::setShouldUseLighting( bool useLighting )
{
	getMaterial()->setShouldUseLighting( useLighting );
	super::setShouldUseLighting( useLighting );	// pass along to any children
}

bool CC3MeshNode::shouldUseLightProbes()
{
	return _shouldUseLightProbes; 
}

void CC3MeshNode::setShouldUseLightProbes( bool shouldUse )
{
	_shouldUseLightProbes = shouldUse;
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
	return (_material && _material->hasBumpMap())
				? getGlobalTransformMatrix()->transformHomogeneousVector( CC3Vector4().fromDirection(_material->getLightDirection()) )
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
	if ( !_shaderContext ) 
		_shaderContext = new CC3ShaderContext;		// retained - don't use setter

	return _shaderContext;
}

// Set shader context if not the same, and pass along to descendants
void CC3MeshNode::setShaderContext( CC3ShaderContext* shaderContext )
{
	if (shaderContext == _shaderContext) 
		return;

	CC_SAFE_RELEASE( _shaderContext );
	_shaderContext = shaderContext;
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

	if (_shouldApplyOpacityAndColorToMeshContent) 
	{
		if ( _mesh )
			_mesh->setColor( color );	// for meshes with colored vertices
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
	if (_shouldApplyOpacityAndColorToMeshContent) 
		_mesh->setOpacity( opacity );	// for meshes with colored vertices

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
	return _shouldApplyOpacityAndColorToMeshContent; 
}

void CC3MeshNode::setShouldApplyOpacityAndColorToMeshContent( bool shouldApply )
{
	_shouldApplyOpacityAndColorToMeshContent = shouldApply;
	super::setShouldApplyOpacityAndColorToMeshContent( shouldApply );
}

GLfloat CC3MeshNode::getLineWidth()
{
	return _lineWidth; 
}

void CC3MeshNode::setLineWidth( GLfloat aLineWidth )
{
	_lineWidth = aLineWidth;
	super::setLineWidth( aLineWidth );
}

bool CC3MeshNode::shouldSmoothLines()
{
	return _shouldSmoothLines; 
}

void CC3MeshNode::setShouldSmoothLines( bool shouldSmooth )
{
	_shouldSmoothLines = shouldSmooth;
	super::setShouldSmoothLines( shouldSmooth );
}

GLenum CC3MeshNode::getLineSmoothingHint()
{
	return _lineSmoothingHint; 
}

void CC3MeshNode::setLineSmoothingHint( GLenum aHint )
{
	_lineSmoothingHint = aHint;
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
	if ( _material )
		_material->removeAllTextures(); 
}

void CC3MeshNode::setTexture( CC3Texture* aTexture, GLuint texUnit )
{
	getMaterial()->setTexture( aTexture, texUnit );
	alignTextureUnit( texUnit );
}

bool CC3MeshNode::expectsVerticallyFlippedTextures()
{
	return _mesh->expectsVerticallyFlippedTextures(); 
}

void CC3MeshNode::setExpectsVerticallyFlippedTextures( bool expectsFlipped )
{
	_mesh->setExpectsVerticallyFlippedTextures( expectsFlipped );
	super::setExpectsVerticallyFlippedTextures( expectsFlipped );
}

bool CC3MeshNode::expectsVerticallyFlippedTextureInTextureUnit( GLuint texUnit )
{
	if ( _mesh )
		return _mesh->expectsVerticallyFlippedTextureInTextureUnit( texUnit );

	return false;
}

void CC3MeshNode::expectsVerticallyFlippedTexture( bool expectsFlipped, GLuint texUnit )
{
	_mesh->setExpectsVerticallyFlippedTexture( expectsFlipped, texUnit );
}

void CC3MeshNode::flipVerticallyTextureUnit( GLuint texUnit )
{
	_mesh->flipVerticallyTextureUnit( texUnit ); 
}

void CC3MeshNode::flipTexturesVertically()
{
	_mesh->flipTexturesVertically();
	super::flipTexturesVertically();
}

void CC3MeshNode::flipHorizontallyTextureUnit( GLuint texUnit )
{
	_mesh->flipHorizontallyTextureUnit( texUnit ); 
}

void CC3MeshNode::flipTexturesHorizontally()
{
	_mesh->flipTexturesHorizontally();
	super::flipTexturesHorizontally();
}

void CC3MeshNode::repeatTexture( ccTex2F repeatFactor, GLuint texUnit )
{
	_mesh->repeatTexture( repeatFactor, texUnit );
}

void CC3MeshNode::repeatTexture( ccTex2F repeatFactor )
{
	_mesh->repeatTexture( repeatFactor ); 
}

CCRect CC3MeshNode::getTextureRectangle()
{
	return _mesh ? _mesh->getTextureRectangle() : kCC3UnitTextureRectangle;
}

void CC3MeshNode::setTextureRectangle( const CCRect& aRect )
{
	_mesh->setTextureRectangle( aRect );  
}

CCRect CC3MeshNode::getTextureRectangleForTextureUnit( GLuint texUnit )
{
	return _mesh ? _mesh->getTextureRectangleForTextureUnit(texUnit) : kCC3UnitTextureRectangle;
}

void CC3MeshNode::setTextureRectangle( const CCRect& aRect, GLuint texUnit )
{
	_mesh->setTextureRectangle( aRect, texUnit );
}


bool CC3MeshNode::hasTextureAlpha()
{
	return _material ? _material->hasTextureAlpha() : false; 
}

bool CC3MeshNode::hasTexturePremultipliedAlpha()
{
	return _material ? _material->hasTexturePremultipliedAlpha() : false; 
}

bool CC3MeshNode::hasPremultipliedAlpha()
{
	return hasTexturePremultipliedAlpha();
}		// Deprecated

bool CC3MeshNode::shouldApplyOpacityToColor()
{
	return _material ? _material->shouldApplyOpacityToColor() : false; 
}

void CC3MeshNode::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	_mesh = NULL;
	_material = NULL;
	_shaderContext = NULL;
	_renderStreamGroupMarker = "";
	_shouldUseLightProbes = false;
	_shouldUseSmoothShading = true;
	_shouldCullBackFaces = true;
	_shouldCullFrontFaces = false;
	_shouldUseClockwiseFrontFaceWinding = false;
	_shouldDisableDepthMask = false;
	_shouldDisableDepthTest = false;
	_shouldCastShadowsWhenInvisible = false;
	_depthFunction = GL_LEQUAL;
	_normalScalingMethod = kCC3NormalScalingAutomatic;
	_lineWidth = 1.0f;
	_shouldSmoothLines = false;
	_lineSmoothingHint = GL_DONT_CARE;
	_shouldApplyOpacityAndColorToMeshContent = false;
	_shouldDrawInClipSpace = false;
	_hasRigidSkeleton = false;
}

void CC3MeshNode::populateFrom( CC3MeshNode* another )
{
	super::populateFrom( another );
	
	// Don't use setters, to avoid side effects, including to bounding volume and tex coords.
	CC_SAFE_RELEASE( _mesh );
	_mesh = another->getMesh();
	CC_SAFE_RETAIN( _mesh );					// retained - Mesh shared between original and copy

	CC_SAFE_RELEASE( _material );
	_material = (CC3Material*)another->getMaterial()->copy();			// retained
	CC_SAFE_RETAIN( _material );
	
	CC_SAFE_RELEASE( _shaderContext );
	_shaderContext = (CC3ShaderContext*)another->getShaderContext()->copy();	// retained
	CC_SAFE_RETAIN( _shaderContext );
	
	_shouldUseSmoothShading = another->shouldUseSmoothShading();
	_shouldCullBackFaces = another->shouldCullBackFaces();
	_shouldCullFrontFaces = another->shouldCullFrontFaces();
	_shouldUseClockwiseFrontFaceWinding = another->shouldUseClockwiseFrontFaceWinding();
	_shouldDisableDepthMask = another->shouldDisableDepthMask();
	_shouldDisableDepthTest = another->shouldDisableDepthTest();
	_shouldCastShadowsWhenInvisible = another->shouldCastShadowsWhenInvisible();
	_depthFunction = another->getDepthFunction();
	_normalScalingMethod = another->getNormalScalingMethod();
	_lineWidth = another->getLineWidth();
	_shouldSmoothLines = another->shouldSmoothLines();
	_lineSmoothingHint = another->getLineSmoothingHint();
	_shouldApplyOpacityAndColorToMeshContent = another->shouldApplyOpacityAndColorToMeshContent();
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
	if ( _mesh )
		_mesh->createGLBuffers();
	
	super::createGLBuffers();
}

void CC3MeshNode::deleteGLBuffers()
{
	if ( _mesh )
		_mesh->deleteGLBuffers();
	
	super::deleteGLBuffers();
}

bool CC3MeshNode::isUsingGLBuffers()
{
	if ( _mesh )
		return _mesh->isUsingGLBuffers(); 

	return false;
}

void CC3MeshNode::releaseRedundantContent()
{
	if ( _mesh )
		_mesh->releaseRedundantContent();

	super::releaseRedundantContent();
}

void CC3MeshNode::retainVertexContent()
{
	if ( _mesh )
		_mesh->retainVertexContent();

	super::retainVertexContent();
}

void CC3MeshNode::retainVertexLocations()
{
	if ( _mesh )
		_mesh->retainVertexLocations();
	
	super::retainVertexLocations();
}

void CC3MeshNode::retainVertexNormals()
{
	if ( _mesh )
		_mesh->retainVertexNormals();
	
	super::retainVertexNormals();
}

void CC3MeshNode::retainVertexTangents()
{
	if ( _mesh )
		_mesh->retainVertexTangents();
	
	super::retainVertexTangents();
}

void CC3MeshNode::retainVertexBitangents()
{
	if ( _mesh )
		_mesh->retainVertexBitangents();
	
	super::retainVertexBitangents();
}

void CC3MeshNode::retainVertexColors()
{
	if ( _mesh )
		_mesh->retainVertexColors();
	
	super::retainVertexColors();
}

void CC3MeshNode::retainVertexBoneIndices()
{
	if ( _mesh )
		_mesh->retainVertexBoneIndices();
	
	super::retainVertexBoneIndices();
}

void CC3MeshNode::retainVertexBoneWeights()
{
	if ( _mesh )
		_mesh->retainVertexBoneWeights();
	
	super::retainVertexBoneWeights();
}

void CC3MeshNode::retainVertexTextureCoordinates()
{
	if ( _mesh )
		_mesh->retainVertexTextureCoordinates();
	
	super::retainVertexTextureCoordinates();
}

void CC3MeshNode::retainVertexIndices()
{
	if ( _mesh )
		_mesh->retainVertexIndices();
	
	super::retainVertexIndices();
}

void CC3MeshNode::doNotBufferVertexContent()
{
	if ( _mesh )
		_mesh->doNotBufferVertexContent();
	
	super::doNotBufferVertexContent();
}

void CC3MeshNode::doNotBufferVertexLocations()
{
	if ( _mesh )
		_mesh->doNotBufferVertexLocations();
	
	super::doNotBufferVertexLocations();
}

void CC3MeshNode::doNotBufferVertexNormals()
{
	if ( _mesh )
		_mesh->doNotBufferVertexNormals();
	
	super::doNotBufferVertexNormals();
}

void CC3MeshNode::doNotBufferVertexTangents()
{
	if ( _mesh )
		_mesh->doNotBufferVertexTangents();
	
	super::doNotBufferVertexTangents();
}

void CC3MeshNode::doNotBufferVertexBitangents()
{
	if ( _mesh )
		_mesh->doNotBufferVertexBitangents();
	
	super::doNotBufferVertexBitangents();
}

void CC3MeshNode::doNotBufferVertexColors()
{
	if ( _mesh )
		_mesh->doNotBufferVertexColors();
	
	super::doNotBufferVertexColors();
}

void CC3MeshNode::doNotBufferVertexBoneIndices()
{
	if ( _mesh )
		_mesh->doNotBufferVertexBoneIndices();
	
	super::doNotBufferVertexBoneIndices();
}

void CC3MeshNode::doNotBufferVertexBoneWeights()
{
	if ( _mesh )
		_mesh->doNotBufferVertexBoneWeights();
	
	super::doNotBufferVertexBoneWeights();
}

void CC3MeshNode::doNotBufferVertexTextureCoordinates()
{
	if ( _mesh )
		_mesh->doNotBufferVertexTextureCoordinates();
	
	super::doNotBufferVertexTextureCoordinates();
}

void CC3MeshNode::doNotBufferVertexIndices()
{
	if ( _mesh )
		_mesh->doNotBufferVertexIndices();
	
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
	gl->enableCullFace(_shouldCullBackFaces || _shouldCullFrontFaces);

	// Set whether back, front or both should be culled.
	// If neither should be culled, handled by capability so leave it as back culling.
	gl->setCullFace( _shouldCullBackFaces
						? (_shouldCullFrontFaces ? GL_FRONT_AND_BACK : GL_BACK)
						: (_shouldCullFrontFaces ? GL_FRONT : GL_BACK) );

	// If back faces are not being culled, then enable two-sided lighting,
	// so that the lighting of the back faces uses negated normals.
	gl->enableTwoSidedLighting( !_shouldCullBackFaces );
	
	// Set the front face winding
	gl->setFrontFace( _shouldUseClockwiseFrontFaceWinding ? GL_CW : GL_CCW );
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
	gl->setShadeModel( _shouldUseSmoothShading ? GL_SMOOTH : GL_FLAT );

	// If per-vertex coloring is being used, attach it to the material
	gl->enableColorMaterial( visitor->shouldDecorateNode() && _mesh && _mesh->hasVertexColors() );
}

/**
 * Template method disables depth testing and/or writing to the depth buffer if the
 * shouldDisableDepthTest and shouldDisableDepthMask property is set to YES, respectively,
 * and set the depth function.
 */
void CC3MeshNode::configureDepthTesting( CC3NodeDrawingVisitor* visitor )
{
	CC3OpenGL* gl = visitor->getGL();
	gl->enableDepthTest( !_shouldDisableDepthTest );
	gl->setDepthMask( !_shouldDisableDepthMask );
	gl->setDepthFunc( _depthFunction );
}

/**
 * Template method that establishes the decal offset parameters to cause the depth
 * of the content being drawn to be offset relative to the depth of the content
 * that has already been drawn.
 */
void CC3MeshNode::configureDecalParameters( CC3NodeDrawingVisitor* visitor )
{
	CC3OpenGL* gl = visitor->getGL();
	bool hasDecalOffset = _decalOffsetFactor || _decalOffsetUnits;
	gl->enablePolygonOffset( hasDecalOffset );
	gl->setPolygonOffsetFactor( _decalOffsetFactor, _decalOffsetUnits );
}

/** Template method to configure line drawing properties. */
void CC3MeshNode::configureLineProperties( CC3NodeDrawingVisitor* visitor )
{
	CC3OpenGL* gl = visitor->getGL();
	gl->setLineWidth( _lineWidth );
	gl->enableLineSmoothing( _shouldSmoothLines );
	gl->setLineSmoothingHint( _lineSmoothingHint );
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
	if ( _mesh )
		_mesh->drawWithVisitor( visitor ); 
}

CC3VertexContent CC3MeshNode::getVertexContentTypes() 
{ 
	return _mesh ? _mesh->getVertexContentTypes() : kCC3VertexContentNone; 
}

void CC3MeshNode::setVertexContentTypes( CC3VertexContent vtxContentTypes )
{
	ensureMesh();
	_mesh->setVertexContentTypes( vtxContentTypes );
	alignMaterialAndMesh();
}

CC3Vector CC3MeshNode::getCenterOfGeometry()
{
	return m_pChildren ? super::getCenterOfGeometry() : getLocalContentCenterOfGeometry();
}

CC3Vector CC3MeshNode::getLocalContentCenterOfGeometry()
{
	return _mesh ? _mesh->getCenterOfGeometry() : CC3Vector::kCC3VectorZero;
}

CC3Box CC3MeshNode::getLocalContentBoundingBox()
{
	return _mesh
			? CC3BoxAddUniformPadding(_mesh->getBoundingBox(), m_fBoundingVolumePadding)
			: kCC3BoxNull;
}

void CC3MeshNode::moveMeshOriginTo( const CC3Vector& aLocation )
{
	_mesh->moveMeshOriginTo( aLocation );
	markBoundingVolumeDirty();
}

void CC3MeshNode::moveMeshOriginToCenterOfGeometry()
{
	_mesh->moveMeshOriginToCenterOfGeometry();
	markBoundingVolumeDirty();
}

GLuint CC3MeshNode::getVertexCount()
{
	return _mesh ? _mesh->getVertexCount() : 0;
}

void CC3MeshNode::setVertexCount( GLuint vCount )
{
	_mesh->setVertexCount( vCount );  
}

GLuint CC3MeshNode::getVertexIndexCount()
{
	return _mesh ? _mesh->getVertexIndexCount() : 0; 
}

void CC3MeshNode::setVertexIndexCount( GLuint vCount )
{
	_mesh->setVertexIndexCount( vCount );  
}

CC3Vector CC3MeshNode::getVertexLocationAt( GLuint index )
{
	return _mesh ? _mesh->getVertexLocationAt(index) : CC3Vector::kCC3VectorZero;
}

void CC3MeshNode::setVertexLocation( const CC3Vector& aLocation, GLuint index )
{
	_mesh->setVertexLocation( aLocation, index );
	markBoundingVolumeDirty();
}

CC3Vector4 CC3MeshNode::getVertexHomogeneousLocationAt( GLuint index )
{
	return _mesh ? _mesh->getVertexHomogeneousLocationAt(index) : kCC3Vector4ZeroLocation;
}

void CC3MeshNode::setVertexHomogeneousLocation( const CC3Vector4& aLocation, GLuint index )
{
	_mesh->setVertexHomogeneousLocation( aLocation, index );
	markBoundingVolumeDirty();
}

CC3Vector CC3MeshNode::getVertexNormalAt( GLuint index )
{
	return _mesh ? _mesh->getVertexNormalAt(index) : CC3Vector::kCC3VectorUnitZPositive;
}

void CC3MeshNode::setVertexNormal( const CC3Vector& aNormal, GLuint index )
{
	_mesh->setVertexNormal( aNormal, index );
}

void CC3MeshNode::flipNormals()
{
	_mesh->flipNormals();
	super::flipNormals();
}

void CC3MeshNode::setPodMaterialIndex( GLint aPodIndex )
{

}

GLint CC3MeshNode::getPodMaterialIndex()
{
	return 0;
}

PODStructPtr CC3MeshNode::getNodePODStructAtIndex( GLuint aPODIndex, CC3PODResource* aPODRez )
{
	if ( aPODRez )
		return aPODRez->getMeshNodePODStructAtIndex( aPODIndex );

	return NULL;
}

void CC3MeshNode::initAtIndex( GLint aPODIndex, CC3PODResource* aPODRez )
{
	super::initAtIndex( aPODIndex, aPODRez );
	SPODNode* pmn = (SPODNode*)getNodePODStructAtIndex( aPODIndex, aPODRez );
	
	// If this node has a mesh, build it
	if ( getPodContentIndex() >= 0 ) 
		setMesh( aPODRez->getMeshAtIndex( getPodContentIndex() ) );
	
	// If this node has a material, build it
	setPodMaterialIndex( pmn->nIdxMaterial );
	if ( getPodMaterialIndex() >= 0)
		setMaterial( aPODRez->getMaterialAtIndex( getPodMaterialIndex() ) );
}

CC3Vector CC3MeshNode::getVertexTangentAt( GLuint index )
{
	return _mesh ? _mesh->getVertexTangentAt(index) : CC3Vector::kCC3VectorUnitXPositive;
}

void CC3MeshNode::setVertexTangent( const CC3Vector& aTangent, GLuint index )
{
	_mesh->setVertexTangent( aTangent, index );
}

CC3Vector CC3MeshNode::getVertexBitangentAt( GLuint index )
{
	return _mesh ? _mesh->getVertexBitangentAt(index) : CC3Vector::kCC3VectorUnitYPositive;
}

void CC3MeshNode::setVertexBitangent( const CC3Vector& aTangent, GLuint index )
{
	_mesh->setVertexBitangent( aTangent, index );
}

GLenum CC3MeshNode::getVertexColorType()
{
	return _mesh ? _mesh->getVertexColorType() : GL_FALSE; 
}

ccColor4F CC3MeshNode::getVertexColor4FAt( GLuint index )
{
	return _mesh ? _mesh->getVertexColor4FAt(index) : kCCC4FBlackTransparent;
}

void CC3MeshNode::setVertexColor4F( const ccColor4F& aColor, GLuint index )
{
	ccColor4F color = aColor;
	if ( shouldApplyOpacityToColor() ) 
		color = CCC4FBlendAlpha( color );
	_mesh->setVertexColor4F( color, index );
}

ccColor4B CC3MeshNode::getVertexColor4BAt( GLuint index )
{
	return _mesh ? _mesh->getVertexColor4BAt(index) : ccc4(  0, 0, 0, 0  );
}

void CC3MeshNode::setVertexColor4B( const ccColor4B& aColor, GLuint index )
{
	ccColor4B color = aColor;
	if ( shouldApplyOpacityToColor() ) 
		color = CCC4BBlendAlpha(aColor);

	_mesh->setVertexColor4B( color, index );
}

GLuint CC3MeshNode::getVertexBoneCount()
{
	return _mesh ? _mesh->getVertexBoneCount() : 0; 
}

GLfloat CC3MeshNode::getVertexWeightForBoneInfluence( GLuint influenceIndex, GLuint vtxIndex )
{
	return _mesh ? _mesh->getVertexWeightForBoneInfluence( influenceIndex, vtxIndex ) : 0.0f;
}

GLfloat* CC3MeshNode::getVertexBoneWeightsAt( GLuint vtxIndex )
{
	return _mesh ? _mesh->getVertexBoneWeightsAt(vtxIndex) : NULL; 
}

void CC3MeshNode::setVertexBoneWeights( GLfloat* weights, GLuint vtxIndex )
{
	_mesh->setVertexBoneWeights( weights, vtxIndex );
}

GLuint CC3MeshNode::getVertexBoneIndexForBoneInfluence( GLuint influenceIndex, GLuint vtxIndex )
{
	return _mesh ? _mesh->getVertexBoneIndexForBoneInfluence( influenceIndex, vtxIndex ) : 0;
}

void CC3MeshNode::setVertexBoneIndex( GLuint boneIndex, GLuint influenceIndex, GLuint vtxIndex )
{
	_mesh->setVertexBoneIndex( boneIndex, influenceIndex, vtxIndex );
}

GLvoid* CC3MeshNode::getVertexBoneIndicesAt( GLuint vtxIndex )
{
	return _mesh ? _mesh->getVertexBoneIndicesAt(vtxIndex) : NULL;
}

void CC3MeshNode::setVertexBoneIndices( GLvoid* boneIndices, GLuint vtxIndex )
{
	_mesh->setVertexBoneIndices( boneIndices, vtxIndex );
}

GLenum CC3MeshNode::getVertexBoneIndexType()
{
	return _mesh->getVertexBoneIndexType(); 
}

ccTex2F CC3MeshNode::getVertexTexCoord2FForTextureUnit( GLuint texUnit, GLuint index )
{
	return _mesh ? _mesh->getVertexTexCoord2FForTextureUnit(texUnit, index) : tex2( 0.0f, 0.0f );
}

void CC3MeshNode::setVertexTexCoord2F( ccTex2F aTex2F, GLuint texUnit, GLuint index )
{
	if ( _mesh )
		_mesh->setVertexTexCoord2F( aTex2F, texUnit, index );
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
	return _mesh ? _mesh->getVertexIndexAt(index) : 0; 
}

void CC3MeshNode::setVertexIndex( GLuint vertexIndex, GLuint index )
{
	if ( _mesh )
		_mesh->setVertexIndex( vertexIndex, index );
}

void CC3MeshNode::updateVertexLocationsGLBuffer()
{
	if ( _mesh )
		_mesh->updateVertexLocationsGLBuffer(); 
}

void CC3MeshNode::updateVertexNormalsGLBuffer()
{
	if ( _mesh )
		_mesh->updateVertexNormalsGLBuffer(); 
}

void CC3MeshNode::updateVertexTangentsGLBuffer()
{
	if ( _mesh )
		_mesh->updateVertexTangentsGLBuffer(); 
}

void CC3MeshNode::updateVertexBitangentsGLBuffer()
{
	if ( _mesh )
		_mesh->updateVertexBitangentsGLBuffer(); 
}

void CC3MeshNode::updateVertexColorsGLBuffer()
{
	if ( _mesh )
		_mesh->updateVertexColorsGLBuffer(); 
}

void CC3MeshNode::updateVertexBoneWeightsGLBuffer()
{
	if ( _mesh )
		_mesh->updateVertexBoneWeightsGLBuffer(); 
}

void CC3MeshNode::updateVertexBoneIndicesGLBuffer()
{
	if ( _mesh )
		_mesh->updateVertexBoneIndicesGLBuffer(); 
}

void CC3MeshNode::updateVertexTextureCoordinatesGLBufferForTextureUnit( GLuint texUnit )
{
	if ( _mesh )
		_mesh->updateVertexTextureCoordinatesGLBufferForTextureUnit( texUnit );
}

void CC3MeshNode::updateVertexTextureCoordinatesGLBuffer()
{
	updateVertexTextureCoordinatesGLBufferForTextureUnit(0);
}

void CC3MeshNode::updateGLBuffers()
{
	if ( _mesh )
		_mesh->updateGLBuffers(); 
}

void CC3MeshNode::updateVertexIndicesGLBuffer()
{
	if ( _mesh )
		_mesh->updateVertexIndicesGLBuffer(); 
}


bool CC3MeshNode::shouldCacheFaces()
{
	return _mesh ? _mesh->shouldCacheFaces() : false; 
}


GLuint CC3MeshNode::getFaceCountFromVertexIndexCount( GLuint vc )
{
	if (_mesh) 
		return _mesh->getFaceCountFromVertexIndexCount( vc );
	
	CCAssert(false, "CC3MeshNode has no mesh and cannot convert vertex count to face count.");
	return 0;
}

GLuint CC3MeshNode::getVertexIndexCountFromFaceCount( GLuint fc )
{
	if (_mesh) 
		return _mesh->getVertexIndexCountFromFaceCount(fc);

	CCAssert(false, "CC3MeshNode has no mesh and cannot convert face count to vertex count.");
	return 0;
}


GLuint CC3MeshNode::findFirst( GLuint maxHitCount, CC3MeshIntersection* intersectons, CC3Ray aRay, bool acceptBackFaces, bool acceptBehind )
{
	if ( !_mesh ) 
		return 0;

	return _mesh->findFirst( maxHitCount, intersectons, aRay, acceptBackFaces, acceptBehind );
}

GLuint CC3MeshNode::findFirstGlobal( GLuint maxHitCount, CC3MeshIntersection* intersections, CC3Ray aRay, bool acceptBackFaces, bool acceptBehind )
{
	if ( !_mesh ) 
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
	if ( _mesh )
		_mesh->setVertexWeight( aWeight, vertexUnit, index );
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

#pragma _NOTE_TODO( "CC3MeshNode::addShadowVolumesForLight( CC3Light* aLight )" )
	//std::string svName = stringWithFormat( "%s-SV-%s", getName().c_str(), aLight->getName().c_str() );
	//CC3Node<CC3ShadowProtocol>* sv = [[self shadowVolumeClass] nodeWithName: svName];
	//sv->selectShaders();

	//// Retain data required to build shadow volume mesh
	//retainVertexLocations();
	//retainVertexIndices();
	//setShouldCacheFaces( true );

	//prewarmForShadowVolumes();		// Force heavy face calcs now instead of lazily during drawing.

	//// Set the active camera to infinite depth of field to accomodate infinite shadow volumes
	//getActiveCamera()->setHasInfiniteDepthOfField( true );

	//aLight->addShadow( sv );			// Add to light before notifying scene a descendant has been added
	//addChild( sv );			// The last thing we do is add the SV to the scene...
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

void CC3MeshNode::applyEffectNamedFromRez( const std::string& effectName, const std::string& rezName )
{
	CC3PFXEffect* pfxEffect = CC3PFXResource::getEffectNamedFromRez( effectName, rezName );
	if ( pfxEffect )
	{
		pfxEffect->populateMeshNode( this );
		pfxEffect->populateMaterial( getMaterial() );
		alignTextureUnits();
	}
	
	super::applyEffectNamedFromRez( effectName, rezName );
}

void CC3MeshNode::applyEffectNamedFromFile( const std::string& effectName, const std::string& filePath )
{
	CC3PFXEffect* pfxEffect = CC3PFXResource::getEffectNamedFromFile( effectName, filePath );
	if ( pfxEffect )
	{
		pfxEffect->populateMeshNode( this );
		pfxEffect->populateMaterial( getMaterial() );
		alignTextureUnits();
	}

	super::applyEffectNamedFromFile( effectName, filePath );
}

CC3NormalScaling CC3MeshNode::getEffectiveNormalScalingMethod()
{
	if ( !(_mesh && _mesh->hasVertexNormals()) ) 
		return kCC3NormalScalingNone;

	switch (_normalScalingMethod) 
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
	return _mesh ? _mesh->getDrawingMode() : GL_TRIANGLES;
}

void CC3MeshNode::setDrawingMode( GLenum drawingMode )
{
	_mesh->setDrawingMode( drawingMode );
}

bool CC3MeshNode::isDrawingPointSprites()
{
	return (getDrawingMode() == GL_POINTS) && (getTextureCount() > 0);
}

void CC3MeshNode::setShouldCacheFaces( bool cacheFaces )
{
	if ( _mesh )
		_mesh->setShouldCacheFaces( cacheFaces );

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

	return _mesh;
}

void CC3MeshNode::alignTextureUnits()
{
	GLuint texCount = getTextureCount();
	for (GLuint texUnit = 0; texUnit < texCount; texUnit++)
		alignTextureUnit( texUnit );
}

void CC3MeshNode::alignTextureUnit( GLuint texUnit )
{
	if ( _mesh )
		_mesh->alignTextureUnit( texUnit, getTextureForTextureUnit(texUnit) );
}

CC3Plane CC3MeshNode::getFacePlaneAt( GLuint faceIndex )
{
	return _mesh ? _mesh->getFacePlaneAt(faceIndex) : CC3PlaneMake( 0.0f, 0.0f, 0.0f, 0.0f );
}

GLuint CC3MeshNode::getFaceCount()
{
	return _mesh ? _mesh->getFaceCount() : 0; 
}

CC3Face CC3MeshNode::getFaceAt( GLuint faceIndex )
{
	return _mesh ? _mesh->getFaceAt(faceIndex) : kCC3FaceZero;
}

CC3Face CC3MeshNode::getFaceFromIndices( const CC3FaceIndices& faceIndices )
{
	return _mesh ? _mesh->getFaceFromIndices( faceIndices ) : kCC3FaceZero;
}

CC3FaceIndices CC3MeshNode::getFaceIndicesAt( GLuint faceIndex )
{
	return _mesh ? _mesh->getFaceIndicesAt( faceIndex ) : kCC3FaceIndicesZero;
}

CC3Vector CC3MeshNode::getFaceCenterAt( GLuint faceIndex )
{
	return _mesh ? _mesh->getFaceCenterAt(faceIndex) : CC3Vector::kCC3VectorZero;
}

CC3Vector CC3MeshNode::getFaceNormalAt( GLuint faceIndex )
{
	return _mesh ? _mesh->getFaceNormalAt( faceIndex ) : CC3Vector::kCC3VectorZero;
}

CC3FaceNeighbours CC3MeshNode::getFaceNeighboursAt( GLuint faceIndex )
{
	CC3FaceNeighbours neighbours;
	neighbours.edges[0] = 0;
	neighbours.edges[1] = 0;
	neighbours.edges[2] = 0;
	return _mesh ? _mesh->getFaceNeighboursAt(faceIndex) : neighbours;
}

CC3Texture* CC3MeshNode::getTextureForTextureUnit( GLuint texUnit )
{
	return _material ? _material->getTextureForTextureUnit( texUnit ) : NULL; 
}

CC3Texture* CC3MeshNode::getTexture()
{
	return _material ? _material->getTexture() : NULL;
}

GLuint CC3MeshNode::getTextureCount()
{
	return _material ? _material->getTextureCount() : 0;
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
