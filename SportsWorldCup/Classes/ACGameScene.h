//
//  GameScene.h
//  ArcherWorldCupX
//
//  Created by apple on 12. 7. 6..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#ifndef ArcherWorldCupX_GameScene_h
#define ArcherWorldCupX_GameScene_h

#include "cocos2d.h"
#include "CommonGame.h"
#include "ACGameInfo.h"
#include "ACGageLayer.h"
#include "ACShotResult.h"
#include "ACroundView.h"
#include "ACoverView.h"
#include "ACfullADScene.h"
#include "KSModalAlert.h"
#include "JNIKelper.h"
#include <string.h>
#include "ACGraphics.h"
#include "ACMiniMap.h"
#include "GraphDog.h"
#include "KS_Util.h"
#include "LoadingScene.h"
#include "GameSystem.h"
#include "SWIntro.h"

using namespace std;
using namespace cocos2d;

class ShotResult;


enum GAMESTEP {
	GSPullingCheck = 1,
	GSHandlingBow = 2,
	GSShootArrow = 3,
	GSMoveArrow = 4,
	GSStopArrow = 5,
	GSShowResult = 6
};
typedef const float JUDGE[10];
#define SCENE_FUNC(_A) static CCScene* scene() { CCScene* scene = CCScene::node(); _A * layer = _A::node(); scene->addChild(layer); return scene; }
class ACGameScene : public cocos2d::CCLayer, public CommonGame
{
public:
	ACGameScene()
	{
		timeCnt = 0;
		pullingStart = false;
		isGameOvering = false;
		//gameMode = ACGameScene::NORMAL;
		
		playInfo->gamePtr = this;
		
		
	}
	~ACGameScene();
	virtual bool init();  
	LAYER_NODE_FUNC(ACGameScene);
    SCENE_FUNC(ACGameScene);
private:
	enum TARGET{_100, _80, _60, _40} target;
	enum {COMBO_COLS = 3, COMBO_FRAME=6, COMBO_WIDTH = 120, COMBO_HEIGHT = 120};
	static JUDGE HITS;
	static JUDGE HITS100;
	static JUDGE HITS80;
	static JUDGE HITS60;
	//const static float HITS40[10];
	static JUDGE HITS40;
	enum {COMBO_GT_PT = 8};
	
private:
	//CCSprite *target;
	CCSprite* target100, *target80, *target60, *target40;
	ACMiniMap* miniMap;
	
	CCLayer *bowSet;
		CCSprite *bowH;
		CCSprite *arrow;
	CCSprite *controller;
	CCSprite *pullImg;
	
	CCSprite *arrowPoint;
	CCLabelBMFont* scoreFnt;
	CCLabelBMFont* timeFnt;
	CCProgressTimer* feverGauge;
	CCPoint userCont;
	int timeCnt;
	
	GAMESTEP gameStep;
	bool isGameOvering; // 타임오버 후 한번만 실행되게 하게끔 하는 변수

	double oax, oay, oaz;
	double currentAcX, currentAcY, currentAcZ;
	ACGraphics graphics;
	ACGameInfo info;
	bool pullingStart;
private:
	void PAUSE(CCObject*);
	void gameTimer(float dt);
	void timeCheck(float dt);
	void controlProc(float dt);
	void pullBow();
	void checkPulling(CCPoint pTouch);
	void clacHandling(CCPoint pTouch);
	void applyHandling();
	void shootArrow();
	virtual void showResult();
	virtual void hidePoint();
	virtual void nextArrow();

    virtual void touchedExit();
	int calcPoint(CCPoint _target,CCPoint _arrow,float offset);
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void registerWithTouchDispatcher();
    virtual void didAccelerate(CCAcceleration* pAccelerationValue);
 
	void finishTimeOver();
	void RETRY();
	void finishGetWeek(JsonBox::Object js);
	void onExitZeroArea(bool isCatchBow);
	void onEnterZero();
	void showTargetNumber(int s);
	void setTargetPosition(CCPoint p);
	CCPoint getTargetPosition();
	string stringWithFormat(const char* _Format, ...);
	int pointFromTargetArrow(CCPoint target, CCPoint arrow);
	int gaugeFromPoint(int point);
	const JUDGE& getHITS()
	{
		switch(target)
		{
			case _100:
				return ACGameScene::HITS100;
				break;
			case _80:
				return ACGameScene::HITS80;
				break;
			case _60:
				return ACGameScene::HITS60;
				break;
			case _40:
				return ACGameScene::HITS40;
				break;
//			case default:
//				return ACGameScene::HITS100;
//				break;
		}
	}
	void setHeadSprite();
	int comboScore(int point);
	void updateScoreFnt(float dt);
	void deleteSprite(CCNode* _obj)
	{
		_obj->removeFromParentAndCleanup(true);
		
	}
	void finishGo(CCNode* n);
	void finishReady(CCNode* n);
	void eatCoin(int n);
private:
	
};


#endif
