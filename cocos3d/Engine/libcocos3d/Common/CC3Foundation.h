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
#ifndef _CCL_CC3FOUNDATION_H_
#define _CCL_CC3FOUNDATION_H_

NS_COCOS3D_BEGIN

#define COCOS3D_VERSION 0x010000

class Cocos3d
{
public:
	static void			startUp( bool isEditor = true );
	static void			shutdown();
	static bool			isEditor();
	static void			addInternalShaderPaths();
	static std::string	version();

private:
	static bool			sm_isEditor;
};

/** An integer 2D point. */
typedef struct 
{
	GLint x;			/**< The X-componenent of the point. */
	GLint y;			/**< The Y-componenent of the point. */
} CC3IntPoint;

/** A CC3IntPoint at the origin. */
static const CC3IntPoint kCC3IntPointZero = { 0, 0 };

static inline std::string stringFromCC3IntPoint( const CC3IntPoint& point )
{
	return CC3String::stringWithFormat( (char*)"(%d, %d)", point.x, point.y );
}

/** Returns a CC3IntPoint structure constructed from the vector components. */
static inline CC3IntPoint CC3IntPointMake(GLint x, GLint y) 
{
	CC3IntPoint v;
	v.x = x;
	v.y = y;
	return v;
}

/** Returns a CC3IntPoint structure constructed from the specified CCPoint. */
static inline CC3IntPoint CC3IntPointFromCGPoint( const CCPoint& pt ) 
{
	return CC3IntPointMake((GLint)pt.x, (GLint)pt.y); 
}

/** Returns whether the two points are equal by comparing their respective components. */
static inline bool CC3IntPointsAreEqual(CC3IntPoint p1, CC3IntPoint p2)
{
	return	(p1.x == p2.x) &&
		(p1.y == p2.y);
}

/** Returns whether the specified point is zero, as specified by kCC3IntPointZero. */
static inline bool CC3IntPointIsZero(CC3IntPoint p) { return CC3IntPointsAreEqual(p, kCC3IntPointZero); }

/** Returns a point representing the sum of the two specified points, by add their respective components. */
static inline CC3IntPoint CC3IntPointAdd(CC3IntPoint p1, CC3IntPoint p2)
{
	return	CC3IntPointMake(p1.x + p2.x, p1.y + p2.y);
}

/** An integer 2D size. */
typedef struct 
{
	GLsizei width;			/**< The width measurement. */
	GLsizei height;			/**< The height measurement. */
} CC3IntSize;

/** A CC3IntSize of zero extent. */
static const CC3IntSize kCC3IntSizeZero = { 0, 0 };

/** Returns a CC3IntSize structure constructed from the dimensional components. */
static inline CC3IntSize CC3IntSizeMake(GLint w, GLint h) 
{
	CC3IntSize sz;
	sz.width = w;
	sz.height = h;
	return sz;
}

/** Returns a CC3IntSize structure constructed from the specified CGSize. */
static inline CC3IntSize CC3IntSizeFromCGSize(const CCSize& sz) 
{
	return CC3IntSizeMake((GLint)sz.width, (GLint)sz.height); 
}

/** Returns a CGSize structure constructed from the specified CC3IntSize. */
static inline CCSize CGSizeFromCC3IntSize(CC3IntSize isz) 
{
	return CCSizeMake(isz.width, isz.height); 
}

/** Returns whether the two sizes are equal by comparing their respective components. */
static inline bool CC3IntSizesAreEqual(CC3IntSize s1, CC3IntSize s2) 
{
	return	(s1.width == s2.width) &&
		(s1.height == s2.height);
}

/** Returns whether the specified size is zero, as specified by kCC3IntSizeZero. */
static inline bool CC3IntSizeIsZero(CC3IntSize s) 
{
	return CC3IntSizesAreEqual(s, kCC3IntSizeZero); 
}

/** A struct representing an integer tessellation. */
typedef CC3IntPoint CC3Tessellation;

/** Returns a CC3Tessellation structure constructed from the specified components. */
static inline CC3Tessellation CC3TessellationMake(GLint x, GLint y) 
{
	return CC3IntPointMake(x, y); 
}


/** An integer 3D vector. */
typedef struct 
{
	GLint x;			/**< The X-componenent of the vector. */
	GLint y;			/**< The Y-componenent of the vector. */
	GLint z;			/**< The Z-componenent of the vector. */
} CC3IntVector;

static inline std::string stringFromCC3IntVector( const CC3IntVector& vec )
{
	return CC3String::stringWithFormat( (char*)"(%d, %d, %d)", vec.x, vec.y, vec.z );
}

/** Returns a CC3IntVector structure constructed from the vector components. */
static inline CC3IntVector CC3IntVectorMake(GLint x, GLint y, GLint z) 
{
	CC3IntVector v;
	v.x = x;
	v.y = y;
	v.z = z;
	return v;
}

/** Returns whether the two vectors are equal by comparing their respective components. */
static inline bool CC3IntVectorsAreEqual(CC3IntVector v1, CC3IntVector v2)
{
	return	(v1.x == v2.x) &&
		(v1.y == v2.y) &&
		(v1.z == v2.z);
}

static inline std::string stringFromCCPoint( const CCPoint& point )
{
	return CC3String::stringWithFormat( (char*)"(%.3f, %.3f)", point.x, point.y );
}

/**
 * Orthonormalizes the specified array of vectors, using a Gram-Schmidt process,
 * and returns the orthonormal results in the same array.
 *
 * The vectorCount argument indicates the number of vectors in the vectors array argument.
 *
 * Upon completion, each vector in the specfied array will be a unit vector that
 * is orthagonal to all of the other vectors in the array.
 *
 * The first vector in the array is used as the starting point for orthonormalization.
 * Since the Gram-Schmidt process is biased towards the starting vector, if this function
 * will be used repeatedly on the same set of vectors, it is recommended that the order
 * of the vectors in the array be changed on each call to this function, to ensure that
 * the starting bias be averaged across each of the vectors over the long term.
 */
void CC3VectorOrthonormalize(CC3Vector* vectors, GLuint vectorCount);

/**
 * Orthonormalizes the specified array of three vectors, using a Gram-Schmidt process,
 * and returns the orthonormal results in the same array.
 *
 * The number of vectors in the specified array must be exactly three.
 *
 * Upon completion, each vector in the specfied array will be a unit vector that
 * is orthagonal to all of the other vectors in the array.
 *
 * The first vector in the array is used as the starting point for orthonormalization.
 * Since the Gram-Schmidt process is biased towards the starting vector, if this function
 * will be used repeatedly on the same set of vectors, it is recommended that the order
 * of the vectors in the array be changed on each call to this function, to ensure that
 * the starting bias be averaged across each of the vectors over the long term.
 */
static inline void CC3VectorOrthonormalizeTriple(CC3Vector* triVector) 
{
	return CC3VectorOrthonormalize(triVector, 3); 
}


/** 
 * An integer 4D vector.
 *
 * This structure can be referenced as containing 4 individual XYZW axes components,
 * or containing a 3D XYZ vector, plus a W component.
*/
typedef struct 
{
	union {
		struct {
			GLint x;			/**< The X-componenent of the vector. */
			GLint y;			/**< The Y-componenent of the vector. */
			GLint z;			/**< The Z-componenent of the vector. */
		};
		
		struct {
			CC3IntVector v;		/**< The X, Y & Z components as a 3D vector. */
		};
	};
	GLint w;					/**< The homogeneous ratio factor. */
} CC3IntVector4;

static inline std::string stringFromCC3IntVector4( const CC3IntVector4& vec )
{
	return CC3String::stringWithFormat( (char*)"(%d, %d, %d, %d)", vec.x, vec.y, vec.z, vec.w );
}

/** Returns a CC3IntVector4 structure constructed from the vector components. */
static inline CC3IntVector4 CC3IntVector4Make(GLint x, GLint y, GLint z, GLint w)
{
	CC3IntVector4 v;
	v.x = x;
	v.y = y;
	v.z = z;
	v.w = w;
	return v;
}

/** Returns whether the two vectors are equal by comparing their respective components. */
static inline bool CC3IntVector4sAreEqual(CC3IntVector4 v1, CC3IntVector4 v2) 
{
	return (v1.w == v2.w) && CC3IntVectorsAreEqual(v1.v, v2.v);
}

//typedef CC3Vector4 CC3Quaternion;


/** Returns a ccTex2F structure constructed from the vector components. */
static inline ccTex2F CC3TexCoordsMake(GLfloat u, GLfloat v) 
{
	ccTex2F tc;
	tc.u = u;
	tc.v = v;
	return tc;
}

/** Convenience alias macro to create ccTex2F with less keystrokes. */
#define cc3tc(U,V) CC3TexCoordsMake((U),(V))

/**
 * Defines a simple vertex, containing location and color.
 * Useful for painting solid colors that ignore lighting conditions.
 */
typedef struct 
{
	CC3Vector location;			/**< The 3D location of the vertex. */
	ccColor4F color;			/**< The color at the vertex. */
} CC3ColoredVertex;

/**
 * Defines a simple vertex, containing location, normal and color.
 * Useful for painting solid colors that interact with lighting conditions.
 */
typedef struct 
{
	CC3Vector location;			/**< The 3D location of the vertex. */
	CC3Vector normal;			/**< The 3D normal at the vertex. */
	ccColor4F color;			/**< The color at the vertex. */
} CC3LitColoredVertex;

/**
 * Defines a simple vertex, containing location, normal, and texture coordinate
 * data. Useful for interleaving vertex data for presentation to the GL engine.
 */
typedef struct 
{
	CC3Vector location;			/**< The 3D location of the vertex. */
	CC3Vector normal;			/**< The 3D normal at the vertex. */
	ccTex2F texCoord;			/**< The 2D coordinate of this vertex on the texture. */
} CC3TexturedVertex;


/**
 * An angle such as a heading or inclination.
 * Can be measured in degrees or radians and may be positive or negative.
 */ 
typedef GLfloat CC3Angle;

/** Specifies a vector using angular coordinate axes. Angles are measured in degrees or radians. */
typedef struct 
{
	CC3Angle heading;				/**< The horizontal heading. */
	CC3Angle inclination;			/**< The inclination from horizontal. */
	GLfloat radius;					/**< The radial distance. */
} CC3AngularVector;

/** Returns an CC3AngularVector structure constructed from the vector components. */
static inline CC3AngularVector CC3AngularVectorMake(GLfloat heading, GLfloat inclination, GLfloat radius) 
{
	CC3AngularVector av;
	av.heading = heading;
	av.inclination = inclination;	
	av.radius = radius;
	return av;
}

/**
 * Returns an CC3AngularVector providing the heading, inclination & radius of the specified CC3Vector.
 * Heading is measured in degrees, in the X-Z plane, clockwise from the negative Z-axis.
 * Inclination is measured in degrees, with up being in the positive-Y direction.
 */
static inline CC3AngularVector CC3AngularVectorFromVector(CC3Vector aCoord) 
{
	CC3AngularVector av;
	av.radius = aCoord.length();
	av.inclination = av.radius ? CC3RadToDeg(asinf(aCoord.y / av.radius)) : 0.0f;
	av.heading = CC3RadToDeg(atan2f(aCoord.x, -aCoord.z));
	return av;
}

/**
 * Returns a CC3Vector from the specified CC3AngularVector.
 * Heading is measured in degrees, in the X-Z plane, clockwise from the negative Z-axis.
 * Inclination is measured in degrees, with up being in the positive-Y direction.
 */
static inline CC3Vector CC3VectorFromAngularVector(CC3AngularVector av) 
{
	CC3Vector unitDir;
	
	// First, incline up the Y-axis from the negative Z-axis.
	GLfloat radInclination = CC3DegToRad(av.inclination);
	unitDir.y = sinf(radInclination);
	GLfloat xzLen = cosf(radInclination);
	
	// Now rotate around the Y-axis to the heading. The length of the projection of the direction
	// vector into the X-Z plane is the length of the projection onto the negative Z-axis after
	// the initial inclination. Use this length as the basis for calculating the X & Z CC3Vectors.
	// The result is a unit direction vector projected into all three axes.
	GLfloat radHeading = CC3DegToRad(av.heading);
	unitDir.x = xzLen * sinf(radHeading);
	unitDir.z = -xzLen * cosf(radHeading);
	return unitDir * av.radius;
}

/**
 * Returns the difference between two CC3AngularVectors, by subtracting the corresponding heading,
 * inclination & radial components. Note that this is NOT true vector arithmetic, which would
 * yield a completely different angular and radial results.
 */
static inline CC3AngularVector CC3AngularVectorDifference(CC3AngularVector minuend, CC3AngularVector subtrahend) 
{
	CC3AngularVector difference;
	difference.heading = CC3SemiCyclicAngle(minuend.heading - subtrahend.heading);
	difference.inclination = minuend.inclination - subtrahend.inclination;
	difference.radius = minuend.radius - subtrahend.radius;
	return difference;
}

/**
 * Defines a triangular face of the mesh, comprised of three vertex indices,
 * each a GLuint, stored in winding order.
 */
typedef struct 
{
	GLuint vertices[3];	/**< The indices of the vertices of the face, stored in winding order. */
} CC3FaceIndices;

/** A CC3FaceIndices with all vertices set to zero. */
static const CC3FaceIndices kCC3FaceIndicesZero = { {0, 0, 0} };

/** 
 * Returns a CC3FaceIndices structure constructed from the three
 * specified vertex indices, which should be supplied in winding order.
 */
static inline CC3FaceIndices CC3FaceIndicesMake(GLuint i0, GLuint i1, GLuint i2) 
{
	CC3FaceIndices fi;
	fi.vertices[0] = i0;
	fi.vertices[1] = i1;
	fi.vertices[2] = i2;
	return fi;
}

/** Defines a sphere. */
typedef struct 
{
	CC3Vector center;			/**< The center of the sphere. */
	GLfloat radius;				/**< The radius of the sphere */
} CC3Sphere;

/** Returns a CC3Spere constructed from the specified center and radius. */
static inline CC3Sphere CC3SphereMake(CC3Vector center, GLfloat radius) 
{
	CC3Sphere s;
	s.center = center;
	s.radius = radius;
	return s;
}

/** Returns a CC3Spere that circumscribes the specified box. */
static inline CC3Sphere CC3SphereFromCircumscribingBox( const CC3Box& box )
{
	CC3Vector center = box.getCenter();
	GLfloat radius = box.maximum.distance( center );
	return CC3SphereMake(center, radius);
}

/** Returns whether the specified location lies within the specified sphere. */
static inline bool CC3IsLocationWithinSphere(CC3Vector aLocation, CC3Sphere aSphere) 
{
	// Compare the squares of the distances to avoid taking an expensive square root. 
	GLfloat radiusSquared = aSphere.radius * aSphere.radius;
	return aLocation.distanceSquared( aSphere.center ) <= radiusSquared;
}

/** Returns whether the specified spheres intersect. */
static inline bool CC3DoesSphereIntersectSphere(CC3Sphere sphereOne, CC3Sphere sphereTwo) 
{
	// Expand the first sphere to have a radius equal to the sume of the two radii,
	// and test whether the center of the other sphere is inside the expanded sphere.
	CC3Sphere bigSphere = CC3SphereMake(sphereOne.center, (sphereOne.radius + sphereTwo.radius));
	return CC3IsLocationWithinSphere(sphereTwo.center, bigSphere);
}

/** Returns the smallest CC3Sphere that contains the two specified spheres. */
CC3Sphere CC3SphereUnion(CC3Sphere s1, CC3Sphere s2);

/** Returns whether the specified ray intersects the specified sphere. */
bool CC3DoesRayIntersectSphere(CC3Ray aRay, CC3Sphere aSphere);

/**
 * Returns the location that the specified ray intersects the specified sphere, or returns
 * kCC3VectorNull if the ray does not intersect the sphere, or the sphere is behind the ray.
 *
 * The result takes into consideration the startLocation of the ray, and will return
 * kCC3VectorNull if the sphere is behind the startLocation, even if the line projecting
 * back through the startLocation in the negative direction of the ray intersects the sphere.
 *
 * The ray may start inside the sphere, in which case, the returned location represents
 * the exit location of the ray.
 */
CC3Vector CC3RayIntersectionWithSphere(CC3Ray aRay, CC3Sphere aSphere);

/**
 * Returns the coefficients of the quadratic equation that describes the points of
 * intersection between the specified ray and sphere.
 *
 * Given the equation for a sphere at the origin:  x*x + y*y + z*z = r*r, and the
 * equation for a ray in the same frame of reference: p = s + tv, where s is the
 * ray start, v is the ray direction, and p is a point on the ray, we can solve for
 * the intersection points of the ray and sphere. The result is a quadratic equation
 * in t: at*t + bt + c = 0, where: a = v*v, b = 2(s.v), and c = s*s - r*r.
 *
 * The a, b and c elements of the returned CC3Plane structure contain the a, b and c
 * coefficients of the quadratic equation, respectively. The d element of the returned
 * CC3Plane structure contains the discriminant of the quadratic equation (d = b*b - 4ac).
 *
 * The returned quadratic coefficients are not a plane.
 * The CC3Plane structure is simply used for convenience.
 *
 * Reference: Mathematics for 3D Game Programming and Computer Graphics, 3rd ed. book, by Eric Lengyel
 */
CC3Plane CC3RaySphereIntersectionEquation(CC3Ray aRay, CC3Sphere aSphere);


/**
 * The constant, linear, and quadratic coefficients of the attenuation function equation:
 * (a + b*r + c*r*r), where r is the radial distance between a the source (light or camera)
 * and the 3D location at which we want to calculate attenuation.
 */
typedef struct 
{
	GLfloat a;				/**< The constant coefficient in the attenuation function. */
	GLfloat b;				/**< The linear coefficient in the attenuation function. */
	GLfloat c;				/**< The quadratic coefficient in the attenuation function. */
} CC3AttenuationCoefficients;

/** Attenuation coefficients corresponding to no attenuation with distance (constant size). */
static const CC3AttenuationCoefficients kCC3AttenuationNone = {1.0, 0.0, 0.0};

/** Returns a CC3AttenuationCoefficients structure constructed from the specified coefficients. */
static inline CC3AttenuationCoefficients CC3AttenuationCoefficientsMake(GLfloat constant,
																		GLfloat linear,
																		GLfloat quadratic) 
{
	CC3AttenuationCoefficients coeffs;
	coeffs.a = constant;
	coeffs.b = linear;
	coeffs.c = quadratic;
	return coeffs;
}

/** Returns whether the two attenuation coefficents are equal by comparing their respective components. */
static inline bool CC3AttenuationCoefficientsAreEqual(CC3AttenuationCoefficients ac1,
													  CC3AttenuationCoefficients ac2) 
{
	return (ac1.a == ac2.a) && (ac1.b == ac2.b) && (ac1.c == ac2.c);
}

/**
 * If at least one of the coefficients in the specified attenuation is not zero, the specified
 * attenuation coefficients is returned unchanged. However, if all three coefficients of the
 * specified attenuation coefficients are zero, the value is illegal (and will cause divide-by-zero
 * errors, especially in shaders). If that is the case, this function returns kCC3AttenuationNone.
 */
static inline CC3AttenuationCoefficients CC3AttenuationCoefficientsLegalize(CC3AttenuationCoefficients ac) 
{
	return (ac.a != 0.0f || ac.b != 0.0f || ac.c != 0.0f) ? ac : kCC3AttenuationNone;
}

/** 
 * A rectangle defining a GL viewport.
 *
 * This can also be used for any rectangular group of pixels.
 */
typedef union 
{
	struct {
		GLint x;				/**< The X-position of the bottom-left corner of the viewport. */
		GLint y;				/**< The Y-position of the bottom-left corner of the viewport. */
		GLsizei w;				/**< The width of the viewport. */
		GLsizei h;				/**< The height of the viewport. */
	};
	struct {
		CC3IntPoint origin;		/**< The origin of the viewport. */
		CC3IntSize size;		/**< The size of the viewport. */
	};
} CC3Viewport;

/** An empty or undefined viewport. */
static const CC3Viewport kCC3ViewportZero = { {0, 0, 0, 0} };


/** Returns a CC3Viewport structure constructed from the specified components. */
static inline CC3Viewport CC3ViewportMake(GLint x, GLint y, GLint w, GLint h) 
{
	CC3Viewport vp;
	vp.x = x;
	vp.y = y;
	vp.w = w;
	vp.h = h;
	return vp;
}

/** Returns a CC3Viewport constructed from the specified origin and size. */
static inline CC3Viewport CC3ViewportFromOriginAndSize(CC3IntPoint origin, CC3IntSize size) 
{
	CC3Viewport vp;
	vp.origin = origin;
	vp.size = size;
	return vp;
}

/** Returns whether the two viewports are equal by comparing their respective components. */
static inline bool CC3ViewportsAreEqual(CC3Viewport vp1, CC3Viewport vp2) 
{
	return (vp1.x == vp2.x) && (vp1.y == vp2.y) && (vp1.w == vp2.w) && (vp1.h == vp2.h);
}

/** Returns whether the specified viewport is equal to the zero viewport, specified by kCC3ViewportZero. */
static inline bool CC3ViewportIsZero(CC3Viewport vp) 
{
	return CC3ViewportsAreEqual(vp, kCC3ViewportZero);
}

/**
 * Returns whether the specified point lies within the specified viewport.
 * A point is considered inside the viewport if its coordinates lie inside
 * the viewport or on the minimum X or minimum Y edge.
 */
static inline bool CC3ViewportContainsPoint(CC3Viewport vp, CCPoint point) 
{
	return (point.x >= vp.x) && (point.x < vp.x + vp.w) &&
	(point.y >= vp.y) && (point.y < vp.y + vp.h);
}

/** Returns a CC3Viewport constructed from the specified CGRect. */
static inline CC3Viewport CC3ViewportFromCGRect(CCRect rect) 
{
	return CC3ViewportFromOriginAndSize(CC3IntPointFromCGPoint(rect.origin), CC3IntSizeFromCGSize(rect.size));
}

/** Returns a CCRect constructed from the specified viewport. */
static inline CCRect CGRectFromCC3Viewport(CC3Viewport vp) 
{
	return CCRectMake(vp.x, vp.y, vp.w, vp.h);
}

/** 
 * Returns a CC3Viewport of the same size as the specified viewport, but whose origin is 
 * translated from the origin of the specified viewport by the by the specified amount. 
 */
static inline CC3Viewport CC3ViewportTranslate(CC3Viewport vp, CC3IntPoint offset) 
{
	return CC3ViewportFromOriginAndSize(CC3IntPointAdd(vp.origin, offset), vp.size);
}


/** Returns a GLfloat between 0 and 1 converted from the specified GLubyte value between 0 and 255. */
static inline GLfloat CCColorFloatFromByte(GLubyte colorValue) 
{
	return (GLfloat)colorValue * kCC3OneOver255;
}

/**
 * Returns a GLubyte between 0 and 255 converted from the specified GLfloat value.
 *
 * The specified float value is clamped to between 0 and 1 before conversion, so that
 * the Glubyte does not overflow or underflow, which would create unexpected colors.
 */
static inline GLubyte CCColorByteFromFloat(GLfloat colorValue) 
{
	return (GLubyte)(CLAMP(colorValue, 0.0f, 1.0f) * 255.0f);
}

/**
 * Returns the luminosity of the specified color as calculated using the ITU-R BT.709
 * conversion for high-definition television (HDTV):
 *
 *   luminosity = (0.2126 * red) + (0.7152 * green) + (0.0722 * blue),
 */
static inline GLfloat CC3LuminosityBT709(GLfloat red, GLfloat green, GLfloat blue) 
{
	return (0.2126f * red) + (0.7152f * green) + (0.0722f * blue);
}

/**
 * Returns the luminosity of the specified color as calculated using the ITU-R BT.601
 * conversion for standard-definition television (SDTV):
 *
 *   luminosity = (0.299 * red) + (0.587 * green) + (0.114 * blue),
 */
static inline GLfloat CC3LuminosityBT601(GLfloat red, GLfloat green, GLfloat blue) 
{
	return (0.299f * red) + (0.587f * green) + (0.114f * blue);
}

/** Opaque Red */
static const ccColor4F kCCC4FRed = { 1.0, 0.0, 0.0, 1.0 };

/** Opaque Green */
static const ccColor4F kCCC4FGreen = { 0.0, 1.0, 0.0, 1.0 };

/** Opaque Blue */
static const ccColor4F kCCC4FBlue = { 0.0, 0.0, 1.0, 1.0 };

/** Opaque Cyan */
static const ccColor4F kCCC4FCyan = { 0.0, 1.0, 1.0, 1.0 };

/** Opaque Magenta */
static const ccColor4F kCCC4FMagenta = { 1.0, 0.0, 1.0, 1.0 };

/** Opaque Yellow */
static const ccColor4F kCCC4FYellow = { 1.0, 1.0, 0.0, 1.0 };

/** Opaque Orange */
static const ccColor4F kCCC4FOrange = { 1.0, 0.5, 0.0, 1.0 };

/** Opaque Light Gray */
static const ccColor4F kCCC4FLightGray = { 0.75, 0.75, 0.75, 1.0 };

/** Opaque Gray */
static const ccColor4F kCCC4FGray = { 0.5, 0.5, 0.5, 1.0 };

/** Opaque Dark Gray */
static const ccColor4F kCCC4FDarkGray = { 0.25, 0.25, 0.25, 1.0 };

/** Opaque White */
static const ccColor4F kCCC4FWhite = { 1.0, 1.0, 1.0, 1.0 };

/** Opaque Black */
static const ccColor4F kCCC4FBlack = { 0.0, 0.0, 0.0, 1.0 };

/** Transparent Black */
static const ccColor4F kCCC4FBlackTransparent = {0.0, 0.0, 0.0, 0.0};


#if COCOS2D_VERSION < 0x010100		// cocos2d 1.0 - exists in cocos2d versions above
/** Convenience alias macro to create ccColor4F with less keystrokes. */
#define ccc4f(R,G,B,A) CCC4FMake((R),(G),(B),(A))
#endif

/** Returns a ccColor4F structure constructed from the specified components */
static inline ccColor4F CCC4FMake(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) 
{
	ccColor4F color;
	color.r = red;
	color.g = green;
	color.b = blue;
	color.a = alpha;
	return color;
}

/** Returns a ccColor4F structure constructed from the specified ccColor4B */
static inline ccColor4F CCC4FFromCCC4B(ccColor4B byteColor) 
{
	return ccc4f(CCColorFloatFromByte(byteColor.r),
				 CCColorFloatFromByte(byteColor.g),
				 CCColorFloatFromByte(byteColor.b),
				 CCColorFloatFromByte(byteColor.a));
}

/** Returns a ccColor4F structure constructed from the specified ccColor3B and opacity. */
static inline ccColor4F CCC4FFromColorAndOpacity(ccColor3B byteColor, CCOpacity opacity) 
{
	return ccc4f(CCColorFloatFromByte(byteColor.r),
				 CCColorFloatFromByte(byteColor.g),
				 CCColorFloatFromByte(byteColor.b),
				 GLfloatFromCCOpacity(opacity));
}

/** Returns the intensity of the specified color, calculated as the arithmetic mean of the R, G & B components. */
static inline GLfloat CCC4FIntensity(ccColor4F color) 
{
	return (color.r + color.g + color.b) * kCC3OneThird;
}

/** Returns whether the two colors are equal by comparing their respective components. */
static inline bool CCC4FAreEqual(ccColor4F c1, ccColor4F c2) 
{
	return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b && c1.a == c2.a;
}

/**
 * Returns the result of adding the two specified colors, by adding the corresponding components.
 * Each of the resulting color components is clamped to be between 0.0 and 1.0.
 * This can also be thought of as a translation of the first color by the second.
 */
static inline ccColor4F CCC4FAdd(ccColor4F rgba, ccColor4F translation) 
{
	return ccc4f(CLAMP(rgba.r + translation.r, 0.0f, 1.0f),
				 CLAMP(rgba.g + translation.g, 0.0f, 1.0f),
				 CLAMP(rgba.b + translation.b, 0.0f, 1.0f),
				 CLAMP(rgba.a + translation.a, 0.0f, 1.0f));
}

/**
 * Returns the difference between two colors, by subtracting the subtrahend from the minuend,
 * which is accomplished by subtracting each of the corresponding r,g, b, a components.
 * Each of the resulting color components is clamped to be between 0.0 and 1.0.
 */
static inline ccColor4F CCC4FDifference(ccColor4F minuend, ccColor4F subtrahend) 
{
	return ccc4f(CLAMP(minuend.r - subtrahend.r, 0.0f, 1.0f),
				 CLAMP(minuend.g - subtrahend.g, 0.0f, 1.0f),
				 CLAMP(minuend.b - subtrahend.b, 0.0f, 1.0f),
				 CLAMP(minuend.a - subtrahend.a, 0.0f, 1.0f));
}

/**
 * Returns a ccColor4F structure whose values are those of the specified original color,
 * where each color component has been translated by the specified offset.
 * Each of the resulting color components is clamped to be between 0.0 and 1.0.
 */
static inline ccColor4F CCC4FUniformTranslate(ccColor4F rgba, GLfloat offset) 
{
	return CCC4FAdd(rgba, ccc4f(offset, offset, offset, offset));
}

/**
 * Returns a ccColor4F structure whose values are those of the specified original color,
 * multiplied by the specified scaling factor.
 * Each of the resulting color components is clamped to be between 0.0 and 1.0.
 */
static inline ccColor4F CCC4FUniformScale(ccColor4F rgba, GLfloat scale) 
{
	return ccc4f(CLAMP(rgba.r * scale, 0.0f, 1.0f),
				 CLAMP(rgba.g * scale, 0.0f, 1.0f),
				 CLAMP(rgba.b * scale, 0.0f, 1.0f),
				 CLAMP(rgba.a * scale, 0.0f, 1.0f));
}

/**
 * Returns the result of modulating the specified colors, by multiplying the corresponding
 * components. Each of the resulting color components is clamped to be between 0.0 and 1.0.
 */
static inline ccColor4F CCC4FModulate(ccColor4F rgba, ccColor4F modulation) 
{
	return ccc4f(CLAMP(rgba.r * modulation.r, 0.0f, 1.0f),
				 CLAMP(rgba.g * modulation.g, 0.0f, 1.0f),
				 CLAMP(rgba.b * modulation.b, 0.0f, 1.0f),
				 CLAMP(rgba.a * modulation.a, 0.0f, 1.0f));
}

/**
 * Returns a ccColor4F structure whose values are a weighted average of the specified base color and
 * the blend color. The parameter blendWeight should be between zero and one. A value of zero will leave
 * the base color unchanged. A value of one will result in the blend being the same as the blend color.
 */
static inline ccColor4F CCC4FBlend(ccColor4F baseColor, ccColor4F blendColor, GLfloat blendWeight) 
{
	return ccc4f(CC3WeightedAverage(baseColor.r, blendColor.r, blendWeight),
				 CC3WeightedAverage(baseColor.g, blendColor.g, blendWeight),
				 CC3WeightedAverage(baseColor.b, blendColor.b, blendWeight),
				 CC3WeightedAverage(baseColor.a, blendColor.a, blendWeight));
}

/**
 * Returns a ccColor4F color whose R, G & B components are those of the specified color multiplied
 * by the alpha value of the specified color, clamping to the range between zero and one if needed.
 * The alpha value remains unchanged.
 * 
 * This function performs the same operation on the specified color that is known as pre-multiplied
 * alpha when applied to the texels of a texture.
 */
static inline ccColor4F CCC4FBlendAlpha(ccColor4F rgba) 
{
	return ccc4f(CLAMP(rgba.r * rgba.a, 0.0f, 1.0f),
				 CLAMP(rgba.g * rgba.a, 0.0f, 1.0f),
				 CLAMP(rgba.b * rgba.a, 0.0f, 1.0f),
				 rgba.a);
}

/**
 * Returns a random ccColor4F where each component value between the specified min inclusive and
 * the specified max exclusive. This can be useful when creating particle systems.
 */
static inline ccColor4F RandomCCC4FBetween(ccColor4F min, ccColor4F max) 
{
	return ccc4f(CC3RandomFloatBetween(min.r, max.r),
				 CC3RandomFloatBetween(min.g, max.g),
				 CC3RandomFloatBetween(min.b, max.b),
				 CC3RandomFloatBetween(min.a, max.a));
}

/** Returns whether the two colors are equal by comparing their respective components. */
static inline bool CCC4BAreEqual(ccColor4B c1, ccColor4B c2) 
{
	return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b && c1.a == c2.a;
}

/** Returns a ccColor4B structure constructed from the specified ccColor4F */
static inline ccColor4B CCC4BFromCCC4F(ccColor4F floatColor) 
{
	return ccc4(CCColorByteFromFloat(floatColor.r),
				CCColorByteFromFloat(floatColor.g),
				CCColorByteFromFloat(floatColor.b),
				CCColorByteFromFloat(floatColor.a));
}

/** Returns a ccColor4B structure constructed from the specified ccColor3B and opacity. */
static inline ccColor4B CCC4BFromColorAndOpacity(ccColor3B byteColor, CCOpacity opacity) 
{
	return ccc4(byteColor.r, byteColor.g, byteColor.b, GLubyteFromCCOpacity(opacity));
}

/**
 * Returns a ccColor4B color whose R, G & B components are those of the specified color multiplied
 * by the alpha value of the specified color, clamping to the range between zero and 255 if needed.
 * The alpha value remains unchanged.
 * 
 * This function performs the same operation on the specified color that is known as pre-multiplied
 * alpha when applied to the texels of a texture.
 */
static inline ccColor4B CCC4BBlendAlpha(ccColor4B rgba) 
{
	GLfloat alpha = rgba.a * kCC3OneOver255;
	return ccc4((GLubyte)CLAMP(rgba.r * alpha, 0, 255),
				(GLubyte)CLAMP(rgba.g * alpha, 0, 255),
				(GLubyte)CLAMP(rgba.b * alpha, 0, 255),
				rgba.a);
}

/** Returns whether the two colors are equal by comparing their respective components. */
static inline bool CCC3BAreEqual(ccColor3B c1, ccColor3B c2) 
{
	return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
}

/** Returns a ccColor3B structure constructed from the specified ccColor4F */
static inline ccColor3B CCC3BFromCCC4F(ccColor4F floatColor) 
{
	return ccc3(CCColorByteFromFloat(floatColor.r),
				CCColorByteFromFloat(floatColor.g),
				CCColorByteFromFloat(floatColor.b));
}

/** Returns a ccColor3B structure constructed from the specified ccColor4B */
static inline ccColor3B CCC3BFromCCC4B(ccColor4B color) { return *(ccColor3B*)&color; }

/**
 * Returns an ccColor3B structure whose values are a weighted average of the specified base color and
 * the blend color. The parameter blendWeight should be between zero and one. A value of zero will leave
 * the base color unchanged. A value of one will result in the blend being the same as the blend color.
 */
static inline ccColor3B CCC3BBlend(ccColor3B baseColor, ccColor3B blendColor, GLfloat blendWeight) 
{
	return ccc3((GLubyte)CC3WeightedAverage(baseColor.r, blendColor.r, blendWeight),
				(GLubyte)CC3WeightedAverage(baseColor.g, blendColor.g, blendWeight),
				(GLubyte)CC3WeightedAverage(baseColor.b, blendColor.b, blendWeight));
}

/** 
 * Returns whether the two boolean values are in equal state.
 *
 * This test will work if the two booleans each contain different non-zero content to indicate
 * logical YES, whereas a simple == comparison would give an erroneous result in that situation.
 */
static inline bool CC3BooleansAreEqual(bool b1, bool b2) { return (b1 && b2) || (!b1 && !b2); }


/** Returns whether the specified bit in the specified bitfield is set to one. */
static inline bool CC3IsBitSet(GLbitfield bits, GLuint bitIdx) 
{
	CCAssert(bitIdx < (8 * sizeof(bits)), "Bit index is too large for the bitfield.");
	GLbitfield marker = 1;
	marker <<= bitIdx;
	return (bits & marker) ? true : false;
}

/** Returns whether the specified bit in the specified bitfield is set to zero. */
static inline bool CC3IsBitClear(GLbitfield bits, GLuint bitIdx) { return !CC3IsBitSet(bits, bitIdx); }

/** 
 * Sets the specified bit in the specified bitfield is to one if the specified value is YES,
 * or to zero if the specified value is NO.
 */
static inline void CC3SetBit(GLbitfield* bits, GLuint bitIdx, bool val) 
{
	CCAssert(bitIdx < (8 * sizeof(bits)), "Bit index is too large for the bitfield.");
	GLbitfield marker = 1;
	marker <<= bitIdx;
	if (val)
		*bits |= marker;
	else
		*bits &= ~marker;
}

static inline std::string stringFromCCSize( const CCSize& size )
{
	return CC3String::stringWithFormat( (char*)"(%.2f, %.2f)", size.width, size.height );
}

/** Returns a string description of the specified ccColor4F in the form "(r, g, b, a)" */
static inline std::string stringFromCCC4F(ccColor4F rgba) 
{
	return CC3String::stringWithFormat( (char*)"(%.3f, %.3f, %.3f, %.3f)", rgba.r, rgba.g, rgba.b, rgba.a );
}

/**
 * Reverses the order of the rows in the specified data block.
 * The transformation is performed in-place.
 *
 * This function can be useful for vertically flipping the contents of a texture or image,
 * which is a common requirement when transferring content data between an OpenGL texture
 * and an iOS or OSX image. Or it may be used to reverse the order of the rows of any
 * row-major memory structure.
 *
 * The specified data block is assumed to be in row-major order, containing the specified
 * number of rows, and with the specified number of bytes in each row. The total number of
 * bytes in the data block must be at least (bytesPerRow * rowCount).
 */
void CC3FlipVertically(GLubyte* rowMajorData, GLuint rowCount, GLuint bytesPerRow);

/** Temporarily turn off compiler warnings for hidden variable shadowing. */
#define CC3_PUSH_NOSHADOW	_Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wshadow\"")

/** Resume compiler warnings for hidden variable shadowing. */
#define CC3_POP_NOSHADOW	_Pragma("GCC diagnostic pop")

/** Temporarily turn off compiler warnings of unimplemented selectors. */
#define CC3_PUSH_NOSELECTOR	_Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wselector\"")

/** Resume compiler warnings of unimplemented selectors. */
#define CC3_POP_NOSELECTOR	_Pragma("GCC diagnostic pop")

const static CCRect CCRectNull = CCRectMake( FLOAT_INFINITY, FLOAT_INFINITY, 0, 0 );
static inline CCRect CCRectUnion( const CCRect& a, const CCRect& b )
{
	if ( a.equals( CCRectNull ) )
		return b;

	if ( b.equals( CCRectNull ) )
		return a;

	float minX = MIN( a.getMinX(), b.getMaxX() );
	float minY = MIN( a.getMaxY(), b.getMinY() );
	float maxX = MAX( a.getMaxX(), b.getMaxX() );
	float maxY = MAX( a.getMaxY(), b.getMaxY() );

	return CCRectMake( minX, minY, maxX-minX, maxY-minY);
}

NS_COCOS3D_END

#endif
