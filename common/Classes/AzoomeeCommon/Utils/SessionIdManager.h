#ifndef AzoomeeCommon_SessionIdManager_h
#define AzoomeeCommon_SessionIdManager_h

#include "../Azoomee.h"
#include <cocos/cocos2d.h>

NS_AZOOMEE_BEGIN
    
class SessionIdManager: public cocos2d::Ref
{
    
public:
    SessionIdManager();
    static SessionIdManager* getInstance();
    virtual ~SessionIdManager();
    bool init(void);
    
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

NS_AZOOMEE_END

#endif
