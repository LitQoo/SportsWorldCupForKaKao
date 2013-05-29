//
//  Option.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 2. 15..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//

#ifndef SportsWorldCup_Option_cpp
#define SportsWorldCup_Option_cpp
#include "cocos2d.h"
#include "JsonBox.h"
#include "UIMediator.h"
USING_NS_CC;


class Option : public CCLayerColor, public CCTextFieldDelegate, public UIColleague
{
private:
	struct _PositionTag
	{
		CCPoint MOREAPPS, ILIKEIT, EXIT, EFF, BGM, NICKNAME, COUNTRY;
	} position;
	CCSprite* thiz;
	UIMediator* mediator;
	CCSprite* effectSound;
	CCSprite* BGM;
	CCTextFieldTTF* nameTextField;
	CCSprite* country;
	vector<string> countryList;
	void imageClick(char* flagcode);
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
		thiz->runAction(CCMoveTo::actionWithDuration(0.3, ccp(0, 216 - nameTextField->getPosition().y)));
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

	void ILIKEIT(CCObject*);
	void EXIT(CCObject*);
	void REVIEW_YES();
	void GIFTCODE(CCObject*);
public:
	Option()
	{
		position.MOREAPPS = ccp(145, 95);
		position.ILIKEIT = ccp(219, 95);
		position.EXIT = ccp(325, 86);
		position.EFF = ccp(229, 163);
		position.BGM = ccp(339, 163);
		position.COUNTRY = ccp(260, 167);
		position.NICKNAME = ccp(200, 241);
	}
	virtual ~Option();
	void CLOSE(CCObject*);
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	bool getIsValidName();
	void alertInvalidName();
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	static Option* node(UIMediator *m)
	{
		Option *pRet = new Option();
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

#endif
