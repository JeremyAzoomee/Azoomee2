#ifndef __OFFLINETEXT_H__
#define __OFFLINETEXT_H__

#include <cocos/cocos2d.h>
#include "ElectricDreamsButton.h"
#include "OfflineChecker.h"

class OfflineText : public cocos2d::Layer, public ElectricDreamsButtonDelegate, public OfflineCheckerDelegate
{
public:
    static cocos2d::Scene* createScene();
    
    CREATE_FUNC(OfflineText);
    void onEnter();
    virtual bool init();
    
    void createForLogin();
    void createForLoginNoUser();
    void createForOfflineHub();

    
    void buttonPressed(ElectricDreamsButton* button);
    void connectivityStateChanged(bool online);
    
private:
    
    void createForOfflineHubWhenOffline();
    void createForOfflineHubWhenOnline();
    void addOfflineLogoToScreen();
    void addOnlineLogoToScreen();
    void addExitOfflineModeButtonToScreen();
    void addRetryButtonToScreen();
    
    void addTextTitleToScreen(std::string text);
    void addTextSubTitleToScreen(std::string text);
    void addTextBodyToScreen(std::string text);
    
    cocos2d::Point visibleOrigin;
    cocos2d::Size visibleSize;
};

#endif
