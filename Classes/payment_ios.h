#import <UIKit/UIKit.h>
#import <StoreKit/StoreKit.h>

@interface payment_ios : UIViewController <SKProductsRequestDelegate, SKPaymentTransactionObserver>

@property (nonatomic, retain) SKProduct * oneMonthSubscription;

+ (id)sharedPayment_ios;

-(void)makeOneMonthPayment;
-(void)restorePayment;

@end
