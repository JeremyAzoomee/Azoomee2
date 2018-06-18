#ifndef __SETTINGS_CONTROL_LAYER_H__
#define __SETTINGS_CONTROL_LAYER_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>
#include <AzoomeeCommon/UI/RequestAdultPinLayer.h>
#include <AzoomeeCommon/API/HttpRequestCreator.h>


USING_NS_CC;
NS_AZOOMEE_BEGIN

class SettingsControlLayer : public Layer, public ElectricDreamsButtonDelegate, public RequestAdultPinLayerDelegate, public Azoomee::HttpRequestCreatorResponseDelegate
{
private:
    Size _visibleSize;
    Vec2 _origin;
    
    bool returnToChatScene = false;
    
    Layer* currentTabLayer = nullptr;
    
    LayerColor *backgroundLayer = nullptr;
    float linePositionY;
    
    Sprite* confirmationNotification = nullptr;
    
    ElectricDreamsButton *cancelButton = nullptr;
    ElectricDreamsButton *accountButton = nullptr;
    ElectricDreamsButton *confirmationButton = nullptr;
    ElectricDreamsButton *childrenButton = nullptr;
    ElectricDreamsButton *onlineSafetyButton = nullptr;
    
    HttpRequestCreator* _pendingFRHttpRequest = nullptr;
    
    void createSettingsLayer();
    void addListenerToLayer(Layer* layerToAdd);
    
    void createSettingsController();
    void createCancelButton();
    void createLine();
    void createTabs();
    void createConfirmationNotification();
    void checkForConfirmationNotifications();
    
    void removeSelf();
    
    RequestAdultPinLayer* _awaitingAdultPinLayer = nullptr;
    void createAdultPinLayerWithDelegate();
    void removeAdultPinLayerDelegate();
    
    //Button Actions
    void selectNewTab(Layer* newCurrentLayer, ElectricDreamsButton* buttonToBringForward);
    
public:
    virtual bool init();
    
    static Layer* createFromChat();
    
    CREATE_FUNC(SettingsControlLayer);
    void onExit();
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
    void AdultPinCancelled(RequestAdultPinLayer* layer);
    void AdultPinAccepted(RequestAdultPinLayer* layer);
    void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body);
    void onHttpRequestFailed(const std::string& requestTag, long errorCode);
    
    ~SettingsControlLayer();
};

NS_AZOOMEE_END

#endif
