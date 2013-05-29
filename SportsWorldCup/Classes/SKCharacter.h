//
//  SKCharacter.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 5. 10..
//
//

#ifndef __SportsWorldCup__SKCharacter__
#define __SportsWorldCup__SKCharacter__
#include "HWFromTo.h"
#include "AdvanceSprite.h"
class SKCharacter : public AdvanceSprite
{
private:
	
	CCAnimation* charAnimation;
	CCAnimation* rightAnimation;
	CCAnimation* leftAnimation;
	CCAnimation* leftBreakAnimation;
	CCAnimation* rightBreakAnimation;
	float charX;
	float charDistance;
	float prevCharX;
	float prevDistance;
	bool isFirstSetX;
	bool isFirstSetD;
	void createAnimation(CCAnimation* animation, string imgFile, int cols, int frames, int width_per_each, int height_per_each)
	{
		///	adHuman->add
		CCSprite* sprite = CCSprite::spriteWithFile(imgFile.c_str());
		for(int f = 1; f<=frames; f++) // 프레임 개수만큼.
		{
			int col = (f - 1 ) % cols;
			int row = (f - 1 ) / cols;
			animation->addFrameWithTexture(sprite->getTexture(), CCRectMake(width_per_each*col, height_per_each * row, width_per_each, height_per_each));
		}
	}
public:
	enum {GROUND, BOOSTING, DEBOOSTING}BOOSTSTATE; // 젤 아래, 앞으로 나아가는 상태, 유지, 다시 내려오는 상태.
	enum CRASHTYPE {LEFT_CRASH=-1, NO_CRASH = 0, RIGHT_CRASH=1} ;
	virtual ~SKCharacter()
	{
		delete charAnimation;
		delete rightAnimation;
		delete leftAnimation;
		delete leftBreakAnimation;
		delete rightBreakAnimation;
	}
	enum {LEFT=-1, NONE=0, RIGHT=1}MOVESTATE; // 이동 방향을 나타냄. 실제 그래픽과 비동적임.
	enum {LEFT_ACTION, STRAIGHT_ACTION, RIGHT_ACTION, ETC} ACTION_STATE; // 이동 방향에 따른 실제 그래픽이 표시되고 있는지 여부. 그래픽과 동기적임. ETC : 기타, 맞았을 때와 같은 상황
	enum {STUN=1, NORMAL=2, UNBEATABLE=3} PLAYERSTATE; // STUN : 맞아서 다리를 들고 있는 상태, NORMAL : 일반 진행, UNBEATABLE : 무적 상태
	FromToWithDuration boostEffect;
	float stunRemainTime;
	float unbeatableRemainTime;
	static float STUN_INIT_TIME; // 스턴 걸리는 시간
	static float UNBEATABLE_INIT_TIME; // 무적의 시간
	static float CHAR_BASEY; // 캐릭터가 실제 y 하단에서 얼마나 위로 떨어져 있는가.
	static float FRONT_DURATION; // 앞으로 나가기 까지의 시간
	static float BACK_DURATION; // 뒤로 들어가기 까지의 시간
	void toStun(enum CRASHTYPE ct){
		stunRemainTime = STUN_INIT_TIME;
		PLAYERSTATE = STUN;
		if(ct == CRASHTYPE::LEFT_CRASH)
		{
			leftStunAction();
			this->runAction(CCBlink::actionWithDuration(STUN_INIT_TIME + UNBEATABLE_INIT_TIME, 7));
		}else if(ct == CRASHTYPE::RIGHT_CRASH)
		{
			rightStunAction();
			this->runAction(CCBlink::actionWithDuration(STUN_INIT_TIME + UNBEATABLE_INIT_TIME, 7));
		}
		
	}
	void updateState(float dt){
		if(PLAYERSTATE == STUN)
		{
			stunRemainTime -= dt;
			if(stunRemainTime <= 0)
			{
				PLAYERSTATE = UNBEATABLE;
				unbeatableRemainTime = UNBEATABLE_INIT_TIME;
				goStraight();
			}
		}
		else if(PLAYERSTATE == UNBEATABLE)
		{
			unbeatableRemainTime -= dt;
			if(unbeatableRemainTime <= 0)
			{
				PLAYERSTATE = NORMAL;
			}
		}
	}
	SKCharacter()
	{
		MOVESTATE = NONE;
		ACTION_STATE = STRAIGHT_ACTION;
		PLAYERSTATE = NORMAL;
		isFirstSetD = isFirstSetX = true;
		
		BOOSTSTATE = GROUND;
		boostEffect.init(boostEffect.getValue(), 30.f, FRONT_DURATION);
	}
	
	virtual void createSprite();
	void render();
	void leftAction();
	void rightAction();
	void goStraight();
	void leftStunAction();
	void rightStunAction();
	
	void crashObstacleAction(enum CRASHTYPE ct);
	void setX(float x){
		prevCharX = charX;
		if(isFirstSetX)
			prevCharX = x;
		isFirstSetX = false;
		charX = x;
	}
	void setDistance(float d){
		prevDistance = charDistance;
		if(isFirstSetD)
			prevDistance = d;
		isFirstSetD = false;
		charDistance = d;
	}
	float getX(){return charX;}
	float getDistance(){return charDistance;}
	float getPrevX(){return prevCharX;}
	float getPrevDistance(){return prevDistance;}
};

#endif /* defined(__SportsWorldCup__SKCharacter__) */
