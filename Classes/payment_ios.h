#import <StoreKit/StoreKit.h>

@interface payment_ios : NSObject <SKProductsRequestDelegate, SKPaymentTransactionObserver>

@property (nonatomic, retain) SKProduct * oneMonthTrial;

-(void)getProducts;

@end
