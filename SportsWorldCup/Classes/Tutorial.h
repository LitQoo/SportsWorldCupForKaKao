//
//  Tutorial.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 27..
//
//

#ifndef __SportsWorldCup__Tutorial__
#define __SportsWorldCup__Tutorial__

#include "GameSystem.h"
#include "BS2Play.h"
#include "AKPlay.h"
#include "HWPlay.h"
#include "BS2BasketGame.h"
#include "AKAngryGameScene.h"
#include "HWFirstGameScene.h"
#include "ACGameScene.h"
#include "HWGameDirector.h"
#include "ACPlay.h"

#include "cocos2d.h"
USING_NS_CC;
#define SCENE_FUNC(_A) static CCScene* scene() { CCScene* scene = CCScene::node(); _A * layer = _A::node(); scene->addChild(layer); return scene; }
class Tutorial : public CCLayerColor
{
private:
public:
	SCENE_FUNC(Tutorial);
	LAYER_NODE_FUNC(Tutorial);
	Tutorial(){}
	virtual ~Tutorial()
	{}
	
	void START(CCObject*)
	{
		KSoundEngine::sharedEngine()->playSound("select.mp3");
		playInfo->initGameAndStart(playInfo->currentGame);
	}

	virtual bool init()
	{
		CCLayerColor::initWithColor(ccc4f(40, 57, 80, 0));
		CCLog("cho jay");
		CCSprite* mainBack = CCSprite::spriteWithFile("main_back.png");
		mainBack->setPosition(ccp(240, 160));
		addChild(mainBack);

		CCMenu* _menu = CCMenu::menuWithItems(0);
		_menu->setPosition(ccp(0, 0));
		addChild(_menu);
		CCMenuItem* tuto;
		if(playInfo->currentGame == "BS")
		{
			tuto = CCMenuItemImage::itemFromNormalImage("tutorial_basketball.png", "tutorial_basketball.png", this, menu_selector(ThisClassType::START));
			tuto->setPosition(ccp(240, 160));

		}
		else if(playInfo->currentGame == "HW")
		{
			tuto = CCMenuItemImage::itemFromNormalImage("tutorial_hurdle.png", "tutorial_hurdle.png", this, menu_selector(ThisClassType::START));
			tuto->setPosition(ccp(240, 160));

			
		}
		else if(playInfo->currentGame == "AC")
		{
			tuto = CCMenuItemImage::itemFromNormalImage("tutorial_archery.png", "tutorial_archery.png", this, menu_selector(ThisClassType::START));
			tuto->setPosition(ccp(240, 160));
			

		}
		else if(playInfo->currentGame == "AK")
		{
			tuto = CCMenuItemImage::itemFromNormalImage("tutorial_clay.png", "tutorial_clay.png", this, menu_selector(ThisClassType::START));
			tuto->setPosition(ccp(240, 160));
		}
		else if(playInfo->currentGame == "SK")
		{
			tuto = CCMenuItemImage::itemFromNormalImage("tutorial_ski.png", "tutorial_ski.png", this, menu_selector(ThisClassType::START));
			tuto->setPosition(ccp(240, 160));
			
		}
		_menu->addChild(tuto, 2);
		return true;
	}

};
#endif /* defined(__SportsWorldCup__Tutorial__) */
