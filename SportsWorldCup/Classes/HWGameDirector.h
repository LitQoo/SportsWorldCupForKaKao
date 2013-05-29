//
//  HWGameDirector.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 2. 1..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//

#ifndef SportsWorldCup_HWGameDirector_cpp
#define SportsWorldCup_HWGameDirector_cpp

#include "cocos2d.h"
USING_NS_CC;

#define SCENE_FUNC(_A) static CCScene* scene() { CCScene* scene = CCScene::node(); _A * layer = _A::node(); scene->addChild(layer); return scene; }
class HWGameDirector : public CCLayer, public CCTextFieldDelegate
{
public:
	HWGameDirector();
	virtual ~HWGameDirector();
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	SCENE_FUNC(HWGameDirector);
	LAYER_NODE_FUNC(HWGameDirector);
};

#endif
