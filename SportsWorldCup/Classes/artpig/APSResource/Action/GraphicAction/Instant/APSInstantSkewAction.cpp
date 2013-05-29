/****************************************************************************
 Copyright (c) 2012 ArtPig Software LLC
 
 http://www.artpigsoft.com
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "APSInstantSkewAction.h"
#include "APSGraphicGroup.h"
//#include "APSCCActionInterval.h"

using namespace cocos2d;
using namespace std;

APS_BEGIN_NAMESPACE(artpig)



const string &APSInstantSkewAction::getEndSkewKey() {
    static const string key = "endSkew";
    return key;
}

APSInstantSkewAction::APSInstantSkewAction(APSDictionary *properties) :
m_endSkew(CCPointZero)
{
    if (properties) {
        this->initWithProperties(properties);
    }
}

APSInstantSkewAction::~APSInstantSkewAction() {
}

bool APSInstantSkewAction::initWithProperties(APSDictionary *properties) {
    if(!APSInstantGraphicAction::initWithProperties(properties))
        return false;
    
    // set endSkew
    APSString *str = (APSString *)properties->getObjectForKey(this->getEndSkewKey());
    if (str) {
        this->m_endSkew = str->getPointValue();
    }
    
    return true;
    
}

#pragma #### Protected ####

void APSInstantSkewAction::instantAction() {
    CCNode *target = this->getTargetGraphic()->getNode();
    CCPoint es = this->getEndSkew();
    if (this->getRelative()) {
        target->setSkewX(target->getSkewX()+es.x);
        target->setSkewY(target->getSkewY()+es.y);
    } else {
        target->setSkewX(es.x);
        target->setSkewY(es.y);
    }
}

CCFiniteTimeAction * APSInstantSkewAction::makeMidCCAction(bool universal, float scale) {
#if COCOS2D_VERSION >= 0x00020000
    return CCSequence::create(CCSkewBy::create(0, 0, 0), CCCallFunc::create(this, callfunc_selector(APSInstantSkewAction::instantAction)));
#else
    return CCSequence::actionOneTwo(CCSkewBy::actionWithDuration(0, 0, 0), CCCallFunc::actionWithTarget(this, callfunc_selector(APSInstantSkewAction::instantAction)));
#endif//APS_SKIP
    }

APS_END_NAMESPACE

