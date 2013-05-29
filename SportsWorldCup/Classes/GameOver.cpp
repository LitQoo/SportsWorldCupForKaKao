//
//  GameOver.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 2. 27..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//
#include "GameOver.h"
#include "GraphDog.h"
#include "GameSystem.h"
#include "BS2Play.h"
#include "HWPlay.h"
#include "ACPlay.h"
#include "AKPlay.h"
#include "WCPlay.h"
#include "SKPlay.h"
#include "KS_Util.h"

#include <sstream>
#include "Bridge.h"
#include "NSDefault.h"
#include "WCSaveData.h"
#include "AKSaveData.h"
#include "ACSaveData.h"
#include "BS2SaveData.h"
#include "HWSaveData.h"
#include "SKSaveData.h"
#include "KSModalAlert.h"
#include "FBConnector.h"
#include "SWIntro.h"
GameOver::GameOver()
{
	// 테이블 좌표 잡아주기.
	int arrY[] = {171, 154, 137, 120, 103, 86, 69, 52, 35, 18};

	for(int i=0; i<10; i++)
	{
		position.rank[i].x = 15;
		position.flag[i].x = 40;
		position.user[i].x = 52;
		position.score[i].x = 220;
		position.time[i].x = 244;
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
	
	position.beginRankPosition = ccp(246, 215);
	position.endRankPosition = ccp(27, 215);
}
GameOver::~GameOver()
{
	GraphDog::get()->removeCommand(this); 
}
bool GameOver::init()
{
	CCSprite::init();
		
//	this->setIsTouchEnabled(true);
	
	thiz = CCSprite::spriteWithFile("gameover.png");
	addChild(thiz);
	
		

	

	
	
//	mediator->refreshEXP();
	schedule(schedule_selector(GameOver::update));
	return true;
}
void GameOver::update(ccTime dt)
{
	
}

void GameOver::appearMe()
{
	
	
	int score = 0;
	if(playInfo->currentGame == "AK")
	{
		score = akplay->gameScore.getVar();
	}
	else if(playInfo->currentGame == "BS")
	{
		score = bs2play->gameScore.getVar();
	}
	else if(playInfo->currentGame == "AC")
	{
		score = acplay->gameScore.getVar();
	}
	else if(playInfo->currentGame == "HW")
	{
		score = hwplay->gameScore.getVar();
	}
	else if(playInfo->currentGame == "SK")
	{
		score = skplay->gameScore.getVar();
	}
	else if(playInfo->currentGame == "WORLDCUP")
	{
		score = wcplay->gameScore.getVar();
	}
	
	beforeEXP = NSDefault::getExp();
	int m = 1;
	if(playInfo->selectedItems["exp"])
	{
		m = 2;
		mediator->showExp2();
	}
	
	pair<int, int> lv_exp = playInfo->getLV_EXP(NSDefault::getExp());
	 // 경험치 얻을 수 있는 맥스치를 정해줌, 그리고 얻은 경험치를 어느 정도 절삭함.
	int getExp = MIN(playInfo->expFromLv(lv_exp.first), powf(score, 0.70f) * 10.f / 10000.f);
	NSDefault::setExp(NSDefault::getExp() + (getExp * m));
	afterEXP = NSDefault::getExp();
	
	mediator->refreshEXP();
	if(playInfo->getLV_EXP(beforeEXP).first < playInfo->getLV_EXP(afterEXP).first)
	{
		// level up! //##
		if(mediator->getHeartSize() < 5)
		{
			mediator->addHeartSprite();
			NSDefault::setHeartNumber(NSDefault::getHeartNumber() + 1);
		}
		
		mediator->showLevelUp();
	}
}
void GameOver::finishGetScores(JsonBox::Object js)
{
//	ostringstream oss;
//	oss << js << endl;
//	CCLog("**********************************************************************\n%s", oss.str().c_str());
	
	try {
//		KS::KSLog("%", js);
		KS::KSLog("%", js["state"].getString().c_str());
		if(js["state"].getString() != "ok")
			throw string(ML::key("retrying"));
		mediator->incLoadCount();
		CCLabelBMFont* allUsers = CCLabelBMFont::labelWithString(KS_Util::stringWithFormat("%d", js["alluser"].getInt()).c_str(), "main1.fnt");
//		allUsers->setColor(ccc3(0,0,0));
		allUsers->setPosition(ccp(226, 264));
		thiz->addChild(allUsers, 2);
		
		
		
		JsonBox::Array jList = js["list"].getArray();
		int st = js["timestamp"].getInt();
		int z = jList.size();

		float myrank = js["myrank"].getInt();
		int myscore = 0;
		int week = playInfo->currentWeek;
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
			if(duration < 3600)
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
				thiz->addChild(rankFnt, 2);
			}
			CCSprite* flag = KS::spriteWithSpriteFrameName(KS_Util::stringWithFormat("%s.png", entry["flag"].getString().c_str()).c_str());
			if(flag)
			{
				flag->setPosition(position.flag[i]);
				thiz->addChild(flag, 2);
				flag->setScale(0.9f);
			}
			CCLabelTTF* nickTTF = CCLabelTTF::labelWithString(nick.c_str(), defaultFont, 12.f);
			nickTTF->setColor(ccc3(0, 0, 0));
			nickTTF->setAnchorPoint(ccp(0.f, 0.5f));
			nickTTF->setPosition(position.user[i]);
			
			
			thiz->addChild(nickTTF, 2);
			
			CCLabelBMFont* scoreFnt = CCLabelBMFont::labelWithString(KS_Util::stringWithFormat("%d", score).c_str(), "rankscore.fnt");
			scoreFnt->setPosition(position.score[i]);
			scoreFnt->setAnchorPoint(ccp(1.f, 0.5f));
			
			
			thiz->addChild(scoreFnt, 2);
			
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
			thiz->addChild(timeFnt, 2);
		}
		
		if(playInfo->currentGame == "AC")
		{
			PUT_SCORE(ACSaveData, myscore, week, myrank);
		}
		else if(playInfo->currentGame == "AK")
		{
			PUT_SCORE(AKSaveData, myscore, week, myrank);
		}
		else if(playInfo->currentGame == "BS")
		{
			PUT_SCORE(BS2SaveData, myscore, week, myrank);
		}
		else if(playInfo->currentGame == "HW")
		{
			PUT_SCORE(HWSaveData, myscore, week, myrank);
		}
		else if(playInfo->currentGame == "SK")
		{
			PUT_SCORE(SKSaveData, myscore, week, myrank);
		}
		else  if(playInfo->currentGame == "WORLDCUP")
		{
			PUT_SCORE(WCSaveData, myscore, week, myrank);
		}

		
		mediator->sendFacebookScore();
		
		
	} catch (const string& msg) {
		mediator->showFailedGameOver(msg);
		
	}
	
	
	
	KS::setAllVisible(thiz, false);
	thiz->setIsVisible(true);
}


void GameOver::onEnterTransitionDidFinish()
{
	CCSprite::onEnterTransitionDidFinish();
}
void GameOver::onExit()
{
	CCSprite::onExit();
}


