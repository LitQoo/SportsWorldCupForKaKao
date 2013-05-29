//
//  PauseLayer.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 19..
//
//

#ifndef __SportsWorldCup__PauseLayer__
#define __SportsWorldCup__PauseLayer__
#include "cocos2d.h"

#include "ClayShop.h"
#include "NSDefault.h"
#include "GraphDog.h"
#include "JsonBox.h"
#include "BS2Play.h"
#include "HWPlay.h"
#include "ACPlay.h"
#include "AKPlay.h"
#include "HWGameDirector.h"
#include "BS2BasketGame.h"
#include "ACGameScene.h"
#include "AKAngryGameScene.h"
#include "LoadingScene.h"
#include "RestartScene.h"
#include "GoToHome.h"
#include "CoverLayer.h"

#define SCENE_FUNC(_A) static CCScene* scene() { CCScene* scene = CCScene::node(); _A * layer = _A::node(); scene->addChild(layer); return scene; }


class PauseLayer : public CoverLayer
{
protected:
	CCMenuItem* con, *restart, *home;
	void startSpecificGame(GameShop* gs, string gameCode)
	{
//		ClayShop* gs = ClayShop::node(0);
		if(NSDefault::getGold() >= gs->getRealPrice())
		{
			ReplaceSceneParam(RestartScene, RestartScene::GameType(gs, gameCode));
//			JsonBox::Object param;
//			param["type"] = gameCode.c_str();
//			
//			playInfo->selectedItems.clear();
//			playInfo->selectedItems = gs->selectedItems;
//			graphdog->command("startScores", &param, this, gd_selector(ThisClassType::finishStartScores));
		}
		else
		{
			KSModalAlert("", ML::key("require_coin").c_str(), 1,
						 "OK", NOPARAM);
		}
	}
	
public:
	//	virtual void ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent);
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
private:
	virtual void restartGame() = 0;
	virtual void goToHome()
	{
		ReplaceScene(GoToHome);
//		graphdog->command("getweek", 0, this, gd_selector(ThisClassType::finishGetWeek));
	}
	void finishGetWeek(JsonBox::Object js)
	{
		playInfo->currentWeek = js["week"].getInt();
		ReplaceSceneParam(SWIntro, INIT_MODE::GAMEOVER);
	}
	void CONTINUE(CCObject*);
	void RESTART(CCObject*);
	void HOME(CCObject*);
	
	CCSprite* thiz;
	void EXIT();
};

class ACPauseLayer : public PauseLayer
{
public:
	LAYER_NODE_FUNC(ACPauseLayer);
private:
	virtual void restartGame()
	{
		startSpecificGame(ArcheryShop::node(0), "AC");
	}
};

class AKPauseLayer : public PauseLayer
{
public:
	LAYER_NODE_FUNC(AKPauseLayer);
private:
	virtual void restartGame()
	{
		startSpecificGame(ClayShop::node(0), "AK");
	}

};

class HWPauseLayer : public PauseLayer
{
public:
	LAYER_NODE_FUNC(HWPauseLayer);
private:
	virtual void restartGame()
	{
		startSpecificGame(HurdleShop::node(0), "HW");
	}

};

class BSPauseLayer : public PauseLayer
{
public:
	LAYER_NODE_FUNC(BSPauseLayer);
private:
	virtual void restartGame()
	{
		startSpecificGame(BasketShop::node(0), "BS");
	}

};

class SKPauseLayer : public PauseLayer
{
public:
	LAYER_NODE_FUNC(SKPauseLayer);
private:
	virtual void restartGame()
	{
		startSpecificGame(SkiShop::node(0), "SK");
	}
};
#endif /* defined(__SportsWorldCup__PauseLayer__) */
