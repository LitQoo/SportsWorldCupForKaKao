//
//  MultiLanguage_ios.h
//  AngryKeeper
//
//  Created by ksoo k on 12. 9. 20..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#ifndef AngryKeeper_MultiLanguage_ios_h
#define AngryKeeper_MultiLanguage_ios_h

#include "cocos2d.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS



#include <string>
#include "URLDelegator.h"

using namespace std;
string multi_ios(const char* str);

void showMoreApps_ios();
void showReviewPage_ios();
void showWebView_ios(const char* url, int x, int y, int width, int height);

#import <Foundation/Foundation.h>
@interface KSWebView : NSObject <UIWebViewDelegate>{
    UIWebView* webView;
	CCObject* target;
	RecvSelType selector;
}
+(id) sharedWebView;
-(void) loadWebView:(NSString*)_url x:(int)x y:(int)y width:(int)width height:(int)height;
-(void) loadWebView:(NSString*)_url x:(int)x y:(int)y width:(int)width height:(int)height target:(CCObject*)_target selector:(RecvSelType)_selector;
-(void) deleteWebView;
-(void) hideWebView;
-(void) showWebView;
-(void) clearTarget;
@end

@interface KSWebData : NSObject
{
	NSMutableData* receivedData;
	CCObject* target;
	RecvSelType selector;
}
@property (retain, nonatomic) NSMutableData* receivedData;
//CCObject* _target, RecvSelType _selector)
+(id)ksWebData;
-(void)loadData:(NSString*)_url target:(CCObject*)_target selector:(RecvSelType)_selector;
-(void)clearTarget;
@end
#endif

#endif