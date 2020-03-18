//
//  SettingsSupportPage.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 23/08/2018.
//

#include "SettingsSupportPage.h"
#include <TinizineCommon/Utils/LocaleManager.h>
#include <TinizineCommon/UI/LayoutParams.h>
#include <TinizineCommon/UI/Colour.h>
#include <TinizineCommon/NativeShare/NativeShare.h>
#include <TinizineCommon/Data/AppConfig.h>
#include "Style.h"

using namespace cocos2d;

USING_NS_TZ

NS_AZ_BEGIN

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
    _headerBanner->setText(AppConfig::getInstance()->getVersionNumberToDisplay());
    _headerBanner->setLayoutParameter(CreateTopLinearLayoutParam());
    this->addChild(_headerBanner);
    
    _supportBox = ui::Layout::create();
    _supportBox->setContentSize(Size(this->getContentSize().width - 100, 0));
    _supportBox->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,50,0,0)));
    _supportBox->setBackGroundImage("res/settings/rounded_rect.png");
    _supportBox->setBackGroundImageScale9Enabled(true);
	_supportBox->setLayoutType(ui::Layout::Type::VERTICAL);
    this->addChild(_supportBox);
	
	float backgroundHeight = 0;
	
	Label* title = Label::createWithTTF(_("Need some help?"), Style::Font::Medium(), 75);
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
	
	backgroundHeight += 200 + titleHolder->getContentSize().height;
	
	Label* contact = Label::createWithTTF(_("Contact us directly at"), Style::Font::Medium(), 75);
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
	
	backgroundHeight += 75 + contactHolder->getContentSize().height;
	
	Label* contactLink = Label::createWithTTF("維他命E是一種抗氧化劑，它一直在保護您的細胞和組織免受損害。您的皮膚和眼睛也喜歡它。\", \"維他命E—生育酚—喜歡脂肪。\", \"像松鼠一樣，吃些堅果—它們含豐富維他命E。您也可以在種子，水果和綠葉蔬菜中找到它。", "fonts/NotoSansTC-Regular.otf", 75);
    contactLink->setWidth(_supportBox->getContentSize().width * 0.8f);
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
			Application::getInstance()->openURL(_("mailto:help@azoomee.com"));
		}
	});
	contactLinkHolder->addChild(contactLink);
	_supportBox->addChild(contactLinkHolder);
	
	backgroundHeight += 25 + contactHolder->getContentSize().height + 200;
	
	_supportBox->setContentSize(Size(_supportBox->getContentSize().width, backgroundHeight));
	
    Super::onEnter();
}

NS_AZ_END
