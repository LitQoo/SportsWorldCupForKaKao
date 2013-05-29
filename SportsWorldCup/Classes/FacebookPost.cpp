//
//  FacebookPost.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 5..
//
//

#include "FacebookPost.h"
#include "Bridge.h"
#include "NSDefault.h"
#include "KSBaseScene.h"
#include "IntroMain.h"
#include "KSoundEngine.h"
//#include "IntroHead.h"

static const int FB_X = 12;
static const int FB_Y = 53;
static const int FB_W = 245;
static const int FB_H = 194;

static const int FB_PROFILE_W = 33;
static const int FB_PROFILE_H = 33;
static const int FB_CELL_H = 35;
static const int INVITE_TAG = 0x34545;
static const int HEART_TAG = 0x84353;
static const int CELL_MENUS = 0xcf487;
static const int GETHEART_TAG = 0xfeabf3;


FacebookPost::~FacebookPost()
{
	FBDelegator::getInstance()->removeTarget(this);
}
LQTableViewCell* FacebookPost::mailCell(CCObject* obj)
{
	CCNode *node = (CCNode *)obj;
    int row =node->getTag();
	KS::KSLog("row value = %", row);
	LQTableViewCell *cell = new LQTableViewCell;
    cell->init(0, 0, FB_W, FB_CELL_H);
	cell->autorelease();
	
	JsonBox::Object _info=fbMailList[row].getObject();
	JsonBox::Object _from=_info["from"].getObject();
	string name = _from["name"].getString();
	string fbid = _from["id"].getString();
	string time = "from";//_info["created_time"].getString();
	
	CCSprite* profileSprite =  FBConnector::get()->getProfileSprite(fbid, FB_PROFILE_W, FB_PROFILE_H);
	
    if(profileSprite!=NULL)
	{
        profileSprite->setPosition(ccp(34, 17));
        cell->addChild(profileSprite,1);
    }
	
	CCSprite* _back = CCSprite::spriteWithFile("facebook_list3.png");
	
	_back->setAnchorPoint(ccp(0, 0));
	cell->addChild(_back);
	CCLabelTTF* nameTTF = CCLabelTTF::labelWithString(name.c_str(), defaultFont, 13.f);
	nameTTF->setColor(ccc3(0, 0, 0));
	nameTTF->setAnchorPoint(ccp(0, 0.5));
	nameTTF->setPosition(ccp(60, 12));
	cell->addChild(nameTTF, 1);
	
	CCLabelTTF* timeTTF = CCLabelTTF::labelWithString(time.c_str(), defaultFont, 7.f);
	timeTTF->setColor(ccc3(0, 0, 0));
	timeTTF->setAnchorPoint(ccp(0, 0.5f));
	timeTTF->setPosition(ccp(60, 25));
	cell->addChild(timeTTF, 1);
	
	CCMenu* _menu = CCMenu::menuWithItems(0);
	_menu->setPosition(ccp(0, 0));
	_menu->setTag(CELL_MENUS);
	cell->addChild(_menu, 1);
	CCMenuItem* getHeart = CCMenuItemImage::itemFromNormalImage("facebook_heartplus.png", "facebook_heartplus.png",
																this, menu_selector(FacebookPost::GETHEART));
	getHeart->setTag(GETHEART_TAG);
	getHeart->setUserData((void*)row);
	getHeart->setPosition(ccp(204, 16));
	_menu->addChild(getHeart, 2);
	
	return cell;
}

void FacebookPost::GETHEART(CCObject* item)
{
	CCMenuItem* realItem = (CCMenuItem*)item;
	int row = (int)realItem->getUserData();
	JsonBox::Object _info=fbMailList[row].getObject();
    string mailID = _info["id"].getString();
	KS::KSLog("mailID = %", mailID.c_str());
	
	FBConnector::get()->removeRequests(mailID, this, fb_selector(FacebookPost::removeRequestFinish));
}



void FacebookPost::removeRequestFinish(JsonBox::Value v)
{
	JsonBox::Object o2 = v.getObject();
	
	if(o2["error"].getInt() == FBConnectorResult::FBSuccess)
	{
		string mailID = o2["id"].getString();
		
		for(auto iter = fbMailList.begin(); iter != fbMailList.end(); ++iter)
		{
			JsonBox::Object _obj = iter->getObject();
			string searchedID = _obj["id"].getString();
			if(searchedID == mailID)
			{
				fbMailList.erase(iter);
				break;
			}
		}
		
		postBoxView->loadTable(fbMailList.size());
		if(mediator->getHeartSize() < 5)
			mediator->addHeartSprite();
//		if(pHead->getHeartSize() < 5)
//			pHead->addHeartSprite();
		NSDefault::setHeartNumber(NSDefault::getHeartNumber() + 1);
		//remainHeartFnt->setString(KS_Util::stringWithFormat("%d", NSDefault::getHeartNumber()).c_str());
	}
	
}
void FacebookPost::loadRequest()
{
	CCSprite* postBox = CCSprite::spriteWithFile("kakao_postbox.png");
	postBox->setPosition(ccp(135, 150));
	thiz->addChild(postBox, 2);
//	FBConnector::get()->getRequests(this, fb_selector(FacebookPost::getRequestFinish));
//	fbLoading->setIsVisible(true);
}
void FacebookPost::getRequestFinish(JsonBox::Value v2)
{
	KS::KSLog("--------------------------getRequestFinish-----------------------");
	fbLoading->setIsVisible(false);
	fbMailList = v2.getArray();
	postBoxView->loadTable(fbMailList.size());
	//postBoxView->setIsVisible(true);
	//facebookPostBox->setIsVisible(false);
	//facebookBack->setIsVisible(true);
}

bool FacebookPost::init()
{
	//	CCNode::init();
	//	CCNode::node()
	CCLayer::init();
	setIsTouchEnabled(true);
	CCSprite* back = CCSprite::spriteWithFile("main_postbox_back.png");
	
	addChild(back);
	thiz = back;
	
	
	float loadingX = FB_X + FB_W / 2.f;
	float loadingY = FB_Y + FB_H / 2.f;
	auto animator = SceneUtil::playAnimation("loading.png", 0.1, 9, 9, 56, 56, true);
	animator.second->runAction(animator.first);
	thiz->addChild(animator.second, INT_MAX);
	animator.second->setPosition(ccp(loadingX, loadingY));
	fbLoading = animator.second;
	fbLoading->setIsVisible(false);
	
	postBoxView = new LQTableView;
	postBoxView->init(FB_W, FB_H, this, callTableCell_selector(FacebookPost::mailCell));
	postBoxView->autorelease();
	postBoxView->setPosition(ccp(139, 150));
 	thiz->addChild(postBoxView,10);

	
	CCMenu* _menu = CCMenu::menuWithItems(0);
	_menu->setPosition(CCPointZero);
	thiz->addChild(_menu);
	{
		CCMenuItem* item = CCMenuItemImage::itemFromNormalImage("main_font_back.png", "main_font_back_down.png",
																this, menu_selector(FacebookPost::BACK));
		item->setPosition(ccp(132, 26));
		_menu->addChild(item, 2);
	}
		//	this->runAction(CCRepeat::actionWithAction(CCSequence::actionOneTwo(CCMoveBy::actionWithDuration(1.f, ccp(0, 100)),
	//																		  CCMoveBy::actionWithDuration(1.f, ccp(0, -100))), 100));
	return true;
}

void FacebookPost::BACK(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	mediator->showBackToFacebook();	
}
