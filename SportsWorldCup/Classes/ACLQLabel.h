//
//  LQLabel.h
//  ArcherWorldCupX
//
//  Created by Kim HyunSoo on 12. 8. 8..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#ifndef LQLib_LQLabel_h
#define LQLib_LQLabel_h

#include "cocos2d.h"
#include <string.h>


using namespace cocos2d;
using namespace std;

//class GameScene;

class LQLabel : public cocos2d::CCLayer {

    public:
    CCLabelTTF *textLabel;
    CCLabelTTF *shadowLabel;
    CCLayerColor *backRect;
    CCRect *frame;
    virtual bool init();
    ~LQLabel();
    void setFrame(float x,float y,float width,float height);
    void setBackgroundColor(ccColor4B _color);
    void setBackgroundClear();
    void setText(const char *_text);
    void setTextColor(const ccColor3B &var);
    void setTextSize(float _size);
    void setShadow(bool isOn,float depth);
    void setShadowColor(const ccColor3B &var);
};



#endif