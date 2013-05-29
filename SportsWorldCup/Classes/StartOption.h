//
//  StartOption.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 6..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//

#ifndef SportsWorldCup_StartOption_cpp
#define SportsWorldCup_StartOption_cpp

#include "cocos2d.h"
USING_NS_CC;


class StartOption : public CCLayer, public CCTextFieldDelegate
{
public:
	StartOption();
	virtual ~StartOption();
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void registerWithTouchDispatcher();
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	static CCScene* scene() {
		CCScene* scene = CCScene::node();
		StartOption * layer = StartOption::node();
		scene->addChild(layer);
		return scene;
	}

	LAYER_NODE_FUNC(StartOption);
private:
	CCSprite* mini_back;
	CCLayer* myLayer;
	void OK(CCObject*);
	CCTextFieldTTF* nameTextField;
	CCSprite* country;
	vector<string> countryList;
	void imageClick(char* flagcode);
	virtual bool onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen) ///< ¿œ¥‹ ¿‘∑¬« ≈Õ æ¯¿Ω.
	{
		return false;
	}
	virtual bool onTextFieldAttachWithIME(CCTextFieldTTF * sender)
    {
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		myLayer->runAction(CCMoveTo::actionWithDuration(0.3, ccp(0, 260 - nameTextField->getPosition().y)));
//#endif
        return false;
    }
	virtual bool onTextFieldDetachWithIME(CCTextFieldTTF * sender)
    {
		
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		myLayer->runAction(CCMoveTo::actionWithDuration(0.3, ccp(0, 0)));
//#endif
        return false;
    }
	virtual void keyBackClicked()
	{
		CCDirector::sharedDirector()->end();
	}
};

#endif
