#ifndef COPPA_Privacy_Layer_ios_H
#define COPPA_Privacy_Layer_ios_H

#include "cocos2d.h"
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>

NS_AZOOMEE_BEGIN

class COPPA_PrivacyLayer : public cocos2d::Layer, public ElectricDreamsButtonDelegate
{
private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
    cocos2d::LayerColor *backgroundLayer = nullptr;
    
    cocos2d::LayerColor* detailsLayer = nullptr;
    
    ElectricDreamsButton* backButton = nullptr;
    
    float MarginSize;
    
    cocos2d::Label* titleLabel = nullptr;
    cocos2d::Label* optionalLabel = nullptr;
    
    void createBackgroundLayer();
    void addListenerToBackgroundLayer();
    
    void createUpSaleLayer();
    
    void addTitle();
    void addButtons();
    void addALLBulletsAndLabel();
    void addBulletAndLabel(std::string regularText, float yPosition);
    
public:
    virtual bool init();
    
    CREATE_FUNC(COPPA_PrivacyLayer);
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
};

NS_AZOOMEE_END

#endif
