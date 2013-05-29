//  CCScrollLayer.cpp
//  Museum
//
//  Created by GParvaneh on 29/12/2010.
//  Copyright 2010. All rights reserved.
//  Ported to C++ by Lior Tamam on 03/04/2011
#include "CCScrollLayer.h"

CCScrollLayer* CCScrollLayer::layerWithLayers(CCMutableArray<CCLayer*> *layers, int widthOffset)
{	
	CCScrollLayer *pRet = new CCScrollLayer();
	if (pRet && pRet->initWithLayers(layers, widthOffset))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet)
	return NULL;
}

bool CCScrollLayer::initWithLayers(CCMutableArray<CCLayer*> *layers, int widthOffset)
{	
	if (CCLayer::init())
	{		
		// Make sure the layer accepts touches
		CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this,0,true);
		
		// Set up the starting variables
		if(!widthOffset)
		{
			widthOffset = 0;
		}	
		currentScreen = 1;
		
		// offset added to show preview of next/previous screens
		scrollWidth  = (int)CCDirector::sharedDirector()->getWinSize().width - widthOffset;
		scrollHeight = (int)CCDirector::sharedDirector()->getWinSize().height;
		startWidth = scrollWidth;
		startHeight = scrollHeight;
		
		// Loop through the array and add the screens
		unsigned int i;
		for (i=0; i<layers->count(); i++)
		{
			CCLayer* l = layers->getObjectAtIndex(i);
			l->setAnchorPoint(ccp(0,0));
			l->setPosition(ccp((i*scrollWidth),0));
			addChild(l);			
		}
		
		// Setup a count of the available screens
		totalScreens = layers->count();
		return true;	
	}
	else
	{
		return false;
	}	
}

void CCScrollLayer::moveToPage(int page)
{	
	CCEaseBounce* changePage = CCEaseBounce::actionWithAction(CCMoveTo::actionWithDuration(0.3f, ccp(-((page-1)*scrollWidth),0)));
	this->runAction(changePage);
	currentScreen = page;	
}

void CCScrollLayer::moveToNextPage()
{	
	CCEaseBounce* changePage = CCEaseBounce::actionWithAction(CCMoveTo::actionWithDuration(0.3f, ccp(-(((currentScreen+1)-1)*scrollWidth),0)));
	
	this->runAction(changePage);
	currentScreen = currentScreen+1;	
}

void CCScrollLayer::moveToPreviousPage()
{	
	CCEaseBounce* changePage =CCEaseBounce::actionWithAction(CCMoveTo::actionWithDuration(0.3f, ccp(-(((currentScreen-1)-1)*scrollWidth),0)));
	this->runAction(changePage);
	currentScreen = currentScreen-1;	
}

void CCScrollLayer::onExit()
{
	CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
	CCLayer::onExit();
}

bool CCScrollLayer::ccTouchBegan(CCTouch *touch, CCEvent *withEvent)
{
	
	CCPoint touchPoint = touch->locationInView(touch->view());
	touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);
	
	startSwipe = (int)touchPoint.x;
	return true;
}

void CCScrollLayer::ccTouchMoved(CCTouch *touch, CCEvent *withEvent)
{	
	CCPoint touchPoint = touch->locationInView(touch->view());
	touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);
	
	this->setPosition(ccp((-(currentScreen-1)*scrollWidth)+(touchPoint.x-startSwipe),0));
}

void CCScrollLayer::ccTouchEnded(CCTouch *touch, CCEvent *withEvent)
{
	
	CCPoint touchPoint = touch->locationInView(touch->view());
	touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);
	
	int newX = (int)touchPoint.x;
		
	if ( (newX - startSwipe) < -scrollWidth / 3 && (currentScreen+1) <= totalScreens )
	{
		this->moveToNextPage();
	}
	else if ( (newX - startSwipe) > scrollWidth / 3 && (currentScreen-1) > 0 )
	{
		this->moveToPreviousPage();
	}
	else
	{
		this->moveToPage(currentScreen);		
	}		
}