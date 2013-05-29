//
//  SWIntro.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 1. 23..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//
#include "SWIntro.h"

#include "DailyCheck.h"
#include "ChangeRank.h"
#include "SKGameScene.h"
#include "CCImage2.h"
#include "Tutorial.h"
#include "GameSystem.h"
#include "LoadingScene.h"
#include "WCPlay.h"
#include "WCSaveData.h"
#include "BS2Play.h"
#include "AKPlay.h"
#include "HWPlay.h"
#include "SKPlay.h"
#include "BS2BasketGame.h"
#include "AKAngryGameScene.h"
#include "HWFirstGameScene.h"
#include "ACGameScene.h"
#include "HWGameDirector.h"
#include "ACPlay.h"
#include "AKSaveData.h"
#include "ACSaveData.h"
#include "BS2SaveData.h"
#include "HWSaveData.h"
#include "KSModalAlert.h"
#include "Bridge.h"
#include "KSBlock.h"
#include "HWFromTo.h"
#include "GraphDog.h"
#include "KSBlock.h"
#include "GameSystem.h"
#include "Bridge.h"
#include "JNIKelper.h"
#include "HouseAd.h"
#include "KSoundEngine.h"
using namespace std;

SWIntro::SWIntro()
{
	loadCount = 0;
	bLoaded = false;
	catchingColleague = 0;
	
	inviteEvent = 0;
}

void SWIntro::finishGetUpdate(JsonBox::Object js)
{
	incLoadCount();
	try {
		KS::KSLog("%", js);
		if(js["state"].getString() != "ok")
		{
			throw string(ML::key("netfail"));
		}
		JsonBox::Array myList = js["list"].getArray();
		if(myList.size() <= 0)
		{
			return;
		}
		JsonBox::Object list0 = myList[0].getObject();
		JsonBox::Object userdata = list0["userdata"].getObject();
		int need = userdata["need"].getInt();
		int lastestVersion = userdata["version"].getInt();
		int currentVersion = graphdog->getAppVersion();
		
		if(lastestVersion > currentVersion) // 업데이트 해야한다
		{
			if(need > currentVersion) // 필수 인가
			{
				CCLog("must");
				KSModalAlert("", ML::key("mustupdate").c_str(), 1,
							 "OK", this, callfunc_selector(ThisClassType::UPDATE_OK));
			}
			else
			{
				CCLog("not must");
				KSModalAlert("", ML::key("update").c_str(), 2,
							 "NO", NOPARAM,
							 "YES", this, callfunc_selector(ThisClassType::UPDATE_OK));
			}
		}
		else // 업데이트 필요없다
		{
			JsonBox::Object param;
			param["rid"] = list0["rid"].getInt();
			graphdog->command("removerequest", &param, 0, 0);
		}
	}
	catch (const string& msg) {
		
	}
}
void SWIntro::UPDATE_OK()
{
	KSModalAlert("", ML::key("mustupdate").c_str(), 1,
				 "OK", this, callfunc_selector(ThisClassType::UPDATE_OK));
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	write_review();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	::otherApp("595879505");
#endif
}
void SWIntro::finishGetHouseAd(JsonBox::Object js)
{
	
	try {
		KS::KSLog("%", js);
		
		if(js["state"].getString() != "ok")
		{
			incLoadCount();
			throw string("");
		}
		
		JsonBox::Array myList = js["list"].getArray();
		if(myList.size() <= 0)
		{
			incLoadCount();
			return;
		}
		JsonBox::Object list0 = myList[0].getObject();
		JsonBox::Object userdata = list0["userdata"].getObject();
		CCLog("%s", userdata["image"].getString().c_str());
		houseAdUrl = userdata["image"].getString();
		storeLocation = userdata["storeid"].getString();
		aid = userdata["aid"].getString();
		pthread_t p_thread;
		if(pthread_create(&p_thread, NULL, t_function, this) == 0)
		{
			incLoadCount();
			throw string("It can not create thread");
		}
	} catch (const string& msg) {
		
//		KSModalAlert("", msg.c_str(), 1,
//					 "OK", NOPARAM);
	}

}



size_t SWIntro::WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;
    
    mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
    
    if (mem->memory == NULL) {
        /* out of memory! */
        KS::KSLog("not enough memory (realloc returned NULL)");
        exit(EXIT_FAILURE);
    }
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    return realsize;
}

void* SWIntro::t_function(void* _caller)
{
	SWIntro* caller = (SWIntro*)_caller;
	
	CURLcode retCode = curl_global_init(CURL_GLOBAL_ALL);
	if(retCode != CURLcode::CURLE_OK)
		return NULL;
	CURL *curl_handle = curl_easy_init();
	
	//while (1)
//	int i = 0;
	do{
		string pURL = caller->houseAdUrl;
		MemoryStruct chunk = {(char*)malloc(1), 0};
		
		curl_easy_setopt(curl_handle, CURLOPT_URL, pURL.c_str());
		curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, true);
		curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
		curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
		if(curl_easy_perform(curl_handle) != CURLcode::CURLE_OK)
			return NULL;
		caller->chunks.push(pair<MemoryStruct, string>(chunk, caller->storeLocation));
		
		//CCScheduler::sharedScheduler()->scheduleSelector(schedule_selector(MoreApps::imageDown), caller, 0,false);
		//CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(FBConnector::imageDown), FBConnector::get(), 0, false, 0, 0);
	}while(0); // 반복문이 필요없어서 이렇게 대체... 나중에 필요하면 바꿈.
	caller->isComplete = true;
	caller->incLoadCount();
	curl_easy_cleanup(curl_handle);
	curl_global_cleanup();
	return NULL;
	
}

void SWIntro::imageDown(ccTime dt)
{
	while(!chunks.empty())
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		CCImage* img = new CCImage;
		CCImage* img2 = new CCImage;
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		CCImage2* img = new CCImage2; //in cocos2d-x 1.x
		CCImage2* img2 = new CCImage2; //in cocos2d-x 1.x
#endif
		CCTexture2D* texture = new CCTexture2D();
		CCTexture2D* texture2 = new CCTexture2D();
		auto chunk_index = chunks.front();
		try {			
			auto chunk_index = chunks.front();
			
			if(img->initWithImageData(chunk_index.first.memory, (long)chunk_index.first.size, CCImage::kFmtUnKnown) == false)
				throw "";
			if(img2->initWithImageData(chunk_index.first.memory, (long)chunk_index.first.size, CCImage::kFmtUnKnown) == false)
				throw "";
			
			if(texture->initWithImage(img) == false)
				throw "";
			if(texture2->initWithImage(img2) == false)
				throw "";
			CCSprite* sprite = CCSprite::spriteWithTexture(texture);
			CCSprite* sprite2 = CCSprite::spriteWithTexture(texture2);
//			sprite->setScale(2.f);
//			sprite2->setScale(2.f);
			
//			sprite->setPosition(ccp(240, 160));
//			addChild(sprite, 3);
//			CCSprite* xButton = CCSprite::spriteWithFile("cancel_down.png");
//			xButton->setPosition(ccp(sprite->getContentSize().width, sprite->getContentSize().height));
//			sprite->addChild(xButton);
			HouseAd* had = HouseAd::node();
			had->setAd(sprite, sprite2, storeLocation, aid);
			addChild(had, INT_MAX);
			
			texture->release();
			delete img;// in cocos2d-x 1.x
			if(chunk_index.first.memory)
				free(chunk_index.first.memory);
			chunks.pop();
		} catch (...) {
			texture->release();
			delete img;// in cocos2d-x 1.x
			if(chunk_index.first.memory)
				free(chunk_index.first.memory);
			chunks.pop();
		}
	}
}
void SWIntro::afterInit()
{
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("flags.plist");
	
	float loadingX = 240;
	float loadingY = 160;
	auto animator = SceneUtil::playAnimation("loading.png", 0.1, 9, 9, 56, 56, true);
	animator.second->runAction(animator.first);
	addChild(animator.second, INT_MAX);
	animator.second->setPosition(ccp(loadingX, loadingY));
	loading = animator.second;
	loading->setIsVisible(false);
	/*
	 Home Trip.mp3");
	 Retro Space.mp3");
	 Let me Fly.mp3");
	 Jacky.mp3");
	 */
	//	KSoundEngine::sharedEngine()->playSound("Home Trip.mp3");
	KSoundEngine::sharedEngine()->playSound("RetroSpace.mp3");
	//	KSoundEngine::sharedEngine()->playSound("Let me Fly.mp3");
	//	KSoundEngine::sharedEngine()->playSound("Jacky.mp3");
	if(initMode == INIT_MODE::STARTUP)
	{
		schedule(schedule_selector(SWIntro::imageDown));
		GraphDog::get()->setFlag(NSDefault::getCountry());
		GraphDog::get()->setNick(NSDefault::getUserName());
		GraphDog::get()->setup("sportsworldcup", "GDSK3388", "com.litqoo.lib.FBConnectorBase", 14);
		
		CCLog("end setup %s", KS::getLocalCode().c_str());
		//		graphdog->setLanguage(KS::getLocalCode());
		//////////////
		vector<CommandParam> vcp;
		{
			JsonBox::Object param;
			//			JsonBox::Array category;
			param["category"] = "housead";
			param["limit"] = 1;
			param["autoremove"] = true;
			vcp.push_back(CommandParam("getrequests", param, this, gd_selector(ThisClassType::finishGetHouseAd)));
		}
		{
			JsonBox::Object param;
			//			JsonBox::Array category;
			param["category"] = "update";
			param["autoremove"] = false;
			vcp.push_back(CommandParam("getrequests", param, this, gd_selector(ThisClassType::finishGetUpdate)));
		}
		graphdog->command(vcp);
		
		
		//		param["limit"] = 1;
	}
	//this->setIsTouchEnabled(true);
	CCSprite* mainBack = CCSprite::spriteWithFile("main_back.png");
	mainBack->setPosition(ccp(240, 160));
	addChild(mainBack);
	//CCSize aa = CCEGLView::sharedOpenGLView().getSize();
	
	gameOverWindow = GameOver::node(this);
	gameOverWindow->setPosition(ccp(-400, 100));
	addChild(gameOverWindow, 3);
	
	battleGameOverWindow = BattleGameOver::node(this);
	battleGameOverWindow->setPosition(ccp(-400, 100));
	addChild(battleGameOverWindow, 3);
	
	giftCodeWindow = 0;
	
	
	moreAppsWindow = 0;
	
	
	introHead = IntroHead::node(this);
	playInfo->pIntroHead = introHead;
	introHead->syncTimeWithInc();
	addChild(introHead, INT_MAX);
	
	
	
	
	countryRank = CountryRank::node(this);
	countryRank->setPosition(ccp(143, 500));
	addChild(countryRank, 3);
	
	postBox = FacebookPost::node(this);
	postBox->setPosition(ccp(200, 500));
	addChild(postBox, 3);
	
	optionWindow = Option::node(this);
	optionWindow->setPosition(ccp(200, 500));
	addChild(optionWindow, 3);
	
	medalWindow = 0;
	
	
	gameSelect = 0;
	legendaryWindow = 0;
	
	
	
	myScoreWindow = MyScore::node(this);
	myScoreWindow->setPosition(ccp(position.MYSCORE.x + 250, position.MYSCORE.y));
	addChild(myScoreWindow, 3);
	
	rubyWindow = 0;
	goldWindow = 0;
	heartWindow = 0;
	archeryWindow = 0;
	basketWindow = 0;
	hurdleWindow = 0;
	clayWindow = 0;
	skiWindow = 0;
	battleShop = 0;
	CCMenu* _menu = CCMenu::menuWithItems(0);
	_menu->setPosition(CCPointZero);
	addChild(_menu, 3);
	{
		startGameBtn = CCMenuItemImageDown::itemFromNormalImage("main_start.png", "main_start.png",
																this, menu_selector(ThisClassType::START));
		startGameBtn->setPosition(ccp(position.START.x, position.START.y - 200));
		_menu->addChild(startGameBtn, 3);
		
		worldcupMark = CCSprite::spriteWithFile("worldcup_startgold.png");
		worldcupMark->setPosition(ccp(146, 18));
		startGameBtn->addChild(worldcupMark, 2);
		worldcupMark->setIsVisible(false);
		
	}
	
	{
		moreapps = CCMenuItemImageDown::itemFromNormalImage("main_moreapps.png", "main_moreapps.png",
															this, menu_selector(ThisClassType::MOREAPPS));
		moreapps->setPosition(ccp(position.MOREAPPS.x + 300, position.MOREAPPS.y));
		
		
		_menu->addChild(moreapps, 3);
	}
	{
		option = CCMenuItemImageDown::itemFromNormalImage("main_option.png", "main_option.png",
														  this, menu_selector(ThisClassType::OPTION));
		option->setPosition(ccp(position.OPTION.x + 300, position.OPTION.y));
		_menu->addChild(option, 3);
	}
	{
		home = CCMenuItemImageDown::itemFromNormalImage("gameover_home.png", "gameover_home.png",
														this, menu_selector(ThisClassType::HOME));
		home->setPosition(ccp(position.HOME.x + 300, position.HOME.y));
		_menu->addChild(home, 3);
	}
	titleBack = CCSprite::spriteWithFile("main_titleback.png");
	titleBack->setPosition(ccp(240, 160));
	titleBack->runAction(CCRepeatForever::actionWithAction(CCRotateBy::actionWithDuration(2.f*1.f/0.33f, 360.f)));
	addChild(titleBack, 2);
	title = CCSprite::spriteWithFile("main_title.png");
	title->setPosition(ccp(240, 160));
	addChild(title, 2);
	
	facebookRank = FacebookRank::node(this);
	facebookRank->setPosition(ccp(143, 500));
	addChild(facebookRank, 3);
	
	string newGameFileName;
	
	if(initMode == INIT_MODE::STARTUP)
	{
		
		vector<CommandParam> vcp;
		{
			vcp.push_back(CommandParam("getweek", 0, this, gd_selector(ThisClassType::finishGetWeek)));
		}
		graphdog->command(vcp);
		
		if(FBConnector::get()->isUsed() && FBConnector::get()->isLogin())
		{
			facebookRank->fbLoading->setIsVisible(true);
			FBConnector::get()->getRequests(facebookRank, fb_selector(FacebookRank::getRequestCount));
			FBConnector::get()->getRequests(facebookRank, fb_selector(FacebookRank::getRequestFakeFinish));
			FBConnector::get()->getScores(facebookRank, fb_selector(FacebookRank::facebookGetScoresFinish));
		}
		
		newGameFileName = "gameover_newgame.png";
		
		restartGame = CCMenuItemImageDown::itemFromNormalImage(newGameFileName.c_str(), newGameFileName.c_str(),
															   this, menu_selector(ThisClassType::RESTART));
		restartGame->setPosition(ccp(position.RESTART.x, position.RESTART.y - 200));
		_menu->addChild(restartGame, 3);

	}
	else if(initMode == INIT_MODE::GAMEOVER)
	{
		int gainScore;
		if(playInfo->currentGame == "BS")
		{
			gainScore = bs2play->gameScore.getVar();
		}
		else if(playInfo->currentGame == "AK")
		{
			gainScore = akplay->gameScore.getVar();
		}
		else if(playInfo->currentGame == "AC")
		{
			gainScore = acplay->gameScore.getVar();
		}
		else if(playInfo->currentGame == "HW")
		{
			gainScore = hwplay->gameScore.getVar();
		}
		else if(playInfo->currentGame == "SK")
		{
			gainScore = skplay->gameScore.getVar();
		}
		JsonBox::Object param;
		param["score"] = gainScore;
		param["isover"] = true;
		
		JsonBox::Object userdata;
		for(auto iter = wcplay->scoreEachGame.begin(); iter != wcplay->scoreEachGame.end(); ++iter)
		{
			userdata[iter->first] = iter->second;
		}
		
		userdata["gold"] = playInfo->__ateCoin;
		userdata["level"] = playInfo->getLV_EXP(NSDefault::getExp()).first;
		userdata["exp"] = NSDefault::getExp();
		userdata["havingmoney"] = NSDefault::getGold();
		userdata["heart"] = NSDefault::getHeartNumber();
		userdata["device"] = graphdog->getDeviceInfo();
		string items;
		for(auto iter = playInfo->selectedItems.begin(); iter != playInfo->selectedItems.end(); ++iter)
		{
			if((*iter).second)
				items += (*iter).first + "/";
		}
		userdata["items"] = items;
		
		
		
		param["userdata"] = userdata;
		JsonBox::Object param2;
		param2["type"] = "WORLDCUP";
		
		vector<CommandParam> vcp;
		vcp.push_back(CommandParam("getscores", param, gameOverWindow, gd_selector(GameOver::finishGetScores)));
		
		graphdog->command(vcp);
		
		newGameFileName = "gameover_newgame.png";
		restartGame = CCMenuItemImageDown::itemFromNormalImage(newGameFileName.c_str(), newGameFileName.c_str(),
															   this, menu_selector(ThisClassType::RESTART));
		restartGame->setPosition(ccp(position.RESTART.x, position.RESTART.y - 200));
		_menu->addChild(restartGame, 3);
	}
	
	else if(initMode == INIT_MODE::WORLDCUP_GAMEOVER)
	{
		JsonBox::Object param;
		param["score"] = wcplay->accScore.getVar() + wcplay->gameScore.getVar();
		param["isover"] = true;
		JsonBox::Object userdata;
		for(auto iter = wcplay->scoreEachGame.begin(); iter != wcplay->scoreEachGame.end(); ++iter)
		{
			userdata[iter->first] = iter->second;
		}
		
		
		userdata["gold"] = 2643;
		param["userdata"] = userdata;
		JsonBox::Object param2;
		param2["type"] = "WORLDCUP";
		
		vector<CommandParam> vcp;
		vcp.push_back(CommandParam("getscores", param, gameOverWindow, gd_selector(GameOver::finishGetScores))); //## 중요.
		graphdog->command(vcp);
		
		
		
		newGameFileName = "worldcup_gameover_restart.png";
		CCScene* runningScene = CCDirector::sharedDirector()->getRunningScene();
		runningScene->addChild(ChangeRank::node(), INT_MAX);
		
		restartGame = CCMenuItemImageDown::itemFromNormalImage(newGameFileName.c_str(), newGameFileName.c_str(),
															   this, menu_selector(ThisClassType::RESTART));
		restartGame->setPosition(ccp(position.RESTART.x, position.RESTART.y - 200));
		_menu->addChild(restartGame, 3);
	}
	else if(initMode == INIT_MODE::BATTLE_GAMEOVER)
	{
		newGameFileName = "worldcup_gameover_restart.png";
		restartGame = CCMenuItemImageDown::itemFromNormalImage(newGameFileName.c_str(), newGameFileName.c_str(),
															   this, menu_selector(ThisClassType::RESTART));
		restartGame->setPosition(ccp(position.RESTART.x, position.RESTART.y - 200));
		_menu->addChild(restartGame, 3);
		JsonBox::Object ee;
		battleGameOverWindow->finishGetScores(ee);
		
	}
	
	//	CCLog("GraphDogLib::getDeviceInfo() ------- %s", GraphDogLib::getDeviceInfo().c_str());
	
}

SWIntro::~SWIntro()
{
	FBDelegator::getInstance()->removeTarget(this);
	
	CCArray* children = this->getChildren();
	for(int i=0; i<children->count(); i++)
	{
		CCObject* tempNode = children->objectAtIndex(i);
		if(tempNode != 0 && 1<tempNode->retainCount())
		{
			while(1<tempNode->retainCount())
				tempNode->release();
		}
	}
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
	CCTextureCache::sharedTextureCache()->removeAllTextures();
	
	CCNotificationCenter::sharedNotifCenter()->removeObserver(this, "sendHeartTo");
	CCNotificationCenter::sharedNotifCenter()->removeObserver(this, "viewProfile");
	CCNotificationCenter::sharedNotifCenter()->removeObserver(this, "viewPostBox");
	CCNotificationCenter::sharedNotifCenter()->removeObserver(this, "showInviteFriendWindow");
	CCNotificationCenter::sharedNotifCenter()->removeObserver(this, "inviteFriend");
	CCNotificationCenter::sharedNotifCenter()->removeObserver(this, "battleSelectWindow");
	CCNotificationCenter::sharedNotifCenter()->removeObserver(this, "showBattleShop");
}

bool SWIntro::init()
{
	CCLayer::init();
	runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.1f),
									   CCCallFunc::actionWithTarget(this, callfunc_selector(SWIntro::afterInit))));
	
	
	
	CCNotificationCenter::sharedNotifCenter()->addObserver(this, callfuncO_selector(ThisClassType::sendHeartTo), "sendHeartTo", 0);
	CCNotificationCenter::sharedNotifCenter()->addObserver(this, callfuncO_selector(ThisClassType::viewProfile), "viewProfile", 0);
	CCNotificationCenter::sharedNotifCenter()->addObserver(this, callfuncO_selector(ThisClassType::viewPostBox), "viewPostBox", 0);
	CCNotificationCenter::sharedNotifCenter()->addObserver(this, callfuncO_selector(ThisClassType::showInviteFriendWinndow), "showInviteFriendWindow", 0);
	CCNotificationCenter::sharedNotifCenter()->addObserver(this, callfuncO_selector(ThisClassType::inviteFriend), "inviteFriend", 0);
	CCNotificationCenter::sharedNotifCenter()->addObserver(this, callfuncO_selector(ThisClassType::battleSelectWindow), "battleSelectWindow", 0);
	CCNotificationCenter::sharedNotifCenter()->addObserver(this, callfuncO_selector(ThisClassType::showBattleShop), "showBattleShop", 0);
	
//	CCNotificationCenter::sharedNotifCenter()->addObserver(this, callfuncO_selector(ThisClassType::showInviteEvent), "showInviteEvent", 0);
//	CCNotificationCenter::sharedNotifCenter()->addObserver(this, callfuncO_selector(ThisClassType::hideInviteEvent), "hideInviteEvent", 0);
	
	
	return true;
}


void SWIntro::goReview()
{
	write_review();
	NSDefault::setIsReview(1);
}
void SWIntro::finishFacebookSendScore(JsonBox::Value v)
{
	//if(FBConnector::get()->isUsed() && FBConnector::get()->isLogin())
	
	KS::KSLog("%", v);
	{
		facebookRank->fbLoading->setIsVisible(true);
		FBConnector::get()->getRequests(facebookRank, fb_selector(FacebookRank::getRequestCount));
		FBConnector::get()->getRequests(facebookRank, fb_selector(FacebookRank::getRequestFakeFinish));
		FBConnector::get()->getScores(facebookRank, fb_selector(FacebookRank::facebookGetScoresFinish));
	}
}
void SWIntro::GAMEOVERRETRY()
{
	int gainScore;
	if(playInfo->currentGame == "BS")
	{
		gainScore = bs2play->gameScore.getVar();
	}
	else if(playInfo->currentGame == "AK")
	{
		gainScore = akplay->gameScore.getVar();
	}
	else if(playInfo->currentGame == "AC")
	{
		gainScore = acplay->gameScore.getVar();
	}
	else if(playInfo->currentGame == "HW")
	{
		gainScore = hwplay->gameScore.getVar();
	}
	else if(playInfo->currentGame == "WORLDCUP")
	{
		gainScore = wcplay->accScore.getVar() + wcplay->gameScore.getVar();
	}
	JsonBox::Object param;
	param["score"] = gainScore;
	param["isover"] = true;
	
	JsonBox::Object userdata;
	for(auto iter = wcplay->scoreEachGame.begin(); iter != wcplay->scoreEachGame.end(); ++iter)
	{
		userdata[iter->first] = iter->second;
	}
	userdata["gold"] = playInfo->__ateCoin;
	userdata["level"] = playInfo->getLV_EXP(NSDefault::getExp()).first;
	userdata["exp"] = NSDefault::getExp();
	userdata["havingmoney"] = NSDefault::getGold();
	userdata["heart"] = NSDefault::getHeartNumber();
	userdata["device"] = graphdog->getDeviceInfo();
	string items;
	for(auto iter = playInfo->selectedItems.begin(); iter != playInfo->selectedItems.end(); ++iter)
	{
		if((*iter).second)
			items += (*iter).first + "/";
	}
	userdata["items"] = items;
	param["userdata"] = userdata;
	graphdog->command("getscores", &param, gameOverWindow, gd_selector(GameOver::finishGetScores));
}

void SWIntro::finishGetWeek(JsonBox::Object js)
{
	incLoadCount();
//	mediator->incLoadCount();
	if(js["state"].getString() != "ok")
		return;
	playInfo->currentWeek = js["week"].getInt();
	playInfo->whenRecvLeftTime = GameSystem::getCurrentTime_s();
	setFacebookLeftTime(js["lefttime"].getInt());
	if(playInfo->currentWeek != ACSaveData::getWeeklyMaxScore().first)
		ACSaveData::setWeeklyMaxScore(playInfo->currentWeek, 0);
	if(playInfo->currentWeek != AKSaveData::getWeeklyMaxScore().first)
		AKSaveData::setWeeklyMaxScore(playInfo->currentWeek, 0);
	if(playInfo->currentWeek != BS2SaveData::getWeeklyMaxScore().first)
		BS2SaveData::setWeeklyMaxScore(playInfo->currentWeek, 0);
	if(playInfo->currentWeek != HWSaveData::getWeeklyMaxScore().first)
		HWSaveData::setWeeklyMaxScore(playInfo->currentWeek, 0);
	if(playInfo->currentWeek != WCSaveData::getWeeklyMaxScore().first)
		WCSaveData::setWeeklyMaxScore(playInfo->currentWeek, 0);
}


void SWIntro::MOREAPPS(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	showMoreApps();
}
void SWIntro::OPTION(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	
	if(catchingColleague != optionWindow)
		showOption();	
}

void SWIntro::showRankWithMenu()
{
	if(medalWindow == 0)
	{
		medalWindow = MyMedal::node(this);
		medalWindow->setPosition(ccp(position.MEDAL_FRAME.x + 250, position.MEDAL_FRAME.y));
		addChild(medalWindow, 3);
	}
	medalWindow->runAction(CCMoveBy::actionWithDuration(0.3f, ccp(300, 0)));
	home->runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.1f), CCMoveBy::actionWithDuration(0.3f, ccp(300, 0))));
	restartGame->runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.01f), CCMoveBy::actionWithDuration(0.3f, ccp(0, -200))));
	startGameBtn->runAction(CCMoveTo::actionWithDuration(0.5f, position.START));
	title->runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.2f), CCMoveTo::actionWithDuration(0.3f, position.CUP)));
	titleBack->runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.2f), CCMoveTo::actionWithDuration(0.3f, position.CUP)));
	
	moreapps->runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.3f), CCMoveTo::actionWithDuration(0.3f, position.MOREAPPS)));
	option->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.3f), CCMoveTo::actionWithDuration(0.3f, position.OPTION),
										  KSFunc(
												 {
													 startGameBtn->setIsEnabled(true);
												 }
												 ) ,0));
	myScoreWindow->runAction(CCMoveTo::actionWithDuration(0.3f, ccp(position.MYSCORE.x + 250, position.MYSCORE.y)));
	startGameBtn->setIsEnabled(false);
//	titleBack->runAction(CCFadeIn::actionWithDuration(0.5f));
//	title->runAction(CCFadeIn::actionWithDuration(0.5f));
	if(gameSelect && gameSelect == catchingColleague)
		gameSelect->hideGames();
	showRank();
}
void SWIntro::showRank()
{
	if(true)
	{
		showFacebookRank();
	}
	else //##
	{
		
	}
	
	return; //##
	if(FBConnector::get()->isUsed())
	{
		showFacebookRank();
	}
	else
	{
		int r = rand()%5;
		if(r == 0)
		{
			showFacebookRank();
		}
		else
		{
			showCountryRank();
		}
	}
	
}

void SWIntro::incLoadCount()
{
	loadCount++;
	if(bLoaded) // 이미 로드 되어있으면 별다른 조치를 안취함
		return;
	int loadCnt = 0;
	// 처음엔 country, getweek, syncwithinc
	// 게임오버 시 country, gameover 로드, syncTime 로드 그래서 3
	if(initMode == INIT_MODE::STARTUP)
	{
		loadCnt++; // getweek
		loadCnt++; // syncwithinc
		loadCnt++; // 공지.
		loadCnt++; // 업데이트 확인.
	}else
	{
		loadCnt++; // gameover
		loadCnt++; // syncwithinc
	}
	
	bool loadComplete = false;
	if(FBConnector::get()->isUsed() && FBConnector::get()->isLogin())
	{
		if(loadCount >= loadCnt + 1) // facebook 때문에 + 1
		{
			loadComplete = true;
		}
	}
	else
	{
		if(loadCount >= loadCnt)
			loadComplete = true;
	}
	
	if(loadComplete)
	{
		if(initMode == INIT_MODE::STARTUP)
		{
			CCDirector::sharedDirector()->getRunningScene()->addChild(DailyCheck::node(), INT_MAX);
			
			bLoaded = true;
			showRank();
			moreapps->setPosition(ccp(position.MOREAPPS.x + 300, position.MOREAPPS.y));
			option->setPosition(ccp(position.OPTION.x + 300, position.OPTION.y));
			startGameBtn->setPosition(ccp(position.START.x, position.START.y - 200));
			
			moreapps->runAction(CCMoveTo::actionWithDuration(0.5f, position.MOREAPPS));
			option->runAction(CCMoveTo::actionWithDuration(0.5f, position.OPTION));
			startGameBtn->runAction(CCMoveTo::actionWithDuration(0.5f, position.START));
			titleBack->runAction(CCMoveTo::actionWithDuration(0.5f, position.CUP));
			title->runAction(CCMoveTo::actionWithDuration(0.5f, position.CUP));
			
			introHead->runAction(CCMoveTo::actionWithDuration(0.5f, position.INTROHEAD));
		}
		else if(initMode == INIT_MODE::GAMEOVER || initMode == INIT_MODE::WORLDCUP_GAMEOVER)
		{
			bLoaded = true;
			showGameOver();
			
		}
		else if(initMode == INIT_MODE::BATTLE_GAMEOVER)
		{
			bLoaded = true;
			showBattleGameOver();
		}
	}
}
void SWIntro::HOME(CCObject*)
{
//	medalWindow->runAction(CCMoveBy::actionWithDuration(0.3f, ccp(300, 0)));
//	home->runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.1f), CCMoveBy::actionWithDuration(0.3f, ccp(300, 0))));
//	restartGame->runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.01f), CCMoveBy::actionWithDuration(0.3f, ccp(0, -200))));
//	startGame->runAction(CCMoveTo::actionWithDuration(0.5f, position.START));
//	title->runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.2f), CCMoveTo::actionWithDuration(0.3f, position.CUP)));
//	titleBack->runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.2f), CCMoveTo::actionWithDuration(0.3f, position.CUP)));
//	
//	moreapps->runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.3f), CCMoveTo::actionWithDuration(0.3f, position.MOREAPPS)));
//	option->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.3f), CCMoveTo::actionWithDuration(0.3f, position.OPTION),
//					  KSFunc(
//							{
//								startGame->setIsEnabled(true);
//							}
//					) ,0));
//	myScoreWindow->runAction(CCMoveTo::actionWithDuration(0.3f, ccp(position.MYSCORE.x + 250, position.MYSCORE.y)));
//	startGame->setIsEnabled(false);
//	titleBack->runAction(CCFadeIn::actionWithDuration(0.5f));
//	title->runAction(CCFadeIn::actionWithDuration(0.5f));
//	gameSelect->hideGames();
//	showRank();
	if(checkBeforeMoving() == false)
		return;
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	showWorldcupMark(false);
	showRankWithMenu();
}

void SWIntro::startGame(CCNode* colleague, JsonBox::Object param)
{
	if(param["type"].getString() == "WORLDCUP" || param["type"].getString() == "BATTLE")
	{
		if(NSDefault::getGold() >= GameSystem::WORLDCUP_ENTRANCE_FEE) // ##
		{
			BASETIME = 30.f; // 30초 기준.
			DisableTouch* cl = DisableTouch::node();
			addChild(cl);
			ostringstream oss;
			oss << (int)cl;
			param["callback"] = oss.str();
			loading->setIsVisible(true);
			
			graphdog->command("startscores", &param, this, gd_selector(SWIntro::finishStartScores));
		}
		else
		{
			KSModalAlert("", ML::key("require_coin").c_str(), 1, "OK",
						 NOPARAM);
		}
	}
	else if(NSDefault::getHeartNumber() > 0 || NSDefault::getIsTodayFree())
	{
		GameShop* gs = dynamic_cast<GameShop*>(colleague);
		if(gs)
		{
			if(NSDefault::getGold() >= gs->getRealPrice())
			{
				BASETIME = 60.f; // 60초 기준
				playInfo->selectedItems.clear();
				playInfo->selectedItems = gs->selectedItems;
				
				DisableTouch* cl = DisableTouch::node();
				addChild(cl);
				
				ostringstream oss;
				oss << (int)cl;
				param["callback"] = oss.str();
				loading->setIsVisible(true);
				
				graphdog->command("startscores", &param, this, gd_selector(SWIntro::finishStartScores));
			}
			else
			{
				KSModalAlert("", ML::key("require_coin").c_str(), 1, "OK",
							 NOPARAM);
			}
		}
	}
	else
	{
		KSModalAlert("", ML::key("require_heart").c_str(), 1, "OK",
					 NOPARAM);
		showHeartShop();
	}
}
void SWIntro::START(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	// 이미 게임 샵 화면이면 선택된 게임을 시작함.
	if(catchingColleague == hurdleWindow || catchingColleague == archeryWindow || catchingColleague == basketWindow ||
		catchingColleague == clayWindow || catchingColleague == skiWindow || catchingColleague == battleShop)
	{
		JsonBox::Object param;
		if(catchingColleague == hurdleWindow)
		{
			param["type"] = "HW";
		}
		else if(catchingColleague == archeryWindow)
		{
			param["type"] = "AC";
		}
		else if(catchingColleague == basketWindow)
		{
			param["type"] = "BS";
		}
		else if(catchingColleague == clayWindow)
		{
			param["type"] = "AK";
		}
		else if(catchingColleague == skiWindow)
		{
			param["type"] = "SK";
		}
		else if(catchingColleague == battleShop)
		{
			playInfo->createWorldcupOrder();
			wcplay->initVars();
			param["type"] = "BATTLE";
		}
		startGame(catchingColleague, param);
	}
	else if(catchingColleague == gameSelect)
	{
		if(gameSelect->getCurrentIndex() == GameSelect::COMINGSOON) // 월드컵모드라면 돈이 필요함.
		{
			playInfo->createWorldcupOrder();
			wcplay->initVars();
			JsonBox::Object param;
			param["type"] = "WORLDCUP";
			startGame(catchingColleague, param);
//			KSModalAlert("", "coming soon", 1, "OK",
//								  NOPARAM);
		}
		else
		{
			
			switch(gameSelect->getCurrentIndex())
			{
				case GameSelect::BASKETBALL_INDEX:
					showBasketShop();
					break;
				case GameSelect::CLAY_INDEX:
					showClayShop();
					break;
				case GameSelect::HURDLE_INDEX:
					showHurdleShop();
					break;
				case GameSelect::ARCHERY_INDEX:
					showArcheryShop();
					break;
				case GameSelect::SKI_INDEX:
					showSkiShop();
					break;
				case GameSelect::BATTLE_INDEX:
					battleSelectWindow();
					break;
			}
			if(gameSelect->getCurrentIndex() != GameSelect::BATTLE_INDEX)
				gameSelect->hideGames();
		}
		
	}
	// 아닌 경우엔 게임 셀렉을 띄움.
	else
	{
		if(checkBeforeMoving() == false)
			return;
		title->runAction(CCMoveTo::actionWithDuration(0.3f, ccp(position.CUP.x + 300.f, position.CUP.y) ));
		titleBack->runAction(CCMoveTo::actionWithDuration(0.3f, ccp(position.CUP.x + 300.f, position.CUP.y) ));
		
		moreapps->runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.1f),
													 CCMoveTo::actionWithDuration(0.3f, ccp(position.MOREAPPS.x + 300.f, position.MOREAPPS.y) )) );
		option->runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.1f),
												   CCMoveTo::actionWithDuration(0.3f, ccp(position.OPTION.x + 300.f, position.OPTION.y) ))
												   
												   );
		
		if(medalWindow == 0)
		{
			medalWindow = MyMedal::node(this);
			medalWindow->setPosition(ccp(position.MEDAL_FRAME.x + 250, position.MEDAL_FRAME.y));
			addChild(medalWindow, 3);
		}
		
		medalWindow->runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.2f), CCMoveTo::actionWithDuration(0.3f, position.MEDAL_FRAME)));
		home->runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.3f), CCMoveTo::actionWithDuration(0.3f, position.HOME)));
		
		myScoreWindow->runAction(CCMoveTo::actionWithDuration(0.3f, ccp(position.MYSCORE.x + 250, position.MYSCORE.y)));
		showGameSelect();
	}
}

void SWIntro::RESTART(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	if(initMode == INIT_MODE::WORLDCUP_GAMEOVER)
	{ // 바로
		playInfo->createWorldcupOrder();
		wcplay->initVars();
		JsonBox::Object param;
		param["type"] = "WORLDCUP";
		startGame(catchingColleague, param);
	}
	else if(initMode == INIT_MODE::BATTLE_GAMEOVER)
	{
		playInfo->createWorldcupOrder();
		wcplay->initVars();
		JsonBox::Object param;
		param["type"] = "BATTLE";
		startGame(catchingColleague, param);
	}
	else
	{
		if(catchingColleague == hurdleWindow || catchingColleague == archeryWindow || catchingColleague == basketWindow ||
		   catchingColleague == clayWindow || catchingColleague == skiWindow)
		{
			START(0);
		}
		else// if(catchingColleague == gameOverWindow)
		{
			if(playInfo->currentGame == "BS")
			{
				showBasketShop();
			}
			else if(playInfo->currentGame == "HW")
			{
				showHurdleShop();
			}
			else if(playInfo->currentGame == "AC")
			{
				showArcheryShop();
			}
			else if(playInfo->currentGame == "AK")
			{
				showClayShop();
			}
			else if(playInfo->currentGame == "SK")
			{
				showSkiShop();
			}
		}
	}
	
}
void SWIntro::finishStartScores(JsonBox::Object js)
{
	loading->setIsVisible(false);
//	loading->removeFromParentAndCleanup(true);
	JsonBox::Object obj = js["param"].getObject();
	string _cl = obj["callback"].getString();
	
	CCNode* cl = (CCNode*)atoi(_cl.c_str());
	try {
		ostringstream oss;
		oss << js << endl;
		KS::KSLog("%", oss.str().c_str());
		
		

		if(js["state"].getString() != "ok")
		{
			throw string(ML::key("netfail"));
		}
		GameShop* gs = dynamic_cast<GameShop*>(catchingColleague);
		if(gs)
		{
			NSDefault::setGold(NSDefault::getGold() - gs->getRealPrice());
			if(gs->getRealPrice() != 0)
			{
				KSoundEngine::sharedEngine()->playSound("usemoney.mp3");
				JsonBox::Object gParam;
				gParam["category"] = "useitem";
				gParam["log"] = gs->getSelectedItems();
				graphdog->command("writelog", &gParam, 0, 0);
			}
			introHead->setGold(NSDefault::getGold());
		}
		JsonBox::Object paramObj = js["param"].getObject();
		string gameType = paramObj["type"].getString();
		
		playInfo->selectedItems["item2"] = true; //## 임시로 쉽게
		if(gameType == "WORLDCUP")
		{
			NSDefault::setGold(NSDefault::getGold() - GameSystem::WORLDCUP_ENTRANCE_FEE);
			KSoundEngine::sharedEngine()->playSound("usemoney.mp3");
			JsonBox::Object gParam;
			gParam["category"] = "startchallenge";
			gParam["log"] = "챌린지 시작함 ㅋㅋㅋ";
			graphdog->command("writelog", &gParam, 0, 0);
			introHead->setGold(NSDefault::getGold());
		}
		else if(gameType == "BATTLE")
		{
			//NSDefault::setGold(NSDefault::getGold() - GameSystem::WORLDCUP_ENTRANCE_FEE); //##
			KSoundEngine::sharedEngine()->playSound("usemoney.mp3");
			JsonBox::Object gParam;
			gParam["category"] = "startbattle";
			gParam["log"] = "배틀시작";
			graphdog->command("writelog", &gParam, 0, 0);
			introHead->setGold(NSDefault::getGold());
		}
		else
		{
			if(NSDefault::getIsTodayFree() == false) // 24H 무료 모드가 아니면 깎음.
			{
				introHead->removeHeartAnimation();
				if(NSDefault::getHeartNumber() == 5)
					NSDefault::setHeartBaseTime(GameSystem::getCurrentTime_s());
				NSDefault::setHeartNumber(NSDefault::getHeartNumber() - 1);
			}
			
			
		}
		
		// 보이는 것들 다 치우고 참가자 모집 에니메이션 띄움.
		float executeTime = 0.f;
		if(gameType != "BATTLE")
		{
			executeTime = 4.5f;
			runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.3f),
											   KSFunc(
													  {
														  catchingColleague->runAction(CCMoveTo::actionWithDuration(0.3f, ccp(position.CONTENT.x - 400, position.CONTENT.y)));
														  introHead->runAction(CCMoveTo::actionWithDuration(0.3f, ccp(position.INTROHEAD.x, position.INTROHEAD.y + 100)));
														  if(medalWindow)
															  medalWindow->runAction(CCMoveTo::actionWithDuration(0.3f, ccp(position.MEDAL_FRAME.x + 300, position.MEDAL_FRAME.y )));
														  if(myScoreWindow)
															  myScoreWindow->runAction(CCMoveTo::actionWithDuration(0.3f, ccp(position.MYSCORE.x + 300, position.MYSCORE.y)));
														  
														  if(catchingColleague != gameOverWindow)
														  {
															  title->runAction(CCMoveTo::actionWithDuration(0.3f, ccp(position.CUP.x + 300.f, position.CUP.y) ));
															  titleBack->runAction(CCMoveTo::actionWithDuration(0.3f, ccp(position.CUP.x + 300.f, position.CUP.y) ));
														  }
														  
														  home->runAction(CCMoveTo::actionWithDuration(0.3f, ccp(position.HOME.x + 300.f, position.HOME.y) ));
														  startGameBtn->runAction(CCMoveTo::actionWithDuration(0.3f, ccp(position.START.x + 300.f, position.START.y) ));
														  restartGame->runAction(CCMoveTo::actionWithDuration(0.3f, ccp(position.START.x + 300.f, position.START.y) ));

														  
														  
														  runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.4f),
																							 KSFunc({
															  CCLog("search_title.png");
															  CCSprite* searchTitle = CCSprite::spriteWithFile("search_title.png");
															  searchTitle->setPosition(ccp(240, 250));
															  addChild(searchTitle, 2);
															  searchTitle->runAction(CCFadeIn::actionWithDuration(0.3f));
															  loading->runAction(CCFadeIn::actionWithDuration(0.3f));
															  loading->setIsVisible(true);
															  
															  runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(1.5f),
																								 KSFunc({
																  CCSprite* searchList = CCSprite::spriteWithFile("enemylist.png");
																  searchList->setPosition(ccp(240, -100));
																  addChild(searchList, INT_MAX);
																  searchList->runAction(CCMoveTo::actionWithDuration(0.4f, ccp(240, 160)));
															  })));
														  })));
														  
														  
														  
													  })));

		}
		else
			executeTime = 0.5f;
//		CCMoveBy::action
		
				
		runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(executeTime), //0.5f
										   KSFunc(
												  {
													  playInfo->currentGame = gameType;
													  
													  
													  
													  if(playInfo->currentGame == "WORLDCUP")
													  {
														  playInfo->selectedItems.clear();
														  string ret = playInfo->getWorldcupGame();
														  
														  //														  std::pair<std::string, KSProtectVar<int>> tt = std::make_pair(ret, KSProtectVar<int>(0));
														  //														  wcplay->scoreEachGame.push_back(tt);
														  playInfo->popGame();
														  playInfo->initGameAndStart(ret);
													  }
													  else if(playInfo->currentGame == "BATTLE")
													  {
														  playInfo->selectedItems.clear();
														  string ret = playInfo->getWorldcupGame();
														  playInfo->popGame();
														  playInfo->initGameAndStart(ret);
													  }
													  else
													  {
														  if(playInfo->currentGame == "BS")
														  {
															  if(NSDefault::getBasketCount() <= 5)
															  {
																  NSDefault::incBasketCount();
																  ReplaceScene(Tutorial);
															  }
															  else
															  {
																  bs2play->initVars();
																  ReplaceScene(BasketGame);
															  }
															  
														  }
														  else if(playInfo->currentGame == "HW")
														  {
															  if(NSDefault::getHurdleCount() <= 5)
															  {
																  NSDefault::incHurdleCount();
																  ReplaceScene(Tutorial);
															  }
															  else
															  {
																  hwplay->initVars();
																  ReplaceScene(HWGameDirector);
															  }
														  }
														  else if(playInfo->currentGame == "AC")
														  {
															  if(NSDefault::getArcherCount() <= 5)
															  {
																  NSDefault::incArcherCount();
																  ReplaceScene(Tutorial);
															  }
															  else
															  {
																  acplay->initVars();
																  ReplaceScene(ACGameScene);
															  }
															  
														  }
														  else if(playInfo->currentGame == "AK")
														  {
															  if(NSDefault::getClayCount() <= 5)
															  {
																  NSDefault::incClayCount();
																  ReplaceScene(Tutorial);
															  }
															  else
															  {
																  akplay->initVars();
																  ReplaceScene(AngryGameScene);
															  }
															  
														  }
														  else if(playInfo->currentGame == "SK")
														  {
															  if(NSDefault::getSkiCount() <= 5)
															  {
																  NSDefault::incSkiCount();
																  ReplaceScene(Tutorial);
															  }
															  else
															  {
																  skplay->initVars();
																  ReplaceScene(SKGameScene);
															  }
														  }
														  //playInfo->initGameAndStart(playInfo->currentGame); // 이거 왜 있는거지?
													  }
												  }
												  
												  )));
	} catch (const string& msg) {
		cl->removeFromParentAndCleanup(true);
		KSModalAlert("", msg.c_str(), 1,
					 "OK", NOPARAM);
	}
}

void SWIntro::finishInfo(JsonBox::Object js)
{
}

void SWIntro::getScores(JsonBox::Object js)
{
}

void SWIntro::onEnter()
{
	CCLayer::onEnter();
	

}
void SWIntro::onEnterTransitionDidFinish()
{
	CCLayer::onEnterTransitionDidFinish();
	
		
	
}
void SWIntro::onExit()
{
	playInfo->pIntroHead = 0;
	CCLayer::onExit();
}

void SWIntro::showFacebookRank(){
	if(checkBeforeMoving() == false)
		return;
	
	if(inviteEvent)
		inviteEvent->runAction(CCMoveTo::actionWithDuration(0.3f, ccp(position.EVENT.x + 300, position.EVENT.y)));
	title->runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.3f), CCMoveTo::actionWithDuration(0.3f, position.CUP)));
	titleBack->runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.3f), CCMoveTo::actionWithDuration(0.3f, position.CUP)));
	
	moreapps->runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.3f), CCMoveTo::actionWithDuration(0.3f, position.MOREAPPS)));
	option->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.3f), CCMoveTo::actionWithDuration(0.3f, position.OPTION),
										  KSFunc(
												 {
													 startGameBtn->setIsEnabled(true);
												 }
												 ) ,0));
	startGameBtn->runAction(CCMoveTo::actionWithDuration(0.5f, position.START));
	
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	facebookRank->setPosition(ccp(143, -144));
	facebookRank->runAction(CCMoveTo::actionWithDuration(0.5f, position.CONTENT));
	if(FBConnector::get()->isLogin())
	{
		FBConnector::get()->getRequests(facebookRank, fb_selector(FacebookRank::getRequestCount));
	}
	catchingColleague = facebookRank;
}
void SWIntro::showBackToFacebook(){
	if(checkBeforeMoving() == false)
		return;
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	facebookRank->setPosition(ccp(143, -144));
	facebookRank->runAction(CCMoveTo::actionWithDuration(0.5f, position.CONTENT));
	catchingColleague = facebookRank;
	if(FBConnector::get()->isLogin())
	{
		FBConnector::get()->getRequests(facebookRank, fb_selector(FacebookRank::getRequestCount));
	}
}
void SWIntro::showCountryRank(){
	if(checkBeforeMoving() == false)
		return;
	
	
	
	
	
	
	
	
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	countryRank->setPosition(ccp(143, -144));
	countryRank->runAction(CCMoveTo::actionWithDuration(0.5f, position.CONTENT));
	catchingColleague = countryRank;
}
void SWIntro::showFacebookPost(){
	if(checkBeforeMoving() == false)
		return;
	
	
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	postBox->setPosition(ccp(143, -144));
	postBox->runAction(CCMoveTo::actionWithDuration(0.5f, position.CONTENT));
	catchingColleague = postBox;
	postBox->loadRequest();
}
void SWIntro::showOption()
{
	if(checkBeforeMoving() == false)
		return;
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	optionWindow->setPosition(ccp(143, -144));
	optionWindow->runAction(CCMoveTo::actionWithDuration(0.5f, position.CONTENT));
	catchingColleague = optionWindow;
}
void SWIntro::showGameSelect()
{
	if(checkBeforeMoving() == false)
		return;
	
	if(gameSelect == 0)
	{
		gameSelect = GameSelect::node(this);
		gameSelect->setPosition(ccp(-400, 500));
		addChild(gameSelect, 3);
	}
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	gameSelect->showGames();
	gameSelect->setPosition(ccp(143, -144));
	gameSelect->runAction(CCMoveTo::actionWithDuration(0.5f, position.CONTENT));
	catchingColleague = gameSelect;
	
}
void SWIntro::showLegendaryRecord(string gameType)
{
	if(checkBeforeMoving() == false)
		return;
	if(legendaryWindow == 0)
	{
		legendaryWindow = Legendary::node(this);
		legendaryWindow->setPosition(ccp(-400, 500));
		addChild(legendaryWindow, 3);
	}
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	legendaryWindow->loadRecord(gameType);
	legendaryWindow->setPosition(ccp(143, -144));
	legendaryWindow->runAction(CCMoveTo::actionWithDuration(0.5f, position.CONTENT));
	catchingColleague = legendaryWindow;
	if(gameSelect)
		gameSelect->hideGames();
}

void SWIntro::showGameOver()
{
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	introHead->runAction(CCMoveTo::actionWithDuration(0.5f, position.INTROHEAD));
	gameOverWindow->setPosition(ccp(143, -144));
	introHead->expBar->setPercentage(0);
	gameOverWindow->runAction(CCSequence::actionOneTwo(CCMoveTo::actionWithDuration(0.5f, position.CONTENT), CCCallFunc::actionWithTarget(gameOverWindow, callfunc_selector(GameOver::appearMe)) ));
	
	myScoreWindow->runAction(CCMoveTo::actionWithDuration(0.3f, position.MYSCORE));
	home->runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.2f), CCMoveTo::actionWithDuration(0.3f, position.GAMEOVER_HOME)));
	restartGame->runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.3f), CCMoveTo::actionWithDuration(0.3f, position.RESTART)));
	
	titleBack->runAction(CCSequence::actions(CCFadeOut::actionWithDuration(0.5f), CCMoveTo::actionWithDuration(0.1f, ccp(-400, -400)),
											 CCFadeIn::actionWithDuration(0.1f), 0));
	title->runAction(CCSequence::actions(CCFadeOut::actionWithDuration(0.5f), CCMoveTo::actionWithDuration(0.1f, ccp(-400, -400)),
										 CCFadeIn::actionWithDuration(0.1f), 0));
	
	
	
	catchingColleague = gameOverWindow;
}

void SWIntro::showBattleGameOver()
{
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	introHead->runAction(CCMoveTo::actionWithDuration(0.5f, position.INTROHEAD));
	battleGameOverWindow->setPosition(ccp(143, -144));
	introHead->expBar->setPercentage(0);
	battleGameOverWindow->runAction(CCSequence::actionOneTwo(CCMoveTo::actionWithDuration(0.5f, position.CONTENT), CCCallFunc::actionWithTarget(battleGameOverWindow, callfunc_selector(BattleGameOver::appearMe)) ));

	//myScoreWindow->runAction(CCMoveTo::actionWithDuration(0.3f, position.MYSCORE)); //## 메달 주는지 확인해야함.
	home->runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.2f), CCMoveTo::actionWithDuration(0.3f, position.GAMEOVER_HOME)));
	restartGame->runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.3f), CCMoveTo::actionWithDuration(0.3f, position.RESTART)));
	
	title->runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.2f), CCMoveTo::actionWithDuration(0.3f, position.CUP)));
	titleBack->runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.2f), CCMoveTo::actionWithDuration(0.3f, position.CUP)));
	
	// 사라지는 버전.
//	titleBack->runAction(CCSequence::actions(CCFadeOut::actionWithDuration(0.5f), CCMoveTo::actionWithDuration(0.1f, ccp(-400, -400)),
//											 CCFadeIn::actionWithDuration(0.1f), 0));
//	title->runAction(CCSequence::actions(CCFadeOut::actionWithDuration(0.5f), CCMoveTo::actionWithDuration(0.1f, ccp(-400, -400)),
//										 CCFadeIn::actionWithDuration(0.1f), 0));
	
	
	
	catchingColleague = battleGameOverWindow;
}
void SWIntro::showFailedGameOver(const string& msg)
{
	KSModalAlert("", msg.c_str(), 1,
				 "RETRY", this, callfunc_selector(ThisClassType::GAMEOVERRETRY));
}
void SWIntro::showAlert(const string& msg)
{
	KSModalAlert("", msg.c_str(), 1,
				 "OK", NOPARAM);
}
void SWIntro::showRubyShop()
{
	if(rubyWindow == catchingColleague)
		return;
	if(checkBeforeMoving() == false)
		return;
	if(gameSelect == catchingColleague)
		gameSelect->hideGames();
	if(rubyWindow == 0)
	{
		rubyWindow = RubyShop::node(this);
		rubyWindow->setPosition(ccp(200, 500));
		addChild(rubyWindow, 3);
	}
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	rubyWindow->setPosition(ccp(143, -144));
	rubyWindow->runAction(CCMoveTo::actionWithDuration(0.5f, position.CONTENT));
	catchingColleague = rubyWindow;
	
}
void SWIntro::showGoldShop()
{
	if(goldWindow == catchingColleague)
		return;
	if(checkBeforeMoving() == false)
		return;
	if(gameSelect == catchingColleague)
		gameSelect->hideGames();
	if(goldWindow == 0)
	{
		goldWindow = GoldShop::node(this);
		goldWindow->setPosition(ccp(200, 500));
		addChild(goldWindow, 3);
	}
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	goldWindow->setPosition(ccp(143, -144));
	goldWindow->runAction(CCMoveTo::actionWithDuration(0.5f, position.CONTENT));
	catchingColleague = goldWindow;
}
void SWIntro::showHeartShop()
{
	if(heartWindow == catchingColleague)
		return;
	if(checkBeforeMoving() == false)
		return;
	if(gameSelect == catchingColleague)
		gameSelect->hideGames();
	if(heartWindow == 0)
	{
		heartWindow = HeartShop::node(this);
		heartWindow->setPosition(ccp(200, 500));
		addChild(heartWindow, 3);
	}
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	heartWindow->setPosition(ccp(143, -144));
	heartWindow->runAction(CCMoveTo::actionWithDuration(0.5f, position.CONTENT));
	catchingColleague = heartWindow;
}
void SWIntro::showArcheryShop()
{
	if(checkBeforeMoving() == false)
		return;
	
	if(archeryWindow == 0)
	{
		archeryWindow = ArcheryShop::node(this);
		archeryWindow->setPosition(ccp(200, 500));
		addChild(archeryWindow, 3);
	}
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	archeryWindow->setPosition(ccp(143, -144));
	archeryWindow->runAction(CCMoveTo::actionWithDuration(0.5f, position.CONTENT));
	catchingColleague = archeryWindow;
}

void SWIntro::showSkiShop()
{
	if(checkBeforeMoving() == false)
		return;
	
	if(skiWindow == 0)
	{
		skiWindow = SkiShop::node(this);
		skiWindow->setPosition(ccp(200, 500));
		addChild(skiWindow, 3);
	}
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	skiWindow->setPosition(ccp(143, -144));
	skiWindow->runAction(CCMoveTo::actionWithDuration(0.5f, position.CONTENT));
	catchingColleague = skiWindow;
}

void SWIntro::showHurdleShop()
{
	if(checkBeforeMoving() == false)
		return;
	if(hurdleWindow == 0)
	{
		hurdleWindow = HurdleShop::node(this);
		hurdleWindow->setPosition(ccp(200, 500));
		addChild(hurdleWindow, 3);
	}
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	hurdleWindow->setPosition(ccp(143, -144));
	hurdleWindow->runAction(CCMoveTo::actionWithDuration(0.5f, position.CONTENT));
	catchingColleague = hurdleWindow;
}
void SWIntro::showBasketShop()
{
	if(checkBeforeMoving() == false)
		return;
	if(basketWindow == 0)
	{
		basketWindow = BasketShop::node(this);
		basketWindow->setPosition(ccp(200, 500));
		addChild(basketWindow, 3);
	}
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	basketWindow->setPosition(ccp(143, -144));
	basketWindow->runAction(CCMoveTo::actionWithDuration(0.5f, position.CONTENT));
	catchingColleague = basketWindow;
}
void SWIntro::showClayShop()
{
	if(checkBeforeMoving() == false)
		return;
	if(clayWindow == 0)
	{
		clayWindow = ClayShop::node(this);
		clayWindow->setPosition(ccp(200, 500));
		addChild(clayWindow, 3);
	}
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	clayWindow->setPosition(ccp(143, -144));
	clayWindow->runAction(CCMoveTo::actionWithDuration(0.5f, position.CONTENT));
	catchingColleague = clayWindow;
}
void SWIntro::showGiftCode()
{
	if(checkBeforeMoving() == false)
		return;
	if(giftCodeWindow == 0)
	{
		giftCodeWindow = GiftCode::node(this);
		giftCodeWindow->setPosition(ccp(-400, 100));
		addChild(giftCodeWindow, 3);
	}
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	giftCodeWindow->setPosition(ccp(143, -144));
	giftCodeWindow->runAction(CCMoveTo::actionWithDuration(0.5f, position.CONTENT));
	catchingColleague = giftCodeWindow;
}
void SWIntro::showMoreApps()
{
	if(moreAppsWindow == catchingColleague)
		return;
	if(checkBeforeMoving() == false)
		return;
	
	if(moreAppsWindow != 0)
		moreAppsWindow->removeFromParentAndCleanup(true);
	
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	moreAppsWindow = MoreApps::node(this);
	moreAppsWindow->setPosition(ccp(-400, 100));
	addChild(moreAppsWindow, 3);

		
	moreAppsWindow->setPosition(ccp(143, -144));
	moreAppsWindow->runAction(CCMoveTo::actionWithDuration(0.5f, position.CONTENT));
	catchingColleague = moreAppsWindow;
}
void SWIntro::sendFacebookScore()
{
	int ac = playInfo->currentWeek == ACSaveData::getWeeklyMaxScore().first ? ACSaveData::getWeeklyMaxScore().second : 0;
	int ak = playInfo->currentWeek == AKSaveData::getWeeklyMaxScore().first ? AKSaveData::getWeeklyMaxScore().second : 0;
	int bs = playInfo->currentWeek == BS2SaveData::getWeeklyMaxScore().first ? BS2SaveData::getWeeklyMaxScore().second : 0;
	int hw = playInfo->currentWeek == HWSaveData::getWeeklyMaxScore().first ? HWSaveData::getWeeklyMaxScore().second : 0;
	
	if(FBConnector::get()->isUsed() && FBConnector::get()->isLogin())
	{
		FBConnector::get()->sendScore(ac + ak + bs + hw, this, fb_selector(SWIntro::finishFacebookSendScore));
		//			FBConnector::get()->getScores(facebookRank, fb_selector(FacebookRank::facebookGetScoresFinish));
	}
}

void SWIntro::showBattleShop()
{
	CCLog("battleshop");
	if(checkBeforeMoving() == false)
		return;
	if(battleShop == 0)
	{
		battleShop = BattleShop::node(this);
		battleShop->setPosition(ccp(200, 500));
		addChild(battleShop, 3);
	}
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	battleShop->setPosition(ccp(143, -144));
	battleShop->runAction(CCMoveTo::actionWithDuration(0.5f, position.CONTENT));
	catchingColleague = battleShop;
	
//	playInfo->createWorldcupOrder();
//	wcplay->initVars();
//	JsonBox::Object param;
//	param["type"] = "WORLDCUP";
//	startGame(catchingColleague, param);
}
void SWIntro::showJudgeOnGameOver(string result, int rank, int score, int max, int weekly)
{
	myScoreWindow->showJudgeOnGameOver(result, rank, score, max, weekly);
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	if(score >= 200000 && rand()%15 == 0 && NSDefault::getIsReview() == 0)
	{
		NSDefault::setIsReview(1);
		KSModalAlert("", ML::key("opennewmap").c_str(), 2,
					 "NO", NOPARAM,
					 "YES", this, callfunc_selector(ThisClassType::goReview));
	}
}
void SWIntro::showLevelUp()
{
	KSoundEngine::sharedEngine()->playSound("levelup.mp3");
	auto mator = SceneUtil::playAnimation("levelup.png", 0.2, 3, 6, 170, 170, 1);
	auto ac = CCSequence::actions(mator.first,CCCallFuncN::actionWithTarget(this, callfuncN_selector(ThisClassType::deleteSprite)), 0);
	mator.second->runAction(ac);
	mator.second->setPosition(ccp(380, 180));
	addChild(mator.second, 3);
}
void SWIntro::showExp2()
{
	auto mator = SceneUtil::playAnimation("expx2.png", 0.2, 3, 3, 52, 32, 6);
	auto ac = CCSequence::actions(mator.first,CCCallFuncN::actionWithTarget(this, callfuncN_selector(ThisClassType::deleteSprite)), 0);
	mator.second->setScale(1.5f);
	mator.second->runAction(ac);
	mator.second->setPosition(ccp(420, 270));
	addChild(mator.second, 3);
}
void SWIntro::refreshEXP()
{
	introHead->refreshEXP();
}
void SWIntro::showWorldcupMark(bool b)
{
	worldcupMark->setIsVisible(b);
}

void SWIntro::sendHeartTo()
{
	CCMenu* _menu = CCMenu::menuWithItems(0);
	_menu->setPosition(ccp(0, 0));
	addChild(_menu, INT_MAX);
	
	CCMenuItem* kakaoInvite = CCMenuItemImage::itemFromNormalImage("heartsend.png", "heartsend.png", this, menu_selector(ThisClassType::closeSendWindow));
	kakaoInvite->setPosition(ccp(240, 160));
	_menu->addChild(kakaoInvite, 2);
}
void SWIntro::closeSendWindow(CCObject* s)
{
	CCNode* e = (CCNode*)s;
	e->getParent()->removeFromParentAndCleanup(true);
}
void SWIntro::viewProfile()
{
	
}
void SWIntro::viewPostBox()
{
	if(checkBeforeMoving() == false)
		return;
	
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	postBox->setPosition(ccp(143, -144));
	postBox->runAction(CCMoveTo::actionWithDuration(0.5f, position.CONTENT));
	catchingColleague = postBox;
	postBox->loadRequest();
}
void SWIntro::showInviteFriendWinndow()
{
	if(checkBeforeMoving() == false)
		return;
	
	if(inviteEvent == 0)
	{
		inviteEvent = CCSprite::spriteWithFile("event.png");
		
		addChild(inviteEvent, 2);
	}
	
	inviteEvent->setPosition(ccp(position.EVENT.x + 300, position.EVENT.y));
	inviteEvent->runAction(CCMoveTo::actionWithDuration(0.3f, position.EVENT));
	title->runAction(CCMoveBy::actionWithDuration(0.3f, ccp(300, 0)));
	titleBack->runAction(CCMoveBy::actionWithDuration(0.3f, ccp(300, 0)));
	
	moreapps->runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.1f), CCMoveBy::actionWithDuration(0.3f, ccp(300, 0))));
	option->runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.1f), CCMoveBy::actionWithDuration(0.3f, ccp(300, 0))));
	startGameBtn->runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.2f), CCMoveBy::actionWithDuration(0.3f, ccp(300, 0))));
	
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	countryRank->setPosition(ccp(143, -144));
	countryRank->runAction(CCMoveTo::actionWithDuration(0.5f, position.CONTENT));
	catchingColleague = countryRank;
	
	
//	showInviteEvent();
}
void SWIntro::inviteFriend()
{
	CCMenu* _menu = CCMenu::menuWithItems(0);
	_menu->setPosition(ccp(0, 0));
	addChild(_menu, INT_MAX);
	
	CCMenuItem* kakaoInvite = CCMenuItemImage::itemFromNormalImage("invite.png", "invite.png", this, menu_selector(ThisClassType::closeSendWindow));
	kakaoInvite->setPosition(ccp(240, 160));
	_menu->addChild(kakaoInvite, 2);
	
}

void SWIntro::battleSelectWindow()
{
	CCMenu* _menu = CCMenu::menuWithItems(0);
	_menu->setPosition(ccp(0, 0));
	addChild(_menu, INT_MAX);
	
	CCMenuItem* item = CCMenuItemImage::itemFromNormalImage("match_select.png", "match_select.png", this, menu_selector(ThisClassType::closeBattleSelectWindow));
	item->setPosition(ccp(240, 160));
	_menu->addChild(item, 2);
	
}

void SWIntro::closeBattleSelectWindow(CCObject* s)
{
	CCNode* e = (CCNode*)s;
	e->getParent()->removeFromParentAndCleanup(true);
	CCNotificationCenter::sharedNotifCenter()->postNotification("selectedBattle");
}





