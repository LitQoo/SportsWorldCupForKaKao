//
//  FirstHurdleMapLayer.m
//  HurdleWorldCup
//
//  Created by ksoo k on 12. 4. 30..
//  Copyright (c) 2012≥‚ __MyCompanyName__. All rights reserved.
//

#include "HWFirstHurdleMapLayer.h"
#include "HWSharingClass.h"
#include "HWPlay.h"
#include "Percentage.h"
#include "HWGameInformation.h"
// #include "ccConfig.h"
#include "GameSystem.h"

extern int mygravity;


void FirstHurdleMapLayer::onEnterTransitionDidFinish()
{
	HurdleMapLayer::onEnterTransitionDidFinish();
	
	this->createSprite(1.f);
}

GroundType FirstHurdleMapLayer::obsTypeDeterminer()
{
	return TTYPE;
}
HurdleType FirstHurdleMapLayer::hurdleTypeDeterminer(GroundType gt)
{
	return HURDLE_TRACK_C;
	if(hwplay->combo.getVar() <= 7)
	{ // easy
		int r = rand()%10 + 1;
		if(r == 1 || r == 2 || r == 3 || r == 4 || r == 5)
			return HURDLE_TRACK_A;
		else if(r == 6 || r == 7 || r == 8 || r == 9)
			return HURDLE_TRACK_B;
		else if(r == 10)
			return HURDLE_TRACK_C;
		return HURDLE_TRACK_A;
	}
	else if(hwplay->combo.getVar() <= 15)
	{ // medium
		int r = rand()%3;
		if(r == 0)
			return HURDLE_TRACK_A;
		else if(r == 1)
			return HURDLE_TRACK_B;
		else if(r == 2)
			return HURDLE_TRACK_C;
		return HURDLE_TRACK_A;
	}
	else {
		int r = rand()%10 + 1;
		if(r == 1 || r == 2)
			return HURDLE_TRACK_A;
		else if(r == 3 || r == 4 || r == 5 || r == 6)
			return HURDLE_TRACK_B;
		else if(r == 7 || r == 8 ||r == 9 || r == 10)
			return HURDLE_TRACK_C;
		return HURDLE_TRACK_A;
	}
}

void FirstHurdleMapLayer::cachingPList()
{
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("trackmap.plist");
	
	mapBatch = CCSpriteBatchNode::batchNodeWithFile("trackmap.png", 100);
	this->addChild(mapBatch, 0);
}
void FirstHurdleMapLayer::createScreen()
{
	CCMutableArray<Obstacle*>* newObject = new CCMutableArray<Obstacle*>();
	newObject->autorelease();

	int r = rand()%100;
	int t = 50;
	
	t -= (pInfo->runStep / 2) * 15;
	//r = 6;

	if(0<=r && r <= t) // t * 10 % 확률로 허들 패턴.
	{
		HurdleMapLayer::majorIsHurdle(newObject);
	}
	else if(r > t) // ∆Æ∑¢ ¿ß¡÷
	{
		HurdleMapLayer::majorIsTrack(newObject);
	}
	

	for(int i=0; i<newObject->count(); i++)
	{
		Obstacle* t = newObject->getObjectAtIndex(i);
		this->setHurdleSprite(t);
		Obstacles->addObject(t);
	}
	
}
void FirstHurdleMapLayer::createSprite(float scale)
{
	float curPos = fabsf(this->getPosition().x);
	
	float viewRange = 500 / scale;
	for(int i=0; i<Obstacles->count(); i++)
	{
		Obstacle* t = Obstacles->getObjectAtIndex(i);
		int addCounter = 0;
		for(int i = 0; i<t->length; i++)
		{
			float x = (t->start + patternWidth * i);
			if(curPos + viewRange >= x && i >= t->drawCount)
			{
				CCSprite* spr;
				spr = CCSprite::spriteWithSpriteFrameName("track2.png");
				spr->getTexture()->setAliasTexParameters();
				spr->setAnchorPoint(CCPointMake(0,0.5));
				
				spr->setPosition(CCPointMake(t->start + i*patternWidth, t->height));
				mapBatch->addChild(spr, -1);
				
				addCounter++;
				t->sprites.push_back(spr);
			}
		}
		
		{
			string firstFrontFileName;
			string middleFrontFileName;
			string lastFrontFileName;

			int r = t->start%2;
			if(r == 0) // groundType
			{
				firstFrontFileName = "worldcup_track1_first.png";
				middleFrontFileName = "worldcup_track1_middle.png";
				lastFrontFileName = "worldcup_track1_last.png";
			}else
			{
				firstFrontFileName = "worldcup_track2_first.png";
				middleFrontFileName = "worldcup_track2_middle.png";
				lastFrontFileName = "worldcup_track2_last.png";
			}
			for(int j=0; j<=0; j++) // ºº∑Œ
			{ //
				for(int i=0; i<t->length; i++) // ∞°∑Œ,¿Âæ÷π∞¿« addChild
				{
					float x = (t->start + patternWidth * i);
					if(curPos + viewRange >= x && i >= t->drawCount)
					{
						CCSprite* pattern;
						if(t->length == 1)
						{
							pattern = CCSprite::spriteWithSpriteFrameName(middleFrontFileName.c_str());
							pattern->getTexture()->setAliasTexParameters();
							pattern->setAnchorPoint(CCPointMake(0,1));
							pattern->setPosition(ccp(t->start, t->height - patternWidth * j - 10));
							mapBatch->addChild(pattern, -1);
						}
						else if(i == 0)
						{

							pattern = CCSprite::spriteWithSpriteFrameName(firstFrontFileName.c_str());
							pattern->getTexture()->setAliasTexParameters();
							pattern->setAnchorPoint(CCPointMake(0,1));
							pattern->setPosition(ccp(t->start + i*patternWidth, t->height - patternWidth * j - 10));
							mapBatch->addChild(pattern, -1);
							
						}
						else if(i == t->length - 1)
						{

							pattern = CCSprite::spriteWithSpriteFrameName(lastFrontFileName.c_str());
							pattern->getTexture()->setAliasTexParameters();
							pattern->setAnchorPoint(CCPointMake(0,1));
							pattern->setPosition(ccp(t->start + i*patternWidth, t->height - patternWidth * j - 10));
							mapBatch->addChild(pattern, -1);
						}
						else {
							pattern = CCSprite::spriteWithSpriteFrameName(middleFrontFileName.c_str());
							pattern->getTexture()->setAliasTexParameters();
							pattern->setAnchorPoint(CCPointMake(0,1));
							pattern->setPosition(ccp(t->start + i*patternWidth, t->height - patternWidth * j - 10));
							mapBatch->addChild(pattern, -1);
						}
						
						t->sprites.push_back(pattern);
					}
				}
			}
		}
		
		t->drawCount += addCounter;
	}
}
void FirstHurdleMapLayer::setBelowGround(Obstacle* t)
{
	return;
	string firstFrontFileName;
	string middleFrontFileName;
	string lastFrontFileName;
	int r = rand()%2;
	if(r == 0) // groundType
	{
		firstFrontFileName = "worldcup_track1_first.png";
		middleFrontFileName = "worldcup_track1_middle.png";
		lastFrontFileName = "worldcup_track1_last.png";
	}else
	{
		firstFrontFileName = "worldcup_track2_first.png";
		middleFrontFileName = "worldcup_track2_middle.png";
		lastFrontFileName = "worldcup_track2_last.png";
	}
	for(int j=0; j<=0; j++) // ºº∑Œ
	{ //
		if(t->length == 1)
		{
			CCSprite* pattern = CCSprite::spriteWithSpriteFrameName(middleFrontFileName.c_str());
			pattern->getTexture()->setAliasTexParameters();
			pattern->setAnchorPoint(CCPointMake(0,1));
			pattern->setPosition(ccp(t->start,t->height - patternWidth * j - 10));
			mapBatch->addChild(pattern, -1);
		}
		else {
			for(int i=0; i<t->length; i++) // ∞°∑Œ,¿Âæ÷π∞¿« addChild
			{
				if(i == 0)
				{

					{CCSprite* pattern = CCSprite::spriteWithSpriteFrameName(firstFrontFileName.c_str());
					pattern->getTexture()->setAliasTexParameters();
					pattern->setAnchorPoint(CCPointMake(0,1));
					pattern->setPosition(ccp(t->start + i*patternWidth,t->height - patternWidth * j - 10));
					mapBatch->addChild(pattern, -1);
					}
				}
				else if(i == t->length - 1)
				{

					CCSprite* pattern = CCSprite::spriteWithSpriteFrameName(lastFrontFileName.c_str());
					pattern->getTexture()->setAliasTexParameters();
					pattern->setAnchorPoint(CCPointMake(0,1));
					pattern->setPosition(ccp(t->start + i*patternWidth,t->height - patternWidth * j - 10));
					mapBatch->addChild(pattern, -1);
				}
				else {
					CCSprite* pattern = CCSprite::spriteWithSpriteFrameName(middleFrontFileName.c_str());
					pattern->getTexture()->setAliasTexParameters();
					pattern->setAnchorPoint(CCPointMake(0,1));
					pattern->setPosition(ccp(t->start + i*patternWidth,t->height - patternWidth * j - 10));
					mapBatch->addChild(pattern, -1);
				}
			}

		}
		//		if(t->height - patternWidth * j<100)
		//			break;
	}
}
void FirstHurdleMapLayer::setTrackSprite(Obstacle* t)
{
	for(int i=0; i<t->length; i++) // ¿Âæ÷π∞¿« addChild
	{ // 
		CCSprite* spr;
		spr = CCSprite::spriteWithSpriteFrameName("track2.png");
		spr->getTexture()->setAliasTexParameters();
		spr->setAnchorPoint(CCPointMake(0,0.5));
		spr->setPosition(CCPointMake(t->start + i*patternWidth,t->height));
		//spr->setScaleX(1.1f);
		mapBatch->addChild(spr, -1);

		//for(int j=0; ; j++)
		//{
		//CCSprite* pattern = CCSprite::spriteWithFile("back1_pattern.png");
		//pattern.texture->setAliasTexParameters();
		//pattern->setAnchorPoint(CCPointMake(0,1));
		//pattern->setPosition(ccp(t->start + i*patternWidth,t->height - patternWidth * j - 7));
		//this->addChild(pattern, -1);
		//if(t->height - patternWidth * j<0)
		//break;
		//}
	}
}

int FirstHurdleMapLayer::createHurdlePattern(Obstacle* obs, int type, int _begin, int _end)
{
	if(type == 0)
	{
		for(int i=1; i<=10; i++)
		{ // i | I l ...
			Hurdle* h = new Hurdle();
			h->autorelease();
			if(playInfo->selectedItems["item1"] == false)
				h->setType(this->hurdleTypeDeterminer(obs->type)); // ∂• ≈∏¿‘ø° ¥Î«ÿ ¿˚¿˝«— «„µÈ¿ª ¡÷∑¡∞Ì «ﬂ¿∏≥™ ±◊≥… ∑£¥˝¿”.
			else
			{
				h->setType(HURDLE_TRACK_B); //
			}
			
			_begin += rand() % HurdleMapLayer::HURDLE_RANGE;
			if(_begin > _end - HurdleMapLayer::MIN_START_FROM ) // ∆Æ∑¢ ≥°ø°º± ª˝º∫ æ»«œ∞‘≤˚.
				return -1;
			h->setPosition(_begin); // «ŸΩ…
			obs->getHurdles()->addObject(h);
			_begin += getNormalHurdleMargin();
		}
		
		return _begin;
	}
	else if(type == 1)
	{ // o0O0o 
		for(int i=1; i<=5; i++)
		{
			if(_begin > _end - HurdleMapLayer::MIN_START_FROM ) // ∆Æ∑¢ ≥°ø°º± ª˝º∫ æ»«œ∞‘≤˚.
				return -1;
			Hurdle* h = new Hurdle();
			h->autorelease();
			if( i == 1 || i == 5)
				h->setType(HURDLE_TRACK_A);
			if( i == 2 || i == 4)
				h->setType(HURDLE_TRACK_B);
			if ( i == 3 )
			{
				if(playInfo->selectedItems["item1"])
				{
					h->setType(HURDLE_TRACK_B);
				}
				else
				{
					h->setType(HURDLE_TRACK_C);
				}
			}
			h->setPosition(_begin);
			obs->getHurdles()->addObject(h);
			_begin += 20 + hwplay->speedCombo * 0.3;
		}
		_begin += HurdleMapLayer::MIN_HURDLE_MARGIN;
		return _begin;
	}
	else if(type == 2)
	{ // i i i i i i i i i
		int nu = rand()%5 + 4;
		for(int i=1; i<=nu; i++)
		{
			if(_begin > _end - HurdleMapLayer::MIN_START_FROM ) // ∆Æ∑¢ ≥°ø°º± ª˝º∫ æ»«œ∞‘≤˚.
				return -1;
			Hurdle* h = new Hurdle();
			h->autorelease();
			if(rand()%2)
			{
				if(playInfo->selectedItems["item1"])
					h->setType(HURDLE_TRACK_B);
				else
					h->setType(HURDLE_TRACK_C);
			}
			else
				h->setType(HURDLE_TRACK_B);
			//rand()%2?:h->setType(HURDLE_TRACK_B);

			h->setPosition(_begin);
			obs->getHurdles()->addObject(h);

			_begin += getMinHurdleMargin();
		}
		_begin += HurdleMapLayer::MIN_HURDLE_MARGIN;
		return _begin;
	}
	else if(type == 3)
	{ // i ii iii iiii iiiii iiiiii
		int r = MIN(4, rand() % 5 + 3);
		for(int i=1; i<=r; i++)
		{
			if(_begin > _end - HurdleMapLayer::MIN_START_FROM ) // ∆Æ∑¢ ≥°ø°º± ª˝º∫ æ»«œ∞‘≤˚.
				return -1;
			
			for(int j = 1; j<=i; j++)
			{
				Hurdle* h = new Hurdle();
				h->autorelease();
				if(playInfo->selectedItems["item1"])
					h->setType(HURDLE_TRACK_B);
				else
					h->setType(HURDLE_TRACK_C);
				h->setPosition(_begin);
				obs->getHurdles()->addObject(h);
				
				_begin += 30;
			}
			_begin += getMinHurdleMargin();
		}
		_begin += HurdleMapLayer::MIN_HURDLE_MARGIN;
		return _begin;
	}
	else if(type == 4)
	{ // iiiii iiii iii ii i
		int r = MIN(4, rand() % 2 + 4);
		for(int i=r; i>=1; i--)
		{
			if(_begin > _end - HurdleMapLayer::MIN_START_FROM ) // ∆Æ∑¢ ≥°ø°º± ª˝º∫ æ»«œ∞‘≤˚.
				return -1;
			
			for(int j = 1; j<=i; j++)
			{
				Hurdle* h = new Hurdle();
				h->autorelease();
				if(playInfo->selectedItems["item1"])
					h->setType(HURDLE_TRACK_B);
				else
					h->setType(HURDLE_TRACK_C);
				h->setPosition(_begin);
				obs->getHurdles()->addObject(h);
				
				_begin += 30;
			}
			_begin += getMinHurdleMargin();
		}
		_begin += HurdleMapLayer::MIN_HURDLE_MARGIN;
		return _begin;
	}
	return _begin;
}

void FirstHurdleMapLayer::addHurdles(Obstacle* obs)
{
	int _begin = obs->getStart() + HurdleMapLayer::MIN_START_FROM;
	int _end = obs->getStart() + obs->length * patternWidth;

	while(1)
	{
		/* typeDeterminer */
		int r = rand()%15 + 1; //1,10]

		if(r <= ( (hwplay->combo.getVar()) >= 10 ? 10 : (hwplay->combo.getVar())) ) // combo 에 따른 확률로 특수 패턴 출연.
		{
			int t;
			Percentage p;
			p.add(20);
			p.add(40);
			p.add(20);
			p.add(20);
			if(p.prob(0))
			{
				t = 1; // ooOoo 패턴
			}
			else if(p.prob(1))
			{
				t = 2; // i i i i i 패턴.
			}
			else if(p.prob(2))
			{
				t = 3; // i ii iii iiii iiiii 패턴.
			}
			else if(p.prob(3))
			{
				t = 4;
			}
			_begin = this->createHurdlePattern(obs, t, _begin, _end); // special pattern
		}
		else {
			_begin = this->createHurdlePattern(obs, 0, _begin, _end); // gerneric pattern
		}

		if(_begin == -1)
			break;
		///////////////////////////////////////////////

	}
}

void FirstHurdleMapLayer::setHurdleSprite(Obstacle* t)
{
	for(int i=t->getHurdles()->count()-1; i>=0; i--)
	{
		Hurdle* h = t->getHurdles()->getObjectAtIndex(i);
		

		if(h->type == HURDLE_TRACK_A)
		{
			CCSprite* spr;
			spr = CCSprite::spriteWithSpriteFrameName("hurdle1.png");
			spr->setAnchorPoint(CCPointMake(0.5,0));
			spr->setPosition(CCPointMake(h->position,t->height));
			mapBatch->addChild(spr,1);
			spr->getTexture()->setAliasTexParameters(); 
			h->setHurdleSprite(spr);
		}
		else if(h->type == HURDLE_TRACK_B)
		{
			CCSprite* spr;
			spr = CCSprite::spriteWithSpriteFrameName("hurdle2.png");
			spr->setAnchorPoint(CCPointMake(0.5,0));
			spr->setPosition(CCPointMake(h->position,t->height));
			mapBatch->addChild(spr);
			spr->getTexture()->setAliasTexParameters(); 
			h->setHurdleSprite(spr);
		}
		else if(h->type == HURDLE_TRACK_C)
		{
			CCSprite* spr;
			spr = CCSprite::spriteWithSpriteFrameName("hurdle3.png");
			spr->setAnchorPoint(CCPointMake(0.5,0));
			spr->setPosition(CCPointMake(h->position,t->height));
			mapBatch->addChild(spr);
			spr->getTexture()->setAliasTexParameters(); 
			h->setHurdleSprite(spr);
		}   

	}
}
