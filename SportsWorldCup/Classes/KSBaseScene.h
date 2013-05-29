//
//  KSBaseScene.h
//  BasketWorldCup2
//
//  Created by ksoo k on 12. 12. 3..
//  Copyright (c) 2012년 LitQoo Inc. All rights reserved.
//

#ifndef BasketWorldCup2_KSBaseScene_cpp
#define BasketWorldCup2_KSBaseScene_cpp

#include "cocos2d.h"
#include <algorithm>
using namespace std;
USING_NS_CC;

#define SCENE_FUNC(_A) static CCScene* scene() { CCScene* scene = CCScene::node(); _A * layer = _A::node(); scene->addChild(layer); return scene; }
class KSBaseScene : public CCLayerColor
{
public:
	KSBaseScene(){}
	virtual ~KSBaseScene(){}
	virtual bool init();
	// 배치노드, 딜레이 기간, 가로개수, 프레임수, 가로크기, 세로 크기, 무한반복여부, 리턴값은 스프라이트포인터
	pair<CCActionInterval*, CCSprite*> playAnimation(CCSpriteBatchNode* batchNode, float delay, int cols, int frames, int width_per_each, int height_per_each, bool isInfinite);
	pair<CCActionInterval*, CCSprite*> playAnimation(const char* spriteName, float delay, int cols, int frames, int width_per_each, int height_per_each, bool isInfinite);
	void deleteSprite(CCNode* _n)
	{
		_n->getParent()->removeChild(_n, true);
	}
	SCENE_FUNC(KSBaseScene);
	LAYER_NODE_FUNC(KSBaseScene);
};

class SceneUtil
{
public:
	static pair<CCActionInterval*, CCSprite*> playAnimation(CCSpriteBatchNode* batchNode, float delay, int cols, int frames, int width_per_each, int height_per_each, bool isInfinite)
	{
		CCSprite* Sprite = CCSprite::spriteWithBatchNode(batchNode, CCRectMake(0, 0, width_per_each, height_per_each));
		CCAnimation* Animation = CCAnimation::animation();
		Animation->setDelay(delay);
		
		for(int f = 1; f<=frames; f++) // 프레임 개수만큼.
		{
			int col = (f - 1 ) % cols;
			int row = (f - 1 ) / cols;
			Animation->addFrameWithTexture(batchNode->getTexture(), CCRectMake(width_per_each*col, height_per_each * row, width_per_each, height_per_each));
		}
		
		CCActionInterval* ac;
		if(isInfinite)
		{
			ac = CCRepeatForever::actionWithAction(CCAnimate::actionWithAnimation(Animation, false));
		}
		else
		{
			ac = CCAnimate::actionWithAnimation(Animation, false);
		}
		
		batchNode->addChild(Sprite);
		return pair<CCActionInterval*, CCSprite*>(ac, Sprite);
	}
	static pair<CCActionInterval*, CCSprite*> playAnimation(const char* spriteName, float delay, int cols, int frames, int width_per_each, int height_per_each, bool isInfinite)
	{
		CCAnimation* Animation = CCAnimation::animation();
		CCSprite* sprite = CCSprite::spriteWithFile(spriteName);
		Animation->setDelay(delay);
		for(int f = 1; f<=frames; f++) // 프레임 개수만큼.
		{
			int col = (f - 1 ) % cols;
			int row = (f - 1 ) / cols;
			Animation->addFrameWithTexture(sprite->getTexture(), CCRectMake(width_per_each*col, height_per_each * row, width_per_each, height_per_each));
			
		}
		
		CCActionInterval* ac;
		if(isInfinite)
		{
			ac = CCRepeatForever::actionWithAction(CCAnimate::actionWithAnimation(Animation, false));
		}
		else
		{
			ac = CCAnimate::actionWithAnimation(Animation, false);
		}
		
		CCSprite* ret = CCSprite::spriteWithTexture(sprite->getTexture(), CCRectMake(0, 0, width_per_each, height_per_each));
		return pair<CCActionInterval*, CCSprite*>(ac, ret);

	}
	static pair<CCActionInterval*, CCSprite*> playAnimation(const char* spriteName, float delay, int cols, int frames, int width_per_each, int height_per_each, int repeat)
	{
		CCAnimation* Animation = CCAnimation::animation();
		CCSprite* sprite = CCSprite::spriteWithFile(spriteName);
		Animation->setDelay(delay);
		for(int f = 1; f<=frames; f++) // 프레임 개수만큼.
		{
			int col = (f - 1 ) % cols;
			int row = (f - 1 ) / cols;
			Animation->addFrameWithTexture(sprite->getTexture(), CCRectMake(width_per_each*col, height_per_each * row, width_per_each, height_per_each));
			
		}
		
		CCActionInterval* ac = CCRepeat::actionWithAction(CCAnimate::actionWithAnimation(Animation), repeat);
		
		CCSprite* ret = CCSprite::spriteWithTexture(sprite->getTexture(), CCRectMake(0, 0, width_per_each, height_per_each));
		return pair<CCActionInterval*, CCSprite*>(ac, ret);
		
	}
};
#endif
