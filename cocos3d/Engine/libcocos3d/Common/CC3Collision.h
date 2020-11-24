//
//  CC3Collision.h
//  Cocos3d
//
//  Created by Jason Wong on 19/12/2016.
//  Copyright © 2016 http://www.cocos3dx.org/. All rights reserved.
//
#ifndef _CC3_COLLISION_H_
#define _CC3_COLLISION_H_

NS_COCOS3D_BEGIN

class CC3Collision
{
public:
    /**
     * Returns whether two moving spheres are intersecting or not
     * with sphere s0 moving with velocity v0, and sphere s1 moving with velocity v1
     */
    static bool testMovingSphereSphere( CC3Sphere s0, CC3Sphere s1, const CC3Vector& v0, const CC3Vector& v1 );
    
    /**
     * Intersect sphere s with movement vector v with plane p. If intersecting
     * return time t of collision and point q at which sphere hits plane
     */
    static bool testMovingSpherePlane( CC3Sphere s, const CC3Vector& v, const CC3Plane& p, float &t, CC3Vector &q );
    
    /**
     * Intersects ray r with sphere s and, if intersecting,
     * returns t value of intersection and intersection point q
     */
    static bool testRaySphere( const CC3Ray& r, CC3Sphere s, float &t, CC3Vector &q );
};

NS_COCOS3D_END

#endif /* _CC3_COLLISION_H_ */
