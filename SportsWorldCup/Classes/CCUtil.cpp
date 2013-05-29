//
//  CCUtil.m
//  HurdleWorldCup
//
//  Created by ksoo k on 12. 5. 15..
//  Copyright (c) 2012≥‚ __MyCompanyName__. All rights reserved.
//

#include "CCUtil.h"


int CCUtil::getNumberOfChild(CCNode* node)
{
	if(node->getChildren()->count() == 0)
		return 0;
	else {
		int total = 0;
		for(int i=0; i<node->getChildren()->count(); i++)
		{
			CCNode* n = (CCNode*)node->getChildren()->objectAtIndex(i);
			total += 1 + CCUtil::getNumberOfChild(n);
		}
		return total;
	}	
}

bool CCUtil::isLeft(CCPoint a, CCPoint b, CCPoint c)
{
	int t = a.x*b.y + b.x*c.y + c.x*a.y - a.x * c.y - b.x * a.y - c.x * b.y;
	if(t > 0)
		return true;
	else {
		return false;
	}
}


// º±∫–AB øÕ CD ∞° ±≥¬˜«œ∏È return true;
bool CCUtil::isIntersect(CCPoint a, CCPoint b, CCPoint c, CCPoint d)
{
	bool _cond1 = (CCUtil::isLeft(a, b, c) ^ CCUtil::isLeft(a, b, d)) && (CCUtil::isLeft(c, d, a) ^ CCUtil::isLeft(c, d, b)); // 완전교차 조건
	if(_cond1) 
		return true;
	bool _cond2 = CCUtil::isOnSegment(c, d, a) || CCUtil::isOnSegment(c, d, b) || CCUtil::isOnSegment(a, b, c) || CCUtil::isOnSegment(a, b, d);
	if(_cond2)
		return true;
	return false;
}

bool CCUtil::isOnSegment(CCPoint a, CCPoint b, CCPoint c)
{

	double S = 0.5 * fabs((double)a.x*(double)b.y + (double)b.x*(double)c.y + (double)c.x*(double)a.y - (double)b.x*(double)a.y - (double)c.x*(double)b.y - (double)a.x*(double)c.y);
	if(S <= 0.01 && a.x <= c.x && c.x <= b.x)
	{
		return true;
	}
	else {
		return false;
	}
}

