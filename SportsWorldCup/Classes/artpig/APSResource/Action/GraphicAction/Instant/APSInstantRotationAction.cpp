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

#include "APSInstantRotationAction.h"
#include "APSGraphicGroup.h"


using namespace cocos2d;
using namespace std;

APS_BEGIN_NAMESPACE(artpig)


const string &APSInstantRotationAction::getEndRotationKey() {
    static const string key = "endRotation";
    return key;
}

APSInstantRotationAction::APSInstantRotationAction(APSDictionary *properties) :
m_endRotation(0.f)
{
    if (properties) {
        this->initWithProperties(properties);
    }
}

APSInstantRotationAction::~APSInstantRotationAction() {
}

bool APSInstantRotationAction::initWithProperties(APSDictionary *properties) {
    if(!APSInstantGraphicAction::initWithProperties(properties))
        return false;
    
    // set endRotation
    APSNumber *number = (APSNumber *)properties->getObjectForKey(this->getEndRotationKey());
    if (number) {
        this->m_endRotation = number->getFloat();
    }
    
    return true;
}


void APSInstantRotationAction::instantAction() {
    CCNode *target = this->getTargetGraphic()->getNode();
    if (this->getRelative()) {
        target->setRotation(target->getRotation()+this->getEndRotation());
    } else {
        target->setRotation(this->getEndRotation());
    }
}

#pragma #### Protected ####

CCFiniteTimeAction * APSInstantRotationAction::makeMidCCAction(bool universal, float scale) {
    // Before calling instant action, the same type of Cocos2D action should be applied to stop currently running action.
#if COCOS2D_VERSION >= 0x00020000
    return CCSequence::create(CCRotateBy::create(0, 0), CCCallFunc::create(this, callfunc_selector(APSInstantRotationAction::instantAction)));
#else
    return CCSequence::actionOneTwo(CCRotateBy::actionWithDuration(0, 0), CCCallFunc::actionWithTarget(this, callfunc_selector(APSInstantRotationAction::instantAction)));
#endif//APS_SKIP
    }

APS_END_NAMESPACE

