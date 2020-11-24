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

#ifndef _CC3_RAY_H_
#define _CC3_RAY_H_

NS_COCOS3D_BEGIN

/////////////////////////////////////////////////////////////////////////////////////
///             CC3Ray
/**
 * Defines a ray or line in 3D space, by specifying a starting location and direction.
 *
 * For a line, the startLocation can variously be interpreted as the location of any
 * point on the line.
 */
struct CC3Ray
{
    CC3Vector       startLocation;	/**< The location where the ray starts. */
    CC3Vector       direction;		/**< The direction in which the ray points. */
    
    CC3Ray() {  }
    CC3Ray( const CC3Vector& loc, const CC3Vector& dir ) : startLocation(loc), direction(dir) {  }
    CC3Ray( float locX, float locY, float locZ, float dirX, float dirY, float dirZ ) : startLocation(locX, locY, locZ), direction(dirX, dirY, dirZ) {  }
    
    inline void             set( const CC3Vector& loc, const CC3Vector& dir );
    inline bool             passes( const CC3Vector& loc ) const;
    inline std::string      stringfy() const;
};

inline void CC3Ray::set( const CC3Vector &loc, const CC3Vector &dir )
{
    startLocation = loc;
    direction = dir;
}

/** Returns whether the specified location lies on the specified ray. */
inline bool CC3Ray::passes( const CC3Vector &aLocation ) const
{
    // Get a vector from the start of the ray to the location to be tested.
    // Project that vector onto the ray to find the projection of the location
    // onto the ray. If the projected location is the same as the initial
    // location, then the location is on the ray.
    CC3Vector locVect = aLocation.difference( startLocation );
    float proj = locVect.dot( direction );
    CC3Vector projVect = direction * proj;
    CC3Vector projLoc = startLocation + projVect;
    return aLocation.equals( projLoc );
}

/** Returns a string description of the specified CC3Ray struct. */
inline std::string CC3Ray::stringfy() const
{
    return CC3String::stringWithFormat( (char*)"(Start: %s, Towards: %s)",
                                       startLocation.stringfy().c_str(),
                                       direction.stringfy().c_str() );
}

NS_COCOS3D_END

#endif /* _CC3_RAY_H_ */
