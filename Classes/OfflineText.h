#ifndef __OFFLINETEXT_H__
#define __OFFLINETEXT_H__

#include "cocos2d.h"

class OfflineText : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    CREATE_FUNC(OfflineText);
    void onEnter();
    virtual bool init();
    
    void createForLogin();
    void createForOfflineHub();
    
private:
    void addOfflineLogoToScreen();
    void addTextTitleToScreen(std::string text);
    void addTextBodyToScreen(std::string text);
    
    cocos2d::Point visibleOrigin;
    cocos2d::Size visibleSize;
};

#endif
