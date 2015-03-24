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
#ifndef _CC3_BACKGROUNDER_H_
#define _CC3_BACKGROUNDER_H_

NS_COCOS3D_BEGIN

typedef void (*bgBlock) ( );
/**
 * CC3Backgrounder performs activity on a background thread by submitting tasks to 
 * a Grand Central Dispatch (GCD) queue. In order to ensure that the GL engine is
 * presented activity in an defined order, CC3Backgrounder is a singleton.
 *
 * This core behaviour can be nulified by setting the shouldRunOnRequestingThread property
 * to YES, which forces tasks submitted to this backgrounder to be run on the same thread
 * from which the tasks are queued. This behaviour can be useful when loading OpenGL objects
 * that need to be subsequently deleted. It is important that OpenGL objects are deleted
 * from the same thread on which they are loaded.
 */
class CC3Backgrounder : public CCObject 
{
public:
	CC3Backgrounder();
	~CC3Backgrounder();

	void				init();

	/**
	 * Specifies the priority of the GCD global dispatch queue to which background tasks are dispatched.
	 *
	 * Setting this property will affect any subsequent tasks submitted to the runBlock: method.
	 *
	 * The value of this property must be one of the following GCD constants:
	 *	- DISPATCH_QUEUE_PRIORITY_HIGH
	 *	- DISPATCH_QUEUE_PRIORITY_DEFAULT
	 *	- DISPATCH_QUEUE_PRIORITY_LOW
	 *	- DISPATCH_QUEUE_PRIORITY_BACKGROUND (available starting with iOS 5)
	 *
	 * The initial value of this property is DISPATCH_QUEUE_PRIORITY_BACKGROUND when running
	 * under iOS 5 or above, or DISPATCH_QUEUE_PRIORITY_LOW otherwise.
	 */
	long				getQueuePriority();
	void				setQueuePriority( long priority );

	/** 
	 * If the value of the shouldRunOnRequestingThread property is NO (the default), the specified
	 * block of code is dispatched to the global GCD queue identified by the value of the queuePriority
	 * property, and the current thread continues without waiting for the dispatched code to complete.
	 *
	 * If the value of the shouldRunOnRequestingThread property is YES, the specified block of code
	 * is run immediately on the current thread, and further thread activity waits until the specified
	 * block has completed.
	 */
	void				runBlock( bgBlock block );

	/**
	 * Waits the specified number of seconds, then executes the specified block of code 
	 * either on a background thread, or the current thread, depending on the value of 
	 * the shouldRunOnRequestingThread property.
 
	 * If the value of the shouldRunOnRequestingThread property is NO (the default), the specified
	 * block of code is dispatched to the global GCD queue identified by the value of the queuePriority
	 * property. If the value of the shouldRunOnRequestingThread property is YES, the specified block
	 * of code is run on the current thread.
	 */
	void				runBlock( bgBlock block, float seconds );

	/**
	 * Indicates that tasks should be run on the same thread as the invocator of the task requests.
	 *
	 * The initial value of this property is NO, indicating that tasks will be dispatched to a 
	 * background thread for running. Set this property to YES to force tasks to run on the same
	 * thread as the request is made.
	 */
	bool				shouldRunTasksOnRequestingThread();
	void				setShouldRunTasksOnRequestingThread( bool shouldRun );

	/** Returns the singleton backgrounder instance. */
	static CC3Backgrounder* sharedBackgrounder();

protected:
	void				updateTaskQueuePriority();
	void				initQueuePriority();
	void				initTaskQueue();
	void				deleteTaskQueue();

	void				runBlockNow( bgBlock block ); 

protected:
	//dispatch_queue_t	_taskQueue;
	long				_queuePriority;
	bool				_shouldRunTasksOnRequestingThread : 1;
};


NS_COCOS3D_END

#endif
