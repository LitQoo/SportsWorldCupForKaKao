//
//  GameOver.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 2. 27..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//
#include "BattleGameOver.h"
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
BattleGameOver::BattleGameOver()
{
	position.TITLES[0] = ccp(134, 101);
	position.TITLES[1] = ccp(134, 79);
	position.TITLES[2] = ccp(134, 56);
	position.TITLES[3] = ccp(134, 33);
	
	position.rankTable = ccp(143, 159);
	position.home = ccp(378, 99);
	position.next = ccp(378, 39);
	position.CUP = ccp(383, 216); // 383, 216
	
	position.LEFTPLAYER = ccp(57, 236);
	position.LEFT_TOTAL_SCORE = ccp(58, 161);
	position.LEFT_SCORES[0] = ccp(50, 102);
	position.LEFT_SCORES[1] = ccp(50, 79);
	position.LEFT_SCORES[2] = ccp(50, 56);
	position.LEFT_SCORES[3] = ccp(50, 33);
	
	position.RIGHTPLAYER = ccp(211, 236);
	position.RIGHT_TOTAL_SCORE = ccp(211, 161);
	position.RIGHT_SCORES[0] = ccp(219, 102);
	position.RIGHT_SCORES[1] = ccp(219, 79);
	position.RIGHT_SCORES[2] = ccp(219, 56);
	position.RIGHT_SCORES[3] = ccp(219, 33);
	
	leftGame.init(0, 0, 0);
	rightGame.init(0, 0, 0);
	isJudged = false;
}
BattleGameOver::~BattleGameOver()
{
	GraphDog::get()->removeCommand(this);
}
bool BattleGameOver::init()
{
	CCSprite::init();
	wcplay->enemyScoreEachGame.clear();
	wcplay->enemyScoreEachGame.push_back(make_pair("AK", 253000)); //##
	wcplay->enemyScoreEachGame.push_back(make_pair("BS", 523400)); //##
	wcplay->enemyScoreEachGame.push_back(make_pair("HW", 583900)); //##
	wcplay->enemyScoreEachGame.push_back(make_pair("AC", 485700)); //##

	//	this->setIsTouchEnabled(true);
	leftGame.init(0, wcplay->gameScore.getVar(), wcplay->gameScore.getVar() / 60.f);
	int enemyScore = wcplay->enemyScoreEachGame[wcplay->scoreEachGame.size() - 1].second;
	rightGame.init(0, enemyScore, enemyScore / 60.f);
	
	wcplay->accScore += wcplay->gameScore.getVar();
	//updateWeeklyAndMaxScore(); //## 체크.
	
//	CCSprite* mainBack = CCSprite::spriteWithFile("main_back.png");
//	mainBack->setPosition(ccp(240, 160));
//	addChild(mainBack);
	

	
	thiz = CCSprite::spriteWithFile("match_round.png");
//	thiz->setPosition(ccp(position.rankTable.x, position.rankTable.y - 400));
	addChild(thiz);
	
	leftPlayer = CCSprite::spriteWithFile("kim.png");
	leftPlayer->setPosition(position.LEFTPLAYER);
	thiz->addChild(leftPlayer, 2);
	leftPlayer->setScale(0.8f);
	
	rightPlayer = CCSprite::spriteWithFile("seo.png");
	rightPlayer->setPosition(position.RIGHTPLAYER);
	thiz->addChild(rightPlayer, 2);
	rightPlayer->setScale(0.8f);
	//	mediator->refreshEXP();
	
	return true;
}
void BattleGameOver::update(ccTime dt)
{
	static int _i = 0;
	static int _ii = 0;
	_ii = !_ii;
	_i++;
	
	bool a = leftGame.step();
	bool b = rightGame.step();
	
	int leftTotal = (int)(leftGame.getValue() + leftTotalScore);
	int rightTotal = (rightGame.getValue() + rightTotalScore);
	leftScoreFnt->setString(KS_Util::stringWithFormat("%d", (int)leftGame.getValue()).c_str());
	rightScoreFnt->setString(KS_Util::stringWithFormat("%d", (int)rightGame.getValue()).c_str());
	
	leftTotalFnt->setString(KS_Util::stringWithFormat("%d", leftTotal).c_str());
	rightTotalFnt->setString(KS_Util::stringWithFormat("%d", rightTotal).c_str());
	
	if(a == false && b == false && !isJudged) // 둘다 점수 계산이 완료 되었다면,
	{
		float growTime = 0.4f;
		isJudged = true;
		if(leftTotal < rightTotal)
		{
			leftPlayer->runAction(CCScaleTo::actionWithDuration(growTime, 0.6f));
			rightPlayer->runAction(CCScaleTo::actionWithDuration(growTime, 1.f));
			runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(growTime),
											   
											   CCCallFuncND::actionWithTarget(this, callfuncND_selector(ThisClassType::finishScoreCounting), (void*)0)
											   ));
			winner = "LOSE";
		}
		else if(leftTotal > rightTotal)
		{
			leftPlayer->runAction(CCScaleTo::actionWithDuration(growTime, 1.f));
			rightPlayer->runAction(CCScaleTo::actionWithDuration(growTime, 0.6f));
			runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(growTime),
											   CCCallFuncND::actionWithTarget(this, callfuncND_selector(ThisClassType::finishScoreCounting), (void*)0)
											   ));
			winner = "WIN";
		}
		else // 비겼으므로 둘다 승자.
		{

		}
		
	}
}

void BattleGameOver::finishScoreCounting(void* obj)
{
	if(winner == "WIN")
	{
		CCSprite* winMark = CCSprite::spriteWithFile("winner.png");
		leftPlayer->addChild(winMark);
		winMark->setAnchorPoint(ccp(0.5f, 0.f));
		
		winMark->setPosition(ccp(leftPlayer->getContentSize().width / 2.f, leftPlayer->getContentSize().height - 15));
	}
	else if(winner == "LOSE")
	{
		CCSprite* winMark = CCSprite::spriteWithFile("winner.png");
		rightPlayer->addChild(winMark);
		winMark->setAnchorPoint(ccp(0.5f, 0.f));
		
		winMark->setPosition(ccp(rightPlayer->getContentSize().width / 2.f, rightPlayer->getContentSize().height - 15));
	}
}
void BattleGameOver::appearMe()
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
void BattleGameOver::finishGetScores(JsonBox::Object js)
{
	//	ostringstream oss;
	//	oss << js << endl;
	//	CCLog("**********************************************************************\n%s", oss.str().c_str());
	
	
	
//	thiz->runAction(CCMoveTo::actionWithDuration(0.5f, position.rankTable));
	
		mediator->incLoadCount();
		
		
//		if(playInfo->currentGame == "AC") //##
//		{
//			PUT_SCORE(ACSaveData, myscore, week, myrank);
//		}
//		else if(playInfo->currentGame == "AK")
//		{
//			PUT_SCORE(AKSaveData, myscore, week, myrank);
//		}
//		else if(playInfo->currentGame == "BS")
//		{
//			PUT_SCORE(BS2SaveData, myscore, week, myrank);
//		}
//		else if(playInfo->currentGame == "HW")
//		{
//			PUT_SCORE(HWSaveData, myscore, week, myrank);
//		}
//		else if(playInfo->currentGame == "SK")
//		{
//			PUT_SCORE(SKSaveData, myscore, week, myrank);
//		}
//		else  if(playInfo->currentGame == "WORLDCUP")
//		{
//			PUT_SCORE(WCSaveData, myscore, week, myrank);
//		}
		
		
		mediator->sendFacebookScore();
		
	initGameList();
	
	schedule(schedule_selector(BattleGameOver::update));
}


void BattleGameOver::onEnterTransitionDidFinish()
{
	CCSprite::onEnterTransitionDidFinish();
}
void BattleGameOver::onExit()
{
	CCSprite::onExit();
}
void BattleGameOver::initGameList()
{
	int i=0;
	map<string, string> title2filename;
	title2filename["AK"] = "title_clay.png";
	title2filename["BS"] = "title_basketball.png";
	title2filename["AC"] = "title_archery.png";
	title2filename["HW"] = "title_hurdle.png";
	title2filename["SK"] = "title_alpineski.png";
	
	string gameName;
	while((gameName = playInfo->getWorldcupGame()) != "") // empty 될 때 까지.
	{
		playInfo->popGame();
		wcplay->scoreEachGame.push_back(make_pair(gameName, 0));
	}
	for(auto x : wcplay->scoreEachGame)
	{
		CCPoint titlePos = position.TITLES[i];
		string filename = title2filename[x.first];
		CCSprite* title = CCSprite::spriteWithFile(filename.c_str());
		thiz->addChild(title, 2);
		title->setPosition(titlePos);
		
		i++;
	}
	
	
	i = 0;
	leftTotalScore = 0;
	rightTotalScore = 0;
	for(int i=0; i<wcplay->scoreEachGame.size() - 1; i++)
	{
		leftTotalScore += wcplay->scoreEachGame[i].second;
		rightTotalScore += wcplay->enemyScoreEachGame[i].second;
		
		CCLabelTTF* leftScoreTTF = CCLabelTTF::labelWithString(KS_Util::stringWithFormat("%d", wcplay->scoreEachGame[i].second).c_str(),
															   defaultFont, 13.f);
		leftScoreTTF->setColor(ccc3(0, 0, 0));
		CCPoint leftScorePos = position.LEFT_SCORES[i];
		leftScoreTTF->setPosition(leftScorePos);
		thiz->addChild(leftScoreTTF, 3);
		
		CCLabelTTF* rightScoreTTF = CCLabelTTF::labelWithString(KS_Util::stringWithFormat("%d", wcplay->enemyScoreEachGame[i].second).c_str(),
																defaultFont, 13.f);
		rightScoreTTF->setColor(ccc3(0, 0, 0));
		CCPoint rightScorePos = position.RIGHT_SCORES[i];
		rightScoreTTF->setPosition(rightScorePos);
		thiz->addChild(rightScoreTTF, 3);
	}
	
	leftScoreFnt = CCLabelTTF::labelWithString("",
											   defaultFont, 13.f);
	leftScoreFnt->setPosition(position.LEFT_SCORES[wcplay->scoreEachGame.size() - 1]);
	leftScoreFnt->setColor(ccc3(0, 0, 0));
	thiz->addChild(leftScoreFnt, 3);
	
	rightScoreFnt = CCLabelTTF::labelWithString("",
												defaultFont, 13.f);
	rightScoreFnt->setPosition(position.RIGHT_SCORES[wcplay->scoreEachGame.size() - 1]);
	rightScoreFnt->setColor(ccc3(0, 0, 0));
	thiz->addChild(rightScoreFnt, 3);
	
	leftTotalFnt = CCLabelTTF::labelWithString("", defaultFont, 13.f);
	leftTotalFnt->setPosition(position.LEFT_TOTAL_SCORE);
	leftTotalFnt->setColor(ccc3(0,0,0));
	thiz->addChild(leftTotalFnt, 3);
	
	rightTotalFnt = CCLabelTTF::labelWithString("", defaultFont, 13.f);
	rightTotalFnt->setPosition(position.RIGHT_TOTAL_SCORE);
	rightTotalFnt->setColor(ccc3(0,0,0));
	thiz->addChild(rightTotalFnt, 3);
}

