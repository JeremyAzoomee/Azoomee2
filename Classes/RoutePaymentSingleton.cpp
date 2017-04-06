#include "RoutePaymentSingleton.h"
#include "AnalyticsSingleton.h"

#include "ParentDataProvider.h"

#include "AmazonPaymentSingleton.h"
#include "GooglePaymentSingleton.h"
#include "StringFunctions.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include "platform/android/jni/JniHelper.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
   #include "ApplePaymentSingleton.h"
#endif

USING_NS_CC;

using namespace cocos2d;

static RoutePaymentSingleton *_sharedRoutePaymentSingleton = NULL;

RoutePaymentSingleton* RoutePaymentSingleton::getInstance()
{
    if (! _sharedRoutePaymentSingleton)
    {
        _sharedRoutePaymentSingleton = new RoutePaymentSingleton();
        _sharedRoutePaymentSingleton->init();
    }
    
    return _sharedRoutePaymentSingleton;
}

RoutePaymentSingleton::~RoutePaymentSingleton(void)
{
}

bool RoutePaymentSingleton::init(void)
{
    return true;
}
void RoutePaymentSingleton::startInAppPayment()
{
    if(osIsIos())
    {
        #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            AnalyticsSingleton::getInstance()->registerIAPOS("iOS");
            //start ios payment
            ApplePaymentSingleton::getInstance()->makeMonthlyPayment();
        #endif
        return;
    }
    
    if(osIsAndroid())
    {
        AnalyticsSingleton::getInstance()->registerIAPOS("Google");
        GooglePaymentSingleton::getInstance()->startIABPayment();
        return;
    }
    
    if(osIsAmazon())
    {
        AnalyticsSingleton::getInstance()->registerIAPOS("Amazon");
        AmazonPaymentSingleton::getInstance()->startIAPPayment();
        return;
    }
}

bool RoutePaymentSingleton::showIAPContent()
{
    return !ParentDataProvider::getInstance()->isPaidUser();
}

std::string RoutePaymentSingleton::getOSManufacturer()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    JniMethodInfo t;
    JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "getOSBuildManufacturer", "()Ljava/lang/String;");
    jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
    const char *resultCStr = t.env->GetStringUTFChars(str, NULL);
    std::string resultStr(resultCStr);
    t.env->ReleaseStringUTFChars(str, resultCStr);
    
    CCLOG("DEVICE TYPE:%s",resultStr.c_str());
    
    if (resultStr == "Amazon")
    {
        return "Amazon";
    }
    else
    {
        return "Google";
    }
#else
    return "iOS";
#endif

}

bool RoutePaymentSingleton::osIsIos()
{
    if(getOSManufacturer() == "iOS") return true;
    else return false;
}

bool RoutePaymentSingleton::osIsAndroid()
{
    if(getOSManufacturer() == "Google") return true;
    else return false;
}

bool RoutePaymentSingleton::osIsAmazon()
{
    if(getOSManufacturer() == "Amazon") return true;
    else return false;
}

void RoutePaymentSingleton::refreshAppleReceiptFromButton()
{
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ApplePaymentSingleton::getInstance()->refreshReceipt(true);
    #endif
}

void RoutePaymentSingleton::checkIfAppleReceiptRefreshNeeded()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if(ParentDataProvider::getInstance()->getBillingProvider() == "APPLE" && isDateStringOlderThanToday(ParentDataProvider::getInstance()->getBillingDate()))
    {
        ApplePaymentSingleton::getInstance()->refreshReceipt(false);
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if(ParentDataProvider::getInstance()->getBillingProvider() == "APPLE" && !ParentDataProvider::getInstance()->isPaidUser())
    {
        MessageBox::createWith(ERROR_CODE_APPLE_SUBSCRIPTION_ON_NON_APPLE, nullptr);
    }
#endif
}
