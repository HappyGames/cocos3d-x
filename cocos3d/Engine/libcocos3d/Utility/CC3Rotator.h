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
#ifndef _CCL_CC3ROTATOR_H_
#define _CCL_CC3ROTATOR_H_

NS_COCOS3D_BEGIN

/** Enumeration of rotation types. */
typedef enum {
	kCC3RotationTypeUnknown,					/**< Unknown rotation type. */
	kCC3RotationTypeEuler,						/**< Rotation by Euler angles. */
	kCC3RotationTypeQuaternion,					/**< Rotation by quaternion. */
	kCC3RotationTypeAxisAngle,					/**< Rotation by angle around arbirary axis. */
	kCC3RotationTypeDirection,					/**< Rotation by pointing in a specific direction. */
	kCC3RotationTypeLocation,					/**< Rotation by looking at a particular location. */
} CC3RotationType;

/**
 * Enumeration of options for constraining the rotation of a CC3Node when attempting to point
 * at a target node or targetLocation. Targetting can be constrained to use either local or
 * global coordinates, and can be further constrained to rotate only around a single axis.
 */
typedef enum {
	kCC3TargettingConstraintLocalUnconstrained,		/**< Rotate around all axes in the local coordinate system. */
	kCC3TargettingConstraintLocalXAxis,				/**< Rotate only around the X-axis in the local coordinate system. */
	kCC3TargettingConstraintLocalYAxis,				/**< Rotate only around the Y-axis in the local coordinate system. */
	kCC3TargettingConstraintLocalZAxis,				/**< Rotate only around the Z-axis in the local coordinate system. */
	kCC3TargettingConstraintGlobalUnconstrained,	/**< Rotate around all axes in the global coordinate system. */
	kCC3TargettingConstraintGlobalXAxis,			/**< Rotate only around the X-axis in the global coordinate system. */
	kCC3TargettingConstraintGlobalYAxis,			/**< Rotate only around the Y-axis in the global coordinate system. */
	kCC3TargettingConstraintGlobalZAxis,			/**< Rotate only around the Z-axis in the global coordinate system. */
} CC3TargettingConstraint;

/**
 * CC3otator encapsulates the various mechanisms of rotating a node, and converts between them.
 * Nodes delegate responsibility for managing their rotation to an encapsulated instance of CC3Rotator.
 * 
 * Depending on the rotator subclass, rotations can be read in any of the following forms:
 *   - three Euler angles
 *   - rotation angle around an arbitrary rotation axis
 *   - quaternion
 * 
 * This base class represents a read-only rotator and performs only identity rotations.
 * It's primary purpose is to save memory in nodes that do not require any rotation.
 *
 * The CC3MutableRotator class adds the ability to set rotations, and is more commonly used.
 * The CC3DirectionalRotator class further adds directional rotational mechanisms (such as
 * rotating to look in a particular direction), and the CC3TargetttingRotator extends this to
 * rotating to look at a particular target location or node, and optionally track that target
 * location or node as it or the node of the rotator move around.
 */
class CC3Node;
class CC3Rotator : public CCObject
{
public:
	CC3Rotator();
	virtual ~CC3Rotator();

public:
	/**
	 * Indicates whether this rotator supports changing rotation properties, including rotation,
	 * quaternion, rotationAxis, and rotationAngle, and supports incremental rotation through the
	 * rotateBy...family of methods.
	 *
	 * This implementation always returns NO. Subclasses that support changing rotation
	 * properties will override.
	*/
	virtual bool				isMutable();

	/**
	 * Indicates whether this rotator supports rotating to point towards a specific direction
	 * (ie- "look-towards").
	 *
	 * This implementation always returns NO. Subclasses that support pointing towards
	 * a specific direction will override.
	 */
	virtual bool				isDirectional();

	/**
	 * Indicates whether this rotator supports rotating to point towards a specific target node
	 * or target location (ie- "look-at"), including 
	 *
	 * This implementation always returns NO. Subclasses that support targetting
	 * a specific direction will override.
	 */
	virtual bool				isTargettable();

	/**
	 * The rotation matrix derived from the rotation or quaternion properties. Rotation can be
	 * specified in terms of either of these properties, and read by either property, even if set
	 * by the other property. The matrix will reflect the rotational property most recently set.
	 *
	 * The rotation matrix for each instance is local to the node and does not include rotational
	 * information about the node's ancestors.
	 *
	 * This base class always returns nil. Subclasses that support changing rotation will override.
	 */
	virtual CC3Matrix*			getRotationMatrix();

	/**
	 * The rotational orientation of the node in 3D space, relative to the parent of the
	 * node. This value contains three Euler angles, defining a rotation of this node
	 * around the X, Y and Z axes. Each angle is specified in degrees.
	 *
	 * Rotation is performed in Y-X-Z order, which is the OpenGL default. Depending on
	 * the nature of the object you are trying to control, you can think of this order
	 * as yaw, then pitch, then roll, or heading, then inclination, then tilt,
	 *
	 * This base class always returns kCC3VectorZero.
	 */
	virtual CC3Vector			getRotation();

	/**
	 * The rotation of the node in 3D space, relative to the parent of this node,
	 * expressed as a quaternion.
	 *
	 * This base class always returns kCC3QuaternionIdentity.
	 */
	virtual CC3Quaternion		getQuaternion();

	/**
	 * The axis of rotation of the node in 3D space, relative to the parent of this
	 * node, expressed as a directional vector. This axis can be used in conjunction
	 * with the rotationAngle property to describe the rotation as a single angular
	 * rotation around an arbitrary axis.
	 *
	 * This base class always returns kCC3VectorZero.
	 */
	virtual CC3Vector			getRotationAxis();

	/**
	 * The angular rotation around the axis specified in the rotationAxis property.
	 *
	 * This base class always returns zero.
	 */
	virtual GLfloat				getRotationAngle();

	/**
	 * The global location towards which this node is facing.
	 *
	 * Always returns kCC3VectorNull. Subclasses that support target tracking will override.
	 */
	virtual CC3Vector			getTargetLocation();

	/**
	 * Indicates whether rotation should be constrained when attempting to rotate the node to
	 * point at the target or targetLocation.
	 *
	 * Always returns kCC3TargettingConstraintGlobalUnconstrained. Subclasses that support
	 * targetting will override.
	 */
	virtual CC3TargettingConstraint	getTargettingConstraint();

	/**
	 * The target node at which this rotator is pointed.
	 *
	 * Always returns nil. Subclasses that support target tracking will override.
	 */
	virtual CC3Node*			getTarget();

	/**
	 * Indicates whether the node should track the node set in the target
	 * property as the target and the node carrying this rotator move around.
	 *
	 * Always returns NO. Subclasses that support target tracking will override.
	 */
	virtual bool				shouldTrackTarget();

	/**
	 * Returns whether the node should update itself towards the target.
	 *
	 * This implementation always returns NO.
	 * Subclasses that support targets will override.
	 */
	virtual bool				shouldUpdateToTarget();

	/**
	 * Indicates whether the node should automatically find and track the camera
	 * as its target. If this property is set to YES, the node will automatically
	 * find and track the camera without having to set the target and shouldTrackTarget
	 * properties explicitly.
	 *
	 * Always returns NO. Subclasses that support target tracking will override.
	 */
	virtual bool				shouldAutotargetCamera();

	/**
	* Returns whether the node should rotate to face a target location.
	*
	* This implementation always returns NO.
	* Subclasses that support target locations will override.
	*/
	virtual bool				shouldRotateToTargetLocation();

	/**
	 * If the target node of the node carrying this rotator is a CC3Light, the target
	 * can be tracked by the node for the purpose of updating the lighting of a contained
	 * bump-map texture, instead of rotating to face the light, as normally occurs with tracking.
	 *
	 * This property indicates whether the node should update its globalLightPosition
	 * from the tracked location of the light, instead of rotating to face the light.
	 *
	 * Always returns NO. Subclasses that support target tracking will override.
	 */
	virtual bool				isTrackingForBumpMapping();

	/**
	 * Returns whether this rotator updates the target direction by tracking a target.
	 *
	 * Always returns NO. Subclasses that support target tracking will override.
	 */
	virtual bool				isTrackingTargetDirection();

	/**
	 * If the specified node is the target node at which this rotator is pointed,
	 * the target of this rotator is set to nil.
	 *
	 * This method is required in order to be able to clear the target without retrieving it
	 * outside this object to test if it is nil. Since the target is weakly referenced, it may be
	 * deallocated while this rotator still maintains a reference to it. Any subsequent attempt
	 * to retrieve the target reference may result in attempting to retain and autorelease it,
	 * particularly under ARC, which will create a zombie object error.
	 */
	virtual bool				clearIfTarget( CC3Node* node );

	/** Allocates and initializes an autoreleased instance with an identity rotationMatrix. */
	static CC3Rotator*			rotator();
	virtual CCObject*			copyWithZone(CCZone* pZone);

	/**
	 * Template method that populates this instance from the specified other instance.
	 *
	 * This method is invoked automatically during object copying via the copy or
	 * copyWithZone: method. In most situations, the application should use the
	 * copy method, and should never need to invoke this method directly.
	 * 
	 * Subclasses that add additional instance state (instance variables) should extend
	 * copying by overriding this method to copy that additional state. Superclass that
	 * override this method should be sure to invoke the superclass implementation to
	 * ensure that superclass state is copied as well.
	 */
	virtual void				populateFrom( CC3Rotator* another );
	virtual std::string			description();

	/**
	 * Returns a string containing a more complete description of this rotator,
	 * including rotation properties.
	 */
	virtual std::string			fullDescription();

	/**
	 * Rotates the specified direction vector, and returns the transformed direction.
	 */
	virtual CC3Vector			transformDirection( CC3Vector direction );

	virtual bool				init();

	virtual void				applyRotationTo( CC3Matrix* matrix );
};

/**
 * CC3MutableRotator encapsulates the various mechanisms for specifiying rotation,
 * and converts between them.
 * 
 * Rotations can be specified in any of the following methods:
 *   - three Euler angles
 *   - rotation angle around an arbitrary rotation axis
 *   - quaternion
 * Subclasses may also specify other rotational mechanisms (such as pointing).
 *
 *
 * The rotator maintains an internal rotationMatrix, separate from the node's globalTransformMatrix,
 * and the rotator can use this rotationMatrix to convert between different rotational
 * specifications. As such, the rotation of a node can be set using any one of the above
 * specifications, and read back as any of the other specifications.
 */
class CC3MutableRotator : public CC3Rotator
{
	DECLARE_SUPER( CC3Rotator );

public:
	CC3MutableRotator();
	~CC3MutableRotator();

	static CC3MutableRotator*	rotator();

	virtual bool				init();
	virtual bool				isMutable();
	virtual bool				isDirectional();

	/**
	 * The rotation matrix derived from the rotation or quaternion properties. Rotation can be
	 * specified in terms of either of these properties, and read by either property, even if set
	 * by the other property. The matrix will reflect the rotational property most recently set.
	 *
	 * The rotation matrix for each instance is local to the node and does not include rotational
	 * information about the node's ancestors.
	 */
	virtual CC3Matrix*			getRotationMatrix();
	virtual void				setRotationMatrix( CC3Matrix* matrix );

	/**
	 * The rotational orientation of the node in 3D space, relative to the parent of the
	 * node. This value contains three Euler angles, defining a rotation of this node
	 * around the X, Y and Z axes. Each angle is specified in degrees.
	 *
	 * Rotation is performed in Y-X-Z order, which is the OpenGL default. Depending on
	 * the nature of the object you are trying to control, you can think of this order
	 * as yaw, then pitch, then roll, or heading, then inclination, then tilt,
	 *
	 * When setting this value, each component is converted to modulo +/-360 degrees.
	 */
	virtual CC3Vector			getRotation();
	virtual void				setRotation( CC3Vector roation );

	/**
	 * The rotation of the node in 3D space, relative to the parent of this node,
	 * expressed as a quaternion.
	 */
	virtual CC3Quaternion		getQuaternion();
	virtual void				setQuaternion( CC3Quaternion quaternion );

	/**
	 * The axis of rotation of the node in 3D space, relative to the parent of this
	 * node, expressed as a directional vector. This axis can be used in conjunction
	 * with the rotationAngle property to describe the rotation as a single angular
	 * rotation around an arbitrary axis.
	 */
	virtual CC3Vector			getRotationAxis();
	virtual void				setRotationAxis( CC3Vector axis );

	/**
	 * The angular rotation around the axis specified in the rotationAxis property.
	 *
	 * When setting this value, it is converted to modulo +/-360 degrees.
	 */
	virtual GLfloat				getRotationAngle();
	virtual void				setRotationAngle( GLfloat angle );
	
	/** Rotates this rotator from its current state by the specified Euler angles in degrees. */
	void						rotateBy( CC3Vector aRotation );

	/** Rotates this rotator from its current state by the specified quaternion. */
	void						rotateByQuaternion( CC3Quaternion aQuaternion );
	virtual CC3Vector4			getRotationAxisAngle();

	/**
	 * Rotates this rotator from its current state by rotating around
	 * the specified axis by the specified angle in degrees.
	 */
	void						rotateByAngle( GLfloat anAngle, CC3Vector anAxis );

	/**
	 * Indicates whether the rotation matrix is dirty and needs to be recalculated.
	 *
	 * This property is automatically set to YES when one of the rotation properties or operations
	 * have been changed, and is reset to NO once the rotationMatrix has been recalculated.
	 */
	virtual bool				isRotationDirty();

	/**
	 * Indicates that the rotation matrix is dirty and needs to be recalculated.
	 *
	 * This method is invoked automatically as needed. Usually the application never needs
	 * to invoke this method directly.
	 */
	virtual void				markRotationDirty();
	void						markRotationClean();

	/**
	 * When a large number of incremental rotations are applied to a rotator using the rotateBy...
	 * family of methods, accumulated rounding errors can cause the basis vectors of the underlying
	 * rotation matrix to lose mutual orthogonality (no longer be orthogonal to each other), and to
	 * become individually unnormalized (no longer be unit vectors).
	 * 
	 * Although uncommon, it is possible for visible errors to creep into the rotation of this rotator,
	 * after many, many incremental rotations.
	 *
	 * If this happens, you can invoke this method to orthonormalize the basis vectors of the underlying
	 * rotation matrix.
	 *
	 * Instead of keeping track of when to invoke this method within the application, you can also set the
	 * class-side autoOrthonormalizeCount property to have this method automatically invoked periodically.
	 *
	 * Upon completion of this method, each basis vector in the underlying matrix will
	 * be a unit vector that is orthagonal to the other two basis vectors in this matrix.
	 *
	 * Error creep only appears through repeated use of the the rotateBy... family of methods. Error
	 * creep does not occur when the rotation is set explicitly through any of the rotation properties
	 * (rotation, quaternion, rotationAxis/rotationAngle, etc), as these properties populate the
	 * rotation matrix directly in orthonormal form each time then are set. Use of this method is not
	 * needed if rotations have been set directly using these properties, even when set many times.
	 *
	 * This method uses using a Gram-Schmidt process to orthonormalize the basis vectors of the underlying
	 * rotation matrix. The Gram-Schmidt process is biased towards the basis vector chosen to start the
	 * calculation process. To minimize the effect of this, this implementation chooses a different basis
	 * vector to start the orthonormalization process each time this method is invoked, to average the
	 * bias across all basis vectors over time.
	 */
	void						orthonormalize();

	/**
	 * Indicates how often the basis vectors of the underlying rotation matrix should be orthonormalized.
	 *
	 * If this property is set to a value greater than zero, this rotator keeps track of how many times
	 * one of the rotateBy... family of methods of a CC3MutableRotator has been invoked. When that count
	 * reaches the value of this property, the orthonormalize method is invoked to orthonormalize the
	 * underlying matrix, and the count is set back to zero to start the cycle again. See the notes for
	 * the CC3MutableRotator orthonormalize method for a further discussion of orthonormalization.
	 *
	 * If this property is set to zero, orthonormalization will not occur automatically. The application
	 * can invoke the orthonormalize method to cause the rotation matrix to be orthonormalized manually.
	 *
	 * The initial value of this property is zero, indicating that orthonormalization will not occur automatically.
	 */
	static GLubyte				getAutoOrthonormalizeCount();

	/**
	 * Sets how often the basis vectors of the underlying rotation matrix should be orthonormalized.
	 *
	 * If this property is set to a value greater than zero, this rotator keeps track of how many times
	 * one of the rotateBy... family of methods of a CC3MutableRotator has been invoked. When that count
	 * reaches the value of this property, the orthonormalize method is invoked to orthonormalize the
	 * underlying matrix, and the count is set back to zero to start the cycle again. See the notes for
	 * the CC3MutableRotator orthonormalize method for a further discussion of orthonormalization.
	 *
	 * If this property is set to zero, orthonormalization will not occur automatically. The application
	 * can invoke the orthonormalize method to cause the rotation matrix to be orthonormalized manually.
	 *
	 * The initial value of this property is zero, indicating that orthonormalization will not occur automatically.
	 */
	static void					setAutoOrthonormalizeCount( GLubyte aCount );
	void						autoOrthonormalize();
	 

	/** Initializes this instance to use the specified matrix as the rotationMatrix. */
	virtual void				initOnRotationMatrix( CC3Matrix* aMatrix );

	/** Allocates and initializes an autoreleased instance to use the specified matrix as the rotationMatrix. */
	static CC3MutableRotator*	rotatorOnRotationMatrix( CC3Matrix* aMatrix );

	CC3Vector4					getRotationVector();
	GLubyte						getRotationType();
	GLubyte						getOrthonormalizationStartColumnNumber();
	GLubyte						getIncrementalRotationCount();

	virtual void				populateFrom( CC3MutableRotator* another );
	virtual CCObject*			copyWithZone(CCZone* pZone);

	virtual std::string			fullDescription();

	virtual void				applyRotation();
	virtual void				applyRotationTo( CC3Matrix* matrix );
	virtual CC3Vector			transformDirection( CC3Vector direction );

protected:
	CC3Matrix*					m_rotationMatrix;
	CC3Vector4					m_rotationVector;
	GLubyte						m_incrementalRotationCount;
	GLubyte						m_rotationType : 4;
	GLubyte						m_orthonormalizationStartColumnNumber : 2;
	GLubyte						m_targettingConstraint : 4;					// For CC3TargettingRotator subclass
	bool						m_shouldReverseForwardDirection : 1;		// For CC3DirectionalRotator subclass
	bool						m_isNewTarget : 1;							// For CC3TargettingRotator subclass
	bool						m_shouldTrackTarget : 1;					// For CC3TargettingRotator subclass
	bool						m_shouldAutotargetCamera : 1;				// For CC3TargettingRotator subclass
	bool						m_isTrackingForBumpMapping : 1;				// For CC3TargettingRotator subclass
	bool						m_isTargetLocationDirty : 1;				// For CC3TargettingRotator subclass
};

/**
 * CC3DirectionalRotator is a subclass of CC3MutableRotator that adds the ability to set
 * rotation based on directional information.
 * 
 * In addition to specifying rotations in terms of three Euler angles, a rotation axis and a
 * rotation angle, or a quaternion, rotations of this class can be specified in terms of pointing
 * in a particular forwardDirection, and orienting so that 'up' is in a particular referenceUpDirection.
 *
 * The rotationMatrix of this rotator can be used to convert between directional rotation, Euler
 * angles, and quaternions. As such, the rotation of a node can be specified as a quaternion or
 * a set of Euler angles, and then read back as a fowardDirection, upDirection, and rightDirection.
 * Or, conversely, rotation may be specified by pointing to a particular forwardDirection and
 * referenceUpDirection, and then read as a quaternion or a set of Euler angles.
 *
 * The shouldReverseForwardDirection property can be used to determine whether rotation should
 * rotate the negative-Z-axis of the local coordinate system to point in the forwardDirection,
 * or should rotate the positive-Z-axis to the forwardDirection.
 */
class CC3DirectionalRotator : public CC3MutableRotator
{
public:
	DECLARE_SUPER( CC3MutableRotator );

	static CC3DirectionalRotator* rotator();

	/**
	* Indicates whether this rotator supports rotating to point towards a specific direction
	* (ie- "look-towards").
	*
	* This implementation always returns YES.
	*/
	virtual bool				isDirectional();

	/**
	 * The direction towards which this node is pointing, relative to the parent of the node.
	 *
	 * A valid direction vector is required. Attempting to set this property
	 * to the zero vector (kCC3VectorZero) will raise an assertion error.
	 *
	 * See the discussion in the notes of the same property in CC3Node for more info.
	 *
	 * The initial value of this property is kCC3VectorUnitZPositive.
	 */
	CC3Vector					getForwardDirection();
	void						setForwardDirection( CC3Vector direction );

	/**
	 * Indicates whether the effect of setting the forwardDirection property should be reversed.
	 *
	 * Based on the OpenGL default orientation, setting the forwardDirection of a node rotates
	 * the node to align the negative-Z-axis of the node with the defined forward direction.
	 * 
	 * Setting this property to YES will invert that behaviour so that positive-Z-axis of the
	 * node will be aligned with the defined forwardDirection.
	 * 
	 * The initial value of this property is NO, indicating that the negative-Z-axis of the node
	 * will be aligned with the forwardDirection property.
	 */
	bool						shouldReverseForwardDirection();
	void						setShouldReverseForwardDirection( bool revserse );

	/**
	 * The direction that is considered to be 'up'.
	 *
	 * A valid direction vector is required. Attempting to set this property
	 * to the zero vector (kCC3VectorZero) will raise an assertion error.
	 *
	 * See the discussion in the notes of the same property in CC3Node for more info.
	 *
	 * The initial value of this property is kCC3VectorUnitYPositive.
	 */
	CC3Vector					getReferenceUpDirection();
	void						setReferenceUpDirection( CC3Vector direction );

	/**
	 * The direction, in the local coordinate system, that is considered to be 'up'. This corresponds
	 * to the referenceUpDirection, after it has been transformed by the rotationMatrix of this instance.
	 *
	 * See the discussion in the notes of the same property in CC3Node for more info.
	 *
	 * The initial value of this property is kCC3VectorUnitYPositive.
	 */
	CC3Vector					getUpDirection();

	/* The direction in the local coordinate system that is considered to be "off to the right"
	 * relative to the forwardDirection and upDirection.
	 * 
	 * See the discussion in the notes of the same property in CC3Node for more info.
	 *
	 * The initial value of this property is kCC3VectorUnitXPositive.
	 */
	CC3Vector					getRightDirection();

	/** Initializes this instance to use the specified matrix as the rotationMatrix. */
	virtual void				initOnRotationMatrix( CC3Matrix* aMatrix );

	/*Make compiler happier*/
	virtual void				populateFrom( CC3MutableRotator* another );
	virtual void				populateFrom( CC3DirectionalRotator* another );
	virtual CCObject*			copyWithZone(CCZone* pZone);

	virtual void				applyRotation();
	std::string					fullDescription();

protected:
	CC3Vector					m_referenceUpDirection;
};

/**
 * CC3TargettingRotator is a subclass of CC3DirectionalRotator that can automatically track
 * the location of another node, or a specific location in 3D space.
 * 
 * In addition to specifying rotations in terms of three Euler angles, a rotation axis and a
 * rotation angle, a quaternion, or a direction, rotations of this class can be specified in
 * terms of pointing at a specific target location in space, or at a specific target node.
 * Further, the rotator can optionally be configured to track that target location or node
 * as the target node, or the node using this rotator, move.
 */
class CC3TargettingRotator : public CC3DirectionalRotator
{
	DECLARE_SUPER( CC3DirectionalRotator );
public:
	CC3TargettingRotator();
	~CC3TargettingRotator();

	static CC3TargettingRotator* rotator();

	/**
	 * Indicates whether this rotator supports rotating to point towards a specific target node
	 * or target location (ie- "look-at"), including 
	 *
	 * This implementation always returns YES.
	 */
	virtual bool				isTargettable();

	/** 
	 * The global location towards which this node is facing.
	 *
	 * The target location is determined by the node and is cached by the directional rotator.
	 */
	CC3Vector					getTargetLocation();
	void						setTargetLocation( CC3Vector loc );
	bool						isDirtyByTargetLocation();

	/**
	 * Indicates whether rotation should be constrained when attempting to rotate the node to
	 * point at the target or targetLocation.
	 *
	 * The initial value of this property is kCC3TargettingConstraintGlobalUnconstrained.
	 */
	virtual CC3TargettingConstraint getTargettingConstraint();
	virtual void				setTargettingConstraint( CC3TargettingConstraint constraint );

	/**
	 * Rotates to look at the specified target location as viewed from the specified eye location,
	 * and orienting with reference to the specifie up direction. The direction, and both locations
	 * are specified in the local coordinate system.
	 *
	 * This is the classic LookAt rotational function.
	 */
	void						rotateToTargetLocation( CC3Vector targLoc, CC3Vector eyeLoc, CC3Vector upDir );

	/**
	 * The target node at which this rotator is pointed. If the shouldTrackTarget property
	 * is set to YES, the node will track the target so that it always points to the
	 * target, regardless of how the target and this node move through the 3D scene.
	 *
	 * The target is held as a weak reference. If you destroy the target node, you must
	 * remove it as the target of this rotator.
	 */
	virtual CC3Node*			getTarget();
	virtual void				setTarget( CC3Node* target );

	/**
	 * Indicates whether the node should track the node set in the target property
	 * as the target and the node carrying this rotator move around.
	 *
	 * This initial value of this property is NO.
	 */
	virtual bool				shouldTrackTarget();
	virtual void				setShouldTrackTarget( bool track );

	/**
	 * Indicates whether the node should automatically find and track the camera
	 * as its target. If this property is set to YES, the node will automatically
	 * find and track the camera without having to set the target and shouldTrackTarget
	 * properties explicitly.
	 *
	 * This initial value of this property is NO.
	 */
	virtual bool				shouldAutotargetCamera();
	virtual void				setShouldAutotargetCamera( bool bAuto );

	/**
	 * Returns whether the node should update itself towards the target.
	 *
	 * Returns YES if the target property is set and the shouldTrackTarget returns YES.
	 */
	virtual bool				shouldUpdateToTarget();

	/**
	 * Returns whether this node should rotate to face the targetLocation.
	 * It will do so if it is not tracking for bump-mapping purposes, and the 
	 * targetDirection was just set, or shouldTrackTarget is YES.
	 */
	bool						shouldRotateToTargetLocation();

	/**
	 * If the target node of the node carrying this rotator is a CC3Light, the target
	 * can be tracked by the node for the purpose of updating the lighting of a contained
	 * bump-map texture, instead of rotating to face the light, as normally occurs with tracking.
	 * 
	 * This property indicates whether the node should update its globalLightPosition
	 * from the tracked location of the light, instead of rotating to face the light.
	 *
	 * The initial property is set to NO.
	 */
	virtual bool				isTrackingForBumpMapping();
	virtual void				setIsTrackingForBumpMapping( bool isBM );

	/**
	 * Returns whether this rotator updates the target direction by tracking a target.
	 *
	 * Returns YES if this rotator has a target node, the shouldTrackTarget property is
	 * set to YES, and the isTrackingForBumpMapping property is set to NO.
	 */
	bool						isTrackingTargetDirection();

	virtual bool				clearIfTarget( CC3Node* node );
	virtual void				initOnRotationMatrix( CC3Matrix* aMatrix );

	/*Make compiler happier*/
	virtual void				populateFrom( CC3MutableRotator* another );
	virtual void				populateFrom( CC3TargettingRotator* another );
	virtual CCObject*			copyWithZone(CCZone* pZone);

	virtual std::string			fullDescription();

protected:
	bool						isNewTarget();

protected:
	CC3Node*					m_pTarget;
};

NS_COCOS3D_END

#endif

