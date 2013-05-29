//
//  Bird.h
//  AngryKeeper
//
//  Created by ksoo k on 12. 9. 10..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#ifndef AngryKeeper_Bird_h
#define AngryKeeper_Bird_h

#include <vector>
#include "cocos2d.h"
//
USING_NS_CC;
using namespace std;




class AngryGameScene;

class Bird : public CCSprite
{
private:
	bool isGoal;
	float a, b, c;	
	//int birdNumber;
protected:
	bool toRight;
public:
	bool getRight(){return toRight;}
	enum {FRAME_NUM = 4, BIRD_WIDTH = 19, BIRD_HEIGHT = 12};
	enum {FAKEFRAME_NUM = 11, FAKE_WIDTH = 42, FAKE_HEIGHT = 20};
	vector<CCPoint> path;
	float velocity;
	
	float startTime;
	AngryGameScene* gameDirector;
	CCAnimation* dishAnimation;
	float currentX;
public:
	virtual void damaged() = 0;
	void initABC();
	virtual void initFlying(bool, bool) = 0;
	Bird(AngryGameScene* d, float velo);
	virtual ~Bird();
	double getDistance(CCPoint a, CCPoint b)
	{
		return sqrt( (a.x-b.x)*(a.x-b.x) + (a.y - b.y)*(a.y - b.y) );
	}
	void setNextPath(float dt);
	bool getIsGoal(){return isGoal;}
	float pathFunction(float x){return a*x*x + b*x + c;}
	void createAnimation(CCAnimation* animation, string imgFile, int cols, int frames, int width_per_each, int height_per_each)
	{
		///		adHuman->add
		CCSprite* sprite = CCSprite::spriteWithFile(imgFile.c_str());
		for(int f = 1; f<=frames; f++) // 프레임 개수만큼.
		{
			int col = (f - 1 ) % cols;
			int row = (f - 1 ) / cols;
			animation->addFrameWithTexture(sprite->getTexture(), CCRectMake(width_per_each*col, height_per_each * row, width_per_each, height_per_each));
		}
	}
};

typedef Bird Flying;
class Dish : public Flying
{
private:
	int dishNumber;
public:
	virtual void damaged(){
		//gameDirector->onShotBird();
	}
	int getDishNumber(){return dishNumber;}
	void setDishNumber(int r){dishNumber = r;}
	Dish(AngryGameScene* d, float velo) : Flying(d, velo) { }
	static Dish* sprite(AngryGameScene* director, float velo)
	{
		Dish* e = new Dish(director, velo);
		e->cocos2d::CCSprite::init();
		e->autorelease();
		return e;
	}
	virtual void initFlying(bool toR, bool zeroMode);
};

class NotDish : public Flying
{
public:
	virtual void damaged();
	NotDish(AngryGameScene* d, float velo) : Flying(d, velo) { }
	static NotDish* sprite(AngryGameScene* director, float velo)
	{
		NotDish* e = new NotDish(director, velo);
		e->cocos2d::CCSprite::init();
		e->autorelease();
		return e;
	}
	virtual void initFlying(bool toR, bool zeroMode);
};

#endif
