#ifndef UAP_Details_Layer_ios_H
#define UAP_Details_Layer_ios_H

#include "cocos2d.h"
#include "ElectricDreamsButton.h"

USING_NS_CC;

class IAPDetailsLayer_ios : public Layer, public ElectricDreamsButtonDelegate
{
private:
    Size visibleSize;
    Vec2 origin;
    
    LayerColor *backgroundLayer;
    
    LayerColor* IAPDetailsLayer;
    
    ElectricDreamsButton* backButton;
    
    float MarginSize;
    
    Label* titleLabel;
    Label* optionalLabel;
    
    void createBackGroundLayer();
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

#endif
