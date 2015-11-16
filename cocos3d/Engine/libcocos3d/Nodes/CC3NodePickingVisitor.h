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
#ifndef _CCL_CC3NODE_PICKING_VISITOR_H_
#define _CCL_CC3NODE_PICKING_VISITOR_H_

NS_COCOS3D_BEGIN
class CC3Node;
class CC3RenderSurface;

/**
 * CC3NodePickingVisitor is a CC3NodeDrawingVisitor that is passed to a node when
 * it is visited during node picking operations using color-buffer based picking.
 *
 * The visit: method must be invoked with a CC3Scene instance as the argument.
 *
 * Node picking is the act of picking a 3D node from user input, such as a touch.
 * One method of accomplishing this is to draw the scene such that each object is
 * drawn in a unique solid color. Once the scene is drawn, the color of the pixel
 * that has been touched can be read from the OpenGL ES color buffer, and mapped
 * back to the object that was painted with that color.
 */
class CC3NodePickingVisitor : public CC3NodeDrawingVisitor 
{
	DECLARE_SUPER( CC3NodeDrawingVisitor );
public:
	CC3NodePickingVisitor();
	~CC3NodePickingVisitor();
	/** 
	 * Returns the node that was most recently picked.
	 *
	 * This picked node is valid at a point in time. The value of this property is cleared 
	 * immediately after it has been read. Subsequent queries of this property will return nil,
	 * so you should cache the returned value elsewhere if you need it to persist.
	 */
	CC3Node*					getPickedNode();

	/** 
	 * Indicates the value to shift the bits of the value of the tag property of each node to
	 * determine the color to paint that node.
	 *
	 * The initial value of this property is zero, indicating that the node tag value will not
	 * be shifted when converting it to and from a color. Increasing the value will increase the
	 * color separation between different nodes, which can be helpful during development when 
	 * debugging node picking visually (ie- when the shouldDisplayPickingRender property of the
	 * CC3Scene is set to YES), However, increasing the shift value will also decrease the number
	 * of nodes that can be displayed and resolved on screen.
	 *
	 * This value is a shift value that operates on the bits of the tag value. A value of one
	 * will effectively double the tag value before it is converted to a color, a value of two
	 * will quadruple the tag value, etc.
	 */
	GLuint						getTagColorShift();
	void						setTagColorShift( GLuint shift );

	/**
	 * Template property that returns the default value used to automatically
	 * set the value of the renderSurface property.
	 *
	 * Overridden to return the value of the pickingSurface property of the
	 * CC3SceneDrawingSurfaceManager held in the surfaceManager property.
	 */
	CC3RenderSurface*			getDefaultRenderSurface();

	/**
	 * Aligns this visitor to use the same camera as the specified visitor.
	 *
	 * The camera and surfaceManager properties of this visitor are set to those of the specified visitor.
	 *
	 * The renderSurface property is left cleared, so that the defaultRenderSurface property will set
	 * it to the pickingSurface of the surfaceManager, when the renderSurface property is next accessed.
	 */
	void						alignShotWith( CC3NodeDrawingVisitor* otherVisitor );

	/** Overridden to initially set the shouldDecorateNode to NO. */
	void						init();

	/** Clears the render surface and the pickedNode property. */
	void						open();

	/**
	 * Reads the color of the pixel at the touch point, maps that to the tag of the CC3Node
	 * that was touched, and sets the picked node in the pickedNode property.
	 *
	 * Clears the depth buffer in case the primary scene rendering is using the same surface.
	 */
	void						close();

	/**
	 * Maps the specified color to a tag, by combining the bits of the red, green, and blue
	 * colors into a single integer value. This is the compliment of the colorFromNodeTag: method.
	 */
	GLuint						tagFromColor( const ccColor4B& color );

	/**
	 * Overridden because what matters here is not visibility, but touchability.
	 * Invisible nodes will be drawn if touchable.
	 */
	virtual bool				isNodeVisibleForDrawing( CC3Node* aNode );

	/** Overridden to draw the node in a uniquely identifiable color. */
	virtual void				draw( CC3Node* aNode );

	/** Maps the specified node to a unique color, and paints the node with that color. */
	void						paintNode( CC3Node* aNode );

	/**
	 * Maps the specified integer tag to a color, by spreading the bits of the integer across
	 * the red, green and blue unsigned bytes of the color. This permits 2^24 objects to be
	 * encoded by colors. This is the compliment of the tagFromColor: method.
	 */
	ccColor4B					colorFromNodeTag( GLuint tag );

	std::string					fullDescription();

	static CC3NodePickingVisitor* visitor();

protected:
	CC3Node*					_pickedNode;
	GLuint						_tagColorShift;
};

NS_COCOS3D_END

#endif
