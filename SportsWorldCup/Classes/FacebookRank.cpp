//
//  FacebookRank.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 2. 28..
//
//

#include "FacebookRank.h"
#include "Bridge.h"
#include "KS_Util.h"
#include "NSDefault.h"
#include "KSoundEngine.h"
#include "KSModalAlert.h"
#include "SWIntro.h"
#include "GraphDog.h"
#include "KSoundEngine.h"
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
void FacebookRank::FACEBOOKLOGIN()
{
	KS::KSLog("FACEBOOK LOGIN");
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	facebookLogin->setIsVisible(false);
	FBConnector::get()->login(this, fb_selector(FacebookRank::facebookLoginFinish));
}

FacebookRank::~FacebookRank()
{
	FBDelegator::getInstance()->removeTarget(this);
}
bool FacebookRank::init()
{
//	CCNode::init();
//	CCNode::node()
	CCLayer::init();
	leftTime_s = -1;
	
	setIsTouchEnabled(true);
	CCSprite* back = CCSprite::spriteWithFile("friendrank.png");
	
	addChild(back);
	thiz = back;
	leftTimeFnt = CCLabelBMFont::labelWithString("", "main2.fnt");
	leftTimeFnt->setPosition(ccp(218, 263));
	thiz->addChild(leftTimeFnt, 2);
	
	float loadingX = FB_X + FB_W / 2.f;
	float loadingY = FB_Y + FB_H / 2.f;
	auto animator = SceneUtil::playAnimation("loading.png", 0.1, 9, 9, 56, 56, true);
	animator.second->runAction(animator.first);
	thiz->addChild(animator.second, INT_MAX);
	animator.second->setPosition(ccp(loadingX, loadingY));
	fbLoading = animator.second;
	fbLoading->setIsVisible(false);
	
		lqView = new LQTableView;
    lqView->init(FB_W, FB_H, this, callTableCell_selector(FacebookRank::friendCell));
	lqView->autorelease();
	lqView->setPosition(ccp(139, 150));
 	thiz->addChild(lqView,10);
	lqView->setIsVisible(false);
	
	lqView->setIsVisible(true);
	
	
	CCMenu* _menu = CCMenu::menuWithItems(0);
	_menu->setPosition(CCPointZero);
	thiz->addChild(_menu);
	{
		CCMenuItem* fblogin = CCMenuItemImage::itemFromNormalImage("facebook_login.png", "facebook_login.png",
																   this, menu_selector(FacebookRank::FACEBOOKLOGIN));
		facebookLogin = fblogin;
		fblogin->setPosition(ccp(130, 150));
		_menu->addChild(fblogin, 2);
		fblogin->setIsVisible(false);
		
	}
	{
		CCMenuItem* item = CCMenuItemImage::itemFromNormalImage("main_postbox.png", "main_postbox.png",
																this, menu_selector(FacebookRank::POSTBOX));
		postbox = item;
		item->setPosition(ccp(38, 26));
		_menu->addChild(item, 2);
	}
	if(FBConnector::get()->isUsed())
	{
		if(!FBConnector::get()->isLogin())
		{
			KS::KSLog("is not login");
			fbLoading->setIsVisible(true);
			FBConnector::get()->login(this, fb_selector(FacebookRank::facebookLoginFinish));
		}
		else
		{
			KS::KSLog("islogin");
		}
	}
	else
	{
		facebookGetScoresFinish(JsonBox::Value());
		//facebookLogin->setIsVisible(true);
		
	}
	
	postboxNewSprite = CCSprite::spriteWithFile("facebook_new.png");
	//facebookPostBox->addChild(postboxNewSprite, 1);
	postboxNewSprite->setPosition(ccp(58, 34));
	postboxNewSprite->setIsVisible(false);
	thiz->addChild(postboxNewSprite, 2);
	
	reqCount = CCLabelBMFont::labelWithString("", "main2.fnt");
	reqCount->setPosition(ccp(58, 36));
	thiz->addChild(reqCount, 3);
	{
		CCMenuItem* invite = CCMenuItemImage::itemFromNormalImage("main_invite.png", "main_invite.png",
																   this, menu_selector(FacebookRank::FRIENDINVITE));
		invite->setPosition(ccp(233, 25));
		_menu->addChild(invite, 2);
		
//		invite->runAction(
//						CCRepeatForever::actionWithAction( CCSequence::actionOneTwo(CCEaseOut::actionWithAction(CCMoveBy::actionWithDuration(0.8, ccp(0, 5)), 1.3),
//																					CCEaseOut::actionWithAction(CCMoveBy::actionWithDuration(0.8, ccp(0, -5)), 1.3))) );
	}
//	this->runAction(CCRepeat::actionWithAction(CCSequence::actionOneTwo(CCMoveBy::actionWithDuration(1.f, ccp(0, 100)),
//																		  CCMoveBy::actionWithDuration(1.f, ccp(0, -100))), 100));
	
	schedule(schedule_selector(ThisClassType::update));
	return true;
}
void FacebookRank::finishGetWeek(JsonBox::Object js)
{
	if(js["state"].getString() != "ok")
		return;
	playInfo->currentWeek = js["week"].getInt();
	playInfo->whenRecvLeftTime = GameSystem::getCurrentTime_s();
	mediator->setFacebookLeftTime(js["lefttime"].getInt());
}
void FacebookRank::update(ccTime dt)
{
	int remainS = (leftTime_s - (GameSystem::getCurrentTime_s() - playInfo->whenRecvLeftTime));
	static int lastReqTime = -1;
	if(remainS < 0 && playInfo->whenRecvLeftTime >= lastReqTime)
	{
		lastReqTime = GameSystem::getCurrentTime_s();
		graphdog->command("getweek", 0, this, gd_selector(ThisClassType::finishGetWeek));
	}
	else if(remainS >= 0)
	{
		int h = remainS / 3600;
		remainS %= 3600;
		int m = (remainS) / 60;
		remainS %= 60;
		int s = remainS;
		leftTimeFnt->setString(KS_Util::stringWithFormat("%02d:%02d:%02d", h, m, s).c_str());
	}
	
}
void FacebookRank::FRIENDINVITE(CCObject*)
{
	CCNotificationCenter::sharedNotifCenter()->postNotification("showInviteFriendWindow");
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	
}
void FacebookRank::getRequestCount(JsonBox::Value v2)
{
	int c = v2.getArray().size();
	if(c > 0)
	{
		postboxNewSprite->setIsVisible(true);
		reqCount->setString(KS_Util::stringWithFormat("%d", c).c_str());
	}
	else
	{
		postboxNewSprite->setIsVisible(false);
		reqCount->setString("");
	}
	
	
	//postBoxView->setIsVisible(true);
	//facebookPostBox->setIsVisible(false);
	//facebookBack->setIsVisible(true);
}
void FacebookRank::POSTBOX(CCObject*)
{
	CCNotificationCenter::sharedNotifCenter()->postNotification("viewPostBox");
	KSoundEngine::sharedEngine()->playSound("select.mp3");
//	mediator->showFacebookPost();
//	KS::KSLog("%", __FUNCTION__);
}

void FacebookRank::facebookLoginFinish(JsonBox::Value v2)
{
	
	
	FBConnector::get()->getRequests(this, fb_selector(FacebookRank::getRequestCount));
	fbLoading->setIsVisible(false);
	KS::KSLog("FACEBOOK LOGIN FINISH");
	JsonBox::Object o2 = v2.getObject();
	//ostringstream oss;
	KS::KSLog("%\n", v2);
	if(o2["error"].getInt() == FBConnectorResult::FBSuccess)
	{
		//facebookPostBox->setIsVisible(true);
		facebookLogin->setIsVisible(false);
		FBConnector::get()->getRequests(this, fb_selector(FacebookRank::getRequestFakeFinish));
		FBConnector::get()->getScores(this, fb_selector(FacebookRank::facebookGetScoresFinish));
		JsonBox::Object gParam;
		gParam["category"] = "facebook";
		gParam["log"] = "로그인성공";
		graphdog->command("writelog", &gParam, 0, 0);
	}
	else
	{
		mediator->incLoadCount();
		facebookLogin->setIsVisible(true);
		KSModalAlert("", ML::key("fail_facebook").c_str(), 1,
							  "OK", NOPARAM);
		JsonBox::Object gParam;
		gParam["category"] = "facebook";
		gParam["log"] = "로그인실패";
		graphdog->command("writelog", &gParam, 0, 0);
	}
}

void FacebookRank::getRequestFakeFinish(JsonBox::Value v2)
{
//	if(v2.getArray().empty() == false)
//	{
//		// new image
//		postboxNewSprite->setIsVisible(true);
//	}
}
void FacebookRank::facebookGetScoresFinish(JsonBox::Value v2)
{
	//##
	string ee = R"(
	[{
		"score" : 1800000,
		"user" : {
			"id" : "100005054377514",
			"name" : "서영호"
		}
	},
	 {
		 "score" : 1320000,
		 "user" : {
			 "id" : "100001580868158",
			 "name" : "박정순"
		 }
	 },
	 {
		 "score" : 950000,
		 "user" : {
			 "id" : "100001181176876",
			 "name" : "한경수"
		 }
	 },
	 {
		 "score" : 777700,
		 "user" : {
			 "id" : "100002141448085",
			 "name" : "김현수"
		 }
	 },
	 {
		 "score" : -1,
		 "user" : {
			 "id" : "100001852056292",
			 "name" : "윤영일"
		 }
	 },
	 {
		 "score" : -1,
		 "user" : {
			 "id" : "100001120061763",
			 "name" : "Dong Chan Kim"
		 }
	 },
	 {
		 "score" : -1,
		 "user" : {
			 "id" : "100000058239459",
			 "name" : "HanNa Lee"
		 }
	 },
	 {
		 "score" : -1,
		 "user" : {
			 "id" : "100001044028923",
			 "name" : "구소영"
		 }
	 },
	 {
		 "score" : -1,
		 "user" : {
			 "id" : "100002733823889",
			 "name" : "Juhyung  Jo"
		 }
	 },
	 {
		 "score" : -1,
		 "user" : {
			 "id" : "100001994288109",
			 "name" : "MyoungHa Jeon"
		 }
	 },
	 {
		 "score" : -1,
		 "user" : {
			 "id" : "100002313379644",
			 "name" : "강창근"
		 }
	 },
	 {
		 "score" : -1,
		 "user" : {
			 "id" : "100001284139011",
			 "name" : "강민진"
		 }
	 },
	 {
		 "score" : -1,
		 "user" : {
			 "id" : "100003258905967",
			 "name" : "Hyunseung  Lee"
		 }
	 },
	 {
		 "score" : -1,
		 "user" : {
			 "id" : "100000052631247",
			 "name" : "Onnuri Won"
		 }
	 },
	 {
		 "score" : -1,
		 "user" : {
			 "id" : "100003865841276",
			 "name" : "박경식"
		 }
	 },
	 {
		 "score" : -1,
		 "user" : {
			 "id" : "100002174300058",
			 "name" : "Heo  Juyoung"
		 }
	 },
	 {
		 "score" : -1,
		 "user" : {
			 "id" : "100002823821309",
			 "name" : "Sang Min Choi"
		 }
	 },
	 {
		 "score" : -1,
		 "user" : {
			 "id" : "100003177112703",
			 "name" : "YoonJe  Jo"
		 }
	 },
	 {
		 "score" : -1,
		 "user" : {
			 "id" : "100003585328664",
			 "name" : "JuHui Gim"
		 }
	 },
	 {
		 "score" : -1,
		 "user" : {
			 "id" : "100003701964757",
			 "name" : "이요한"
		 }
	 },
	 {
		 "score" : -1,
		 "user" : {
			 "id" : "100003717687100",
			 "name" : "김성봉"
		 }
	 },
	 {
		 "score" : -1,
		 "user" : {
			 "id" : "100002148369520",
			 "name" : "박세홍"
		 }
	 },
	 {
		 "score" : -1,
		 "user" : {
			 "id" : "100001295146366",
			 "name" : "Kyeongnam  Park"
		 }
	 },
	 {
		 "score" : -1,
		 "user" : {
			 "id" : "100003228661254",
			 "name" : "반치용"
		 }
	 },
	 {
		 "score" : -1,
		 "user" : {
			 "id" : "100002489488385",
			 "name" : "Yehyun  Lee"
		 }
	 },
	 {
		 "score" : -1,
		 "user" : {
			 "id" : "100001512012674",
			 "name" : "최대길"
		 }
	 },
	 {
		 "score" : -1,
		 "user" : {
			 "id" : "100000547842869",
			 "name" : "Subin Jeong"
		 }
	 },
	 {
		 "score" : -1,
		 "user" : {
			 "id" : "100002331041736",
			 "name" : "Changhee Kim"
		 }
	 },
	 {
		 "score" : -1,
		 "user" : {
			 "id" : "100003127760657",
			 "name" : "최용민"
		 }
	 },
	 {
		 "score" : -1,
		 "user" : {
			 "id" : "100001978971610",
			 "name" : "SangHoon Jung"
		 }
	 },
	 {
		 "score" : -1,
		 "user" : {
			 "id" : "100001765165320",
			 "name" : "Momoai Jay Um"
		 }
	 },
	 {
		 "score" : -1,
		 "user" : {
			 "id" : "100001814436647",
			 "name" : "김대현"
		 }
	 },
	 {
		 "score" : -1,
		 "user" : {
			 "id" : "100002011764452",
			 "name" : "김현우"
		 }
	 },
	 {
		 "score" : -1,
		 "user" : {
			 "id" : "100003168263570",
			 "name" : "Sangouk  Lee"
		 }
	 }
	 ]
	
	)";
	v2.loadFromString(ee); //##
	mediator->incLoadCount();
	fbLoading->setIsVisible(false);
	fblist = v2.getArray();
	lqView->loadTable(fblist.size());
	KS::KSLog("%", v2);
	//FBConnector::get()->sendScore(44444, this, fb_selector(IntroMain::facebookTest3));
}

void FacebookRank::VIEWPROFILE(CCObject*)
{
	CCMenu* _menu = CCMenu::menuWithItems(0);
	thiz->addChild(_menu, INT_MAX);
	_menu->setPosition(ccp(0, 0));
	
	CCMenuItem* item = CCMenuItemImage::itemFromNormalImage("friend_date.png", "friend_date.png", this, menu_selector(ThisClassType::CLOSEPROFILE));
	item->setPosition(ccp(134, 150));
	_menu->addChild(item, 2);
}
void FacebookRank::CLOSEPROFILE(CCObject* s)
{
	CCNode* _s = (CCNode*)s;
	_s->getParent()->removeFromParentAndCleanup(true);
}
LQTableViewCell* FacebookRank::friendCell(CCObject *obj){
    CCNode *node = (CCNode *)obj;
    int row = node->getTag();
	
	LQTableViewCell *cell = new LQTableViewCell;
    cell->init(0, 0, FB_W, FB_CELL_H);
	cell->autorelease();
	
 	JsonBox::Object _info=fblist[row].getObject();
    JsonBox::Object _user=_info["user"].getObject();
	int score = _info["score"].getInt();
	string name = _user["name"].getString();
	string fbid = _user["id"].getString();
	string myid = FBConnector::get()->userID;
	//string imgurl =	KS_Util::stringWithFormat("graph.facebook.com/%s/picture?width=%d&height=%d", _user["id"].getString().c_str(), FB_PROFILE_W, FB_PROFILE_H);
	//string imgurl = CCString::createWithFormat("graph.facebook.com/%s/picture?width=64&height=64",_user["id"].getString().c_str())->getCString();
	
	CCMenu* _menu = CCMenu::menuWithItems(0);
	_menu->setPosition(ccp(0, 0));
	_menu->setTag(CELL_MENUS);
	cell->addChild(_menu, 2);
	
	
	CCMenuItem* profileSpriteFake = CCMenuItemImage::itemFromNormalImage("transparent_pixel.png", "transparent_pixel.png",
			this, menu_selector(ThisClassType::VIEWPROFILE));
	profileSpriteFake->setPosition(ccp(47, 17));
	profileSpriteFake->setScale(FB_PROFILE_W);
	CCSprite* profileSprite =  FBConnector::get()->getProfileSprite(_user["id"].getString(), FB_PROFILE_W, FB_PROFILE_H);
    if(profileSprite!=NULL)
	{
        profileSprite->setPosition(ccp(47, 17));
		cell->addChild(profileSprite, 1);
        _menu->addChild(profileSpriteFake, 1);
    }
		
	
	
	CCSprite* _back = 0;
	if(fbid == myid)
	{
		_back = CCSprite::spriteWithFile("facebook_list2.png");
	}


	
	CCLabelTTF* nameTTF = CCLabelTTF::labelWithString(name.c_str(), defaultFont, 9.f);
	nameTTF->setColor(ccc3(0, 0, 0));
	nameTTF->setAnchorPoint(ccp(0, 0.5));
	nameTTF->setPosition(ccp(76, 25));
	cell->addChild(nameTTF, 1);
	
	//CCLabelTTF* rankTTF = CCLabelTTF::labelWithString(KS_Util::stringWithFormat("%d", row+1).c_str(), defaultFont, 12.f);
	int _ranking = row+1;
	
	if(score < 0)
	{
		CCLabelBMFont* rankTTF = CCLabelBMFont::labelWithString("-", "facebook_count.fnt");
		if(score < 0)
		{
			rankTTF->setString("-");
		}
		rankTTF->setPosition(ccp(17, 17));
		cell->addChild(rankTTF, 1);
		if(_back == 0)
		{
			_back = CCSprite::spriteWithFile("facebook_list1.png");
		}
	}
	else
	{
		if(_ranking==1){
			CCSprite *rankSprite = CCSprite::spriteWithFile("main_facebook_gold.png");
			rankSprite->setPosition(ccp(17, 17));
			cell->addChild(rankSprite,1);
			if(_back == 0)
			{
				_back = CCSprite::spriteWithFile("facebook_goldback.png");
			}
		}else if(_ranking==2){
			CCSprite *rankSprite = CCSprite::spriteWithFile("main_facebook_silver.png");
			rankSprite->setPosition(ccp(17, 17));
			cell->addChild(rankSprite,1);
			if(_back == 0)
			{
				_back = CCSprite::spriteWithFile("facebook_silverback.png");
			}
		}else if(_ranking==3){
			CCSprite *rankSprite = CCSprite::spriteWithFile("main_facebook_bronze.png");
			rankSprite->setPosition(ccp(17, 17));
			cell->addChild(rankSprite,1);
			if(_back == 0)
			{
				_back = CCSprite::spriteWithFile("facebook_bronzeback.png");
			}
		}else{
			CCLabelBMFont* rankTTF = CCLabelBMFont::labelWithString(KS_Util::stringWithFormat("%d", row+1).c_str(), "facebook_count.fnt");
			rankTTF->setPosition(ccp(17, 17));
			cell->addChild(rankTTF, 1);
			if(_back == 0)
			{
				_back = CCSprite::spriteWithFile("facebook_list1.png");
			}
		}
	}
	
	_back->setAnchorPoint(ccp(0, 0));
	cell->addChild(_back);
	CCPoint opPoint = ccp(205, 18);
	if(fbid != myid)
	{
		CCSprite* _checked;
		if(score < 0 )
			_checked = CCSprite::spriteWithFile("facebook_heartsendwait.png");
		else
			_checked = CCSprite::spriteWithFile("facebook_heartsendwait.png");
		_checked->setPosition(opPoint);
		cell->addChild(_checked, 1);
	}
	string scoreStr;
	
	if(score < 0)
	{
		scoreStr = ML::key("invite");
		if(NSDefault::getIsInvitable(fbid))
		{
			CCMenuItem* invite = CCMenuItemImage::itemFromNormalImage("facebook_heartplus.png", "facebook_heartplus.png",
																	  this, menu_selector(FacebookRank::INVITE));
			invite->setTag(INVITE_TAG);
			invite->setUserData((void*)row);
			invite->setPosition(opPoint					);
			_menu->addChild(invite, 2);
			
		}
		CCLabelTTF* inviteMsg = CCLabelTTF::labelWithString(scoreStr.c_str(), defaultFont, 14.f);
		inviteMsg->setColor(ccc3(0, 0, 0));
		inviteMsg->setAnchorPoint(ccp(0, 0.5f));
		inviteMsg->setPosition(ccp(85, 10));
		cell->addChild(inviteMsg, 1);
	}
	else
	{
		scoreStr = KS_Util::stringWithFormat("%d", score);
		
		if(NSDefault::getIsSendable(fbid) && fbid != myid)
		{
			CCMenuItem* sendHeart = CCMenuItemImage::itemFromNormalImage("facebook_heartsend.png", "facebook_heartsend.png",
																		 this, menu_selector(FacebookRank::SENDHEART));
			sendHeart->setTag(HEART_TAG);
			sendHeart->setUserData((void*)row);
			sendHeart->setPosition(opPoint);
			_menu->addChild(sendHeart, 1);
			
		}
		
		CCLabelBMFont* scoreBM;
		
		if(_ranking == 1)
		{
			scoreBM = CCLabelBMFont::labelWithString(scoreStr.c_str(), "facebook_gold.fnt");
		}
		else if(_ranking == 2)
		{
			scoreBM = CCLabelBMFont::labelWithString(scoreStr.c_str(), "facebook_silver.fnt");
		}
		else if(_ranking == 3)
		{
			scoreBM = CCLabelBMFont::labelWithString(scoreStr.c_str(), "facebook_bronze.fnt");
		}
		else
		{
			scoreBM = CCLabelBMFont::labelWithString(scoreStr.c_str(), "facebook_normal.fnt");
		}
		scoreBM->setAnchorPoint(ccp(0, 0.5f));
		scoreBM->setPosition(ccp(77, 10));
		cell->addChild(scoreBM, 1);
	}
	
	return cell;
	
}


void FacebookRank::INVITE(CCObject* item)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	CCNotificationCenter::sharedNotifCenter()->postNotification("inviteFriend");
	
//	KS::KSLog("invite start");
//	CCMenuItem* realItem = (CCMenuItem*)item;
//	int row = (int)realItem->getUserData();
//	JsonBox::Object _info=fblist[row].getObject();
//    JsonBox::Object _user=_info["user"].getObject();
//	string fbid = _user["id"].getString();
//	
//	// 24시간 안에 초대를 보낸적이 있으면 앱 리케스트로 돌린다.
//	if(NSDefault::getInviteIn24H())
//	{
//		// request
//		// 보내는건 센드 리퀘스트지만 받는건 invitationFinish.
//		FBConnector::get()->sendRequest(fbid, ML::key("together"), "INVITE", this, fb_selector(FacebookRank::invitationFinish));
//	}
//	else
//	{
//		// invite
//		FBConnector::get()->sendInvite(fbid, "SportsWorldCup", ML::key("together"), "http://litqoo.com/sportsworldcup", "http://litqoo.com/sportsworldcup/icon.jpg",
//									   this, fb_selector(FacebookRank::invitationFinish));
//	}
	
}
void FacebookRank::invitationFinish(JsonBox::Value v)
{
	KS::KSLog("invitation finish");
	JsonBox::Object gParam;
	gParam["category"] = "facebook";
	gParam["log"] = "초대";
	graphdog->command("writelog", &gParam, 0, 0);
	JsonBox::Object vo = v.getObject();
	if(vo["error"].getInt() == FBConnectorResult::FBSuccess) // succesx
	{
		NSDefault::setInviteIn24H();
		string returnedID = vo["id"].getString();
		NSDefault::setIsInvitable(returnedID);
		int searchedRow;
		for(int i = 0; i<fblist.size(); i++)
		{
			JsonBox::Object _info= fblist[i].getObject();
			JsonBox::Object _user=_info["user"].getObject();
			string fbid = _user["id"].getString();
			if(fbid == returnedID)
			{
				searchedRow = i;
				break;
			}
		}
		KS::KSLog("% %", __FILE__, __LINE__);
		CCArray* cellArray = lqView->getCellArray();
		KS::KSLog("% %", __FILE__, __LINE__);
		for(int i = 0; i<cellArray->count(); i++)
		{
			LQTableViewCell* cell = (LQTableViewCell*)cellArray->objectAtIndex(i);
			if(cell->index == searchedRow)
			{
				CCMenuItem* realItem = (CCMenuItem*)cell->getChildByTag(CELL_MENUS)->getChildByTag(INVITE_TAG);
				realItem->removeFromParentAndCleanup(true);
				break;
			}
		}
		KS::KSLog("% %", __FILE__, __LINE__);
		
		if(mediator->getHeartSize() < 5)
		{
			mediator->addHeartSprite();
		}
		NSDefault::setHeartNumber(NSDefault::getHeartNumber() + 1);
		//remainHeartFnt->setString(KS_Util::stringWithFormat("%d", NSDefault::getHeartNumber()).c_str());
	}
	else
	{
		
	}
}
void FacebookRank::sendHeartFinish(JsonBox::Value v)
{
	
	
	KS::KSLog("sendheart finish");
	JsonBox::Object gParam;
	gParam["category"] = "facebook";
	gParam["log"] = "하트보냄";
	graphdog->command("writelog", &gParam, 0, 0);
	JsonBox::Object vo = v.getObject();
	
	
	//string fbid = vo["id"].getString();
	if(vo["error"].getInt() == FBConnectorResult::FBSuccess) // succesx
	{
		string returnedID = vo["id"].getString();
		NSDefault::setIsSendable(returnedID);
		int searchedRow;
		for(int i = 0; i<fblist.size(); i++)
		{
			JsonBox::Object _info= fblist[i].getObject();
			JsonBox::Object _user=_info["user"].getObject();
			string fbid = _user["id"].getString();
			if(fbid == returnedID)
			{
				searchedRow = i;
				break;
			}
		}
		KS::KSLog("% %", __FILE__, __LINE__);
		CCArray* cellArray = lqView->getCellArray();
		for(int i = 0; i<cellArray->count(); i++)
		{
			LQTableViewCell* cell = (LQTableViewCell*)cellArray->objectAtIndex(i);
			if(cell->index == searchedRow)
			{
				KS::KSLog("% %", __FILE__, __LINE__);
				CCMenuItem* realItem = (CCMenuItem*)cell->getChildByTag(CELL_MENUS)->getChildByTag(HEART_TAG);
				KS::KSLog("% %", __FILE__, __LINE__);
				realItem->removeFromParentAndCleanup(true);
				break;
			}
		}
		KS::KSLog("% %", __FILE__, __LINE__);
	}
	else
	{
		
	}
}


void FacebookRank::SENDHEART(CCObject* item)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	CCNotificationCenter::sharedNotifCenter()->postNotification("sendHeartTo");
	
//	KS::KSLog("sendHeart start");
//	CCMenuItem* realItem = (CCMenuItem*)item;
//	int row = (int)realItem->getUserData();
//	JsonBox::Object _info=fblist[row].getObject();
//    JsonBox::Object _user=_info["user"].getObject();
//	string fbid = _user["id"].getString();
//	
//	string bbmsg = KS_Util::stringWithFormat(ML::key("bbyong").c_str(), FBConnector::get()->userName.c_str());
//	//	string bbmsg = "제발 한경수!";
//	//	KSModalAlert("", bbmsg.c_str(), 1,
//	//						  "OK", 0, 0);
////	::send
//	
//	FBConnector::get()->sendRequest(fbid,  bbmsg, "HEART",
//									this, fb_selector(ThisClassType::sendHeartFinish));
}

