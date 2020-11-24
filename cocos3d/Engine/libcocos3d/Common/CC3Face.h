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

#ifndef _CC3_FACE_H_
#define _CC3_FACE_H_

NS_COCOS3D_BEGIN

/** Defines a triangular face of the mesh, comprised of three vertices, stored in winding order. */
struct CC3Face
{
    CC3Vector               vertices[3];	/**< The vertices of the face, stored in winding order. */
    
    /** A CC3Face with all vertices set to zero. */
    static const CC3Face    kCC3FaceZero;
    
    static inline CC3Face   invert( const CC3Face& face );
    
    CC3Face() {  }
    inline CC3Face( const CC3Vector& v0, const CC3Vector& v1, const CC3Vector& v2 );
    inline CC3Vector        getCenter() const;
    inline CC3Vector        getNormal() const;
};

/**
 * Returns a CC3Face structure that has the same vertices
 * as the specified face, but in the opposite winding order.
 */
inline CC3Face CC3Face::invert( const CC3Face& face )
{
    CC3Face outFace( face.vertices[0], face.vertices[2], face.vertices[1] );
    return outFace;
}

/**
 * Returns a CC3Face structure constructed from the three specified vectors,
 * which should be supplied in winding order.
 */
inline CC3Face::CC3Face( const CC3Vector& v0, const CC3Vector& v1, const CC3Vector& v2 )
{
    vertices[0] = v0;
    vertices[1] = v1;
    vertices[2] = v2;
}

/**
 * Returns the location of the center of the specified face, calculated
 * as the mathematical average of the three vertices that define the face.
 */
inline CC3Vector CC3Face::getCenter() const
{
    const CC3Vector* vtx = &vertices[0];
    return cc3v((vtx[0].x + vtx[1].x + vtx[2].x) * kCC3OneThird,
                (vtx[0].y + vtx[1].y + vtx[2].y) * kCC3OneThird,
                (vtx[0].z + vtx[1].z + vtx[2].z) * kCC3OneThird);
}

/**
 * Returns a normalized normal vector derived from the location and winding order
 * of the three vertices in the specified face.
 *
 * The direction of the normal vector is affected by the winding order of the
 * vertices in the face. The vertices should wind vertex[0] -> vertex[1] -> vertex[2].
 * The normal will point in the direction that has the three points winding in a
 * counter-clockwise direction, according to a right-handed coordinate system.
 * If the direction of the normal is important, be sure the winding order of
 * the points in the face is correct.
 */
inline CC3Vector CC3Face::getNormal() const
{
    return vertices[1].difference(vertices[0]).cross( vertices[2].difference(vertices[0]) ).normalize();
}


/** Defines the barycentric weights of the three vertices of a triangle, in the same order as the vertices in a CC3Face. */
typedef struct
{
    GLfloat weights[3];		/**< The barycentric weights of the three vertices of the face. */
} CC3BarycentricWeights;

/** Returns a CC3BarycentricWeights structure constructed from the three specified weights. */
static inline CC3BarycentricWeights CC3BarycentricWeightsMake(GLfloat b0, GLfloat b1, GLfloat b2)
{
    CC3BarycentricWeights bcw;
    bcw.weights[0] = b0;
    bcw.weights[1] = b1;
    bcw.weights[2] = b2;
    return bcw;
}

/**
 * Returns whether the specified barycentric weights indicate a location inside a triangle.
 *
 * To be inside a triangle, none of the weights may be negative. If any of the weights are negative,
 * the specified barycentric weights represent a location outside a triangle.
 */
static inline bool CC3BarycentricWeightsAreInsideTriangle(CC3BarycentricWeights bcw)
{
    return bcw.weights[0] >= 0.0f && bcw.weights[1] >= 0.0f && bcw.weights[2] >= 0.0f;
}

/**
 * Returns the barycentric weights for the specified location on the plane of the specified face.
 * The returned weights are specified in the same order as the vertices of the face.
 *
 * The specified location should be on the plane of the specified face.
 *
 * If the location is on the plane of the specified face, the three returned weights will add up to one.
 * If all three of the returned weights are positive, then the location is inside the triangle
 * defined by the face, otherwise, at least one of the returned weights will be negative.
 */
CC3BarycentricWeights CC3FaceBarycentricWeights(CC3Face face, CC3Vector aLocation);

/**
 * Returns the 3D cartesian location on the specified face that corresponds to the specified
 * barycentric coordinate weights.
 */
static inline CC3Vector CC3FaceLocationFromBarycentricWeights(CC3Face face, CC3BarycentricWeights bcw)
{
    CC3Vector* c = face.vertices;
    GLfloat* b = bcw.weights;
    CC3Vector v;
    v.x = b[0] * c[0].x + b[1] * c[1].x + b[2] * c[2].x;
    v.y = b[0] * c[0].y + b[1] * c[1].y + b[2] * c[2].y;
    v.z = b[0] * c[0].z + b[1] * c[1].z + b[2] * c[2].z;
    return v;
}

NS_COCOS3D_END

#endif /* _CC3_FACE_H_ */
