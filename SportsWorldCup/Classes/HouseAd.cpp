//
//  HouseAd.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 4. 12..
//
//

#include "HouseAd.h"
#include "Bridge.h"
#include "GraphDog.h"
#include "KS_Util.h"
#include "KSoundEngine.h"

bool HouseAd::init()
{
	CoverLayer::init();
	setAnchorPoint(ccp(0.5, 0.5));
	setPosition(ccp(240, 160));
	m_bIsRelativeAnchorPoint = true;
	
	prioMenu = PriorityMenu::menuWithItems(kCCMenuTouchPriority - 2, 0);
	
	prioMenu->setPosition(ccp(0, 0));
	addChild(prioMenu);
	return true;
}

void HouseAd::setAd(CCSprite* img, CCSprite* selectedimg,string store, string aid)
{
	this->store = store;
	this->aid = aid;
	CCMenuItem* adMenu = CCMenuItemSprite::itemFromNormalSprite(img, selectedimg, this, menu_selector(HouseAd::APP));
	
	CCSize screenSize = CCEGLView::sharedOpenGLView().getSize();
	adMenu->setPosition(ccp(screenSize.width / 2.f, screenSize.height / 2.f));
	prioMenu->addChild(adMenu, 2);
	
	CCMenuItem* xButton = CCMenuItemImage::itemFromNormalImage("cancel_down.png", "cancel_down.png",
															   this, menu_selector(HouseAd::CLOSE));
	xButton->setPosition(ccp(adMenu->getPosition().x + adMenu->getContentSize().width / 2.f,
							 adMenu->getPosition().y + adMenu->getContentSize().height/ 2.f));
	prioMenu->addChild(xButton, 2);
	
	
	
}

void HouseAd::APP(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	if(aid == "")
	{
		::otherApp(store);
		CLOSE(0);
	}
	else
	{
		JsonBox::Object storeOk_param;
		storeOk_param["aid"] = aid;
		storeOk_param["callback"] = store;
		graphdog->command("addcpievent", &storeOk_param, this, gd_selector(HouseAd::finishAddCpiEvent));
	}
}

void HouseAd::finishAddCpiEvent(JsonBox::Object js)
{
	JsonBox::Object param = js["param"].getObject();
	::otherApp(param["callback"].getString()); // addCPI 가 성공하든 못하든 무조건 이동.
	CLOSE(0); // 창 닫기
}


void HouseAd::CLOSE(CCObject*)
{
	removeFromParentAndCleanup(true);
}


