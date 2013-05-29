//
//  SKObstacle.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 5. 10..
//
//

#include "SKObstacle.h"
#include "KS_Util.h"

int SKObject::myOrder = 50000;
const float SKObstacle::JUDGE_HEIGHT = 0;

void SKObject::setObsX(float o)
{
	prevObsX = obsX;
	if(isFirstSetX)
		prevObsX = o;
	isFirstSetX = false;
	obsX = o;
}
float SKObject::getObsX()
{
	return obsX;
}
void SKObject::setDistance(float d)
{
	prevObsDistance = obsDistance;
	if(isFirstSetD)
		prevObsDistance = d;
	isFirstSetD = false;
	obsDistance = d;
}
float SKObject::getDistance()
{
	return obsDistance;
}

float SKObject::getPrevObsDistance()
{
	return prevObsDistance;
}
float SKObject::getPrevObsX()
{
	return prevObsX;
}
void SKGrass::createSprite()
{
	obsAnimation = new CCAnimation;//::animation();
	obsAnimation->init();
	createAnimation(obsAnimation, KS_Util::stringWithFormat("sk_grass%d.png", rand()%3 + 1).c_str(), 1, 1, 23, 30);
	addFrames(obsAnimation->getFrames());
	this->startAnimation(1, 1, -1, 0, 0, 15, false, false);
	this->setAnchorPoint(ccp(0.5, 0.05f));
}
void SKPense::createSprite()
{
	obsAnimation = new CCAnimation;//::animation();
	obsAnimation->init();
	createAnimation(obsAnimation, "sk_tacle10.png", 1, 1, 280, 100);
	addFrames(obsAnimation->getFrames());
	this->startAnimation(1, 1, -1, 0, 0, 15, false, false);
	this->setAnchorPoint(ccp(0.5, 0.05f));
	obsSize = SIZE;
}

void SKBigStone::createSprite()
{
	obsAnimation = new CCAnimation;//::animation();
	obsAnimation->init();
	createAnimation(obsAnimation, "sk_tacle8.png", 1, 1, 170, 160);
	addFrames(obsAnimation->getFrames());
	this->startAnimation(1, 1, -1, 0, 0, 15, false, false);
	this->setAnchorPoint(ccp(0.5, 0.05f));
	obsSize = SIZE;
}

void SKCuttingTree1::createSprite()
{
	obsAnimation = new CCAnimation;//::animation();
	obsAnimation->init();
	createAnimation(obsAnimation, "sk_tacle5.png", 1, 1, 113, 100);
	addFrames(obsAnimation->getFrames());
	this->startAnimation(1, 1, -1, 0, 0, 15, false, false);
	this->setAnchorPoint(ccp(0.5, 0.05f));
	obsSize = SIZE;
}
void SKCuttingTree2::createSprite()
{
	obsAnimation = new CCAnimation;//::animation();
	obsAnimation->init();
	createAnimation(obsAnimation, "sk_tacle6.png", 1, 1, 72, 100);
	addFrames(obsAnimation->getFrames());
	this->startAnimation(1, 1, -1, 0, 0, 15, false, false);
	this->setAnchorPoint(ccp(0.5, 0.05f));
	obsSize = SIZE;
}

#pragma mark SKStone
void SKStone::createSprite()
{
	obsAnimation = new CCAnimation;//::animation();
	obsAnimation->init();
	createAnimation(obsAnimation, "sk_tacle7.png", 1, 1, 85, 100);
	addFrames(obsAnimation->getFrames());
	this->startAnimation(1, 1, -1, 0, 0, 15, false, false);
	this->setAnchorPoint(ccp(0.5, 0.05f));
	obsSize = SIZE;
}
#pragma mark SKTree
void SKTree::createSprite()
{
	obsAnimation = new CCAnimation;//::animation();
	obsAnimation->init();
	createAnimation(obsAnimation, KS_Util::stringWithFormat("sk_tacle%d.png", rand()%3 + 1).c_str(), 1, 1, 156, 200);
	addFrames(obsAnimation->getFrames());
	this->startAnimation(1, 1, -1, 0, 0, 15, false, false);
	this->setAnchorPoint(ccp(0.5, 0.05f));
	obsSize = SIZE;
}





