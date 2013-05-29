//
//  Bird.cpp
//  AngryKeeper
//
//  Created by ksoo k on 12. 9. 10..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "AKBird.h"
#include "KS_Util.h"
#include "AKTextureFactory.h"

#include "AKPListManager.h"
#include "AKAngryGameScene.h"
#include "KSBaseScene.h"
#include "AdvanceSprite.h"


Bird::Bird(AngryGameScene* d, float velo)
{
	gameDirector = d;
	isGoal = false;
	velocity = velo;
	startTime = 0;
	
	
}


Bird::~Bird()
{
	//birdBody->release();
	delete dishAnimation;
}

void Bird::initABC()
{
	float u,w;
	float sx, sy;
	if(!toRight)
	{
		u = 50;
		w = 300;
		//w = 100;
		w -= rand() % 200;
		sx = 550;
		sy = 100;
		path.push_back(ccp(sx, sy));
		path.push_back(ccp(u, w));
		setPosition(ccp(sx, sy));
	}
	else
	{
		//velocity *= 1.23f;
		velocity *= 1.4f;
		u = 480 - 50;
		w = 270;
		//w = 100;
		w -= rand() % 150;
		sx = -120;
		sy = 100;
		path.push_back(ccp(sx, sy));
		path.push_back(ccp(u, w));
		setPosition(ccp(sx, sy));
	}
	
	
	
	float x1 = sx;
	float y1 = sy;
	currentX = sx;
	// 꼭지점 u, w 과 나머지 한점 x1, y1 을 지날 때 이차 방정식의 a,b,c 구하기.
	a = (y1 - w) / ((u - x1)*(u - x1));
	c = ((y1*u*u) - 2*w*x1*u + w*x1*x1) / ((u - x1)*(u - x1));
	b = -2*a*u;
	
}

void Dish::initFlying(bool toR, bool zeroMode)
{
	toRight = toR;
	initABC();
	AdvanceSprite* dish = new AdvanceSprite;
	dish->init();
	dish->autorelease();
	
	dishAnimation = new CCAnimation();
	dishAnimation->init();
	addChild(dish, 1);
	
	if(toR)
	{
		int r = rand()%2 + 2; // 2~3 범위에 대해.
		createAnimation(dishAnimation, KS_Util::stringWithFormat("ak_dish%d.png", r).c_str(), FRAME_NUM, FRAME_NUM, BIRD_WIDTH, BIRD_HEIGHT);
		dishNumber = r;
	}
	else
	{
		dishNumber = 1;
		createAnimation(dishAnimation, "ak_dish1.png", FRAME_NUM, FRAME_NUM, BIRD_WIDTH, BIRD_HEIGHT);
	}
	dish->addFrames(dishAnimation->getFrames());
	dish->startAnimation(0, FRAME_NUM - 1, -1, 0, 0, 15, false, false);
	
	
	if(zeroMode)
	{
		CCParticleSystemPoint* dishPart = CCParticleSystemPoint::particleWithFile("ak_dishshadow.plist");

		CCSize dishSize = dish->getContentSize();
		dishPart->setPosition(ccp(dishSize.width / 2.f, dishSize.height / 2.f));
		//dishPart->setAnchorPoint(ccp(0, 0));
		dish->addChild(dishPart, -1);
	}
	
	
}

void NotDish::initFlying(bool toR, bool zeroMode)
{
	toRight = toR;
	initABC();
	
	AdvanceSprite* dish = new AdvanceSprite;
	dish->init();
	dish->autorelease();
	
	dishAnimation = new CCAnimation();
	dishAnimation->init();
	addChild(dish, 1);
	
	int bird = rand()%2 + 1;
	
	createAnimation(dishAnimation, KS_Util::stringWithFormat("ak_bird%d.png", bird).c_str(), FAKEFRAME_NUM, FAKEFRAME_NUM, FAKE_WIDTH, FAKE_HEIGHT);
	dish->addFrames(dishAnimation->getFrames());
	dish->startAnimation(0, FAKEFRAME_NUM - 1, -1, 0, 0, 15, false, false);
	if(!toR)
		dish->setFlipX(true);
}



void Bird::setNextPath(float dt)
{
	if(isGoal == false)
	{
		if(toRight)
			currentX += MIN(250.f, velocity) * dt; //## 250
		else
			currentX -= MIN(230.f, velocity) * dt; // 230.f
//		else
//			currentX -= MIN(310.f, velocity) * dt;
		float Y = pathFunction(currentX);
		setPosition(ccp(currentX, Y));
		if((currentX < 0 && !toRight) || (currentX > 480 && toRight))
		{
			isGoal = true;
		}
	}
}


void NotDish::damaged(){
	
	gameDirector->onShotBird(toRight);
}