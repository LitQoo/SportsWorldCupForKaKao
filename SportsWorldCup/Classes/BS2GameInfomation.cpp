//
//  GameInfomation.cpp
//  BasketWorldCup2
//
//  Created by ksoo k on 13. 1. 14..
//
//

#include "BS2GameInfomation.h"
#include "GameSystem.h"
float BS2GameInformation::BACKBOARD_LEN = 70;
float BS2GameInformation::GROUND_LEN = 10;
float BS2GameInformation::RING_SIZE = 35;
float BS2GameInformation::POST_E = 0.7f;
float BS2GameInformation::POST_U = 0.1f;
const float BS2GameInformation::TIPIN_TIME = 0.8f;
const float BS2GameInformation::AFTER_JUDGE_TIME = 0.3f;

const float BS2GameInformation::ZERO_DEC_VALUE = -0.06f;//-0.06f;
const float BS2GameInformation::INC_VALUE_IN_ZERO = 7.f; // 13.f
const float BS2GameInformation::INC_VALUE_IN_NORMAL = 8.f; // 12.f
const float BS2GameInformation::NOGOAL_DEC_VALUE_IN_NORMAL = 0.f;
const float BS2GameInformation::NOGOAL_DEC_VALUE_IN_ZERO = -25.f;//-25.f;
int BS2GameInformation::INIT_GAME_TIME = 40;
void BS2GameInformation::setBalance()
{
	POST_E = 1.f;//MIN(0.8f + (playInfo->stage.getVar()-1) * 0.3f / 5.f, 1.f);
	remainTime = INIT_GAME_TIME;// - 4 * (playInfo->stage.getVar() - 1);
	movingPost = false;
}
