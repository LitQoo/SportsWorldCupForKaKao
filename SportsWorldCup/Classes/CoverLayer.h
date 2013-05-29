//
//  CoverLayer.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 4. 12..
//
//

#ifndef __SportsWorldCup__CoverLayer__
#define __SportsWorldCup__CoverLayer__

#include "cocos2d.h"

USING_NS_CC;

class CoverLayer : public CCLayerColor
{
private:
	virtual void registerWithTouchDispatcher()
	{
		CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, kCCMenuTouchPriority - 1, true);
	}
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		CCPoint location = pTouch->locationInView();
		location = CCDirector::sharedDirector()->convertToGL(location);
		
		return true; // swallow;
	}
	
public:
	virtual bool init()
	{
		if ( !this->CCLayerColor::initWithColor(ccc4f(0,0,0,150)) )
			return false;
		
		setIsTouchEnabled(true);
		return true;
	}
	
	CoverLayer() : CCLayerColor(){}
	virtual ~CoverLayer() {
		CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
	}
	LAYER_NODE_FUNC(CoverLayer);
};

#endif /* defined(__SportsWorldCup__CoverLayer__) */
