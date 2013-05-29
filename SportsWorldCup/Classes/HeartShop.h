//
//  HeartShop.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 11..
//
//

#ifndef __SportsWorldCup__HeartShop__
#define __SportsWorldCup__HeartShop__


#include "cocos2d.h"
#include "JsonBox.h"
#include "UIMediator.h"
#include <map>
#include "KSoundEngine.h"
using namespace std;
USING_NS_CC;


class HeartShop : public CCSprite, public UIColleague
{
private:
	struct __POSITION{
		CCPoint pier[6];
		CCPoint pierPrice[6];
	}position;
	CCSprite* thiz;
	UIMediator* mediator;
	map<string, int> rubies;
	void PIER1(CCObject*);
	void PIER2(CCObject*);
	void PIER3(CCObject*);
	void PIER4(CCObject*);
	void PIER5(CCObject*);
	void PIER6(CCObject*);
	void CLOSE(CCObject*)
	{
		KSoundEngine::sharedEngine()->playSound("select.mp3");
		mediator->showRankWithMenu();
	}
public:
	HeartShop()
	{
		position.pier[0] = ccp(71, 199);
		position.pier[1] = ccp(192, 199);
		position.pier[2] = ccp(71, 124);
		position.pier[3] = ccp(192, 124);
		position.pier[4] = ccp(71, 50);
		position.pier[5] = ccp(192, 50);
		position.pierPrice[0] = ccp(80, 181);
		position.pierPrice[1] = ccp(200, 181);
		position.pierPrice[2] = ccp(80, 107);
		position.pierPrice[3] = ccp(200, 107);
		position.pierPrice[4] = ccp(80, 32);
		position.pierPrice[5] = ccp(200, 32);
	}
	virtual ~HeartShop();
	virtual bool init();
	static HeartShop* node(UIMediator *m)
	{
		HeartShop *pRet = new HeartShop();
		pRet->setMediator(m);
		if (pRet && pRet->init())
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			delete pRet;
			pRet = NULL;
			return NULL;
		}
	}
	void setMediator(UIMediator* m)
	{
		mediator = m;
	}
};


#endif /* defined(__SportsWorldCup__HeartShop__) */
