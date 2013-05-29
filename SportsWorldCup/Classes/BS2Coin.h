//
//  Coin.h
//  BasketWorldCup2
//
//  Created by ksoo k on 12. 11. 12..
//
//

#ifndef __BasketWorldCup2__Coin__
#define __BasketWorldCup2__Coin__

#include "chipmunk.h"
#include "cocos2d.h"
#define STATIC_OBJECT 0
USING_NS_CC;
class Coin
{
private:
	int price;
	enum {COIN_R = 15};
public:
	enum {absV = 3};
	const Coin& operator=(const Coin& _c)
	{
		price = _c.price;
		coinShape = _c.coinShape;
		coinBody = _c.coinBody;
		return *this;
	}
	Coin(){}
	Coin(int p);
	void setPrice(int p) { price = p; }
	int getPrice(){return price;}
	cpShape* coinShape;
	cpBody* coinBody;
	void attachMe(cpSpace* space);
	void setPosition(CCPoint pos) { coinBody->p = cpv(pos.x, pos.y); }
};

#endif 