//
//  CCLabelTTF+CCLabelTTFExt.m
//  HurdleWorldCup
//
//  Created by ksoo k on 12. 5. 25..
//  Copyright (c) 2012³â __MyCompanyName__. All rights reserved.
//

#include "KSLabelTTF.h"
#include "ccTypes.h"
#include <string>
using namespace std;

CCSprite* KSLabelTTF::labelWithString(string str, string fontName, float fontSize, ccColor3B color, float strokeSize, ccColor3B strokeColor) 
{	
	CCLabelTTF *label = CCLabelTTF::labelWithString(str.c_str(), fontName.c_str(), fontSize);
	
	CCRenderTexture* rt = CCRenderTexture::renderTextureWithWidthAndHeight(label->getTexture()->getContentSize().width + strokeSize*2 , label->getTexture()->getContentSize().height+strokeSize*2);
	
	label->setFlipY(true);
	label->setColor(strokeColor);
	
	ccBlendFunc originalBlendFunc = label->getBlendFunc();
	ccBlendFunc _t = {GL_SRC_ALPHA, GL_ONE};
	label->setBlendFunc(_t);
	CCPoint bottomLeft = ccp(label->getTexture()->getContentSize().width * label->getAnchorPoint().x + strokeSize, label->getTexture()->getContentSize().height * label->getAnchorPoint().y + strokeSize);
	CCPoint position = ccpSub(label->getPosition(), ccp(-label->getContentSize().width / 2.0f,-label->getContentSize().height / 2.0f));
	
	rt->begin();
	
	for (int i=0; i<360; i++) // you should optimize that for your needs
	{
		label->setPosition(ccp(bottomLeft.x + sin(CC_DEGREES_TO_RADIANS(i))*strokeSize,bottomLeft.y + cos(CC_DEGREES_TO_RADIANS(i))*strokeSize));
		label->visit();
	}
	
	label->setPosition(bottomLeft);
	label->setBlendFunc(originalBlendFunc);
	label->setColor(color);
	label->visit();
	
	rt->end();
	
	rt->setPosition(position);
	
	return CCSprite::spriteWithTexture(rt->getSprite()->getTexture());	
}