//
//  MoreApps.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 22..
//
//


#include "MoreApps.h"
#include "KS_Util.h"
#include "GraphDog.h"
#include "Bridge.h"
#include "KSBaseScene.h"
#include <sstream>
#include <unistd.h>
#include <queue>
#include "KSModalAlert.h"
#include "NSDefault.h"
#include "CCImage2.h"
#include "KSoundEngine.h"

using namespace std;



size_t MoreApps::WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    
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

bool MoreApps::init()
{
	CCLayerColor::initWithColor(ccc4f(0,0, 0, 0));
	this->setIsTouchEnabled(true);
	thiz = CCSprite::spriteWithFile("moreapps_back.png");
	addChild(thiz);
	
	moreAppsMenu = CCMenu::menuWithItems(0);
	moreAppsMenu->setPosition(ccp(0, 0));
	thiz->addChild(moreAppsMenu, 4);
	
	CCMenuItem* close = CCMenuItemImage::itemFromNormalImage("main_exit.png", "main_exit.png", this, menu_selector(ThisClassType::CLOSE));
	close->setPosition(ccp(234, 263));
	moreAppsMenu->addChild(close, 2);

	
	float loadingX = 132;
	float loadingY = 144;
	auto animator = SceneUtil::playAnimation("loading.png", 0.1, 9, 9, 56, 56, true);
	animator.second->runAction(animator.first);
	thiz->addChild(animator.second, INT_MAX);
	animator.second->setPosition(ccp(loadingX, loadingY));
	loading = animator.second;
	
	JsonBox::Object param;
	param["limit"] = 3;
	graphdog->command("getcpilist", &param, this, gd_selector(ThisClassType::finishGetCPIList));
	
//	for(int i=0; i<30; i++)
//	{
//		imgUrls.push_back("http://upload.wikimedia.org/wikipedia/en/thumb/f/f2/Edit-clear.svg/40px-Edit-clear.svg.png");
//		imgUrls.push_back("http://upload.wikimedia.org/wikipedia/en/thumb/f/f2/Edit-clear.svg/40px-Edit-clear.svg.png");
//	}
	//while(1)
	
	
	
	schedule(schedule_selector(MoreApps::imageDown));
	
	reward = CCMenuItemImage::itemFromNormalImage("moreapps_rewards.png", "moreapps_rewards.png",
														   this, menu_selector(ThisClassType::REWARD));
	reward->setIsVisible(false);
	moreAppsMenu->addChild(reward, 2);
	reward->setPosition(ccp(134, 36));
	
	CCLabelTTF* desc = CCLabelTTF::labelWithString(ML::key("rewardafterexe").c_str(), defaultFont, 11.f);
	desc->setColor(ccc3(0, 0, 0));
	desc->setPosition(ccp(107, 15));
	reward->addChild(desc);
	
	return true;
}
void MoreApps::REWARD(CCObject*)
{
	JsonBox::Object  param;
	param["limit"] = 10;
	param["category"] = "cpievent";
	param["autoremove"] = true;
	graphdog->command("getrequests", &param, this, gd_selector(ThisClassType::finishRequest));
}

void* MoreApps::t_function(void* _caller)
{
	MoreApps* caller = (MoreApps*)_caller;

	CURLcode retCode = curl_global_init(CURL_GLOBAL_ALL);
	if(retCode != CURLcode::CURLE_OK)
		return NULL;
	CURL *curl_handle = curl_easy_init();

	//while (1)
	int i = 0;
	for(auto iter = caller->imgUrls.begin(); iter != caller->imgUrls.end(); ++iter, i++)
	{
		string pURL = *iter;
		CCSize rSize = CCDirector::sharedDirector()->getWinSizeInPixels();
		float sc = rSize.width / CCDirector::sharedDirector()->getWinSize().width;
		float imgSize = 38;
		imgSize *= sc;
		pURL += KS_Util::stringWithFormat("=s%d-c", (int)imgSize);
		MemoryStruct chunk = {(char*)malloc(1), 0};
		
		curl_easy_setopt(curl_handle, CURLOPT_URL, pURL.c_str());
		curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, true);
		curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
		curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
		if(curl_easy_perform(curl_handle) != CURLcode::CURLE_OK)
			return NULL;
		caller->chunks.push(pair<MemoryStruct, int>(chunk, i));
		
		//CCScheduler::sharedScheduler()->scheduleSelector(schedule_selector(MoreApps::imageDown), caller, 0,false);
		//CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(FBConnector::imageDown), FBConnector::get(), 0, false, 0, 0);
	}
	caller->isComplete = true;
	curl_easy_cleanup(curl_handle);
	curl_global_cleanup();
	return NULL;
	
}
void MoreApps::imageDown(ccTime dt)
{
	//unschedule(schedule_selector(MoreApps::imageDown)); //in cocos2d-x 1.x
	

	while(!chunks.empty())
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		CCImage* img = new CCImage;
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		CCImage2* img = new CCImage2; //in cocos2d-x 1.x
#endif
		CCTexture2D* texture = new CCTexture2D();
		auto chunk_index = chunks.front();
		try {

			auto chunk_index = chunks.front();

			if(img->initWithImageData(chunk_index.first.memory, (long)chunk_index.first.size, CCImage::kFmtUnKnown) == false)
				throw "";
			
			if(texture->initWithImage(img) == false)
				throw "";
			CCSprite* sprite = CCSprite::spriteWithTexture(texture);
			
			sprite->setPosition(position[chunk_index.second].icon);
			thiz->addChild(sprite, 3);
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
	
	if(isComplete && loading)
	{
		loading->removeFromParentAndCleanup(true);
		loading = 0;
	}
}

void MoreApps::otherApp(CCObject* o)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	int code = (int)((CCMenuItemImage*)o)->getUserData();
	string store = codeToApp[code].store;
	
	string aid = codeToApp[code].aid;
	KS::KSLog("% %zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz", code, codeToApp[code].store);
	
	JsonBox::Object storeOk_param;
	storeOk_param["aid"] = aid;
	storeOk_param["callback"] = store;
	graphdog->command("addcpievent", &storeOk_param, this, gd_selector(ThisClassType::finishAddCpiEvent));
	
}
void MoreApps::STORE_OK()
{
	reward->setIsVisible(true);
//	string store = js["callback"].getString();
	::otherApp(storeLocation);
}
void MoreApps::finishAddCpiEvent(JsonBox::Object js)
{
	KS::KSLog("%", js);
	
	try
	{
		if(js["state"].getString() != "ok")
			throw ML::key("netfail");
		KSModalAlert("", ML::key("rewardafterexe").c_str(), 1,
					 "YES", this, callfunc_selector(ThisClassType::STORE_OK));
		storeLocation = js["callback"].getString();
	}
	catch(const string& msg)
	{
		KSModalAlert("", msg.c_str(), 1,
					 "OK", NOPARAM);
		
	}
	
}
void MoreApps::finishRequest(JsonBox::Object js)
{
	ostringstream oss;
	oss << js << endl;
	KS::KSLog("%", oss.str().c_str());
	
	try
	{
		if(js["state"].getString() != "ok")
			throw ML::key("netfail");

		JsonBox::Array jsList = js["list"].getArray();
		if(jsList.size() == 0)
			throw ML::key("noreward");
		int totalReward = 0;
 		for(auto iter = jsList.begin(); iter != jsList.end(); ++iter)
		{
			JsonBox::Object userdata = (*iter)["userdata"].getObject();
			totalReward += atoi( userdata["reward"].getString().c_str() );
		}
		
		KSModalAlert("", KS_Util::stringWithFormat("get %d golds.", totalReward).c_str(), 1,
					 "OK", NOPARAM);
		NSDefault::setGold(NSDefault::getGold() + totalReward);
		mediator->setGold(NSDefault::getGold());
		reward->setIsVisible(false);
	}
	catch(const string& msg)
	{
		KSModalAlert("", msg.c_str(), 1,
						 "OK", NOPARAM);
		
	}
}
void MoreApps::finishGetCPIList(JsonBox::Object js)
{
	ostringstream oss;
	oss << js << endl;
	KS::KSLog("%", oss.str().c_str());
//	if(
	try
	{
		if(js["state"].getString() != "ok")
			throw string("..");
		
		JsonBox::Array listObject = js["list"].getArray();
		if(js["reward"].getBoolean())// || )
		{
			reward->setIsVisible(true);
		}
		else
		{
			if(listObject.size() <= 0) // 보상도 없고 목록도 없으면 앱스토어 이동.
			{
				throw string("");
			}
		}
		int i = 0;
		for(auto app = listObject.begin(); app != listObject.end(); ++app, i++)
		{
			JsonBox::Object _app = (*app).getObject();
			CCMenuItem* b = CCMenuItemImage::itemFromNormalImage("moreapps_list.png", "moreapps_list.png",
																 this, menu_selector(ThisClassType::otherApp));
			b->setUserData((void*)i);
			codeToApp[i] = App(_app["store"].getString(), _app["id"].getString());
			
			b->setPosition(position[i].back);
			moreAppsMenu->addChild(b, 2);
			CCLabelTTF* t = CCLabelTTF::labelWithString(_app["title"].getString().c_str(), defaultFont, 20);
			CCLabelTTF* d = CCLabelTTF::labelWithString(_app["descript"].getString().c_str(), defaultFont, 12);
			string g = "+" + _app["reward"].getString();
			CCLabelBMFont* reward = CCLabelBMFont::labelWithString(g.c_str(), "moreapps.fnt");
			
			t->setColor(ccc3(32, 63, 76));
			d->setColor(ccc3(32, 63, 76));
			t->setAnchorPoint(ccp(0, 0.5f));
			d->setAnchorPoint(ccp(0, 0.5f));
			t->setPosition(position[i].title);
			d->setPosition(position[i].desc);
			reward->setPosition(position[i].reward);
			thiz->addChild(t, 5);
			thiz->addChild(d, 5);
			thiz->addChild(reward, 5);
			imgUrls.push_back(_app["iconimg"].getString());
		}
		
		pthread_t p_thread;
		pthread_create(&p_thread, NULL, t_function, this);
	}
	catch(const string& msg)
	{
		moreapps();
		loading->removeFromParentAndCleanup(true);
		loading = 0;
		//KS::KSLog(msg.c_str());
//		KSModalAlert("", msg.c_str(), 1,
//					 "다시 시도합니다.", this, callfunc_selector(ThisClassType::RETRY));
	}
	
}
void MoreApps::registerWithTouchDispatcher()
{
	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, false);
}
bool MoreApps::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	CCTouch* touch = pTouch;
	
	CCPoint location(ccp(0, 0));
	location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
	location = thiz->convertToNodeSpace(location);

	
	return true;
}