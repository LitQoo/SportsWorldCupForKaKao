//
//  UIMediator.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 5..
//
//

#ifndef __SportsWorldCup__UIMediator__
#define __SportsWorldCup__UIMediator__


class IntroHead;
class FacebookRank;
class FacebookPost;
class IntroMain;
class FriendInvite;
#include "cocos2d.h"
USING_NS_CC;
//#include "IntroHead.h"



enum INIT_MODE{STARTUP, GAMEOVER, WORLDCUP_GAMEOVER, BATTLE_GAMEOVER};

class UIMediator
{
public:
	virtual void showFacebookRank() = 0;
	virtual void showBackToFacebook() = 0;
	virtual void showFriendInvite() = 0;
	virtual void showFacebookPost() = 0;
	virtual void showRank() = 0;
	virtual void showRankWithMenu() = 0;
	virtual void showGameSelect() = 0;
	virtual void showGameOver() = 0;
	virtual void showRubyShop() = 0;
	virtual void showGoldShop() = 0;
	virtual void showHeartShop() = 0;
	virtual void showArcheryShop() = 0;
	virtual void showHurdleShop() = 0;
	virtual void showBasketShop() = 0;
	virtual void showClayShop() = 0;
	virtual void showSkiShop() = 0;
	virtual void showGiftCode() = 0;
	virtual void showMoreApps() = 0;
	virtual void showLegendaryRecord(string gameType) = 0;
	virtual void sendFacebookScore() = 0;
	virtual void showFailedGameOver(const string& msg) = 0;
	virtual void showAlert(const string& msg) = 0;
	virtual void showLevelUp() = 0;
	virtual void showExp2() = 0;
	virtual void refreshEXP() = 0;
	virtual void incLoadCount() = 0;
	virtual int getHeartSize() = 0;
	virtual void addHeartSprite() = 0;
	virtual void showWorldcupMark(bool b) = 0;
	virtual enum INIT_MODE getInitMode() = 0;
	virtual void setGold(int n) = 0;
	virtual void setRuby(int n) = 0;
	virtual void setFacebookLeftTime(int s) = 0;
	virtual void showJudgeOnGameOver(string result, int rank, int score, int max, int weekly) = 0;
	virtual int getCurrentTime_s() = 0;
	virtual ~UIMediator(){
	}
};
class UIColleague
{
public:
	virtual void setMediator(UIMediator* m) = 0;
	virtual ~UIColleague(){}
};
#endif /* defined(__SportsWorldCup__UIMediator__) */
