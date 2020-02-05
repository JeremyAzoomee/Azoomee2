#ifndef __REQUEST_ADULT_PIN_LAYER_H__
#define __REQUEST_ADULT_PIN_LAYER_H__

#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/Input/TextInputLayer.h>
#include <AzoomeeCommon/API/HttpRequestCreator.h>

NS_AZOOMEE_BEGIN

class RequestAdultPinLayer;

class RequestAdultPinLayerDelegate
{
public:
    virtual void AdultPinCancelled(RequestAdultPinLayer* layer) = 0;
    virtual void AdultPinAccepted(RequestAdultPinLayer* layer) = 0;
};

class RequestAdultPinLayer : public cocos2d::ui::Layout, public TextInputLayerDelegate, public Azoomee::HttpRequestCreatorResponseDelegate
{
    typedef cocos2d::ui::Layout Super;
private:
    cocos2d::EventListenerCustom* _biometricValidationSuccessListener = nullptr;
    cocos2d::EventListenerCustom* _biometricValidationFailureListener = nullptr;
    cocos2d::EventListenerCustom* _biometricValidationErrorListener = nullptr;
    
    static const std::string kAndroidMsgBoxName;
    
    cocos2d::LayerColor *backgroundLayer = nullptr;
    
    cocos2d::ui::Scale9Sprite* windowLayer = nullptr;
    
    float percentageOfScreenForBox;
    std::string currentTypedPinNo = "";
    
    TextInputLayer* editBox_pin = nullptr;
	
	cocos2d::ui::Button* _accept = nullptr;
    
    cocos2d::Label* enterYourPinTitle;
    
    void createBackgroundLayer();
    void addListenerToBackgroundLayer();
    void addListenerToBiometricValidationSuccess();
    void addListenerToBiometricValidationFailure();
    void setPercentageofScreenForBox();
    void addUIObjects();
    
    void removeSelf(float dt);
    
    void androidBiometricAuthenticationSuccess();
    void androidBiometricAuthenticationFailure();
    
    bool _pinIsForPayment = false;
    
    void requestUpdatedPin();
    
    void resizeWindowAndObjects();
    
    //Delegate functions
    void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body);
    void onHttpRequestFailed(const std::string& requestTag, long errorCode);
    
public:
    virtual bool init();
    virtual void onExit();
    
    CREATE_FUNC(RequestAdultPinLayer);
    
    CC_SYNTHESIZE(RequestAdultPinLayerDelegate*, _delegate, Delegate);
    
    //Delegate Functions
    void textInputIsValid(TextInputLayer* inputLayer, bool isValid);
    void textInputReturnPressed(TextInputLayer* inputLayer);
    void editBoxEditingDidBegin(TextInputLayer* inputLayer);
    void editBoxEditingDidEnd(TextInputLayer* inputLayer);
    
    void onSizeChanged();
    void checkPinAgainstStoredPin();
    void setPinIsForPayment(bool pinIsForPayment);
    
};

NS_AZOOMEE_END

#endif
