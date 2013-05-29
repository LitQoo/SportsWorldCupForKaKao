//
//  AngryGameScene.h
//  AngryKeeper
//
//  Created by ksoo k on 12. 9. 10..
//  Copyright (c) 2012?? __MyCompanyName__. All rights reserved.
//

#ifndef AngryKeeper_AngryGameScene_h
#define AngryKeeper_AngryGameScene_h
#include "WCPlay.h"
#include "BattleMiddle.h"
#include "cocos2d.h"
#include "AKGun1.h"
#include "AKBird.h"
#include "AKBomb.h"
#include <queue>
#include "AKGameInfo.h"
#include "AKGraphics.h"
#include "GameSystem.h"
#include "KS_Util.h"
#include "SWIntro.h"
#include "LoadingScene.h"
#include "CommonGame.h"
#include "GraphDog.h"
#include "WorldCupMiddle.h"

using namespace std;
USING_NS_CC;

#define SCENE_FUNC(_A) static CCScene* scene() { CCScene* scene = CCScene::node(); _A * layer = _A::node(); scene->addChild(layer); return scene; }
class AngryGameScene : public CCLayer, public CommonGame
{
public:
	AngryGameScene();
	virtual ~AngryGameScene();
	virtual bool init();  
	SCENE_FUNC(AngryGameScene);
	LAYER_NODE_FUNC(AngryGameScene);
	static float rnd(){return ((float)(rand()) / (float)(RAND_MAX));}
	void onShotBird(bool);
private:
	enum {CROSS_BEAT_STEP = 7};
	enum {LEFT_AIM_X = 240 - 50, LEFT_AIM_Y = 100, RIGHT_AIM_X = 240 + 50, RIGHT_AIM_Y = 100};
	enum {DISH_BROKEN_FRAME = 5, DISH_BROKEN_WIDTH = 95, DISH_BROKEN_HEIGHT = 95};
	enum {FAIL_FRAME = 5, FAIL_WIDTH = 40, FAIL_HEIGHT = 40};
	enum {COMBO_COLS = 3, COMBO_FRAME=6, COMBO_WIDTH = 120, COMBO_HEIGHT = 120};
	static const CCPoint GUNPOSITION;
	static const float DISABLE_TIME;
	static const float COMBO_RESET_TIME;
	static const float INIT_SCALE;
	static const float MAX_SCALE;
	static const float MIN_SCALE;
	virtual void ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent);
	//virtual void ccTouchesMoved(CCSet* pTouches, CCEvent* pEvent);
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	virtual void update(ccTime dt);
	virtual void draw()
	{
		CCLayer::draw();
	}
	void deleteSprite(CCNode* _obj)
	{		
		_obj->removeFromParentAndCleanup(true);
    
	}
	bool onLeftFire();
	bool onRightFire();
	bool killBird(CCSprite* gunMark);
	void onShot(CCSprite* dish);
	void onFailShot(bool right);
	void onEnterZeroMode();
	void onExitZeroMode();
	//void decZeroGauge(bool bird_arrive_goal);
	void shakeScreenOnFire();
	void timeChecker(float);
	void createFlyingProc(float dt);
		void createFlying(bool toR);
	void moveFlying(float dt);
	void finishGo(CCNode* n);
	void finishReady(CCNode* n);
	void traceTarget(float dt);
		void traceLeftTarget(float dt);
		void traceRightTarget(float dt);
	float getVelocity();
	int scoreFromCombo();
	void updateScoreFnt(float dt);
	void finishTimeOver();
	void finishGetWeek(JsonBox::Object js)
	{
		try
		{
			if(js["state"].getString() != "ok")
				throw ML::key("netfail");
			wcplay->scoreEachGame.push_back(make_pair(string("AK"), wcplay->gameScore.getVar()));
			playInfo->currentWeek = js["week"].getInt();
			if(playInfo->currentGame == "WORLDCUP")
			{
				if(playInfo->getWorldcupGame() != "")
					ReplaceScene(WorldCupMiddle);
				else
					ReplaceSceneParam(SWIntro, INIT_MODE::WORLDCUP_GAMEOVER);
			}
			else if(playInfo->currentGame == "BATTLE")
			{
				if(playInfo->getWorldcupGame() != "")
					ReplaceScene(BattleMiddle);
				else
					ReplaceSceneParam(SWIntro, INIT_MODE::BATTLE_GAMEOVER);
			}
			else
			{
				ReplaceSceneParam(SWIntro, INIT_MODE::GAMEOVER);
				
			}

		}
		catch(const string& msg)
		{
			KSModalAlert("", msg.c_str(), 1,
						 ML::key("retrying").c_str(), this, callfunc_selector(ThisClassType::RETRY));
		}
	}
	void RETRY()
	{
		graphdog->command("getweek", 0, this, gd_selector(ThisClassType::finishGetWeek));
	}
	CCFiniteTimeAction* shakeAction(CCPoint originalPosition)
	{
		CCMoveBy* move1 = CCMoveBy::actionWithDuration(0.05f, CCPointMake(-4,3));
		CCMoveBy* move2 = CCMoveBy::actionWithDuration(0.05f, CCPointMake(3,-2));
		CCMoveBy* move3 = CCMoveBy::actionWithDuration(0.05f, CCPointMake(-3,2));
		CCMoveBy* move4 = CCMoveBy::actionWithDuration(0.05f, CCPointMake(3,-4));
		CCMoveBy* move5 = CCMoveBy::actionWithDuration(0.05f, CCPointMake(-4,2));
		
		CCMoveBy* move6 = CCMoveBy::actionWithDuration(0.05f, CCPointMake(3,2));
		CCMoveBy* move7 = CCMoveBy::actionWithDuration(0.05f, CCPointMake(-4,-2));
		CCMoveBy* move8 = CCMoveBy::actionWithDuration(0.05f, CCPointMake(5,2));
		CCMoveBy* move9 = CCMoveBy::actionWithDuration(0.05f, CCPointMake(-2,-4));
		CCMoveBy* move10 = CCMoveBy::actionWithDuration(0.05f, CCPointMake(5,3));
		CCMoveTo* moveBack = CCMoveTo::actionWithDuration(0.05f, originalPosition);
		
		auto shakeWorldSeq = CCSequence::actions(move1,move2,move3,move4,move5,move6,move7,move8,move9,move10,moveBack, 0);
		return shakeWorldSeq;
	}
	CCFiniteTimeAction* shortShakeAction(CCPoint originalPosition)
	{
		CCMoveBy* move1 = CCMoveBy::actionWithDuration(0.05f, CCPointMake(-4,3));
		CCMoveBy* move2 = CCMoveBy::actionWithDuration(0.05f, CCPointMake(3,-2));
		CCMoveBy* move3 = CCMoveBy::actionWithDuration(0.05f, CCPointMake(-3,2));
		CCMoveBy* move4 = CCMoveBy::actionWithDuration(0.05f, CCPointMake(3,-4));
		CCMoveBy* move5 = CCMoveBy::actionWithDuration(0.05f, CCPointMake(-4,2));
		
		CCMoveTo* moveBack = CCMoveTo::actionWithDuration(0.05f, originalPosition);
		
		auto shakeWorldSeq = CCSequence::actions(move1,move2,move3,move4,move5,moveBack, 0);
		return shakeWorldSeq;
	}
	void eatCoin(int n, CCPoint from);
	void PAUSE(CCObject*);
private:
	int newVar;
	AKGameInfo info;
	AKGraphics graphics;
	
	double scorePerCombo(int combo){return combo;}
};


#endif
