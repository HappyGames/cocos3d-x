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

#include "cocos3d.h"

NS_COCOS3D_BEGIN

const CC3Face CC3Face::kCC3FaceZero( cc3v(0, 0, 0), cc3v(0, 0, 0), cc3v(0, 0, 0) );

CC3BarycentricWeights CC3FaceBarycentricWeights(CC3Face face, CC3Vector aLocation)
{
    CC3BarycentricWeights bcw;
    GLfloat* b = bcw.weights;
    CC3Vector* c = face.vertices;
    
    // Create basis vectors using the first face corner (index 0) as the origin
    CC3Vector vp = aLocation - c[0];
    CC3Vector v1 = c[1] - c[0];
    CC3Vector v2 = c[2] - c[0];
    
    // Create dot products required to solve the two equations and two unknowns
    GLfloat dot11 = v1.dot( v1 );
    GLfloat dot12 = v1.dot( v2 );
    GLfloat dot1p = v1.dot( vp );
    GLfloat dot22 = v2.dot( v2 );
    GLfloat dot2p = v2.dot( vp );
    
    GLfloat invDenom = 1.0f / (dot11 * dot22 - dot12 * dot12);	// Denominator
    
    b[1] = (dot22 * dot1p - dot12 * dot2p) * invDenom;
    b[2] = (dot11 * dot2p - dot12 * dot1p) * invDenom;
    b[0] = 1.0f - b[1] - b[2];
    
    //LogTrace(@"The barycentric weights of location %@ in triangle %@ are %@ using basis vectors",
    //	NSStringFromCC3Vector(aLocation), NSStringFromCC3Face(face), NSStringFromCC3BarycentricWeights(bcw));
    return bcw;
}

NS_COCOS3D_END
