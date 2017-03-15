#include "PaymentSingleton.h"
#include "HttpRequestCreator.h"
#include "external/json/document.h"
#include "ModalMessages.h"
#include "MessageBox.h"
#include "LoginScene.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif

USING_NS_CC;

using namespace cocos2d;

static PaymentSingleton *_sharedPaymentSingleton = NULL;

PaymentSingleton* PaymentSingleton::getInstance()
{
    if (! _sharedPaymentSingleton)
    {
        _sharedPaymentSingleton = new PaymentSingleton();
        _sharedPaymentSingleton->init();
    }
    
    return _sharedPaymentSingleton;
}

PaymentSingleton::~PaymentSingleton(void)
{
}

bool PaymentSingleton::init(void)
{
    return true;
}

void PaymentSingleton::startAmazonPayment()
{
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

void PaymentSingleton::amazonPaymentMade(std::string requestId, std::string receiptId, std::string amazonUserid)
{
    ModalMessages::getInstance()->startLoading();
    
    HttpRequestCreator* httpRequestCreator = new HttpRequestCreator();
    httpRequestCreator->requestBody = StringUtils::format("{\"requestId\": \"%s\", \"receiptId\": \"%s\", \"amazonUserId\": \"%s\"}", requestId.c_str(), receiptId.c_str(), amazonUserid.c_str());
    httpRequestCreator->requestTag = "iapAmazonPaymentMade";
    httpRequestCreator->createEncryptedPostHttpRequest();
}

void PaymentSingleton::onAmazonPaymentMadeAnswerReceived(std::string responseDataString)
{
    ModalMessages::getInstance()->stopLoading();
    
    CCLOG("The response id is: %s", responseDataString.c_str());
    
    rapidjson::Document paymentData;
    paymentData.Parse(responseDataString.c_str());
    
    if(paymentData.HasParseError())
    {
        //Handle parse error
        return;
    }
    
    if(paymentData.HasMember("receiptStatus"))
    {
        if(paymentData["receiptStatus"].IsString())
        {
            if(StringUtils::format("%s", paymentData["receiptStatus"].GetString()) == "FULFILLED")
            {
                std::string receiptId = paymentData["receiptId"].GetString();
                fulfillAmazonPayment(receiptId);
                
                Director::getInstance()->replaceScene(LoginScene::createScene(ERROR_CODE_AMAZON_PURCHASE_SUCCESSFUL));
            }
            else
            {
                //handle status is not fulfilled
                MessageBox::createWith(ERROR_CODE_AMAZON_PURCHASE_FAILURE, nullptr);
                return;
            }
        }
        else
        {
            //handle receiptStatus value unexpected
            MessageBox::createWith(ERROR_CODE_AMAZON_PURCHASE_FAILURE, nullptr);
            return;
        }
    }
    else
    {
        //Handle string not having member of receiptStatus
        MessageBox::createWith(ERROR_CODE_AMAZON_PURCHASE_FAILURE, nullptr);
        return;
    }
    
}

bool PaymentSingleton::isAmazonDevice()
{
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    CCLOG("ASKING FOR AMAZON DEVICE");
    
    //cocos2d::JniMethodInfo methodInfo;
    //jclass cls2 = methodInfo.env->FindClass("org/cocos2dx/cpp/AppActivity");
    
    /*jmethodID midCallBack = methodInfo.env->GetStaticMethodID(cls2, "getAnswer", "()Ljava/lang/String;"); //"getStringToCppSide" is the name of a Java function of ofAndroid.java
    jstring resultJNIStr = (jstring)methodInfo.env->CallObjectMethod(cls2, midCallBack);
    const char *resultCStr = methodInfo.env->GetStringUTFChars(resultJNIStr, NULL);
    std::string resultStr(resultCStr);
    //methodInfo.env->ReleaseStringUTFChars(resultJNIStr, resultCStr);*/
    
    
    JniMethodInfo t;
    JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "getOSBuildManufacturer", "()Ljava/lang/String;");
    CCLOG("amazon : found getAnswer");
    jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
    CCLOG("amazon : got str");
    const char *resultCStr = t.env->GetStringUTFChars(str, NULL);
    CCLOG("amazon : got resultCStr");
    std::string resultStr(resultCStr);
    
    
    
         //const char *resultCStr = (char*)JniHelper::jstring2string(str).c_str();
        //std::string resultStr(resultCStr);
    CCLOG("trying to Show Amazon Device");
    CCLOG("IS AMAZON DEVICE:%s",resultStr.c_str());
    
    
    /*cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "getAnswer", "()V");
    
    jclass diagLayerClass = methodInfo.env->FindClass("org/cocos2dx/cpp/AppActivity");
    CCLOG("Got Class");
    jmethodID getDESCDiagLayerMethodID = methodInfo.env->GetMethodID(diagLayerClass, "getAnswer", "()Ljava/lang/String;");
    CCLOG("getDESCDiagLayerMethodID");
    jstring resultJNIStr = (jstring) methodInfo.env->CallObjectMethod(diagLayerClass, getDESCDiagLayerMethodID);
    CCLOG("resultJNIStr");
    const char *resultCStr = methodInfo.env->GetStringUTFChars(resultJNIStr, NULL);
    CCLOG("resultCStr");
    std::string resultStr(resultCStr);

    
    
    
    
    
    
    /*cocos2d::JniMethodInfo methodInfo;
    
    cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "getAnswer", "()V");
    
    jstring resultJNIStr = (jstring)methodInfo.env->CallObjectMethod(methodInfo.classID, methodInfo.methodID);
    const char *resultCStr = methodInfo.env->GetStringUTFChars(resultJNIStr, NULL);
    std::string resultStr(resultCStr);
    methodInfo.env->ReleaseStringUTFChars(resultJNIStr, resultCStr);*/
    
    
    
    
    
    
    
    
    //CCLOG("IS AMAZON DEVICE:%s",resultStr.c_str());
    
    //methodInfo.env->DeleteLocalRef(methodInfo.classID);
    
   // cocos2d::JniMethodInfo methodInfo;
    
    //cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "getAnswer", "()V");
    
    //CCLOG("ASKING FOR AMAZON DEVICE");
    
     /*   jmethodID midCallBack = getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "getAnswer", "()V");
        jstring resultJNIStr = (jstring)methodInfo.env->CallObjectMethod("org/cocos2dx/cpp/AppActivity", midCallBack);
        const char *resultCStr = methodInfo.env->GetStringUTFChars(resultJNIStr, NULL);
        std::string resultStr(resultCStr);
        //methodInfo.env->ReleaseStringUTFChars(resultJNIStr, resultCStr);
    
        CCLOG("IS AMAZON DEVICE:%s",resultStr.c_str());*/

    //jclass cls2 = methodInfo.env->FindClass("AppActivity");
    //jmethodID mid2 = methodInfo.env->GetStaticMethodID(cls2, "getAnswer", "()V");

    //auto text = methodInfo.env->CallStaticVoidMethod(cls2, mid2);
    
    //CCLOG("IS AMAZON DEVICE:%s",text.c_str());
    /*cocos2d::JniMethodInfo methodInfo;
    
    if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "getAnswer", "()V"))
    {
        return;
    }
    
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
    
    
    
    methodInfo.env->DeleteLocalRef(methodInfo.classID)
    
    
    const char *DiagLayerContainer_getDESC(JNIEnv *env, jobject diagLayer) {
        jclass diagLayerClass = env->FindClass(PARSER_CLASS);
        jmethodID getDESCDiagLayerMethodID = env->GetMethodID(diagLayerClass, "getDESCDiagLayer", "(Ljava/lang/Object;)Ljava/lang/String;");
        jstring returnString = (jstring) env->CallObjectMethod(diagLayer, getDESCDiagLayerMethodID);
        return env->GetStringUTFChars(returnString, JNI_FALSE);
    
    
    const char *js = env->GetStringUTFChars(returnString, NULL);
    std::string cs(js);
    env->ReleaseStringUTFChars(returnString, js);
    return cs;*/
    
        return true;
    #else
        return false;
    #endif
}

void PaymentSingleton::fulfillAmazonPayment(std::string receiptId)
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
