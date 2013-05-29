//
//  GameScene.h
//  ArcherWorldCupX
//
//  Created by apple on 12. 7. 6..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#ifndef SKGameScene_GameScene_h
#define SKGameScene_GameScene_h

#include "HWFromTo.h"
#include "SKGameInfo.h"
#include "SKObstacleLayer.h"
#include "cocos2d.h"
#include "CommonGame.h"
#include "KSModalAlert.h"
#include "JNIKelper.h"
#include <string.h>
#include "GraphDog.h"
#include "KS_Util.h"
#include "LoadingScene.h"
#include "GameSystem.h"
#include "SWIntro.h"
#include "SKGraphics.h"

using namespace std;
using namespace cocos2d;

#define SCENE_FUNC(_A) static CCScene* scene() { CCScene* scene = CCScene::node(); _A * layer = _A::node(); scene->addChild(layer); return scene; }
class SKGameScene : public cocos2d::CCLayer, public CommonGame
{
public:
	SKGameScene()
	{
		
		currentTouchCount = 0;
		playInfo->gamePtr = this;
		gameTimer = 0;
		lastCreationDistance = 0.f;
		nextCreationDistance = MIN_CREATION_DISTANCE + (MAX_CREATION_DISTANCE - MIN_CREATION_DISTANCE) * KS_Util::rnd();
		
		lastGroundCreationTime = 0.f;
		nextGroundCreationTime = GND_CREATION_TIME;
		
		lastDecoCreationTime = 0.f;
		nextDecoCreationTime = (MIN_CREATION_DISTANCE + (MAX_CREATION_DISTANCE - MIN_CREATION_DISTANCE) * KS_Util::rnd() ) / 2.f;
		
		lastFlagCreationDistance = 0.f;
		nextFlagCreationDistance = (FLAG_MIN_CREATION_DISTANCE + (FLAG_MAX_CREATION_DISTANCE - FLAG_MIN_CREATION_DISTANCE) * KS_Util::rnd() );
		skierSpeed = DEFAULT_SPEED;
		
		
//		aheadRemainTime = 0.f;
	}
	~SKGameScene();
	virtual bool init();  
	LAYER_NODE_FUNC(SKGameScene);
    SCENE_FUNC(SKGameScene);
	static float PLAYERWIDTH;
private:
	void PAUSE(CCObject*);
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void registerWithTouchDispatcher();
    
 
	void finishTimeOver();
	void RETRY();
	void finishGetWeek(JsonBox::Object js);
	void onExitZero();
	void onEnterZero();
	
	void deleteSprite(CCNode* _obj)
	{
		_obj->removeFromParentAndCleanup(true);
		
	}
	void finishGo(CCNode* n);
	void finishReady(CCNode* n);
	void eatCoin(int n);
	void update(ccTime dt);
	void timeProcess(float dt);
	void createObstacleProcess();
	void createGroundProcess();
	void createDecorationProcess();
	void createFlagProcess();
	void moveSkier(float dt);
	void crashProcess();
	void judgeChaProcess();
	void onCha();
	void onCrash();
	int scoreFromWhat();
	void updateScoreFnt(float dt);
	CCFiniteTimeAction* shakeAction(CCPoint originalPosition);

private:
	enum {COMBO_COLS = 3, COMBO_FRAME=6, COMBO_WIDTH = 120, COMBO_HEIGHT = 120};
	SKGraphics graphics;
	SKGameInfo info;
	SKObstacleLayer* obstacleLayer;
	SKCharacter* skChar;
	static float MIN_CREATION_DISTANCE; // 장애물 생성 최소간격
	static float MAX_CREATION_DISTANCE; // 장애물 생성 최대간격
	static float CRASH_ACTION_TIME;
	float lastCrashTime;
	
	float gameTimer;
	float nextCreationDistance;
	float lastCreationDistance;
	
	
	static float GND_CREATION_TIME;
	static float MAX_SPEED;
	static float MIN_SPEED;
	static float DEFAULT_SPEED;
	static float COMBO_RESET_TIME;
	
//	static float FLAG_CREATION_DISTANCE;
	static float FLAG_MIN_CREATION_DISTANCE;
	static float FLAG_MAX_CREATION_DISTANCE;
	
	float lastGroundCreationTime;
	float nextGroundCreationTime;
	float skierSpeed;
	
	float nextDecoCreationTime;
	float lastDecoCreationTime;
	
	float nextFlagCreationDistance;
	float lastFlagCreationDistance;
	
	int currentTouchCount;
	
//	float aheadRemainTime; // 앞으로 쭉 나갔다가 다시 들어오기까지의 시간
	static float AHEAD_INIT_TIME;
	
};


#endif
