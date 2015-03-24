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
#include "cocos3d.h"

NS_COCOS3D_BEGIN

void CC3PerformanceStatistics::addUpdateTime( float deltaTime )
{
	_updatesHandled++;
	_accumulatedUpdateTime += deltaTime;
}

void CC3PerformanceStatistics::addNodesUpdated( GLuint nodeCount )
{
	_nodesUpdated += nodeCount; 
}

void CC3PerformanceStatistics::incrementNodesUpdated() 
{
	_nodesUpdated++; 
}

void CC3PerformanceStatistics::addNodesTransformed( GLuint nodeCount )
{
	_nodesTransformed += nodeCount; 
}

void CC3PerformanceStatistics::incrementNodesTransformed()
{
	_nodesTransformed++; 
}

void CC3PerformanceStatistics::addFrameTime( float deltaTime )
{
	_framesHandled++;
	_accumulatedFrameTime += deltaTime;
}

void CC3PerformanceStatistics::addNodesVisitedForDrawing( GLuint nodeCount )
{
	_nodesVisitedForDrawing += nodeCount; 
}

void CC3PerformanceStatistics::incrementNodesVisitedForDrawing()
{
	_nodesVisitedForDrawing++; 
}

void CC3PerformanceStatistics::addNodesDrawn( GLuint nodeCount )
{
	_nodesDrawn += nodeCount; 
}

void CC3PerformanceStatistics::incrementNodesDrawn()
{
	_nodesDrawn++; 
}

void CC3PerformanceStatistics::addDrawingCallsMade( GLuint callCount )
{
	_drawingCallsMade += callCount; 
}

void CC3PerformanceStatistics::addFacesPresented( GLuint faceCount )
{
	_facesPresented += faceCount; 
}

void CC3PerformanceStatistics::addSingleCallFacesPresented( GLuint faceCount )
{
	_drawingCallsMade++;
	_facesPresented += faceCount;
}

GLfloat CC3PerformanceStatistics::getUpdateRate()
{
	return _accumulatedUpdateTime ? ((GLfloat)_updatesHandled / _accumulatedUpdateTime) : 0.0f;
}

GLfloat CC3PerformanceStatistics::getAverageNodesUpdatedPerUpdate()
{
	return _framesHandled ? ((GLfloat)_nodesUpdated / (GLfloat)_updatesHandled) : 0.0f;
}

GLfloat CC3PerformanceStatistics::getAverageNodesTransformedPerUpdate()
{
	return _framesHandled ? ((GLfloat)_nodesTransformed / (GLfloat)_updatesHandled) : 0.0f;
}

GLfloat CC3PerformanceStatistics::getFrameRate()
{
	return (_accumulatedFrameTime != 0.0f) ? ((GLfloat)_framesHandled / _accumulatedFrameTime) : 0.0f;
}

GLfloat CC3PerformanceStatistics::getAverageNodesDrawnPerFrame()
{
	return _framesHandled ? ((GLfloat)_nodesDrawn / (GLfloat)_framesHandled) : 0.0f;
}

GLfloat CC3PerformanceStatistics::getAverageNodesVisitedForDrawingPerFrame()
{
	return _framesHandled ? ((GLfloat)_nodesVisitedForDrawing / (GLfloat)_framesHandled) : 0.0f;
}

GLfloat CC3PerformanceStatistics::getAverageDrawingCallsMadePerFrame()
{
	return _framesHandled ? ((GLfloat)_drawingCallsMade / (GLfloat)_framesHandled) : 0.0f;
}

GLfloat CC3PerformanceStatistics::getAverageFacesPresentedPerFrame()
{
	return _framesHandled ? ((GLfloat)_facesPresented / (GLfloat)_framesHandled) : 0.0f;
}

void CC3PerformanceStatistics::init()
{
	reset();
}

CC3PerformanceStatistics* CC3PerformanceStatistics::statistics()
{
	CC3PerformanceStatistics* pStatistics = new CC3PerformanceStatistics;
	pStatistics->init();
	pStatistics->autorelease();

	return pStatistics;
}

void CC3PerformanceStatistics::reset()
{
	_updatesHandled = 0;
	_accumulatedUpdateTime = 0;
	_nodesUpdated = 0;
	_nodesTransformed = 0;
	
	_framesHandled = 0;
	_accumulatedFrameTime = 0.0;
	_nodesVisitedForDrawing = 0;
	_nodesDrawn = 0;
	_drawingCallsMade = 0;
	_facesPresented = 0;
}

void CC3PerformanceStatistics::populateFrom( CC3PerformanceStatistics* another )
{
	_updatesHandled = another->getUpdatesHandled();
	_accumulatedUpdateTime = another->getAccumulatedUpdateTime();
	_nodesUpdated = another->getNodesUpdated();
	_nodesTransformed = another->getNodesTransformed();
	
	_framesHandled = another->getFramesHandled();
	_accumulatedFrameTime = another->getAccumulatedFrameTime();
	_nodesVisitedForDrawing = another->getNodesVisitedForDrawing();
	_nodesDrawn = another->getNodesDrawn();
	_drawingCallsMade = another->getDrawingCallsMade();
	_facesPresented = another->getFacesPresented();
}

CCObject* CC3PerformanceStatistics::copyWithZone( CCZone* zone )
{
	CC3PerformanceStatistics* aCopy = new CC3PerformanceStatistics;
	aCopy->populateFrom( this );
	return aCopy;
}

std::string CC3PerformanceStatistics::fullDescription()
{
	std::string desc = stringWithFormat( (char*)"%CC3PerformanceStatistics fps: %.0f", getFrameRate() );
	return  stringWithFormat( (char*)"%s nodes drawn: %.0f, GL calls: %.0f, faces: %.0f",
			desc.c_str(), getAverageNodesDrawnPerFrame(),
			getAverageDrawingCallsMadePerFrame(), getAverageFacesPresentedPerFrame() );
}

GLuint CC3PerformanceStatistics::getFacesPresented()
{
	return _facesPresented;
}

GLuint CC3PerformanceStatistics::getDrawingCallsMade()
{
	return _drawingCallsMade;
}

GLuint CC3PerformanceStatistics::getNodesDrawn()
{
	return _nodesDrawn;
}

GLuint CC3PerformanceStatistics::getNodesVisitedForDrawing()
{
	return _nodesVisitedForDrawing;
}

GLfloat CC3PerformanceStatistics::getAccumulatedFrameTime()
{
	return _accumulatedFrameTime;
}

GLuint CC3PerformanceStatistics::getFramesHandled()
{
	return _framesHandled;
}

GLuint CC3PerformanceStatistics::getNodesTransformed()
{
	return _nodesTransformed;
}

GLuint CC3PerformanceStatistics::getNodesUpdated()
{
	return _nodesUpdated;
}

GLfloat CC3PerformanceStatistics::getAccumulatedUpdateTime()
{
	return _accumulatedUpdateTime;
}

GLuint CC3PerformanceStatistics::getUpdatesHandled()
{
	return _updatesHandled;
}

GLint* CC3PerformanceStatisticsHistogram::getUpdateRateHistogram()
{
	return _updateRateHistogram; 
}
	   
GLint* CC3PerformanceStatisticsHistogram::getFrameRateHistogram()
{
	return _frameRateHistogram; 
}
	   
GLint CC3PerformanceStatisticsHistogram::getIndexOfInterval( float deltaTime )
{
	return CLAMP((GLint)(1.0 / deltaTime), 0, kCC3RateHistogramSize - 1);
}

void CC3PerformanceStatisticsHistogram::addUpdateTime( float deltaTime )
{
	super::addUpdateTime( deltaTime );
	_updateRateHistogram[getIndexOfInterval(deltaTime)]++;
}

void CC3PerformanceStatisticsHistogram::addFrameTime( float deltaTime )
{
	super::addFrameTime( deltaTime );
	_frameRateHistogram[getIndexOfInterval(deltaTime)]++;
}

void CC3PerformanceStatisticsHistogram::reset()
{
	super::reset();
	memset(_frameRateHistogram, 0, kCC3RateHistogramSize * sizeof(_frameRateHistogram[0]));
	memset(_updateRateHistogram, 0, kCC3RateHistogramSize * sizeof(_updateRateHistogram[0]));
}

void CC3PerformanceStatisticsHistogram::populateFrom( CC3PerformanceStatisticsHistogram* another )
{
	super::populateFrom( another );
	memcpy(_frameRateHistogram, another->getFrameRateHistogram(), kCC3RateHistogramSize * sizeof(_frameRateHistogram[0]));
	memcpy(_updateRateHistogram, another->getUpdateRateHistogram(), kCC3RateHistogramSize * sizeof(_updateRateHistogram[0]));
}

CCObject* CC3PerformanceStatisticsHistogram::copyWithZone( CCZone* zone )
{
	CC3PerformanceStatisticsHistogram* pVal = new CC3PerformanceStatisticsHistogram;
	pVal->init();
	pVal->populateFrom( this );

	return pVal;
}

NS_COCOS3D_END
