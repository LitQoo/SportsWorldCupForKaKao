//
//  WCSaveData.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 4. 4..
//
//

#ifndef __SportsWorldCup__WCSaveData__
#define __SportsWorldCup__WCSaveData__

#include <string>
#include <sstream>
#include "SaveData.h"

using namespace std;
class WCSaveData
{
public:
	static int getMaxScore()
	{
		int ii = atoi(saveData->getValue("WC_MAXSCORE", "0").c_str());
		return ii;
	}
	static void setMaxScore(int s)
	{
		ostringstream oss;oss << s;
		saveData->setKeyValue("WC_MAXSCORE", oss.str());
	}
	static int getMaxCombo()
	{
		int ii = atoi(saveData->getValue("WC_MAXCOMBO", "0").c_str());
		return ii;
	}
	static void setMaxCombo(int s)
	{
		ostringstream oss;oss << s;
		saveData->setKeyValue("WC_MAXCOMBO", oss.str());
	}
	static pair<int, int> getWeeklyMaxScore() // score, week
	{
		int ii = atoi(saveData->getValue("WC_WMAXSCORE", "0").c_str());
		int w = atoi(saveData->getValue("WC_LASTWEEK", "0").c_str());
		return pair<int, int>(w, ii);
	}
	static void setWeeklyMaxScore(int w, int s)
	{
		{
			ostringstream oss;oss << s;
			saveData->setKeyValue("WC_WMAXSCORE", oss.str());
		}
		{
			ostringstream oss;oss << w;
			saveData->setKeyValue("WC_LASTWEEK", oss.str());
		}
	}
};

#endif /* defined(__SportsWorldCup__WCSaveData__) */
