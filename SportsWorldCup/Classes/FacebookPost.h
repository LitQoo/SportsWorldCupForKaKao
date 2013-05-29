//
//  FacebookPost.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 5..
//
//

#ifndef __SportsWorldCup__FacebookPost__
#define __SportsWorldCup__FacebookPost__

#include "cocos2d.h"
#include "LQTableView.h"
#include "UIMediator.h"

USING_NS_CC;
class IntroHead;
class IntroMain;
class FacebookPost : public CCLayer, public UIColleague
{
private:
	JsonBox::Array fbMailList;

	LQTableView* postBoxView;
	CCSprite* fbLoading;
	CCSprite* thiz;
	UIMediator* mediator;
private:
	LQTableViewCell* mailCell(CCObject* t);
	void GETHEART(CCObject* item);
	void removeRequestFinish(JsonBox::Value v);
	
public:
	virtual ~FacebookPost();
	virtual bool init();
	static FacebookPost* node(UIMediator *m)
	{
		FacebookPost *pRet = new FacebookPost();
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
	void loadRequest();
	void getRequestFinish(JsonBox::Value v2);
	void setMediator(UIMediator* m)
	{
		mediator = m;
	}
	void BACK(CCObject*);
};


#endif /* defined(__SportsWorldCup__FacebookPost__) */
