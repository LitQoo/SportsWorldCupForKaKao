//
//  MoreApps.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 22..
//
//

#ifndef __SportsWorldCup__MoreApps__
#define __SportsWorldCup__MoreApps__

#include "cocos2d.h"
#include "JsonBox.h"
#include "UIMediator.h"
#include <vector>
#include <string>
#include <queue>
#include "KSoundEngine.h"
USING_NS_CC;

struct App
{
	string store;
	string aid;
	
	App(const string& st, const string& ai)
	{
		aid = ai;
		store = st;
	}
	App(){}
};

class MoreApps : public CCLayerColor, public UIColleague
{
private:
	string storeLocation;
	CCMenu* moreAppsMenu;
	CCSprite* loading;
	map<int, App> codeToApp;
	struct _position
	{
		CCPoint back;
		CCPoint icon;
		CCPoint title;
		CCPoint desc;
		CCPoint reward;
	}position[4];
	struct MemoryStruct {
		char *memory;
		size_t size;
	};
	CCSprite* thiz;
	UIMediator* mediator;
	CCMenuItem* reward;
	virtual void registerWithTouchDispatcher();
	virtual bool onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen)
	{
		/*if (_calcCharCount(sender->getString()) > 5)
		 return true;*/
		return false;
	}
	void finishAddCpiEvent(JsonBox::Object js);
	void finishGetCPIList(JsonBox::Object js);
	void finishRequest(JsonBox::Object js);
	static void* t_function(void*);
	static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
	void imageDown(ccTime dt);
	void otherApp(CCObject* o);
public:
	bool isComplete;
	std::vector<std::string> imgUrls;
	std::queue<pair<MemoryStruct, int>> chunks;
	void CLOSE(CCObject*)
	{
		KSoundEngine::sharedEngine()->playSound("select.mp3");
		mediator->showRankWithMenu();
	}
	void STORE_OK();
	void REWARD(CCObject*);
	MoreApps()
	{
		isComplete = false;
		for(int i=0; i<4; i++)
		{
			int _diff = 56;
			position[i].back = ccp(132 + 3, 210 - _diff * i);
			position[i].title = ccp(59 + 3, 219 - _diff * i);
			position[i].desc = ccp(59 + 3, 197 - _diff * i);
			position[i].icon = ccp(32 + 3, 212 - _diff * i);
			position[i].reward = ccp(228, 204 - _diff * i);
		}
	}
	virtual ~MoreApps()
	{
		CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
	}
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init();
//	virtual void onEnterTransitionDidFinish();
//	virtual void onExit();
	static MoreApps* node(UIMediator *m)
	{
		MoreApps *pRet = new MoreApps();
		pRet->setMediator(m);
		if (pRet && pRet->init())
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			delete pRet;
			pRet = NULL;
			return NULL;
		}
	}
	void setMediator(UIMediator* m)
	{
		mediator = m;
	}	
};

#endif /* defined(__SportsWorldCup__MoreApps__) */
