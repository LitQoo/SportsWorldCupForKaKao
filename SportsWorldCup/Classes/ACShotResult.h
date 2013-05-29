//
//  ShotResult.h
//  ArcherWorldCupX
//
//  Created by apple on 12. 7. 9..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#ifndef ArcherWorldCupX_ShotResult_h
#define ArcherWorldCupX_ShotResult_h

#include "cocos2d.h"
#include <string.h>
#include "ACLQLabel.h"
#include "ACLQButton.h"
using namespace cocos2d;
using namespace std;


class ShotResult : public cocos2d::CCLayerColor	{
	LQLabel *pointlbl;
	LQLabel *bonuslbl;
	LQLabel *combolbl;
	LQLabel *totalPointlbl;
    
	CCSprite *target;
	CCLayer *targetLayer;
    
	CCLabelTTF *lastShot;
	
public:
	
    
    LQButton *nextButton;
    LQButton *exitButton;
//	CCMenu *menu;
    
	virtual bool initWithDelegate(cocos2d::CCObject *_target, SEL_MenuHandler _selector, SEL_MenuHandler _exitSelector);
	~ShotResult();
	
	void addShot(int point,CCPoint arrowPoint);
	void setCombo(int combo);
	void setBonus(float bonus);
	void setTotalPoint(int totalPoint);
	string stringWithFormat(const char* _Format, ...);
	void showHistroy();
	void setButtonTitle(const char* _title);
};

#endif
