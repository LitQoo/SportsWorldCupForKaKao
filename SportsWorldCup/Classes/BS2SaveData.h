//
//  BS2SaveData.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 1. 24..
//
//

#ifndef __SportsWorldCup__BS2SaveData__
#define __SportsWorldCup__BS2SaveData__

#include <string>
#include <sstream>
#include "SaveData.h"

using namespace std;
class BS2SaveData
{
public:
	static int getMaxScore()
	{
		int ii = atoi(saveData->getValue("BS2_MAXSCORE", "0").c_str());
		return ii;	
	}
	static void setMaxScore(int s)
	{
		ostringstream oss;oss << s;
		saveData->setKeyValue("BS2_MAXSCORE", oss.str());
	}
	static int getMaxCombo()
	{
		int ii = atoi(saveData->getValue("BS2_MAXCOMBO", "0").c_str());
		return ii;
	}
	static void setMaxCombo(int s)
	{
		ostringstream oss;oss << s;
		saveData->setKeyValue("BS2_MAXCOMBO", oss.str());
	}
	static pair<int, int> getWeeklyMaxScore() // week, score
	{
		int ii = atoi(saveData->getValue("BS_WMAXSCORE", "0").c_str());
		int w = atoi(saveData->getValue("BS_LASTWEEK", "0").c_str());
		return pair<int, int>(w, ii);
	}
	static void setWeeklyMaxScore(int w, int s)
	{
		{
			ostringstream oss;oss << s;
			saveData->setKeyValue("BS_WMAXSCORE", oss.str());
		}
		{
			ostringstream oss;oss << w;
			saveData->setKeyValue("BS_LASTWEEK", oss.str());
		}
	}
};
#endif /* defined(__SportsWorldCup__BS2SaveData__) */
