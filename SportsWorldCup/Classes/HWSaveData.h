//
//  HWSaveData.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 1. 24..
//
//

#ifndef __SportsWorldCup__HWSaveData__
#define __SportsWorldCup__HWSaveData__


#include <string>
#include <sstream>
#include "SaveData.h"

using namespace std;
class HWSaveData
{
public:
	static int getMaxScore()
	{
		int ii = atoi(saveData->getValue("HW_MAXSCORE", "0").c_str());
		return ii;
	}
	static void setMaxScore(int s)
	{
		ostringstream oss;oss << s;
		saveData->setKeyValue("HW_MAXSCORE", oss.str());
	}
	static int getMaxCombo()
	{
		int ii = atoi(saveData->getValue("HW_MAXCOMBO", "0").c_str());
		return ii;
	}
	static void setMaxCombo(int s)
	{
		ostringstream oss;oss << s;
		saveData->setKeyValue("HW_MAXCOMBO", oss.str());
	}
	static pair<int, int> getWeeklyMaxScore() // score, week
	{
		int ii = atoi(saveData->getValue("HW_WMAXSCORE", "0").c_str());
		int w = atoi(saveData->getValue("HW_LASTWEEK", "0").c_str());
		return pair<int, int>(w, ii);
	}
	static void setWeeklyMaxScore(int w, int s)
	{
		{
			ostringstream oss;oss << s;
			saveData->setKeyValue("HW_WMAXSCORE", oss.str());
		}
		{
			ostringstream oss;oss << w;
			saveData->setKeyValue("HW_LASTWEEK", oss.str());
		}
	}
};

#endif /* defined(__SportsWorldCup__HWSaveData__) */
