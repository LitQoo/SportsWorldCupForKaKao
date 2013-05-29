//
//  RubyShop.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 11..
//
//

#include "RubyShop.h"
#include "KS_Util.h"
#include "JNIKelper.h"
#include "Bridge.h"
#include "NSDefault.h"
#include "Str2Token.h"
#include "KSoundEngine.h"
#include "KSModalAlert.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#import "InApp.h"
#endif
#include "GraphDog.h"
#include "DisableTouch.h"

RubyShop::RubyShop()
{
	position.pier[0] = ccp(71, 199);
	position.pier[1] = ccp(192, 199);
	position.pier[2] = ccp(71, 124);
	position.pier[3] = ccp(192, 124);
	position.pier[4] = ccp(71, 50);
	position.pier[5] = ccp(192, 50);
	position.pierPrice[0] = ccp(80, 181);
	position.pierPrice[1] = ccp(200, 181);
	position.pierPrice[2] = ccp(80, 107);
	position.pierPrice[3] = ccp(200, 107);
	position.pierPrice[4] = ccp(80, 32);
	position.pierPrice[5] = ccp(200, 32);
	
	cashs["pier1"] = 10;
	cashs["pier2"] = 25;
	cashs["pier3"] = 40;
	cashs["pier4"] = 90;
	cashs["pier5"] = 200;
	cashs["pier6"] = 750;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	[[InApp sharedInstance] setDelegate:this];
#endif
}

RubyShop::~RubyShop()
{
	URLDelegator::getInstance()->removeTarget(this);
	InAppDelegator::getInstance()->removeTarget(this);
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	[[InApp sharedInstance] setDelegate:0];
#endif
}
bool RubyShop::init()
{
	CCSprite::init();
	CCSprite* back = CCSprite::spriteWithFile("shop_rubyback.png");
	
	addChild(back);
	thiz = back;
	
	CCMenu* _menu = CCMenu::menuWithItems(0);
	_menu->setPosition(CCPointZero);
	thiz->addChild(_menu);
	
	CCMenuItem* close = CCMenuItemImage::itemFromNormalImage("main_exit.png", "main_exit.png", this, menu_selector(ThisClassType::CLOSE));
	close->setPosition(ccp(234, 263));
	_menu->addChild(close, 2);
	
	for(int i=0; i<6; i++)
	{
		string filename = KS_Util::stringWithFormat("shop_ruby%d.png", i+1);
		
		CCMenuItem* item;
		switch(i)
		{
			case 0:
				item = CCMenuItemImage::itemFromNormalImage(filename.c_str(), filename.c_str(),
															this, menu_selector(ThisClassType::PIER1));
				break;
			case 1:
				item = CCMenuItemImage::itemFromNormalImage(filename.c_str(), filename.c_str(),
															this, menu_selector(ThisClassType::PIER2));
				break;
			case 2:
				item = CCMenuItemImage::itemFromNormalImage(filename.c_str(), filename.c_str(),
															this, menu_selector(ThisClassType::PIER3));
				break;
			case 3:
				item = CCMenuItemImage::itemFromNormalImage(filename.c_str(), filename.c_str(),
															this, menu_selector(ThisClassType::PIER4));
				break;
			case 4:
				item = CCMenuItemImage::itemFromNormalImage(filename.c_str(), filename.c_str(),
															this, menu_selector(ThisClassType::PIER5));
				break;
			case 5:
				item = CCMenuItemImage::itemFromNormalImage("option_giftcode.png", "option_giftcode.png",
															this, menu_selector(ThisClassType::PIER6));
				break;
			
		}			
		
		item->setPosition(position.pier[i]);
		_menu->addChild(item, 2);
		
	}
	
	
	const char* fontName = defaultFont;
	
	CCLabelTTF* _1 = CCLabelTTF::labelWithString("$ 0.99", fontName, 20);
	_1->setPosition(position.pierPrice[0]);
	_1->setColor(ccc3(255, 255, 255));
	thiz->addChild(_1, 2);
	
	CCLabelTTF* _2 = CCLabelTTF::labelWithString("$ 1.99", fontName, 20);
	_2->setPosition(position.pierPrice[1]);
	_2->setColor(ccc3(255, 255, 255));
	thiz->addChild(_2, 2);
	
	CCLabelTTF* _3 = CCLabelTTF::labelWithString("$ 2.99", fontName, 20);
	_3->setPosition(position.pierPrice[2]);
	_3->setColor(ccc3(255, 255, 255));
	thiz->addChild(_3, 2);
	
	CCLabelTTF* _4 = CCLabelTTF::labelWithString("$ 4.99", fontName, 20);
	_4->setPosition(position.pierPrice[3]);
	_4->setColor(ccc3(255, 255, 255));
	thiz->addChild(_4, 2);
	
	CCLabelTTF* _5 = CCLabelTTF::labelWithString("$ 9.99", fontName, 20);
	_5->setPosition(position.pierPrice[4]);
	_5->setColor(ccc3(255, 255, 255));
	thiz->addChild(_5, 2);
	
//	CCLabelTTF* _6 = CCLabelTTF::labelWithString("$ 5.99", fontName, 20);
//	_6->setPosition(position.pierPrice[5]);
//	_6->setColor(ccc3(255, 255, 255));
//	thiz->addChild(_6, 2);
	
//	JsonBox::Value jsonValue;
//	jsonValue.loadFromString(prices);
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	string prices = JNIKelper::getInstance()->callJava_getPrice("pier1/pier2/pier3/pier4/pier5/pier6");
	JsonBox::Value jsonValue;
	jsonValue.loadFromString(prices);
	_1->setString(jsonValue["pier1"].getString().c_str());
	_2->setString(jsonValue["pier2"].getString().c_str());
	_3->setString(jsonValue["pier3"].getString().c_str());
	_4->setString(jsonValue["pier4"].getString().c_str());
	_5->setString(jsonValue["pier5"].getString().c_str());
//	_6->setString(jsonValue["pier6"].getString());
//	Str2Token tok(prices, "/");
//	vector<string> vPrices;
//	while(tok.hasToken())
//	{
//		vPrices.push_back(tok.nextToken());
//	}
//	if(vPrices.size() >= 6)
//	{
//		_1->setString(vPrices[0].c_str());
//		_2->setString(vPrices[1].c_str());
//		_3->setString(vPrices[2].c_str());
//		_4->setString(vPrices[3].c_str());
//		_5->setString(vPrices[4].c_str());
//		//_6->setString(vPrices[5].c_str());
//		//_5->setString(vPrices[4].c_str());
//	}
	
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	
#endif
	
	return true;
}
void RubyShop::finishedPurchase(string _itemName, int _resultCode)
{
	string itemName = _itemName;
	if(itemName == "pier1")
	{
		NSDefault::setRuby(NSDefault::getRuby() + cashs[itemName]);
	}
	else if(itemName == "pier2")
	{
		NSDefault::setRuby(NSDefault::getRuby() + cashs[itemName]);
	}
	else if(itemName == "pier3")
	{
		NSDefault::setRuby(NSDefault::getRuby() + cashs[itemName]);

	}
	else if(itemName == "pier4")
	{
		NSDefault::setRuby(NSDefault::getRuby() + cashs[itemName]);
	}
	else if(itemName == "pier5")
	{
		NSDefault::setRuby(NSDefault::getRuby() + cashs[itemName]);
	}
	else if(itemName == "pier6")
	{
		NSDefault::setRuby(NSDefault::getRuby() + cashs[itemName]);
	}


	mediator->setRuby(NSDefault::getRuby());
	JsonBox::Object gParam;
	gParam["category"] = "inapp";
	gParam["log"] = itemName;
	graphdog->command("writelog", &gParam, 0, 0);
	KS::KSLog("itemName % resultCode %", itemName.c_str(), _resultCode);
}

void RubyShop::finishProcess()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	if(disableTouch)
	{
		disableTouch->removeFromParentAndCleanup(true);
		disableTouch = 0;
	}
	
	if(loading)
	{
		loading->removeFromParentAndCleanup(true);
		loading = 0;
	}
#else
	// nothing do to.
#endif
}
void RubyShop::PIER1(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JNIKelper::getInstance()->callJava_buyItem("pier1", this, recvInApp_selector(ThisClassType::finishedPurchase));
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	if([SKPaymentQueue canMakePayments])
	{
		showLoading();
		[[SKPaymentQueue defaultQueue] addTransactionObserver:[InApp sharedInstance]];
		SKProductsRequest *request= [[SKProductsRequest alloc] initWithProductIdentifiers: [NSSet setWithObject: @"com.litqoo.SportsWorldCup.bpier1"]];
		request.delegate = [InApp sharedInstance];
		[request start];
	}
#endif
}
void RubyShop::PIER2(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JNIKelper::getInstance()->callJava_buyItem("pier2", this, recvInApp_selector(ThisClassType::finishedPurchase));
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		if([SKPaymentQueue canMakePayments])
		{
			showLoading();
			[[SKPaymentQueue defaultQueue] addTransactionObserver:[InApp sharedInstance]];
			SKProductsRequest *request= [[SKProductsRequest alloc] initWithProductIdentifiers: [NSSet setWithObject: @"com.litqoo.SportsWorldCup.pier2"]];
			request.delegate = [InApp sharedInstance];
			[request start];
		}
#endif
}
void RubyShop::PIER3(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JNIKelper::getInstance()->callJava_buyItem("pier3", this, recvInApp_selector(ThisClassType::finishedPurchase));
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	if([SKPaymentQueue canMakePayments])
	{
		showLoading();
		[[SKPaymentQueue defaultQueue] addTransactionObserver:[InApp sharedInstance]];
		SKProductsRequest *request= [[SKProductsRequest alloc] initWithProductIdentifiers: [NSSet setWithObject: @"com.litqoo.SportsWorldCup.pier3"]];
		request.delegate = [InApp sharedInstance];
		[request start];
	}

#endif
}
void RubyShop::PIER4(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JNIKelper::getInstance()->callJava_buyItem("pier4", this, recvInApp_selector(ThisClassType::finishedPurchase));
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	if([SKPaymentQueue canMakePayments])
	{
		showLoading();
		[[SKPaymentQueue defaultQueue] addTransactionObserver:[InApp sharedInstance]];
		SKProductsRequest *request= [[SKProductsRequest alloc] initWithProductIdentifiers: [NSSet setWithObject: @"com.litqoo.SportsWorldCup.pier4"]];
		request.delegate = [InApp sharedInstance];
		[request start];
	}

#endif
}
void RubyShop::PIER5(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JNIKelper::getInstance()->callJava_buyItem("pier5", this, recvInApp_selector(ThisClassType::finishedPurchase));
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	if([SKPaymentQueue canMakePayments])
	{
		showLoading();
		[[SKPaymentQueue defaultQueue] addTransactionObserver:[InApp sharedInstance]];
		SKProductsRequest *request= [[SKProductsRequest alloc] initWithProductIdentifiers: [NSSet setWithObject: @"com.litqoo.SportsWorldCup.pier5"]];
		request.delegate = [InApp sharedInstance];
		[request start];
	}

#endif
}

void RubyShop::showLoading()
{
	DisableTouch* cl = DisableTouch::node(ccc4f(0, 0, 0, 180));
	disableTouch = cl;
	cl->setAnchorPoint(ccp(0.5, 0.5));
	cl->setPosition(ccp(240, 160));
	getParent()->addChild(cl, INT_MAX );
	
	float loadingX = 240;
	float loadingY = 150;//160;
	auto animator = SceneUtil::playAnimation("loading.png", 0.1, 9, 9, 56, 56, true);
	animator.second->runAction(animator.first);
	getParent()->addChild(animator.second, INT_MAX);
	animator.second->setPosition(ccp(loadingX, loadingY));
	loading = animator.second;
	loading->setScale(1.f);
}
void RubyShop::PIER6(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	// 기프트 코드...
	
	mediator->showGiftCode();
}