#ifndef __IAP_UPSALE_LAYER_H__
#define __IAP_UPSALE_LAYER_H__

#include <cocos/cocos2d.h>
#include "ElectricDreamsButton.h"
#include "AwaitingAdultPinLayer.h"

USING_NS_CC;

class IAPUpsaleLayer : public Layer, public ElectricDreamsButtonDelegate, public AwaitingAdultPinLayerDelegate
{
private:
    Size visibleSize;
    Vec2 origin;
    
    LayerColor *backgroundLayer;
    
    LayerColor* UpsaleLayer;
    
    ElectricDreamsButton* startTrialButton;
    ElectricDreamsButton* restoreButton;
    ElectricDreamsButton* notNowButton;
    
    Label* titleLabel;
    Label* cancelAnytimeLabel;
    
    Sprite* oomeesImage;
    
    void askForPin();
    
    void createBackGroundLayer();
    void addListenerToBackgroundLayer();
    
    void createUpSaleLayer();
    
    void addTitle();
    void addCancelAnytimeLabel();
    void addButtons();
    void addImage();
    void addALLBulletsAndLabel();
    void addBulletAndLabel(std::string BOLDtext, std::string regularText, float yPosition);
    void addOptionalSubscriptionLabel();

    void removeSelf();
    
public:
    bool requiresPinCode;
    
    static Layer* createRequiresPin();
    
    virtual bool init();
    
    CREATE_FUNC(IAPUpsaleLayer);

    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
    void AdultPinCancelled(AwaitingAdultPinLayer* layer);
    void AdultPinAccepted(AwaitingAdultPinLayer* layer);
};

#endif
