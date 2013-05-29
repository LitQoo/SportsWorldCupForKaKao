//
//  SportsWorldCupAppController.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 1. 22..
//  Copyright __MyCompanyName__ 2013년. All rights reserved.
//

#import "RootViewController.h"

#import "CCApplication.h"

#include "cocos2d.h"
USING_NS_CC;
#include "GameSystem.h"

@implementation RootViewController

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/

/*
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
}
 
*/

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
	return UIInterfaceOrientationIsLandscape( interfaceOrientation );
    
    // switch to this line if you want to set portrait view
    // return UIInterfaceOrientationIsPortrait( interfaceOrientation );
}
- (NSUInteger) supportedInterfaceOrientations{
	return UIInterfaceOrientationMaskLandscape;
}

- (BOOL) shouldAutorotate {
	return YES;
}


- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
}


- (void) stopActionsForAd
{
	//Stop Director
	//[[EAGLView sharedEGLView] stopAnimating];
	//	[[CCDirector sharedDirector] stopAnimation];
	CCDirector::sharedDirector()->pause();
}

- (void) startActionsForAd
{
	CCDirector::sharedDirector()->resume();
	//	if(CCDirector::sharedDirector()->isPaused())
	//	{
	//		cocos2d::CCDirector::sharedDirector()->resume();
	//		removeChild(blackBack, true);
	//	}
	//	else
	//	{
	//
	//		cocos2d::CCDirector::sharedDirector()->pause();
	//	}
}

#define ADMOB_PUBLISHER_ID @"a150e0ed7d99fbc"


///////////////////////////////////////////////////////////////////////////////////////////////

@end
