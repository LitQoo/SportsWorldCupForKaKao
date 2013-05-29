//
//  WorldCupMiddle.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 4. 4..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//
#include "WorldCupMiddle.h"
#include "BS2Play.h"
#include "AKPlay.h"
#include "HWPlay.h"
#include "BS2BasketGame.h"
#include "AKAngryGameScene.h"
#include "HWFirstGameScene.h"
#include "ACGameScene.h"
#include "HWGameDirector.h"
#include "ACPlay.h"
#include "AKSaveData.h"
#include "ACSaveData.h"
#include "BS2SaveData.h"
#include "HWSaveData.h"

#include "GraphDog.h"
#include "WCPlay.h"
#include "WCSaveData.h"
#include "KS_Util.h"
#include "KSoundEngine.h"
WorldCupMiddle::WorldCupMiddle()
{
	int arrY[] = {185, 167, 149, 131, 114, 95, 77, 59, 41, 23};
	
	for(int i=0; i<10; i++)
	{
		position.rank[i].x = 15;
		position.flag[i].x = 40;
		position.user[i].x = 52;
		position.score[i].x = 220;
		position.time[i].x = 240;
	}
	for(int i=0; i<10; i++)
	{
		int Y = arrY[i];
		position.rank[i].y = Y;
		position.flag[i].y = Y;
		position.user[i].y = Y;
		position.score[i].y = Y;
		position.time[i].y = Y;
	}
	
	position.beginRankPosition = ccp(246, 235);
	position.endRankPosition = ccp(27, 235);
	
	position.rankTable = ccp(143, 159);
	position.myScoreTable = ccp(378, 221);
	position.home = ccp(378, 99);
	position.next = ccp(378, 39);
}
WorldCupMiddle::~WorldCupMiddle()
{
	CCArray* children = this->getChildren();
	for(int i=0; i<children->count(); i++)
	{
		CCObject* tempNode = children->objectAtIndex(i);
		if(tempNode != 0 && 1<tempNode->retainCount())
		{
			while(1<tempNode->retainCount())
				tempNode->release();
		}
	}
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
	CCTextureCache::sharedTextureCache()->removeAllTextures();

}

/*
 void initGameAndStart()
 {
 string game = getWorldcupGame();
 if(game != "")
 {
 if(game == "AC")
 {
 
 }
 else if(game == "AK")
 {
 
 }
 else if(game == "BS")
 {
 
 }
 else if(game == "HW")
 {
 
 }
 }
 }

 */
bool WorldCupMiddle::init()
{
	CCLayer::init();
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("flags.plist");
	_gainScore = gainScore = wcplay->gameScore.getVar();
	_accScore = wcplay->accScore.getVar();
	
	wcplay->accScore += wcplay->gameScore.getVar();
	updateWeeklyAndMaxScore();
	
	CCSprite* mainBack = CCSprite::spriteWithFile("main_back.png");
	mainBack->setPosition(ccp(240, 160));
	addChild(mainBack);
	
	titleBack = CCSprite::spriteWithFile("main_titleback.png");
	titleBack->setPosition(ccp(240, 160));
	titleBack->runAction(CCRepeatForever::actionWithAction(CCRotateBy::actionWithDuration(2.f*1.f/0.33f, 360.f)));
	addChild(titleBack, 2);
	title = CCSprite::spriteWithFile("main_title.png");
	title->setPosition(ccp(240, 160));
	addChild(title, 2);
	
	rankTable = CCSprite::spriteWithFile("worldcup_round_back.png");
	rankTable->setPosition(ccp(position.rankTable.x, position.rankTable.y - 400));
	addChild(rankTable);
	
	myScoreTable = CCSprite::spriteWithFile("worldcup_round_scoreback.png");
	myScoreTable->setPosition(ccp(position.myScoreTable.x + 300, position.myScoreTable.y));
	addChild(myScoreTable);
	
	CCMenu* _menu = CCMenu::menuWithItems(0);
	_menu->setPosition(ccp(0, 0));
	addChild(_menu);
	
	_next = CCMenuItemImage::itemFromNormalImage("worldcup_round_next.png", "worldcup_round_next.png",
															 this, menu_selector(ThisClassType::NEXT));
	_next->setPosition(ccp(position.next.x + 300, position.next.y));
	_menu->addChild(_next, 2);
	
	_home = CCMenuItemImage::itemFromNormalImage("middle_retire.png", "middle_retire.png",
															 this, menu_selector(ThisClassType::HOME));
	_home->setPosition(ccp(position.home.x + 300, position.home.y));
	_menu->addChild(_home, 2);
	
	JsonBox::Object param;
	
	param["score"] = wcplay->accScore.getVar();
	param["isover"] = false;
	graphdog->command("getscores", &param, this, gd_selector(ThisClassType::finishGetScores));
	
	
//	CCLog("gainScore : %d\n acc Score %d", gainScore, wcplay->accScore.getVar());
	
	me = CCSprite::spriteWithFile("gameover_me.png");
	me->setPosition(ccp(246, 235));
	rankTable->addChild(me, 2);
	
	meFnt = CCLabelBMFont::labelWithString("0", "reachvalue.fnt");
	meFnt->setColor(ccc3(27, 69, 137));
	meFnt->setPosition(ccp(me->getContentSize().width / 2.f, 38));
	me->addChild(meFnt, 3);
	rankX.init(me->getPosition().x, me->getPosition().x, 1.f);
	rankPercent.init(100.f, 100.f, 1.f);
	
	{
		scoreFnt = CCLabelBMFont::labelWithString(KS_Util::stringWithFormat("%d", gainScore).c_str(), "gameover_score1.fnt");
		scoreFnt->setAnchorPoint(ccp(0, 0.5f));
		scoreFnt->setPosition(ccp(76, 134));
		myScoreTable->addChild(scoreFnt, 2);
	}
	{
		totalFnt = CCLabelBMFont::labelWithString(KS_Util::stringWithFormat("%d", _accScore).c_str(), "gameover_score1.fnt");
		totalFnt->setAnchorPoint(ccp(0, 0.5f));
		totalFnt->setPosition(ccp(76, 101));
		myScoreTable->addChild(totalFnt, 2);
	}
	{
		weeklyFnt = CCLabelBMFont::labelWithString(KS_Util::stringWithFormat("%d", WCSaveData::getWeeklyMaxScore().second).c_str(), "gameover_score1.fnt");
		weeklyFnt->setAnchorPoint(ccp(0, 0.5f));
		weeklyFnt->setPosition(ccp(76, 69));
		myScoreTable->addChild(weeklyFnt, 2);
	}
	{
		maxFnt = CCLabelBMFont::labelWithString(KS_Util::stringWithFormat("%d", WCSaveData::getMaxScore()).c_str(), "gameover_score1.fnt");
		maxFnt->setAnchorPoint(ccp(0, 0.5f));
		maxFnt->setPosition(ccp(76, 37));
		myScoreTable->addChild(maxFnt, 2);
	}
	

	
	
	return true;
}
void WorldCupMiddle::updateWeeklyAndMaxScore()
{
	int week = playInfo->currentWeek;
	int myscore = wcplay->accScore.getVar();
	
	if(WCSaveData::getMaxScore() < myscore)
	{ // total max
		WCSaveData::setMaxScore(myscore);
		WCSaveData::setWeeklyMaxScore(week, myscore);
	}
	else
	{
		if(week != WCSaveData::getWeeklyMaxScore().first)
		{
			WCSaveData::setWeeklyMaxScore(week, myscore);
		}
		else
		{
			if(WCSaveData::getWeeklyMaxScore().second < myscore)
			{ // weekly
				WCSaveData::setWeeklyMaxScore(week, myscore);
			}
			else
			{
				// myscore
			}
		}
	}
}
void WorldCupMiddle::update(ccTime dt)
{
//	dt = 1.f/60.f;
	rankX.step(dt);
	rankPercent.step(dt);
	me->setPosition(ccp(rankX.getValue(), me->getPosition().y));
	meFnt->setString(KS_Util::stringWithFormat("%.0f%%", (float)(rankPercent.getValue() * 100.f)).c_str());

	static int _i = 0;
	static int _ii = 0;
	_ii = !_ii;
	_i++;
	//KSoundEngine::sharedEngine()->playSound(KS_Util::stringWithFormat("sound/scoring%d.mp3", _ii + 1));
	int decreaseUnit;
	
	decreaseUnit = gainScore / 3.f * dt;
	
	if(_gainScore > 0)
	{
		KSoundEngine::sharedEngine()->playSound(KS_Util::stringWithFormat("scoring%d.mp3", _ii + 1));
		if(_gainScore < decreaseUnit)
		{
			//initScore += addScore;
			_accScore += _gainScore;
			_gainScore = 0;
		}
		else {
			if(decreaseUnit <= 0)
			{
				_accScore += _gainScore;
				_gainScore = 0;
			}
			else {
				_gainScore -= decreaseUnit;
				_accScore += decreaseUnit;
			}
			
			//initScore += decreaseUnit;
		}
		
		totalFnt->setString(KS_Util::stringWithFormat("%d", _accScore).c_str());
		
		if(_accScore > WCSaveData::getWeeklyMaxScore().second)
		{
			// ...
			//KS_Util::stringWithFormat("%d", playInfo->gameScore.getVar()).c_str());
		}
	}
}

void WorldCupMiddle::startSchedule()
{
	schedule(schedule_selector(ThisClassType::update), 0.117f / 2.f);
}
void WorldCupMiddle::finishGetScores(JsonBox::Object js)
{
	//	ostringstream oss;
	//	oss << js << endl;
	//	CCLog("**********************************************************************\n%s", oss.str().c_str());
	
	try {
		KS::KSLog("%", js);
		KS::KSLog("%", js["state"].getString().c_str());
		if(js["state"].getString() != "ok")
			throw string(ML::key("retrying"));
		
		titleBack->runAction(CCFadeOut::actionWithDuration(0.5f));
		title->runAction(CCFadeOut::actionWithDuration(0.5f));

		rankTable->runAction(CCMoveTo::actionWithDuration(0.5f, position.rankTable));
		myScoreTable->runAction(CCMoveTo::actionWithDuration(0.5f, position.myScoreTable));
		_home->runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.2f), CCMoveTo::actionWithDuration(0.5f, position.home)));
		_next->runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.4f), CCMoveTo::actionWithDuration(0.5f, position.next)));
		runAction(CCCallFunc::actionWithTarget(this, callfunc_selector(ThisClassType::startSchedule)));
		CCLabelBMFont* allUsers = CCLabelBMFont::labelWithString(KS_Util::stringWithFormat("%d", js["alluser"].getInt()).c_str(), "main1.fnt");
//		allUsers->setColor(ccc3(255, 255, 255));
		allUsers->setPosition(ccp(227, 290));
		rankTable->addChild(allUsers, 2);
		
		
		
		JsonBox::Array jList = js["list"].getArray();
		int st = js["timestamp"].getInt();
		float myrank = js["myrank"].getInt();
		int myscore = 0;

		for(int i=0; i<jList.size() && i < 10; i++)
		{
			JsonBox::Object entry = jList[i].getObject();
			string nick = entry["nick"].getString();
			int score = entry["score"].getInt();
			int duration;
			int rank = entry["rank"].getInt();
			
			duration = entry["playtime"].getInt();
//			if(entry["etime"].getInt() == 0) // 안끝났다면
//			{
//				duration = st - entry["stime"].getInt();
//			}
//			else
//			{
//				duration = entry["etime"].getInt() - entry["stime"].getInt();
//			}
			string mmss;
			if(duration  < 3600)
			{
				mmss = KS_Util::stringWithFormat("%2d'%2d\"", (int)(duration / 60), duration % 60);
			}
			else
			{
				int hours = duration / 3600;
				if(hours > 100)
					mmss = KS_Util::stringWithFormat("inf", hours);
				else if(hours > 1)
					mmss = KS_Util::stringWithFormat("%d hrs", hours);
				else
					mmss = KS_Util::stringWithFormat("%d hr", hours);
			}
			
			CCLabelTTF* rankFnt;
			if(!(1 <= rank && rank <= 3))
			{
				rankFnt = CCLabelTTF::labelWithString(KS_Util::stringWithFormat("%d", entry["rank"].getInt()).c_str(), defaultFont, 12.f);
				rankFnt->setColor(ccc3(0, 0, 0));
				rankFnt->setPosition(position.rank[i]);
				rankTable->addChild(rankFnt, 2);
			}
			CCSprite* flag = KS::spriteWithSpriteFrameName(KS_Util::stringWithFormat("%s.png", entry["flag"].getString().c_str()).c_str());
			if(flag)
			{
				flag->setPosition(position.flag[i]);
				rankTable->addChild(flag, 2);
				flag->setScale(0.9f);
			}
			CCLabelTTF* nickTTF = CCLabelTTF::labelWithString(nick.c_str(), defaultFont, 12.f);
			nickTTF->setColor(ccc3(0, 0, 0));
			nickTTF->setAnchorPoint(ccp(0.f, 0.5f));
			nickTTF->setPosition(position.user[i]);
			
			
			rankTable->addChild(nickTTF, 2);
			
			CCLabelBMFont* scoreFnt = CCLabelBMFont::labelWithString(KS_Util::stringWithFormat("%d", score).c_str(), "rankscore.fnt");
			scoreFnt->setPosition(position.score[i]);
			scoreFnt->setAnchorPoint(ccp(1.f, 0.5f));
			
			
			rankTable->addChild(scoreFnt, 2);
			
			CCLabelTTF* timeFnt = CCLabelTTF::labelWithString(mmss.c_str(), defaultFont, 12.f);
			timeFnt->setColor(ccc3(0, 0, 0));
			timeFnt->setPosition(position.time[i]);
			if(entry["isme"].getBoolean())
			{
				scoreFnt->setColor(ccc3(255, 0, 0));
				nickTTF->setColor(ccc3(255, 0, 0));
				timeFnt->setColor(ccc3(255, 0, 0));
				if(!(1 <= rank && rank <= 3))
					rankFnt->setColor(ccc3(255, 0, 0));
				myscore = score;
			}
			rankTable->addChild(timeFnt, 2);
		}
		
		
		rankPercent.init(1.f, (myrank / (float)js["alluser"].getInt()), 2.f);
		rankX.init(me->getPosition().x, position.endRankPosition.x + (position.beginRankPosition.x - position.endRankPosition.x) * (myrank / (float)js["alluser"].getInt()),
				   2.f);
		
		
		
		
		
		
	} catch (const string& msg) {
		KSModalAlert("", msg.c_str(), 1,
					 ML::key("retrying").c_str(), this, callfunc_selector(ThisClassType::GAMEOVERRETRY));
		
	}
	
	
	
}

void WorldCupMiddle::GAMEOVERRETRY()
{
	JsonBox::Object param;
	
	param["score"] = wcplay->accScore.getVar();
	param["isover"] = false;
	graphdog->command("getscores", &param, this, gd_selector(ThisClassType::finishGetScores));
}
void WorldCupMiddle::NEXT(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	string r = playInfo->getWorldcupGame();
	
	
	playInfo->popGame();
	if(r == "")
		CCLog("not next");
	else
		playInfo->initGameAndStart(r);
}

void WorldCupMiddle::HOME(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	ReplaceSceneParam(SWIntro, INIT_MODE::WORLDCUP_GAMEOVER);
}

void WorldCupMiddle::onEnterTransitionDidFinish()
{
	CCLayer::onEnterTransitionDidFinish();
}
void WorldCupMiddle::onExit()
{
	
	
	CCLayer::onExit();
}
/*
 string res;
 if(WCSaveData::getMaxScore() < myscore)
 { // total max
 WCSaveData::setMaxScore(myscore);
 WCSaveData::setWeeklyMaxScore(week, myscore);
 res = "max";
 }
 else
 {
 if(week != WCSaveData::getWeeklyMaxScore().first)
 {
 WCSaveData::setWeeklyMaxScore(week, myscore);
 res = "weekly";
 }
 else
 {
 if(WCSaveData::getWeeklyMaxScore().second < myscore)
 { // weekly
 WCSaveData::setWeeklyMaxScore(week, myscore);
 res = "weekly";
 }
 else
 {
 // myscore
 res = "myscore";
 }
 }
 }
 
 */

