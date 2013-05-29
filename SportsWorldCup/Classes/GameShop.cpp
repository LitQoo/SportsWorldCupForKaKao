//
//  GameShop.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 12..
//
//

#include "GameShop.h"
#include "KS_Util.h"
#include "GameSystem.h"
#include "Bridge.h"
#include "KSModalAlert.h"
#include "PriorityMenu.h"
#include "NSDefault.h"
#include "KSoundEngine.h"
const float GameShop::SALERATE = 0.7f;
bool GameShop::init()
{
	CCSprite::init();
	
	
	
	
	
	return true;
}
void GameShop::LEVELLIMIT(CCObject* m)
{
	const char* category = (const char*)((CCMenuItemImage*)m)->getUserData();
	string category_s = category;
	mediator->showAlert(KS_Util::stringWithFormat("Level limit is %d.", levelLimits[category_s]).c_str());
}
void GameShop::initMenu(string gamename, string itemfile1, string itemfile2)
{
	CCSprite* back = CCSprite::spriteWithFile("itemshop_back.png");
	
	addChild(back);
	thiz = back;
	
	PriorityMenu* pMenu = PriorityMenu::menuWithItems(kCCMenuTouchPriority - 1, 0);
	pMenu->setPosition(CCPointZero);
	thiz->addChild(pMenu, 2);
	
	CCMenu* _menu = CCMenu::menuWithItems(0);
	_menu->setPosition(CCPointZero);
	thiz->addChild(_menu);
	
	{
		CCMenuItem* timeItem1 = CCMenuItemImage::itemFromNormalImage("item_time1.png", "item_time1.png");
		CCMenuItem* timeItem2 = CCMenuItemImage::itemFromNormalImage("item_time1.png", "item_time1.png");
		times = CCMenuItemToggle::itemWithTarget(this, menu_selector(ThisClassType::TIME), timeItem1, timeItem2, 0);
		times->setPosition(position.time);
		_menu->addChild(times, 2);
		
		auto l_e = playInfo->getLV_EXP(NSDefault::getExp());
		if(l_e.first < levelLimits["time"])
		{
			CCMenuItem* lock = CCMenuItemImage::itemFromNormalImage("item_lock.png", "item_lock.png",
																	this, menu_selector(ThisClassType::LEVELLIMIT));
			pMenu->addChild(lock, 2);
			lock->setPosition(position.time);
			lock->setUserData((void*)"time");
		}
	}
	{
		CCMenuItem* shieldItem1 = CCMenuItemImage::itemFromNormalImage("item_shield1.png", "item_shield1.png");
		CCMenuItem* shieldItem2 = CCMenuItemImage::itemFromNormalImage("item_shield1.png", "item_shield1.png");
		shields = CCMenuItemToggle::itemWithTarget(this, menu_selector(ThisClassType::SHIELD), shieldItem1, shieldItem2, 0);
		shields->setPosition(position.shield);
		_menu->addChild(shields, 2);
		
		auto l_e = playInfo->getLV_EXP(NSDefault::getExp());
		if(l_e.first < levelLimits["shield"])
		{
			CCMenuItem* lock = CCMenuItemImage::itemFromNormalImage("item_lock.png", "item_lock.png",
																	this, menu_selector(ThisClassType::LEVELLIMIT));
			pMenu->addChild(lock, 2);
			lock->setPosition(position.shield);
			lock->setUserData((void*)"shield");
		}
	}
	{
		CCMenuItem* expItem1 = CCMenuItemImage::itemFromNormalImage("item_exp1.png", "item_exp1.png");
		CCMenuItem* expItem2 = CCMenuItemImage::itemFromNormalImage("item_exp1.png", "item_exp1.png");
		exps = CCMenuItemToggle::itemWithTarget(this, menu_selector(ThisClassType::EXP), expItem1, expItem2, 0);
		exps->setPosition(position.exp);
		_menu->addChild(exps, 2);
		
		auto l_e = playInfo->getLV_EXP(NSDefault::getExp());
		if(l_e.first < levelLimits["exp"])
		{
			CCMenuItem* lock = CCMenuItemImage::itemFromNormalImage("item_lock.png", "item_lock.png",
																	this, menu_selector(ThisClassType::LEVELLIMIT));
			pMenu->addChild(lock, 2);
			lock->setPosition(position.exp);
			lock->setUserData((void*)"exp");
		}
	}
	{
		CCMenuItem* item1 = CCMenuItemImage::itemFromNormalImage(itemfile1.c_str(), itemfile1.c_str());
		CCMenuItem* item2 = CCMenuItemImage::itemFromNormalImage(itemfile1.c_str(), itemfile1.c_str());
		CCMenuItemToggle* items = CCMenuItemToggle::itemWithTarget(this, menu_selector(ThisClassType::GAMEITEM1), item1, item2, 0);
		gameItem1 = items;
		items->setPosition(position.gameItem1);
		_menu->addChild(items, 2);
		
		auto l_e = playInfo->getLV_EXP(NSDefault::getExp());
		if(l_e.first < levelLimits["item1"])
		{
			CCMenuItem* lock = CCMenuItemImage::itemFromNormalImage("item_lock.png", "item_lock.png",
																	this, menu_selector(ThisClassType::LEVELLIMIT));
			pMenu->addChild(lock, 2);
			lock->setPosition(position.gameItem1);
			lock->setUserData((void*)"item1");
		}
	}
	{
		CCMenuItem* item1 = CCMenuItemImage::itemFromNormalImage(itemfile2.c_str(), itemfile2.c_str());
		CCMenuItem* item2 = CCMenuItemImage::itemFromNormalImage(itemfile2.c_str(), itemfile2.c_str());
		CCMenuItemToggle* items = CCMenuItemToggle::itemWithTarget(this, menu_selector(ThisClassType::GAMEITEM2), item1, item2, 0);
		gameItem2 = items;
		items->setPosition(position.gameItem2);
		_menu->addChild(items, 2);
		
		auto l_e = playInfo->getLV_EXP(NSDefault::getExp());
		if(l_e.first < levelLimits["item2"])
		{
			CCMenuItem* lock = CCMenuItemImage::itemFromNormalImage("item_lock.png", "item_lock.png",
																	this, menu_selector(ThisClassType::LEVELLIMIT));
			pMenu->addChild(lock, 2);
			lock->setPosition(position.gameItem2);
			lock->setUserData((void*)"item2");
		}
	}
	
	{
		CCSprite* ch = CCSprite::spriteWithFile("item_check.png");
		ch->setPosition(ccp(73, 225));
		thiz->addChild(ch, 2);
		ch->setIsVisible(false);
		checks.push_back(ch);
	}
	{
		CCSprite* ch = CCSprite::spriteWithFile("item_check.png");
		ch->setPosition(ccp(152, 225));
		thiz->addChild(ch, 2);
		ch->setIsVisible(false);
		checks.push_back(ch);
	}
	{
		CCSprite* ch = CCSprite::spriteWithFile("item_check.png");
		ch->setPosition(ccp(231, 225));
		thiz->addChild(ch, 2);
		ch->setIsVisible(false);
		checks.push_back(ch);
	}
	{
		CCSprite* ch = CCSprite::spriteWithFile("item_check.png");
		ch->setPosition(ccp(112, 147));
		thiz->addChild(ch, 2);
		ch->setIsVisible(false);
		checks.push_back(ch);
	}
	{
		CCSprite* ch = CCSprite::spriteWithFile("item_check.png");
		ch->setPosition(ccp(192, 147));
		thiz->addChild(ch, 2);
		ch->setIsVisible(false);
		checks.push_back(ch);
	}

	if(playInfo->currentGame == gamename)
	{
		for( auto iter = playInfo->selectedItems.begin(); iter != playInfo->selectedItems.end(); ++iter)
		{
			if(iter->second)
			{
				isSaled = true;
				break;
			}
		}
		selectedItems = playInfo->selectedItems;
		if(playInfo->selectedItems["time"] == true)
		{
			times->setSelectedIndex(1);
			checks[2]->setIsVisible(true);
			onTime();
		}
		if(playInfo->selectedItems["shield"] == true)
		{
			shields->setSelectedIndex(1);
			checks[1]->setIsVisible(true);
			onShield();
		}
		if(playInfo->selectedItems["exp"] == true)
		{
			exps->setSelectedIndex(1);
			checks[0]->setIsVisible(true);
			onExp();
		}
		if(playInfo->selectedItems["item1"] == true)
		{
			gameItem1->setSelectedIndex(1);
			checks[3]->setIsVisible(true);
			onItem1();
		}
		if(playInfo->selectedItems["item2"] == true)
		{
			gameItem2->setSelectedIndex(1);
			checks[4]->setIsVisible(true);
			onItem2();
		}
	}
	
	saleTable = CCSprite::spriteWithFile("itemshop_totalsale.png");
	saleTable->setPosition(ccp(135, 22));
	thiz->addChild(saleTable, 3);
	
	strokePrice = CCSprite::spriteWithFile("itemshop_sale.png");
	strokePrice->setPosition(ccp(130, 32));
	thiz->addChild(strokePrice, 5);
	
	price = CCLabelBMFont::labelWithString("0", "total_gold.fnt");
	price->setPosition(ccp(180, 24));
	price->setColor(ccc3(0, 0, 0));
	thiz->addChild(price, 2);
	
	originalPrice = CCLabelBMFont::labelWithString("0", "total_salegold.fnt");
	originalPrice->setPosition(ccp(135, 24));
	originalPrice->setColor(ccc3(0, 0, 0));
	thiz->addChild(originalPrice, 4);
	originalPrice->setIsVisible(false);
	
	saledPrice = CCLabelBMFont::labelWithString("0", "total_salegold.fnt");
	saledPrice->setPosition(ccp(201, 24));
	saledPrice->setColor(ccc3(0, 0, 0));
	thiz->addChild(saledPrice, 4);
	saledPrice->setIsVisible(false);
	
	refreshPriceTable();
	
	desc = CCLabelTTF::labelWithString("", defaultFont, 14.f);
	desc->setPosition(ccp(130, 66));
	thiz->addChild(desc, 4);
}

void GameShop::refreshPriceTable()
{
	
	
	if(isSaled)
	{
		saleTable->setIsVisible(true);
		strokePrice->setIsVisible(true);
		saledPrice->setIsVisible(true);
		originalPrice->setIsVisible(true);
		price->setIsVisible(false);
		
		originalPrice->setString(KS_Util::stringWithFormat("%d", getTotalPrice()).c_str());
		saledPrice->setString(KS_Util::stringWithFormat("%d", (int)(getTotalPrice() * SALERATE)).c_str());
	}
	else
	{
		// 180, 24
		saleTable->setIsVisible(false);
		strokePrice->setIsVisible(false);
		saledPrice->setIsVisible(false);
		originalPrice->setIsVisible(false);
		price->setIsVisible(true);
		
		price->setString(KS_Util::stringWithFormat("%d", getTotalPrice()).c_str());
	}
	
	
}
void GameShop::TIME(CCObject* s)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	CCMenuItemToggle* _s = (CCMenuItemToggle*)s;
	
	int i = _s->getSelectedIndex();
	desc->setString(ML::key("time").c_str());
	//선택 해지할 때
	if(i == 0)
	{
		checks[2]->setIsVisible(false);
		selectedItems["time"] = false;
		isSaled = false;
		offTime();
	}
	//선택할 때
	else if(i == 1)
	{
		int trueCount = 0;
		for(auto iter = selectedItems.begin(); iter != selectedItems.end(); ++iter)
		{
			if(iter->second == true)
				trueCount++;
		}
		
		if(trueCount >= 3)
		{
			_s->setSelectedIndex(0);
			KSModalAlert("", ML::key("Item Number <= 3").c_str(), 1,
								  "ok", NOPARAM);
		}
		else
		{
			isSaled = false;
			checks[2]->setIsVisible(true);
			selectedItems["time"] = true;
			
			onTime();
		}
		
		
	}
	refreshPriceTable();
}
void GameShop::SHIELD(CCObject* s)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	CCMenuItemToggle* _s = (CCMenuItemToggle*)s;
	
	int i = _s->getSelectedIndex();
	desc->setString(ML::key("shield").c_str());
	//선택 해지할 때
	if(i == 0)
	{
		checks[1]->setIsVisible(false);
		selectedItems["shield"] = false;
		isSaled = false;
		offShield();
	}
	//선택할 때
	else if(i == 1)
	{
		int trueCount = 0;
		for(auto iter = selectedItems.begin(); iter != selectedItems.end(); ++iter)
		{
			if(iter->second == true)
				trueCount++;
		}
		
		if(trueCount >= 3)
		{
			_s->setSelectedIndex(0);
			KSModalAlert("", ML::key("Item Number <= 3").c_str(), 1,
								  "ok", NOPARAM);
		}
		else
		{
			isSaled = false;
			checks[1]->setIsVisible(true);
			selectedItems["shield"] = true;
			onShield();
			
		}
	}
	refreshPriceTable();
}
void GameShop::EXP(CCObject* s)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	CCMenuItemToggle* _s = (CCMenuItemToggle*)s;
	desc->setString(ML::key("exp").c_str());
	int i = _s->getSelectedIndex();
	//선택 해지할 때
	if(i == 0)
	{
		isSaled = false;
		checks[0]->setIsVisible(false);
		selectedItems["exp"] = false;
		offExp();
	}
	//선택할 때
	else if(i == 1)
	{
		int trueCount = 0;
		for(auto iter = selectedItems.begin(); iter != selectedItems.end(); ++iter)
		{
			if(iter->second == true)
				trueCount++;
		}
		
		if(trueCount >= 3)
		{
			_s->setSelectedIndex(0);
			KSModalAlert("", ML::key("Item Number <= 3").c_str(), 1,
								  "ok", NOPARAM);
		}
		else
		{
			isSaled = false;
			checks[0]->setIsVisible(true);
			selectedItems["exp"] = true;
			onExp();
		}
	}
	refreshPriceTable();
}

void GameShop::GAMEITEM1(CCObject* s)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	CCMenuItemToggle* _s = (CCMenuItemToggle*)s;
	
	int i = _s->getSelectedIndex();
	desc->setString(getItem1Desc().c_str());
	//선택 해지할 때
	if(i == 0)
	{
		isSaled = false;
		checks[3]->setIsVisible(false);
		selectedItems["item1"] = false;
		offItem1();
	}
	//선택할 때
	else if(i == 1)
	{
		int trueCount = 0;
		for(auto iter = selectedItems.begin(); iter != selectedItems.end(); ++iter)
		{
			if(iter->second == true)
				trueCount++;
		}
		
		if(trueCount >= 3)
		{
			_s->setSelectedIndex(0);
			KSModalAlert("", ML::key("Item Number <= 3").c_str(), 1,
								  "ok",NOPARAM);
		}
		else
		{
			isSaled = false;
			checks[3]->setIsVisible(true);
			selectedItems["item1"] = true;
			onItem1();
		}
	}
	refreshPriceTable();
}
void GameShop::GAMEITEM2(CCObject* s)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	CCMenuItemToggle* _s = (CCMenuItemToggle*)s;
	desc->setString(getItem2Desc().c_str());
	int i = _s->getSelectedIndex();
	//선택 해지할 때
	if(i == 0)
	{
		isSaled = false;
		checks[4]->setIsVisible(false);
		selectedItems["item2"] = false;
		offItem2();
	}
	//선택할 때
	else if(i == 1)
	{
		int trueCount = 0;
		for(auto iter = selectedItems.begin(); iter != selectedItems.end(); ++iter)
		{
			if(iter->second == true)
				trueCount++;
		}
		
		if(trueCount >= 3)
		{
			_s->setSelectedIndex(0);
			KSModalAlert("", ML::key("Item Number <= 3").c_str(), 1,
								  "ok", NOPARAM);
		}
		else
		{
			isSaled = false;
			checks[4]->setIsVisible(true);
			selectedItems["item2"] = true;
			onItem2();
		}
	}
	refreshPriceTable();
}
GameShop::~GameShop()
{
	
}