//
//  BasketGame.h
//  BasketWorldCup2
//
//  Created by ksoo k on 12. 10. 31..
//  Copyright (c) 2012년 LitQoo Inc. All rights reserved.
//

#ifndef BasketWorldCup2_BasketGame_cpp
#define BasketWorldCup2_BasketGame_cpp

#include "cocos2d.h"
#include "chipmunk.h"
#include "drawSpace.h"
#include "ChipmunkDisplayer.h"
#include <map>
#include <vector>

//#include "PinchZoomLayer.h"
#include "BS2GameScreen.h"
#include "DisableTouch.h"
#include "KSBaseScene.h"
#include "CommonGame.h"

using namespace std;

USING_NS_CC;

struct zorder
{
	enum{EAT_COIN_EFF=9, BALL_HIDE_EFF=10, POST=1, SHADOW=9, BALL_SHADOW=5, BALL=8, TIME_ICE=5,
		SHOW_BALL_EFF=15, BACK=-1, TIP_IN_EFF=7, ZERO_BALL_EFF=7, MINI_RANK=1, ITEM_TRACER=11, TRACER=10,
		COMBO=3, COMBO_ZERO=3, COMBO_NUMBER=3, COMBO_NUMBER_ZERO=3, CLEAN_SHOT=8, COIN=8, FRONT_POST=10, LUCKY=8,
		POST_MOVE_BAR=2, SCORE=15, COMBO_EFF=2, TIPBOX = 15};
};

#define SCENE_FUNC(_A) static CCScene* scene() { CCScene* scene = CCScene::node(); _A * layer = _A::node(); scene->addChild(layer); return scene; }

class BasketGame : public CCLayerColor, public CCTextFieldDelegate, public CommonGame
{
public:
	BasketGame();
	virtual ~BasketGame();
//	virtual void ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent);
	virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);	
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	SCENE_FUNC(BasketGame);
	LAYER_NODE_FUNC(BasketGame);

	void pauseStopWatch2();
	void resumeStopWatch2();
	void setScore(int n);
//	void setTime(float t);
	void eatCoin(int n);
	float getFever();
	void incFever(float n);
	void setRemainTime(float t);
	void hideInterface();
	void showInterface();
	void pointMaxSprite();
	void setTimeWarning();
	void setTimeNoWarning();
	void setBonusTime();
	CCSprite* getFullTipSprite(){return fullTipSprite;}

	void timeOver();

	enum {FULLTIP_WIDTH = 110, FULLTIP_HEIGHT = 35};
	enum {BACKGROUND_NUMBER = 5};
	void uiUpdate(int score);
	enum {COMBO_COLS = 3, COMBO_FRAME=6, COMBO_WIDTH = 120, COMBO_HEIGHT = 120};
	void showCombo();
	void updateScoreFnt(float dt);
	void showTipInSprite();
	void hideTipInSprite();
private:
	void deleteSprite(CCNode* _n)
	{
		_n->getParent()->removeChild(_n, true);
	}
//	PauseLayer* pauseLayer;
	CCPoint oldPosition1, oldPosition2;
	CCSprite* fullTipSprite;
	CCMenuItem* pause;
	GameScreen* gameScreen;
	//CCSprite* interface;
	CCLabelBMFont*scoreFnt, *timeFnt;
	CCProgressTimer* feverGauge;
	vector<ItemType> seq;
	CCSprite* watch1, *watch2, *watch3;
	CCSprite* tipInableSprite;
	enum {TIPIN_X = 432, TIPIN_Y = 219};
	CCSprite* moveSprite, *doubleSprite;
	int markCount;
private:
	void applyTime();
	void applyPlusBall();
	void applyDouble();
	void applyMove();
	void applyGuide();
//	void
	virtual void registerWithTouchDispatcher();
	void addNewSpriteX(float x, float y);
	void timepp();
	void finishGo(CCNode* n);
	void finishReady(CCNode* n);
	void PAUSE(CCObject*);
public:
};

#endif
