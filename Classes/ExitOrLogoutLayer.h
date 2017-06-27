#ifndef __EXIT_OR_LOGOUT_LAYER_H__
#define __EXIT_OR_LOGOUT_LAYER_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>
#include "AwaitingAdultPinLayer.h"
#include "PrivacyAndTermsLayer.h"

NS_AZOOMEE_BEGIN

class ExitOrLogoutLayer : public cocos2d::Layer, public ElectricDreamsButtonDelegate, public AwaitingAdultPinLayerDelegate
{
private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
    cocos2d::LayerColor *backgroundLayer;
    cocos2d::Layer* windowLayer;
    
    ElectricDreamsButton *logoutButton;
    ElectricDreamsButton *cancelButton;
    ElectricDreamsButton *iapButton;
    
    cocos2d::Layer* privacyAndTerms;
    
    void askForPin();
    void createAndFadeInLayer();
    void addListenerToBackgroundLayer();
    void addExitOrLogoutUIObjects();
    void addRichTextLabel(std::string BOLDText);
    
    void removeSelf();
    
public:
    virtual bool init();
    
    CREATE_FUNC(ExitOrLogoutLayer);
    
    void setCenterPosition(cocos2d::Vec2 position);
    cocos2d::Vec2 getCenterPosition();
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
    void AdultPinCancelled(AwaitingAdultPinLayer* layer);
    void AdultPinAccepted(AwaitingAdultPinLayer* layer);
};

NS_AZOOMEE_END

#endif
