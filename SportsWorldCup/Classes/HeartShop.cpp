//
//  HeartShop.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 11..
//
//

#include "HeartShop.h"
#include "KS_Util.h"
#include "NSDefault.h"
#include "KSModalAlert.h"
#include "KSoundEngine.h"
#include "GraphDog.h"

bool HeartShop::init()
{
	CCSprite::init();
	CCSprite* back = CCSprite::spriteWithFile("shop_heartback.png");
	
	addChild(back);
	thiz = back;
	
	CCMenu* _menu = CCMenu::menuWithItems(0);
	_menu->setPosition(CCPointZero);
	thiz->addChild(_menu);
	CCMenuItem* close = CCMenuItemImage::itemFromNormalImage("main_exit.png", "main_exit.png", this, menu_selector(ThisClassType::CLOSE));
	close->setPosition(ccp(234, 263));
	_menu->addChild(close, 2);
	for(int i=0; i<6; i++)
	{
		string filename = KS_Util::stringWithFormat("shop_heart%d.png", i+1);
		
		CCMenuItem* item;
		switch(i)
		{
			case 0:
				item = CCMenuItemImage::itemFromNormalImage(filename.c_str(), filename.c_str(),
															this, menu_selector(ThisClassType::PIER1));
				break;
			case 1:
				item = CCMenuItemImage::itemFromNormalImage(filename.c_str(), filename.c_str(),
															this, menu_selector(ThisClassType::PIER2));
				break;
			case 2:
				item = CCMenuItemImage::itemFromNormalImage(filename.c_str(), filename.c_str(),
															this, menu_selector(ThisClassType::PIER3));
				break;
			case 3:
				item = CCMenuItemImage::itemFromNormalImage(filename.c_str(), filename.c_str(),
															this, menu_selector(ThisClassType::PIER4));
				break;
			case 4:
				item = CCMenuItemImage::itemFromNormalImage(filename.c_str(), filename.c_str(),
															this, menu_selector(ThisClassType::PIER5));
				break;
			case 5:
				item = CCMenuItemImage::itemFromNormalImage(filename.c_str(), filename.c_str(),
															this, menu_selector(ThisClassType::PIER6));
				break;
				
		}
		
		item->setPosition(position.pier[i]);
		_menu->addChild(item, 2);
		
	}
	
	return true;
}

HeartShop::~HeartShop()
{
	
}

void HeartShop::PIER1(CCObject*)
{
	KS::KSLog("PIER1");
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	int g = 1000;
	if(NSDefault::getGold() >= g)
	{
		NSDefault::setGold(NSDefault::getGold() - g);
		KSoundEngine::sharedEngine()->playSound("usemoney.mp3");
		mediator->setGold(NSDefault::getGold());
		if(mediator->getHeartSize() < 5)
		{
			mediator->addHeartSprite();
		}
		NSDefault::setHeartNumber(NSDefault::getHeartNumber() + 1);
		KSModalAlert("", "COMPLETE!", 1,
					 "OK", NOPARAM);
		JsonBox::Object gParam;
		gParam["category"] = "buyheart";
		gParam["log"] = "하트 1개 삼 ㅋㅋ";
		graphdog->command("writelog", &gParam, 0, 0);
	}
	else
	{
		KSModalAlert("", ML::key("require_coin").c_str(), 1,
					 "OK", NOPARAM);
	}
}
void HeartShop::PIER2(CCObject*)
{
	KS::KSLog("PIER2");
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	int g = 2000;
	if(NSDefault::getGold() >= g)
	{
		NSDefault::setGold(NSDefault::getGold() - g);
		KSoundEngine::sharedEngine()->playSound("usemoney.mp3");
		mediator->setGold(NSDefault::getGold());
		if(mediator->getHeartSize() < 5)
		{
			mediator->addHeartSprite();
		}
		if(mediator->getHeartSize() < 5)
		{
			mediator->addHeartSprite();
		}
		if(mediator->getHeartSize() < 5)
		{
			mediator->addHeartSprite();
		}
		NSDefault::setHeartNumber(NSDefault::getHeartNumber() + 3);
		KSModalAlert("", "COMPLETE!", 1,
					 "OK", NOPARAM);
		JsonBox::Object gParam;
		gParam["category"] = "buyheart";
		gParam["log"] = "하트 3개 삼 ㅋㅋ";
		graphdog->command("writelog", &gParam, 0, 0);
	}
	else
	{
		KSModalAlert("", ML::key("require_coin").c_str(), 1,
					 "OK", NOPARAM);
	}
}
void HeartShop::PIER3(CCObject*)
{
	KS::KSLog("PIER3");
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	int g = 4000;
	if(NSDefault::getGold() >= g)
	{
		NSDefault::setGold(NSDefault::getGold() - g);
		KSoundEngine::sharedEngine()->playSound("usemoney.mp3");
		mediator->setGold(NSDefault::getGold());
		for(int i=0; i<7; i++)
		{
			if(mediator->getHeartSize() < 5)
			{
				mediator->addHeartSprite();
			}
		}
		NSDefault::setHeartNumber(NSDefault::getHeartNumber() + 7);
		KSModalAlert("", "COMPLETE!", 1,
					 "OK", NOPARAM);
		JsonBox::Object gParam;
		gParam["category"] = "buyheart";
		gParam["log"] = "하트 7개 삼 ㅋㅋ";
		graphdog->command("writelog", &gParam, 0, 0);
	}
	else
	{
		KSModalAlert("", ML::key("require_coin").c_str(), 1,
					 "OK", NOPARAM);
	}
}
void HeartShop::PIER4(CCObject*)
{
	KS::KSLog("PIER4");
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	int g = 10000;
	if(NSDefault::getGold() >= g)
	{
		NSDefault::setGold(NSDefault::getGold() - g);
		KSoundEngine::sharedEngine()->playSound("usemoney.mp3");
		mediator->setGold(NSDefault::getGold());
		for(int i=0; i<20; i++)
		{
			if(mediator->getHeartSize() < 5)
			{
				mediator->addHeartSprite();
			}
		}
		NSDefault::setHeartNumber(NSDefault::getHeartNumber() + 20);
		KSModalAlert("", "COMPLETE!", 1,
					 "OK", NOPARAM);
		JsonBox::Object gParam;
		gParam["category"] = "buyheart";
		gParam["log"] = "하트 20개 삼 ㅋㅋ";
		graphdog->command("writelog", &gParam, 0, 0);
	}
	else
	{
		KSModalAlert("", ML::key("require_coin").c_str(), 1,
					 "OK", NOPARAM);
	}
}
void HeartShop::PIER5(CCObject*)
{
	KS::KSLog("PIER5");
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	int g = 20000;
	if(NSDefault::getGold() >= g)
	{
		NSDefault::setGold(NSDefault::getGold() - g);
		KSoundEngine::sharedEngine()->playSound("usemoney.mp3");
		mediator->setGold(NSDefault::getGold());
		for(int i=0; i<50; i++)
		{
			if(mediator->getHeartSize() < 5)
			{
				mediator->addHeartSprite();
			}		
		}
		NSDefault::setHeartNumber(NSDefault::getHeartNumber() + 50);
		KSModalAlert("", "COMPLETE!", 1,
					 "OK", NOPARAM);
		JsonBox::Object gParam;
		gParam["category"] = "buyheart";
		gParam["log"] = "하트 50개 삼 ㅋㅋ";
		graphdog->command("writelog", &gParam, 0, 0);
	}
	else
	{
		KSModalAlert("", ML::key("require_coin").c_str(), 1,
					 "OK", NOPARAM);
	}
}
void HeartShop::PIER6(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	if(NSDefault::getIsTodayFree())
	{
		KSModalAlert("", "Already Buying", 1,
					 "OK", NOPARAM);
	}
	else
	{
		int g = 10000;
		if(NSDefault::getGold() >= g)
		{
			NSDefault::setGold(NSDefault::getGold() - g);
			KSoundEngine::sharedEngine()->playSound("usemoney.mp3");
			mediator->setGold(NSDefault::getGold());
			NSDefault::setIsTodayFree(true);
			NSDefault::setFreeTodayBuyTime(mediator->getCurrentTime_s());
			KSModalAlert("", "COMPLETE!", 1,
						 "OK", NOPARAM);
			JsonBox::Object gParam;
			gParam["category"] = "buyheart";
			gParam["log"] = "오늘 하트 무료";
			graphdog->command("writelog", &gParam, 0, 0);
		}
		else
		{
			KSModalAlert("", ML::key("require_coin").c_str(), 1,
						 "OK", NOPARAM);
		}
	}
	
	
	
	KS::KSLog("PIER6");
}
