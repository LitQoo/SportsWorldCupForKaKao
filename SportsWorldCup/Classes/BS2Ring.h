//
//  Ring.h
//  BasketWorldCup2
//
//  Created by ksoo k on 12. 10. 31..
//
//

#ifndef __BasketWorldCup2__Ring__
#define __BasketWorldCup2__Ring__

#include "chipmunk.h"
#include "cocos2d.h"
USING_NS_CC;
class Ring
{
public:
	Ring(){firstPosition = true;}
	void init(float backBoardLength, float groundLength, float ringSize, float e, float u);
	void attachMe(cpSpace* space);
	cpShape* getBackBoardShape() { return backBoardShape; }
	//cpBody* getBackBoardBody(){return backBoardBody;}
	cpVect getBackBoardBodyPos(){return backBoardBody->p;}
	cpVect getPrevBackBoardBodyPos(){return previousPosition;}
	cpShape* getGround(){return groundShape;}
	cpShape* getRing1(){return ring1Shape;}
	cpShape* getRing2(){return ring2Shape;}
	cpShape* getRing3(){return ring3Shape;}
	float getRingSize(){return ringSize;}
	void setPosition(cpVect p)
	{
		if(firstPosition)
		{
			previousPosition = p;
			backBoardBody->p = p;
			firstPosition = false;
		}
		else
		{
			previousPosition = backBoardBody->p;
			backBoardBody->p = p;
		}

	}
	cpVect getPosition(){return backBoardBody->p;}
private:
	static const float BACKBOARD_E;
	float ringSize;
	cpBody* backBoardBody;
	cpShape* backBoardShape;
	bool firstPosition;
	cpShape* groundShape;
	cpShape* ring1Shape;
	cpShape* ring2Shape;
	cpShape* ring3Shape;
	cpShape* subObject;
	cpVect previousPosition;
};

#endif /* defined(__BasketWorldCup2__Ring__) */
