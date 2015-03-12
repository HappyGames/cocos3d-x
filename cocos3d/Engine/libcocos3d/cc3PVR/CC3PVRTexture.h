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
#ifndef _CC3_PVR_TEXTURE_H_
#define _CC3_PVR_TEXTURE_H_
#include "../Materials/CC3Texture.h"
#include "textures/CCTexturePVR.h"

NS_COCOS3D_BEGIN

/** 
 * The representation of a PVR texture that has been loaded into the GL engine.
 *
 * This class is used for all 2D and cube-map textures loaded from a PVR file type.
 *
 * This class is part of a class-cluster under the parent CC3Texture class. Although you can
 * invoke an instance creation method on this class directly, you will more commonly invoke
 * them on the CC3Texture class instead. The creation and initialization methods will ensure
 * that the correct subclass for the texture type, and in some cases, the texture file type,
 * is created and returned. Because of this class-cluster structure, be aware that the class
 * of the instance returned by an instance creation or initialization method may be different
 * than the receiver of that method.
 */
class CC3PVRTextureContent;
class CC3PVRTexture : public CC3Texture 
{
public:
	/**
	 * PVR textures cannot be flipped after loading. This property is overridden so
	 * that changes are ignored, and to always return NO.
	 */
	bool						shouldFlipVerticallyOnLoad();
	void						setShouldFlipVerticallyOnLoad( bool shouldFlip );

	/**
	 * PVR textures cannot be flipped after loading. This property is overridden so
	 * that changes are ignored, and to always return NO.
	 */
	bool						shouldFlipHorizontallyOnLoad();
	void						setShouldFlipHorizontallyOnLoad( bool shouldFlip );

	GLenum						getSamplerSemantic();
	bool						isTexture2D();
	GLenum						getTextureTarget();
	GLenum						getInitialAttachmentFace();
	void						bindTextureContent( CC3PVRTextureContent* texContent, GLenum target );
	/** Replacing pixels not supported in compressed PVR textures. */
	void						replacePixels( const CC3Viewport& rect, GLenum target, ccColor4B* colorArray );
	GLuint						getTextureUnitFromVisitor( CC3NodeDrawingVisitor* visitor );
	void						incrementTextureUnitInVisitor( CC3NodeDrawingVisitor* visitor );

protected:
	bool						_isTextureCube : 1;
};

/**
 * A helper class used by the CC3PVRTexture class cluster during the loading of a
 * texture from a PVR file using the PowerVR library.
 */
class CC3PVRTextureContent : public CCObject 
{
public:
	/** The texture ID used to identify this texture to the GL engine. */
	GLuint						getTextureID();

	/** The size of this texture in pixels. */
	CC3IntSize					getSize();

	/**
	 * Returns the pixel format of the texture.
	 *
	 * See the pixelFormat property of CC3Texture for the range of possible values.
	 */
	GLenum						getPixelFormat();

	/**
	 * Returns the pixel data type.
	 *
	 * Possible values depend on the value of the pixelFormat property. See the pixelType
	 * property of CC3Texture for the range of possible values.
	 */
	GLenum						getPixelType();

	/** Returns whether this texture contains a mipmap. */
	bool						hasMipmap();

	/** 
	 * Returns whether the alpha channel of this texture has already been multiplied
	 * into each of the RGB color channels.
	 */
	bool						hasPremultipliedAlpha();

	/** Returns whether this texture is a standard two-dimentional texture. */
	bool						isTexture2D();

	/** Returns whether this texture is a six-sided cube-map texture. */
	bool						isTextureCube();

	/**
	 * Initializes this instance by loaded content from the specified PVR file.
	 *
	 * The specified file path may be either an absolute path, or a path relative to the
	 * application resource directory. If the file is located directly in the application
	 * resources directory, the specified file path can simply be the name of the file.
	 *
	 * Returns nil if the file could not be loaded.
	 */
	void						initFromFile( const std::string& filePath );

protected:
	GLuint						_textureID;
	CC3IntSize					_size;
	GLenum						_pixelFormat;
	GLenum						_pixelType;
	bool						_hasMipmap : 1;
	bool						_isTextureCube : 1;
	bool						_hasPremultipliedAlpha : 1;
};

NS_COCOS3D_END

#endif
