#include "cocos3d.h"

NS_COCOS3D_BEGIN

const CC3Quaternion CC3Quaternion::kCC3QuaternionIdentity ( 0.0, 0.0, 0.0, 1.0 );
const CC3Quaternion CC3Quaternion::kCC3QuaternionZero ( 0.0, 0.0, 0.0, 0.0 );
const CC3Quaternion CC3Quaternion::kCC3QuaternionNull ( INFINITY, INFINITY, INFINITY, INFINITY );

// Simple function, but too much expanded code to make inline
CC3Quaternion CC3Quaternion::fromRotation( const CC3Vector& aRotation )
{
	CC3Matrix3x3 rotMtx;
	CC3Matrix3x3PopulateFromRotationYXZ(&rotMtx, aRotation);

	CC3Quaternion quat = CC3Matrix3x3ExtractQuaternion(&rotMtx);
	return *((CC3Quaternion*)(&quat));
}

// Simple function, but too much expanded code to make inline
CC3Vector CC3Quaternion::toRotation() 
{
	CC3Quaternion aQuaternion = *((CC3Quaternion*)this);
	CC3Matrix3x3 rotMtx;
	CC3Matrix3x3PopulateFromQuaternion(&rotMtx, aQuaternion);
	return CC3Matrix3x3ExtractRotationYXZ(&rotMtx);
}


#define kSlerpCosAngleLinearEpsilon 0.01	// about 8 degrees

CC3Quaternion CC3Quaternion::slerp( const CC3Quaternion& q2, float blendFactor ) 
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
		CC3Vector4 negateQuat = q2.negate();
		CC3Quaternion quat = *(CC3Quaternion*)&negateQuat;
		return slerp( quat, blendFactor); 
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
	return *(CC3Quaternion*)&result;
}

NS_COCOS3D_END
