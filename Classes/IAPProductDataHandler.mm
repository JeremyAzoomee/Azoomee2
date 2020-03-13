#include "IAPProductDataHandler.h"
#include "PaymentViewController_ios.h"

using namespace cocos2d;

USING_NS_TZ

NS_AZ_BEGIN

void IAPProductDataHandler::fetchProductData()
{
#ifndef AZOOMEE_VODACOM_BUILD
    if(!isProductDataFetched())
    {
        [[PaymentViewController sharedPayment_ios] startProductPriceQuery];
    }
#endif
}

NS_AZ_END
