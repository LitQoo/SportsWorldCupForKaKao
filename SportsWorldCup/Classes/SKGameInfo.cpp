//
//  AKGameInfo.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 2. 19..
//
//

#include "SKGameInfo.h"
#include "SKGameScene.h"
#include "KS_Util.h"

const float SKGameInfo::ZERO_DEC_VALUE = -0.13f;
const float SKGameInfo::INC_VALUE_IN_ZERO = 12.f; // 13.f
const float SKGameInfo::INC_VALUE_IN_NORMAL = 14.f; // 12.f

const float SKGameInfo::CRASH_DEC_VALUE_IN_NORMAL = 0.f;
const float SKGameInfo::CRASH_DEC_VALUE_IN_ZERO = -30.f;


int SKGameInfo::INIT_GAME_TIME = 40; // 40.f
#include "GameSystem.h"
SKGameInfo::SKGameInfo()
{
	in10s = false;
	usedShield = false;
	if(playInfo->currentGame == "WORLDCUP" || playInfo->currentGame == "BATTLE")
	{
		SKGameInfo::INIT_GAME_TIME = SKGameInfo::WORLDCUP_TIME;
	}
	else
	{
		SKGameInfo::INIT_GAME_TIME = SKGameInfo::NON_WORLDCUP_TIME;
	}
	{
		if(playInfo->selectedItems["item2"])
		{
			// 아이템 처리.
		}
	}
	
	gameMode = SKGameInfo::NORMAL;
	timeAfterZero = 0.f;
	remainTime = INIT_GAME_TIME;
	bonusTime = 0.f;
	
	{
		if(playInfo->selectedItems["time"])
			bonusTime = (float)INIT_GAME_TIME * 0.1f;
	}
	

	ateGoldCount = 0;
	isGameOvering = false;
}

void SKGameInfo::initBalanceElement()
{
	
}