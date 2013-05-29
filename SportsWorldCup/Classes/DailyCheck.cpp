//
//  ChangeRank.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 5. 24..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//
#include "DailyCheck.h"
#include "KS_Util.h"
#include "Bridge.h"


DailyCheck::DailyCheck()
{
	
}
DailyCheck::~DailyCheck()
{
	CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
}
void DailyCheck::registerWithTouchDispatcher()
{
	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, INT_MIN, true);
}
bool DailyCheck::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	CCTouch* touch = pTouch;
	
	CCPoint location(ccp(0, 0));
	location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
	
	removeFromParentAndCleanup(true);
	return true;
}
bool DailyCheck::init()
{
	CCLayer::init();
	
	this->setIsTouchEnabled(true);
	CCSprite* back = CCSprite::spriteWithFile("complete.png");
	back->setPosition(ccp(240, 160));
	addChild(back);
	
	return true;
}


void DailyCheck::onEnterTransitionDidFinish()
{
	CCLayer::onEnterTransitionDidFinish();
}
void DailyCheck::onExit()
{
	
	
	CCLayer::onExit();
}
