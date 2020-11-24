/*
 * Cocos3D-X 1.0.0
 * Copyright (c) 2014-2015 Jason Wang
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

const CC3Box CC3Box::kCC3BoxZero( 0, 0, 0, 0, 0, 0 );
const CC3Box CC3Box::kCC3BoxNull( FLOAT_INFINITY, FLOAT_INFINITY, FLOAT_INFINITY, FLOAT_INFINITY, FLOAT_INFINITY, FLOAT_INFINITY );

CC3Vector CC3RayIntersectionWithBox( const CC3Ray& aRay, const CC3Box& bb )
{
    if ( bb.isNull() ) return CC3Vector::kCC3VectorNull;	// Short-circuit null bounding box
    CC3Vector4 closestHit = CC3Vector4::kCC3Vector4Null;
    closestHit = CC3RayIntersectionWithBoxSide(aRay, bb, CC3Vector::kCC3VectorUnitXPositive, closestHit);
    closestHit = CC3RayIntersectionWithBoxSide(aRay, bb, CC3Vector::kCC3VectorUnitXNegative, closestHit);
    closestHit = CC3RayIntersectionWithBoxSide(aRay, bb, CC3Vector::kCC3VectorUnitYPositive, closestHit);
    closestHit = CC3RayIntersectionWithBoxSide(aRay, bb, CC3Vector::kCC3VectorUnitYNegative, closestHit);
    closestHit = CC3RayIntersectionWithBoxSide(aRay, bb, CC3Vector::kCC3VectorUnitZPositive, closestHit);
    closestHit = CC3RayIntersectionWithBoxSide(aRay, bb, CC3Vector::kCC3VectorUnitZNegative, closestHit);
    return closestHit.cc3Vector();
}

CC3Vector4 CC3RayIntersectionWithBoxSide( const CC3Ray& aRay, const CC3Box& bb, const CC3Vector& sideNormal, const CC3Vector4& prevHit )
{
    // Determine which corner to use from the direction of the edge plane normal,
    // create the edge plane, and determine where the ray intersects the edge plane.
    CC3Vector corner = (sideNormal.x + sideNormal.y + sideNormal.z > 0) ? bb.maximum : bb.minimum;
    CC3Plane sidePlane = CC3Plane::plane( sideNormal, corner );
    CC3Vector4 sideHit = CC3RayIntersectionWithPlane(aRay, sidePlane);
    
    // If ray is parallel to edge plane, or if edge plane is behind the ray
    // start, we have no intersection, so return the previous intersection.
    if (sideHit.w < 0.0f || sideHit.isNull())
        return prevHit;
    
    // To avoid missed intersections due to rounding errors when checking if the
    // intersection is within the bounding box, force the side plane intersection
    // explicitly onto the appropriate side of the bounding box.
    if (sideNormal.x) sideHit.x = corner.x;
    if (sideNormal.y) sideHit.y = corner.y;
    if (sideNormal.z) sideHit.z = corner.z;
    
    // If the side intersection location is not within
    // the bounding box, return the previous intersection.
    if ( !bb.contains(sideHit.cc3Vector()) ) return prevHit;
    
    // If the ray distance to this side is less than the previous intersection,
    // return this intersection, otherwise return the previous intersection.
    return sideHit.w < prevHit.w ? sideHit : prevHit;
}

NS_COCOS3D_END
