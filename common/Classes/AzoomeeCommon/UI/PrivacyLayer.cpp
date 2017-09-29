#include "PrivacyLayer.h"
#include "ElectricDreamsTextStyles.h"
#include "../Strings.h"
#include "ModalWebview.h"
#include "../Data/Urls.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool PrivacyLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    createButtons();
    setSizeAndAddItems();
    
    return true;
}

//---------------------- Create Layer -----------------------------

void PrivacyLayer::createButtons()
{
    privacyButton = ElectricDreamsButton::createTextAsButtonAqua(StringMgr::getInstance()->getStringForKey(T_and_C_Privacy_Button), 40, true);
    privacyButton->setMixPanelButtonName("PrivacyPolicyButton");
    privacyButton->setDelegate(this);
}

void PrivacyLayer::setSizeAndAddItems()
{
    this->setContentSize(privacyButton->getContentSize());
    
    privacyButton->setPosition(0,0);
    this->addChild(privacyButton);
}


//---------------------- public Functions After Setup -----------------------------

void PrivacyLayer::setCenterPosition(Vec2 position)
{
    this->setPosition(Vec2(position.x - this->getContentSize().width/2, position.y - this->getContentSize().height/2));
}

Vec2 PrivacyLayer::getCenterPosition()
{
    return Vec2(this->getPositionX() + this->getContentSize().width/2, this->getPositionY() + this->getContentSize().height/2);
}

//----------------------- Delegate Functions ----------------------------

void PrivacyLayer::buttonPressed(ElectricDreamsButton* button)
{
    if(button == privacyButton)
        ModalWebview::createWithURL(Url::PrivacyPolicyNoLinks);
}

NS_AZOOMEE_END


