#ifndef AzoomeeCommon_SessionIdManager_h
#define AzoomeeCommon_SessionIdManager_h

#include "../Tinizine.h"
#include <cocos/cocos2d.h>

NS_TZ_BEGIN
    
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

NS_TZ_END

#endif
