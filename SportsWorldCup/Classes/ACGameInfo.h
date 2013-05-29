//
//  GameInfo.h
//  ArcherWorldCupX
//
//  Created by apple on 12. 7. 7..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#ifndef ArcherWorldCupX_GameInfo_h
#define ArcherWorldCupX_GameInfo_h

#include "GameSystem.h"

struct ACGameInfo
{
	bool in10s;
	bool usedShield;
	static const float ZERO_DEC_VALUE;
	static const float INC_VALUE_IN_ZERO;
	static const float INC_VALUE_IN_NORMAL;
	static const float CRASH_DEC_VALUE_IN_NORMAL;
	static const float CRASH_DEC_VALUE_IN_ZERO;
	float remainTime;
	float bonusTime;
	int shotCounterAfterZero;
	int shotCounter;
	enum {MAX_WIND = 20, MIN_WIND=0};
	enum {NON_WORLDCUP_TIME = 48, WORLDCUP_TIME = 25}; //48 ,25
	static int INIT_GAME_TIME;
	int wind;
	int ateGoldCount;
	enum GAMEMODETYPE {NORMAL = 0, ZEROING=1, ZERO=2, OUTZEROING=3} gameMode;
	ACGameInfo()
	{
		in10s = false;
		usedShield = false;
		ateGoldCount = 0;
		if(playInfo->currentGame == "WORLDCUP" || playInfo->currentGame == "BATTLE")
		{
			ACGameInfo::INIT_GAME_TIME = ACGameInfo::WORLDCUP_TIME;
		}
		else
		{
			ACGameInfo::INIT_GAME_TIME = ACGameInfo::NON_WORLDCUP_TIME;
		}
		remainTime = INIT_GAME_TIME;
		shotCounterAfterZero = 0;
		shotCounter = 0;
		wind = 0.f;

		bonusTime = playInfo->selectedItems["time"] ? (float)INIT_GAME_TIME * 0.1f : 0.f;
	}
};


#endif


