//
//  Coin.cpp
//  BasketWorldCup2
//
//  Created by ksoo k on 12. 11. 12..
//
//

#include "BS2Coin.h"
#include "BS2GameScreen.h"

Coin::Coin(int p) : price(p)
{
#if STATIC_OBJECT == 1
	coinBody = cpBodyNewStatic();
#else
	coinBody = cpBodyNew(INFINITY, INFINITY);
#endif
	coinShape = cpCircleShapeNew(coinBody, COIN_R, cpv(0,0));
	//CCLog("coin shape %x", coinShape);
	coinShape->collision_type = GameScreen::COIN_COLLISION;
	//coinBody->
	coinBody->v = cpv(0, absV);
}

void Coin::attachMe(cpSpace* space)
{
#if STATIC_OBJECT == 1
	cpSpaceAddStaticShape(space, coinShape);
	cpSpaceAddBody(space, coinShape->body);
#else
	cpSpaceAddShape(space, coinShape);
#endif

}