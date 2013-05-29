#pragma once
#include "BattleMiddle.h"
#include "HWGameScene.h"
#include "HWSharingClass.h"
#include "NSDefault.h"
#include "GraphDog.h"
#include "KS_Util.h"
#include "LoadingScene.h"
#include "GameSystem.h"
#include "SWIntro.h"
#include "WorldCupMiddle.h"
#include "WCPlay.h"
//#include "RoundScene.h"
class FirstGameScene : public GameScene
{
public:
	static int NORMALBACK_SIZE;
	static int NORMALBACK_HEIGHT_SIZE;
	static int FARBACK_SIZE;
	static int FARBACK_HEIGHT_SIZE;
	
private:
	CCSprite* farBack1;CCSprite* farBack2;
	float farBackPos;
	CCSprite* normalBack1;CCSprite* normalBack2;
	float normalBackPos;
	CCSprite* ad_back1; CCSprite* ad_back2;
	float adBackPos;
	float closeBackPos;
	
	CCMutableArray<CCSprite*>* closeBacks;
	float lastBackPos;
	int screenCount;
	bool _threerepeat;
	int randCount;
	CCSpriteBatchNode* trackBatch;
public:

	virtual ~FirstGameScene()
	{
		closeBacks->release();
	}
	static CCScene* scene()
	{
		CCScene * scene = CCScene::node();
		FirstGameScene* layer = FirstGameScene::node();
		scene->addChild(layer);
		return scene;
	}

	// implement the "static node()" method manually
	LAYER_NODE_FUNC(FirstGameScene);
private:
	
	virtual void onEnterTransitionDidFinish();
	virtual void setBackground();
	virtual void initHurdleMapAndPlayer();
	virtual bool init();
	virtual void randomSound(ccTime dt);
	virtual void cachingPList();
	virtual void unjiCheck();
	virtual void timeCheck(ccTime dt);
	virtual void moveBackground(ccTime dt);
	virtual void moveMap(ccTime dt);
	virtual void gameTest(ccTime dt);
	virtual void draw();
	void finishTimeOver();
	void finishGetWeek(JsonBox::Object js)
	{
		try
		{
			if(js["state"].getString() != "ok")
				throw string(ML::key("netfail"));
			wcplay->scoreEachGame.push_back(make_pair(string("HW"), wcplay->gameScore.getVar()));
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
	
};