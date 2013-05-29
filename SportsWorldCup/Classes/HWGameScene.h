#pragma once
#include "CCScene.h"
#include "HWHurdleMapLayer.h"
#include "HWPlayer.h"
// #include "LQScores.h"

#include "KS_Util.h"
#include "HWGameInformation.h"
#include "KSBaseScene.h"
#include "CommonGame.h"

#define SCENE_FUNC(_A) static CCScene* scene() { CCScene* scene = CCScene::node(); _A * layer = _A::node(); scene->addChild(layer); return scene; }

USING_NS_CC;


class GameScene : public CCLayer, public CommonGame
{
public:
	void deleteSprite(CCNode* _n)
	{
		_n->getParent()->removeChild(_n, true);
	}
	enum {INIT_GAME_MODE = 40};
	GameScene()
	{
		playInfo->gamePtr = this;
		innerInfo.touching = false;
		info.runStep = 1;
		innerInfo.beforeRunStep = 1;
		info.runSpeed2.init(calSpeed(), calSpeed(), 0.f);
		info.gameMode = HWGameInformation::NORMAL;
		innerInfo.timeAfterZero = 0;
		
	}
	virtual ~GameScene();
	virtual bool init();
	
protected:
	CCSet* crashedHurdle;
	CCLayer* interfaceLayer;
	CCLayer* rootLayer;

	
	CCLabelBMFont* scoreFnt;
	CCLabelBMFont* timeFnt;
	CCProgressTimer* feverGauge;
	HurdleMapLayer* hl;
	Player1* player;
	CCSprite* watch1, *watch2, *watch3;
	HWGameInformation info;
	HWInnerVar innerInfo;
	CCLabelBMFont* ateGoldFnt;
	CCSprite* zeroBack;
	CCMenuItem* pause;
	enum {COIN_WIDTH = 31, COIN_HEIGHT = 31};
	enum {COMBO_COLS = 3, COMBO_FRAME=6, COMBO_WIDTH = 120, COMBO_HEIGHT = 120};
protected: // cocos2d
	virtual void registerWithTouchDispatcher();
	virtual void onEnterTransitionDidFinish();
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void onExit();

public:
private:
	void eatCoin(int n, CCPoint from);
	void defaultJump();
	
	void setSpeed(float s);
	int score(int combo, int jump)
	{
		return (100*combo + 10*jump) * (info.gameMode == HWGameInformation::ZERO ? 2 : 1);
	}
	void displayStatus(ccTime dt);
	void crashOnGround(Obstacle* t);
	void crashOnHurdle(Hurdle* h);
	void onUnji();
	virtual void cachingPList() = 0;
	virtual void initHurdleMapAndPlayer() = 0;
	virtual void unjiCheck() = 0;
	virtual void moveBackground(ccTime dt) = 0;
	void onHurdleScore();
	void onGroundScore(bool perfect);
	void onEnterZeroMode();
	void PAUSE(CCObject*);
protected:
	void onExitZeroMode();
	float calSpeed();
	void feelSpeed(int speedCombo);
	void resJumpCheck();
	virtual void gameTest(ccTime dt) = 0;
	void moveMan(ccTime dt);
	void moveMap(ccTime dt);
	void crashCheck();
	void adjustRunSpeed(ccTime dt);
	float givenTime();
	void timeCheck(ccTime dt);
	void translation(float dt);

	void finishAnimation(CCNode* sender);
	void finishGo(CCNode* n);
	void finishReady(CCNode* n);


};



