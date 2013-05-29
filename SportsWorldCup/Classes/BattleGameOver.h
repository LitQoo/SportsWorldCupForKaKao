#ifndef SportsWorldCup_BattleGameOver_cpp
#define SportsWorldCup_BattleGameOver_cpp

#include "cocos2d.h"
#include "JsonBox.h"
#include "UIMediator.h"
#include "HWFromTo.h"
USING_NS_CC;


#define PUT_SCORE(CLASS, SCORE, WEEK, RANK) do{\
string res; \
if(CLASS::getMaxScore() < SCORE)\
{ \
CLASS::setMaxScore(SCORE); \
CLASS::setWeeklyMaxScore(WEEK, SCORE); \
res = "max"; \
} \
else \
{\
if(WEEK != CLASS::getWeeklyMaxScore().first)\
{\
CLASS::setWeeklyMaxScore(WEEK, SCORE);\
res = "weekly";\
}\
else\
{\
if(CLASS::getWeeklyMaxScore().second < myscore)\
{ \
CLASS::setWeeklyMaxScore(WEEK, SCORE);\
res = "weekly";\
}\
else\
{\
res = "myscore";\
}\
}\
}\
mediator->showJudgeOnGameOver(res, RANK, SCORE, CLASS::getMaxScore(), CLASS::getWeeklyMaxScore().second); \
}while(0)

#define SCENE_FUNC(_A) static CCScene* scene() { CCScene* scene = CCScene::node(); _A * layer = _A::node(); scene->addChild(layer); return scene; }
class BattleGameOver : public CCSprite, public UIColleague
{
private:
	string winner;
	FromTo leftGame;
	FromTo rightGame;
	CCLabelTTF* leftScoreFnt, *rightScoreFnt, *leftTotalFnt, *rightTotalFnt;
	CCMenuItem* _next, *_home;
	CCSprite* leftPlayer;
	CCSprite* rightPlayer;
	int leftTotalScore;
	int rightTotalScore;
	bool isJudged;
	struct __POSITION{
		CCPoint TITLES[4];
		
		CCPoint LEFTPLAYER;
		CCPoint LEFT_TOTAL_SCORE;
		CCPoint LEFT_SCORES[4];
		
		CCPoint RIGHTPLAYER;
		CCPoint RIGHT_TOTAL_SCORE;
		CCPoint RIGHT_SCORES[4];
		
		CCPoint CUP;
		CCPoint rankTable;
		CCPoint home;
		CCPoint next;
	}position;
	CCSprite* thiz;
	UIMediator* mediator;
	void update(ccTime dt);
	
	void initGameList();
public:
	int beforeEXP;
	int afterEXP;
	BattleGameOver();
	virtual ~BattleGameOver();
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	static BattleGameOver* node(UIMediator *m)
	{
		BattleGameOver *pRet = new BattleGameOver();
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
	void appearMe();
	
public:
	void finishGetScores(JsonBox::Object js);
	enum {WIN=0, LOSE=1};
	void finishScoreCounting(void* obj);
};

#endif
