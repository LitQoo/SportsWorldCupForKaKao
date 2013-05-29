//
//  Player.m
//  HurdleWorldCup
//
//  Created by ksoo k on 12. 4. 10..
//  Copyright (c) 2012≥‚ __MyCompanyName__. All rights reserved.
//


#include "HWPlayer.h"
#include "NSDefault.h"
#include "HWSharingClass.h"
#include "KS_Util.h"
#include "HWPlay.h"
// # #include "KSoundEngine.h"

extern int mygravity;

static int MIN_JUMP = 175;




const int Player1::JUMPING = 1;
const int Player1::UNJI = 2;
const int Player1::NORMAL = 4;
const int Player1::SUPER = 32;
const int Player1::REVIVE = 64;
void Player1::blink()
{
	KS::KSLog("blink!!");
	runHuman->runAction(
						CCSequence::actions(
											CCBlink::actionWithDuration(1.2, 7),
											KSFunc({STATE &= ~SUPER;}),
											0));

	jumpHuman->runAction(
						 CCSequence::actions(
											 CCBlink::actionWithDuration(1.2, 7),
											 KSFunc({ STATE &= ~SUPER;} ),
											 0));
	//player->STATE &= ~Player::SUPER;
}



void Player1::jumpUp(float power)
{
	if(STATE & NORMAL)
	{
		this->jumpSound();
		
		float jumpSize;
		jumpSize = power;
		// √÷º“¿« ∆ƒøˆ sqrtf((p - player->maxJumpBenefic()) * 2 * 980)
		
		this->setPosition(ccp(this->getPosition().x,this->getPosition().y+1));
		//this->unjiWithUnjiTime(hwplay->gameTimer, jumpSize); // 20,30]
		

		setFrame(HurdleMan::JUMP, 0);
		
		float centerFrame = jumpAnimation->getFrames()->count() / 2.f;
		setJumpHumanDelay(((float)jumpSize/mygravity)/centerFrame);
		playJumpAnimation();

	}
}
int Player1::height()
{
	if(STATE & NORMAL)
		return 75; //
	else if(STATE & JUMPING){
		return 75;
	}
	else {
		return 75;
	}
}

int Player1::width()
{ // ∏ˆ ∂◊∂◊¿Ã.
	return 20;
}

int Player1::jumpBenefic()
{
	if(jumpHuman->getOpacity()>0)
	{
		int frame_number = jumpHuman->getCurrentIndex();
		switch (frame_number) {
			case 0: return 14;
			case 1: return 22;
			case 2: return 26;
			case 3: return 22;
			case 4: return 26;
			case 5: return 32;
			case 6: return 40;
			case 7: return 36;
			case 8: return 27;
			case 9: return 17;
			case 10:return 6;
			case 11:return 0;
			case 12:return 0;
			default:return 0;
		}

	}
	else
		return 0;
}



void Player1::startRun(float d)
{
	setFrame(HurdleMan::RUN, 0);
}




void HurdleMan::initMan()
{
	jumpHuman = new AdvanceSprite;
	jumpHuman->init();
	jumpHuman->autorelease();
	createAnimation(jumpAnimation, "hw_jumpman.png", JUMPMAN_COLS, JUMPMAN_FRAMES, JUMPMAN_WIDTH, JUMPMAN_HEIGHT);
	jumpHuman->addFrames(jumpAnimation->getFrames());
	//jumpHuman->startAnimation(0, JUMPMAN_FRAMES, -1, 0, 0, 20, false, false);
	jumpHuman->pauseAnimation();
	jumpHuman->setOpacity(0);
	addChild(jumpHuman, 1);
	
	runHuman = new AdvanceSprite;
	runHuman->init();
	runHuman->autorelease();
	createAnimation(runAnimation, "hw_runman.png", RUNMAN_COLS, RUNMAN_FRAMES, RUNMAN_WIDTH, RUNMAN_HEIGHT);
	runHuman->addFrames(runAnimation->getFrames());
	runHuman->setOpacity(0);
	addChild(runHuman, 1);
	
	standHuman = CCSprite::spriteWithFile("hw_startman.png");
	addChild(standHuman, 1);
	
	
	standHuman->setAnchorPoint(ccp(0.5, 0));
	runHuman->setAnchorPoint(ccp(0.5, 0));
	jumpHuman->setAnchorPoint(ccp(0.5, 0));
	
	
	int h = standHuman->getTextureRect().size.height;
	
	auto animator2 = SceneUtil::playAnimation("hw_zero_back.png", 0.09, 3, 3, 90, 120, true);
	burstBack = animator2.second;
	burstBack->setAnchorPoint(ccp(1, 0.5));
	burstBack->runAction(animator2.first);
	burstBack->setPosition(ccp(30, h/2));
	addChild(burstBack, -1);
	burstBack->setIsVisible(false);
	//burstBack->setColor(ccc3(100, 100, 100));

	Burst* part = Burst::burst();
	part->setPosition(ccp(-10, h/2));
	addChild(part, -2);
	burstParticle = part;
	burstParticle->setIsVisible(false);
//	burstParticle->setIsVisible(false);
//	burstParticle->setEmissionRate(12.f);
//	burstParticle->setIsVisible(false);
}

void Burst::update(ccTime dt)
{
	gTimer += dt;
	
	if(nextCreateTime <= gTimer)
	{
		createTime = gTimer;
		nextCreateTime += 0.12f;
		
		CCSprite* t = CCSprite::spriteWithTexture(bn->getTexture());
		FromTo ft;
		ft.init(0.f, -130.f, 5.f * 1.5f);
		bursts.push_back(make_tuple(t, ft, convertToWorldSpace(CCPointZero).y ));
		
		t->setPosition(ccp(-10, 10));
		t->setScale(5.f);
//		t->runAction(CCSequence::actions(CCMoveBy::actionWithDuration(0.3f, ccp(-100, 0)), CCCallFuncN::actionWithTarget(this, callfuncN_selector(Burst::deleteSprite)),0));
		bn->addChild(t);
	}
	
	for(auto iter = bursts.begin(); iter != bursts.end(); )
	{
		auto& second = get<1>(*iter);
		CCSprite* first = get<0>(*iter);
		float startY = get<2>(*iter);
		if(second.step() == false)
		{
			first->removeFromParentAndCleanup(true);
			bursts.erase(iter++);
		}
		else
		{
			CCPoint current = convertToWorldSpace(CCPointZero);
			float diff = startY - current.y;
			first->setPosition(ccp( second.getValue(), diff));
			++iter;
		}
	}
}

/////////