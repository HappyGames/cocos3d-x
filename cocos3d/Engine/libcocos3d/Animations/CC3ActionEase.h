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

#ifndef _CC3_ACTION_EASE_H_
#define _CC3_ACTION_EASE_H_

NS_COCOS3D_BEGIN

class CC3ActionEase : public CC3ActionInterval
{
public:
    virtual ~CC3ActionEase();
    
    /** initializes the action */
    bool                        initWithAction(CC3ActionInterval *pAction);
    virtual CCObject*           copyWithZone( CCZone* pZone );
    virtual void                startWithTarget( CC3Node *pTarget );
    virtual void                stop();
    virtual void                update( float time );
    virtual CC3ActionInterval*  reverse();
    virtual CC3ActionInterval*  getInnerAction();
    
public:
    /** creates the action */
    static CC3ActionEase*       create( CC3ActionInterval *pAction );
    
protected:
    /** The inner action */
    CC3ActionInterval*          m_pInner;
};

class CC3EaseBounce : public CC3ActionEase
{
public:
    float                       bounceTime( float time );

    virtual CCObject*           copyWithZone( CCZone* pZone );
    virtual CC3ActionInterval*  reverse();
    
public:
    
    /** creates the action */
    static CC3EaseBounce*       create( CC3ActionInterval* pAction );
};

NS_COCOS3D_END

#endif

