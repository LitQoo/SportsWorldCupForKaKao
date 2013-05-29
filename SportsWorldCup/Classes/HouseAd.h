//
//  HouseAd.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 4. 12..
//
//

#ifndef __SportsWorldCup__HouseAd__
#define __SportsWorldCup__HouseAd__

#include "CoverLayer.h"
#include "cocos2d.h"
USING_NS_CC;
#include "PriorityMenu.h"
#include "JsonBox.h"

class HouseAd : public CoverLayer
{
private:
	PriorityMenu* prioMenu;
	string store;
	string aid;
	void APP(CCObject*);
	void CLOSE(CCObject*);
	void finishAddCpiEvent(JsonBox::Object js);
public:
	virtual bool init();
	LAYER_NODE_FUNC(HouseAd);
	
	void setAd(CCSprite* img, CCSprite* selectedimg,string store, string aid);
};

#endif /* defined(__SportsWorldCup__HouseAd__) */
