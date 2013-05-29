//
//  PinchZoomLayer.h
//  BasketWorldCup2
//
//  Created by ksoo k on 12. 11. 6..
//  Copyright (c) 2012년 LitQoo Inc. All rights reserved.
//

#ifndef BasketWorldCup2_PinchZoomLayer_cpp
#define BasketWorldCup2_PinchZoomLayer_cpp

#include "cocos2d.h"
#include "KSBaseScene.h"
USING_NS_CC;

#define SCENE_FUNC(_A) static CCScene* scene() { CCScene* scene = CCScene::node(); _A * layer = _A::node(); scene->addChild(layer); return scene; }
class PinchZoomLayer : public KSBaseScene, public CCTextFieldDelegate
{
private:
	
public:
	PinchZoomLayer();
	virtual ~PinchZoomLayer();
	virtual void ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent);
	virtual void ccTouchesMoved(CCSet* pTouches, CCEvent* pEvent);
	virtual void ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent);
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	SCENE_FUNC(PinchZoomLayer);
	LAYER_NODE_FUNC(PinchZoomLayer);
protected:
	CCRect getValidArea()
	{
		CCRect _b = boundingBox();
		_b.size.width = 480.f / getScale();
		_b.size.height = 320.f / getScale();
		return _b;
	}
};

#endif
