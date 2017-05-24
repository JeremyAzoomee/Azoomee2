#include "SessionIdManager.h"
#include "../Analytics/AnalyticsSingleton.h"

using namespace cocos2d;
using namespace Azoomee;

namespace Azoomee
{

    static SessionIdManager *_sharedSessionIdManager = NULL;

    SessionIdManager* SessionIdManager::getInstance()
    {
        if (! _sharedSessionIdManager)
        {
            _sharedSessionIdManager = new SessionIdManager();
            _sharedSessionIdManager->init();
        }
        
        return _sharedSessionIdManager;
    }

    SessionIdManager::~SessionIdManager(void)
    {
    }

    bool SessionIdManager::init(void)
    {
        timeStampGoingBackground = 0;
        sessionId = "";
        generateSessionId();
        return true;
    }

    std::string SessionIdManager::getCurrentSessionId()
    {
        return sessionId;
    }
    
    void SessionIdManager::registerAppWentBackgroundEvent()
    {
        timeStampGoingBackground = time(NULL);
    }
    
    void SessionIdManager::registerAppCameForegroundEvent()
    {
        if(generatingNewSessionIdRequired()) generateSessionId();
    }
    
    //---------------------------------------PRIVATE METHODS--------------------------------------
    
    void SessionIdManager::generateSessionId()
    {
        std::string oldSessionId = sessionId;
        
        sessionId = "";
        static const char alphanum[] = "0123456789abcdefghijklmnopqrstuvwxyz";
       
        for(int i = 0; i < 10; i++) sessionId += alphanum[rand() % (sizeof(alphanum) - 1)];
        
        AnalyticsSingleton::getInstance()->registerSessionId(sessionId);
        cocos2d::log("NEW SESSIONID CREATED: %s", sessionId.c_str());
        
        if(oldSessionId != "")
        {
            AnalyticsSingleton::getInstance()->sessionIdHasChanged(oldSessionId, sessionId);
        }
    }
    
    bool SessionIdManager::generatingNewSessionIdRequired()
    {
        if(sessionId == "") return true;
        if(timeStampGoingBackground == 0) return true;
        if(time(NULL) - timeStampGoingBackground > 30) return true;
        
        cocos2d::log("SESSIONID NOT CHANGED: %s", sessionId.c_str());
        
        return false;
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    extern "C"
    {
        JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_NativeView_JNIRegisterAppWentBackgroundEvent(JNIEnv* env, jobject thiz);
    };
    
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_NativeView_JNIRegisterAppWentBackgroundEvent(JNIEnv* env, jobject thiz)
    {
        SessionIdManager::getInstance()->registerAppWentBackgroundEvent();
    }
    
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    extern "C"
    {
        JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_NativeView_JNIRegisterAppCameForegroundEvent(JNIEnv* env, jobject thiz);
    };
    
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_NativeView_JNIRegisterAppCameForegroundEvent(JNIEnv* env, jobject thiz)
    {
        SessionIdManager::getInstance()->registerAppCameForegroundEvent();
    }
    
#endif

}

