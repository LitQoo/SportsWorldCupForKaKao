//
//  BallShape.h
//  BasketWorldCup2
//
//  Created by ksoo k on 12. 11. 7..
//
//

#ifndef __BasketWorldCup2__BallShape__
#define __BasketWorldCup2__BallShape__
#include "chipmunk.h"
#include "cocos2d.h"
#include "BS2ChipSprites.h"

USING_NS_CC;
class BallBase
{
private:
	cpShape* ballShape;
	bool goaled;
	bool reflected;
	int reflectCount;
	bool grounding;
	bool shoot;
	bool isDoubleShot;
	CCPoint firstPosition;
	float goalTime;
	bool isJudged;
	int groundCount;
	bool crashable;
public:
	bool getIsJudged(){return isJudged;}
	void setIsJudged(bool b){isJudged = b;}
	void setGoalTime(float t) { goalTime = t; }
	float getGoalTime(){return goalTime;}
	int getRefelectCount(){return reflectCount;}
	void setFirstPosition(CCPoint _t){firstPosition = _t;}
	CCPoint getFirstPosition(){return firstPosition;}
	CCPoint previousPosition;
	BallBase(){ballShape = 0; goaled = false; grounding = false; reflected = false;
		shoot = false;
		reflectCount = 0;
		isJudged = false;
		groundCount = 0;
		isDoubleShot = false;
     
	}
	// 팁인 가능한 공인지 판별
	bool isTipInAble(){
		return goaled == false && grounding == false && reflected == true && shoot;
		//return ;
	}
	void setCrashable(){crashable = true;}
	bool getCrashable(){return crashable;}
	void setIsDoubleShot(){isDoubleShot = true;}
	bool getIsDoubleShot(){return isDoubleShot;}
	void setShoot(){shoot = true;}
	void setReflected(int n){reflected = true; reflectCount+=n;}
	void setGrounding(){grounding = true; groundCount++;}
	bool getShoot(){return shoot;}
	bool getReflected(){return reflected;}
	bool getGrounding(){return grounding;}
	int getGroundCount(){return groundCount;}
	void setPosition(CCPoint pos) { ballShape->body->p = {pos.x, pos.y}; }
	void setShape(cpShape* _s) { ballShape = _s; }
	void setChipSprites(ChipSprites* _s){ballShape->data = _s;}
	void setGoaled(){goaled = true;}
	bool getGoaled(){return goaled;}
	cpShape* getShape(){return ballShape;}
	ChipSprites* getChipSprites(){return (ChipSprites*)ballShape->data;}
	void create(float _m = 150, float _r = 14)
	{
		cpBody* _ballBody = cpBodyNew(_m, cpMomentForCircle(_m, 0, _r, cpv(0,0))/3.f);
		
		//_ballBody->v_limit = 10;
		{
			ballShape = cpCircleShapeNew(_ballBody, _r, cpv(0,0));
			
			ballShape->u = 0.01f;
			ballShape->e = 1.f; // 1.f //##
//			controlableBall = staticBallShape;
//			CCSprite* ballSprite = CCSprite::spriteWithFile("ball.png");
//			addChild(ballSprite);
//			staticBallShape->data = ballSprite;
//			ballSprite->setPosition(_ballPos);
			
//			CCPoint _ballPos = selectBallPosition(playInfo->stage.getVar());
//			_ballBody->p = {_ballPos.x, _ballPos.y};
			
//			cpSpaceAddStaticShape(space, staticBallShape);
		}

	}
};

#endif /* defined(__BasketWorldCup2__BallShape__) */
