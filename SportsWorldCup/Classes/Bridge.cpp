//
//  MultiLanguage.cpp
//  AngryKeeper
//
//  Created by ksoo k on 12. 9. 20..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//
#include "Bridge.h"
//#include "Bridge_ios.h"
//#import "Bridge_ios.h"
#include "Bridge_ios.h"
//#import "AppController.h"
#include "JNIKelper.h"
#include "cocos2d.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
const char* defaultFont = "";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
const char* defaultFont = "AppleGothic";
#endif

string ML::key(const string &k)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	return JNIKelper::getInstance()->callJava_getUTF8String(k);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	return multi_ios(k.c_str());
#endif	
}


void showMoreApps()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JNIKelper::getInstance()->callJava_simple("showMoreApps");
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS	
	showMoreApps_ios();
#endif	
}

void showReviewPage()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JNIKelper::getInstance()->callJava_simple("showReview");
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS	
	showReviewPage_ios();
#endif	
}



void moreapps()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	NSURL *url = [NSURL URLWithString:@"itms-apps://phobos.apple.com/WebObjects/MZSearch.woa/wa/search?submit=sellAllLockup&media=software&entity=software&term=litqoo"];
	[[UIApplication sharedApplication] openURL:url];
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JNIKelper::getInstance()->callJava_simple("showMoreApps");
	//JNIKelper::getInstance()->callJava_buyItem("100ruby", this, recvInApp_selector(BasketIntro::test));
#endif
}
void write_review()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	NSURL *url = [NSURL URLWithString:@"itms-apps://itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?id=595879505&onlyLatestVersion=true&pageNumber=0&sortOrdering=1&type=Purple+Software"];
	[[UIApplication sharedApplication] openURL:url];
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JNIKelper::getInstance()->callJava_simple("showReview");
#endif
}
void otherApp(string appcode)
{
	
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	NSString* appcode_ios = [NSString stringWithCString:appcode.c_str() encoding:NSUTF8StringEncoding];
	NSString* urlstr = [NSString stringWithFormat:@"itms-apps://phobos.apple.com/WebObjects/MZStore.woa/wa/viewSoftware?id=%@&mt=8", appcode_ios];
	NSURL *url = [NSURL URLWithString:urlstr];
	[[UIApplication sharedApplication] openURL:url];
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JNIKelper::getInstance()->callJava_otherApp(appcode);
#endif
}