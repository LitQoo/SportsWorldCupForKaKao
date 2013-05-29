//
//  RestartScene.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 19..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//

#ifndef SportsWorldCup_RestartScene_cpp
#define SportsWorldCup_RestartScene_cpp

#include "cocos2d.h"
#include "GameShop.h"

USING_NS_CC;

#define SCENE_FUNC(_A) static CCScene* scene() { CCScene* scene = CCScene::node(); _A * layer = _A::node(); scene->addChild(layer); return scene; }
class RestartScene : public CCLayer, public CCTextFieldDelegate
{
private:
	void finishStartScores(JsonBox::Object js);
	int itemPrice;
public:
	typedef pair<GameShop*, string> GameType;
	GameType shop_gamecode;
//	string gameCode;
	RestartScene();
	virtual ~RestartScene();
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void registerWithTouchDispatcher();
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	static CCScene* scene(GameType gameCode) {
		CCScene* scene = CCScene::node();
		RestartScene * layer = RestartScene::node(gameCode);
		scene->addChild(layer);
		return scene;
	}
	static RestartScene* node(GameType gameCode)
	{
		RestartScene *pRet = new RestartScene();
		pRet->shop_gamecode = gameCode;
		pRet->shop_gamecode.first->retain();
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
};

#endif
