#ifndef _CC3_CC3QUATERNION_H_
#define _CC3_CC3QUATERNION_H_

NS_COCOS3D_BEGIN

class CC3Quaternion : public CC3Vector4
{
public:
	/** A CC3Quaternion that represents the identity quaternion. */
	static const CC3Quaternion kCC3QuaternionIdentity;

	/** A CC3Vector4 of zero length at the origin. */
	static const CC3Quaternion kCC3QuaternionZero;

	/** The null CC3Quaternion. Useful for marking an uninitialized quaternion. */
	static const CC3Quaternion kCC3QuaternionNull;

public:
	inline CC3Quaternion() : CC3Vector4()
	{

	}

	inline CC3Quaternion( float _x, float _y, float _z, float _w )
		: CC3Vector4( _x, _y, _z, _w )
	{

	}

	/**
	 * Returns a quaternion that is the conjugate of the specified quaterion.
	 * The X, Y, Z dimensions are negated, but the W is not.
	 */
	inline CC3Quaternion conjugate() 
	{
		CC3Vector4 result = CC3Quaternion().fromCC3Vector( this->cc3Vector().negate(), this->w );

		return *(CC3Quaternion*)&result;
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
	inline CC3Quaternion invert() 
	{
		CC3Quaternion quat = conjugate();
		CC3Vector4 result = quat.scaleUniform(1.0f / lengthSquared());
		return *(CC3Quaternion*)&result;
	}

	/**
	 * Returns the result of multiplying qL on the left by qR on the right.
	 *
	 * This represents a concatenation of two rotations, so that a point rotated by the result is
	 * equivalent to the point being rotated first by qL and then by qR: (qL.qR).p == qR.(qL.p).
	 */
	inline CC3Quaternion multiply( const CC3Quaternion& qR ) 
	{
		return CC3Quaternion((w * x) + (x * qR.w) + (y * qR.z) - (z * qR.y),
								 (w * qR.y) - (x * qR.z) + (y * qR.w) + (z * qR.x),
								 (w * qR.z) + (x * qR.y) - (y * qR.x) + (z * qR.w),
								 (w * qR.w) - (x * qR.x) - (y * qR.y) - (z * qR.z));
	}

	/**
	 * Converts the specified vector that represents an rotation in axis-angle form
	 * to the corresponding quaternion. The X, Y & Z components of the incoming vector
	 * contain the rotation axis, and the W component specifies the angle, in degrees.
	 */
	inline CC3Quaternion fromAxisAngle( const CC3Vector4& axisAngle ) 
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
	 * Converts the specified Euler angle rotational vector to a quaternion.
	 *
	 * The specified rotation vector contains three Euler angles measured in degrees.
	 */
	CC3Quaternion fromRotation( const CC3Vector& aRotation );

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
	CC3Quaternion slerp( const CC3Quaternion& q2, float blendFactor );

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

};

NS_COCOS3D_END

#endif
