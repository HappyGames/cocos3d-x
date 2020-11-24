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

static CC3Cache* _textureCache = NULL;

static void ensureCache()
{
	if ( !_textureCache )
	{
		_textureCache = CC3Cache::weakCacheForType( "texture" );
		_textureCache->retain();
	}
}

CC3Texture::CC3Texture()
{
	m_textureID = 0;
	m_ccTexture = NULL;
}

CC3Texture::~CC3Texture()
{
	remove();
	deleteGLTexture();
	CC_SAFE_RELEASE( m_ccTexture ); 
}

GLuint CC3Texture::getTextureID()
{
	ensureGLTexture();
	return m_textureID;
}

void CC3Texture::ensureGLTexture()
{ 
	if (!m_textureID) 
		m_textureID = CC3OpenGL::sharedGL()->generateTexture();
}

/**
* If the GL texture is also tracked by a CCTexture, the CCTexture will delete the GL texture
* when it is deallocated, but we must tell the 3D state engine to stop tracking this texture.
* Otherwise, if no CCTexture is tracking the GL texture, delete it from the GL engine now.
*/
void CC3Texture::deleteGLTexture()
{
	if (m_ccTexture)
		CC3OpenGL::sharedGL()->clearTextureBinding( m_textureID );
	else		  
		CC3OpenGL::sharedGL()->deleteTexture( m_textureID );

	m_textureID = 0;
}

/** If the texture has been created, set its debug label as well. */
void CC3Texture::setName( const std::string& name )
{
	super::setName( name );
	checkGLDebugLabel();
}

void CC3Texture::removeAllTextures()
{
	if ( _textureCache )
		_textureCache->removeAllObjects();

	CC_SAFE_RELEASE( _textureCache );
}

bool CC3Texture::isPreloading()
{
	return _textureCache ? _textureCache->isWeak() : false;
}

void CC3Texture::setIsPreloading( bool bPreloading )
{
	ensureCache();
	_textureCache->setIsWeak( !isPreloading() );
}

/** Sets the GL debug label, if required. */
void CC3Texture::checkGLDebugLabel()
{
	if (m_textureID) 
		CC3OpenGL::sharedGL()->setTextureDebugLabel( getName().c_str(), m_textureID );
}

bool CC3Texture::isPOTWidth()
{ 
	return (m_size.width == CCNextPOT(m_size.width)); 
}

bool CC3Texture::isPOTHeight()
{ 
	return (m_size.height == CCNextPOT(m_size.height)); 
}

bool CC3Texture::isPOT()
{ 
	return isPOTWidth() && isPOTHeight(); 
}

GLenum CC3Texture::getSamplerSemantic()
{
	return kCC3SemanticNone;
}

bool CC3Texture::isTexture2D()
{ 
	return false; 
}

bool CC3Texture::isTextureCube()
{ 
	return false;
}

bool CC3Texture::isUpsideDown()
{
	return m_isUpsideDown;
}

void CC3Texture::setIsUpsideDown( bool bUpsideDown )
{
	m_isUpsideDown = bUpsideDown;
}

CCSize CC3Texture::getCoverage()
{
	return m_coverage;
}

bool CC3Texture::hasAlpha()
{
	return m_hasAlpha;
}

void CC3Texture::setHasAlpha( bool hasAlpha )
{
	m_hasAlpha = hasAlpha;
}

bool CC3Texture::hasPremultipliedAlpha()
{
	return m_hasPremultipliedAlpha;
}

void CC3Texture::setHasPremultipliedAlpha( bool bHasAlpha )
{
	m_hasPremultipliedAlpha = bHasAlpha;
}

GLenum CC3Texture::getTextureTarget()
{
	CCAssert( false, "getTextureTarget is not implemented" );
	return GL_ZERO;
}

GLenum CC3Texture::getInitialAttachmentFace()
{
	CCAssert( false, "getInitialAttachmentFace is not implemented" );
	return GL_ZERO;
}

CC3TextureUnit* CC3Texture::getTextureUnit()
{
	return NULL; 
}

void CC3Texture::setTextureUnit( CC3TextureUnit* textureUnit )
{

}

CC3Vector CC3Texture::getLightDirection() { return CC3Vector::kCC3VectorZero; }

void CC3Texture::setLightDirection( const CC3Vector& aDirection )
{

}

bool CC3Texture::isBumpMap()
{
	return false; 
}

CC3Texture* CC3Texture::getTexture()
{ 
	return this; 
}

bool CC3Texture::defaultShouldFlipVerticallyOnLoad()
{ 
	return false; 
}

void CC3Texture::setDefaultShouldFlipVerticallyOnLoad( bool shouldFlip )
{

}

bool CC3Texture::defaultShouldFlipHorizontallyOnLoad()
{
	return false; 
}

void CC3Texture::setDefaultShouldFlipHorizontallyOnLoad( bool shouldFlip )
{

}

void CC3Texture::setShouldFlipVerticallyOnLoad( bool flip )
{
	m_shouldFlipVerticallyOnLoad = flip;
}

bool CC3Texture::shouldFlipVerticallyOnLoad()
{
	return m_shouldFlipVerticallyOnLoad;
}

void CC3Texture::setShouldFlipHorizontallyOnLoad( bool flip )
{
	m_shouldFlipHorizontallyOnLoad = flip;
}

bool CC3Texture::shouldFlipHorizontallyOnLoad()
{
	return m_shouldFlipHorizontallyOnLoad;
}

void CC3Texture::bindTextureContent( CC3CCTexture* texContent, GLenum target )
{
	checkTextureOrientation( texContent );

	m_size = CC3IntSizeMake((GLint)texContent->getPixelsWide(), (GLint)texContent->getPixelsHigh());
	m_coverage = CCSizeMake(texContent->getMaxS(), texContent->getMaxT());
	m_pixelFormat = texContent->getPixelGLFormat();
	m_pixelType = texContent->getPixelGLType();
	m_hasAlpha = texContent->hasAlpha();
	m_hasPremultipliedAlpha = texContent->hasPremultipliedAlpha();
	m_isUpsideDown = texContent->isUpsideDown();

	CC3OpenGL* gl = CC3OpenGL::sharedGL();
	GLuint tuIdx = 0;		// Choose the texture unit in which to work

	unsigned int bitsPerPixel;
	//Hack: bitsPerPixelForFormat returns wrong number for RGB_888 textures. See function.
	if(texContent->getPixelFormat() == CCTexturePixelFormat_RGB888)
		bitsPerPixel = 24;
	else
		bitsPerPixel = texContent->bitsPerPixelForFormat();

	unsigned int bytesPerRow = texContent->getPixelWidth() * bitsPerPixel / 8;

	unsigned int byteAlignment = 0;
	if(bytesPerRow % 8 == 0)
		byteAlignment = 8;
	else if(bytesPerRow % 4 == 0)
		byteAlignment = 4;
	else if(bytesPerRow % 2 == 0)
		byteAlignment = 2;
	else
		byteAlignment = 1;

	gl->bindTexture( getTextureID(), getTextureTarget(), tuIdx );
	gl->loadTexureImage( texContent->getImageData(), target, 0, m_size, m_pixelFormat, m_pixelType, byteAlignment/*getByteAlignment()*/, tuIdx );

	bindTextureParametersAt( tuIdx, gl );

	CHECK_GL_ERROR_DEBUG();
}

GLuint CC3Texture::getByteAlignment()
{
	// First see if we can figure it out based on pixel type
	switch (m_pixelType) {
	case GL_UNSIGNED_SHORT:
	case GL_UNSIGNED_SHORT_4_4_4_4:
	case GL_UNSIGNED_SHORT_5_5_5_1:
	case GL_UNSIGNED_SHORT_5_6_5:
		return CC3IntIsEven(m_size.width) ? 4 : 2;

	case GL_UNSIGNED_INT:
		//case GL_UNSIGNED_INT_24_8_OES:
#pragma _NOTE_TODO( "platform camptility" )
		return 4;
	}

	// Pixel type at this point is GL_UNSIGNED_BYTE.
	// See if we can figure it out based on pixel format.
	switch (m_pixelFormat) 
	{
	case GL_RGBA: return 4;
	case GL_LUMINANCE_ALPHA: 
		return CC3IntIsEven(m_size.width) ? 4 : 2;
	}

	// Boundary is at byte level, so check it based on whether
	// texture width is divisible by either 4 or 2.
	return CC3IntIsEven(m_size.width) ? (CC3IntIsEven(m_size.width / 2) ? 4 : 2) : 1;
}

void CC3Texture::bindTextureOfColor( const ccColor4B& color, const CC3IntSize& size, GLenum target )
{
	CC3Texture2DContent* texContent = new CC3Texture2DContent;
	texContent->initWithSize( size, color );
	bindTextureContent( texContent, target );
	texContent->release();
}

CC3CCTexture* CC3Texture::getSizedContent()
{
	if (m_ccTexture) 
	{
		m_ccTexture->resizeTo( getSize() );
		return m_ccTexture;
	} 
	else 
	{
		CC3Texture2DContent* texContent = new CC3Texture2DContent;
		texContent->initWithSize( getSize(), getPixelFormat(), getPixelType() );
		texContent->autorelease();

		return texContent;
	}
}

void CC3Texture::generateMipmap()
{
	if ( hasMipmap() || !isPOT())
		return;

	CC3OpenGL* gl = CC3OpenGL::sharedGL();
	GLuint tuIdx = 0;	// Choose the texture unit in which to work
	GLenum target = getTextureTarget();
	gl->bindTexture( getTextureID() , target, tuIdx );
	gl->generateMipmapForTarget( target, tuIdx );
	m_hasMipmap = true;

	markTextureParametersDirty();
}

/** Indicates whether Mipmaps should automatically be generated for any loaded textures. */
static bool _shouldGenerateMipmaps = true;

bool CC3Texture::shouldGenerateMipmaps()
{ 
	return _shouldGenerateMipmaps;
}

void CC3Texture::setShouldGenerateMipmaps( bool shouldMipmap )
{
	_shouldGenerateMipmaps = shouldMipmap;
}

GLenum CC3Texture::getMinifyingFunction()
{
	if ( hasMipmap() ) 
		return m_minifyingFunction;

	switch (m_minifyingFunction) 
	{
	case GL_LINEAR:
	case GL_LINEAR_MIPMAP_NEAREST:
	case GL_LINEAR_MIPMAP_LINEAR:
		return GL_LINEAR;

	case GL_NEAREST:
	case GL_NEAREST_MIPMAP_NEAREST:
	case GL_NEAREST_MIPMAP_LINEAR:
	default:
		return GL_NEAREST;
	}
}

void CC3Texture::setMinifyingFunction( GLenum minifyingFunction )
{
	m_minifyingFunction = minifyingFunction;
	markTextureParametersDirty();
}

GLenum CC3Texture::getMagnifyingFunction()
{ 
	return m_magnifyingFunction; 
}

void CC3Texture::setMagnifyingFunction( GLenum magnifyingFunction )
{
	m_magnifyingFunction = magnifyingFunction;
	markTextureParametersDirty();
}

GLenum CC3Texture::getHorizontalWrappingFunction()
{
	return isPOT() ? m_horizontalWrappingFunction : GL_CLAMP_TO_EDGE;
}

void CC3Texture::setHorizontalWrappingFunction( GLenum horizontalWrappingFunction )
{
	m_horizontalWrappingFunction = horizontalWrappingFunction;
	markTextureParametersDirty();
}

GLenum CC3Texture::getVerticalWrappingFunction()
{
	return isPOT() ? m_verticalWrappingFunction : GL_CLAMP_TO_EDGE;
}

void CC3Texture::setVerticalWrappingFunction( GLenum verticalWrappingFunction )
{
	m_verticalWrappingFunction = verticalWrappingFunction;
	markTextureParametersDirty();
}

ccTexParams CC3Texture::getTextureParameters()
{
	ccTexParams texParams;
	texParams.minFilter = getMinifyingFunction();		// Must use property
	texParams.magFilter = getMagnifyingFunction();
	texParams.wrapS = getHorizontalWrappingFunction();
	texParams.wrapT = getVerticalWrappingFunction();
	return texParams;
}

void CC3Texture::setTextureParameters( const ccTexParams& texParams )
{
	m_minifyingFunction = texParams.minFilter;
	m_magnifyingFunction = texParams.magFilter;
	m_horizontalWrappingFunction = texParams.wrapS;
	m_verticalWrappingFunction = texParams.wrapT;
	markTextureParametersDirty();
}

void CC3Texture::markTextureParametersDirty()
{ 
	m_texParametersAreDirty = true; 
}

/** Default texture parameters. */
static ccTexParams _defaultTextureParameters = { GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR, GL_REPEAT, GL_REPEAT };

ccTexParams CC3Texture::defaultTextureParameters()
{
	return _defaultTextureParameters; 
}

void CC3Texture::setDefaultTextureParameters( const ccTexParams& texParams )
{
	_defaultTextureParameters = texParams; 
}

// This method uses no direct iVar references, to allow subclasses (incl CC3TextureUnitTexture) to override.
void CC3Texture::drawWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	CCAssert(getTextureID(), "%CC3Texture:: cannot be bound to the GL engine because it has not been loaded.");

	CC3OpenGL* gl = visitor->getGL();
	GLuint tuIdx = getTextureUnitFromVisitor( visitor );
	GLenum target = getTextureTarget();

	gl->enableTexturing( true, target, tuIdx );
	gl->bindTexture( getTextureID(), target, tuIdx );

	bindTextureParametersAt( tuIdx, gl );
	bindTextureEnvironmentWithVisitor( visitor );

	incrementTextureUnitInVisitor( visitor );
}

/** If the texture parameters are dirty, binds them to the GL texture unit state. */
void CC3Texture::bindTextureParametersAt( GLuint tuIdx, CC3OpenGL* gl )
{
	if ( !m_texParametersAreDirty ) 
		return;

	// Use property accessors to allow adjustments from the raw values
	GLenum target = getTextureTarget();
	gl->setTextureMinifyFunc( getMinifyingFunction(), target, tuIdx );
	gl->setTextureMagnifyFunc( getMagnifyingFunction(), target, tuIdx );
	gl->setTextureHorizWrapFunc( getHorizontalWrappingFunction(), target, tuIdx );
	gl->setTextureVertWrapFunc( getVerticalWrappingFunction(), target, tuIdx );

	m_texParametersAreDirty = false;
}

/** Binds the default texture unit environment to the GL engine. */
void CC3Texture::bindTextureEnvironmentWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	CC3TextureUnit::bindDefaultWithVisitor( visitor );
}

/** 
* Returns the appopriate texture unit, by retrieving it from the specfied visitor.
*
* The visitor keeps track of separate counters for 2D and cube-map textures,
* and subclasses of this class will determine which of these to retrieve.
*/
GLuint CC3Texture::getTextureUnitFromVisitor( CC3NodeDrawingVisitor* visitor )
{
	CCAssert(false, "getTextureUnitFromVisitor: is not implemented by CC3Texture");
	return 0;
}

/**
* Increments the appopriate texture unit in the specfied visitor.
*
* The visitor keeps track of separate counters for 2D and cube-map textures, 
* and subclasses of this class will determine which of these to increment.
*/
void CC3Texture::incrementTextureUnitInVisitor( CC3NodeDrawingVisitor* visitor )
{
	CCAssert( false, "incrementTextureUnitInVisitor: is not implemented by CC3Texture");
}

bool CC3Texture::loadTarget( GLenum target, const std::string& filePath )
{
	if ( m_sName.empty() ) 
		setName( textureNameFromFilePath( filePath ).c_str() );

	//MarkRezActivityStart();

	CC3Texture2DContent* content = new CC3Texture2DContent;
	if ( !content->initFromFile( filePath ) )
	{
		content->release();
		CC3_TRACE( "CC3Texture could not load texture from file %s", filePath.c_str() );
		return false;
	}

	bindTextureContent( content, target );
	content->release();		// Could be big, so get rid of it immediately

	//LogRez(@"%@ loaded from file %@ in %.3f ms", self, filePath, GetRezActivityDuration() * 1000);
	return true;
}

bool CC3Texture::loadFromFile( const std::string& filePath )
{
	bool wasLoaded = loadTarget( getTextureTarget(), filePath );
	if (wasLoaded && shouldGenerateMipmaps()) 
		generateMipmap();

	checkGLDebugLabel();

	return wasLoaded;
}

void CC3Texture::checkTextureOrientation( CC3CCTexture* texContent )
{
	bool flipHorz = shouldFlipHorizontallyOnLoad();
	bool flipVert = !XOR(texContent->isUpsideDown(), shouldFlipVerticallyOnLoad());

	if (flipHorz && flipVert)
		texContent->rotateHalfCircle();		// Do both in one pass
	else if (flipHorz)
		texContent->flipHorizontally();
	else if (flipVert)
		texContent->flipVertically();
}

void CC3Texture::replacePixels( const CC3Viewport& rect, GLenum target, ccColor4B* colorArray )
{
	// If needed, convert the contents of the array to the format and type of this texture
	convertContent( colorArray, (rect.w * rect.h) );

	CC3OpenGL* gl = CC3OpenGL::sharedGL();
	GLuint tuIdx = 0;		// Choose the texture unit in which to work
	gl->bindTexture( getTextureID(), getTextureTarget(), tuIdx );
	gl->loadTexureSubImage( (const GLvoid*) colorArray, target, 0, rect, m_pixelFormat, m_pixelType, 1, tuIdx );
}

/**
* Converts the pixels in the specified array to the format and type used by this texture.
* Upon completion, the specified pixel array will contain the converted pixels.
*
* Since the pixels in any possible converted format will never consume more memory than
* the pixels in the incoming 32-bit RGBA format, the conversion is perfomed in-place.
*/
void CC3Texture::convertContent( ccColor4B* colorArray, GLuint pixCount )
{
	switch (m_pixelType) {
	case GL_UNSIGNED_BYTE:
		switch (m_pixelFormat) {
		case GL_RGB: {
			ccColor3B* rgbArray = (ccColor3B*)colorArray;
			for (GLuint pixIdx = 0; pixIdx < pixCount; pixIdx++)
				rgbArray[pixIdx] = CCC3BFromCCC4B(colorArray[pixIdx]);
			break;
					 }
		case GL_ALPHA: {
			GLubyte* bArray = (GLubyte*)colorArray;
			for (GLuint pixIdx = 0; pixIdx < pixCount; pixIdx++)
				bArray[pixIdx] = colorArray[pixIdx].a;
			break;
					   }
		case GL_LUMINANCE: {
			GLubyte* bArray = (GLubyte*)colorArray;
			for (GLuint pixIdx = 0; pixIdx < pixCount; pixIdx++) {
				ccColor4B* pRGBA = colorArray + pixIdx;
				GLfloat luma = CC3LuminosityBT709(CCColorFloatFromByte(pRGBA->r),
					CCColorFloatFromByte(pRGBA->g),
					CCColorFloatFromByte(pRGBA->b));
				bArray[pixIdx] = CCColorByteFromFloat(luma);
			}
			break;
						   }
		case GL_LUMINANCE_ALPHA: {
			GLushort* usArray = (GLushort*)colorArray;
			for (GLuint pixIdx = 0; pixIdx < pixCount; pixIdx++) {
				ccColor4B* pRGBA = colorArray + pixIdx;
				GLfloat luma = CC3LuminosityBT709(CCColorFloatFromByte(pRGBA->r),
					CCColorFloatFromByte(pRGBA->g),
					CCColorFloatFromByte(pRGBA->b));
				usArray[pixIdx] = (((GLushort)CCColorByteFromFloat(luma)  << 8) |
					((GLushort)pRGBA->a));
			}
			break;
								 }
		case GL_RGBA:		// Already in RGBA format so do nothing!
		default:
			break;
		}
		break;
	case GL_UNSIGNED_SHORT_5_6_5: {
		GLushort* usArray = (GLushort*)colorArray;
		for (GLuint pixIdx = 0; pixIdx < pixCount; pixIdx++) {
			ccColor4B* pRGBA = colorArray + pixIdx;
			usArray[pixIdx] = ((((GLushort)pRGBA->r >> 3) << 11) |
				(((GLushort)pRGBA->g >> 2) <<  5) |
				(((GLushort)pRGBA->b >> 3)));
		}
		break;
								  }
	case GL_UNSIGNED_SHORT_4_4_4_4: {
		GLushort* usArray = (GLushort*)colorArray;
		for (GLuint pixIdx = 0; pixIdx < pixCount; pixIdx++) {
			ccColor4B* pRGBA = colorArray + pixIdx;
			usArray[pixIdx] = ((((GLushort)pRGBA->r >> 4) << 12) |
				(((GLushort)pRGBA->g >> 4) <<  8) |
				(((GLushort)pRGBA->b >> 4) <<  4) |
				(((GLushort)pRGBA->a >> 4)));
		}
		break;
									}
	case GL_UNSIGNED_SHORT_5_5_5_1: {
		GLushort* usArray = (GLushort*)colorArray;
		for (GLuint pixIdx = 0; pixIdx < pixCount; pixIdx++) {
			ccColor4B* pRGBA = colorArray + pixIdx;
			usArray[pixIdx] = ((((GLushort)pRGBA->r >> 3) << 11) |
				(((GLushort)pRGBA->g >> 3) <<  6) |
				(((GLushort)pRGBA->b >> 3) <<  1) |
				(((GLushort)pRGBA->a >> 7)));
		}
		break;
									}
	default:
		break;
	}
}

void CC3Texture::resizeTo( const CC3IntSize& size )
{
	m_size = size;
	m_hasMipmap = false;
}

CC3CCTexture* CC3Texture::getCCTexture()
{
	if (!m_ccTexture) 
		setCCTexture( CC3Texture2DContent::textureFromCC3Texture( this ) );
	return m_ccTexture;
}

/** Sets the CCTexture content. */
void CC3Texture::setCCTexture( CC3CCTexture* texContent )
{
	if (texContent == m_ccTexture) 
		return;

	CC_SAFE_RELEASE(m_ccTexture);
	m_ccTexture = texContent;
	CC_SAFE_RETAIN(texContent);

	cacheCCTexture2D();
}


/**
* If the class-side shouldCacheAssociatedCCTextures propery is set to YES, and a CCTexture
* with the same name as this texture does not already exist in the CCTextureCache, adds the
* CCTexture returned by the ccTexture property to the CCTextureCache.
*/
void CC3Texture::cacheCCTexture2D()
{
	if ( shouldCacheAssociatedCCTextures() )
		m_ccTexture->addToCacheWithName( getName().c_str() );
}

static bool _shouldCacheAssociatedCCTextures = false;

bool CC3Texture::shouldCacheAssociatedCCTextures()
{
	return _shouldCacheAssociatedCCTextures; 
}

void CC3Texture::setShouldCacheAssociatedCCTextures( bool shouldCache )
{
	_shouldCacheAssociatedCCTextures = shouldCache;
}


void CC3Texture::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	m_ccTexture = NULL;
	m_textureID = 0;
	m_size = CC3IntSizeMake(0, 0);
	m_coverage = CCSizeZero;
	m_pixelFormat = GL_RGBA;
	m_pixelType = GL_UNSIGNED_BYTE;
	m_hasMipmap = false;
	m_hasAlpha = false;
	m_hasPremultipliedAlpha = false;
	m_isUpsideDown = false;
	m_shouldFlipVerticallyOnLoad = defaultShouldFlipVerticallyOnLoad();
	m_shouldFlipHorizontallyOnLoad = defaultShouldFlipHorizontallyOnLoad();
	setTextureParameters( defaultTextureParameters() );	// Marks params dirty
}

void CC3Texture::populateFrom( CC3Texture* another )
{
	CCAssert(false, "CC3Texture should not be copied.");
}

CCObject* CC3Texture::copyWithZone( CCZone* zone )
{
	CC3Texture* pVal = new CC3Texture;
	pVal->init();
	pVal->populateFrom( this );

	return pVal;
}

//
//+(Class) textureClassForFile: (NSString*) filePath {
//	NSString* lcPath = filePath.lowercaseString;
//	if ([lcPath hasSuffix: @".pvr"] ||
//		[lcPath hasSuffix: @".pvr.gz"] ||
//		[lcPath hasSuffix: @".pvr.ccz"] ) {
//		return CC3PVRTexture.class;
//	}
//	return CC3Texture2D.class;
//}

/**
* Determine the correct class in this cluster for loading the specified file. If this is not
* the correct class, release this instance and instantiate and return an instance of the correct
* class. If this IS the correct class, perform normal init and load the specified file.
*/
bool CC3Texture::initFromFile( const std::string& filePath )
{
	/*Class texClz = [self.class textureClassForFile: filePath];
	if (self.class != texClz) {
	[self release];
	return [[texClz alloc] initFromFile: filePath];
	}

	if ( (self = [self init]) ) {
	if ( ![self loadFromFile: filePath] ) {
	[self release];
	return nil;
	}
	}
	return self;*/

	if ( !init() )
		return false;

	return loadFromFile( filePath );
}

bool CC3Texture::initCubeWithSideLength( GLuint sideLength, GLenum format, GLenum type )
{
	return false;
}

CC3Texture* CC3Texture::textureCubeWithSideLength( GLuint sideLength, GLenum format, GLenum type )
{
	CC3Texture* pTex = new CC3TextureCube;
	if ( pTex->initCubeWithSideLength( sideLength, format, type ) )
	{
		pTex->autorelease();
		return pTex;
	}

	CC_SAFE_DELETE( pTex );
	return pTex;
}

CC3Texture* CC3Texture::textureFromFile( const char* filePath )
{
	CC3Texture* tex = getTextureNamed( textureNameFromFilePath( filePath ) );
	if ( tex )
		return tex;

	tex = new CC3Texture2D;
	if ( tex->initFromFile( filePath ) )
	{
		tex->autorelease();
		addTexture( tex );
	}
	else
	{
		CC_SAFE_DELETE( tex );
	}

	return tex;
}

std::string CC3Texture::textureNameFromFilePath( const std::string& filePath )
{ 
	return filePath;
	// return CC3String::getFileName( filePath );
}

void CC3Texture::remove()
{
	CC3Texture::removeTexture( this );
}

void CC3Texture::addTexture( CC3Texture* texture )
{
	ensureCache();
	_textureCache->addObject( texture );
}

void CC3Texture::removeTexture( CC3Texture* texture )
{
	if ( _textureCache )
		_textureCache->removeObject( texture );
}

void CC3Texture::removeTextureNamed( const std::string& name )
{
	if ( _textureCache )
		_textureCache->removeObjectNamed( name );
}

CC3Texture* CC3Texture::getTextureNamed( const std::string& name )
{
	if (  _textureCache )
		return (CC3Texture*)_textureCache->getObjectNamed( name );

	return NULL;
}

bool CC3Texture::initWithPixelFormat( GLenum format, GLenum pixelType )
{
	if ( !init() )
		return false;

	m_pixelFormat = format;
	m_pixelType = pixelType;

	return true;
}

bool CC3Texture::initWithPixelFormat( GLenum pixelType )
{
	return initWithPixelFormat( m_pixelFormat, pixelType );
}

CC3Texture* CC3Texture::textureWithPixelFormat( GLenum format, GLenum pixelType )
{
	CC3Texture* pTexture = new CC3Texture2D;
	if ( pTexture->initWithPixelFormat( format, pixelType ) )
	{
		pTexture->autorelease();
		return pTexture; 
	}

	CC_SAFE_DELETE( pTexture );
	return pTexture;
}

GLenum CC3Texture::getPixelFormat()
{
	return m_pixelFormat;
}

GLenum CC3Texture::getPixelType()
{
	return m_pixelType;
}

bool CC3Texture::initWithSize( const CC3IntSize& size, const ccColor4B& color )
{
	return true;
}

bool CC3Texture::initWithSize( const CC3IntSize& size, GLenum format, GLenum pixelType )
{
	return true;
}

CC3Texture* CC3Texture::textureWithSize( const CC3IntSize& size, GLenum format, GLenum pixelType )
{
	CC3Texture* pTexture = new CC3Texture2D;
	if ( pTexture->initWithSize( size, format, pixelType ) )
	{
		pTexture->autorelease();
		return pTexture; 
	}

	CC_SAFE_DELETE( pTexture );
	return pTexture;
}

bool CC3Texture::initWithCCTexture( CC3CCTexture* ccTexture )
{
	return true;
}

bool CC3Texture::initCubeFromFiles( const std::string& posXFilePath, const std::string& negXFilePath, const std::string& posYFilePath, 
	const std::string& negYFilePath, const std::string& posZFilePath, const std::string& negZFilePath )
{
	return false;
}

bool CC3Texture::initCubeFromFilePattern( const std::string& aFilePathPattern )
{
	return true;
}

CC3Texture* CC3Texture::textureCubeFromFiles( const char* posXFilePath, const char* negXFilePath, const char* posYFilePath, 
	const char* negYFilePath, const char* posZFilePath, const char* negZFilePath )
{
	CC3Texture* pTex = new CC3TextureCube;
	if ( pTex->initCubeFromFiles( posXFilePath, negXFilePath, posYFilePath, negYFilePath, posZFilePath, negZFilePath ) )
	{
		pTex->autorelease();
		return pTex;
	}

	CC_SAFE_DELETE( pTex );
	return pTex;
}

CC3Texture* CC3Texture::textureCubeFromFilePattern( const std::string& aFilePathPattern )
{
	std::string texName = textureNameFromFilePath( aFilePathPattern );
	CC3Texture* pTex = getTextureNamed( texName );
	if ( pTex )
		return pTex;

	pTex = new CC3TextureCube;
	if ( pTex->initCubeFromFilePattern( aFilePathPattern ) )
	{
		pTex->autorelease();
		return pTex;
	}

	CC_SAFE_DELETE( pTex );
	return pTex;
}

bool CC3Texture::initCubeWithPixelFormat( GLenum format, GLenum type )
{
	return true;
}

CC3Texture* CC3Texture::textureCubeWithPixelFormat( GLenum format, GLenum type )
{
	CC3Texture* pTex = new CC3TextureCube;
	if ( pTex->initCubeWithPixelFormat( format, type ) )
	{
		pTex->autorelease();
		return pTex;
	}

	CC_SAFE_DELETE( pTex );
	return pTex;
}

bool CC3Texture::initCubeColoredForAxes()
{
	return true;
}

std::string CC3Texture::constructorDescription()
{
	return "CC3Texture";
}

std::string CC3Texture::cachedTexturesDescription()
{
	return "CC3Texture";
}

CC3Texture2DContent::CC3Texture2DContent()
{
	m_imageData = NULL;
}

CC3Texture2DContent::~CC3Texture2DContent()
{
	CC_SAFE_FREE( m_imageData );
}

CC3Texture2DContent* CC3Texture2DContent::textureFromCC3Texture( CC3Texture* texture )
{
	CC3Texture2DContent* pTexture = new CC3Texture2DContent;
	pTexture->initFromCC3Texture( texture );
	pTexture->autorelease();

	return pTexture;
}

void CC3Texture2DContent::initFromCC3Texture( CC3Texture* texture )
{
	m_imageData = NULL;
	m_uName = texture->getTextureID();
	m_uPixelsWide = texture->getSize().width;
	m_uPixelsHigh = texture->getSize().height;
	m_fMaxS = texture->getCoverage().width;
	m_fMaxT = texture->getCoverage().height;
	m_tContentSize = CCSizeMake((float)m_uPixelsWide * m_fMaxS, (float)m_uPixelsHigh * m_fMaxT);
	m_bHasPremultipliedAlpha = texture->hasPremultipliedAlpha();
	//		CC2_TEX_ANTIALIASED = YES;
	//		CC2_TEX_CONTENT_SCALE = 1.0;
	m_bHasMipmaps = texture->hasMipmap();
	//#endif
	m_isUpsideDown = texture->isUpsideDown();
	m_pixelGLFormat = texture->getPixelFormat();
	m_pixelGLType = texture->getPixelType();
	updatePixelFormat();
}

void CC3Texture2DContent::updatePixelFormat()
{
	m_ePixelFormat = CCTexturePixelFormatFromGLFormatAndType(m_pixelGLFormat, m_pixelGLType);
}

GLenum CC3Texture2DContent::getPixelGLFormat()
{
	return m_pixelGLFormat;
}

GLenum CC3Texture2DContent::getPixelGLType()
{
	return m_pixelGLType;
}

bool CC3Texture2DContent::isUpsideDown()
{
	return m_isUpsideDown;
}

bool CC3Texture2DContent::initFromFile( const std::string& filePath)
{
	if ( CC3STBImage::shouldUseForFileExtension( CC3String::getExtension(filePath) ) )
		return initFromSTBIFile( filePath );
	else
		return initFromOSFile( filePath );
}

bool CC3Texture2DContent::initFromSTBIFile( const std::string& filePath )
{
	if( super::init() ) 
	{
		CC3STBImage* stbImage = CC3STBImage::imageFromFile( filePath.c_str() );
		if (!stbImage) 
			return false;

		m_imageData = stbImage->extractImageData();

		m_tContentSize = CGSizeFromCC3IntSize(stbImage->getSize());
		m_uPixelsWide = stbImage->getSize().width;
		m_uPixelsHigh = stbImage->getSize().height;
		m_fMaxS = 1.0f;
		m_fMaxT = 1.0f;
		m_bHasPremultipliedAlpha = false;
		//CC2_TEX_ANTIALIASED = YES;
		//CC2_TEX_CONTENT_SCALE = 1.0;

		m_isUpsideDown = true;			// Loaded upside-down
		m_pixelGLFormat = stbImage->getPixelFormat();
		m_pixelGLType = stbImage->getPixelType();
		updatePixelFormat();

		return true;
	}

	return false;
}

bool CC3Texture2DContent::initFromOSFile( const std::string& filePath )
{
	std::string lowerCase(filePath);
	for (unsigned int i = 0; i < lowerCase.length(); ++i)
	{
		lowerCase[i] = tolower(lowerCase[i]);
	}

	CCImage::EImageFormat eImageFormat = CCImage::kFmtUnKnown;
	if (std::string::npos != lowerCase.find(".png"))
	{
		eImageFormat = CCImage::kFmtPng;
	}
	else if (std::string::npos != lowerCase.find(".jpg") || std::string::npos != lowerCase.find(".jpeg"))
	{
		eImageFormat = CCImage::kFmtJpg;
	}
	else if (std::string::npos != lowerCase.find(".tif") || std::string::npos != lowerCase.find(".tiff"))
	{
		eImageFormat = CCImage::kFmtTiff;
	}
	else if (std::string::npos != lowerCase.find(".webp"))
	{
		eImageFormat = CCImage::kFmtWebp;
	}

	CCImage* pImage = new CCImage;
	if ( !pImage->initWithImageFile( filePath.c_str(), eImageFormat ) )
	{
		pImage->release();
		return false;
	} 

	/// Calculate image data size first
	m_imageDataSize = getImageDataSize( pImage, pImage->getWidth(), pImage->getHeight() );

	bool bRet = initWithImage( pImage );
	if ( !bRet )
	{
		pImage->release();
		return false;
	}

	pImage->release();

	return true;
#if 0
#if CC3_IOS
	// Resolve an absolute path in either the application bundle resource
	// directory or the Cocos3D bundle resource directory.
	NSString* absFilePath = CC3ResolveResourceFilePath(filePath);
	LogErrorIf(!absFilePath, @"Could not locate texture file '%@' in either the application resources or the Cocos3D library resources", filePath);
	UIImage* uiImg = [UIImage imageWithContentsOfFile: absFilePath];

#if CC3_CC2_1
#if COCOS2D_VERSION < 0x010100
	return [self initWithImage: uiImg];
#else
	return [self initWithImage: uiImg resolutionType: kCCResolutionUnknown];
#endif // COCOS2D_VERSION < 0x010100

#else
	return [self initWithCGImage: uiImg.CGImage];
#endif	// CC3_CC2_1

#endif	// CC_IOS

#if CC3_OSX
	// Resolve an absolute path in either the application bundle resource
	// directory or the Cocos3D bundle resource directory.
	NSString* absFilePath = CC3ResolveResourceFilePath(filePath);
	LogErrorIf(!absFilePath, @"Could not locate texture file '%@' in either the application resources or the Cocos3D library resources", filePath);
	NSData* imgData = [NSData dataWithContentsOfFile: absFilePath];
	NSBitmapImageRep* image = [NSBitmapImageRep imageRepWithData: imgData];
	return [self initWithCGImage: image.CGImage];
#endif	// CC_OSX
#endif
}

const GLvoid* CC3Texture2DContent::getImageData()
{
	return m_imageData;
}

bool CC3Texture2DContent::initWithData(const void* data, CCTexture2DPixelFormat pixelFormat, unsigned int pixelsWide, unsigned int pixelsHigh, const CCSize& contentSize)
{
	//return super::initWithData(data, pixelFormat, pixelsWide, pixelsHigh, contentSize);

	if ( super::init() ) 
	{
		m_tContentSize = contentSize;
		m_uPixelsWide = pixelsWide;
		m_uPixelsHigh = pixelsHigh;
		m_ePixelFormat = pixelFormat;
		m_fMaxS = pixelsWide ? (contentSize.width / (float)pixelsWide) : 1.0f;
		m_fMaxT = pixelsHigh ? (contentSize.height / (float)pixelsHigh) : 1.0f;
		m_bHasPremultipliedAlpha = false;

		if ( m_imageDataSize )
		{
			m_imageData = new unsigned char[m_imageDataSize];
			memcpy( m_imageData, data, m_imageDataSize );
		}
		else
		{
			CC_SAFE_FREE( m_imageData );
		}

		m_pixelGLFormat = CC3PixelGLFormatFromCCTexturePixelFormat( pixelFormat );
		m_pixelGLType = CC3PixelGLTypeFromCCTexturePixelFormat( pixelFormat );
		m_isUpsideDown = true;
	}
	return true;
}

GLuint CC3Texture2DContent::getImageDataSize( CCImage *image, unsigned int width, unsigned int height )
{
	unsigned char*            tempData = image->getData();
	bool                      hasAlpha = image->hasAlpha();
	CCTexture2DPixelFormat    pixelFormat;
	size_t                    bpp = image->getBitsPerComponent();

	// compute pixel format
	if (hasAlpha)
	{
		pixelFormat = kCCTexture2DPixelFormat_Default;
	}
	else
	{
		if (bpp >= 8)
		{
			pixelFormat = kCCTexture2DPixelFormat_RGB888;
		}
		else 
		{
			pixelFormat = kCCTexture2DPixelFormat_RGB565;
		}

	}

	// Repack the pixel data into the right format
	unsigned int length = width * height;

	if (pixelFormat == kCCTexture2DPixelFormat_RGB565 || pixelFormat == kCCTexture2DPixelFormat_RGBA4444 || pixelFormat == kCCTexture2DPixelFormat_RGB5A1)
	{
		length = width * height * 2;
	}
	else if (pixelFormat == kCCTexture2DPixelFormat_A8)
	{
		length = width * height;
	}

	if (hasAlpha && pixelFormat == kCCTexture2DPixelFormat_RGB888)
	{
		length = width * height * 3;
	}

	/// RGBA vs RGB
	length *= ( hasAlpha ? 4 : 3 );
	return length;
}

void CC3Texture2DContent::flipHorizontally()
{
	if ( !m_imageData ) return;		// If no data, nothing to flip!

	GLuint rowCnt = (GLuint)getPixelHeight();
	GLuint colCnt = (GLuint)getPixelWidth();
	GLuint lastColIdx = colCnt - 1;
	GLuint halfColCnt = colCnt / 2;

	GLubyte* rowStart = (GLubyte*)m_imageData;
	GLuint bytesPerPixel = getBytesPerPixel();
	GLuint bytesPerRow = bytesPerPixel * colCnt;
	GLubyte* tmpPixel = new GLubyte[bytesPerPixel];

	for (GLuint rowIdx = 0; rowIdx < rowCnt; rowIdx++) {
		for (GLuint colIdx = 0; colIdx < halfColCnt; colIdx++) {
			GLubyte* firstPixel = rowStart + (bytesPerPixel * colIdx);
			GLubyte* lastPixel = rowStart + (bytesPerPixel * (lastColIdx - colIdx));
			memcpy(tmpPixel, firstPixel, bytesPerPixel);
			memcpy(firstPixel, lastPixel, bytesPerPixel);
			memcpy(lastPixel, tmpPixel, bytesPerPixel);
		}
		rowStart += bytesPerRow;
	}

	CC_SAFE_DELETE_ARRAY( tmpPixel );
}

void CC3Texture2DContent::flipVertically()
{
	if ( !m_imageData ) return;		// If no data, nothing to flip!

	CC3FlipVertically((GLubyte*)m_imageData,
		(GLuint)getPixelHeight(),
		(GLuint)getPixelWidth() * getBytesPerPixel());

	m_isUpsideDown = !m_isUpsideDown;		// Orientation has changed
}

void CC3Texture2DContent::rotateHalfCircle()
{
	if ( !m_imageData )
		return;		// If no data, nothing to rotate!

	GLuint rowCnt = (GLuint)getPixelHeight();
	GLuint lastRowIdx = rowCnt - 1;
	GLuint halfRowCnt = (rowCnt + 1) / 2;		// Use ceiling to capture any middle row: (A+B-1)/B
	GLuint colCnt = (GLuint)getPixelWidth();
	GLuint lastColIdx = colCnt - 1;

	GLubyte* pixData = (GLubyte*)m_imageData;
	GLuint bytesPerPixel = getBytesPerPixel();
	GLuint bytesPerRow = bytesPerPixel * colCnt;
	GLubyte* tmpPixel = new GLubyte[bytesPerPixel];

	for (GLuint rowIdx = 0; rowIdx < halfRowCnt; rowIdx++) {
		GLubyte* lowerRow = pixData + (bytesPerRow * rowIdx);
		GLubyte* upperRow = pixData + (bytesPerRow * (lastRowIdx - rowIdx));
		for (GLuint colIdx = 0; colIdx < colCnt; colIdx++) {
			GLubyte* firstPixel = lowerRow + (bytesPerPixel * colIdx);
			GLubyte* lastPixel = upperRow + (bytesPerPixel * (lastColIdx - colIdx));
			memcpy(tmpPixel, firstPixel, bytesPerPixel);
			memcpy(firstPixel, lastPixel, bytesPerPixel);
			memcpy(lastPixel, tmpPixel, bytesPerPixel);
		}
	}

	CC_SAFE_DELETE_ARRAY( tmpPixel );
	m_isUpsideDown = !m_isUpsideDown;		// Orientation has changed
}

void CC3Texture2DContent::resizeTo( const CC3IntSize& size )
{
	super::resizeTo(size);
}

void CC3Texture2DContent::deleteImageData()
{
	CC_SAFE_FREE( m_imageData );
}

bool CC3Texture2DContent::initWithSize( const CC3IntSize& size, const ccColor4B& color )
{
	if ( initWithSize( size, GL_RGBA, GL_UNSIGNED_BYTE ) ) 
	{
		GLuint pxCnt = size.width * size.height;
		ccColor4B* pixels = (ccColor4B*)malloc(pxCnt * sizeof(color));
		for (GLuint pxIdx = 0; pxIdx < pxCnt; pxIdx++) 
			pixels[pxIdx] = color;

		m_imageData = pixels;

		return true;
	}

	return false;
}

bool CC3Texture2DContent::initWithSize( const CC3IntSize& size, GLenum format, GLenum type )
{
	if ( super::init() ) 
	{
		resizeTo( size );

		m_bHasPremultipliedAlpha = false;
		//CC2_TEX_ANTIALIASED = YES;
		//CC2_TEX_CONTENT_SCALE = 1.0;

		m_imageData = NULL;
		m_isUpsideDown = false;		// Empty texture is not upside down!
		m_pixelGLFormat = format;
		m_pixelGLType = type;
		updatePixelFormat();

		return true;
	}

	return false;
}

CC3Texture2D::CC3Texture2D()
{
	m_ccTexture = NULL;
}

GLenum CC3Texture2D::getSamplerSemantic()
{
	return kCC3SemanticTexture2DSampler; 
}

bool CC3Texture2D::isTexture2D()
{
	return true; 
}

GLenum CC3Texture2D::getTextureTarget()
{
	return GL_TEXTURE_2D; 
}

GLenum CC3Texture2D::getInitialAttachmentFace()
{
	return GL_TEXTURE_2D; 
}

//-(Class) textureContentClass { return CC3Texture2DContent.class; }

/** If the specified texture content is new to this texture, the contained content is updated. */
void CC3Texture2D::bindTextureContent( CC3CCTexture* texContent, GLenum target )
{
	super::bindTextureContent( texContent, target );

	if (texContent == m_ccTexture) 
		return;

	if ( m_ccTexture ) 
		m_ccTexture->setName( 0 );			// Clear ID of existing so it won't delete GL texture when deallocated

	// Align texture ID's and delete the texture data from main memory

	if ( texContent )
	{
		texContent->setName( getTextureID() );
		texContent->deleteImageData();
	}

	setCCTexture( texContent );		// Keep track of the 2D texture content
}

void CC3Texture2D::resizeTo( const CC3IntSize& size )
{
	if ( CC3IntSizesAreEqual(size, m_size) ) 
		return;

	super::resizeTo( size );

	bindTextureContent( getSizedContent(), getTextureTarget() );
}

GLuint CC3Texture2D::getTextureUnitFromVisitor( CC3NodeDrawingVisitor* visitor )
{
	return visitor->getCurrent2DTextureUnit();
}

void CC3Texture2D::incrementTextureUnitInVisitor( CC3NodeDrawingVisitor* visitor )
{
	visitor->increment2DTextureUnit();
}

void CC3Texture2D::replacePixels( const CC3Viewport& rect, ccColor4B* colorArray )
{
	super::replacePixels( rect, getTextureTarget(), colorArray );
}

static bool _defaultShouldFlip2DVerticallyOnLoad = true;

bool CC3Texture2D::defaultShouldFlipVerticallyOnLoad()
{
	return _defaultShouldFlip2DVerticallyOnLoad; 
}

void CC3Texture2D::setDefaultShouldFlipVerticallyOnLoad( bool shouldFlip )
{
	_defaultShouldFlip2DVerticallyOnLoad = shouldFlip;
}

static bool _defaultShouldFlip2DHorizontallyOnLoad = false;

bool CC3Texture2D::defaultShouldFlipHorizontallyOnLoad()
{
	return _defaultShouldFlip2DHorizontallyOnLoad; 
}

void CC3Texture2D::setDefaultShouldFlipHorizontallyOnLoad( bool shouldFlip )
{
	_defaultShouldFlip2DHorizontallyOnLoad = shouldFlip;
}

bool CC3Texture2D::initWithPixelFormat( GLenum format, GLenum type )
{
	if ( init() ) 
	{
		setShouldFlipVerticallyOnLoad( false );	// Nothing to flip
		m_pixelFormat = format;
		m_pixelType = type;

		return true;
	}

	return false;
}

bool CC3Texture2D::initWithSize( const CC3IntSize& size, GLenum format, GLenum type )
{
	if ( initWithPixelFormat( format, type ) )
	{
		resizeTo( size );
		return true;
	}

	return false;
}

bool CC3Texture2D::initWithSize( const CC3IntSize& size, const ccColor4B& color )
{
	if ( init() ) 
	{
		bindTextureOfColor( color, size, getTextureTarget() );
		return true;
	}

	return false;
}

bool CC3Texture2D::initWithCCTexture( CC3CCTexture* ccTexture )
{
	if ( init() ) 
	{
		m_ccTexture = ccTexture;
		CC_SAFE_RETAIN( ccTexture );
		m_textureID = ccTexture->getName();
		m_size = CC3IntSizeMake( (GLint)ccTexture->getPixelWidth(), (GLint)ccTexture->getPixelHeight());
		m_coverage = CCSizeMake(ccTexture->getMaxS(), ccTexture->getMaxT());
		m_pixelFormat = ccTexture->getPixelGLFormat();
		m_pixelType = ccTexture->getPixelGLType();
		m_hasMipmap = ccTexture->hasMipmap();
		m_hasAlpha = ccTexture->hasAlpha();
		m_hasPremultipliedAlpha = ccTexture->hasPremultipliedAlpha();
		m_isUpsideDown = ccTexture->isUpsideDown();
		m_shouldFlipVerticallyOnLoad = false;
		m_shouldFlipHorizontallyOnLoad = false;
		setTextureParameters( defaultTextureParameters() );	// Marks params dirty

		return true;
	}

	return false;
}

CC3IntSize CC3Texture::getSize()
{
	return m_size;
}

bool CC3Texture::hasMipmap()
{
	return m_hasMipmap;
}

CC3TextureUnitTexture::CC3TextureUnitTexture()
{
	m_pTexture = NULL;
	m_pTextureUnit = NULL;
}

CC3TextureUnitTexture::~CC3TextureUnitTexture()
{
	CC_SAFE_RELEASE( m_pTexture );
	CC_SAFE_RELEASE( m_pTextureUnit );
}


CC3TextureUnitTexture* CC3TextureUnitTexture::textureWithTexture( CC3Texture* texture )
{
	CC3TextureUnitTexture* pTexture = new CC3TextureUnitTexture;
	pTexture->initWithTexture( texture );
	pTexture->autorelease();

	return pTexture;
}

CC3TextureUnit* CC3TextureUnitTexture::getTextureUnit()
{
	return m_pTextureUnit;
}

CC3Texture* CC3TextureUnitTexture::getTexture()
{
	return m_pTexture;
}

void CC3TextureUnitTexture::setTexture( CC3Texture* texture )
{
	if (texture == m_pTexture) 
		return;

	CC_SAFE_RELEASE( m_pTexture );
	m_pTexture = texture;
	CC_SAFE_RETAIN( texture );

	if (m_sName.empty() && texture ) 
		setName( texture->getName() );
}

void CC3TextureUnitTexture::setTextureUnit( CC3TextureUnit* textureUnit )
{
	if (textureUnit == m_pTextureUnit) 
		return;

	CC_SAFE_RELEASE( m_pTextureUnit );
	m_pTextureUnit = textureUnit;
	CC_SAFE_RETAIN( textureUnit );
}

GLuint CC3TextureUnitTexture::getTextureID()
{
	if ( m_pTexture )
		return m_pTexture->getTextureID(); 

	return -1;
}

CC3IntSize CC3TextureUnitTexture::getSize()
{
	if ( m_pTexture )
		return m_pTexture->getSize(); 

	return kCC3IntSizeZero;
}

bool CC3TextureUnitTexture::isPOTWidth()
{
	if ( m_pTexture )
		return m_pTexture->isPOTWidth(); 

	return false;
}

bool CC3TextureUnitTexture::isPOTHeight()
{
	if ( m_pTexture )
		return m_pTexture->isPOTHeight(); 

	return false;
}

bool CC3TextureUnitTexture::isPOT()
{
	if ( m_pTexture )
		return m_pTexture->isPOT(); 

	return false;
}

GLenum CC3TextureUnitTexture::getSamplerSemantic()
{
	if ( m_pTexture )
		return m_pTexture->getSamplerSemantic();

	return 0;
}

bool CC3TextureUnitTexture::isTexture2D()
{
	if ( m_pTexture )
		return m_pTexture->isTexture2D(); 

	return false;
}

bool CC3TextureUnitTexture::isTextureCube()
{
	if ( m_pTexture )
		return m_pTexture->isTextureCube(); 

	return false;
}

CCSize CC3TextureUnitTexture::getCoverage()
{
	if ( m_pTexture )
		return m_pTexture->getCoverage();

	return CCSizeZero;
}

GLenum CC3TextureUnitTexture::getPsixelFormat()
{
	if ( m_pTexture )
		return m_pTexture->getPixelFormat();

	return 0;
}

GLenum CC3TextureUnitTexture::getPixelType()
{
	if ( m_pTexture )
		return m_pTexture->getPixelType(); 

	return 0;
}

bool CC3TextureUnitTexture::hasAlpha()
{
	if ( m_pTexture )
		return m_pTexture->hasAlpha();

	return false;
}

void CC3TextureUnitTexture::setHasAlpha( bool hasAlpha )
{
	if ( m_pTexture )
		m_pTexture->setHasAlpha( hasAlpha ); 
}

bool CC3TextureUnitTexture::hasPremultipliedAlpha()
{
	if ( m_pTexture )
		return m_pTexture->hasPremultipliedAlpha();

	return false;
}

void CC3TextureUnitTexture::setHasPremultipliedAlpha( bool hasPremultipliedAlpha )
{
	if ( m_pTexture )
		m_pTexture->setHasPremultipliedAlpha( hasPremultipliedAlpha );
}

bool CC3TextureUnitTexture::isUpsideDown()
{
	if ( m_pTexture )
		return m_pTexture->isUpsideDown(); 

	return false;
}

void CC3TextureUnitTexture::setIsUpsideDown( bool isUpsideDown )
{
	if ( m_pTexture )
		m_pTexture->setIsUpsideDown( isUpsideDown ); 
}

GLenum CC3TextureUnitTexture::getTextureTarget()
{
	if ( m_pTexture )
		return m_pTexture->getTextureTarget(); 

	return 0;
}

GLenum CC3TextureUnitTexture::getInitialAttachmentFace()
{
	if ( m_pTexture )
		return m_pTexture->getInitialAttachmentFace();

	return 0;
}

bool CC3TextureUnitTexture::shouldFlipVerticallyOnLoad()
{
	if ( m_pTexture )
		return m_pTexture->shouldFlipVerticallyOnLoad();

	return false;
}

void CC3TextureUnitTexture::setShouldFlipVerticallyOnLoad( bool shouldFlipVerticallyOnLoad )
{
	if ( m_pTexture )
		m_pTexture->setShouldFlipVerticallyOnLoad( shouldFlipVerticallyOnLoad );
}

bool CC3TextureUnitTexture::shouldFlipHorizontallyOnLoad()
{
	if ( m_pTexture )
		return m_pTexture->shouldFlipHorizontallyOnLoad();

	return false;
}

void CC3TextureUnitTexture::setShouldFlipHorizontallyOnLoad( bool shouldFlipHorizontallyOnLoad )
{
	if ( m_pTexture )
		m_pTexture->setShouldFlipHorizontallyOnLoad( shouldFlipHorizontallyOnLoad );
}

bool CC3TextureUnitTexture::hasMipmap()
{
	if ( m_pTexture )
		return m_pTexture->hasMipmap();

	return false;
}

void CC3TextureUnitTexture::generateMipmap()
{
	if ( m_pTexture )
		m_pTexture->generateMipmap(); 
}

GLenum CC3TextureUnitTexture::getMinifyingFunction()
{
	if ( m_pTexture )
		return m_pTexture->getMinifyingFunction(); 

	return 0;
}

void CC3TextureUnitTexture::setMinifyingFunction( GLenum minifyingFunction )
{
	if ( m_pTexture )
		m_pTexture->setMinifyingFunction( minifyingFunction );
}

GLenum CC3TextureUnitTexture::getMagnifyingFunction()
{
	if ( m_pTexture )
		return m_pTexture->getMagnifyingFunction();

	return 0;
}

void CC3TextureUnitTexture::setMagnifyingFunction( GLenum magnifyingFunction )
{
	if ( m_pTexture )
		m_pTexture->setMagnifyingFunction( magnifyingFunction );
}

GLenum CC3TextureUnitTexture::getHorizontalWrappingFunction()
{
	if ( m_pTexture )
		return m_pTexture->getHorizontalWrappingFunction();

	return 0;
}

void CC3TextureUnitTexture::setHorizontalWrappingFunction( GLenum horizontalWrappingFunction )
{
	if ( m_pTexture )
		m_pTexture->setHorizontalWrappingFunction( horizontalWrappingFunction );
}

GLenum CC3TextureUnitTexture::getVerticalWrappingFunction()
{
	if ( m_pTexture )
		return m_pTexture->getVerticalWrappingFunction();

	return 0;
}

void CC3TextureUnitTexture::setVerticalWrappingFunction( GLenum verticalWrappingFunction )
{
	if ( m_pTexture )
		m_pTexture->setVerticalWrappingFunction( verticalWrappingFunction );
}

ccTexParams CC3TextureUnitTexture::getTextureParameters()
{
	if ( m_pTexture )
		return m_pTexture->getTextureParameters();

	ccTexParams params = {GL_LINEAR,GL_LINEAR,GL_REPEAT,GL_REPEAT};
	return params;
}

void CC3TextureUnitTexture::setTextureParameters( const ccTexParams& textureParameters )
{
	if ( m_pTexture )
		m_pTexture->setTextureParameters( textureParameters );
}


void CC3TextureUnitTexture::replacePixels( const CC3Viewport& rect, GLenum target, ccColor4B* colorArray )
{
	if ( m_pTexture )
		m_pTexture->replacePixels( rect, target, colorArray );
}

void CC3TextureUnitTexture::resizeTo( const CC3IntSize& size )
{
	if ( m_pTexture )
		m_pTexture->resizeTo( size ); 
}

CC3Vector CC3TextureUnitTexture::getLightDirection()
{
	return m_pTextureUnit ? m_pTextureUnit->getLightDirection() : CC3Vector::kCC3VectorZero; 
}

void CC3TextureUnitTexture::setLightDirection( const CC3Vector& aDirection )
{
	if ( m_pTextureUnit ) 
		m_pTextureUnit->setLightDirection( aDirection );
}

bool CC3TextureUnitTexture::isBumpMap()
{
	return (m_pTextureUnit && m_pTextureUnit->isBumpMap()); 
}

void CC3TextureUnitTexture::bindTextureParametersAt( GLuint tuIdx, CC3OpenGL* gl )
{
	if ( m_pTexture )
		m_pTexture->bindTextureParametersAt( tuIdx, gl );
}

/** Binds texture unit environment to the GL engine. */
void CC3TextureUnitTexture::bindTextureEnvironmentWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	if (m_pTextureUnit)
		m_pTextureUnit->bindWithVisitor( visitor );
	else
		super::bindTextureEnvironmentWithVisitor( visitor );
}

GLuint CC3TextureUnitTexture::getTextureUnitFromVisitor( CC3NodeDrawingVisitor* visitor )
{
	if ( m_pTexture )
		return m_pTexture->getTextureUnitFromVisitor( visitor );

	return 0;
}

void CC3TextureUnitTexture::incrementTextureUnitInVisitor( CC3NodeDrawingVisitor* visitor )
{
	if ( m_pTexture )
		m_pTexture->incrementTextureUnitInVisitor( visitor );
}

void CC3TextureUnitTexture::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	m_pTexture = NULL;
	m_pTextureUnit = NULL;
}

void CC3TextureUnitTexture::initWithTexture( CC3Texture* texture )
{
	init();
	setTexture( texture );
}

bool CC3TextureUnitTexture::initFromFile( const std::string& filePath )
{
	CC3Texture* pTexture = CC3Texture::textureFromFile( filePath.c_str() );
	if ( !pTexture )
		return false;

	initWithTexture( pTexture );

	return true;
}

CC3TextureUnitTexture* CC3TextureUnitTexture::textureFromFile( const std::string& filePath )
{
	CC3TextureUnitTexture* pTexture = new CC3TextureUnitTexture;
	if ( pTexture->initFromFile( filePath ) )
	{
		pTexture->autorelease();
		return pTexture;
	}

	CC_SAFE_DELETE( pTexture );
	return pTexture;
}

bool CC3TextureUnitTexture::initWithPixelFormat( GLenum format, GLenum type )
{
	CC3Texture* pTexture = CC3Texture::textureWithPixelFormat( format, type );
	if ( !pTexture )
		return false;

	initWithTexture( pTexture );

	return true;
}

CC3TextureUnitTexture* CC3TextureUnitTexture::textureWithPixelFormat( GLenum format, GLenum type )
{
	CC3TextureUnitTexture* pTex = new CC3TextureUnitTexture;
	if ( pTex->initCubeWithPixelFormat( format, type )	)
	{
		pTex->autorelease();
		return pTex;
	}

	CC_SAFE_DELETE( pTex );
	return pTex;
}

bool CC3TextureUnitTexture::initWithSize( const CC3IntSize& size, GLenum format, GLenum type )
{
	CC3Texture* pTexture = CC3Texture::textureWithSize( size, format, type );
	if ( !pTexture )
		return false;

	initWithTexture( pTexture );

	return true;
}

CC3TextureUnitTexture* CC3TextureUnitTexture::textureWithSize( const CC3IntSize& size, GLenum format, GLenum type )
{
	CC3TextureUnitTexture* pTex = new CC3TextureUnitTexture;
	if ( pTex->initWithSize( size, format, type )	)
	{
		pTex->autorelease();
		return pTex;
	}

	CC_SAFE_DELETE( pTex );
	return pTex;
}

bool CC3TextureUnitTexture::initCubeFromFiles( const std::string& posXFilePath, const std::string& negXFilePath, 
	const std::string& posYFilePath, const std::string& negYFilePath, 
	const std::string& posZFilePath, const std::string& negZFilePath )
{

	CC3Texture* pTexture = CC3Texture::textureCubeFromFiles( posXFilePath.c_str(), negXFilePath.c_str(), 
		posYFilePath.c_str(), negYFilePath.c_str(), posZFilePath.c_str(), negZFilePath.c_str() );
	if ( !pTexture )
		return false;

	initWithTexture( pTexture );

	return true;
}

CC3TextureUnitTexture* CC3TextureUnitTexture::textureCubeFromFiles( const std::string& posXFilePath, const std::string& negXFilePath, 
	const std::string& posYFilePath, const std::string& negYFilePath, 
	const std::string& posZFilePath, const std::string& negZFilePath )
{
	CC3TextureUnitTexture* pTex = new CC3TextureUnitTexture;
	if ( pTex->initCubeFromFiles( posXFilePath, negXFilePath, posYFilePath, negYFilePath, posZFilePath, negZFilePath )	)
	{
		pTex->autorelease();
		return pTex;
	}

	CC_SAFE_DELETE( pTex );
	return pTex;
}

bool CC3TextureUnitTexture::initCubeFromFilePattern( const std::string& aFilePathPattern )
{
	CC3Texture* pTexture = CC3Texture::textureCubeFromFilePattern( aFilePathPattern );
	if ( !pTexture )
		return false;

	initWithTexture( pTexture );

	return true;
}

CC3TextureUnitTexture* CC3TextureUnitTexture::textureCubeFromFilePattern( const std::string& aFilePathPattern )
{
	CC3TextureUnitTexture* pTex = new CC3TextureUnitTexture;
	if ( pTex->initCubeFromFilePattern( aFilePathPattern ) )
	{
		pTex->autorelease();
		return pTex;
	}

	CC_SAFE_DELETE( pTex );
	return pTex;
}

bool CC3TextureUnitTexture::initCubeWithPixelFormat( GLenum format, GLenum type )
{
	CC3Texture* pTexture = CC3Texture::textureCubeWithPixelFormat( format, type );
	if ( !pTexture )
		return false;

	initWithTexture( pTexture );

	return true;
}

CC3TextureUnitTexture* CC3TextureUnitTexture::textureCubeWithPixelFormat( GLenum format, GLenum type )
{
	CC3TextureUnitTexture* pTex = new CC3TextureUnitTexture;
	if ( pTex->initCubeWithPixelFormat( format, type ) )
	{
		pTex->autorelease();
		return pTex;
	}

	CC_SAFE_DELETE( pTex );
	return pTex;
}

bool CC3TextureUnitTexture::initCubeWithSize( const CC3IntSize& size, GLenum format, GLenum type )
{
	CC3Texture* pTexture = CC3Texture::textureCubeWithSideLength( size.width, format, type );
	if ( !pTexture )
		return false;

	initWithTexture( pTexture );

	return true;
}

CC3TextureUnitTexture* CC3TextureUnitTexture::textureCubeWithSize( const CC3IntSize& size, GLenum format, GLenum type )
{
	CC3TextureUnitTexture* pTex = new CC3TextureUnitTexture;
	if ( pTex->initCubeWithSize( size, format, type ) )
	{
		pTex->autorelease();
		return pTex;
	}

	CC_SAFE_DELETE( pTex );
	return pTex;
}

/** Don't invoke super, because normal textures are not copyable */
void CC3TextureUnitTexture::populateFrom( CC3TextureUnitTexture* another )
{
	copyUserDataFrom( another );					// From CC3Identifiable

	setTexture( another->getTexture() );						// Shared across copies

	if ( another->getTextureUnit() )
		setTextureUnit( (CC3TextureUnit*)another->getTextureUnit()->copy()->autorelease() );
}

CCObject* CC3TextureUnitTexture::copyWithZone( CCZone* zone )
{
	CC3TextureUnitTexture* pVal = new CC3TextureUnitTexture;
	pVal->init();
	pVal->populateFrom( this );

	return pVal;
}

std::string CC3TextureUnitTexture::fullDescription()
{
	/*if ( _texture )
		return stringWithFormat( "CC3TextureUnitTexture with texture %s in %s", _texture->fullDescription().c_str()
		, _textureUnit ? _textureUnit->fullDescription().c_str() : "" );*/
	return "CC3TextureUnitTexture";
}

void CC3CCTexture::resizeTo( const CC3IntSize& size )
{
	deleteImageData();

	m_tContentSize = CGSizeFromCC3IntSize(size);
	m_uPixelsWide = size.width;
	m_uPixelsHigh = size.height;
	m_fMaxS = 1.0f;
	m_fMaxT = 1.0f;
}

void CC3CCTexture::addToCacheWithName( const char* texName )
{

}

bool CC3CCTexture::initFromFile( const std::string& file )
{
	return false;
}

bool CC3CCTexture::init()
{
	return true;
}

GLuint CC3CCTexture::getPixelGLFormat()
{
	return CC3PixelGLFormatFromCCTexturePixelFormat(getPixelFormat());
}

GLenum CC3CCTexture::getPixelGLType()
{
	return CC3PixelGLTypeFromCCTexturePixelFormat(getPixelFormat());
}

GLuint CC3CCTexture::getBytesPerPixel()
{
	switch (getPixelGLFormat()) 
	{
	case GL_RGBA: 
		{
			switch (getPixelGLType()) 
			{
			case GL_UNSIGNED_BYTE:
				return 4;
			case GL_UNSIGNED_SHORT_4_4_4_4:
			case GL_UNSIGNED_SHORT_5_5_5_1:
				return 2;
			default:
				break;
			}
			break;
		}

	case GL_RGB: 
		{
			switch (getPixelGLType()) 
			{
			case GL_UNSIGNED_BYTE:
				return 3;
			case GL_UNSIGNED_SHORT_5_6_5:
				return 2;
			default:
				break;
			}
			break;
		}

	case GL_LUMINANCE_ALPHA:
		return 2;

	case GL_LUMINANCE:
	case GL_ALPHA:
		return 1;

	case GL_DEPTH_COMPONENT: 
		{
			switch (getPixelGLType()) 
			{
			case GL_UNSIGNED_INT:
				return 4;
			case GL_UNSIGNED_SHORT:
				return 2;
			default:
				break;
			}
			break;
		}

	case GL_DEPTH_STENCIL:
		return 4;

	default:
		break;
	}
	CCAssert( false, "CC3CCTexture encountered unexpected combination of pixel format type" );
	return 0;
}

bool CC3CCTexture::hasAlpha()
{
	switch (getPixelGLFormat()) 
	{
	case GL_RGBA:
	case GL_LUMINANCE_ALPHA:
	case GL_ALPHA:
		return true;
	default:
		return false;
	}
}

bool CC3CCTexture::isUpsideDown()
{
	return true;
}

const GLvoid* CC3CCTexture::getImageData()
{
	return NULL;
}

void CC3CCTexture::deleteImageData()
{

}

void CC3CCTexture::setName( GLuint name )
{
	m_uName = name;
}

bool CC3CCTexture::hasMipmap()
{
	return m_bHasMipmaps;
}

GLuint CC3CCTexture::getPixelWidth()
{
	return m_uPixelsWide;
}

GLuint CC3CCTexture::getPixelHeight()
{
	return m_uPixelsHigh;
}

void CC3CCTexture::flipHorizontally()
{

}

void CC3CCTexture::flipVertically()
{

}

void CC3CCTexture::rotateHalfCircle()
{

}

CCTexturePixelFormat CCTexturePixelFormatFromGLFormatAndType(GLenum pixelFormat, GLenum pixelType) 
{
	switch (pixelFormat) 
	{
	case GL_RGBA: 
		{
			switch (pixelType) 
			{
			case GL_UNSIGNED_BYTE:
				return CCTexturePixelFormat_RGBA8888;
			case GL_UNSIGNED_SHORT_4_4_4_4:
				return CCTexturePixelFormat_RGBA4444;
			case GL_UNSIGNED_SHORT_5_5_5_1:
				return CCTexturePixelFormat_RGB5A1;
			default:
				return CCTexturePixelFormat_Default;
			}
		}

	case GL_RGB: 
		{
			switch (pixelType)
			{
			case GL_UNSIGNED_BYTE:
				return CCTexturePixelFormat_RGB888;
			case GL_UNSIGNED_SHORT_5_6_5:
				return CCTexturePixelFormat_RGB565;
			default:
				return CCTexturePixelFormat_Default;
			}
		}

	case GL_LUMINANCE_ALPHA:
		return CCTexturePixelFormat_AI88;

	case GL_LUMINANCE:
	case GL_ALPHA:
		return CCTexturePixelFormat_A8;

	default:
		return CCTexturePixelFormat_Default;
	}
}


GLenum CC3PixelGLFormatFromCCTexturePixelFormat(CCTexturePixelFormat pixelFormat)
{
	switch(pixelFormat) 
	{
	case CCTexturePixelFormat_RGBA8888: return GL_RGBA;
	case CCTexturePixelFormat_RGBA4444:	return GL_RGBA;
	case CCTexturePixelFormat_RGB5A1:	return GL_RGBA;
	case CCTexturePixelFormat_RGB565:	return GL_RGB;
	case CCTexturePixelFormat_RGB888:	return GL_RGB;
	case CCTexturePixelFormat_AI88:		return GL_LUMINANCE_ALPHA;
	case CCTexturePixelFormat_A8:		return GL_ALPHA;
	default:
		CCAssert(false, "Could not map OpenGL texel format from unexpected CCTexturePixelFormat %lu");
		return GL_ZERO;
	}
}

GLenum CC3PixelGLTypeFromCCTexturePixelFormat(CCTexturePixelFormat pixelFormat)
{
	switch(pixelFormat) 
	{
	case CCTexturePixelFormat_RGBA8888: return GL_UNSIGNED_BYTE;
	case CCTexturePixelFormat_RGBA4444:	return GL_UNSIGNED_SHORT_4_4_4_4;
	case CCTexturePixelFormat_RGB5A1:	return GL_UNSIGNED_SHORT_5_5_5_1;
	case CCTexturePixelFormat_RGB565:	return GL_UNSIGNED_SHORT_5_6_5;
	case CCTexturePixelFormat_RGB888:	return GL_UNSIGNED_BYTE;
	case CCTexturePixelFormat_AI88:		return GL_UNSIGNED_BYTE;
	case CCTexturePixelFormat_A8:		return GL_UNSIGNED_BYTE;
	default:
		CCAssert(false, "Could not map OpenGL texel type from unexpected CCTexturePixelFormat %lu"/*, (unsigned long)pixelFormat*/);
		return GL_ZERO;
	}
}
//
//
//#pragma mark -
//#pragma mark CCTextureCache extension
//
//@implementation CCTextureCache (CC3)
//
//#if CC3_CC2_1
//#	define CC2_DICT_LOCK		dictLock_
//#	define CC2_TEX_DICT			textures_
//
//void addTexture: (CCTexture*) tex2D named: (NSString*) texName {
//	if ( !tex2D ) return;
//	
//	[CC2_DICT_LOCK lock];
//	[CC2_TEX_DICT setObject: tex2D forKey: texName];
//	[CC2_DICT_LOCK unlock];
//}
//
//#else	// CC2 2 and above
//#	define CC2_DICT_QUEUE		_dictQueue
//
//#if COCOS2D_VERSION < 0x020100
//#	define CC2_TEX_DICT			textures_
//#else
//#	define CC2_TEX_DICT			_textures
//#endif	// COCOS2D_VERSION < 0x020100
//
//void addTexture: (CCTexture*) tex2D named: (NSString*) texName {
//	if ( !tex2D || !texName ) return;
//	
//	dispatch_sync(CC2_DICT_QUEUE, ^{
//		if ( ![CC2_TEX_DICT objectForKey: texName] )
//			[CC2_TEX_DICT setObject: tex2D forKey: texName];
//	});
//}
//
//#endif	// CC3_CC2_1
//
//@end


void CC3TextureCube::replacePixels( const CC3Viewport& rect, GLenum faceTarget, ccColor4B* colorArray )
{
	super::replacePixels(rect, faceTarget, colorArray);
}

GLenum CC3TextureCube::getSamplerSemantic()
{
	return kCC3SemanticTextureCubeSampler; 
}

bool CC3TextureCube::isTextureCube()
{
	return true; 
}

GLenum CC3TextureCube::getTextureTarget()
{
	return GL_TEXTURE_CUBE_MAP; 
}

GLenum CC3TextureCube::getInitialAttachmentFace()
{
	return GL_TEXTURE_CUBE_MAP_POSITIVE_X; 
}

//-(Class) textureContentClass { return CC3Texture2DContent.class; }

/** Default texture parameters for cube maps are different. */
static ccTexParams _defaultCubeMapTextureParameters = { GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };

ccTexParams CC3TextureCube::defaultTextureParameters()
{
	return _defaultCubeMapTextureParameters; 
}

void CC3TextureCube::setDefaultTextureParameters( const ccTexParams& texParams )
{
	_defaultCubeMapTextureParameters = texParams; 
}

void CC3TextureCube::resizeTo( const CC3IntSize& size )
{
	if ( CC3IntSizesAreEqual(size, m_size) ) 
		return;

	super::resizeTo( size );
	CC3CCTexture* texContent = getSizedContent();
	bindTextureContent( texContent, GL_TEXTURE_CUBE_MAP_POSITIVE_X );
	bindTextureContent( texContent, GL_TEXTURE_CUBE_MAP_NEGATIVE_X );
	bindTextureContent( texContent, GL_TEXTURE_CUBE_MAP_POSITIVE_Y );
	bindTextureContent( texContent, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y );
	bindTextureContent( texContent, GL_TEXTURE_CUBE_MAP_POSITIVE_Z );
	bindTextureContent( texContent, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z );
}

bool CC3TextureCube::loadFromFile( const std::string& filePath )
{
	CCAssert( false, "CC3TextureCube is used to load six cube textures. It cannot load a single texture." );
	return false;
}

bool CC3TextureCube::loadCubeFace( GLenum faceTarget, const std::string& filePath )
{
	return loadTarget( faceTarget, filePath );
}

bool CC3TextureCube::loadFromFiles( const std::string& posXFilePath, const std::string& negXFilePath, 
	const std::string& posYFilePath, const std::string& negYFilePath,
	const std::string& posZFilePath, const std::string& negZFilePath )
{
	bool success = true;
	
	success &= loadCubeFace( GL_TEXTURE_CUBE_MAP_POSITIVE_X, posXFilePath );
	success &= loadCubeFace( GL_TEXTURE_CUBE_MAP_NEGATIVE_X, negXFilePath );
	success &= loadCubeFace( GL_TEXTURE_CUBE_MAP_POSITIVE_Y, posYFilePath );
	success &= loadCubeFace( GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, negYFilePath );
	success &= loadCubeFace( GL_TEXTURE_CUBE_MAP_POSITIVE_Z, posZFilePath );
	success &= loadCubeFace( GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, negZFilePath );
	
	if (success && shouldGenerateMipmaps()) 
		generateMipmap();

	checkGLDebugLabel();
	return success;
}

bool CC3TextureCube::loadFromFilePattern( const std::string& aFilePathPattern )
{
	if ( m_sName.empty() ) 
		setName( textureNameFromFilePath(aFilePathPattern) );

	return loadFromFiles( CC3String::stringWithFormat( (char*)aFilePathPattern.c_str(), "PosX" ),
						  CC3String::stringWithFormat( (char*)aFilePathPattern.c_str(), "NegX" ),
						  CC3String::stringWithFormat( (char*)aFilePathPattern.c_str(), "PosY" ),
						  CC3String::stringWithFormat( (char*)aFilePathPattern.c_str(), "NegY" ),
						  CC3String::stringWithFormat( (char*)aFilePathPattern.c_str(), "PosZ" ),
						  CC3String::stringWithFormat( (char*)aFilePathPattern.c_str(), "NegZ" ) );
}

GLuint CC3TextureCube::getTextureUnitFromVisitor( CC3NodeDrawingVisitor* visitor )
{
	return visitor->getCurrentCubeTextureUnit();
}

void CC3TextureCube::incrementTextureUnitInVisitor( CC3NodeDrawingVisitor* visitor )
{
	visitor->incrementCubeTextureUnit();
}

static bool _defaultShouldFlipCubeVerticallyOnLoad = false;

bool CC3TextureCube::defaultShouldFlipVerticallyOnLoad()
{
	return _defaultShouldFlipCubeVerticallyOnLoad; 
}

void CC3TextureCube::setDefaultShouldFlipVerticallyOnLoad( bool shouldFlip )
{
	_defaultShouldFlipCubeVerticallyOnLoad = shouldFlip;
}

static bool _defaultShouldFlipCubeHorizontallyOnLoad = true;

bool CC3TextureCube::defaultShouldFlipHorizontallyOnLoad()
{
	return _defaultShouldFlipCubeHorizontallyOnLoad; 
}

void CC3TextureCube::setDefaultShouldFlipHorizontallyOnLoad( bool shouldFlip )
{
	_defaultShouldFlipCubeHorizontallyOnLoad = shouldFlip;
}

bool CC3TextureCube::initCubeFromFiles( const std::string& posXFilePath, const std::string& negXFilePath, 
	const std::string& posYFilePath, const std::string& negYFilePath,
	const std::string& posZFilePath, const std::string& negZFilePath )
{

	if ( init() )
	{
		if ( !loadFromFiles( posXFilePath, negXFilePath, posYFilePath,
			negYFilePath, posZFilePath, negZFilePath ) ) 
			return false;

		return true;
	}

	return false;
}

bool CC3TextureCube::initCubeFromFilePattern( const std::string& aFilePathPattern )
{
	if ( init() )
	{
		if ( !loadFromFilePattern( aFilePathPattern ) ) 
			return false;

		return true;
	}
	
	return false;
}

bool CC3TextureCube::initCubeWithPixelFormat( GLenum format, GLenum type )
{
	if ( init() ) 
	{
		setShouldFlipVerticallyOnLoad( false );	// Nothing to flip
		m_pixelFormat = format;
		m_pixelType = type;

		return true;
	}

	return false;
}

bool CC3TextureCube::initCubeWithSideLength( GLuint sideLength, GLenum format, GLenum type )
{
	if ( initCubeWithPixelFormat( format, type ) )
	{
		resizeTo( CC3IntSizeMake(sideLength, sideLength) );
		return true;
	}

	return false;
}

bool CC3TextureCube::initCubeColoredForAxes()
{
	if ( init() ) 
	{
		CC3IntSize texSize = CC3IntSizeMake(1, 1);
		bindTextureOfColor( CCC4BFromCCC4F(kCCC4FRed)    , texSize, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
		bindTextureOfColor( CCC4BFromCCC4F(kCCC4FCyan)   , texSize, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
		bindTextureOfColor( CCC4BFromCCC4F(kCCC4FGreen)  , texSize, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
		bindTextureOfColor( CCC4BFromCCC4F(kCCC4FMagenta), texSize, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
		bindTextureOfColor( CCC4BFromCCC4F(kCCC4FBlue)   , texSize, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
		bindTextureOfColor( CCC4BFromCCC4F(kCCC4FYellow) , texSize, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);

		return true;
	}

	return false;
}

std::string CC3TextureCube::constructorDescription()
{
	return CC3String::stringWithFormat( (char*)"CC3TextureCube textureCubeFromFilePattern: \"%s\"];", getName().c_str() );
}

NS_COCOS3D_END
