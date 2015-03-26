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

	/** Returns a string description of the specified CC3Vector4 struct in the form "(x, y, z, w)" */
	inline std::string stringfy() 
	{
		return CC3String::stringWithFormat( (char*)"(%.3f, %.3f, %.3f, %.3f)", x, y, z, w );
	}

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
	inline bool equals( const CC3Vector4& other ) 
	{
		return  (this->w == other.w) && this->x == other.x && this->y == other.y && this->z == other.z;
	}

	/** Returns whether the two vectors are equal by comparing their respective components. */
	inline bool equals( const CC3Vector4& other ) const
	{
		return  (this->w == other.w) && this->x == other.x && this->y == other.y && this->z == other.z;
	}

	/** Returns whether the specified vector is equal to the zero vector, specified by kCC3Vector4Zero. */
	inline bool isZero() 
	{ 
		return this->equals( kCC3Vector4Zero ); 
	}

	/** Returns whether the specified vector is equal to the null vector, specified by kCC3Vector4Null. */
	inline bool isNull() 
	{
		return this->equals( kCC3Vector4Null ); 
	}

	/**
	 * Returns whether the vector represents a direction, rather than a location.
	 *
	 * It is directional if the w component is zero.
	 */
	inline bool isDirectional() 
	{ 
		return (this->w == 0.0f); 
	}

	/**
	 * Returns whether the vector represents a location, rather than a direction.
	 *
	 * It is locational if the w component is not zero.
	 */
	inline bool isLocational() 
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
	inline float dot( const CC3Vector4& other ) 
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
	inline float lengthSquared() 
	{ 
		return this->dot( *this ); 
	}

	/**
	 * Returns the scalar length of the specified vector from the origin, including the w-component
	 * This is calculated as sqrt(x*x + y*y + z*z + w*w) and will always be positive.
	 */
	inline GLfloat length() 
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
	inline CC3Vector4 negate() 
	{ 
		return CC3Vector4( cc3v(-x, -y, -z), -w ); 
	}

	/**
	 * Returns a vector that is the negative of the specified homogeneous
	 * vector in the X, Y & Z axes. The W component is left unchanged.
	 */
	inline CC3Vector4 homogeneousNegate() 
	{
		return CC3Vector4( cc3v(-x, -y, -z), w );
	}

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
};

NS_COCOS3D_END

#endif
