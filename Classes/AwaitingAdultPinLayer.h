#ifndef __AWAITING_ADULT_PIN_LAYER_H__
#define __AWAITING_ADULT_PIN_LAYER_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/Input/TextInputLayer.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>
#include <AzoomeeCommon/UI/MessageBox.h>

NS_AZOOMEE_BEGIN

class AwaitingAdultPinLayer;

class AwaitingAdultPinLayerDelegate
{
public:
    virtual void AdultPinCancelled(AwaitingAdultPinLayer* layer) = 0;
    virtual void AdultPinAccepted(AwaitingAdultPinLayer* layer) = 0;
};

class AwaitingAdultPinLayer : public cocos2d::Layer, public TextInputLayerDelegate, public ElectricDreamsButtonDelegate, public MessageBoxDelegate
{
private:
    cocos2d::EventListenerCustom* _biometricValidationSuccessListener = nullptr;
    cocos2d::EventListenerCustom* _biometricValidationFailureListener = nullptr;
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
    cocos2d::LayerColor *backgroundLayer = nullptr;
    
    cocos2d::Layer* windowLayer = nullptr;
    
    TextInputLayer* editBox_pin = nullptr;
    
    ElectricDreamsButton *acceptButton = nullptr;
    ElectricDreamsButton *cancelButton = nullptr;
    
    void createAndFadeInLayer();
    void addListenerToBackgroundLayer();
    void addListenerToBiometricValidationSuccess();
    void addListenerToBiometricValidationFailure();
    void addUIObjects();
    
    void removeSelf(float dt);
    
public:
    virtual bool init();
    
    CREATE_FUNC(AwaitingAdultPinLayer);
    
    CC_SYNTHESIZE(AwaitingAdultPinLayerDelegate*, _delegate, Delegate);
    
    //Delegate Functions
    void textInputIsValid(TextInputLayer* inputLayer, bool isValid);
    void textInputReturnPressed(TextInputLayer* inputLayer);
    void editBoxEditingDidBegin(TextInputLayer* inputLayer);
    void editBoxEditingDidEnd(TextInputLayer* inputLayer);
    void buttonPressed(ElectricDreamsButton* button);
    void MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle);
    
    void secondCheckForPin();
    
};

NS_AZOOMEE_END

#endif
