//
//  ACMiniMap.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 2. 25..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//

#ifndef SportsWorldCup_ACMiniMap_cpp
#define SportsWorldCup_ACMiniMap_cpp

#include "cocos2d.h"
USING_NS_CC;


class ACMiniMap : public CCSprite
{
private:
	CCSprite* back,*back2;
	CCSprite* pastPoint;
	CCSprite* nowPoint;
	CCLabelAtlas* score;
	void deleteSprite(CCNode* _n)
	{

		_n->getParent()->removeChild(_n, true);
	}
	void finish1to2();
	void finish2to1();
	void finish2to1_1();
public:
	static const float RADIUS;
	void pushHistroy(CCPoint direction, int point);
	ACMiniMap();
	virtual ~ACMiniMap();
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	LAYER_NODE_FUNC(ACMiniMap);
};

#endif
