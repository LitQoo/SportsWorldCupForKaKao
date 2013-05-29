//
//  Graphics.h
//  BasketWorldCup2
//
//  Created by ksoo k on 13. 1. 14..
//
//

#ifndef __BasketWorldCup2__Graphics__
#define __BasketWorldCup2__Graphics__
#include "cocos2d.h"
#include <vector>
#include <queue>
USING_NS_CC;
using namespace std;

class CCLabelTTFFactory
{
private:
	CCNode* parent;
	int depth;
	queue<CCLabelTTF*> ttfGarage;
public:
	void reserve(CCNode* _parent, int _depth, int n, int charSize, const char* font, float fs);
	CCLabelTTF* getCCLabelTTF();
};
class GameScreen;
struct Graphics
{
	GameScreen* gs;
	CCLabelBMFont* ateGoldFnt;
	//CCSpriteBatchNode* comboBatchNode;
	vector<CCSprite*> tracer;
//	CCLabelTTFFactory ttfFactory;
	CCLabelBMFont* scoreFnt;
	CCLabelBMFont* timeFnt;
//	CCSprite* miniRank;
	CCSprite* tipGuide;
	CCSpriteBatchNode* goalFrontBatchNode;
	CCSpriteBatchNode* cleanShotBatchNode;
	CCSpriteBatchNode* luckyBatchNode;
	CCSpriteBatchNode* coinBatchNode;
	CCSprite* goalFront;
	CCSpriteBatchNode* item_tracer;
	typedef CCSprite* pCCSprite;
	CCProgressTimer* zeroSprite;
	void initGraphics();
	void push_backTracer(const pCCSprite& _b) {tracer.push_back(_b);}
	Graphics(GameScreen* g){
		gs = g;
	}
	void preCreateMiniRank();
	void initBatchNode();
	void hideTracer();
	void goalInEffect(int score);
	enum {TRACER_NUMBER = 50};
	enum {GOALFRONT_WIDTH = 42, GOALFRONT_HEIGHT = 40};
	enum {SCREEN_WIDTH = 600, SCREEN_HEIGHT = 400};
//	enum {SCREEN_WIDTH = 480, SCREEN_HEIGHT = 320};
	enum {X_MIN = 240, X_MAX = SCREEN_WIDTH - 100, Y_MIN = 100, Y_MAX = SCREEN_HEIGHT - 150};
	enum {CLEAN_WIDTH=120, CLEAN_HEIGHT=120, LUCKY_WIDTH=120, LUCKY_HEIGHT=120};
	enum {COIN_WIDTH = 31, COIN_HEIGHT = 31}; // 31
	enum {COMBO_WIDTH = 70, COMBO_HEIGHT=45};
};


#endif /* defined(__BasketWorldCup2__Graphics__) */
