//
//  PriorityMenu.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 19..
//
//

#ifndef __PriorityMenu__
#define __PriorityMenu__
#include "cocos2d.h"


class PriorityMenu : public cocos2d::CCMenu
{
private:
	int touchPriority;
	virtual void registerWithTouchDispatcher()
	{
		cocos2d::CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, touchPriority, true);
		//CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, kCCMenuTouchPriority - 3, true);
	}
	
public:
	void setTouchPriority(int t){touchPriority = t;}
	static PriorityMenu* node(int touchPriority_)
	{
		return menuWithItem(touchPriority_, NULL);
	}
	static PriorityMenu* menuWithItem(int touchPriority_, cocos2d::CCMenuItem* item)
	{
		return menuWithItems(touchPriority_, item, NULL);
	}
	static PriorityMenu* menuWithItems(int touchPriority_, cocos2d::CCMenuItem* item, ...)
	{
		va_list args;
		va_start(args,item);
		PriorityMenu *pRet = new PriorityMenu();
		pRet->setTouchPriority(touchPriority_);
		if (pRet && pRet->initWithItems(item, args))
		{
			pRet->autorelease();
			va_end(args);
			return pRet;
		}
		va_end(args);
		CC_SAFE_DELETE(pRet)
		return NULL;
	}
};

#endif /* defined(__SportsWorldCup__PriorityMenu__) */
