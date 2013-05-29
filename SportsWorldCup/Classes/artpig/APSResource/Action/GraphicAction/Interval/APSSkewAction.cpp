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

#include "APSSkewAction.h"

#include "APSGraphicGroup.h"
#include "APSCCActionInterval.h"

using namespace cocos2d;
using namespace std;

APS_BEGIN_NAMESPACE(artpig)


const string &APSSkewAction::getEndSkewKey() {
    static const string key = "endSkew";
    return key;
}

APSSkewAction::APSSkewAction(APSDictionary *properties) :
m_startSkew(CCPointZero),
m_endSkew(CCPointZero)
{
    if (properties) {
        this->initWithProperties(properties);
    }
}

APSSkewAction::~APSSkewAction() {
}

bool APSSkewAction::initWithProperties(APSDictionary *properties) {
    bool ok = APSIntervalGraphicAction::initWithProperties(properties);
    
    // set endSkew
    APSString *str = (APSString *)properties->getObjectForKey(this->getEndSkewKey());
    if (str) {
        this->m_endSkew = str->getPointValue();
    }
    
    /*
     v1.0.1
     APSNumber *number = (APSNumber *)properties->getObjectForKey(kAPSSkewAction_endSkewXKey);
     if (number) {
     this->m_endSkew.x = number->getFloat();
     }
     
     number = (APSNumber *)properties->getObjectForKey(kAPSSkewAction_endSkewYKey);
     if (number) {
     this->m_endSkew.y = number->getFloat();
     }
     */
    return ok;
    
}

#pragma #### Protected ####

void APSSkewAction::saveNodeState() {
    APSGraphic *graphic = this->getTargetGraphic();
    if (graphic) {
        CCNode *node = graphic->getNode();
        this->m_startSkew.x = node->getSkewX();
        this->m_startSkew.y = node->getSkewY();
    }
}

void APSSkewAction::restoreNodeState() {
    APSGraphic *graphic = this->getTargetGraphic();
    if (graphic) {
        CCNode *node = graphic->getNode();
        node->setSkewX(this->m_startSkew.x);
        node->setSkewY(this->m_startSkew.y);
    }
}

CCActionInterval *APSSkewAction::makeGraphicCCAction(bool universal, float scale) {
    if (this->getRelative()) {
#if COCOS2D_VERSION >= 0x00020000
        return CCSkewBy::create(this->getDuration(), this->m_endSkew.x, this->m_endSkew.y);
#else
        return CCSkewBy::actionWithDuration(this->getDuration(), this->m_endSkew.x, this->m_endSkew.y);
#endif//APS_SKIP
    } else {
        return APSCCSkewTo::actionWithDuration(this->getDuration(), this->m_endSkew.x, this->m_endSkew.y);
    }
}

APS_END_NAMESPACE
