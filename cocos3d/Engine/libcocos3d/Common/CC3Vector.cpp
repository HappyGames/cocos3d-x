#include "cocos3d.h"

NS_COCOS3D_BEGIN


/** A CC3Vector of zero length at the origin. */
const CC3Vector	CC3Vector::kCC3VectorZero = CC3Vector::make( 0.0f, 0.0f, 0.0f );

/** The null CC3Vector. It cannot be drawn, but is useful for marking an uninitialized vector. */
const CC3Vector	CC3Vector::kCC3VectorNull = CC3Vector::make( INFINITY, INFINITY, INFINITY );

/** A CC3Vector with each component equal to one, representing the diagonal of a unit cube. */
const CC3Vector	CC3Vector::kCC3VectorUnitCube = CC3Vector::make( 1.0f, 1.0f, 1.0f );

/** The diagonal length of a unit cube. */
const GLfloat	CC3Vector::kCC3VectorUnitCubeLength( 1.73205f );

/** Unit vector pointing in the same direction as the positive X-axis. */
const CC3Vector	CC3Vector::kCC3VectorUnitXPositive = CC3Vector::make( 1.0,  0.0,  0.0 );

/** Unit vector pointing in the same direction as the positive Y-axis. */
const CC3Vector	CC3Vector::kCC3VectorUnitYPositive = CC3Vector::make( 0.0,  1.0,  0.0 );

/** Unit vector pointing in the same direction as the positive Z-axis. */
const CC3Vector	CC3Vector::kCC3VectorUnitZPositive = CC3Vector::make( 0.0,  0.0,  1.0 );

/** Unit vector pointing in the same direction as the negative X-axis. */
const CC3Vector	CC3Vector::kCC3VectorUnitXNegative = CC3Vector::make( -1.0,  0.0,  0.0 );

/** Unit vector pointing in the same direction as the negative Y-axis. */
const CC3Vector	CC3Vector::kCC3VectorUnitYNegative = CC3Vector::make( 0.0, -1.0,  0.0 );

/** Unit vector pointing in the same direction as the negative Z-axis. */
const CC3Vector	CC3Vector::kCC3VectorUnitZNegative = CC3Vector::make( 0.0,  0.0, -1.0 );

NS_COCOS3D_END
