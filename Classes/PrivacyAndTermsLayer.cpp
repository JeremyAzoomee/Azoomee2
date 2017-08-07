#include "PrivacyAndTermsLayer.h"
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/ModalWebview.h>
#include <AzoomeeCommon/Data/Urls.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

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
    privacyButton = ElectricDreamsButton::createTextAsButton(StringMgr::getInstance()->getStringForKey(T_and_C_Privacy_Button), 50, true);
    privacyButton->setMixPanelButtonName("PrivacyPolicyButton");
    privacyButton->setDelegate(this);
    
    termsButton = ElectricDreamsButton::createTextAsButton(StringMgr::getInstance()->getStringForKey(T_and_C_Terms_Button), 50, true);
    termsButton->setMixPanelButtonName("TermsButton");
    termsButton->setDelegate(this);
}

void PrivacyAndTermsLayer::createLabel()
{
    andLabel = createLabelContentDescription(StringMgr::getInstance()->getStringForKey(T_and_C_And));
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
        ModalWebview::createWithURL(Url::PrivacyPolicy);
    else if(button == termsButton)
        ModalWebview::createWithURL(Url::TermsOfUse);
}

NS_AZOOMEE_END


