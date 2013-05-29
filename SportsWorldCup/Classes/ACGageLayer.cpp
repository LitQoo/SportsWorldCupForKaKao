//
//  GageLayer.cpp
//  ArcherWorldCupX
//
//  Created by apple on 12. 7. 7..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//


#include "ACGageLayer.h"

bool GageLayer::init(){
	if ( !CCLayerColor::init() )
    {
        return false;
    }
	
	this->setContentSize(CCSizeMake(320,75));
	this->setColor((ccColor3B){100,100,100});
	
	CCSprite *windBG= CCSprite::spriteWithFile("wind.png");
	windBG->setAnchorPoint(ccp(0,0));
	windBG->setPosition(ccp(5,0));
	this->addChild(windBG,1);
	
	
	CCSprite *timeBG= CCSprite::spriteWithFile("time.png");
	timeBG->setAnchorPoint(ccp(0,0));
	timeBG->setPosition(ccp(240,0));
	this->addChild(timeBG,1);

	
	windPower = CCLabelTTF::labelWithString("-", "AppleGothic", 20);
	windPower->setColor(ccRED);
	windPower->setAnchorPoint(ccp(0.5,0.5));
	windPower->setPosition(ccp(41,36));
	this->addChild(windPower,2);
	
	windDir = CCLabelTTF::labelWithString("-", "appleGhodic", 12);
	windDir->setColor(ccRED);
	windDir->setAnchorPoint(ccp(0.5,0.5));
	windDir->setPosition(ccp(41,52));
	this->addChild(windDir,2);
	
	leftTime = CCLabelTTF::labelWithString("-", "appleGhodic", 20);
	leftTime->setAnchorPoint(ccp(0.5,0.5));
	leftTime->setColor(ccBLACK);
	leftTime->setPosition(ccp(276,36));
	this->addChild(leftTime,2);
	
	wind=0;
	leftSec=0;
	
	return true;
}

void GageLayer::initWind()
{
	this->setWind(2-rand()%5);
}

void GageLayer::setWind(int _wind){
	wind=_wind;
	windPower->setString(this->stringWithFormat("%d",abs(wind)).c_str());
	
	if(_wind>0){
		windDir->setString("▶");
	}else if(_wind<0){
		windDir->setString("◀");
	}else{
		windDir->setString("-");
	}
}

void GageLayer::setTime(int _time){
	leftSec=_time;
	float sec = leftSec/10.0f;
	leftTime->setString(this->stringWithFormat("%.1f",sec).c_str());
	if(leftSec<50)leftTime->setColor(ccRED);
	else if(leftSec<100)leftTime->setColor(ccBLUE);
	else leftTime->setColor(ccBLACK);
}

void GageLayer::initTime(){
	leftSec=150;
	this->setTime(leftSec);
	
}

GageLayer::~GageLayer(){
}

string GageLayer::stringWithFormat(const char* _Format, ...)
{
	va_list argptr;
	va_start(argptr, _Format);
	char buf[256];
	vsprintf(buf, _Format, argptr);
	string str = buf;
	
	va_end(argptr);
	return str;
}