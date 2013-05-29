//
//  HurdleShop.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 11..
//
//

#include "HurdleShop.h"
bool HurdleShop::init()
{
	GameShop::init();
	GameShop::initMenu("HW", "item_highhurdle1.png", "item_wideview1.png");
	
	return true;
}

HurdleShop::~HurdleShop()
{
	
}