//
//  ClayShop.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 11..
//
//

#include "ClayShop.h"
#include "KS_Util.h"
#include "GameSystem.h"


bool ClayShop::init()
{
	GameShop::init();
	
	GameShop::initMenu("AK", "item_bird1.png", "item_large1.png");
	return true;
}
ClayShop::~ClayShop()
{
	
}