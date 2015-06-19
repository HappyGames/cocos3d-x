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
#include "cocos3d.h"

NS_COCOS3D_BEGIN

/** Returns index of first occurrence of object, CC_INVALID_INDEX if object not found. */
unsigned int getObjectIndexInArray(ccArray *arr, CCObject* object)
{
	const unsigned int arrNum = arr->num;
	CCObject** ptr = arr->arr;
	for(unsigned int i = 0; i < arrNum; ++i, ++ptr)
	{
		if( *ptr == object ) return i;
	}

	return CC_INVALID_INDEX;
}

/** Returns a Boolean value that indicates whether object is present in array. */
bool arrayContainsObject(ccArray *arr, CCObject* object)
{
	return getObjectIndexInArray(arr, object) != CC_INVALID_INDEX;
}

/** Appends an object. Behavior undefined if array doesn't have enough capacity. */
void appendObjectToArray(ccArray *arr, CCObject* object)
{
	CCAssert(object != NULL, "Invalid parameter!");
	object->retain();
	arr->arr[arr->num] = object;
	arr->num++;
}

void CC3ActionManager::addAction( CC3Action *pAction, CC3Node *pTarget, bool paused )
{
	CCAssert(pAction != NULL, "");
	CCAssert(pTarget != NULL, "");

	_hashElement *pElement = NULL;
	// we should convert it to CCObject*, because we save it as CCObject*
	CCObject *tmp = pTarget;
	HASH_FIND_INT(m_pTargets, &tmp, pElement);
	if (! pElement)
	{
		pElement = (_hashElement*)calloc(sizeof(*pElement), 1);
		pElement->paused = paused;
		pTarget->retain();
		pElement->target = pTarget;
		HASH_ADD_INT(m_pTargets, target, pElement);
	}

	actionAllocWithHashElement(pElement);

	CCAssert(! arrayContainsObject(pElement->actions, pAction), "");
	appendObjectToArray(pElement->actions, pAction);

	pAction->startWithTarget(pTarget);
}

void CC3ActionManager::removeAction( CC3Action *pAction )
{ 
	// explicit null handling
	if (pAction == NULL)
	{
		return;
	}

	tHashElement *pElement = NULL;
	CCObject *pTarget = pAction->getTargetNode();
	HASH_FIND_INT(m_pTargets, &pTarget, pElement);
	if (pElement)
	{
		unsigned int i = getObjectIndexInArray(pElement->actions, pAction);
		if (UINT_MAX != i)
		{
			removeActionAtIndex(i, pElement);
		}
	}
	else
	{
		CC3_WARNING( "CC3ActionManager::removeAction: Target not found" );
	}
}

void CC3ActionManager::removeAction( CCObject* action )
{
	CCAction* pAction = (CCAction*)action;
	if ( ((int)pAction->getUserData()) > 0 )
		removeAction( (CC3Action*)pAction );
	else
		CCActionManager::removeAction( pAction );
}

void CC3ActionManager::active()
{
	//////////////////////////////////////////////////////////////////////////
	// Use a customized action manager
	// Unschedule first
	CCDirector *pDirector = CCDirector::sharedDirector();
	CCActionManager* pMgr = pDirector->getActionManager();
	pDirector->getScheduler()->unscheduleUpdateForTarget( pMgr );
	CC3ActionManager* pActionMgr = new CC3ActionManager;
	pDirector->setActionManager( pActionMgr );
	// Reschedule action manager
	pDirector->getScheduler()->scheduleUpdateForTarget( pActionMgr, kCCPrioritySystem, false );
	pActionMgr->release();
}

NS_COCOS3D_END
