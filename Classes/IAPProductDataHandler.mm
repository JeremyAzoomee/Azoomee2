#include "IAPProductDataHandler.h"
#include "PaymentViewController_ios.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

void IAPProductDataHandler::fetchProductData()
{
    [[PaymentViewController sharedPayment_ios] startProductPriceQuery];
}

NS_AZOOMEE_END
