//
//  SKCharacter.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 5. 10..
//
//

#include "SKCharacter.h"


float SKCharacter::CHAR_BASEY = 42.f;
float SKCharacter::STUN_INIT_TIME = 1.f;
float SKCharacter::UNBEATABLE_INIT_TIME = 1.f;
float SKCharacter::FRONT_DURATION = 0.8f;
float SKCharacter::BACK_DURATION = 1.2f;
void SKCharacter::createSprite()
{
	this->setAnchorPoint(ccp(0.5, 0.f));
	
	charAnimation = new CCAnimation;//::animation();
	charAnimation->init();
	createAnimation(charAnimation, "sk_ch_center.png", 6, 14, 150, 140);
	addFrames(charAnimation->getFrames());
	this->startAnimation(1, 11, -1, 0, 0, 20, false, false);
//	setFrameRate(0.07f);
	rightAnimation = new CCAnimation;
	rightAnimation->init();
	createAnimation(rightAnimation, "sk_ch_right.png", 2, 2, 150, 140);
	
	leftAnimation = new CCAnimation;
	leftAnimation->init();
	createAnimation(leftAnimation, "sk_ch_left.png", 2, 2, 150, 140);
	
	
	
	leftBreakAnimation = new CCAnimation;
	leftBreakAnimation->init();
	createAnimation(leftBreakAnimation, "sk_ch_leftbreak.png", 2, 2, 150, 140);
	
	rightBreakAnimation = new CCAnimation;
	rightBreakAnimation->init();
	createAnimation(rightBreakAnimation, "sk_ch_rightbreak.png", 2, 2, 150, 140);
	
	
}
void SKCharacter::leftAction()
{
	CCLog("left!!");
	addFrames(leftAnimation->getFrames());
	this->startAnimation(1, 2, 1, 0, 0, 15, false, false);
	ACTION_STATE = LEFT_ACTION;
	
	BOOSTSTATE = SKCharacter::DEBOOSTING;
	boostEffect.init(boostEffect.getValue(), 0.f, BACK_DURATION);
}
void SKCharacter::rightAction()
{
	addFrames(rightAnimation->getFrames());
	this->startAnimation(1, 2, 1, 0, 0, 15, false, false);
	ACTION_STATE = RIGHT_ACTION;
	
	BOOSTSTATE = SKCharacter::DEBOOSTING;
	boostEffect.init(boostEffect.getValue(), 0.f, BACK_DURATION);
}
void SKCharacter::goStraight()
{
	addFrames(charAnimation->getFrames());
	this->startAnimation(1, 11, -1, 0, 0, 15, false, false);
	ACTION_STATE = STRAIGHT_ACTION;
	boostEffect.init(boostEffect.getValue(), 30.f, FRONT_DURATION);
	BOOSTSTATE = SKCharacter::BOOSTING;
	
}
void SKCharacter::leftStunAction()
{
	addFrames(leftBreakAnimation->getFrames());
	this->startAnimation(1, 2, 1, 0, 0, 30, false, false);
	ACTION_STATE = ETC;
}
void SKCharacter::rightStunAction()
{
	addFrames(rightBreakAnimation->getFrames());
	this->startAnimation(1, 2, 1, 0, 0, 30, false, false);
	ACTION_STATE = ETC;
}
void SKCharacter::crashObstacleAction(enum CRASHTYPE ct)
{
	if(ct == CRASHTYPE::LEFT_CRASH)
	{
		addFrames(leftBreakAnimation->getFrames());
		this->startAnimation(1, 2, -1, 0, 0, 15, false, false);
	}
	else
	{
		addFrames(rightBreakAnimation->getFrames());
		this->startAnimation(1, 2, -1, 0, 0, 15, false, false);
	}
}

void SKCharacter::render()
{
	/*
	 enum {LEFT=-1, NONE=0, RIGHT=1}MOVESTATE; // 이동 방향을 나타냄. 실제 그래픽과 비동적임.
	 enum {LEFT_ACTION, STRAIGHT_ACTION, RIGHT_ACTION} ACTION_STATE; // 이동 방향에 따른 실제 그래픽이 표시되고 있는지 여부. 그래픽과 동기적임.
	 enum {STUN=1, NORMAL=2, UNBEATABLE=3} PLAYERSTATE; // STUN : 맞아서 다리를 들고 있는 상태, NORMAL : 일반 진행, UNBEATABLE : 무적 상태
	 */
	if(PLAYERSTATE != STUN)
	{
//		CCLog("MOVESTATE : %d", MOVESTATE);
		if(MOVESTATE == LEFT && ACTION_STATE != LEFT_ACTION)
		{
			leftAction();

		}
		else if(MOVESTATE == RIGHT && ACTION_STATE != RIGHT_ACTION)
		{
			rightAction();
			
		}
		else if(MOVESTATE == NONE && ACTION_STATE != STRAIGHT_ACTION)
		{
			goStraight();
			
		}
	}
	else
	{
//		CCLog("else");
	}
		
}

