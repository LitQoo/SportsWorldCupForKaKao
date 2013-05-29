//
//  GoToHome.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 19..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//

#ifndef SportsWorldCup_GoToHome_cpp
#define SportsWorldCup_GoToHome_cpp

#include "cocos2d.h"
#include "JsonBox.h"
USING_NS_CC;

#define SCENE_FUNC(_A) static CCScene* scene() { CCScene* scene = CCScene::node(); _A * layer = _A::node(); scene->addChild(layer); return scene; }
class GoToHome : public CCLayer, public CCTextFieldDelegate
{
private:
	void finishGetWeek(JsonBox::Object js);
	void RETRY();
public:
	GoToHome();
	virtual ~GoToHome();
//	virtual void ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent);
//	virtual void ccTouchesMoved(CCSet* pTouches, CCEvent* pEvent);
//	virtual void ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent);
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void registerWithTouchDispatcher();
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	SCENE_FUNC(GoToHome);
	LAYER_NODE_FUNC(GoToHome);
};

#endif
