//
//  GameScreen.cpp
//  BasketWorldCup2
//
//  Created by ksoo k on 12. 11. 6..
//  Copyright (c) 2012년 LitQoo Inc. All rights reserved.
//
#include "BS2GameScreen.h"
#include "GameSystem.h"
#include "BS2Coin.h"
#include <algorithm>
#include "NSDefault.h"
#include "LoadingScene.h"

#include "WCPlay.h"
#include "BS2BasketGame.h"
#include <vector>


#include "BS2Geometry.h"
#include "CCParticleSystemPoint.h"
#include "BS2Play.h"
#include "BS2SaveData.h"
#include "KSoundEngine.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	#import "Bridge_ios.h"
#endif
#include "SWIntro.h"
#include "GameOver.h"
#include "Bridge.h"
const float Turn::DOUBLE_SHOT_COOLTIME = 1.5f;
const float Turn::VALID_MOVE_TIME = 0.2f;
const float Turn::FAST_SHOT_TIME = 0.1f;
const float Turn::DELAY_JUMP_TO_ROUND = 5.f;


static GameScreen* gs;
static void eachShape(void *ptr, void* unused)
{
	cpShape *shape = (cpShape*) ptr;
	ChipSprites* chipSprites = (ChipSprites*)shape->data;
	if( chipSprites )
	{
		cpBody *body = shape->body;
		
		// TIP: cocos2d and chipmunk uses the same struct to store it's position
		// chipmunk uses: cpVect, and cocos2d uses CGPoint but in reality the are the same
		// since v0.7.1 you can mix them if you want.
		cpVect v = cpv(0, 0);
		if(shape->klass->type == cpShapeType::CP_CIRCLE_SHAPE)
		{
			v = cpCircleShapeGetOffset(shape);
		}
		
		chipSprites->setPosition(CCPointMake(body->p.x + v.x, body->p.y + v.y));
		chipSprites->setRotation((float) CC_RADIANS_TO_DEGREES( -body->a ));
	}
}
static void eatCoin(cpSpace *space, void *obj, void *data)
{
	GameScreen* gs = (GameScreen*)data;
	gs->eatCoin((cpShape*)obj);
}

void GameScreen::eatCoin(cpShape* shape)
{
	KSoundEngine::sharedEngine()->playSound("se_takegold01.mp3");
	int p = coins[shape].first.getPrice();
	cpVect coinPosition = shape->body->p;
	NSDefault::setGold(NSDefault::getGold() + p);
	info.ateGoldCount+=p;
	playInfo->__ateCoin = info.ateGoldCount;
	graphics.ateGoldFnt->setString(KS_Util::stringWithFormat("%d", info.ateGoldCount).c_str());
	{
		auto retainAnimation = SceneUtil::playAnimation("coin.png", 0.07f, 6, 6, Graphics::COIN_WIDTH, Graphics::COIN_HEIGHT, false);
		addChild(retainAnimation.second, zorder::EAT_COIN_EFF);
		ccBezierConfig bc;
		bc.controlPoint_1 = retainAnimation.second->getPosition();
		bc.controlPoint_2 = ccp(700, 100);
		bc.endPosition = ccp(520, 10);
		auto moveAction = CCBezierTo::actionWithDuration(1.2f, bc);
		auto action2 = CCSpawn::actions(retainAnimation.first, moveAction, CCScaleTo::actionWithDuration(0.8f, 0.5f)/*, CCFadeOut::actionWithDuration(1.2f)*/, 0); // 가면서 사라짐.
		auto action3 = CCCallFuncN::actionWithTarget(this, callfuncN_selector(KSBaseScene::deleteSprite));
		auto totalAction = CCSequence::actionOneTwo(action2, action3);
		retainAnimation.second->setPosition(ccp(coinPosition.x, coinPosition.y));
		retainAnimation.second->runAction(totalAction);
	}
	pBasketGame->eatCoin(p);
	
	coins.erase(shape);
#if STATIC_OBJECT == 1
	cpSpaceRemoveStaticShape(space, shape);
	cpSpaceRemoveBody(space, shape->body);
	cpBodyFree(shape->body);
	cpShapeFree(shape);
#else
	cpSpaceRemoveShape(space, shape);
	cpBodyFree(shape->body);
	cpShapeFree(shape);
#endif
	
	ChipSprites* coinSprites = (ChipSprites*)shape->data;
	coinSprites->removeAllNode();
}

void GameScreen::removeCoin(cpShape* shape)
{
	coins.erase(shape);
#if STATIC_OBJECT == 1
	cpSpaceRemoveStaticShape(space, shape);
	cpSpaceRemoveBody(space, shape->body);
	cpBodyFree(shape->body);
	cpShapeFree(shape);
#else
	cpSpaceRemoveShape(space, shape);
	cpBodyFree(shape->body);
	cpShapeFree(shape);
#endif
	
	ChipSprites* coinSprites = (ChipSprites*)shape->data;
	coinSprites->removeAllNode();
}
void GameScreen::removeAllCoin()
{
	for(auto iter = coins.begin(); iter != coins.end();)
	{
		cpShape* shape = iter->first;
		
		coins.erase(iter++);
#if STATIC_OBJECT == 1
		cpSpaceRemoveStaticShape(space, shape);
		cpSpaceRemoveBody(space, shape->body);
		cpBodyFree(shape->body);
		cpShapeFree(shape);
#else
		cpSpaceRemoveShape(space, shape);
		cpBodyFree(shape->body);
		cpShapeFree(shape);
#endif
		
		ChipSprites* coinSprites = (ChipSprites*)shape->data;
		coinSprites->removeAllNode();
	}

}
static void removeBall(cpSpace *space, void *obj, void *data)
{
	GameScreen* gs = (GameScreen*)data;
	gs->removeBall((cpShape*)obj);
}
static void excuteOnExitZeroArea(cpSpace *space, void *obj, void *data)
{
	GameScreen* gs = (GameScreen*)data;
	//gs->onExitZeroArea();
}
void GameScreen::removeBall(cpShape* shape)
{
	ChipSprites* a_chip = (ChipSprites*)(shape->data);
	int* ud = (int*)(a_chip->getReader()->getUserData());
	//static int ii = 2;
	if(ud != (int*)ChipSprites::REMOVING)
	{
		a_chip->getReader()->setUserData((void*)ChipSprites::REMOVING);
		auto seq = CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(2.f),
											CCCallFuncND::actionWithTarget(this, callfuncND_selector(GameScreen::removeBall_2), shape));
		
		runAction(seq);
	}
}

void GameScreen::removeBall_2(CCNode* node, void* data)
{
	cpShape* shape = (cpShape*)data;
	cpBody* body = shape->body;
	ChipSprites* a_chip = (ChipSprites*)shape->data;
	
	currentBalls.erase(shape);
	cpSpaceRemoveShape(space, shape);
	cpShapeFree(shape);
	
	cpSpaceRemoveBody(space, body);
	cpBodyFree(body);
	
	CCParticleSystemPoint* part = CCParticleSystemPoint::particleWithFile("balldisappeareffect.plist");
	part->setIsAutoRemoveOnFinish(true);
	part->setPosition(a_chip->getReader()->getPosition());
	addChild(part, zorder::BALL_HIDE_EFF);
	a_chip->removeAllNode();
}
static cpBool defaultCollisionFunc(cpArbiter *arb, cpSpace *space, void *data)
{
	cpShape* a, *b;
	cpArbiterGetShapes(arb, &a, &b);
	//CCLog("a : %d, b : %d", a->collision_type, b->collision_type);
	if(b->collision_type == GameScreen::COIN_COLLISION)
		return false; // 내가 처리
	return true;
}

static cpBool collisionFunc(cpArbiter *arb, cpSpace *space, void *data)
{
	cpShape* a, *b;
	cpArbiterGetShapes(arb, &a, &b);
	//CCLog("zz %d %d", a->collision_type, b->collision_type);
	if(a->collision_type == GameScreen::DYNAMIC_BALL_COLLISION_TYPE && b->collision_type == GameScreen::DYNAMIC_BALL_COLLISION_TYPE)
	{
		map<cpShape*, BallBase>* pCurrentBalls = (map<cpShape*, BallBase>*)data;
		if(pCurrentBalls->find(a) != pCurrentBalls->end() && pCurrentBalls->find(b) != pCurrentBalls->end()) // found
		{
			if((*pCurrentBalls)[a].getCrashable() && (*pCurrentBalls)[b].getCrashable())
			{
				
			}
			else
				return false;
		}
	}
	if(a->collision_type == GameScreen::DYNAMIC_BALL_COLLISION_TYPE && b->collision_type == GameScreen::STATIC_BALL_COLLISION_TYPE)
		return false;
	
	// 골대와 춤돌
	if(a->collision_type == GameScreen::DYNAMIC_BALL_COLLISION_TYPE && b->collision_type == GameScreen::GOAL_POST_COLLISION)
	{
		KSoundEngine::sharedEngine()->playSound("se_backboard01.mp3");
		map<cpShape*, BallBase>* pCurrentBalls = (map<cpShape*, BallBase>*)data;
		if(pCurrentBalls->find(a) != pCurrentBalls->end()) // found
		{
			(*pCurrentBalls)[a].setReflected(1);
		}
	}
	// 링과 공이 충돌
	if(a->collision_type == GameScreen::DYNAMIC_BALL_COLLISION_TYPE && b->collision_type == GameScreen::GOAL_RINGPOST_COLLISION)
	{
		KSoundEngine::sharedEngine()->playSound("se_rim02.mp3");
		map<cpShape*, BallBase>* pCurrentBalls = (map<cpShape*, BallBase>*)data;
		if(pCurrentBalls->find(a) != pCurrentBalls->end()) // found
		{
			(*pCurrentBalls)[a].setReflected(1);
		}
	}

	// 땅과 충돌!
	if(a->collision_type == GameScreen::DYNAMIC_BALL_COLLISION_TYPE && b->collision_type == GameScreen::GROUND_COLLISION)
	{
		map<cpShape*, BallBase>* pCurrentBalls = (map<cpShape*, BallBase>*)data;
		if(pCurrentBalls->find(a) != pCurrentBalls->end()) // found
		{
			
			ChipSprites* cs = (ChipSprites*)a->data;
			int target = 3;
			// target 의 사이즈가 될 때 까지 제거함. 공에 붙은 각종 효과를 제거하는 것.
			while(cs && cs->sprites.size() > target)
			{
				CCNode* _n = cs->sprites[target].first;
//				_n->stopAllActions();
				
				cs->sprites.erase(cs->sprites.begin() + target);
				_n->getParent()->removeChild(_n, true);
			}
			// 땅에 부딪혔는데, 골인 상태가 아니라면 콤보 초기화 시킴.
			if( (*pCurrentBalls)[a].getGoaled() == false && (*pCurrentBalls)[a].getGrounding() == false)
			{
				gs->onNoGoal(a);
				if(gs->getGameMode() == BS2GameInformation::ZERO)
				{
					//GameScreen* gs = (GameScreen*)(cs->getReader()->getParent());
					cpSpaceAddPostStepCallback(space, excuteOnExitZeroArea, a, gs);
				}
			}
			(*pCurrentBalls)[a].setGrounding();
			if((*pCurrentBalls)[a].getGroundCount() >= 2)
			{
				//GameScreen* gs = (GameScreen*)(cs->getReader()->getParent());
				cpSpaceAddPostStepCallback(space, removeBall, a, gs);
			}
			
			
			//a_chip->sprites.
			KSoundEngine::sharedEngine()->playSound("bounce.mp3");
		}
		
	}
	
	// 공과 코인과 충돌
	if(a->collision_type == GameScreen::DYNAMIC_BALL_COLLISION_TYPE && b->collision_type == GameScreen::COIN_COLLISION)
	{
		//CCLog("__ %d %d", a->collision_type, b->collision_type);
		//GameScreen* gs = (GameScreen*)data;
		map<cpShape*, pair<Coin,int>>* pCurrentCoins = &gs->getCoins();
	
		if(pCurrentCoins->find(b) != pCurrentCoins->end()) // found
		{
			if(gs->getBalls().find(a) != gs->getBalls().end())
			{
				if(gs->getBalls()[a].getGrounding() == false)
				{
					cpSpaceAddPostStepCallback(space, eatCoin, b, gs);
				}
			}
			else
			{
//				CCLog("this is warning! %x", a );
			}
			return false;
			//(*pCurrentBalls)[arb->a].setGrounding();
		}
	}
	
//	// 코인과 코인 충돌.
//	if(a->collision_type == GameScreen::COIN_COLLISION && b->collision_type == GameScreen::COIN_COLLISION)
//	{
//		return false;
//	}
	
	return true;
	//CCLog("a type %d b type %d", arb->a->collision_type, arb->b->collision_type);
}



GameScreen::GameScreen() : info(this), graphics(this)
{
	gs = this;
	info.setBalance();
	gameTimer = 0.f;
	
	turn.doubleShotWaiting = false;
	turn.isMovingBall = false;
	turn.isUsingItem = false;
	

	{
		if(playInfo->selectedItems["item1"])
		{
			info.onceTipIn = true;
		}
	}
	//isChangingScene = false;
	
	
	
	KS::KSLog("% %d", __FUNCTION__, __LINE__);
}
GameScreen::~GameScreen()
{

	cpSpaceRemoveCollisionHandler(space, GameScreen::DYNAMIC_BALL_COLLISION_TYPE, STATIC_BALL_COLLISION_TYPE);
	cpSpaceRemoveCollisionHandler(space, GameScreen::DYNAMIC_BALL_COLLISION_TYPE, GameScreen::DYNAMIC_BALL_COLLISION_TYPE);
	cpSpaceRemoveCollisionHandler(space, GameScreen::DYNAMIC_BALL_COLLISION_TYPE, GOAL_POST_COLLISION);
	cpSpaceRemoveCollisionHandler(space, GameScreen::DYNAMIC_BALL_COLLISION_TYPE, GOAL_RINGPOST_COLLISION);
//	cpSpaceRemoveCollisionHandler(space, 0, IGNORE);
	cpSpaceRemoveCollisionHandler(space, GameScreen::DYNAMIC_BALL_COLLISION_TYPE, GROUND_COLLISION);
	cpSpaceRemoveCollisionHandler(space, GameScreen::DYNAMIC_BALL_COLLISION_TYPE, COIN_COLLISION);
	//cpSpaceRemoveCollisionHandler(space, COIN_COLLISION, COIN_COLLISION);
	cpSpaceFreeChildren(space);
	cpSpaceFree(space);
	CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
	chipSpritesFactory.clear();
	
	URLDelegator::getInstance()->removeTarget(this);
	InAppDelegator::getInstance()->removeTarget(this);
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	[[KSWebData ksWebData] clearTarget];
#endif
	KS::KSLog("~GameScreen");
}


void GameScreen::movingPostProc()
{
	if(info.movingPost == false)
		return;
	float MAX_H = 250;
	float MIN_H = 130;
	
	
	float _speed = MIN(0.7f, 0.2f + (info.postSpeed) * 0.05f);
	cpVect postV;
	if(info.postDirection < 0) // 내려
	{
		postV = cpv(0, -_speed);
	}
	else if(info.postDirection > 0) // 올라
	{
		postV = cpv(0, _speed);
	}
	
	
	r1.setPosition( cpvadd(r1.getPosition(), postV));
	if(r1.getPosition().y < MIN_H)
	{
		r1.setPosition( cpv(r1.getPosition().x, MIN_H) );
		info.postDirection = 1;
	}
	if(r1.getPosition().y > MAX_H)
	{
		r1.setPosition( cpv(r1.getPosition().x, MAX_H) );
		info.postDirection = -1;
	}
	
	
//	r1.setPosition( cpvadd(r1.getPosition(), info.postDirection));
//	if(info.postDirection.y < 0)
//	{
//		
//	}
//	else if(info.postDirection.y > 0)
//	{
//		if(r1.getPosition().y > MAX_H)
//		{
//			r1.setPosition( cpv(r1.getPosition().x, MAX_H) );
//			info.postDirection = cpv(0, -_speed);
//		}
//	}
}
bool GameScreen::init()
{
	CCLayerColor::initWithColor(ccc4f(255, 255, 255, 0));
	
	playInfo->__ateCoin = 0;
	setAnchorPoint(ccp(0, 0));
	setPosition(ccp(0, 0));
	setScale(1.f/1.23f);
//	setScale(Graphics::SCREEN_WIDTH / 480.f);
	graphics.initBatchNode();
	graphics.initGraphics();
	graphics.preCreateMiniRank();
	cpInitChipmunk();
	cpBody *staticBody = cpBodyNew(INFINITY, INFINITY);
	space = cpSpaceNew();
	///
//	cDisplayer = ChipmunkDisplayer::node();
//	cDisplayer->mySpace = space;
//	addChild(cDisplayer, 2);
	///
	//space->damping = 1.0f;
	cpSpaceSetDefaultCollisionHandler(space, defaultCollisionFunc, 0, 0, 0, 0);
	cpSpaceAddCollisionHandler(space, GameScreen::DYNAMIC_BALL_COLLISION_TYPE, STATIC_BALL_COLLISION_TYPE, &collisionFunc, NULL, NULL, NULL, NULL);
	cpSpaceAddCollisionHandler(space, GameScreen::DYNAMIC_BALL_COLLISION_TYPE, GameScreen::DYNAMIC_BALL_COLLISION_TYPE, &collisionFunc, NULL, NULL, NULL, &currentBalls);
	cpSpaceAddCollisionHandler(space, GameScreen::DYNAMIC_BALL_COLLISION_TYPE, GOAL_POST_COLLISION, &collisionFunc, NULL, NULL, NULL, &currentBalls);
	cpSpaceAddCollisionHandler(space, GameScreen::DYNAMIC_BALL_COLLISION_TYPE, GOAL_RINGPOST_COLLISION, &collisionFunc, NULL, NULL, NULL, &currentBalls);
	//cpSpaceAddCollisionHandler(space, 0, IGNORE, &collisionFunc, NULL, NULL, NULL, &currentBalls);
	cpSpaceAddCollisionHandler(space, GameScreen::DYNAMIC_BALL_COLLISION_TYPE, GROUND_COLLISION, &collisionFunc, NULL, NULL, NULL, &currentBalls);
	cpSpaceAddCollisionHandler(space, GameScreen::DYNAMIC_BALL_COLLISION_TYPE, COIN_COLLISION, &collisionFunc, NULL, NULL, NULL, this);
	//cpSpaceAddCollisionHandler(space, COIN_COLLISION, COIN_COLLISION, &collisionFunc, NULL, NULL, NULL, 0);
	
	cpSpaceResizeStaticHash(space, 400.0f, 40);
	cpSpaceResizeActiveHash(space, 100, 600);
	
	space->gravity = cpv(0, BS2GameInformation::GRAVITY);
	space->elasticIterations = space->iterations;

	cpShape *shape;
	CCSize wins = CCSizeMake(Graphics::SCREEN_WIDTH, Graphics::SCREEN_HEIGHT);

	// bottom
	shape = cpSegmentShapeNew(staticBody, cpv(0, BS2GameInformation::GROUND_HEIGHT), cpv(wins.width, BS2GameInformation::GROUND_HEIGHT), 5.0f);
	
	shape->e = 0.98f; shape->u = 2.0f;
	
	shape->collision_type = GROUND_COLLISION;
	cpSpaceAddStaticShape(space, shape);
	
	
	// left
	shape = cpSegmentShapeNew(staticBody, cpv(0,0), cpv(0,wins.height), 5.0f);
//	CCLog("left wall %x", shape);
	shape->e = .95f; shape->u = 0.1f;
	cpSpaceAddStaticShape(space, shape);
	
	// right
	shape = cpSegmentShapeNew(staticBody, cpv(wins.width,0), cpv(wins.width,wins.height), 5.0f);
	shape->e = .95f; shape->u = 0.1f;
	cpSpaceAddStaticShape(space, shape);
	initBack();
	initRing();
 	CCPoint ballPos = selectBallPosition();
	createBall(ballPos);
	//createCoin( (r1.getBackBoardBodyPos().x + ballPos.x) / 2.f);
	step(1.f/50.f);
	
	CCSprite* ui_gold = CCSprite::spriteWithFile("ui_gold.png");
	ui_gold->setPosition(ccp(543, 19));
	addChild(ui_gold, 10);
	
	graphics.ateGoldFnt = CCLabelBMFont::labelWithString("0", "main1.fnt");
	graphics.ateGoldFnt->setPosition(ccp(543 + 9, 19));
	addChild(graphics.ateGoldFnt, 11);
	schedule(schedule_selector(GameScreen::tickSound), 1.0f);
	return true;
}

void GameScreen::startGame()
{
	schedule(schedule_selector(GameScreen::update));
}
void GameScreen::retainCoinAnimation()
{
	
}

void GameScreen::createCoin(int xPos)
{
	// 배치노드, 딜레이 기간, 가로개수, 프레임수, 가로크기, 세로 크기, 무한반복여부, 리턴값은 스프라이트포인터
	
	Coin _c(1);
	auto coinAnimation = SceneUtil::playAnimation("coin.png", 0.07f, 6, 6, Graphics::COIN_WIDTH, Graphics::COIN_HEIGHT, true);
	coinAnimation.second->runAction(coinAnimation.first);
	addChild(coinAnimation.second, zorder::COIN);
	CCParticleSystemPoint* part = CCParticleSystemPoint::particleWithFile("moneyeffect.plist");
	addChild(part);
	
	ChipSprites* chip = chipSpritesFactory.create();
	chip->addCCNodeWithFlag(coinAnimation.second);
	chip->addCCNodeWithFlag(part);
	_c.coinShape->data = chip;
	coins[_c.coinShape].first = _c;
	_c.setPosition(ccp(xPos, 320));
	_c.attachMe(space);
}
cpShape* GameScreen::createCoin(int xPos, int yPos, int group)
{
	Coin _c(1);
	
	auto coinAnimation = SceneUtil::playAnimation(graphics.coinBatchNode, 0.07f, 6, 6, Graphics::COIN_WIDTH, Graphics::COIN_HEIGHT, true);
	coinAnimation.second->runAction(coinAnimation.first);
	//CCParticleSystemPoint* part = CCParticleSystemPoint::particleWithFile("moneyeffect.plist");
	//addChild(part);
	ChipSprites* chip = chipSpritesFactory.create();
	chip->addCCNodeWithFlag(coinAnimation.second);
	//chip->addCCNodeWithFlag(part);
	_c.coinShape->data = chip;
	coins[_c.coinShape] = pair<Coin, int>(_c, group);
	_c.setPosition(ccp(xPos, yPos));
	_c.attachMe(space);
	return _c.coinShape;
}

void GameScreen::initRing()
{
	{
		r1.init(BS2GameInformation::BACKBOARD_LEN, BS2GameInformation::GROUND_LEN, BS2GameInformation::RING_SIZE,
				BS2GameInformation::POST_E, BS2GameInformation::POST_U);
		r1.getBackBoardShape()->collision_type = GOAL_POST_COLLISION;
		r1.getRing1()->collision_type = GOAL_RINGPOST_COLLISION;
		r1.getRing2()->collision_type = GOAL_RINGPOST_COLLISION;
		r1.getRing3()->collision_type = STATIC_BALL_COLLISION_TYPE;
		r1.getGround()->collision_type = GOAL_POST_COLLISION;
		
		CCSprite* post = CCSprite::spriteWithFile("goalback.png");
		ChipSprites* chip = chipSpritesFactory.create();
		chip->addCCNodeWithFlag(post);
		
		graphics.tipGuide = CCSprite::spriteWithFile("tipguide.png");
		graphics.tipGuide->setPosition(ccp(108, 131));
		post->addChild(graphics.tipGuide, 2);
		graphics.tipGuide->setOpacity(0);
		
		r1.getGround()->data = chip;
		addChild(post, zorder::POST);
		r1.attachMe(space);
		
		ChipSprites* chipFront = chipSpritesFactory.create();
		chipFront->addCCNodeWithFlag(graphics.goalFront);
		r1.getRing3()->data = chipFront;
		graphics.goalFrontBatchNode->addChild(graphics.goalFront);
		
	}
}
void GameScreen::drawShadow()
{
	for(auto iter = currentBalls.begin(); iter != currentBalls.end(); ++iter)
	{
		ChipSprites* cs = iter->second.getChipSprites();
		if(cs && cs->sprites.size() >= 3)
		{
			CCNode* shadow = cs->getObjFromIndex(2);
			CCPoint readerPos = cs->getReader()->getPosition();
			shadow->setPosition(ccp(readerPos.x, BS2GameInformation::GROUND_HEIGHT + 4));
		}
		
	}
}
void GameScreen::update(ccTime dt)
{
	dt = 1.f/60.f;
	gameTimer += dt;
	step(1.f/50.f);
	drawShadow();
	checkGoal();
	//createCoinsProc(dt);
	//moveCoins(dt);
	//removeCoins();
	doubleShot();
	sceneChanger();
	timeProcess(dt);
	judgeCleanShot();
	movingPostProc();
	pBasketGame->updateScoreFnt(dt);
	if(info.gameMode == BS2GameInformation::ZERO)
	{
		pBasketGame->incFever(BS2GameInformation::ZERO_DEC_VALUE);
		if(pBasketGame->getFever() <= 0.f)
		{
			onExitZeroArea();
		}
	}
	
//
	
}
void GameScreen::timeProcess(ccTime dt)
{
	if(info.gameMode != BS2GameInformation::ZERO)
		info.remainTime -= dt;
	if(info.remainTime <= 10 && !info.in10s)
	{
		KSoundEngine::sharedEngine()->playSound("hurryup.mp3");
		info.in10s = true;
		pBasketGame->setTimeWarning();
	}
	if(info.remainTime >= 0.f)
	{
		pBasketGame->setRemainTime(info.remainTime);
	}
	// 시간 초과되면 아래 블록이 딱 한번 실행됨
	else 
	{

		{
			if(playInfo->selectedItems["time"])
			{
				pBasketGame->setBonusTime();
			}
		}
		if(info.remainTime <= -info.bonusTime && info.isTimeOver == false)
		{
			setIsTouchEnabled(false);
			pBasketGame->setIsTouchEnabled(false);
			info.isTimeOver = true;
			
			if(getPosition().x != 0 || getPosition().y != 0)
			{
				auto _r = CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.7f), CCMoveTo::actionWithDuration(0.5f, ccp(0, 0)));
				_r->setTag(DOWNSCROLL);
				runAction(_r);
			}
			//		pBasketGame->dec_ball_timeover();
			setIsTouchEnabled(false);
			pBasketGame->timeOver();
			
		}
		
	}
	

}
void GameScreen::tickSound(ccTime dt)
{
	if(info.remainTime < 11.f)
	{
		// KSoundEngine::sharedEngine()->playSound("se_timewarning.mp3"); // tick tock 소리. time
	}
	
}

void GameScreen::finishTimeOver()
{
	
	graphdog->command("getweek", 0, this, gd_selector(ThisClassType::finishGetWeek));
}
void GameScreen::checkGoal()
{
	cpCircleShape* Ring1 = (cpCircleShape*)r1.getRing1();
	cpCircleShape* Ring2 = (cpCircleShape*)r1.getRing2();
	CCPoint A, B, a, b;
	
	A = {r1.getBackBoardBodyPos().x, r1.getBackBoardBodyPos().y};
	A = ccpAdd(A, {Ring1->c.x, Ring1->c.y});

	B = {r1.getBackBoardBodyPos().x, r1.getBackBoardBodyPos().y};
	B = ccpAdd(B, {Ring2->c.x, Ring2->c.y});
	
	a = {r1.getPrevBackBoardBodyPos().x, r1.getPrevBackBoardBodyPos().y};
	a = ccpAdd(a, {Ring1->c.x, Ring1->c.y});
	
	b = {r1.getPrevBackBoardBodyPos().x, r1.getPrevBackBoardBodyPos().y};
	b = ccpAdd(b, {Ring2->c.x, Ring2->c.y});
	
	vector<CCPoint> polygon;
	polygon.push_back(A);
	polygon.push_back(B);
	polygon.push_back(b);
	polygon.push_back(a);
	
	float ringAngle = atan2f(ccpSub(B, A).y, ccpSub(B, A).x);
	ringAngle += -90;
	
	
	for(auto iter = currentBalls.begin(); iter != currentBalls.end(); ++iter)
	{
		if(iter->second.getGrounding() && iter->second.getShoot() == false && iter->second.getGoaled() == false)
			continue;
		CCPoint C, D;
		
		C = {iter->second.previousPosition.x, iter->second.previousPosition.y};
		D = {iter->second.getShape(	)->body->p.x, iter->second.getShape()->body->p.y};
		float ballAngle = atan2f(ccpSub(D, C).y, ccpSub(D, C).x);
		ballAngle = CC_RADIANS_TO_DEGREES(ballAngle);

		if(Geometry::lineInPolygon(C, D, polygon) && iter->second.getGoaled() == false && fabsf(ballAngle - ringAngle) < 90.f)
		{
			onGoal(iter->second);
			////////////////////////////////////////////////////////
		}
		iter->second.previousPosition = {iter->second.getShape()->body->p.x, iter->second.getShape()->body->p.y};
	}
}

void GameScreen::onNoGoal(cpShape* shape)
{
	bool shieldMode = false;

	{
		if(playInfo->selectedItems["shield"] == true && info.usedShield == false)
		{
			info.usedShield = true;
			shieldMode = true;
			KSoundEngine::sharedEngine()->playSound("shield.mp3");
			CCSprite* shield;
			auto shieldMator = SceneUtil::playAnimation("shield.png", 0.07, 4, 8, 160, 160, 2);
			shield = shieldMator.second;
			shield->runAction(CCSequence::actionOneTwo(shieldMator.first, CCCallFuncN::actionWithTarget(this, callfuncN_selector(ThisClassType::deleteSprite))));
			shield->setPosition(ccp(240, 200));
			addChild(shield, 10000);
		}
	}
	if(shieldMode == false)
	{
		if(info.gameMode == BS2GameInformation::ZERO)
		{
			pBasketGame->incFever(BS2GameInformation::NOGOAL_DEC_VALUE_IN_ZERO);
		}
		else
		{
			pBasketGame->incFever(BS2GameInformation::NOGOAL_DEC_VALUE_IN_NORMAL);
		}
		
		bs2play->combo = 0;
		KSoundEngine::sharedEngine()->playSound("uhuh.mp3");
//		info.postSpeed = 0; //##
		info.seqFailCounter++;
		
		if(info.seqFailCounter >= 3)
		{
			info.seqFailCounter = 0;
			
			CCSprite* tipbox = CCSprite::spriteWithFile("tipbox.png");
			CCLabelTTF* desc = CCLabelTTF::labelWithString(ML::key("usebackboard").c_str(), defaultFont, 14.f);
			
			desc->setPosition(ccp(111, 23));
			tipbox->addChild(desc, 2);
			
			tipbox->setPosition(ccp(300, 310));
			tipbox->setOpacity(0);
			addChild(tipbox, zorder::TIPBOX);
			
			graphics.tipGuide->runAction(CCSequence::actions(CCFadeIn::actionWithDuration(0.3f), CCDelayTime::actionWithDuration(3.f), CCFadeOut::actionWithDuration(0.5f),	0));
			tipbox->runAction(CCSequence::actions(CCFadeIn::actionWithDuration(0.3f), CCDelayTime::actionWithDuration(3.f), CCFadeOut::actionWithDuration(0.5f),
												  CCCallFuncN::actionWithTarget(this, callfuncN_selector(ThisClassType::deleteSprite)), 0));
		}
	}
}
void GameScreen::onGoal(BallBase& second)
{
	if(info.gameMode == BS2GameInformation::ZERO)
	{
		pBasketGame->incFever(BS2GameInformation::INC_VALUE_IN_ZERO + rand()%5);
	}
	else
	{
		pBasketGame->incFever(BS2GameInformation::INC_VALUE_IN_NORMAL + rand()%5);
	}
	
	KSoundEngine::sharedEngine()->playSound("se_wagnet04.mp3");
	info.seqFailCounter = 0;
	bs2play->combo++;
	
	bool bShout = false;
	if(info.gameMode == BS2GameInformation::ZERO)
	{
		if(bs2play->combo.getVar() % 5 == 0)
		{
			KSoundEngine::sharedEngine()->playSound("shout.mp3");
			bShout = true;
		}
	}
	
	if(bShout == false)
	{
		int r = rand() % 8;
		KSoundEngine::sharedEngine()->playSound(KS_Util::stringWithFormat("p%d.mp3", r).c_str());
	}

	info.goalCount++;
	info.postSpeed++;
	
	pBasketGame->showCombo();
//	switch(bs2play->combo.getVar())
//	{
//		case 1:
//			KSoundEngine::sharedEngine()->playSound("se_combo1.mp3");
//			break;
//		case 2:
//			KSoundEngine::sharedEngine()->playSound("se_combo2.mp3");
//			break;
//		case 3:
//			KSoundEngine::sharedEngine()->playSound("se_combo3.mp3");
//			break;
//		case 4:
//			KSoundEngine::sharedEngine()->playSound("se_combo4.mp3");
//			break;
//		case 5:
//			KSoundEngine::sharedEngine()->playSound("se_combo5.mp3");
//			break;
//		case 6:
//			KSoundEngine::sharedEngine()->playSound("se_combo6.mp3");
//			break;
//		default:
//			KSoundEngine::sharedEngine()->playSound("se_combo6.mp3");
//			break;
//	}
	
	
//	NSDefault::setPrice(NSDefault::getPrice() + second.getAtePrice());
//	if(second.getAtePrice() > 0)
//	{
//		auto retainAnimation = playAnimation("coin.png", 0.07f, 6, 6, Graphics::COIN_WIDTH, Graphics::COIN_HEIGHT, false);
//		addChild(retainAnimation.second);
//		ccBezierConfig bc;
//		bc.controlPoint_1 = retainAnimation.second->getPosition();
//		bc.controlPoint_2 = ccp(250, 100);
//		bc.endPosition = ccp(470, 400);
//		auto moveAction = CCBezierTo::actionWithDuration(1.2f, bc);
//		auto action2 = CCSpawn::actions(retainAnimation.first, moveAction, CCScaleTo::actionWithDuration(0.8f, 0.5f)/*, CCFadeOut::actionWithDuration(1.2f)*/, 0); // 가면서 사라짐.
//		auto action3 = CCCallFuncN::actionWithTarget(this, callfuncN_selector(KSBaseScene::deleteSprite));
//		auto totalAction = CCSequence::actionOneTwo(action2, action3);
//		retainAnimation.second->setPosition(ccp(second.getShape()->body->p.x, second.getShape()->body->p.y));
//		retainAnimation.second->runAction(totalAction);
//	}
//	pBasketGame->setCoin(NSDefault::getPrice());
	
	int comboscore = comboScore(info.goalCount, bs2play->combo.getVar());
	bs2play->gameScore += comboscore;
	wcplay->gameScore = bs2play->gameScore.getVar();
	
	if(bs2play->gameScore.getVar() > BS2SaveData::getMaxScore())
		pBasketGame->pointMaxSprite();
	pBasketGame->setScore(bs2play->gameScore.getVar());
	second.setGoaled();
	
	
	graphics.goalInEffect(comboscore);
	pBasketGame->uiUpdate(comboscore);
	ChipSprites* _ballSprites = (ChipSprites*)second.getShape()->data;
	CCSprite* _ball = (CCSprite*)_ballSprites->getReader();
	_ball->getParent()->reorderChild(_ball, 8);
	
	second.setGoalTime(gameTimer);
	
	if(info.gameMode != BS2GameInformation::ZERO && pBasketGame->getFever() >= 100.f && info.isTimeOver == false)
	{
		graphics.zeroSprite->runAction(CCSequence::actions(CCProgressFromTo::actionWithDuration(0.4f, 0, 100.f),
												  CCCallFuncN::actionWithTarget(this, callfuncN_selector(GameScreen::onZeroArea)), 0));

	}
}

void GameScreen::goZeroArea()
{
	
	if(info.gameMode != BS2GameInformation::ZERO)
	{
		graphics.zeroSprite->runAction(CCSequence::actions(CCProgressFromTo::actionWithDuration(0.4f, 0, 100.f),
														   CCCallFuncN::actionWithTarget(this, callfuncN_selector(GameScreen::onZeroArea)), 0));
		
	}

}
void GameScreen::onZeroArea(CCNode*)
{
	KSoundEngine::sharedEngine()->playSound("RetroSpace.mp3");
	KSoundEngine::sharedEngine()->playSound("se_crowd01.mp3");
	KSoundEngine::sharedEngine()->playSound("fever.mp3");
	//KSoundEngine::sharedEngine()->playSound("se_crowd02.mp3");

	info.movingPost = true;
	info.postSpeed = 0; 
	info.gameMode = BS2GameInformation::ZERO;
	pBasketGame->pauseStopWatch2();

	

#if (ENABLE_CODE == TRUE)
	info.coinStep = 0;
	int index = info.coinStep;
	for(int posY = info.coinsArea[index].origin.y;posY <= info.coinsArea[index].origin.y+info.coinsArea[index].size.height; posY += Graphics::COIN_HEIGHT)
	{
		for(int posX = info.coinsArea[index].origin.x;posX <= info.coinsArea[index].origin.x + info.coinsArea[index].size.width; posX += Graphics::COIN_WIDTH)
		{
			createCoin(posX, posY, index);
		}
	}
#endif
}

void GameScreen::onExitZeroArea()
{
	KSoundEngine::sharedEngine()->playSound("hotshots.mp3");
	KSoundEngine::sharedEngine()->playSound("exitzero.mp3");
	graphics.zeroSprite->runAction(CCSequence::actions(CCProgressFromTo::actionWithDuration(0.4f, 100.f, 0.f),
													   CCCallFuncN::actionWithTarget(this, callfuncN_selector(GameScreen::onFinishZeroArea)), 0));
	info.gameMode = BS2GameInformation::NORMAL;
	info.movingPost = false;
	if(playInfo->selectedItems["item1"])
	{
		info.onceTipIn = true;
		pBasketGame->showTipInSprite();
	}
	removeAllCoin();
	pBasketGame->resumeStopWatch2();
}
void GameScreen::onFinishZeroArea()
{
	
}
void GameScreen::judgeCleanShot()
{
	for(auto iter = currentBalls.begin(); iter != currentBalls.end(); ++iter)
	{
		if(iter->second.getGoaled() && iter->second.getGoalTime() + BS2GameInformation::AFTER_JUDGE_TIME <= gameTimer&& iter->second.getIsJudged() == false) // 판정이 되지 않았다면,
		{
			iter->second.setIsJudged(true); // 판정 했음.
			
			if(iter->second.getRefelectCount() <= 2)
			{
				// clean shot
				int tipInPoint = 20;
				if(info.gameMode == BS2GameInformation::ZERO)
					tipInPoint *= 1.75f;

						
				auto animator = SceneUtil::playAnimation(graphics.cleanShotBatchNode, 0.07f, 3, 6, Graphics::CLEAN_WIDTH, Graphics::CLEAN_WIDTH, false);
				animator.second->setPosition(ccp(170, 130));
				animator.second->setScale(1.3f);
				auto action2 = CCDelayTime::actionWithDuration(1.f);
				auto action3 = CCCallFuncN::actionWithTarget(this, callfuncN_selector(GameScreen::deleteSprite));
				animator.second->runAction(CCSequence::actions(animator.first, action2, CCFadeOut::actionWithDuration(0.4f), action3, 0));
			}
			else if(iter->second.getRefelectCount() >= 7)
			{
				int tipInPoint = 13;
				if(info.gameMode == BS2GameInformation::ZERO)
					tipInPoint *= 1.75f;

				
				auto animator = SceneUtil::playAnimation(graphics.luckyBatchNode, 0.07f, 3, 6, Graphics::LUCKY_WIDTH, Graphics::LUCKY_HEIGHT, false);
				animator.second->setScale(1.3f);
				animator.second->setPosition(ccp(170, 130));
				auto action1 = CCSequence::actions(animator.first, CCDelayTime::actionWithDuration(1.f), CCFadeOut::actionWithDuration(0.4f),
												   CCCallFuncN::actionWithTarget(this, callfuncN_selector(KSBaseScene::deleteSprite)), 0);
				animator.second->runAction(action1);
				
				//KSoundEngine::sharedEngine()->playSound("se_lucky.mp3");
			}
			else
			{
				if(bs2play->combo.getVar()>1 && bs2play->combo.getVar()<5){
					KSoundEngine::sharedEngine()->playSound(KS_Util::stringWithFormat("se_combo%d.mp3",rand()%6+1));
				}
				int tipInPoint = 13;
				if(info.gameMode == BS2GameInformation::ZERO)
					tipInPoint *= 1.75f;
			}			
		}
	}
}
int GameScreen::comboScore(int goalCount, float combo)
{
	return (combo * 400) * (info.gameMode == BS2GameInformation::ZERO ? 2 : 1);
	return (combo * 100 + goalCount * 50) * (info.gameMode == BS2GameInformation::ZERO ? 2 : 1) * 2;
//	double jarum = combo*100.f;
//	jarum = ((int)(jarum / 10)) * 10;
//	return jarum;
}
float GameScreen::decideCoinPosition()
{
	CCRect validArea = validCoinArea();
	return (selectBallPosition().x + 100)/2.f; // 100 은 두번 째 골대의 x 위치
	return validArea.origin.x + rand() % (int)(validArea.size.width - validArea.origin.x);
}
CCRect GameScreen::validCoinArea()
{
	CCRect ret;
	ret.origin.x = r1.getBackBoardBodyPos().x + cpCircleShapeGetOffset(r1.getRing2()).x;
	ret.size.width = getXmid() - ret.origin.x;
	
	ret.origin.y = r1.getBackBoardBodyPos().y + cpCircleShapeGetOffset(r1.getRing2()).y;
	ret.size.height = BS2GameInformation::COIN_UPBOUND - ret.origin.y;
	
	return ret;
}


void GameScreen::draw()
{

}

CCPoint GameScreen::createBall(CCPoint _ballPos)
{
	BallBase _bb;
	_bb.create();
	_bb.getShape()->collision_type = STATIC_BALL_COLLISION_TYPE;
	
	_bb.getShape()->body->p = {_ballPos.x, _ballPos.y};
	_bb.setFirstPosition(_ballPos);
	_bb.previousPosition = _ballPos;

	controlableBall = _bb.getShape();
	cpSpaceAddStaticShape(space, _bb.getShape());
	string ballName;
	if(NSDefault::getIsReview())
	{
		ballName += KS_Util::stringWithFormat("ball%d.png", rand()%6 + 1);
	}
	else
		ballName = "ball1.png";
	
	ChipSprites* chipBall = chipSpritesFactory.create();
	auto animator = SceneUtil::playAnimation(ballName.c_str(), 0.2, 4, 4, 90, 90, true);
	animator.second->runAction(animator.first);
	chipBall->addCCNodeWithFlag(animator.second);
	addChild(animator.second, zorder::BALL);
//	CCLog("%x", animator.second);
	
	CCParticleSystemPoint* appearBallEff = CCParticleSystemPoint::particleWithFile("ballback.plist");
	appearBallEff->setIsAutoRemoveOnFinish(true);
	appearBallEff->setPosition(_ballPos);
	addChild(appearBallEff, zorder::SHOW_BALL_EFF);
//	CCLog("%x", appearBallEff);
	
	CCSprite* _shadow = CCSprite::spriteWithFile("ball_shadow.png");
	_shadow->setUserData((void*)"myoung am");
	addChild(_shadow, zorder::SHADOW);
	chipBall->addCCNodeWithFlag(_shadow, ChipSprites::D_POSITION);
	
	CCSprite* _ballshadow = CCSprite::spriteWithFile("ball_shadow1.png");
	_ballshadow->setUserData((void*)"shadow!");
	addChild(_ballshadow, zorder::BALL_SHADOW);
	chipBall->addCCNodeWithFlag(_ballshadow, 0);
	
	_bb.setChipSprites(chipBall);
	
	_bb.setPosition(_ballPos);
//	CCLog("%x ball create", controlableBall);
	currentBalls[controlableBall] = _bb;
	return _ballPos;
}

double GameScreen::GaussRandom(int start, int end, int detail)
{
	double r = 0.0;
	
	for(int i = 0;i< detail;++i)
		r += (double)rand()/(double)RAND_MAX;
	
	r /= (double)detail;
	
	return r * (end - start) + start;
}

int GameScreen::getXmid()
{
	int limitStage = MIN(8, (bs2play->combo.getVar() - 1) / 3.f);
	return Graphics::X_MIN + limitStage * (Graphics::X_MAX - Graphics::X_MIN) / 6.f;
}
CCPoint GameScreen::selectBallPosition()
{
//	int stage = 5;
	int Xmid = getXmid();
//	int Ymid = Graphics::Y_MIN + ( stage*2 - 1 ) * (Graphics::Y_MAX - Graphics::Y_MIN) / 10.f;
	int X1 = -(Graphics::X_MAX - Graphics::X_MIN)/2.f;
	int X2 = (Graphics::X_MAX - Graphics::X_MIN)/2.f;
//	int Y1 = -(Graphics::Y_MAX - Graphics::Y_MIN)/2.f;
//	int Y2 = (Graphics::Y_MAX - Graphics::Y_MIN)/2.f;
	double _reX;
	do {
		_reX = Xmid + GaussRandom(X1, X2, 2);
	} while (!(Graphics::X_MIN < _reX && _reX < Graphics::X_MAX));
	
	double _reY = rand() % (Graphics::Y_MAX - Graphics::Y_MIN) + Graphics::Y_MIN;

	return ccp(_reX, _reY);
}
void GameScreen::initBack()
{
	
	CCSprite* _s = CCSprite::spriteWithFile(KS_Util::stringWithFormat("back%d.png",
																	 bs2play->backNumber).c_str());
	_s->setPosition(ccp(Graphics::SCREEN_WIDTH / 2.f, 0));
	_s->setAnchorPoint(ccp(0.5, 0));
	_s->setScale(Graphics::SCREEN_WIDTH / 480.f);
	CCSize s = CCEGLView::sharedOpenGLView().getSize();
	//_s->setPosition(ccp(s.width / 2.f, 0));
	addChild(_s, zorder::BACK);
}

void GameScreen::step(ccTime delta)
{
	int steps = 1;
	CGFloat dt = delta/(CGFloat)steps;
	
	for(int i=0; i<steps; i++)
	{
		cpSpaceStep(space, dt);
		
		
		//CCLog("___ %f %f", staticBallShape->body->p.x, staticBallShape->body->p.y);
	}
	for(auto iter = currentBalls.begin(); iter != currentBalls.end(); ++iter)
	{
		if(iter->first->body->v.y < 0)
		{
			iter->second.setCrashable();
		}
	}
	cpSpaceHashEach(space->activeShapes, &eachShape, NULL);
	cpSpaceHashEach(space->staticShapes, &eachShape, NULL);
}
bool GameScreen::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{	
	CCTouch* touch = (CCTouch*)pTouch;
	CCPoint _t = touch->locationInView();
	
	CCPoint location(ccp(0, 0));
	location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
	CCPoint relLocation(ccp(0, 0));
	relLocation = convertToNodeSpace(location);
	
	bool isTipInShoot = false;
	for(auto iter = currentBalls.begin(); iter != currentBalls.end(); ++iter)
	{
		ChipSprites* _ballChipSprite = iter->second.getChipSprites();
		CCNode* _ballSprite = _ballChipSprite->getReader();
		CCRect rect = CCRectMake(0, 0, _ballSprite->getContentSizeInPixels().width, _ballSprite->getContentSizeInPixels().height);
		rect.size.width /= getScale();
		rect.size.height /= getScale();
		rect = CC_RECT_PIXELS_TO_POINTS(CCRectApplyAffineTransform(rect,_ballSprite->nodeToParentTransform()) );
		
		if(CCRect::CCRectContainsPoint(rect, relLocation))
		{
			if(iter->second.isTipInAble() && info.onceTipIn)
			{
				info.onceTipIn = false;
				pBasketGame->hideTipInSprite();
				KSoundEngine::sharedEngine()->playSound("se_usetipin.mp3");
				isTipInShoot = true;
				turn.lastThrowTime = gameTimer;
				CCPoint _v_ccp = getTipInShot(BS2GameInformation::TIPIN_TIME, iter->second.getShape());
				auto animator = SceneUtil::playAnimation("moveeffect.png", 0.1, 4, 4, 77, 77, true); // tip-in effect
				animator.second->runAction(animator.first);
				animator.second->setUserData((void*)"tip-in efffect");
				addChild(animator.second, zorder::TIP_IN_EFF);
				((ChipSprites*)iter->second.getShape()->data)->addCCNodeWithFlag(animator.second);
				cpVect _v = {_v_ccp.x, _v_ccp.y};
				iter->second.getShape()->body->v = {_v.x, _v.y};
				return true;
			}
		}
	}
	if(controlableBall != 0)
	{		
		if(!turn.doubleShotWaiting && clicks.empty())
		{
			pBasketGame->hideInterface();
			clicks[(int)touch].beginPoint = location;
			clicks[(int)touch].currentPoint = location;
			touchPoints.push_back(TouchType(gameTimer, location) );
			turn.ballCatchTime = gameTimer;
			
#if (ENABLE_CODE == TRUE)
			if(info.gameMode == BS2GameInformation::ZERO)
			{
				
				int index = info.coinStep;
				//coins 안의 그룹번호가 index 인것을 찾아 지움
				for(auto iter = coins.begin(); iter != coins.end();)
				{
					if(iter->second.second == index)
					{
						auto bIter = iter++;
						removeCoin( bIter->first );
					}
					else
						++iter;
				}
				
				for(int posY = info.coinsArea[index].origin.y;posY <= info.coinsArea[index].origin.y+info.coinsArea[index].size.height; posY += Graphics::COIN_HEIGHT)
				{
					for(int posX = info.coinsArea[index].origin.x;posX <= info.coinsArea[index].origin.x + info.coinsArea[index].size.width; posX += Graphics::COIN_WIDTH)
					{
						createCoin(posX, posY, index);
					}
				}
				
			}
#endif
			//isObjectTouch = true;
		}		
	}
	return true;
}
void GameScreen::ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent)
{
	{
		CCTouch* touch = (CCTouch*)pTouch;
		
		/*if(touch->view()->isKindOfClass(EAGLView::class()) == false) // #
		 continue;*/
		CCPoint _t = touch->locationInView();
		CCPoint location(ccp(0, 0));
		location =CCDirector::sharedDirector()->convertToGL(touch->locationInView());

		
		if( clicks.find((int)touch) != clicks.end() )
		{
			clicks[(int)touch].currentPoint = location;
			touchPoints.push_back(TouchType(gameTimer, location) );
			if(touchPoints.size() >= 100)
				touchPoints.pop_front();
			//isObjectTouch = true;
			showTracer(clicks[(int)touch].currentPoint, clicks[(int)touch].beginPoint);
			
			CCPoint movingPt = convertToWorldSpace( location );
			movingPt.y -= getPositionY();

			if(movingPt.x < 20 || movingPt.x > 460 || movingPt.y < 10) // cancel
			{
				cancelCatch();
			}			
		}
		else if(turn.isMovingBall)
		{
			controlableBall->body->p = cpv(location.x, location.y);
			//isObjectTouch = true;
		}
	}
}
void GameScreen::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
{
	CCTouch* touch = (CCTouch*)pTouch;
	/*if(touch->view()->isKindOfClass(EAGLView::class()) == false) // #
	 continue;*/
	CCPoint location(ccp(0, 0));
	location = convertToNodeSpace( CCDirector::sharedDirector()->convertToGL(touch->locationInView()) );
	
	if(clicks.find((int)touch) != clicks.end())
	{
		
		//clicks[(int)touch].currentPoint = location;
		
		//CCPoint _temp = ccpSub(clicks[(int)touch].currentPoint, clicks[(int)touch].beginPoint);
		pBasketGame->showInterface();
		if(getPosition().x != 0 || getPosition().y != 0)
		{
			auto _r = CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.2f), CCMoveTo::actionWithDuration(0.3f, ccp(0, 0)));
			_r->setTag(DOWNSCROLL);
			runAction(_r);
		}
		KS::KSLog("counter !! %", touchPoints.size());
		CCPoint currentPoint = touchPoints[0].second; // 하나도 없을리는 없음.
		bool invalidTime = false;
		currentPoint =touchPoints[touchPoints.size()-1].second;
		CCPoint throwV = ccpSub(currentPoint, clicks[(int)touch].beginPoint);
		
		if(gameTimer  <= turn.ballCatchTime + Turn::FAST_SHOT_TIME || ccpLength(throwV) <= 30) // 일반 터치스러운건 공 안쏘고 캔슬시킴.
			
		{
			//currentPoint = location;// touchPoints[touchPoints.size()-1].second;
			invalidTime = true; // temprary
		}
		else
		{
			for(auto iter = touchPoints.rbegin(); iter != touchPoints.rend(); ++iter)
			{
				TouchType& _t = *iter;
				
				if(_t.first < gameTimer - Turn::VALID_MOVE_TIME)
				{
					//currentPoint = _t.second;
					break;
				}
			}
		}
		
		touchPoints.clear();
		
		if(!invalidTime)
		{
			CCPoint _temp = ccpSub(currentPoint, clicks[(int)touch].beginPoint);
			showTracer(currentPoint, clicks[(int)touch].beginPoint);
			_temp = ccpMult(_temp, POWER_MUL);
			
			_temp = limitV(_temp);
			CCPoint prevJustPosition = {controlableBall->body->p.x, controlableBall->body->p.y};
			
			info.coinStep++;
			info.coinStep %= 3;
			throwBall(-_temp.x, -_temp.y);
//			CCLog("child = %d", KS_Util::getNumberOfChild(this));
			KS::KSLog("throw % %", _temp.x, _temp.y);
			turn.isUsingItem = false;
			info.throwCounter++;
			//pBasketGame->setBallViewer(info.throwCounter + 1); // 여기!
			CCPoint createdPosition;
			if(info.myItem == DOUBLE_SHOT)
			{
				turn.oneshotTime = gameTimer;
				turn.doubleShotWaiting = true;
				turn.oneshotVelocity = {-_temp.x, -_temp.y};
				createdPosition = prevJustPosition;
			}
			else
			{
				createdPosition = selectBallPosition();
				//CCLog("!!!!!!!!!!!!!!! %f %f", createdPosition.x, createdPosition.y);
			}
			info.myItem = NONE;
			
			
			
			turn.lastThrowTime = gameTimer;
			createBall(createdPosition);
		}
		graphics.hideTracer();
		clicks.erase((int)touch);
		
		//runAction(CCSpawn::actionOneTwo(CCMoveTo::actionWithDuration(0.5f, ccp(120, 80)), CCScaleTo::actionWithDuration(0.5f, 0.5f)));
	}
	else if(turn.isMovingBall)
	{
		info.myItem = NONE;
		turn.isMovingBall = false;
		controlableBall->body->p = cpv(location.x, location.y);
	}
}
void GameScreen::ccTouchCancelled(CCTouch* pTouch, CCEvent* pEvent)
{
	clicks.clear();
}


void GameScreen::passVirtualSpace(cpSpace* virtualSpace, float s)
{
	CGFloat dt = 1.f/50.f;///(CGFloat)steps;
	//dt 로 s 초를 흐르게 해야됨.
	int steps = s / dt;
	int _c = 0;
	for(int acc = 0; acc < steps; acc++)
	{
		cpSpaceStep(virtualSpace, dt);
	}
}

void GameScreen::showTracer(CCPoint currentPoint, CCPoint beginPoint)
{
	for_each(graphics.tracer.begin(), graphics.tracer.end(), [](CCSprite* spr)
			 {
				 
				 spr->setIsVisible(false);
			 });
	
	{
		cpSpace* virtualSpace = cpSpaceNew();
		virtualSpace->damping = space->damping;
		cpSpaceResizeStaticHash(virtualSpace, 100.0f, 40);
		cpSpaceResizeActiveHash(virtualSpace, 500, 600);
		
		
		virtualSpace->gravity = space->gravity;
		virtualSpace->elasticIterations = virtualSpace->iterations;
		
		
		
		cpBody* _virtualBallBody = cpBodyNew(controlableBall->body->m, cpMomentForCircle(controlableBall->body->m, 0,
																						 ((cpCircleShape*)controlableBall)->r, cpv(0,0)));
		cpShape* _virtualBallShape;
		{
			_virtualBallShape = cpCircleShapeNew(_virtualBallBody, ((cpCircleShape*)controlableBall)->r, cpv(0,0));
			_virtualBallBody->p = controlableBall->body->p;
			
			_virtualBallShape->e = controlableBall->e;
			_virtualBallShape->u = controlableBall->u;
			
			cpSpaceAddBody(virtualSpace, _virtualBallBody);
			cpSpaceAddShape(virtualSpace, _virtualBallShape);
			
			CCPoint _temp = ccpSub(currentPoint, beginPoint);
			_temp = ccpMult(_temp, POWER_MUL);
			_temp = limitV(_temp);
			//				_temp.x = 100;
			//				_temp.y = -100;
			_virtualBallShape->body->v = cpv(-_temp.x, -_temp.y);
			//CCLog("jojun %f %f", -_temp.x, -_temp.y);
		}
		{			
			float g = -BS2GameInformation::GRAVITY;
			float _t;
			int stage = 5;
			int vStage = MIN(6, stage * 1);
			float guideTime = 0.2f;
			if(info.gameMode != BS2GameInformation::GAMEMODETYPE::ZERO)
				guideTime += pBasketGame->getFever() / 100.f * 0.2f;
			else
				guideTime += MAX(0.f, 0.2f - bs2play->combo.getVar() * 0.02f);
			_t = 2.f * _virtualBallShape->body->v.y / g * ((100.f - vStage * 7.f) / 100.f) + guideTime;
			float _difft = 0.f;
			if(info.isGuided == true)
			{
				_difft = 0.17f; // 2.f * _virtualBallShape->body->v.y / g + 0.15f - _t;
			}
			else
			{
				_t = MAX(_t, _virtualBallShape->body->v.y / g + 0.11f);
			}
			float maxHeight = _virtualBallShape->body->p.y + (_virtualBallShape->body->v.y)*(_virtualBallShape->body->v.y) / (2.f*g);
			maxHeight += 15;
			if(maxHeight >= Graphics::SCREEN_HEIGHT)
			{
				setPosition(ccp(0, 0 + Graphics::SCREEN_HEIGHT - maxHeight));
			}
			graphics.item_tracer->removeAllChildrenWithCleanup(true);
			float dt = 0.05f;
			
			float x = 0.f;
			int i = 0;
			do {
				passVirtualSpace(virtualSpace, dt); // dt 초가 흘렀을 때의 처리.
				float X = _virtualBallBody->p.x;
				float Y = _virtualBallBody->p.y;
				
				//showTracer();
				if(x <= _t)
				{
					if(i <= graphics.tracer.size()-1) // valid index
					{
						CCSprite* spr = graphics.tracer[i];
						spr->setOpacity(255);
						spr->setIsVisible(true);
						spr->setPosition(ccp(X, Y));
						//float absY = convertToWorldSpace(ccp(X, Y)).y;
						//CCLog("%f abs scale %f", Y, getScale());
						
					}
				}
				else
				{ // bonus guide
					auto animator = SceneUtil::playAnimation(graphics.item_tracer, 0.07f, 6, 6, 12, 12, true);
					animator.second->setPosition(ccp(X, Y));
					animator.second->runAction(animator.first);
				}
				
				i++;
				x += dt;
			} while (x <= _t + _difft);
		}
		cpSpaceFreeChildren(virtualSpace);
		cpSpaceFree(virtualSpace);
	}

}

void GameScreen::throwBall(float Vx, float Vy)
{
	cpSpaceAddBody(space, controlableBall->body);
	cpSpaceRemoveStaticShape(space, controlableBall);
	cpSpaceAddShape(space, controlableBall);
	
	
	controlableBall->collision_type = DYNAMIC_BALL_COLLISION_TYPE;
	controlableBall->e = 0.7f;
	controlableBall->u = 5.f; 
	//controlableBall->body->v = cpv(Vx, Vy);
	
	controlableBall->body->v = cpv(Vx, Vy); // throw
	controlableBall->body->w = -10.f;
	if(turn.doubleShotWaiting)
		currentBalls[controlableBall].setIsDoubleShot();
	currentBalls[controlableBall].setShoot();
	
	if(info.gameMode == BS2GameInformation::ZERO)
	{
		ChipSprites* cs = (ChipSprites*)controlableBall->data;
		CCParticleSystemPoint* part = CCParticleSystemPoint::particleWithFile("zeroball.plist");
		addChild(part, zorder::ZERO_BALL_EFF);
		cs->addCCNodeWithFlag(part, ChipSprites::D_POSITION);
	}
	
	controlableBall = 0;
	if(info.gameMode == BS2GameInformation::ZERO)
	{
		KSoundEngine::sharedEngine()->playSound("se_throwballinzero.mp3");
	}
	else
	{
		KSoundEngine::sharedEngine()->playSound("throw.mp3");
	}
	
}
void GameScreen::finishDoubleShot()
{
	//pBasketGame->closeShop();
}
void GameScreen::useItem(enum ItemType it)
{
	turn.isUsingItem = true;
	if(it == ItemType::DOUBLE_SHOT)
	{
		CCSprite* doubleSprite = CCSprite::spriteWithFile("double.png");
		addChild(doubleSprite);
		doubleSprite->setPosition(ccp(controlableBall->body->p.x, controlableBall->body->p.y + 50));
		doubleSprite->setScale(0.3f);
		auto action1 = CCSpawn::actions(CCScaleTo::actionWithDuration(0.3f, 1.f), 0);
		auto action2 = CCSequence::actions(action1, CCDelayTime::actionWithDuration(0.8f), CCCallFunc::actionWithTarget(this, callfunc_selector(GameScreen::finishDoubleShot)), CCCallFuncN::actionWithTarget(this, callfuncN_selector(KSBaseScene::deleteSprite)),0);
		doubleSprite->runAction(action2);
		
		info.myItem = it;
	}
	else if(it == ItemType::GUIDE_LINE)
	{
		info.isGuided = true;
	}
	else if(it == ItemType::MOVE_BALL)
	{
		info.movingTime = 0.6f;
		info.firstPosition = controlableBall->body->p;
		schedule(schedule_selector(GameScreen::movingBall));
	}
	else if(it == ItemType::TIME_PLUS)
	{
		info.remainTime += 10.f;
		pBasketGame->setTimeNoWarning();
		unschedule(schedule_selector(GameScreen::tickSound));
	}
}



void GameScreen::movingBall(ccTime dt)
{
	cpVect diff = cpvsub(cpv(200, 220), info.firstPosition);
	cpVect dis = cpvmult(diff, 1.f/info.movingTime);
	controlableBall->body->p = cpvadd(controlableBall->body->p, cpvmult(dis, dt));
	bool _end = false;
	if(diff.x >= 0)
	{
		if(controlableBall->body->p.x >= 200)
		{
			_end = true;
		}
	}
	else
	{
		if(controlableBall->body->p.x < 200)
		{
			_end = true;
		}
	}
	
	if(_end)
	{
		controlableBall->body->p = cpv(200, 220);
		//pBasketGame->closeShop();
		unschedule(schedule_selector(GameScreen::movingBall));
	}
}
void GameScreen::doubleShot()
{
	if(turn.oneshotTime + Turn::DOUBLE_SHOT_COOLTIME <= gameTimer && turn.doubleShotWaiting == true)
	{
		throwBall(turn.oneshotVelocity.x, turn.oneshotVelocity.y);
		turn.doubleShotWaiting = false;
		turn.lastThrowTime = gameTimer;
		
		CCPoint newBallPos = selectBallPosition();
		createBall(newBallPos);
	}
}



CCPoint GameScreen::limitV(CCPoint v)
{
	if(ccpDistance(ccp(0,0), v) > BS2GameInformation::INITIAL_V_LIMIT)
	{
		float theta = atan2f(v.y, v.x);
		v.x = BS2GameInformation::INITIAL_V_LIMIT * cosf(theta);
		v.y = BS2GameInformation::INITIAL_V_LIMIT * sinf(theta);
	}
	return v;
}


void GameScreen::registerWithTouchDispatcher()
{
	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
}

void GameScreen::cancelCatch()
{
	graphics.hideTracer();
	CCSprite* touchhere = CCSprite::spriteWithFile("touchhere.png");
	touchhere->setPosition(ccp(240, 160));
	getParent()->getParent()->addChild(touchhere);
	auto total2 = CCBlink::actionWithDuration(1.2f, 4);
	auto _callback = CCCallFuncN::actionWithTarget(this, callfuncN_selector(KSBaseScene::deleteSprite));
	if(getPosition().x != 0 || getPosition().y != 0)
	{
		auto _r = CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.0f), CCMoveTo::actionWithDuration(0.5f, ccp(0, 0)));
		_r->setTag(DOWNSCROLL);
		runAction(_r);
	}
	touchhere->runAction(CCSequence::actionOneTwo(total2, _callback));
	clicks.clear();
	touchPoints.clear();
	pBasketGame->showInterface();
}
void GameScreen::sceneChanger()
{
	if(Turn::DELAY_JUMP_TO_ROUND + turn.lastThrowTime <= gameTimer &&
	   !turn.doubleShotWaiting &&
	   !info.isTimeOver)
	{
		// # ReplaceScene(BasketGameOver);
	}
	else if(Turn::DELAY_JUMP_TO_ROUND + turn.lastThrowTime <= gameTimer && !turn.doubleShotWaiting &&  info.throwCounter >= 10 && !info.isTimeOver)
	{
		// # ReplaceScene(Basket2Round);
	}
}
CCPoint GameScreen::getTipInShot(float s, cpShape* shape)
{
	float Y = cpvadd(r1.getBackBoardBodyPos(), cpCircleShapeGetOffset(r1.getRing1()) ).y;
	float X = cpvadd(r1.getBackBoardBodyPos(), cpCircleShapeGetOffset(r1.getRing1()) ).x + r1.getRingSize() / 2.f;
	float g = -BS2GameInformation::GRAVITY;
	float y0 = shape->body->p.y;
	float x0 = shape->body->p.x;
	float _s = s;
	float theta = atan2f(0.5f * g * _s*_s + Y - y0, x0 - X);
	float V = (x0 - X) / (_s * cos(theta));
	float Vx = -cos(theta) * V;
	float Vy = sin(theta) * V;
	return ccp(Vx, Vy);
}




void CCLabelTTFFactory::reserve(CCNode* _parent, int _depth, int n, int charSize, const char* font, float fs)
{
	
	parent = _parent;
	depth = _depth;
	string s;
	s.assign("a", charSize);
	const char* _s = s.c_str();
	for(int i=0; i<n; i++)
	{
		CCLabelTTF* _create = CCLabelTTF::labelWithString(_s, font, fs);
		_create->setIsVisible(false);
		parent->addChild(_create, depth);
		ttfGarage.push(_create);
	}
	
}
CCLabelTTF* CCLabelTTFFactory::getCCLabelTTF()
{
	CCAssert(!ttfGarage.empty(), "!ttfGarage.empty()");
	
	CCLabelTTF* _a = ttfGarage.front();
	ttfGarage.pop();
	_a->setIsVisible(true);
	return _a;
}




