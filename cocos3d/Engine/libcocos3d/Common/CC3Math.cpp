/*
 * Cocos3D-X 1.0.0
 * Author: Bill Hollings
 * Copyright (c) 2010-2014 The Brenwill Workshop Ltd. All rights reserved.
 * http://www.brenwill.com
 *
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


float Cyclic(float value, float period) 
{
	float modVal = fmod(value, period);
	return (modVal < 0.0) ? (modVal + period) : modVal;
}

float CyclicDifference(float minuend, float subtrahend, float period) 
{
	float semiPeriod = period / 2.0f;
	float diff = minuend - subtrahend;
	// If the difference is outside the range (period/2 >= diff >= -period/2),
	// adjust it so that it takes the difference in the other direction to
	// arrive at a smaller change.
	if(diff > semiPeriod) {
		diff -= period;
	} else if(diff < -semiPeriod) {
		diff += period;
	}
	return diff;
}


#define kRandomUIntMax 0x100000000LL

unsigned int RandomUInt() {
	return rand();
}

unsigned int RandomUIntBelow(unsigned int max) {
	return RandomUInt() % max;
}

double RandomDouble() {
	return (double)RandomUInt() / (double)kRandomUIntMax;
}

double RandomDoubleBetween(double min, double max) {
	return min + (RandomDouble() * (max - min));
}

float RandomFloat() {
	return (float)RandomDouble();
}

float RandomFloatBetween(float min, float max) {
	return (float)RandomDoubleBetween(min, max);
}


NS_COCOS3D_END
