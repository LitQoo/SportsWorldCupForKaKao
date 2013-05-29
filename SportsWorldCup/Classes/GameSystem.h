#pragma once

//#include "AppDelegate.h"
#include "cocos2d.h"
#include "CommonGame.h"

#include "KSProtectVar.h"
#include "IntroHead.h"

#include <map>
#include <string>
#include <deque>
USING_NS_CC;

extern int TIMEOVERWIDTH;
extern int TIMEOVERHEIGHT;
extern float BASETIME;
class GameSystem
{
public:
	int __ateCoin;
	string currentGame;
	string prevGame;
	map<string, bool> selectedItems;
	int currentWeek;
	int whenRecvLeftTime;
	CommonGame* gamePtr;
	IntroHead* pIntroHead;
public:
	static GameSystem* sharedObject()
	{
		static GameSystem* _ins = 0;
		if(_ins == 0)
		{
			_ins = new GameSystem();
		}
		return _ins;
	}
	void initLvTable();
	pair<int, int> getLV_EXP(int exp);
	int expFromLv(int lv);
	static int getCurrentDay();
	static int getCurrentTime_s();
	enum {HEART_CHARGE_TIME = 10 * 60, DEFAULT_MAX_HEART = 5}; // X minutes
	enum {WORLDCUP_ENTRANCE_FEE = 2000};
	//
	
	void createWorldcupOrder();
	void initGameAndStart(string game);
	string getWorldcupGame();
	void popGame();
private:
	enum {MAX_LEVEL = 100};
	std::deque<std::string> worldcupOrder;
	int lvTable[MAX_LEVEL];

	GameSystem()
	{
		__ateCoin = 0;
		gamePtr = 0;
		pIntroHead = 0;
	}
};

extern GameSystem* playInfo;
// +(BOOL)isRetinaDisplay; // #



