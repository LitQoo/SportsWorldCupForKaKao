//
//  ChipmunkDisplayer.h
//  BasketWorldCup2
//
//  Created by ksoo k on 12. 11. 5..
//  Copyright (c) 2012년 LitQoo Inc. All rights reserved.
//

#ifndef BasketWorldCup2_ChipmunkDisplayer_cpp
#define BasketWorldCup2_ChipmunkDisplayer_cpp

#include "cocos2d.h"

#include "chipmunk.h"
#include "drawSpace.h"
USING_NS_CC;

#define SCENE_FUNC(_A) static CCScene* scene() { CCScene* scene = CCScene::node(); _A * layer = _A::node(); scene->addChild(layer); return scene; }
class ChipmunkDisplayer : public CCLayer, public CCTextFieldDelegate
{
private:
	void draw()
	{
		drawSpaceOptions options = {
			1, // drawHash
			0, // drawBBs,
			1, // drawShapes
			4.0, // collisionPointSize
			4.0, // bodyPointSize,
			2.0 // lineThickness
		};
		//glPushMatrix();
		
		glPushMatrix();
		
		glScalef(CC_CONTENT_SCALE_FACTOR(), CC_CONTENT_SCALE_FACTOR(), 1.0f);
		drawSpace(mySpace, &options);
		
		glPopMatrix();
		
		
		//glScale
		//glPopMatrix();
	}
public:
	cpSpace* mySpace;
	ChipmunkDisplayer();
	virtual ~ChipmunkDisplayer();
	virtual void ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent);
	virtual void ccTouchesMoved(CCSet* pTouches, CCEvent* pEvent);
	virtual void ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent);
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	SCENE_FUNC(ChipmunkDisplayer);
	LAYER_NODE_FUNC(ChipmunkDisplayer);
};

#endif
