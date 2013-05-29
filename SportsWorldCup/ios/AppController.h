//
//  SportsWorldCupAppController.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 1. 22..
//  Copyright __MyCompanyName__ 2013년. All rights reserved.
//

#import "Facebook.h"
#import "FBConnector.h"

@class RootViewController;
extern NSString *const FBSessionStateChangedNotification;



@interface AppController : NSObject <FBDialogDelegate,UIAccelerometerDelegate, UIAlertViewDelegate, UITextFieldDelegate,UIApplicationDelegate> {
@public
	string fbRValue;
	int fbDelekey;
	
@protected
	UIWindow *window;
    RootViewController	*viewController;
}
@property (retain, nonatomic) RootViewController* viewController;
@property (nonatomic, retain) Facebook *facebook;
@property   (retain,nonatomic)  UIWindow *window;
@end

