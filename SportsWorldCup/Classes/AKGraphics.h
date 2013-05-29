//
//  AKGraphics.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 2. 19..
//
//

#ifndef __SportsWorldCup__AKGraphics__
#define __SportsWorldCup__AKGraphics__

#include "AKBird.h"
#include "cocos2d.h"
#include <list>

//한글 테스트.
//편집이다.
USING_NS_CC;
struct AKGraphics
{
	vector<CCSprite*> guns; //
	enum GUNPOSITION{LEFT_GUN=0, CENTER_GUN=1, RIGHT_GUN=2} ;
	void showGun(enum GUNPOSITION g)
	{
		for(int i=0; i<guns.size(); i++)
		{
			guns[i]->setIsVisible(false);
		}
		guns[g]->setIsVisible(true);
	}
	list<Bird*> birds;
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
	vector<ccColor4F> dishColors;
	AKGraphics()
	{
		dishColors.push_back(ccc4FFromccc4B(ccc4f(100, 100, 100, 255))); // dishX
		dishColors.push_back(ccc4FFromccc4B(ccc4f(220, 68, 158, 255))); // dish1
		dishColors.push_back(ccc4FFromccc4B(ccc4f(255, 123, 19, 255))); // dish2
		dishColors.push_back(ccc4FFromccc4B(ccc4f(67, 178, 228, 255))); // dish3
		dishColors.push_back(ccc4FFromccc4B(ccc4f(100, 100, 100, 255))); // dish4
		dishColors.push_back(ccc4FFromccc4B(ccc4f(100, 100, 100, 255))); // dish5
		dishColors.push_back(ccc4FFromccc4B(ccc4f(100, 100, 100, 255))); // dish6
		dishColors.push_back(ccc4FFromccc4B(ccc4f(100, 100, 100, 255))); // dish7
		dishColors.push_back(ccc4FFromccc4B(ccc4f(100, 100, 100, 255))); // dish8
		
	}
};
#endif /* defined(__SportsWorldCup__AKGraphics__) */
