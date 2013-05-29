//
//  InApp.h
//  BasketWorldCup2
//
//  Created by ksoo k on 13. 1. 21..
//
//

#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>
#import "VerificationController.h"


class RubyShop;

@interface InApp : NSObject<SKProductsRequestDelegate, SKPaymentTransactionObserver>
{
	RubyShop* rubyLayer;
}
+ (InApp *) sharedInstance;
-(void)setDelegate:(RubyShop*)delegate;
@end
