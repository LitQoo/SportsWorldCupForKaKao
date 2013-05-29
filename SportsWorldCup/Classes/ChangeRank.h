//
//  ChangeRank.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 5. 24..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//

#ifndef SportsWorldCup_ChangeRank_cpp
#define SportsWorldCup_ChangeRank_cpp

#include "cocos2d.h"
USING_NS_CC;


class ChangeRank : public CCLayer, public CCTextFieldDelegate
{
public:
	ChangeRank();
	virtual ~ChangeRank();
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	LAYER_NODE_FUNC(ChangeRank);
	virtual void registerWithTouchDispatcher();
private:
	CCSprite* kim4, *pink5, *upArrow, *downArrow;
	void moving();
};

#endif
