#ifndef __IAP_UPSALE_LAYER_H__
#define __IAP_UPSALE_LAYER_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>
#include "AwaitingAdultPinLayer.h"


NS_AZOOMEE_BEGIN

class IAPUpsaleLayer : public cocos2d::Layer, public ElectricDreamsButtonDelegate, public AwaitingAdultPinLayerDelegate
{
private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
    cocos2d::LayerColor *backgroundLayer;
    
    cocos2d::LayerColor* UpsaleLayer;
    
    ElectricDreamsButton* startTrialButton;
    ElectricDreamsButton* restoreButton;
    ElectricDreamsButton* notNowButton;
    ElectricDreamsButton* learnMoreButton;
    ElectricDreamsButton* privacyButton;
    
    cocos2d::Label* titleLabel;
    cocos2d::Label* optionalLabel;
    
    cocos2d::Sprite* oomeesImage;
    
    float bulletTextXposition;
    
    void askForPin();
    
    bool restoreButtonPressed = false;
    
    void createBackgroundLayer();
    void addListenerToBackgroundLayer();
    
    void createUpSaleLayer();
    
    void addTitle();
    void addButtons();
    void addImage();
    void addALLBulletsAndLabel();
    void addBulletAndLabel(std::string BOLDtext, std::string regularText, float yPosition);
    void addOptionalSubscriptionLabel();

    void removeSelf();
    
public:
    bool requiresPinCode = false;
    
    static cocos2d::Layer* createRequiresPin();
    
    virtual bool init();
    
    CREATE_FUNC(IAPUpsaleLayer);

    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
    void AdultPinCancelled(AwaitingAdultPinLayer* layer);
    void AdultPinAccepted(AwaitingAdultPinLayer* layer);
};

NS_AZOOMEE_END

#endif
