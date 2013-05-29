//
//  SportsWorldCupAppDelegate.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 1. 22..
//  Copyright __MyCompanyName__ 2013년. All rights reserved.
//

#include "AppDelegate.h"
#include "BattleMiddle.h"
#include "cocos2d.h"
#include "KSoundEngine.h"
#include "SWIntro.h"

#include "GameSystem.h"
#include "GraphDog.h"
#include "SaveData.h"
#include "NSDefault.h"
#include "StartOption.h"
#include "UIMediator.h"
#include "LoadingScene.h"
#include "SKGameScene.h"
#include "SKPlay.h"



USING_NS_CC;

AppDelegate::AppDelegate()
{
	KS::KSLog("%", __FUNCTION__);
}

AppDelegate::~AppDelegate()
{
	KS::KSLog("destory");
}

bool AppDelegate::initInstance()
{
	KS::KSLog("%", __FUNCTION__);
    bool bRet = false;
    do 
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

        // Initialize OpenGLView instance, that release by CCDirector when application terminate.
        // The HelloWorld is designed as HVGA.
        CCEGLView * pMainWnd = new CCEGLView();
        CC_BREAK_IF(! pMainWnd
            || ! pMainWnd->Create(TEXT("cocos2d: Hello World"), 480, 320));

#endif  // CC_PLATFORM_WIN32
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        // OpenGLView is initialized in AppDelegate.mm on ios platform, nothing need to do here.
#endif  // CC_PLATFORM_IOS

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)        
        // OpenGLView is initialized in HelloWorld/android/jni/helloworld/main.cpp
		// the default setting is to create a fullscreen view
		// if you want to use auto-scale, please enable view->create(320,480) in main.cpp
#endif  // CC_PLATFORM_ANDROID
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WOPHONE)

        // Initialize OpenGLView instance, that release by CCDirector when application terminate.
        // The HelloWorld is designed as HVGA.
        CCEGLView* pMainWnd = new CCEGLView(this);
        CC_BREAK_IF(! pMainWnd || ! pMainWnd->Create(320,480, WM_WINDOW_ROTATE_MODE_CW));

#ifndef _TRANZDA_VM_  
        // on wophone emulator, we copy resources files to Work7/TG3/APP/ folder instead of zip file
        cocos2d::CCFileUtils::setResource("HelloWorld.zip");
#endif

#endif  // CC_PLATFORM_WOPHONE

#if (CC_TARGET_PLATFORM == CC_PLATFORM_AIRPLAY)
		// MaxAksenov said it's NOT a very elegant solution. I agree, haha
		CCDirector::sharedDirector()->setDeviceOrientation(kCCDeviceOrientationLandscapeLeft);
#endif
        
        bRet = true;
    } while (0);
    return bRet;
}
typedef struct tagResource { CCSize sizeInPixel; CCSize sizeDesign; char directory[100]; }Resource;
static Resource resPhone = { CCSizeMake(480, 320), CCSizeMake(480, 320), "iphone" }; static Resource resPhoneRetina35 = { CCSizeMake(960, 640), CCSizeMake(480, 320), "ipad" }; static Resource resPhoneRetina40 = { CCSizeMake(1136, 640), CCSizeMake(568, 320), "ipad" }; static Resource resTable = { CCSizeMake(1024, 768), CCSizeMake(1024, 768), "ipad" }; static Resource resTableRetina = { CCSizeMake(2048, 1536), CCSizeMake(1024, 768), "ipadhd" };


bool AppDelegate::applicationDidFinishLaunching()
{
	KS::KSLog("%", __FUNCTION__);
	srand(time(0));
	playInfo->initLvTable();
	// initialize director
	CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(&CCEGLView::sharedOpenGLView());

	CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGBA8888);
	CCDirector::sharedDirector()->setProjection(CCDirectorProjection2D);
	CCDirector::sharedDirector()->setDepthTest(false);
	CCFileUtils::setiPhoneRetinaDisplaySuffix("");
	
    // enable High Resource Mode(2x, such as iphone4) and maintains low resource on other devices.
    // pDirector->enableRetinaDisplay(true);

	// sets landscape mode
	//pDirector->setDeviceOrientation(kCCDeviceOrientationLandscapeLeft);

	if(!pDirector->enableRetinaDisplay(true))
	{
		pDirector->setContentScaleFactor(2.0f);
	}
	
	// turn on display FPS
	pDirector->setDisplayFPS(false);

	// set FPS. the default value is 1.0/60 if you don't call this
	pDirector->setAnimationInterval(1.0 / 60);
	KS::KSLog("AppDelegate");
	
	saveData->createJSON();
	// create a scene. it's an autorelease object
//	CCScene *pScene = LoadingScene<SKGameScene>::scene();
//	skplay->initVars();
//	pDirector->runWithScene(pScene);

//	{
//		CCScene *pScene = LoadingScene<BattleMiddle>::scene();
//		pDirector->runWithScene(pScene);
//	}
	
//	CCScene *pScene = SKGameScene::scene();
	
	if(NSDefault::getUserName() == "")
	{
		CCScene *pScene = StartOption::scene();
		pDirector->runWithScene(pScene);
	}
	else
	{
		CCScene *pScene = LoadingScene<SWIntro, decltype(INIT_MODE::STARTUP)>::scene(INIT_MODE::STARTUP);
		pDirector->runWithScene(pScene);
	}
	
	// run
	KS::KSLog("--------------------------------");
	KSoundEngine::sharedEngine()->setIsBGM(NSDefault::getBGM());
	KSoundEngine::sharedEngine()->setIsEffect(NSDefault::getEffectSound());

	
	
	KSoundEngine::sharedEngine()->registerSoundOnEffect("scoring1.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("scoring2.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("select.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("bounce.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("start.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("throw.mp3");
	
	KSoundEngine::sharedEngine()->registerSoundOnEffect("se_applyitem.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("se_backboard01.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("se_crowd01.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("se_endzero.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("se_levelup.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("se_rim02.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("se_takegold01.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("se_takemedal.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("se_throwballinzero.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("se_timeover.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("se_timewarning.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("se_tipinfull.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("se_useheart01.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("se_useitem.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("se_usetipin.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("se_wagnet04.mp3");
//	KSoundEngine::sharedEngine()->registerSoundOnBGM("se_bgm0.mp3");
	
	//KSoundEngine::sharedEngine()->registerSoundOnEffect("se_last.mp3");
	//KSoundEngine::sharedEngine()->registerSoundOnEffect("se_lucky.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("se_crowd02.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("crashonhurdle.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("normaljump.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("bbang.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("birddeath.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("gameselect.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("uhuh.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("bonustime.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("shield.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("usemoney.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("useheart.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("getmedal.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("levelup.mp3");
//	KSoundEngine::sharedEngine()->registerSoundOnBGM("Home Trip.mp3");
//	KSoundEngine::sharedEngine()->registerSoundOnBGM("RetroSpace.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnBGM("RetroSpace.mp3");
//	KSoundEngine::sharedEngine()->registerSoundOnBGM("Let me Fly.mp3");
//	KSoundEngine::sharedEngine()->registerSoundOnBGM("Jacky.mp3");
	
	KSoundEngine::sharedEngine()->registerSoundOnBGM("hotshots.mp3");
	
//	KSoundEngine::sharedEngine()->registerSoundOnBGM("beat area.mp3");
//	KSoundEngine::sharedEngine()->registerSoundOnBGM("Block Buster.mp3");
//	KSoundEngine::sharedEngine()->registerSoundOnBGM("Monster Truck.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("SE_arrow.wav");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("SE_pull.wav");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("SE_slot.wav");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("shout.mp3");

	KSoundEngine::sharedEngine()->registerSoundOnEffect("readygo.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("slot.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("fever.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("hurryup.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("timeover.mp3");
	
	KSoundEngine::sharedEngine()->registerSoundOnEffect("p0.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("p1.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("p2.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("p3.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("p4.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("p5.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("p6.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("p7.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("exitzero.mp3");
	
	KSoundEngine::sharedEngine()->registerSoundOnEffect("skigihap.wav");
	playInfo->initLvTable();
	FBConnector::get()->setup("119950698180257");
	return true;
}


bool onceExit = false;
// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
	// if you use SimpleAudioEngine, it must be pause
	CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	if(playInfo->gamePtr && !CCDirector::sharedDirector()->isPaused())
		playInfo->gamePtr->PAUSE(0);
	onceExit = true;
	CCDirector::sharedDirector()->pause();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
	if(!playInfo->gamePtr)
		CCDirector::sharedDirector()->resume();
	
	// 한번이라도 나갔다가 들어오면, 서버에서 다시 시간을 받음...
	if(onceExit)
	{
		if(playInfo->pIntroHead)
			playInfo->pIntroHead->syncTime();
	}
	// if you use SimpleAudioEngine, it must resume here
	CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();

}
