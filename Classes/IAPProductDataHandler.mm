#include "IAPProductDataHandler.h"
#include "PaymentViewController_ios.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

void IAPProductDataHandler::fetchProductData()
{
#ifndef AZOOMEE_VODACOM_BUILD
    if(!isProductDataFetched())
    {
        [[PaymentViewController sharedPayment_ios] startProductPriceQuery];
    }
#endif
}

NS_AZOOMEE_END
