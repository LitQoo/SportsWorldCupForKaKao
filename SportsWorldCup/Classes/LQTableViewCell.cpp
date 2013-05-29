//
//  LQTableViewCell.cpp
//  archervszombie
//
//  Created by LitQoo on 12. 12. 18..
//  Copyright (c) 2012년 LitQoo Inc. All rights reserved.
//
#include "LQTableViewCell.h"

bool LQTableViewCell::init(float x, float y, float width, float height){
    
    if(!CCLayerColor::initWithColorWidthHeight(ccc4f(0, 0, 0, 0), width, height)){
        return false;
    }
    isEnd=false;
    this->setPosition(ccp(x,y));
    return true;
}

LQTableViewCell::~LQTableViewCell(){
    
}