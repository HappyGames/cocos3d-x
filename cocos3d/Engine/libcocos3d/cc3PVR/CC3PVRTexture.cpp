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

bool CC3PVRTexture::shouldFlipVerticallyOnLoad()
{
	return false; 
}

void CC3PVRTexture::setShouldFlipVerticallyOnLoad( bool _shouldFlipVerticallyOnLoad )
{

}

bool CC3PVRTexture::shouldFlipHorizontallyOnLoad()
{
	return false; 
}

void CC3PVRTexture::setShouldFlipHorizontallyOnLoad( bool _shouldFlipHorizontallyOnLoad )
{

}

GLenum CC3PVRTexture::getSamplerSemantic()
{
	return isTextureCube() ? kCC3SemanticTextureCubeSampler : kCC3SemanticTexture2DSampler;
}

bool CC3PVRTexture::isTexture2D()
{
	return !isTextureCube(); 
}

GLenum CC3PVRTexture::getTextureTarget()
{
	return isTextureCube() ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D; 
}

GLenum CC3PVRTexture::getInitialAttachmentFace()
{
	return isTextureCube() ? GL_TEXTURE_CUBE_MAP_POSITIVE_X : GL_TEXTURE_2D;
}

//-(Class) textureContentClass { return CC3PVRTextureContent.class; }

void CC3PVRTexture::bindTextureContent( CC3PVRTextureContent* texContent, GLenum target )
{
	if (!texContent) 
		return;
	
	deleteGLTexture();		// Delete any existing texture in the GL engine
	
	m_textureID = texContent->getTextureID();
	m_size = texContent->getSize();
	m_hasMipmap = texContent->hasMipmap();
	m_hasPremultipliedAlpha = texContent->hasPremultipliedAlpha();
	_isTextureCube = texContent->isTextureCube();
	m_coverage = CCSizeMake(1.0f, 1.0f);				// PVR textures are always POT
	m_pixelFormat = texContent->getPixelFormat();
	m_pixelType = texContent->getPixelType();
	
	//LogTrace(@"Bound PVR texture ID %u", _textureID);
	
	// Update the texture parameters depending on whether the PVR file is 2D or cube-map.
	setTextureParameters( isTextureCube()
								? CC3TextureCube::defaultTextureParameters()
								: CC3Texture2D::defaultTextureParameters() );
}

/** Replacing pixels not supported in compressed PVR textures. */
void CC3PVRTexture::replacePixels( const CC3Viewport& rect, GLenum target, ccColor4B* colorArray )
{
	CCAssert( false, "replacePixels:inTarget:withContent: is not implemented" );
	//CC3AssertUnimplemented(@"replacePixels:inTarget:withContent:");
}

GLuint CC3PVRTexture::getTextureUnitFromVisitor( CC3NodeDrawingVisitor* visitor )
{
	return isTextureCube() ? visitor->getCurrentCubeTextureUnit() : visitor->getCurrent2DTextureUnit();
}

void CC3PVRTexture::incrementTextureUnitInVisitor( CC3NodeDrawingVisitor* visitor )
{
	if ( isTextureCube() )
		visitor->incrementCubeTextureUnit();
	else
		visitor->increment2DTextureUnit();
}

bool CC3PVRTextureContent::isTexture2D()
{
	return !isTextureCube(); 
}


#if 0
#if CC3_IOS

-(id) initFromFile: (NSString*) filePath {
	if ( (self = [super init]) ) {
		
		// Resolve an absolute path in either the application bundle resource
		// directory or the Cocos3D bundle resource directory.
		// Split the path into directory and file names, sset the PVR read path
		// to the directory and pass the unqualified file name to the parser.
		NSString* absFilePath = CC3ResolveResourceFilePath(filePath);
		LogErrorIf(!absFilePath, @"Could not locate texture file '%@' in either the application resources or the Cocos3D library resources", filePath);
		NSString* fileName = absFilePath.lastPathComponent;
		NSString* dirName = absFilePath.stringByDeletingLastPathComponent;
		
		CPVRTResourceFile::SetReadPath([dirName stringByAppendingString: @"/"].UTF8String);
		
		PVRTextureHeaderV3 pvrHeader;
		BOOL wasLoaded = PVRTTextureLoadFromPVR(fileName.UTF8String,
												&_textureID,
												&pvrHeader) == PVR_SUCCESS;
		if ( !wasLoaded ) {
			LogError(@"Could not load texture %@.", absFilePath);
			[self release];
			return nil;
		}
		_size = CC3IntSizeMake(pvrHeader.u32Width, pvrHeader.u32Height);
		_hasMipmap = (pvrHeader.u32MIPMapCount > 1);
		_isTextureCube = (pvrHeader.u32NumFaces > 1);
		_hasPremultipliedAlpha = ((pvrHeader.u32Flags & PVRTEX3_PREMULTIPLIED) != 0);
		_pixelFormat = GL_ZERO;		// Unknown - could query from GL if needed
		_pixelType = GL_ZERO;		// Unknown - could query from GL if needed
	}
	return self;
}

#else

-(id) initFromFile: (NSString*) filePath {
	LogError(@"Could not load texture %@ because PVR files are not supported on this platform.", filePath);
	return nil;
}

#endif	// CC3_IOS

#endif

NS_COCOS3D_END
