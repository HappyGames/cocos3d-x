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
#include "cocos3d.h"

NS_COCOS3D_BEGIN


/** A CC3Vector of zero length at the origin. */
const CC3Vector	CC3Vector::kCC3VectorZero( 0.0f, 0.0f, 0.0f );

/** The null CC3Vector. It cannot be drawn, but is useful for marking an uninitialized vector. */
const CC3Vector	CC3Vector::kCC3VectorNull( FLOAT_INFINITY, FLOAT_INFINITY, FLOAT_INFINITY );

/** A CC3Vector with each component equal to one, representing the diagonal of a unit cube. */
const CC3Vector	CC3Vector::kCC3VectorUnitCube( 1.0f, 1.0f, 1.0f );

/** The diagonal length of a unit cube. */
const GLfloat	CC3Vector::kCC3VectorUnitCubeLength( 1.73205f );

/** Unit vector pointing in the same direction as the positive X-axis. */
const CC3Vector	CC3Vector::kCC3VectorUnitXPositive( 1.0,  0.0,  0.0 );

/** Unit vector pointing in the same direction as the positive Y-axis. */
const CC3Vector	CC3Vector::kCC3VectorUnitYPositive( 0.0,  1.0,  0.0 );

/** Unit vector pointing in the same direction as the positive Z-axis. */
const CC3Vector	CC3Vector::kCC3VectorUnitZPositive( 0.0,  0.0,  1.0 );

/** Unit vector pointing in the same direction as the negative X-axis. */
const CC3Vector	CC3Vector::kCC3VectorUnitXNegative( -1.0,  0.0,  0.0 );

/** Unit vector pointing in the same direction as the negative Y-axis. */
const CC3Vector	CC3Vector::kCC3VectorUnitYNegative( 0.0, -1.0,  0.0 );

/** Unit vector pointing in the same direction as the negative Z-axis. */
const CC3Vector	CC3Vector::kCC3VectorUnitZNegative( 0.0,  0.0, -1.0 );

NS_COCOS3D_END
