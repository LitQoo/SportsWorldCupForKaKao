//
//  FirstHurdleMapLayer.h
//  HurdleWorldCup
//
//  Created by ksoo k on 12. 4. 30..
//  Copyright (c) 2012³â __MyCompanyName__. All rights reserved.
//
#pragma once
#include "HWHurdleMapLayer.h"

class FirstHurdleMapLayer : public HurdleMapLayer
{
private:
	CCSpriteBatchNode* mapBatch;
public:
	virtual ~FirstHurdleMapLayer(){}
	
	FirstHurdleMapLayer(){}
private:
	virtual void onEnterTransitionDidFinish();
	virtual void createSprite(float scale);
	virtual int createHurdlePattern(Obstacle* obs, int type, int _begin, int _end);
public:
	static CCScene* scene()
	{
		CCScene * scene = CCScene::node();
		FirstHurdleMapLayer* layer = FirstHurdleMapLayer::node();
		scene->addChild(layer);
		return scene;
	}

	// implement the "static node()" method manually
	LAYER_NODE_FUNC(FirstHurdleMapLayer);

private: // implementation
	virtual void addHurdles(Obstacle* obs);

	virtual void createScreen();
	virtual CCSpriteBatchNode* getMapBatch(){return mapBatch;}
	virtual void cachingPList();
public: // implementation
	virtual GroundType obsTypeDeterminer();
	virtual HurdleType hurdleTypeDeterminer(GroundType gt);
	virtual void setBelowGround(Obstacle* t);
	virtual void setTrackSprite(Obstacle* t);
	virtual void setHurdleSprite(Obstacle* t);
};
