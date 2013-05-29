//
//  BasketShop.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 11..
//
//

#include "BasketShop.h"
bool BasketShop::init()
{
	CCSprite::init();
	GameShop::initMenu("BS", "item_tipin1.png", "item_longguide1.png");
	
	
	return true;
}

BasketShop::~BasketShop()
{
	
}