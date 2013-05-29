
//
//  IntroHead.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 2. 5..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//
#include "IntroHead.h"
#include "KS_Util.h"
#include "NSDefault.h"
#include "GameSystem.h"
#include "ShopLayer.h"
#include "LoadingScene.h"
#include "IntroMain.h"
#include "KSBlock.h"
#include "KSoundEngine.h"
#include "GraphDog.h"
#include "KSModalAlert.h"
#include "DisableTouch.h"
IntroHead::IntroHead()
{
	introTime = 0.f;
	isSync = false;
	loading = 0;
}
IntroHead::~IntroHead()
{

}

bool IntroHead::init()
{
	CCLayer::init();
	setPosition(ccp(0, 350));
	//this->setIsTouchEnabled(true);
	this->setIsKeypadEnabled(true);
	CCSprite* back = CCSprite::spriteWithFile("main_head.png");
	back->setAnchorPoint(ccp(0.5f, 0.f));
	back->setPosition(ccp(240, 0.f));
	addChild(back);
	
	CCSprite* star = CCSprite::spriteWithFile("main_levelstar.png");
	star->setPosition(ccp(374, 17));
	addChild(star, 2);
	CCMenu* _menu = CCMenu::menuWithItems(0);
	_menu->setPosition(ccp(0, 0));
	addChild(_menu);
	{
		CCMenuItem* plus = CCMenuItemImage::itemFromNormalImage("main_plus.png", "main_plus.png", this, menu_selector(IntroHead::RUBYPLUS));
		plus->setPosition(ccp(71, 15));
		_menu->addChild(plus, 2);
	}
	{
		CCMenuItem* plus = CCMenuItemImage::itemFromNormalImage("main_plus.png", "main_plus.png", this, menu_selector(IntroHead::COINPLUS));
		plus->setPosition(ccp(189, 15));
		_menu->addChild(plus, 2);
	}
	{
		CCMenuItem* plus = CCMenuItemImage::itemFromNormalImage("main_plus.png", "main_plus.png", this, menu_selector(IntroHead::HEARTPLUS));
		plus->setPosition(ccp(344, 15));
		_menu->addChild(plus, 2);
	}
	{
		
	}
	rubyFnt = CCLabelBMFont::labelWithString(KS_Util::stringWithFormat("%d", NSDefault::getRuby()).c_str(), "main1.fnt");
	rubyFnt->setAnchorPoint(ccp(1, 0.5f));
	rubyFnt->setPosition(ccp(58, 15));
	addChild(rubyFnt, 2);
	
	goldFnt = CCLabelBMFont::labelWithString(KS_Util::stringWithFormat("%d", NSDefault::getGold()).c_str(), "main1.fnt");
	goldFnt->setAnchorPoint(ccp(1, 0.5f));
	goldFnt->setPosition(ccp(175, 15));
	addChild(goldFnt, 2);
	
	
	initHeartSprite();
	
	rechargeFnt = CCLabelBMFont::labelWithString("", "main2.fnt");
	rechargeFnt->setPosition(ccp(311, 16));
	addChild(rechargeFnt, 2);
	
	KS::KSLog("EXP : %", NSDefault::getExp());
	pair<int, int> lv_exp = playInfo->getLV_EXP(NSDefault::getExp());
	levelFnt = CCLabelBMFont::labelWithString(KS_Util::stringWithFormat("%d", lv_exp.first).c_str(), "main2.fnt");
	levelFnt->setPosition(ccp(372, 15));
	addChild(levelFnt, 2);
	
	expBar = CCProgressTimer::progressWithFile("main_level.png");
	expBar->setType(CCProgressTimerType::kCCProgressTimerTypeHorizontalBarLR);
	addChild(expBar, 1);
	expBar->setPercentage((float)lv_exp.second / (float)playInfo->expFromLv(lv_exp.first) * 100.f);
	expBar->setPosition(ccp(425, 16));
	
	lvPerFnt = CCLabelBMFont::labelWithString(KS_Util::stringWithFormat("%d%%", (int)((float)lv_exp.second / (float)playInfo->expFromLv(lv_exp.first) * 100.f)).c_str(), "main2.fnt");
	lvPerFnt->setPosition(ccp(425, 15));
	addChild(lvPerFnt, 2);

	//, *goldFnt, *rechargeFnt, *levelFnt, *expPercent;
	
	currentBack = false;
	exitLabel = 0;
	schedule(schedule_selector(IntroHead::checkBackKey));
	schedule(schedule_selector(IntroHead::timeChecker));
	return true;
}

void IntroHead::refreshEXP()
{
	pair<int, int> lv_exp = playInfo->getLV_EXP(NSDefault::getExp());
	levelFnt->setString(KS_Util::stringWithFormat("%d", lv_exp.first).c_str());
	lvPerFnt->setString(KS_Util::stringWithFormat("%d%%", (int)((float)lv_exp.second / (float)playInfo->expFromLv(lv_exp.first) * 100.f)).c_str());
	expBar->setPercentage(0);
	expBar->runAction(CCProgressTo::actionWithDuration(0.5f, (float)lv_exp.second / (float)playInfo->expFromLv(lv_exp.first) * 100.f));
}
void IntroHead::keyBackClicked()
{
	//CCLog("Itnro::back");
	
	if(currentBack)
	{
		CCDirector::sharedDirector()->end();
	}
	else
	{
		//exitLabel = CCLabelTTF::labelWithString("press back button to exit", "", 16);
		exitLabel = CCSprite::spriteWithFile("pressbackkey.png");
		//exitLabel->setColor(ccc3(255, 0, 0));
		
		exitLabel->setPosition(ccp(240, 20));
		addChild(exitLabel, INT_MAX);
		currentBack = true;
	}
	
	backKeyTime = introTime;
}

void IntroHead::setRuby(int n)
{
	rubyFnt->setString(KS_Util::stringWithFormat("%d", n).c_str());
}
void IntroHead::setGold(int n)
{
//	auto temp = goldFnt->getColor();
//	goldFnt->setColor(ccc3(255, 0, 0));
//	goldFnt->runAction(CCSequence::actions(
//										   CCDelayTime::actionWithDuration(0.01f),
//										   KSFuncN(fnt, {((CCLabelTTF*)goldFnt)->setColor(temp);}),
//										   0));
	goldFnt->setString(KS_Util::stringWithFormat("%d", n).c_str());
}
void IntroHead::checkBackKey(ccTime dt)
{
	introTime += dt;
	if(introTime - backKeyTime >= 2)
	{
		
		currentBack = false;
		removeChild(exitLabel, true);
	}
}

void IntroHead::onEnterTransitionDidFinish()
{
	CCLayer::onEnterTransitionDidFinish();
}
void IntroHead::onExit()
{
	
	
	CCLayer::onExit();
}

void IntroHead::addHeartSprite()
{
	CCPoint coord [] = {CCPoint(218, 16), CCPoint(234, 16), CCPoint(251, 16), CCPoint(267, 16), CCPoint(284, 16)};
	CCAssert(hearts.size() < 5, "...");
	CCPoint heartPos = coord[hearts.size()];
	CCSprite* h = CCSprite::spriteWithFile("main_heart.png");
	addChild(h, 1);
	h->setPosition(heartPos);
	hearts.push_back(h);
}
void IntroHead::removeHeartAnimation()
{
	if(NSDefault::getHeartNumber() <= 5)
	{
		//removeHeartSprite();
		KS::KSLog("%", hearts.size());
		CCAssert(!hearts.empty(), "...");
		
		CCSprite* h = hearts[hearts.size() - 1];
		ccBezierConfig bc;
		bc.controlPoint_1 = h->getPosition();
		auto heartPang = SceneUtil::playAnimation("heartpang.png", 0.07, 5, 5, 90, 90, 1);
		heartPang.second->runAction(CCSequence::actionOneTwo(heartPang.first, CCCallFuncN::actionWithTarget(this, callfuncN_selector(ThisClassType::deleteSprite))));
		heartPang.second->setPosition(h->getPosition());
		addChild(heartPang.second, 2);
		bc.controlPoint_2 = h->getPosition();//ccp(250, 70);
		bc.endPosition = h->getPosition();//ccp(430, 70);
		auto action1 = CCBezierTo::actionWithDuration(0.01f, bc); // 1.f
		auto action2 = CCFadeOut::actionWithDuration(0.01f); // 1.5f
		auto action3 = CCSpawn::actionOneTwo(action1, action2);
		auto action5 = CCSequence::actions(action3, CCCallFuncN::actionWithTarget(this, callfuncN_selector(IntroHead::deleteSprite)), 0);
		h->runAction(action5);
	}
	else
	{		
	}
	
	KSoundEngine::sharedEngine()->playSound("useheart.mp3");
	//NSDefault::setHeartBaseTime(PlayInfo::getCurrentTime_s());
	//remainHeartFnt->setString(KS_Util::stringWithFormat("%d", NSDefault::getHeartNumber() - 1).c_str());
}
void IntroHead::initHeartSprite()
{
	// 356, 132
	// 380, "
	// 405,
	// 429,
	// 454,
	CCPoint coord [] = {CCPoint(217, 16), CCPoint(234, 16), CCPoint(250, 16), CCPoint(267, 16), CCPoint(283, 16)};
	int fillNumber = MIN(NSDefault::getHeartNumber(), GameSystem::DEFAULT_MAX_HEART);
	for(int i=0; i<fillNumber; i++)
	{
		CCPoint heartPos = coord[i];
		CCSprite* h = CCSprite::spriteWithFile("main_heart.png");
		addChild(h, 1);
		h->setPosition(heartPos);
		hearts.push_back(h);
	}
	
	//CCPoint t = {356, 132}
	
	
}
void IntroHead::timeChecker(ccTime dt)
{
	// 하트 차는 조건 검사
	int currentTimeStamp = timestamp + (playInfo->getCurrentTime_s() - oldLocalTime);
	// 싱크 안되어 있으면 패스.
	if(isSync == false)
		return;
	if(currentTimeStamp - NSDefault::getHeartBaseTime() >= GameSystem::HEART_CHARGE_TIME &&
	   NSDefault::getHeartNumber() < GameSystem::DEFAULT_MAX_HEART)
	{
		int howAdd = (int)((currentTimeStamp - NSDefault::getHeartBaseTime()) / GameSystem::HEART_CHARGE_TIME);
		if(NSDefault::getHeartNumber() < 5 && NSDefault::getHeartNumber() + howAdd > 5)
			howAdd = 5 - NSDefault::getHeartNumber();
		
		if(NSDefault::getHeartNumber() + howAdd <= 5)
		{
			for(int i=0; i<howAdd; i++)
			{
				if(hearts.size() < 5)
				{
					addHeartSprite();
				}
				else
					break;
			}
		}
		NSDefault::setHeartNumber(NSDefault::getHeartNumber() + howAdd);
//		if(shop)
//			shop->notifiedHeart(NSDefault::getHeartNumber());
		//remainHeartFnt->setString(KS_Util::stringWithFormat("%d", NSDefault::getHeartNumber()).c_str());
		NSDefault::setHeartBaseTime(currentTimeStamp -
									(currentTimeStamp - NSDefault::getHeartBaseTime()) % GameSystem::HEART_CHARGE_TIME);
		
	}
	
	int rechargeRemainTime_s = GameSystem::HEART_CHARGE_TIME - currentTimeStamp + NSDefault::getHeartBaseTime();
	int minu = rechargeRemainTime_s / 60.f;
	int seco = rechargeRemainTime_s % 60;
	string t = KS_Util::stringWithFormat("%02d:%02d", minu, seco);
	if(NSDefault::getHeartNumber() >= GameSystem::DEFAULT_MAX_HEART) 
	{
		rechargeFnt->setString(KS_Util::stringWithFormat("%d", NSDefault::getHeartNumber()).c_str());
//		if(shop)
//			shop->notifiedTime("MAX");
		
	}
	else
	{
		rechargeFnt->setString(t.c_str());
//		if(shop)
//			shop->notifiedTime(t);
	}
}

void IntroHead::RUBYPLUS(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	mediator->showRubyShop();
	KS::KSLog("%", __FUNCTION__);
}
void IntroHead::COINPLUS(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	mediator->showGoldShop();
	KS::KSLog("%", __FUNCTION__);
}
void IntroHead::HEARTPLUS(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	mediator->showHeartShop();
	KS::KSLog("%", __FUNCTION__);
}
void IntroHead::syncTimeWithInc()
{
	CCLog("syncTime start");
	withInc = true;
	graphdog->command("timestamp", 0, this, gd_selector(ThisClassType::finishSyncTime));
}
void IntroHead::syncTime()
{
	CCLog("syncTime start");
	
	JsonBox::Object param;
	DisableTouch* cl = DisableTouch::node(ccc4f(0, 0, 0, 180));
	cl->setAnchorPoint(ccp(0.5, 0.5));
	cl->setPosition(ccp(240, 160));
	getParent()->addChild(cl, INT_MAX );
	
	float loadingX = 240;
	float loadingY = 150;//160;
	auto animator = SceneUtil::playAnimation("loading.png", 0.1, 9, 9, 56, 56, true);
	animator.second->runAction(animator.first);
	getParent()->addChild(animator.second, INT_MAX);
	animator.second->setPosition(ccp(loadingX, loadingY));
	loading = animator.second;
	loading->setScale(1.f);
	
	withInc = false;
	
	
	
	ostringstream oss;
	oss << (int)cl;
	param["callback"] = oss.str();
	graphdog->command("timestamp", &param, this, gd_selector(ThisClassType::finishSyncTime));
}

void IntroHead::syncTime(unsigned int st)
{
	oldLocalTime = playInfo->getCurrentTime_s();
	timestamp = st;
	isSync = true;
}
void IntroHead::finishSyncTime(JsonBox::Object js)
{
	CCLog("syncTime end");
	if(loading)
	{
		loading->removeFromParentAndCleanup(true);
		loading = 0;
	}
	try
	{
		JsonBox::Object obj = js["param"].getObject();
		string _cl = obj["callback"].getString();
		if(_cl != "")
		{
			CCNode* cl = (CCNode*)atoi(_cl.c_str());
			cl->removeFromParentAndCleanup(true);
		}
		
		if(js["state"].getString() != "ok")
			throw string(ML::key("netfail"));
		oldLocalTime = playInfo->getCurrentTime_s();
		timestamp = js["timestamp"].getInt();
		if(NSDefault::getIsTodayFree())
		{
			if(timestamp - NSDefault::getFreeTodayBuyTime() > 60*60 * 24) // 24H 지났으면
			{
				NSDefault::setIsTodayFree(false);
			}
		}
		isSync = true;
 		if(withInc)
			mediator->incLoadCount();
	}
	catch(const string& msg)
	{
		KSModalAlert("", msg.c_str(), 1,
					 ML::key("retrying").c_str(), this, callfunc_selector(ThisClassType::RETRY));
	}
	
}

void IntroHead::RETRY()
{
	graphdog->command("timestamp", 0, this, gd_selector(ThisClassType::finishSyncTime));
}


