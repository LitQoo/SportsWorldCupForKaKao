//
//  LQLabel.cpp
//  ArcherWorldCupX
//
//  Created by Kim HyunSoo on 12. 8. 8..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#include "ACLQLabel.h"

bool LQLabel::init(){
	if(!CCLayer::init()){
		return false;
	}
    
    textLabel = new CCLabelTTF;
    textLabel->initWithString("", "AppleGothic", 20.f);
    this->addChild(textLabel,3);
    
    shadowLabel = new CCLabelTTF;
    shadowLabel->initWithString("", "AppleGothic", 20.f);
    this->addChild(shadowLabel,2);
    shadowLabel->setIsVisible(false);
    
    
    frame = new CCRect(0, 0, 0, 0);
    
    backRect = new CCLayerColor;
	backRect->initWithColorWidthHeight(ccc4f(0, 0, 0, 0),frame->size.width,frame->size.height);
    this->addChild(backRect,1);
    
    
    return true;
}

void LQLabel::setFrame(float x,float y,float width,float height){
	frame->origin.x = x;
	frame->origin.y = y;
	frame->size.width = width;
	frame->size.height = height;
    this->setPosition(ccp(x,y));
    
    float __x =  frame->size.width * (1.0-this->getAnchorPoint().x) - frame->size.width;
    float __y =  frame->size.height * (1.0-this->getAnchorPoint().y) - frame->size.height;
                             
    textLabel->setPosition(ccp(__x+frame->size.width/2,__y+frame->size.height/2));
    backRect->setPosition(ccp(frame->size.width * (1.0-this->getAnchorPoint().x) - frame->size.width,
                              frame->size.height * (1.0-this->getAnchorPoint().y) - frame->size.height));
    
}

void LQLabel::setText(const char *_text){
    
    textLabel->setString(_text);
    shadowLabel->setString(_text);
}
void LQLabel::setTextColor(const ccColor3B &var){
    textLabel->setColor(var);
}

void LQLabel::setShadowColor(const ccColor3B &var){
    shadowLabel->setColor(var);
}
void LQLabel::setTextSize(float _size){
//    textLabel->setFontSize(_size);
//    shadowLabel->setFontSize(_size);
}

void LQLabel::setBackgroundColor(ccColor4B _color){
    this->setBackgroundClear();
    backRect = new CCLayerColor;
    backRect->initWithColorWidthHeight(_color,frame->size.width,frame->size.height);
    backRect->setPosition(ccp(frame->size.width * (1.0-this->getAnchorPoint().x) - frame->size.width,
                              frame->size.height * (1.0-this->getAnchorPoint().y) - frame->size.height));
    
    this->addChild(backRect,1);
}

void LQLabel::setShadow(bool isOn,float depth){
    shadowLabel->setIsVisible(isOn);
    float __x =  frame->size.width * (1.0-this->getAnchorPoint().x) - frame->size.width;
    float __y =  frame->size.height * (1.0-this->getAnchorPoint().y) - frame->size.height;
    shadowLabel->setPosition(ccp(__x+frame->size.width/2,__y+frame->size.height/2- depth));
}
void LQLabel::setBackgroundClear(){
    if (backRect!=NULL) {
        backRect->removeFromParentAndCleanup(true);
        backRect->release();
        backRect=NULL;
    }
    
    backRect = new CCLayerColor;
    backRect->initWithColorWidthHeight(ccc4f(0, 0, 0, 0),frame->size.width,frame->size.height);
    backRect->setPosition(ccp(frame->size.width * (1.0-this->getAnchorPoint().x) - frame->size.width,
                              frame->size.height * (1.0-this->getAnchorPoint().y) - frame->size.height));
    
    this->addChild(backRect,1);

}
LQLabel::~LQLabel(){
    
    if(backRect!=NULL)backRect->release();
	delete frame;
    shadowLabel->release();
    textLabel->release();
}

