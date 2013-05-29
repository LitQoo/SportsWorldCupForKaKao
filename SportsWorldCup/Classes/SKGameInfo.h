#ifndef SK_GameInfo_h
#define SK_GameInfo_h

#include "GameSystem.h"
#include "cocos2d.h"

using namespace cocos2d;



struct SKGameInfo
{
	static CCPoint CHARACTER_POSITION;
	
	bool in10s; // 10초 안에 진입했는지.
	bool usedShield;
	int ateGoldCount;
	
	//	int comboInZeroMode;
	float remainTime;

	enum GAMEMODETYPE {NORMAL = 0, ZERO=2} gameMode;
	static int INIT_GAME_TIME;
	enum {NON_WORLDCUP_TIME = 40, WORLDCUP_TIME = 27}; // 40, 27
	static const float ZERO_DEC_VALUE;
	static const float INC_VALUE_IN_ZERO;
	static const float INC_VALUE_IN_NORMAL;
	static const float CRASH_DEC_VALUE_IN_NORMAL;
	static const float CRASH_DEC_VALUE_IN_ZERO;
	float lastChaTime;
	float timeAfterZero;
	float bonusTime;
	SKGameInfo();
	bool isGameOvering; // 타임오버 후 한번만 실행되게 하게끔 하는 변수
	void initBalanceElement();
};



#endif


