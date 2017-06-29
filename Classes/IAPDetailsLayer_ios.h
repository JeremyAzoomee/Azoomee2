#ifndef UAP_Details_Layer_ios_H
#define UAP_Details_Layer_ios_H

#include "cocos2d.h"
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>

NS_AZOOMEE_BEGIN

class IAPDetailsLayer_ios : public cocos2d::Layer, public ElectricDreamsButtonDelegate
{
private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
    cocos2d::LayerColor *backgroundLayer;
    
    cocos2d::LayerColor* IAPDetailsLayer;
    
    ElectricDreamsButton* backButton;
    
    float MarginSize;
    
    cocos2d::Label* titleLabel;
    cocos2d::Label* optionalLabel;
    
    void createBackgroundLayer();
    void addListenerToBackgroundLayer();
    
    void createUpSaleLayer();
    
    void addTitle();
    void addButtons();
    void addALLBulletsAndLabel();
    void addBulletAndLabel(std::string regularText, float yPosition);
    
public:
    virtual bool init();
    
    CREATE_FUNC(IAPDetailsLayer_ios);
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
};

NS_AZOOMEE_END

#endif
