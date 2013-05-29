//
//  RubyShop.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 11..
//
//

#ifndef __SportsWorldCup__RubyShop__
#define __SportsWorldCup__RubyShop__

#include "cocos2d.h"
#include "JsonBox.h"
#include "UIMediator.h"
#include <map>
#include "KSoundEngine.h"
using namespace std;
USING_NS_CC;


class RubyShop : public CCSprite, public UIColleague
{
private:
	struct __POSITION{
		CCPoint pier[6];
		CCPoint pierPrice[6];
	}position;
	CCSprite* thiz;
	UIMediator* mediator;
	map<string, int> cashs;
	
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
	void showLoading();
	CCSprite* loading;
	CCNode* disableTouch;
public:
	RubyShop();
	virtual ~RubyShop();
	virtual bool init();
	static RubyShop* node(UIMediator *m)
	{
		RubyShop *pRet = new RubyShop();
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
	void finishProcess();
	void setMediator(UIMediator* m)
	{
		mediator = m;
	}
	void finishedPurchase(string _itemName, int _resultCode);
};


#endif /* defined(__SportsWorldCup__RubyShop__) */
