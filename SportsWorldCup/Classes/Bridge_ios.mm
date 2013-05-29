//
//  MultiLanguage_ios.mm
//  AngryKeeper
//
//  Created by ksoo k on 12. 9. 20..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>

#include "Bridge_ios.h"
//#import "AppDelegate.h"
//#import "CCApplication.h"
#import "AppController.h"
#import "RootViewController.h"
#import "cocos2d.h"
#import "EAGLView.h"
#import "GTMNSString+HTML.h"


//#import <Foundation/Foundation.h>

string multi_ios(const char* str)
{
	NSString* _s2 = [NSString stringWithUTF8String:str];
	NSString* s = NSLocalizedString(_s2, nil);
	//NSString* s = NSLocalizedString(@"f_review", nil);
	const char* _s1 = [s UTF8String];
	return string(_s1);
}

void showMoreApps_ios()
{
	NSURL *url = [NSURL URLWithString:@"itms-apps://phobos.apple.com/WebObjects/MZSearch.woa/wa/search?submit=sellAllLockup&media=software&entity=software&term=litqoo"];
    [[UIApplication sharedApplication] openURL:url];
}
void showReviewPage_ios()
{
	NSURL *url = [NSURL URLWithString:@"mailto:?subject=Let's_Play_ArcherWorldCup_togather&body=http://itunes.apple.com/app/id581650751?mt=8"];
    [[UIApplication sharedApplication] openURL:url];
}


@implementation KSWebView
-(void) hideWebView
{
	if(webView)
		[webView setHidden:YES];
}
-(void) showWebView
{
	if(webView)
		[webView setHidden:NO];
}
-(void) deleteWebView
{
	if(webView)
	{
		[webView removeFromSuperview];
		webView = nil;
	}
}

+(id) sharedWebView
{
    static KSWebView* _instance = nil;
    @synchronized(self)  // atomic
    {
        if(_instance == nil)
			_instance = [[KSWebView alloc] init];
		return _instance;
    }
}

-(id)init
{
    if( (self=[super init]))
    {
//        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardWasShown1:)
//													 name:UIKeyboardDidShowNotification object:nil];
//		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardWasShown2:)
//													 name:UIKeyboardWillShowNotification object:nil];

		//

    }
    return self;
}

- (void) dealloc
{
    [super dealloc];
}

-(void) clearTarget
{
	target = 0;
	selector = 0;
}
-(void) loadWebView:(NSString*)_url x:(int)x y:(int)y width:(int)width height:(int)height
{
	webView = [[UIWebView alloc] initWithFrame:CGRectMake(x, y, width, height)];
	webView.opaque = NO;
	webView.backgroundColor = [UIColor clearColor];
	webView.delegate = self;
	[webView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:_url]]];

	//[[[UIApplication sharedApplication] keyWindow] addSubview:webView];
	
	//[[[[UIApplication sharedApplication] delegate] window] addSubview:webView];
	
	[[EAGLView sharedEGLView] addSubview:webView];
	
	//[[[EAGLView sharedEGLView] window] addSubview:webView];
	
	[webView release];

	target = 0;
	//[[[CCDirector sharedDirector] openGLView] addSubview:webView];
}
-(void) loadWebView:(NSString*)_url x:(int)x y:(int)y width:(int)width height:(int)height
			 target:(CCObject*)_target selector:(RecvSelType)_selector
{
	webView = [[UIWebView alloc] initWithFrame:CGRectMake(x, y, width, height)];
	webView.opaque = NO;
	webView.backgroundColor = [UIColor clearColor];
	webView.delegate = self;

	[webView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:_url]]];
	
	//[[[UIApplication sharedApplication] keyWindow] addSubview:webView];
	
	//[[[[UIApplication sharedApplication] delegate] window] addSubview:webView];
	
	[[EAGLView sharedEGLView] addSubview:webView];
	
	//[[[EAGLView sharedEGLView] window] addSubview:webView];
	
	[webView release];
	target = _target;
	selector = _selector;
}

-(void)webViewDidFinishLoad:(UIWebView *)webView
{
	//NSString *html = [webView stringByEvaluatingJavaScriptFromString:@"document.getElementById('returnValue').value"];
	NSString *html = [webView stringByEvaluatingJavaScriptFromString:@"document.body.innerHTML"];

	html = [html gtm_stringByUnescapingFromHTML];
	//[html stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
	NSLog(@"%@", html);
	string _htmlSource = [html cStringUsingEncoding:NSUTF8StringEncoding];
	if(target && selector)
		(target->*selector)(_htmlSource);
	NSLog(@"Did");
	


}

-(void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error
{
    NSLog(@"fail");
}

@end

@implementation KSWebData
@synthesize receivedData;
+(id)ksWebData
{
	return [[[KSWebData alloc]init]autorelease];
}
-(id)init
{
    if( (self=[super init]))
    {

    }
    return self;
}

- (void) dealloc
{
    [super dealloc];
}
-(void)loadData:(NSString*)_url target:(CCObject*)_target selector:(RecvSelType)_selector
{
	target = _target;
	selector = _selector;
	
	NSURL *url = [NSURL URLWithString:_url];
	NSLog(@"%@", _url);
	NSURLRequest *request = [[NSURLRequest alloc] initWithURL:url];
	NSURLConnection *con = [[NSURLConnection alloc] initWithRequest:request
														   delegate:self];
	if(con) {
		NSMutableData *data = [[NSMutableData alloc] init];
		self.receivedData = data;
		[data release];
	}else {
		//예외 처리
	}
	[request release];
}
-(void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response {
	[receivedData setLength:0];
}
-(void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data {
	[receivedData appendData:data];
}
-(void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error {
	[connection release];
	self.receivedData = nil;
	(target->*selector)("");
}
-(void)connectionDidFinishLoading:(NSURLConnection *)connection {
	NSString* str = [[NSString alloc]initWithData:receivedData encoding:NSUTF8StringEncoding];
	string _s = [str cStringUsingEncoding:NSUTF8StringEncoding];
	[str release];
	if(target && selector)
	{
		(target->*selector)(_s);
	}
	[connection release];
	self.receivedData = nil;
}

-(void)clearTarget
{
	target = nil;
	selector = nil;
}

@end
void showWebView_ios(const char* url, int x, int y, int width, int height)
{
	NSString* _url = [NSString stringWithUTF8String:url];
	[[KSWebView sharedWebView] loadWebView:_url x:235 y:55 width:205 height:240];

}