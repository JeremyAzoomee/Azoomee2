//
//  SettingsSupportPage.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 23/08/2018.
//

#include "SettingsSupportPage.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/NativeShare/NativeShare.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool SettingsSupportPage::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    this->setLayoutType(ui::Layout::Type::VERTICAL);
    
    return true;
}

void SettingsSupportPage::onEnter()
{
    _headerBanner = SettingsPageHeader::create();
    _headerBanner->setContentSize(Size(this->getContentSize().width, 316));
    _headerBanner->setText(ConfigStorage::getInstance()->getVersionNumberToDisplay());
    _headerBanner->setLayoutParameter(CreateTopLinearLayoutParam());
    this->addChild(_headerBanner);
    
    _supportBox = ui::Layout::create();
    _supportBox->setContentSize(Size(this->getContentSize().width - 100, this->getContentSize().height * 0.45f));
    _supportBox->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,50,0,0)));
    _supportBox->setBackGroundImage("res/settings/rounded_rect.png");
    _supportBox->setBackGroundImageScale9Enabled(true);
	_supportBox->setLayoutType(ui::Layout::Type::VERTICAL);
    this->addChild(_supportBox);
	
	Label* title = Label::createWithTTF(_("Need some help?"), Style::Font::Medium, 75);
	title->setWidth(_supportBox->getContentSize().width * 0.8f);
	title->setHorizontalAlignment(TextHAlignment::CENTER);
	title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	title->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	title->setTextColor(Color4B::BLACK);
	
	ui::Layout* titleHolder = ui::Layout::create();
	titleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,200,0,0)));
	titleHolder->setContentSize(title->getContentSize());
	titleHolder->addChild(title);
	_supportBox->addChild(titleHolder);
	
	Label* support = Label::createWithTTF(_("Visit our support page at"), Style::Font::Medium, 75);
	support->setWidth(_supportBox->getContentSize().width * 0.8f);
	support->setHorizontalAlignment(TextHAlignment::CENTER);
	support->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	support->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	support->setTextColor(Color4B::BLACK);
	
	ui::Layout* supportHolder = ui::Layout::create();
	supportHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,75,0,0)));
	supportHolder->setContentSize(support->getContentSize());
	supportHolder->addChild(support);
	_supportBox->addChild(supportHolder);
	
	Label* supportLink = Label::createWithTTF("support.azoomee.com", Style::Font::Medium, 75);
	supportLink->setHorizontalAlignment(TextHAlignment::CENTER);
	supportLink->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	supportLink->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	supportLink->setTextColor(Color4B::BLACK);
	
	DrawNode* supportDrawNode = DrawNode::create();
	supportDrawNode->drawRect(Vec2(0, -7), Vec2(supportLink->getContentSize().width, -6), Color4F::BLACK);
	supportLink->addChild(supportDrawNode);
	
	ui::Layout* supportLinkHolder = ui::Layout::create();
	supportLinkHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,25,0,0)));
	supportLinkHolder->setContentSize(supportLink->getContentSize());
	supportLinkHolder->addChild(supportLink);
	supportLinkHolder->setTouchEnabled(true);
	supportLinkHolder->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			Application::getInstance()->openURL("http://support.azoomee.com");
		}
	});
	_supportBox->addChild(supportLinkHolder);
	
	Label* orText = Label::createWithTTF(_("Or"), Style::Font::Medium, 75);
	orText->setWidth(_supportBox->getContentSize().width * 0.8f);
	orText->setHorizontalAlignment(TextHAlignment::CENTER);
	orText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	orText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	orText->setTextColor(Color4B::BLACK);
	
	ui::Layout* orTextHolder = ui::Layout::create();
	orTextHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,75,0,0)));
	orTextHolder->setContentSize(orText->getContentSize());
	orTextHolder->addChild(orText);
	_supportBox->addChild(orTextHolder);
	
	Label* contact = Label::createWithTTF(_("Contact us directly at"), Style::Font::Medium, 75);
	contact->setWidth(_supportBox->getContentSize().width * 0.8f);
	contact->setHorizontalAlignment(TextHAlignment::CENTER);
	contact->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	contact->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	contact->setTextColor(Color4B::BLACK);
	
	ui::Layout* contactHolder = ui::Layout::create();
	contactHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,75,0,0)));
	contactHolder->setContentSize(contact->getContentSize());
	contactHolder->addChild(contact);
	_supportBox->addChild(contactHolder);
	
	Label* contactLink = Label::createWithTTF("help@azoomee.com", Style::Font::Medium, 75);
	contactLink->setHorizontalAlignment(TextHAlignment::CENTER);
	contactLink->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	contactLink->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	contactLink->setTextColor(Color4B::BLACK);
	
	DrawNode* contactDrawNode = DrawNode::create();
	contactDrawNode->drawRect(Vec2(0, -7), Vec2(contactLink->getContentSize().width, -6), Color4F::BLACK);
	contactLink->addChild(contactDrawNode);
	
	ui::Layout* contactLinkHolder = ui::Layout::create();
	contactLinkHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,25,0,0)));
	contactLinkHolder->setContentSize(contactLink->getContentSize());
	contactLinkHolder->setTouchEnabled(true);
	contactLinkHolder->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			Application::getInstance()->openURL("mailto:help@azoomee.com");
		}
	});
	contactLinkHolder->addChild(contactLink);
	_supportBox->addChild(contactLinkHolder);
	
    Super::onEnter();
}

NS_AZOOMEE_END
