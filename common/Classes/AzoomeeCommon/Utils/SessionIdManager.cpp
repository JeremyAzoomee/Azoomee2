#include "SessionIdManager.h"
#include "../Analytics/AnalyticsSingleton.h"
#include <chrono>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

static std::auto_ptr<SessionIdManager> sSessionIdManagerSharedInstance;
static int lengthOfGeneratedId = 20;

SessionIdManager::SessionIdManager()
{
    timeStampGoingBackground = 0;
    timeStampAndroidSceneChange = 0;
    backgroundTimeInContent = 0;
    sessionId = "";
    generateSessionId();
}

SessionIdManager* SessionIdManager::getInstance()
{
    if(!sSessionIdManagerSharedInstance.get())
    {
        sSessionIdManagerSharedInstance.reset(new SessionIdManager());
    }
    return sSessionIdManagerSharedInstance.get();
}

SessionIdManager::~SessionIdManager(void)
{
    
}

std::string SessionIdManager::getCurrentSessionId()
{
    return sessionId;
}

void SessionIdManager::registerAppWentBackgroundEvent()
{
    timeStampGoingBackground = time(NULL);
    
    if(!eventHappenedDuringAndroidSceneChange())
    {
        AnalyticsSingleton::getInstance()->enteredBackgroundEvent();
    }
}

void SessionIdManager::registerAppCameForegroundEvent()
{
    if(generatingNewSessionIdRequired())
    {
        generateSessionId();
    }
    
    if(!eventHappenedDuringAndroidSceneChange())
    {
        increaseBackgroundTimeInContent();
        AnalyticsSingleton::getInstance()->enteredForegroundEvent();
    }
}

void SessionIdManager::registerAndroidSceneChangeEvent()
{
    timeStampAndroidSceneChange = time(NULL);
}

void SessionIdManager::resetBackgroundTimeInContent()
{
    backgroundTimeInContent = 0;
}

long SessionIdManager::getBackgroundTimeInContent()
{
    return backgroundTimeInContent;
}

//---------------------------------------PRIVATE METHODS--------------------------------------

void SessionIdManager::generateSessionId()
{
    std::string oldSessionId = sessionId;
    
    //generate new sessionid and narrow down overlapping
    
    //get timestamp in microseconds
    
    std::chrono::microseconds ms = std::chrono::duration_cast< std::chrono::microseconds >(std::chrono::system_clock::now().time_since_epoch());
    
    long epochTimeInMicroSeconds = ms.count();
    
    //get some device specific identifiers to decrease overlap chance further
    
    const std::string &deviceData = ConfigStorage::getInstance()->getDeviceInformation();
    const std::string &deviceIDFA = ConfigStorage::getInstance()->getDeviceAdvertisingId();
    std::string deviceString = deviceData + deviceIDFA;
    
    std::hash<std::string> hasher;
    std::size_t hashedDeviceString = hasher(deviceString);
    
    //summarise two values and make it fit in an int type
    
    std::size_t randomResetLong = epochTimeInMicroSeconds + hashedDeviceString;
    
    while (randomResetLong >= INT_MAX)
    {
        randomResetLong = atol(StringUtils::format("%lu", randomResetLong).substr(1).c_str()); //removing first digit until value is smaller than INT_MAX
    }
    
    int randomResetInt = (int)randomResetLong;
    
    //reset random generator
    srand(randomResetInt);
    
    sessionId = "";
    static const char alphanum[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
   
    for(int i = 0; i < lengthOfGeneratedId; i++)
    {
       sessionId += alphanum[rand() % (sizeof(alphanum) - 1)]; 
    }
    
    AnalyticsSingleton::getInstance()->registerSessionId(sessionId);
    
    if(oldSessionId != "")
    {
        AnalyticsSingleton::getInstance()->sessionIdHasChanged(oldSessionId);
    }
}

bool SessionIdManager::generatingNewSessionIdRequired()
{
    if(sessionId == "")
    {
        return true;
    }
    
    if(timeStampGoingBackground == 0)
    {
        return true;
    }
    
    if(time(NULL) - timeStampGoingBackground > 30)
    {
        return true;
    }
    
    return false;
}

bool SessionIdManager::eventHappenedDuringAndroidSceneChange()
{
    if(timeStampAndroidSceneChange == 0)
    {
        return false;
    }
    
    if(time(NULL) - timeStampAndroidSceneChange > 2)
    {
        return false;
    }
    
    return true;
}

void SessionIdManager::increaseBackgroundTimeInContent()
{
    if(timeStampGoingBackground == 0)
    {
        return;
    }
    
    long timeSpentBackground = time(NULL) - timeStampGoingBackground;
    
    if(timeSpentBackground > 2)
    {
        backgroundTimeInContent += timeSpentBackground;
    }
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
    {
        JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_JNICalls_JNIRegisterAppWentBackgroundEvent(JNIEnv* env, jobject thiz);
    };

    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_JNICalls_JNIRegisterAppWentBackgroundEvent(JNIEnv* env, jobject thiz)
    {
        SessionIdManager::getInstance()->registerAppWentBackgroundEvent();
    }

#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

    extern "C"
    {
        JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_JNICalls_JNIRegisterAppCameForegroundEvent(JNIEnv* env, jobject thiz);
    };

    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_JNICalls_JNIRegisterAppCameForegroundEvent(JNIEnv* env, jobject thiz)
    {
        SessionIdManager::getInstance()->registerAppCameForegroundEvent();
    }

#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

    extern "C"
    {
        JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_JNICalls_JNIRegisterAndroidSceneChangeEvent(JNIEnv* env, jobject thiz);
    };

    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_JNICalls_JNIRegisterAndroidSceneChangeEvent(JNIEnv* env, jobject thiz)
    {
        SessionIdManager::getInstance()->registerAndroidSceneChangeEvent();
    }

#endif


NS_AZOOMEE_END

