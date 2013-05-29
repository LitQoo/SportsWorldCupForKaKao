//
//  ShotResult.cpp
//  ArcherWorldCupX
//
//  Created by apple on 12. 7. 9..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#include "ACShotResult.h"

bool ShotResult::initWithDelegate(cocos2d::CCObject *_target, SEL_MenuHandler _selector, SEL_MenuHandler _exitSelector){
	if(!CCLayerColor::init()){
		return false;
	}

//	pointlbl = new CCLabelTTF;
//	pointlbl->initWithString("10 point", "AppleGhodic", 30);
//	pointlbl->setAnchorPoint(ccp(0.5,0.5));
//	pointlbl->setPosition(ccp(160,320));
//	pointlbl->setColor(ccBLACK);
//	this->addChild(pointlbl,1);
    
    pointlbl = new LQLabel;
    pointlbl->init();
    pointlbl->setAnchorPoint(ccp(0.5,0.5));
    pointlbl->setText("10 Point");
    pointlbl->setTextSize(30);
    pointlbl->setTextColor(ccBLACK);
    pointlbl->setShadowColor(ccBLACK);
    pointlbl->setShadow(true, 2);
    pointlbl->setFrame(160,320, 200, 30);
    this->addChild(pointlbl,1);
	
//	bonuslbl = new CCLabelTTF;
//	bonuslbl->initWithString("bonus", "AppleGhodic", 20);
//	bonuslbl->setAnchorPoint(ccp(0.5,0.5));
//	bonuslbl->setPosition(ccp(160,180));
//	bonuslbl->setColor(ccORANGE);
//	this->addChild(bonuslbl,1);
    
    bonuslbl = new LQLabel;
    bonuslbl->init();
    bonuslbl->setAnchorPoint(ccp(0.5,0.5));
    bonuslbl->setText("Bonus");
    bonuslbl->setTextSize(20);
    bonuslbl->setTextColor(ccORANGE);
    bonuslbl->setShadowColor(ccBLACK);
    bonuslbl->setShadow(true, 1);
    bonuslbl->setFrame(160,180, 200, 30);
    this->addChild(bonuslbl,1);
	
//	combolbl = new CCLabelTTF;
//	combolbl->initWithString("combo", "AppleGhodic", 20);
//	combolbl->setAnchorPoint(ccp(0.5,0.5));
//	combolbl->setPosition(ccp(160,155));
//	combolbl->setColor(ccBLACK);
//	this->addChild(combolbl,1);
    
    combolbl = new LQLabel;
    combolbl->init();
    combolbl->setAnchorPoint(ccp(0.5,0.5));
    combolbl->setText("Combo");
    combolbl->setTextSize(20);
    combolbl->setTextColor(ccGRAY);
    combolbl->setShadowColor(ccBLACK);
    combolbl->setShadow(true, 1);
    combolbl->setFrame(160,155, 200, 30);
    this->addChild(combolbl,1);
	
//	totalPointlbl = new CCLabelTTF;
//	totalPointlbl->initWithString("totalPoint", "AppleGhodic", 20);
//	totalPointlbl->setAnchorPoint(ccp(0.5,0.5));
//	totalPointlbl->setPosition(ccp(160,130));
//	totalPointlbl->setColor(ccRED);
//	this->addChild(totalPointlbl,1);
	
    totalPointlbl = new LQLabel;
    totalPointlbl->init();
    totalPointlbl->setAnchorPoint(ccp(0.5,0.5));
    totalPointlbl->setText("TotalPoint");
    totalPointlbl->setTextSize(20);
    totalPointlbl->setTextColor(ccRED);
    totalPointlbl->setShadowColor(ccBLACK);
    totalPointlbl->setShadow(true, 1);
    totalPointlbl->setFrame(160,130, 200, 30);
    this->addChild(totalPointlbl,1);
    
	target=new CCSprite;
	target->initWithFile("pan.png");
	target->setAnchorPoint(ccp(0.5,0.5));
	target->setPosition(ccp(160,250));
	this->addChild(target,1);
		
	targetLayer=new CCLayer;
	targetLayer->setAnchorPoint(ccp(0.5,0.5));
	targetLayer->setPosition(ccp(160,250));
	this->addChild(targetLayer,2);
	
	lastShot=NULL;
	
	
    exitButton = new LQButton;
    exitButton->init();
    exitButton->setAnchorPoint(ccp(0.5,0.5));
    exitButton->setFrame( 50, 40, 45, 45);
    exitButton->setText("Exit");
    exitButton->setTextSize(12);
    exitButton->setTextColor(ccWHITE);
    exitButton->setTargetAndSelector(_target, _exitSelector);
    exitButton->setBackgroundColor(ccc4f(0, 0, 0, 255));
    this->addChild(exitButton,3);
    
    nextButton = new LQButton;
    nextButton->init();
    nextButton->setAnchorPoint(ccp(0.5,0.5));
    nextButton->setFrame(160, 40, 100, 45);
    nextButton->setText("Next");
    nextButton->setTextSize(20);
   // nextButton->setShadowColor(ccc3(180, 180, 0));
   // nextButton->setShadow(true, 1);
    nextButton->setTextColor(ccWHITE);
    nextButton->setTargetAndSelector(_target, _selector);
    nextButton->setBackgroundColor(ccc4f(230, 0, 0, 255));
    this->addChild(nextButton,3);
    
    
//    CCLayerColor *nbback = CCLayerColor::create(ccc4(0,0,0,255), 150, 40);
//    //nbback->setAnchorPoint(ccp(0.5,0.5));
//    nbback->setPosition(ccp(160-75,50-20));
//    this->addChild(nbback,1);
//    
//    CCLayerColor *ebback = CCLayerColor::create(ccc4(0,0,0,255), 50, 40);
//    //ebback->setAnchorPoint(ccp(0.5,0.5));
//    ebback->setPosition(ccp(50-25,50-20));
//    this->addChild(ebback,1);
//    
//	nextButton= new CCLabelTTF;
//	nextButton->initWithString("Next Arrow", "AppleGhodic", 20);
//	CCMenuItemLabel *nb= CCMenuItemLabel::create(nextButton, _target, _selector);
//	
//    nb->setAnchorPoint(ccp(0.5,0.5));
//    nb->setPosition(ccp(160,50));
//    
//    CCMenuItem *eb = CCMenuItemLabel::create(CCLabelTTF::create("exit", "AppleGhodic", 20), _target,_exitSelector);
//   
//    eb->setAnchorPoint(ccp(0.5,0.5));
//    eb->setPosition(ccp(50,50));
//    
//    
//
//    
//    menu = CCMenu::create(eb,nb,NULL);
//    //menu->alignItemsHorizontally();
//	menu->setAnchorPoint(ccp(0,0));
//	menu->setPosition(ccp(0,0));
//	this->addChild(menu,3);

	return true;
}

void ShotResult::addShot(int point,CCPoint arrowPoint){
	
	target->setScale(0.2);
	targetLayer->setIsVisible(false);
	
	CCScaleTo *action1_1 = CCScaleTo::actionWithDuration(0.2, 1.0);
	CCMoveTo *action1_2 = CCMoveTo::actionWithDuration(0.2,ccp(160,250));
	CCSpawn *action1=CCSpawn::actionOneTwo(action1_1,action1_2);
	CCCallFunc *action2 = CCCallFunc::actionWithTarget(this, callfunc_selector(ShotResult::showHistroy));
	CCFiniteTimeAction *actions = CCSequence::actions(action1,action2,NULL);
	target->runAction(actions);

	
	pointlbl->setText(this->stringWithFormat("%d Point!!",point).c_str());
	if(point>=9)pointlbl->setTextColor(ccORANGE);
	else if(point>=7)pointlbl->setTextColor(ccRED);
	else if(point>=5)pointlbl->setTextColor(ccBLUE);
	else pointlbl->setTextColor(ccBLACK);
	
	if(lastShot!=NULL)lastShot->setColor(ccORANGE);
	
	if(point>0){
		CCLabelTTF *s = new CCLabelTTF;
		s->initWithString("X", "AppleGhodic", 10);
		s->setColor(ccGREEN);
		s->setAnchorPoint(ccp(0.5,0.5));
		s->setPosition(arrowPoint);
		targetLayer->addChild(s,2);
		lastShot=s;
		s->release();
	}
}

void ShotResult::showHistroy(){
	targetLayer->setIsVisible(true);
}
void ShotResult::setCombo(int combo){
	combolbl->setIsVisible(combo==0?false:true);
	combolbl->setText(this->stringWithFormat("%d Combo!!",combo).c_str());
}
void ShotResult::setBonus(float bonus){
	bonuslbl->setIsVisible(bonus==0?false:true);
	if(bonus>0){
		bonuslbl->setText(this->stringWithFormat("+%.1f Bonus Arrow!!",bonus).c_str());
	}else{
		bonuslbl->setText(this->stringWithFormat("%.0f Arrow!!",bonus).c_str());
	}
	
}
void ShotResult::setTotalPoint(int totalPoint){
    totalPointlbl->setText(this->stringWithFormat("Total %d Point!!",totalPoint).c_str());
}

void ShotResult::setButtonTitle(const char* _title){
	nextButton->setText(_title);
}
ShotResult::~ShotResult(){
	pointlbl->release();
	bonuslbl->release();
	combolbl->release();
	totalPointlbl->release();
	target->release();
	nextButton->release();
    targetLayer->release();
}

string ShotResult::stringWithFormat(const char* _Format, ...)
{
	va_list argptr;
	va_start(argptr, _Format);
	char buf[256];
	vsprintf(buf, _Format, argptr);
	string str = buf;
	
	va_end(argptr);
	return str;
}