#ifndef AzoomeeCommon_SessionIdManager_h
#define AzoomeeCommon_SessionIdManager_h

#include "../Azoomee.h"
#include <cocos/cocos2d.h>

NS_AZOOMEE_BEGIN
    
class AppBackgroundManager
{
    
public:
    AppBackgroundManager();
    static AppBackgroundManager* getInstance();
    virtual ~AppBackgroundManager();

    void registerAppWentBackgroundEvent();
    void registerAppCameForegroundEvent();
    void registerAndroidSceneChangeEvent();
    void resetBackgroundTimeInContent();
    long getBackgroundTimeInContent();
    
private:
    long _timeStampGoingBackground;
    long _timeStampAndroidSceneChange;
    long _backgroundTimeInContent;
    
    bool eventHappenedDuringAndroidSceneChange();
    void increaseBackgroundTimeInContent();
};

NS_AZOOMEE_END

#endif
