//
//  GameSelect.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 6..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//

#ifndef SportsWorldCup_GameSelect_cpp
#define SportsWorldCup_GameSelect_cpp
#include "UIMediator.h"
#include "cocos2d.h"
#include <vector>
using namespace std;
USING_NS_CC;


class GameSelect : public CCLayer, public UIColleague

{
private:
	UIMediator* mediator;
	CCSprite* thiz;
	vector<CCNode*> gameSprites; // 게임선택창을 위아래로 내릴 때 각 개체를 컨트롤 하기 위해 필요
	int currentOrder;
	CCPoint beginPoint;
	CCPoint endPoint;

	void RECORD_HURDLE(CCObject*);
	void RECORD_ARCHER(CCObject*);
	void RECORD_BASKETBALL(CCObject*);
	void RECORD_CLAY(CCObject*);
	void RECORD_WORLDCUP(CCObject*);
	void RECORD_SKI(CCObject*);
	void BATTLE_SELECT(CCObject*);
//	enum {NONE, UPWARD, DOWNWARD}preserveCmd;
	bool isMoving;
	enum {GAME_COUNT = 7};
	int gameOrder[GAME_COUNT];
	int slidedWithMove;
	CCMenu* _menu;
public:
	static const int HURDLE_INDEX = 0;
	static const int ARCHERY_INDEX = 1;
	static const int CLAY_INDEX = 2;
	static const int BASKETBALL_INDEX = 3;
	static const int SKI_INDEX = 4;
	static const int COMINGSOON = 5;
	static const int BATTLE_INDEX = 6;
	GameSelect();
	virtual ~GameSelect();
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	void hideGames();
	void showGames();
	int getCurrentIndex()
	{
		CCAssert(0 <= currentOrder && currentOrder < sizeof(gameOrder) / sizeof(gameOrder[0]), "index error");
		return gameOrder[currentOrder];
	}
	static GameSelect* node(UIMediator *m)
	{
		GameSelect *pRet = new GameSelect();
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
	virtual void registerWithTouchDispatcher();
	void setMediator(UIMediator* m)
	{
		mediator = m;
	}
	void finishMoving();
	void syncInfo();
	void selectedBattle(CCObject*);
};

#endif
