#import "PaymentViewController_ios.h"
#include "ApplePaymentSingleton.h"
#include "RoutePaymentSingleton.h"
#include "LoginLogicHandler.h"
#include "IAPProductDataHandler.h"

using namespace Azoomee;

#define ONE_MONTH_PAYMENT @"AZ_Premium_Monthly"

@interface PaymentViewController ()

@end

@implementation PaymentViewController

-(void)startProductPriceQuery
{
    _noPurchaseAfterQuery = true;
    
    NSSet * productIdentifiers = [NSSet setWithObjects:ONE_MONTH_PAYMENT, nil];
    
    SKProductsRequest *productsRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:productIdentifiers];
    
    productsRequest.delegate = self;
    [productsRequest start];
}

+ (id)sharedPayment_ios {
    static PaymentViewController *sharedPayment_ios = nil;
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
                if(!_noPurchaseAfterQuery)
                {
                    self.oneMonthSubscription = skProduct;
                    [self startPaymentQueue];
                }
                else
                {
                    NSNumberFormatter * _priceFormatter;
                    _priceFormatter = [[NSNumberFormatter alloc] init];
                    [_priceFormatter setFormatterBehavior:NSNumberFormatterBehavior10_4];
                    [_priceFormatter setNumberStyle:NSNumberFormatterCurrencyStyle];
                    [_priceFormatter setLocale:skProduct.priceLocale];
                    NSString *priceHumanReadable = [_priceFormatter stringFromNumber:skProduct.price];
                    
                    IAPProductDataHandler::getInstance()->setHumanReadableProductPrice(std::string([priceHumanReadable cStringUsingEncoding:NSUTF8StringEncoding]));
                    IAPProductDataHandler::getInstance()->setProductPrice([skProduct.price floatValue]);
                }
            }
        }
    }
    @catch (NSException * e)
    {
        if(!_noPurchaseAfterQuery)
        {
            RoutePaymentSingleton::getInstance()->purchaseFailureErrorMessage([[NSString stringWithFormat:@"%@: %@", e.name, e.userInfo] UTF8String]);
        }
        else
        {
            IAPProductDataHandler::getInstance()->productDataFetchFailed();
            _noPurchaseAfterQuery = false;
        }
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
                RoutePaymentSingleton::getInstance()->purchaseFailureErrorMessage([[NSString stringWithFormat:@"SKPaymentTransactionStateFailed: %@",transaction.error.localizedDescription] UTF8String]);
                NSLog(@"Transaction error: %@", transaction.error.localizedDescription);
                
                break;
            }
            case SKPaymentTransactionStateRestored:
            {
                [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
                RoutePaymentSingleton::getInstance()->doublePurchaseMessage();
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
    RoutePaymentSingleton::getInstance()->doublePurchaseMessage();
}

- (void) paymentQueue:(SKPaymentQueue *)queue restoreCompletedTransactionsFailedWithError:(NSError *)error
{
    RoutePaymentSingleton::getInstance()->purchaseFailureErrorMessage([[NSString stringWithFormat:@"restoreCompletedTransactionsFailedWithError: %@",error.localizedDescription] UTF8String]);
}

- (void)request:(SKRequest *)request didFailWithError:(NSError *)error
{
    NSLog(@"DidFailWithError error: %@", error.localizedDescription);
    
    if(RoutePaymentSingleton::getInstance()->pressedRestorePurchaseButton)
        RoutePaymentSingleton::getInstance()->purchaseFailureErrorMessage([[NSString stringWithFormat:@"DidFailWithError: %@",error.localizedDescription] UTF8String]);
    else
        if(!_noPurchaseAfterQuery)
        {
            LoginLogicHandler::getInstance()->doLoginLogic();
        }
        else
        {
            _noPurchaseAfterQuery = false;
        }
}

-(void)requestDidFinish:(SKRequest *)request
{
    if(_noPurchaseAfterQuery)
    {
        _noPurchaseAfterQuery = false;
        return;
    }
    
    bool receiptExist = [[NSFileManager defaultManager] fileExistsAtPath:[[[NSBundle mainBundle] appStoreReceiptURL] path]];
    
    if(receiptExist && !RoutePaymentSingleton::getInstance()->pressedIAPStartButton)
    {
        [self sendReceiptToBackend];
    }
}

-(void) sendReceiptToBackend
{
    NSURL *receiptURL = [[NSBundle mainBundle] appStoreReceiptURL];
    NSData *receipt = [NSData dataWithContentsOfURL:receiptURL];
    NSString* receiptString = [receipt base64EncodedStringWithOptions:0];
    
    ApplePaymentSingleton::getInstance()->transactionStatePurchased([receiptString UTF8String]);
}

@end
