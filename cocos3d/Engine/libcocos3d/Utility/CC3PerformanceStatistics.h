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
#ifndef _CCL_PERFORMANCE_STATISTICS_H_
#define _CCL_PERFORMANCE_STATISTICS_H_

NS_COCOS3D_BEGIN
/**
 * Collects statistics about the updating and drawing performance of the 3D scene.
 *
 * To allow flexibility in calculating statistics, this class does not automatically
 * clear the accumulated statistics. It is the responsibility of the application to
 * read the values, and invoke the reset method on the instance periodically, to ensure
 * this instance does not overflow. Depending on the complexity and capabilities of your
 * application, you should reset the performance statistics at least every few seconds.
 */
class CC3PerformanceStatistics : public CCObject
{
public:
	void						init();

	/** The number of updates that have been processed since the reset method was last invoked. */
	GLuint						getUpdatesHandled();

	/** The total time accumulated for updates since the reset method was last invoked. */
	GLfloat						getAccumulatedUpdateTime();

	/**
	 * Adds the specified single-update delta-time to the accumulated update time,
	 * and increments the count of updates handled by one.
	 */
	virtual void				addUpdateTime( float deltaTime );

	/** The total number of nodes updated since the reset method was last invoked. */
	GLuint						getNodesUpdated();

	/** Adds the specified number of nodes to the nodesUpdated property.  */
	void						addNodesUpdated( GLuint nodeCount );

	/** Increments the nodesUpdated property by one. */
	void						incrementNodesUpdated();

	/**
	 * The total number of nodes whose globalTransformMatrix was recalculated
	 * since the reset method was last invoked.
	 */
	GLuint						getNodesTransformed();

	/** Adds the specified number of nodes to the nodesTransformed property.  */
	void						addNodesTransformed( GLuint nodeCount );

	/** Increments the nodesTransformed property by one. */
	void						incrementNodesTransformed();

	/** The number of frames that have been processed since the reset method was last invoked. */
	GLuint						getFramesHandled();

	/** The total time accumulated for frames since the reset method was last invoked. */
	float						getAccumulatedFrameTime();

	/**
	 * Adds the specified single-frame delta-time to the accumulated frame time,
	 * and increments the count of frame handled by one.
	 */
	virtual void				addFrameTime( float deltaTime );

	/**
	 * The total number of nodes visited for drawing since the reset method was last invoked.
	 * This includes both nodes that were drawn, and nodes that were culled prior to drawing.
	 *
	 * The difference between this property and the nodesDrawn property is the total number of
	 * nodes  that were not visible or were culled and not presented to the GL engine for drawing.
	 */
	GLuint						getNodesVisitedForDrawing();

	/** Adds the specified number of nodes to the nodesVisitedForDrawing property.  */
	void						addNodesVisitedForDrawing( GLuint nodeCount );

	/** Increments the nodesVisitedForDrawing property by one. */
	void						incrementNodesVisitedForDrawing();

	/**
	 * The total number of nodes drawn by the GL engine since the reset method was last invoked.
	 *
	 * The difference between the nodesVisitedForDrawing property and this property is the
	 * total number of nodes that were not visible or were culled and not presented to the
	 * GL engine for drawing.
	 */
	GLuint						getNodesDrawn();

	/** Adds the specified number of nodes to the nodesDrawn property.  */
	void						addNodesDrawn( GLuint nodeCount );

	/** Increments the nodesDrawn property by one. */
	void						incrementNodesDrawn();

	/**
	 * The total number of drawing calls that were made to the GL engine
	 * (glDrawArrays & glDrawElements) since the reset method was last invoked.
	 */
	GLuint						getDrawingCallsMade();

	/** Adds the specified number of drawing calls to the drawingCallsMade property.  */
	void						addDrawingCallsMade( GLuint callCount );

	/**
	 * The total number of triangle faces presented to the GL engine since the reset method
	 * was last invoked.
	 *
	 * When drawing lines or points, this will be the total number of lines
	 * or points presented to the GL engine. This is not necessarily the number of triangles
	 * (or other primitives) that were actually drawn, because the GL engine will cull faces
	 * that are not visible to the camera.
	 */
	GLuint						getFacesPresented();

	/** Adds the specified number of faces to the facesPresented property.  */
	void						addFacesPresented( GLuint faceCount );

	/**
	 * Canvenience method that adds the specified number of faces to the facesPresented
	 * property, and increments by one the number of drawing calls made.
	 */
	void						addSingleCallFacesPresented( GLuint faceCount );

	/**
	 * The average update rate, calculated by dividing the
	 * updatesHandled property by the accumulatedUpdateTime property.
	 */
	GLfloat						getUpdateRate();

	/**
	 * The average nodes updated per update, calculated by dividing the
	 * nodesUpdated property by the updatesHandled property.
	 */
	GLfloat						getAverageNodesUpdatedPerUpdate();

	/**
	 * The average nodes whose globalTransformMatrix was recalculated per update, calculated
	 * by dividing the nodesTransformed property by the updatesHandled property.
	 */
	GLfloat						getAverageNodesTransformedPerUpdate();

	/**
	 * The average drawing frame rate, calculated by dividing the
	 * framesHandled property by the accumulatedFrameTime property.
	 */
	GLfloat						getFrameRate();

	/**
	 * The average nodes visited per drawing frame, calculated by dividing the
	 * nodesVisitedForDrawing property by the framesHandled property.
	 *
	 * The difference between this property and the averageNodesDrawnPerFrame property is
	 * the average number of nodes per frame that were not visible or were culled and not
	 * presented to the GL engine for drawing.
	 */
	GLfloat						getAverageNodesVisitedForDrawingPerFrame();

	/**
	 * The average nodes drawn per drawing frame, calculated by dividing the nodesDrawn
	 * property by the framesHandled property.
	 *
	 * The difference between the averageNodesVisitedForDrawingPerFrame property and this
	 * property is the average number of nodes per frame that were not visible or were
	 * culled and not presented to the GL engine for drawing.
	 */
	GLfloat						getAverageNodesDrawnPerFrame();

	/**
	 * The average GL drawing calls made per drawing frame, calculated by dividing the
	 * drawingCallsMade property by the framesHandled property.
	 */
	GLfloat						getAverageDrawingCallsMadePerFrame();

	/**
	 * The average number of triangle faces presented to the GL engine per drawing frame,
	 * calculated by dividing the facesPresented property by the framesHandled property.
	 *
	 * When drawing lines or points, this will be the total number of lines
	 * or points presented to the GL engine. This is not necessarily the number of triangles
	 * (or other primitives) that were actually drawn, because the GL engine will cull faces
	 * that are not visible to the camera.
	 */
	GLfloat						getAverageFacesPresentedPerFrame();

	/** Allocates and initializes an autoreleased instance. */
	static CC3PerformanceStatistics* statistics();

	/**
	 * Resets all the performance statistics back to zero.
	 *
	 * To allow flexibility in calculating statistics, this class does not automatically
	 * clear the accumulated statistics. It is the responsibility of the application to
	 * read the values, and invoke the reset method on the instance periodically, to ensure
	 * this instance does not overflow. Depending on the complexity and capabilities of your
	 * application, you should reset the performance statistics at least every few seconds.
	 */
	virtual void				reset();

	/** Returns a detailed descripton of this instance. */
	std::string					fullDescription();

	void						populateFrom( CC3PerformanceStatistics* another );
	virtual CCObject*			copyWithZone( CCZone* zone );

protected:
	GLuint						_updatesHandled;
	float						_accumulatedUpdateTime;
	GLuint						_nodesUpdated;
	GLuint						_nodesTransformed;
	
	GLuint						_framesHandled;
	float						_accumulatedFrameTime;
	GLuint						_nodesVisitedForDrawing;
	GLuint						_nodesDrawn;
	GLuint						_drawingCallsMade;
	GLuint						_facesPresented;
};

// Number of buckets in each of the histograms
#define kCC3RateHistogramSize 80

/**
 * Collects statistics about the updating and drawing performance of the 3D scene,
 * including a histogram for each of the raw updateRate and frameRate properties.
 *
 * These histograms provide more detail than the updateRate and frameRate properties,
 * which are, respectively, averages of the individual updateRates and frameRates,
 * since the previous reset.
 *
 * To allow flexibility in calculating statistics, this class does not automatically
 * clear the accumulated statistics, including the histograms. It is the responsibility
 * of the application to read the values, and invoke the reset method on the instance
 * periodically, to ensure this instance does not overflow. Depending on the complexity
 * and capabilities of your application, you should reset the performance statistics at
 * least every few seconds.
 */
class CC3PerformanceStatisticsHistogram : public CC3PerformanceStatistics 
{
	DECLARE_SUPER( CC3PerformanceStatistics );
public:
	/**
	 * Returns a histogram of the value of the update rate, as calculated on each update
	 * pass. This provides more detail than the updateRate property, which is an average
	 * of the individual updateRates, since the previous reset.
	 *
	 * This histogram is cleared when the reset method is invoked.
	 */
	GLint*						getUpdateRateHistogram();

	/**
	 * Returns a histogram of the value of the frame rate, as calculated on each frame
	 * drawing pass. This provides more detail than the frameRate property, which is an
	 * average of the individual frameRates, since the previous reset.
	 *
	 * This histogram is cleared when the reset method is invoked.
	 */
	GLint*						getFrameRateHistogram();

	GLint						getIndexOfInterval( float deltaTime );

	virtual void				addUpdateTime( float deltaTime );
	virtual void				addFrameTime( float deltaTime );
	virtual void				reset();

	void						populateFrom( CC3PerformanceStatisticsHistogram* another );
	virtual	CCObject*			copyWithZone( CCZone* zone );

protected:
	GLint						_updateRateHistogram[kCC3RateHistogramSize];
	GLint						_frameRateHistogram[kCC3RateHistogramSize];
};

NS_COCOS3D_END

#endif
