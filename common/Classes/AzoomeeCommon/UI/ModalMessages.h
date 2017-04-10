#ifndef AzoomeeCommon_ModalMessages_h
#define AzoomeeCommon_ModalMessages_h

#include <cocos/cocos2d.h>


namespace Azoomee
{

class ModalMessages : public cocos2d::Ref
{
public:
    /** Returns the shared instance of the Game Manager */
    static ModalMessages* getInstance(void);
    
private:
    void createAndFadeInLayer();
    void addListenerToBackgroundLayer();
    void removeLayer();
    
    cocos2d::LayerColor *loadingLayer;
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
public:
    
    virtual ~ModalMessages();
    bool init(void);
    
    void startLoading();
    void stopLoading();
    
    void showMixpanelNotification();
    void showMixpanelNotificationWithID(int notificationID);
};
  
}

#endif
