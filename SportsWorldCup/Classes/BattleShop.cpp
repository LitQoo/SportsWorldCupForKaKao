//
//  BattleShop.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 5. 23..
//
//

#include "BattleShop.h"
#include "KS_Util.h"
#include "GameSystem.h"

bool BattleShop::init()
{
	GameShop::init();
	
	CCSprite* back = CCSprite::spriteWithFile("match_itemshop.png");
	
	addChild(back);
	thiz = back;
	//GameShop::initMenu("AK", "item_bird1.png", "item_large1.png");
	return true;
}
BattleShop::~BattleShop()
{
	
}