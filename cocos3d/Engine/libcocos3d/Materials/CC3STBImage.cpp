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
#include "CC3STBImage.h"

NS_COCOS3D_BEGIN

// STBI Image loading library - define STBI_HEADER_FILE_ONLY to include .c as a header file
#define STBI_HEADER_FILE_ONLY
#include "stb_image.c"

CC3STBImage::CC3STBImage()
{

}

CC3STBImage::~CC3STBImage()
{
	deleteImageData();
}

void CC3STBImage::deleteImageData()
{
	stbi_image_free(_imageData);
	_imageData = NULL;
}

GLubyte* CC3STBImage::extractImageData()
{
	GLubyte* imgData = _imageData;
	_imageData = NULL;
	return imgData;
}

CC3IntSize CC3STBImage::getSize()
{
	return _size;
}

GLenum CC3STBImage::getPixelFormat()
{
	switch (_componentCount) {
		case 4:		return GL_RGBA;
		case 3:		return GL_RGB;
		case 2:		return GL_LUMINANCE_ALPHA;
		case 1:		return GL_LUMINANCE;
		default:	return GL_ZERO;
	}
}

GLenum CC3STBImage::getPixelType()
{ 
	return GL_UNSIGNED_BYTE; 
}

bool CC3STBImage::loadFromFile( const char* filePath )
{
	deleteImageData();		// Delete any existing image data first.
	
	// Resolve an absolute path in either the application bundle resource
	// directory or the Cocos3D bundle resource directory.
	std::string absFilePath = CCFileUtils::sharedFileUtils()->fullPathForFilename( filePath );

	_imageData = stbi_load(absFilePath.c_str(),
						   (int*)&_size.width,
						   (int*)&_size.height,
						   (int*)&_componentCount, 0);
	if (!_imageData) 
	{
		CCLOGERROR("Could not load image file %s using STBI library", absFilePath.c_str() );
		return false;
	}

	return true;
}

void CC3STBImage::init()
{
	_imageData = NULL;
	_size = kCC3IntSizeZero;
	_componentCount = 0;
}

bool CC3STBImage::initFromFile( const char* aFilePath )
{
	init();
		
	return loadFromFile( aFilePath );
}

CC3STBImage* CC3STBImage::imageFromFile( const char* filePath )
{
	CC3STBImage* image = new CC3STBImage;
	image->initFromFile( filePath );
	image->autorelease();

	return image;
}


static CCSet* _useForFileExtensions = NULL;

CCSet* CC3STBImage::useForFileExtensions()
{
	if (!_useForFileExtensions) {
		_useForFileExtensions = CCSet::create();

		_useForFileExtensions->addObject( CCString::create( "ppng" ) );
		_useForFileExtensions->addObject( CCString::create( "pjpg" ) );
		_useForFileExtensions->addObject( CCString::create( "ptga" ) );
		_useForFileExtensions->addObject( CCString::create( "pbmp" ) );
		_useForFileExtensions->addObject( CCString::create( "ppsd" ) );
		_useForFileExtensions->addObject( CCString::create( "pgif" ) );
		_useForFileExtensions->addObject( CCString::create( "phdr" ) );
		_useForFileExtensions->addObject( CCString::create( "ppic" ) );

		_useForFileExtensions->retain();
	}

	return _useForFileExtensions;
}

bool CC3STBImage::shouldUseForFileExtension( const std::string& fileExtension )
{
	CCSet* pExtensions = useForFileExtensions();
	CCString *pExt;
	CCSetIterator setIter;
	for (setIter = pExtensions->begin(); setIter != pExtensions->end(); ++setIter)
	{
		pExt = (CCString *)(*setIter);
		if ( pExt->compare( fileExtension.c_str() ) == 0 ) 
			return true;
	}

	return false;
}

NS_COCOS3D_END
