//
//  GameScreen.h
//  BasketWorldCup2
//
//  Created by ksoo k on 12. 11. 6..
//  Copyright (c) 2012년 LitQoo Inc. All rights reserved.
//

#ifndef BasketWorldCup2_GameScreen_cpp
#define BasketWorldCup2_GameScreen_cpp
#include "BattleMiddle.h"

#include "WCPlay.h"
#include "cocos2d.h"
#include "BS2PinchZoomLayer.h"
#include "chipmunk.h"
#include "drawSpace.h"
#include "ChipmunkDisplayer.h"
#include "BS2BallBase.h"
#include <vector>
#include "BS2Ring.h"
#include "BS2Coin.h"
#include <deque>
#include <queue>
#include "KSBaseScene.h"
#include "BS2Graphics.h"
#include "BS2GameInfomation.h"

#include "GraphDog.h"
#include "KS_Util.h"
#include "LoadingScene.h"
#include "GameSystem.h"
#include "SWIntro.h"
#include "WorldCupMiddle.h"

class BasketGame;


using namespace std;
USING_NS_CC;

struct Turn
{
	static const float DELAY_JUMP_TO_ROUND;
	static const float FAST_SHOT_TIME;
	float ballCatchTime;
	float oneshotTime;
	bool doubleShotWaiting;
	bool isMovingBall;
	static const float VALID_MOVE_TIME;
	CCPoint oneshotVelocity;
	static const float DOUBLE_SHOT_COOLTIME;
	float lastThrowTime;
	bool isUsingItem;
};





#define SCENE_FUNC(_A) static CCScene* scene() { CCScene* scene = CCScene::node(); _A * layer = _A::node(); scene->addChild(layer); return scene; }
class GameScreen :  public CCLayerColor, public CCTextFieldDelegate
{
public:
	enum {POWER_MUL = 4, DYNAMIC_BALL_COLLISION_TYPE = 4, STATIC_BALL_COLLISION_TYPE = 5,
		GOAL_POST_COLLISION = 6, GROUND_COLLISION = 7, COIN_COLLISION = 8, IGNORE = 9, GOAL_RINGPOST_COLLISION=10};
	
public:
	virtual bool init();
	GameScreen();
	virtual ~GameScreen();
	SCENE_FUNC(GameScreen);
	LAYER_NODE_FUNC(GameScreen);
	map<cpShape*, pair<Coin, int>>& getCoins(){return coins;}
	map<cpShape*, BallBase>& getBalls(){return currentBalls;}
	void eatCoin(cpShape* shape);
	void removeCoin(cpShape* shape);
	void removeAllCoin();
	void removeBall(cpShape* shape);
	void removeBall_2(CCNode* node, void* data);
	
	void useItem(enum ItemType it);
	void setBasketGame(BasketGame* b) { pBasketGame = b; }
	void cancelCatch();
	int getThrowCounter(){return info.throwCounter;}
	void startGame();
	float getRemainTime(){return info.remainTime;}
	void setRemainTime(float t) { info.remainTime = t;}
	void setThrowCounter(int n){info.throwCounter = n;}

	void onExitZeroArea();
	void onNoGoal(cpShape*);
	void goZeroArea();
	void tickSound(ccTime dt);
	void finishTimeOver();
	BS2GameInformation::GAMEMODETYPE getGameMode(){return info.gameMode;}
	cpShape* controlableBall;
	Turn turn;
private: // functions
	virtual void registerWithTouchDispatcher();
	void initBack();
	void initRing();
	void createCoin(int xPos);
	CCPoint createBall(CCPoint _ballPos);
	void step(ccTime dt);
	void update(ccTime dt);
	//void moveCoins(ccTime dt);
	//void removeCoins();
	void sceneChanger();
	void judgeCleanShot();
	virtual void draw();
	virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
	virtual void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);
	virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);
	virtual void ccTouchCancelled(CCTouch* pTouch, CCEvent* pEvent);
	void passVirtualSpace(cpSpace* virtualSpace, float s);
	void checkGoal();
	CCPoint getTipInShot(float s, cpShape* shape);
	float decideCoinPosition();
	CCRect validCoinArea();
	int getXmid();
	void throwBall(float Vx, float Vy);
	void doubleShot();
	void showTracer(CCPoint currentPoint, CCPoint beginPoint);
	int comboScore(int combo, float d);
	CCPoint limitV(CCPoint v);
	void timeProcess(ccTime dt);
	void retainCoinAnimation();
	void movingBall(ccTime dt);
	void finishDoubleShot();
	void drawShadow();
	double GaussRandom(int start, int end, int detail);
	CCPoint selectBallPosition();
	void movingPostProc();
	cpShape* createCoin(int xPos, int yPos, int group);
	
	void onGoal(BallBase&);
	void onZeroArea(CCNode* node);
	
	void onFinishZeroArea();
	void deleteSprite(CCNode* _n)
	{
		_n->getParent()->removeChild(_n, true);
	}

	void finishGetWeek(JsonBox::Object js)
	{
		try
		{
			if(js["state"].getString() != "ok")
				throw ML::key("netfail");
			wcplay->scoreEachGame.push_back(make_pair(string("BS"), wcplay->gameScore.getVar()));
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
private: // vars
	enum {BACKGROUND_NUMBER = 10};
	enum {DOWNSCROLL = 0x123123};
	//BallBase
	BasketGame* pBasketGame;
	//string data;
	typedef pair<float, CCPoint> TouchType;
	deque<TouchType> touchPoints;
	map<cpShape*, pair<Coin, int>> coins;
	ChipmunkDisplayer* cDisplayer;
	cpSpace *space;
	ChipSpritesFactory chipSpritesFactory;
	Ring r1;
	map<cpShape*, BallBase> currentBalls;
	struct ClickPair
	{
		CCPoint beginPoint;
		CCPoint currentPoint;
	};
	float gameTimer;
	// id 별로 ClickPair 를 보관하는데, ended 가 호출되면 삭제함.
	// ClickPair 로 최초 클릭점과 moved 될 때의 점을 가지고 있음.
	map<int, ClickPair> clicks;
	BS2GameInformation info;
	Graphics graphics;
	
};

#endif
