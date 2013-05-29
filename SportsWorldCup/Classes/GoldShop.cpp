//
//  GoldShop.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 11..
//
//

#include "GoldShop.h"
#include "KS_Util.h"
#include "NSDefault.h"
#include "KSModalAlert.h"
#include "KSoundEngine.h"
#include "GraphDog.h"

bool GoldShop::init()
{
	CCSprite::init();
	CCSprite* back = CCSprite::spriteWithFile("shop_goldback.png");
	
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
		string filename = KS_Util::stringWithFormat("shop_gold%d.png", i+1);
		
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

GoldShop::~GoldShop()
{
	
}


void GoldShop::PIER1(CCObject*)
{
	KS::KSLog("PIER1");
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	if(NSDefault::getRuby() >= 10)
	{
		NSDefault::setRuby(NSDefault::getRuby() - 10);
		NSDefault::setGold(NSDefault::getGold() + 10000);
		mediator->setGold(NSDefault::getGold());
		mediator->setRuby(NSDefault::getRuby());
		
		KSModalAlert("", "COMPLETE!", 1,
					 "OK", NOPARAM);
		
		JsonBox::Object gParam;
		gParam["category"] = "buygold";
		gParam["log"] = "10000";
		graphdog->command("writelog", &gParam, 0, 0);
	}
	else
	{
		KSModalAlert("", ML::key("require_ruby").c_str(), 1,
					 "OK", NOPARAM);
	}
}
void GoldShop::PIER2(CCObject*)
{
	KS::KSLog("PIER2");
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	if(NSDefault::getRuby() >= 25)
	{
		NSDefault::setRuby(NSDefault::getRuby() - 25);
		NSDefault::setGold(NSDefault::getGold() + 25000);
		mediator->setGold(NSDefault::getGold());
		mediator->setRuby(NSDefault::getRuby());
		KSModalAlert("", "COMPLETE!", 1,
					 "OK", NOPARAM);
		JsonBox::Object gParam;
		gParam["category"] = "buygold";
		gParam["log"] = "25000";
		graphdog->command("writelog", &gParam, 0, 0);
	}
	else
	{
		KSModalAlert("", ML::key("require_ruby").c_str(), 1,
					 "OK", NOPARAM);
	}
}
void GoldShop::PIER3(CCObject*)
{
	KS::KSLog("PIER3");
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	if(NSDefault::getRuby() >= 40)
	{
		NSDefault::setRuby(NSDefault::getRuby() - 40);
		NSDefault::setGold(NSDefault::getGold() + 40000);
		mediator->setGold(NSDefault::getGold());
		mediator->setRuby(NSDefault::getRuby());
		KSModalAlert("", "COMPLETE!", 1,
					 "OK", NOPARAM);
		JsonBox::Object gParam;
		gParam["category"] = "buygold";
		gParam["log"] = "40000";
		graphdog->command("writelog", &gParam, 0, 0);
	}
	else
	{
		KSModalAlert("", ML::key("require_ruby").c_str(), 1,
					 "OK", NOPARAM);
	}
}
void GoldShop::PIER4(CCObject*)
{
	KS::KSLog("PIER4");
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	if(NSDefault::getRuby() >= 90)
	{
		NSDefault::setRuby(NSDefault::getRuby() - 90);
		NSDefault::setGold(NSDefault::getGold() + 90000);
		mediator->setGold(NSDefault::getGold());
		mediator->setRuby(NSDefault::getRuby());
		KSModalAlert("", "COMPLETE!", 1,
					 "OK", NOPARAM);
		JsonBox::Object gParam;
		gParam["category"] = "buygold";
		gParam["log"] = "90000";
		graphdog->command("writelog", &gParam, 0, 0);
	}
	else
	{
		KSModalAlert("", ML::key("require_ruby").c_str(), 1,
					 "OK", NOPARAM);
	}
}
void GoldShop::PIER5(CCObject*)
{
	KS::KSLog("PIER5");
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	if(NSDefault::getRuby() >= 200)
	{
		NSDefault::setRuby(NSDefault::getRuby() - 200);
		NSDefault::setGold(NSDefault::getGold() + 200000);
		mediator->setGold(NSDefault::getGold());
		mediator->setRuby(NSDefault::getRuby());
		KSModalAlert("", "COMPLETE!", 1,
					 "OK", NOPARAM);
		JsonBox::Object gParam;
		gParam["category"] = "buygold";
		gParam["log"] = "200000";
		graphdog->command("writelog", &gParam, 0, 0);
	}
	else
	{
		KSModalAlert("", ML::key("require_ruby").c_str(), 1,
					 "OK", NOPARAM);
	}
}
void GoldShop::PIER6(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	mediator->showMoreApps();
	KS::KSLog("PIER6");
	JsonBox::Object gParam;
	gParam["category"] = "buygold";
	gParam["log"] = "모어앱으로 갔음ㅋㅋ";
	graphdog->command("writelog", &gParam, 0, 0);
}

