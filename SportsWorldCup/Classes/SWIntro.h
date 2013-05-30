//
//  SWIntro.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 1. 23..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//

#ifndef SportsWorldCup_SWIntro_cpp
#define SportsWorldCup_SWIntro_cpp

#include "BattleGameOver.h"
#include "BattleShop.h"
#include "MoreApps.h"
#include "GiftCode.h"
#include "HWPlayer.h"
#include "HWFromTo.h"
#include "IntroHead.h"
#include "IntroMain.h"
#include "JsonBox.h"
#include "UIMediator.h"
#include "FacebookRank.h"
#include "FriendInvite.h"
#include "FacebookPost.h"
#include "Option.h"
#include "DisableTouch.h"
#include "MyMedal.h"
#include "GameSelect.h"
#include "CCMenuItemImageDown.h"
#include "GameOver.h"
#include "MyScore.h"
#include "RubyShop.h"
#include "GoldShop.h"
#include "HeartShop.h"
#include "ArcheryShop.h"
#include "ClayShop.h"
#include "BasketShop.h"
#include "HurdleShop.h"
#include "SkiShop.h"
#include "KSModalAlert.h"
#include "ACSaveData.h"
#include "AKSaveData.h"
#include "BS2SaveData.h"
#include "HWSaveData.h"
#include "Legendary.h"
#include "cocos2d.h"

#include <sstream>



struct _PositionTag
{
	CCPoint FRIENDINVITE_FRAME;
	CCPoint FACEBOOK_FRAME;
	CCPoint MEDAL_FRAME;
	CCPoint GOLD_NUMBER;
	CCPoint SILVER_NUMBER;
	CCPoint BRONZE_NUMBER ;
	CCPoint T10_NUMBER ;
	CCPoint MOREAPPS ;
	CCPoint OPTION ;
	CCPoint BASKETGAME;
	CCPoint ARCHERYGAME;
	CCPoint HURDLEGAME;
	CCPoint CLAYGAME;
	CCPoint FIRSTFLAG ;
	CCPoint SECONDFLAG;
	CCPoint THIRDFLAG ;
	
	CCPoint FIRSTPT ;
	CCPoint FIRSTUSER ;
	
	CCPoint SECONDPT ;
	CCPoint SECONDUSER;
	
	CCPoint THIRDPT;
	CCPoint THIRDUSER ;
	
	CCPoint MAXSCORE;
	CCPoint WEEKLYMAX;
	CCPoint START ;
	CCPoint TOLEFT ;
	
	CCPoint HOME;
	CCPoint GAMEOVER_HOME;
	CCPoint RESTART;
	CCPoint CUP;
	CCPoint MYSCORE;
	
	CCPoint EVENT;
	CCPoint CONTENT;
	CCPoint INTROHEAD;
	_PositionTag()
	{
		INTROHEAD = ccp(0, 288);
		CONTENT = ccp(143, 144);
		EVENT = ccp(380, 144);
		FRIENDINVITE_FRAME = ccp(587, 145);
		FACEBOOK_FRAME = ccp(241, 145);
		MEDAL_FRAME = ccp(378, 213);
		GOLD_NUMBER = ccp(52, 202);
		SILVER_NUMBER = ccp(52, 145);
		BRONZE_NUMBER = ccp(52, 87);
		T10_NUMBER = ccp(52, 29);
		MOREAPPS = ccp(330, 107);
		OPTION = ccp(428, 107);
		BASKETGAME = ccp(436, 245);
		ARCHERYGAME = ccp(436, 179);
		HURDLEGAME = ccp(436, 113);
		CLAYGAME = ccp(436, 47);
		FIRSTFLAG = ccp(529, 229);
		SECONDFLAG = ccp(529, 186);
		THIRDFLAG = ccp(529, 143);
		
		FIRSTPT = ccp(610, 233);
		FIRSTUSER = ccp(595, 218);
		
		SECONDPT = ccp(610, 191);
		SECONDUSER = ccp(595, 175);
		
		THIRDPT = ccp(610, 148);
		THIRDUSER = ccp(595, 133);
		
		MAXSCORE = ccp(552, 80);
		WEEKLYMAX = ccp(552, 33);
		START = ccp(378, 46);
		TOLEFT = ccp(372, 153);
		
		HOME = ccp(378, 112);
		CUP = ccp(379, 206);
		MYSCORE = ccp(379, 194);
		
		GAMEOVER_HOME = ccp(378, 88);
		RESTART = ccp(378, 32);
	}
};
USING_NS_CC;
class SWIntro : public CCLayer, public CCTextFieldDelegate, public UIMediator
{
public:
	enum INIT_MODE initMode;
	SWIntro();
	virtual ~SWIntro();
	virtual bool init();
	void afterInit();
	virtual void onEnterTransitionDidFinish();
	virtual void onEnter();
	virtual void onExit();
	static CCScene* scene(enum INIT_MODE e) {
		CCScene* scene = CCScene::node();
		SWIntro * layer = SWIntro::node(e);
		scene->addChild(layer);
		return scene;
	}
	static SWIntro* node(enum INIT_MODE e)
	{
		SWIntro *pRet = new SWIntro();
		pRet->initMode = e;
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
	void finishGetWeek(JsonBox::Object js);
	
	virtual void showFacebookRank();
	virtual void showBackToFacebook();
	virtual void showFriendInvite();
	virtual void showFacebookPost();
	virtual void showOption();
	virtual void showGameSelect();
	virtual void showLegendaryRecord(string gameType);
	virtual void showGameOver();
	virtual void showBattleGameOver();
	virtual void showFailedGameOver(const string& msg);
	virtual void showAlert(const string& msg);
	virtual void showRubyShop();
	virtual void showGoldShop();
	virtual void showHeartShop();
	virtual void showArcheryShop();
	virtual void showHurdleShop();
	virtual void showBasketShop();
	virtual void showClayShop();
	virtual void showSkiShop();
	virtual void showGiftCode();
	virtual void showMoreApps();
	virtual void sendFacebookScore();
	void showBattleShop();
	virtual void showJudgeOnGameOver(string result, int rank, int score, int max, int weekly);
	virtual void showLevelUp();
	virtual void showExp2();
	virtual void refreshEXP();
	virtual void showWorldcupMark(bool);
	virtual void incLoadCount();
	virtual int getHeartSize(){
		return introHead->getHeartSize();
	}
	virtual void addHeartSprite(){
		introHead->addHeartSprite();
	}
	virtual void setGold(int n)
	{
		introHead->setGold(n);
	}
	virtual void setRuby(int n)
	{
		introHead->setRuby(n);
	}
	virtual void setFacebookLeftTime(int s)
	{
		facebookRank->leftTime_s = s;
	}
	virtual enum INIT_MODE getInitMode(){return initMode;}
	virtual int getCurrentTime_s()
	{
		return introHead->timestamp + (playInfo->getCurrentTime_s() - introHead->oldLocalTime);
	}
	void finishFacebookSendScore(JsonBox::Value v);
private:
	bool checkBeforeMoving()
	{
		if(optionWindow->getIsValidName() == false)
		{
			optionWindow->alertInvalidName();
			return false;
		}
		if(catchingColleague)
		{
			DisableTouch* cl = DisableTouch::node();
			addChild(cl);
			catchingColleague->runAction(CCSequence::actions(CCMoveBy::actionWithDuration(0.5f, ccp(0, 287)), CCMoveBy::actionWithDuration(0.1f, ccp(0, 100)),
															 KSFunc(cl->removeFromParentAndCleanup(true);),0 ));
		}
		
		return true;
	}
	void goReview();
	void deleteSprite(CCNode* _n)
	{
		_n->getParent()->removeChild(_n, true);
	}
	_PositionTag position;
	CCMenuItemImageDown* startGameBtn, *moreapps, *option, *home, *restartGame;
	CCSprite* gameOn;
	CCSprite* loading;
	CCSprite* titleBack;
	CCSprite* title;
	CCSprite* worldcupMark;
	CCSprite* inviteEvent;
	int loadCount;
	bool bLoaded;
	CCNode* catchingColleague;
	IntroHead* introHead;
	FacebookRank* facebookRank;
	FriendInvite* friendInvite;
	FacebookPost* postBox;
	Option* optionWindow;
	MyMedal* medalWindow;
	GameSelect* gameSelect;
	GameOver* gameOverWindow;
	BattleGameOver* battleGameOverWindow;
	GiftCode* giftCodeWindow;
	MoreApps* moreAppsWindow;
	MyScore* myScoreWindow;
	RubyShop* rubyWindow;
	GoldShop* goldWindow;
	HeartShop* heartWindow;
	ArcheryShop* archeryWindow;
	BasketShop* basketWindow;
	HurdleShop* hurdleWindow;
	BattleShop* battleShop;
	ClayShop* clayWindow;
	SkiShop* skiWindow;
	Legendary* legendaryWindow;
	string houseAdUrl;
	string storeLocation;
	string aid;
	bool isComplete;
	std::queue<pair<MemoryStruct, string>> chunks;
	void START(CCObject*);
	void UPDATE_OK();
	void startGame(CCNode* colleague, JsonBox::Object param);
	void RESTART(CCObject*);
	void HOME(CCObject*);
	void MOREAPPS(CCObject*);
	void OPTION(CCObject*);
	
	
	void RETRY();
	void GAMEOVERRETRY();
	void finishInfo(JsonBox::Object js);
	void finishStartScores(JsonBox::Object js);
	void finishGetHouseAd(JsonBox::Object js);
	void finishGetUpdate(JsonBox::Object js);
	void getScores(JsonBox::Object js);
	void showRank();
	void showRankWithMenu();
	static void* t_function(void* _caller);
	static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
	void imageDown(ccTime dt);
	
	////////////////////// KAKAO
public:
	void sendHeartTo();
	void closeSendWindow(CCObject*);
	void viewProfile();
	void viewPostBox();
	void showInviteFriendWinndow();
	void inviteFriend();
	void battleSelectWindow();
	void closeBattleSelectWindow(CCObject*);
};

#endif




