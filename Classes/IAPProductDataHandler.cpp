#include "IAPProductDataHandler.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"

static const std::string kAzoomeeActivityJavaClassName = "org/cocos2dx/cpp/AppActivity";

#endif

using namespace cocos2d;

NS_AZOOMEE_BEGIN
    
static std::auto_ptr<IAPProductDataHandler> sIAPProductDataHandlerSharedInstance;

IAPProductDataHandler* IAPProductDataHandler::getInstance()
{
    if(!sIAPProductDataHandlerSharedInstance.get())
    {
        sIAPProductDataHandlerSharedInstance.reset(new IAPProductDataHandler());
    }
    return sIAPProductDataHandlerSharedInstance.get();
}

IAPProductDataHandler::~IAPProductDataHandler(void)
{
    
}

IAPProductDataHandler::IAPProductDataHandler()
{
    
}

//implemented in mm file only.
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
void IAPProductDataHandler::fetchProductData()
{
    return;
}
#endif

//SETTERS

void IAPProductDataHandler::setHumanReadableProductPrice(const std::string &productPriceHumanReadable)
{
    _productPriceHumanReadable = productPriceHumanReadable;
    
    cocos2d::log("IAPPRICESET: %s", _productPriceHumanReadable.c_str());
}

void IAPProductDataHandler::setIntroductoryMonths(int introductoryMonths)
{
    _introductoryMonths = introductoryMonths;
}

void IAPProductDataHandler::setIntroductoryPrice(float introductoryPrice)
{
    _introductoryPrice = introductoryPrice;
}

void IAPProductDataHandler::setProductPrice(float productPrice)
{
    _productPrice = productPrice;
}

void IAPProductDataHandler::setTrialPeriodDays(int trialPeriodDays)
{
    _trialPeriodDays = trialPeriodDays;
}

void IAPProductDataHandler::setPriceCurrency(const std::string &currency)
{
    _priceCurrency = currency;
}

//GETTERS

std::string IAPProductDataHandler::getHumanReadableProductPrice() const
{
    return _productPriceHumanReadable;
}

int IAPProductDataHandler::getIntroductoryMonths()
{
    return _introductoryMonths;
}
float IAPProductDataHandler::getIntroductoryPrice()
{
    return _introductoryPrice;
}

float IAPProductDataHandler::getProductPrice()
{
    return _productPrice;
}

std::string IAPProductDataHandler::getPriceCurrency() const
{
    return _priceCurrency;
}

int IAPProductDataHandler::getTrialPeriodDays()
{
    return _trialPeriodDays;
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_setHumanReadablePrice(JNIEnv* env, jobject thiz, jstring price);
};

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_setHumanReadablePrice(JNIEnv* env, jobject thiz, jstring price)
{
    const char* cPrice = env->GetStringUTFChars(price, NULL);
    IAPProductDataHandler::getInstance()->setHumanReadableProductPrice(cPrice);
}

#endif

NS_AZOOMEE_END
