//
//  LQTableView.h
//  archervszombie
//
//  Created by LitQoo on 12. 12. 18..
//  Copyright (c) 2012년 LitQoo Inc. All rights reserved.
//

#ifndef archervszombie_LQTableView_cpp
#define archervszombie_LQTableView_cpp

#include "cocos2d.h"
#include "LQTableViewCell.h"
#include "FBConnector.h"
using namespace cocos2d;

typedef LQTableViewCell* (CCObject::*SEL_CallTableCell)(CCObject*);
#define callTableCell_selector(_SELECTOR) (SEL_CallTableCell)(&_SELECTOR)


class LQTableView : public cocos2d::CCLayerColor {
private:
    SEL_CallTableCell selector;
    CCObject *target;
    CCArray *cellArray;
    CCSize size;
    LQTableViewCell *firstCell;
    LQTableViewCell *lastCell;
    float touchy;
    long touchStartTime;
    float touchStartY;
    float moveSpeed;
public:
    int totalRow;
    bool isAnimated;
    virtual bool init(float width, float height, CCObject *_target,SEL_CallTableCell _selector);
    void loadTable(int);
    void moveList(float y);
    void moveAnimationUp();
    void moveAnimationDown();
    void moveAnimation();
    void reset();
    
    virtual void visit();
    CCArray* getCellArray(){return cellArray;}
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void registerWithTouchDispatcher();
	
	LQTableView()
	{
		totalRow = 0;
		setAnchorPoint(ccp(0.5f, 0.5f));
		m_bIsRelativeAnchorPoint = true;
	}
    virtual ~LQTableView();
    
};

#endif
