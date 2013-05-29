//
//  GameScene.m
//  HurdleWorldCup
//
//  Created by ksoo k on 12. 4. 10..
//  Copyright (c) 2012≥‚ __MyCompanyName__. All rights reserved.
//

#include "HWGameScene.h"

#include "KSoundEngine.h"

#include "HWSharingClass.h"
#include "NSDefault.h"
#include "CCDirector.h"

#include "KS_Util.h"
#include "CCObject.h"
#include "KSoundEngine.h"
#include "CCUtil.h"

#include "LoadingScene.h"
#include "HWPlay.h"
#include "HWSaveData.h"
#include "cocos2d.h"

#include "JNIKelper.h"
#include "GameOver.h"
#include "Bridge.h"
#include "PauseLayer.h"
#include "WCPlay.h"
#include "KSoundEngine.h"
#define ONEKILL 0

extern int mygravity;

float GameScene::calSpeed()
{
	float r = 6 + (float)(hwplay->speedCombo * 0.3f);
	//r = MIN(11.f, r);
	return r;
}





void GameScene::setSpeed(float s)
{
	info.runSpeed = s;
}


float GameScene::givenTime()
{
	// _sha->stage
	//	return 2.5f;
	float t = 40.f; //## 40.f
	return t;
	//return 60.f - _sha->stage * ;
}


void GameScene::onEnterTransitionDidFinish()
{
	CCLayer::onEnterTransitionDidFinish();
	

	this->initHurdleMapAndPlayer(); // override
	rootLayer = CCLayer::node();
	rootLayer->setScale(info.bonusScale);
	innerInfo.scaleFromTo.init(1 * info.bonusScale, 1 * info.bonusScale, 0.f);
	innerInfo.positionFromTo.init(0, 0, 0.f);
	interfaceLayer = CCLayer::node();
//	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("gameui.plist");
	{
		CCLabelBMFont* _plusScore = CCLabelBMFont::labelWithString("+333", "hw_bonusfont.fnt");
		this->addChild(_plusScore);
		_plusScore->setIsVisible(false);
	}
	{
		
		CCLabelBMFont* comboViewer = CCLabelBMFont::labelWithString(KS_Util::stringWithFormat("%d"/*"%d COMBO"*/,hwplay->combo.getVar()).c_str(), "hw_combo.fnt");
		comboViewer->setPosition(ccp(player->getPosition().x+70,player->getPosition().y + player->height() + rootLayer->getPosition().y - 5));
		comboViewer->setScale(0.6);
		this->addChild(comboViewer, 12);
		comboViewer->setIsVisible(false);
		//id myAction1 = CCMoveBy::actionWithDuration(0.7, ccp(0,10));
		//id myAction2 = CCFadeOut::actionWithDuration(0.5);
		auto myAction1 = CCScaleTo::actionWithDuration(0.3, 1.0);
		auto myAction2 = CCFadeOut::actionWithDuration(0.3);
		comboViewer->runAction(CCSequence::actions(myAction1,myAction2, CCCallFuncN::actionWithTarget(this, callfuncN_selector(GameScene::finishAnimation)), nil));
	}

	this->cachingPList();

	//this->setScale(0.2);


	hwplay->gameTimer = 0.;

	innerInfo.minJumpInPixel = 58.f;//55//60.f;
	innerInfo.maxJumpInPixel = 9999.f;//55.f;//9999;//130.f;

	innerInfo.reservedJump = HWInnerVar::NONE;
	innerInfo.isTimeover = false;
	innerInfo.isReady = true;

	//crashedHurdle = CCMutableSet::alloc()->init();
	crashedHurdle = new CCSet();
	//clickCategory = new CCMutableDictionary<string, CCString*>();

	innerInfo.isScoring = FALSE;
	//this->setVars();
	//this->setBackground();
	//startTime = hwplay->gameTimer;

	////////////////////////////


	//////////////////////
	

	interfaceLayer->setPosition(ccp(0,0));

	this->addChild(interfaceLayer, 555);
	this->addChild(rootLayer, 6);
	///////////////////////////////////////////////////////////////////////////////////////////////////
	

	this->setIsTouchEnabled(true);
	//info.nowDistance = 0;

	rootLayer->addChild(hl);
	info.humanPosition = 80;
	this->setSpeed(this->calSpeed());
	player->setPosition(ccp(info.humanPosition, hl->getFirstAttitude()));
	//player->setIsRelativeAnchorPoint(true);
	rootLayer->addChild(player, 0);

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//speedUpSprite = CCSprite::spriteWithFile("hw_speedupeffect.png");


	///////////////////////////////////////////////////////////////
	scoreFnt = CCLabelBMFont::labelWithString("0", "uiscore.fnt");
	scoreFnt->setAnchorPoint(ccp(0.5, 1.0f));
	scoreFnt->setPosition(ccp(240, 315.f));
	scoreFnt->setScale(0.6f);
	interfaceLayer->addChild(scoreFnt, 2);
	
	timeFnt = CCLabelBMFont::labelWithString(KS_Util::stringWithFormat("%.0f", givenTime()).c_str(), "time1.fnt");
	interfaceLayer->addChild(timeFnt, 2);
	timeFnt->setPosition(ccp(431,274));
	
	watch1 = CCSprite::spriteWithFile("ui_time1.png");
	watch1->setPosition(ccp(431, 277));
	addChild(watch1, 5);
	auto watchMator = SceneUtil::playAnimation("ui_time2.png", 0.05, 2, 2, 85, 75, true);
	watchMator.second->runAction(watchMator.first);
	watch2 = watchMator.second;
	watch2->setPosition(ccp(431, 277));
	addChild(watch2, 5);
	watch2->setIsVisible(false);
	watch3 = CCSprite::spriteWithFile("ui_time3.png");
	watch3->setPosition(ccp(431, 277));
	addChild(watch3, 5);
	watch3->setIsVisible(false);

		
	CCSprite* fever_back = CCSprite::spriteWithFile("ui_zerobar_back.png");
	fever_back->setPosition(ccp(240, 28));
	interfaceLayer->addChild(fever_back);
	
	feverGauge = CCProgressTimer::progressWithFile("ui_zerobar_front.png");
	feverGauge->setPosition(ccp(240, 28));
	feverGauge->setType(CCProgressTimerType::kCCProgressTimerTypeHorizontalBarLR);
	
	interfaceLayer->addChild(feverGauge);
	
	KSoundEngine::sharedEngine()->playSound("readygo.mp3");
	auto mator = SceneUtil::playAnimation("ready.png", 0.1, 2, 10, 256, 65, false);
	auto action = CCSequence::actions(mator.first, CCCallFuncN::actionWithTarget(this, callfuncN_selector(ThisClassType::finishReady)), 0);
	mator.second->runAction(action);
	addChild(mator.second, 1000);
	mator.second->setPosition(ccp(240, 160));
		
	map<string, string> item2img;
	item2img["time"] = "item_time1_1.png"; item2img["shield"] = "item_shield1_1.png"; item2img["exp"] = "item_exp1_1.png";
	item2img["item1"] = "item_highhurdle1_1.png"; item2img["item2"] = "item_wideview1_1.png";
	
	CCPoint itemPositions1[] = {ccp(240, 120)};
	CCPoint itemPositions2[] = {ccp(240 - 30, 120), ccp(240 + 30, 120)};
	CCPoint itemPositions3[] = {ccp(180, 120), ccp(240, 120), ccp(300, 120)};

	int itemPositionsIndex = 0;
	int items = 0;
	for(auto iter = playInfo->selectedItems.begin(); iter != playInfo->selectedItems.end(); ++iter)
	{
		if(iter->second)
			items++;
	}

	for(auto selectedItemIter = playInfo->selectedItems.begin(); selectedItemIter != playInfo->selectedItems.end(); ++selectedItemIter)
	{
		if(selectedItemIter->second)
		{
			
			CCSprite* it = CCSprite::spriteWithFile( item2img[selectedItemIter->first].c_str() );
			if(items == 1)
				it->setPosition(itemPositions1[itemPositionsIndex]);
			else if(items == 2)
				it->setPosition(itemPositions2[itemPositionsIndex]);
			else if(items == 3)
				it->setPosition(itemPositions3[itemPositionsIndex]);
			addChild(it, 999);
			it->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(1.f), CCFadeOut::actionWithDuration(0.3f),
											  CCCallFuncN::actionWithTarget(this, callfuncN_selector(ThisClassType::deleteSprite)), 0));
			itemPositionsIndex++;
		}
	}
	
	CCSprite* ui_gold = CCSprite::spriteWithFile("ui_gold.png");
	ui_gold->setPosition(ccp(435, 15));
	addChild(ui_gold, 10);
	
	ateGoldFnt = CCLabelBMFont::labelWithString("0","main1.fnt");
	ateGoldFnt->setPosition(ccp(443, 15));
	addChild(ateGoldFnt, 11);
	
	
	CCMenu* _menu = CCMenu::menuWithItems(0);
	addChild(_menu, 4);
	_menu->setPosition(CCPointZero);
	
	pause = CCMenuItemImage::itemFromNormalImage("ui_stop.png", "ui_stop.png",
															 this, menu_selector(ThisClassType::PAUSE));
	_menu->addChild(pause, 2);
	pause->setPosition(ccp(24 + 10, 295 - 10));

	this->schedule(schedule_selector(GameScene::displayStatus), 1.f/60.f);
}
void GameScene::PAUSE(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	HWPauseLayer* pauseLayer = HWPauseLayer::node();
	//pauseLayer->setPosition(ccp(0, -20));
	addChild(pauseLayer, 1000);
	
	CCLog("pause");
}

//

void GameScene::eatCoin(int n, CCPoint from)
{
	NSDefault::setGold(NSDefault::getGold() + n);
	info.ateGoldCount+=n;
	playInfo->__ateCoin = info.ateGoldCount;
	ateGoldFnt->setString(KS_Util::stringWithFormat("%d", info.ateGoldCount).c_str());
	for(int i=0; i<n; i++)
	{
		auto retainAnimation = SceneUtil::playAnimation("coin.png", 0.07f, 6, 6, COIN_WIDTH, COIN_HEIGHT, false);
		addChild(retainAnimation.second, 10);
		retainAnimation.second->setPosition(from);
		ccBezierConfig bc;
		bc.controlPoint_1 = retainAnimation.second->getPosition();
		bc.controlPoint_2 = ccp(200 + rand()%50, 50);
		bc.endPosition = ccp(460, 20);
		auto moveAction = CCSequence::actions(CCDelayTime::actionWithDuration(i * 0.2f), KSFunc(KSoundEngine::sharedEngine()->playSound("se_takegold01.mp3");),
											  CCBezierTo::actionWithDuration(1.2f, bc), 0);
		auto action2 = CCSpawn::actions(retainAnimation.first, moveAction, CCScaleTo::actionWithDuration(0.8f, 0.5f)/*, CCFadeOut::actionWithDuration(1.2f)*/, 0); // 가면서 사라짐.
		auto action3 = CCCallFuncN::actionWithTarget(this, callfuncN_selector(ThisClassType::deleteSprite));
		auto totalAction = CCSequence::actionOneTwo(action2, action3);
		retainAnimation.second->runAction(totalAction);
	}
	
	
}

void GameScene::finishGo(CCNode* n)
{
	n->removeFromParentAndCleanup(true);
	innerInfo.startTime = hwplay->gameTimer;
	this->schedule(schedule_selector(GameScene::gameTest));
	innerInfo.isReady = false;
}
void GameScene::finishReady(CCNode* n)
{
	n->removeFromParentAndCleanup(true);
	auto mator = SceneUtil::playAnimation("go.png", 0.075, 2, 4, 256, 65, false);
	auto action0 = CCSpawn::actions(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.1f), CCScaleTo::actionWithDuration(0.4f, 4.f)), CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.3f), CCFadeOut::actionWithDuration(0.2f)), mator.first, 0);
	auto action = CCSequence::actions(action0, CCCallFuncN::actionWithTarget(this, callfuncN_selector(ThisClassType::finishGo)), 0);
	mator.second->runAction(action);
	addChild(mator.second, 1000);
	mator.second->setPosition(ccp(240, 160));
}

// on "dealloc" you need to release all your retained objects
GameScene::~GameScene()
{
//	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("gameui.plist");

	playInfo->gamePtr = 0;
	// # CCNotificationCenter::defaultCenter()->removeObserver(this);
	crashedHurdle->release();
	//clickCategory->release();
	hl->release();


//	CCArray* children = this->getChildren();
//
//	for(int i=0; i<children->count(); i++)
//	{
//		CCObject* tempNode = children->objectAtIndex(i);
//		if(tempNode != nil && 1<tempNode->retainCount())
//		{
//			while(1<tempNode->retainCount())
//				tempNode->release();
//		}
//	}
//
//	CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
//	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
//	CCTextureCache::sharedTextureCache()->removeAllTextures();
}






void GameScene::onHurdleScore()
{
	if(info.gameMode == HWGameInformation::ZERO)
		eatCoin(2, ccp(info.humanPosition, hl->getPosition().y + hl->BEGIN_HEIGHT()));
	hwplay->combo++;
	if(hwplay->combo.getVar() % 6 == 0)
	{
		CCSprite* d = CCSprite::spriteWithFile("kim4.png");
		d->setScale(0.2f);
		d->runAction(CCScaleTo::actionWithDuration(0.4f, 0.8f));
		
		interfaceLayer->addChild(d, 400);
		d->setPosition(ccp(85, 70));
		
		d->runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.4f), KSFuncN(x, {
			CCSprite* step1 = CCSprite::spriteWithFile("step1.png");
			step1->setPosition(ccp(15, 30));
			x->addChild(step1);
		})));
		
		d->runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.7f), KSFuncN(x, {
			CCSprite* step2 = CCSprite::spriteWithFile("step2.png");
			step2->setPosition(ccp(70, 70));
			x->addChild(step2);
		})));
		
		d->runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(1.3f), CCScaleTo::actionWithDuration(0.4f, 0.01f)));
		
		d->runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(1.9f), KSFuncN(x, {
			x->removeFromParentAndCleanup(true);
		})));
		
		d->setPosition(ccp(85, 70));
	}
	
	bool bShout = false;
	if(info.gameMode == HWGameInformation::ZERO)
	{
		if(hwplay->combo.getVar() % 10 == 0)
		{
			KSoundEngine::sharedEngine()->playSound("shout.mp3");
			bShout = true;
		}
	}
	
	if(bShout == false)
	{
		if(hwplay->combo.getVar() % 5 == 0)
		{
			int r = rand() % 8;
			KSoundEngine::sharedEngine()->playSound(KS_Util::stringWithFormat("p%d.mp3", r).c_str());
		}
	}
//	KSoundEngine::sharedEngine()->playSound("combo.wav");
	
	hl->setBalance(hwplay->combo.getVar());
	
	if(info.gameMode == HWGameInformation::ZERO)
	{
		feverGauge->setPercentage(feverGauge->getPercentage() + HWInnerVar::INC_VALUE_IN_ZERO);
	}
	else
	{
		feverGauge->setPercentage(feverGauge->getPercentage() + HWInnerVar::INC_VALUE_IN_NORMAL);
		if(feverGauge->getPercentage()>=100.f && info.isGameOvering == false)
		{
			info.gameMode = HWGameInformation::ZERO;
			onEnterZeroMode();
		}
	}
	
	auto mator = SceneUtil::playAnimation("game_combo.png", 0.1, COMBO_COLS, COMBO_FRAME, COMBO_WIDTH, COMBO_HEIGHT, false);
	mator.second->runAction(CCSequence::actions(mator.first,
												//CCDelayTime::actionWithDuration(0.5f),
												CCCallFuncN::actionWithTarget(this, callfuncN_selector(ThisClassType::deleteSprite)), 0));
	mator.second->setPosition(ccp(85, 50));

	interfaceLayer->addChild(mator.second, 2);
	
	CCLabelBMFont* comboFnt = CCLabelBMFont::labelWithString(KS_Util::stringWithFormat("%d", hwplay->combo.getVar()).c_str(), "gamecombo.fnt");
	comboFnt->setPosition(ccp(85, 70));
	comboFnt->setScale(0.3f);
	comboFnt->runAction(CCSequence::actions(CCEaseBackOut::actionWithAction(CCScaleTo::actionWithDuration(0.3, 1.f)),
											CCDelayTime::actionWithDuration(0.5f), CCFadeOut::actionWithDuration(0.5f),
											CCCallFuncN::actionWithTarget(this, callfuncN_selector(ThisClassType::deleteSprite)), 0));
	interfaceLayer->addChild(comboFnt, 2);
	
	int addValue = score(hwplay->combo.getVar(), 0);
	hwplay->gameScore += (addValue);
	wcplay->gameScore = hwplay->gameScore.getVar();
	
	CCLog("%d -----", hwplay->gameScore.getVar());
	CCLabelBMFont* bonusFnt = CCLabelBMFont::labelWithString(KS_Util::stringWithFormat("+%d", addValue).c_str(), "bonusscore.fnt");
	bonusFnt->setPosition(ccp(scoreFnt->getPosition().x + scoreFnt->getContentSize().width / 3.f,
							  scoreFnt->getPosition().y - scoreFnt->getContentSize().height / 3.f));
	interfaceLayer->addChild(bonusFnt, 2);
	float virtualMaxScore = MAX(100000, HWSaveData::getMaxScore());
	float goalScale = MIN(1.3f, 0.6f + (float)hwplay->gameScore.getVar() * 2.f / (float)virtualMaxScore);
	bonusFnt->setScale(0.3f);
	auto action0 = CCEaseBackOut::actionWithAction(CCScaleTo::actionWithDuration(1.f, goalScale));
	auto action1 = CCDelayTime::actionWithDuration(0.2f);
	auto action2 = CCFadeOut::actionWithDuration(0.2f);
	auto action3 = CCCallFuncN::actionWithTarget(this, callfuncN_selector(GameScene::deleteSprite));
	auto totalAction = CCSequence::actions(action0, action1, action2, action3, 0);
	bonusFnt->runAction(totalAction);
	
	
	scoreFnt->runAction(CCEaseBackOut::actionWithAction(CCScaleTo::actionWithDuration(1.f, goalScale)));
	

}

void GameScene::onEnterZeroMode()
{
	KSoundEngine::sharedEngine()->playSound("RetroSpace.mp3");
//	KSoundEngine::sharedEngine()->stopBackground();
//	KSoundEngine::sharedEngine()->play
	
	hwplay->speedCombo = 13.f;
	info.runSpeed2.init(info.runSpeed2.getValue(), calSpeed(), 3.f);
	player->fasterRun();
	feelSpeed(hwplay->speedCombo);
	
	player->showBurst();
	zeroBack = CCSprite::spriteWithFile("hw_zero.png");
	zeroBack->setScale(2.f);
	zeroBack->setAnchorPoint(ccp(0, 0));
	zeroBack->setPosition(ccp(500, 0));
	zeroBack->runAction(CCMoveTo::actionWithDuration(1.f, ccp(-140, 0)));
	addChild(zeroBack, 3);
	watch2->pauseSchedulerAndActions();
	for(int i=0; i<hl->getObstacles()->count(); i++)
	{
		Obstacle* ob = hl->getObstacles()->getObjectAtIndex(i);
		
		if(-hl->getPosition().x/* + info.humanPosition*/ <= ob->getStart() + ob->getLength() * hl->getPatternWidth())// && -hl->getPosition().x  > hl->getPatternWidth() + ob->getStart())
			//(ob->getStart() <= -hl->getPosition().x && -hl->getPosition().x <= ob->getStart() + ob->getLength() * hl->getPatternWidth()))
		{
			//ob->getHurdles()
			for(int j = 0; j<ob->getHurdles()->count(); j++)
			{
				Hurdle* h = ob->getHurdles()->getObjectAtIndex(j);
				if(h->getHurdleSprite())
				{
					h->getHurdleSprite()->runAction(CCSequence::actions(
																		CCSpawn::actions(CCRotateBy::actionWithDuration(0.3, 70.f), CCMoveBy::actionWithDuration(0.3, ccp(400, 400)), nil),
																		//															CCSpawn::actions(CCMoveBy::actionWithDuration(0.3, ccp(200,350.f)), CCRotateBy::actionWithDuration(0.3, 20.f), nil),
																		nil));

				}
				crashedHurdle->addObject(h);
			}
			break;			
		}
	}
	KSoundEngine::sharedEngine()->playSound("fever.mp3");
}

void GameScene::onExitZeroMode()
{
//	KSoundEngine::sharedEngine()->playSound("RetroSpace.mp3");
	KSoundEngine::sharedEngine()->playSound("hotshots.mp3");
	
	KSoundEngine::sharedEngine()->playSound("exitzero.mp3");
	player->hideBurst();
	zeroBack->runAction(CCMoveTo::actionWithDuration(1.f, ccp(-500 - 240, 0)));


	
	hwplay->speedCombo = 0.f;
	info.runStep = 1;
	innerInfo.beforeRunStep = 1;
	innerInfo.timeAfterZero = 0.f;
	info.runSpeed2.init(info.runSpeed2.getValue(), calSpeed(), 5.f);
	feelSpeed(0);
	player->normalRun();
	feverGauge->setPercentage(0);
	watch2->resumeSchedulerAndActions();
}
void GameScene::onGroundScore(bool perfect)
{
	bool trackPerfectJump = perfect;
	//hwplay->speedCombo++;
	//feelSpeed(hwplay->speedCombo);
	
	if(info.gameMode == HWGameInformation::ZERO)
	{
		feverGauge->setPercentage(feverGauge->getPercentage() + HWInnerVar::INC_VALUE_IN_ZERO);
	}
	else
	{
		feverGauge->setPercentage(feverGauge->getPercentage() + HWInnerVar::INC_VALUE_IN_NORMAL);
		if(feverGauge->getPercentage()>=100.f && info.isGameOvering == false)
		{
			info.gameMode = HWGameInformation::ZERO;
			onEnterZeroMode();
		}
	}
	
	if(info.gameMode == HWGameInformation::ZERO)
		eatCoin(4, ccp(info.humanPosition, hl->getPosition().y + hl->BEGIN_HEIGHT()));
	hwplay->combo++;
	CCLog("%d", hwplay->combo.getVar());
	if(hwplay->combo.getVar() % 6 == 0)
	{
		CCSprite* d = CCSprite::spriteWithFile("kim4.png");
		d->setScale(0.2f);
		d->runAction(CCScaleTo::actionWithDuration(0.4f, 0.8f));
		
		interfaceLayer->addChild(d, 400);
		d->setPosition(ccp(85, 70));
		
		d->runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.4f), KSFuncN(x, {
			CCSprite* step1 = CCSprite::spriteWithFile("step1.png");
			step1->setPosition(ccp(15, 30));
			x->addChild(step1);
		})));
		
		d->runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.7f), KSFuncN(x, {
			CCSprite* step2 = CCSprite::spriteWithFile("step2.png");
			step2->setPosition(ccp(70, 70));
			x->addChild(step2);
		})));
		
		d->runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(1.3f), CCScaleTo::actionWithDuration(0.4f, 0.01f)));
		
		d->runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(1.9f), KSFuncN(x, {
			x->removeFromParentAndCleanup(true);
		})));

		d->setPosition(ccp(85, 70));
	}
	
	bool bShout = false;
	if(info.gameMode == HWGameInformation::ZERO)
	{
		if(hwplay->combo.getVar() % 10 == 0)
		{
			KSoundEngine::sharedEngine()->playSound("shout.mp3");
			bShout = true;
		}
	}
	
	if(bShout == false)
	{
		if(hwplay->combo.getVar() % 5 == 0)
		{
			int r = rand() % 8;
			KSoundEngine::sharedEngine()->playSound(KS_Util::stringWithFormat("p%d.mp3", r).c_str());
		}
	}

	hl->setBalance(hwplay->combo.getVar());
	int addValue = score(hwplay->combo.getVar(), 0);
	
	hwplay->gameScore += (addValue);
	wcplay->gameScore = hwplay->gameScore.getVar();
	CCLabelBMFont* bonusFnt = CCLabelBMFont::labelWithString(KS_Util::stringWithFormat("+%d", addValue).c_str(), "bonusscore.fnt");
	bonusFnt->setPosition(ccp(scoreFnt->getPosition().x + scoreFnt->getContentSize().width / 3.f,
							  scoreFnt->getPosition().y - scoreFnt->getContentSize().height / 3.f));
	interfaceLayer->addChild(bonusFnt, 2);
	float virtualMaxScore = MAX(100000, HWSaveData::getMaxScore());
	float goalScale = MIN(1.3f, 0.6f + (float)hwplay->gameScore.getVar() * 2.f / (float)virtualMaxScore);
	bonusFnt->setScale(0.3f);
	auto action0 = CCEaseBackOut::actionWithAction(CCScaleTo::actionWithDuration(1.f, goalScale));
	auto action1 = CCDelayTime::actionWithDuration(0.2f);
	auto action2 = CCFadeOut::actionWithDuration(0.2f);
	auto action3 = CCCallFuncN::actionWithTarget(this, callfuncN_selector(GameScene::deleteSprite));
	auto totalAction = CCSequence::actions(action0, action1, action2, action3, 0);
	bonusFnt->runAction(totalAction);
	
	
	scoreFnt->runAction(CCEaseBackOut::actionWithAction(CCScaleTo::actionWithDuration(1.f, goalScale)));
	
	//scoreFnt->runAction(CCEaseBackOut::actionWithAction(CCScaleTo::actionWithDuration(1.f, 1.0f + HWSaveData::getMaxScore() / hwplay->gameScore.getVar())));
	
	
	if(hwplay->combo.getVar() >= 2) //##
	{
		
	}
	
	
	auto mator = SceneUtil::playAnimation("game_combo.png", 0.1, COMBO_COLS, COMBO_FRAME, COMBO_WIDTH, COMBO_HEIGHT, false);
	mator.second->runAction(CCSequence::actions(mator.first,
												//CCDelayTime::actionWithDuration(0.5f),
												CCCallFuncN::actionWithTarget(this, callfuncN_selector(ThisClassType::deleteSprite)), 0));
	mator.second->setPosition(ccp(85, 50));
	mator.second->setScale(0.75f);
	interfaceLayer->addChild(mator.second, 2);
	
	CCLabelBMFont* comboFnt = CCLabelBMFont::labelWithString(KS_Util::stringWithFormat("%d", hwplay->combo.getVar()).c_str(), "gamecombo.fnt");
	comboFnt->setPosition(ccp(85, 70));
	comboFnt->setScale(0.3f);
	comboFnt->runAction(CCSequence::actions(CCEaseBackOut::actionWithAction(CCScaleTo::actionWithDuration(0.3, 1.f)),
											CCDelayTime::actionWithDuration(0.5f), CCFadeOut::actionWithDuration(0.5f),
											CCCallFuncN::actionWithTarget(this, callfuncN_selector(ThisClassType::deleteSprite)), 0));
	interfaceLayer->addChild(comboFnt, 2);
//	else
//	{
//		hwplay->gameScore += this->scoreAdder(150);
//		plusScore->setString("+150");
//	}
}
void GameScene::crashOnGround(Obstacle* t)
{
	bool trackPerfectJump = FALSE;
	if(info.humanPosition - hl->getPosition().x - t->start <= 25) // Perfect Jump Condition //##
	{
		trackPerfectJump = TRUE;
	}
	player->setPosition(ccp(player->getPosition().x,t->height));
	
	if(innerInfo.isScoring && !(player->STATE & Player1::REVIVE) && info.isGameOvering == false)
	{
		onHurdleScore();
	}
	
	if(innerInfo.prevGround != t && t->start != 0 && !(player->STATE & Player1::REVIVE) && info.isGameOvering == false)
	{
		onGroundScore(trackPerfectJump); // 안쪽에서 feelspeed(setscale 함)
	}

	//////////////////////////////////////////////
	
	innerInfo.isScoring = false;
	
	float goalHillHeight = (hl->BEGIN_HEIGHT() - t->height);
	innerInfo.positionFromTo.init(innerInfo.positionFromTo.getValue(), goalHillHeight*rootLayer->getScale(), 0.6f);
	//rootLayer->setPosition(ccp(rootLayer->getPosition().x, innerInfo.goalHillHeight*rootLayer->getScale())); // original : 0.6
	player->startRun(0.05 - (info.runSpeed - 4)*0.03/4.0);
	CCLog("run!!");

	player->STATE &= ~Player1::UNJI;
	player->STATE &= ~Player1::JUMPING;
	player->STATE &= ~Player1::REVIVE;
	player->STATE |= Player1::NORMAL;
//	speedUpSprite->setIsVisible(true);
	if(innerInfo.reservedJump == HWInnerVar::RESJUMP)
	{
		innerInfo.noTouchBeforeResJump = !innerInfo.touching;
		innerInfo.resJumpTime = hwplay->gameTimer;
		defaultJump();
		innerInfo.reservedJump = HWInnerVar::NONE;
	}
	innerInfo.prevGround = t; // ¡°ºˆ∏¶ ¡÷±‚¿ß«— ¿Ã¿¸∂•¿ª ¿˙¿Â
	
	
}

void GameScene::feelSpeed(int speedCombo)
{
	//if(speedCombo <= 13)
	{
		int vsc = speedCombo >= 13 ? 13 : 0;
		CCLog("runstep = %f", info.runStep);
		
		innerInfo.scaleFromTo.init(innerInfo.scaleFromTo.getValue(), MIN(0.96f, 1 - 0.02f * vsc + (info.runStep-1) * 0.03f)* info.bonusScale, 2.f);
		CCLog("scale = %f", 1 - 0.02f * speedCombo + info.runStep * 0.03f);
		//CCPoint yy = rootLayer->getAnchorPoint();
		rootLayer->setAnchorPoint(ccp(info.humanPosition / 480.f, hl->BEGIN_HEIGHT() / 320.f));
	}
//	if(speedCombo <= 12)
//		rootLayer->setScale(1 - 0.03f * speedCombo);
		//rootLayer->runAction(CCScaleTo::actionWithDuration(0.2, 1 - 0.03f * speedCombo));
}
void GameScene::resJumpCheck()
{
	const float minPressJumpTime = 4.f/60.f;
	if(innerInfo.noTouchBeforeResJump /*&& innerInfo.reservedJump == HWInnerVar::RESJUMP*/ && hwplay->gameTimer - innerInfo.resJumpTime >= minPressJumpTime)
	{
		innerInfo.noTouchBeforeResJump = false;
		//innerInfo.reservedJump = HWInnerVar::NONE;

		if(innerInfo.yVelocity > 0)
			innerInfo.yVelocity /= 3.f;
	}
}
void GameScene::crashOnHurdle(Hurdle* h)
{
	//info.speedCombo = 0;
	//feelSpeed(info.speedCombo);
	if(info.gameMode == HWGameInformation::ZERO && innerInfo.timeAfterZero < 7.f)
	{
		innerInfo.isScoring = false;
		h->getHurdleSprite()->runAction(CCSequence::actions(
															CCSpawn::actions(CCRotateBy::actionWithDuration(0.3, 70.f), CCMoveBy::actionWithDuration(0.3, ccp(400, 350)), nil),
//															CCSpawn::actions(CCMoveBy::actionWithDuration(0.3, ccp(200,350.f)), CCRotateBy::actionWithDuration(0.3, 20.f), nil),
															nil));
		crashedHurdle->addObject(h);
		return;
	}
	innerInfo.isScoring = false;
	h->getHurdleSprite()->setAnchorPoint(ccp(0.5,0));

	h->getHurdleSprite()->runAction(CCSequence::actions(
		CCSpawn::actions(CCRotateBy::actionWithDuration(0.3, 70.f), CCMoveBy::actionWithDuration(0.3, ccp(130,20)), nil),
		CCSpawn::actions(CCMoveBy::actionWithDuration(0.3, ccp(20,-20.f)), CCRotateBy::actionWithDuration(0.3, 20.f), nil),
		nil));
	crashedHurdle->addObject(h);
	
	CCMoveBy* move1 = CCMoveBy::actionWithDuration(0.05f, CCPointMake(-4,3));
	CCMoveBy* move2 = CCMoveBy::actionWithDuration(0.05f, CCPointMake(3,-2));
	CCMoveBy* move3 = CCMoveBy::actionWithDuration(0.05f, CCPointMake(-3,2));
	CCMoveBy* move4 = CCMoveBy::actionWithDuration(0.05f, CCPointMake(3,-4));
	CCMoveBy* move5 = CCMoveBy::actionWithDuration(0.05f, CCPointMake(-4,2));

	CCMoveBy* move6 = CCMoveBy::actionWithDuration(0.05f, CCPointMake(3,2));
	CCMoveBy* move7 = CCMoveBy::actionWithDuration(0.05f, CCPointMake(-4,-2));
	CCMoveBy* move8 = CCMoveBy::actionWithDuration(0.05f, CCPointMake(5,2));
	CCMoveBy* move9 = CCMoveBy::actionWithDuration(0.05f, CCPointMake(-2,-4));
	CCMoveBy* move10 = CCMoveBy::actionWithDuration(0.05f, CCPointMake(5,3));
	CCMoveTo* moveBack = CCMoveTo::actionWithDuration(0.05f, CCPointMake(0,0));

	auto shakeWorldSeq = CCSequence::actions(move1,move2,move3,move4,move5,move6,move7,move8,move9,move10,moveBack,nil); 
	this->runAction(shakeWorldSeq);

//	KSoundEngine::sharedEngine()->playSound("crashonhurdle.mp3");
	
	bool shieldMode = false;
	if(playInfo->selectedItems["shield"] == true && info.usedShield == false)
	{
		KSoundEngine::sharedEngine()->playSound("shield.mp3");
		info.usedShield = true;
		shieldMode = true;
		
		CCSprite* shield;
		auto shieldMator = SceneUtil::playAnimation("shield.png", 0.07, 4, 8, 160, 160, 2);
		shield = shieldMator.second;
		shield->runAction(CCSequence::actionOneTwo(shieldMator.first, CCCallFuncN::actionWithTarget(this, callfuncN_selector(ThisClassType::deleteSprite))));
		shield->setPosition(ccp(240, 200));
		addChild(shield, 10000);
		//		shield->runAction(CCSequence::actions(CCBlink::actionWithDuration(0.8, 3.f), CCDelayTime::actionWithDuration(2.f), CCFadeOut::actionWithDuration(0.5f),
//											  CCCallFuncN::actionWithTarget(this, callfuncN_selector(ThisClassType::deleteSprite)), 0));
	}
	if(shieldMode == false)
	{
		if(info.gameMode == HWGameInformation::ZERO)
		{
			feverGauge->setPercentage(feverGauge->getPercentage() + HWInnerVar::CRASH_DEC_VALUE_IN_ZERO);
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
	if(shieldMode == false)
	{
		hwplay->combo = 0;
		KSoundEngine::sharedEngine()->playSound("uhuh.mp3");
		KSoundEngine::sharedEngine()->playSound("crashonhurdle.mp3");
	}
	hl->setBalance(hwplay->combo.getVar());

	player->STATE |= Player1::SUPER;
	player->blink();
	//## 깜빡이는 캐릭
	
}
void GameScene::crashCheck()
{
	float realPosition = info.humanPosition - hl->getPosition().x;
	bool _unji = TRUE;
	for(int i=0; i<hl->getObstacles()->count(); i++)
	{
		Obstacle* t = hl->getObstacles()->getObjectAtIndex(i);
		CCPoint A = CCPointMake(t->start - player->width()*2,t->height);
		CCPoint B = CCPointMake(t->start + t->length * hl->getPatternWidth() + player->width(), t->height);
		CCPoint C = CCPointMake(info.humanPosition - hl->getPrevPosition().x,player->getPrevPosition());
		CCPoint D = CCPointMake(info.humanPosition - hl->getPosition().x,player->getPosition().y + player->jumpBenefic());
		player->setPrevJumpBenefic(player->jumpBenefic());

		//CCLog("height = %d, prevY = %f, cur = %f", t->height, player->getPrevPosition(), (float)(player->getPosition().y + player->jumpBenefic()));
		if(CCUtil::isIntersect(A, B, C, D) && C.y >= D.y)
		{	
			_unji = FALSE;
			if (player->STATE & Player1::JUMPING || player->STATE & Player1::UNJI)
			{
				CCLog("CRASH");
				this->crashOnGround(t);
			}
			
		}
		//t->hurdles
		// 허들 충돌체크
		for(int i=0; i<t->getHurdles()->count(); i++)
		{
			Hurdle* h = t->getHurdles()->getObjectAtIndex(i);
			if(crashedHurdle->containsObject(h)) // 이미 부딪힌 허들은 패스
				continue;
			float halfwidth = h->HurdleWidth()/2.f;
			if(max(realPosition - player->width()/2.f,h->position - halfwidth) <= min(realPosition + player->width()/2.f,h->position + halfwidth)) // x √Êµπ
			{
				innerInfo.isScoring = true;
				int hurdleheight = t->height + h->HurdleHeight();

				if((player->STATE & Player1::NORMAL || player->STATE & Player1::JUMPING) && (player->STATE & Player1::SUPER) == 0)
				{
					float AABB1 = t->height;
					float AABB2 = player->getPosition().y + player->jumpBenefic();
					float AABB3 = hurdleheight;
					float AABB4 = player->getPosition().y + player->height() + player->jumpBenefic();
					if(max(AABB1,AABB2) <= min(AABB3,AABB4))
					{
						this->crashOnHurdle(h);
					}
				}
			}
		}
	}
	player->setPrevPosition(player->getPosition().y + player->jumpBenefic());
	if(_unji && (player->STATE & Player1::NORMAL ) && (player->STATE & Player1::UNJI) == 0)
	{
		player->STATE &= ~Player1::NORMAL;
		player->STATE |= Player1::UNJI;
		onUnji();
	}
}
void GameScene::translation(float dt)
{
	innerInfo.scaleFromTo.step(dt);
	innerInfo.positionFromTo.step(dt);
	rootLayer->setScale(innerInfo.scaleFromTo.getValue());
	//player->burstParticle->setScale(innerInfo.scaleFromTo.getValue() * 0.6f); //##


//	player->burstParticle->cocos2d::CCParticleSystem::setStartSize(150 * innerInfo.scaleFromTo.getValue());
//	player->burstParticle->cocos2d::CCParticleSystem::setEndSize(250 * innerInfo.scaleFromTo.getValue());
	rootLayer->setPosition(rootLayer->getPosition().x, innerInfo.positionFromTo.getValue()); //##
}
void GameScene::moveMan(ccTime dt)
{
	if((player->STATE & Player1::NORMAL) == 0)
	{
		innerInfo.yVelocity += HWInnerVar::hurdleG * dt;
		if(innerInfo.yVelocity < 0 && !innerInfo.beginDrop)
		{
			innerInfo.beginDrop = true;
			player->decreaseManAfterJump();
		}
		
		player->moveMan(innerInfo.yVelocity*dt);
	}
}
void GameScene::onUnji()
{
	{
		//player->unjiWithUnjiTime(hwplay->gameTimer, 0);
		innerInfo.yVelocity = 0;
		innerInfo.beginDrop = true;
//		speedUpSprite->setIsVisible(false);
	}

}
void GameScene::registerWithTouchDispatcher()
{
	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, false);
}

bool GameScene::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	if(innerInfo.isReady == true)
		return true;
	
	innerInfo.touching = true;
	if(player->STATE & Player1::REVIVE)
		return true;
	if((player->STATE & Player1::UNJI || player->STATE & Player1::JUMPING ) && (player->STATE & Player1::REVIVE) == 0)
	{
		innerInfo.reservedJump = HWInnerVar::RESJUMP; // 예약 점프 기능, 해제 하려면 주석해제
	}
	else {
		
	}
	if(player->STATE & Player1::NORMAL)
	{
		innerInfo.resJumpTime = hwplay->gameTimer;
		defaultJump();
	}

	return true;
}
void GameScene::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	innerInfo.touching = false;
	if(player->STATE & Player1::REVIVE)
		return;
	
	const float minPressJumpTime = 4.f/60.f;
	if(hwplay->gameTimer - innerInfo.resJumpTime < minPressJumpTime)
	{
		innerInfo.noTouchBeforeResJump = true;
//		if(innerInfo.yVelocity > 0)
//			innerInfo.yVelocity /= 3.f;
	}
	else
	{
		if(innerInfo.yVelocity > 0)
			 innerInfo.yVelocity /= 3.f;
	}
		
	
}
void GameScene::defaultJump()
{
	player->STATE &= ~Player1::NORMAL;
	player->STATE |= Player1::JUMPING;
	player->setFrame(HurdleMan::JUMP, 0);
	innerInfo.yVelocity = HWInnerVar::leapVelocity;
	innerInfo.beginDrop = false;
	
	KSoundEngine::sharedEngine()->playSound("normaljump.mp3");
	for(int i=0; i<hl->getObstacles()->count(); i++)
	{
		Obstacle* ob = hl->getObstacles()->getObjectAtIndex(i);
		if( (ob->getStart() <= -hl->getPosition().x + info.humanPosition && -hl->getPosition().x + info.humanPosition <= ob->getStart() + ob->getLength() * hl->getPatternWidth()))
		{
			auto landAnimator = SceneUtil::playAnimation("hw_landing.png", 0.05, 4, 4, 90, 35, false);
			landAnimator.second->setAnchorPoint(ccp(0.5, 0));
			landAnimator.second->setPosition(ccp(-hl->getPosition().x + info.humanPosition, ob->height));
			landAnimator.second->runAction(CCSequence::actions(landAnimator.first, CCDelayTime::actionWithDuration(0.3f),
																	CCCallFuncN::actionWithTarget(this, callfuncN_selector(ThisClassType::deleteSprite)), 0));
			hl->addChild(landAnimator.second, 10);
			break;
		}
	}

}

void GameScene::onExit()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	//JNIKelper::getInstance()->callJava_simple("refreshAD"); // #
#endif
	CCLayer::onExit();
}


void GameScene::adjustRunSpeed(ccTime dt)
{
	info.runSpeed2.step(dt);
	//CCLog("%f ---- run ", info.runSpeed2.getValue());
	this->setSpeed(info.runSpeed2.getValue());
}

void GameScene::displayStatus(ccTime dt)
{
	int goalScore = hwplay->gameScore.getVar();
	int displayedScore = atoi(scoreFnt->getString());
	if(displayedScore < goalScore )
	{
		int adderValue = (goalScore - displayedScore) / 30 + 21;
		adderValue = MIN(adderValue, goalScore - displayedScore);
		scoreFnt->setString(KS_Util::stringWithFormat("%d", displayedScore + adderValue).c_str());
	}

	
	
	if(innerInfo.isReady == true)
	{
		timeFnt->setString(KS_Util::stringWithFormat("%.0f", BASETIME).c_str());
	}
	else {
		if(info.remainTime >= 0)
			timeFnt->setString(KS_Util::stringWithFormat("%.0f",info.remainTime * BASETIME / (float)info.INIT_GAME_TIME).c_str());
		else
			timeFnt->setString("0");
	}	
}



void GameScene::finishAnimation(CCNode* sender)
{
	CCSprite* s = (CCSprite*)sender;

	s->getParent()->removeChild(s, true);
}


void GameScene::moveMap(ccTime dt)
{
	float movedis = fabsf(hl->getPosition().x);
}


bool GameScene::init()
{
	CCLayer::init();
	KSoundEngine::sharedEngine()->playSound("hotshots.mp3");
//	KSoundEngine::sharedEngine()->playSound("beat area.mp3");
//	KSoundEngine::sharedEngine()->playSound("Block Buster.mp3");
//	KSoundEngine::sharedEngine()->playSound("Monster Truck.mp3");

	auto t1 = SceneUtil::playAnimation("game_combo.png", 0.1, COMBO_COLS, COMBO_FRAME, COMBO_WIDTH, COMBO_HEIGHT, false); // texture 로딩. 바로 해제될 녀석.
	auto t2 = CCLabelBMFont::labelWithString("00", "gamecombo.fnt"); // texture 로딩. 바로 해제될 녀석.
	t1.second->setPosition(ccp(-200, -200));
	t1.second->runAction(CCSequence::actionOneTwo(t1.first, CCCallFuncN::actionWithTarget(this, callfuncN_selector(ThisClassType::deleteSprite))));
	addChild(t1.second);
	addChild(t2); removeChild(t2, true);
	return true;
}