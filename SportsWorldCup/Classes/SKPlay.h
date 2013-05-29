//
//  ACPlay.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 1. 24..
//
//

#ifndef __SportsWorldCup__SKPlay__
#define __SportsWorldCup__SKPlay__

#include "KSProtectVar.h"
#include <map>
#include <string>
using namespace std;

// 다른씬에서 쓰는 게임 관련 변수.
class SKPlay
{
public:
	KSProtectVar<int> gameScore;
	KSProtectVar<int> combo;
	map<string, bool> items;
public:
	static SKPlay* sharedObject()
	{
		static SKPlay* _ins = 0;
		if(_ins == 0)
		{
			_ins = new SKPlay();
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
	SKPlay() : combo(0), gameScore(0)
	
	{}
};
extern SKPlay* skplay;

#endif /* defined(__SportsWorldCup__ACPlay__) */
