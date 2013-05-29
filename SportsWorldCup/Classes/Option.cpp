//
//  Option.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 2. 15..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//
#include "Option.h"
#include "KSModalAlert.h"
#include "Bridge.h"
#include "NSDefault.h"
#include "KSImageSelector.h"
#include "KSoundEngine.h"
#include "KS_Util.h"
#include "GraphDog.h"
#include "KSoundEngine.h"




Option::~Option()
{
	CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
}

void Option::registerWithTouchDispatcher()
{
	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, false);
}
bool Option::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	CCTouch* touch = pTouch;

	CCPoint location(ccp(0, 0));
	location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
	location = thiz->convertToNodeSpace(location);
	nameTextField->detachWithIME();
	if(CCRect::CCRectContainsPoint(nameTextField->boundingBox(), location))
	{
		//CCLog("attach IME");
		nameTextField->attachWithIME();
	}
	if(country)
	{
		if(CCRect::CCRectContainsPoint(country->boundingBox(), location))
		{
			KSImageSelector* selector = new KSImageSelector();
			selector->init();
			selector->setImageList(countryList);
			selector->loadImageList(this, image_selector(Option::imageClick));
			selector->show();
			
			//this->addChild(selector, INT_MAX);
			
			selector->release();
		}
	}
	
	
	if(CCRect::CCRectContainsPoint(effectSound->boundingBox(),location))
	{
		// EFFECT ªÁøÓµÂ ±◊∏≤ ≈¨∏Ø
		KSoundEngine::sharedEngine()->playSound("select.mp3");
		CCPoint prevPos = effectSound->getPosition();
		CCPoint prevAnc = effectSound->getAnchorPoint();
		if(NSDefault::getEffectSound())
		{
			NSDefault::setEffectSound(false);
			
			effectSound->removeFromParentAndCleanup(true);
			effectSound = CCSprite::spriteWithFile("sound_off.png");
			effectSound->setAnchorPoint(prevAnc);
			effectSound->setPosition ( prevPos );
			
			thiz->addChild(effectSound, 100);
			KSoundEngine::sharedEngine()->setIsEffect(false);
		}
		else {
			NSDefault::setEffectSound(true);
			effectSound->removeFromParentAndCleanup(true);
			effectSound = CCSprite::spriteWithFile("sound_on.png");
			effectSound->setAnchorPoint(prevAnc);
			effectSound->setPosition (prevPos);
			thiz->addChild(effectSound, 100);
			KSoundEngine::sharedEngine()->setIsEffect(true);
		}
	}
	
	if(CCRect::CCRectContainsPoint(BGM->boundingBox(),location))
	{
		// BGM ªÁøÓµÂ ±◊∏≤ ≈¨∏Ø
		KSoundEngine::sharedEngine()->playSound("select.mp3");
		CCPoint prevPos = BGM->getPosition();
		CCPoint prevAnc = BGM->getAnchorPoint();
		if(NSDefault::getBGM())
		{
			NSDefault::setBGM(false);
			BGM->removeFromParentAndCleanup(true);
			BGM = CCSprite::spriteWithFile("sound_off.png");
			BGM->setAnchorPoint(prevAnc);
			BGM->setPosition(prevPos);
			
			thiz->addChild(BGM, 100);
			KSoundEngine::sharedEngine()->setIsBGM(false);
			KSoundEngine::sharedEngine()->stopBackground();
		}
		else {
			NSDefault::setBGM(true);
			// it was doubt,			[this removeChild:BGM cleanup:true];
			
			BGM->removeFromParentAndCleanup(true);
			BGM = CCSprite::spriteWithFile("sound_on.png");
			BGM->setAnchorPoint(prevAnc);
			BGM->setPosition(prevPos);
			thiz->addChild(BGM, 100);
			KSoundEngine::sharedEngine()->setIsBGM(true);
			KSoundEngine::sharedEngine()->playSound("RetroSpace.mp3");
//			KSoundEngine::sharedEngine()->setBackgroundVolume(1.2);
			
		}
	}

	
	
	return true;
}
void Option::GIFTCODE(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	mediator->showGiftCode();
}
bool Option::init()
{
	CCLayerColor::initWithColor(ccc4f(0,0, 0, 0));
	this->setIsTouchEnabled(true);
	
	thiz = CCSprite::spriteWithFile("option_back.png");
	addChild(thiz);
	
	CCMenu* _menu = CCMenu::menuWithItems(0);
	_menu->setPosition(ccp(0, 0));
	thiz->addChild(_menu);
	
	CCLabelTTF* app_version = CCLabelTTF::labelWithString(string("version : " + graphdog->getAppVersionString()).c_str(), defaultFont, 12.f);
	app_version->setColor(ccc3(0, 0, 0));
	app_version->setPosition(ccp(180, 240));
	thiz->addChild(app_version, 100);
	
	CCMenuItem* close = CCMenuItemImage::itemFromNormalImage("main_exit.png", "main_exit.png", this, menu_selector(Option::CLOSE));
	close->setPosition(ccp(234, 263));
	_menu->addChild(close, 2);
	
	CCMenuItem* giftCode = CCMenuItemImage::itemFromNormalImage("option_giftcode.png", "option_giftcode.png",
												this, menu_selector(ThisClassType::GIFTCODE));
	giftCode->setPosition(ccp(73, 64));
	_menu->addChild(giftCode, 2);
	
	CCMenuItem* ilikeit = CCMenuItemImage::itemFromNormalImage("option_ilikeit.png", "option_ilikeit.png", this, menu_selector(Option::ILIKEIT));
	ilikeit->setPosition(ccp(192, 64));
	_menu->addChild(ilikeit, 2);
	
	CCSize FieldSize = CCSize( 150, 40 );
	
	nameTextField = CCTextFieldTTF::textFieldWithPlaceHolder( "<input here>", FieldSize, CCTextAlignmentLeft, defaultFont, 18 );
	nameTextField->setAnchorPoint(ccp(0, 0.5));
	nameTextField->setDelegate(this);
	thiz->addChild( nameTextField, 999 );
	nameTextField->setPosition( ccp(111, 209) );
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
		
		string ret = NSDefault::getCountry() + ".png";

		if(NSDefault::getCountry() == "")
			country = CCSprite::spriteWithFile("unknown_flag.png");
		else
			country = KS::spriteWithSpriteFrameName(ret.c_str());
		if(country)
		{
			country->setScale(1.3);
			country->setPosition( ccp(127, 175) );
			thiz->addChild(country, 100);
		}
		
		//t.x , t.y ¿ßƒ°∞° ±π±‚.
	}
	if(NSDefault::getEffectSound())
	{
		effectSound = CCSprite::spriteWithFile("sound_on.png");
	}
	else {
		effectSound = CCSprite::spriteWithFile("sound_off.png");
	}
	effectSound->setPosition(ccp(109, 132));
	
	thiz->addChild(effectSound, 100);
	
	if(NSDefault::getBGM())
	{
		BGM = CCSprite::spriteWithFile("sound_on.png");
	}
	else {
		BGM = CCSprite::spriteWithFile("sound_off.png");
	}
	//BGM->setAnchorPoint(ccp(0,0.5));
	BGM->setPosition(ccp(213, 132));
	// it was doubt,		[this addChild:BGM z:100];
	
	thiz->addChild(BGM, 100);
	return true;
}
void Option::CLOSE(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	nameTextField->detachWithIME();
	
	mediator->showRank();
}
bool Option::getIsValidName()
{
	string name_utf8 = nameTextField->getString();
	wstring result;
	utf8::utf8to16(name_utf8.begin(), name_utf8.end(), back_inserter(result));
	//wstring name = KS_Util::utf(nameTextField->getString());
	if(result.length() > 8 || result.length() < 1)
	{
//		KSModalAlert("", ML::key("nick_length").c_str(), 1,
//							  "ok", nil, nil);
		return false;
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
		GraphDog::get()->setNick(NSDefault::getUserName());
		return true;
	}
}

void Option::alertInvalidName()
{
	KSModalAlert("", ML::key("nick_length").c_str(), 1,
						  "ok", NOPARAM);

}
void Option::onEnterTransitionDidFinish()
{
	CCLayerColor::onEnterTransitionDidFinish();
}
void Option::onExit()
{
	
	
	CCLayerColor::onExit();
}



void Option::ILIKEIT(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	KSModalAlert("", ML::key("review").c_str(), 2,
				 "NO", (CCObject*)0, (SEL_CallFunc)0,
				 "YES", this, callfunc_selector(ThisClassType::REVIEW_YES));
	
}

void Option::REVIEW_YES()
{
	write_review();
	NSDefault::setIsReview(1);
}
void Option::EXIT(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	KS::KSLog("%", __FUNCTION__);
}

void Option::imageClick(char* flagcode)
{
	CCPoint pos = country->getPosition();
	country->removeFromParentAndCleanup(true);
	string fileExtension = flagcode;
	fileExtension += ".png";
	country = CCSprite::spriteWithSpriteFrameName(fileExtension.c_str());
	country->setScale(1.3);
	country->setPosition(pos);
	thiz->addChild(country, 100);
	NSDefault::setCountry(flagcode);
	
	GraphDog::get()->setFlag(NSDefault::getCountry());
	
}





