#ifndef _CC3_CC3VECTOR4_H_
#define _CC3_CC3VECTOR4_H_

NS_COCOS3D_BEGIN

/** 
 * A 4D vector, such as a homogeneous vector in 4D graphics matrix space, or a quaternion.
 *
 * This structure can be referenced as containing 4 individual XYZW axes components, 
 * or containing a 3D XYZ vector, plus a W component.
 *
 * Although CC3Vector4 and CC3Quaternion have the same internal structure as GLKVector4 and
 * GLKQuaternion, the structures may have different byte alignment requirements. Avoid casting
 * directly between GLKVector4 and CC3Vector4, or GLKQuaternion and CC3Quaternion, as this is
 * not guaranteed to work reliably. Instead, use the functions CC3Vector4FromGLKVector4,
 * GLKVector4FromCC3Vector4, CC3QuaternionFromGLKQuaternion, and GLKQuaternionFromCC3Quaternion
 * to convert between the two structures.
 *
 * You can, however, reliably copy an array of GLKVector4s or GLKQuaternions to an array of 
 * CC3Vector4s or CC3Quaternions, and vice-versa, by simply using memcpy, or equivalent memory
 * copying function. This is also true of single CC3Vector4, CC3Quaternion, GLKVector4 and 
 * GLKQuaterion structures. Copying is successful because the array or pointer declarations 
 * will ensure the respective byte-alignment requirements, and since the internal structures
 * are identical, the contents of the copy will be identical.
 */
class CC3Vector4
{
public:
	GLfloat x;			/**< The X-componenent of the vector or quaternion. */
	GLfloat y;			/**< The Y-componenent of the vector or quaternion. */
	GLfloat z;			/**< The Z-componenent of the vector or quaternion. */
	GLfloat w;			/**< The homogeneous ratio factor or the real part of a quaternion. */

public:
	inline CC3Vector4( const CC3Vector& vec, float _w )
	{
		this->x = vec.x;
		this->y = vec.y;
		this->z = vec.z;
		this->w = _w;
	}

	inline CC3Vector4()
	{
		this->x = 0.f;
		this->y = 0.f;
		this->z = 0.f;
		this->w = 0.f;
	}

	inline CC3Vector4( float _x, float _y, float _z, float _w )
	{
		this->x = _x;
		this->y = _y;
		this->z = _z;
		this->w = _w;
	}

	/** A CC3Vector4 of zero length at the origin. */
	static const CC3Vector4 kCC3Vector4Zero;

	/** A CC3Vector4 location at the origin. As a definite location, the W component is 1.0. */
	static const CC3Vector4 kCC3Vector4ZeroLocation;

	/** The null CC3Vector4. It cannot be drawn, but is useful for marking an uninitialized vector. */
	static const CC3Vector4 kCC3Vector4Null;

	/** A CC3Quaternion that represents the identity quaternion. */
	static const CC3Vector4 kCC3QuaternionIdentity;

	/** A CC3Vector4 of zero length at the origin. */
	static const CC3Vector4 kCC3QuaternionZero;

	/** The null CC3Quaternion. Useful for marking an uninitialized quaternion. */
	static const CC3Vector4 kCC3QuaternionNull;

	/** Returns a string description of the specified CC3Vector4 struct in the form "(x, y, z, w)" */
	inline std::string stringfy() const
	{
		return CC3String::stringWithFormat( (char*)"(%.3f, %.3f, %.3f, %.3f)", x, y, z, w );
	}

	/** Returns a CC3Vector4 structure constructed from the vector components. */
	static inline CC3Vector4 make( float x, float y, float z, float w ) 
	{
		CC3Vector4 v;
		v.x = x;
		v.y = y;
		v.z = z;
		v.w = w;
		return v;
	}

	/** Returns a CC3Vector4 structure constructed from a 3D vector and a w component. */
	inline CC3Vector4 fromCC3Vector( const CC3Vector& v, GLfloat w) 
	{
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;
		this->w = w;

		return *this;
	}

	/**
	 * Returns a CC3Vector4 homogeneous coordinate constructed from a 3D location.
	 *
	 * The W component of the returned vector is set to 1.0.
	 */
	inline CC3Vector4 fromLocation( const CC3Vector& aLocation )
	{
		return fromCC3Vector( aLocation, 1.f );
	}

	/**
	 * Returns a CC3Vector4 homogeneous coordinate constructed from a 3D direction.
	 *
	 * The W component of the returned vector is set to 0.0.
	 */
	inline CC3Vector4 fromDirection( const CC3Vector& aDirection ) 
	{
		return fromCC3Vector( aDirection, 0.0f );
	}

	/** Returns whether the two vectors are equal by comparing their respective components. */
	inline bool equals( const CC3Vector4& other ) const
	{
		return  FEQUAL(this->w, other.w) && FEQUAL(this->x, other.x) 
			&& FEQUAL(this->y, other.y) && FEQUAL(this->z, other.z);
	}

	/** Returns whether the specified vector is equal to the zero vector, specified by kCC3Vector4Zero. */
	inline bool isZero() const
	{ 
		return this->equals( kCC3Vector4Zero ); 
	}

	/** Returns whether the specified vector is equal to the null vector, specified by kCC3Vector4Null. */
	inline bool isNull() const
	{
		return this->equals( kCC3Vector4Null );
	}

	/**
	 * Returns whether the vector represents a direction, rather than a location.
	 *
	 * It is directional if the w component is zero.
	 */
	inline bool isDirectional() const
	{ 
		return (this->w == 0.0f); 
	}

	/**
	 * Returns whether the vector represents a location, rather than a direction.
	 *
	 * It is locational if the w component is not zero.
	 */
	inline bool isLocational() const
	{ 
		return !isDirectional(); 
	}

	/**
	 * If the specified homogeneous vector represents a location (w is not zero), returns a
	 * homoginized copy of the vector, by dividing each component by the w-component (including
	 * the w-component itself, leaving it with a value of one). If the specified vector is a
	 * direction (w is zero), or is already homogenized (w is one) the vector is returned unchanged.
	 */
	inline CC3Vector4 homogenize() 
	{
		if (this->w == 0.0f || this->w == 1.0f) 
			return *this;

		CC3Vector vec3( x, y, z );
		return CC3Vector4().fromCC3Vector(vec3.scaleUniform(1.0f / this->w), 1.0f);
	}

	/**
	 * Returns a CC3Vector structure constructed from a CC3Vector4. The CC3Vector4 is first
	 * homogenized (via CC3Vector4Homogenize), before copying the resulting x, y & z
	 * coordinates into the CC3Vector.
	 */
	inline CC3Vector homogenizedCC3Vector() 
	{
		CC3Vector4 vec4 = homogenize();
		return CC3Vector( vec4.x, vec4.y, vec4.z );
	}

	inline CC3Vector& cc3Vector()
	{
		return *((CC3Vector*)(&x));
	}

	inline CC3Vector& cc3Vector() const
	{
		return *((CC3Vector*)(&x));
	}

	/** Returns the result of scaling the original vector by the corresponding scale factor uniformly along all axes. */
	inline CC3Vector4 scaleUniform( float scale ) 
	{
		CC3Vector vec3( x, y, z );
		return CC3Vector4().fromCC3Vector( vec3.scaleUniform(scale), this->w * scale );
	}

	inline CC3Vector4 scaleUniform( float scale ) const
	{
		CC3Vector vec3( x, y, z );
		return CC3Vector4().fromCC3Vector( vec3.scaleUniform(scale), this->w * scale );
	}

	/**
	 * Returns the result of scaling the original vector by the corresponding scale
	 * factor uniformly along the X, Y & Z axes. The W component is left unchanged.
	 *
	 * Use this method for scaling 4D homgeneous coordinates.
	 */
	inline CC3Vector4 homogeneousScaleUniform( float scale ) 
	{
		CC3Vector vec3( x, y, z );
		return CC3Vector4().fromCC3Vector( vec3.scaleUniform(scale), this->w );
	}

	/** Returns the dot-product of the two given vectors (v1 . v2). */
	inline float dot( const CC3Vector4& other ) const
	{
		return this->x * other.x + this->y * other.y + this->z * other.z + this->w * other.w;
	}

	/**
	 * Returns the square of the scalar length of the specified vector from the origin, including
	 * the w-component. This is calculated as (x*x + y*y + z*z + w*w) and will always be positive.
	 *
	 * This function is useful for comparing vector sizes without having to run an expensive
	 * square-root calculation.
	 */
	inline float lengthSquared() const
	{ 
		return this->dot( *this ); 
	}

	/**
	 * Returns the scalar length of the specified vector from the origin, including the w-component
	 * This is calculated as sqrt(x*x + y*y + z*z + w*w) and will always be positive.
	 */
	inline GLfloat length() const
	{
		// Avoid expensive sqrt calc if vector is unit length or zero
		float lenSq = lengthSquared();
		return (lenSq == 1.0f || lenSq == 0.0f) ? lenSq : sqrtf(lenSq);
	}

	/** Returns a normalized copy of the specified vector so that its length is 1.0. The w-component is also normalized. */
	inline CC3Vector4 normalize() 
	{
		float lenSq = lengthSquared();
		if (lenSq == 0.0f || lenSq == 1.0f) 
			return *this;

		return this->scaleUniform( 1.0f / sqrtf(lenSq) );
	}

	/** Returns a vector that is the negative of the specified vector in all dimensions, including W. */
	inline CC3Vector4 negate() const
	{ 
		return CC3Vector4( cc3v(-x, -y, -z), -w ); 
	}

	/**
	 * Returns a vector that is the negative of the specified homogeneous
	 * vector in the X, Y & Z axes. The W component is left unchanged.
	 */
	inline CC3Vector4 homogeneousNegate() const
	{
		return CC3Vector4( cc3v(-x, -y, -z), w );
	}

	/**
	 * Returns the result of adding the two specified vectors, by adding the
	 * corresponding components of both vectors.
	 *
	 * If one vector is a location (W=1) and the other is a direction (W=0),
	 * this can be thought of as a translation of the location in that direction.
	 */
	inline CC3Vector4 add( const CC3Vector4& translation ) 
	{
		CC3Vector vec3( x, y, z );
		return CC3Vector4().fromCC3Vector( vec3.add( translation.cc3Vector() ), this->w + translation.w );
	}

	inline CC3Vector4 add( const CC3Vector4& translation ) const
	{
		CC3Vector vec3( x, y, z );
		return CC3Vector4().fromCC3Vector( vec3.add( translation.cc3Vector() ), this->w + translation.w );
	}

	/**
	 * Returns the difference between two vectors, by subtracting the subtrahend from the
	 * minuend, which is accomplished by subtracting each of the corresponding components.
	 *
	 * If both vectors are locations (W=1), the result will be a direction (W=0).
	 */
	inline CC3Vector4 difference( const CC3Vector4& subtrahend ) 
	{
		return CC3Vector4().fromCC3Vector( this->cc3Vector().difference( subtrahend.cc3Vector() ), this->w - subtrahend.w);
	}

	inline CC3Vector4 difference( const CC3Vector4& subtrahend ) const
	{
		return CC3Vector4().fromCC3Vector( this->cc3Vector().difference( subtrahend.cc3Vector() ), this->w - subtrahend.w);
	}

	//////////////////////////////////////////////////////////////////////////
	/// Quaternion special operations
	/**
	 * Returns a quaternion that is the conjugate of the specified quaterion.
	 * The X, Y, Z dimensions are negated, but the W is not.
	 */
	inline CC3Vector4 conjugate() 
	{
		return CC3Vector4().fromCC3Vector( this->cc3Vector().negate(), this->w );;
	}

	/**
	 * Returns the inverse of the specified quaterion.
	 *
	 * The inverse of a quaternion is another quaternion such that multiplying a quaternion by
	 * its inverse results in the identity quaternion.
	 *
	 * The returned inverse quaternion is calculated as the conjugate of the specified quaternion
	 * divided by the square of the length of the specified quaternion.
	 */
	inline CC3Vector4 invert() 
	{
		CC3Vector4 quat = conjugate();
		CC3Vector4 result = quat.scaleUniform(1.0f / lengthSquared());
		return result;
	}

	/**
	 * Returns the result of multiplying qL on the left by qR on the right.
	 *
	 * This represents a concatenation of two rotations, so that a point rotated by the result is
	 * equivalent to the point being rotated first by qL and then by qR: (qL.qR).p == qR.(qL.p).
	 */
	inline CC3Vector4 multiply( const CC3Vector4& qR ) 
	{
		return CC3Vector4((w * x) + (x * qR.w) + (y * qR.z) - (z * qR.y),
								 (w * qR.y) - (x * qR.z) + (y * qR.w) + (z * qR.x),
								 (w * qR.z) + (x * qR.y) - (y * qR.x) + (z * qR.w),
								 (w * qR.w) - (x * qR.x) - (y * qR.y) - (z * qR.z));
	}

	/**
	 * Converts the specified vector that represents an rotation in axis-angle form
	 * to the corresponding quaternion. The X, Y & Z components of the incoming vector
	 * contain the rotation axis, and the W component specifies the angle, in degrees.
	 */
	inline CC3Vector4 fromAxisAngle( const CC3Vector4& axisAngle ) 
	{
		// If q is a quaternion, (rx, ry, rz) is the rotation axis, and ra is
		// the rotation angle (negated for right-handed coordinate system), then:
		// q = ( sin(ra/2)*rx, sin(ra/2)*ry, sin(ra/2)*rz, cos(ra/2) )
	
		GLfloat halfAngle = -CC3DegToRad(axisAngle.w) / 2.0f;		// negate for RH system
		CC3Vector axis = axisAngle.cc3Vector().normalize();
		fromCC3Vector(axis.scaleUniform(sinf(halfAngle)), cosf(halfAngle));

		return *this;
	}

	/**
	 * Converts the specified quaternion to a vector that represents a rotation in
	 * axis-angle form. The X, Y & Z components of the returned vector contain the
	 * rotation axis, and the W component specifies the angle, in degrees.
	 */
	inline CC3Vector4 toAxisAngle() 
	{
		// If q is a quaternion, (rx, ry, rz) is the rotation axis, and ra is
		// the rotation angle (negated for right-handed coordinate system), then:
		// q = ( sin(ra/2)*rx, sin(ra/2)*ry, sin(ra/2)*rz, cos(ra/2) )
		// ra = acos(q.w) * 2
		// (rx, ry, rz) = (q.x, q.y, q.z) / sin(ra/2)
	
		CC3Vector4 q = normalize();
		GLfloat halfAngle = -acosf(q.w);						// Negate to preserve orientation
		GLfloat angle = -CC3RadToDeg(halfAngle) * 2.0f;		// Negate for RH system
	
		// If angle is zero, rotation axis is undefined. Use zero vector.
		CC3Vector axis;
		if (halfAngle != 0.0f)
			axis = q.cc3Vector().scaleUniform( 1.0f / sinf(halfAngle) );
		else
			axis = CC3Vector::kCC3VectorZero;
	
		return CC3Vector4().fromCC3Vector(axis, angle);
	}

	/**
	 * Returns the specified vector rotated by the specified quaternion.
	 *
	 * This uses a highly optimized version of the basic quaternion rotation equation: qvq(-1):
	 *
	 *   v' = v + 2 * cross(cross(v, q.v) + (q.w * v), q.v)
	 *
	 * Derivation of this algorithm can be found in the following Wikipedia article:
	 * http://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation#Performance_comparisons
	 * which describes the algo in terms of a left-handed coordinate system. It has been modified
	 * here for the right-handed coordinate system of OpenGL.
	 *
	 * The right-handed algo is also described here:
	 * http://twistedpairdevelopment.wordpress.com/2013/02/11/rotating-a-vector-by-a-quaternion-in-glsl/
	 *
	 * A related algo can be found derived (for left-handed coordinates) here:
	 * http://mollyrocket.com/forums/viewtopic.php?t=833&sid=3a84e00a70ccb046cfc87ac39881a3d0
	 */
	inline CC3Vector rotateVector( const CC3Vector& v ) 
	{
		CC3Vector crossResult = v.cross( cc3Vector() );
		CC3Vector scaleResult = v.scaleUniform( w );

		crossResult = crossResult.add( scaleResult );
		crossResult = crossResult.cross( cc3Vector() );
		crossResult.scaleUniform( 2.f );

		return v.add( crossResult );
	}

	/**
	 * Converts the specified Euler angle rotational vector to a quaternion.
	 *
	 * The specified rotation vector contains three Euler angles measured in degrees.
	 */
	CC3Vector4 fromRotation( const CC3Vector& aRotation );

	/**
	 * Converts the specified quaternion to a Euler angle rotational vector.
	 *
	 * The returned rotation vector contains three Euler angles measured in degrees.
	 */
	CC3Vector toRotation();

	/**
	 * Returns a spherical linear interpolation between two quaternions, based on the blendFactor.
	 * which should be between zero and one inclusive. The returned quaternion is calculated as
	 * q1 + (blendFactor * (q2 - q1)). If the blendFactor is either zero or one exactly, this
	 * function short-circuits to simply return q1 or q2 respectively.
	 */
	CC3Vector4 slerp( const CC3Vector4& q2, float blendFactor );
};

typedef CC3Vector4 CC3Quaternion;

NS_COCOS3D_END

#endif
