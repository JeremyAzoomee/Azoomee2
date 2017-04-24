#include "AmazonPaymentSingleton.h"
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
using namespace Azoomee;


static AmazonPaymentSingleton *_sharedAmazonPaymentSingleton = NULL;

AmazonPaymentSingleton* AmazonPaymentSingleton::getInstance()
{
    if (! _sharedAmazonPaymentSingleton)
    {
        _sharedAmazonPaymentSingleton = new AmazonPaymentSingleton();
        _sharedAmazonPaymentSingleton->init();
    }
    
    return _sharedAmazonPaymentSingleton;
}

AmazonPaymentSingleton::~AmazonPaymentSingleton(void)
{
}

bool AmazonPaymentSingleton::init(void)
{
    return true;
}

//--------------------PAYMENT FUNCTIONS------------------

void AmazonPaymentSingleton::startIAPPayment()
{
    if(!iapAttemptInprogress)
    {
        iapAttemptInprogress = true;
        requestAttempts = 0;
        
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        
        cocos2d::JniMethodInfo methodInfo;
        if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "startAmazonPurchase", "()V"))
        {
            return;
        }
        
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
        
    #endif
    }
}

void AmazonPaymentSingleton::amazonPaymentMade(std::string requestId, std::string receiptId, std::string amazonUserid)
{
    CCLOG("IAP Request made with requestid:%s receiptID:%s userID:%s", requestId.c_str(),receiptId.c_str(),amazonUserid.c_str());
    
    savedRequestId = requestId;
    savedReceiptId = receiptId;
    savedAmazonUserid = amazonUserid;
    
    HttpRequestCreator* httpRequestCreator = new HttpRequestCreator();
    httpRequestCreator->requestBody = StringUtils::format("{\"requestId\": \"%s\", \"receiptId\": \"%s\", \"amazonUserId\": \"%s\"}", requestId.c_str(), receiptId.c_str(), amazonUserid.c_str());
    httpRequestCreator->requestTag = "iapAmazonPaymentMade";
    httpRequestCreator->createEncryptedPostHttpRequest();
}

void AmazonPaymentSingleton::onAmazonPaymentMadeAnswerReceived(std::string responseDataString)
{
    CCLOG("The response id is: %s", responseDataString.c_str());
    
    rapidjson::Document paymentData;
    paymentData.Parse(responseDataString.c_str());
    
    if(paymentData.HasParseError())
    {
        requestAttempts = requestAttempts + 1;
        amazonPaymentMade(savedRequestId, savedReceiptId, savedAmazonUserid);
        return;
    }
    
    bool paymentFailed = true;
    
    if(paymentData.HasMember("receiptStatus"))
    {
        if(paymentData["receiptStatus"].IsString())
        {
            // EXPIRED, INVALID, UNCERTAIN
            if(StringUtils::format("%s", paymentData["receiptStatus"].GetString()) == "FULFILLED")
            {
                AnalyticsSingleton::getInstance()->iapSubscriptionSuccessEvent();
                paymentFailed = false;
                
                std::string receiptId = paymentData["receiptId"].GetString();
                fulfillAmazonPayment(receiptId);
                
                removeModalLayer();
                
                BackEndCaller::getInstance()->newSubscriptionJustStarted = true;
                BackEndCaller::getInstance()->autoLogin();
            }
            else
                AnalyticsSingleton::getInstance()->iapSubscriptionErrorEvent(StringUtils::format("%s", paymentData["receiptStatus"].GetString()));
        }
    }

    if(paymentFailed)
    {
        if(requestAttempts < 4)
        {
            requestAttempts = requestAttempts + 1;
            amazonPaymentMade(savedRequestId, savedReceiptId, savedAmazonUserid);
        }
        else
        {
            removeModalLayer();
            MessageBox::createWith(ERROR_CODE_PURCHASE_FAILURE, nullptr);
            return;
        }
    }
}

void AmazonPaymentSingleton::backendRequestFailed()
{
    iapAttemptInprogress = false;
    
    MessageBox::createWith(ERROR_CODE_PURCHASE_FAILURE, nullptr);
    
}

void AmazonPaymentSingleton::fulfillAmazonPayment(std::string receiptId)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo methodInfo;
    
    if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "fulfillAmazonPurchase", "(Ljava/lang/String;)V"))
    {
        return;
    }
    jstring jReceiptId = methodInfo.env->NewStringUTF(receiptId.c_str());
    
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jReceiptId);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
#endif

}

void AmazonPaymentSingleton::purchaseFailed()
{
    CCLOG("PaymentSingleton: PURCHASE FAILED");
    AnalyticsSingleton::getInstance()->iapSubscriptionFailedEvent();
    removeModalLayer();
}

//-------------------- Modal Layer Functions----------------

void AmazonPaymentSingleton::createModalLayer()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    modalLayer = LayerColor::create(Color4B(0,0,0,150), visibleSize.width, visibleSize.height);
    modalLayer->setPosition(origin.x, origin.y);
    modalLayer->setOpacity(0);
    Director::getInstance()->getRunningScene()->addChild(modalLayer);
    
    addListenerToBackgroundLayer();
    
    modalLayer->runAction(FadeTo::create(0.5, 255));
}

void AmazonPaymentSingleton::removeModalLayer()
{
    iapAttemptInprogress = false;
    if(modalLayer) //This might be called when loading is not active, so better to check first
    {
        Director::getInstance()->getRunningScene()->removeChild(modalLayer);
    }
}

void AmazonPaymentSingleton::addListenerToBackgroundLayer()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *touch, Event *event) //Lambda callback, which is a C++ 11 feature.
    {
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), modalLayer);
}

//-------------------- JAVA RETURN FUNCTIONS-----------------

void showDoublePurchase()
{
    auto funcCallAction = CallFunc::create([=](){
        AnalyticsSingleton::getInstance()->iapSubscriptionDoublePurchaseEvent();
        AmazonPaymentSingleton::getInstance()->removeModalLayer();
        MessageBox::createWith(ERROR_CODE_PURCHASE_DOUBLE, nullptr);
    });
    
    Director::getInstance()->getRunningScene()->runAction(Sequence::create(DelayTime::create(1), funcCallAction, NULL)); //need time to get focus back from amazon window, otherwise the app will crash
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_purchaseHappened(JNIEnv* env, jobject thiz, jstring requestId, jstring receiptId, jstring amazonUserid);
};

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_purchaseHappened(JNIEnv* env, jobject thiz, jstring requestId, jstring receiptId, jstring amazonUserid)
{
    const char* cRequestId = env->GetStringUTFChars(requestId, NULL);
    const char* cReceiptId = env->GetStringUTFChars(receiptId, NULL);
    const char* cAmazonUserid = env->GetStringUTFChars(amazonUserid, NULL);
    
    CCLOG("COCOS2DX: I have the data: requestid: %s, receiptid: %s, amazonuserid: %s", cRequestId, cReceiptId, cAmazonUserid);
    
    AmazonPaymentSingleton::getInstance()->amazonPaymentMade(cRequestId, cReceiptId, cAmazonUserid);
}

extern "C"

{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_alreadyPurchased(JNIEnv* env, jobject thiz);
};

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_alreadyPurchased(JNIEnv* env, jobject thiz)
{
    CCLOG("COCOS2DX: alreadyPurchased CALLED!!!!!");
    showDoublePurchase();
}

extern "C"

{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_purchaseFailed(JNIEnv* env, jobject thiz);
};

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_purchaseFailed(JNIEnv* env, jobject thiz)
{
    CCLOG("COCOS2DX: PURCHASE FAILED");
    AmazonPaymentSingleton::getInstance()->purchaseFailed();
}

extern "C"

{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_userDataFailed(JNIEnv* env, jobject thiz);
};

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_userDataFailed(JNIEnv* env, jobject thiz)
{
    CCLOG("COCOS2DX: USER DATA FAILED");
    AnalyticsSingleton::getInstance()->iapUserDataFailedEvent();
}

extern "C"

{
    JNIEXPORT jstring JNICALL Java_org_cocos2dx_cpp_AppActivity_getAmazonSku(JNIEnv* env, jobject thiz);
};

JNIEXPORT jstring JNICALL Java_org_cocos2dx_cpp_AppActivity_getAmazonSku(JNIEnv* env, jobject thiz)
{
    return env->NewStringUTF(ConfigStorage::getInstance()->getIapSkuForProvider("amazon").c_str());
}

#endif