//
//  ACGraphics.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 2. 22..
//
//

#ifndef __SportsWorldCup__SKGraphics__
#define __SportsWorldCup__SKGraphics__
#include "cocos2d.h"
USING_NS_CC;
struct SKGraphics
{
	CCMenuItem* pause;
	CCProgressTimer* zeroSprite;
	CCProgressTimer* feverGauge;
	CCLabelBMFont* scoreFnt;
	CCLabelBMFont* timeFnt;
	CCSprite* watch1, *watch2, *watch3;
	CCSprite* leftAim;
	CCSprite* rightAim;
	CCLabelBMFont* ateGoldFnt;
	CCSprite* disableLeft,*disableRight;
	enum {AIM_FRAME = 4, AIM_SIZE = 55};
	enum {COIN_WIDTH = 31, COIN_HEIGHT = 31};
	SKGraphics()
	{
	}
};

#endif /* defined(__SportsWorldCup__ACGraphics__) */
