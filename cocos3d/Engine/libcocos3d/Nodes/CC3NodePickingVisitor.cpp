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

CC3NodePickingVisitor::CC3NodePickingVisitor()
{
	m_pPickedNode = NULL;
}

CC3NodePickingVisitor::~CC3NodePickingVisitor()
{
	CC_SAFE_RELEASE( m_pPickedNode );
}

/** 
 * Since node picking is asynchronous, the picked node is retained when stored here.
 * When accessing, retain and autorelease it, then release and clear the iVar.
 * In the case where this is the last reference, the retain/autorelease will ensure
 * it continues to live for processing by the caller.
 */
CC3Node* CC3NodePickingVisitor::getPickedNode()
{
	if ( m_pPickedNode )
	{
		m_pPickedNode->retain();
		m_pPickedNode->autorelease();
	}

	CC3Node* pn =  m_pPickedNode;
	CC_SAFE_RELEASE_NULL( m_pPickedNode );

	return pn;
}

/** Overridden to initially set the shouldDecorateNode to NO. */
void CC3NodePickingVisitor::init()
{
	super::init();

	m_pPickedNode = NULL;
	m_shouldDecorateNode = false;
	m_tagColorShift = 0;
}

/** Clears the render surface and the pickedNode property. */
void CC3NodePickingVisitor::open()
{
	super::open();
	getRenderSurface()->clearColorAndDepthContent();
}

/**
 * Reads the color of the pixel at the touch point, maps that to the tag of the CC3Node
 * that was touched, and sets the picked node in the pickedNode property.
 *
 * Clears the depth buffer in case the primary scene rendering is using the same surface.
 */
void CC3NodePickingVisitor::close()
{
	CC3RenderSurface* surface = getRenderSurface();

	// Get layer touch point and convert to pixels
	CCPoint touchPoint = ccpMult(getScene()->getTouchedNodePicker()->getTouchPoint(),
								 CCDirector::sharedDirector()->getContentScaleFactor());

	// Convert the touch point to the coordinates of the surface viewport by offsetting
	// the touch point by the surface viewport origin
	CC3IntPoint vpTouchPoint = CC3IntPointAdd(CC3IntPointFromCGPoint(touchPoint), surface->getViewport().origin);

	// Read the pixel from the surface
	ccColor4B pixColor;
	surface->readColorContentFrom( CC3ViewportMake(vpTouchPoint.x, vpTouchPoint.y, 1, 1), &pixColor );
	
	// Fetch the node whose tags is mapped from the pixel color
	CC_SAFE_RELEASE( m_pPickedNode );
	m_pPickedNode = getScene()->getNodeTagged( tagFromColor( pixColor ) );
	CC_SAFE_RETAIN( m_pPickedNode );

	//LogTrace(@"%@ picked %@ from color %@ at position %@", self, _pickedNode,
	//		 NSStringFromCCC4B(pixColor), NSStringFromCC3IntPoint(vpTouchPoint));
	
	super::close();
}

CC3RenderSurface* CC3NodePickingVisitor::getDefaultRenderSurface()
{
	return (getScene()->shouldDisplayPickingRender()
			? getSurfaceManager()->getViewSurface()
			: getSurfaceManager()->getPickingSurface());
}

void CC3NodePickingVisitor::alignShotWith( CC3NodeDrawingVisitor* otherVisitor )
{
	setSurfaceManager( otherVisitor->getSurfaceManager() );		// Also clears renderSurface property
	setCamera( otherVisitor->getCamera() );
}

/**
 * Overridden because what matters here is not visibility, but touchability.
 * Invisible nodes will be drawn if touchable.
 */
bool CC3NodePickingVisitor::isNodeVisibleForDrawing( CC3Node* aNode )
{
	return aNode->isTouchable();
}

/** Overridden to draw the node in a uniquely identifiable color. */
void CC3NodePickingVisitor::draw( CC3Node* aNode )
{
	paintNode( aNode );
	super::draw( aNode );
}

/** Maps the specified node to a unique color, and paints the node with that color. */
void CC3NodePickingVisitor::paintNode( CC3Node* aNode )
{
	setCurrentColor4B( colorFromNodeTag( aNode->getTag() ) );
	//LogTrace(@"%@ painting %@ with color %@", self, aNode, NSStringFromCCC4B(self.currentColor4B));
}

/**
 * Maps the specified integer tag to a color, by spreading the bits of the integer across
 * the red, green and blue unsigned bytes of the color. This permits 2^24 objects to be
 * encoded by colors. This is the compliment of the tagFromColor: method.
 */
ccColor4B CC3NodePickingVisitor::colorFromNodeTag( GLuint tag )
{
	tag <<= m_tagColorShift;
	GLuint mask = 255;
	GLubyte r = (tag >> 16) & mask;
	GLubyte g = (tag >> 8) & mask;
	GLubyte b = tag & mask;
	return ccc4(r, g, b, 0);	// Alpha ignored during pure-color painting
}

/**
 * Maps the specified color to a tag, by combining the bits of the red, green, and blue
 * colors into a single integer value. This is the compliment of the colorFromNodeTag: method.
 */
GLuint CC3NodePickingVisitor::tagFromColor( const ccColor4B& color )
{
	return (((GLuint)color.r << 16) | ((GLuint)color.g << 8) | (GLuint)color.b) >> m_tagColorShift;
}

std::string CC3NodePickingVisitor::fullDescription()
{
	//return [NSString stringWithFormat: @"%@, picked: %@", super.fullDescription, _pickedNode];
	return "CC3NodePickingVisitor";
}

CC3NodePickingVisitor* CC3NodePickingVisitor::visitor()
{
	CC3NodePickingVisitor* pVal = new CC3NodePickingVisitor;
	pVal->init();
	pVal->autorelease();

	return pVal;
}

NS_COCOS3D_END
