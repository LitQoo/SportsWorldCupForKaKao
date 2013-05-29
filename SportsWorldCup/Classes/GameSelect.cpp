//
//  GameSelect.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 6..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//
#include "GameSelect.h"
#include <map>
#include <string>
#include <algorithm>
#include "NSDefault.h"
#include "ACSaveData.h"
#include "AKSaveData.h"
#include "BS2SaveData.h"
#include "HWSaveData.h"
#include "Bridge.h"
#include "WCSaveData.h"
#include "KSoundEngine.h"
#include "SKSaveData.h"

using namespace std;
GameSelect::GameSelect()
{
	currentOrder = 0;
	isMoving = false;
}
GameSelect::~GameSelect()
{
	CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
	CCNotificationCenter::sharedNotifCenter()->removeObserver(this, "selectedBattle");
}
void GameSelect::registerWithTouchDispatcher()
{
	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, false);
}
bool GameSelect::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	slidedWithMove = false;
	CCTouch* touch = pTouch;

	CCPoint location(ccp(0, 0));
	location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
	location = thiz->convertToNodeSpace(location);
	CCRect ggg = CCRectMake(0, 0, thiz->getContentSize().width, thiz->getContentSize().height);
	CCRectApplyAffineTransform(ggg, thiz->nodeToWorldTransform());
	if(CCRect::CCRectContainsPoint(ggg, location))
	{
		beginPoint = location;
	}
	else
	{
		beginPoint = ccp(-100, -100);
	}
	return true;
}
void GameSelect::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	CCTouch* touch = pTouch;
	
	CCPoint location(ccp(0, 0));
	location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
	location = thiz->convertToNodeSpace(location);
	
	CCRect ggg = CCRectMake(0, 0, thiz->getContentSize().width, thiz->getContentSize().height);
	CCRectApplyAffineTransform(ggg, thiz->nodeToWorldTransform());
	endPoint = location;
	if(isMoving)
		return;
	if(beginPoint.y >= 0)
	{
		if(endPoint.y - beginPoint.y >= 70)
		{ // downward
			if(GAME_COUNT - 1 <= currentOrder)
				return;
			isMoving = true;
			currentOrder++;
			for(auto iter = gameSprites.begin(); iter != gameSprites.end(); ++iter)
			{
				(*iter)->runAction(CCSequence::actionOneTwo(CCMoveBy::actionWithDuration(0.15f, ccp(0, 144)), CCCallFunc::actionWithTarget(this, callfunc_selector(GameSelect::finishMoving))));
			}
			slidedWithMove = true;
			beginPoint.y = endPoint.y;
			KSoundEngine::sharedEngine()->playSound("gameselect.mp3");
		}
		else if(beginPoint.y - endPoint.y >= 70)
		{ // upward
			if(0 >= currentOrder)
				return;
			isMoving = true;
			currentOrder--;
			for(auto iter = gameSprites.begin(); iter != gameSprites.end(); ++iter)
			{
				(*iter)->runAction(CCSequence::actionOneTwo(CCMoveBy::actionWithDuration(0.15f, ccp(0, -144)), CCCallFunc::actionWithTarget(this, callfunc_selector(GameSelect::finishMoving))));
			}
			slidedWithMove = true;
			beginPoint.y = endPoint.y;
			KSoundEngine::sharedEngine()->playSound("gameselect.mp3");
		}
	}
}
void GameSelect::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	if(slidedWithMove)
		return;
	CCTouch* touch = pTouch;
	
	CCPoint location(ccp(0, 0));
	location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
	location = thiz->convertToNodeSpace(location);
	
	CCRect ggg = CCRectMake(0, 0, thiz->getContentSize().width, thiz->getContentSize().height);
	CCRectApplyAffineTransform(ggg, thiz->nodeToWorldTransform());
	endPoint = location;
	if(isMoving)
		return;
	if(beginPoint.y >= 0)
	{
		if(beginPoint.y <= 30)
		{ // downward
			if(GAME_COUNT - 1 <= currentOrder)
				return;
			isMoving = true;
			currentOrder++;
			for(auto iter = gameSprites.begin(); iter != gameSprites.end(); ++iter)
			{
				(*iter)->runAction(CCSequence::actionOneTwo(CCMoveBy::actionWithDuration(0.2f, ccp(0, 144)), CCCallFunc::actionWithTarget(this, callfunc_selector(GameSelect::finishMoving))));
			}
			KSoundEngine::sharedEngine()->playSound("gameselect.mp3");
		}
		else if(beginPoint.y >= thiz->getContentSize().height - 50)
		{ // upward
			if(0 >= currentOrder)
				return;
			isMoving = true;
			currentOrder--;
			for(auto iter = gameSprites.begin(); iter != gameSprites.end(); ++iter)
			{
				(*iter)->runAction(CCSequence::actionOneTwo(CCMoveBy::actionWithDuration(0.2f, ccp(0, -144)), CCCallFunc::actionWithTarget(this, callfunc_selector(GameSelect::finishMoving))));
			}
			KSoundEngine::sharedEngine()->playSound("gameselect.mp3");
		}
	}
}

void GameSelect::hideGames()
{
	for(auto iter = gameSprites.begin(); iter != gameSprites.end(); ++iter)
	{
		(*iter)->runAction(CCFadeOut::actionWithDuration(0.2f));
		KS::KSLog("%", (*iter)->getChildrenCount());
		CCArray* children = (*iter)->getChildren();
		if(children)
		{
			for(int i=0; i<children->count(); i++)
			{
				CCNode* o = (CCNode*)children->objectAtIndex(i);
				o->setIsVisible(false);
				o->runAction(CCFadeOut::actionWithDuration(0.2f));
			}
		}
	}
	KS::setAllVisible(_menu, false);
	
}
void GameSelect::showGames()
{
	if(getCurrentIndex() == GameSelect::COMINGSOON || getCurrentIndex() == GameSelect::BATTLE_INDEX)
	{ // 스타트에 -gold 표시하는 코드.
		mediator->showWorldcupMark(true);
	}
	else
	{ // 스타트에 -gold 표시 해제.
		mediator->showWorldcupMark(false);
	}
	for(auto iter = gameSprites.begin(); iter != gameSprites.end(); ++iter)
	{
		(*iter)->runAction(CCFadeIn::actionWithDuration(0.1f));
		CCLog("%", (*iter)->getChildrenCount());
		CCArray* children = (*iter)->getChildren();
		if(children)
		{
			for(int i=0; i<children->count(); i++)
			{
				CCNode* o = (CCNode*)children->objectAtIndex(i);
				o->setIsVisible(true);
				o->runAction(CCFadeIn::actionWithDuration(0.2f));
			}
		}
	}
	KS::setAllVisible(_menu, true);
}

void GameSelect::syncInfo()
{
	
}
void GameSelect::finishMoving()
{
	isMoving = false;
	
	if(getCurrentIndex() == GameSelect::COMINGSOON || getCurrentIndex() == GameSelect::BATTLE_INDEX)
	{ // 스타트에 -gold 표시하는 코드.
		mediator->showWorldcupMark(true);
	}
	else
	{ // 스타트에 -gold 표시 해제.
		mediator->showWorldcupMark(false);
	}
}

void GameSelect::RECORD_HURDLE(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	KS::KSLog("%", __FUNCTION__);
	mediator->showLegendaryRecord("HW");
}
void GameSelect::RECORD_ARCHER(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	KS::KSLog("%", __FUNCTION__);
	mediator->showLegendaryRecord("AC");
}
void GameSelect::RECORD_BASKETBALL(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	KS::KSLog("%", __FUNCTION__);
	mediator->showLegendaryRecord("BS");
}
void GameSelect::RECORD_CLAY(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	KS::KSLog("%", __FUNCTION__);
	mediator->showLegendaryRecord("AK");
}

void GameSelect::RECORD_WORLDCUP(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	KS::KSLog("%", __FUNCTION__);
	mediator->showLegendaryRecord("WORLDCUP");
}
void GameSelect::RECORD_SKI(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	KS::KSLog("%", __FUNCTION__);
	mediator->showLegendaryRecord("SK");
}

void GameSelect::BATTLE_SELECT(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	CCNotificationCenter::sharedNotifCenter()->postNotification("battleSelectWindow");
}
bool GameSelect::init()
{
	CCLayer::init();
		
	this->setIsTouchEnabled(true);
	thiz = CCSprite::spriteWithFile("gameselect_back.png");
	addChild(thiz);
	
	for(int i=0; i<sizeof(gameOrder) / sizeof(int); i++)
	{
		gameOrder[i] = i;
	}
//	AKSaveData::getMaxScore()
	vector< tuple<string, int, int, SEL_MenuHandler> > spriteFile;

	spriteFile.push_back(make_tuple("start_hurdle.png", HWSaveData::getMaxScore(), HWSaveData::getWeeklyMaxScore().second, menu_selector(ThisClassType::RECORD_HURDLE)));
	spriteFile.push_back(make_tuple("start_archery.png",ACSaveData::getMaxScore(), ACSaveData::getWeeklyMaxScore().second, menu_selector(ThisClassType::RECORD_ARCHER)));
	spriteFile.push_back(make_tuple("start_clay.png", AKSaveData::getMaxScore(), AKSaveData::getWeeklyMaxScore().second, menu_selector(ThisClassType::RECORD_CLAY)));
	spriteFile.push_back(make_tuple("start_basketball.png", BS2SaveData::getMaxScore(), BS2SaveData::getWeeklyMaxScore().second, menu_selector(ThisClassType::RECORD_BASKETBALL)));
	spriteFile.push_back(make_tuple("start_ski.png", SKSaveData::getMaxScore(), SKSaveData::getWeeklyMaxScore().second, menu_selector(ThisClassType::RECORD_SKI)));
	spriteFile.push_back(make_tuple("start_comingsoon.png", WCSaveData::getMaxScore(), WCSaveData::getWeeklyMaxScore().second, menu_selector(ThisClassType::RECORD_WORLDCUP)));
	spriteFile.push_back(make_tuple("match.png", WCSaveData::getMaxScore(), WCSaveData::getWeeklyMaxScore().second, menu_selector(ThisClassType::RECORD_WORLDCUP)));
	
	for(int i=0; i<rand()%GAME_COUNT; i++)
	{
		int excludeN = 2; // 챌린지, 배틀은 맨 아래에 고정.
		random_shuffle(&gameOrder[0], &gameOrder[sizeof(gameOrder) / sizeof(int) - excludeN], [](int n){
			return rand()%n;
		} );
	}
	
	// max, weekly 달고 레코드 넣는 작업. 배틀은 안붙임.
	int excludeN = 1;
	for(int i=0; i<sizeof(gameOrder) / sizeof(int); i++)
	{
		string a = get<0>( spriteFile[gameOrder[i]] );
		int maxScore = get<1>( spriteFile[gameOrder[i]] );
		int weeklyScore = get<2>( spriteFile[gameOrder[i]] );
		SEL_MenuHandler handler = get<3>( spriteFile[gameOrder[i]] );
		
		CCNode* temp;
//		if(i == sizeof(gameOrder) / sizeof(int) - 1) // 배틀이면
//		{
//			
//			CCMenu* _menu = CCMenu::menuWithItems(0);
//			_menu->setPosition(ccp(0, 0));
//			thiz->addChild(_menu, 2);
//			
//			
//			CCSprite* temp = CCSprite::spriteWithFile(a.c_str());
//			CCSprite* temp2 = CCSprite::spriteWithFile(a.c_str());
//			
//			CCMenuItem* item = CCMenuItemSprite::itemFromNormalSprite(temp, temp2, this, menu_selector(ThisClassType::BATTLE_SELECT));
//			item->setPosition(ccp(134, 144 - 144 * (i)));
//			_menu->addChild(item, 2);
//			gameSprites.push_back(item);
//			continue; // break;
//		}
//		else
		{
			temp = CCSprite::spriteWithFile(a.c_str());
			temp->setPosition(ccp(134, 144 - 144 * (i)));
			thiz->addChild(temp, 2);
			gameSprites.push_back(temp);
		}
		
		if(i == sizeof(gameOrder) / sizeof(int) - 1) // 배틀이면
			continue; //##
		_menu = CCMenu::menuWithItems(0);
		_menu->setPosition(ccp(0, 0));
		temp->addChild(_menu, 3);
		CCMenuItem* record = CCMenuItemImage::itemFromNormalImage("record.png", "record.png", this, handler);
		record->setPosition(ccp(222, 125)); // 222, 125
		_menu->addChild(record, 2);
		if((maxScore < 0 || weeklyScore < 0) == false)
		{
			CCSprite* gameMax = CCSprite::spriteWithFile("gameover_max.png");
			gameMax->setScale(0.5f);
			gameMax->setPosition(ccp(72, 38));
			temp->addChild(gameMax, 2);
			
			CCLabelBMFont* maxFnt = CCLabelBMFont::labelWithString(KS_Util::stringWithFormat("%d", maxScore).c_str(), "gameover_score1.fnt");
			maxFnt->setPosition(ccp(72, 20));
			temp->addChild(maxFnt, 2);
			
			CCSprite* gameWeekly = CCSprite::spriteWithFile("gameover_weekly.png");
			gameWeekly->setScale(0.5f);
			gameWeekly->setPosition(ccp(192, 38));
			temp->addChild(gameWeekly, 2);
			
			CCLabelBMFont* weeklyFnt = CCLabelBMFont::labelWithString(KS_Util::stringWithFormat("%d", weeklyScore).c_str(), "gameover_score1.fnt");
			weeklyFnt->setPosition(ccp(192, 20));
			temp->addChild(weeklyFnt, 2);
		}
		
	}

	CCNotificationCenter::sharedNotifCenter()->addObserver(this, callfuncO_selector(ThisClassType::selectedBattle), "selectedBattle", 0);
	return true;
}
void GameSelect::onEnterTransitionDidFinish()
{
	CCLayer::onEnterTransitionDidFinish();
}
void GameSelect::onExit()
{
	
	
	CCLayer::onExit();
}

void GameSelect::selectedBattle(CCObject*)
{
	
	CCNotificationCenter::sharedNotifCenter()->postNotification("showBattleShop");
}

