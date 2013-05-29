//
//  GameOver.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 2. 27..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//

#ifndef SportsWorldCup_GameOver_cpp
#define SportsWorldCup_GameOver_cpp

#include "cocos2d.h"
#include "JsonBox.h"
#include "UIMediator.h"
#include "HWFromTo.h"
USING_NS_CC;


#define PUT_SCORE(CLASS, SCORE, WEEK, RANK) do{\
	string res; \
	if(CLASS::getMaxScore() < SCORE)\
	{ \
		CLASS::setMaxScore(SCORE); \
		CLASS::setWeeklyMaxScore(WEEK, SCORE); \
		res = "max"; \
	} \
	else \
	{\
		if(WEEK != CLASS::getWeeklyMaxScore().first)\
		{\
			CLASS::setWeeklyMaxScore(WEEK, SCORE);\
			res = "weekly";\
		}\
		else\
		{\
			if(CLASS::getWeeklyMaxScore().second < myscore)\
			{ \
				CLASS::setWeeklyMaxScore(WEEK, SCORE);\
				res = "weekly";\
			}\
			else\
			{\
				res = "myscore";\
			}\
		}\
	}\
	mediator->showJudgeOnGameOver(res, RANK, SCORE, CLASS::getMaxScore(), CLASS::getWeeklyMaxScore().second); \
}while(0)

#define SCENE_FUNC(_A) static CCScene* scene() { CCScene* scene = CCScene::node(); _A * layer = _A::node(); scene->addChild(layer); return scene; }
class GameOver : public CCSprite, public UIColleague
{
private:
	struct __POSITION{
		CCPoint rank[10];
		CCPoint flag[10];
		CCPoint user[10];
		CCPoint score[10];
		CCPoint time[10];
		CCPoint beginRankPosition;
		CCPoint endRankPosition;
	}position;
	CCSprite* thiz;
	UIMediator* mediator;
		void update(ccTime dt);
	
	
public:
	int beforeEXP;
	int afterEXP;
	GameOver();
	virtual ~GameOver();
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	static GameOver* node(UIMediator *m)
	{
		GameOver *pRet = new GameOver();
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
	void appearMe();
public:
	void finishGetScores(JsonBox::Object js);
};

#endif
