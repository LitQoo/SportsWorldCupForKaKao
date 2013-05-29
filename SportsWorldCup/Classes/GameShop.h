//
//  GameShop.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 12..
//
//

#ifndef __SportsWorldCup__GameShop__
#define __SportsWorldCup__GameShop__

#include "cocos2d.h"
#include "JsonBox.h"
#include "UIMediator.h"
#include <map>
#include <string>
using namespace std;
USING_NS_CC;
class GameShop : public CCSprite, public UIColleague
{
protected:
	static const float SALERATE;
	struct __POSITION{
		CCPoint time, shield, exp, gameItem1, gameItem2;
	}position;
	bool isSaled;
	CCSprite* thiz;
	UIMediator* mediator;
	CCLabelTTF* desc;
	void TIME(CCObject*);
	void SHIELD(CCObject*);
	void EXP(CCObject*);
	void GAMEITEM1(CCObject*);
	void GAMEITEM2(CCObject*);
	vector<CCSprite*> checks;
	virtual void onTime() = 0;
	virtual void offTime() = 0;
	virtual void onShield() = 0;
	virtual void offShield() = 0;
	virtual void onExp() = 0;
	virtual void offExp() = 0;
	virtual void onItem1() = 0;
	virtual void offItem1() = 0;
	virtual void onItem2() = 0;
	virtual void offItem2() = 0;
	virtual string getItem1Desc() = 0;
	virtual string getItem2Desc() = 0;
	
	int getTotalPrice()
	{
		const map<string, int>& itemPricesTable = priceTable();
		int totalPrice = 0;
		for(auto iter = selectedItems.begin(); iter != selectedItems.end(); ++iter)
		{
			if(iter->second)
				totalPrice += itemPricesTable.at(iter->first);
		}
		return totalPrice;
	}
	void refreshPriceTable();
	CCSprite* saleTable, *strokePrice;
	CCLabelBMFont* saledPrice, *originalPrice;
	CCLabelBMFont* price;
	
	map<string, int> levelLimits;
public:
	CCMenuItemToggle* times, *shields, *exps, *gameItem1, *gameItem2;
	map<string, bool> selectedItems;
	GameShop()
	{
		position.exp = ccp(53, 203);
		position.shield = ccp(132, 203);
		position.time = ccp(211, 203);
		position.gameItem1 = ccp(92, 126);
		position.gameItem2 = ccp(172, 126);
		isSaled = false;
		levelLimits["exp"] = 1;
		levelLimits["shield"] = 10;
		levelLimits["time"] = 25; // 25
		levelLimits["item1"] = 5;
		levelLimits["item2"] = 20;
	}
	int getRealPrice()
	{
		if(isSaled)
		{
			return getTotalPrice() * SALERATE;
		}
		else
		{
			return getTotalPrice();
		}
	}
	string getSelectedItems()
	{
		string items;
		for(auto iter = selectedItems.begin(); iter != selectedItems.end(); ++iter)
		{
			if(iter->second)
			{
				items += iter->first + "/";
			}
		}
		return items;
	}

	virtual ~GameShop();
	virtual bool init();
	virtual map<string, int> priceTable() = 0;
	void initMenu(string, string itemfile1, string itemfile2);
	void setMediator(UIMediator* m)
	{
		mediator = m;
	}
	void LEVELLIMIT(CCObject* m);
};


#endif /* defined(__SportsWorldCup__GameShop__) */
