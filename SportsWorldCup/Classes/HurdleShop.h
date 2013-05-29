//
//  HurdleShop.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 11..
//
//

#ifndef __SportsWorldCup__HurdleShop__
#define __SportsWorldCup__HurdleShop__

#include "cocos2d.h"
#include "JsonBox.h"
#include "GameShop.h"
#include "Bridge.h"
#include "KS_Util.h"
USING_NS_CC;


class HurdleShop : public GameShop
{
protected:
	virtual void onTime(){KS::KSLog("% %", __FILE__, __FUNCTION__);}
	virtual void offTime(){KS::KSLog("% %", __FILE__, __FUNCTION__);}
	virtual void onShield(){KS::KSLog("% %", __FILE__, __FUNCTION__);}
	virtual void offShield(){KS::KSLog("% %", __FILE__, __FUNCTION__);}
	virtual void onExp(){KS::KSLog("% %", __FILE__, __FUNCTION__);}
	virtual void offExp(){KS::KSLog("% %", __FILE__, __FUNCTION__);}
	virtual void onItem1(){KS::KSLog("% %", __FILE__, __FUNCTION__);}
	virtual void offItem1(){KS::KSLog("% %", __FILE__, __FUNCTION__);}
	virtual void onItem2(){KS::KSLog("% %", __FILE__, __FUNCTION__);}
	virtual void offItem2(){KS::KSLog("% %", __FILE__, __FUNCTION__);}
	virtual string getItem1Desc(){
		return ML::key("nohighhurdle");
	}
	virtual string getItem2Desc(){
		return ML::key("wide");
	}
public:
	HurdleShop()
	{

	}
	virtual ~HurdleShop();
	virtual bool init();
	virtual map<string, int> priceTable(){
		map<string, int> 	 itemPrices;
		itemPrices["exp"] = 400;
		itemPrices["shield"] = 1000;
		itemPrices["time"] = 1600;
		itemPrices["item1"] = 700;
		itemPrices["item2"] = 1300;
		return itemPrices;
	}
	static HurdleShop* node(UIMediator *m)
	{
		HurdleShop *pRet = new HurdleShop();
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

};


#endif /* defined(__SportsWorldCup__HurdleShop__) */
