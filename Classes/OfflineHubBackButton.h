#ifndef __OFFLINEHUB_BACKBUTTON_H__
#define __OFFLINEHUB_BACKBUTTON_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

class OfflineHubBackButton : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    CREATE_FUNC(OfflineHubBackButton);
    
    virtual bool init();
    
private:
    cocos2d::Sprite* createBackButton();
    void addListenerToBackButton(cocos2d::Sprite* toBeAddedTo);

};

NS_AZOOMEE_END

#endif
