//
//  SKFlag.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 5. 21..
//
//

#include "SKFlag.h"

#include "KS_Util.h"
void SKFlag::createSprite()
{
	obsAnimation = new CCAnimation;//::animation();
	obsAnimation->init();
	createAnimation(obsAnimation, KS_Util::stringWithFormat("sk_flag%d.png", rand()%2 + 1).c_str(), 1, 1, 55, 120);
	addFrames(obsAnimation->getFrames());
	this->startAnimation(1, 1, -1, 0, 0, 15, false, false);
	this->setAnchorPoint(ccp(0.5, 0.05f));
	obsSize = SIZE;
}