/****************************************************************************
 Copyright (c) 2012 ArtPig Software LLC
 
 http://www.artpigsoft.com
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "APSCCActionInterval.h"
#include "APSGraphicGroup.h"

APS_BEGIN_NAMESPACE(artpig)

using namespace cocos2d;

//
// APSCCFadeChildrenTo
//
APSCCFadeChildrenTo::APSCCFadeChildrenTo() :
m_reverseAction(NULL),
m_targetGraphic(NULL),
m_fromOpacity(APS_MAX_OPACITY),
m_toOpacity(APS_MAX_OPACITY)
{
}

APSCCFadeChildrenTo::~APSCCFadeChildrenTo() {
    CC_SAFE_RELEASE(m_reverseAction);
}

APSCCFadeChildrenTo* APSCCFadeChildrenTo::actionWithDuration(float duration, GLubyte opacity, APSGraphic *targetGraphic)
{
	APSCCFadeChildrenTo *pFadeTo = new APSCCFadeChildrenTo();
	pFadeTo->initWithDuration(duration, opacity, targetGraphic);
	pFadeTo->autorelease();
    
    return pFadeTo;
}

bool APSCCFadeChildrenTo::initWithDuration(float duration, GLubyte opacity, APSGraphic *targetGraphic)
{
	if (CCActionInterval::initWithDuration(duration))
	{
        m_targetGraphic = targetGraphic;
        m_toOpacity = opacity;
		return true;
	}
    
	return false;
}

CCObject* APSCCFadeChildrenTo::copyWithZone(cocos2d::CCZone *pZone)
{
	CCZone* pNewZone = NULL;
	APSCCFadeChildrenTo* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject) 
	{
		//in case of being called at sub class
		pCopy = (APSCCFadeChildrenTo*)(pZone->m_pCopyObject);
	}
	else
	{
		pCopy = new APSCCFadeChildrenTo();
		pZone = pNewZone = new CCZone(pCopy);
	}
    
	CCActionInterval::copyWithZone(pZone);
    
	pCopy->initWithDuration(m_fDuration, m_toOpacity, m_targetGraphic);
	
	CC_SAFE_DELETE(pNewZone);
	return pCopy;
}

void APSCCFadeChildrenTo::startWithTarget(CCNode *node)
{
    // node parameter is ignored.
    CCNode *target = m_targetGraphic->getNode();
	CCActionInterval::startWithTarget(target);
    
    m_fromOpacity = m_targetGraphic->getRunningOpacity();
    
    // Before progress this action, fromOpacity is assigned to toOpacity of reverseAction
    if (this->m_reverseAction) {
        this->m_reverseAction->setToOpacity(m_fromOpacity);
    }
}

void APSCCFadeChildrenTo::update(float time)
{
    m_targetGraphic->setRunningOpacity((GLubyte)(m_fromOpacity + (m_toOpacity - m_fromOpacity) * time));
    m_targetGraphic->updateAccOpacity();
}

CCActionInterval *APSCCFadeChildrenTo::reverse() {
    if (!m_reverseAction) {
        this->m_reverseAction = new APSCCFadeChildrenTo;
        this->m_reverseAction->initWithDuration(this->getDuration(), 1.f, this->getTargetGraphic());
    }
    return this->m_reverseAction;
}



//
//  APSCCMoveTo
//
APSCCMoveTo::APSCCMoveTo() :
m_reverseAction(NULL)
{
}

APSCCMoveTo::~APSCCMoveTo() {
    CC_SAFE_RELEASE(m_reverseAction);
}

APSCCMoveTo *APSCCMoveTo::actionWithDuration(float duration, const CCPoint& position) {
    APSCCMoveTo* pAction = new APSCCMoveTo();
    
    pAction->initWithDuration(duration, position);
    pAction->autorelease();
    
    return pAction;
}

CCObject *APSCCMoveTo::copyWithZone(CCZone* pZone) {
	CCZone* pNewZone = NULL;
	APSCCMoveTo* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject) 
	{
		//in case of being called at sub class
		pCopy = (APSCCMoveTo*)(pZone->m_pCopyObject);
	}
	else
	{
		pCopy = new APSCCMoveTo();
		pZone = pNewZone = new CCZone(pCopy);
	}
    
	CCMoveTo::copyWithZone(pZone);
    
	pCopy->initWithDuration(this->getDuration(), m_endPosition);
	
	CC_SAFE_DELETE(pNewZone);
	return pCopy;
}

void APSCCMoveTo::startWithTarget(CCNode *node) {
    CCMoveTo::startWithTarget(node);
    
    // Before progress this action, set reverse action's endPosition.
    if (this->m_reverseAction) {
        ((APSCCMoveTo*)this->m_reverseAction)->m_endPosition = node->getPosition();
    }
}

CCActionInterval *APSCCMoveTo::reverse() {
    if (!m_reverseAction) {
        this->m_reverseAction = new APSCCMoveTo;
        ((APSCCMoveTo*)this->m_reverseAction)->initWithDuration(this->getDuration(), ccp(0.f,0.f));
    }
    return this->m_reverseAction;
}


//
//  APSCCBezierTo
//
APSCCBezierTo::APSCCBezierTo() :
m_reverseAction(NULL)
{
}

APSCCBezierTo::~APSCCBezierTo() {
    CC_SAFE_RELEASE(m_reverseAction);
}

APSCCBezierTo *APSCCBezierTo::actionWithDuration(float duration, const ccBezierConfig& c) {
    APSCCBezierTo* pAction = new APSCCBezierTo();
    
    pAction->initWithDuration(duration, c);
    pAction->autorelease();
    
    return pAction;
}

/** initializes the action with duration, bezier config, and finishRecovery */
bool APSCCBezierTo::initWithDuration(float duration, const ccBezierConfig& c) {
    if (CCBezierTo::initWithDuration(duration, c)) {
        m_originalConfig = c;
        return true;
    } else {
        return false;
    }
    
}

CCObject *APSCCBezierTo::copyWithZone(CCZone* pZone) {
	CCZone* pNewZone = NULL;
	APSCCBezierTo* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject) 
	{
		//in case of being called at sub class
		pCopy = (APSCCBezierTo*)(pZone->m_pCopyObject);
	}
	else
	{
		pCopy = new APSCCBezierTo();
		pZone = pNewZone = new CCZone(pCopy);
	}
    
	CCBezierTo::copyWithZone(pZone);
    
	pCopy->initWithDuration(this->getDuration(), m_originalConfig);
	
	CC_SAFE_DELETE(pNewZone);
	return pCopy;
}

void APSCCBezierTo::startWithTarget(CCNode *node) {
    
    m_sConfig = m_originalConfig;
    
    CCBezierTo::startWithTarget(node);
    
    // Before progress this action, set reverse action's endPosition.
    if (this->m_reverseAction) {
        ((APSCCBezierTo*)this->m_reverseAction)->m_originalConfig.endPosition = node->getPosition();
    }    
}

CCActionInterval *APSCCBezierTo::reverse() {
    if (!m_reverseAction) {
        this->m_reverseAction = new APSCCBezierTo;
        ccBezierConfig config = { this->m_originalConfig.controlPoint_2, this->m_originalConfig.controlPoint_1, ccp(0.f,0.f) };
        this->m_reverseAction->initWithDuration(this->getDuration(), config);
    }
    return this->m_reverseAction;
}

//
//  APSCCRotateTo
//

APSCCRotateTo::APSCCRotateTo() :
m_reverseAction(NULL)
{
}

APSCCRotateTo::~APSCCRotateTo() {
    CC_SAFE_RELEASE(m_reverseAction);
}


APSCCRotateTo *APSCCRotateTo::actionWithDuration(float duration, float angle) {
    APSCCRotateTo *action = new APSCCRotateTo;
    action->initWithDuration(duration, angle);
    action->autorelease();
    return action;
}


CCObject* APSCCRotateTo::copyWithZone(CCZone* pZone) {
	CCZone* pNewZone = NULL;
	APSCCRotateTo* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject) 
	{
		//in case of being called at sub class
		pCopy = (APSCCRotateTo*)(pZone->m_pCopyObject);
	}
	else
	{
		pCopy = new APSCCRotateTo();
		pZone = pNewZone = new CCZone(pCopy);
	}
    
	CCRotateTo::copyWithZone(pZone);
    
	pCopy->initWithDuration(this->getDuration(), this->m_fDstAngle);
	
	CC_SAFE_DELETE(pNewZone);
	return pCopy;
}

void APSCCRotateTo::startWithTarget(CCNode *node) {
    CCRotateTo::startWithTarget(node);
    
    // Before progress this action, set reverse action's dstAngle.
    if (this->m_reverseAction) {
        this->m_reverseAction->m_fDstAngle = node->getRotation();
    }    
}

CCActionInterval *APSCCRotateTo::reverse() {
    if (!m_reverseAction) {
        this->m_reverseAction = new APSCCRotateTo;
        ((APSCCRotateTo*)this->m_reverseAction)->initWithDuration(this->getDuration(), 0.f);
    }
    return this->m_reverseAction;
}


//
//  APSCCScaleTo
//


APSCCScaleTo::APSCCScaleTo() :
m_reverseAction(NULL)
{
}

APSCCScaleTo::~APSCCScaleTo() {
    CC_SAFE_RELEASE(m_reverseAction);
}

APSCCScaleTo *APSCCScaleTo::actionWithDuration(float duration, float sx, float sy) {
    APSCCScaleTo *action = new APSCCScaleTo;
    action->initWithDuration(duration, sx, sy);
    action->autorelease();
    return action;
}

CCObject* APSCCScaleTo::copyWithZone(CCZone* pZone) {
	CCZone* pNewZone = NULL;
	APSCCScaleTo* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject) 
	{
		//in case of being called at sub class
		pCopy = (APSCCScaleTo*)(pZone->m_pCopyObject);
	}
	else
	{
		pCopy = new APSCCScaleTo();
		pZone = pNewZone = new CCZone(pCopy);
	}
    
	CCScaleTo::copyWithZone(pZone);
    
	pCopy->initWithDuration(this->getDuration(), this->m_fEndScaleX, this->m_fEndScaleY);
	
	CC_SAFE_DELETE(pNewZone);
	return pCopy;
}

void APSCCScaleTo::startWithTarget(CCNode *node) {
    CCScaleTo::startWithTarget(node);
    
    // Before progress this action, set reverse action's scale.
    if (this->m_reverseAction) {
        this->m_reverseAction->m_fEndScaleX = node->getScaleX();
        this->m_reverseAction->m_fEndScaleY = node->getScaleY();
    }    
}

CCActionInterval *APSCCScaleTo::reverse() {
    if (!m_reverseAction) {
        this->m_reverseAction = new APSCCScaleTo;
        this->m_reverseAction->initWithDuration(this->getDuration(), 1.f, 1.f);
    }
    return this->m_reverseAction;
}

//
//  APSCCSkewTo
//
APSCCSkewTo::APSCCSkewTo() :
m_reverseAction(NULL)
{
}

APSCCSkewTo::~APSCCSkewTo() {
    CC_SAFE_RELEASE(m_reverseAction);
}

APSCCSkewTo *APSCCSkewTo::actionWithDuration(float duration, float sx, float sy) {
    APSCCSkewTo* pAction = new APSCCSkewTo();
    
    pAction->initWithDuration(duration, sx, sy);
    pAction->autorelease();
    
    return pAction;
}

CCObject *APSCCSkewTo::copyWithZone(CCZone* pZone) {
	CCZone* pNewZone = NULL;
	APSCCSkewTo* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject)
	{
		//in case of being called at sub class
		pCopy = (APSCCSkewTo*)(pZone->m_pCopyObject);
	}
	else
	{
		pCopy = new APSCCSkewTo();
		pZone = pNewZone = new CCZone(pCopy);
	}
    
	CCSkewTo::copyWithZone(pZone);
    
	pCopy->initWithDuration(this->getDuration(), this->m_fEndSkewX, this->m_fEndSkewY);
	
	CC_SAFE_DELETE(pNewZone);
	return pCopy;
}

void APSCCSkewTo::startWithTarget(CCNode *node) {
    CCSkewTo::startWithTarget(node);
    
    // Before progress this action, set reverse action's endPosition.
    if (this->m_reverseAction) {
        APSCCSkewTo* skewTo = (APSCCSkewTo*)this->m_reverseAction;
        skewTo->m_fEndSkewX = node->getSkewX();
        skewTo->m_fEndSkewY = node->getSkewY();
    }
}

CCActionInterval *APSCCSkewTo::reverse() {
    if (!m_reverseAction) {
        this->m_reverseAction = new APSCCSkewTo;
        ((APSCCSkewTo*)this->m_reverseAction)->initWithDuration(this->getDuration(), 0.f, 0.f);
    }
    return this->m_reverseAction;
}

/** @brief APSCCAnimateChildren 
 */

APSCCAnimateChildren::APSCCAnimateChildren() :
m_targetGraphic(NULL),
m_interval(0.1f),
m_wholeRange(false),
m_reversed(false),
m_originFrameIndex(0),
m_frameCount(0)
{
}

APSCCAnimateChildren::~APSCCAnimateChildren() {
    
}

APSCCAnimateChildren *APSCCAnimateChildren::actionWithDuration(float duration, unsigned int frameCount, artpig::APSGraphicGroup *targetGraphic) {
    APSCCAnimateChildren *action = new APSCCAnimateChildren;
    action->initWithDuration(duration, frameCount, targetGraphic);
    action->autorelease();
    return action;
}

APSCCAnimateChildren *APSCCAnimateChildren::actionWithDuration(float duration, unsigned int frameCount, artpig::APSGraphicGroup *targetGraphic, float interval) {
    APSCCAnimateChildren *action = new APSCCAnimateChildren;
    action->initWithDuration(duration, frameCount, targetGraphic, interval);
    action->autorelease();
    return action;
}

bool APSCCAnimateChildren::initWithDuration(float duration, unsigned int frameCount, artpig::APSGraphicGroup *targetGraphic) {
    if (CCActionInterval::initWithDuration(duration)) {
        m_frameCount = frameCount;
        m_targetGraphic = targetGraphic;
        m_wholeRange = true;
        return true;
    }
    return false;
}

bool APSCCAnimateChildren::initWithDuration(float duration, unsigned int frameCount, artpig::APSGraphicGroup *targetGraphic, float interval) {
    if (CCActionInterval::initWithDuration(duration)) {
        m_frameCount = frameCount;
        m_interval = interval;
        m_targetGraphic = targetGraphic;
        m_wholeRange = false;
        return true;
    }
    return false;
}

CCObject* APSCCAnimateChildren::copyWithZone(CCZone* pZone) {
	CCZone* pNewZone = NULL;
	APSCCAnimateChildren* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject) 
	{
		//in case of being called at sub class
		pCopy = (APSCCAnimateChildren*)(pZone->m_pCopyObject);
	}
	else
	{
		pCopy = new APSCCAnimateChildren();
		pZone = pNewZone = new CCZone(pCopy);
	}
    
	APSCCAnimateChildren::copyWithZone(pZone);
    
    if (this->getWholeRange()) {
        pCopy->initWithDuration(this->getDuration(), this->getFrameCount(), this->getTargetGraphic());
    } else {
        pCopy->initWithDuration(this->getDuration(), this->getFrameCount(), this->getTargetGraphic(), this->getInterval());
    }
	
	CC_SAFE_DELETE(pNewZone);
	return pCopy;
}

void APSCCAnimateChildren::update(float time) {
    unsigned int idx;
    
    if (this->getReversed()) {
        time = 1-time;
        if (time < 0.) {
            time = 0.;
        }
    }
    
    unsigned int frameCount = this->getFrameCount();
    if (this->getWholeRange()) {
        idx = time * frameCount;
        
        if( idx >= frameCount )
            idx = frameCount -1;
    } else {
        // +0.005 fixes arithmetic round up
        float duration = this->getDuration()/this->getInterval()+0.005;
        unsigned int frame = time*duration+this->getOriginFrameIndex();
        idx = frame%frameCount;
	}
    
    m_targetGraphic->setRunningFrameIndex(idx);
    
    m_targetGraphic->updateChildSpriteFrames();
	
}

CCActionInterval *APSCCAnimateChildren::reverse() {
    APSCCAnimateChildren *reversedAction = APSCCAnimateChildren::actionWithDuration(this->getDuration(), this->getFrameCount(), this->getTargetGraphic());
    
    reversedAction->m_reversed = !this->getReversed();
    
    return reversedAction;
}


APSCCAnimate::APSCCAnimate() :
m_interval(0.1),
m_originFrameIndex(0)
{
    
}

APSCCAnimate::~APSCCAnimate() {
}

APSCCAnimate *APSCCAnimate::actionWithDuration(float duration, CCAnimation *animation, bool restoreOriginalFrame) {
    APSCCAnimate *action = new APSCCAnimate;
    action->initWithDuration(duration, animation, restoreOriginalFrame);
    action->autorelease();
    return action;
}

APSCCAnimate *APSCCAnimate::actionWithDuration(float duration, CCAnimation *animation, bool restoreOriginalFrame, float interval) {
    APSCCAnimate *action = new APSCCAnimate;
    action->initWithDuration(duration, animation, restoreOriginalFrame, interval);
    action->autorelease();
    return action;
}

bool APSCCAnimate::initWithDuration(float duration, CCAnimation *animation, bool restoreOriginalFrame) {
#if COCOS2D_VERSION >= 0x00020000
    animation->setRestoreOriginalFrame(restoreOriginalFrame);
    if (CCAnimate::initWithAnimation(animation)) {
        this->setDuration(duration);
        m_interval = animation->getDelayPerUnit();
#else
    if (CCAnimate::initWithDuration(duration, animation, restoreOriginalFrame)) {
        m_interval = animation->getDelay();
#endif//APS_SKIP
        return true;
    } else {
        return false;
    }
}

bool APSCCAnimate::initWithDuration(float duration, CCAnimation *animation, bool restoreOriginalFrame, float interval) {
#if COCOS2D_VERSION >= 0x00020000
    animation->setRestoreOriginalFrame(restoreOriginalFrame);
    animation->setDelayPerUnit(interval);
    if (CCAnimate::initWithAnimation(animation)) {
        this->setDuration(duration);
        m_interval = animation->getDelayPerUnit();
#else
    if (CCAnimate::initWithDuration(duration, animation, restoreOriginalFrame)) {
        this->m_interval = interval;
#endif//APS_SKIP
        return true;
    } else {
        return false;
    }
}

CCObject* APSCCAnimate::copyWithZone(CCZone* pZone) {
	CCZone* pNewZone = NULL;
	APSCCAnimate* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject) 
	{
		//in case of being called at sub class
		pCopy = (APSCCAnimate*)(pZone->m_pCopyObject);
	}
	else
	{
		pCopy = new APSCCAnimate();
		pZone = pNewZone = new CCZone(pCopy);
	}
    
	CCAnimate::copyWithZone(pZone);
    
#if COCOS2D_VERSION >= 0x00020000
    this->getAnimation()->setRestoreOriginalFrame(this->getAnimation()->getRestoreOriginalFrame());
    this->getAnimation()->setDelayPerUnit(this->getInterval());
    pCopy->initWithAnimation(this->getAnimation());
    pCopy->setDuration(this->getDuration());
    pCopy->m_interval = this->getInterval();
#else
    pCopy->initWithDuration(this->getDuration(), this->getAnimation(), this->m_bRestoreOriginalFrame, this->getInterval());
#endif//APS_SKIP
	
	CC_SAFE_DELETE(pNewZone);
	return pCopy;
}

CCActionInterval *APSCCAnimate::reverse() {
#if COCOS2D_VERSION >= 0x00020000
    CCArray* pOldArray = this->getAnimation()->getFrames();
    CCArray* pNewArray = CCArray::create(pOldArray->count());
    
    CCARRAY_VERIFY_TYPE(pOldArray, CCAnimationFrame*);
    
    if (pOldArray->count() > 0)
    {
        CCObject* pObj = NULL;
        CCARRAY_FOREACH_REVERSE(pOldArray, pObj)
        {
            CCAnimationFrame* pElement = (CCAnimationFrame*)pObj;
            if (! pElement)
            {
                break;
            }
            
            pNewArray->addObject((CCAnimationFrame*)(pElement->copy()->autorelease()));
        }
    }
    
    CCAnimation *newAnim = CCAnimation::create(pNewArray, this->getAnimation()->getDelayPerUnit(), this->getAnimation()->getLoops());
    newAnim->setRestoreOriginalFrame(this->getAnimation()->getRestoreOriginalFrame());
	return APSCCAnimate::actionWithDuration(m_fDuration, newAnim, this->getAnimation()->getRestoreOriginalFrame(), this->getInterval());
#else
	CCMutableArray<CCSpriteFrame*> *pOldArray = this->getAnimation()->getFrames();
	CCMutableArray<CCSpriteFrame*> *pNewArray = new CCMutableArray<CCSpriteFrame*>(pOldArray->count());
    
	if (pOldArray->count() > 0)
	{
		CCSpriteFrame *pElement;
		CCMutableArray<CCSpriteFrame*>::CCMutableArrayRevIterator iter;
		for (iter = pOldArray->rbegin(); iter != pOldArray->rend(); iter++)
		{
			pElement = *iter;
			if (! pElement)
			{
				break;
			}
            
			pNewArray->addObject((CCSpriteFrame*)(pElement->copy()->autorelease()));
		}
	}
    
	CCAnimation *pNewAnim = CCAnimation::animationWithFrames(pNewArray, m_pAnimation->getDelay());
	pNewArray->release();
    
	return APSCCAnimate::actionWithDuration(m_fDuration, pNewAnim, m_bRestoreOriginalFrame, this->getInterval());
#endif//APS_SKIP
}


APS_END_NAMESPACE

