#ifndef __OFFLINETEXT_H__
#define __OFFLINETEXT_H__

#include "cocos2d.h"
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
    void createForOfflineHubWhenOnline();
    
    void buttonPressed(ElectricDreamsButton* button);
    void connectivityStateChanged(bool online);
    
private:
    
    void addOfflineLogoToScreen();
    void addOnlineLogoToScreen();
    void addExitOfflineModeButtonToScreen();
    void addTextTitleToScreen(std::string text);
    void addTextBodyToScreen(std::string text);
    
    cocos2d::Point visibleOrigin;
    cocos2d::Size visibleSize;
};

#endif
