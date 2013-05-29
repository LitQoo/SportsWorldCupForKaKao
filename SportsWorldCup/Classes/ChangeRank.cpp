//
//  ChangeRank.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 5. 24..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//
#include "ChangeRank.h"
#include "KS_Util.h"
#include "Bridge.h"


ChangeRank::ChangeRank()
{
	
}
ChangeRank::~ChangeRank()
{
	CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
}
void ChangeRank::registerWithTouchDispatcher()
{
	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, INT_MIN, false);
}
bool ChangeRank::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	CCTouch* touch = pTouch;

	CCPoint location(ccp(0, 0));
	location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
	
	removeFromParentAndCleanup(true);
	return true;
}
bool ChangeRank::init()
{
	CCLayer::init();
		
	this->setIsTouchEnabled(true);
	CCSprite* back = CCSprite::spriteWithFile("rank_chage.png");
	back->setPosition(ccp(240, 160));
	addChild(back);
	
	CCSprite* kim4 = CCSprite::spriteWithFile("kim4.png");
	kim4->setPosition(ccp(176, 158)); 
	addChild(kim4);
	
	CCLabelTTF* score1 = CCLabelTTF::labelWithString("123456789", defaultFont, 14.f);
	kim4->addChild(score1);
	score1->setAnchorPoint(ccp(0.5, 0.f));
	score1->setPosition(ccp(kim4->getContentSize().width / 2.f, kim4->getContentSize().height));
	upArrow = CCSprite::spriteWithFile("arrow_up.png");
	upArrow->setAnchorPoint(ccp(0.5f, 1.f));
	upArrow->setPosition(ccp(kim4->getContentSize().width / 2.f, 0));
	kim4->addChild(upArrow);
	
	CCSprite* pink5 = CCSprite::spriteWithFile("pink5.png");
	pink5->setPosition(ccp(303, 158));
	addChild(pink5);
	CCLabelTTF* score2 = CCLabelTTF::labelWithString("123456789", defaultFont, 14.f);
	pink5->addChild(score2);
	score2->setAnchorPoint(ccp(0.5, 0.f));
	score2->setPosition(ccp(pink5->getContentSize().width / 2.f, pink5->getContentSize().height));
	downArrow = CCSprite::spriteWithFile("arrow_down.png");
	downArrow->setAnchorPoint(ccp(0.5f, 0.f));
	downArrow->setPosition(ccp(pink5->getContentSize().width / 2.f, pink5->getContentSize().height));
	pink5->addChild(downArrow);
	
	upArrow->setIsVisible(false);
	downArrow->setIsVisible(false);
	
	kim4->runAction(CCMoveBy::actionWithDuration(1.f, ccp(0, 30)));
	pink5->runAction(CCMoveBy::actionWithDuration(1.f, ccp(0, -30)));
	runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.3f), CCCallFunc::actionWithTarget(this, callfunc_selector(ThisClassType::moving))));
	
	return true;
}

void ChangeRank::moving()
{
	upArrow->setIsVisible(true);
	downArrow->setIsVisible(true);
}
void ChangeRank::onEnterTransitionDidFinish()
{
	CCLayer::onEnterTransitionDidFinish();
}
void ChangeRank::onExit()
{
	
	
	CCLayer::onExit();
}
