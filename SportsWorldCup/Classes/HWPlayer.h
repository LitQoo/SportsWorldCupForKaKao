//
//  Player.h
//  HurdleWorldCup
//
//  Created by ksoo k on 12. 4. 10..
//  Copyright (c) 2012≥‚ __MyCompanyName__. All rights reserved.
//
#pragma once
#include "CCLayer.h"
#include "cocos2d.h"
#include "AdvanceSprite.h"
#include "KSBlock.h"
#include "KSBaseScene.h"
#include "HWFromTo.h"
using namespace cocos2d;
class Burst : public CCLayer
{
private:
	vector<tuple<CCSprite*, FromTo, float>> bursts;
	
	void deleteSprite(CCNode* _n)
	{
		_n->getParent()->removeChild(_n, true);
	}
	CCSpriteBatchNode* bn;
	float gTimer;
	float nextCreateTime, createTime;
	void update(ccTime dt);
public:
	LAYER_NODE_FUNC(Burst);
	static Burst* burst()
	{
		Burst* b = new Burst();
		b->init();
		b->autorelease();
		return b;
	}
	virtual bool init()
	{
		CCLayer::init();
		gTimer = 0;
		nextCreateTime = 0;
		bn = CCSpriteBatchNode::batchNodeWithFile("hw_manshadow.png", 10);
		addChild(bn);
		
		schedule(schedule_selector(Burst::update));
		return true;
	}
};


class HurdleMan : public CCSprite
{
	static const int RUNMAN_FRAMES = 16;
	static const int RUNMAN_WIDTH = 50;
	static const int RUNMAN_HEIGHT = 75;
	
	static const int RUNMAN_COLS = 10;
	
	static const int JUMPMAN_FRAMES = 13;
	static const int JUMPMAN_WIDTH = 55;
	static const int JUMPMAN_HEIGHT = 77;
	static const int JUMPMAN_COLS = 9;
protected:
	bool isFaster;
	CCAnimation* runAnimation;
	CCAnimation* jumpAnimation;
	CCSprite* burstBack;
	
	AdvanceSprite* jumpHuman;
	AdvanceSprite* runHuman;
	CCSprite* standHuman;
	void createAnimation(CCAnimation* animation, string imgFile, int cols, int frames, int width_per_each, int height_per_each)
	{
		///		adHuman->add
		CCSprite* sprite = CCSprite::spriteWithFile(imgFile.c_str());
		for(int f = 1; f<=frames; f++) // 프레임 개수만큼.
		{
			int col = (f - 1 ) % cols;
			int row = (f - 1 ) / cols;
			animation->addFrameWithTexture(sprite->getTexture(), CCRectMake(width_per_each*col, height_per_each * row, width_per_each, height_per_each));
		}
	}

public:
	Burst* burstParticle;
	HurdleMan()
	{
//		setAnchorPoint(ccp(0.5f, 0.5f));
//		m_bIsRelativeAnchorPoint = false;
		isFaster = false;
		runAnimation = new CCAnimation();
		runAnimation->init();
		
		jumpAnimation = new CCAnimation();
		jumpAnimation->init();
	}
	void setJumpHumanDelay(float f)
	{
		jumpHuman->setFrameRate(f);
	}
	void setRunHumanDelay(float f)
	{
		runHuman->setFrameRate(f);
	}
	void playJumpAnimation()
	{
		jumpHuman->resumeAnimation();
	}
	void showBurst()
	{
		burstBack->setIsVisible(true);
		burstParticle->setIsVisible(true);
	}
	void hideBurst()
	{
		burstBack->setIsVisible(false);
		burstParticle->setIsVisible(false);
	}
	enum COMMAND{STAND, RUN, JUMP};
	static HurdleMan* sprite()
	{
		HurdleMan* e = new HurdleMan();
		e->cocos2d::CCSprite::init();
		e->autorelease();
		return e;
	}
	
	void decreaseManAfterJump()
	{
		standHuman->setOpacity(0);
		runHuman->setOpacity(0);
		jumpHuman->setOpacity(255);
		jumpHuman->stopAnimaiton();
		jumpHuman->startAnimation(6, JUMPMAN_FRAMES-1, 1, 0, 0, 35, false, false);
	}
	void fasterRun()
	{
		isFaster = true;
	}
	void normalRun()
	{
		isFaster = false;
	}
	void setFrame(enum COMMAND s, int index)
	{
		switch(s)
		{
			case RUN:
				standHuman->setOpacity(0);
				jumpHuman->setOpacity(0);
				runHuman->setOpacity(255);
				//runHuman->moveTo(index);
				if(isFaster)
					runHuman->startAnimation(0, RUNMAN_FRAMES - 1, -1, 0, 0, 30, false, false);
				else
					runHuman->startAnimation(0, RUNMAN_FRAMES - 1, -1, 0, 0, 20, false, false);
				break;
			case JUMP:
				standHuman->setOpacity(0);
				runHuman->setOpacity(0);
				jumpHuman->setOpacity(255);
				jumpHuman->moveTo(index);
				runHuman->startAnimation(0, 1, 1, 0, 0, 100, false, false);
				
				jumpHuman->startAnimation(0, 7, 1, 0, 0, 35, false, false);
				break;
			case STAND:
				jumpHuman->setOpacity(0);
				runHuman->setOpacity(0);
				standHuman->setOpacity(255);
				break;
		}
	}
	void initMan();
	
	virtual ~HurdleMan()
	{
		delete runAnimation;
		delete jumpAnimation;
	}	
};

class Player1 : public HurdleMan
{
protected:
	float prevPosition;
	int prevJumpBenefic;
	
public:
	bool unjing; // 현재 떨어져있는지.
	int STATE;
	static const int JUMPING;
	static const int UNJI;
	static const int NORMAL;
	static const int SUPER;
	static const int REVIVE;
public:
	void moveMan(float displacement)
	{
		//setPrevPosition(getPosition().y);
		setPosition(ccp(getPosition().x, getPosition().y + displacement));
	}
	int maxJumpBenefic()	{		return 35;	}
	int getPrevJumpBenefic()	{return prevJumpBenefic;	}
	void setPrevJumpBenefic(int p){		prevJumpBenefic = p;	}
	float getPrevPosition()	{		return prevPosition;	}
	void setPrevPosition(float arg)	{		prevPosition = arg;	}
	int height();
	int width();
	virtual void jumpSound(){}
	int jumpBenefic();
	//void unjiWithUnjiTime(double t, float vel);
	void startRun(float d);
	void jumpUp(float power);
	void blink();
public:
	virtual bool init()
	{
		HurdleMan::init();
		prevPosition = getPosition().y;
		STATE = NORMAL;
		prevJumpBenefic = 0;
		unjing = false;
		return true;
	}
	static Player1* node()
	{
		Player1* e = new Player1();
		e->init();
		e->initMan();
		
		e->autorelease();
		return e;
	}
};

