//
//  ChangeRank.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 5. 24..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//

#ifndef SportsWorldCup_DailyCheck_cpp
#define SportsWorldCup_DailyCheck_cpp

#include "cocos2d.h"
USING_NS_CC;


class DailyCheck : public CCLayer, public CCTextFieldDelegate
{
public:
	DailyCheck();
	virtual ~DailyCheck();
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	LAYER_NODE_FUNC(DailyCheck);
	virtual void registerWithTouchDispatcher();
private:
};

#endif
