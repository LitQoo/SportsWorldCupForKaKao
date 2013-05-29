#pragma once

#include "cocos2d.h"
#include <string>
#include <vector>
#include "Bridge.h"
using namespace std;
using namespace cocos2d;

class KSImageSelector;
typedef void (CCLayer::*SEL_ImageHandler)(const char*);

#define image_selector(_SELECTOR) (SEL_ImageHandler)(&_SELECTOR)

class KSMenu : public CCMenu
{
private:
	bool moved;
	CCPoint beginPt;
public:
	virtual void registerWithTouchDispatcher()
	{
		moved = false;
		CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, kCCMenuTouchPriority, false);
		this->setIsTouchEnabled(true);
	}
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event)
	{
		beginPt = touch->locationInView();	
		beginPt = CCDirector::sharedDirector()->convertToGL( beginPt );
		return CCMenu::ccTouchBegan(touch, event);
	}
	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event)
	{
		CCMenu::ccTouchMoved(touch, event);
		CCPoint clickPt = touch->locationInView();	
		clickPt = CCDirector::sharedDirector()->convertToGL( clickPt );
		if(sqrt( (clickPt.x - beginPt.x)*(clickPt.x - beginPt.x) + (clickPt.y - beginPt.y)*(clickPt.y - beginPt.y) ) >= 10)
		{
			//CCLog("moving");
			moved = true;
		}
		
	}
	virtual void ccTouchEnded(CCTouch *touch, CCEvent* event)
	{
		if(moved)
		{
			//CCLog("moved");
			if (m_pSelectedItem)
			{
				m_pSelectedItem->unselected();
			}
			
			m_eState = kCCMenuStateWaiting;
		}
		else
		{
			//CCLog("not moved");
			CCMenu::ccTouchEnded(touch, event);
		}
		
		moved = false;
	}
	
	//void ccTouchCancelled(CCTouch *touch, CCEvent* event)
	//{
	//	/*CC_UNUSED_PARAM(touch);
	//	CC_UNUSED_PARAM(event);
	//	CCAssert(m_eState == kCCMenuStateTrackingTouch, "[Menu ccTouchCancelled] -- invalid state");
	//	if (m_pSelectedItem)
	//	{
	//		m_pSelectedItem->unselected();
	//	}
	//	m_eState = kCCMenuStateWaiting;*/
	//}

	/*virtual void ccTouchMoved(CCTouch* touch, CCEvent* event)
	{
		CCMenu::ccTouchMoved(touch, event);
		//CCLog("HH");
		moved = true;
	}*/
	/*virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event)
	{
		CCMenu::ccTouchBegan()
		return false;
	}*/
};
class KSImageSelector : public CCLayerColor
{
private:
	float x, y, marginX, marginY;
	float imageWidth, imageHeight;
	vector<string> imageList;
	CCLayer* selectorTarget;
	SEL_ImageHandler selector;
	CCPoint clickBegin;
	CCMenu* myMenu;
	int lineCount;
	
	//void (*selector)(const char*);
private:
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
	{
		CCSetIterator it = pTouches->begin();
		CCTouch* touch = (CCTouch*)(*it);

		clickBegin = touch->locationInView( );
		clickBegin = CCDirector::sharedDirector()->convertToGL( clickBegin );
	}

	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
	{
		CCSetIterator it = pTouches->begin();
		CCTouch* touch = (CCTouch*)(*it);

		CCPoint touchLocation = touch->locationInView();
		touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
		float nMoveY = touchLocation.y - clickBegin.y;

		CCPoint curPos  = myMenu->getPosition();
		CCPoint nextPos = ccp(curPos.x, curPos.y + nMoveY);
		CCSize winSize = CCSize(480, 320);
		if (nextPos.y < 0.0f)
		{
			myMenu->setPosition(CCPointZero);
			return;
		}

		if (nextPos.y > (lineCount * (imageHeight + marginY) - winSize.height))
		{
			myMenu->setPosition(ccp(0, lineCount * (imageHeight + marginY) - winSize.height));
			return;
		}

		myMenu->setPosition(nextPos);
		clickBegin = touchLocation;
		//s_tCurPos   = nextPos;
	}
	//virtual void registerWithTouchDispatcher()
	//{
	//	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, kCCMenuTouchPriority, true);
	//	//this->setIsTouchEnabled(true);
	//}
	//virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	//{
	//	return true;
	//}
public:
	KSImageSelector() : CCLayerColor()
	{
		marginX = marginY = 10.f;
		x = y = 0.f;
		x = marginX;
		y = 320.f;
		lineCount = 1;
		imageWidth = imageHeight = 24;
		setIsTouchEnabled(true);
	}
	virtual ~KSImageSelector()
	{

	}
	virtual bool init()
	{
		CCLayerColor::initWithColor(ccc4f(0,0,0,0));

		return true;
	}
	void setImageList(vector<string> _il)
	{
		imageList = _il;
	}
	void loadImageList(CCLayer* target, SEL_ImageHandler _selector)
	{
		selectorTarget = target;
		selector = _selector;
		myMenu = new KSMenu();
		myMenu->init();
		myMenu->setPosition(ccp(0, 0));
		
		for(auto iter = imageList.begin(); iter != imageList.end(); ++iter)
		{
			string c = *iter;
			c += ".png";
			//CCLog("load %s", c.c_str());
			CCSprite* spr1 = CCSprite::spriteWithSpriteFrameName(c.c_str());
			CCSprite* spr2 = CCSprite::spriteWithSpriteFrameName(c.c_str());
			CCMenuItemSprite* spr = CCMenuItemSprite::itemFromNormalSprite(spr1, spr2, this, menu_selector(KSImageSelector::clickImage));
			
			spr->setUserData((void*)iter->c_str());

			CCLabelTTF* countryName = CCLabelTTF::labelWithString((*iter).c_str(), defaultFont, 12);

			spr->addChild(countryName, 1);
			countryName->setPosition(ccp(spr->boundingBox().size.width/2., 0));
			//CCLog("loaded");
			spr->setAnchorPoint(ccp(0, 1));
			myMenu->addChild(spr, 1);
			
			if(x + spr->boundingBox().size.width > 480)
			{
				y -= imageHeight + marginY;
				x = marginX;
				lineCount++;
			}

			spr->setPosition(ccp(x, y));
			x += imageWidth + marginX;
		}
		this->addChild(myMenu, INT_MAX);
		myMenu->release();
	}
	void clickImage(CCObject* sender)
	{
		CCMenuItemImage* _t = (CCMenuItemImage*)sender;
		const char* fileName = (const char*)_t->getUserData();
		(selectorTarget->*selector)(fileName);

		CCDirector::sharedDirector()->popScene();
		//this->getParent()->removeChild(this, true);
	}
	void show()
	{
		CCScene * scene = CCScene::node();
		scene->addChild(this);
		CCDirector::sharedDirector()->pushScene(scene);
	}
	
public:
	static CCScene* scene()
	{
		CCScene * scene = CCScene::node();
		KSImageSelector* layer = KSImageSelector::node();
		scene->addChild(layer);
		return scene;
	}

	// implement the "static node()" method manually
	LAYER_NODE_FUNC(KSImageSelector);
};