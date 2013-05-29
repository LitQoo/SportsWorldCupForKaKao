//
//  SportsWorldCupAppController.mm
//  SportsWorldCup
//
//  Created by ksoo k on 13. 1. 22..
//  Copyright __MyCompanyName__ 2013년. All rights reserved.
//
#import <UIKit/UIKit.h>
#import "AppController.h"
#import "cocos2d.h"
#import "EAGLView.h"
#import "AppDelegate.h"

#import "RootViewController.h"

@implementation AppController
@synthesize window;
@synthesize viewController;

#pragma mark -
#pragma mark Application lifecycle

NSString *const FBSessionStateChangedNotification = @"com.litqoo.archervszombie:FBSessionStateChangedNotification";
// cocos2d application instance
static AppDelegate s_sharedApplication;
@synthesize facebook = _facebook;
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {

    // Override point for customization after application launch.

    // Add the view controller's view to the window and display.
    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
	
    EAGLView *__glView = [EAGLView viewWithFrame: [window bounds]
                                     pixelFormat: kEAGLColorFormatRGBA8
                                     depthFormat: GL_DEPTH_COMPONENT16_OES
                              preserveBackbuffer: NO
                                      sharegroup: nil
                                   multiSampling: NO
                                 numberOfSamples: 0 ];
	[__glView setMultipleTouchEnabled:YES];
	
    // Use RootViewController manage EAGLView
    viewController = [[RootViewController alloc] initWithNibName:nil bundle:nil];
    viewController.wantsFullScreenLayout = YES;
    viewController.view = __glView;
	
    // Set RootViewController to window
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
	{
		// warning: addSubView doesn't work on iOS6
		[window addSubview: viewController.view];
	}
	else
	{
		// use this mehod on ios6
		[window setRootViewController:viewController];
	}

    [window makeKeyAndVisible];

    [[UIApplication sharedApplication] setStatusBarHidden: YES];

    cocos2d::CCApplication::sharedApplication().run();
    return YES;
}


- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
	//cocos2d::CCDirector::sharedDirector()->pause();
	cocos2d::CCDirector::sharedDirector()->stopAnimation();
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
	cocos2d::CCDirector::sharedDirector()->startAnimation();
//	cocos2d::CCDirector::sharedDirector()->resume();
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
    cocos2d::CCApplication::sharedApplication().applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
    cocos2d::CCApplication::sharedApplication().applicationWillEnterForeground();
}

- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
}

#pragma mark - Facebook related methods
-(BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation
{
    NSLog(@"User session found333333");
    return [FBSession.activeSession handleOpenURL:url];
}
- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url{
    
    NSLog(@"User session found1111111111");
    return [FBSession.activeSession handleOpenURL:url];
}
- (void)sessionStateChanged:(FBSession *)session
                      state:(FBSessionState)state
                      error:(NSError *)error
{
    
    NSLog(@"User session found222222");
    
    switch (state) {
        case FBSessionStateOpen:
            if (!error) {
                // We have a valid session
                NSLog(@"User session found");
            }
            break;
        case FBSessionStateClosed:
        case FBSessionStateClosedLoginFailed:
            [FBSession.activeSession closeAndClearTokenInformation];
            break;
        default:
            break;
    }
    
    [[NSNotificationCenter defaultCenter] postNotificationName:FBSessionStateChangedNotification
                                                        object:session];
    
    if (error) {
        UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"Error"
                                                            message:error.localizedDescription
                                                           delegate:nil
                                                  cancelButtonTitle:@"OK"
                                                  otherButtonTitles:nil];
        [alertView show];
    }
}
- (void) dialogCompleteWithUrl:(NSURL*) url
{
	
	NSLog(@"test =>>%@",url);
    if ([url.absoluteString rangeOfString:@"post_id="].location != NSNotFound || [url.absoluteString rangeOfString:@"request="].location != NSNotFound) {
        //alert user of successful post
		if(fbDelekey>0) FBConnector::get()->sendRequestResult(FBSuccess,fbDelekey,fbRValue);
    } else {
        //user pressed "cancel"
		if(fbDelekey>0) FBConnector::get()->sendRequestResult(FBErrorCancel,fbDelekey,fbRValue);
    }
}

//- (void)dialogDidNotCompleteWithUrl:(NSURL *)url{
//	if(fbDelekey>0) FBConnector::get()->sendRequestResult(FBErrorETC,fbDelekey,fbRValue);
//}
//
//- (void)dialogDidComplete:(FBDialog *)dialog{
//	NSLog(@"dialog1 %@",dialog.params);
//    int delekey =[[dialog.params objectForKey:@"delekey"] intValue];
//    if(delekey>0) FBConnector::get()->sendRequestResult(FBSuccess,[[dialog.params objectForKey:@"delekey"] intValue],[[dialog.params objectForKey:@"returnValue"] cStringUsingEncoding:NSUTF8StringEncoding]);
//}
- (void)dialogDidNotComplete:(FBDialog *)dialog{
	NSLog(@"dialog2 %@",dialog.params);
    int delekey =[[dialog.params objectForKey:@"delekey"] intValue];
    if(delekey>0) FBConnector::get()->sendRequestResult(FBErrorCancel,[[dialog.params objectForKey:@"delekey"] intValue],[[dialog.params objectForKey:@"returnValue"] cStringUsingEncoding:NSUTF8StringEncoding]);
}
- (void)dialog:(FBDialog*)dialog didFailWithError:(NSError *)error{
	NSLog(@"dialog3 %@",dialog.params);
    int delekey =[[dialog.params objectForKey:@"delekey"] intValue];
    if(delekey>0) FBConnector::get()->sendRequestResult(FBErrorETC,[[dialog.params objectForKey:@"delekey"] intValue],[[dialog.params objectForKey:@"returnValue"] cStringUsingEncoding:NSUTF8StringEncoding]);
}

- (BOOL)openSessionWithAllowLoginUI:(BOOL)allowLoginUI {
    return [FBSession openActiveSessionWithReadPermissions:nil
                                              allowLoginUI:allowLoginUI
                                         completionHandler:^(FBSession *session,FBSessionState state,NSError *error) {
                                             
                                             [self sessionStateChanged:session state:state error:error];
                                         }];
}

#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
     cocos2d::CCDirector::sharedDirector()->purgeCachedData();
}


- (void)dealloc {
    [super dealloc];
}


@end

