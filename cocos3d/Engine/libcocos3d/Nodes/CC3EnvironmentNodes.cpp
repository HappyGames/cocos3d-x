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

CC3EnvironmentNode::CC3EnvironmentNode()
{
	m_pTexture = NULL;
}

CC3EnvironmentNode::~CC3EnvironmentNode()
{
	CC_SAFE_RELEASE( m_pTexture );
}

bool CC3EnvironmentNode::isLightProbe()
{
	return true; 
}

void CC3EnvironmentNode::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	m_pTexture = NULL;
}

void CC3EnvironmentNode::initWithName( const std::string& name, CC3Texture* texture )
{
	super::initWithName( name );
	setTexture( texture );
}

void CC3EnvironmentNode::initWithName( const std::string& name )
{
	super::initWithName( name );
}

CC3EnvironmentNode* CC3EnvironmentNode::nodeWithName( const std::string& name, CC3Texture* texture )
{
	CC3EnvironmentNode* pNode = new CC3EnvironmentNode;
	pNode->initWithName( name );
	pNode->setTexture( texture );
	pNode->autorelease();

	return pNode;
}

void CC3EnvironmentNode::initWithTexture( CC3Texture* texture )
{
	return initWithName( texture->getName(), texture );
}

CC3EnvironmentNode* CC3EnvironmentNode::nodeWithTexture( CC3Texture* texture )
{
	CC3EnvironmentNode* pNode = new CC3EnvironmentNode;
	pNode->initWithTexture( texture );
	pNode->autorelease();

	return pNode;
}

CC3Texture* CC3EnvironmentNode::getTexture()
{
	return m_pTexture;
}

void CC3EnvironmentNode::setTexture( CC3Texture* texture )
{
	CC_SAFE_RELEASE( m_pTexture );
	CC_SAFE_RETAIN( texture );
	m_pTexture = texture;
}

bool CC3LightProbe::isLightProbe()
{
	return true; 
}

ccColor4F CC3LightProbe::getDiffuseColor()
{
	return m_diffuseColor; 
}

void CC3LightProbe::setDiffuseColor( const ccColor4F& aColor )
{
	m_diffuseColor = aColor;
	super::setDiffuseColor(aColor);	// pass along to any descendant
}

void CC3LightProbe::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	m_diffuseColor = kCCC4FWhite;
}

CC3LightProbe* CC3LightProbe::nodeWithTexture( CC3Texture* texture )
{
	CC3LightProbe* pLight = new CC3LightProbe;
	pLight->initWithTexture( texture );
	pLight->autorelease();

	return pLight;
}

NS_COCOS3D_END
