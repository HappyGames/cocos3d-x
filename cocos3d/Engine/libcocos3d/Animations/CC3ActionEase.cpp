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

CC3ActionEase* CC3ActionEase::create( CC3ActionInterval *pAction )
{
    CC3ActionEase *pRet = new CC3ActionEase();
    if (pRet)
    {
        if (pRet->initWithAction(pAction))
            pRet->autorelease();
        else
            CC_SAFE_RELEASE_NULL(pRet);
    }
    
    return pRet;
}

bool CC3ActionEase::initWithAction( CC3ActionInterval *pAction )
{
    CCAssert(pAction != NULL, "");
    
    if (CC3ActionInterval::initWithDuration(pAction->getDuration()))
    {
        m_pInner = pAction;
        pAction->retain();
        
        return true;
    }
    
    return false;
}

CCObject* CC3ActionEase::copyWithZone( CCZone *pZone )
{
    CCZone* pNewZone = NULL;
    CC3ActionEase* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject)
    {
        //in case of being called at sub class
        pCopy = (CC3ActionEase*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CC3ActionEase();
        pZone = pNewZone = new CCZone(pCopy);
    }
    
    CC3ActionInterval::copyWithZone(pZone);
    
    pCopy->initWithAction((CC3ActionInterval *)(m_pInner->copy()->autorelease()));
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

CC3ActionEase::~CC3ActionEase(void)
{
    CC_SAFE_RELEASE(m_pInner);
}

void CC3ActionEase::startWithTarget(CC3Node *pTarget)
{
    CC3ActionInterval::startWithTarget(pTarget);
    m_pInner->startWithTarget(m_pTargetCC3Node);
}

void CC3ActionEase::stop(void)
{
    m_pInner->stop();
    CC3ActionInterval::stop();
}

void CC3ActionEase::update(float time)
{
    m_pInner->update(time);
}

CC3ActionInterval* CC3ActionEase::reverse(void)
{
    return CC3ActionEase::create(m_pInner->reverse());
}

CC3ActionInterval* CC3ActionEase::getInnerAction()
{
    return m_pInner;
}


//
// EaseBounce
//

CC3EaseBounce* CC3EaseBounce::create( CC3ActionInterval* pAction )
{
    CC3EaseBounce *pRet = new CC3EaseBounce();
    if (pRet)
    {
        if (pRet->initWithAction(pAction))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }
    
    return pRet;
}

CCObject* CC3EaseBounce::copyWithZone(CCZone *pZone)
{
    CCZone* pNewZone = NULL;
    CC3EaseBounce* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject)
    {
        //in case of being called at sub class
        pCopy = (CC3EaseBounce*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CC3EaseBounce();
        pNewZone = new CCZone(pCopy);
    }
    
    pCopy->initWithAction((CC3ActionInterval *)(m_pInner->copy()->autorelease()));
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

float CC3EaseBounce::bounceTime(float time)
{
    if (time < 1 / 2.75)
    {
        return 7.5625f * time * time;
    } else
        if (time < 2 / 2.75)
        {
            time -= 1.5f / 2.75f;
            return 7.5625f * time * time + 0.75f;
        } else
            if(time < 2.5 / 2.75)
            {
                time -= 2.25f / 2.75f;
                return 7.5625f * time * time + 0.9375f;
            }
    
    time -= 2.625f / 2.75f;
    return 7.5625f * time * time + 0.984375f;
}

CC3ActionInterval* CC3EaseBounce::reverse()
{
    return CC3EaseBounce::create(m_pInner->reverse());
}


NS_COCOS3D_END
