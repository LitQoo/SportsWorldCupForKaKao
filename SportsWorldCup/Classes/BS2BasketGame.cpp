//
//  BasketGame.cpp
//  BasketWorldCup2
//
//  Created by ksoo k on 12. 10. 31..
//  Copyright (c) 2012년 LitQoo Inc. All rights reserved.
//
#include "BS2BasketGame.h"
#include "BS2Ring.h"
#include "KS_Util.h"
#include "GameSystem.h"
#include "NSDefault.h"
#include "LoadingScene.h"

#include "KSModalAlert.h"

#include "KSoundEngine.h"
#include "BS2Play.h"
#include "BS2SaveData.h"
#include "PauseLayer.h"
#include "KSoundEngine.h"
BasketGame::BasketGame()
{
	playInfo->gamePtr = this;
	markCount = 0;
	KS::KSLog("%, %", __FILE__, __LINE__);
	ItemType items[] = {MOVE_BALL, GUIDE_LINE,
		DOUBLE_SHOT, TIME_PLUS, LOT};
	
	moveSprite = doubleSprite = 0;

//	
//	pair<GameScreen::ItemType, int> items2[] = {
//		pair<GameScreen::ItemType, int>(GameScreen::MOVE_BALL, MOVE_PRICE),
//		pair<GameScreen::ItemType, int>(GameScreen::GUIDE_LINE, LONGGUIDE_PRICE),
//		pair<GameScreen::ItemType, int>(GameScreen::DOUBLE_SHOT, DOUBLE_PRICE),
//		pair<GameScreen::ItemType, int>(GameScreen::BALL_PLUS, ONEPLUS_PRICE),
//		pair<GameScreen::ItemType, int>(GameScreen::TIME_PLUS, TIMEPLUS_PRICE),
//		pair<GameScreen::ItemType, int>(GameScreen::LOT, MOVE_PRICE),
//	
//	};
	seq.assign(&items[0], &items[sizeof(items) / sizeof(ItemType)]);
	KS::KSLog("%, %", __FILE__, __LINE__);
}
BasketGame::~BasketGame()
{
	CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
	playInfo->gamePtr = 0;
//	CCArray* children = this->getChildren();
//	
//	for(int i=0; i<children->count(); i++)
//	{
//		CCObject* tempNode = children->objectAtIndex(i);
//		if(tempNode != 0 && 1<tempNode->retainCount())
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
bool BasketGame::init()
{
	KS::KSLog("%, %", __FILE__, __LINE__);
	CCLayerColor::initWithColor(ccc4f(255, 255, 0, 255));
	KSoundEngine::sharedEngine()->playSound("hotshots.mp3");
//	KSoundEngine::sharedEngine()->playSound("beat area.mp3");
//	KSoundEngine::sharedEngine()->playSound("Block Buster.mp3");
//	KSoundEngine::sharedEngine()->playSound("Monster Truck.mp3");

	
	
	tipInableSprite = CCSprite::spriteWithFile("item_tipin1_1.png");
	tipInableSprite->setPosition(ccp(TIPIN_X, TIPIN_Y));
	tipInableSprite->setIsVisible(false);
	addChild(tipInableSprite, 2);
	auto t1 = SceneUtil::playAnimation("game_combo.png", 0.1, COMBO_COLS, COMBO_FRAME, COMBO_WIDTH, COMBO_HEIGHT, false); // texture 로딩. 바로 해제될 녀석.
	auto t2 = CCLabelBMFont::labelWithString("00", "gamecombo.fnt"); // texture 로딩. 바로 해제될 녀석.
	t1.second->setPosition(ccp(-200, -200));
	t1.second->runAction(CCSequence::actionOneTwo(t1.first, CCCallFuncN::actionWithTarget(this, callfuncN_selector(ThisClassType::deleteSprite))));
	addChild(t1.second);
	
	addChild(t2); removeChild(t2, true);
	int remainder;
	if(NSDefault::getIsReview()) //
		remainder = BACKGROUND_NUMBER;
	else
		remainder = 1;
	bs2play->backNumber = rand() % remainder + 1;
	switch (bs2play->backNumber) {
		case 1:
			setColor(ccc3(147, 217, 236));
			break;
		case 2:
			setColor(ccc3(6, 22, 51));
			break;
		case 3:
			setColor(ccc3(205, 245, 245));
			break;
		case 4:
			setColor(ccc3(2, 35, 87));
			break;
		case 5:
			setColor(ccc3(116, 163, 201));
			break;
		case 6:
			setColor(ccc3(6, 21, 52));
			break;
		case 7:
			setColor(ccc3(204, 245, 255));
			break;
		case 8:
			setColor(ccc3(0, 33, 88));
			break;
		case 9:
			setColor(ccc3(114, 162, 203));
			break;
		case 10:
			setColor(ccc3(144, 217, 237));
			break;
		default:
			break;
	}
//	CCSprite* _pattern = CCSprite::spriteWithFile("back_patten.png");
//	_pattern->setPosition(ccp(240, 160));
//	addChild(_pattern, 0);

	//gameScreen = PinchZoomLayer::node();

	KS::KSLog("%, %", __FILE__, __LINE__);
	gameScreen = GameScreen::node();
	gameScreen->setBasketGame(this);
	
	//gameScreen->setContentSize(CCSizeMake(960, 640));
	//gameScreen->setScale(4.f);
	//gameScreen->setPosition(ccpMult(ccp(480/8, 320/8), 4));
	addChild(gameScreen);
	
	gameScreen->setIsTouchEnabled(false);
//	cancelCatchSprite = CCSprite::spriteWithFile("cancelcatch.png");
//	cancelCatchSprite->setOpacity(0);
//	cancelCatchSprite->setPosition(ccp(50, 280));
//	addChild(cancelCatchSprite, 1);
	
	
//	CCSpriteBatchNode* readyBatch = CCSpriteBatchNode::batchNodeWithFile("ready.png");
//	addChild(readyBatch);
//	auto animator = playAnimation(readyBatch, 0.07f, 3, 15, 165, 55, false);
	
	KS::KSLog("%, %", __FILE__, __LINE__);
	
//	interface = CCSprite::spriteWithFile("gui.png");
//	interface->setAnchorPoint(ccp(0, 1));
//	interface->setPosition(ccp(0, 320));
//	addChild(interface, 1);
	
	KS::KSLog("%, %", __FILE__, __LINE__);
	
	

	

	KS::KSLog("%, %", __FILE__, __LINE__);
	//CCLabelBMFont* remainBallFont, *scoreFont, *timeFont, *coinFont;
	
	
	watch1 = CCSprite::spriteWithFile("ui_time1.png");
	watch1->setPosition(ccp(431, 277));
	addChild(watch1, 1);
	auto watchMator = SceneUtil::playAnimation("ui_time2.png", 0.05, 2, 2, 85, 75, true);
	watchMator.second->runAction(watchMator.first);
	watch2 = watchMator.second;
	watch2->setPosition(ccp(431, 277));
	addChild(watch2, 1);
	watch2->setIsVisible(false);
	watch3 = CCSprite::spriteWithFile("ui_time3.png");
	watch3->setPosition(ccp(431, 277));
	addChild(watch3, 1);
	watch3->setIsVisible(false);
	
	timeFnt = CCLabelBMFont::labelWithString("        ", "time1.fnt");
	addChild(timeFnt, 2);
	timeFnt->setPosition(ccp(431,274));
	
	scoreFnt = CCLabelBMFont::labelWithString("0", "uiscore.fnt");
	scoreFnt->setAnchorPoint(ccp(0.0, 0.f));
	scoreFnt->setPosition(ccp(50, 4.f));
	scoreFnt->setScale(0.6f);
	addChild(scoreFnt, 2);

	//timeFont->setPosition(ccp(0, 0));
	
		
	CCMenu* _menu = CCMenu::menuWithItems(0);
	addChild(_menu);
	_menu->setPosition(ccp(0, 0));

	CCPoint pos[] = {CCPoint(157, 119), CCPoint(234, 119), CCPoint(311, 119)};
	int ii = 0;
	bool isUseItem = false;
	
	
	if(isUseItem)
	{
		KSoundEngine::sharedEngine()->playSound("se_applyitem.mp3");
	}
	
	CCSprite* fever_back = CCSprite::spriteWithFile("ui_zerobar_back.png");
	fever_back->setPosition(ccp(240, 298));
	addChild(fever_back);
	
	feverGauge = CCProgressTimer::progressWithFile("ui_zerobar_front.png");
	feverGauge->setPosition(ccp(240, 298));
	feverGauge->setType(CCProgressTimerType::kCCProgressTimerTypeHorizontalBarLR);
	addChild(feverGauge);
	//playInfo->selectedItems 를
	
	KS::KSLog("%, %", __FILE__, __LINE__);
	KSoundEngine::sharedEngine()->playSound("readygo.mp3");
	auto mator = SceneUtil::playAnimation("ready.png", 0.1, 2, 10, 256, 65, false);
	auto action = CCSequence::actions(mator.first, CCCallFuncN::actionWithTarget(this, callfuncN_selector(ThisClassType::finishReady)), 0);
	mator.second->runAction(action);
	addChild(mator.second, 1000);
	mator.second->setPosition(ccp(240, 160));
	

	{
		map<string, string> item2img;
		item2img["time"] = "item_time1_1.png"; item2img["shield"] = "item_shield1_1.png"; item2img["exp"] = "item_exp1_1.png";
		item2img["item1"] = "item_tipin1_1.png"; item2img["item2"] = "item_longguide1_1.png";
		
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
				if(selectedItemIter->first == "item1") // tip-in 일 경우 이동시킴.
				{
					it->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(1.f), CCMoveTo::actionWithDuration(0.3f, ccp(TIPIN_X, TIPIN_Y)),
													  CCCallFuncN::actionWithTarget(this, callfuncN_selector(ThisClassType::deleteSprite)),
													  KSFunc(tipInableSprite->setIsVisible(true);),
													  0));
				}
				else
				{
					it->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(1.f), CCFadeOut::actionWithDuration(0.3f),
													  CCCallFuncN::actionWithTarget(this, callfuncN_selector(ThisClassType::deleteSprite)), 0));
				}
				itemPositionsIndex++;
			}
		}
	}
	CCSprite* left = CCSprite::spriteWithFile("left.png");
	left->setAnchorPoint(ccp(1, 0.5f));
	left->setPosition(ccp(0, 160));
	addChild(left, INT_MAX);
	
	CCSprite* right = CCSprite::spriteWithFile("right.png");
	right->setAnchorPoint(ccp(0.f, 0.5f));
	right->setPosition(ccp(480, 160));
	addChild(right, INT_MAX);
	
//	CCMenu* _menu = CCMenu::menuWithItems(0);
//	addChild(_menu, 3);
//	_menu->setPosition(CCPointZero);
	
	
	pause = CCMenuItemImage::itemFromNormalImage("ui_stop.png", "ui_stop.png",
															 this, menu_selector(ThisClassType::PAUSE));
	_menu->addChild(pause, 2);
	pause->setPosition(ccp(24 + 10, 295 - 10));
	timeFnt->setString(KS_Util::stringWithFormat("%.0f", BASETIME).c_str());
	return true;
}

void BasketGame::showTipInSprite()
{
	//tipInableSprite->runAction(CCSequence::actions(,0)););
	tipInableSprite->runAction( CCFadeIn::actionWithDuration(0.5f) );
}

void BasketGame::hideTipInSprite()
{
	tipInableSprite->runAction(CCFadeOut::actionWithDuration(0.5f));
}

//
void BasketGame::PAUSE(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	BSPauseLayer* pauseLayer = BSPauseLayer::node();
	//pauseLayer->setPosition(ccp(0, -20));
	addChild(pauseLayer, 1000);
	
	KS::KSLog("pause");
}


void BasketGame::pauseStopWatch2()
{
	watch2->pauseSchedulerAndActions();
}
void BasketGame::resumeStopWatch2()
{
	watch2->resumeSchedulerAndActions();
}
void BasketGame::finishGo(CCNode* n)
{
	n->removeFromParentAndCleanup(true);
	this->setIsTouchEnabled(true);
	gameScreen->setIsTouchEnabled(true);
	gameScreen->startGame();
//	schedule(schedule_selector(ThisClassType::update));
}

void BasketGame::finishReady(CCNode* n)
{
	n->removeFromParentAndCleanup(true);
	auto mator = SceneUtil::playAnimation("go.png", 0.075, 2, 4, 256, 65, false);
	auto action0 = CCSpawn::actions(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.1f), CCScaleTo::actionWithDuration(0.4f, 4.f)), CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.3f), CCFadeOut::actionWithDuration(0.2f)), mator.first, 0);
	auto action = CCSequence::actions(action0, CCCallFuncN::actionWithTarget(this, callfuncN_selector(ThisClassType::finishGo)), 0);
	mator.second->runAction(action);
	addChild(mator.second, 1000);
	mator.second->setPosition(ccp(240, 160));
}

void BasketGame::uiUpdate(int score)
{
	CCLabelBMFont* bonusFnt = CCLabelBMFont::labelWithString(KS_Util::stringWithFormat("+%d", score).c_str(), "bonusscore.fnt");
	bonusFnt->setPosition(ccp(scoreFnt->getPosition().x + scoreFnt->getContentSize().width / 3.f,
							  scoreFnt->getPosition().y + scoreFnt->getContentSize().height / 3.f));
	addChild(bonusFnt, 2);
	float virtualMaxScore = MAX(100000, BS2SaveData::getMaxScore());
	
	float goalScale = MIN(1.3f, 0.6f + (float)bs2play->gameScore.getVar() * 2.f / (float)virtualMaxScore);
	bonusFnt->setScale(0.3f);
	auto action0 = CCEaseBackOut::actionWithAction(CCScaleTo::actionWithDuration(1.f, goalScale));
	auto action1 = CCDelayTime::actionWithDuration(0.2f);
	auto action2 = CCFadeOut::actionWithDuration(0.2f);

	auto action3 = CCCallFuncN::actionWithTarget(this, callfuncN_selector(BasketGame::deleteSprite));
	auto totalAction = CCSequence::actions(action0, action1, action2, action3, 0);
	bonusFnt->runAction(totalAction);
	
	
	scoreFnt->runAction(CCEaseBackOut::actionWithAction(CCScaleTo::actionWithDuration(1.f, goalScale)));
	
	//scoreFnt->runAction(CCEaseBackOut::actionWithAction(CCScaleTo::actionWithDuration(1.f, 1.0f + BS2SaveData::getMaxScore() / bs2play->gameScore.getVar())));
}
void BasketGame::applyTime()
{
	gameScreen->setRemainTime(gameScreen->getRemainTime() + 10.f);
	setRemainTime(gameScreen->getRemainTime());
	
	CCParticleSystemPoint* part = CCParticleSystemPoint::particleWithFile("itemeffect.plist");
	part->setPosition(ccp(260, 305));
	//part->setPosition(ccpMult(ccp(coinAnimation.second->getContentSize().width, coinAnimation.second->getContentSize().height), 0.5f));
	addChild(part, 10);
}

void BasketGame::applyPlusBall()
{
	CCParticleSystemPoint* part = CCParticleSystemPoint::particleWithFile("itemeffect.plist");
	part->setPosition(ccp(320, 305));
	//part->setPosition(ccpMult(ccp(coinAnimation.second->getContentSize().width, coinAnimation.second->getContentSize().height), 0.5f));
	addChild(part, 10);
}
void BasketGame::applyDouble()
{
	doubleSprite = CCSprite::spriteWithFile("item_double.png");
	doubleSprite->setPosition(ccp(439, 34));
	addChild(doubleSprite);
}
void BasketGame::applyMove()
{
	moveSprite = CCSprite::spriteWithFile("item_move.png");
	moveSprite->setPosition(ccp(42, 34));
	addChild(moveSprite);
}

void BasketGame::applyGuide()
{
	gameScreen->useItem(ItemType::GUIDE_LINE);
}





void BasketGame::setTimeWarning()
{
	watch1->setIsVisible(false);
	watch2->setIsVisible(true);
}

void BasketGame::setTimeNoWarning()
{
	watch1->setIsVisible(true);
	watch2->setIsVisible(false);
}

void BasketGame::timeOver()
{
	pause->setIsEnabled(false);
	auto mator = SceneUtil::playAnimation("timeover.png", 0.1, 2, 4, TIMEOVERWIDTH, TIMEOVERHEIGHT, 8);
	KSoundEngine::sharedEngine()->playSound("timeover.mp3");
	mator.second->setPosition(ccp(240, 160));
	mator.second->setScale(1.2f);
	auto action = CCSequence::actionOneTwo(mator.first, CCCallFunc::actionWithTarget(gameScreen, callfunc_selector(GameScreen::finishTimeOver)));
	mator.second->runAction(action);
	addChild(mator.second, INT_MAX);
}
void BasketGame::setBonusTime()
{
	if(!watch3->getIsVisible())
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
	
}
void BasketGame::pointMaxSprite()
{
}

void BasketGame::incFever(float n)
{
	feverGauge->setPercentage(feverGauge->getPercentage() + n);
}

float BasketGame::getFever()
{
	return feverGauge->getPercentage();
}

void BasketGame::onEnterTransitionDidFinish()
{
	CCLayer::onEnterTransitionDidFinish();
}
void BasketGame::onExit()
{
	
	
	CCLayer::onExit();
}


bool BasketGame::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	CCTouch* touch = pTouch;
	
	/*if(touch->view()->isKindOfClass(EAGLView::class()) == false) // #
	 continue;*/
	
	CCPoint location(ccp(0, 0));
	location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
//	if(CCRect::CCRectContainsPoint(cancelCatchSprite->boundingBox(), location))
//	{
//		CCLog("cancel!!!");
//		gameScreen->cancelCatch();
//		return true;
//	}
	

	if(gameScreen->getThrowCounter() < 10 && moveSprite != 0 && moveSprite->getOpacity() > 0 && CCRect::CCRectContainsPoint(moveSprite->boundingBox(), location)
	   && gameScreen->controlableBall)
	{
		KSoundEngine::sharedEngine()->playSound("se_useitem.mp3");
		moveSprite->runAction(CCFadeOut::actionWithDuration(0.6f));
		gameScreen->useItem(ItemType::MOVE_BALL);
		return true;
	}
	if(gameScreen->getThrowCounter() < 10 && doubleSprite != 0 && doubleSprite->getOpacity() > 0&& CCRect::CCRectContainsPoint(doubleSprite->boundingBox(), location)
	   && gameScreen->controlableBall)
	{
		KSoundEngine::sharedEngine()->playSound("se_useitem.mp3");
		doubleSprite->runAction(CCFadeOut::actionWithDuration(0.6f));
		gameScreen->useItem(ItemType::DOUBLE_SHOT);
		return true;
	}
	return false;
}

void BasketGame::timepp()
{
	gameScreen->useItem(ItemType::TIME_PLUS);
}

void BasketGame::setRemainTime(float t)
{
	timeFnt->setString(KS_Util::stringWithFormat("%.0f", t * BASETIME / (float)BS2GameInformation::INIT_GAME_TIME).c_str());
}
void BasketGame::setScore(int n)
{
	//scoreFnt->setString(KS_Util::stringWithFormat("%d", n).c_str());
}

void BasketGame::eatCoin(int p)
{
	
}
void BasketGame::hideInterface()
{
}
void BasketGame::showInterface()
{
}
//static const float MAX_SCALE = 2.0f;
//static const float MIN_SCALE = 0.25f;
//
//#define CLAMP(x,y,z) MIN(MAX(x,y),z)



void BasketGame::registerWithTouchDispatcher()
{
	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, kCCMenuTouchPriority, true);
}


void BasketGame::updateScoreFnt(float dt)
{
	int goalScore = bs2play->gameScore.getVar();
	int displayedScore = atoi(scoreFnt->getString());
	if(displayedScore < goalScore )
	{
		int adderValue = (goalScore - displayedScore) / 30 + 21;
		adderValue = MIN(adderValue, goalScore - displayedScore);
		scoreFnt->setString(KS_Util::stringWithFormat("%d", displayedScore + adderValue).c_str());
	}
}

void BasketGame::showCombo()
{
	if(bs2play->combo.getVar() % 7 == 0)
	{
		CCSprite* d = CCSprite::spriteWithFile("kim4.png");
		d->setScale(0.2f);
		d->runAction(CCScaleTo::actionWithDuration(0.4f, 0.8f));
		
		addChild(d, 30);
		
		
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
		
		d->setPosition(ccp(110, 170));	
	}

	
	auto mator = SceneUtil::playAnimation("game_combo.png", 0.1, COMBO_COLS, COMBO_FRAME, COMBO_WIDTH, COMBO_HEIGHT, false);
	mator.second->runAction(CCSequence::actions(mator.first,
												//CCDelayTime::actionWithDuration(0.5f),
												CCCallFuncN::actionWithTarget(this, callfuncN_selector(BasketGame::deleteSprite)), 0));
	mator.second->setPosition(ccp(110, 150));
	addChild(mator.second, 2);
	
	CCLabelBMFont* comboFnt = CCLabelBMFont::labelWithString(KS_Util::stringWithFormat("%d", bs2play->combo.getVar()).c_str(), "gamecombo.fnt");
	comboFnt->setPosition(ccp(110, 170));
	comboFnt->setScale(0.3f);
	comboFnt->runAction(CCSequence::actions(CCEaseBackOut::actionWithAction(CCScaleTo::actionWithDuration(0.3, 1.f)),
											CCDelayTime::actionWithDuration(0.5f), CCFadeOut::actionWithDuration(0.5f),
											CCCallFuncN::actionWithTarget(this, callfuncN_selector(BasketGame::deleteSprite)), 0));
	addChild(comboFnt, 2);
}