#include "GooglePaymentSingleton.h"
#include "HttpRequestCreator.h"
#include "external/json/document.h"
#include "MessageBox.h"
#include "BackEndCaller.h"
#include "ParentDataProvider.h"
#include "AnalyticsSingleton.h"

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
    
    modalLayer = LayerColor::create(Color4B(0,0,0,150), visibleSize.width, visibleSize.height);
    modalLayer->setPosition(origin.x, origin.y);
    modalLayer->setOpacity(0);
    Director::getInstance()->getRunningScene()->addChild(modalLayer);
    
    addListenerToBackgroundLayer();
    
    modalLayer->runAction(FadeTo::create(0.5, 255));
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

void GooglePaymentSingleton::finishedIABPayment()
{
    removeModalLayer();
}

void GooglePaymentSingleton::purchaseFailed()
{
    CCLOG("PaymentSingleton: PURCHASE FAILED");
    AnalyticsSingleton::getInstance()->iapSubscriptionFailedEvent();
    removeModalLayer();
}

void GooglePaymentSingleton::showDoublePurchase()
{
    auto funcCallAction = CallFunc::create([=](){
        AnalyticsSingleton::getInstance()->iapSubscriptionDoublePurchaseEvent();
        removeModalLayer();
        MessageBox::createWith(ERROR_CODE_PURCHASE_DOUBLE, nullptr);
    });
    
    Director::getInstance()->getRunningScene()->runAction(Sequence::create(DelayTime::create(1), funcCallAction, NULL)); //need time to get focus back from google window, otherwise the app will crash
}


//--------------------PAYMENT FUNCTIONS------------------

void GooglePaymentSingleton::startIABPayment()
{
    if(paymentInProgress) return;
    
    paymentInProgress = true;
    createModalLayer();
    
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
    
    CCLOG("COCOS2DXGOOGLE: I have the data: developerPayload: %s, orderId: %s, token: %s", cDeveloperPayload, cOrderId, cToken);
    
    //call backend here to upgrade user, on success use autologin to relogin the user.
    GooglePaymentSingleton::getInstance()->finishedIABPayment();
}

extern "C"

{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_googleAlreadyPurchased(JNIEnv* env, jobject thiz);
};

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_googleAlreadyPurchased(JNIEnv* env, jobject thiz)
{
    CCLOG("COCOS2DXGOOGLE: alreadyPurchased CALLED!!!!!");
    GooglePaymentSingleton::getInstance()->showDoublePurchase();
}

extern "C"

{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_googlePurchaseFailed(JNIEnv* env, jobject thiz);
};

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_googlePurchaseFailed(JNIEnv* env, jobject thiz)
{
    CCLOG("COCOS2DXGOOGLE: PURCHASE FAILED");
    GooglePaymentSingleton::getInstance()->purchaseFailed();
}

#endif
