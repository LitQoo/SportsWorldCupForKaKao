//
//  MyMedal.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 7..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//

#ifndef SportsWorldCup_MyMedal_cpp
#define SportsWorldCup_MyMedal_cpp

#include "UIMediator.h"
#include "cocos2d.h"
USING_NS_CC;


class MyMedal : public CCLayer, public UIColleague
{
private:
	UIMediator* mediator;
	CCSprite* thiz;
	CCLabelBMFont* g, *s, *b, *t, *w;
public:
	void syncInfo();
	MyMedal();
	virtual ~MyMedal();
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	static MyMedal* node(UIMediator *m)
	{
		MyMedal *pRet = new MyMedal();
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
