//
//  ImportantMessageForParents.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 14/08/2019.
//

#include "ImportantMessageForParents.h"
#include <TinizineCommon/UI/Colour.h>
#include <TinizineCommon/UI/LayoutParams.h>
#include <TinizineCommon/Utils/LocaleManager.h>
#include "Style.h"

using namespace cocos2d;

USING_NS_TZ

NS_AZ_BEGIN

bool ImportantMessageForParents::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setSizeType(SizeType::PERCENT);
    setSizePercent(Vec2(1.0f,1.0f));
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    setSwallowTouches(true);
    
    setBackGroundColorType(BackGroundColorType::SOLID);
    setBackGroundColor(Color3B::WHITE);
    
    _contentLayout = ui::Layout::create();
    _contentLayout->setLayoutType(Type::VERTICAL);
    _contentLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _contentLayout->setNormalizedPosition(Vec2(0.5f, 0.85f));
    _contentLayout->setContentSize(Size(getContentSize().width * 0.7f, 0));
    addChild(_contentLayout);
    
    createText();
    
    ui::Button* closeButton = ui::Button::create("res/onboarding/close.png");
    closeButton->setAnchorPoint(Vec2(-0.2,1.2));
    closeButton->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
    closeButton->ignoreContentAdaptWithSize(false);
    closeButton->setContentSize(Size(100, 100));
    closeButton->setColor(Color3B::BLACK);
    closeButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            this->removeFromParent();
        }
    });
    addChild(closeButton);
    
    return true;
}

void ImportantMessageForParents::onSizeChanged()
{
    Super::onSizeChanged();
    _contentLayout->setContentSize(Size(getContentSize().width * 0.7f, 0));
    createText();
}

void ImportantMessageForParents::createText()
{
    _contentLayout->removeAllChildren();
    
    const float maxWidth = _contentLayout->getContentSize().width;
    
    ui::ImageView* logo = ui::ImageView::create("res/introAssets/Azoomee_WHITE.png");
    logo->setContentSize(Size(850,130));
    logo->setColor(Colours::Color_3B::darkIndigo);
    logo->setLayoutParameter(CreateLeftLinearLayoutParam(ui::Margin(0,0,0,165)));
    logo->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _contentLayout->addChild(logo);
    
    DynamicText* title1 = DynamicText::create(_("Azoomee purchases and your child's privacy"), Style::Font::PoppinsBold(), 84);
    title1->setLayoutParameter(CreateLeftLinearLayoutParam(ui::Margin(0,0,0,65)));
    title1->setMaxLineWidth(maxWidth);
    title1->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    title1->setTextColor(Color4B::BLACK);
    title1->setOverflow(Label::Overflow::RESIZE_HEIGHT);
    title1->setTextHorizontalAlignment(TextHAlignment::LEFT);
    title1->setTextVerticalAlignment(TextVAlignment::TOP);
    _contentLayout->addChild(title1);
    
    DynamicText* para1 = DynamicText::create(_("Making an Azoomee purchase allows children to use Azoomee's messaging service (Chat). Chat allows parents and their children to send text messages, emojis, pre-made stickers and other images to other Azoomee users."), Style::Font::PoppinsRegular(), 42);
    para1->setLayoutParameter(CreateLeftLinearLayoutParam(ui::Margin(0,0,0,100)));
    para1->setMaxLineWidth(maxWidth);
    para1->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    para1->setTextColor(Color4B(Colours::Color_3B::brownGrey));
    para1->setOverflow(Label::Overflow::RESIZE_HEIGHT);
    para1->setTextHorizontalAlignment(TextHAlignment::LEFT);
    para1->setTextVerticalAlignment(TextVAlignment::TOP);
    _contentLayout->addChild(para1);
    
    DynamicText* title2 = DynamicText::create(_("Privacy & Safety:"), Style::Font::PoppinsBold(), 84);
    title2->setLayoutParameter(CreateLeftLinearLayoutParam(ui::Margin(0,0,0,65)));
    title2->setMaxLineWidth(maxWidth);
    title2->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    title2->setTextColor(Color4B::BLACK);
    title2->setOverflow(Label::Overflow::RESIZE_HEIGHT);
    title2->setTextHorizontalAlignment(TextHAlignment::LEFT);
    title2->setTextVerticalAlignment(TextVAlignment::TOP);
    _contentLayout->addChild(title2);
    
    DynamicText* para2 = DynamicText::create(_("Parental consent is required before a child can access Chat. By making an Azoomee purchase, you are providing your consent as a parent or guardian for children to use Azoomee's messaging service (Chat)."), Style::Font::PoppinsRegular(), 42);
    para2->setLayoutParameter(CreateLeftLinearLayoutParam());
    para2->setMaxLineWidth(maxWidth);
    para2->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    para2->setTextColor(Color4B(Colours::Color_3B::brownGrey));
    para2->setOverflow(Label::Overflow::RESIZE_HEIGHT);
    para2->setTextHorizontalAlignment(TextHAlignment::LEFT);
    para2->setTextVerticalAlignment(TextVAlignment::TOP);
    _contentLayout->addChild(para2);
}

NS_AZ_END
