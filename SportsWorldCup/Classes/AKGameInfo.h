//
//  AKGameInfo.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 2. 19..
//
//

#ifndef __SportsWorldCup__AKGameInfo__
#define __SportsWorldCup__AKGameInfo__


struct AKGameInfo
{
	bool in10s; // 10초 안에 진입했는지.
	bool usedShield;
	int ateGoldCount;
	
	float lastLeftShotTime;
	float lastRightShotTime;
//	int comboInZeroMode;
	bool isDisable;
	float disableTime;
	float gameTimer;
	float remainTime;
	float minTerm;
	float maxTerm;
	float lastCreateTime;
	float nextCreateTime;
	//

	float lastCreateTime2;
	float nextCreateTime2;
	//
	enum GAMEMODETYPE {NORMAL = 0, ZEROING=1, ZERO=2, OUTZEROING=3} gameMode;
	static int INIT_GAME_TIME;
	enum {NON_WORLDCUP_TIME = 40, WORLDCUP_TIME = 27}; // 40, 27
	static const float ZERO_DEC_VALUE;
	static const float INC_VALUE_IN_ZERO;
	static const float INC_VALUE_IN_NORMAL;
	static const float CRASH_DEC_VALUE_WHEN_NOTARGET_IN_NORMAL;
	static const float CRASH_DEC_VALUE_WHEN_WRONGTARGET_IN_NORMAL;
	static const float CRASH_DEC_VALUE_WHEN_NOTARGET_IN_ZERO;
	static const float CRASH_DEC_VALUE_WHEN_WRONGTARGET_IN_ZERO;
	static const float INIT_MINTERM;
	static const float INIT_MAXTERM;
	int zeroStep;
	float timeAfterZero;
	int beforeRunStep;
	float bonusTime;
	AKGameInfo();
	float bonusScale;
	bool isGameOvering; // 타임오버 후 한번만 실행되게 하게끔 하는 변수
	void initBalanceElement();
};

#endif /* defined(__SportsWorldCup__AKGameInfo__) */
