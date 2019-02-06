#include "PrivacyAndTermsLayer.h"
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/ModalWebview.h>
#include <AzoomeeCommon/Data/Urls.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

Layer* PrivacyAndTermsLayer::createGreenish()
{
    auto layer = PrivacyAndTermsLayer::create();
    layer->textColor = Style::Color::greenish;
    layer->createButtons();
    layer->createLabel();
    layer->setSizeAndAddItems();
    
    return layer;
}

Layer* PrivacyAndTermsLayer::createWhite()
{
    auto layer = PrivacyAndTermsLayer::create();
    layer->createButtons();
    layer->createLabel();
    layer->setSizeAndAddItems();
    
    return layer;
}

bool PrivacyAndTermsLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

//---------------------- Create Layer -----------------------------

void PrivacyAndTermsLayer::createButtons()
{
    privacyButton = ElectricDreamsButton::createTextAsButtonWithColor(_("Privacy Policy"), 50, true, textColor);
    privacyButton->setMixPanelButtonName("PrivacyPolicyButton");
    privacyButton->setDelegate(this);
    
    termsButton = ElectricDreamsButton::createTextAsButtonWithColor(_("Terms of Use"), 50, true,textColor);
    termsButton->setMixPanelButtonName("TermsButton");
    termsButton->setDelegate(this);
}

void PrivacyAndTermsLayer::createLabel()
{
    andLabel = createLabelContentDescription(_("and"));
    andLabel->setColor(textColor);
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
        ModalWebview::createWithURL(Url::PrivacyPolicyNoLinks);
    else if(button == termsButton)
        ModalWebview::createWithURL(Url::TermsOfUse);
}

NS_AZOOMEE_END


