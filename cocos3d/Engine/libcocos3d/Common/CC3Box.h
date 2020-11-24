/*
 * Cocos3D-X 1.0.0
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

#ifndef _CC3_BOX_H_
#define _CC3_BOX_H_

NS_COCOS3D_BEGIN

/////////////////////////////////////////////////////////////////////////////////////
///             CC3Box
/**
 * Defines an axially-aligned-box or bounding-box (AABB), describing
 * a 3D volume by specifying the minimum and maximum 3D corners.
 */
struct CC3Box
{
    CC3Vector       minimum;			/**< The minimum corner (bottom-left-rear). */
    CC3Vector       maximum;			/**< The maximum corner (top-right-front). */
    
    // Constants
    /** A CC3Box of zero origin and dimensions. */
    static const CC3Box     kCC3BoxZero;
    /** The null bounding box. It cannot be drawn, but is useful for marking an uninitialized bounding box. */
    static const CC3Box     kCC3BoxNull;
    
    CC3Box() {  }
    CC3Box( const CC3Vector& min, const CC3Vector& max ) : minimum(min), maximum(max) {  }
    CC3Box( float minX, float minY, float minZ,
           float maxX, float maxY, float maxZ ) : minimum(minX, minY, minZ), maximum(maxX, maxY, maxZ) {  }
    
    inline bool         equals( const CC3Box& other ) const;
    inline bool         isZero() const;
    inline bool         isNull() const;
    inline CC3Vector    getCenter() const;
    inline CC3Vector    getSize() const;
    inline bool         contains( const CC3Vector& loc ) const;
    inline CC3Box       boxEngulfLocation( const CC3Vector& loc ) const;
    inline CC3Box       boxUnion( const CC3Box& box ) const;
};


/** Returns whether the two bounding boxes are equal by comparing their respective components. */
inline bool CC3Box::equals( const CC3Box& other ) const
{
    return minimum.equals( other.minimum )
    && maximum.equals( other.maximum );
}

/**
 * Returns whether the specified bounding box is equal to
 * the zero bounding box, specified by kCC3BoxZero.
 */
inline bool CC3Box::isZero() const
{
    return this->equals( kCC3BoxZero );
}

/**
 * Returns whether the specified bounding box is equal to
 * the null bounding box, specified by kCC3BoxNull.
 */
inline bool CC3Box::isNull() const
{
    // return minimum.isNull() && maximum.isNull();
    return this->equals( kCC3BoxNull );
}

/** Returns the geometric center of the specified bounding box. */
inline CC3Vector CC3Box::getCenter() const
{
    return ( minimum + maximum ) * 0.5f;
}

/** Returns the size of the specified bounding box, between the minimum to maximum corners. */
inline CC3Vector CC3Box::getSize() const
{
    return maximum - minimum;
}

/** Returns whether the specified bounding box contains the specified location. */
inline bool CC3Box::contains( const CC3Vector &aLoc ) const
{
    if (aLoc.x > maximum.x) return false;
    if (aLoc.x < minimum.x) return false;
    if (aLoc.y > maximum.y) return false;
    if (aLoc.y < minimum.y) return false;
    if (aLoc.z > maximum.z) return false;
    if (aLoc.z < minimum.z) return false;
    
    return true;
}

/**
 * Returns the smallest CC3Box that contains both the specified bounding box
 * and location. If the specified bounding box is null, returns a bounding box of zero
 * size at the specified location.
 */
inline CC3Box CC3Box::boxEngulfLocation( const CC3Vector& aLoc ) const
{
    CC3Box bbOut;
    if ( this->isNull() )
    {
        bbOut.minimum = aLoc;
        bbOut.maximum = aLoc;
    }
    else
    {
        bbOut.minimum.x = MIN(minimum.x, aLoc.x);
        bbOut.minimum.y = MIN(minimum.y, aLoc.y);
        bbOut.minimum.z = MIN(minimum.z, aLoc.z);
        
        bbOut.maximum.x = MAX(maximum.x, aLoc.x);
        bbOut.maximum.y = MAX(maximum.y, aLoc.y);
        bbOut.maximum.z = MAX(maximum.z, aLoc.z);
    }
    
    return bbOut;
}

/**
 * Returns the smallest CC3Box that contains the two specified bounding boxes.
 * If either bounding box is the null bounding box, simply returns the other bounding box
 * (which may also be the null bounding box).
 */
inline CC3Box CC3Box::boxUnion( const CC3Box& bb2 ) const
{
    if( this->isNull() ) return bb2;
    if( bb2.isNull() ) return *this;
    
    CC3Box box;
    box = this->boxEngulfLocation( bb2.minimum );
    box = box.boxEngulfLocation( bb2.maximum );
    
    return box;
}

/**
 * Returns a bounding box that has the same dimensions as the specified bounding box, but with
 * each corner expanded outward by the specified amount of padding.
 *
 * The padding value is added to the maximum vector, and subtracted from the minimum vector.
 */
static inline CC3Box CC3BoxAddPadding( const CC3Box& bb, const CC3Vector& padding )
{
    CC3Box bbPadded;
    bbPadded.maximum = bb.maximum.add( padding );
    bbPadded.minimum = bb.minimum.difference( padding );
    return bbPadded;
}

/**
 * Returns a box that has the same dimensions as the specified  box, but with
 * each corner expanded outward by the specified amount of padding.
 *
 * The padding value is added to all three components of the maximum vector, and subtracted
 * from all three components of the minimum vector.
 */
static inline CC3Box CC3BoxAddUniformPadding( const CC3Box& bb, float padding )
{
    return (padding != 0.0f) ? CC3BoxAddPadding(bb, cc3v(padding, padding, padding)) : bb;
}

/** Returns a box constructed by translating the specified box by the specified translation offset. */
static inline CC3Box CC3BoxTranslate( const CC3Box& bb, const CC3Vector& offset )
{
    CC3Box bbXltd;
    bbXltd.maximum = bb.maximum.add( offset );
    bbXltd.minimum = bb.minimum.add( offset );
    return bbXltd;
}

/**
 * Returns a box constructed by translating the specified box by a translation that is a
 * fraction of the box size, as determined by the specified offsetScale. For example, a
 * value of -0.2 in the X-component of the offsetScale, will move the box 20% of its
 * dimension in the X-axis, in the direction of the negative X-axis.
 */
static inline CC3Box CC3BoxTranslateFractionally( const CC3Box& bb, const CC3Vector& offsetScale )
{
    return CC3BoxTranslate( bb, bb.getSize().scale(offsetScale) );
}

/**
 * Returns a bounding box of the same size as the specified bounding box, and whose center
 * lies at the origin of the coordinate system. This effectivly moves the bounding box so
 * that its center is at the origin.
 */
static inline CC3Box CC3BoxMoveCenterToOrigin( const CC3Box& bb )
{
    return CC3BoxTranslate( bb, bb.getCenter().negate() );
}

/**
 * Returns a bounding box constructed by scaling the specified bounding box by the specified
 * scale value. Scaling can be different along each axis of the box.
 *
 * This has the effect of multiplying each component of each of the vectors representing the
 * minimum and maximum corners of the box by the corresponding component in the scale vector.
 */
static inline CC3Box CC3BoxScale( const CC3Box& bb, const CC3Vector& scale )
{
    CC3Box bbScaled;
    bbScaled.maximum = bb.maximum.scale( scale );
    bbScaled.minimum = bb.minimum.scale( scale );
    return bbScaled;
}

/**
 * Returns a bounding box constructed by scaling the specified bounding box by the specified
 * scale value. The same scaling is applied to each axis of the box.
 *
 * This has the effect of multiplying each component of each of the vectors representing the
 * minimum and maximum corners of the box by the scale value.
 */
static inline CC3Box CC3BoxScaleUniform( const CC3Box& bb, GLfloat scale )
{
    CC3Box bbScaled;
    bbScaled.maximum = bb.maximum * scale;
    bbScaled.minimum = bb.minimum * scale;
    return bbScaled;
}

/**
 * Returns the location that the specified ray intersects the specified bounding box,
 * on the side of the bounding box that has the specified normal, but only if the
 * intersection distance is less than the specified previous intersection location.
 *
 * The distance measurement is specified in the W component of the returned 4D vector.
 *
 * If the ray does not intersect the specified side of the bounding box, if the side
 * is behind the ray, or if the intersection distance is larger than that for the
 * specified previous intersection location is returned.
 *
 * This method first creates the plane on which the side exists, finds the intersection
 * of the ray and that plane, determines whether the intersection location is actually
 * within the bounding box, and then tests whether the intersection distance is less
 * than for the specified previous intersection.
 */
CC3Vector4 CC3RayIntersectionWithBoxSide( const CC3Ray& aRay, const CC3Box& bb, const CC3Vector& sideNormal, const CC3Vector4& prevHit );

/**
 * Returns the location that the specified ray intersects the specified bounding box,
 * or returns kCC3VectorNull if the ray does not intersect the bounding box, or the
 * bounding box is behind the ray.
 *
 * The result takes into consideration the startLocation of the ray, and will return
 * kCC3VectorNull if the bounding box is behind the startLocation, even if the line
 * projecting back through the startLocation in the negative direction of the ray
 * intersects the bounding box.
 *
 * The ray may start inside the bounding box, and the exit location of the ray will be returned.
 */
CC3Vector CC3RayIntersectionWithBox( const CC3Ray& aRay, const CC3Box& bb );

NS_COCOS3D_END

#endif /* _CC3_BOX_H_ */
