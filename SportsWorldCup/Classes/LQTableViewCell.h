//
//  LQTableViewCell.h
//  archervszombie
//
//  Created by LitQoo on 12. 12. 18..
//  Copyright (c) 2012년 LitQoo Inc. All rights reserved.
//

#ifndef archervszombie_LQTableViewCell_cpp
#define archervszombie_LQTableViewCell_cpp

#include "cocos2d.h"

using namespace cocos2d;

class LQTableViewCell : public cocos2d::CCLayerColor {
    
public:
    int index;
    bool isEnd;
    virtual bool init(float x, float y, float width, float height);
    ~LQTableViewCell();
    
};

#endif
