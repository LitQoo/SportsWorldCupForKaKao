//
//  RestartScene.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 19..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//
#include "RestartScene.h"
#include "JsonBox.h"
#include "GameSystem.h"
#include "KS_Util.h"
#include "BS2Play.h"
#include "HWPlay.h"
#include "ACPlay.h"
#include "AKPlay.h"
#include "HWGameDirector.h"
#include "BS2BasketGame.h"
#include "ACGameScene.h"
#include "AKAngryGameScene.h"
#include "NSDefault.h"


RestartScene::RestartScene()
{
	
}
RestartScene::~RestartScene()
{
	shop_gamecode.first->release();
	//CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
}



void RestartScene::registerWithTouchDispatcher()
{
	//CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, kCCMenuTouchPriority - 1, true);
}
bool RestartScene::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	CCTouch* touch = pTouch;

	CCPoint location(ccp(0, 0));
	location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
	
	return true;
}
void RestartScene::finishStartScores(JsonBox::Object js)
{
	try {
		if(js["state"].getString() != "ok")
		{
			throw string(ML::key("netfail"));
		}
		
		NSDefault::setGold(NSDefault::getGold() - shop_gamecode.first->getRealPrice());
		if(shop_gamecode.first->getRealPrice() != 0)
		{
			JsonBox::Object gParam;
			gParam["category"] = "useitem";
			gParam["log"] = shop_gamecode.first->getSelectedItems();
			graphdog->command("writelog", &gParam, 0, 0);
			
			KSoundEngine::sharedEngine()->playSound("usemoney.mp3");
		}
		if(NSDefault::getIsTodayFree() == false) // 24H 무료 모드가 아니면 깎음
		{
			if(NSDefault::getHeartNumber() == 5)
				NSDefault::setHeartBaseTime(GameSystem::getCurrentTime_s());
			NSDefault::setHeartNumber(NSDefault::getHeartNumber() - 1);
		}
		
		
		playInfo->currentGame = ((JsonBox::Object)(js["param"].getObject()))["type"].getString();
		
		if(playInfo->currentGame == "BS")
		{
			bs2play->initVars();
			ReplaceScene(BasketGame);
		}
		else if(playInfo->currentGame == "HW")
		{
			hwplay->initVars();
			ReplaceScene(HWGameDirector);
			
		}
		else if(playInfo->currentGame == "AC")
		{
			acplay->initVars();
			ReplaceScene(ACGameScene);
		}
		else if(playInfo->currentGame == "AK")
		{
			akplay->initVars();
			ReplaceScene(AngryGameScene);
		}
	}
	catch (const string& msg) {
		KSModalAlert("", msg.c_str(), 1,
					 "OK", NOPARAM);
	}
}
bool RestartScene::init()
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
	
	GameShop* gs = shop_gamecode.first;
	itemPrice = gs->getRealPrice(); // 선택한 아이템의 가격 (할인 포함)
	string gameCode = shop_gamecode.second;
	JsonBox::Object param;
	param["type"] = gameCode.c_str();

	playInfo->selectedItems.clear();
	playInfo->selectedItems = gs->selectedItems;
	graphdog->command("startscores", &param, this, gd_selector(ThisClassType::finishStartScores));

	//this->setIsTouchEnabled(true);
	
	return true;
}
void RestartScene::onEnterTransitionDidFinish()
{
	CCLayer::onEnterTransitionDidFinish();
}
void RestartScene::onExit()
{
	
	
	CCLayer::onExit();
}
