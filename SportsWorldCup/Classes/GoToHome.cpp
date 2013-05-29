//
//  GoToHome.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 19..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//
#include "GoToHome.h"
#include "WCPlay.h"
#include "GraphDog.h"
#include "SWIntro.h"
#include "GameSystem.h"
#include "LoadingScene.h"
#include "KS_Util.h"

GoToHome::GoToHome()
{
	
}
GoToHome::~GoToHome()
{
//	CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
}



void GoToHome::registerWithTouchDispatcher()
{
//	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, kCCMenuTouchPriority - 1, true);
}
bool GoToHome::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	CCTouch* touch = pTouch;

	CCPoint location(ccp(0, 0));
	location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
	
	return true;
}
//
//void GoToHome::ccTouchesMoved(CCSet* pTouches, CCEvent* pEvent)
//{
//	
//}
//void GoToHome::ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent)
//{
//	
//}
bool GoToHome::init()
{
	CCLayer::init();
	CCSprite* mainBack = CCSprite::spriteWithFile("main_back.png");
	mainBack->setPosition(ccp(240, 160));
	addChild(mainBack);
	CCLayer* myLayer = CCLayer::node();
	addChild(myLayer);
	CCSprite* titleBack = CCSprite::spriteWithFile("main_titleback.png");
	titleBack->setPosition(ccp(240, 160));
	titleBack->runAction(CCRepeatForever::actionWithAction(CCRotateBy::actionWithDuration(2.f*1.f/0.33f, 360.f)));
	myLayer->addChild(titleBack, 0);
	CCSprite* title = CCSprite::spriteWithFile("main_title.png");
	title->setPosition(ccp(240, 160));
	myLayer->addChild(title, 0);
	
	//this->setIsTouchEnabled(true);
	graphdog->command("getweek", 0, this, gd_selector(ThisClassType::finishGetWeek));
	return true;
}
void GoToHome::finishGetWeek(JsonBox::Object js)
{
	try
	{
		if(js["state"].getString() != "ok")
			throw string(ML::key("netfail"));
		playInfo->currentWeek = js["week"].getInt();
		
		wcplay->scoreEachGame.push_back(make_pair(playInfo->prevGame, wcplay->gameScore.getVar()));
		if(playInfo->currentGame == "WORLDCUP")
		{
			ReplaceSceneParam(SWIntro, INIT_MODE::WORLDCUP_GAMEOVER);
			
		}
		else if(playInfo->currentGame == "BATTLE")
		{
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

void GoToHome::RETRY()
{
	graphdog->command("getweek", 0, this, gd_selector(ThisClassType::finishGetWeek));
}
void GoToHome::onEnterTransitionDidFinish()
{
	CCLayer::onEnterTransitionDidFinish();
}
void GoToHome::onExit()
{
	
	
	CCLayer::onExit();
}
