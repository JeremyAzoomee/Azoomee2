#ifndef Privacy_And_Terms_Layer_H
#define Privacy_And_Terms_Layer_H

#include "cocos2d.h"
#include "ElectricDreamsButton.h"

USING_NS_CC;

class PrivacyAndTermsLayer : public Layer, public ElectricDreamsButtonDelegate
{
private:
    
    ElectricDreamsButton *privacyButton;
    ElectricDreamsButton *termsButton;
    
    Label* andLabel;
    
    void createButtons();
    void createLabel();
    void setSizeAndAddItems();

public:
    virtual bool init();
    
    CREATE_FUNC(PrivacyAndTermsLayer);
    
    void setCenterPosition(Vec2 position);
    Vec2 getCenterPosition();
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
};

#endif
