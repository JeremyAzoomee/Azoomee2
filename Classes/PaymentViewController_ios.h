#import <UIKit/UIKit.h>
#import <StoreKit/StoreKit.h>

@interface PaymentViewController : UIViewController <SKProductsRequestDelegate, SKPaymentTransactionObserver>

@property (nonatomic, retain) SKProduct * oneMonthSubscription;

+ (id)sharedPayment_ios;

-(void)makeOneMonthPayment;
-(void)restorePayment;

@end
