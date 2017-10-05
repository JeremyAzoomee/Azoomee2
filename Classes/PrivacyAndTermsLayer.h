#ifndef Privacy_And_Terms_Layer_H
#define Privacy_And_Terms_Layer_H

#include "cocos2d.h"
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>
#include <AzoomeeCommon/UI/Style.h>

NS_AZOOMEE_BEGIN

class PrivacyAndTermsLayer : public cocos2d::Layer, public Azoomee::ElectricDreamsButtonDelegate
{
private:
    cocos2d::Color3B textColor = Style::Color::white;
    
    Azoomee::ElectricDreamsButton *privacyButton = nullptr;
    Azoomee::ElectricDreamsButton *termsButton = nullptr;
    
    cocos2d::Label* andLabel = nullptr;
    
    void createButtons();
    void createLabel();
    void setSizeAndAddItems();

public:
    
    static Layer* createGreenish();
    static Layer* createWhite();
    
    virtual bool init();
    
    CREATE_FUNC(PrivacyAndTermsLayer);
    
    void setCenterPosition(cocos2d::Vec2 position);
    cocos2d::Vec2 getCenterPosition();
    
    //Delegate Functions
    void buttonPressed(Azoomee::ElectricDreamsButton* button);
};

NS_AZOOMEE_END

#endif
