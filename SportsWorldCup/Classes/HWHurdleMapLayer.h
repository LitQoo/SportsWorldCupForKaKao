//
//  HurdleMapLayer.h
//  HurdleWorldCup
//
//  Created by ksoo k on 12. 4. 6..
//  Copyright (c) 2012≥‚ __MyCompanyName__. All rights reserved.
//
#pragma once
#include "CCLayer.h"
#include "cocos2d.h"
#include "HWHurdle.h"
#include <list>
using namespace std;
using namespace cocos2d;





class Flag : public CCObject 
{
public:
	int type;
	enum {STARTLINE_1,MIDDLELINE_1,FINISHLINE_1,STARTLINE_2,MIDDLELINE_2,FINISHLINE_2} flagType;
	int position;
	void setPosition(int p){position = p;}
};


enum GroundType {ATYPE=0,BTYPE,CTYPE,DTYPE,TTYPE,SAFARI_A,SAFARI_B} ; // ∂• ≈∏¿‘

class Obstacle : public CCObject
{
public:
	int start;
	int length;
	int height;
	int drawCount;
	enum GroundType type; // ∂• ≈∏¿‘
	list<CCSprite*> sprites;
	CCMutableArray<Hurdle*>* hurdles;

	//Hurdle hurdle;
	//static int MIN_HURDLE_MARGIN;
	//static int MIN_START_FROM;
	//static int HURDLE_RANGE;
public:
	void setHeight(int h){height = h;}
	int getHeight(){return height;}
	void setLength(int l){length = l;}
	int getLength(){return length;}




	void setStart(int s){start = s;}
	int getStart(){return start;}
	CCMutableArray<Hurdle*>* getHurdles(){return hurdles;}

	Obstacle()
	{
		hurdles = new CCMutableArray<Hurdle*>();
		drawCount = 0;
	}
	virtual ~Obstacle()
	{
		hurdles->release();

	}
	
};

struct HWGameInformation;
class HurdleMapLayer : public CCLayer
{
protected:
	HWGameInformation* pInfo;
	CCMutableArray<Obstacle*>* Obstacles;
	int patternWidth;
	int startCreationPosition;
	int prevHeight;
	int prevCreationPosition;
	CCPoint prevPosition;
	
	int MIN_HURDLE_MARGIN;
	int MIN_START_FROM;
	int HURDLE_RANGE;
	
	static const int STARTLENGTH = 10; // 시작 트랙의 길이
	int MINLENGTH;// = 9;	   // 트랙최소길이
	int LENGTHRANGE;// = 35; // 트랙길이의 유동성
	int MINHEIGHT;// = 20;  // 트랙의 최소 높이

	int MARGINRANGE;// 트랙간의 -- 의 유동성 / 커질 수록 난이도 높아짐...
	int MAXBELOWHEIGHTDIFF;// 트랙간의 최대 I 차이        // 커질 수록 난이도가 높아진다.
	
	// 망나니 모드 변수
	int MAXBELOWHEIGHTDIFF_2;// 트랙간의 최대 I 차이      // 커질 수록 난이도가 높아진다.
	int LENGTHRANGE_2;// = 3; // 트랙길이의 유동성

	int MARGINRANGE_2;// 트랙간의 여백의 유동성
	
protected:
	virtual void onEnterTransitionDidFinish();
	int getMinHurdleMargin();
	int getNormalHurdleMargin();
	int getCrazyTrackMargin(float diff);
	int getTrackMargin();
private:
	virtual void addHurdles(Obstacle* obs) = 0;
	
	virtual CCSpriteBatchNode* getMapBatch() = 0;
	virtual void cachingPList() = 0;
public:
	void setInfo(HWGameInformation* gi){pInfo = gi;}
	virtual void createScreen() = 0;
	void setBalance(int combo);
	int getPatternWidth(){return patternWidth;}
	void setPatternWidth(int arg){patternWidth = arg;}
	CCPoint getPrevPosition(){return prevPosition;}
	void setPrevPosition(CCPoint arg){prevPosition = arg;}
	CCMutableArray<Obstacle*>* getObstacles(){return Obstacles;}

public:
	bool reviveEvent;
	int revivePosition;
	HurdleMapLayer()
	{
		
		revivePosition = 0;
		reviveEvent = false;
		MIN_HURDLE_MARGIN = 270; // 허들 간의 최소 허들 간격
		MIN_START_FROM = 350; // 끝과 시작부분의 근접선에선 허들이 나오지 않음
		HURDLE_RANGE = 200; // 허들 간격의 유동성.
	}
	virtual ~HurdleMapLayer();
	
	bool createObstacle(float scale);
	virtual void createSprite(float scale) = 0;
	void removeSprite();
	int getFirstAttitude();
	
	virtual int BEGIN_HEIGHT();

	/////////////////////////////////////////////////////////
	//abstract method
	virtual GroundType obsTypeDeterminer() = 0;
	virtual HurdleType hurdleTypeDeterminer(GroundType gt) = 0;
	virtual void setBelowGround(Obstacle* t) = 0;
	virtual void setTrackSprite(Obstacle* t) = 0;
	virtual void setHurdleSprite(Obstacle* t) = 0;
	virtual void majorIsTrack(CCMutableArray<Obstacle*>* newObject);
	virtual void majorIsHurdle(CCMutableArray<Obstacle*>* newObject);
	//abstract method

	void setPosition(CCPoint position);
};

