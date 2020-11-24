//
//  CC3Collision.cpp
//  Cocos3d
//
//  Created by Jason Wang on 19/12/2016.
//  Copyright © 2016 http://www.cocos3dx.org/. All rights reserved.
//
#include "cocos3d.h"

NS_COCOS3D_BEGIN

bool CC3Collision::testMovingSphereSphere( CC3Sphere s0, CC3Sphere s1, const CC3Vector& v0, const CC3Vector& v1 )
{
    // Expand sphere s1 by the radius of s0
    s1.radius += s0.radius;
    
    // Subtract movement of s1 from both s0 and s1, making s1 stationary
    CC3Vector v = v0 - v1;
    
    // Can now test directed segment s = s0.c + tv, v = (v0-v1)/||v0-v1|| against
    // the expanded sphere for intersection
    float vlen = v.length();
    
    CC3Ray ray( s0.center, v / vlen );
    CC3Vector q;
    float t = 0.f;
    if ( testRaySphere(ray, s1, t, q) )
        return t <= vlen;
    
    return false;
}

bool CC3Collision::testMovingSpherePlane( CC3Sphere s, const CC3Vector& v, const CC3Plane& p, float &t, CC3Vector &q )
{
    CC3Vector normal = p.getNormal();
    // Compute distance of sphere center to plane
    float dist = p.distance( s.center );
    
    if ( fabs(dist) <= s.radius )
    {
        // The sphere is already overlapping the plane. Set time of
        // intersection to zero and q to sphere center
        t = 0.0f;
        q = s.center;
        CC3_TRACE( "intersecting" );
        return true;
    }
    else
    {
        float denom = normal.dot( v );
        if (denom * dist >= 0.0f)
        {
            // No intersection as sphere moving parallel to or away from plane
            return false;
        }
        else
        {
            // Sphere is moving towards the plane
            // Use +r in computations if sphere in front of plane, else -r
            float r = dist > 0.0f ? s.radius : -s.radius;
            t = (r - dist) / denom;
            q = s.center + v * t - normal * r;
            
            CC3_TRACE( "curent t value: %.4f, max t is %.4f, and speed is %.2f", t, s.radius / v.length(), v.length() );
            // return t <= kCC3FloatEpsilon;
            // return t <= s.radius / v.length();
            return true;
        }
    }
}

bool CC3Collision::testRaySphere( const CC3Ray& ray, CC3Sphere s, float &t, CC3Vector &q )
{
    CC3Vector m = ray.startLocation - s.center;
    float b = m.dot( ray.direction );
    float c = m.dot( m ) - s.radius * s.radius;
    // Exit if r’s origin outside s (c > 0) and r pointing away from s (b > 0)
    if (c > 0.0f && b > 0.0f)
        return false;
    
    float discr = b * b - c;
    // A negative discriminant corresponds to ray missing sphere
    if ( discr < 0.0f )
        return false;
    
    // Ray now found to intersect sphere, compute smallest t value of intersection
    t = -b - sqrtf( discr );
    
    // If t is negative, ray started inside sphere so clamp t to zero
    if (t < 0.0f)
        t = 0.0f;
    
    q = ray.startLocation + ray.direction * t;
    
    return true;
}

NS_COCOS3D_END

