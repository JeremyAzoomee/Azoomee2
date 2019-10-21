#import <UIKit/UIKit.h>
#import <StoreKit/StoreKit.h>

@interface PaymentViewController : UIViewController <SKProductsRequestDelegate, SKPaymentTransactionObserver>

@property (nonatomic, retain) NSString* oneMonthSubscriptionID;
@property (nonatomic, retain) SKProduct* oneMonthSubscription;
@property (assign) BOOL purchaseAfterQuery;
#if defined(AZOOMEE_ENVIRONMENT_CI)
@property (nonatomic, retain) NSString* oneMonthRecurringSubscriptionID;
@property (nonatomic, retain) SKProduct* oneMonthRecurringSubscription;
@property (assign) BOOL purchaseRecurringAfterQuery;
#endif

+ (id)sharedPayment_ios;

-(void)makeOneMonthPayment;
#if defined(AZOOMEE_ENVIRONMENT_CI)
-(void)makeOneMonthRecurringPayment;
#endif

-(void)restorePayment;
-(void)startProductPriceQuery;

@end
