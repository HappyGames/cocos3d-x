/*
 * Cocos3D-X 1.0.0 
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
#ifndef _CCL_CC3_ACTIONMANAGER_H_
#define _CCL_CC3_ACTIONMANAGER_H_

NS_COCOS3D_BEGIN
class CC3Action;
class CC3ActionManager : public cocos2d::CCActionManager
{
public:
	/** Adds an action with a target. 
     If the target is already present, then the action will be added to the existing target.
     If the target is not present, a new instance of this target will be created either paused or not, and the action will be added to the newly created target.
     When the target is paused, the queued actions won't be 'ticked'.
     */
    virtual void				addAction( CC3Action *pAction, CC3Node *pTarget, bool paused );
	virtual void				removeAction( CCObject* action );

	/** Active this action manager which will replace the original cocos2d action manager
	 Call this function once in AppDelegate mabye.
	 */
	static void					active();

protected:
	/** Removes an action given an action reference.
     */
    virtual void				removeAction( CC3Action *pAction );
};

NS_COCOS3D_END

#endif
