#include "GooglePaymentSingleton.h"
#include "HttpRequestCreator.h"
#include "external/json/document.h"
#include "MessageBox.h"
#include "BackEndCaller.h"
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif

USING_NS_CC;

using namespace cocos2d;

static GooglePaymentSingleton *_sharedGooglePaymentSingleton = NULL;

GooglePaymentSingleton* GooglePaymentSingleton::getInstance()
{
    if (! _sharedGooglePaymentSingleton)
    {
        _sharedGooglePaymentSingleton = new GooglePaymentSingleton();
        _sharedGooglePaymentSingleton->init();
    }
    
    return _sharedGooglePaymentSingleton;
}

GooglePaymentSingleton::~GooglePaymentSingleton(void)
{
}

bool GooglePaymentSingleton::init(void)
{
    return true;
}

void GooglePaymentSingleton::createModalLayer()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    modalLayer = LayerColor::create(Color4B(0,0,0,0), visibleSize.width, visibleSize.height);
    modalLayer->setPosition(origin.x, origin.y);
    modalLayer->setOpacity(0);
    Director::getInstance()->getRunningScene()->addChild(modalLayer);
    
    addListenerToBackgroundLayer();
    
    modalLayer->runAction(FadeTo::create(0.5, 150));
}

void GooglePaymentSingleton::removeModalLayer()
{
    paymentInProgress = false;
    if(modalLayer) //This might be called when loading is not active, so better to check first
    {
        Director::getInstance()->getRunningScene()->removeChild(modalLayer);
    }
}

void GooglePaymentSingleton::addListenerToBackgroundLayer()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *touch, Event *event) //Lambda callback, which is a C++ 11 feature.
    {
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), modalLayer);
}

void GooglePaymentSingleton::startBackEndPaymentVerification(std::string developerPayload, std::string orderId, std::string token)
{
    savedDeveloperPayload = developerPayload;
    savedOrderId = orderId;
    savedToken = token;
    
    auto funcCallAction = CallFunc::create([=](){
        HttpRequestCreator* httpRequestCreator = new HttpRequestCreator();
        httpRequestCreator->requestBody = StringUtils::format("{\"orderId\": \"%s\", \"subscriptionId\": \"%s\", \"purchaseToken\": \"%s\"}", orderId.c_str(), ConfigStorage::getInstance()->getIapSkuForProvider("google").c_str(), token.c_str());
        httpRequestCreator->requestTag = "iabGooglePaymentMade";
        httpRequestCreator->createEncryptedPostHttpRequest();
    });
    
    Director::getInstance()->getRunningScene()->runAction(Sequence::create(DelayTime::create(1), funcCallAction, NULL)); //need time to get focus back from google window, otherwise the app will crash
}

void GooglePaymentSingleton::backendRequestFailed()
{
    purchaseFailedAfterFulfillment();
}

void GooglePaymentSingleton::onGooglePaymentVerificationAnswerReceived(std::string responseDataString)
{
    CCLOG("The response id is: %s", responseDataString.c_str());
    
    rapidjson::Document paymentData;
    paymentData.Parse(responseDataString.c_str());
    
    if(paymentData.HasParseError())
    {
        requestAttempts = requestAttempts + 1;
        startBackEndPaymentVerification(savedDeveloperPayload, savedOrderId, savedToken);
        return;
    }
    
    if(paymentData.HasMember("receiptStatus"))
    {
        if(paymentData["receiptStatus"].IsString())
        {
            if(StringUtils::format("%s", paymentData["receiptStatus"].GetString()) == "FULFILLED")
            {
                AnalyticsSingleton::getInstance()->iapSubscriptionSuccessEvent();
                
                removeModalLayer();
                
                BackEndCaller::getInstance()->newTrialJustStarted = true;
                BackEndCaller::getInstance()->autoLogin();
                
                return;
            }
        }
    }

    if(requestAttempts < 4)
    {
        requestAttempts = requestAttempts + 1;
        startBackEndPaymentVerification(savedDeveloperPayload, savedOrderId, savedToken);
        return;
    }

    purchaseFailedAfterFulfillment();
}

void GooglePaymentSingleton::purchaseFailedBeforeFulfillment()
{
    auto funcCallAction = CallFunc::create([=](){
        this->prepareForErrorMessage();
        MessageBox::createWith(ERROR_CODE_PURCHASE_FAILURE, nullptr);
    });
    
    Director::getInstance()->getRunningScene()->runAction(Sequence::create(DelayTime::create(1), funcCallAction, NULL)); //need time to get focus back from google window, otherwise the app will crash
}

void GooglePaymentSingleton::purchaseFailedAfterFulfillment()
{
    prepareForErrorMessage();
    MessageBox::createWith(ERROR_CODE_PURCHASE_FAILURE_CONTACT_US, nullptr);
}

void GooglePaymentSingleton::doublePurchase()
{
    auto funcCallAction = CallFunc::create([=](){
        this->prepareForErrorMessage();
        MessageBox::createWith(ERROR_CODE_PURCHASE_DOUBLE, nullptr);
    });
    
    Director::getInstance()->getRunningScene()->runAction(Sequence::create(DelayTime::create(1), funcCallAction, NULL)); //need time to get focus back from google window, otherwise the app will crash
}

void GooglePaymentSingleton::prepareForErrorMessage()
{
    paymentInProgress = false;
    AnalyticsSingleton::getInstance()->iapSubscriptionFailedEvent();
    removeModalLayer();
}

//--------------------PAYMENT FUNCTIONS------------------

void GooglePaymentSingleton::startIABPayment()
{
    if(paymentInProgress) return;
    
    paymentInProgress = true;
    createModalLayer();
    requestAttempts = 0;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    cocos2d::JniMethodInfo methodInfo;
    if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "startGooglePurchase", "()V"))
    {
        return;
    }
    
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
    
#endif
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_googlePurchaseHappened(JNIEnv* env, jobject thiz, jstring developerPayload, jstring orderId, jstring token);
};

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_googlePurchaseHappened(JNIEnv* env, jobject thiz, jstring developerPayload, jstring orderId, jstring token)
{
    const char* cDeveloperPayload = env->GetStringUTFChars(developerPayload, NULL);
    const char* cOrderId = env->GetStringUTFChars(orderId, NULL);
    const char* cToken = env->GetStringUTFChars(token, NULL);
    
    GooglePaymentSingleton::getInstance()->startBackEndPaymentVerification(std::string(cDeveloperPayload), std::string(cOrderId), std::string(cToken));
}

extern "C"

{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_googlePurchaseFailed(JNIEnv* env, jobject thiz);
};

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_googlePurchaseFailed(JNIEnv* env, jobject thiz)
{
    GooglePaymentSingleton::getInstance()->purchaseFailedBeforeFulfillment();
}

extern "C"

{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_googleDoublePurchase(JNIEnv* env, jobject thiz);
};

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_googleDoublePurchase(JNIEnv* env, jobject thiz)
{
    GooglePaymentSingleton::getInstance()->doublePurchase();
}

extern "C"

{
    JNIEXPORT jstring JNICALL Java_org_cocos2dx_cpp_AppActivity_getGoogleSku(JNIEnv* env, jobject thiz);
};

JNIEXPORT jstring JNICALL Java_org_cocos2dx_cpp_AppActivity_getGoogleSku(JNIEnv* env, jobject thiz)
{
    return env->NewStringUTF(ConfigStorage::getInstance()->getIapSkuForProvider("google").c_str());
}

extern "C"

{
    JNIEXPORT jstring JNICALL Java_org_cocos2dx_cpp_AppActivity_getLoggedInParentUserId(JNIEnv* env, jobject thiz);
};

JNIEXPORT jstring JNICALL Java_org_cocos2dx_cpp_AppActivity_getLoggedInParentUserId(JNIEnv* env, jobject thiz)
{
    return env->NewStringUTF(ParentDataProvider::getInstance()->getLoggedInParentId().c_str());
}

extern "C"

{
    JNIEXPORT jstring JNICALL Java_org_cocos2dx_cpp_AppActivity_getDeveloperKey(JNIEnv* env, jobject thiz);
};

JNIEXPORT jstring JNICALL Java_org_cocos2dx_cpp_AppActivity_getDeveloperKey(JNIEnv* env, jobject thiz)
{
    return env->NewStringUTF(ConfigStorage::getInstance()->getDeveloperPublicKey().c_str());
}

#endif
