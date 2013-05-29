//
//  GageLayer.h
//  ArcherWorldCupX
//
//  Created by apple on 12. 7. 7..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#ifndef ArcherWorldCupX_GageLayer_h
#define ArcherWorldCupX_GageLayer_h


#include "cocos2d.h"
#include <math.h>
#include <string>
using namespace std;

using namespace cocos2d;

class GageLayer : public cocos2d::CCLayerColor {
		
public:
	LAYER_NODE_FUNC(GageLayer);
	virtual bool init();
	CCLabelTTF *windPower;
	CCLabelTTF *windDir;
	CCLabelTTF *leftTime;
	
	int wind;
	int dir;
	int leftSec;
	
	void setWind(int _wind);
	void setTime(int _time);
	void initWind();
	void initTime();
	string stringWithFormat(const char* _Format, ...);
	~GageLayer();
};
#endif
