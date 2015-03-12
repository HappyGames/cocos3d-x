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
#include "CC3Backgrounder.h"
#include "../OpenGL/CC3OpenGL.h"

NS_COCOS3D_BEGIN

/** The default backgrounder task queue name. */
#define kCC3BackgrounderDefaultTaskQueueName	"org.cocos3d.backgrounder.default"

CC3Backgrounder::CC3Backgrounder()
{

}

CC3Backgrounder::~CC3Backgrounder()
{
	// deleteTaskQueue();
}

long CC3Backgrounder::getQueuePriority()
{
	return _queuePriority; 
}

void CC3Backgrounder::setQueuePriority( long queuePriority )
{
	if (queuePriority == _queuePriority) 
		return;
	_queuePriority = queuePriority;
	updateTaskQueuePriority();
}

/** Set the initial queue priority. */
void CC3Backgrounder::initQueuePriority()
{
	// setQueuePriority( DISPATCH_QUEUE_PRIORITY_BACKGROUND );
}

/** Initialize the serial task queue. */
void CC3Backgrounder::initTaskQueue()
{
	// _taskQueue = dispatch_queue_create(kCC3BackgrounderDefaultTaskQueueName, NULL);
}

/** Delete the serial task queue. */
void CC3Backgrounder::deleteTaskQueue()
{
	// dispatch_release(_taskQueue);
	//_taskQueue = NULL;
}

/** Update the underlying target queue of the serial task queue to match the queue priority. */
void CC3Backgrounder::updateTaskQueuePriority()
{
	// dispatch_set_target_queue(_taskQueue, dispatch_get_global_queue(_queuePriority, 0));
}

void CC3Backgrounder::runBlock( bgBlock block )
{
	if (_shouldRunTasksOnRequestingThread) 
	{
		runBlockNow( block );
	} else {
		// dispatch_async(_taskQueue, ^{ [self runBlockNow: block]; });
	}
}

void CC3Backgrounder::runBlock( bgBlock block, float seconds )
{
	if (_shouldRunTasksOnRequestingThread) {
		// [NSThread.currentThread runBlock: block after: seconds];
	} else {
		// dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (seconds * NSEC_PER_SEC)),
		//			   _taskQueue,
		//			   ^{ [self runBlockNow: block]; });
	}
}

void CC3Backgrounder::runBlockNow( bgBlock block )
{
	block();
}

void CC3Backgrounder::init()
{		
	// Any OpenGL tasks that run on the background rely on the background OpenGL context,
	// which is derived from the foreground OpenGL context. Ensure that the foreground
	// OpenGL context exists before creating the backgrounder and running background tasks.
	CC3OpenGL::sharedGL();
		
	initTaskQueue();
	initQueuePriority();
}

static CC3Backgrounder* _singleton;

CC3Backgrounder* CC3Backgrounder::sharedBackgrounder()
{
	if (!_singleton) 
		_singleton = new CC3Backgrounder;		// retained

	return _singleton;
}

NS_COCOS3D_END
