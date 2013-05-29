

#ifndef SportsWorldCup_BattleMiddle_cpp
#define SportsWorldCup_BattleMiddle_cpp

#include "cocos2d.h"
#include "JsonBox.h"
#include "HWFromTo.h"

USING_NS_CC;

#define SCENE_FUNC(_A) static CCScene* scene() { CCScene* scene = CCScene::node(); _A * layer = _A::node(); scene->addChild(layer); return scene; }
class BattleMiddle : public CCLayer, public CCTextFieldDelegate
{
private:
	FromTo leftGame;
	FromTo rightGame;
	CCLabelTTF* leftScoreFnt, *rightScoreFnt, *leftTotalFnt, *rightTotalFnt;
	CCMenuItem* _next, *_home;
	CCSprite* leftPlayer;
	CCSprite* rightPlayer;
	int leftTotalScore;
	int rightTotalScore;
private:
	void updateWeeklyAndMaxScore();
	void NEXT(CCObject*);
	void HOME(CCObject*);
	CCSprite* thiz;

	void GAMEOVERRETRY();
	void finishGetScores(JsonBox::Object js);
	CCSprite *titleBack, *title;



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
	virtual void update(ccTime dt);
	void startSchedule();
	void initGameList();
public:
	BattleMiddle();
	virtual ~BattleMiddle();
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	SCENE_FUNC(BattleMiddle);
	LAYER_NODE_FUNC(BattleMiddle);
};

#endif
