#import "payment_ios.h"
#include "PaymentSingleton_ios.h"

#define ONE_MONTH_PAYMENT @"AZ_Premium_Monthly"

@interface payment_ios ()

@end

@implementation payment_ios


-(void)makeOneMonthPayment
{
    if(self.oneMonthSubscription == nil)
    {
        //request the prices and titles of IAP
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
    @catch (NSException * e) {
        
        PaymentSingleton_ios::getInstance()->ErrorMessage();
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
                
                PaymentSingleton_ios::getInstance()->transactionStatePurchased(std::string([receiptString UTF8String]));
                
                [self release];
                break;
            }
            case SKPaymentTransactionStateFailed:
            {
                NSURL *receiptURL = [[NSBundle mainBundle] appStoreReceiptURL];
                NSData *receipt = [NSData dataWithContentsOfURL:receiptURL];
                NSString* receiptString = [receipt base64EncodedStringWithOptions:0];
                
                PaymentSingleton_ios::getInstance()->ErrorMessage();
                NSLog(@"Transaction error: %@", transaction.error.localizedDescription);
                
                [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
                [self release];
                break;
            }
            case SKPaymentTransactionStateRestored:
            {
                [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
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
    PaymentSingleton_ios::getInstance()->DoublePurchase();
    [self release];
}

- (void) paymentQueue:(SKPaymentQueue *)queue restoreCompletedTransactionsFailedWithError:(NSError *)error
{
    PaymentSingleton_ios::getInstance()->ErrorMessage();
    [self release];
}

- (void)request:(SKRequest *)request didFailWithError:(NSError *)error
{
    PaymentSingleton_ios::getInstance()->ErrorMessage();
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
    }
    else
    {
        
    }
}
/*
 
 
 - (void)purchase:(SKProduct *)product{
 SKPayment *payment = [SKPayment paymentWithProduct:product];
 
 [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
 [[SKPaymentQueue defaultQueue] addPayment:payment];
 }
 
 - (IBAction) restore{
 //this is called when the user restores purchases, you should hook this up to a button
 [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
 [[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
 }
 
 - (void) paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue *)queue
 {
 NSLog(@"received restored transactions: %i", queue.transactions.count);
 for(SKPaymentTransaction *transaction in queue.transactions){
 if(transaction.transactionState == SKPaymentTransactionStateRestored){
 //called when the user successfully restores a purchase
 NSLog(@"Transaction state -> Restored");
 
 //if you have more than one in-app purchase product,
 //you restore the correct product for the identifier.
 //For example, you could use
 //if(productID == kRemoveAdsProductIdentifier)
 //to get the product identifier for the
 //restored purchases, you can use
 //
 //NSString *productID = transaction.payment.productIdentifier;
 [self doRemoveAds];
 [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
 break;
 }
 }
 }
 
 - (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions{
 for(SKPaymentTransaction *transaction in transactions){
 //if you have multiple in app purchases in your app,
 //you can get the product identifier of this transaction
 //by using transaction.payment.productIdentifier
 //
 //then, check the identifier against the product IDs
 //that you have defined to check which product the user
 //just purchased
 
 switch(transaction.transactionState){
 case SKPaymentTransactionStatePurchasing: NSLog(@"Transaction state -> Purchasing");
 //called when the user is in the process of purchasing, do not add any of your own code here.
 break;
 case SKPaymentTransactionStatePurchased:
 //this is called when the user has successfully purchased the package (Cha-Ching!)
 [self doRemoveAds]; //you can add your code for what you want to happen when the user buys the purchase here, for this tutorial we use removing ads
 [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
 NSLog(@"Transaction state -> Purchased");
 break;
 case SKPaymentTransactionStateRestored:
 NSLog(@"Transaction state -> Restored");
 //add the same code as you did from SKPaymentTransactionStatePurchased here
 [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
 break;
 case SKPaymentTransactionStateFailed:
 //called when the transaction does not finish
 if(transaction.error.code == SKErrorPaymentCancelled){
 NSLog(@"Transaction state -> Cancelled");
 //the user cancelled the payment ;(
 }
 [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
 break;
 }
 }
 }*/


@end
