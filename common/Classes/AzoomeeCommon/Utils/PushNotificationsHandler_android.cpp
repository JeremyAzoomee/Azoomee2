#include "PushNotificationsHandler.h"
#include <cocos/platform/android/jni/JniHelper.h>

using namespace cocos2d;
static const std::string kAzoomeeActivityJavaClassName = "org/cocos2dx/cpp/AppActivity";

namespace Azoomee{
    
    static std::auto_ptr<PushNotificationsHandler> sPushNotificationsHandlerSharedInstance;
    
    PushNotificationsHandler* PushNotificationsHandler::getInstance()
    {
        if(!sPushNotificationsHandlerSharedInstance.get())
        {
            sPushNotificationsHandlerSharedInstance.reset(new PushNotificationsHandler());
        }
        return sPushNotificationsHandlerSharedInstance.get();
    }
    
    PushNotificationsHandler::~PushNotificationsHandler(void)
    {
        
    }
    
    void PushNotificationsHandler::setNamedUserIdentifierForPushChannel(const std::string &channelName)
    {
        
        JniHelper::callStaticVoidMethod(kAzoomeeActivityJavaClassName, "jniSetNamedUserIdentifierForPushChannel", channelName.c_str());
    }
    
    void PushNotificationsHandler::setTagForPushChannel(const std::string &tagGroup, const std::string &tag)
    {
        JniHelper::callStaticVoidMethod(kAzoomeeActivityJavaClassName, "jniSetTagForPushChannel" , tagGroup.c_str(), tag.c_str());
    }
    
    void PushNotificationsHandler::enablePushNotifications()
    {
        JniHelper::callStaticVoidMethod(kAzoomeeActivityJavaClassName, "jniEnablePushNotifications");
    }
    
    void PushNotificationsHandler::disablePushNotifications()
    {
        JniHelper::callStaticVoidMethod(kAzoomeeActivityJavaClassName, "jniDisablePushNotifications");
    }
    
    void PushNotificationsHandler::resetExistingNotifications()
    {
        JniHelper::callStaticVoidMethod(kAzoomeeActivityJavaClassName, "jniClearNotificationCenter");
    }
}
