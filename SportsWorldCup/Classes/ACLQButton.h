//
//  LQButton.h
//  ArcherWorldCupX
//
//  Created by Kim HyunSoo on 12. 8. 8..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#ifndef ArcherWorldCupX_LQButton_h
#define ArcherWorldCupX_LQButton_h
#include "cocos2d.h"
#include "ACLQLabel.h"
#include <string.h>


using namespace cocos2d;
using namespace std;


class LQButton : public LQLabel {
    
    CCObject* target;
    SEL_MenuHandler selector;
    
    public:
    virtual bool init();
    ~LQButton();
    
    void setTargetAndSelector(CCObject *_target, SEL_MenuHandler _selector);
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void registerWithTouchDispatcher();
    
};


#endif
