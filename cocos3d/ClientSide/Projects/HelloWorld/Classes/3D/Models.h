/*
 * Models.h
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
 */
#include "cocos3d.h"

USING_NS_COCOS3D;

/**
 * Customized POD resource class to handle the idiosyncracies of how the POD file is
 * handled in the original PVRT demo app. This is not normally necessary. Normally,
 * the POD file should be created accurately to reflect the scene.
 */
class IntroducingPODResource : public CC3PODResource
{
	DECLARE_SUPER( CC3PODResource );
public:
	/**
	 * Return a customized light class, to handle the idiosyncracies of the way the original
	 * PVR demo app uses the POD file data. This shouldn't usually be necessary.
	 */
	CC3Light*					buildLightAtIndex( unsigned int lightIndex );

	/**
	 * The PVRT example ignores all but ambient and diffuse material properties from the POD
	 * file and uses default values instead. To duplicate...force other properties to defaults.
	 */
	CC3Material*				buildMaterialAtIndex( unsigned int materialIndex );

	static IntroducingPODResource* resourceFromFile( const std::string& filePath );
};

/**
 * Customized light class to handle the idiosyncracies of how lights from the POD file
 * is handled in the original PVRT demo app. This is not normally necessary. Normally,
 * the POD file should be created accurately to reflect the scene.
 */
class IntroducingPODLight : public CC3PODLight
{
	DECLARE_SUPER( CC3PODLight );
public:
	/** Although the POD file contains direction info, it is ignored in this demo (as in the PVRT example). */
	void						applyDirection();

	/** 
	 * Although the POD file contains light color info, it is ignored in this demo (as in the PVRT example)
	 * and the GL default values are used instead.
	 */
	void						applyColorWithVisitor( CC3NodeDrawingVisitor* visitor );
};

/**
 * A specialized mesh node that tracks its instantaneous global velocity, even when
 * controlled by a CCAction, and even when moved as part of another larger node.
 *
 * After each update, this node compares its previous global location to the current
 * global location, and calculates an instantaneous velocity.
 */
class PhysicsMeshNode : public CC3MeshNode 
{
	DECLARE_SUPER( CC3MeshNode );
public:
	void						initWithTag( GLuint aTag, const std::string& aName );
	/** The global location of this node on the previous update. */
	CC3Vector					getPreviousGlobalLocation();
	/** The current velocity, as calculated during the previous update. */
	CC3Vector					getVelocity();

	/** After the node has been transformed, calculated its new velocity. */
	void						updateAfterTransform( CC3NodeUpdatingVisitor* visitor );

	static PhysicsMeshNode*		nodeWithName( const std::string& aName );

protected:
	CC3Vector					previousGlobalLocation;
	CC3Vector					velocity;
};

/** Simple class that models a door that can be open or closed.  */
class DoorMeshNode : public CC3MeshNode 
{
	DECLARE_SUPER( CC3MeshNode );
public:
	/** Indicates whether the door is open or closed. */
	bool						isOpen();
	void						setIsOpen( bool isOpen );

	void						initWithTag( GLuint aTag, const std::string& aName );
	static DoorMeshNode*		nodeWithName( const std::string& aName );

protected:
	bool						m_isOpen;
};

/**
 * A customized node that automatically rotates by adjusting its rotational aspects on
 * each update pass, and can slow the rotation speed over time based on a friction property.
 *
 * To rotate a node using changes in rotation using the rotateBy... family of methods,
 * as is done to this node, does NOT requre a specialized class. This specialized class
 * is required to handle the freewheeling and friction nature of the behaviour after the
 * rotation has begun.
 */
class SpinningNode : public CC3Node 
{
	DECLARE_SUPER( CC3Node );
public:
	/**
	 * The axis that the cube spins around.
	 *
	 * This is different than the rotationAxis property, because this is the axis around which
	 * a CHANGE in rotation will occur. Depending on how the node is already rotated, this may
	 * be very different than the rotationAxis.
	 */
	CC3Vector					getSpinAxis();
	void						setSpinAxis( const CC3Vector& spinAxis );
	
	/**
	 * The speed of rotation. This value can be directly updated, and then will automatically
	 * be slowed down over time according to the value of the friction property.
	 */
	GLfloat						getSpinSpeed();
	void						setSpinSpeed( GLfloat spinSpeed );
	
	/**
	 * The friction value that is applied to the spinSpeed to slow it down over time.
	 *
	 * A value of zero will not slow rotation down at all and the node will continue
	 * spinning indefinitely.
	 */
	GLfloat						getFriction();
	void						setFriction( GLfloat friction );
	
	/** Indicates whether the node is spinning without direct control by touch events. */
	bool						isFreeWheeling();
	void						setIsFreeWheeling( bool isFreeWheeling );

	void						initWithTag( GLuint aTag, const std::string& aName );

	void						populateFrom( CC3Node* another );

	/**
	 * On each update, if freewheeling, rotate the node around the spinAxis, by an
	 * angle determined by the spinSpeed. Then slow the spinSpeed down based on the
	 * friction value and how long the friction has been applied since the last update.
	 * Stop rotating altogether once the speed is low enough to be unnoticable, so that
	 * we don't continue to perform transforms (and rebuilding shadows) unnecessarily.
	 */
	void						updateBeforeTransform( CC3NodeUpdatingVisitor* visitor );

	static SpinningNode*		nodeWithName( const std::string& aName );

protected:
	CC3Vector					m_spinAxis;
	GLfloat						m_spinSpeed;
	GLfloat						m_friction;
	bool						m_isFreeWheeling;
};

/**
 * LandingCraft is a specialized node that creates and holds an army of other nodes, based on
 * a template node that is repeatedly copied, and the copies are distributed around the scene.
 */
class LandingCraft : public CC3Node
{
public:
	/**
	 * Creates many copies of the specified template node, and places them around the scene.
	 * Each of the copies is independently animated at different speeds using CCActionIntervals,
	 * to demonstrate the individuality of nodes, even though they share the same mesh data.
	 *
	 * The landing motion rains the invading nodes down from the sky, again using CCActionIntervals.
	 * These actions are also independently timed so that the invading nodes drop randomly like rain.
	 */
	void						populateArmyWith( CC3Node* templateNode );

	/** Removes the invasion army by fading them away and then removing them from the scene. */
	void						evaporate();

	static LandingCraft*		nodeWithName( const std::string& aName );
};

///**
// * Category extention to the CCActionManager class to extract the total number of CCActions
// * that are currently running. This is just used to log that number, to demonstrate that
// * the multitude of CCActions are being cleaned up properly when the invading army is removed.
// */
//@interface CCActionManager (LandingCraft)
//
///** The total number of currently running actions. */
//-(uint) numberOfRunningActions;
//
//@end
//

#define kParticlesPerSide		30
#define kParticlesSpacing		40

/**
 * A point particle type that simply hangs where it is located. When the particle is initialized,
 * the location is set from the index, so that the particles are laid out in a simple rectangular
 * grid in the X-Z plane, with kParticlesPerSide particles on each side of the grid. This particle
 * type contains no additional state information.
 */
class HangingPointParticle : public CC3PointParticle
{
	DECLARE_SUPER( CC3PointParticle ); 
public:
	/**
	 * Uses the index of the particle to determine its location relative to the origin of
	 * the emitter. The particles are laid out in a simple rectangular grid in the X-Z plane,
	 * with kParticlesPerSide particles on each side of the grid.
	 *
	 * Each particle is assigned a random color and size.
	 */
	void						initializeParticle();
};

/**
 * A mesh particle type that simply hangs where it is located. When the particle is initialized,
 * the location is set from the index, so that the particles are laid out in a simple rectangular
 * grid in the X-Z plane, with kParticlesPerSide particles on each side of the grid. This particle
 * type contains no additional state information.
 */
class HangingMeshParticle : public CC3ScalableMeshParticle 
{
	DECLARE_SUPER( CC3ScalableMeshParticle );
public:
	/**
	 * The speed of rotation, in degrees per second.
	 *
	 * This initial value is set to a random value during initialization.
	 */
	GLfloat						getRotationSpeed();
	void						setRotationSpeed( GLfloat speed );

	/**
	 * Uses the index of the particle to determine its location relative to the origin of
	 * the emitter. The particles are laid out in a simple rectangular grid in the X-Z plane,
	 * with kParticlesPerSide particles on each side of the grid.
	 */
	void						initializeParticle();

	/** Derive the texture rectangle from the particle index, in a modulus of eight options . */
	CCRect						getTextureRectangle();

	/**
	 * Rotate the particle around the rotation axis that was picked in the initializeParticle method.
	 * To avoid processing rotations and vertices that are not noticable, small rotational changes
	 * are accumultated until the kRotationalUpdateThreshold angle is reached, and then are applied.
	 *
	 * You can experiment with different values for this threshold and observe how it changes the
	 * performance in terms of frames-per-second. A value of zero for the threshold will update the
	 * rotation on every frame.
	 */
	void						updateBeforeTransform( CC3NodeUpdatingVisitor* visitor );

	void						populateFrom( HangingMeshParticle* another );
	virtual CCObject*			copyWithZone( CCZone* zone );

	GLfloat						getAccumulatedAngleChange();
	void						setAccumulatedAngleChange( GLfloat angleChange );

protected:
	GLfloat						rotationSpeed;
	GLfloat						accumulatedAngleChange;
};

/** A mesh particle type that rotates steadily around a random axis, and fades over its lifetime.  */
class RotatingFadingMeshParticle : public CC3UniformlyEvolvingMeshParticle
{
	DECLARE_SUPER( CC3UniformlyEvolvingMeshParticle );
public:
	/** Picks a random rotational axis and rotational velocity, and fades the particle over its lifetime. */
	void						initializeParticle();
};

/**
 * A mesh node whose mesh is created from a text label that is wrapped around the arc of a circle
 * whose center is behind the text. The effect is like a marquee on a round tower.
 *
 * This example demonstrates both the use of bitmapped text labels, and the ability to
 * manipulate the locations of vertices programmatically.
 */
class CylinderLabel : public CC3BitmapLabelNode
{
	DECLARE_SUPER( CC3BitmapLabelNode );
public:
	void						initWithTag( GLuint aTag, const std::string& aName );
	void						populateFrom( CylinderLabel* another );
	virtual CCObject*			copyWithZone( CCZone* zone );

	void						populateAsBitmapFontLabelFromString( const std::string& lblString, const std::string& fontFile, 
		GLfloat lineHt, CCTextAlignment textAlign, const CCPoint& origin, const CC3Tessellation& divsPerChar );

	static CylinderLabel*		nodeWithName( const std::string& aName );

	/**
	 * The radius of the cylinder. This defines the curvature of the text label.
	 *
	 * The initial value is 1000.
	 */
	GLfloat						getRadius();
	void						setRadius( GLfloat radius );

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
	void						wrapTextToArc();

protected:
	GLfloat						radius;
};
