//
//  AKPlay.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 1. 24..
//
//

#ifndef __SportsWorldCup__AKPlay__
#define __SportsWorldCup__AKPlay__

#include "KSProtectVar.h"
#include <map>
#include <string>
using namespace std;

// 다른씬에서 쓰는 게임 관련 변수.
class AKPlay
{
public:
	KSProtectVar<int> gameScore;
	KSProtectVar<int> combo;
	map<string, bool> items;
public:
	static AKPlay* sharedObject()
	{
		static AKPlay* _ins = 0;
		if(_ins == 0)
		{
			_ins = new AKPlay();
		}
		return _ins;
	}
public:
	void initVars()
	{
		combo = 0;
		gameScore = 0;
	}
private:
	AKPlay() : combo(0), gameScore(0)
	
	{}
};

extern AKPlay* akplay;
#endif /* defined(__SportsWorldCup__AKPlay__) */
