//
//  SettingsYourAccountPage.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 28/08/2018.
//

#include "SettingsYourAccountPage.h"
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/UI/Colour.h>
#include <AzoomeeCommon/Utils/LocaleManager.h>
#include <AzoomeeCommon/UI/ModalWebview.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Data/Parent/UserAccountManager.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include "Urls.h"
#include "LoginController.h"
#include "Style.h"

using namespace cocos2d;

USING_NS_TZ

NS_AZ_BEGIN

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
    _editAccountLayer = EditAccountLayer::create();
    _editAccountLayer->setContentSize(Size(this->getContentSize().width - 100, this->getContentSize().height * 0.5f));
    _editAccountLayer->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,50,0,0)));
    this->addChild(_editAccountLayer);
	
	float footerheight = 0;
	
    _footerBanner = ui::Layout::create();
    _footerBanner->setContentSize(Size(this->getContentSize().width, 0));
	_footerBanner->setLayoutType(ui::Layout::Type::VERTICAL);
    
    this->addChild(_footerBanner);
    
    Label* privacy = Label::createWithTTF(_("Privacy Policy"), Style::Font::Medium(), 60);
	privacy->setTextColor(Color4B(Colours::Color_3B::skyBlue));
    privacy->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    privacy->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	privacy->setHorizontalAlignment(TextHAlignment::CENTER);
	privacy->setVerticalAlignment(TextVAlignment::CENTER);
    
    DrawNode* privacyDrawNode = DrawNode::create();
	privacyDrawNode->drawRect(Vec2(_footerBanner->getContentSize().width * 0.4f - privacy->getContentSize().width / 2, -7), Vec2(_footerBanner->getContentSize().width * 0.4f + privacy->getContentSize().width / 2, -6), Color4F(Colours::Color_3B::skyBlue));
    privacy->addChild(privacyDrawNode);
	
	privacy->setOverflow(Label::Overflow::SHRINK);
	privacy->setDimensions(_footerBanner->getContentSize().width * 0.8f, privacy->getContentSize().height);
    
    ui::Button* privacyButton = ui::Button::create();
	privacyButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    privacyButton->setContentSize(privacy->getContentSize());
    privacyButton->ignoreContentAdaptWithSize(false);
    privacyButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    privacyButton->setPosition(Vec2(_footerBanner->getContentSize().width * 0.5f, _footerBanner->getContentSize().height * 0.5f));
    privacyButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            ModalWebview::createWithURL(Url::PrivacyPolicyNoLinks, "res/buttons/windowCloseButton.png");
        }
    });
    
    privacyButton->addChild(privacy);
    
    _footerBanner->addChild(privacyButton);
	
	footerheight += privacyButton->getContentSize().height;
	
    Label* terms = Label::createWithTTF(_("Terms of Use"), Style::Font::Medium(), 60);
	terms->setTextColor(Color4B(Colours::Color_3B::skyBlue));
    terms->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    terms->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	terms->setHorizontalAlignment(TextHAlignment::CENTER);
	terms->setVerticalAlignment(TextVAlignment::CENTER);
	
    DrawNode* termsDrawNode = DrawNode::create();
	termsDrawNode->drawRect(Vec2(_footerBanner->getContentSize().width * 0.4f - terms->getContentSize().width / 2, -7), Vec2(_footerBanner->getContentSize().width * 0.4f + terms->getContentSize().width / 2, -6), Color4F(Colours::Color_3B::skyBlue));
    terms->addChild(termsDrawNode);
	
	terms->setOverflow(Label::Overflow::SHRINK);
	terms->setDimensions(_footerBanner->getContentSize().width * 0.8f, terms->getContentSize().height);
    
    ui::Button* termsButton = ui::Button::create();
	termsButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
    termsButton->setContentSize(terms->getContentSize());
    termsButton->ignoreContentAdaptWithSize(false);
    termsButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    termsButton->setPosition(Vec2(25, _footerBanner->getContentSize().height * 0.5f));
    termsButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            ModalWebview::createWithURL(Url::TermsOfUse, "res/buttons/windowCloseButton.png");
        }
    });
    
    termsButton->addChild(terms);
    
    _footerBanner->addChild(termsButton);
	
	footerheight += termsButton->getContentSize().height + 100;
	
	Label* logout = Label::createWithTTF(_("Log out"), Style::Font::Medium(), 60);
	logout->setTextColor(Color4B(Colours::Color_3B::skyBlue));
	logout->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	logout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	logout->setHorizontalAlignment(TextHAlignment::CENTER);
	logout->setVerticalAlignment(TextVAlignment::CENTER);
	
	DrawNode* logoutDrawNode = DrawNode::create();
	logoutDrawNode->drawRect(Vec2(_footerBanner->getContentSize().width * 0.4f - logout->getContentSize().width / 2, -7), Vec2(_footerBanner->getContentSize().width * 0.4f + logout->getContentSize().width / 2, -6), Color4F(Colours::Color_3B::skyBlue));
	logout->addChild(logoutDrawNode);
	
	logout->setOverflow(Label::Overflow::SHRINK);
	logout->setDimensions(_footerBanner->getContentSize().width * 0.8f, logout->getContentSize().height);
	
	ui::Button* logoutButton = ui::Button::create();
	logoutButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	logoutButton->setContentSize(logout->getContentSize());
	logoutButton->ignoreContentAdaptWithSize(false);
	logoutButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	logoutButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			AnalyticsSingleton::getInstance()->logoutParentEvent();
			UserAccountManager::getInstance()->logoutChild();
			
			AudioMixer::getInstance()->stopBackgroundMusic();
			
			LoginController::getInstance()->forceNewLogin();
		}
	});
	
	logoutButton->addChild(logout);
	
	_footerBanner->addChild(logoutButton);
	
	footerheight += logoutButton->getContentSize().height + 100;
	
	float margin = this->getContentSize().height - _editAccountLayer->getContentSize().height - footerheight - 150;
	
	_footerBanner->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,margin,0,0)));
	_footerBanner->setContentSize(Size(_footerBanner->getContentSize().width, footerheight));
	
    Super::onEnter();
}

NS_AZ_END

