#include "RoutePaymentSingleton.h"
#include "AnalyticsSingleton.h"

#include "ParentDataProvider.h"

#include "AmazonPaymentSingleton.h"
#include "GooglePaymentSingleton.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
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
        AnalyticsSingleton::getInstance()->registerIAPOS("iOS");
        //start ios payment
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

bool RoutePaymentSingleton::OS_is_IAP_Compatible()
{
    if(osIsIos()) return false; //change this to true when ios iap is implemented
    if(osIsAndroid()) return true;
    if(osIsAmazon()) return true;
    
    return false;
}

bool RoutePaymentSingleton::showIAPContent()
{
    return (OS_is_IAP_Compatible() && !ParentDataProvider::getInstance()->isPaidUser());
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
