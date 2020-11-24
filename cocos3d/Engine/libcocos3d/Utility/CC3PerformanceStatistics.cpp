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

void CC3PerformanceStatistics::addUpdateTime( float deltaTime )
{
	m_updatesHandled++;
	m_accumulatedUpdateTime += deltaTime;
}

void CC3PerformanceStatistics::addNodesUpdated( GLuint nodeCount )
{
	m_nodesUpdated += nodeCount; 
}

void CC3PerformanceStatistics::incrementNodesUpdated() 
{
	m_nodesUpdated++; 
}

void CC3PerformanceStatistics::addNodesTransformed( GLuint nodeCount )
{
	m_nodesTransformed += nodeCount; 
}

void CC3PerformanceStatistics::incrementNodesTransformed()
{
	m_nodesTransformed++; 
}

void CC3PerformanceStatistics::addFrameTime( float deltaTime )
{
	m_framesHandled++;
	m_accumulatedFrameTime += deltaTime;
}

void CC3PerformanceStatistics::addNodesVisitedForDrawing( GLuint nodeCount )
{
	m_nodesVisitedForDrawing += nodeCount; 
}

void CC3PerformanceStatistics::incrementNodesVisitedForDrawing()
{
	m_nodesVisitedForDrawing++; 
}

void CC3PerformanceStatistics::addNodesDrawn( GLuint nodeCount )
{
	m_nodesDrawn += nodeCount; 
}

void CC3PerformanceStatistics::incrementNodesDrawn()
{
	m_nodesDrawn++; 
}

void CC3PerformanceStatistics::addDrawingCallsMade( GLuint callCount )
{
	m_drawingCallsMade += callCount; 
}

void CC3PerformanceStatistics::addFacesPresented( GLuint faceCount )
{
	m_facesPresented += faceCount; 
}

void CC3PerformanceStatistics::addSingleCallFacesPresented( GLuint faceCount )
{
	m_drawingCallsMade++;
	m_facesPresented += faceCount;
}

GLfloat CC3PerformanceStatistics::getUpdateRate()
{
	return m_accumulatedUpdateTime ? ((GLfloat)m_updatesHandled / m_accumulatedUpdateTime) : 0.0f;
}

GLfloat CC3PerformanceStatistics::getAverageNodesUpdatedPerUpdate()
{
	return m_framesHandled ? ((GLfloat)m_nodesUpdated / (GLfloat)m_updatesHandled) : 0.0f;
}

GLfloat CC3PerformanceStatistics::getAverageNodesTransformedPerUpdate()
{
	return m_framesHandled ? ((GLfloat)m_nodesTransformed / (GLfloat)m_updatesHandled) : 0.0f;
}

GLfloat CC3PerformanceStatistics::getFrameRate()
{
	return (m_accumulatedFrameTime != 0.0f) ? ((GLfloat)m_framesHandled / m_accumulatedFrameTime) : 0.0f;
}

GLfloat CC3PerformanceStatistics::getAverageNodesDrawnPerFrame()
{
	return m_framesHandled ? ((GLfloat)m_nodesDrawn / (GLfloat)m_framesHandled) : 0.0f;
}

GLfloat CC3PerformanceStatistics::getAverageNodesVisitedForDrawingPerFrame()
{
	return m_framesHandled ? ((GLfloat)m_nodesVisitedForDrawing / (GLfloat)m_framesHandled) : 0.0f;
}

GLfloat CC3PerformanceStatistics::getAverageDrawingCallsMadePerFrame()
{
	return m_framesHandled ? ((GLfloat)m_drawingCallsMade / (GLfloat)m_framesHandled) : 0.0f;
}

GLfloat CC3PerformanceStatistics::getAverageFacesPresentedPerFrame()
{
	return m_framesHandled ? ((GLfloat)m_facesPresented / (GLfloat)m_framesHandled) : 0.0f;
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
	m_updatesHandled = 0;
	m_accumulatedUpdateTime = 0;
	m_nodesUpdated = 0;
	m_nodesTransformed = 0;
	
	m_framesHandled = 0;
	m_accumulatedFrameTime = 0.0;
	m_nodesVisitedForDrawing = 0;
	m_nodesDrawn = 0;
	m_drawingCallsMade = 0;
	m_facesPresented = 0;
}

void CC3PerformanceStatistics::populateFrom( CC3PerformanceStatistics* another )
{
	m_updatesHandled = another->getUpdatesHandled();
	m_accumulatedUpdateTime = another->getAccumulatedUpdateTime();
	m_nodesUpdated = another->getNodesUpdated();
	m_nodesTransformed = another->getNodesTransformed();
	
	m_framesHandled = another->getFramesHandled();
	m_accumulatedFrameTime = another->getAccumulatedFrameTime();
	m_nodesVisitedForDrawing = another->getNodesVisitedForDrawing();
	m_nodesDrawn = another->getNodesDrawn();
	m_drawingCallsMade = another->getDrawingCallsMade();
	m_facesPresented = another->getFacesPresented();
}

CCObject* CC3PerformanceStatistics::copyWithZone( CCZone* zone )
{
	CC3PerformanceStatistics* aCopy = new CC3PerformanceStatistics;
	aCopy->populateFrom( this );
	return aCopy;
}

std::string CC3PerformanceStatistics::fullDescription()
{
	std::string desc = CC3String::stringWithFormat( (char*)"%CC3PerformanceStatistics fps: %.0f", getFrameRate() );
	return  CC3String::stringWithFormat( (char*)"%s nodes drawn: %.0f, GL calls: %.0f, faces: %.0f",
			desc.c_str(), getAverageNodesDrawnPerFrame(),
			getAverageDrawingCallsMadePerFrame(), getAverageFacesPresentedPerFrame() );
}

GLuint CC3PerformanceStatistics::getFacesPresented()
{
	return m_facesPresented;
}

GLuint CC3PerformanceStatistics::getDrawingCallsMade()
{
	return m_drawingCallsMade;
}

GLuint CC3PerformanceStatistics::getNodesDrawn()
{
	return m_nodesDrawn;
}

GLuint CC3PerformanceStatistics::getNodesVisitedForDrawing()
{
	return m_nodesVisitedForDrawing;
}

GLfloat CC3PerformanceStatistics::getAccumulatedFrameTime()
{
	return m_accumulatedFrameTime;
}

GLuint CC3PerformanceStatistics::getFramesHandled()
{
	return m_framesHandled;
}

GLuint CC3PerformanceStatistics::getNodesTransformed()
{
	return m_nodesTransformed;
}

GLuint CC3PerformanceStatistics::getNodesUpdated()
{
	return m_nodesUpdated;
}

GLfloat CC3PerformanceStatistics::getAccumulatedUpdateTime()
{
	return m_accumulatedUpdateTime;
}

GLuint CC3PerformanceStatistics::getUpdatesHandled()
{
	return m_updatesHandled;
}

GLint* CC3PerformanceStatisticsHistogram::getUpdateRateHistogram()
{
	return m_updateRateHistogram; 
}
	   
GLint* CC3PerformanceStatisticsHistogram::getFrameRateHistogram()
{
	return m_frameRateHistogram; 
}
	   
GLint CC3PerformanceStatisticsHistogram::getIndexOfInterval( float deltaTime )
{
	return CLAMP((GLint)(1.0 / deltaTime), 0, kCC3RateHistogramSize - 1);
}

void CC3PerformanceStatisticsHistogram::addUpdateTime( float deltaTime )
{
	super::addUpdateTime( deltaTime );
	m_updateRateHistogram[getIndexOfInterval(deltaTime)]++;
}

void CC3PerformanceStatisticsHistogram::addFrameTime( float deltaTime )
{
	super::addFrameTime( deltaTime );
	m_frameRateHistogram[getIndexOfInterval(deltaTime)]++;
}

void CC3PerformanceStatisticsHistogram::reset()
{
	super::reset();
	memset(m_frameRateHistogram, 0, kCC3RateHistogramSize * sizeof(m_frameRateHistogram[0]));
	memset(m_updateRateHistogram, 0, kCC3RateHistogramSize * sizeof(m_updateRateHistogram[0]));
}

void CC3PerformanceStatisticsHistogram::populateFrom( CC3PerformanceStatisticsHistogram* another )
{
	super::populateFrom( another );
	memcpy(m_frameRateHistogram, another->getFrameRateHistogram(), kCC3RateHistogramSize * sizeof(m_frameRateHistogram[0]));
	memcpy(m_updateRateHistogram, another->getUpdateRateHistogram(), kCC3RateHistogramSize * sizeof(m_updateRateHistogram[0]));
}

CCObject* CC3PerformanceStatisticsHistogram::copyWithZone( CCZone* zone )
{
	CC3PerformanceStatisticsHistogram* pVal = new CC3PerformanceStatisticsHistogram;
	pVal->init();
	pVal->populateFrom( this );

	return pVal;
}

NS_COCOS3D_END
