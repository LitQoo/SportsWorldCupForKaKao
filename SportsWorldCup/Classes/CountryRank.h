//
//  CountryRank.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 2. 28..
//
//

#ifndef __SportsWorldCup__CountryRank__
#define __SportsWorldCup__CountryRank__
#include "cocos2d.h"
#include "JsonBox.h"
#include "UIMediator.h"
USING_NS_CC;


class CountryRank : public CCSprite, public UIColleague
{
private:
	struct __POSITION{
		CCPoint user[10];
		CCPoint score[10];
		CCPoint flag[10];
	}position;
	CCSprite* thiz;
	UIMediator* mediator;
public:
	CountryRank()
	{
		position.user[0] = ccp(23, 220);
		position.user[1] = ccp(23, 203);
		position.user[2] = ccp(23, 186);
		position.user[3] = ccp(23, 169);
		position.user[4] = ccp(23, 152);
		position.user[5] = ccp(23, 135);
		position.user[6] = ccp(23, 118);
		position.user[7] = ccp(23, 101);
		position.user[8] = ccp(23, 84);
		position.user[9] = ccp(23, 67);
		
		position.score[0] = ccp(241, 220);
		position.score[1] = ccp(241, 203);
		position.score[2] = ccp(241, 186);
		position.score[3] = ccp(241, 169);
		position.score[4] = ccp(241, 152);
		position.score[5] = ccp(241, 135);
		position.score[6] = ccp(241, 118);
		position.score[7] = ccp(241, 101);
		position.score[8] = ccp(241, 84);
		position.score[9] = ccp(241, 67);
		
		position.flag[0] = ccp(132, 221);
		position.flag[1] = ccp(132, 204);
		position.flag[2] = ccp(132, 187);
		position.flag[3] = ccp(132, 170);
		position.flag[4] = ccp(132, 153);
		position.flag[5] = ccp(132, 136);
		position.flag[6] = ccp(132, 119);
		position.flag[7] = ccp(132, 102);
		position.flag[8] = ccp(132, 85);
		position.flag[9] = ccp(132, 68);
	}
	virtual ~CountryRank();
	virtual bool init();
	static CountryRank* node(UIMediator *m)
	{
		CountryRank *pRet = new CountryRank();
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
	
	void FACEBOOKRANK(CCObject*);
	void INVITEFRIEND(CCObject*);
};

#endif /* defined(__SportsWorldCup__CountryRank__) */
