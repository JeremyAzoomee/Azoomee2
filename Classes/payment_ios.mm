#import "payment_ios.h"
#include "ApplePaymentSingleton.h"

#define ONE_MONTH_PAYMENT @"AZ_Premium_Monthly"

@interface payment_ios ()

@end

@implementation payment_ios

+ (id)sharedPayment_ios {
    static payment_ios *sharedPayment_ios = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedPayment_ios = [[self alloc] init];
    });
    return sharedPayment_ios;
}

-(void)makeOneMonthPayment
{
    if(self.oneMonthSubscription == nil)
    {
        NSSet * productIdentifiers = [NSSet setWithObjects:ONE_MONTH_PAYMENT, nil];
        
        SKProductsRequest *productsRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:productIdentifiers];
        
        productsRequest.delegate = self;
        [productsRequest start];
    }
    else
        [self startPaymentQueue];
}

-(void)restorePayment
{
    SKReceiptRefreshRequest* request = [[SKReceiptRefreshRequest alloc] init];
    request.delegate = self;
    [request start];
}

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response {
    
    @try {
        NSArray * skProducts = response.products;
        for (SKProduct * skProduct in skProducts) {
            
            if([skProduct.productIdentifier isEqualToString:ONE_MONTH_PAYMENT] )
            {
                self.oneMonthSubscription = skProduct;
                [self startPaymentQueue];
            }
        }
    }
    @catch (NSException * e)
    {
        ApplePaymentSingleton::getInstance()->ErrorMessage(false);
    }
}

-(void) startPaymentQueue
{
    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
    
    SKPayment * payment = [SKPayment paymentWithProduct:self.oneMonthSubscription];
    [[SKPaymentQueue defaultQueue] addPayment:payment];
}

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    for (SKPaymentTransaction * transaction in transactions) {
        switch (transaction.transactionState)
        {
            case SKPaymentTransactionStatePurchased:
            {
                [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
                
                [self sendReceiptToBackend];
                
                break;
            }
            case SKPaymentTransactionStateFailed:
            {
                [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
                ApplePaymentSingleton::getInstance()->ErrorMessage(false);
                NSLog(@"Transaction error: %@", transaction.error.localizedDescription);
                
                break;
            }
            case SKPaymentTransactionStateRestored:
            {
                [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
                 ApplePaymentSingleton::getInstance()->DoublePurchase();
            }
            case SKPaymentTransactionStateDeferred:
            {
                
            }
            default:
                break;
        }
    };
}

- (void) paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue *)queue
{
    ApplePaymentSingleton::getInstance()->DoublePurchase();
}

- (void) paymentQueue:(SKPaymentQueue *)queue restoreCompletedTransactionsFailedWithError:(NSError *)error
{
    ApplePaymentSingleton::getInstance()->ErrorMessage(false);
}

- (void)request:(SKRequest *)request didFailWithError:(NSError *)error
{
    NSLog(@"DidFailWithError error: %@", error.localizedDescription);
    
    if(ApplePaymentSingleton::getInstance()->refreshFromButton)
        ApplePaymentSingleton::getInstance()->ErrorMessage(false);
    else
        ApplePaymentSingleton::getInstance()->ErrorMessage(true);
}

-(void)requestDidFinish:(SKRequest *)request
{
    bool receiptExist = [[NSFileManager defaultManager] fileExistsAtPath:[[[NSBundle mainBundle] appStoreReceiptURL] path]];
    
    if(receiptExist && !ApplePaymentSingleton::getInstance()->makingMonthlyPayment)
    {
        [self sendReceiptToBackend];
    }
}

-(void) sendReceiptToBackend
{
    NSURL *receiptURL = [[NSBundle mainBundle] appStoreReceiptURL];
    NSData *receipt = [NSData dataWithContentsOfURL:receiptURL];
    NSString* receiptString = [receipt base64EncodedStringWithOptions:0];
    
    ApplePaymentSingleton::getInstance()->transactionStatePurchased(std::string([receiptString UTF8String]));
}

@end
