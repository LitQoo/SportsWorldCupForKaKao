//
//  Hurdle.h
//  HurdleWorldCup
//
//  Created by ksoo k on 12. 5. 3..
//  Copyright (c) 2012³â __MyCompanyName__. All rights reserved.
//

#pragma once
#include "cocos2d.h"
#include "KS_Util.h"
USING_NS_CC;

typedef enum {HURDLE_A=0,HURDLE_B,HURDLE_C,HURDLE_D,HURDLE_TRACK_A,HURDLE_TRACK_B,HURDLE_TRACK_C,HURDLE_SAFARI_A,HURDLE_SAFARI_B,HURDLE_SAFARI_C,HURDLE_SAFARI_D} HurdleType;
class Hurdle : public CCObject
{
private:
	//virtual bool init();
public:
	int position;
	HurdleType type;
	int HurdleHeight();
	int HurdleWidth();
	CCSprite* getHurdleSprite(){return hurdleSprite;}
	Hurdle();
	virtual ~Hurdle()
	{
		//hurdleSprite->release();
	}
	void setType(HurdleType h){type = h;}
	void setPosition(int p){position = p;}
	void setHurdleSprite(CCSprite* spr){hurdleSprite = spr;}
private:
	CCSprite* hurdleSprite;
};

