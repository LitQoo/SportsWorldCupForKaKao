//
//  GiftCode.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 22..
//
//

#ifndef __SportsWorldCup__GiftCode__
#define __SportsWorldCup__GiftCode__

#include "cocos2d.h"
#include "JsonBox.h"
#include "UIMediator.h"
USING_NS_CC;


class GiftCode : public CCLayerColor, public CCTextFieldDelegate, public UIColleague
{
private:
	CCSprite* thiz;
	UIMediator* mediator;
	CCTextFieldTTF* giftCodeTTF;
	
	virtual void registerWithTouchDispatcher();
	virtual bool onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen)
	{
		/*if (_calcCharCount(sender->getString()) > 5)
		 return true;*/
		return false;
	}
	virtual bool onTextFieldAttachWithIME(CCTextFieldTTF * sender)
    {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		thiz->runAction(CCMoveTo::actionWithDuration(0.3, ccp(0, 216 - giftCodeTTF->getPosition().y)));
#endif
		
        return false;
    }
	virtual bool onTextFieldDetachWithIME(CCTextFieldTTF * sender)
    {
		
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		thiz->runAction(CCMoveTo::actionWithDuration(0.3, ccp(0, 0)));
#endif
        return false;
    }
	
	void CANCEL(CCObject*);
	void OK(CCObject*);
	void CONFIRM_CODE(JsonBox::Object js);
public:
	void CLOSE(CCObject*)
	{
		giftCodeTTF->detachWithIME();
		mediator->showRankWithMenu();
	}
	GiftCode()
	{
		
	}
	virtual ~GiftCode(){
		CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
	}
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init();

	static GiftCode* node(UIMediator *m)
	{
		GiftCode *pRet = new GiftCode();
		pRet->setMediator(m);
		if (pRet && pRet->init())
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			delete pRet;
			pRet = NULL;
			return NULL;
		}
	}
	void setMediator(UIMediator* m)
	{
		mediator = m;
	}
	
	
	
};


#endif /* defined(__SportsWorldCup__GiftCode__) */
