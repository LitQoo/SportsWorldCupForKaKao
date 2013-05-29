//
//  InApp.m
//  BasketWorldCup2
//
//  Created by ksoo k on 13. 1. 21..
//
//

#import "InApp.h"
#import "VerificationController.h"
#include "RubyShop.h"

static InApp* singleton;
@implementation InApp




+ (InApp *)sharedInstance
{
	if (singleton == nil)
    {
		singleton = [[InApp alloc] init];
	}
	return singleton;
}

-(void)setDelegate:(RubyShop*)delegate
{
	rubyLayer = delegate;
}
-(void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
	for(SKProduct *product in response.products){
		
		/*
		 localizedDescription --> 아이템 설명
		 localizedTitle --> 아이템 이름
		 price  ->> 아이템 가격
		 priceLocale  -> 지역별 가격표시 ex) $,원 표시등..
		 productIdentifier -->제품식별코드
		 
		 아래와 같이 product.xxxxx 로 접근해서 화면에 뿌려주시면 되실것입니다.
		 product.localizedTitle
		 */
		NSLog(@"%@", [product productIdentifier]);
		SKPayment *payment = [SKPayment paymentWithProduct:product];
		[[SKPaymentQueue defaultQueue] addPayment:payment];
	}
	[request release];
}

-(void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
	//[[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
	//[buyButton setVisible:YES];
	for(SKPaymentTransaction* transaction in transactions)
	{
		switch(transaction.transactionState)
		{
			case SKPaymentTransactionStatePurchasing:
				// 구매 진행중.
				break;
			case SKPaymentTransactionStatePurchased:
				// 구매됐다.
				// transaction.payment.productIdentifier = 구매한 항목 id
				//NSLog(@"%@ is purchased", transaction.payment.productIdentifier);

				if([[VerificationController sharedInstance] verifyPurchase:transaction])
				{
					if([transaction.payment.productIdentifier isEqualToString:@"com.litqoo.SportsWorldCup.bpier1"])
					{
						rubyLayer->finishedPurchase("pier1", 0);
						//rubyLayer->finishedPurchase("10ruby", 0);
					}
					else if([transaction.payment.productIdentifier isEqualToString:@"com.litqoo.SportsWorldCup.pier2"])
					{
						rubyLayer->finishedPurchase("pier2", 0);
						//rubyLayer->finishedPurchase("25ruby", 0);
					}
					else if([transaction.payment.productIdentifier isEqualToString:@"com.litqoo.SportsWorldCup.pier3"])
					{
						rubyLayer->finishedPurchase("pier3", 0);
						//rubyLayer->finishedPurchase("40ruby", 0);
					}
					else if([transaction.payment.productIdentifier isEqualToString:@"com.litqoo.SportsWorldCup.pier4"])
					{
						rubyLayer->finishedPurchase("pier4", 0);
						//rubyLayer->finishedPurchase("b200ruby", 0);
					}
					else if([transaction.payment.productIdentifier isEqualToString:@"com.litqoo.SportsWorldCup.pier5"])
					{
						rubyLayer->finishedPurchase("pier5", 0);
						//rubyLayer->finishedPurchase("free", 0);
					}
					else if([transaction.payment.productIdentifier isEqualToString:@"com.litqoo.SportsWorldCup.pier6"])
					{
						rubyLayer->finishedPurchase("pier6", 0);
						//rubyLayer->finishedPurchase("free", 0);
					}
					
					if(rubyLayer)
					{
						//rubyLayer->hideLoading();
					}
					UIAlertView *alert =
					[[[UIAlertView alloc]
					  initWithTitle:@"Congratulation"
					  message:@"Purchase was completed successfully"
					  delegate:nil
					  cancelButtonTitle:@"CONFIRM"
					  otherButtonTitles:nil]autorelease];
					[alert show];
					NSLog(@"%@ is purchased", transaction.payment.productIdentifier);
				}
				[[SKPaymentQueue defaultQueue] finishTransaction:transaction];
				
				rubyLayer->finishProcess();
				break;
			case SKPaymentTransactionStateRestored:
				// 이미 지불했음.
				[[SKPaymentQueue defaultQueue] finishTransaction:transaction];
				rubyLayer->finishProcess();
				break;
			case SKPaymentTransactionStateFailed:
				// 구매를 취소했거나 에러발생.
				//if(transaction.error.code != SKErrorPaymentCancelled)
			{
				// 구매 과정중 오류가 발생했으니 사용자에게 알린다.
				UIAlertView *alert =
									  [[[UIAlertView alloc]
									   initWithTitle:@"ERROR"
									   message:@"Try purchasing again"
									   delegate:nil
									   cancelButtonTitle:@"OK"
									   otherButtonTitles:nil]autorelease];
				[alert show];
				[[SKPaymentQueue defaultQueue] finishTransaction:transaction];
				rubyLayer->finishProcess();
//				if(rubyLayer)
//				{
//					rubyLayer->hideLoading();
//				}
				break;
			}
				
		}
	}
}



@end
