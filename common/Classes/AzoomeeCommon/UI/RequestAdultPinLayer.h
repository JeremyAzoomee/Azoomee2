#ifndef __REQUEST_ADULT_PIN_LAYER_H__
#define __REQUEST_ADULT_PIN_LAYER_H__

#include <cocos/cocos2d.h>
#include "../Azoomee.h"
#include "../Input/TextInputLayer.h"
#include "ElectricDreamsButton.h"
#include "MessageBox.h"
#include "../API/HttpRequestCreator.h"

NS_AZOOMEE_BEGIN

class RequestAdultPinLayer;

class RequestAdultPinLayerDelegate
{
public:
    virtual void AdultPinCancelled(RequestAdultPinLayer* layer) = 0;
    virtual void AdultPinAccepted(RequestAdultPinLayer* layer) = 0;
};

class RequestAdultPinLayer : public cocos2d::Layer, public TextInputLayerDelegate, public ElectricDreamsButtonDelegate, public MessageBoxDelegate, public Azoomee::HttpRequestCreatorResponseDelegate
{
private:
    
    cocos2d::LayerColor *backgroundLayer;
    
    cocos2d::ui::Scale9Sprite* windowLayer;
    
    float percentageOfScreenForBox;
    std::string currentTypedPinNo = "";
    
    TextInputLayer* editBox_pin;
    
    ElectricDreamsButton *acceptButton;
    ElectricDreamsButton *cancelButton;
    ElectricDreamsButton *placeHolderAcceptButton;
    
    cocos2d::Label* enterYourPinTitle;
    
    void createBackgroundLayer();
    void addListenerToBackgroundLayer();
    void setPercentageofScreenForBox();
    void addUIObjects();
    
    void removeSelf(float dt);
    
    void requestUpdatedPin();
    void checkPinAgainstStoredPin();
    
    void resizeWindowAndObjects();
    
    //Delegate functions
    void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body);
    void onHttpRequestFailed(const std::string& requestTag, long errorCode);
    
public:
    virtual bool init();
    
    CREATE_FUNC(RequestAdultPinLayer);
    
    CC_SYNTHESIZE(RequestAdultPinLayerDelegate*, _delegate, Delegate);
    
    //Delegate Functions
    void textInputIsValid(TextInputLayer* inputLayer, bool isValid);
    void textInputReturnPressed(TextInputLayer* inputLayer);
    void editBoxEditingDidBegin(TextInputLayer* inputLayer);
    void editBoxEditingDidEnd(TextInputLayer* inputLayer);
    void buttonPressed(ElectricDreamsButton* button);
    void MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle);
    
    void onSizeChanged();
    
};

NS_AZOOMEE_END

#endif
