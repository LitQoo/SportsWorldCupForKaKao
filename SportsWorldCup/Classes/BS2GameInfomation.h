//
//  GameInfomation.h
//  BasketWorldCup2
//
//  Created by ksoo k on 13. 1. 14..
//
//

#ifndef __BasketWorldCup2__GameInfomation__
#define __BasketWorldCup2__GameInfomation__

#include "chipmunk.h"
#include "BS2Graphics.h"
#include "KSProtectVar.h"
#include "GameSystem.h"

enum ItemType { DOUBLE_SHOT, MOVE_BALL, GUIDE_LINE, TIME_PLUS, RANDOM, NONE, LOT};

class GameScreen;

// 게임에서만 쓰이는 변수.  다른 씬에서는 안씀
struct BS2GameInformation
{
	bool in10s;
	GameScreen* gs;
	bool usedShield; // 게임내에서 실드아이템을 사용했는지...
	int ateGoldCount; // 먹은 동전 수. 표시를 위해 존재함.
	int throwCounter; // 던진 횟수
	int seqFailCounter; // 연속으로 실패한 횟수. 성공하면 초기화됨.
	bool onceTipIn; // 처음이나 제로 나갈 때 팁인 아이템을 썼다면 true 되고, 팁인 쓰면 false 셋.
	ItemType myItem; // 현재 게임내에서 선택된 아이템을 저장함.
	bool isTimeOver; // 남은 시간이 0초이하일 때 딱 한번만 뭔가를 실행하기 위해
	int postDirection; // 골대를 이동 시키는 방향
	bool movingPost; // 골대를 이동시킬지 여부
	int postSpeed; // 골대 이동 스피드
	cpVect firstPosition; // 무브아이템을 눌렀을 당시에 있던 공의 좌표
	float movingTime; // 무브아이템을 썼을 때 공이 움직이는 시간
	bool isGuided; // 가이드 아이템을 썼을 때 트루가 됨
	float remainTime; // 남은 시간
	float bonusTime; // 보너스 타임
	int coinStep; // 코인 단계
	CCRect coinsArea[3]; // 단계별 영역

	int goalCount; // 몇번 넣었는지.
	enum GAMEMODETYPE {NORMAL = 0, ZEROING=1, ZERO=2, OUTZEROING=3} gameMode;
	static int INIT_GAME_TIME;
	enum {NON_WORLDCUP_TIME = 43, WORLDCUP_TIME = 24}; //43, 24
	BS2GameInformation(GameScreen* g)
	{
		in10s = false;
		usedShield = false;
		bonusTime = 0.f;
		if(playInfo->currentGame == "WORLDCUP" || playInfo->currentGame == "BATTLE") 
		{
			BS2GameInformation::INIT_GAME_TIME = BS2GameInformation::WORLDCUP_TIME;
		}
		else
		{
			BS2GameInformation::INIT_GAME_TIME = BS2GameInformation::NON_WORLDCUP_TIME;
		}
		
		if(playInfo->selectedItems["time"])
			bonusTime = (float)INIT_GAME_TIME * 0.1f;
		
		seqFailCounter = 0;
		ateGoldCount = 0;
		onceTipIn = false;
		postSpeed = 0;
		gs = g;
		goalCount = 0;
		throwCounter = 0;
		isTimeOver = false;
		postDirection = -1;
		//postDirection = cpv(0, -0.15f);
		isGuided = false;
		myItem = NONE;
		gameMode = GAMEMODETYPE::NORMAL;
		coinStep = 0;
		coinsArea[0].size.width = 130; // 130
		coinsArea[0].origin.x = 120;
		coinsArea[1].size.width = 130; // 130
		coinsArea[1].origin.x = 120;
		coinsArea[2].size.width = 130; // 130
		coinsArea[2].origin.x = 120;
		coinsArea[0].size.height = coinsArea[1].size.height = 31*3 - 35;
		coinsArea[2].size.height = 31 * 8 - 35;
		
		coinsArea[0].origin.y = GROUND_HEIGHT + 200 -31*3 + 40 ;
		coinsArea[1].origin.y = coinsArea[0].origin.y + 31*3;
		coinsArea[2].origin.y = coinsArea[1].origin.y + 31*3;
		

		{
			if(playInfo->selectedItems["item2"])
			{
				isGuided = true;
			}
		}
	}
	void setBalance();
	static float BACKBOARD_LEN;
	static float GROUND_LEN;
	static float RING_SIZE;
	static float POST_E;
	static float POST_U;
	static const float TIPIN_TIME;
	static const float AFTER_JUDGE_TIME;
	static const float ZERO_DEC_VALUE;
	static const float INC_VALUE_IN_ZERO;
	static const float INC_VALUE_IN_NORMAL;
	static const float NOGOAL_DEC_VALUE_IN_NORMAL;
	static const float NOGOAL_DEC_VALUE_IN_ZERO;
	enum {GRAVITY = -800};
	enum {MAX_BALL_NUM = 10};
	enum {COIN_UPBOUND = Graphics::SCREEN_HEIGHT, COIN_DOWNBOUND = 40, ITEM_CREATE_COOLTIME = 3};
	static const int INITIAL_V_LIMIT = 1200;
	//static const int THROW_NUM_LIMIT = 10;
	
	enum {CRITERIA_ZEROAREA = 5};
	static const int GROUND_HEIGHT = 41; //41

};


#endif /* defined(__BasketWorldCup2__GameInfomation__) */
