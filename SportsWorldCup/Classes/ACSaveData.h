//
//  ACSaveData.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 1. 24..
//
//

#ifndef __SportsWorldCup__ACSaveData__
#define __SportsWorldCup__ACSaveData__


#include <string>
#include <sstream>
#include "SaveData.h"

using namespace std;
class ACSaveData
{
public:
	
	static int getMaxScore()
	{
		int ii = atoi(saveData->getValue("AC_MAXSCORE", "0").c_str());
		return ii;
	}
	static void setMaxScore(int s)
	{
		ostringstream oss;oss << s;
		saveData->setKeyValue("AC_MAXSCORE", oss.str());
	}
	static int getMaxCombo()
	{
		int ii = atoi(saveData->getValue("AC_MAXCOMBO", "0").c_str());
		return ii;
	}
	static void setMaxCombo(int s)
	{
		ostringstream oss;oss << s;
		saveData->setKeyValue("AC_MAXCOMBO", oss.str());
	}
	static pair<int, int> getWeeklyMaxScore() // score, week
	{
		int ii = atoi(saveData->getValue("AC_WMAXSCORE", "0").c_str());
		int w = atoi(saveData->getValue("AC_LASTWEEK", "0").c_str());
		return pair<int, int>(w, ii);
	}
	static void setWeeklyMaxScore(int w, int s)
	{
		{
			ostringstream oss;oss << s;
			saveData->setKeyValue("AC_WMAXSCORE", oss.str());
		}
		{
			ostringstream oss;oss << w;
			saveData->setKeyValue("AC_LASTWEEK", oss.str());
		}
	}
};

#endif /* defined(__SportsWorldCup__ACSaveData__) */
