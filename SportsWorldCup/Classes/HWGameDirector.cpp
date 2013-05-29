//
//  HWGameDirector.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 2. 1..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//
#include "HWGameDirector.h"
#include "HWFirstGameScene.h"

HWGameDirector::HWGameDirector()
{
	
}
HWGameDirector::~HWGameDirector()
{

}


bool HWGameDirector::init()
{
	CCLayer::init();
		
	this->setIsTouchEnabled(true);

	
	CCLayer* gameLayer = FirstGameScene::node();
	addChild(gameLayer);
	
	CCSprite* left = CCSprite::spriteWithFile("left.png");
	left->setAnchorPoint(ccp(1, 0.5f));
	left->setPosition(ccp(0, 160));
	addChild(left, INT_MAX);
	
	CCSprite* right = CCSprite::spriteWithFile("right.png");
	right->setAnchorPoint(ccp(0.f, 0.5f));
	right->setPosition(ccp(480, 160));
	addChild(right, INT_MAX);
	return true;
}
void HWGameDirector::onEnterTransitionDidFinish()
{
	CCLayer::onEnterTransitionDidFinish();
}
void HWGameDirector::onExit()
{
	
	
	CCLayer::onExit();
}
