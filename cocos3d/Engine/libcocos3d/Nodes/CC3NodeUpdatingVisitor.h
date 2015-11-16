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
#ifndef _CCL_CC3NODE_UPDATING_VISITOR_H_
#define _CCL_CC3NODE_UPDATING_VISITOR_H_

NS_COCOS3D_BEGIN
class CC3Node;

/**
 * CC3NodeUpdatingVisitor is a CC3NodeVisitor that is passed to a node when it is visited
 * during updating and transforming operations.
 *
 * This visitor encapsulates the time since the previous update.
 */
class CC3NodeUpdatingVisitor : public CC3NodeVisitor 
{
	DECLARE_SUPER( CC3NodeVisitor );
public:
	static CC3NodeUpdatingVisitor* visitor();
	/**
	 * This property gives the interval, in seconds, since the previous update. This value can be
	 * used to create realistic real-time motion that is independent of specific frame or update rates.
	 * Depending on the setting of the maxUpdateInterval property of the CC3Scene instance, the value
	 * of this property may be clamped to an upper limit. See the description of the CC3Scene
	 * maxUpdateInterval property for more information about clamping the update interval.
	 */
	float						getDeltaTime();
	void						setDeltaTime( float dt );

	virtual void				processBeforeChildren( CC3Node* aNode );
	virtual void				processAfterChildren( CC3Node* aNode );
	std::string					fullDescription();

protected:
	float						_deltaTime;
};

NS_COCOS3D_END

#endif
