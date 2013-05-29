#include "BattleMiddle.h"
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
BattleMiddle::BattleMiddle()
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
}
BattleMiddle::~BattleMiddle()
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
bool BattleMiddle::init()
{
	CCLayer::init();
//	wcplay->gameScore = 3333; // 방금 얻은 점수 정의 //##
//	wcplay->scoreEachGame.push_back(make_pair("ak", 3333)); //##
//	wcplay->scoreEachGame.push_back(make_pair("bs", 3333)); //##
//	wcplay->scoreEachGame.push_back(make_pair("hw", 3333)); //##
	
	wcplay->enemyScoreEachGame.clear();
	wcplay->enemyScoreEachGame.push_back(make_pair("AK", 253000)); //##
	wcplay->enemyScoreEachGame.push_back(make_pair("BS", 523400)); //##
	wcplay->enemyScoreEachGame.push_back(make_pair("HW", 583900)); //##
	wcplay->enemyScoreEachGame.push_back(make_pair("AC", 485700)); //##

	
	leftGame.init(0, wcplay->gameScore.getVar(), wcplay->gameScore.getVar() / 60.f);
	int enemyScore = wcplay->enemyScoreEachGame[wcplay->scoreEachGame.size() - 1].second;
	rightGame.init(0, enemyScore, enemyScore / 60.f);

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

	thiz = CCSprite::spriteWithFile("match_round.png");
	thiz->setPosition(ccp(position.rankTable.x, position.rankTable.y - 400));
	addChild(thiz);
	
		
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
	
	leftPlayer = CCSprite::spriteWithFile("kim.png");
	leftPlayer->setPosition(position.LEFTPLAYER);
	thiz->addChild(leftPlayer, 2);
	
	
	rightPlayer = CCSprite::spriteWithFile("seo.png");
	rightPlayer->setPosition(position.RIGHTPLAYER);
	thiz->addChild(rightPlayer, 2);
	
	JsonBox::Object ee;
	finishGetScores(ee);
	
	return true;
}
void BattleMiddle::updateWeeklyAndMaxScore()
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
void BattleMiddle::update(ccTime dt)
{
	//	dt = 1.f/60.f;
	static int _i = 0;
	static int _ii = 0;
	_ii = !_ii;
	_i++;
	
	bool a = leftGame.step();
	bool b = rightGame.step();
	if(a == false && b == false)
	{
		_home->runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.3f), CCMoveTo::actionWithDuration(0.2f, position.home)));
		_next->runAction(CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.4f), CCMoveTo::actionWithDuration(0.2f, position.next)));
	}
	leftScoreFnt->setString(KS_Util::stringWithFormat("%d", (int)leftGame.getValue()).c_str());
	rightScoreFnt->setString(KS_Util::stringWithFormat("%d", (int)rightGame.getValue()).c_str());
	
	leftTotalFnt->setString(KS_Util::stringWithFormat("%d", (int)(leftGame.getValue() + leftTotalScore)).c_str());
	rightTotalFnt->setString(KS_Util::stringWithFormat("%d", (int)(rightGame.getValue() + rightTotalScore)).c_str());
}


void BattleMiddle::startSchedule()
{
	schedule(schedule_selector(ThisClassType::update), 0.117f / 2.f);
}
void BattleMiddle::finishGetScores(JsonBox::Object js)
{
	titleBack->runAction(CCMoveTo::actionWithDuration(0.5f, position.CUP));
	title->runAction(CCMoveTo::actionWithDuration(0.5f, position.CUP));
	
	thiz->runAction(CCMoveTo::actionWithDuration(0.5f, position.rankTable));
	
	runAction(CCCallFunc::actionWithTarget(this, callfunc_selector(ThisClassType::startSchedule)));
	
	
	
	
	
	initGameList();
}

void BattleMiddle::GAMEOVERRETRY()
{
	JsonBox::Object param;
	
	param["score"] = wcplay->accScore.getVar();
	param["isover"] = false;
	graphdog->command("getscores", &param, this, gd_selector(ThisClassType::finishGetScores));
}
void BattleMiddle::NEXT(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	string r = playInfo->getWorldcupGame();
	
	
	playInfo->popGame();
	if(r == "")
		CCLog("not next");
	else
		playInfo->initGameAndStart(r);
}

void BattleMiddle::HOME(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	wcplay->gameScore = 0;
	ReplaceSceneParam(SWIntro, INIT_MODE::BATTLE_GAMEOVER);
}

void BattleMiddle::onEnterTransitionDidFinish()
{
	CCLayer::onEnterTransitionDidFinish();
}
void BattleMiddle::onExit()
{
	
	
	CCLayer::onExit();
}

void BattleMiddle::initGameList()
{
	int i=0;
	map<string, string> title2filename;
	title2filename["AK"] = "title_clay.png";
	title2filename["BS"] = "title_basketball.png";
	title2filename["AC"] = "title_archery.png";
	title2filename["HW"] = "title_hurdle.png";
	title2filename["SK"] = "title_alpineski.png";
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

