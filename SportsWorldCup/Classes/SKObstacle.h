//
//  SKObstacle.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 5. 10..
//
//

#ifndef __SportsWorldCup__SKObstacle__
#define __SportsWorldCup__SKObstacle__

#include "cocos2d.h"
USING_NS_CC;
#include "AdvanceSprite.h"

class SKObject : public AdvanceSprite
{
public:
	static int myOrder;
	SKObject()
	{
//		myOrder--;
//		if(myOrder < 0)
//		{
//			myOrder = 50000;
//		}
	}
	virtual ~SKObject(){
		delete obsAnimation;
	}
	
protected:
	
	float obsX;
	float obsDistance;
	bool isFirstSetX;
	bool isFirstSetD;
	float prevObsX;
	float prevObsDistance;
	bool isJudged;
	CCAnimation* obsAnimation;
	float obsSize;
public:
	void setObsX(float o);
	float getObsX();
	void setDistance(float d);
	float getDistance();
	float getPrevObsDistance();
	float getPrevObsX();
	float getObsSize(){return obsSize;}
	void setIsJudged(bool b){isJudged = b;}
	bool getIsJudged(){return isJudged;}
	virtual void createSprite() = 0;
	void createAnimation(CCAnimation* animation, string imgFile, int cols, int frames, int width_per_each, int height_per_each)
	{
		///	adHuman->add
		CCSprite* sprite = CCSprite::spriteWithFile(imgFile.c_str());
		for(int f = 1; f<=frames; f++) // 프레임 개수만큼.
		{
			int col = (f - 1 ) % cols;
			int row = (f - 1 ) / cols;
			animation->addFrameWithTexture(sprite->getTexture(), CCRectMake(width_per_each*col, height_per_each * row, width_per_each, height_per_each));
		}
	}
};
// 만들 때 obsAnimation new 하고 파괴될 때 파괴함.
class SKObstacle : public SKObject
{
public:
	static const float JUDGE_HEIGHT;
	bool deco; // deco 가 true 이면 충돌 검사를 하지 않음.
	SKObstacle()
	{
		isJudged = false;
		deco = false;
		obsSize = 0;
		isFirstSetD = isFirstSetX = true;
		
	}
	virtual ~SKObstacle(){}
};

class SKGrass : public SKObstacle
{
public:
	SKGrass()
	{
		deco = true;
	}
	virtual void createSprite();
};
class SKMoving : public SKObstacle
{
public:
	virtual void createSprite()
	{
	}
};

class SKSudden : public SKObstacle
{
public:
	virtual void createSprite()
	{
	}
};
class SKPense : public SKObstacle
{
public:
	static const int SIZE = 280;
	virtual void createSprite();
};
class SKBigStone : public SKObstacle
{
public:
	static const int SIZE = 145;
	virtual void createSprite();
};
class SKCuttingTree1 : public SKObstacle
{
public:
	static const int SIZE = 105;
	virtual void createSprite();
};
class SKCuttingTree2 : public SKObstacle
{
public:
	static const int SIZE = 50;
	virtual void createSprite();
};
class SKStone : public SKObstacle
{
public:
	static const int SIZE = 60;
	virtual void createSprite();
};
class SKTree : public SKObstacle
{
public:
	static const int SIZE = 30;
	virtual void createSprite();
};

class SKGround : public AdvanceSprite
{
public:
	float obsX;
	float obsDistance;
	CCAnimation* obsAnimation;
	virtual void createSprite()
	{
		obsAnimation = new CCAnimation;//::animation();
		obsAnimation->init();
//		createAnimation(obsAnimation, "sk_floor.png", 1, 1, 480, 100);
		
		static int i=0;
		i++;
		if(i % 2 == 0)
		{
			createAnimation(obsAnimation, "sk_floor.png", 1, 1, 480, 1);
		}
		else
		{
			createAnimation(obsAnimation, "sk_tran.png", 1, 1, 480, 1);
		}
			
		
		addFrames(obsAnimation->getFrames());
		this->startAnimation(1, 1, -1, 0, 0, 15, false, false);
		this->setAnchorPoint(ccp(0.5, 0.f));
	}
	void createAnimation(CCAnimation* animation, string imgFile, int cols, int frames, int width_per_each, int height_per_each)
	{
		///	adHuman->add
		CCSprite* sprite = CCSprite::spriteWithFile(imgFile.c_str());
		for(int f = 1; f<=frames; f++) // 프레임 개수만큼.
		{
			int col = (f - 1 ) % cols;
			int row = (f - 1 ) / cols;
			animation->addFrameWithTexture(sprite->getTexture(), CCRectMake(width_per_each*col, height_per_each * row, width_per_each, height_per_each));
		}
	}
};
#endif /* defined(__SportsWorldCup__SKObstacle__) */
