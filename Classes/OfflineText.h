#ifndef __OFFLINETEXT_H__
#define __OFFLINETEXT_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>
#include "OfflineChecker.h"

NS_AZOOMEE_BEGIN

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
    
    void onExit();
    
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

NS_AZOOMEE_END

#endif
