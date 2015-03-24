/*
 * Models.m
 *
 * Cocos3D-X 1.0.0
 * Author: Bill Hollings
 * Copyright (c) 2011-2014 The Brenwill Workshop Ltd. All rights reserved.
 * http://www.brenwill.com
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
 * 
 * See header file Models.h for full API documentation.
 */
#include "Models.h"
#include "actions/CCActionInstant.h"

#define kDropHeight 700.0

CC3Light* IntroducingPODResource::buildLightAtIndex( unsigned int lightIndex )
{
	return IntroducingPODLight::nodeAtIndex( lightIndex, this );
}

CC3Material* IntroducingPODResource::buildMaterialAtIndex( unsigned int materialIndex )
{
	CC3Material* mat = super::buildMaterialAtIndex( materialIndex );
	mat->setSpecularColor( kCC3DefaultMaterialColorSpecular );
	mat->setEmissionColor( kCC3DefaultMaterialColorEmission );
	mat->setShininess( kCC3DefaultMaterialShininess );
	return mat;
}

IntroducingPODResource* IntroducingPODResource::resourceFromFile( const std::string& filePath )
{
	IntroducingPODResource* pRez = new IntroducingPODResource;
	if ( pRez->initFromFile( filePath ) )
	{
		pRez->autorelease();
		return pRez;
	}

	CC_SAFE_DELETE( pRez );
	return pRez;
}

void IntroducingPODLight::applyDirection()
{

}

void IntroducingPODLight::applyColorWithVisitor( CC3NodeDrawingVisitor* visitor )
{
	CC3OpenGL* gl = visitor->getGL();
	gl->setLightAmbientColor( kCC3DefaultLightColorAmbient, getLightIndex() );
	gl->setLightDiffuseColor( kCC3DefaultLightColorDiffuse, getLightIndex() );
	gl->setLightSpecularColor( kCC3DefaultLightColorSpecular, getLightIndex() );
}

void PhysicsMeshNode::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	velocity = kCC3VectorZero;
	previousGlobalLocation = kCC3VectorZero;
}

CC3Vector PhysicsMeshNode::getPreviousGlobalLocation()
{
	return previousGlobalLocation;
}

CC3Vector PhysicsMeshNode::getVelocity()
{
	return velocity;
}

void PhysicsMeshNode::updateAfterTransform( CC3NodeUpdatingVisitor* visitor )
{
	CC3Vector currGlobalLoc = getGlobalLocation();
	CC3Vector movement = CC3VectorDifference(currGlobalLoc, getPreviousGlobalLocation());
	velocity = CC3VectorScaleUniform(movement, 1.0f / visitor->getDeltaTime());
	previousGlobalLocation = currGlobalLoc;
}

PhysicsMeshNode* PhysicsMeshNode::nodeWithName( const std::string& aName )
{
	PhysicsMeshNode* pNode = new PhysicsMeshNode;
	pNode->initWithName( aName );
	pNode->autorelease();

	return pNode;
}

void DoorMeshNode::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	setIsOpen( false );
}

DoorMeshNode* DoorMeshNode::nodeWithName( const std::string& aName )
{
	DoorMeshNode* pNode = new DoorMeshNode;
	pNode->initWithName( aName );
	pNode->autorelease();

	return pNode;
}

bool DoorMeshNode::isOpen()
{
	return m_isOpen;
}

void DoorMeshNode::setIsOpen( bool isOpen )
{
	m_isOpen = isOpen;
}

void SpinningNode::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	setSpinAxis( kCC3VectorZero );
	setSpinSpeed( 0.0f );
	setFriction( 0.0f );
	setIsFreeWheeling( false );
}

void SpinningNode::setIsFreeWheeling( bool isFreeWheeling )
{
	m_isFreeWheeling = isFreeWheeling;
}

bool SpinningNode::isFreeWheeling()
{
	return m_isFreeWheeling;
}

void SpinningNode::setFriction( GLfloat friction )
{
	m_friction = friction;
}

GLfloat SpinningNode::getFriction()
{
	return m_friction;
}

void SpinningNode::setSpinSpeed( GLfloat spinSpeed )
{
	m_spinSpeed = spinSpeed;
}

GLfloat SpinningNode::getSpinSpeed()
{
	return m_spinSpeed;
}

CC3Vector SpinningNode::getSpinAxis()
{
	return m_spinAxis;
}

void SpinningNode::setSpinAxis( const CC3Vector& spinAxis )
{
	m_spinAxis = spinAxis;
}

// Template method that populates this instance from the specified other instance.
// This method is invoked automatically during object copying via the copyWithZone: method.
void SpinningNode::populateFrom( CC3Node* another )
{
	super::populateFrom( another );
	
	// Only copy these properties if the original is of the same class
	SpinningNode* spinNode = dynamic_cast<SpinningNode*>(another);
	if ( spinNode )
	{
		setSpinAxis( spinNode->getSpinAxis() );
		setSpinSpeed( spinNode->getSpinSpeed() );
		setFriction( spinNode->getFriction() );
		setIsFreeWheeling( spinNode->isFreeWheeling() );
	}
}


// Don't bother continuing to rotate once below this speed (in degrees per second)
#define kSpinningMinSpeed	6.0

/**
 * On each update, if freewheeling, rotate the node around the spinAxis, by an
 * angle determined by the spinSpeed. Then slow the spinSpeed down based on the
 * friction value and how long the friction has been applied since the last update.
 * Stop rotating altogether once the speed is low enough to be unnoticable, so that
 * we don't continue to perform transforms (and rebuilding shadows) unnecessarily.
 */
void SpinningNode::updateBeforeTransform( CC3NodeUpdatingVisitor* visitor )
{
	GLfloat dt = visitor->getDeltaTime();
	GLfloat spinSpeed = getSpinSpeed();
	if (isFreeWheeling() && spinSpeed > kSpinningMinSpeed) 
	{
		GLfloat deltaAngle = spinSpeed * dt;
		rotateByAngle( deltaAngle, getSpinAxis() );
		spinSpeed -= (deltaAngle * getFriction());
		//LogTrace(@"Spinning %@ by %.3f at speed %.3f", self, deltaAngle, spinSpeed);
	}
}

SpinningNode* SpinningNode::nodeWithName( const std::string& aName )
{
	SpinningNode* pNode = new SpinningNode;
	pNode->initWithName( aName );
	pNode->autorelease();

	return pNode;
}

void LandingCraft::populateArmyWith( CC3Node* templateNode )
{
	// To help demonstrate that the hordes of actioned nodes that make up this army are being managed
	// correctly, log the current number of nodes and actions, before the army has been created.
	//LogInfo(@"Before populating %@ there are %i instances of CC3Identifiable subclasses in existance, and  %u actions running.",
	//		self, [CC3Identifiable instanceCount], [CCDirector.sharedDirector.actionManager numberOfRunningActions]);
	
	// Create many copies (invadersPerHalfSide * 2) ^ 2,
	// and space them out throughout the area of the ground plane, in a grid pattern.
	int invadersPerHalfSide = 5;
	GLfloat spacing = 1000.0f / invadersPerHalfSide;
	for (int ix = -invadersPerHalfSide; ix <= invadersPerHalfSide; ix++) 
	{
		for (int iz = -invadersPerHalfSide; iz <= invadersPerHalfSide; iz++) 
		{
			GLfloat xLoc = spacing * ix;
			GLfloat zLoc = spacing * iz;
			
			// Don't drop invaders into the central area where the main robot is.
			if (fabsf(xLoc) > 100.0f || fabsf(zLoc) > 100.0f) 
			{
				CC3Node* invader = (CC3Node*)templateNode->copy();
				invader->setLocation( cc3v(xLoc, kDropHeight, zLoc) );
				
				// If the invader has an animation, run its animation, otherwise rotate
				// it horizontally. In either case, the rate of motion is randomized so
				// that each invader moves at its own speed.
#if 0
				CCActionInterval* invaderAction;
				if ( invader->containsAnimation() )
					invaderAction = CC3ActionAnimate::actionWithDuration( CC3RandomFloatBetween(2.5, 10.0) );
				else
					invaderAction = CC3ActionRotateBy::actionWithDuration( 1.0f, cc3v(0.0, CC3RandomFloatBetween(30.0, 90.0), 0.0) );
				
				CCActionInterval* groundAction = CCActionRepeat::create( invaderAction, kCC3MaxGLuint );
				
				// Create a landing action that is a bouncing drop of random duration, to simulate raining down.
				CC3Vector landingLocation = cc3v(invader->getLocation().x, 0.0f, invader->getLocation().z);
				CCActionInterval* landingAction = CCActionEaseBounceOut::create( CC3ActionMoveTo::actionWithDuration( CC3RandomFloatBetween(1.0f, 2.0f), landingLocation ) );
				
				// Set up a sequence on the invader...first drop, and then animate or rotate
				invader->runAction( CCActionSequence::createWithTwoActions( landingAction, groundAction ) );
				invader->runAction( landingAction );
#endif
				
				addChild( invader );		// Add the child to the landing craft
			}
		}
	}
	
	// Create OpenGL ES buffers for the vertex arrays to keep things fast and efficient,
	// and to save memory, release the vertex data in main memory because it is now redundant.
	createGLBuffers();
	releaseRedundantContent();
	
	// To help demonstrate that the hordes of actioned nodes that make up this army are being managed
	// correctly, log the current number of nodes and actions, now that the army has been created.
	//LogInfo(@"After populating %@ there are %i instances of CC3Identifiable subclasses in existance, and  %u actions running.",
	//		self, [CC3Identifiable instanceCount], [CCDirector.sharedDirector.actionManager numberOfRunningActions]);
}

/** Uses a CCActionSequence action to first fade the army away and then remove it. */
void LandingCraft::evaporate()
{
	//runAction( CCActionSequence::create( CCActionFadeOut::create(1.0f), CC3ActionRemove::action() );
}

LandingCraft* LandingCraft::nodeWithName( const std::string& aName )
{
	LandingCraft* pVal = new LandingCraft;
	pVal->initWithName( aName );
	pVal->autorelease();

	return pVal;
}

/**
 * Uses the index of the particle to determine its location relative to the origin of
 * the emitter. The particles are laid out in a simple rectangular grid in the X-Z plane,
 * with kParticlesPerSide particles on each side of the grid.
 *
 * Each particle is assigned a random color and size.
 */
void HangingPointParticle::initializeParticle()
{
	super::initializeParticle();
	GLint zIndex = _particleIndex / kParticlesPerSide;
	GLint xIndex = _particleIndex % kParticlesPerSide;
	
	GLfloat xStart = -kParticlesPerSide * kParticlesSpacing / 2.0f;
	GLfloat zStart = -kParticlesPerSide * kParticlesSpacing / 2.0f;
	
	setLocation( cc3v(xStart + (xIndex * kParticlesSpacing),
						 0.0,
						 zStart + (zIndex * kParticlesSpacing) ) );
	
	setColor4F( RandomCCC4FBetween(kCCC4FDarkGray, kCCC4FWhite) );
	
	GLfloat avgSize = ((CC3PointParticleEmitter*)_emitter)->getParticleSize();
	setSize( CC3RandomFloatBetween(avgSize * 0.75, avgSize * 1.25) );
}

void HangingMeshParticle::setRotationSpeed( GLfloat speed )
{
	rotationSpeed = speed;
}

GLfloat HangingMeshParticle::getRotationSpeed()
{
	return rotationSpeed;
}

/**
 * Uses the index of the particle to determine its location relative to the origin of
 * the emitter. The particles are laid out in a simple rectangular grid in the X-Z plane,
 * with kParticlesPerSide particles on each side of the grid.
 */
void HangingMeshParticle::initializeParticle()
{
	super::initializeParticle();

	GLuint particleIndex = _emitter->getParticleCount();
	GLint zIndex = particleIndex / kParticlesPerSide;
	GLint xIndex = particleIndex % kParticlesPerSide;
	
	GLfloat xStart = -kParticlesPerSide * kParticlesSpacing / 2.0f;
	GLfloat zStart = -kParticlesPerSide * kParticlesSpacing / 2.0f;
	
	setLocation( cc3v(xStart + (xIndex * kParticlesSpacing),
						 0.0,
						 zStart + (zIndex * kParticlesSpacing) ) );

	
	// Apply a texture rectangle based on the particleIndex.
	setTextureRectangle( getTextureRectangle() );
	
	setRotationAxis( CC3VectorNormalize(cc3v(CC3RandomFloatBetween(0.0, 1.0),
												CC3RandomFloatBetween(0.0, 1.0),
												CC3RandomFloatBetween(0.0, 1.0))) );
	setRotationSpeed( CC3RandomFloatBetween(-30.0, 30.0) );
	
	// To improve performance, accumulate rotational changes and apply
	// only when a threshold has been reached.
	setAccumulatedAngleChange( 0.0f );
}

/** Derive the texture rectangle from the particle index, in a modulus of eight options . */
CCRect HangingMeshParticle::getTextureRectangle()
{
	unsigned int particleIndex = _emitter->getParticleCount();
	switch (particleIndex % 8) 
	{
		case 1:
			return CCRectMake(0.25, kCC3OneThird, 0.25, kCC3OneThird);	// Front
		case 2:
			return CCRectMake(0.0, kCC3OneThird, 0.25, kCC3OneThird);	// Left
		case 3:
			return CCRectMake(0.5, kCC3OneThird, 0.25, kCC3OneThird);	// Right
		case 4:
			return CCRectMake(0.75, kCC3OneThird, 0.25, kCC3OneThird);	// Back
		case 5:
			return CCRectMake(0.25, (kCC3OneThird + kCC3OneThird), 0.25, kCC3OneThird);	// Top
		case 6:
			return CCRectMake(0.25, 0.0, 0.25, kCC3OneThird);			// Bottom
		case 0:
		case 7:
		default:
			return kCC3UnitTextureRectangle;							// Entire texture
	}
}

/**
 * Angular threshold, in degrees, that must be reached before accumulated rotational changes
 * are applied to the particle.
 *
 * You can experiment with different values for this threshold and observe how it changes the
 * performance in terms of frames-per-second. A value of zero for the threshold will update the
 * rotation on every frame.
 */
#define kRotationalUpdateThreshold	6.0

/**
 * Rotate the particle around the rotation axis that was picked in the initializeParticle method.
 * To avoid processing rotations and vertices that are not noticable, small rotational changes
 * are accumultated until the kRotationalUpdateThreshold angle is reached, and then are applied.
 *
 * You can experiment with different values for this threshold and observe how it changes the
 * performance in terms of frames-per-second. A value of zero for the threshold will update the
 * rotation on every frame.
 */
void HangingMeshParticle::updateBeforeTransform( CC3NodeUpdatingVisitor* visitor )
{
	GLfloat angleChange = getAccumulatedAngleChange();
	angleChange	+= getRotationSpeed() * visitor->getDeltaTime();
	setAccumulatedAngleChange( angleChange ); 

	if (angleChange > kRotationalUpdateThreshold) {
		GLfloat rotationAngle = getRotationAngle();
		rotationAngle += accumulatedAngleChange;
		setRotationAngle( rotationAngle );
		setAccumulatedAngleChange( 0.f );
	}
}

void HangingMeshParticle::populateFrom( HangingMeshParticle* another )
{
	super::populateFrom( another );
	setRotationSpeed( another->getRotationSpeed() );
	setAccumulatedAngleChange( another->getAccumulatedAngleChange() );
}

CCObject* HangingMeshParticle::copyWithZone( CCZone* zone )
{
	HangingMeshParticle* pVal = new HangingMeshParticle;
	pVal->init();
	pVal->populateFrom( this );
	
	return pVal;
}

GLfloat HangingMeshParticle::getAccumulatedAngleChange()
{
	return accumulatedAngleChange;
}

void HangingMeshParticle::setAccumulatedAngleChange( GLfloat angleChange )
{
	accumulatedAngleChange = angleChange;
}

/** Picks a random rotational axis and rotational velocity, and fades the particle over its lifetime. */
void RotatingFadingMeshParticle::initializeParticle()
{
	super::initializeParticle();
	
	// Select a random rotation axis and velocity
	setRotationAxis( CC3VectorNormalize(cc3v(CC3RandomFloatBetween(0.0, 1.0),
												CC3RandomFloatBetween(0.0, 1.0),
												CC3RandomFloatBetween(0.0, 1.0))) );
	
	// Alternate between rotating right or left.
	// Particles are always emitted at the end, so particle index should be randomly odd/even.
	unsigned int particleIndex = _emitter->getParticleCount();
	float dirSign = CC3IntIsEven(particleIndex) ? 1 : -1;
	setRotationAngle( 0.0f );
	setRotationAngleVelocity( dirSign * CC3RandomFloatBetween(45.0, 120.0) );
	
	setUniformScale( CC3RandomFloatBetween(0.5, 2.0) );
	
	// Set the color velocity to change only the opacity, to fade the particle away
	setColor4F( _emitter->getDiffuseColor() );
	setColorVelocity( ccc4f(0.0f, 0.0f, 0.0f, -(1.0 / getLifeSpan())) );
}

GLfloat CylinderLabel::getRadius()
{
	return radius;
}

void CylinderLabel::setRadius( GLfloat radius )
{
	this->radius = radius;
}

void CylinderLabel::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	setRadius( 1000.f );
}

void CylinderLabel::populateFrom( CylinderLabel* another )
{
	super::populateFrom( another );
	setRadius( another->getRadius() );
}

CCObject* CylinderLabel::copyWithZone( CCZone* zone )
{
	CylinderLabel* pVal = new CylinderLabel;
	pVal->init();
	pVal->populateFrom( this );
	pVal->addCopiesOfChildrenFrom( this ); 

	return pVal;
}

/**
 * At this point, the vertices lie in the X-Y plane. Wrap them around around a circle whose radius
 * is defined by the radius property, and whose center lies behind the X-Y text plane at (0, 0, -radius).
 *
 * The current X-component of the vertex location defines how far around the circle the vertex
 * will be located. Dividing this by the radius determines its positional angle in radians.
 * The new X- and Z-components of the vertex location can then be determined by trigonometry,
 * and the normal is set to point radially outward.
 *
 * Finally, the origin of the mesh is moved to the center of the circle, so that when this mesh
 * is positioned or rotated, it will be relative to the center of the circle. Since this may all be done
 * dynamically whenever the label text changes, update any GL buffers with the new mesh vertex locations.
 */
void CylinderLabel::wrapTextToArc()
{
	GLuint vtxCount = getVertexCount();
	for (GLuint vIdx = 0; vIdx < vtxCount; vIdx++) 
	{
		CC3Vector vtxLoc = getVertexLocationAt( vIdx );
		GLfloat angleInRads = vtxLoc.x / radius;

		GLfloat s = sinf(angleInRads);
		GLfloat c = cosf(angleInRads);
		setVertexLocation( cc3v((radius * s), vtxLoc.y, (radius * (c - 1.0f))), vIdx );
		setVertexNormal( cc3v(s, 0.0f, c), vIdx );
	}
	moveMeshOriginTo( cc3v(0, 0, -radius) );
}

void CylinderLabel::populateAsBitmapFontLabelFromString( const std::string& lblString, const std::string& fontFile, GLfloat lineHt, CCTextAlignment textAlign, const CCPoint& origin, 
							const CC3Tessellation& divsPerChar )
{
	super::populateAsBitmapFontLabelFromString( lblString, fontFile, lineHt, textAlign, origin, divsPerChar );
	wrapTextToArc();
}

CylinderLabel* CylinderLabel::nodeWithName( const std::string& aName )
{
	CylinderLabel* pNode = new CylinderLabel;
	pNode->initWithName( aName );
	pNode->autorelease();

	return pNode;
}
