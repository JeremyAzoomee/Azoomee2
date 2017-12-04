#include "IAPProductDataHandler.h"
#include <AzoomeeCommon/UI/Style.h>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"

static const std::string kAzoomeeActivityJavaClassName = "org/cocos2dx/cpp/AppActivity";

#endif

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const std::string &IAPProductDataHandler::kDefaultPrice = "£4.99";
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

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
void IAPProductDataHandler::fetchProductData()
{
    if(!isProductDataFetched())
    {
        JniHelper::callStaticVoidMethod("org/cocos2dx/cpp/AppActivity", "setupInAppPurchase");
    }
}
#endif

bool IAPProductDataHandler::isProductDataFetched()
{
    return _productPriceHumanReadable != "";
}

void IAPProductDataHandler::productDataFetchFailed()
{
    cocos2d::log("Fetching IAP info was not successful");
}

//SETTERS

void IAPProductDataHandler::setHumanReadableProductPrice(const std::string &productPriceHumanReadable)
{
    _productPriceHumanReadable = productPriceHumanReadable;
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
    if(_productPriceHumanReadable == "")
    {
        return kDefaultPrice;
    }
    
    return _productPriceHumanReadable;
}
                 
cocos2d::Label* IAPProductDataHandler::createOptionalSubscriptionLabel()
{
    cocos2d::Label *optionalLabel = Label::createWithTTF("Then " + getHumanReadableProductPrice() + "/month. No commitment, cancel anytime.", Style::Font::Regular, 40);
    optionalLabel->setColor(Style::Color::brightAqua);
    optionalLabel->setAnchorPoint(Vec2(0.5,0.5));
    optionalLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    return optionalLabel;
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

extern "C"
{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_priceFetchFailed(JNIEnv* env, jobject thiz);
};

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_priceFetchFailed(JNIEnv* env, jobject thiz)
{
    IAPProductDataHandler::getInstance()->productDataFetchFailed();
}

#endif

NS_AZOOMEE_END
