#ifndef __SETTINGS_CONTROL_LAYER_H__
#define __SETTINGS_CONTROL_LAYER_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>
#include "AwaitingAdultPinLayer.h"
#include <AzoomeeCommon/API/HttpRequestCreator.h>


USING_NS_CC;
NS_AZOOMEE_BEGIN

class SettingsControlLayer : public Layer, public ElectricDreamsButtonDelegate, public AwaitingAdultPinLayerDelegate, public Azoomee::HttpRequestCreatorResponseDelegate
{
private:
    Size visibleSize;
    Vec2 origin;
    
    bool returnToChatScene = false;
    
    Layer* currentTabLayer;
    
    LayerColor *backgroundLayer;
    float linePositionY;
    
    Sprite* confirmationNotification;
    
    ElectricDreamsButton *cancelButton;
    ElectricDreamsButton *accountButton;
    ElectricDreamsButton *confirmationButton;
    ElectricDreamsButton *childrenButton;
    
    void createSettingsLayer();
    void addListenerToLayer(Layer* layerToAdd);
    
    void createSettingsController();
    void createCancelButton();
    void createLine();
    void createTabs();
    void createConfirmationNotification();
    void checkForConfirmationNotifications();
    
    void removeSelf();
    
    //Button Actions
    void selectNewTab(Layer* newCurrentLayer, ElectricDreamsButton* buttonToBringForward);
    
public:
    virtual bool init();
    
    static Layer* createFromChat();
    
    CREATE_FUNC(SettingsControlLayer);
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
    void AdultPinCancelled(AwaitingAdultPinLayer* layer);
    void AdultPinAccepted(AwaitingAdultPinLayer* layer);
    void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body);
    void onHttpRequestFailed(const std::string& requestTag, long errorCode);
};

NS_AZOOMEE_END

#endif
