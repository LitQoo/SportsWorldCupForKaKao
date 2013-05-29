//
//  HWPlay.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 1. 24..
//
//

#ifndef __SportsWorldCup__HWPlay__
#define __SportsWorldCup__HWPlay__

#include "KSProtectVar.h"
#include <map>
#include <string>
using namespace std;

// 다른씬에서 쓰는 게임 관련 변수.
class HWPlay
{
public:
	KSProtectVar<int> gameScore;
	KSProtectVar<int> combo;
	int speedCombo;
	float gameTimer;
	map<string, bool> items;
public:
	static HWPlay* sharedObject()
	{
		static HWPlay* _ins = 0;
		if(_ins == 0)
		{
			_ins = new HWPlay();
		}
		return _ins;
	}
public:
	void initVars()
	{
		combo = 0;
		speedCombo = 0;
		gameScore = 0;
		gameTimer = 0.f;
	}
private:
	HWPlay() : combo(0), gameScore(0)
	
	{}
};
extern HWPlay* hwplay;

#endif /* defined(__SportsWorldCup__HWPlay__) */
