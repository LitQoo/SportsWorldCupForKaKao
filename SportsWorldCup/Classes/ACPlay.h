//
//  ACPlay.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 1. 24..
//
//

#ifndef __SportsWorldCup__ACPlay__
#define __SportsWorldCup__ACPlay__

#include "KSProtectVar.h"
#include <map>
#include <string>
using namespace std;

// 다른씬에서 쓰는 게임 관련 변수.
class ACPlay
{
public:
	KSProtectVar<int> gameScore;
	KSProtectVar<int> combo;
	map<string, bool> items;
public:
	static ACPlay* sharedObject()
	{
		static ACPlay* _ins = 0;
		if(_ins == 0)
		{
			_ins = new ACPlay();
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
	ACPlay() : combo(0), gameScore(0)
	
	{}
};
extern ACPlay* acplay;

#endif /* defined(__SportsWorldCup__ACPlay__) */
