//
//  BS2Play.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 1. 23..
//
//

#ifndef __SportsWorldCup__BS2Play__
#define __SportsWorldCup__BS2Play__




#include "KSProtectVar.h"

#include <map>
#include <string>
using namespace std;


// 다른씬에서 쓰는 게임 관련 변수.
class BS2Play
{
public:
	KSProtectVar<int> gameScore;
	KSProtectVar<int> combo;
	map<std::string, bool> items;
	int backNumber;
public:
	static BS2Play* sharedObject()
	{
		static BS2Play* _ins = 0;
		if(_ins == 0)
		{
			_ins = new BS2Play();
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
	BS2Play() : combo(0), gameScore(0)
	{}
};

extern BS2Play* bs2play;
#endif /* defined(__SportsWorldCup__BS2Play__) */
