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
    JNIEXPORT void JNICALL Java_com_tinizine_azoomee_common_sdk_pusher_PusherSDKAuth_buildSignedAzoomeeRequest(JNIEnv* env, jclass, jobject auth, jstring channelName, jstring socketID)
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
                jstring bodyJ = cocos2d::StringUtils::newStringUTFJNI(t.env, httpRequest->getRequestData());
                
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
                
                t.env->CallVoidMethod(auth, t.methodID);
                
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
}


NS_AZOOMEE_BEGIN

static bool sPusherInitialised = false;

void PusherSDK::subscribeToChannel(const std::string& channelName)
{
    if(!sPusherInitialised)
    {
        // Ensure the JNI native methods are compiled into the binary by making a dummy ref to it
        Java_com_tinizine_azoomee_common_sdk_pusher_PusherSDKAuth_buildSignedAzoomeeRequest(nullptr, nullptr, nullptr, nullptr, nullptr);
        
        JniHelper::callStaticVoidMethod(kPusherJavaClassName, "initialise", _appKey);
        sPusherInitialised = true;
    }
    
    _subscribedChannels.push_back(channelName);
    JniHelper::callStaticVoidMethod(kPusherJavaClassName, "subscribeToChannel", channelName);
}

void PusherSDK::closeChannel(const std::string& channelName)
{
    if(!sPusherInitialised)
    {
        return;
    }
    
    JniHelper::callStaticVoidMethod(kPusherJavaClassName, "closeChannel", channelName);
    const auto& it = std::find(_subscribedChannels.begin(), _subscribedChannels.end(), channelName);
    if(it != _subscribedChannels.end())
    {
        _subscribedChannels.erase(it);
    }
}


NS_AZOOMEE_END
