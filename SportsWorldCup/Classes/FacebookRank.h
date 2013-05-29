//
//  FacebookRank.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 2. 28..
//
//

#ifndef __SportsWorldCup__FacebookRank__
#define __SportsWorldCup__FacebookRank__

#include "cocos2d.h"
#include "LQTableView.h"
#include "IntroHead.h"
#include "IntroMain.h"

USING_NS_CC;

class FacebookRank : public CCLayer, public UIColleague
{
private:
	LQTableView* lqView;
	JsonBox::Array fblist;
	CCMenuItem* postbox;
	CCMenuItem* facebookLogin;
	CCSprite* postboxNewSprite;
	
	CCSprite* thiz;
	CCLabelBMFont* reqCount;
	UIMediator* mediator;
	CCLabelBMFont* leftTimeFnt;
private:
	LQTableViewCell* friendCell(CCObject* t);
		void INVITE(CCObject* item);
	void SENDHEART(CCObject* item);
	void sendHeartFinish(JsonBox::Value v);
	void invitationFinish(JsonBox::Value v);
	
	void facebookLoginFinish(JsonBox::Value v2);
	void POSTBOX(CCObject*);
	void FACEBOOKLOGIN();
	virtual void update(ccTime dt);

public:
	void getRequestFakeFinish(JsonBox::Value v2);
	CCSprite* fbLoading;
	float leftTime_s;
	virtual bool init();
	static FacebookRank* node(UIMediator *m)
	{ 
		FacebookRank *pRet = new FacebookRank();
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
	void facebookGetScoresFinish(JsonBox::Value v2);
	void COUNTRYRANK(CCObject*);
	void VIEWPROFILE(CCObject*);
	void CLOSEPROFILE(CCObject*);
	void finishGetWeek(JsonBox::Object js);
	void getRequestCount(JsonBox::Value v2);
	void setMediator(UIMediator* m)
	{
		mediator = m;
	}
	virtual ~FacebookRank();
};

#endif /* defined(__SportsWorldCup__FacebookRank__) */
