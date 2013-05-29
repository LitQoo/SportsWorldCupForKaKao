//
//  IntroHead.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 2. 5..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//

#ifndef SportsWorldCup_IntroHead_cpp
#define SportsWorldCup_IntroHead_cpp

#include "cocos2d.h"
#include "ShopLayer.h"
#include "UIMediator.h"
#include "JsonBox.h"

USING_NS_CC;

class IntroMain;
class IntroHead : public CCLayer, public UIColleague
{
private:
	CCLabelBMFont* rubyFnt, *goldFnt, *rechargeFnt, *levelFnt, *expPercent;
	
	CCLabelBMFont* lvPerFnt;
	CCSprite* loading;
	bool currentBack;
	CCSprite* exitLabel;
	float introTime;
	float backKeyTime;
	vector<CCSprite*> hearts;
	ShopLayer* shop;
	IntroMain* pMain;
	
	UIMediator* mediator;
public:
	bool isSync; // 시간이 싱크되었는지...
	unsigned int timestamp;
	unsigned int oldLocalTime;
	bool withInc;
private:
	void timeChecker(ccTime dt);
	void checkBackKey(ccTime dt);
	virtual void keyBackClicked();
	void initHeartSprite(); // NSDefault::getHeartNumber() 기준으로 초기화해줌.
	
	
	void deleteSprite(CCNode* _n)
	{
		_n->getParent()->removeChild(_n, true);
	}
	void start(CCNode*, void* s)
	{
		CCDirector::sharedDirector()->replaceScene((CCScene*)s);
		((CCScene*)s)->release();
	}
private:
	void RUBYPLUS(CCObject*);
	void COINPLUS(CCObject*);
	void HEARTPLUS(CCObject*);
public:
	CCProgressTimer* expBar;
	IntroHead();
	virtual ~IntroHead();
	int getHeartSize(){return hearts.size();}
	void addHeartSprite(); // 4개 이하일 때만 호출해야 됨
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	void setMediator(UIMediator* m)
	{
		mediator = m;
	}
	static IntroHead* node(UIMediator *m)
	{
		IntroHead *pRet = new IntroHead();
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
	void refreshEXP();
	void removeHeartAnimation( ); // NSDefault::getHeartNumber() <= PlayInfo::DEFAULT_MAX_HEART 이면 호출함.
	void setRuby(int n);
	void setGold(int n);
	void syncTime();
	void syncTimeWithInc();
	void syncTime(unsigned int st);
	void RETRY();
	void finishSyncTime(JsonBox::Object json);
};

#endif
