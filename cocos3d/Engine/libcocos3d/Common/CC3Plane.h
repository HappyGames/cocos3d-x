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

#ifndef _CC3_PLANE_H_
#define _CC3_PLANE_H_


NS_COCOS3D_BEGIN


/////////////////////////////////////////////////////////////////////////////////////
///             CC3Plane
/** The coefficients of the equation for a plane in 3D space (ax + by + cz + d = 0). */
struct CC3Plane
{
    float       a;	/**< The a coefficient in the planar equation. */
    float       b;	/**< The b coefficient in the planar equation. */
    float       c;	/**< The c coefficient in the planar equation. */
    float       d;	/**< The d coefficient in the planar equation. */
    
    static const CC3Plane kCC3PlaneZero;    /** An undefined plane. */
    
    CC3Plane() : a(0), b(0), c(0), d(0) {  }
    CC3Plane( float _a, float _b, float _c, float _d ) : a(_a), b(_b), c(_c), d(_d) {  }
    
    static CC3Plane         plane( const CC3Vector& normal, const CC3Vector& loc );
    static CC3Plane         planeFromLocations( const CC3Vector& v1, const CC3Vector& v2, const CC3Vector& v3 );
    static CC3Plane         planeFromFace( const CC3Face& face );
    static CC3Plane         negate( const CC3Plane& plane );
    static CC3Plane         normalize( const CC3Plane& plane );
    
    inline CC3Vector        getNormal() const;
    inline std::string      stringfy() const;
    inline bool             equals( const CC3Plane& plane ) const;
    inline bool             isZero() const;
    inline float            distance( const CC3Vector& loc ) const;
    inline bool             isInFront( const CC3Vector& loc ) const;
    inline bool             isInFront( const CC3Vector4& loc ) const;
};

/** Returns a string description of the specified CC3Plane struct in the form "(a, b, c, d)" */
inline std::string CC3Plane::stringfy() const
{
    return CC3String::stringWithFormat( (char*)"(%.3f, %.3f, %.3f, %.3f)", a, b, c, d );
}

/** Returns the normal of the plane, which is (a, b, c) from the planar equation. */
inline CC3Vector CC3Plane::getNormal() const
{
    return *(CC3Vector*)&a;
}

/** Returns a CC3Plane that has the specified normal and intersects the specified location. */
inline CC3Plane CC3Plane::plane( const CC3Vector& n, const CC3Vector& loc )
{
    float d = -loc.dot( n );
    return CC3Plane( n.x, n.y, n.z, d );
}

/**
 * Returns a CC3Plane structure that contains the specified locations.
 *
 * The direction of the normal of the returned plane is dependent on the winding order
 * of the three locations. Winding is done in the order the locations are specified
 * (v1 -> v2 -> v3), and the normal will point in the direction that has the three
 * locations winding in a counter-clockwise direction, according to a right-handed
 * coordinate system. If the direction of the normal is important, be sure to specify
 * the three points in the appropriate order.
 */
inline CC3Plane CC3Plane::planeFromLocations( const CC3Vector& v1, const CC3Vector& v2, const CC3Vector& v3 )
{
    CC3Vector n = v2.difference(v1).cross( v3.difference(v1) ).normalize();
    return plane( n, v1 );
}

/**
 * Returns a CC3Plane structure that contains the points in the specified face.
 *
 * The direction of the normal of the returned plane is dependent on the winding order
 * of the face, which is taken to be vertex[0] -> vertex[1] -> vertex[2], and the normal
 * will point in the direction that has the three points winding in a counter-clockwise
 * direction, according to a right-handed coordinate system. If the direction of the
 * normal is important, be sure the winding order of the points in the face is correct.
 */
inline CC3Plane CC3Plane::planeFromFace( const CC3Face& face )
{
    return planeFromLocations( face.vertices[0], face.vertices[1], face.vertices[2] );
}

/** Returns whether the two planes are equal by comparing their respective components. */
inline bool CC3Plane::equals( const CC3Plane& p2 ) const
{
    return FEQUAL(a, p2.a)
    && FEQUAL(b, p2.b)
    && FEQUAL(c, p2.c)
    && FEQUAL(d, p2.d);
}

/** Returns whether the specified plane is equal to the zero plane, specified by kCC3PlaneZero. */
inline bool CC3Plane::isZero() const
{
    return equals( kCC3PlaneZero );
}

/**
 * Returns a plane that is the negative of the specified plane in all dimensions, including D.
 *
 * The returned plane represents a plane that is coincident with the specified plane,
 * but whose normal points in the opposite direction.
 */
inline CC3Plane CC3Plane::negate( const CC3Plane& p )
{
    return CC3Plane( -p.a, -p.b, -p.c, -p.d );
}

/** Returns a normalized copy of the specified CC3Plane so that the length of its normal (a, b, c) is 1.0 */
inline CC3Plane CC3Plane::normalize( const CC3Plane& p )
{
    float ooNormLen = 1.0f / p.getNormal().length();
    return CC3Plane( p.a * ooNormLen,
                    p.b * ooNormLen,
                    p.c * ooNormLen,
                    p.d * ooNormLen );
}

/**
 * Returns the distance from the specified location to the specified plane.
 *
 * The distance is returned in terms of the length of the normal. If the normal
 * is of unit length, then the distance is in absolute units.
 */
inline float CC3Plane::distance( const CC3Vector& v ) const
{
    return v.dot( getNormal() ) + d;
}

/**
 * Returns whether the specified vector is in front of the specified normalized plane.
 *
 * If the vector is a location, being "in front" means the location is on the side of
 * the plane from which the plane normal points.
 *
 * If the vector is a direction, being "in front" means that the direction points away
 * from the plane on the same side of the plane as the normal points away from the plane.
 */
inline bool CC3Plane::isInFront( const CC3Vector &loc ) const
{
    return ( distance(loc) > 0.0f );
}

/**
 * Returns whether the specified 4D homogeneous vector is in front of the
 * specified normalized plane.
 *
 * If the vector is a location (w = 1), being "in front" means the location
 * is on the side of the plane from which the plane normal points.
 *
 * If the vector is a direction (w = 0), being "in front" means that the
 * direction points away from the plane on the same side of the plane as
 * the normal points away from the plane.
 */
inline bool CC3Plane::isInFront( const CC3Vector4& v ) const
{
    CC3Vector4& vec4 = *(CC3Vector4*)&a;
    return vec4.dot( v ) > 0.0f;
}

/**
 * Returns the location of the point where the specified ray intersects the specified plane.
 *
 * The returned result is a 4D vector, where the x, y & z components give the intersection location
 * in 3D space, and the w component gives the distance from the startLocation of the ray to the
 * intersection location, in multiples of the ray direction vector. If this value is negative, the
 * intersection point is in the direction opposite to the direction of the ray.
 *
 * If the ray is parallel to the plane, no intersection occurs, and the returned 4D vector
 * will be equal to kCC3Vector4Null.
 */
CC3Vector4 CC3RayIntersectionWithPlane(CC3Ray ray, CC3Plane plane);

/**
 * Returns the instersection point of the three specified planes, or returns kCC3VectorNull
 * if the planes do not intersect at a single point, which can occur if the planes are
 * parallel, or if one plane is parallel to the line of intersection of the other two planes.
 */
CC3Vector CC3TriplePlaneIntersection(CC3Plane p1, CC3Plane p2, CC3Plane p3);



NS_COCOS3D_END


#endif /* _CC3_PLANE_H_ */
