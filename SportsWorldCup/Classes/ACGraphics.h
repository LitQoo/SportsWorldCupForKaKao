//
//  ACGraphics.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 2. 22..
//
//

#ifndef __SportsWorldCup__ACGraphics__
#define __SportsWorldCup__ACGraphics__
#include "cocos2d.h"
USING_NS_CC;
struct ACGraphics
{
	static const CCPoint INIT_TARGET_POS;
	static const CCPoint LEFTWINDPOS;
	static const CCPoint RIGHTWINDPOS;
	static const CCPoint WINDVALUEPOS;
	CCLayer* windLayer;
		CCProgressTimer* leftWind, *rightWind;
		CCLabelBMFont* windValue;
	CCLabelBMFont* ateGoldFnt;
	CCSprite* watch1, *watch2, *watch3;
	CCSprite* leftHead, *rightHead;
	CCProgressTimer* zeroSprite;
	CCMenuItem* pause;
	CCLabelBMFont* scoreFnt;
	enum {COIN_WIDTH = 31, COIN_HEIGHT = 31};
};

#endif /* defined(__SportsWorldCup__ACGraphics__) */
