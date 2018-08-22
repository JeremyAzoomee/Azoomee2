//
//  SettingsHub.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 22/08/2018.
//

#include "SettingsHub.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include "SceneManagerScene.h"

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
    
    _titleLayout = ui::Layout::create();
    _titleLayout->setContentSize(Size(visibleSize.width, 150));
    _titleLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _titleLayout->setBackGroundColor(Style::Color::skyBlue);
    _contentLayout->addChild(_titleLayout);
    
    _titleBarButton = ui::Button::create("res/settings/exit_button.png");
    _titleBarButton->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
    _titleBarButton->setAnchorPoint(Vec2(-0.5,0.5));
    _titleLayout->addChild(_titleBarButton);
    _titleBarButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(ChildSelector));
        }
    });
    
    _titleText = ui::Text::create("Settings", Style::Font::Medium, 91);
    _titleText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _titleText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _titleText->setTextColor(Color4B::WHITE);
    _titleLayout->addChild(_titleText);
    
    _navigationLayout = ui::Layout::create();
    _navigationLayout->setContentSize(Size(visibleSize.width, visibleSize.height - _titleLayout->getContentSize().height));
    _navigationLayout->setLayoutType(ui::Layout::Type::VERTICAL);
    _contentLayout->addChild(_navigationLayout);
    
    _kidsButton = ui::Layout::create();
    _kidsButton->setContentSize(Size(visibleSize.width, (_navigationLayout->getContentSize().height * 0.2) - 10));
    _kidsButton->setLayoutParameter(CreateTopLinearLayoutParam(ui::Margin(0,0,0,10)));
    _kidsButton->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _kidsButton->setBackGroundColor(Color3B::WHITE);
    _navigationLayout->addChild(_kidsButton);
    
    ui::ImageView* icon = ui::ImageView::create("res/settings/your_kids_icon_2.png");
    icon->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
    icon->setAnchorPoint(Vec2(-0.5,0.5));
    _kidsButton->addChild(icon);
    
    ui::ImageView* arrow = ui::ImageView::create("res/settings/right_arrow_2.png");
    arrow->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
    arrow->setAnchorPoint(Vec2(1.5,0.5));
    _kidsButton->addChild(arrow);
    
    ui::Layout* textHolder = ui::Layout::create();
    textHolder->setContentSize(Size(_kidsButton->getContentSize().width - (icon->getContentSize().width * 2) - (arrow->getContentSize().width * 1.5),_kidsButton->getContentSize().height));
    textHolder->setPosition(Vec2((icon->getContentSize().width * 1.75f), _kidsButton->getContentSize().height * 0.5f));
    textHolder->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    textHolder->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    textHolder->setBackGroundColor(Color3B::YELLOW);
    _kidsButton->addChild(textHolder);
    
    _friendshipsButton = ui::Layout::create();
    _friendshipsButton->setContentSize(Size(visibleSize.width, (_navigationLayout->getContentSize().height * 0.2) - 10));
    _friendshipsButton->setLayoutParameter(CreateTopLinearLayoutParam(ui::Margin(0,0,0,10)));
    _friendshipsButton->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _friendshipsButton->setBackGroundColor(Color3B::WHITE);
    _navigationLayout->addChild(_friendshipsButton);
    
    _yourAccountButton = ui::Layout::create();
    _yourAccountButton->setContentSize(Size(visibleSize.width, (_navigationLayout->getContentSize().height * 0.2) - 10));
    _yourAccountButton->setLayoutParameter(CreateTopLinearLayoutParam(ui::Margin(0,0,0,10)));
    _yourAccountButton->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _yourAccountButton->setBackGroundColor(Color3B::WHITE);
    _navigationLayout->addChild(_yourAccountButton);
    
    _onlineSafetyButton = ui::Layout::create();
    _onlineSafetyButton->setContentSize(Size(visibleSize.width, (_navigationLayout->getContentSize().height * 0.2) - 10));
    _onlineSafetyButton->setLayoutParameter(CreateTopLinearLayoutParam(ui::Margin(0,0,0,10)));
    _onlineSafetyButton->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _onlineSafetyButton->setBackGroundColor(Color3B::WHITE);
    _navigationLayout->addChild(_onlineSafetyButton);
    
    _supportButton = ui::Layout::create();
    _supportButton->setContentSize(Size(visibleSize.width, (_navigationLayout->getContentSize().height * 0.2) - 10));
    _supportButton->setLayoutParameter(CreateTopLinearLayoutParam(ui::Margin(0,0,0,10)));
    _supportButton->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _supportButton->setBackGroundColor(Color3B::WHITE);
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

NS_AZOOMEE_END
