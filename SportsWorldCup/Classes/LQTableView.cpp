//
//  LQTableView.cpp
//  archervszombie
//
//  Created by LitQoo on 12. 12. 18..
//  Copyright (c) 2012년 LitQoo Inc. All rights reserved.
//
#include "LQTableView.h"



bool LQTableView::init(float width, float height, CCObject *_target,SEL_CallTableCell _selector)
{
    if(!CCLayerColor::initWithColorWidthHeight(ccc4f(0, 100, 100, 0), width, height)){
        return false;
    }
	//CCNode::setContentSize(CCSizeMake(width, height));
	target = _target;
	selector = _selector;
    this->setIsTouchEnabled(true);
    //this->setAnchorPoint(ccp(0,0));
    
    
    // = new CCRect;
    size.width = width;
	size.height = height;
    cellArray = CCArray::array();
	cellArray->retain();
    isAnimated=false;
    
    lastCell=NULL;
    firstCell=NULL;
    return true;
}


void LQTableView::visit()
{
    glEnable(GL_SCISSOR_TEST);
	int viewport [4];
    glGetIntegerv (GL_VIEWPORT, viewport);
	CCSize rSize = CCEGLView::sharedOpenGLView().getSize();
	float wScale = viewport[2] / rSize.width;
	float hScale = viewport[3] / rSize.height;
	CCPoint ct = getParent()->convertToWorldSpace(getPosition());
    //float x = rect.origin.x*wScale + viewport[0];
	float x = (ct.x - size.width/2.f) * wScale + viewport[0];
//    float y = rect.origin.y*hScale + viewport[1];
	float y = (ct.y - size.height/2.f) * hScale + viewport[1];
//	CCPoint at = CCPointApplyAffineTransform(ccp(x, y), nodeToParentTransform());
    float w = size.width*wScale;
    float h = size.height*hScale;
//	w *= nodeToWorldTransform().a;
//	h *= nodeToWorldTransform().d;
    glScissor(x, y, w, h);
    
//
//    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    glScissor(rect->origin.x*2,rect->origin.y*2, rect->size.width*2, rect->size.height*3);
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    glScissor(rect->origin.x*2,rect->origin.y*2, rect->size.width*2, rect->size.height*3);
//#endif
    
    CCLayerColor::visit();
    glDisable(GL_SCISSOR_TEST);
}

void LQTableView::loadTable(int r){
	reset();
	totalRow = r;
	if(r == 0)
		return;
	
    int index=0;
    int celly=0;
    while (1) {
        CCNode *node = new CCNode;
        node->setTag(index);
        LQTableViewCell *cell = (LQTableViewCell *)(target->*selector)(node);
		delete node;
        cell->index=index;
        celly+=cell->getContentSize().height+1;

        
        if(celly > size.height+cell->getContentSize().height){
//            cell->setPosition(ccp(0,size.height-celly));
//            cellArray->addObject(cell);
//            this->addChild(cell,2);
			
//            if(lastCell!=NULL)lastCell->removeFromParentAndCleanup(true);
//            
//            cell->setPosition(ccp(0,0));
//            cell->setScale(0.9);
//            lastCell=cell;
//            this->addChild(cell,1);
            
            break;
        }else{
            cell->setPosition(ccp(0,size.height-celly));
            cellArray->addObject(cell);
            this->addChild(cell,2);
        }
        
        
        
        
        index++;
        if(index>totalRow-1)break;
    }
    
}

void LQTableView::moveList(float y){
    if(y>10)y=10;
    if(y<-10)y=-10;
    float newy;
//    float newy=0;
//    
//    LQTableViewCell *headCell=(LQTableViewCell *)cellArray->objectAtIndex(0);
//    
//    
//    newy=headCell->getPositionY()-y;
//    if(newy>this->rect->size.height-headCell->getContentSize().height){
//        if(headCell->index!=totalRow-1){
//            if(firstCell!=NULL){
//                firstCell->removeFromParentAndCleanup(true);
//            }
//        firstCell=headCell;
//        cellArray->removeObjectAtIndex(0);
//        firstCell->setPosition(ccp(0,this->rect->size.height-firstCell->getContentSize().height));
//        this->reorderChild(firstCell, 1);
//        }
//    }else if(firstCell!=NULL && headCell->getPositionY()<this->rect->size.height-headCell->getContentSize().height-firstCell->getContentSize().height){
//        if(firstCell!=NULL){
//           // cellArray->addObject
//            cellArray->insertObject(firstCell, 0);
//            this->reorderChild(firstCell,2);
//            firstCell->setPositionY(headCell->getPositionY()+headCell->getContentSize().height+1);
//            
//            
//            if(firstCell->index-1>=0){
//                CCNode *node = new CCNode;
//                node->setTag(firstCell->index-1);
//                LQTableViewCell *newcell=(LQTableViewCell *)(target->*selector)(node);
//                newcell->index=firstCell->index-1;
//                newcell->setPosition(ccp(0,this->rect->size.height-newcell->getContentSize().height));
//                firstCell=newcell;
//                this->addChild(newcell,1);
//            }else{
//                firstCell=NULL;
//            }
//
//        }
//    }
//    
//
//    
//    LQTableViewCell *cell=(LQTableViewCell *)cellArray->lastObject();
//    if(cell==NULL)return;
//    if(cell->getPositionY()>cell->getContentSize().height){
//        if(lastCell!=NULL){
//            cellArray->addObject(lastCell);
//            this->reorderChild(lastCell,2);
//            lastCell->setScale(1.0);
//            lastCell->setPositionY(cell->getPositionY()-lastCell->getContentSize().height-1);
//            
//            if(lastCell->index+1<totalRow){
//                CCNode *node = new CCNode;
//                node->setTag(lastCell->index+1);
//                LQTableViewCell *newcell=(LQTableViewCell *)(target->*selector)(node);
//                newcell->index=lastCell->index+1;
//                newcell->setPosition(ccp(0,0));
//                newcell->setScale(0.9);
//                lastCell=newcell;
//                this->addChild(newcell,1);
//            }else{
//                lastCell=NULL;
//            }
//        }
//    }else if(cell->getPositionY()<=0){
//        if(cell->index!=0){
//            if(lastCell!=NULL){
//                
//                //FBConnector::get()->removeCallbackCell(lastCell);
//                lastCell->removeFromParentAndCleanup(true);}
//        lastCell=cell;
//        cellArray->removeLastObject();
//        cell->setPositionY(0);
//        lastCell->setScale(0.9);
//        this->reorderChild(lastCell,1);
//        }
//    }
    LQTableViewCell *headCell=(LQTableViewCell *)cellArray->objectAtIndex(0);
    if(headCell->getPositionY()>this->size.height){
        if(headCell->index!=totalRow-1){
            headCell->removeFromParentAndCleanup(true);
            cellArray->removeObject(headCell);
        }
    }else if(headCell->getPositionY()<this->size.height-headCell->getContentSize().height){
        if(headCell->index!=0){
            CCNode *node = new CCNode;
            node->setTag(headCell->index-1);
            LQTableViewCell *newcell=(LQTableViewCell *)(target->*selector)(node);
            newcell->index=headCell->index-1;
            newcell->setPosition(ccp(0,headCell->getPositionY()+headCell->getContentSize().height+1));
            this->addChild(newcell,2);
            cellArray->insertObject(newcell, 0);
        }
    }
    
    LQTableViewCell *cell=(LQTableViewCell *)cellArray->lastObject();
    if(cell->getPositionY()>0){
        if(cell->index!=totalRow-1){
            CCNode *node = new CCNode;
            node->setTag(cell->index+1);
            LQTableViewCell *newcell=(LQTableViewCell *)(target->*selector)(node);
            newcell->index=cell->index+1;
            newcell->setPosition(ccp(0,cell->getPositionY()-cell->getContentSize().height-1));
            this->addChild(newcell,2);
            cellArray->addObject(newcell);
        }
    }else if(cell->getPositionY()<cell->getContentSize().height*-1){
        if(cell->index!=0){
            cell->removeFromParentAndCleanup(true);
            cellArray->removeObject(cell);

        }
    }
    
    for (int i=0; i<cellArray->count(); i++) {
        LQTableViewCell *cell = (LQTableViewCell *)cellArray->objectAtIndex(i);
        newy = cell->getPositionY()-y;
        cell->setPositionY(newy);
    }
    
    
    
}

void LQTableView::reset(){
    totalRow=0;
    this->removeAllChildrenWithCleanup(true);
    cellArray->removeAllObjects();
    
}

void LQTableView::registerWithTouchDispatcher()
{
	
	//CCLayer::registerWithTouchDispatcher();
	
	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, kCCMenuTouchPriority - 2, false);
}



bool LQTableView::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    //return true;
    CCTouch* touch = pTouch;
    CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
	if(totalRow<=0) return true;
    //location = this->convertToNodeSpace(location);
    
    if(CCRect::CCRectContainsPoint(this->boundingBox(), location) && this->getIsVisible())
    {
        touchy=location.y;
        
        timeval time;
        gettimeofday(&time, NULL);
        touchStartTime = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec;
        touchStartY=location.y;
    }
    
    
    this->unschedule(schedule_selector(LQTableView::moveAnimation));
    this->unschedule(schedule_selector(LQTableView::moveAnimationUp));
    this->unschedule(schedule_selector(LQTableView::moveAnimationDown));
    isAnimated=false;
	return true;
}

void LQTableView::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
	
}
void  LQTableView::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
	
    if(totalRow<=0)return;
    // return;
    CCTouch* touch = pTouch;
    CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
    //location = this->convertToNodeSpace(location);
    
    if(CCRect::CCRectContainsPoint(this->boundingBox(), location) && this->getIsVisible())
    {
        float now = location.y;
        this->moveList(touchy-now);
        touchy=now;
        

    }else{
        LQTableViewCell* headCell = (LQTableViewCell *)cellArray->objectAtIndex(0);
        
        if(headCell->index==0 && headCell->getPositionY()<this->size.height-headCell->getContentSize().height-5){
            //if(this->isScheduled(schedule_selector(LQTableView::moveAnimationUp))){
            if(isAnimated==false){
                this->schedule(schedule_selector(LQTableView::moveAnimationUp));
                return;
            }
            //}
        }
        
        LQTableViewCell* tailCell = (LQTableViewCell *)cellArray->lastObject();
        if(tailCell->index==this->totalRow-1 && tailCell->getPositionY()>5 && headCell->index!=0){
            //if(this->isScheduled(schedule_selector(LQTableView::moveAnimationDown))){
            if(isAnimated==false){
                this->schedule(schedule_selector(LQTableView::moveAnimationDown));
                return;
            }
            //}
        }
        
        timeval time;
        gettimeofday(&time, NULL);
        long _time = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec - touchStartTime;
        float _y=location.y-touchStartY;
        float _spd = _y/_time*10000;
        
        
        if(isAnimated==false && abs((int)_spd)>2){
            if(_spd>0)moveSpeed=((int)_spd*10)/10.f-2;
            else moveSpeed=((int)_spd*10)/10.f+2;
            this->schedule(schedule_selector(LQTableView::moveAnimation));
        }

    }
    
}
void  LQTableView::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
   if(totalRow<=0)return;
   // return;
    CCTouch* touch = pTouch;
    CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
   // location = this->convertToNodeSpace(location);
    
    
    if(CCRect::CCRectContainsPoint(this->boundingBox(), location) && this->getIsVisible())
    {
        float now = location.y;
        this->moveList(touchy-now);
        touchy=now;
        

        
        LQTableViewCell* headCell = (LQTableViewCell *)cellArray->objectAtIndex(0);
        if(headCell->index==0 && headCell->getPositionY()<this->size.height){
            if(isAnimated==false)this->schedule(schedule_selector(LQTableView::moveAnimationUp));
        }
        
        LQTableViewCell* tailCell = (LQTableViewCell *)cellArray->lastObject();
        if(tailCell->index==this->totalRow-1 && headCell->getPositionY()>1 && headCell->index!=0){
            if(isAnimated==false)this->schedule(schedule_selector(LQTableView::moveAnimationDown));
        }
        
        timeval time;
        gettimeofday(&time, NULL);
        long _time = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec - touchStartTime;
        float _y=location.y-touchStartY;
        float _spd = _y/_time*10000;
        
        
        if(isAnimated==false && abs((int)_spd)>2){
            if(_spd>0)moveSpeed=((int)_spd*10)/10.f-2;
            else moveSpeed=((int)_spd*10)/10.f+2;
            this->schedule(schedule_selector(LQTableView::moveAnimation));
        }
    }
    
}

void LQTableView::moveAnimation(){
    
    LQTableViewCell* headCell = (LQTableViewCell *)cellArray->objectAtIndex(0);
    if(headCell->index==0 && headCell->getPositionY()<this->size.height-headCell->getContentSize().height){
        this->unschedule(schedule_selector(LQTableView::moveAnimation));
        this->schedule(schedule_selector(LQTableView::moveAnimationUp));
        return;
    }
    
    LQTableViewCell* tailCell = (LQTableViewCell *)cellArray->lastObject();
    if(tailCell->index==this->totalRow-1 && tailCell->getPositionY()>0 && headCell->index!=0){
        this->unschedule(schedule_selector(LQTableView::moveAnimation));
        this->schedule(schedule_selector(LQTableView::moveAnimationDown));
        return;
    }
    
    isAnimated=true;
    if(moveSpeed>0)moveSpeed-=0.1;
    else if(moveSpeed<0)moveSpeed+=0.1;
    if((moveSpeed<0.1 && moveSpeed>-0.1) || isAnimated==false){
        this->unschedule(schedule_selector(LQTableView::moveAnimation));
        moveSpeed=0;
        isAnimated=false;
    }
    this->moveList(-1*moveSpeed);
    

}

void LQTableView::moveAnimationUp(){
    isAnimated=true;
    LQTableViewCell* headCell = (LQTableViewCell *)cellArray->objectAtIndex(0);
    if(headCell->getPositionY()<this->size.height-headCell->getContentSize().height){
        float _y = this->size.height-headCell->getContentSize().height-headCell->getPositionY();
        if(_y<=10){
            moveList(_y*-1);
            this->unschedule(schedule_selector(LQTableView::moveAnimationUp));
            isAnimated=false;
        }else moveList(-10);
    }else{
        this->unschedule(schedule_selector(LQTableView::moveAnimationUp));
        isAnimated=false;
    }
}

void LQTableView::moveAnimationDown(){
    isAnimated=true;
    LQTableViewCell* tailCell = (LQTableViewCell *)cellArray->lastObject();
    if(tailCell->getPositionY()>1){
        float _y = tailCell->getPositionY()-1;
        if(_y<=10){
            moveList(_y);
            this->unschedule(schedule_selector(LQTableView::moveAnimationDown));
            isAnimated=false;
        }
        else moveList(10);
    }else{
        this->unschedule(schedule_selector(LQTableView::moveAnimationDown));
        isAnimated=false;
    }
}

LQTableView::~LQTableView(){
    cellArray->release();
}