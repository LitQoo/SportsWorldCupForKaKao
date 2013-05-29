//
//  HWGameInformation.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 1. 31..
//
//

#include "HWGameInformation.h"


const float HWInnerVar::leapVelocity = 350;
const float HWInnerVar::hurdleG = -800.f;

const float HWInnerVar::ZERO_DEC_VALUE = -0.07f;
const float HWInnerVar::INC_VALUE_IN_ZERO = 4.0f; // 13.f
const float HWInnerVar::INC_VALUE_IN_NORMAL = 7.f; // 12.f
const float HWInnerVar::CRASH_DEC_VALUE_IN_NORMAL = 0.f;
const float HWInnerVar::CRASH_DEC_VALUE_IN_ZERO = -40.f;
int HWGameInformation::INIT_GAME_TIME = 42;