#import "PaymentViewController_ios.h"
#include "ApplePaymentSingleton.h"
#include "RoutePaymentSingleton.h"
#include "LoginLogicHandler.h"
#include "IAPProductDataHandler.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>

using namespace Azoomee;

@interface PaymentViewController ()
-(void) queryProductInfo;
-(void) sendReceiptToBackendWithTransactionID:(NSString*)transactionID;
@end

@implementation PaymentViewController

-(void)startProductPriceQuery
{
    self.purchaseAfterQuery = NO;
    [self queryProductInfo];
}

+ (id)sharedPayment_ios {
    static PaymentViewController *sharedPayment_ios = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedPayment_ios = [[self alloc] init];
    });
    return sharedPayment_ios;
}

- (id)init {
    self = [super init];
    if (self) {
        [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
    }
    return self;
}

-(void)makeOneMonthPayment
{
    if(self.oneMonthSubscription == nil)
    {
        self.purchaseAfterQuery = YES;
        [self queryProductInfo];
    }
    else
    {
        [self startPaymentQueue];
    }
}

-(void)restorePayment
{
    SKReceiptRefreshRequest* request = [[SKReceiptRefreshRequest alloc] init];
    request.delegate = self;
    [request start];
}

// Sent immediately before -requestDidFinish:
- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response {
    
    @try {
        NSArray * skProducts = response.products;
        for (SKProduct * skProduct in skProducts) {
            
            if([skProduct.productIdentifier isEqualToString:self.oneMonthSubscriptionID] )
            {
                self.oneMonthSubscription = skProduct;
                
                if(self.purchaseAfterQuery)
                {
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
        if(self.purchaseAfterQuery)
        {
            RoutePaymentSingleton::getInstance()->purchaseFailureErrorMessage([[NSString stringWithFormat:@"%@: %@", e.name, e.userInfo] UTF8String]);
        }
        else
        {
            IAPProductDataHandler::getInstance()->productDataFetchFailed();
            self.purchaseAfterQuery = YES;
        }
    }
}

-(void)queryProductInfo
{
#if defined(AZOOMEE_ENVIRONMENT_TEST)
    const std::string& productID = "ios-test";
#elif defined(AZOOMEE_ENVIRONMENT_CI)
    const std::string& productID = "ios-ci";
#else
    const std::string& productID = "ios-prod";
#endif
    
    self.oneMonthSubscriptionID = [NSString stringWithUTF8String:ConfigStorage::getInstance()->getIapSkuForProvider(productID).c_str()];
    NSSet * productIdentifiers = [NSSet setWithObjects:self.oneMonthSubscriptionID, nil];
    
    SKProductsRequest *productsRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:productIdentifiers];
    
    productsRequest.delegate = self;
    [productsRequest start];
}

-(void) startPaymentQueue
{
//    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
    
    SKPayment * payment = [SKPayment paymentWithProduct:self.oneMonthSubscription];
    [[SKPaymentQueue defaultQueue] addPayment:payment];
}

// Sent when the transaction array has changed (additions or state changes).  Client should check state of transactions and finish as appropriate.
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    for (SKPaymentTransaction * transaction in transactions) {
        switch (transaction.transactionState)
        {
            case SKPaymentTransactionStatePurchased:
            {
                [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
                NSString* transactionID = transaction.transactionIdentifier;
                
                [self sendReceiptToBackendWithTransactionID:transactionID];
                
                break;
            }
            case SKPaymentTransactionStateFailed:
            {
                [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
                if(transaction.error.code != SKErrorPaymentCancelled)
                {
                    RoutePaymentSingleton::getInstance()->purchaseFailureErrorMessage([[NSString stringWithFormat:@"SKPaymentTransactionStateFailed: %@",transaction.error.localizedDescription] UTF8String]);
                    NSLog(@"Transaction error: %@", transaction.error.localizedDescription);
                }
                else
                {
                    RoutePaymentSingleton::getInstance()->canceledAction();
                }
                break;
            }
            case SKPaymentTransactionStateRestored:
            {
                [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
//                RoutePaymentSingleton::getInstance()->doublePurchaseMessage();
            }
            case SKPaymentTransactionStateDeferred:
            {
                
            }
            default:
                break;
        }
    };
}

// Sent when all transactions from the user's purchase history have successfully been added back to the queue.
- (void) paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue *)queue
{
    RoutePaymentSingleton::getInstance()->doublePurchaseMessage();
}

// Sent when an error is encountered while adding transactions from the user's purchase history back to the queue.
- (void) paymentQueue:(SKPaymentQueue *)queue restoreCompletedTransactionsFailedWithError:(NSError *)error
{
    RoutePaymentSingleton::getInstance()->purchaseFailureErrorMessage([[NSString stringWithFormat:@"restoreCompletedTransactionsFailedWithError: %@",error.localizedDescription] UTF8String]);
}

- (void)request:(SKRequest *)request didFailWithError:(NSError *)error
{
    NSLog(@"DidFailWithError error: %@", error.localizedDescription);
    
    // Error code 16 is undocumented because it's a private framework (StoreServices), however 16 corresponds to the user cancelling the login/password entry
    // In this instance we don't want to show a popup, but in all other cases we continue and show an error popup
    if([error.domain isEqualToString:@"SSErrorDomain"] && error.code == 16)
    {
        RoutePaymentSingleton::getInstance()->canceledAction();
        return;
    }
    
    if(RoutePaymentSingleton::getInstance()->pressedRestorePurchaseButton)
    {
        RoutePaymentSingleton::getInstance()->purchaseFailureErrorMessage([[NSString stringWithFormat:@"DidFailWithError: %@",error.localizedDescription] UTF8String]);
    }
    else
    {
        if(self.purchaseAfterQuery)
        {
            LoginLogicHandler::getInstance()->doLoginLogic();
        }
        else
        {
            self.purchaseAfterQuery = YES;
        }
    }
}

-(void)requestDidFinish:(SKRequest *)request
{
    
    if(!self.purchaseAfterQuery)
    {
        self.purchaseAfterQuery = YES;
        return;
    }
    
    bool receiptExist = [[NSFileManager defaultManager] fileExistsAtPath:[[[NSBundle mainBundle] appStoreReceiptURL] path]];
    
    if(receiptExist && !RoutePaymentSingleton::getInstance()->pressedIAPStartButton)
    {
        [self sendReceiptToBackendWithTransactionID:nil];
    }
}

-(void) sendReceiptToBackendWithTransactionID:(NSString*)transactionIdentifier
{
    NSURL *receiptURL = [[NSBundle mainBundle] appStoreReceiptURL];
    NSData *receipt = [NSData dataWithContentsOfURL:receiptURL];
    NSString* receiptString = [receipt base64EncodedStringWithOptions:0];
    
    const std::string& transactionID = (transactionIdentifier != nil) ? [transactionIdentifier UTF8String] : "";
    
    ApplePaymentSingleton::getInstance()->transactionStatePurchased([receiptString UTF8String], transactionID);
}

@end
