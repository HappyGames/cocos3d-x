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

#ifndef _CC3_CCRANGE_H_
#define _CC3_CCRANGE_H_

NS_COCOS3D_BEGIN

struct CCRange
{
    unsigned long location;
    unsigned long length;
 
    // Constructors
    CCRange()
        : location(0), length(0)      {  }
    
    CCRange( unsigned long loc, unsigned long len )
        : location(loc), length(len)  {  }

    
    inline unsigned long    maxRange() const;
    inline bool             isInRange( unsigned long loc ) const;
    inline CCRange          getIntersection( const CCRange& otherRange );
    inline CCRange          unionRange( const CCRange& otherRange );
};

inline unsigned long CCRange::maxRange() const
{
    return location + length;
}

inline bool CCRange::isInRange( unsigned long loc ) const
{
    return loc >= location && loc < maxRange();
}

inline CCRange CCRange::getIntersection( const CCRange& otherRange )
{
    unsigned long min, loc, max1 = maxRange(), max2 = otherRange.maxRange();
    CCRange result;
    
    min = ( max1 < max2 ) ? max1 : max2;
    loc = ( location > otherRange.location ) ? location : otherRange.location;
    
    if ( min < loc )
    {
        result.location = result.length = 0;
    }
    else
    {
        result.location = loc;
        result.length = min - loc;
    }
    
    return result;
}

inline CCRange CCRange::unionRange( const CCRange& otherRange )
{
    unsigned long max, loc, max1 = maxRange(), max2 = otherRange.maxRange();
    CCRange result;
    
    max = ( max1 > max2 ) ? max1 : max2;
    loc = ( location < otherRange.location ) ? location : otherRange.location;
    
    result.location = loc;
    result.length = max - result.location;
    
    return result;
}

/// Helpers
static inline CCRange CCRangeMake( unsigned long location, unsigned long length )
{
    CCRange range( location, length );
    return range;
}

#define ccr( a, b ) CCRangeMake( (a), (b) )

NS_COCOS3D_END

#endif /* _CC3_CCRANGE_H_ */
