#import <UIKit/UIKit.h>
#import <StoreKit/StoreKit.h>

@interface PaymentViewController : UIViewController <SKProductsRequestDelegate, SKPaymentTransactionObserver>

@property (nonatomic, retain) NSString* oneMonthSubscriptionID;
@property (nonatomic, retain) SKProduct* oneMonthSubscription;
@property (assign) BOOL purchaseAfterQuery;

+ (id)sharedPayment_ios;

-(void)makeOneMonthPayment;

-(void)restorePayment;
-(void)startProductPriceQuery;

@end
