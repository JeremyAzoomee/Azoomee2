#ifndef AzoomeeCommon_SessionIdManager_h
#define AzoomeeCommon_SessionIdManager_h

#include "../Tinizine.h"
#include <cocos/cocos2d.h>

NS_TZ_BEGIN
    
class SessionIdManager
{
    
public:
    SessionIdManager();
    static SessionIdManager* getInstance();
    virtual ~SessionIdManager();
    
    std::string getCurrentSessionId();
    void registerAppWentBackgroundEvent();
    void registerAppCameForegroundEvent();
    void registerAndroidSceneChangeEvent();
    void resetBackgroundTimeInContent();
    long getBackgroundTimeInContent();
    
private:
    std::string sessionId;
    long timeStampGoingBackground;
    long timeStampAndroidSceneChange;
    long backgroundTimeInContent;
    
    bool generatingNewSessionIdRequired();
    bool eventHappenedDuringAndroidSceneChange();
    void generateSessionId();
    void increaseBackgroundTimeInContent();
};

NS_TZ_END

#endif
