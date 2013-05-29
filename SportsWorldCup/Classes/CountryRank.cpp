//
//  CountryRank.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 2. 28..
//
//

#include "CountryRank.h"
#include <sstream>
#include "KS_Util.h"
#include "Bridge.h"

#include "IntroMain.h"
#include "GraphDog.h"
#include "GameSystem.h"
#include "KSoundEngine.h"
using namespace std;


bool CountryRank::init()
{
	CCSprite::init();
	
	
	
	CCSprite* back = CCSprite::spriteWithFile("invite_back.png");
	
	addChild(back);
	thiz = back;
	
	CCMenu* _menu = CCMenu::menuWithItems(0);
	_menu->setPosition(CCPointZero);
	thiz->addChild(_menu);
	
	{
		
		CCMenuItem* item = CCMenuItemImage::itemFromNormalImage("main_font_back.png", "main_font_back_down.png",
																this, menu_selector(CountryRank::FACEBOOKRANK));
		item->setPosition(ccp(75, 25));
		_menu->addChild(item, 2);
//		item->runAction(
//						CCRepeatForever::actionWithAction( CCSequence::actionOneTwo(CCEaseOut::actionWithAction(CCMoveBy::actionWithDuration(0.8, ccp(0, 5)), 1.3),
//																					CCEaseOut::actionWithAction(CCMoveBy::actionWithDuration(0.8, ccp(0, -5)), 1.3))) );
	}
	{
		CCMenuItem* item = CCMenuItemImage::itemFromNormalImage("invitelist.png", "invitelist.png",
																this, menu_selector(CountryRank::INVITEFRIEND));
		item->setPosition(ccp(135, 150));
		_menu->addChild(item, 2);
		
	}
	return true;
}

void CountryRank::INVITEFRIEND(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	CCNotificationCenter::sharedNotifCenter()->postNotification("inviteFriend");
}

void CountryRank::FACEBOOKRANK(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	mediator->showFacebookRank();
}
CountryRank::~CountryRank()
{
	//graphdog->removeCommand(this);
}


