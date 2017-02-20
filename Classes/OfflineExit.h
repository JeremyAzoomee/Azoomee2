#ifndef __OFFLINEHUB_SCENE_H__
#define __OFFLINEHUB_SCENE_H__

#include "cocos2d.h"

class OfflineExit : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    CREATE_FUNC(OfflineExit);
    void onEnter();
    virtual bool init();
    
private:
    void addOfflineLogoToScreen();
    void addOfflineTextToScren();
    void addExitButtonToScreen();
    
    cocos2d::Point visibleOrigin;
    cocos2d::Size visibleSize;
};

#endif
