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
#ifndef _CCL_CC3NODELISTENERS_H_
#define _CCL_CC3NODELISTENERS_H_
#include "../Utility/CC3Foundation.h"
#include <pthread.h>

NS_COCOS3D_BEGIN

class CC3Node;
/**
 * This protocol defines the behaviour requirements for objects that wish to be
 * notified about the basic existence of a node.
 */
class CC3NodeListenerProtocol
{
public:
	/**
	 * Callback method that will be invoked when the node has been deallocated.
	 *
	 * Although the sending node is still alive when sending this message, its state is
	 * unpredictable, because all subclass state will have been released or detroyed when
	 * this message is sent. The receiver of this message should not attempt to send any
	 * messages to the sender. Instead, it should simply clear any references to the node.
	 */
	virtual void				nodeWasDestroyed( CC3Node* node ) = 0;
};

/**
 * This protocol defines the behaviour requirements for objects that wish to be
 * notified whenever the transform of a node has changed.
 *
 * This occurs when one of the transform properties (location, rotation & scale)
 * of the node, or any of its structural ancestor nodes, has changed.
 *
 * A transform listener can be registered with a node via the addTransformListener: method.
 *
 * Each listener registered with a node will be sent the nodeWasTransformed: notification
 * message when the globalTransformMatrix of this node is recalculated, or is set directly.
 */
class CC3NodeTransformListenerProtocol : public CC3NodeListenerProtocol
{
public:
	/** Callback method that will be invoked when the globalTransformMatrix of the specified node has changed. */
	virtual void				nodeWasTransformed( CC3Node* node ) = 0;
};

/** Manages a collection of transform listeners on behalf of a CC3Node. */
class CC3NodeTransformListeners : public CCObject 
{
public:
	CC3NodeTransformListeners();
	~CC3NodeTransformListeners();

	/** Returns the number of listeners. */
	unsigned int				getListenerCount();

	/** Returns whether there are no listeners. */
	bool						isEmpty();

	/**
	 * Returns a copy of the collection of objects that have requested that they be notified
	 * whenever the transform of the node has changed.
	 *
	 * Each object in the returned collection implements the CC3NodeTransformListenerProtocol,
	 * and will be sent the nodeWasTransformed: notification message when the transform of this
	 * node changes.
	 *
	 * Objects can be added to this collection by using the addTransformListener: method, and
	 * removed using the removeTransformListener: method. This property returns a copy of the
	 * collection stored in this node. You can safely invoke the addTransformListener: or
	 * removeTransformListener: methods while iterating the returned collection.
	 *
	 * Transform listeners are weakly referenced. Each listener should know who it has subscribed
	 * to, and must remove itself as a listener (using the removeTransformListener: method) when
	 * appropriate, such as when being deallocated.
	 */
	std::vector<CC3NodeTransformListenerProtocol*>&	transformListeners();

	/**
	 * Adds the specified object as a transform listener.
	 *
	 * It is safe to invoke this method more than once for the same listener, or
	 * with a nil listener. In either case, this method simply ignores the request.
	 *
	 * Transform listeners are weakly referenced. Each listener should know who it has subscribed
	 * to, and must remove itself as a listener (using the removeTransformListener: method) when
	 * appropriate, such as when being deallocated.
	 */
	void						addTransformListener( CC3NodeTransformListenerProtocol* listener );

	/**
	 * Removes the specified object as a transform listener.
	 *
	 * It is safe to invoke this method with a listener that was not previously added,
	 * or with a nil listener. In either case, this method simply ignores the request.
	 */
	void						removeTransformListener( CC3NodeTransformListenerProtocol* listener );

	/** Removes all transform listeners. */
	void						removeAllTransformListeners();

	/** Notify the transform listeners that the node has been transformed. */
	void						notifyTransformListeners();

	/** Notify the transform listeners that the node has been destroyed. */
	void						notifyDestructionListeners();

	/** Initializes this instance to track transform listeners for the specified node. */
	void						initForNode( CC3Node* node );

	static CC3NodeTransformListeners* listenersForNode( CC3Node* node );

protected:
	CC3Node*					m_pNode;
	std::vector<CC3NodeTransformListenerProtocol*>	m_transformListenerWrappers;
	pthread_mutex_t				m_mutex;
};


NS_COCOS3D_END

#endif
