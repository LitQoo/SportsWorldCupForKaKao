//
//  CCLabelTTF+CCLabelTTFExt.h
//  HurdleWorldCup
//
//  Created by ksoo k on 12. 5. 25..
//  Copyright (c) 2012³â __MyCompanyName__. All rights reserved.
//

#pragma once
#include "CCLabelTTF.h"
#include "cocos2d.h"

USING_NS_CC;
class KSLabelTTF : public CCLabelTTF
{
public:
	static CCSprite* labelWithString(string str, string fontName, float fontSize, ccColor3B color, float strokeSize, ccColor3B strokeColor);
};
