//
//  MyScore.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 8..
//
//

#include "MyScore.h"

MyScore::MyScore()
{
	
}
MyScore::~MyScore()
{
	
}


bool MyScore::init()
{
	CCLayer::init();
	
	this->setIsTouchEnabled(true);
	
	thiz = CCSprite::spriteWithFile("gameover_scoreback.png");
	addChild(thiz);
	
	return true;
}
void MyScore::onEnterTransitionDidFinish()
{
	CCLayer::onEnterTransitionDidFinish();
}
void MyScore::onExit()
{
	
	
	CCLayer::onExit();
}
