#include "PrivacyAndTermsLayer.h"
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>

bool PrivacyAndTermsLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    createButtons();
    createLabel();
    setSizeAndAddItems();
    
    return true;
}

//---------------------- Create Layer -----------------------------

void PrivacyAndTermsLayer::createButtons()
{
    privacyButton = ElectricDreamsButton::createTextAsButton("Privacy Policy", 50, true);
    privacyButton->setDelegate(this);
    
    termsButton = ElectricDreamsButton::createTextAsButton("Terms of Use", 50, true);
    termsButton->setDelegate(this);
}

void PrivacyAndTermsLayer::createLabel()
{
    andLabel = createLabelContentDescription("and");
}

void PrivacyAndTermsLayer::setSizeAndAddItems()
{
    float layerWidth = privacyButton->getContentSize().width + termsButton->getContentSize().width + andLabel->getContentSize().width + 40;
    
    this->setContentSize(Size(layerWidth,privacyButton->getContentSize().height));
    
    privacyButton->setPosition(0,0);
    this->addChild(privacyButton);
    
    andLabel->setPosition(privacyButton->getContentSize().width+20+andLabel->getContentSize().width/2, andLabel->getContentSize().height/2);
    this->addChild(andLabel);
    
    termsButton->setPosition(andLabel->getPositionX()+andLabel->getContentSize().width/2+20,0);
    this->addChild(termsButton);
}


//---------------------- public Functions After Setup -----------------------------

void PrivacyAndTermsLayer::setCenterPosition(Vec2 position)
{
    this->setPosition(Vec2(position.x - this->getContentSize().width/2, position.y - this->getContentSize().height/2));
}

Vec2 PrivacyAndTermsLayer::getCenterPosition()
{
    return Vec2(this->getPositionX() + this->getContentSize().width/2, this->getPositionY() + this->getContentSize().height/2);
}

//----------------------- Delegate Functions ----------------------------

void PrivacyAndTermsLayer::buttonPressed(ElectricDreamsButton* button)
{
    if(button == privacyButton)
    {
        Application::getInstance()->openURL("http://azoomee.com/index.php/privacy-policy-2/");
    }
    else if(button == termsButton)
    {
        Application::getInstance()->openURL("http://azoomee.com/index.php/terms-and-conditions/");
    }

}


