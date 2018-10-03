//
//  SettingsYourAccountPage.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 28/08/2018.
//

#include "SettingsYourAccountPage.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/ModalWebview.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Data/Parent/ParentDataParser.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/Data/Urls.h>
#include "LoginLogicHandler.h"
#include "PrivacyAndTermsLayer.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool SettingsYourAccountPage::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    this->setLayoutType(ui::Layout::Type::VERTICAL);
    
    return true;
}

void SettingsYourAccountPage::onEnter()
{
    _headerBanner = SettingsPageHeader::create();
    _headerBanner->setContentSize(Size(this->getContentSize().width, 316));
    _headerBanner->setText(StringMgr::getInstance()->getStringForKey(SETTINGS_PAGE_HEADING_YOUR_ACCOUNT));
    _headerBanner->setLayoutParameter(CreateTopLinearLayoutParam());
    this->addChild(_headerBanner);
    
    _editAccountLayer = EditAccountLayer::create();
    _editAccountLayer->setContentSize(Size(this->getContentSize().width - 100, this->getContentSize().height * 0.5f));
    _editAccountLayer->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,50,0,0)));
    this->addChild(_editAccountLayer);
    
    float margin = this->getContentSize().height - _headerBanner->getContentSize().height - _editAccountLayer->getContentSize().height - 200;
    
    _footerBanner = ui::Layout::create();
    _footerBanner->setContentSize(Size(this->getContentSize().width, 150));
    _footerBanner->setBackGroundColor(Style::Color::skyBlue);
    _footerBanner->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _footerBanner->setLayoutParameter(CreateTopLinearLayoutParam(ui::Margin(0,margin,0,0)));
    
    this->addChild(_footerBanner);
    
    Label* logout = Label::createWithTTF(StringMgr::getInstance()->getStringForKey(BUTTON_LOG_OUT), Style::Font::Medium, 60);
    logout->setTextColor(Color4B::WHITE);
    logout->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    logout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    ui::Button* logoutButton = ui::Button::create();
    logoutButton->setContentSize(logout->getContentSize());
    logoutButton->ignoreContentAdaptWithSize(false);
    logoutButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    logoutButton->setPosition(Vec2(_footerBanner->getContentSize().width - 25, _footerBanner->getContentSize().height * 0.5f));
    logoutButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            AnalyticsSingleton::getInstance()->logoutParentEvent();
            ParentDataParser::getInstance()->logoutChild();
            
            AudioMixer::getInstance()->stopBackgroundMusic();
            
            LoginLogicHandler::getInstance()->forceNewLogin();
        }
    });
    
    logoutButton->addChild(logout);
    
    _footerBanner->addChild(logoutButton);
    
    Label* privacy = Label::createWithTTF(StringMgr::getInstance()->getStringForKey(T_and_C_Privacy_Button), Style::Font::Medium, 60);
    privacy->setTextColor(Color4B::WHITE);
    privacy->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    privacy->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    DrawNode* privacyDrawNode = DrawNode::create();
    privacyDrawNode->drawRect(Vec2(0, -7), Vec2(privacy->getContentSize().width, -6), Color4F::WHITE);
    privacy->addChild(privacyDrawNode);
    
    ui::Button* privacyButton = ui::Button::create();
    privacyButton->setContentSize(privacy->getContentSize());
    privacyButton->ignoreContentAdaptWithSize(false);
    privacyButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    privacyButton->setPosition(Vec2(_footerBanner->getContentSize().width * 0.5, _footerBanner->getContentSize().height * 0.5f));
    privacyButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            ModalWebview::createWithURL(Url::PrivacyPolicyNoLinks);
        }
    });
    
    privacyButton->addChild(privacy);
    
    _footerBanner->addChild(privacyButton);
    
    Label* terms = Label::createWithTTF(StringMgr::getInstance()->getStringForKey(T_and_C_Terms_Button), Style::Font::Medium, 60);
    terms->setTextColor(Color4B::WHITE);
    terms->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    terms->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    DrawNode* termsDrawNode = DrawNode::create();
    termsDrawNode->drawRect(Vec2(0, -7), Vec2(terms->getContentSize().width, -6), Color4F::WHITE);
    terms->addChild(termsDrawNode);
    
    ui::Button* termsButton = ui::Button::create();
    termsButton->setContentSize(privacy->getContentSize());
    termsButton->ignoreContentAdaptWithSize(false);
    termsButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    termsButton->setPosition(Vec2(25, _footerBanner->getContentSize().height * 0.5f));
    termsButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            ModalWebview::createWithURL(Url::TermsOfUse);
        }
    });
    
    termsButton->addChild(terms);
    
    _footerBanner->addChild(termsButton);
    
    Super::onEnter();
}

NS_AZOOMEE_END

