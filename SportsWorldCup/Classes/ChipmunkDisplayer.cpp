//
//  ChipmunkDisplayer.cpp
//  BasketWorldCup2
//
//  Created by ksoo k on 12. 11. 5..
//  Copyright (c) 2012년 LitQoo Inc. All rights reserved.
//
#include "ChipmunkDisplayer.h"

ChipmunkDisplayer::ChipmunkDisplayer()
{
	
}
ChipmunkDisplayer::~ChipmunkDisplayer()
{

}
void ChipmunkDisplayer::ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent)
{
	CCTouch* pTouch = (CCTouch*)pTouches->anyObject();
	CCPoint location = pTouch->locationInView();
	location = CCDirector::sharedDirector()->convertToGL(location);
	
}
void ChipmunkDisplayer::ccTouchesMoved(CCSet* pTouches, CCEvent* pEvent)
{
	
}
void ChipmunkDisplayer::ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent)
{
	
}
bool ChipmunkDisplayer::init()
{
	CCLayer::init();
		
	this->setIsTouchEnabled(true);
	
	return true;
}
void ChipmunkDisplayer::onEnterTransitionDidFinish()
{
	CCLayer::onEnterTransitionDidFinish();
}
void ChipmunkDisplayer::onExit()
{
	
	
	CCLayer::onExit();
}
