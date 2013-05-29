#pragma once

#include "cocos2d.h"
#include "KS_Util.h"
#include <stdarg.h>
#include <string>
#include "Bridge.h"
using namespace std;
USING_NS_CC;

#define NOPARAM (CCObject*)0, (SEL_CallFunc)0

class KSCoverLayer : public CCLayerColor
{
public:
	static int touchPriority;
private:
	virtual void registerWithTouchDispatcher()
	{
		CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, touchPriority, true);
	}
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		return true; // swallow;
	}
public:
	KSCoverLayer() : CCLayerColor(){
		touchPriority--;
	}
	virtual ~KSCoverLayer() {
		touchPriority++;
	}

	virtual bool init()
	{
		if ( !this->CCLayerColor::initWithColor(ccc4f(0,0,0,150)) )
			return false;
		m_bIsRelativeAnchorPoint = true;
		this->setIsTouchEnabled(true);
		return true;
	}
	LAYER_NODE_FUNC(KSCoverLayer);
};

class KSCallBack : public CCNode
{
public:
	KSCallBack(CCObject * _selectorTarget,
		SEL_CallFunc _selector )
	{
		selectorTarget = _selectorTarget;
		selector = _selector;
	}

	virtual ~KSCallBack()
	{

	}
	static KSCallBack* callBackWithOption(
		CCLayer* coverLayer,
		CCObject *selectorTarget,
		SEL_CallFunc selector)
	{
		KSCallBack *cncb = new KSCallBack(selectorTarget, selector);
		coverLayer->addChild(cncb);
		cncb->release();
		return cncb;
	}
	void Execute(CCObject *menu_item);
private:
	CC_SYNTHESIZE(CCObject*, selectorTarget, selectorTarget);
	CC_SYNTHESIZE(SEL_CallFunc, selector, selector);
};

void KSModalAlert(	char const* title,	char const *message, int nargs, ...);