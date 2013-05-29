//
//  PauseLayer.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 19..
//
//

#include "PauseLayer.h"
#include "PriorityMenu.h"
#include "KS_Util.h"
#include "NSDefault.h"
#include "KSoundEngine.h"
bool PauseLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint location = pTouch->locationInView();
	location = CCDirector::sharedDirector()->convertToGL(location);
	
//	if(CCRect::CCRectContainsPoint(resume->boundingBox(), location))
//	{
//		getParent()->removeChild(this, true);
//	}
//	
//	if(CCRect::CCRectContainsPoint(home->boundingBox(), location))
//	{
//		
//	}
	
	return true; // swallow;
}
void PauseLayer::EXIT()
{
//	ReplaceScene(BasketGameOver);
}
bool PauseLayer::init()
{
	CoverLayer::init();
	
	CCSprite* _back = CCSprite::spriteWithFile("stop_back.png");
	_back->setPosition(ccp(240, 160));
	addChild(_back);
	thiz = _back;
//	resume = CCSprite::spriteWithFile("stopad_play.png");
//	resume->setPosition(ccp(397, 126));
//	addChild(resume);
//	
//	home = CCSprite::spriteWithFile("stopad_home.png");
//	home->setPosition(ccp(397, 229));
//	addChild(home);
//	
//	adSprite = CCSprite::spriteWithFile("stop_ad.png");
//	adSprite->setPosition(ccp(188, 157));
//	addChild(adSprite);
	
	PriorityMenu* _menu = PriorityMenu::menuWithItems(kCCMenuTouchPriority-2, 0);
	_menu->setPosition(CCPointZero);
	thiz->addChild(_menu);
	
	
	
	if(playInfo->currentGame == "WORLDCUP" || playInfo->currentGame == "BATTLE")
	{
		con = CCMenuItemImage::itemFromNormalImage("stop_continue1.png", "stop_continue1.png",
												   this, menu_selector(ThisClassType::CONTINUE));
		con->setPosition(ccp(115, 135));
		_menu->addChild(con, 2);
		
		home = CCMenuItemImage::itemFromNormalImage("stop_home1.png", "stop_home1.png",
													this, menu_selector(ThisClassType::HOME));
		home->setPosition(ccp(115, 65));
		_menu->addChild(home, 2);

				
		
	}
	else
	{
		restart = CCMenuItemImage::itemFromNormalImage("stop_restart.png", "stop_restart.png", "stop_restart_disable.png",
													   this, menu_selector(ThisClassType::RESTART));
		if(NSDefault::getHeartNumber() <= 0 && NSDefault::getIsTodayFree() == false)
		{
			restart->setIsEnabled(false);
		}
		restart->setPosition(ccp(115, 100));
		_menu->addChild(restart, 2);
		
		con = CCMenuItemImage::itemFromNormalImage("stop_continue.png", "stop_continue.png",
												   this, menu_selector(ThisClassType::CONTINUE));
		con->setPosition(ccp(115, 146));
		_menu->addChild(con, 2);
		
		home = CCMenuItemImage::itemFromNormalImage("stop_home.png", "stop_home.png",
													this, menu_selector(ThisClassType::HOME));
		home->setPosition(ccp(115, 53));
		_menu->addChild(home, 2);
	
		
	}
	
	
	runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.1f),
									   KSFunc({
											//CCDirector::sharedDirector()->stopAnimation();
										})));
	return true;
}

void PauseLayer::CONTINUE(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	this->removeFromParentAndCleanup(true);
}
void PauseLayer::RESTART(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	if(NSDefault::getHeartNumber() > 0 || NSDefault::getIsTodayFree())
	{
		con->setIsEnabled(false);
		restart->setIsEnabled(false);
		home->setIsEnabled(false);
		restartGame();
	}
	else
	{
		KSModalAlert("", ML::key("require_heart").c_str(), 1,
					 "OK", NOPARAM);
	}
}
void PauseLayer::HOME(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	goToHome();
}
void PauseLayer::onEnterTransitionDidFinish()
{
	CoverLayer::onEnterTransitionDidFinish();
	//this->getParent()->pauseSchedulerAndActions();
	CCDirector::sharedDirector()->pause();
	
}

void PauseLayer::onExit()
{
	CCDirector::sharedDirector()->resume();
	//this->getParent()->resumeSchedulerAndActions();
	//CCDirector::sharedDirector()->startAnimation();
	CoverLayer::onExit();
}