#import <UIKit/UIKit.h>
#import <StoreKit/StoreKit.h>

@interface payment_ios : NSObject <SKProductsRequestDelegate, SKPaymentTransactionObserver>

@property (nonatomic, retain) SKProduct * oneMonthSubscription;

-(void)makeOneMonthPayment;

@end
