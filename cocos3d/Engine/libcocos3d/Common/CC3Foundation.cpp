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


CC3Vector4 CC3RayIntersectionWithPlane(CC3Ray ray, CC3Plane plane) 
{
	// For a plane defined by v.pn + d = 0, where v is a point on the plane, pn is the normal
	// of the plane and d is a constant, and a ray defined by v(t) = rs + t*rd, where rs is
	// the ray start rd is the ray direction, and t is a multiple, the intersection occurs
	// where the two are equal: (rs + t*rd).pn + d = 0.
	// Solving for t gives t = -(rs.pn + d) / rd.pn
	// The denominator rd.pn will be zero if the ray is parallel to the plane.
	CC3Vector pn = CC3PlaneNormal(plane);
	CC3Vector rs = ray.startLocation;
	CC3Vector rd = ray.direction;
	GLfloat dirDotNorm = CC3VectorDot(rd, pn);

	if (dirDotNorm == 0.0f) 
		return kCC3Vector4Null;		// Ray is parallel to plane, so no intersection

	GLfloat dirDist = -(CC3VectorDot(rs, pn) + plane.d) / CC3VectorDot(rd, pn);
	CC3Vector loc = CC3VectorAdd(rs, CC3VectorScaleUniform(rd, dirDist));
	return CC3Vector4FromCC3Vector(loc, dirDist);
}


CC3Vector CC3TriplePlaneIntersection(CC3Plane p1, CC3Plane p2, CC3Plane p3) 
{
	// For three planes of the form p.n + d = 0, the point of intersection is:
	//    pi = -( d1(n2 x n3) + d2(n3 x n1) + d3(n1 x n2) ) / ((n1 x n2).n3)
	// If the denominator is zero, the planes do not intersect at a single point.

	CC3Vector n1 = CC3PlaneNormal(p1);
	CC3Vector n2 = CC3PlaneNormal(p2);
	CC3Vector n3 = CC3PlaneNormal(p3);

	GLfloat n1xn2dotn3 = CC3VectorDot(CC3VectorCross(n1, n2), n3);
	if (n1xn2dotn3 == 0.0f) return kCC3VectorNull;

	CC3Vector d1n2xn3 = CC3VectorScaleUniform(CC3VectorCross(n2, n3), p1.d);
	CC3Vector d2n3xn1 = CC3VectorScaleUniform(CC3VectorCross(n3, n1), p2.d);
	CC3Vector d3n1xn2 = CC3VectorScaleUniform(CC3VectorCross(n1, n2), p3.d);

	CC3Vector sum = CC3VectorAdd(CC3VectorAdd(d1n2xn3, d2n3xn1), d3n1xn2);

	return CC3VectorScaleUniform(sum, (-1.0f / n1xn2dotn3));
}

CC3Box CC3BoxEngulfLocation(CC3Box bb, CC3Vector aLoc) 
{
	CC3Box bbOut;
	if(CC3BoxIsNull(bb)) {
		bbOut.minimum = aLoc;
		bbOut.maximum = aLoc;
	} else {
		bbOut.minimum.x = MIN(bb.minimum.x, aLoc.x);
		bbOut.minimum.y = MIN(bb.minimum.y, aLoc.y);
		bbOut.minimum.z = MIN(bb.minimum.z, aLoc.z);

		bbOut.maximum.x = MAX(bb.maximum.x, aLoc.x);
		bbOut.maximum.y = MAX(bb.maximum.y, aLoc.y);
		bbOut.maximum.z = MAX(bb.maximum.z, aLoc.z);
	}	
	return bbOut;
}

CC3BarycentricWeights CC3FaceBarycentricWeights(CC3Face face, CC3Vector aLocation) 
{
	CC3BarycentricWeights bcw;
	GLfloat* b = bcw.weights;
	CC3Vector* c = face.vertices;

	// Create basis vectors using the first face corner (index 0) as the origin
	CC3Vector vp = CC3VectorDifference(aLocation, c[0]);
	CC3Vector v1 = CC3VectorDifference(c[1], c[0]);
	CC3Vector v2 = CC3VectorDifference(c[2], c[0]);

	// Create dot products required to solve the two equations and two unknowns
	GLfloat dot11 = CC3VectorDot(v1, v1);
	GLfloat dot12 = CC3VectorDot(v1, v2);
	GLfloat dot1p = CC3VectorDot(v1, vp);
	GLfloat dot22 = CC3VectorDot(v2, v2);
	GLfloat dot2p = CC3VectorDot(v2, vp);

	GLfloat invDenom = 1.0f / (dot11 * dot22 - dot12 * dot12);	// Denominator

	b[1] = (dot22 * dot1p - dot12 * dot2p) * invDenom;
	b[2] = (dot11 * dot2p - dot12 * dot1p) * invDenom;
	b[0] = 1.0f - b[1] - b[2];

	//LogTrace(@"The barycentric weights of location %@ in triangle %@ are %@ using basis vectors",
	//	NSStringFromCC3Vector(aLocation), NSStringFromCC3Face(face), NSStringFromCC3BarycentricWeights(bcw));
	return bcw;
}


void CC3VectorOrthonormalize(CC3Vector* vectors, GLuint vectorCount) 
{
	//LogTrace(@"Vectors BEFORE orthonormalization: %@", NSStringFromCC3Vectors(vectors, vectorCount));

	for (GLuint currIdx = 0; currIdx < vectorCount; currIdx++) 
	{
		// Get the current vector, and subtract any projection from any previously processed vector.
		// (ie- subtract the portion of each previously processed vector that is parallel to this one).
		// Keep the current vector being cleaned separate from the original vector, so
		// keep the projections into the current vector consistent across all previous vectors.
		CC3Vector currVector = vectors[currIdx];
		CC3Vector cleanedCurrVector = currVector;
		for (GLuint prevIdx = 0; prevIdx < currIdx; prevIdx++) {
			CC3Vector prevVector = vectors[prevIdx];
			CC3Vector projPrevVector = CC3VectorScaleUniform(prevVector, CC3VectorDot(currVector, prevVector));
			cleanedCurrVector = CC3VectorDifference(cleanedCurrVector, projPrevVector);
		}
		// Replace the current vector with its orthonormalized version
		vectors[currIdx] = CC3VectorNormalize(cleanedCurrVector);
	}

	//LogTrace(@"Vectors AFTER orthonormalization: %@", NSStringFromCC3Vectors(vectors, vectorCount));
}


CC3Vector4 CC3RayIntersectionWithBoxSide(CC3Ray aRay, CC3Box bb, CC3Vector sideNormal, CC3Vector4 prevHit) 
{
	// Determine which corner to use from the direction of the edge plane normal,
	// create the edge plane, and determine where the ray intersects the edge plane.
	CC3Vector corner = (sideNormal.x + sideNormal.y + sideNormal.z > 0) ? bb.maximum : bb.minimum;
	CC3Plane sidePlane = CC3PlaneFromNormalAndLocation(sideNormal, corner);
	CC3Vector4 sideHit = CC3RayIntersectionWithPlane(aRay, sidePlane);

	// If ray is parallel to edge plane, or if edge plane is behind the ray
	// start, we have no intersection, so return the previous intersection.
	if (sideHit.w < 0.0f || CC3Vector4IsNull(sideHit)) return prevHit;

	// To avoid missed intersections due to rounding errors when checking if the
	// intersection is within the bounding box, force the side plane intersection
	// explicitly onto the appropriate side of the bounding box.
	if (sideNormal.x) sideHit.x = corner.x;
	if (sideNormal.y) sideHit.y = corner.y;
	if (sideNormal.z) sideHit.z = corner.z;

	// If the side intersection location is not within
	// the bounding box, return the previous intersection.
	if ( !CC3BoxContainsLocation(bb, sideHit.v) ) return prevHit;

	// If the ray distance to this side is less than the previous intersection,
	// return this intersection, otherwise return the previous intersection.
	return sideHit.w < prevHit.w ? sideHit : prevHit;
}


CC3Vector  CC3RayIntersectionWithBox(CC3Ray aRay, CC3Box bb) 
{
	if (CC3BoxIsNull(bb)) return kCC3VectorNull;	// Short-circuit null bounding box
	CC3Vector4 closestHit = kCC3Vector4Null;
	closestHit = CC3RayIntersectionWithBoxSide(aRay, bb, kCC3VectorUnitXPositive, closestHit);
	closestHit = CC3RayIntersectionWithBoxSide(aRay, bb, kCC3VectorUnitXNegative, closestHit);
	closestHit = CC3RayIntersectionWithBoxSide(aRay, bb, kCC3VectorUnitYPositive, closestHit);
	closestHit = CC3RayIntersectionWithBoxSide(aRay, bb, kCC3VectorUnitYNegative, closestHit);
	closestHit = CC3RayIntersectionWithBoxSide(aRay, bb, kCC3VectorUnitZPositive, closestHit);
	closestHit = CC3RayIntersectionWithBoxSide(aRay, bb, kCC3VectorUnitZNegative, closestHit);
	return closestHit.v;
}


CC3Vector CC3RayIntersectionWithSphere(CC3Ray aRay, CC3Sphere aSphere)
{
	// Retrieve the quadratic equation that describes the points of interesection.
	CC3Plane eqn = CC3RaySphereIntersectionEquation(aRay, aSphere);
	if (eqn.d < 0.0f) return kCC3VectorNull;	// No intersection if discriminant is negative.

	// There are two roots of the quadratic equation: ((-b +/- sqrt(D)) / 2a),
	// where D is the discriminant (b*b - 4ac). Get the square root of the
	// discriminant to avoid calculating it more than once.
	GLfloat sqrtDisc = sqrtf(eqn.d);

	// Test the smaller root first. If it is negative, then that intersection location
	// is behind the startLocation of the ray. If so, test the second root. If it is
	// negative, then that location is also behind the startLocation of the ray.
	// If the first root is negative and the second root is positive, it's an indication
	// that the startLocation is inside the sphere, and the second location is the exit point.
	GLfloat t = (-eqn.b - sqrtDisc) / (2.0f * eqn.a);
	if (t < 0.0f) t = (-eqn.b + sqrtDisc) / (2.0f * eqn.a);

	// If t is positive, the corresponding intersection location is on the ray.
	// Find that location on the ray as: p = s + tv and return it.
	if (t >= 0.0f) {
		CC3Vector tv = CC3VectorScaleUniform(aRay.direction, t);
		return CC3VectorAdd(aRay.startLocation, tv);
	}

	// Both intersection locations are behind the startLocation of the ray
	return kCC3VectorNull;
}


bool CC3DoesRayIntersectSphere(CC3Ray aRay, CC3Sphere aSphere) 
{
	// Intersection occurs if the discriminant of the quadratic equation that
	// describes the points of intersection between the ray and sphere, is not negative.
	return CC3RaySphereIntersectionEquation(aRay, aSphere).d >= 0.0f;
}


CC3Sphere CC3SphereUnion(CC3Sphere s1, CC3Sphere s2) 
{
	CC3Vector uc, mc, is1, is2, epF, epB;

	// The center of the union sphere will lie on the line between the centers of the
	// two component spheres. We will look for the two end points (epF & epB) that
	// define the diameter of the sphere on that same line. Each endpoint is the
	// farther of the two points where the spheres intersect this center line.
	// The distance is measured from the midpoint of the line between the centers.
	// This comparison is performed twice, once in each direction of the line.

	// Unit vector between the centers
	uc = CC3VectorNormalize(CC3VectorDifference(s2.center, s1.center));

	// The location midpoint between the centers. This is used as an origin
	// for comparing distances along the line that intersects both centers.
	mc = CC3VectorAverage(s1.center, s2.center);

	// Calculate where each sphere intersects the line in the direction of the unit
	// vector between the centers. Then take the intersection point that is farther
	// from the midpoint along this line as the foward endpoint.
	is1 = CC3VectorAdd(s1.center, CC3VectorScaleUniform(uc, s1.radius));
	is2 = CC3VectorAdd(s2.center, CC3VectorScaleUniform(uc, s2.radius));
	epF = (CC3VectorDistanceSquared(is1, mc) > CC3VectorDistanceSquared(is2, mc)) ? is1 : is2;

	// Calculate where each sphere intersects the line in the opposite direction of
	// the unit vector between the centers. Then take the intersection point that is
	// farther from the midpoint along this line as the backward endpoint.
	is1 = CC3VectorDifference(s1.center, CC3VectorScaleUniform(uc, s1.radius));
	is2 = CC3VectorDifference(s2.center, CC3VectorScaleUniform(uc, s2.radius));
	epB = (CC3VectorDistanceSquared(is1, mc) > CC3VectorDistanceSquared(is2, mc)) ? is1 : is2;

	// The resulting union sphere has a center at the midpoint between the two endpoints,
	// and a radius that is half the distance between the two endpoints.
	CC3Sphere rslt;
	rslt.center = CC3VectorAverage(epF, epB);
	rslt.radius = CC3VectorDistance(epF, epB) * 0.5f;
	return rslt;
}


CC3Plane CC3RaySphereIntersectionEquation(CC3Ray aRay, CC3Sphere aSphere) 
{
	// The quadratic intersection equation assumes the sphere is at the origin,
	// so translate the ray to the sphere's reference frame.
	CC3Vector rayStart = CC3VectorDifference(aRay.startLocation, aSphere.center);

	// Calculate the coefficients of the quadratic intersection equation
	GLfloat a = CC3VectorLengthSquared(aRay.direction);
	GLfloat b = 2.0f * CC3VectorDot(rayStart, aRay.direction);
	GLfloat c = CC3VectorLengthSquared(rayStart) - (aSphere.radius * aSphere.radius);

	// Calculate the discriminant of the quadratic solution
	GLfloat d = (b * b) - (4.0f * a * c);

	// Return the coefficients and discriminant as a plane
	//LogTrace(@"Intersection equation for ray %@ and sphere %@: %@",
	//	NSStringFromCC3Ray(aRay), NSStringFromCC3Spere(aSphere),
	//	NSStringFromCC3Plane(CC3PlaneMake(a, b, c, d)));
	return CC3PlaneMake(a, b, c, d);
}


#define kSlerpCosAngleLinearEpsilon 0.01	// about 8 degrees

CC3Quaternion CC3QuaternionSlerp(CC3Quaternion q1, CC3Quaternion q2, GLfloat blendFactor) 
{
	// Short-circuit if we know it's one of the end-points.
	if (blendFactor == 0.0f) return q1;
	if (blendFactor == 1.0f) return q2;

	GLfloat theta, cosTheta, oneOverSinTheta, v1Weight, v2Weight;

	cosTheta = CC3Vector4Dot(q1, q2) / (CC3Vector4Length(q1) * CC3Vector4Length(q2));

	// (Q and −Q map to the same rotation), the rotation path may turn either the "short way"
	// (less than 180°) or the "long way" (more than 180°). Long paths can be prevented by
	// negating one end if the dot product, cos(theta), is negative, thus ensuring that
	// −90° ≤ theta ≤ 90°. Taken from http://en.wikipedia.org/wiki/Slerp
	if (cosTheta < 0.0) { return CC3QuaternionSlerp(q1, CC3QuaternionNegate(q2), blendFactor); }

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
	CC3Vector4 result = CC3QuaternionNormalize(CC3Vector4Add(CC3QuaternionScaleUniform(q1, v1Weight),
		CC3QuaternionScaleUniform(q2, v2Weight)));
	//LogTrace(@"SLERP with cos %.3f at %.3f between %@ and %@ is %@", cosTheta, blendFactor, 
	//	NSStringFromCC3Quaternion(q1), NSStringFromCC3Quaternion(q2),
	//	NSStringFromCC3Quaternion(result));
	return result;
}



void CC3FlipVertically(GLubyte* rowMajorData, GLuint rowCount, GLuint bytesPerRow) 
{
	if ( !rowMajorData ) 
		return;		// If no data, nothing to flip!

	GLubyte* tmpRow = new GLubyte[bytesPerRow];
	GLuint lastRowIdx = rowCount - 1;
	GLuint halfRowCnt = rowCount / 2;
	for (GLuint rowIdx = 0; rowIdx < halfRowCnt; rowIdx++) {
		GLubyte* lowerRow = rowMajorData + (bytesPerRow * rowIdx);
		GLubyte* upperRow = rowMajorData + (bytesPerRow * (lastRowIdx - rowIdx));
		memcpy(tmpRow, upperRow, bytesPerRow);
		memcpy(upperRow, lowerRow, bytesPerRow);
		memcpy(lowerRow, tmpRow, bytesPerRow);
		//LogTrace(@"Swapped %u bytes in %p between row %u at %p and row %u at %p",
		//	bytesPerRow, rowMajorData, rowIdx, lowerRow, (lastRowIdx - rowIdx), upperRow);
	}

	CC_SAFE_DELETE_ARRAY( tmpRow );
}

// Simple function, but too much expanded code to make inline
CC3Vector CC3RotationFromQuaternion(CC3Quaternion aQuaternion) 
{
	CC3Matrix3x3 rotMtx;
	CC3Matrix3x3PopulateFromQuaternion(&rotMtx, aQuaternion);
	return CC3Matrix3x3ExtractRotationYXZ(&rotMtx);
}

// Simple function, but too much expanded code to make inline
CC3Quaternion CC3QuaternionFromRotation(CC3Vector aRotation) 
{
	CC3Matrix3x3 rotMtx;
	CC3Matrix3x3PopulateFromRotationYXZ(&rotMtx, aRotation);
	return CC3Matrix3x3ExtractQuaternion(&rotMtx);
}

NS_COCOS3D_END
