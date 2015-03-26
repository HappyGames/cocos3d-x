#ifndef _CC3_CC3VECOTR_H_
#define _CC3_CC3VECOTR_H_

NS_COCOS3D_BEGIN

/** 
 * A vector in 3D space. 
 *
 * Although CC3Vector has the same internal structure as GLKVector3, the structures may 
 * have different byte alignment requirements. Avoid casting directly between GLKVector3 
 * and CC3Vector, as this is not guaranteed to work reliably. Instead, use the functions
 * CC3VectorFromGLKVector3 and GLKVector3FromCC3Vector to convert between the two structures.
 *
 * You can, however, reliably copy an array of GLKVector3s to an array of CC3Vectors, and 
 * vice-versa, by simply using memcpy, or equivalent memory copying function. This is also 
 * true of single CC3Vector and GLKVector3 structures. Copying is successful because the 
 * array or pointer declarations will ensure the respective byte-alignment requirements, and
 * since the internal structures are identical, the contents of the copy will be identical.
 */
class CC3Vector
{
public:
	float			x;
	float			y;
	float			z;

	/// Common constants
	/** A CC3Vector of zero length at the origin. */
	static const CC3Vector	kCC3VectorZero;

	/** The null CC3Vector. It cannot be drawn, but is useful for marking an uninitialized vector. */
	static const CC3Vector	kCC3VectorNull;

	/** A CC3Vector with each component equal to one, representing the diagonal of a unit cube. */
	static const CC3Vector	kCC3VectorUnitCube;

	/** The diagonal length of a unit cube. */
	static const GLfloat	kCC3VectorUnitCubeLength;

	/** Unit vector pointing in the same direction as the positive X-axis. */
	static const CC3Vector	kCC3VectorUnitXPositive;

	/** Unit vector pointing in the same direction as the positive Y-axis. */
	static const CC3Vector	kCC3VectorUnitYPositive;

	/** Unit vector pointing in the same direction as the positive Z-axis. */
	static const CC3Vector	kCC3VectorUnitZPositive;

	/** Unit vector pointing in the same direction as the negative X-axis. */
	static const CC3Vector	kCC3VectorUnitXNegative;

	/** Unit vector pointing in the same direction as the negative Y-axis. */
	static const CC3Vector	kCC3VectorUnitYNegative;

	/** Unit vector pointing in the same direction as the negative Z-axis. */
	static const CC3Vector	kCC3VectorUnitZNegative;

public:
	inline CC3Vector() : x(0.f), y(0.f), z(0.f) {  }
	inline CC3Vector( float _x, float _y, float _z ) : x(_x), y(_y), z(_z) {  }

	std::string		stringfy();
	std::string		stringfy() const;

	/** Returns whether the two vectors are equal by comparing their respective components. */
	bool			equals( const CC3Vector& other );
	bool			equals( const CC3Vector& other ) const;

	/** Returns whether the specified vector is equal to the zero vector, specified by kCC3VectorZero. */
	bool			isZero();

	/** Returns whether the specified vector is equal to the null vector, specified by kCC3VectorNull. */
	bool			isNull();

	/**
	 * Returns the result of scaling the original vector by the corresponding scale vector.
	 * Scaling can be different for each axis. This has the effect of multiplying each component
	 * of the vector by the corresponding component in the scale vector.
	 */
	CC3Vector		scale( const CC3Vector& scl );

	/**
	 * Returns the result of scaling the original vector by the corresponding scale
	 * factor uniformly along all axes.
	 */
	CC3Vector		scaleUniform( float scl );
	CC3Vector		scaleUniform( float scl ) const;

	/**
	 * Returns a vector that is the negative of the specified vector in all directions.
	 * For vectors that represent directions, the returned vector points in the direction
	 * opposite to the original.
	 */
	CC3Vector		negate();
	CC3Vector		negate() const;

	/**
	 * Returns a vector whose components comprise the minimum value of each of the respective
	 * components of the two specfied vectors. In general, do not expect this method to return
	 * one of the specified vectors, but a new vector, each of the components of which is the
	 * minimum value for that component between the two vectors.
	 */
	CC3Vector		minimize( const CC3Vector& other );

	/**
	 * Returns a vector whose components comprise the maximum value of each of the respective
	 * components of the two specfied vectors. In general, do not expect this method to return
	 * one of the specified vectors, but a new vector, each of the components of which is the
	 * maximum value for that component between the two vectors.
	 */
	CC3Vector		maxmize( const CC3Vector& other );

	/** Returns the dot-product of the two given vectors (v1 . v2). */
	float			dot( const CC3Vector& other );
	float			dot( const CC3Vector& other ) const;

	/**
	 * Returns the square of the scalar length of the specified CC3Vector from the origin.
	 * This is calculated as (x*x + y*y + z*z) and will always be positive.
	 *
	 * This function is useful for comparing vector sizes without having to run an
	 * expensive square-root calculation.
	 */
	float			lengthSquared();
	float			lengthSquared() const;

	/**
	 * Returns the scalar length of the specified CC3Vector from the origin.
	 * This is calculated as sqrt(x*x + y*y + z*z) and will always be positive.
	 */
	float			length();

	/**
	 * Returns a normalized copy of the specified CC3Vector so that its length is 1.0.
	 * If the length is zero, the original vector (a zero vector) is returned.
	 */
	CC3Vector		normalize();
	CC3Vector		normalize() const;

	/**
	 * Returns a CC3Vector that is the inverse of the specified vector in all directions,
	 * such that scaling the original by the inverse using CC3VectorScale will result in
	 * a vector of unit dimension in each direction (1.0, 1.0, 1.0). The result of this
	 * function is effectively calculated by dividing each component of the original
	 * vector into 1.0 (1.0/x, 1.0/y, 1.0/z). It is the responsibility of the caller to
	 * ensure that none of the components of the original is zero.
	 */
	CC3Vector		invert();

	/**
	 * Returns the result of adding the two specified vectors, by adding the corresponding components 
	 * of both vectors. This can also be thought of as a translation of the first vector by the second.
	 */
	CC3Vector		add( const CC3Vector& other );
	CC3Vector		add( const CC3Vector& other ) const;

	/**
	 * Returns the difference between two vectors, by subtracting the subtrahend from the minuend,
	 * which is accomplished by subtracting each of the corresponding x,y,z components.
	 */
	CC3Vector		difference( const CC3Vector& other );
	CC3Vector		difference( const CC3Vector& other ) const;

	/**
	 * Returns a modulo version of the specifed rotation,
	 * so that each component is between (+/-360 degrees).
	 */
	CC3Vector		rotationModulo();

	/**
	 * Returns the difference between two rotation vectors, in terms of the minimal degrees,
	 * along each axis, required to travel between the two roations, given that rotations
	 * are cyclical with a period of 360 degrees. The result may be positive or negative,
	 * but will always be between (+/-180 degrees).
	 *
	 * For example, the difference between 350 and 10 will yield -20 (ie- the smallest change
	 * from 10 degrees to 350 degrees is -20 degrees) rather than +340 (from simple subtraction).
	 * Similarly, the difference between 10 and 350 will yield +20 (ie- the smallest change from
	 * 350 degrees to 10 degrees is +20 degrees) rather than -340 (from simple subtraction).
	 */
	CC3Vector		rotationalDifference( const CC3Vector& other );

	/** Returns the positive scalar distance between the ends of the two specified vectors. */
	float			distance( const CC3Vector& other );
	float			distance( const CC3Vector& other ) const;

	/**
	 * Returns the square of the scalar distance between the ends of the two specified vectors.
	 *
	 * This function is useful for comparing vector distances without having to run an
	 * expensive square-root calculation.
	 */
	float			distanceSquared( const CC3Vector& other );

	/**
	 * Returns a vector that represents the average of the two specified vectors. This is
	 * calculated by adding the two specified vectors and scaling the resulting sum vector by half.
	 *
	 * The returned vector represents the midpoint between a line that joins the endpoints
	 * of the two specified vectors.
	 */
	CC3Vector		average( const CC3Vector& other );

	/** Returns the cross-product of the two given vectors (v1 x v2). */
	CC3Vector		cross( const CC3Vector& other );

	/** Returns YES if the two vectors are either exactly parallel or exactly antiparallel. */
	bool			isParallelWith( const CC3Vector& other );

	/** Returns whether the two vectors are exactly perpendicular. */
	bool			isPerpendicularTo( const CC3Vector& other );

	/**
	 * Returns a linear interpolation between two vectors, based on the blendFactor.
	 * which should be between zero and one inclusive. The returned value is calculated
	 * as v1 + (blendFactor * (v2 - v1)). If the blendFactor is either zero or one
	 * exactly, this method short-circuits to simply return v1 or v2 respectively.
	 */
	CC3Vector		lerp( const CC3Vector& other, float blendFactor );
};

//////////////////////////////////////////////////////////////////////////
/// CC3Vector IMPLEMENTATION
inline std::string CC3Vector::stringfy()
{
	return CC3String::stringWithFormat( (char*)"(%.3f, %.3f, %.3f)", this->x, this->y, this->z );
}

inline std::string CC3Vector::stringfy() const
{
	return CC3String::stringWithFormat( (char*)"(%.3f, %.3f, %.3f)", this->x, this->y, this->z );
}

inline bool CC3Vector::equals( const CC3Vector& other )
{
	return this->x == other.x &&
		this->y == other.y &&
		this->z == other.z;
}

inline bool CC3Vector::equals( const CC3Vector& other ) const
{
	return this->x == other.x &&
		this->y == other.y &&
		this->z == other.z;
}

inline bool CC3Vector::isZero()
{
	return equals( kCC3VectorZero );
}

inline bool CC3Vector::isNull()
{
	return equals( kCC3VectorNull );
}

inline CC3Vector CC3Vector::scale( const CC3Vector& scl )
{
	return CC3Vector( this->x * scl.x,
		this->y * scl.y,
		this->z * scl.z );
}

inline CC3Vector CC3Vector::scaleUniform( float scl )
{
	return CC3Vector( this->x * scl,
		this->y * scl,
		this->z * scl );
}

inline CC3Vector CC3Vector::scaleUniform( float scl ) const
{
	return CC3Vector( this->x * scl,
		this->y * scl,
		this->z * scl );
}

inline CC3Vector CC3Vector::negate()
{
	return CC3Vector( -this->x, -this->y, -this->z );
}

inline CC3Vector CC3Vector::negate() const
{
	return CC3Vector( -this->x, -this->y, -this->z );
}

inline CC3Vector CC3Vector::minimize( const CC3Vector& other )
{
	return CC3Vector( MIN(this->x, other.x),
				MIN(this->y, other.y),
				MIN(this->z, other.z) );
}

inline CC3Vector CC3Vector::maxmize( const CC3Vector& other )
{
	return CC3Vector( MAX(this->x, other.x),
		MAX(this->y, other.y),
		MAX(this->z, other.z) );
}

inline float CC3Vector::dot( const CC3Vector& other )
{
	return ( (this->x * other.x) +
			(this->y * other.y) +
			(this->z * other.z) );
}

inline float CC3Vector::dot( const CC3Vector& other ) const
{
	return ( (this->x * other.x) +
		(this->y * other.y) +
		(this->z * other.z) );
}

inline float CC3Vector::lengthSquared()
{
	return dot( *this );
}

inline float CC3Vector::lengthSquared() const
{
	return dot( *this );
}

inline float CC3Vector::length()
{
	// Avoid expensive sqrt calc if vector is unit length or zero
	float lenSq = lengthSquared();
	return (lenSq == 1.0f || lenSq == 0.0f) ? lenSq : sqrtf(lenSq);
}

inline CC3Vector CC3Vector::normalize()
{
	GLfloat lenSq = lengthSquared();
	if ( lenSq == 0.0f || lenSq == 1.0f ) 
		return *this;

	return scaleUniform( (1.0f / sqrtf(lenSq)) );
}

inline CC3Vector CC3Vector::normalize() const
{
	GLfloat lenSq = lengthSquared();
	if ( lenSq == 0.0f || lenSq == 1.0f ) 
		return *this;

	return scaleUniform( (1.0f / sqrtf(lenSq)) );
}

inline CC3Vector CC3Vector::invert() 
{
	return CC3Vector( 1.0f / this->x,
				1.0f / this->y,
				1.0f / this->z );
}

inline CC3Vector CC3Vector::add( const CC3Vector& translation ) 
{
	return CC3Vector( this->x + translation.x,
				this->y + translation.y,
				this->z + translation.z );
}

inline CC3Vector CC3Vector::add( const CC3Vector& translation ) const
{
	return CC3Vector( this->x + translation.x,
		this->y + translation.y,
		this->z + translation.z );
}

inline CC3Vector CC3Vector::difference( const CC3Vector& subtrahend ) 
{
	return CC3Vector( this->x - subtrahend.x,
				this->y - subtrahend.y,
				this->z - subtrahend.z );
}

inline CC3Vector CC3Vector::difference( const CC3Vector& subtrahend ) const
{
	return CC3Vector( this->x - subtrahend.x,
		this->y - subtrahend.y,
		this->z - subtrahend.z );
}

inline CC3Vector CC3Vector::rotationModulo()
{
	return CC3Vector( CC3CyclicAngle(this->x),
				CC3CyclicAngle(this->y),
				CC3CyclicAngle(this->z) );
}

inline CC3Vector CC3Vector::rotationalDifference( const CC3Vector& subtrahend ) 
{
	return CC3Vector( CC3SemiCyclicAngle(this->x - subtrahend.x),
				CC3SemiCyclicAngle(this->y - subtrahend.y),
				CC3SemiCyclicAngle(this->z - subtrahend.z) );
}

inline float CC3Vector::distance( const CC3Vector& end ) 
{
	CC3Vector thisEnd = end;
	return thisEnd.difference( *this ).length();
}

inline float CC3Vector::distance( const CC3Vector& end ) const
{
	CC3Vector thisEnd = end;
	return thisEnd.difference( *this ).length();
}

inline float CC3Vector::distanceSquared( const CC3Vector& end ) 
{
	CC3Vector thisEnd = end;
	return thisEnd.difference( *this ).lengthSquared();
}

inline CC3Vector CC3Vector::average( const CC3Vector& other ) 
{
	return add( other ).scaleUniform( 0.5f );
}

/** Returns the cross-product of the two given vectors (v1 x v2). */
inline CC3Vector CC3Vector::cross( const CC3Vector& other )
{
	return CC3Vector(this->y * other.z - this->z * other.y,
		this->z * other.x - this->x * other.z,
		this->x * other.y - this->y * other.x);
}

/** Returns YES if the two vectors are either exactly parallel or exactly antiparallel. */
inline bool CC3Vector::isParallelWith( const CC3Vector& other ) 
{
	return ( cross( other ).lengthSquared() == 0.0f );
}

inline bool CC3Vector::isPerpendicularTo( const CC3Vector& other ) 
{
	return ( dot( other ) == 0.0f );
}

inline CC3Vector CC3Vector::lerp( const CC3Vector& other, float blendFactor)
{
	if ( blendFactor == 0.0f ) 
		return *this;
	if ( blendFactor == 1.0f ) 
		return other;

	CC3Vector tmp = other;
	return tmp.difference( *this ).scaleUniform( blendFactor ).add( *this );
}

/** 
 * Minimum acceptable absolute value for a scale transformation component.
 *
 * This is used to ensure that scales used in transforms do not cause uninvertable matrices.
 *
 * The initial value is 1.0e-9f. Set this to another value if appropriate.
 */
static GLfloat kCC3ScaleMin = 1.0e-9f;

/**
 * Ensures the specified value can be used as a component in a scale vector. If the value is
 * greater than kCC3ScaleMin or less than -kCC3ScaleMin, it is returned unchanced, otherwise
 * either -kCC3ScaleMin or kCC3ScaleMin is returned, depending on whether the value is
 * less than zero or not, respectively.
 *
 * This is used to ensure that scales used in transforms do not cause uninvertable matrices.
 */
static inline GLfloat CC3EnsureMinScaleAxis(GLfloat val) 
{
	// Test in order of expected value, for fast return.
	if (val > kCC3ScaleMin) return val;
	if (val >= 0.0f) return kCC3ScaleMin;
	if (val < -kCC3ScaleMin) return val;
	return -kCC3ScaleMin;
}

/**
 * Ensures the absolute value of each of the components in the specified scale vector
 * is greater than kCC3ScaleMin. Any component between -kCC3ScaleMin and kCC3ScaleMin
 * is replaced with -kCC3ScaleMin or kCC3ScaleMin depending on whether the component
 * is less than zero or not, respectively.
 *
 * This can be used to ensure that scales used in transforms do not cause uninvertable matrices.
 */
static inline CC3Vector CC3EnsureMinScaleVector(CC3Vector scale) 
{
	return CC3Vector( CC3EnsureMinScaleAxis(scale.x),
				CC3EnsureMinScaleAxis(scale.y),
				CC3EnsureMinScaleAxis(scale.z) );
}

/** Returns a CC3Vector structure constructed from the vector components. */
static inline CC3Vector CC3VectorMake(GLfloat x, GLfloat y, GLfloat z)
{
	CC3Vector v;
	v.x = x;
	v.y = y;
	v.z = z;
	return v;
}

/** Convenience alias macro to create CC3Vectors with less keystrokes. */
#define cc3v(X,Y,Z) CC3VectorMake((X),(Y),(Z))

NS_COCOS3D_END

#endif
