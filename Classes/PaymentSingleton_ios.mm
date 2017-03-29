#include "PaymentSingleton_ios.h"
#include "MessageBox.h"
#include "payment_ios.h"
#include "ModalMessages.h"

USING_NS_CC;

using namespace cocos2d;

static PaymentSingleton_ios *_sharedPaymentSingleton_ios = NULL;

PaymentSingleton_ios* PaymentSingleton_ios::getInstance()
{
    if (! _sharedPaymentSingleton_ios)
    {
        _sharedPaymentSingleton_ios = new PaymentSingleton_ios();
        _sharedPaymentSingleton_ios->init();
    }
    
    return _sharedPaymentSingleton_ios;
}

PaymentSingleton_ios::~PaymentSingleton_ios(void)
{
}

bool PaymentSingleton_ios::init(void)
{
    return true;
}

void PaymentSingleton_ios::makeMonthlyPayment()
{
    ModalMessages::getInstance()->startLoading();
    
    payment_ios* applePaymentObject = [[payment_ios alloc] init];
    [applePaymentObject retain];
    [applePaymentObject makeOneMonthPayment];
}

void PaymentSingleton_ios::ErrorMessage()
{
    ModalMessages::getInstance()->stopLoading();
    MessageBox::createWith(ERROR_CODE_PURCHASE_FAILURE, nullptr);
}
