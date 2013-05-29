//
//  Geometry.h
//  BasketWorldCup2
//
//  Created by ksoo k on 12. 11. 30..
//
//

#ifndef __BasketWorldCup2__Geometry__
#define __BasketWorldCup2__Geometry__

#include "cocos2d.h"
USING_NS_CC;
class Geometry
{
public:
	static bool pointInPolygon(CCPoint pt, const vector<CCPoint>& polygon)
	{
		int sizeofVertex = polygon.size();
		int followIndex = polygon.size() - 1;
		bool odd = false;
		for(int i=0; i<sizeofVertex; i++)
		{
			// followIndex -> i
			// last, 0
			// 0, 1
			//
			CCPoint followPt = polygon[followIndex];
			CCPoint frontPt = polygon[i];
			if((followPt.y < pt.y && pt.y <= frontPt.y) || (frontPt.y < pt.y && pt.y <= followPt.y))
			{
				float newX = (pt.y - frontPt.y) * (followPt.x - frontPt.x)/(followPt.y - frontPt.y) + frontPt.x;
				if(pt.x < newX)
					odd = !odd;
			}
			
			
			followIndex = i;
		}
		
		return odd;
	}
	static bool lineInPolygon(CCPoint pt1, CCPoint pt2, const vector<CCPoint>& polygon)
	{
		if(pointInPolygon(pt1, polygon) && pointInPolygon(pt2, polygon))
			return true;
		
		int sizeofVertex = polygon.size();
		int followIndex = polygon.size() - 1;
		for(int i=0; i<sizeofVertex; i++)
		{
			// followPt -> frontPt
			CCPoint followPt = polygon[followIndex];
			CCPoint frontPt = polygon[i];
			
			if(ccpSegmentIntersect(followPt, frontPt, pt1, pt2))
				return true;
			
			followIndex = i;
		}
		
		return false;
	}
};

#endif /* defined(__BasketWorldCup2__Geometry__) */
