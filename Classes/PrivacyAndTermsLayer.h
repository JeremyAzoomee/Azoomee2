#ifndef Privacy_And_Terms_Layer_H
#define Privacy_And_Terms_Layer_H

#include "cocos2d.h"
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>

NS_AZOOMEE_BEGIN

class PrivacyAndTermsLayer : public cocos2d::Layer, public Azoomee::ElectricDreamsButtonDelegate
{
private:
    
    Azoomee::ElectricDreamsButton *privacyButton;
    Azoomee::ElectricDreamsButton *termsButton;
    
    cocos2d::Label* andLabel;
    
    void createButtons();
    void createLabel();
    void setSizeAndAddItems();

public:
    virtual bool init();
    
    CREATE_FUNC(PrivacyAndTermsLayer);
    
    void setCenterPosition(cocos2d::Vec2 position);
    cocos2d::Vec2 getCenterPosition();
    
    //Delegate Functions
    void buttonPressed(Azoomee::ElectricDreamsButton* button);
};

NS_AZOOMEE_END

#endif
