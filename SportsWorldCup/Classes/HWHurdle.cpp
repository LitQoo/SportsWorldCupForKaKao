//
//  Hurdle.m
//  HurdleWorldCup
//
//  Created by ksoo k on 12. 5. 3..
//  Copyright (c) 2012³â __MyCompanyName__. All rights reserved.
//

#include "HWHurdle.h"
#include "KS_Util.h"


Hurdle::Hurdle()
{
	hurdleSprite = nil;	
}

int Hurdle::HurdleHeight()
{
	switch(type)
	{
		case HURDLE_A:
			return 35;
		case HURDLE_B:
			return 45 - 5;
		case HURDLE_C:
			return 45 - 5;
		case HURDLE_D:
			return 55 - 5;
		case HURDLE_TRACK_A:
			//return 100;
			return 35;
		case HURDLE_TRACK_B:
			return 45 - 5;
		case HURDLE_TRACK_C:
			return 57 - 5; //##
		case HURDLE_SAFARI_A:
			return 35;
		case HURDLE_SAFARI_B:
			return 45-5;
		case HURDLE_SAFARI_C:
			return 55-5;
		case HURDLE_SAFARI_D:
			return 48;
	}
	return -1;
}

int Hurdle::HurdleWidth()
{
	return 8;
	switch(type)
	{
		case HURDLE_A:
			return 6;
		case HURDLE_B:
			return 6;
		case HURDLE_C:
			return 6;
		case HURDLE_D:
			return 6;
		case HURDLE_TRACK_A:
			return 6;
		case HURDLE_TRACK_B:
			return 6;
		case HURDLE_TRACK_C:
			return 6;
		case HURDLE_SAFARI_A:
			return 6;
		case HURDLE_SAFARI_B:
			return 6;
		case HURDLE_SAFARI_C:
			return 6;
		case HURDLE_SAFARI_D:
			return 6;
	}
	return -1;
}

