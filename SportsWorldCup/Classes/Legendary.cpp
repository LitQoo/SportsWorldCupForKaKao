//
//  Legendary.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 4. 5..
//
//

#include "Legendary.h"

#include "GraphDog.h"
#include "GameSystem.h"
#include "BS2Play.h"
#include "HWPlay.h"
#include "ACPlay.h"
#include "AKPlay.h"
#include "KS_Util.h"

#include <sstream>
#include "Bridge.h"
#include "NSDefault.h"
#include "WCSaveData.h"
#include "AKSaveData.h"
#include "ACSaveData.h"
#include "BS2SaveData.h"
#include "HWSaveData.h"
#include "KSModalAlert.h"
#include "FBConnector.h"
#include "SWIntro.h"
Legendary::Legendary()
{
	// 테이블 좌표 잡아주기.
	int arrY[] = {200, 183, 166, 149, 132, 115, 98, 81, 64, 47};
	
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
Legendary::~Legendary()
{
	GraphDog::get()->removeCommand(this);
}

void Legendary::loadRecord(string gameType)
{
	removeAllChildrenWithCleanup(true);
	thiz = CCSprite::spriteWithFile("worldrecord_back.png");
	addChild(thiz);
	
	CCMenu* _menu = CCMenu::menuWithItems(0);
	_menu->setPosition(CCPointZero);
	thiz->addChild(_menu);
	
	CCMenuItem* close = CCMenuItemImage::itemFromNormalImage("main_exit.png", "main_exit.png", this, menu_selector(ThisClassType::CLOSE));
	close->setPosition(ccp(230, 243));
	_menu->addChild(close, 2);
	
	//	mediator->refreshEXP();

	float loadingX = 132;
	float loadingY = 144;
	auto animator = SceneUtil::playAnimation("loading.png", 0.1, 9, 9, 56, 56, true);
	animator.second->runAction(animator.first);
	thiz->addChild(animator.second, INT_MAX);
	animator.second->setPosition(ccp(loadingX, loadingY));
	loading = animator.second;
	
	JsonBox::Object param;
	//	param["score"] = gainScore;
	//	param["userdata"] = playInfo->__ateCoin;
	param["type"] = gameType;
	
	if(gameType == "AC")
	{
		CCSprite* bbs = CCSprite::spriteWithFile("worldrecord_archery.png");
		bbs->setPosition(ccp(95, 253));
		thiz->addChild(bbs, 2);
	}
	else if(gameType == "AK")
	{
		CCSprite* bbs = CCSprite::spriteWithFile("worldrecord_clay.png");
		bbs->setPosition(ccp(95, 253));
		thiz->addChild(bbs, 2);
	}
	else if(gameType == "BS")
	{
		CCSprite* bbs = CCSprite::spriteWithFile("worldrecord_basketball.png");
		bbs->setPosition(ccp(95, 253));
		thiz->addChild(bbs, 2);
	}
	else if(gameType == "HW")
	{
		CCSprite* bbs = CCSprite::spriteWithFile("worldrecord_hurdle.png");
		bbs->setPosition(ccp(95, 253));
		thiz->addChild(bbs, 2);
	}
	else if(gameType == "SK")
	{
		CCSprite* bbs = CCSprite::spriteWithFile("worldrecord_alpineski.png");
		bbs->setPosition(ccp(95, 253));
		thiz->addChild(bbs, 2);
	}
	else if(gameType == "WORLDCUP")
	{
		CCSprite* bbs = CCSprite::spriteWithFile("worldrecord_challengemode.png");
		bbs->setPosition(ccp(95, 253));
		thiz->addChild(bbs, 2);
	}
	graphdog->command("getmaxscores", &param, this, gd_selector(ThisClassType::finishGetScores));
}
bool Legendary::init()
{
	CCSprite::init();
	
	//	this->setIsTouchEnabled(true);
	
	schedule(schedule_selector(Legendary::update));
	return true;
}

void Legendary::CLOSE(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	mediator->showGameSelect();
}
void Legendary::update(ccTime dt)
{

}

void Legendary::appearMe()
{
	
}
void Legendary::finishGetScores(JsonBox::Object js)
{
	//	ostringstream oss;
	//	oss << js << endl;
	//	CCLog("**********************************************************************\n%s", oss.str().c_str());
	loading->setIsVisible(false);
	try {
		KS::KSLog("%", js);
		KS::KSLog("%", js["state"].getString().c_str());
		if(js["state"].getString() != "ok")
			throw string(ML::key("retrying"));
		CCLabelBMFont* allUsers = CCLabelBMFont::labelWithString(KS_Util::stringWithFormat("%d", js["alluser"].getInt()).c_str(), "main1.fnt");
		//		allUsers->setColor(ccc3(0,0,0));
		allUsers->setAnchorPoint(ccp(1, 0.5f));
		allUsers->setPosition(ccp(187, 21));
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
//			scoreFnt->setColor(ccc3(0, 0, 0));
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
	} catch (const string& msg) {
		//## 실패시 재요청...
		//mediator->showFailedGameOver(msg);
	}
	
	
	
}


void Legendary::onEnterTransitionDidFinish()
{
	CCSprite::onEnterTransitionDidFinish();
}
void Legendary::onExit()
{
	CCSprite::onExit();
}
