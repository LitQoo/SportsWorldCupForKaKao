//
//  AKSaveData.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 1. 24..
//
//

#ifndef __SportsWorldCup__AKSaveData__
#define __SportsWorldCup__AKSaveData__


#include <string>
#include <sstream>
#include "SaveData.h"

using namespace std;
class AKSaveData
{
public:
	static int getMaxScore()
	{
		int ii = atoi(saveData->getValue("AK_MAXSCORE", "0").c_str());
		return ii;
	}
	static void setMaxScore(int s)
	{
		ostringstream oss;oss << s;
		saveData->setKeyValue("AK_MAXSCORE", oss.str());
	}
	static int getMaxCombo()
	{
		int ii = atoi(saveData->getValue("AK_MAXCOMBO", "0").c_str());
		return ii;
	}
	static void setMaxCombo(int s)
	{
		ostringstream oss;oss << s;
		saveData->setKeyValue("AK_MAXCOMBO", oss.str());
	}
	static pair<int, int> getWeeklyMaxScore() // week, score
	{
		int ii = atoi(saveData->getValue("AK_WMAXSCORE", "0").c_str());
		int w = atoi(saveData->getValue("AK_LASTWEEK", "0").c_str());
		return pair<int, int>(w, ii);
	}
	static void setWeeklyMaxScore(int w, int s)
	{
		{
			ostringstream oss;oss << s;
			saveData->setKeyValue("AK_WMAXSCORE", oss.str());
		}
		{
			ostringstream oss;oss << w;
			saveData->setKeyValue("AK_LASTWEEK", oss.str());
		}
	}

};

#endif /* defined(__SportsWorldCup__AKSaveData__) */
