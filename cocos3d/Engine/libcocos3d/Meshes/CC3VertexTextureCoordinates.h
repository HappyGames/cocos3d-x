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
#ifndef _CC3_VERTEX_TEXTURE_COORDINATES_H_
#define _CC3_VERTEX_TEXTURE_COORDINATES_H_

NS_COCOS3D_BEGIN

/** A rectangle with origin zero and unit size for initial value of the textureRectangle property. */
static const CCRect kCC3UnitTextureRectangle = CCRectMake( 0.f, 0.f, 1.f, 1.f );

/**
 * A CC3VertexArray that manages the texture coordinates aspect of an array of vertices.
 *
 * This class supports multi-texturing, and a single CC3VertexTextureCoordinates instance
 * can be applied to multiple texture units.
 * 
 * The vertical axis of the coordinate system of OpenGL is inverted relative to
 * the iOS view coordinate system. This results in textures from most file formats
 * being oriented upside-down, relative to the OpenGL coordinate system. All file
 * formats except PVR format will be oriented upside-down after loading.
 *
 * This class supports the expectsVerticallyFlippedTextures property and several
 * utility methods to help align these texture coordinates with textures.
 * The alignWithTexture: method is invoked automatically whenever a texture is added
 * to the mesh holding these texture coordinates to align these texture coordinates
 * with the new texture. In addition, there are several utility methods that can be
 * invoked to flip the mesh content for a texture, either horizontally or vertically.
 *
 * This class includes several convenience methods that allow the texture coordinates
 * to be adjusted to match the visible area of a particular texture.
 *
 * This class supports covering the mesh with a repeating texture through the
 * repeatTexture: method.
 *
 * This class also supports covering the mesh with only a fractional part of the texture
 * through the use of the textureRectangle property, effectivly permitting sprite-sheet
 * textures to be used with 3D meshes.
 */
class CC3VertexTextureCoordinates : public CC3VertexArray 
{
	DECLARE_SUPER( CC3VertexArray );
public:
	static CC3VertexTextureCoordinates* vertexArray();
	static CC3VertexTextureCoordinates* vertexArrayWithName( const std::string& aName );
	/**
	 * Returns the texture coordinate element at the specified index in the underlying vertex content.
	 *
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * the vertexStride and elementOffset properties to access the correct element.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	ccTex2F						getTexCoord2FAt( GLuint index );

	/**
	 * Sets the texture coordinate element at the specified index in the underlying vertex
	 * content to the specified texture coordinate value.
	 * 
	 * The index refers to vertices, not bytes. The implementation takes into consideration
	 * the vertexStride and elementOffset properties to access the correct element.
	 *
	 * If the releaseRedundantContent method has been invoked and the underlying
	 * vertex content has been released, this method will raise an assertion exception.
	 */
	void						setTexCoord2F( const ccTex2F& aTex2F, GLuint index );

	/**
	 * Indicates whether the texture coordinates of this mesh expects that the texture
	 * was flipped upside-down during texture loading.
	 * 
	 * The vertical axis of the coordinate system of OpenGL is inverted relative to the
	 * CoreGraphics view coordinate system. As a result, some texture file formats may be
	 * loaded upside down. Most common file formats, including JPG, PNG & PVR are loaded
	 * right-way up, but using proprietary texture formats developed for other platforms
	 * may result in textures being loaded upside-down.
	 *
	 * The value of this property is used in combination with the value of the 
	 * isUpsideDown property of a texture to determine whether the texture
	 * will be oriented correctly when displayed using these texture coordinates.
	 *
	 * The alignWithTexture: method compares the value of this property with the
	 * isUpsideDown property of the texture to automatically determine
	 * whether these texture coordinates need to be flipped vertically in order
	 * to display the texure correctly, and will do so if needed. As part
	 * of that inversion, the value of this property will also be flipped, to
	 * indicate that the texture coordinates are now aligned differently.
	 *
	 * The value of this property does not affect the behaviour of, nor
	 * is affected by, the flipVertically , alignWithInvertedTexture:,
	 * alignWithTextureCoverage:, or alignWithInvertedTextureCoverage: methods.
	 * 
	 * The initial value of this property is determined by the value of the class-side
	 * defaultExpectsVerticallyFlippedTextures property at the time an instance of
	 * this class is created and initialized. If you want all meshes to behave the same
	 * way, with respect to this property, set the value of that class-side property.
	 * 
	 * The value of this property is set when the underlying mesh texture coordinates are
	 * built or loaded. See the same property on the CC3NodesResource class to understand
	 * how this property is set during mesh resource loading.
	 */
	bool						expectsVerticallyFlippedTextures();
	void						setExpectsVerticallyFlippedTextures( bool expects );

	/**
	 * This class-side property determines the initial value of the
	 * expectsVerticallyFlippedTextures property when an instance
	 * of this class is created and initialized.
	 *
	 * See the notes for that property for more information.
	 *
	 * The initial value of this class-side property is NO.
	 */
	static bool					defaultExpectsVerticallyFlippedTextures();

	/**
	 * This class-side property determines the initial value of the
	 * expectsVerticallyFlippedTextures property when an instance
	 * of this class are created and initialized.
	 *
	 * See the notes for that property for more information.
	 *
	 * The initial value of this class-side property is NO.
	 */
	static void					setDefaultExpectsVerticallyFlippedTextures( bool expectsFlipped );

	/**
	 * Aligns the texture coordinate array with the specfied texture map size,
	 * which is typically extracted from a specific texture.
	 * 
	 * Under iOS, textures that do not have dimensions that are a power-of-two, will
	 * be padded to dimensions of a power-of-two on loading. The result is that the
	 * texture will be physically larger than is expected by these texture coordinates.
	 * The texture map size indicates the usable size of the texture, and invoking
	 * this method will align these texture coordinates with that usable size.
	 *
	 * For the sake of efficiency, if the specified texCoverage is the same as the
	 * value submitted in the previous invocation (or is equal to (1, 1) on the
	 * first invocation), this method does nothing, to avoid updating the texture
	 * coordinates when no change has occurred.
	 *
	 * For subsequent invocations, if the specified texCoverage is the same as the
	 * value submitted in the previous invocation, this method does nothing, to
	 * avoid updating all the texture coordinates to the value they currently have.
	 *
	 * Care should be taken when using this method, as it changes the actual vertex content.
	 * This may cause mapping conflicts if the same vertex content is shared by other
	 * CC3MeshNodes that use different textures.
	 */
	void						alignWithTextureCoverage( const CCSize& texCoverage );


	/**
	 * Aligns the texture coordinate array with the specfied texture map size,
	 * which is typically extracted from a specific texture, and vertically
	 * flips the texture coordinates.
	 * 
	 * Under iOS, textures that do not have dimensions that are a power-of-two, will
	 * be padded to dimensions of a power-of-two on loading. The result is that the
	 * texture will be physically larger than is expected by these texture coordinates.
	 * The texture map size indicates the usable size of the texture, and invoking
	 * this method will align these texture coordinates with that usable size.
	 *
	 * The texture coordinates are flipped vertically to align with textures that have
	 * been loaded upside down. Under iOS, most texture formats are loaded upside-down,
	 * and this method can be used to compensate.
	 *
	 * This method vertically flips the texture coordinates on each invocation. As a
	 * result, unlike the alignWithTextureCoverage: method, this method updates all the
	 * texture coordinates on each invocation, regardless of whether the specified
	 * texCoverage is the same as on the previous invocation.
	 *
	 * Care should be taken when using this method, as it changes the actual vertex content.
	 * This may cause mapping conflicts if the same vertex content is shared by other
	 * CC3MeshNodes that use different textures.
	 */
	void						alignWithInvertedTextureCoverage( const CCSize& texCoverage );

	/**
	 * Aligns the texture coordinate array with the specfied texture.
	 * 
	 * Under iOS, textures that do not have dimensions that are a power-of-two, will
	 * be padded to dimensions of a power-of-two on loading. The result is that the
	 * texture will be physically larger than is expected by these texture coordinates.
	 *
	 * The usable area of the texture is indicated by its mapSize property, and invoking
	 * this method will align these texture coordinates with the usable size of the
	 * specified texture.
	 *
	 * If the value of the expectsVerticallyFlippedTextures property is different
	 * than the value of the isUpsideDown property of the specified texture, the
	 * texture coordinates are not oriented vertically for the texture. To align them,
	 * this method delegates to the alignWithInvertedTextureCoverage:, passing the mapSize
	 * of the specified texture, to both align the texture coordinates to the usable size
	 * of the texture, and to flip the texture coordinates to align with the texture.
	 *
	 * If the value of the expectsVerticallyFlippedTextures property is the same
	 * as the value of the isUpsideDown property of the specified texture, the
	 * texture coordinates are correctly oriented vertically for the texture. This
	 * method delegates to the alignWithTextureCoverage:, passing the mapSize of the
	 * specified texture, to align the texture coordinates to the usable size of
	 * the texture, but does not flip the texture coordinates.
	 *
	 * To avoid updating the texture coordinates when no change has occurred, if the
	 * coordinates do not need to be flipped vertically, and the specified texture has
	 * the same usable area as the texture used on the previous invocation (or has a
	 * full usable area on the first invocation), this method does nothing.
	 *
	 * Care should be taken when using this method, as it changes the actual vertex content.
	 * This may cause mapping conflicts if the same vertex content is shared by other
	 * CC3MeshNodes that use different textures.
	 */
	void						alignWithTexture( CC3Texture* texture );

	/**
	 * Aligns the texture coordinate array with the specfied texture and vertically
	 * flips the texture coordinates.
	 * 
	 * Under iOS, textures that do not have dimensions that are a power-of-two, will
	 * be padded to dimensions of a power-of-two on loading. The result is that the
	 * texture will be physically larger than is expected by these texture coordinates.
	 *
	 * The usable area of the texture is indicated by its mapSize property, and invoking
	 * this method will align these texture coordinates with the usable size of the
	 * specified texture.
	 *
	 * The texture coordinates are flipped vertically to align with textures that have
	 * been loaded upside down. Under iOS, most texture formats are loaded upside-down,
	 * and the texture coordinates are automatically aligned to compensate (see the
	 * notes for the alignWithTexture: method).
	 *
	 * As a result, the application usually has no need for this method. However, this
	 * method can be used occasionally when the automatic alignment is not effective.
	 *
	 * This method vertically flips the texture coordinates on each invocation. As a
	 * result, unlike the alignWithTexture: method, this method updates all texture
	 * coordinates on each invocation, regardless of whether the specified texCoverage
	 * is the same as on the previous invocation.
	 *
	 * Care should be taken when using this method, as it changes the actual vertex content.
	 * This may cause mapping conflicts if the same vertex content is shared by other
	 * CC3MeshNodes that use different textures.
	 */
	void						alignWithInvertedTexture( CC3Texture* texture );

	/**
	 * Convenience method that flips the texture coordinate mapping horizontally.
	 * This has the effect of flipping the texture horizontally on the model,
	 * and can be useful for creating interesting effects, or mirror images.
	 *
	 * This implementation flips correctly if the mesh is mapped
	 * to only a section of the texture (a texture atlas).
	 */
	void						flipHorizontally();

	/**
	 * Convenience method that flips the texture coordinate mapping vertically.
	 * This has the effect of flipping the texture vertically on the model,
	 * and can be useful for creating interesting effects, or mirror images.
	 *
	 * This implementation flips correctly if the mesh is mapped
	 * to only a section of the texture (a texture atlas).
	 */
	void						flipVertically();

	/**
	 * Defines the rectangular area of the texture that should be mapped to the mesh.
	 *
	 * This property facilitates the use of sprite-sheets, where the mesh is covered by a small
	 * fraction of a larger texture. This technique has many uses, including animating a texture
	 * onto a mesh, where each section of the full texture is really a different frame of a
	 * texture animation, or simply loading one larger texture and using parts of it to texture
	 * many different meshes.
	 *
	 * The dimensions of this rectangle are taken as fractional portions of the full area of the
	 * texture. Therefore, a rectangle with zero origin, and unit size ((0.0, 0.0), (1.0, 1.0))
	 * indicates that the mesh should be covered with the complete texture.
	 * 
	 * A rectangle of smaller size, and/or a non-zero origin, indicates that the mesh should be
	 * covered by a fractional area of the texture. For example, a rectangular value for this
	 * property with origin at (0.5, 0.5), and size of (0.5, 0.5) indicates that only the top-right
	 * quarter of the texture will be used to cover this mesh.
	 *
	 * The bounds of the texture rectangle must fit within a unit rectangle. Both the bottom-left
	 * and top-right corners must lie between zero and one in both the X and Y directions.
	 *
	 * The dimensions of the rectangle in this property are independent of the size specified in
	 * the  alignWithTextureCoverage: and alignWithInvertedTextureCoverage: methods. A unit rectangle
	 * value for this property will automatically take into consideration the adjustment made to
	 * the mesh by those methods, and will display only the part of the texture defined by them.
	 * Rectangular values for this property that are smaller than the unit rectangle will be
	 * relative to the displayable area defined by alignWithTextureCoverage: and
	 * alignWithInvertedTextureCoverage:.
	 *
	 * As an example, if the alignWithTextureCoverage: method was used to limit the mesh to using
	 * only 80% of the texture (perhaps when using a non-POT texture), and this property was set
	 * to a rectangle with origin at (0.5, 0.0) and size (0.5, 0.5), the mesh will be covered by
	 * the bottom-right quarter of the usable 80% of the overall texture.
	 *
	 * The initial value of this property is a rectangle with origin at zero, and unit size,
	 * indicating that the mesh will be covered with the complete usable area of the texture.
	 */
	CCRect						getTextureRectangle();
	void						setTextureRectangle( const CCRect& rect );

	/**
	 * Returns the effective texture rectangle, taking into consideration the usable area of the
	 * texture and whether this vertex array is configured for an inverted texture.
	 *
	 * The value returned is the value of the textureRectangle property, modulated by the mapSize
	 * property of the texture. If the expectsVerticallyFlippedTextures property is YES, the
	 * height of the returned rectangle will be negative.
	 */
	CCRect						getEffectiveTextureRectangle();

	/**
	 * Configures the mesh so that a texture applied to this mesh will be repeated the
	 * specified number of times across the mesh, in each dimension. The repeatFactor
	 * argument contains two numbers, corresponding to how many times in each dimension
	 * the texture should be repeated.
	 * 
	 * As an example, a value of (1, 2) for the repeatValue indicates that the texture
	 * should repeat twice vertically, but not repeat horizontally.
	 * 
	 * When a texture is repeated, the corresponding side of the texture covering this
	 * mesh must have a length that is a power-of-two, otherwise the padding added by
	 * iOS to convert it to a power-of-two length internally will be visible in the
	 * repeating pattern across the mesh.
	 *
	 * For a side that is not repeating, the corresponding side of the texture covering
	 * this mesh does not require a length that is a power-of-two.
	 *
	 * The textureParameters property of any texture covering this mesh should include
	 * the GL_REPEAT setting in each of its texture wrap components that correspond to
	 * a repeatFactor greater than one. The GL_REPEAT setting is the default setting
	 * for CC3Texture.
	 *
	 * For example, if you want to repeat your texture twice in one dimension, but only
	 * once in the other, then you would use a repeatFactor of (1, 2) or (2, 1). For the
	 * side that is repeating twice, the length of that side of the texture must be a
	 * power-of-two. But the other side may have any dimension. The textureParameters
	 * property of the CC3Texture should include the GL_REPEAT setting for the
	 * corresponding texture dimension.
	 *
	 * The dimensions of the repeatFactor are independent of the size specified in the
	 * alignWithTextureCoverage: and alignWithInvertedTextureCoverage: methods, or derived
	 * from the texture by the alignWithTexture or alignWithInvertedTexture methods.
	 * A value of 1.0 for an element in the specified repeatFactor will automatically
	 * take into consideration the adjustment made to the mesh by those methods, and will
	 * display only the part of the texture defined by them.
	 *
	 * You can specify a fractional value for either of the components of the repeatFactor
	 * to expand the texture in that dimension so that only part of the texture appears
	 * in that dimension, while potentially repeating multiple times in the other dimension.
	 */
	void						repeatTexture( const ccTex2F& repeatFactor );

	/**
	 * Aligns the vertex texture coordinates with the area of the texture defined
	 * by the newRect. The oldRect describes the area of the texture that is currently
	 * mapped by the texture coordinates.
	 */
	void						alignWithTextureRectangle( const CCRect& newRect, const CCRect& oldRect );

	// Template method that populates this instance from the specified other instance.
	// This method is invoked automatically during object copying via the copyWithZone: method.
	void						populateFrom( CC3VertexTextureCoordinates* another );
	virtual CCObject*			copyWithZone( CCZone* zone );

	std::string					getNameSuffix();
	void						initWithTag( GLuint aTag, const std::string& aName );
	GLenum						defaultSemantic();


protected:
	CCSize						m_mapSize;
	CCRect						m_textureRectangle;
	bool						m_expectsVerticallyFlippedTextures : 1;
};

NS_COCOS3D_END

#endif
