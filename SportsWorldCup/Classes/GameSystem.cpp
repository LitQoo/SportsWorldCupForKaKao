//
//  SharingClass.m
//  HurdleWorldCup
//
//  Created by ksoo k on 12. 4. 12..
//  Copyright (c) 2012≥‚ __MyCompanyName__. All rights reserved.
//

#include "GameSystem.h"
#include "NSDefault.h"

#include "FacebookRank.h"
#include "BS2Play.h"
#include "AKPlay.h"
#include "HWPlay.h"
#include "SKPlay.h"
#include "SKGameScene.h"
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
#include "GameSystem.h"
#include "LoadingScene.h"

#include <algorithm>
#include "WCPlay.h"
#include "WCSaveData.h"

int GameSystem::getCurrentDay()
{
	tm* tm1;
	//timeval tv;
	// time()
	time_t tt = time(0);
	// gettimeofday( &tv, NULL );
	//localtime()
	tm1 = localtime(&tt);
	return tm1->tm_mon + tm1->tm_mday;
}

int GameSystem::getCurrentTime_s()
{
	//tm* tm1;
	//timeval tv;
	// time()
	time_t tt = time(0);
	// gettimeofday( &tv, NULL );
	//localtime()
	//tm1 = localtime(&tt);
	return tt;
	//	return tm1->tm_mon + tm1->tm_mday;

}

void GameSystem::initLvTable()
{
	lvTable[0] = 0;
	for(int i=1; i<MAX_LEVEL; i++)
	{
		lvTable[i] = 5 + (5 * (i-1)); // 10 은 10만점. 1천만점이 한계.
	}
}
int GameSystem::expFromLv(int lv)
{
	return lvTable[MIN(lv, MAX_LEVEL - 1)];
}
pair<int, int> GameSystem::getLV_EXP(int exp)
{
	for(int i=0; i<MAX_LEVEL; i++)
	{
		if(exp < lvTable[i])
		{
			return pair<int, int>(MAX_LEVEL, exp); // (i, exp); //#
		}
		else
		{
			exp -= lvTable[i];
		}
	}
	return pair<int, int>(MAX_LEVEL, 0); //##
}

void GameSystem::popGame()
{
	if(worldcupOrder.empty() == false)
	{
		prevGame = worldcupOrder.front();
		worldcupOrder.pop_front();
	}
}
string GameSystem::getWorldcupGame()
{
	if(worldcupOrder.empty())
	{
		return "";
	}
	else
	{
		string ret = worldcupOrder.front();		
		return ret;
	}
}
void GameSystem::createWorldcupOrder()
{
	worldcupOrder.clear();
	worldcupOrder.push_back("AC");
	worldcupOrder.push_back("AK");
	worldcupOrder.push_back("BS");
	worldcupOrder.push_back("HW");
//	worldcupOrder.push_back("SK");3

	for(int i=0; i<rand()%worldcupOrder.size(); i++)
	{
		std::random_shuffle(worldcupOrder.begin(), worldcupOrder.end(), [](int n){
			return rand()%n;
		} );
	}
}

void GameSystem::initGameAndStart(string game)
{
	wcplay->gameScore = 0;
	if(game == "AC")
	{
		acplay->initVars();
		ReplaceScene(ACGameScene);
	}
	else if(game == "AK")
	{
		akplay->initVars();
		ReplaceScene(AngryGameScene);
	}
	else if(game == "BS")
	{
		bs2play->initVars();
		ReplaceScene(BasketGame);
	}
	else if(game == "HW")
	{
		hwplay->initVars();
		ReplaceScene(HWGameDirector);
	}
	else if(game == "SK")
	{
		skplay->initVars();
		ReplaceScene(SKGameScene);

	}

}
GameSystem* playInfo = GameSystem::sharedObject();


int TIMEOVERWIDTH = 250;
int TIMEOVERHEIGHT = 105;
float BASETIME = 60.f;