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

CC3LocalContentNode::CC3LocalContentNode()
{

}

CC3LocalContentNode::~CC3LocalContentNode()
{

}

bool CC3LocalContentNode::hasLocalContent()
{
	return true; 
}

GLint CC3LocalContentNode::getZOrder()
{
	return _zOrder; 
}

void CC3LocalContentNode::setZOrder( GLint zo )
{
	_zOrder = zo;
	super::setZOrder( zo );
}

CC3Vector CC3LocalContentNode::getLocalContentCenterOfGeometry()
{
	CC3Box bb = getLocalContentBoundingBox();
	return CC3BoxIsNull(bb) ? CC3Vector::kCC3VectorZero : CC3BoxCenter(bb);
}

CC3Vector CC3LocalContentNode::getGlobalLocalContentCenterOfGeometry()
{
	return getGlobalTransformMatrix()->transformLocation( getLocalContentCenterOfGeometry() );
}

// Overridden to return the localContentBoundingBox if no children.
CC3Box CC3LocalContentNode::getBoundingBox()
{
	return m_pChildren ? super::getBoundingBox() : getLocalContentBoundingBox(); 
}

CC3Box CC3LocalContentNode::getLocalContentBoundingBox()
{
	return kCC3BoxNull; 
}

CC3Box CC3LocalContentNode::getGlobalLocalContentBoundingBox()
{
	// If the global bounding box is null, rebuild it, otherwise return it.
	if (CC3BoxIsNull(_globalLocalContentBoundingBox))
		_globalLocalContentBoundingBox = getLocalContentBoundingBoxRelativeTo( NULL );
	return _globalLocalContentBoundingBox;
}

CC3Box CC3LocalContentNode::getLocalContentBoundingBoxRelativeTo( CC3Node* ancestor )
{
	CC3Box lcbb = getLocalContentBoundingBox();
	if (ancestor == this)
		return lcbb;

	CC3Matrix4x3 tMtx;
	getGlobalTransformMatrix()->populateCC3Matrix4x3( &tMtx );

	if ( ancestor )
		ancestor->getGlobalTransformMatrixInverted()->leftMultiplyIntoCC3Matrix4x3( &tMtx );
	
	// The eight vertices of the transformed local bounding box
	CC3Vector bbVertices[8];
	
	// Get the corners of the local bounding box
	CC3Vector bbMin = lcbb.minimum;
	CC3Vector bbMax = lcbb.maximum;
	
	// Construct all 8 corner vertices of the local bounding box and transform each
	// to the coordinate system of the ancestor. The result is an oriented-bounding-box.
	bbVertices[0] = CC3Matrix4x3TransformLocation(&tMtx, cc3v(bbMin.x, bbMin.y, bbMin.z));
	bbVertices[1] = CC3Matrix4x3TransformLocation(&tMtx, cc3v(bbMin.x, bbMin.y, bbMax.z));
	bbVertices[2] = CC3Matrix4x3TransformLocation(&tMtx, cc3v(bbMin.x, bbMax.y, bbMin.z));
	bbVertices[3] = CC3Matrix4x3TransformLocation(&tMtx, cc3v(bbMin.x, bbMax.y, bbMax.z));
	bbVertices[4] = CC3Matrix4x3TransformLocation(&tMtx, cc3v(bbMax.x, bbMin.y, bbMin.z));
	bbVertices[5] = CC3Matrix4x3TransformLocation(&tMtx, cc3v(bbMax.x, bbMin.y, bbMax.z));
	bbVertices[6] = CC3Matrix4x3TransformLocation(&tMtx, cc3v(bbMax.x, bbMax.y, bbMin.z));
	bbVertices[7] = CC3Matrix4x3TransformLocation(&tMtx, cc3v(bbMax.x, bbMax.y, bbMax.z));
	
	// Construct a transformed mesh bounding box that surrounds the eight global vertices
	CC3Box bb = kCC3BoxNull;
	for (int i = 0; i < 8; i++) 
		bb = CC3BoxEngulfLocation(bb, bbVertices[i]);
	return bb;
}

// Overridden to include local content
CC3Box CC3LocalContentNode::getBoundingBoxRelativeTo( CC3Node* ancestor )
{
	CC3Box lcbb = (shouldContributeToParentBoundingBox()
				   ? getLocalContentBoundingBoxRelativeTo( ancestor )
				   : kCC3BoxNull);
	return CC3BoxUnion(lcbb, super::getBoundingBoxRelativeTo(ancestor));
}

/** Notify up the ancestor chain...then check my children by invoking superclass implementation. */
void CC3LocalContentNode::checkDrawingOrder()
{
	m_pParent->descendantDidModifySequencingCriteria( this );
	super::checkDrawingOrder();
}

void CC3LocalContentNode::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	_globalLocalContentBoundingBox = kCC3BoxNull;
	_zOrder = 0;
}

void CC3LocalContentNode::populateFrom( CC3LocalContentNode* another )
{
	super::populateFrom( another );

	// The globalLocalContentBoundingBox property is left uncopied so that
	// it will start at kCC3BoxNull and be lazily created on next access.

	// Could create a child node
	setShouldDrawLocalContentWireframeBox( another->shouldDrawLocalContentWireframeBox() );
	
	_zOrder = another->getZOrder();
}

CCObject* CC3LocalContentNode::copyWithZone( CCZone* zone )
{
	CC3LocalContentNode* pVal = new CC3LocalContentNode;
	pVal->init();
	pVal->populateFrom( this );
	pVal->addCopiesOfChildrenFrom( this );

	return pVal;
}

/** Overridden to force a lazy recalculation of the globalLocalContentBoundingBox. */
void CC3LocalContentNode::markTransformDirty()
{
	super::markTransformDirty();
	_globalLocalContentBoundingBox = kCC3BoxNull;
}

/** Overridden to return local content box color */
CCColorRef CC3LocalContentNode::getInitialDescriptorColor()
{
	return CCColorRefFromCCC4F(getInitialLocalContentWireframeBoxColor());
}

/** Suffix used to name the local content wireframe. */
#define kLocalContentWireframeBoxSuffix "LCWFB"

/** The name to use when creating or retrieving the wireframe node of this node's local content. */
std::string CC3LocalContentNode::getLocalContentWireframeBoxName()
{
	return CC3String::stringWithFormat( (char*)"%s-%s", getName().c_str(), kLocalContentWireframeBoxSuffix );
}

CC3WireframeBoundingBoxNode* CC3LocalContentNode::getLocalContentWireframeBoxNode()
{
	return (CC3WireframeBoundingBoxNode*)( getNodeNamed( getLocalContentWireframeBoxName().c_str() ) );
}

bool CC3LocalContentNode::shouldDrawLocalContentWireframeBox()
{
	return (getLocalContentWireframeBoxNode() != NULL); 
}

void CC3LocalContentNode::setShouldDrawLocalContentWireframeBox( bool shouldDraw )
{	
	// Fetch the wireframe node from the child nodes.
	CC3WireframeBoundingBoxNode* wf = getLocalContentWireframeBoxNode();
	
	// If the wireframe exists, but should not, remove it
	if (wf && !shouldDraw) 
		wf->remove();
	
	// If there is no wireframe, but there should be, add it by creating a
	// CC3WireframeLocalContentBoundingBoxNode from the localContentBoundingBox
	// property and add it as a child of this node. If the bounding box is null,
	// don't create a wireframe. Since the local content of a node does not
	// normally change shape, the bounding box is NOT set to update its vertices
	// by default from the bounding box of this node on each update pass.
	if(!wf && shouldDraw) 
	{
		CC3Box mbb = getLocalContentBoundingBox();
		if ( !CC3BoxIsNull(mbb) ) 
		{
			wf = CC3WireframeLocalContentBoundingBoxNode::nodeWithName( getLocalContentWireframeBoxName() );
			wf->populateAsWireBox( mbb );
			wf->setEmissionColor( getInitialLocalContentWireframeBoxColor() );
			addChild( wf );
		}
	}
}

/** If default is transparent black, use the color of the node. */
ccColor4F CC3LocalContentNode::getInitialLocalContentWireframeBoxColor()
{
	ccColor4F defaultColor = getLocalContentWireframeBoxColor();
	return CCC4FAreEqual(defaultColor, kCCC4FBlackTransparent)
				? CCC4FFromCCColorRef(getColor()) 
				: defaultColor;
}

// The default color to use when drawing the wireframes of the local content
static ccColor4F localContentWireframeBoxColor = { 1.0, 0.5, 0.0, 1.0 };	// kCCC4FOrange

ccColor4F CC3LocalContentNode::getLocalContentWireframeBoxColor()
{
	return localContentWireframeBoxColor; 
}

void CC3LocalContentNode::setLocalContentWireframeBoxColor( const ccColor4F& aColor )
{
	localContentWireframeBoxColor = aColor;
}

bool CC3LocalContentNode::shouldDrawAllLocalContentWireframeBoxes()
{
	if (!shouldDrawLocalContentWireframeBox())
		return false;

	return super::shouldDrawAllLocalContentWireframeBoxes();
}

void CC3LocalContentNode::setShouldDrawAllLocalContentWireframeBoxes( bool shouldDraw )
{
	setShouldDrawLocalContentWireframeBox( shouldDraw );
	super::setShouldDrawAllLocalContentWireframeBoxes( shouldDraw );
}

bool CC3LocalContentNode::shouldContributeToParentBoundingBox()
{
	return true; 
}

NS_COCOS3D_END
