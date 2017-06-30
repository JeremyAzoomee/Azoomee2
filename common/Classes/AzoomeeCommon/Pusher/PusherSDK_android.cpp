#include "PusherSDK.h"
#include "../API/API.h"
#include "../Data/Parent/ParentDataProvider.h"
#include <cocos/platform/android/jni/JniHelper.h>
#include <algorithm>

using namespace cocos2d;
using namespace Azoomee;

static const std::string kPusherJavaClassName = "com/tinizine/azoomee/common/sdk/pusher/PusherSDK";
static const std::string kPusherAuthJavaClassName = "com/tinizine/azoomee/common/sdk/pusher/PusherSDKAuth";

extern "C"
{
    JNIEXPORT void JNICALL Java_com_tinizine_azoomee_common_sdk_pusher_PusherSDKAuth_buildSignedAzoomeeRequest(JNIEnv* env, jobject obj, jstring channelName, jstring socketID)
    {
        if(env)
        {
            // Create the Azoomee signed request
            ParentDataProvider* parentData = ParentDataProvider::getInstance();
            HttpRequestCreator* httpReqCreator = API::PusherAuthRequest(parentData->getLoggedInParentId(), JniHelper::jstring2string(channelName), JniHelper::jstring2string(socketID), nullptr);
            
            // Convert cocos HttpRequest and pass back to JNI
            cocos2d::network::HttpRequest* httpRequest = httpReqCreator->buildHttpRequest();
            const char* url = httpRequest->getUrl();
            const std::vector<std::string>& headers = httpRequest->getHeaders();
            cocos2d::network::HttpRequest::Type method = httpRequest->getRequestType();
            const char* requestData = httpRequest->getRequestData();
            
            // Set the request params back to the Auth object
            cocos2d::JniMethodInfo t;
            // final String url, final String method, final String[] headerKeys, final String[] headerValues, final String bodyData
            const std::string className = kPusherAuthJavaClassName;
            const std::string methodName = "setRequestParams";
            const std::string signature = "(Ljava/lang/String;Ljava/lang/String;[Ljava/lang/String;[Ljava/lang/String;Ljava/lang/String;)V";
            
            if(cocos2d::JniHelper::getMethodInfo(t, className.c_str(), methodName.c_str(), signature.c_str()))
            {
                jstring urlJ = cocos2d::StringUtils::newStringUTFJNI(t.env, url);
                jstring methodJ = cocos2d::StringUtils::newStringUTFJNI(t.env, method == cocos2d::network::HttpRequest::Type::POST ? "POST" : "GET");
                jstring bodyJ = cocos2d::StringUtils::newStringUTFJNI(t.env, requestData ? requestData : "null");
                
                jobjectArray headerKeysJ = t.env->NewObjectArray(headers.size(), t.env->FindClass("java/lang/String"), 0);
                jobjectArray headerValuesJ = t.env->NewObjectArray(headers.size(), t.env->FindClass("java/lang/String"), 0);
                for(int i = 0; i < headers.size(); ++i)
                {
                    const std::string& header = headers[i];
                    size_t splitIndex = header.find_first_of(":");
                    const std::string& key = header.substr(0, splitIndex);
                    const std::string& value = header.substr(splitIndex + 1, header.length() - splitIndex);
                    
                    jstring keyK = t.env->NewStringUTF(key.c_str());
                    t.env->SetObjectArrayElement(headerKeysJ, i, keyK);
                    t.env->DeleteLocalRef(keyK);
                    
                    jstring valueK = t.env->NewStringUTF(value.c_str());
                    t.env->SetObjectArrayElement(headerValuesJ, i, valueK);
                    t.env->DeleteLocalRef(valueK);
                }
                
                t.env->CallVoidMethod(obj, t.methodID, urlJ, methodJ, headerKeysJ, headerValuesJ, bodyJ);
                
                // Delete local JNI refs which we are done with
                t.env->DeleteLocalRef(t.classID);
                t.env->DeleteLocalRef(urlJ);
                t.env->DeleteLocalRef(methodJ);
                t.env->DeleteLocalRef(bodyJ);
                t.env->DeleteLocalRef(headerKeysJ);
                t.env->DeleteLocalRef(headerValuesJ);
            }
            else
            {
                cocos2d::log("Failed to find java method. Class name: %s, method name: %s, signature: %s", className.c_str(), methodName.c_str(), signature.c_str());
            }
        }
    }
    
    JNIEXPORT void JNICALL Java_com_tinizine_azoomee_common_sdk_pusher_PusherSDK_onPusherEventRecieved(JNIEnv* env, jobject obj, jstring channelName, jstring eventName, jstring data)
    {
        if(env)
        {
            // Create the Pusher event object
            PusherEventRef event = PusherEvent::create(JniHelper::jstring2string(channelName), JniHelper::jstring2string(eventName), JniHelper::jstring2string(data));
            if(event)
            {
                cocos2d::log("Pusher event: channelName=%s, eventName=%s, data=%s", event->channelName().c_str(), event->eventName().c_str(), JniHelper::jstring2string(data).c_str());
                PusherSDK::getInstance()->notifyObservers(event);
            }
        }
    }
}


NS_AZOOMEE_BEGIN

static bool sPusherInitialised = false;

void PusherSDK::subscribeToChannel(const std::string& channelName)
{
    if(!sPusherInitialised)
    {
        // Ensure the JNI native methods are compiled into the binary by making a dummy ref to it
        Java_com_tinizine_azoomee_common_sdk_pusher_PusherSDKAuth_buildSignedAzoomeeRequest(nullptr, nullptr, nullptr, nullptr);
        Java_com_tinizine_azoomee_common_sdk_pusher_PusherSDK_onPusherEventRecieved(nullptr, nullptr, nullptr, nullptr, nullptr);
        
        JniHelper::callStaticVoidMethod(kPusherJavaClassName, "Initialise", _appKey);
        sPusherInitialised = true;
    }
    
    _subscribedChannels.push_back(channelName);
    JniHelper::callStaticVoidMethod(kPusherJavaClassName, "SubscribeToChannel", channelName);
}

void PusherSDK::closeChannel(const std::string& channelName)
{
    if(!sPusherInitialised)
    {
        return;
    }
    
    JniHelper::callStaticVoidMethod(kPusherJavaClassName, "CloseChannel", channelName);
    const auto& it = std::find(_subscribedChannels.begin(), _subscribedChannels.end(), channelName);
    if(it != _subscribedChannels.end())
    {
        _subscribedChannels.erase(it);
    }
}


NS_AZOOMEE_END
