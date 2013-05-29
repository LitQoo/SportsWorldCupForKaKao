//
//  MyScore.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 8..
//
//

#ifndef __SportsWorldCup__MyScore__
#define __SportsWorldCup__MyScore__

#include "UIMediator.h"
#include "cocos2d.h"
#include "KS_Util.h"
#include "Bridge.h"
#include "NSDefault.h"
#include "KSoundEngine.h"

USING_NS_CC;


class MyScore : public CCLayer, public UIColleague
{
private:
	UIMediator* mediator;
	CCSprite* thiz;
public:
	MyScore();
	virtual ~MyScore();
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	static MyScore* node(UIMediator *m)
	{
		MyScore *pRet = new MyScore();
		pRet->setMediator(m);
		if (pRet && pRet->init())
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			delete pRet;
			pRet = NULL;
			return NULL;
		}
	}
	void setMediator(UIMediator* m)
	{
		mediator = m;
	}
	void showJudgeOnGameOver(string result, int rank, int score, int max, int weekly)
	{
		CCPoint medalPosition = ccp(50, 51);
		switch(rank)
		{
			case 1:
			{
				CCSprite* medal = CCSprite::spriteWithFile("main_gold.png");
				medal->setPosition(medalPosition);
				thiz->addChild(medal, 2);
				NSDefault::incGoldMedal();
				break;
			}
			case 2:
			{
				CCSprite* medal = CCSprite::spriteWithFile("main_silver.png");
				medal->setPosition(medalPosition);
				thiz->addChild(medal, 2);
				NSDefault::incSilverMedal();
				break;
			}
			case 3:
			{
				CCSprite* medal = CCSprite::spriteWithFile("main_bronze.png");
				medal->setPosition(medalPosition);
				thiz->addChild(medal, 2);
				NSDefault::incCopperMedal();
				break;
			}
			default:
			{
				if(4 <= rank && rank <= 10)
				{
					CCSprite* medal = CCSprite::spriteWithFile("main_top10.png");
					medal->setPosition(medalPosition);
					thiz->addChild(medal, 2);
					NSDefault::incTop10Medal();
				}
			}
		}
		
		if(1<=rank && rank <= 10)
		{
			KSoundEngine::sharedEngine()->playSound("getmedal.mp3");
		}
		{
			CCLabelBMFont* myscore = CCLabelBMFont::labelWithString(KS_Util::stringWithFormat("%d", score).c_str(), "gameover_score2.fnt");
			myscore->setPosition(ccp(99, 117));
			thiz->addChild(myscore, 2);
		}
		{
			CCLabelBMFont* myscore = CCLabelBMFont::labelWithString(KS_Util::stringWithFormat("%d", max).c_str(), "gameover_score1.fnt");
			myscore->setPosition(ccp(133, 71));
			thiz->addChild(myscore, 2);
		}
		{
			CCLabelBMFont* myscore = CCLabelBMFont::labelWithString(KS_Util::stringWithFormat("%d", weekly).c_str(), "gameover_score1.fnt");
			myscore->setPosition(ccp(132, 36));
			thiz->addChild(myscore, 2);
		}
		
		
		
		CCPoint position = ccp(100, 150);
		if(result == "max")
		{
			CCSprite* res = CCSprite::spriteWithFile("gameover_max.png");
			res->setPosition(position);
			thiz->addChild(res, 2);
			
			
		}
		else if(result == "weekly")
		{
			CCSprite* res = CCSprite::spriteWithFile("gameover_weekly.png");
			res->setPosition(position);
			thiz->addChild(res, 2);
		}
		else if(result == "myscore")
		{
			CCSprite* res = CCSprite::spriteWithFile("gameover_myscore.png");
			res->setPosition(position);
			thiz->addChild(res, 2);
		}
	}
};

#endif /* defined(__SportsWorldCup__MyScore__) */
