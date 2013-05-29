//
//  StartOption.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 6..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//
#include "StartOption.h"
#include "NSDefault.h"
#include "Bridge.h"
#include "KSImageSelector.h"
#include "KSModalAlert.h"
#include "LoadingScene.h"
#include "SWIntro.h"

StartOption::StartOption()
{
	
}
StartOption::~StartOption()
{
	CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
	
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



void StartOption::registerWithTouchDispatcher()
{
	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
}
bool StartOption::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	CCTouch* touch = pTouch;

	CCPoint location(ccp(0, 0));
	location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
	location = convertToNodeSpace(location);
	nameTextField->detachWithIME();
	if(CCRect::CCRectContainsPoint(nameTextField->boundingBox(), location))
	{
		//CCLog("attach IME");
		nameTextField->attachWithIME();
	}
	
	if(CCRect::CCRectContainsPoint(country->boundingBox(), location))
	{
		KSImageSelector* selector = new KSImageSelector();
		selector->init();
		selector->setImageList(countryList);
		selector->loadImageList(this, image_selector(StartOption::imageClick));
		selector->show();
		
		//this->addChild(selector, INT_MAX);
		
		selector->release();
	}
	return true;
}

bool StartOption::init()
{
	CCLayer::init();
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("flags.plist");
	
	this->setIsTouchEnabled(true);
	this->setIsKeypadEnabled(true);
	CCSprite* back = CCSprite::spriteWithFile("main_back.png");
	back->setPosition(ccp(240, 160));
	addChild(back);
	
	myLayer = CCLayer::node();
	addChild(myLayer);
	CCSprite* titleBack = CCSprite::spriteWithFile("main_titleback.png");
	titleBack->setPosition(ccp(240, 200));
	titleBack->runAction(CCRepeatForever::actionWithAction(CCRotateBy::actionWithDuration(2.f*1.f/0.33f, 360.f)));
	myLayer->addChild(titleBack, 0);
	CCSprite* title = CCSprite::spriteWithFile("main_title.png");
	title->setPosition(ccp(240, 200));
	myLayer->addChild(title, 0);
	
	mini_back = CCSprite::spriteWithFile("miniconfig_back.png");
	mini_back->setPosition(ccp(243, 68));
	myLayer->addChild(mini_back, 1);
	
	CCMenu* _menu = CCMenu::menuWithItems(0);
	myLayer->addChild(_menu, 2);
	_menu->setPosition(ccp(0, 0));
	
	CCMenuItem* ok = CCMenuItemImage::itemFromNormalImage("miniconfig_ok.png", "miniconfig_ok.png", this, menu_selector(StartOption::OK));
	ok->setPosition(ccp(394, 69));
	_menu->addChild(ok, 3);
	
	
	
	CCSize FieldSize = CCSize( 150, 40 );
	
	nameTextField = CCTextFieldTTF::textFieldWithPlaceHolder( "<input here>", FieldSize, CCTextAlignmentLeft, defaultFont, 18 );
	nameTextField->setAnchorPoint(ccp(0, 0.5));
	nameTextField->setDelegate(this);
	myLayer->addChild( nameTextField, 999 );
	nameTextField->setPosition( ccp(170, 83) );
	nameTextField->setColor(ccc3(255, 255, 255));
	nameTextField->setString(NSDefault::getUserName().c_str());
	

	
	{
		const char* countries[] = {"ad",    "ae",    "af", "ag",    "ai",
			"al",    "am",    "an",    "ao",    "aq",    "ar",    "as",    "at",
			"au",    "aw",    "ax",    "az",    "ba",    "bb",    "bd",    "be",
			"bf",    "bg",    "bh",    "bi",    "bj",    "bl",    "bm",    "bn",
			"bo",    "br",    "bs",    "bt",    "bw",    "by",    "bz",    "ca",
			"cd",    "cf",    "cg",    "ch",    "ci",    "cl",    "cm",    "cn",
			"co",    "cr",    "cu",    "cv",    "cy",    "cz",    "de",    "dj",
			"dk",    "dm",    "do",    "dz",    "ec",    "ee",    "eg",    "eh",
			"eng",   "er",    "es",    "et",    "fi",    "fj",    "fk",    "fm",
			"fo",    "fr",    "ga",    "gb",    "gd",    "ge",    "gg",    "gh",
			"gl",    "gm",    "gn",    "gq",    "gr",    "gs",    "gt",    "gu",
			"gw",    "gy",    "hk",    "hn",    "hr",    "ht",    "hu",    "id",
			"ie",    "il",    "im",    "in",    "iq",    "ir",    "is",    "it",
			"je",    "jm",    "jo",    "jp",    "ke",    "kg",    "kh",    "ki",
			"km",    "kn",    "kp",    "kr",    "kv",    "kw",    "ky",    "kz",
			"la",    "lb",    "lc",    "li",    "lk",    "lr",    "ls",    "lt",
			"lu",    "lv",    "ly",    "ma",    "mc",    "md",    "me",    "mg",
			"mh",    "mk",    "ml",    "mm",    "mn",    "mo",    "mp",    "mr",
			"ms",    "mt",    "mu",    "mv",    "mw",    "mx",    "my",    "mz",
			"na",    "ne",    "nf",    "ng",    "ni",    "nl",    "no",    "np",
			"nr",    "nz",    "om",    "pa",    "pe",    "pg",    "ph",    "pk",
			"pl",    "pn",    "pr",    "ps",    "pt",    "pw",    "py",    "qa",
			"ro",    "rs",    "ru",    "rw",    "sa",    "sb",    "sc",    "sco",
			"sd",    "se",    "sg",    "sh",    "si",    "sk",    "sl",    "sm",
			"sn",    "so",    "sr",    "st",    "sv",    "sy",    "sz",    "tc",
			"td",    "tg",    "th",    "tj",    "tl",    "tm",    "tn",    "to",
			"tr",    "tt",    "tv",    "tw",    "tz",    "ua",    "ug",    "us",
			"uy",    "uz",    "vc",    "ve",    "vg",    "vi",    "vn",    "vt",
			"vu",    "wal",   "ws",    "ye",    "za",    "zm",    "zw"};
		
		for(int i=0; i<sizeof(countries) / sizeof(countries[0]); i++)
		{
			countryList.push_back(countries[i]);
		}
		
		string ret = "";
		ret += NSDefault::getCountry();
		ret += ".png";
		if(NSDefault::getCountry() == "")
			country = CCSprite::spriteWithFile("unknown_flag.png");
		else
			country = KS::spriteWithSpriteFrameName(ret.c_str());
		if(country)
		{
			country->setScale(1.3);
			country->setPosition( ccp(190, 50) );
			myLayer->addChild(country, 100);
		}
		
		//t.x , t.y ¿ßƒ°∞° ±π±‚.
	}
	
	return true;
}

void StartOption::OK(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	nameTextField->detachWithIME();
	
	if(NSDefault::getCountry() == "")
	{
		KSModalAlert("", ML::key("selectflag").c_str(), 1,
							  "ok", NOPARAM);
		return;
	}
	string name_utf8 = nameTextField->getString();
	wstring result;
	utf8::utf8to16(name_utf8.begin(), name_utf8.end(), back_inserter(result));
	//wstring name = KS_Util::utf(nameTextField->getString());
	if(result.length() > 8 || result.length() < 1)
	{
		KSModalAlert("", ML::key("nick_length").c_str(), 1,
							  "ok", NOPARAM);
	}
	else
	{
		string inputedText = nameTextField->getString();
		for(int i=0; i<inputedText.size(); i++)
		{
			//? & = | ; / \ " '
			char t = inputedText[i];
			if(t == '?' || t == '&' || t == '=' || t == '|' || t == ';' || t == '/' || t == '\\' || t == '"' || t == '\'')
				inputedText[i] = '_';
		}
		NSDefault::setUserName(inputedText);
//		sr<SWIntro>(0, SWIntro::STARTUP);
		ReplaceSceneParam(SWIntro, INIT_MODE::STARTUP);
//		LoadingScene<SWIntro, int>::scene(3);
//		int a = 3;
//		LoadingScene<SWIntro, a>::scene(a);
//		ReplaceScene(SWIntro);
	}
}
void StartOption::onEnterTransitionDidFinish()
{
	CCLayer::onEnterTransitionDidFinish();
}
void StartOption::onExit()
{
	
	
	CCLayer::onExit();
}

void StartOption::imageClick(char* flagcode)
{
	CCPoint pos = country->getPosition();
	country->removeFromParentAndCleanup(true);
	string fileExtension = flagcode;
	fileExtension += ".png";
	country = CCSprite::spriteWithSpriteFrameName(fileExtension.c_str());
	country->setScale(1.3);
	country->setPosition(pos); // pos
	myLayer->addChild(country, 100);
	NSDefault::setCountry(flagcode);
}



