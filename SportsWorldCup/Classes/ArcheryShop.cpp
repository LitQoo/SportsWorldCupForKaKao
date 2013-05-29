//
//  ArcheryShop.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 11..
//
//

#include "ArcheryShop.h"
bool ArcheryShop::init()
{
	CCSprite::init();
	GameShop::initMenu("AC", "item_wind1.png", "item_vibration1.png");
	
	
	return true;
}

ArcheryShop::~ArcheryShop()
{
	
}