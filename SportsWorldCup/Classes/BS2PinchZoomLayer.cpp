//
//  PinchZoomLayer.cpp
//  BasketWorldCup2
//
//  Created by ksoo k on 12. 11. 6..
//  Copyright (c) 2012년 LitQoo Inc. All rights reserved.
//
#include "BS2PinchZoomLayer.h"

PinchZoomLayer::PinchZoomLayer()
{
	
}
PinchZoomLayer::~PinchZoomLayer()
{
	
}
void PinchZoomLayer::ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent)
{
	CCTouch* pTouch = (CCTouch*)pTouches->anyObject();
	CCPoint location = pTouch->locationInView();
	location = CCDirector::sharedDirector()->convertToGL(location);
	
}
void PinchZoomLayer::ccTouchesMoved(CCSet* pTouches, CCEvent* pEvent)
{
	//CCLog("%f %f", getPosition().x, getPosition().y);
	if(pTouches->count() == 2)
	{
		auto iter = pTouches->begin();
		CCTouch* touch1 = (CCTouch*)*iter++;
		CCTouch* touch2 = (CCTouch*)*iter;
		
		CCPoint position1 = CCDirector::sharedDirector()->convertToGL(touch1->locationInView());
		//CCLog("positon %f %f", position1.x, position1.y);
		//position1 = convertToWorldSpace(position1);
		CCPoint oldPosition1 = CCDirector::sharedDirector()->convertToGL(touch1->previousLocationInView());
		//oldPosition1 = convertToNodeSpace(oldPosition1);
		//oldPosition1 = convertToWorldSpace(oldPosition1);
		
		CCPoint position2 = CCDirector::sharedDirector()->convertToGL(touch2->locationInView());
		//position2 = convertToNodeSpace(position2);
		//position2 = convertToWorldSpace(position2);
		
		CCPoint oldPosition2 = CCDirector::sharedDirector()->convertToGL(touch2->previousLocationInView());
		//oldPosition2 = convertToWorldSpace(oldPosition2);
		//oldPosition2 = convertToNodeSpace(oldPosition2);
		
		float distance = ccpDistance(position1, position2);
		float oldDistance = ccpDistance(oldPosition1, oldPosition2);
		float factor = distance / oldDistance;
		//gameScreen->setScale(CLAMP(gameScreen->getScale()*factor, MIN_SCALE, MAX_SCALE));
		
		setScale(getScale()*factor);
		//sprite_->setScale(sprite_->getScale()*factor);
		
		// calculate translation
		
		
		CCPoint newPos1 = ccpAdd(position1, ccpMult(ccpSub(getPosition(), oldPosition1), factor));
		CCPoint newPos2 = ccpAdd(position2, ccpMult(ccpSub(getPosition(), oldPosition2), factor));

		setPosition(ccpMidpoint(newPos1, newPos2));
		//CCLog("pos %f %f, %f", getPosition().x, getPosition().y, getScale());
	}
	else if (pTouches->count() == 1)
	{
		// get touch locations
		auto iter = pTouches->begin();
		CCTouch* touch = (CCTouch*)*iter;
		
		CCPoint touchPosition = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
		CCPoint oldPosition = CCDirector::sharedDirector()->convertToGL(touch->previousLocationInView());
		
		// calculate difference in position
		CCPoint diff = ccpSub(touchPosition, oldPosition);
		setPosition(ccpAdd(getPosition(), diff));
	}
}
void PinchZoomLayer::ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent)
{
	
}
bool PinchZoomLayer::init()
{
	KSBaseScene::init();
	CCNode::setIsRelativeAnchorPoint(true);
	//setPosition(ccp(240, 160));
	//setPosition(ccp(240, 160));
	this->setIsTouchEnabled(true);
	
	
	return true;
	
}
void PinchZoomLayer::onEnterTransitionDidFinish()
{
	KSBaseScene::onEnterTransitionDidFinish();
}
void PinchZoomLayer::onExit()
{
	
	
	KSBaseScene::onExit();
}
