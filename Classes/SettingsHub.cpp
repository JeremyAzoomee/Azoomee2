//
//  SettingsHub.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 22/08/2018.
//

#include "SettingsHub.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "SceneManagerScene.h"
#include "SettingsSupportPage.h"
#include "SettingsOnlineSafetyPage.h"
#include "SettingsYourAccountPage.h"
#include "SettingsFriendshipsPage.h"
#include "SettingsKidsPage.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool SettingsHub::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    
    LayerColor* bgColour = LayerColor::create(Color4B::WHITE, visibleSize.width, visibleSize.height);
    this->addChild(bgColour);
    
    _contentLayout = ui::Layout::create();
    _contentLayout->setContentSize(visibleSize);
    _contentLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _contentLayout->setBackGroundColor(Style::Color::greyBlue);
    _contentLayout->setBackGroundColorOpacity(50);
    _contentLayout->setLayoutType(ui::Layout::Type::VERTICAL);
    this->addChild(_contentLayout);
    
    bool isIphoneX = ConfigStorage::getInstance()->isDeviceIphoneX();
    
    _titleLayout = ui::Layout::create();
    _titleLayout->setContentSize(Size(visibleSize.width, isIphoneX ? 250 : 150));
    _titleLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _titleLayout->setBackGroundColor(Style::Color::skyBlue);
    _contentLayout->addChild(_titleLayout);
    
    _titleBarButton = ui::Button::create("res/settings/exit_button.png");
    _titleBarButton->setNormalizedPosition(isIphoneX ? Vec2(0,0.75) : Vec2::ANCHOR_MIDDLE_LEFT);
    _titleBarButton->setAnchorPoint(Vec2(-0.5,0.5));
    _titleLayout->addChild(_titleBarButton);
    _titleBarButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            if(_inHub)
            {
                Director::getInstance()->replaceScene(SceneManagerScene::createScene(ChildSelector));
            }
            else
            {
                _activeSettingsPageHolder->setPosition(Vec2(visibleSize.width,0));
                _navigationLayout->setPosition(Vec2(0, 0));
                _titleText->setString("Settings");
                _titleBarButton->loadTextureNormal("res/settings/exit_button.png");
                _inHub = true;
            }
        }
    });
    
    _titleText = ui::Text::create("Settings", Style::Font::Medium, 91);
    _titleText->setNormalizedPosition(isIphoneX ? Vec2(0.5,0.25) : Vec2::ANCHOR_MIDDLE);
    _titleText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _titleText->setTextColor(Color4B::WHITE);
    _titleLayout->addChild(_titleText);
    
    _navigationLayout = ui::Layout::create();
    _navigationLayout->setContentSize(Size(visibleSize.width, visibleSize.height - _titleLayout->getContentSize().height));
    _navigationLayout->setLayoutType(ui::Layout::Type::VERTICAL);
    _navigationLayout->setPosition(Vec2(0,0));
    _contentLayout->addChild(_navigationLayout);
    
    _activeSettingsPageHolder = ui::Layout::create();
    _activeSettingsPageHolder->setContentSize(_navigationLayout->getContentSize());
    _activeSettingsPageHolder->setPosition(Vec2(visibleSize.width, 0));
    _contentLayout->addChild(_activeSettingsPageHolder);
    
    _kidsButton = SettingsNavigationButton::create();
    _kidsButton->setContentSize(Size(visibleSize.width, (_navigationLayout->getContentSize().height * 0.2) - 10));
    _kidsButton->setLayoutParameter(CreateTopLinearLayoutParam(ui::Margin(0,0,0,10)));
    _kidsButton->setIconFilename("res/settings/your_kids_icon_2.png");
    _kidsButton->setTitleText("Your Kids");
    _kidsButton->setSubTitleText("Lorem ipsum dolor sit amet, dolor adipiscing elit psum dolor sit.");
    _kidsButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            this->changeToPage(SettingsPages::KIDS);
        }
    });
    _kidsButton->setTouchEnabled(true);
    _navigationLayout->addChild(_kidsButton);
    
    _friendshipsButton = SettingsNavigationButton::create();
    _friendshipsButton->setContentSize(Size(visibleSize.width, (_navigationLayout->getContentSize().height * 0.2) - 10));
    _friendshipsButton->setLayoutParameter(CreateTopLinearLayoutParam(ui::Margin(0,0,0,10)));
    _friendshipsButton->setIconFilename("res/settings/friendships_icon_3.png");
    _friendshipsButton->setTitleText("Friendships");
    _friendshipsButton->setSubTitleText("Lorem ipsum dolor sit amet, dolor adipiscing elit psum dolor sit.");
    _friendshipsButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            this->changeToPage(SettingsPages::FRIENDSHIPS);
        }
    });
    _friendshipsButton->setTouchEnabled(true);
    _navigationLayout->addChild(_friendshipsButton);
    
    _yourAccountButton = SettingsNavigationButton::create();
    _yourAccountButton->setContentSize(Size(visibleSize.width, (_navigationLayout->getContentSize().height * 0.2) - 10));
    _yourAccountButton->setLayoutParameter(CreateTopLinearLayoutParam(ui::Margin(0,0,0,10)));
    _yourAccountButton->setIconFilename("res/settings/your_account_icon_4.png");
    _yourAccountButton->setTitleText("Your Account");
    _yourAccountButton->setSubTitleText("Lorem ipsum dolor sit amet, dolor adipiscing elit psum dolor sit.");
    _yourAccountButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            this->changeToPage(SettingsPages::ACCOUNT);
        }
    });
    _yourAccountButton->setTouchEnabled(true);
    _navigationLayout->addChild(_yourAccountButton);
    
    _onlineSafetyButton = SettingsNavigationButton::create();
    _onlineSafetyButton->setContentSize(Size(visibleSize.width, (_navigationLayout->getContentSize().height * 0.2) - 10));
    _onlineSafetyButton->setLayoutParameter(CreateTopLinearLayoutParam(ui::Margin(0,0,0,10)));
    _onlineSafetyButton->setIconFilename("res/settings/online_safety_icon_4.png");
    _onlineSafetyButton->setTitleText("Online Safety");
    _onlineSafetyButton->setSubTitleText("Lorem ipsum dolor sit amet, dolor adipiscing elit psum dolor sit.");
    _onlineSafetyButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            this->changeToPage(SettingsPages::ONLINE_SAFETY);
        }
    });
    _onlineSafetyButton->setTouchEnabled(true);
    _navigationLayout->addChild(_onlineSafetyButton);
    
    _supportButton = SettingsNavigationButton::create();
    _supportButton->setContentSize(Size(visibleSize.width, (_navigationLayout->getContentSize().height * 0.2) - 10));
    _supportButton->setLayoutParameter(CreateTopLinearLayoutParam(ui::Margin(0,0,0,10)));
    _supportButton->setIconFilename("res/settings/support_icon_4.png");
    _supportButton->setTitleText("Support");
    _supportButton->setSubTitleText("Lorem ipsum dolor sit amet, dolor adipiscing elit psum dolor sit.");
    _supportButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            this->changeToPage(SettingsPages::SUPPORT);
        }
    });
    _supportButton->setTouchEnabled(true);
    _navigationLayout->addChild(_supportButton);
    
    
    return true;
}

void SettingsHub::onEnter()
{
    Super::onEnter();
}

void SettingsHub::onSizeChanged()
{
    
}

void SettingsHub::changeToPage(SettingsPages page)
{
    _activeSettingsPageHolder->removeAllChildren();
    switch(page)
    {
        case SettingsPages::KIDS:
        {
            auto page = SettingsKidsPage::create();
            page->setContentSize(_activeSettingsPageHolder->getContentSize());
            _activeSettingsPageHolder->addChild(page);
            _titleText->setString("Your Kids");
            break;
        }
        case SettingsPages::FRIENDSHIPS:
        {
            auto page = SettingsFriendshipsPage::create();
            page->setContentSize(_activeSettingsPageHolder->getContentSize());
            _activeSettingsPageHolder->addChild(page);
            _titleText->setString("Friendships");
            break;
        }
        case SettingsPages::ACCOUNT:
        {
            auto page = SettingsYourAccountPage::create();
            page->setContentSize(_activeSettingsPageHolder->getContentSize());
            _activeSettingsPageHolder->addChild(page);
            _titleText->setString("Your Account");
            break;
        }
        case SettingsPages::ONLINE_SAFETY:
        {
            auto page = SettingsOnlineSafetyPage::create();
            page->setContentSize(_activeSettingsPageHolder->getContentSize());
            _activeSettingsPageHolder->addChild(page);
            _titleText->setString("Online Safety");
            break;
        }
        case SettingsPages::SUPPORT:
        {
            auto page = SettingsSupportPage::create();
            page->setContentSize(_activeSettingsPageHolder->getContentSize());
            _activeSettingsPageHolder->addChild(page);
            _titleText->setString("Support");
            break;
        }
    }
    _activeSettingsPageHolder->setPosition(Vec2(0,0));
    _navigationLayout->setPosition(Vec2(-this->getContentSize().width, 0));
    _titleBarButton->loadTextureNormal("res/settings/toggle_switch_white.png");
    _inHub = false;
}

NS_AZOOMEE_END
