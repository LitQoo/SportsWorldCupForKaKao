//
//  WorldCupMiddle.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 4. 4..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//

#ifndef SportsWorldCup_WorldCupMiddle_cpp
#define SportsWorldCup_WorldCupMiddle_cpp

#include "cocos2d.h"
#include "JsonBox.h"
#include "HWFromTo.h"

USING_NS_CC;

#define SCENE_FUNC(_A) static CCScene* scene() { CCScene* scene = CCScene::node(); _A * layer = _A::node(); scene->addChild(layer); return scene; }
class WorldCupMiddle : public CCLayer, public CCTextFieldDelegate
{
private:
	int _accScore;
	int _gainScore;
	CCLabelBMFont* scoreFnt, *totalFnt, *weeklyFnt, *maxFnt;
	CCMenuItem* _next, *_home;
private:
	void updateWeeklyAndMaxScore();
	void NEXT(CCObject*);
	void HOME(CCObject*);
	CCSprite* rankTable;
	CCSprite* myScoreTable;
	void GAMEOVERRETRY();
	int gainScore;
	void finishGetScores(JsonBox::Object js);
	CCSprite *titleBack, *title;
	CCSprite* me;
	CCLabelBMFont* meFnt;
	FromToWithDuration rankX;
	FromToWithDuration rankPercent;
	struct __POSITION{
		CCPoint rank[10];
		CCPoint flag[10];
		CCPoint user[10];
		CCPoint score[10];
		CCPoint time[10];
		CCPoint beginRankPosition;
		CCPoint endRankPosition;
		CCPoint rankTable;
		CCPoint myScoreTable;
		CCPoint home;
		CCPoint next;
	}position;
	virtual void update(ccTime dt);
	void startSchedule();
public:
	WorldCupMiddle();
	virtual ~WorldCupMiddle();
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	SCENE_FUNC(WorldCupMiddle);
	LAYER_NODE_FUNC(WorldCupMiddle);
};

#endif
