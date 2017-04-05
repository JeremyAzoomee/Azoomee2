#import "payment_ios.h"
#include "ApplePaymentSingleton.h"

#define ONE_MONTH_PAYMENT @"AZ_Premium_Monthly"

@interface payment_ios ()

@end

@implementation payment_ios


-(void)makeOneMonthPayment
{
    if(self.oneMonthSubscription == nil)
    {
        NSSet * productIdentifiers = [NSSet setWithObjects:
                                      ONE_MONTH_PAYMENT,
                                      nil];
        
        SKProductsRequest *productsRequest = [[SKProductsRequest alloc]
                                              initWithProductIdentifiers:productIdentifiers];
        productsRequest.delegate = self;
        [productsRequest start];
    }
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
                [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
                
                SKPayment * payment = [SKPayment paymentWithProduct:skProduct];
                [[SKPaymentQueue defaultQueue] addPayment:payment];
            }
        }
    }
    @catch (NSException * e)
    {
        ApplePaymentSingleton::getInstance()->ErrorMessage();
    }
}

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    for (SKPaymentTransaction * transaction in transactions) {
        switch (transaction.transactionState)
        {
            case SKPaymentTransactionStatePurchased:
            {
                [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
                
                NSURL *receiptURL = [[NSBundle mainBundle] appStoreReceiptURL];
                NSData *receipt = [NSData dataWithContentsOfURL:receiptURL];
                NSString* receiptString = [receipt base64EncodedStringWithOptions:0];
                
                ApplePaymentSingleton::getInstance()->transactionStatePurchased(std::string([receiptString UTF8String]));
                
                [self release];
                break;
            }
            case SKPaymentTransactionStateFailed:
            {
                [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
                ApplePaymentSingleton::getInstance()->ErrorMessage();
                NSLog(@"Transaction error: %@", transaction.error.localizedDescription);
                
                [self release];
                break;
            }
            case SKPaymentTransactionStateRestored:
            {
                [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
                 ApplePaymentSingleton::getInstance()->DoublePurchase();
                [self release];
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
    [self release];
}

- (void) paymentQueue:(SKPaymentQueue *)queue restoreCompletedTransactionsFailedWithError:(NSError *)error
{
    ApplePaymentSingleton::getInstance()->ErrorMessage();
    [self release];
}

- (void)request:(SKRequest *)request didFailWithError:(NSError *)error
{
    ApplePaymentSingleton::getInstance()->ErrorMessage();
    NSLog(@"DidFailWithError error: %@", error.localizedDescription);
    [self release];
}

-(void)requestDidFinish:(SKRequest *)request
{
    bool receiptExist = [[NSFileManager defaultManager] fileExistsAtPath:[[[NSBundle mainBundle] appStoreReceiptURL] path]];
    
    if(receiptExist)
    {
        NSURL *receiptURL = [[NSBundle mainBundle] appStoreReceiptURL];
        NSData *receipt = [NSData dataWithContentsOfURL:receiptURL];
        NSString* receiptString = [receipt base64EncodedStringWithOptions:0];
        
        ApplePaymentSingleton::getInstance()->transactionStatePurchased(std::string([receiptString UTF8String]));
        
        [self release];
    }
}

@end
