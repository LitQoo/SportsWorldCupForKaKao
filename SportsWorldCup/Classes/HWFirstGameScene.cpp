//
//  SecondGameScene.m
//  HurdleWorldCup
//
//  Created by ksoo k on 12. 4. 27..
//  Copyright (c) 2012≥‚ __MyCompanyName__. All rights reserved.
//

#include "HWFirstGameScene.h"
#include "HWFirstHurdleMapLayer.h"
#include "WCPlay.h"
#include "CCUtil.h"
#include "KSoundEngine.h"
#include "KS_Util.h"
#include "KSLabelTTF.h"

#include "LoadingScene.h"
#include "HWPlay.h"
#include "SWIntro.h"
#include "GameOver.h"
int FirstGameScene::FARBACK_SIZE = 480;
int FirstGameScene::FARBACK_HEIGHT_SIZE = 320;

int FirstGameScene::NORMALBACK_SIZE = 480;
int FirstGameScene::NORMALBACK_HEIGHT_SIZE = 82;

static int AD_SIZE = 573;

extern int mygravity;

void FirstGameScene::onEnterTransitionDidFinish()
{
	GameScene::onEnterTransitionDidFinish();
	
	closeBacks = new CCMutableArray<CCSprite*>();
	///this->setScale(0.2);
	_threerepeat = true;
	screenCount = 0;
	normalBackPos = adBackPos = closeBackPos = farBackPos = 0.f;
	this->setBackground();

//	KSoundEngine::sharedEngine()->playSound("sound/bgm1.mp3");
	
	randCount = 20 + rand()%10;
	this->schedule(schedule_selector(FirstGameScene::randomSound), 1);

}
void FirstGameScene::setBackground()
{
	farBackPos = normalBackPos = closeBackPos = 0.f;
	ad_back1 = CCSprite::spriteWithSpriteFrameName("back1_ad1.png");
	ad_back1->setScale(2.0f);
	ad_back1->getTexture()->setAliasTexParameters();
	ad_back1->setPosition(ccp(0,134));
	ad_back1->setAnchorPoint(ccp(0,0.5));
	ad_back2 = CCSprite::spriteWithSpriteFrameName("back1_ad1.png"); // ø¯∑°¥¬ ad2.png ΩËæÓæﬂµ .
	ad_back2->setScale(2.0f);
	ad_back2->getTexture()->setAliasTexParameters();
	ad_back2->setPosition(ccp(AD_SIZE,134));
	ad_back2->setAnchorPoint(ccp(0,0.5));
	//if(!_sha->optimizingOption)
	{
		trackBatch->addChild(ad_back1, 4); 
		trackBatch->addChild(ad_back2, 4);
	}
	
	
	
	farBack1 = CCSprite::spriteWithSpriteFrameName("farback1.png");
	farBack1->setScale(2.f);
	farBack1->getTexture()->setAliasTexParameters();
	farBack1->setAnchorPoint(ccp(0,0));
	farBack1->setPosition(ccp(0,320 - FARBACK_HEIGHT_SIZE));
	farBack2 = CCSprite::spriteWithSpriteFrameName("farback1.png");
	farBack2->setScale(2.f);
	farBack2->getTexture()->setAliasTexParameters();
	farBack2->setAnchorPoint(ccp(0,0));
	farBack2->setPosition(ccp(FARBACK_SIZE,320 - FARBACK_HEIGHT_SIZE));
	{
		trackBatch->addChild(farBack1, 0);
		trackBatch->addChild(farBack2, 0);
	}
	
	
	normalBack1 = CCSprite::spriteWithSpriteFrameName("normalback1_cloud.png");
	normalBack1->setScale(2.f);
	normalBack1->getTexture()->setAliasTexParameters();
	normalBack1->setAnchorPoint(ccp(0,0));
	normalBack1->setPosition(ccp(0,320 - FirstGameScene::NORMALBACK_HEIGHT_SIZE));
	normalBack2 = CCSprite::spriteWithSpriteFrameName("normalback1_cloud.png");
	normalBack2->setScale(2.f);
	normalBack2->getTexture()->setAliasTexParameters();
	normalBack2->setAnchorPoint(ccp(0,0));
	normalBack2->setPosition(ccp(FirstGameScene::NORMALBACK_SIZE,320 - FirstGameScene::NORMALBACK_HEIGHT_SIZE));
	{
		trackBatch->addChild(normalBack1, 1); trackBatch->addChild(normalBack2, 1);
	}
	
	
	//////////////////////////////////////////////////////////////////
	
	lastBackPos = 0;
	
	screenCount += 2;
	while(lastBackPos <= 480*screenCount) // µŒ»≠∏È æ»ø°.
	{
		if(_threerepeat == true)
		{
			for(int i=0; i<3; i++)
			{
				CCSprite* closeBack = CCSprite::spriteWithSpriteFrameName("closeback1_1.png");
				closeBack->setScaleX(1.02);
				closeBack->setAnchorPoint(ccp(0.5,0));
				closeBack->setPosition(ccp(lastBackPos,130));
				closeBack->getTexture()->setAliasTexParameters();
				lastBackPos += closeBack->getTextureRect().size.width; 
				//if(!_sha->optimizingOption)
				{
					closeBacks->addObject(closeBack);
					trackBatch->addChild(closeBack, 2);
				}				
			}
		}
		else {
			int r = rand()%3;
			r+=2;
			CCSprite* closeBack = CCSprite::spriteWithSpriteFrameName(KS_Util::stringWithFormat("closeback1_%d.png",r).c_str());
			closeBack->setScaleX(1.02);
			closeBack->setAnchorPoint(ccp(0.5,0));
			closeBack->setPosition(ccp(lastBackPos,130));
			closeBack->getTexture()->setAliasTexParameters();
			lastBackPos += closeBack->getTextureRect().size.width;
			//if(!_sha->optimizingOption)
			{
				closeBacks->addObject(closeBack);
				trackBatch->addChild(closeBack, 2);
			}
			
		}
		_threerepeat = !_threerepeat;
	}
}




void FirstGameScene::initHurdleMapAndPlayer()
{
	//hl = FirstHurdleMapLayer::alloc()->init(); // 
	hl = new FirstHurdleMapLayer();
	hl->init();
	hl->setInfo(&info);
	player = Player1::node();
	player->setFrame(HurdleMan::STAND, 0);
}



bool FirstGameScene::init()
{
	GameScene::init();
	info.remainTime = info.INIT_GAME_TIME;
//	setScale(0.1f);
	return true;	
}



void FirstGameScene::randomSound(ccTime dt)
{
	randCount--;
	if(randCount <= 0)
	{
		randCount = 20 + rand()%10;
//		rand()%2? KSoundEngine::sharedEngine()->playSound("sound/tracksub1.mp3") : KSoundEngine::sharedEngine()->playSound("sound/tracksub2.mp3");
		// ªÁøÓµÂ ¿Áª˝
	}
}

void FirstGameScene::cachingPList()
{
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("trackbg.plist");
	trackBatch = CCSpriteBatchNode::batchNodeWithFile("trackbg.png", 50);
	this->addChild(trackBatch);
}
void FirstGameScene::unjiCheck()
{
	bool shieldMode = false;
	if(player->getPosition().y <= 0 && player->unjing == false)
	{
		if(playInfo->selectedItems["shield"] == true && info.usedShield == false)
		{
			KSoundEngine::sharedEngine()->playSound("shield.mp3");
			info.usedShield = true;
			shieldMode = true;
		}
		if(shieldMode == false)
		{
			hwplay->combo = 0;
			KSoundEngine::sharedEngine()->playSound("uhuh.mp3");
		}
//		ReplaceScene(SWIntro);
//		return;
		player->unjing = true;
		hl->reviveEvent = true;
		hl->createScreen();
		hl->reviveEvent = false;
		info.runSpeed2.init(info.runSpeed2.getTo(), info.runSpeed2.getTo(), 0);
		KS::KSLog("run speed % when unji1", info.runSpeed2.getValue());
		info.runSpeed2.step(1);
		KS::KSLog("run speed % when unji2", info.runSpeed2.getValue());
		CCLog("-------------------create Screen-----------------");
		//info.speedCombo = 0;
		//feelSpeed(info.speedCombo);
		
		// 떨어졌을 때 부활
		if(shieldMode == false)
		{
			if(info.gameMode == HWGameInformation::ZERO)
			{
				//feverGauge->setPercentage(feverGauge->getPercentage() + HWInnerVar::CRASH_DEC_VALUE_IN_ZERO); //##
				feverGauge->setPercentage(0);
				//onExitZeroMode();
			}
			else
			{
				feverGauge->setPercentage(feverGauge->getPercentage() + HWInnerVar::CRASH_DEC_VALUE_IN_NORMAL);
			}
		}
		
		
		if(info.gameMode == HWGameInformation::ZERO && feverGauge->getPercentage() <= 0.f)
		{
			info.gameMode = HWGameInformation::NORMAL;
			onExitZeroMode();
		}
		
	}
	

	
	if(player->unjing == true && -hl->getPosition().x >= hl->revivePosition - 150)
	{
		player->unjing = false;
		int targetHeight=-1;
		float targetX = -1;
		for(int i=0; i<hl->getObstacles()->count(); i++)
		{
			Obstacle* ob = hl->getObstacles()->getObjectAtIndex(i);
			
			int playerPosition = info.humanPosition - hl->getPosition().x;
			if(ob->getStart() <= playerPosition && playerPosition <= ob->getStart() + ob->getLength() * hl->getPatternWidth())// && -hl->getPosition().x  > hl->getPatternWidth() + ob->getStart())
				//(ob->getStart() <= -hl->getPosition().x && -hl->getPosition().x <= ob->getStart() + ob->getLength() * hl->getPatternWidth()))
			{
				targetHeight = ob->height;
				targetX = ob->getStart() - hl->getPatternWidth();
				
				float t = fabsf((targetX + hl->getPosition().x) / (60*info.runSpeed2.getValue()) );
				if(targetHeight / t - 0.5*HWInnerVar::hurdleG * t > 2000)
					continue;
				break;
			}
		}
		bool notFound = false;
		if( targetHeight == -1)
		{
			notFound = true;
			CCLog("target is not found");
			float minDistance = 9999999.f;
			for(int i=0; i<hl->getObstacles()->count(); i++)
			{
				Obstacle* ob = hl->getObstacles()->getObjectAtIndex(i);
				float distance =  fabsf(ob->getStart() - fabsf(hl->getPosition().x));
				if(minDistance > distance)
				{
					minDistance = distance;
					targetHeight = ob->height;
					targetX = ob->getStart() - hl->getPatternWidth();
				}
			}
		}
		else
			CCLog("target is found");
		player->setPosition(ccp(player->getPosition().x,1));
		player->setFrame(HurdleMan::JUMP, 0);
		
		innerInfo.beginDrop = false;
		player->STATE &= ~Player1::NORMAL;
		player->STATE &= ~Player1::UNJI;
		player->STATE |= Player1::JUMPING;
		innerInfo.reservedJump = HWInnerVar::NONE;
		player->STATE |= Player1::REVIVE;
		CCLog("%f %f / %f", targetX, hl->getPosition().x, 60*info.runSpeed2.getValue());
		float t = fabsf((targetX + hl->getPosition().x) / (60*info.runSpeed2.getValue()) );
		CCLog("%f / %f - 0.5*%f * %f = %f", (float)targetHeight, t, HWInnerVar::hurdleG, t, targetHeight / t - 0.5*HWInnerVar::hurdleG * t);
		float initialV = targetHeight / t - 0.5*HWInnerVar::hurdleG * t;
		if(notFound)
			initialV = sqrtf( (targetHeight) * 2.f * -HWInnerVar::hurdleG );
		initialV = MIN(1300, initialV);
		
		
		innerInfo.yVelocity = initialV;
		KS::KSLog("initv %", initialV);
		

		hl->setBalance(hwplay->combo.getVar());
		player->STATE |= Player1::SUPER;
		//## 캐릭터 깜빡이기
		player->blink();
	}

	
}

void FirstGameScene::timeCheck(ccTime dt)
{
	if(info.gameMode == HWGameInformation::ZERO)
	{
		innerInfo.timeAfterZero += dt;
		
		int nextStep = MIN((int)(innerInfo.timeAfterZero / 5.f + 1.f), 6);
		if(nextStep != innerInfo.beforeRunStep)
		{
			info.runStep = nextStep;
			innerInfo.beforeRunStep = info.runStep;
			info.runSpeed2.init(info.runSpeed2.getValue(), calSpeed(), 3.f);
			
			feelSpeed(hwplay->speedCombo);
		}
	}
	if(innerInfo.isTimeover == false && info.gameMode != HWGameInformation::ZERO)
	{
		info.remainTime -= dt;
	}
//	if(info.remainTime < 0 && innerInfo.isTimeover == false)
//	{
//		innerInfo.isTimeover = true;	
//		info.remainTime = 0.;
//		//restTimeSpr->runAction(CCRepeatForever::actionWithAction(CCBlink::actionWithDuration(0.24, 1)));
//	}

	
	if(info.remainTime <= 10 && !info.in10s)
	{
		KSoundEngine::sharedEngine()->playSound("hurryup.mp3");
		info.in10s = true;
		watch1->setIsVisible(false);
		watch2->setIsVisible(true);
	}
	if(info.remainTime < 0)
	{
		if(playInfo->selectedItems["time"] && !watch3->getIsVisible())
		{
			watch2->setIsVisible(false);
			watch3->setIsVisible(true);
			timeFnt->setIsVisible(false);
			KSoundEngine::sharedEngine()->playSound("bonustime.mp3");
			auto mator = SceneUtil::playAnimation("bonustime.png", 0.08, 6, 6, 135, 135, 1);
			mator.second->runAction(CCSequence::actions(mator.first, CCCallFuncN::actionWithTarget(this, callfuncN_selector(ThisClassType::deleteSprite)),0));
			mator.second->setPosition(ccp(431, 277));
			addChild(mator.second, 10);
		}
		
		if(info.remainTime <= -info.bonusTime && info.isGameOvering == false)
		{
			info.isGameOvering = true;
			setIsTouchEnabled(false);
			pause->setIsEnabled(false);
			auto mator = SceneUtil::playAnimation("timeover.png", 0.1, 2, 4, TIMEOVERWIDTH, TIMEOVERHEIGHT, 8);
			KSoundEngine::sharedEngine()->playSound("timeover.mp3");
			mator.second->setPosition(ccp(240, 160));
			mator.second->setScale(1.2f);
			auto action = CCSequence::actionOneTwo(mator.first, CCCallFunc::actionWithTarget(this, callfunc_selector(ThisClassType::finishTimeOver)));
			mator.second->runAction(action);
			addChild(mator.second, 10);
		}
	}
	
}



void FirstGameScene::gameTest(ccTime dt)
{
	dt = 1/60.f;
	hwplay->gameTimer += dt;
	
	//맵 만들고
	hl->createObstacle(rootLayer->getScale());
	hl->createSprite(rootLayer->getScale());
	hl->removeSprite();
	
	//이동시키고
	moveMap(dt);
	moveMan(dt);
	moveBackground(dt);
	translation(dt);
	
//	CCLog("startCreationPosition : %d, hl : %f", hl->revivePosition, hl->getPosition().x);
	// 충돌체크 및 시간 체크
	crashCheck();
	unjiCheck();
	timeCheck(dt);
	resJumpCheck();
	
	if(info.gameMode == HWGameInformation::ZERO)
	{
		feverGauge->setPercentage(feverGauge->getPercentage() + HWInnerVar::ZERO_DEC_VALUE);
		if(feverGauge->getPercentage() <= 0.f)
		{
			info.gameMode = HWGameInformation::NORMAL;
			onExitZeroMode();
		}
	}
}
void FirstGameScene::finishTimeOver()
{
	
	graphdog->command("getweek", 0, this, gd_selector(ThisClassType::finishGetWeek));
	
}
void FirstGameScene::moveBackground(ccTime dt)
{
	//CCLog("%f",X);
	farBackPos-=1.f * (0.5 + (info.runSpeed-4)/8);
	normalBackPos -= 2.f * (0.5 + (info.runSpeed-4)/8);
	float dm = 3.f * (0.5 + (info.runSpeed-4)/8);
	
	adBackPos -= dm;
	farBackPos = 0; 
	farBack1->setPosition(ccp(0 + farBackPos,farBack1->getPosition().y));
	farBack2->setPosition(ccp(FARBACK_SIZE  + farBackPos, farBack1->getPosition().y));
	
	normalBack1->setPosition(ccp(0  + normalBackPos, normalBack1->getPosition().y));
	normalBack2->setPosition(ccp(FirstGameScene::NORMALBACK_SIZE+ normalBackPos, normalBack2->getPosition().y));
	
	ad_back1->setPosition(ccp(0  + adBackPos, ad_back1->getPosition().y));
	ad_back2->setPosition(ccp(AD_SIZE+ adBackPos, ad_back2->getPosition().y));
	closeBackPos-=dm;
	
	{
		float movedis = fabsf(farBackPos);
		if(movedis >= FARBACK_SIZE)
		{
			farBackPos += movedis;
		}
	}
	{
		float movedis = fabsf(normalBackPos);
		if(movedis >= FirstGameScene::NORMALBACK_SIZE)
		{
			normalBackPos += movedis;
		}
	}
	{
		float movedis = fabsf(adBackPos);
		if(movedis >= AD_SIZE)
		{
			adBackPos += movedis;
		}
	}
	{
		float movedis = fabsf(closeBackPos);
		//CCLog("GI : %d,DIS : %f",480 * (screenCount - 1),movedis);
		if(480 * (screenCount - 1) <= movedis)
		{
			CCMutableArray<CCSprite*>* temp = new CCMutableArray<CCSprite*>();
			temp->autorelease();
			for(int i=0; i<closeBacks->count(); i++)
			{
				CCSprite* s = closeBacks->getObjectAtIndex(i);
				if(s->getPosition().x < -480)
				{
					temp->addObject(s);
					trackBatch->removeChild(s, true);
				}
			}
			for(int i=0; i<temp->count(); i++)
			{
				CCSprite* s = temp->getObjectAtIndex(i);
				closeBacks->removeObject(s);
			}
			screenCount++;
			while(lastBackPos - 240 <= 480*screenCount) // «—»≠∏È ª˝º∫
			{
				if(_threerepeat == true)
				{
					for(int i=0; i<3; i++)
					{
						CCSprite* closeBack = CCSprite::spriteWithSpriteFrameName("closeback1_1.png");
						closeBack->setScaleX(1.02);
						closeBack->setAnchorPoint(ccp(0.5,0));
						closeBack->setPosition(ccp(lastBackPos - 480*(screenCount-2),130));
						lastBackPos += closeBack->getTextureRect().size.width; 
						closeBacks->addObject(closeBack);
						trackBatch->addChild(closeBack, 2);
					}
				}
				else {
					int r = rand()%3;
					r+=2;
					CCSprite* closeBack = CCSprite::spriteWithSpriteFrameName(KS_Util::stringWithFormat("closeback1_%d.png",r).c_str());
					closeBack->setScaleX(1.02);
					closeBack->setAnchorPoint(ccp(0.5,0));
					closeBack->setPosition(ccp(lastBackPos - 480*(screenCount-2),130));
					lastBackPos += closeBack->getTextureRect().size.width;
					closeBacks->addObject(closeBack);
					trackBatch->addChild(closeBack, 2);
				}
				_threerepeat = !_threerepeat;
			}
		}
		for(int i=0; i<closeBacks->count(); i++)
		{
			CCSprite* s = closeBacks->getObjectAtIndex(i);
			s->setPosition(ccp(s->getPosition().x - dm, s->getPosition().y));
		}
	} 
}

void FirstGameScene::moveMap(ccTime dt)
{	
	GameScene::moveMap(dt);
	adjustRunSpeed(dt);
	float decreaseValue = 60 * info.runSpeed * dt;
	if(player->unjing)
	{
		hl->setPosition(ccp(hl->getPosition().x - decreaseValue * 6, hl->getPosition().y));
	}
	else
	{
		hl->setPosition(ccp(hl->getPosition().x - decreaseValue, hl->getPosition().y));
	}
	
}
void FirstGameScene::draw()
{
}