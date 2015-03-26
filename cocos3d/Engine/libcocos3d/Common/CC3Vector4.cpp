#include "cocos3d.h"

NS_COCOS3D_BEGIN

const CC3Vector4 CC3Vector4::kCC3Vector4Zero( CC3Vector(0.0, 0.0, 0.0), 0.0 );
const CC3Vector4 CC3Vector4::kCC3Vector4ZeroLocation( CC3Vector(0.0, 0.0, 0.0), 1.0 );
const CC3Vector4 CC3Vector4::kCC3Vector4Null( CC3Vector::kCC3VectorNull, INFINITY );

const CC3Vector4 CC3Vector4::kCC3QuaternionIdentity ( 0.0, 0.0, 0.0, 1.0 );
const CC3Vector4 CC3Vector4::kCC3QuaternionZero ( 0.0, 0.0, 0.0, 0.0 );
const CC3Vector4 CC3Vector4::kCC3QuaternionNull ( INFINITY, INFINITY, INFINITY, INFINITY );

// Simple function, but too much expanded code to make inline
CC3Vector4 CC3Vector4::fromRotation( const CC3Vector& aRotation )
{
	CC3Matrix3x3 rotMtx;
	CC3Matrix3x3PopulateFromRotationYXZ(&rotMtx, aRotation);

	return CC3Matrix3x3ExtractQuaternion(&rotMtx);
}

// Simple function, but too much expanded code to make inline
CC3Vector CC3Vector4::toRotation() 
{
	CC3Matrix3x3 rotMtx;
	CC3Matrix3x3PopulateFromQuaternion(&rotMtx, *this);
	return CC3Matrix3x3ExtractRotationYXZ(&rotMtx);
}


#define kSlerpCosAngleLinearEpsilon 0.01	// about 8 degrees

CC3Vector4 CC3Vector4::slerp( const CC3Vector4& q2, float blendFactor ) 
{
	// Short-circuit if we know it's one of the end-points.
	if (blendFactor == 0.0f) return *this;
	if (blendFactor == 1.0f) return q2;

	GLfloat theta, cosTheta, oneOverSinTheta, v1Weight, v2Weight;

	cosTheta = this->dot( q2 ) / (length() * q2.length());

	// (Q and −Q map to the same rotation), the rotation path may turn either the "short way"
	// (less than 180°) or the "long way" (more than 180°). Long paths can be prevented by
	// negating one end if the dot product, cos(theta), is negative, thus ensuring that
	// −90° ≤ theta ≤ 90°. Taken from http://en.wikipedia.org/wiki/Slerp
	if (cosTheta < 0.0) 
	{ 
		return slerp( q2.negate(), blendFactor); 
	}

	// If angle close to zero (cos() close to one), save cycles by interpolating linearly
	if ((1.0 - cosTheta) < kSlerpCosAngleLinearEpsilon) {
		v1Weight = 1.0f - blendFactor;
		v2Weight = blendFactor;
	} else {
		theta = acosf(cosTheta);
		oneOverSinTheta = 1.0f / sinf(theta);
		v1Weight = (sinf(theta * (1.0f - blendFactor)) * oneOverSinTheta);
		v2Weight = (sinf(theta * blendFactor) * oneOverSinTheta);
	}
	CC3Vector4 result = this->scaleUniform(v1Weight).add( q2.scaleUniform(v2Weight) ).normalize();
	//LogTrace(@"SLERP with cos %.3f at %.3f between %@ and %@ is %@", cosTheta, blendFactor, 
	//	NSStringFromCC3Quaternion(q1), NSStringFromCC3Quaternion(q2),
	//	NSStringFromCC3Quaternion(result));
	return result;
}


NS_COCOS3D_END
